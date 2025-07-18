#include "DragNDropManagerFromBelt.h"

using namespace NewInterface;
using namespace CustomUI;


void moveFromBeltToSmallItemsInventory(
	CMainUser* mainUser,
	int beltItemIndex,
	int smallItemInventoryIndex
) {

}

void DragNDropManagerFromBelt::setupRoutesFromBelt(
	CustomUI::DragNDropSystem* system,
	CMainUser* mainUser,
	BeltDragNDropParticipant* fromBelt,
	UserInventoryDragNDropParticipant* toUserItemsInventory
) {

	const auto toReceivers = std::vector<DragNDropReceiver*>{ toUserItemsInventory };
	system->registerRoute(
		fromBelt,
		toReceivers,
		[=](Rect frameOnEnd, std::set<unsigned int> indexesOfReceivers) {

		},
		[=]() {

		}
	);
}