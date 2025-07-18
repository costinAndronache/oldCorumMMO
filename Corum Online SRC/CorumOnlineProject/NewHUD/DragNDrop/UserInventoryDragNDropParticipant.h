#pragma once
#include "../GroupedItemInventoryView.h"
#include "../../NewUI/DragNDropSystem.h"

namespace NewInterface {
	class UserInventoryDragNDropParticipant:
		public CustomUI::DragNDropReceiver,
		public CustomUI::DragNDropSender {
	public:
		UserInventoryDragNDropParticipant(GroupedItemInventoryView* userInventoryView);

		void rebuildInventoryViewWith(
			const std::vector<CItem>& smallItems,
			const std::vector<CItem>& largeItems
		);

		void onLeftMouseDragStart(
			std::function<void(CustomUI::Renderable*, CustomUI::Rect globalFrameStart)> handler) override {
			_handler = handler;
		}

		void resetIndexOnCurrentDragNDropItem();
		GroupedItemInventoryView::IndexResult indexOnCurrentDragNDropItem() { return _indexOnCurrentDragNDropItem; };
		CustomUI::Rect currentGlobalFrame() override { return _managedInventoryView->globalFrame(); }
	private:
		GroupedItemInventoryView::IndexResult _indexOnCurrentDragNDropItem;
		GroupedItemInventoryView* _managedInventoryView;
		std::function<void(CustomUI::Renderable*, CustomUI::Rect globalFrameStart)> _handler;
	};
}

