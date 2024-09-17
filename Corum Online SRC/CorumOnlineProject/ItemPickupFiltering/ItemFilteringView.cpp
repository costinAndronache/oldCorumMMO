#include "ItemFilteringView.h"
using namespace CustomUI;

ItemFilteringView::ItemFilteringView(Rect frame, std::vector<CItem*>& allItems): 
	_frame(frame), _allItems(allItems) {

	Rect inputFieldRect = {
		{ frame.origin.x + 5, frame.origin.y + 5 },
		{ frame.size.width - 10, 30}
	};
	InputFieldResources::initialize();
	_inputField = new InputField(inputFieldRect, InputFieldResources::bgSpriteModel, this);

	Rect tableFrame = {
		{ frame.origin.x + 5, inputFieldRect.maxY() + 5 },
		{ frame.size.width - 10, frame.size.height - (inputFieldRect.size.height + 10) }
	};

	PagedItemViewTableResources::initialize();
	_table = new PagedItemViewTable(tableFrame, SpriteModel::zero);
	_table->setDisplayedItems(allItems);
}


void ItemFilteringView::renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) {
	VECTOR2 scale = _frame.size.divideBy(PagedItemViewTableResources::bgSpriteModel.size);
	VECTOR2 pos = { _frame.origin.x, _frame.origin.y };
	renderer->RenderSprite(PagedItemViewTableResources::bgSpriteModel.sprite,
		&scale, 0, &pos,
		NULL, 0xffffffff,
		order, RENDER_TYPE_DISABLE_TEX_FILTERING);

	_inputField->renderWithRenderer(renderer, order + 1);
	_table->renderWithRenderer(renderer, order + 1);
}

bool ItemFilteringView::handleMouseDown() {
	return _frame.isGlobalMouseInside();
}

bool ItemFilteringView::handleMouseUp() {
	return _frame.isGlobalMouseInside();
}


bool ItemFilteringView::handleKeyDown(WPARAM wparam, LPARAM lparam) {
	return _inputField->handleKeyDown(wparam, lparam);
}

bool ItemFilteringView::handleKeyUp(WPARAM wparam, LPARAM lparam) {
	return _inputField->handleKeyUp(wparam, lparam);
}

void ItemFilteringView::onInputFieldTextChange(InputField* inputField, const char* text) {
	if (strlen(text) == 0) {
		_table->setDisplayedItems(_allItems);
		return;
	}

	updateDisplayedItemsOnNameFilter(text);
}

void ItemFilteringView::updateDisplayedItemsOnNameFilter(const char* nameFilter) {
	char lowerText[InputField::maxChars];
	strcpy(lowerText, nameFilter);
	strlwr(lowerText);

	char lowerName[InputField::maxChars];
	std::vector<CItem*> result;

	for (int i = 0; i < _allItems.size(); i++) {
		CItem* item = _allItems[i];
		strcpy(lowerName, item->GetBaseItem()->szItemName_Eng);
		strlwr(lowerName);

		if (strstr(lowerName, lowerText)) {
			result.push_back(item);
		}

	}

	_table->setDisplayedItems(result);
}