#include "BeltDragNDropParticipant.h"

using namespace NewInterface;
using namespace CustomUI;

BeltDragNDropParticipant::BeltDragNDropParticipant(GenericItemsContainerView* managedBeltView) {
	_managedBeltView = managedBeltView;
	_indexOnCurrentDragNDropItem = -1;
}

void BeltDragNDropParticipant::updateBeltWithItems(const std::vector<CItem>& items) {
	_managedBeltView->updateWithItems(
		items,
	[=](CItem item, SpriteModel sprite, int index, Rect globalFrame) {
		if (item.m_wItemID == 0) { return;  } // empty item 
		if (!_handler) { return; }

		_indexOnCurrentDragNDropItem = index;
		_managedBeltView->setHiddenStateForItemAtIndex(index, true);

		SpriteRenderable* sprr = new SpriteRenderable(globalFrame, sprite);
		_handler(sprr, globalFrame);
		
	}
	);
}

void BeltDragNDropParticipant::resetIndexOnCurrentDragNDropItem() {
	_managedBeltView->setHiddenStateForItemAtIndex(_indexOnCurrentDragNDropItem, false);
	_indexOnCurrentDragNDropItem = -1;
}