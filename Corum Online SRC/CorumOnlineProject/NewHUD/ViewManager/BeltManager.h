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
			std::shared_ptr<GenericItemsContainerView> managedBeltView,
			std::shared_ptr<ItemUsageManager> itemUsageManager
		);

		void updateBeltWithItems(const std::vector<CItem>& items);
		void updateBeltWithItems(const CItem items[MAX_BELT_POOL]);
		void onLeftMouseDragStart(
			std::function<void(std::shared_ptr<CustomUI:: Renderable>, CustomUI::Rect globalFrameStart)> handler) override {
			_handler = handler;
		}

		void tryUseItemAtIndex(int index);

		void resetIndexOnCurrentDragNDropItem();
		int indexOnCurrentDragNDropItem() { return _indexOnCurrentDragNDropItem; };
		CustomUI::Rect currentGlobalFrame() override { return _managedBeltView->globalFrame(); }

		int itemIndexForGlobalPoint(CustomUI::Point p) { return _managedBeltView->itemIndexForGlobalPoint(p); }
	private:
		int _indexOnCurrentDragNDropItem;
		std::shared_ptr<ItemUsageManager> _itemUsageManager;
		std::shared_ptr<GenericItemsContainerView> _managedBeltView;
		std::function<void(std::shared_ptr<CustomUI:: Renderable>, CustomUI::Rect globalFrameStart)> _handler;

		std::vector<CItem> _currentItems;
	};
}


