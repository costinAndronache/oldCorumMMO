#include "GenericItemsContainerView.h"

using namespace CustomUI;
using namespace NewInterface;

GenericItemView::GenericItemView(Rect frameInParent, CustomUI::SpriteModel underlay) {
	_frameInParent = frameInParent;
	auto insets = Insets{ 0, 0, 0, 0 };
	if (underlay.sprite) {
		insets = { 2, 2, 2, 2 };
		registerChildRenderable<SpriteRenderable>([=]() {
			return new SpriteRenderable(bounds(), underlay);
		});
	}

	_button = registerChildRenderable<Button>([=]() {
		return new Button(Button::Sprites::allZero, bounds().withInsets(insets));
	});

	const auto labelFittedHeight = SingleLineLabel::fittedSize(1).height;

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
			_appearance.containerAppearance
		);
	});
}

void GenericItemsContainerView::updateWithItems(
	const std::vector<CItem>& items,
	ItemLongPressHandlerLMB onLongPressItemLMB
) {
	const auto spriteModelForItem = [=](CItem item) -> SpriteModel {
		if (item.m_wItemID == 0) {
			return SpriteModel::zero;
		}

		LP_ITEM_RESOURCE_EX lpItemResourceEx =
			_resourceHash->GetData(item.m_wItemID);

		if (lpItemResourceEx && lpItemResourceEx->pSpr) {
			SpriteModel specimen = {
				lpItemResourceEx->pSpr, _appearance.containerAppearance.sizes.itemSize
			};
			return specimen;
		} else {
			return SpriteModel::zero;
		}
	};

	_itemViews.clear();
	_matrixContainer->rebuild<CItem>(
		items,
		[=](CItem item, int index, Rect frame) {
			auto result = new GenericItemView(frame, _appearance.itemUnderlay);
			
			const auto q = item.GetQuantity();
			if (q > 1) {
				char text[50];
				wsprintf(text, "%d", q);
				result->_instanceCountLabel->updateTextTo(std::string(text));
			}
			const auto sprite = spriteModelForItem(item);
			result->_button->updateSpriteModelTo({ sprite, sprite, sprite });

			result->_button->onLongPressDetectedLEFT([=]() {
				if (onLongPressItemLMB) {
					onLongPressItemLMB(item, sprite, index, result->globalFrame());
				}
			});

			_itemViews.push_back(result);
			return result;
		}
	);
}

int GenericItemsContainerView::itemIndexForGlobalPoint(CustomUI::Point p) {
	for (int i = 0; i < _itemViews.size(); i++) {
		auto view = _itemViews[i];
		if (view->globalFrame().containsPoint(p)) {
			return i;
		}
	}

	return -1;
}

void GenericItemsContainerView::setHiddenStateForItemAtIndex(int index, bool isHidden) {
	if (!(0 <= index && index < _itemViews.size())) { return;  }
	_itemViews[index]->setHidden(isHidden);
}