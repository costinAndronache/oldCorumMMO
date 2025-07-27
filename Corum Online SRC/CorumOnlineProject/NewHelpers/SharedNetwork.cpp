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

void SharedNetwork::send(CTDS_CHAR_LEVELUP statusPointLevelUp) {
	g_pNet->SendMsg((char*)&statusPointLevelUp, statusPointLevelUp.GetPacketSize(), SERVER_INDEX_ZONE);
}

void SharedNetwork::send(CTDS_SKILL_LEVELUP skillLevelUp) {
	g_pNet->SendMsg((char*)&skillLevelUp, skillLevelUp.GetPacketSize(), SERVER_INDEX_ZONE);
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