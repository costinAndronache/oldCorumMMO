#include "ItemFilteringView.h"
#include "ItemInfoView.h"
#include "../InitGame.h"
#include "../CorumResource.h"

using namespace CustomUI;
using namespace ItemPickupFiltering;

ItemFilteringView::ItemFilteringView(Rect frame, std::vector<CItem*>& allItems, std::set<DWORD>& selectedItemIDs, ItemFilteringViewClient* client):
	_frame(frame), _allItems(allItems), _textFilteringSourceItems(allItems), _displayedItems(allItems), _selectedItemIDs(selectedItemIDs),
	_isHidden(true), _client(client) {

	ButtonResources::initialize();

	Rect topContainer = {
		{frame.origin.x + 5, frame.origin.y + 5},
		{frame.size.width - 5, 30}
	};

	const char* title = "Item pick-up filtering";
	Rect titleLabelRect = topContainer
		.withSize(SingleLineLabel::fittedSize(strlen(title)))
		.centeredVerticallyWith(topContainer);

	SingleLineLabel::Appearance titleAppearance = { {255,255,255,255 } };

	_titleLabel = new SingleLineLabel(titleLabelRect, titleAppearance, title);

	SpriteModel closeSpriteModel = { ButtonResources::xClose, ButtonResources::xCloseSize, 0 };
	SpriteModel closePressedSpriteModel = { ButtonResources::xClosePressed, ButtonResources::xCloseSize, 0 };
	Rect closeButtonRect = topContainer
		.fromMaxXOrigin(-30)
		.withWidth(30);

	_closeButton = new Button(closeSpriteModel, closePressedSpriteModel, closeButtonRect, this);

	Rect inputFieldContainer = Rect::zero()
		.horizontallyAlignedWith(topContainer)
		.positionedBelow(topContainer, 2)
		.withHeight(30);

	InputFieldResources::initialize();
	SpriteModel clearBtnModel = { ButtonResources::xRedBlack, ButtonResources::xRedBlackSize, 0 };
	SpriteModel clearBtnPressedModel = { ButtonResources::xRedBlackPressed, ButtonResources::xRedBlackSize, 0 };

	_inputField = new InputField(inputFieldContainer, InputFieldResources::bgSpriteModel, clearBtnModel, clearBtnPressedModel, this);

	Rect categoriesFilterContainer = Rect::zero()
		.horizontallyAlignedWith(inputFieldContainer)
		.positionedBelow(inputFieldContainer, 2)
		.withHeight(40);

	_categoriesFilterView = new ItemCategoryFilterView(categoriesFilterContainer, CategoryType::CategoryTypeAll, this);

	Rect tableContainer = Rect::zero()
		.horizontallyAlignedWith(categoriesFilterContainer)
		.positionedBelow(categoriesFilterContainer, 2)
		.withHeight(frame.maxY() - categoriesFilterContainer.maxY() - 2);

	tableContainer.size.height = PagedItemViewTable::fittedHeightWithin(tableContainer.size.height, 
		ItemInfoViewResources::bgSpriteModel.size.height);

	ItemInfoViewResources::initialize();

	PagedItemViewTableResources::initialize();
	_table = new PagedItemViewTable(tableContainer, this, ItemInfoViewResources::bgSpriteModel.size, allItems.size(), SpriteModel::zero);
	
}

void ItemFilteringView::selectionViewDidChangeSelectionState(SelectionView* view, bool isSelected) {
	DWORD id = (DWORD)view->data;
	if (isSelected) {
		_selectedItemIDs.insert(id);
	}
	else {
		_selectedItemIDs.erase(id);
	}

	if (_client) {
		_client->itemFilteringViewDidUpdateSelection(this, _selectedItemIDs);
	}

	_table->refresh();
}

void ItemFilteringView::renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) {
	if (_isHidden) {
		return;
	}

	VECTOR2 scale = _frame.size.divideBy(PagedItemViewTableResources::bgSpriteModel.size);
	VECTOR2 pos = { _frame.origin.x, _frame.origin.y };
	Color c = { 255, 0, 255, 255 };

	renderer->RenderSprite(PagedItemViewTableResources::bgSpriteModel.sprite,
		&scale, 0, &pos,
		NULL, c.asDXColor(),
		order, RENDER_TYPE_DISABLE_TEX_FILTERING);

	_inputField->renderWithRenderer(renderer, order + 1);
	_table->renderWithRenderer(renderer, order + 1);
	_categoriesFilterView->renderWithRenderer(renderer, order + 1);
	_titleLabel->renderWithRenderer(renderer, order + 1);
	_closeButton->renderWithRenderer(renderer, order + 1);

	for (int i = 0; i < _createdInfoViews.size(); i++) {
		ItemInfoView* iv = static_cast<ItemInfoView*>(_createdInfoViews[i]->renderable());
		if (iv->renderInfoIfMouseHover()) {
			return;
		}
	}
}

bool ItemFilteringView::handleMouseDown() {
	return !_isHidden && _frame.isGlobalMouseInside();
}

bool ItemFilteringView::handleMouseUp() {
	return !_isHidden && _frame.isGlobalMouseInside();
}


bool ItemFilteringView::handleKeyDown(WPARAM wparam, LPARAM lparam) {
	return !_isHidden && _inputField->handleKeyDown(wparam, lparam);
}

