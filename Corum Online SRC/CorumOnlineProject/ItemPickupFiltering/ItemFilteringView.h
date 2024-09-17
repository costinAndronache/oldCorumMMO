#pragma once
#include "PagedItemViewTable.h"
#include "InputField.h"

namespace CustomUI {
	class ItemFilteringView: public InputFieldClient {
	public:
		ItemFilteringView(Rect frame, std::vector<CItem*>& allItems);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order);
		void onInputFieldTextChange(InputField* inputField, const char* text) override;
		bool handleKeyUp(WORD keyCode);
	private:
		std::vector<CItem*> _allItems;
		Rect _frame;
		PagedItemViewTable* _table;
		InputField* _inputField;
	};

}


