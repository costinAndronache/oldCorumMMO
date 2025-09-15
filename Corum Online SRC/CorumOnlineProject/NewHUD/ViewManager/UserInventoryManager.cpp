#include "UserInventoryManager.h"

using namespace NewInterface;
using namespace CustomUI;

UserInventoryManager::UserInventoryManager(
	std::shared_ptr<GroupedItemInventoryView> userInventoryView, 
	std::shared_ptr<ItemUsageManager> itemUsageManager,
	std::shared_ptr<TooltipLayer> toolTipLayer,
	std::shared_ptr<TooltipHelper> toolTipHelper,
	SoundLibrary* soundLibrary
) {
	_managedInventoryView = userInventoryView;
	_itemUsageManager = itemUsageManager;
	_indexOnCurrentDragNDropItem = { -1, GroupedItemInventoryView::Tab::smallItems };

	_toolTipHelper = toolTipHelper;
	_toolTipLayer = toolTipLayer;
	_soundLibrary = soundLibrary;

	_smallItemsTooltipManager = nullptr;
	_largeItemsTooltipManager = nullptr;

	_managedInventoryView->onActiveTabSwitch([=](auto){
		_soundLibrary->playButtonClick();
	});
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
	if (_smallItemsTooltipManager) { _smallItemsTooltipManager->clearAllTooltips(); }
	if (_largeItemsTooltipManager) { _largeItemsTooltipManager->clearAllTooltips(); }

	using LongClickLEFT = GenericItemsContainerView::HandlerItemLongClickLEFT;
	using ClickRIGHT = GenericItemsContainerView::HandlerItemClickRIGHT;

	LongClickLEFT longClickHandlerLEFT = [=](CItem item, CUISpriteModel sprite, int index, Rect globalFrame) {
		if (item.m_wItemID == 0) { return; }
		if (!_handler) { return; }

		_largeItemsTooltipManager->clearAllTooltips();
		_smallItemsTooltipManager->clearAllTooltips();

		_indexOnCurrentDragNDropItem = { index, _managedInventoryView->activeTab() };
		auto sprr = std::make_shared<SpriteRenderable>(globalFrame, sprite);

		_managedInventoryView->setHiddenStateForItemAtIndex(index, _managedInventoryView->activeTab(), true);
		_handler(sprr, globalFrame);
	};

	ClickRIGHT rightClickHandler = [=](CItem item, int index) {
		auto result = _itemUsageManager->tryUseSmallInventoryItem(item, index);
	};

	_smallItemsTooltipManager = std::make_shared<TooltipManager>(
		_toolTipLayer,
		[=](int smallItemIndex) -> TooltipManager::InfoLines {
			return _toolTipHelper->tooltipForItem(smallItems[smallItemIndex]);
		}
	);

	_largeItemsTooltipManager = std::make_shared<TooltipManager>(
		_toolTipLayer,
		[=](int largeItemIndex) -> TooltipManager::InfoLines {
		return _toolTipHelper->tooltipForItem(largeItems[largeItemIndex]);
	});

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

	GenericItemsContainerView::HandlerItemHovering largeItemHovering =
		[=](CItem item, int index, Point mouseCoordsGlobal) {
		if (item.m_wItemID == 0) { return; }
		_largeItemsTooltipManager->handleHoveringEvent(index, mouseCoordsGlobal);
	};

	GenericItemsContainerView::HandlerItemHoveringEnd largeItemHoveringEnd =
		[=](CItem item, int index) {
		if (item.m_wItemID == 0) { return; }
		_largeItemsTooltipManager->handleHoveringEndEvent(index);
	};

	_managedInventoryView->rebuildWith(
		smallItems, 
		largeItems, 
		{longClickHandlerLEFT, rightClickHandler, smallItemHovering, smallItemHoveringEnd },
		{longClickHandlerLEFT, nullptr, largeItemHovering, largeItemHoveringEnd}
	);
}

GroupedItemInventoryView::IndexResult UserInventoryManager::itemIndexForGlobalPoint(CustomUI::Point point) {
	return _managedInventoryView->itemIndexForGlobalPoint(point);
}