#include "GroupedItemInventoryView.h"

using namespace CustomUI;
using namespace NewInterface;

static ItemInventoryView::Appearance smallInventoryAppearance{
	{
		MatrixContainer::VerticalGrowthDirection::downwards,
		{
			{ 40, 40},
			8,
			5,
			5
		}
	},
	48
};

static ItemInventoryView::Appearance largeInventoryAppearance{
	{
		MatrixContainer::VerticalGrowthDirection::downwards,
		{
			{ 40, 80},
			8,
			5,
			5
		}
	},
	24
};

static int radioGroupHeight = 40;

Size GroupedItemInventoryView::preferredSize() {
	const auto sizeSmall = ItemInventoryView::appropriateSizeFor(
		smallInventoryAppearance.containerAppearance.sizes,
		smallInventoryAppearance.elementsCountPerPage
	);

	const auto sizeLarge = ItemInventoryView::appropriateSizeFor(
		largeInventoryAppearance.containerAppearance.sizes,
		largeInventoryAppearance.elementsCountPerPage
	);

	return {
		max(sizeSmall.width, sizeLarge.width),
		max(sizeSmall.height, sizeLarge.height) + radioGroupHeight
	};
}

GroupedItemInventoryView::GroupedItemInventoryView(CustomUI::Rect frameInParent, CItemResourceHash* resourceHash) {
	_frameInParent = frameInParent;



	RadioButtonGroup::LabeledButtonModel specimen;
	specimen.labelModel.appearance.color.a = 255;
	specimen.labelModel.appearance.color.r = 201;
	specimen.labelModel.appearance.color.g = 210;
	specimen.labelModel.appearance.color.b = 250;

	specimen.sprites = Button::Sprites{
		{
			SpriteCollection::genericBackground,
			SpriteCollection::genericBackgroundSize
		},
		SpriteModel::zero,
		{
			SpriteCollection::genericPressedBackground,
			SpriteCollection::genericBackgroundSize
		},
	};

	auto smallModel = specimen; smallModel.labelModel.text = "SMALL";
	auto largeModel = specimen; largeModel.labelModel.text = "LARGE";

	const auto radioButtonGroupFrame = bounds()
		.withHeight(radioGroupHeight);
	_radioButtonGroup = registerChildRenderable<RadioButtonGroup>([=]() {
		return new RadioButtonGroup(
			std::vector<RadioButtonGroup::LabeledButtonModel>{smallModel, largeModel },
			radioButtonGroupFrame,
			0
		);
	});

	_radioButtonGroup->onActiveIndexUpdate([=](unsigned int index) {
		setActiveTab(index == 0 ? Tab::smallItems : Tab::largeItems);
	});

	const auto inventoryFrame = bounds()
		.withInsets({ radioGroupHeight, 0, 0, 0 });

	_smallItemsInventory = registerChildRenderable<ItemInventoryView>([=]() {
		return new ItemInventoryView(inventoryFrame, resourceHash, smallInventoryAppearance);
	});

	_largeItemsInventory = registerChildRenderable<ItemInventoryView>([=]() {
		return new ItemInventoryView(inventoryFrame, resourceHash, largeInventoryAppearance);
	});

}

void GroupedItemInventoryView::rebuildWith(
	const std::vector<CItem>& smallItems,
	const std::vector<CItem>& largeItems
) {
	_smallItemsInventory->rebuild(smallItems);
	_largeItemsInventory->rebuild(largeItems);
}

void GroupedItemInventoryView::setActiveTab(Tab tab) {
	_activeTab = tab;
	_smallItemsInventory->setHidden(tab != Tab::smallItems);
	_largeItemsInventory->setHidden(tab != Tab::largeItems);

	unsigned int index = [=]() {
		switch (tab) {
		case Tab::smallItems: return 0; break;
		case Tab::largeItems: return 1; break;
		}
	}();
	_radioButtonGroup->setActiveButtonIndex(index);
}