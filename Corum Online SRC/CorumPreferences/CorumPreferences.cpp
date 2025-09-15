// CorumPreferences.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "CorumPreferences.h"
#include "../BaseLibrary/libraries/json.hpp"
#include "../BaseLibrary/FileUtilities.h"
#include <fstream>
#include <iostream>
#include "libbase64/base64.hpp"

using namespace CorumPreferences;
using namespace nlohmann;

// not sure why the json functions dont accept named const char* arguments
static auto windowMode = json::json_pointer("/windowMode");
static auto resWidth = "resWidth";
static auto resHeight = "resHeight";

Preferences::Preferences() {
	_resolution = { 1024, 768 };
	_windowMode = true;
}

Preferences Preferences::defaults() { return Preferences(); }

SkillHotkey Preferences::hotkeyForSkill(int skillID, SkillClass skillClass) const {
	const auto& classMap = _skillHotkeyMapPerClass.find(skillClass);
	if(classMap == _skillHotkeyMapPerClass.end()) { return SkillHotkey_unknown; }

	for(const auto& entry: classMap->second) {
		if(entry.second == skillID) {
			return entry.first;
		}
	}

	return SkillHotkey_unknown;
}

int Preferences::skillIDForHotkey(SkillHotkey hk, SkillClass skillClass) const {
	const auto& classMap = _skillHotkeyMapPerClass.find(skillClass);
	if(classMap == _skillHotkeyMapPerClass.end()) { return -1; }

	auto entry = classMap->second.find(hk);
	if(entry == classMap->second.end()) { return -1; }

	return entry->second;

}

std::vector<std::pair<SkillHotkey, int>> Preferences::currentSkillHotkeyEntries(SkillClass skillClass) const {
	std::vector<std::pair<SkillHotkey, int>> result;
	const auto& classMap = _skillHotkeyMapPerClass.find(skillClass);
	if(classMap == _skillHotkeyMapPerClass.end()) { return {}; }

	for(auto entry: classMap->second) {
		result.push_back(entry);
	}

	return result;
}

void Preferences::storeHotkeyForSkill(SkillHotkey hk, int skillID, SkillClass skillClass) {
	auto classMap = _skillHotkeyMapPerClass.find(skillClass);
	if(classMap == _skillHotkeyMapPerClass.end()) { 
		_skillHotkeyMapPerClass.insert({skillClass, {}});
		storeHotkeyForSkill(hk, skillID, skillClass);
	} else {
		classMap->second[hk] = skillID;
	}
}

Preferences* Preferences::buildFromData(const Data& data) {
	auto jsonString = std::string(std::begin(data), std::end(data));

	json fromJSON = json::parse(jsonString, nullptr, false);

	if(fromJSON.is_discarded()) { return new Preferences();}

	auto* result = new Preferences();
	auto def = defaults();

	result->storeWindowMode(fromJSON.value<bool>("windowMode", def.windowMode()));
	result->storeResolution({
		fromJSON.value<int>("resWidth", def.resolution().width),
		fromJSON.value<int>("resHeight", def.resolution().height)
	});

	auto skillHotkeyMapJSON = fromJSON.value<SkillHotkeyMapPerClass>("skillHotkeyMap", SkillHotkeyMapPerClass{});

	result->_skillHotkeyMapPerClass = skillHotkeyMapJSON;

	return result;
}

Preferences* Preferences::buildFromFile(const wchar_t* filePath) {
	return buildFromData(FileUtilities::readBytes(filePath));
}

Data Preferences::serialize() const {
	json serialized {
		{ "windowMode", _windowMode }, 
		{ "resWidth", _resolution.width }, 
		{ "resHeight", _resolution.height},
		{ "skillHotkeyMap", _skillHotkeyMapPerClass}
	};

	auto toString = to_string(serialized);
	return Data(std::begin(toString), std::end(toString));
}

void Preferences::serializeToFile(const wchar_t* filePath) const {
	std::ofstream o(filePath, std::ios::binary);

	auto data = serialize();
	o.write((char*)data.data(), data.size());
}

static json* getJSONObject(const wchar_t* filePath) {
	static json* _json = nullptr;
	if(!_json) {
		json fromJSON = json::parse(FileUtilities::readStringFromFile(filePath), nullptr, false);
		if(!fromJSON.is_discarded()) {
			_json = new json(std::move(fromJSON));
		}
	}

	return _json;
}

static const wchar_t* staticDataJSON = L"DUNGEON_STATIC_DATA.json";
static void iterateAndModify(std::function<void(json& dungeonEntry)> fn) {
	auto _json = getJSONObject(staticDataJSON);
	if(_json) {
		auto& dungeonStaticDataArray = (*_json)["DUNGEON_STATIC_DATA"];
		for(auto& dungeonEntry: dungeonStaticDataArray) {
			fn(dungeonEntry);
		} 
	}
}
void fixDungeonStaticDataEntry(int id, const char* name) {
	iterateAndModify([=](auto& dungeonEntry){
		const int dungeonID = dungeonEntry["ID"];
		if(dungeonID == id) {
			dungeonEntry["Name"] = std::string(name);
		}
	});
}

void fixLayerFormationForAllEntries() {
	iterateAndModify([=](json& dungeonEntry) {
		json& layerFormationEntry = dungeonEntry["LayerFormation"];
		if(layerFormationEntry.is_string()) {
			int layerCount = dungeonEntry["LayerCount"];

			std::string base64Entry = layerFormationEntry;
			auto decoded = Base64::decode(base64Entry);
			const auto byteCount = decoded.size();
			uint32_t* layerIDs = (uint32_t*)malloc(byteCount);
			memcpy(layerIDs, decoded.c_str(), byteCount);

			std::vector<uint32_t> layerFormationIDs(
				layerIDs, layerIDs + (layerCount * sizeof(uint32_t))
			);

			layerFormationIDs.resize(layerCount);

			dungeonEntry["LayerFormation"] = layerFormationIDs;
			std::string name = dungeonEntry["Name"];
		}
	});
}

void serializeCurrentDungeonStaticDataFixes() {
	auto _json = getJSONObject(staticDataJSON);

	if(_json) { 
		auto text = to_string(*_json);
		std::ofstream o(staticDataJSON, std::ios::binary);
		o << text;
	}
}