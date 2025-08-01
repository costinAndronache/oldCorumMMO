#include "BeltManager.h"

using namespace NewInterface;
using namespace CustomUI;

BeltManager::BeltManager(
	GenericItemsContainerView* managedBeltView,
	ItemUsageManager* itemUsageManager
) 
{
	_managedBeltView = managedBeltView;
	_itemUsageManager = itemUsageManager;
	_indexOnCurrentDragNDropItem = -1;
}

void BeltManager::updateBeltWithItems(const std::vector<CItem>& items) {
	_currentItems = items;

	GenericItemsContainerView::HandlerItemClickRIGHT rightClickHandler = [=](CItem item, int index) {
		auto result = _itemUsageManager->tryUseBeltItem(item, index);
	};
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
		
	},
		rightClickHandler
	);
}

void BeltManager::tryUseItemAtIndex(int index) {
	if (!(0 <= index && index < _currentItems.size())) { return; }
	auto item = _currentItems[index];

	_itemUsageManager->tryUseBeltItem(item, index);
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