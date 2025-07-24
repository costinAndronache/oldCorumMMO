#include "UserInventoryManager.h"

using namespace NewInterface;
using namespace CustomUI;

UserInventoryManager::UserInventoryManager(
	GroupedItemInventoryView* userInventoryView, ItemUsageManager* itemUsageManager
) {
	_managedInventoryView = userInventoryView;
	_itemUsageManager = itemUsageManager;
	_indexOnCurrentDragNDropItem = { -1, GroupedItemInventoryView::Tab::smallItems };
}

void UserInventoryManager::resetIndexOnCurrentDragNDropItem() {
	_managedInventoryView->setHiddenStateForItemAtIndex(
		_indexOnCurrentDragNDropItem.index, _indexOnCurrentDragNDropItem.tab, false
	);
	_indexOnCurrentDragNDropItem = { -1, GroupedItemInventoryView::Tab::smallItems };
}

void UserInventoryManager::rebuildInventoryViewWith(
	const std::vector<CItem>& smallItems,
	const std::vector<CItem>& largeItems
) {

	GroupedItemInventoryView::ItemLongPressHandlerLMB longClickHandlerLEFT = [=](CItem item, CUISpriteModel sprite, int index, Rect globalFrame) {
		if (item.m_wItemID == 0) { return; }
		if (!_handler) { return; }

		_indexOnCurrentDragNDropItem = { index, _managedInventoryView->activeTab() };
		SpriteRenderable* sprr = new SpriteRenderable(globalFrame, sprite);

		_managedInventoryView->setHiddenStateForItemAtIndex(index, _managedInventoryView->activeTab(), true);
		_handler(sprr, globalFrame);
	};

	GroupedItemInventoryView::HandlerItemClickRIGHT rightClickHandler = [=](CItem item, int index) {
		auto result = _itemUsageManager->tryUseSmallInventoryItem(item, index);
	};

	_managedInventoryView->rebuildWith(
		smallItems, 
		largeItems, 
		longClickHandlerLEFT,
		longClickHandlerLEFT,
		rightClickHandler
	);
}

GroupedItemInventoryView::IndexResult UserInventoryManager::itemIndexForGlobalPoint(CustomUI::Point point) {
	return _managedInventoryView->itemIndexForGlobalPoint(point);
}