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

	class ItemFilteringView : private CustomUI::InputFieldClient,
		private CustomUI::PagedItemViewTableClient,
		private CustomUI::SelectionViewClient,
		private CustomUI::ButtonClient,
		private ItemCategoryFilterViewClient {
	public:
		ItemFilteringView(CustomUI::Rect frame, std::vector<CItem*>& allItems, std::set<DWORD>& selectedItemIDs, ItemFilteringViewClient* client);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order);
		bool handleKeyUp(WPARAM wparam, LPARAM lparam);
		bool handleKeyDown(WPARAM wparam, LPARAM lparam);
		bool handleMouseDown();
		bool handleMouseUp();

		std::set<DWORD> currentSelectedIDs();
		void setHidden(bool isHidden);
		bool getHidden() { return _isHidden; }
	private:
		bool _isHidden;
		ItemFilteringViewClient* _client;
		std::vector<CItem*> _allItems;
		std::vector<CItem*> _textFilteringSourceItems;
		std::vector<CItem*> _displayedItems;
		std::set<DWORD> _selectedItemIDs;

		CustomUI::Rect _frame;
		std::vector<CustomUI::SelectionView*> _createdInfoViews;
		CustomUI::PagedItemViewTable* _table;
		CustomUI::InputField* _inputField;
		ItemCategoryFilterView* _categoriesFilterView;
		CustomUI::SingleLineLabel* _titleLabel;
		CustomUI::Button* _closeButton;

		void updateTextFilteringSourceItemsTo(std::vector<CItem*> newSourceItems);

		void onInputFieldTextChange(CustomUI::InputField* inputField, const char* text) override;
		void itemCategoryFilterViewDidSwitchToCategory(ItemCategoryFilterView*, CategoryType category) override;
		CustomUI::Renderable* buildRenderableForModelAtIndexWithFrame(int modelIndex, CustomUI::Rect frame) override;
		void updateRenderableWithModelAtIndex(CustomUI::Renderable* renderable, int modelIndex) override;
		void selectionViewDidChangeSelectionState(CustomUI::SelectionView*, bool) override;

		void onButtonPress(CustomUI::Button* button) override;
		void onButtonPressRelease(CustomUI::Button* button) override;
	};
}


