#include "StatusPointManager.h"

StatusPointManager::StatusPointManager(SharedNetwork* network) {
	_network = network;
}

void StatusPointManager::increasePlayerStatusPoint(STATUS_POINT_KIND kind) {
	CTDS_CHAR_LEVELUP pPacket;
	pPacket.byObjectType = (BYTE)OBJECT_TYPE_PLAYER;;
	pPacket.byStatusType = (BYTE)kind;

	_network->send(pPacket);
}

void StatusPointManager::increaseGuardianStatusPoint(STATUS_POINT_KIND kind) {
	CTDS_CHAR_LEVELUP pPacket;
	pPacket.byObjectType = (BYTE)OBJECT_TYPE_MONSTER;;
	pPacket.byStatusType = (BYTE)kind;

	_network->send(pPacket);
}