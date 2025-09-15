#pragma once
#include "Button.h"

#include <vector>
class CItem;

namespace CustomUI {
	class PagedItemViewTableClient {
	public:
		virtual std::shared_ptr<Renderable> buildRenderableForModelAtIndexWithFrame(int modelIndex, Rect frame) = 0;
		virtual void updateRenderableWithModelAtIndex(std::shared_ptr<Renderable> renderable, int modelIndex) = 0;
	};

	class PagedItemViewTable: public Renderable {
	public:
		// must implement two phase initialization factories for all shareable classes
		// enable_shared_from_this's _weakPtr is not yet initialized within constructors
		PagedItemViewTable(
			Rect frameInParent, 
			/*std::weak_ptr<*/PagedItemViewTableClient* client, 
			Size viewsSize, 
			int initialModelCount, 
			SpriteModel bgSpriteModel
		);
		void reloadData(int newItemsCount);
		void refresh();
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order);
		
		static int fittedHeightWithin(int containerHeight, int viewHeight);
	private:
		PagedItemViewTableClient* _client;
		Size _viewsSize;
		SpriteModel _bgSpriteModel;

		std::vector<std::vector<std::shared_ptr<Renderable>>> _viewsTable;
		int _modelCount;

		int _numberOfRows;
		int _numberOfColumns;
		int _currentTopRowIndex;

		std::shared_ptr<Button> _scrollUpBtn;
		std::shared_ptr<Button> _scrollDownBtn;
		int getCurrentModelIndexForDisplayedCell(int row, int column, int totalItems);
		void scrollUp();
		void scrollDown();

		void onButtonPress(Button*);
		void onButtonPressRelease(Button*);
	};

	struct PagedItemViewTableResources {
		static void initialize();
		static SpriteModel bgSpriteModel;
	};
}


