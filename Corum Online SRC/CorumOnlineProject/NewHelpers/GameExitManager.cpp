#include "GameExitManager.h"

GameExitManager::GameExitManager(std::shared_ptr<SharedNetwork> network,
                                 CMainUser* mainUser):
	_network(network), _mainUser(mainUser) {
}

void GameExitManager::exitGame() {
	g_bIsRunning = false;
}

void GameExitManager::exitOnCharSelect() {
	_network->sendDisconnectOnWorldServer();
	::PostMessage(g_hMainWnd, UM_GAME_RESTART, 0, 0);
}

GameExitManager::WorldMapExitFailReason GameExitManager::tryExitOnWorldMap() {
    if (_mainUser->IsMatching()) {
        return WorldMapExitFailReason::playerInPVPMatch;
    } else if (g_pThisDungeon->IsStadium()) {
      if (_mainUser->m_dwGuildWarFlag == G_W_F_OBSERVER) {
        GUILD_MATCH_CANCEL_OBSERVER Packet;
        Packet.wDungeonID = _mainUser->m_wDungeonID;
        _network->send(Packet);
        return WorldMapExitFailReason::noFail;
      } else {
          return WorldMapExitFailReason::playerInGuildWar;
      }
    } else if (_mainUser->GetStatus() == UNIT_STATUS_PLAYER_SHOP) {
        return WorldMapExitFailReason::playerInShopMode;
    } else if (_mainUser->GetStatus() == UNIT_STATUS_PORTAL_MOVING) {
        return WorldMapExitFailReason::playerDuringPortalMoving;
    } else {
        CTDS_WORLD_MOVE pPacket;
        pPacket.dwUserIndex = _mainUser->m_dwUserIndex;
        _network->send(pPacket);
    }
}