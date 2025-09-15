#include "PagedItemViewTable.h"
#include "../InitGame.h"
#include "../CorumResource.h"

using namespace CustomUI;

SpriteModel PagedItemViewTableResources::bgSpriteModel = { NULL, {2, 2} };
void PagedItemViewTableResources::initialize() {
	if (bgSpriteModel.sprite == NULL) {
		char* resourceFile = GetFile("speaking_box.tif", DATA_TYPE_UI);
		bgSpriteModel.sprite = g_pRenderer->CreateSpriteObject(resourceFile,
			0, 0,
			bgSpriteModel.naturalSize.width, bgSpriteModel.naturalSize.height,
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

PagedItemViewTable::PagedItemViewTable(
	Rect frameInParent, 
	PagedItemViewTableClient* client, 
	Size viewsSize, 
	int initialModelCount, 
	SpriteModel bgSpriteModel
):
	_bgSpriteModel(bgSpriteModel), _client(client), _viewsSize(viewsSize), _modelCount(initialModelCount) {

	_frameInParent = frameInParent;
	Size buttonsSize = { 28, 28 };
	Size tableSize = { 
		frameInParent.size.width - buttonsSize.width,
		frameInParent.size.height
	};
	
	const Size referenceSize = viewsSize;

	_numberOfRows = tableSize.height / referenceSize.height;
	_numberOfColumns = tableSize.width / referenceSize.width;
	_currentTopRowIndex = 0;
	
	for (int i = 0; i < _numberOfRows; i++) {
		_viewsTable.push_back({});

		for (int j = 0; j < _numberOfColumns; j++) {
			int index = getCurrentModelIndexForDisplayedCell(i, j, _modelCount);
			Rect viewFrame = { { 
					 (j * referenceSize.width),
					 (i * referenceSize.height)
				}, 
				referenceSize
			};


			const auto renderable = registerChildRenderable<Renderable>([&]() {
				return client->buildRenderableForModelAtIndexWithFrame(index, viewFrame);
			});
			_viewsTable[i].push_back(renderable);
		}
	}

	SpriteModel scrollDownModel = {
		SpriteCollection::downArrow,
		SpriteCollection::downArrowSize
	};

	SpriteModel scrollDownPressedModel = {
		SpriteCollection::downArrowPressed,
		SpriteCollection::downArrowSize
	};

	const auto _bounds = bounds();
	Rect scrollDownBtnFrame = { {_bounds.maxX() - buttonsSize.width, _bounds.maxY() - buttonsSize.height}, buttonsSize };
	_scrollDownBtn = registerChildRenderable<Button>([&]() {
		return std::make_shared<Button>(
			Button::Sprites{ scrollDownModel, SpriteModel::zero, scrollDownPressedModel},
			scrollDownBtnFrame
		);
	});
	

	_scrollDownBtn->onClickEndLEFT([this]() {
		onButtonPressRelease(_scrollDownBtn.get());
	});

	Rect scrollUpBtnFrame = { {_bounds.maxX() - buttonsSize.width, _bounds.origin.y}, buttonsSize };
	_scrollUpBtn = registerChildRenderable<Button>([&]() {
		return std::make_shared<Button>(
			Button::Sprites{ scrollDownModel, SpriteModel::zero, scrollDownPressedModel },
			scrollUpBtnFrame);
	});

	_scrollUpBtn->onClickEndLEFT([this]() {
		onButtonPressRelease(_scrollUpBtn.get());
	});
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
	const auto _globalFrame = globalFrame();

	if (_bgSpriteModel.sprite) {
		_bgSpriteModel.renderWith(renderer, _globalFrame, order);
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
	if (button == _scrollDownBtn.get()) {
		scrollDown();
	}

	if (button == _scrollUpBtn.get()) {
		scrollUp();
	}
}