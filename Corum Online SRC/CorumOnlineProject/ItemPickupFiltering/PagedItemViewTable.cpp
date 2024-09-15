#include "PagedItemViewTable.h"
#include "../InitGame.h"
#include "../CorumResource.h"

using namespace CustomUI;

//#define	SPR_CHATLIST_UPDOWN1            	167779230   	// .\Data\UI\menu_1.tga
//#define	SPR_CHATLIST_UPDOWN2 

static Size standardLargeItemSize = { 32, 64 };
static Size blackBackgroundSize = { 2 , 2 };

static Size backgroundSpriteSize = { 38, 70 };
static V2_SPRITE* blackBackground = NULL;

static Size downArrowSize = { 14, 14 };
static IDISpriteObject* downArrow = NULL;

CItem* PagedItemViewTable::getCurrentItemForDisplayedCell(int row, int column) {
	int index = (_currentTopRowIndex + row) * _numberOfColumns + column;
	if (0 <= index && index < _currentDisplayedItems.size()) {
		return _currentDisplayedItems[index];
	}
	return nullptr;
}

PagedItemViewTable::PagedItemViewTable(Rect frame, std::vector<CItem*> allItems):
	_frame(frame), _allItems(allItems), _currentDisplayedItems(allItems) {
	const Size referenceSize = { backgroundSpriteSize.width, backgroundSpriteSize.height };

	IDISpriteObject* bgspr = g_pRenderer->CreateSpriteObject(GetFile("menu_4.tif", DATA_TYPE_UI),
		0, 0,
		38, 70,
		0);

	ItemInfoView::setupBackgroundSprite(bgspr, backgroundSpriteSize);

	if (blackBackground == NULL) {
		blackBackground = g_pSprManager->CreateSprite(SPR_SPEAKING_BOX, 100, 100, FALSE, 255);
	}


	Size buttonsSize = { 28, 28 };
	Size tableSize = _frame.size;//{ _frame.size.width - buttonsSize.width, _frame.size.height - buttonsSize.height };

	_numberOfRows = tableSize.height / referenceSize.height;
	_numberOfColumns = tableSize.width / referenceSize.width;
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

	ButtonResources::initialize();
	Button::SpriteModel scrollDownModel = {
		ButtonResources::downArrow,
		ButtonResources::downArrowSize,
		0.0
	};
	Button::SpriteModel scrollDownPressedModel = {
		ButtonResources::downArrowPressed,
		ButtonResources::downArrowSize,
		0.0
	};
	Rect scrollDownBtnFrame = { {_frame.maxX() - buttonsSize.width, _frame.maxY() - buttonsSize.height}, buttonsSize };
	_scrollDownBtn = new Button(scrollDownModel, scrollDownPressedModel, scrollDownBtnFrame, this);

	scrollDownModel.rotation = PI;
	scrollDownPressedModel.rotation = PI;
	Rect scrollUpBtnFrame = { {_frame.maxX() - buttonsSize.width, _frame.origin.y}, buttonsSize };
	_scrollUpBtn = new Button(scrollDownModel, scrollDownPressedModel, scrollUpBtnFrame, this);
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

void PagedItemViewTable::scrollUp() {
	_currentTopRowIndex = max(0, _currentTopRowIndex - 1);
	updateDisplayedRowsWithCurrentItems();
}

void PagedItemViewTable::scrollDown() {
	_currentTopRowIndex += 1;
	updateDisplayedRowsWithCurrentItems();
}

void PagedItemViewTable::render() {
	VECTOR2 blackBackgroundScale = {_frame.size.width/blackBackgroundSize.width, _frame.size.height/blackBackgroundSize.height};
	VECTOR2 pos = { _frame.origin.x, _frame.origin.y };

	g_pRenderer->RenderSprite(blackBackground->pSpr
		, &blackBackgroundScale, 0.0f, &pos, NULL, 0xffffffff, 1, RENDER_TYPE_DISABLE_TEX_FILTERING);

	_scrollDownBtn->renderWithRenderer(g_pRenderer, 2);
	_scrollUpBtn->renderWithRenderer(g_pRenderer, 2);

	for (int i = 0; i < _numberOfRows; i++) {
		for (int j = 0; j < _numberOfColumns; j++) {
			_viewsTable[i][j]->renderImageWithRenderer(g_pRenderer, 3);
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

void PagedItemViewTable::onButtonPress(Button* button) {
}

void PagedItemViewTable::onButtonPressRelease(Button* button) {
	if (button == _scrollDownBtn) {
		scrollDown();
	}

	if (button == _scrollUpBtn) {
		scrollUp();
	}
}