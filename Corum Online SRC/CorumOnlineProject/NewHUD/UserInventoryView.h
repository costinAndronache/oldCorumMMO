#pragma once
#include "GenericItemsContainerView.h"
#include "GroupedItemInventoryView.h"
#include "DisplacementHandleRenderable.h"

namespace NewInterface {
	class NewItemsWindow: public CustomUI::Renderable {
	public:
		using EventHandler = std::function<void()>;

		static CustomUI::Size preferredSize();
		NewItemsWindow(CustomUI::Point originInParent, CItemResourceHash* itemResourceHash);


		void onClose(EventHandler handler) { _onCloseHandler = handler; }

		// must replace later with interfaces with only the update methods
		std::shared_ptr<GenericItemsContainerView> equipItemsView() { return _equipItemsView; }
		std::shared_ptr<GroupedItemInventoryView> groupedInventoryView() { return _groupedInventoryView; }

		std::shared_ptr<DisplacementHandleRenderable> displacementHandle() { return _displacementHandle; }
	private:
		std::shared_ptr<DisplacementHandleRenderable> _displacementHandle;
		std::shared_ptr<GenericItemsContainerView> _equipItemsView;
		std::shared_ptr<GroupedItemInventoryView> _groupedInventoryView;
		std::shared_ptr<CustomUI::Button> _closeButton;
		std::shared_ptr<CustomUI::SingleLineLabel> _titleLabel;

		EventHandler _onCloseHandler;
	};
}

