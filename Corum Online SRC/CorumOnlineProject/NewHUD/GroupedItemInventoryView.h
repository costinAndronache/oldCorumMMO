#pragma once
#include "ItemInventoryView.h"
#include "../NewUI/RadioButtonGroup.h"


namespace NewInterface {
	class GroupedItemInventoryView: public CustomUI::Renderable {
	public:
		enum class Tab { smallItems, largeItems};

		static CustomUI::Size preferredSize();
		GroupedItemInventoryView(CustomUI::Rect frameInParent, CItemResourceHash* resourceHash);

		void rebuildWith(
			const std::vector<CItem>& smallItems,
			const std::vector<CItem>& largeItems
		);

		void setActiveTab(Tab tab);
		Tab activeTab() const { return _activeTab; }
	private:
		CustomUI::RadioButtonGroup* _radioButtonGroup;
		ItemInventoryView *_smallItemsInventory, *_largeItemsInventory;
		Tab _activeTab;
	};
}

