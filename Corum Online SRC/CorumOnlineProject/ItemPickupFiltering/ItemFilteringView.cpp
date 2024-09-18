#include "ItemFilteringView.h"
#include "ItemInfoView.h"

using namespace CustomUI;

ItemFilteringView::ItemFilteringView(Rect frame, std::vector<CItem*>& allItems, std::set<DWORD>& selectedItemIDs):
	_frame(frame), _allItems(allItems), _displayedItems(allItems), _selectedItemIDs(selectedItemIDs) {

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

	ItemInfoViewResources::initialize();

	PagedItemViewTableResources::initialize();
	_table = new PagedItemViewTable(tableFrame, this, ItemInfoViewResources::bgSpriteModel.size, allItems.size(), SpriteModel::zero);
}

void ItemFilteringView::selectionViewDidChangeSelectionState(SelectionView* view, bool isSelected) {
	DWORD id = (DWORD)view->data;
	if (isSelected) {
		_selectedItemIDs.insert(id);
	}
	else {
		_selectedItemIDs.erase(id);
	}
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

	for (int i = 0; i < _createdInfoViews.size(); i++) {
		ItemInfoView* iv = static_cast<ItemInfoView*>(_createdInfoViews[i]->renderable());
		if (iv->renderInfoIfMouseHover()) {
			return;
		}
	}
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
		_displayedItems = _allItems;
		_table->reloadData(_displayedItems.size());
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

	const int size = _allItems.size();
	for (int i = 0; i < size; i++) {
		CBaseItem* item = _allItems[i]->GetBaseItem();
		if (!(item && item->szItemName_Eng)) { continue; }
		strcpy(lowerName, item->szItemName_Eng);
		strlwr(lowerName);

		if (
			strstr(lowerName, lowerText) || 
		    (atoi(nameFilter) == item->GetID() && item->GetID() != 0)
			) {
			result.push_back(_allItems[i]);
		}
	}

	_displayedItems = result;
	_table->reloadData(_displayedItems.size());
}

Renderable* ItemFilteringView::buildRenderableForModelAtIndexWithFrame(int modelIndex, Rect frame) {
	CItem* current = NULL;
	if (0 <= modelIndex && modelIndex < _displayedItems.size()) {
		current = _displayedItems[modelIndex];
	}

	ItemInfoView::Model model = { current, ItemInfoViewResources::bgSpriteModel.size };
	ItemInfoView* view = new ItemInfoView(model, frame, ItemInfoViewResources::bgSpriteModel);
	SelectionView* sv = new SelectionView(frame, view, this);
	

	if (current && current->GetBaseItem()) {
		DWORD id = current->GetBaseItem()->GetID();
		sv->setSelectionState(_selectedItemIDs.find(id) != _selectedItemIDs.end());
		sv->data = (void*)id;
	}
	_createdInfoViews.push_back(sv);

	return sv;
}

void ItemFilteringView::updateRenderableWithModelAtIndex(Renderable* renderable, int modelIndex) {
	SelectionView* sv = static_cast<SelectionView*>(renderable);
	ItemInfoView* infoView = static_cast<ItemInfoView*>(sv->renderable());

	CItem* current = NULL;
	if (0 <= modelIndex && modelIndex < _displayedItems.size()) {
		current = _displayedItems[modelIndex];
	}

	if (current && current->GetBaseItem()) {
		DWORD id = current->GetBaseItem()->GetID();
		sv->setSelectionState(_selectedItemIDs.find(id) != _selectedItemIDs.end());
		sv->setEnableSelection(true);
		sv->data = (void*)id;
	}
	else {
		sv->setSelectionState(false);
		sv->setEnableSelection(false);
	}

	ItemInfoView::Model model = { current, ItemInfoViewResources::bgSpriteModel.size };
	infoView->updateModel(model);
}

std::set<DWORD> ItemFilteringView::currentSelectedIDs() {
	return _selectedItemIDs;
}