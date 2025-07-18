#include "UserInventoryDragNDropParticipant.h"

using namespace NewInterface;
using namespace CustomUI;

UserInventoryDragNDropParticipant::UserInventoryDragNDropParticipant(GroupedItemInventoryView* userInventoryView) {
	_managedInventoryView = userInventoryView;
	_indexOnCurrentDragNDropItem = { -1, GroupedItemInventoryView::Tab::smallItems };
}

void UserInventoryDragNDropParticipant::resetIndexOnCurrentDragNDropItem() {
	_managedInventoryView->setHiddenStateForItemAtIndex(
		_indexOnCurrentDragNDropItem.index, _indexOnCurrentDragNDropItem.tab, false
	);
	_indexOnCurrentDragNDropItem = { -1, GroupedItemInventoryView::Tab::smallItems };
}

void UserInventoryDragNDropParticipant::rebuildInventoryViewWith(
	const std::vector<CItem>& smallItems,
	const std::vector<CItem>& largeItems
) {

	GroupedItemInventoryView::ItemLongPressHandlerLMB handler = [=](CItem item, CUISpriteModel sprite, int index, Rect globalFrame) {
		if (item.m_wItemID == 0) { return; }
		if (!_handler) { return; }

		_indexOnCurrentDragNDropItem = { index, _managedInventoryView->activeTab() };
		SpriteRenderable* sprr = new SpriteRenderable(globalFrame, sprite);

		_handler(sprr, globalFrame);
	};

	_managedInventoryView->rebuildWith(smallItems, largeItems, handler, handler);
}

