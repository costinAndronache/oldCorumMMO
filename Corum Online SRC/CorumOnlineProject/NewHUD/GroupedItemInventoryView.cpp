#include "GroupedItemInventoryView.h"

using namespace CustomUI;
using namespace NewInterface;


static int radioGroupHeight = 40;

Size GroupedItemInventoryView::preferredSize() {
	const auto sizeSmall = ItemInventoryView::appropriateSizeFor(
		ItemInventoryView::smallInventoryAppearance.containerAppearance.sizes,
		ItemInventoryView::smallInventoryAppearance.elementsCountPerPage
	);

	const auto sizeLarge = ItemInventoryView::appropriateSizeFor(
		ItemInventoryView::largeInventoryAppearance.containerAppearance.sizes,
		ItemInventoryView::largeInventoryAppearance.elementsCountPerPage
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
			SpriteCollection::genericButtonBackground,
			SpriteCollection::genericButtonBackgroundSize
		},
		SpriteModel::zero,
		{
			SpriteCollection::genericButtonPressedBackground,
			SpriteCollection::genericButtonBackgroundSize
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
		return new ItemInventoryView(inventoryFrame, resourceHash, ItemInventoryView::smallInventoryAppearance);
	});

	_largeItemsInventory = registerChildRenderable<ItemInventoryView>([=]() {
		return new ItemInventoryView(inventoryFrame, resourceHash, ItemInventoryView::largeInventoryAppearance);
	});

}

void GroupedItemInventoryView::rebuildWith(
	const std::vector<CItem>& smallItems,
	const std::vector<CItem>& largeItems,
	ItemLongPressHandlerLMB onSmallItemLongPressLMB,
	ItemLongPressHandlerLMB onLargeItemLongPressLMB,
	HandlerItemClickRIGHT onRightClickSmallItem
) {
	_smallItemsInventory->rebuild(smallItems, onSmallItemLongPressLMB, onRightClickSmallItem);
	_largeItemsInventory->rebuild(largeItems, onLargeItemLongPressLMB);
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

ItemInventoryView* GroupedItemInventoryView::inventoryViewFor(Tab tab) {
	switch (tab) {
	case Tab::smallItems:
		return _smallItemsInventory;
	case Tab::largeItems:
		return _largeItemsInventory;
	}
}

struct GroupedItemInventoryView::IndexResult GroupedItemInventoryView::itemIndexForGlobalPoint(CustomUI::Point p) {
	return { inventoryViewFor(_activeTab)->itemIndexForGlobalPoint(p), _activeTab };
}

void GroupedItemInventoryView::setHiddenStateForItemAtIndex(int index, Tab tab, bool isHidden) {
	inventoryViewFor(_activeTab)->setHiddenStateForItemAtIndex(index, isHidden);
}