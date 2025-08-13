#pragma once
#include "ItemInventoryView.h"
#include "../NewUI/RadioButtonGroup.h"


namespace NewInterface {
	class GroupedItemInventoryView: public CustomUI::Renderable {
	public:
		using Handlers = GenericItemsContainerView::Handlers;
		enum class Tab { smallItems, largeItems};
		using ActiveTabSwitchHandler = std::function<void(Tab)>;

		static CustomUI::Size preferredSize();
		GroupedItemInventoryView(CustomUI::Rect frameInParent, CItemResourceHash* resourceHash);

		void rebuildWith(
			const std::vector<CItem>& smallItems,
			const std::vector<CItem>& largeItems,
			Handlers smallItemHandlers,
			Handlers largeItemHandlers
		);

		void setActiveTab(Tab tab);
		Tab activeTab() const { return _activeTab; }
		void onActiveTabSwitch(ActiveTabSwitchHandler handler) { _tabSwitchHandler = handler; }
		struct IndexResult { int index; Tab tab; } itemIndexForGlobalPoint(CustomUI::Point p); // -1 on not found

		void setHiddenStateForItemAtIndex(int index, Tab tab, bool isHidden);
	private:
		CustomUI::RadioButtonGroup* _radioButtonGroup;
		ItemInventoryView *_smallItemsInventory, *_largeItemsInventory;
		Tab _activeTab;
		ActiveTabSwitchHandler _tabSwitchHandler;
		ItemInventoryView* inventoryViewFor(Tab);
	};
}

