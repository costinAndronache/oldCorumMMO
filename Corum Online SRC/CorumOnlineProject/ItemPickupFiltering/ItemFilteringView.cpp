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

bool ItemFilteringView::handleKeyUp(WORD keyCode) {
	return _inputField->handleKeyUp(keyCode);
}

void ItemFilteringView::onInputFieldTextChange(InputField* inputField, const char* text) {

}