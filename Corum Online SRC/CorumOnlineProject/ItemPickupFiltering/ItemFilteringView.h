#pragma once
#include "PagedItemViewTable.h"
#include "InputField.h"

namespace CustomUI {
	class ItemFilteringView: public InputFieldClient {
	public:
		ItemFilteringView(Rect frame, std::vector<CItem*>& allItems);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order);
		void onInputFieldTextChange(InputField* inputField, const char* text) override;
		bool handleKeyUp(WPARAM wparam, LPARAM lparam);
		bool handleKeyDown(WPARAM wparam, LPARAM lparam);
		bool handleMouseDown();
		bool handleMouseUp();
	private:
		std::vector<CItem*> _allItems;
		Rect _frame;
		PagedItemViewTable* _table;
		InputField* _inputField;

		void updateDisplayedItemsOnNameFilter(const char* nameFilter);
	};

}


