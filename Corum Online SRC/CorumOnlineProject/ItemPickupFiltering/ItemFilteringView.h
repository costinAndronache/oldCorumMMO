#pragma once
#include "PagedItemViewTable.h"
#include "InputField.h"
#include "ItemInfoView.h"
#include "SelectionView.h"
#include "Label.h"
#include <set>

namespace CustomUI {
	class ItemFilteringView: public InputFieldClient, public PagedItemViewTableClient, public SelectionViewClient {
	public:
		ItemFilteringView(Rect frame, std::vector<CItem*>& allItems, std::set<DWORD>& selectedItemIDs);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order);
		void onInputFieldTextChange(InputField* inputField, const char* text) override;
		bool handleKeyUp(WPARAM wparam, LPARAM lparam);
		bool handleKeyDown(WPARAM wparam, LPARAM lparam);
		bool handleMouseDown();
		bool handleMouseUp();

		std::set<DWORD> currentSelectedIDs();

		Renderable* buildRenderableForModelAtIndexWithFrame(int modelIndex, Rect frame) override;
		void updateRenderableWithModelAtIndex(Renderable* renderable, int modelIndex) override;

		void selectionViewDidChangeSelectionState(SelectionView* view, bool isSelected) override;

	private:
		std::vector<CItem*> _allItems;
		Rect _frame;
		std::set<DWORD> _selectedItemIDs;

		std::vector<CItem*> _displayedItems;
		std::vector<SelectionView*> _createdInfoViews;

		PagedItemViewTable* _table;
		InputField* _inputField;
		Button* _labeledButton;

		void updateDisplayedItemsOnNameFilter(const char* nameFilter);
	};

}


