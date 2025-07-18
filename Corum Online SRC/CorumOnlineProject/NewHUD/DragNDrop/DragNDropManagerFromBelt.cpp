#include "DragNDropManagerFromBelt.h"

using namespace NewInterface;
using namespace CustomUI;


void moveFromBeltToSmallItemsInventory(
	CMainUser* mainUser,
	int beltItemIndex,
	int smallItemInventoryIndex
) {

}

void swapBeltItems(CMainUser* mainUser, int fromIndex, int toIndex) {
	printf("\nSwapping belt items from: %d --> to: %d", fromIndex, toIndex);
}

void DragNDropManagerFromBelt::setupRoutesFromBelt(
	CustomUI::DragNDropSystem* system,
	CMainUser* mainUser,
	BeltDragNDropParticipant* fromBelt,
	UserInventoryDragNDropParticipant* toUserItemsInventory
) {

	const auto toReceivers = std::vector<DragNDropReceiver*>{ fromBelt };
	system->registerRoute(
		fromBelt,
		toReceivers,
	[=](Rect frameOnEnd, std::set<unsigned int> indexesOfReceivers) {
		const auto destIndex = fromBelt->itemIndexForGlobalPoint(frameOnEnd.center());
		swapBeltItems(mainUser, fromBelt->indexOnCurrentDragNDropItem(), destIndex);
	},
	[=]() {
		fromBelt->resetIndexOnCurrentDragNDropItem();
	}
	);
}