#pragma once
#include "../GenericItemsContainerView.h"
#include "../../NewUI/DragNDropSystem.h"

namespace NewInterface {
	class BeltDragNDropParticipant: 
		public CustomUI::DragNDropReceiver,
		public CustomUI::DragNDropSender {

	public:
		BeltDragNDropParticipant(GenericItemsContainerView* managedBeltView);

		void updateBeltWithItems(const std::vector<CItem>& items);

		void onLeftMouseDragStart(
			std::function<void(CustomUI:: Renderable*, CustomUI::Rect globalFrameStart)> handler) override {
			_handler = handler;
		}

		void resetIndexOnCurrentDragNDropItem();
		int indexOnCurrentDragNDropItem() { return _indexOnCurrentDragNDropItem; };
		CustomUI::Rect currentGlobalFrame() override { return _managedBeltView->globalFrame(); }
	private:
		int _indexOnCurrentDragNDropItem;
		GenericItemsContainerView* _managedBeltView;
		std::function<void(CustomUI::Renderable*, CustomUI::Rect globalFrameStart)> _handler;
	};
}


