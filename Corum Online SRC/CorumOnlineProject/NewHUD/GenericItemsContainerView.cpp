#include "GenericItemsContainerView.h"

using namespace CustomUI;
using namespace NewInterface;

GenericItemView::GenericItemView(Rect frameInParent, CustomUI::SpriteModel underlay):
	Hoverable(frameInParent) 
{
	auto insets = Insets{ 0, 0, 0, 0 };
	if (underlay.sprite) {
		insets = { 2, 2, 2, 2 };
		registerChildRenderable<SpriteRenderable>([=]() {
			return new SpriteRenderable(bounds(), underlay);
		});
	}

	_button = registerChildRenderable<Button>([=]() {
		return new Button(Button::Sprites::allZero, bounds());
	});

	const auto labelFittedHeight = SingleLineLabel::fittedSize("100").height;

	const auto labelFrame = bounds().withInsets(insets)
		.withHeight(20)
		.fromMaxYOrigin(-3);

	_instanceCountLabel = registerChildRenderable<SingleLineLabel>([=]() {
		return new SingleLineLabel(
			labelFrame, { Color::white, nullptr }, ""
		);
	});
}


GenericItemsContainerView::GenericItemsContainerView(CustomUI::Rect frameInParent, 
							 CItemResourceHash* resourceHash,
							 Appearance appearance) {
	_frameInParent = frameInParent;
	_resourceHash = resourceHash;
	_appearance = appearance;

	_matrixContainer = registerChildRenderable<MatrixContainer>([=]() {
		return new MatrixContainer(
			bounds(),
			_appearance
		);
	});
}

void GenericItemsContainerView::updateWithItems(
	const std::vector<CItem>& items,
	CustomUI::SpriteModel itemUnderlaySprite,
	Handlers handlers
) {
	std::vector<ItemWithUnderlay> withUnderlay(items.size());
	std::transform(
		std::begin(items),
		std::end(items),
		std::begin(withUnderlay),
		[=](CItem item) -> ItemWithUnderlay { return { item, itemUnderlaySprite }; }
	);

	updateWithItems(withUnderlay, handlers);
}

void GenericItemsContainerView::updateWithItems(
	const std::vector<ItemWithUnderlay>& items,
	Handlers handlers
) {
	const auto spriteModelForItem = [=](CItem item) -> SpriteModel {
		if (item.m_wItemID == 0) {
			return SpriteModel::zero;
		}

		LP_ITEM_RESOURCE_EX lpItemResourceEx =
			_resourceHash->GetData(item.m_wItemID);

		if (lpItemResourceEx && lpItemResourceEx->pSpr) {
			SpriteModel specimen = {
				lpItemResourceEx->pSpr, _appearance.sizes.itemSize
			};
			return specimen;
		}
		else {
			return SpriteModel::zero;
		}
	};

	_itemViews.clear();
	_matrixContainer->rebuild<ItemWithUnderlay>(
		items,
		[=](ItemWithUnderlay item, int index, Rect frame) {
		auto result = new GenericItemView(frame, item.itemUnderlaySprite);

		const auto q = item.item.GetQuantity();
		if (q > 1) {
			char text[50];
			wsprintf(text, "%d", q);
			result->_instanceCountLabel->updateTextTo(std::string(text));
		}
		const auto sprite = spriteModelForItem(item.item);
		result->_button->updateSpriteModelTo({ sprite, sprite, sprite });

		if (handlers.longClickLEFT) {
			result->_button->onLongPressDetectedLEFT([=]() {
				handlers.longClickLEFT(item.item, sprite, index, result->globalFrame());
			});
		}

		if (handlers.clickRIGHT) {
			result->_button->onClickEndRIGHT([=]() {
				handlers.clickRIGHT(item.item, index);
			});
		}

		if (handlers.hovering && handlers.hoveringEnd) {
			result->onHover(
				[=](Point globalMousePoint) {
				handlers.hovering(item.item, index, globalMousePoint);
			}, 
				[=]() {
				handlers.hoveringEnd(item.item, index);
			}
			);
		}

		_itemViews.push_back(result);
		return result;
	}
	);
}

int GenericItemsContainerView::itemIndexForGlobalPoint(CustomUI::Point p) {
	for (int i = 0; i < _itemViews.size(); i++) {
		auto view = _itemViews[i];
		const auto viewGlobalFrame = view->globalFrame();
		if (viewGlobalFrame.containsPoint(p)) {
			return i;
		}
	}

	return -1;
}

void GenericItemsContainerView::setHiddenStateForItemAtIndex(int index, bool isHidden) {
	if (!(0 <= index && index < _itemViews.size())) { return;  }
	_itemViews[index]->setHidden(isHidden);
}