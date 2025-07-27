#include "BeltManager.h"

using namespace NewInterface;
using namespace CustomUI;

BeltManager::BeltManager(GenericItemsContainerView* managedBeltView) {
	_managedBeltView = managedBeltView;
	_indexOnCurrentDragNDropItem = -1;
}

void BeltManager::updateBeltWithItems(const std::vector<CItem>& items) {
	_managedBeltView->updateWithItems(
		items,
		NewHUDResources::inventoryItemUnderlaySprite,
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

void BeltManager::resetIndexOnCurrentDragNDropItem() {
	_managedBeltView->setHiddenStateForItemAtIndex(_indexOnCurrentDragNDropItem, false);
	_indexOnCurrentDragNDropItem = -1;
}

void BeltManager::updateBeltWithItems(const CItem items[MAX_BELT_POOL]) {
	std::vector<CItem> temp;
	for (int i = 0; i < MAX_BELT_POOL; i++) {
		temp.push_back(items[i]);
	}
	updateBeltWithItems(temp);
}