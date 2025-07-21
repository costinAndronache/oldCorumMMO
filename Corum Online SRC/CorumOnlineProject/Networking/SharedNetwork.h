#pragma once
#include "../InitGame.h"

class SharedNetwork {
public:
	template<typename T>
	using PacketIncomingEventHandler = std::function<void(T)>;

	SharedNetwork();
	static SharedNetwork* sharedInstance();

	void send(CTDS_ITEM_MOVE);

private:
	
};

