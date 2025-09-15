#pragma once
#include "../GroupedItemInventoryView.h"
#include "../../NewUI/DragNDropSystem.h"
#include "../../NewHelpers/ItemUsageManager.h"
#include "TooltipManager.h"
#include "../../NewHelpers/SoundLibrary.h"

namespace NewInterface {
	class UserInventoryManager:
		public CustomUI::DragNDropReceiver,
		public CustomUI::DragNDropSender {
	public:
		UserInventoryManager(
			std::shared_ptr<GroupedItemInventoryView> userInventoryView, 
			std::shared_ptr<ItemUsageManager> itemUsageManager,
			std::shared_ptr<TooltipLayer> toolTipLayer,
			std::shared_ptr<TooltipHelper> toolTipHelper,
			SoundLibrary* soundLibrary
		);

		void rebuildInventoryViewWith(
			const std::vector<CItem>& smallItems,
			const std::vector<CItem>& largeItems
		);

		void onLeftMouseDragStart(
			std::function<void(std::shared_ptr<CustomUI:: Renderable>, CustomUI::Rect globalFrameStart)> handler) override {
			_handler = handler;
		}

		void resetIndexOnCurrentDragNDropItem();
		GroupedItemInventoryView::IndexResult itemIndexForGlobalPoint(CustomUI::Point);
		GroupedItemInventoryView::IndexResult indexOnCurrentDragNDropItem() { return _indexOnCurrentDragNDropItem; };
		CustomUI::Rect currentGlobalFrame() override { return _managedInventoryView->globalFrame(); }
	private:
		GroupedItemInventoryView::IndexResult _indexOnCurrentDragNDropItem;
		std::shared_ptr<GroupedItemInventoryView> _managedInventoryView;
		std::function<void(std::shared_ptr<CustomUI:: Renderable>, CustomUI::Rect globalFrameStart)> _handler;
		std::shared_ptr<ItemUsageManager> _itemUsageManager;
		
		std::shared_ptr<TooltipLayer> _toolTipLayer;
		std::shared_ptr<TooltipHelper> _toolTipHelper;
		std::shared_ptr<TooltipManager> _smallItemsTooltipManager;
		std::shared_ptr<TooltipManager> _largeItemsTooltipManager;
		SoundLibrary* _soundLibrary;
	};
}

