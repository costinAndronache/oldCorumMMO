#pragma once
#include "../InitGame.h"

class SharedNetwork {
public:
	template<typename T>
	using PacketIncomingEventHandler = std::function<void(T)>;

	SharedNetwork();
	static SharedNetwork* sharedInstance();

	void send(CTDS_ITEM_MOVE);
	void process(DSTC_ITEM_MOVE packet);
	void onNextItemMove(PacketIncomingEventHandler<DSTC_ITEM_MOVE> handler);

	void send(CTDS_CHAR_LEVELUP);
	void send(CTDS_SKILL_LEVELUP);
private:
	PacketIncomingEventHandler<DSTC_ITEM_MOVE> _incomingItemMoveHandler;
};

