#pragma once
#include "SharedNetwork.h"
#include "../../CommonServer/CommonClientDungeon.h"

class StatusPointManager {
public:
	StatusPointManager(std::shared_ptr<SharedNetwork>  network);

	void increasePlayerStatusPoint(STATUS_POINT_KIND kind);
	void increaseGuardianStatusPoint(STATUS_POINT_KIND kind);

private:
	std::shared_ptr<SharedNetwork>  _network;
};

