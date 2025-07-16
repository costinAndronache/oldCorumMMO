#pragma once
#include "../GenericItemsContainerView.h"
#include "../../NewUI/DragNDropSystem.h"

namespace NewInterface {
	class BeltDragNDropParticipant: 
		public CustomUI::DragNDropReceiver,
		public CustomUI::DragNDropSender {

	public:
		BeltDragNDropParticipant(GenericItemsContainerView* managedBeltView, CMainUser* mainUser);

		void onLeftMouseDragStart(
			std::function<void(CustomUI:: Renderable*, CustomUI::Rect globalFrameStart)>) override;
		CustomUI::Rect currentGlobalFrame() override { return _managedBeltView->globalFrame(); }
	private:
		GenericItemsContainerView* _managedBeltView;
	};
}


