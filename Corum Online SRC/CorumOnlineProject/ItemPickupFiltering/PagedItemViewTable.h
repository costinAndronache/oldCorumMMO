#pragma once
#include "ItemInfoView.h"
#include "Button.h"

#include <vector>
class CItem;

namespace CustomUI {
	class PagedItemViewTable: public ButtonClient {
	public:
		PagedItemViewTable(Rect frame, SpriteModel bgSpriteModel);
		void setDisplayedItems(std::vector<CItem*>& items);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order);
		
		void onButtonPress(Button*) override;
		void onButtonPressRelease(Button*) override;
	private:
		std::vector<CItem*> _currentDisplayedItems;
		std::vector<std::vector<ItemInfoView*>> _viewsTable;
		Rect _frame;
		SpriteModel _bgSpriteModel;
		int _numberOfRows;
		int _numberOfColumns;
		int _currentTopRowIndex;

		Button* _scrollUpBtn;
		Button* _scrollDownBtn;
		CItem* getCurrentItemForDisplayedCell(int row, int column);
		void updateDisplayedRowsWithCurrentItems();
		void scrollUp();
		void scrollDown();
	};

	struct PagedItemViewTableResources {
		static void initialize();
		static SpriteModel bgSpriteModel;
	};
}


