#pragma once

#include "BeltDragNDropParticipant.h"
#include "UserInventoryDragNDropParticipant.h"

namespace NewInterface {
	class DragNDropManagerFromBelt {
	public:
		static void setupRoutesFromBelt(
			CustomUI::DragNDropSystem* system,
			CMainUser* mainUser,
			BeltDragNDropParticipant* fromBelt,
			UserInventoryDragNDropParticipant* toUserItemsInventory
		);
	};
}

