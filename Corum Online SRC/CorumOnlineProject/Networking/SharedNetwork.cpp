#include "SharedNetwork.h"
#include "../NetworkClient.h"

SharedNetwork::SharedNetwork() { }

SharedNetwork* SharedNetwork::sharedInstance() {
	static SharedNetwork* _shared = nullptr;
	if (!_shared) { _shared = new SharedNetwork(); }

	return _shared;
}

void SharedNetwork::send(CTDS_ITEM_MOVE itemMove) {
	g_pNet->SendMsg((char*)&itemMove, itemMove.GetPacketSize(), SERVER_INDEX_ZONE);
}