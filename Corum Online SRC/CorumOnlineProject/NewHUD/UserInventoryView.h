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
		GenericItemsContainerView* equipItemsView() { return _equipItemsView; }
		GroupedItemInventoryView* groupedInventoryView() { return _groupedInventoryView; }

		DisplacementHandleRenderable* displacementHandle() { return _displacementHandle; }
	private:
		DisplacementHandleRenderable* _displacementHandle;
		GenericItemsContainerView* _equipItemsView;
		GroupedItemInventoryView* _groupedInventoryView;
		CustomUI::Button* _closeButton;
		CustomUI::SingleLineLabel* _titleLabel;

		EventHandler _onCloseHandler;
	};
}

