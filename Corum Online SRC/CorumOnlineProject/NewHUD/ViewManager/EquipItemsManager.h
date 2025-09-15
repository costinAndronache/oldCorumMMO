#pragma once
#include "../GenericItemsContainerView.h"
#include "../../NewUI/DragNDropSystem.h"
#include "TooltipManager.h"

namespace NewInterface {
	class EquipItemsManager :
		public CustomUI::DragNDropReceiver,
		public CustomUI::DragNDropSender {

	public:
		EquipItemsManager(
			std::shared_ptr<GenericItemsContainerView> managedView,
			std::shared_ptr<TooltipLayer> toolTipLayer,
			std::shared_ptr<TooltipHelper> toolTipHelper
		);

		void updateWithItems(const CItem equipVector[MAX_EQUIP_POOL]);
		void onLeftMouseDragStart(
			std::function<void(std::shared_ptr<CustomUI:: Renderable>, CustomUI::Rect globalFrameStart)> handler) override {
			_handler = handler;
		}

		void resetIndexOnCurrentDragNDropItem();
		int indexOnCurrentDragNDropItem() { return _indexOnCurrentDragNDropItem; };
		CustomUI::Rect currentGlobalFrame() override { return _managedView->globalFrame(); }

		int itemIndexForGlobalPoint(CustomUI::Point p) { return _managedView->itemIndexForGlobalPoint(p); }
	private:
		int _indexOnCurrentDragNDropItem;
		std::shared_ptr<GenericItemsContainerView> _managedView;
		std::function<void(std::shared_ptr<CustomUI:: Renderable>, CustomUI::Rect globalFrameStart)> _handler;

		std::shared_ptr<TooltipLayer> _toolTipLayer;
		std::shared_ptr<TooltipHelper> _toolTipHelper;
		std::shared_ptr<TooltipManager> _equipTooltipManager;
	};
}

