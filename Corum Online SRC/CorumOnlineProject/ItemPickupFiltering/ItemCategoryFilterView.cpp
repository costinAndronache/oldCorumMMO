#include "ItemCategoryFilterView.h"

using namespace CustomUI;
using namespace ItemPickupFiltering;

ItemCategoryFilterView::ItemCategoryFilterView(CustomUI::Rect frame, CategoryType currentSelectedCategory, ItemCategoryFilterViewClient* client): 
	_client(client) {
	std::vector<RadioButtonGroup::LabeledButtonModel> models;
	RadioButtonGroup::LabeledButtonModel specimen;
	specimen.spriteModel.rotation = 0;
	specimen.spriteModel.sprite = ButtonResources::genericBackground;
	specimen.spriteModel.size = ButtonResources::genericBackgroundSize;

	specimen.pressedStateSpriteModel.sprite = ButtonResources::genericPressedBackground;
	specimen.pressedStateSpriteModel.size = ButtonResources::genericBackgroundSize;
	specimen.pressedStateSpriteModel.rotation = 0;

	specimen.labelModel.appearance.color.a = 255;
	specimen.labelModel.appearance.color.r = 201;
	specimen.labelModel.appearance.color.g = 210;
	specimen.labelModel.appearance.color.b = 250;

	const char* names[] = { "ALL", "WEAPON", "WEARABLE", "SMALL", "SELECTED" };
	for (int i = 0; i < 5; i++) {
		specimen.labelModel.text = names[i];
		models.push_back(specimen);
	}

	_radioButtonGroup = new RadioButtonGroup(models, frame, currentSelectedCategory, this);
}

void ItemCategoryFilterView::renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) {
	_radioButtonGroup->renderWithRenderer(renderer, order);
}

void ItemCategoryFilterView::radioButtonGroupToggledActiveButtonAt(CustomUI::RadioButtonGroup*, unsigned int index) {
	if (index >= CategoryType::CategoryTypeAll && index < CategoryType::CategoryType__EnumEnd) {
		if (_client) {
			_client->itemCategoryFilterViewDidSwitchToCategory(this, (CategoryType)index);
		}

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_BTNCLICK, g_v3InterfaceSoundPos, FALSE);
	}
}