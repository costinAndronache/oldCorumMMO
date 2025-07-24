#pragma once
#include "../GroupedItemInventoryView.h"
#include "../../NewUI/DragNDropSystem.h"
#include "../../NewHelpers/ItemUsageManager.h"

namespace NewInterface {
	class UserInventoryManager:
		public CustomUI::DragNDropReceiver,
		public CustomUI::DragNDropSender {
	public:
		UserInventoryManager(GroupedItemInventoryView* userInventoryView, ItemUsageManager* itemUsageManager);

		void rebuildInventoryViewWith(
			const std::vector<CItem>& smallItems,
			const std::vector<CItem>& largeItems
		);

		void onLeftMouseDragStart(
			std::function<void(CustomUI::Renderable*, CustomUI::Rect globalFrameStart)> handler) override {
			_handler = handler;
		}

		void resetIndexOnCurrentDragNDropItem();
		GroupedItemInventoryView::IndexResult itemIndexForGlobalPoint(CustomUI::Point);
		GroupedItemInventoryView::IndexResult indexOnCurrentDragNDropItem() { return _indexOnCurrentDragNDropItem; };
		CustomUI::Rect currentGlobalFrame() override { return _managedInventoryView->globalFrame(); }
	private:
		GroupedItemInventoryView::IndexResult _indexOnCurrentDragNDropItem;
		GroupedItemInventoryView* _managedInventoryView;
		std::function<void(CustomUI::Renderable*, CustomUI::Rect globalFrameStart)> _handler;
		ItemUsageManager* _itemUsageManager;
	};
}

