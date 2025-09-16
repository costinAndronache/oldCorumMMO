#include "stdafx.h"
#include "DungeonStaticData.h"
#include "../BaseLibrary/FileUtilities.h"
#include "../BaseLibrary/JSONUtilities.h"

using namespace DungeonStaticData;
using namespace nlohmann;

const auto ID = std::string("ID");
const auto Port = std::string("Port");
const auto WorldX = std::string("WorldPos_X");
const auto WorldZ = std::string("WorldPos_Z");
const auto Name = std::string("Name");
const auto WorldMapID = std::string("WorldMap");
const auto LayerFormation = std::string("LayerFormation");
const auto GroupID = std::string("GroupID");
const auto serverIP = std::string("ip");
const auto serverPort = std::string("port");
const auto serverType = std::string("serverType");


Dungeon::Dungeon(
	int id, unsigned int port, 
	const std::string& name, 
	double worldX, double worldZ, 
	int worldMapID, int groupID,
	const std::vector<uint32_t>& inputLayerFormationIDs
) : id(id), dungeonServerPort(port), name(name), 
worldMapCoordX(worldX), worldMapCoordZ(worldZ), 
worldMapID(worldMapID), groupID(groupID),
layerFormationIDs(inputLayerFormationIDs)
{
}

Dungeon::Dungeon(const Dungeon& other)
	: id(other.id), dungeonServerPort(other.dungeonServerPort), name(other.name),
	worldMapCoordX(other.worldMapCoordX),
	worldMapCoordZ(other.worldMapCoordZ),
	layerFormationIDs(other.layerFormationIDs),
	worldMapID(other.worldMapID), groupID(other.groupID)
{

}

static DungeonDecodingResult decodeDungeonFromJSON(const json& dungeonJSON) {
	auto missingKeys = JSONUtilities::allMissingValues(
		dungeonJSON,
		std::set<std::string>{ ID, Port, WorldX, WorldZ, Name, WorldMapID, LayerFormation, GroupID }
	);

	if(!missingKeys.empty()) { 
		return DungeonDecodingResult(std::make_shared<MissingKeys>(missingKeys)); 
	}

	auto dungeon = Dungeon {
		dungeonJSON[ID],
		dungeonJSON[Port],
		dungeonJSON[Name],
		dungeonJSON[WorldX],
		dungeonJSON[WorldZ],
		dungeonJSON[WorldMapID],
		dungeonJSON[GroupID],
		dungeonJSON[LayerFormation]
	};

	return DungeonDecodingResult {
		std::make_shared<Dungeon>(dungeon)
	};
}

static DungeonServerDecodingResult decodeServer(const json& serverJSON) {
	auto missingKeys = JSONUtilities::allMissingValues(
		serverJSON,
		std::set<std::string>{ serverIP, serverPort, serverType }
	);

	if(!missingKeys.empty()) { 
		return { std::make_shared<MissingKeys>(missingKeys) };
	}

	return std::make_shared<DungeonServer>(
		DungeonServer {
			serverJSON[serverIP],
			serverJSON[serverPort],
			serverJSON[serverType]
		}
	);
}

DungeonServersFileResult DungeonStaticData::readServersFromFile(const wchar_t* filePath) {
	auto jsonString = FileUtilities::readStringFromFile(filePath);
	auto root = json::parse(jsonString, nullptr, false, true);
	if(root.is_discarded() || !root.is_array()) { 
		return { std::make_shared<std::string>("Not a valid json array") }; 
	}

	std::vector<DungeonServerDecodingResult> results;
	std::transform(
		std::begin(root),
		std::end(root),
		std::back_inserter(results),
		[=](const json& serverJSON) -> DungeonServerDecodingResult {
			return decodeServer(serverJSON);
		}
	);

	return { std::make_shared<decltype(results)>(results) };
}


std::vector<DungeonDecodingResult> DungeonStaticData::readFromFile(const wchar_t* filePath) {
	auto jsonString = FileUtilities::readStringFromFile(filePath);
	auto root = json::parse(jsonString, nullptr, false, true);
	if(root.is_discarded()) { return {}; }

	auto& array = root["DUNGEON_STATIC_DATA"]; 
	if(array.is_discarded() || !array.is_array()) { return {}; }

	std::vector<DungeonDecodingResult> result;
	std::transform(
		std::begin(array),
		std::end(array),
		std::back_inserter(result),
		[](json& dungeonJSON) -> DungeonDecodingResult { 
			return decodeDungeonFromJSON(dungeonJSON);
		}
	);

	return result;
}

DungeonStaticDataManager::DungeonStaticDataManager(const std::vector<Dungeon> dungeons) {
	for(const auto& d: dungeons) {
		_dungeonPerID.insert({d.id, d});
	}
}

DungeonStaticDataManager DungeonStaticDataManager::parseFromFile(const wchar_t* filePath) {
	const auto decodingResults = readFromFile(filePath);
	std::vector<Dungeon> dungeons;

	for(const auto& result: decodingResults) {
		result.sswitch<void>(
			[](std::shared_ptr<MissingKeys> missingKeysPtr) -> void {
			printf("\nCould not decode a dungeon! Missing:\n");
			for(const auto& key: *missingKeysPtr) {
				printf("%s; ", key.c_str());
			}
			return;
		},
			[&](std::shared_ptr<Dungeon> dungeonPtr) -> void {
			dungeons.push_back(*dungeonPtr);
			return;
		});
	}

	return DungeonStaticDataManager(dungeons);
}

void DungeonStaticDataManager::debugPrint() const {
	printf("\nDUNGEON STATIC DATA::\n");

	for(const auto& pair: _dungeonPerID) {
		const auto& dungeon = pair.second;
		printf("{\n");
		printf("\t%s: port:%d\n", dungeon.name.c_str(), dungeon.dungeonServerPort);
		printf("}\n");
	}
}

std::vector<Dungeon> DungeonStaticDataManager::allDungeons() const {
	std::vector<Dungeon> result;
	for(const auto& entry: _dungeonPerID) {
		result.push_back(entry.second);
	}

	return result;
}

std::vector<Dungeon> DungeonStaticDataManager::allDungeonsConnectingTo(int dungeonServerPort) const {
	std::vector<Dungeon> result; 
	for(const auto& pair: _dungeonPerID) {
		if(pair.second.dungeonServerPort == dungeonServerPort) {
			result.push_back(pair.second);
		}
	}

	return result; 
}

bool DungeonStaticDataManager::tryFixup(CZP_QUERY_DUNGEON_INFO_WORLD_ResultRow& item) const {
	auto it = _dungeonPerID.find(item.m_dwID);
	if(it == _dungeonPerID.end()) { return false; }
	const auto& data = it->second;

	strncpy(item.m_szDungeonName, data.name.c_str(), data.name.size());
	item.m_dWorldPos_X = data.worldMapCoordX;
	item.m_dWorldPos_Z = data.worldMapCoordZ;
	item.m_dwPort = data.dungeonServerPort;
	item.m_wWorldMapID = data.worldMapID;

	return true; 
}