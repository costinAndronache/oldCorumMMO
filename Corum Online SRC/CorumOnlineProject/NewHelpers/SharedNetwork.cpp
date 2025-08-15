#include "SharedNetwork.h"


SharedNetwork::SharedNetwork() { }

std::shared_ptr<SharedNetwork> SharedNetwork::sharedInstance() {
  static std::shared_ptr<SharedNetwork> _shared = nullptr;
	if (!_shared) { _shared = std::make_shared<SharedNetwork>(); }

	return _shared;
}

void SharedNetwork::process(DSTC_ITEM_MOVE packet) {
	if (_incomingItemMoveHandler) {
		_incomingItemMoveHandler(packet);
		_incomingItemMoveHandler = nullptr;
	}
}
void SharedNetwork::onNextItemMove(PacketIncomingEventHandler<DSTC_ITEM_MOVE> handler) {
	_incomingItemMoveHandler = handler;
}

void SharedNetwork::sendDisconnectOnWorldServer() {
	g_pNet->DisconnectServer(SERVER_INDEX_WORLD);
}