bool ItemFilteringView::handleKeyUp(WPARAM wparam, LPARAM lparam) {
	return !_isHidden && _inputField->handleKeyUp(wparam, lparam);
}

std::vector<CItem*> itemsMatchingNameFilter(const std::vector<CItem*>& sourceItems, const char* nameFilter);
void ItemFilteringView::onInputFieldTextChange(InputField* inputField, const char* text) {
	_displayedItems = itemsMatchingNameFilter(_textFilteringSourceItems, text);
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

void ItemFilteringView::onButtonPress(Button* button) { }

void ItemFilteringView::onButtonPressRelease(Button* button) {
	if (button == _closeButton) {
		_isHidden = true;
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
}

std::set<DWORD> ItemFilteringView::currentSelectedIDs() {
	return _selectedItemIDs;
}

void ItemFilteringView::setHidden(bool isHidden) {
	_isHidden = isHidden;
}

void ItemFilteringView::updateTextFilteringSourceItemsTo(std::vector<CItem*> newSourceItems) {
	_textFilteringSourceItems = newSourceItems;
	_displayedItems = itemsMatchingNameFilter(_textFilteringSourceItems, _inputField->currentText());
	_table->reloadData(_displayedItems.size());
}

std::vector<CItem*> itemsMatchingSelectedIDs(std::vector<CItem*>& sourceItems, std::set<DWORD> selectedIDs);
std::vector<CItem*> itemsMatchingCategoryIDs(std::vector<CItem*>& sourceItems, std::vector<DWORD> ids);
void ItemFilteringView::itemCategoryFilterViewDidSwitchToCategory(ItemCategoryFilterView*, CategoryType category) {
	std::vector<DWORD> ids;
	
	switch (category) {
	case CategoryType::CategoryTypeAll: {
		updateTextFilteringSourceItemsTo(_allItems);
		return;
	} break;
	case CategoryType::CategoryTypeSelectedOnly: {
		updateTextFilteringSourceItemsTo(itemsMatchingSelectedIDs(_allItems, _selectedItemIDs));
		return;
	} break;
	case CategoryType::CategoryTypeWearable: {
		DWORD arr[] = { ITEM_KIND_AROMR };
		ids.insert(ids.begin(), arr, arr + sizeof(arr));
	} break;
	case CategoryType::CategoryTypeWeapon: {
		DWORD arr[] = { ITEM_KIND_WEAPON };
		ids.insert(ids.begin(), arr, arr + sizeof(arr));
	} break;
	case CategoryType::CategoryTypeSmall: {
		DWORD arr[] = {
			ITEM_KIND_SUPPLIES,
			ITEM_KIND_CONSUMABLE,
			ITEM_KIND_ZODIAC,
			ITEM_KIND_SPECIAL,
			ITEM_KIND_UPGRADE,
			ITEM_KIND_LIQUID,
			ITEM_KIND_EDITION,
			ITEM_KIND_MAGICFIELDARRAY,
			ITEM_KIND_MIXUPGRADE,
			ITEM_KIND_MATERIALS,
			ITEM_KIND_MAGICARRAY,
			ITEM_KIND_RIDE,
			ITEM_KIND_BAG
		};
	
		ids.insert(ids.begin(), arr, arr + sizeof(arr));
	} break;

	}

	updateTextFilteringSourceItemsTo(itemsMatchingCategoryIDs(_allItems, ids));
}

static std::vector<CItem*> itemsMatchingNameFilter(const std::vector<CItem*>& sourceItems, const char* nameFilter) {
	if (strlen(nameFilter) == 0) {
		return sourceItems;
	}

	char lowerText[InputField::maxChars];
	strcpy(lowerText, nameFilter);
	strlwr(lowerText);

	char lowerName[InputField::maxChars];
	std::vector<CItem*> result;

	const int size = sourceItems.size();
	for (int i = 0; i < size; i++) {
		CBaseItem* item = sourceItems[i]->GetBaseItem();
		if (!(item && item->szItemName_Eng)) { continue; }
		strcpy(lowerName, item->szItemName_Eng);
		strlwr(lowerName);

		if (
			strstr(lowerName, lowerText) ||
			(atoi(nameFilter) == item->GetID() && item->GetID() != 0)
			) {
			result.push_back(sourceItems[i]);
		}
	}

	return result;
}

static std::vector<CItem*> itemsMatchingCategoryIDs(std::vector<CItem*>& sourceItems, std::vector<DWORD> ids) {
	std::vector<CItem*> result;
	for (auto it = sourceItems.begin(); it != sourceItems.end(); it++) {
		CItem* item = *it;
		if (!item->GetBaseItem()) {
			continue;
		}
		DWORD id = item->GetBaseItem()->dwCode_ID;
		if (std::find(ids.begin(), ids.end(), id) != ids.end()) {
			result.push_back(item);
		}

	}
	return result;
}

static std::vector<CItem*> itemsMatchingSelectedIDs(std::vector<CItem*>& sourceItems, std::set<DWORD> selectedIDs) {
	std::vector<CItem*> result;
	for (auto it = sourceItems.begin(); it != sourceItems.end(); it++) {
		if (std::find(selectedIDs.begin(), selectedIDs.end(), (*it)->GetID()) != selectedIDs.end()) {
			result.push_back(*it);
		}
	}

	return result;
}















