#pragma once
#include "RadioButtonGroup.h"
#include "ItemPickupFilteringBase.h"

namespace ItemPickupFiltering {
	class ItemCategoryFilterView;
	enum CategoryType {
		CategoryTypeAll = 0,
		CategoryTypeWeapon = 1,
		CategoryTypeWearable = 2,
		CategoryTypeSmall = 3,
		CategoryTypeSelectedOnly = 4,
		CategoryType__EnumEnd
	};

	class ItemCategoryFilterViewClient {
	public:
		virtual void itemCategoryFilterViewDidSwitchToCategory(ItemCategoryFilterView*, CategoryType category) = 0;
	};

	class ItemCategoryFilterView : public CustomUI::Renderable, public CustomUI::RadioButtonGroupClient {
	public:
		ItemCategoryFilterView(CustomUI::Rect frame, CategoryType currentSelectedCategory, ItemCategoryFilterViewClient* client);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) override;
	private:
		void radioButtonGroupToggledActiveButtonAt(CustomUI::RadioButtonGroup*, unsigned int index) override;

		CustomUI::RadioButtonGroup* _radioButtonGroup;
		ItemCategoryFilterViewClient* _client;
	};
}

