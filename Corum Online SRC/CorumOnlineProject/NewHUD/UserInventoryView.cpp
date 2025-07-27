#include "UserInventoryView.h"

using namespace NewInterface;
using namespace CustomUI;

static const auto btnSize = Size{ 40, 40 };

static const auto equipAppearance = GenericItemsContainerView::Appearance{
	MatrixContainer::VerticalGrowthDirection::downwards,
	ItemInventoryView::largeInventoryAppearance.containerAppearance.sizes
};

Size NewItemsWindow::preferredSize() {
	const auto groupedSize = GroupedItemInventoryView::preferredSize();
	const auto equipSize = MatrixContainer::appropriateSizeFor(
		equipAppearance.sizes, MAX_EQUIP_POOL
	);

	return {
		max(groupedSize.width, equipSize.width),
		groupedSize.height + equipSize.height + btnSize.height
	};
}

NewItemsWindow::NewItemsWindow(CustomUI::Point originInParent, CItemResourceHash* itemResourceHash) {
	const auto groupedSize = GroupedItemInventoryView::preferredSize();
	const auto equipSize = MatrixContainer::appropriateSizeFor(
		equipAppearance.sizes, MAX_EQUIP_POOL
	);

	_frameInParent = { 
		originInParent, 
		{
			max(groupedSize.width, equipSize.width),
			groupedSize.height + equipSize.height + btnSize.height
		} 
	};

	const auto _bounds = bounds();

	auto labelFrame = _bounds.withHeight(btnSize.height);
	_titleLabel = registerChildRenderable<SingleLineLabel>([=]() {
		return new SingleLineLabel(
			labelFrame,
			SingleLineLabel::Appearance::defaultAppearance(),
			"INVENTORY"
		);
	});

	const auto btnFrame = _bounds
		.fromMaxXOrigin(-btnSize.width)
		.withSize(btnSize);

	_closeButton = registerChildRenderable<Button>([=]() {
		return new Button(
			Button::Sprites { 
				SpriteCollection::xCloseSprite, 
				SpriteCollection::xCloseSprite, 
				SpriteCollection::xClosePressedSprite 
			},
			btnFrame
		);
	});

	_closeButton->onClickEndLEFT([=]() {
		if (_onCloseHandler) { _onCloseHandler(); }
	});

	const auto equipFrame = Rect{
		{0, (int)btnSize.height},
		equipSize
	};
	
	_equipItemsView = registerChildRenderable<GenericItemsContainerView>([=]() {
		return new GenericItemsContainerView(equipFrame, itemResourceHash, equipAppearance);
	});

	const auto groupedFrame = Rect{
		{0, (int)equipFrame.maxY()},
		groupedSize
	};

	_groupedInventoryView = registerChildRenderable<GroupedItemInventoryView>([=]() {
		return new GroupedItemInventoryView(groupedFrame, itemResourceHash);
	});

	_groupedInventoryView->updateBackground(NewHUDResources::inventoryItemUnderlays.inventoryBackground);

	_groupedInventoryView->setActiveTab(GroupedItemInventoryView::Tab::smallItems);
	updateBackground(NewHUDResources::genericBackgroundSprite);
}

