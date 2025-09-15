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

		std::vector<std::shared_ptr<CustomUI::SelectionView>> _createdInfoViews;
		std::shared_ptr<CustomUI::PagedItemViewTable> _table;
		std::shared_ptr<CustomUI::InputField> _inputField;
		std::shared_ptr<ItemCategoryFilterView> _categoriesFilterView;
		std::shared_ptr<CustomUI::SingleLineLabel> _titleLabel;
		std::shared_ptr<CustomUI::Button> _closeButton;

		void updateTextFilteringSourceItemsTo(std::vector<CItem*> newSourceItems);

		void onInputFieldTextChange(CustomUI::InputField* inputField, const char* text);
		void itemCategoryFilterViewDidSwitchToCategory(ItemCategoryFilterView*, CategoryType category);
		std::shared_ptr<CustomUI::Renderable> buildRenderableForModelAtIndexWithFrame(int modelIndex, CustomUI::Rect frame);
		void updateRenderableWithModelAtIndex(std::shared_ptr<CustomUI::Renderable> renderable, int modelIndex) override;
		void selectionViewDidChangeSelectionState(CustomUI::SelectionView*, bool);

		void onButtonPress(CustomUI::Button* button);
		void onButtonPressRelease(CustomUI::Button* button);
	};
}


