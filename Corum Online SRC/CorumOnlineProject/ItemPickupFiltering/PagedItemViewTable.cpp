#include "PagedItemViewTable.h"
#include "../InitGame.h"
#include "../CorumResource.h"

static Size standardLargeItemSize = { 32, 64 };
static Size blackBackgroundSize = { 2 , 2 };

static Size backgroundSpriteSize = { 34, 68 };
static V2_SPRITE* blackBackground = NULL;

CItem* PagedItemViewTable::getCurrentItemForDisplayedCell(int row, int column) {
	int index = (_currentTopRowIndex + row) * _numberOfColumns + column;
	if (0 <= index && index < _currentDisplayedItems.size()) {
		return _currentDisplayedItems[index];
	}
	return nullptr;
}

PagedItemViewTable::PagedItemViewTable(Rect frame, std::vector<CItem*> allItems):
	_frame(frame), _allItems(allItems), _currentDisplayedItems(allItems) {
	const Size referenceSize = { backgroundSpriteSize.width + 10, backgroundSpriteSize.height + 10 };

	IDISpriteObject* bgspr = g_pRenderer->CreateSpriteObject(GetFile("menu_4.tif", DATA_TYPE_UI),
		144, 1,
		backgroundSpriteSize.width, backgroundSpriteSize.height,
		0);

	ItemInfoView::setupBackgroundSprite(bgspr, backgroundSpriteSize);

	if (blackBackground == NULL) {
		blackBackground = g_pSprManager->CreateSprite(SPR_SPEAKING_BOX, 100, 100, FALSE, 255);
	}

	_numberOfRows = frame.size.height / referenceSize.height;
	_numberOfColumns = frame.size.width / referenceSize.width;
	_currentTopRowIndex = 0;

	for (int i = 0; i < _numberOfRows; i++) {
		_viewsTable.push_back(std::vector<ItemInfoView*>());

		for (int j = 0; j < _numberOfColumns; j++) {
			CItem* current = getCurrentItemForDisplayedCell(i, j);
			Rect viewFrame = { { 
					_frame.origin.x + (j * referenceSize.width),
					_frame.origin.y + (i * referenceSize.height)
				}, 
				referenceSize
			};
			ItemInfoView::Model model = { current, referenceSize };
			_viewsTable[i].push_back(new ItemInfoView(model, viewFrame));
		}
	}
}

void PagedItemViewTable::updateDisplayedRowsWithCurrentItems() {
	for (int i = 0; i < _numberOfRows; i++) {
		for (int j = 0; j < _numberOfColumns; j++) {
			CItem* current = getCurrentItemForDisplayedCell(i, j);
			Rect viewFrame = { {
					_frame.origin.x + (j * standardLargeItemSize.width),
					_frame.origin.y + (i * standardLargeItemSize.height)
				},
				standardLargeItemSize
			};
			ItemInfoView::Model model = { current, standardLargeItemSize };
			_viewsTable[i][j]->updateModel(model);
		}
	}
}

void PagedItemViewTable::updateFilter(const char* nameFilter) {

}

void PagedItemViewTable::render() {

	VECTOR2 blackBackgroundScale = {_frame.size.width/blackBackgroundSize.width, _frame.size.height/blackBackgroundSize.height};
	VECTOR2 pos = { _frame.origin.x, _frame.origin.y };

	g_pRenderer->RenderSprite(blackBackground->pSpr
		, &blackBackgroundScale, 0.0f, &pos, NULL, 0xffffffff, 1, RENDER_TYPE_DISABLE_TEX_FILTERING);

	for (int i = 0; i < _numberOfRows; i++) {
		for (int j = 0; j < _numberOfColumns; j++) {
			_viewsTable[i][j]->renderImageWithRenderer(g_pRenderer);
		}
	}

	for (int i = 0; i < _numberOfRows; i++) {
		for (int j = 0; j < _numberOfColumns; j++) {
			if (_viewsTable[i][j]->renderInfoIfMouseHover()) {
				return;
			}
		}
	}
}