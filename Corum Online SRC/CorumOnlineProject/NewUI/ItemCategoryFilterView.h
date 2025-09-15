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

	typedef std::function<void(CategoryType)> CategoryUpdateHandler;

	class ItemCategoryFilterView : public CustomUI::Renderable {
	public:
		ItemCategoryFilterView(CustomUI::Rect frameInParent, CategoryType currentSelectedCategory);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) override;

		void onCategoryUpdate(CategoryUpdateHandler handler) { _handler = handler; }
	private:
		void radioButtonGroupToggledActiveButtonAt(CustomUI::RadioButtonGroup*, unsigned int index);

		std::shared_ptr<CustomUI::RadioButtonGroup> _radioButtonGroup;
		CategoryUpdateHandler _handler;
	};
}

