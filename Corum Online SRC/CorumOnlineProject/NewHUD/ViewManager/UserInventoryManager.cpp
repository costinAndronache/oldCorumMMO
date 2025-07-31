#include "UserInventoryManager.h"

using namespace NewInterface;
using namespace CustomUI;

UserInventoryManager::UserInventoryManager(
	GroupedItemInventoryView* userInventoryView, 
	ItemUsageManager* itemUsageManager,
	TooltipLayer* toolTipLayer,
	TooltipHelper* toolTipHelper
) {
	_managedInventoryView = userInventoryView;
	_itemUsageManager = itemUsageManager;
	_indexOnCurrentDragNDropItem = { -1, GroupedItemInventoryView::Tab::smallItems };

	_toolTipHelper = toolTipHelper;
	_toolTipLayer = toolTipLayer;
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
	using LongClickLEFT = GenericItemsContainerView::HandlerItemLongClickLEFT;
	using ClickRIGHT = GenericItemsContainerView::HandlerItemClickRIGHT;

	LongClickLEFT longClickHandlerLEFT = [=](CItem item, CUISpriteModel sprite, int index, Rect globalFrame) {
		if (item.m_wItemID == 0) { return; }
		if (!_handler) { return; }

		_indexOnCurrentDragNDropItem = { index, _managedInventoryView->activeTab() };
		SpriteRenderable* sprr = new SpriteRenderable(globalFrame, sprite);

		_managedInventoryView->setHiddenStateForItemAtIndex(index, _managedInventoryView->activeTab(), true);
		_handler(sprr, globalFrame);
	};

	ClickRIGHT rightClickHandler = [=](CItem item, int index) {
		auto result = _itemUsageManager->tryUseSmallInventoryItem(item, index);
	};

	_smallItemsTooltipManager = new TooltipManager(
		_toolTipLayer,
		[=](int smallItemIndex) -> TooltipManager::InfoLines {
			return _toolTipHelper->tooltipForItem(smallItems[smallItemIndex]);
		}
	);

	GenericItemsContainerView::HandlerItemHovering smallItemHovering =
		[=](CItem item, int index, Point mouseCoordsGlobal) {
		if (item.m_wItemID == 0) { return; }
		_smallItemsTooltipManager->handleHoveringEvent(index, mouseCoordsGlobal);
	};

	GenericItemsContainerView::HandlerItemHoveringEnd smallItemHoveringEnd =
		[=](CItem item, int index) {
		if (item.m_wItemID == 0) { return; }
		_smallItemsTooltipManager->handleHoveringEndEvent(index);
	};

	_managedInventoryView->rebuildWith(
		smallItems, 
		largeItems, 
		{longClickHandlerLEFT, rightClickHandler, smallItemHovering, smallItemHoveringEnd },
		{longClickHandlerLEFT, nullptr, nullptr, nullptr}
	);


}

GroupedItemInventoryView::IndexResult UserInventoryManager::itemIndexForGlobalPoint(CustomUI::Point point) {
	return _managedInventoryView->itemIndexForGlobalPoint(point);
}