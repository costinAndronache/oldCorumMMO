#pragma once
#include "PagedItemViewTable.h"
#include "InputField.h"
#include "ItemInfoView.h"
#include "SelectionView.h"
#include "Label.h"
#include "ToggleButton.h"
#include "RadioButtonGroup.h"
#include "ItemCategoryFilterView.h"
#include <set>

namespace ItemPickupFiltering {
	class ItemFilteringView;
	class ItemFilteringViewClient {
	public:
		virtual void itemFilteringViewDidUpdateSelection(ItemFilteringView*, std::set<DWORD> selectedItemIDs) = 0;
	};

	class ItemFilteringView:
		private CustomUI::PagedItemViewTableClient,
		public CustomUI::Renderable {
	public:
		ItemFilteringView(CustomUI::Rect frameInParent, std::vector<CItem*>& allItems, std::set<DWORD>& selectedItemIDs, ItemFilteringViewClient* client);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order);

		std::set<DWORD> currentSelectedIDs();
	private:
		ItemFilteringViewClient* _client;
		std::vector<CItem*> _allItems;
		std::vector<CItem*> _textFilteringSourceItems;
		std::vector<CItem*> _displayedItems;
		std::set<DWORD> _selectedItemIDs;

		std::vector<CustomUI::SelectionView*> _createdInfoViews;
		CustomUI::PagedItemViewTable* _table;
		CustomUI::InputField* _inputField;
		ItemCategoryFilterView* _categoriesFilterView;
		CustomUI::SingleLineLabel* _titleLabel;
		CustomUI::Button* _closeButton;

		void updateTextFilteringSourceItemsTo(std::vector<CItem*> newSourceItems);

		void onInputFieldTextChange(CustomUI::InputField* inputField, const char* text);
		void itemCategoryFilterViewDidSwitchToCategory(ItemCategoryFilterView*, CategoryType category);
		CustomUI::Renderable* buildRenderableForModelAtIndexWithFrame(int modelIndex, CustomUI::Rect frame);
		void updateRenderableWithModelAtIndex(CustomUI::Renderable* renderable, int modelIndex) override;
		void selectionViewDidChangeSelectionState(CustomUI::SelectionView*, bool);

		void onButtonPress(CustomUI::Button* button);
		void onButtonPressRelease(CustomUI::Button* button);
	};
}


