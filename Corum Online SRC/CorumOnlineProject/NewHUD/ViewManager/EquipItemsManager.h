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
			GenericItemsContainerView* managedView,
			TooltipLayer* toolTipLayer,
			TooltipHelper* toolTipHelper
		);

		void updateWithItems(const CItem equipVector[MAX_EQUIP_POOL]);
		void onLeftMouseDragStart(
			std::function<void(CustomUI::Renderable*, CustomUI::Rect globalFrameStart)> handler) override {
			_handler = handler;
		}

		void resetIndexOnCurrentDragNDropItem();
		int indexOnCurrentDragNDropItem() { return _indexOnCurrentDragNDropItem; };
		CustomUI::Rect currentGlobalFrame() override { return _managedView->globalFrame(); }

		int itemIndexForGlobalPoint(CustomUI::Point p) { return _managedView->itemIndexForGlobalPoint(p); }
	private:
		int _indexOnCurrentDragNDropItem;
		GenericItemsContainerView* _managedView;
		std::function<void(CustomUI::Renderable*, CustomUI::Rect globalFrameStart)> _handler;

		TooltipLayer* _toolTipLayer;
		TooltipHelper* _toolTipHelper;
		TooltipManager* _equipTooltipManager;
	};
}

