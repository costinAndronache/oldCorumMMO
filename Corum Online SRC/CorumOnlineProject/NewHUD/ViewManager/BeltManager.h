#pragma once
#include "../GenericItemsContainerView.h"
#include "../../NewUI/DragNDropSystem.h"
#include "../../NewHelpers/ItemUsageManager.h"
namespace NewInterface {
	class BeltManager: 
		public CustomUI::DragNDropReceiver,
		public CustomUI::DragNDropSender {

	public:
		BeltManager(
			GenericItemsContainerView* managedBeltView,
			ItemUsageManager* itemUsageManager
		);

		void updateBeltWithItems(const std::vector<CItem>& items);
		void updateBeltWithItems(const CItem items[MAX_BELT_POOL]);
		void onLeftMouseDragStart(
			std::function<void(CustomUI:: Renderable*, CustomUI::Rect globalFrameStart)> handler) override {
			_handler = handler;
		}

		void tryUseItemAtIndex(int index);

		void resetIndexOnCurrentDragNDropItem();
		int indexOnCurrentDragNDropItem() { return _indexOnCurrentDragNDropItem; };
		CustomUI::Rect currentGlobalFrame() override { return _managedBeltView->globalFrame(); }

		int itemIndexForGlobalPoint(CustomUI::Point p) { return _managedBeltView->itemIndexForGlobalPoint(p); }
	private:
		int _indexOnCurrentDragNDropItem;
		ItemUsageManager* _itemUsageManager;
		GenericItemsContainerView* _managedBeltView;
		std::function<void(CustomUI::Renderable*, CustomUI::Rect globalFrameStart)> _handler;

		std::vector<CItem> _currentItems;
	};
}


