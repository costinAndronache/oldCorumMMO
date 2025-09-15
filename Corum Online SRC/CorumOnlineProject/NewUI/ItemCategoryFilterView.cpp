#include "ItemCategoryFilterView.h"

using namespace CustomUI;
using namespace ItemPickupFiltering;

ItemCategoryFilterView::ItemCategoryFilterView(CustomUI::Rect frameInParent, CategoryType currentSelectedCategory) {
	_frameInParent = frameInParent;

	std::vector<RadioButtonGroup::LabeledButtonModel> models;
	RadioButtonGroup::LabeledButtonModel specimen;

	specimen.labelModel.appearance.color.a = 255;
	specimen.labelModel.appearance.color.r = 201;
	specimen.labelModel.appearance.color.g = 210;
	specimen.labelModel.appearance.color.b = 250;

	specimen.sprites = Button::Sprites{
		{
			SpriteCollection::genericButtonBackground,
			SpriteCollection::genericButtonBackgroundSize
		},
		SpriteModel::zero,
		{
			SpriteCollection::genericButtonPressedBackground,
			SpriteCollection::genericButtonBackgroundSize
		},
	};

	const char* names[] = { "ALL", "WEAPON", "WEARABLE", "SMALL", "SELECTED" };
	for (int i = 0; i < 5; i++) {
		specimen.labelModel.text = names[i];
		models.push_back(specimen);
	}

	_radioButtonGroup = registerChildRenderable<RadioButtonGroup>([&]() {
		return std::make_shared<RadioButtonGroup>(models, bounds(), currentSelectedCategory);
	});

	_radioButtonGroup->onActiveIndexUpdate([&](unsigned int index) {
		radioButtonGroupToggledActiveButtonAt(_radioButtonGroup.get(), index);
	});
}

void ItemCategoryFilterView::renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) {
	_radioButtonGroup->renderWithRenderer(renderer, order);
}

void ItemCategoryFilterView::radioButtonGroupToggledActiveButtonAt(CustomUI::RadioButtonGroup*, unsigned int index) {
	if (index >= CategoryType::CategoryTypeAll && index < CategoryType::CategoryType__EnumEnd) {
		if (_handler) { _handler((CategoryType)index); }

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_BTNCLICK, g_v3InterfaceSoundPos, FALSE);
	}
}