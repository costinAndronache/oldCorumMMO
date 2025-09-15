#pragma once
#include "../InitGame.h"
#include "../NetworkClient.h"

class SharedNetwork {
public:
	template<typename T>
	using PacketIncomingEventHandler = std::function<void(T)>;

	SharedNetwork();
	static std::shared_ptr<SharedNetwork> sharedInstance();
	void process(DSTC_ITEM_MOVE packet);
	void onNextItemMove(PacketIncomingEventHandler<DSTC_ITEM_MOVE> handler);
	void sendDisconnectOnWorldServer();

	template<typename GamePacketType>
	void send(GamePacketType packet) {
          g_pNet->SendMsg(
			  (char *)&packet, 
			  packet.GetPacketSize(),
              SERVER_INDEX_ZONE
		  );
	}

private:
	PacketIncomingEventHandler<DSTC_ITEM_MOVE> _incomingItemMoveHandler;
};

