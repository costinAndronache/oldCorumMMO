#pragma once
#include "SharedNetwork.h"

class GameExitManager {
public:
	enum class WorldMapExitFailReason { 
		noFail, 
		playerInPVPMatch, 
		playerInGuildWar,
		playerInShopMode,
		playerDuringPortalMoving
	};
public:
	GameExitManager(
		std::shared_ptr<SharedNetwork> network,
		CMainUser* _mainUser
	);

	void exitGame();
	void exitOnCharSelect();
    WorldMapExitFailReason tryExitOnWorldMap();

private:
	CMainUser* _mainUser;
	std::shared_ptr<SharedNetwork> _network;
};
