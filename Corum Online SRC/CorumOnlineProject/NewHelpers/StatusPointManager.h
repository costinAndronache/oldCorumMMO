#pragma once
#include "SharedNetwork.h"
#include "../../CommonServer/CommonClientDungeon.h"

class StatusPointManager {
public:
	StatusPointManager(SharedNetwork* network);

	void increasePlayerStatusPoint(STATUS_POINT_KIND kind);
	void increaseGuardianStatusPoint(STATUS_POINT_KIND kind);

private:
	SharedNetwork* _network;
};

