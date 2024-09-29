#pragma once
#include "Button.h"

#include <vector>
class CItem;

namespace CustomUI {
	class PagedItemViewTableClient {
	public:
		virtual Renderable* buildRenderableForModelAtIndexWithFrame(int modelIndex, Rect frame) = 0;
		virtual void updateRenderableWithModelAtIndex(Renderable* renderable, int modelIndex) = 0;

	};

	class PagedItemViewTable: public ButtonClient {
	public:
		PagedItemViewTable(Rect frame, PagedItemViewTableClient* _client, Size viewsSize, int initialModelCount, SpriteModel bgSpriteModel);
		void reloadData(int newItemsCount);
		void refresh();
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order);
		
		void onButtonPress(Button*) override;
		void onButtonPressRelease(Button*) override;
		static int fittedHeightWithin(int containerHeight, int viewHeight);
	private:
		PagedItemViewTableClient* _client;
		Size _viewsSize;
		Rect _frame;
		SpriteModel _bgSpriteModel;

		std::vector<std::vector<Renderable*>> _viewsTable;
		int _modelCount;

		int _numberOfRows;
		int _numberOfColumns;
		int _currentTopRowIndex;

		Button* _scrollUpBtn;
		Button* _scrollDownBtn;
		int getCurrentModelIndexForDisplayedCell(int row, int column, int totalItems);
		void scrollUp();
		void scrollDown();
	};

	struct PagedItemViewTableResources {
		static void initialize();
		static SpriteModel bgSpriteModel;
	};
}


