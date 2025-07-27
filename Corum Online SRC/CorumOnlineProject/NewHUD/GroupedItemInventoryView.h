#pragma once
#include "ItemInventoryView.h"
#include "../NewUI/RadioButtonGroup.h"


namespace NewInterface {
	class GroupedItemInventoryView: public CustomUI::Renderable {
	public:
		using ItemLongPressHandlerLMB = ItemInventoryView::ItemLongPressHandlerLMB;
		using HandlerItemClickRIGHT = ItemInventoryView::HandlerItemClickRIGHT;
		enum class Tab { smallItems, largeItems};

		static CustomUI::Size preferredSize();
		GroupedItemInventoryView(CustomUI::Rect frameInParent, CItemResourceHash* resourceHash);

		void rebuildWith(
			const std::vector<CItem>& smallItems,
			const std::vector<CItem>& largeItems,
			ItemLongPressHandlerLMB onSmallItemLongPressLMB,
			ItemLongPressHandlerLMB onLargeItemLongPressLMB,
			HandlerItemClickRIGHT onRightClickSmallItem
		);

		void setActiveTab(Tab tab);
		Tab activeTab() const { return _activeTab; }

		struct IndexResult { int index; Tab tab; } itemIndexForGlobalPoint(CustomUI::Point p); // -1 on not found

		void setHiddenStateForItemAtIndex(int index, Tab tab, bool isHidden);
	private:
		CustomUI::RadioButtonGroup* _radioButtonGroup;
		ItemInventoryView *_smallItemsInventory, *_largeItemsInventory;
		Tab _activeTab;

		ItemInventoryView* inventoryViewFor(Tab);
	};
}

