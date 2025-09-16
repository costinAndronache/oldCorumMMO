#pragma once
#include "CommonServer.h"
#include "DungeonDataQueryResults.h"
#include <string>
#include <vector>
#include <map>
#include <set>
#include "../BaseLibrary/Either.h"

namespace DungeonStaticData {
	using MissingKeys = std::set<std::string>;

	static const wchar_t* staticDataJSONFile = L"DUNGEON_STATIC_DATA.json";
	static const wchar_t* staticServerDataJSONFile = L"SERVER_STATIC_DATA.json";

	struct DungeonServer {
		const std::string ip;
		const unsigned int port;
		const SERVER_TYPE serverType;
	};

	using DungeonServerDecodingResult = Either<MissingKeys, DungeonServer>;
	using DungeonServersFileResult = Either<
		std::string,
		std::vector<DungeonServerDecodingResult>
	>;

	DungeonServersFileResult COMMONSERVERDECLSPECIFIER readServersFromFile(const wchar_t* filePath);

	struct COMMONSERVERDECLSPECIFIER Dungeon {
		const int id;
		const unsigned int dungeonServerPort;
		const std::string name; 
		const double worldMapCoordX;
		const double worldMapCoordZ;
		const int worldMapID;
		const int groupID; 
		const std::vector<uint32_t> layerFormationIDs;

		Dungeon(
			int id, unsigned int port, 
			const std::string& name, 
			double worldX, double worldZ, 
			int worldMapID, int groupID,
			const std::vector<uint32_t>& layerFormationIDs
		);

		Dungeon(const Dungeon&);
	};

	using DungeonDecodingResult = Either<MissingKeys, Dungeon>;
	std::vector<DungeonDecodingResult> COMMONSERVERDECLSPECIFIER readFromFile(const wchar_t* filePath);

	class COMMONSERVERDECLSPECIFIER DungeonStaticDataManager {
	public:
		static DungeonStaticDataManager parseFromFile(const wchar_t* filePath);
	public:
		DungeonStaticDataManager(const std::vector<Dungeon> dungeons);

		bool tryFixup(CZP_QUERY_DUNGEON_INFO_WORLD_ResultRow&) const;

		std::vector<Dungeon> allDungeonsConnectingTo(int dungeonServerPort) const;
		std::vector<Dungeon> allDungeons() const;

		void debugPrint() const;
	private:
		std::map<int, Dungeon> _dungeonPerID;
	};
}


