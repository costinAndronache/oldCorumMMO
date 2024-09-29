#include "PagedItemViewTable.h"
#include "../InitGame.h"
#include "../CorumResource.h"

using namespace CustomUI;

SpriteModel PagedItemViewTableResources::bgSpriteModel = { NULL, {2, 2}, 0 };
void PagedItemViewTableResources::initialize() {
	if (bgSpriteModel.sprite == NULL) {
		char* resourceFile = GetFile("speaking_box.tif", DATA_TYPE_UI);
		bgSpriteModel.sprite = g_pRenderer->CreateSpriteObject(resourceFile,
			0, 0,
			bgSpriteModel.size.width, bgSpriteModel.size.height,
			0);
	}
}

int PagedItemViewTable::getCurrentModelIndexForDisplayedCell(int row, int column, int totalItems) {
	int index = (_currentTopRowIndex + row) * _numberOfColumns + column;
	if (0 <= index && index < totalItems) {
		return index;
	}
	return -1;
}

PagedItemViewTable::PagedItemViewTable(Rect frame, PagedItemViewTableClient* client, Size viewsSize, int initialModelCount, SpriteModel bgSpriteModel):
	_frame(frame), _bgSpriteModel(bgSpriteModel), _client(client), _viewsSize(viewsSize), _modelCount(initialModelCount) {

	Size buttonsSize = { 28, 28 };
	Size tableSize = { _frame.size.width - buttonsSize.width, _frame.size.height};

	
	const Size referenceSize = viewsSize;

	_numberOfRows = tableSize.height / referenceSize.height;
	_numberOfColumns = tableSize.width / referenceSize.width;
	_currentTopRowIndex = 0;
	
	for (int i = 0; i < _numberOfRows; i++) {
		_viewsTable.push_back(std::vector<Renderable*>());

		for (int j = 0; j < _numberOfColumns; j++) {
			int index = getCurrentModelIndexForDisplayedCell(i, j, _modelCount);
			Rect viewFrame = { { 
					_frame.origin.x + (j * referenceSize.width),
					_frame.origin.y + (i * referenceSize.height)
				}, 
				referenceSize
			};

			_viewsTable[i].push_back(_client->buildRenderableForModelAtIndexWithFrame(index, viewFrame));
		}
	}

	ButtonResources::initialize();
	SpriteModel scrollDownModel = {
		ButtonResources::downArrow,
		ButtonResources::downArrowSize,
		0.0
	};

	SpriteModel scrollDownPressedModel = {
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

int PagedItemViewTable::fittedHeightWithin(int containerHeight, int viewHeight) {
	int count = containerHeight / viewHeight;
	return viewHeight * count;
}

void PagedItemViewTable::refresh() {
	for (int i = 0; i < _numberOfRows; i++) {
		for (int j = 0; j < _numberOfColumns; j++) {
			int index = getCurrentModelIndexForDisplayedCell(i, j, _modelCount);
			_client->updateRenderableWithModelAtIndex(_viewsTable[i][j], index);
		}
	}
}

void PagedItemViewTable::reloadData(int newItemsCount) {
	_modelCount = newItemsCount;
	_currentTopRowIndex = 0;
	refresh();
}

void PagedItemViewTable::scrollUp() {
	_currentTopRowIndex = max(0, _currentTopRowIndex - 1);
	refresh();
}

void PagedItemViewTable::scrollDown() {
	_currentTopRowIndex += 1;
	refresh();
}

void PagedItemViewTable::renderWithRenderer(I4DyuchiGXRenderer *renderer, int order) {
	if (_bgSpriteModel.sprite) {
		VECTOR2 blackBackgroundScale = _frame.size.divideBy(_bgSpriteModel.size);
		VECTOR2 pos = { _frame.origin.x, _frame.origin.y };

		renderer->RenderSprite(_bgSpriteModel.sprite,
			&blackBackgroundScale, 0.0f, &pos, 
			NULL, 0xffffffff, order, RENDER_TYPE_DISABLE_TEX_FILTERING);

	}
	
	_scrollDownBtn->renderWithRenderer(g_pRenderer, order + 1);
	_scrollUpBtn->renderWithRenderer(g_pRenderer, order + 1);

	for (int i = 0; i < _numberOfRows; i++) {
		for (int j = 0; j < _numberOfColumns; j++) {
			_viewsTable[i][j]->renderWithRenderer(g_pRenderer, order + 2);
		}
	}

}

void PagedItemViewTable::onButtonPress(Button* button) { }

void PagedItemViewTable::onButtonPressRelease(Button* button) {
	if (button == _scrollDownBtn) {
		scrollDown();
	}

	if (button == _scrollUpBtn) {
		scrollUp();
	}
}