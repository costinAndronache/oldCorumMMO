#include "BeltItemsView.h"

using namespace CustomUI;
using namespace NewInterface;

BeltItemView::BeltItemView(Rect frameInParent) {
	_frameInParent = frameInParent;
	_button = registerChildRenderable<Button>([=]() {
		return new Button(Button::Sprites::allZero, bounds());
	});

	const auto labelFittedHeight = SingleLineLabel::fittedSize(1).height;

	const auto labelFrame = bounds()
		.withHeight(20)
		.fromMaxYOrigin(-3);

	_instanceCountLabel = registerChildRenderable<SingleLineLabel>([=]() {
		return new SingleLineLabel(
			labelFrame, { Color::white, nullptr }, ""
		);
	});
}

BeltItemsView::BeltItemsView(CustomUI::Rect frameInParent, 
							 const CItemResourceHash* resourceHash,
							 Appearance appearance) {
	_frameInParent = frameInParent;
	_resourceHash = resourceHash;
	_appearance = appearance;

	for (int i = 0; i < MAX_BELT_POOL; i++) {
		const auto frame = Rect{
			{ i * (int)(appearance.itemSize.width + appearance.spacing), 0},
			appearance.itemSize
		};

		auto view = registerChildRenderable<BeltItemView>([=]() {
			return new BeltItemView(frame);
		});

		_itemViews.push_back(view);
	}

}

void BeltItemsView::updateWithItems(const CItem items[MAX_BELT_POOL]) {
	for (int i = 0; i < MAX_BELT_POOL; i++)
	{
		const auto& item = items[i];
		if (item.m_wItemID != 0) {
			LP_ITEM_RESOURCE_EX lpItemResourceEx =
				g_pItemResourceHash->GetData(item.m_wItemID);

			if (lpItemResourceEx->pSpr) {
				SpriteModel specimen = {
					lpItemResourceEx->pSpr, _appearance.itemSize, 0
				};
				_itemViews[i]->_button->updateSpriteModelTo({
					specimen, specimen, specimen 
				});
			}
			const auto q = item.GetQuantity();
			{
				char text[50];
				wsprintf(text, "%d", q);
				_itemViews[i]->_instanceCountLabel->updateTextTo(std::string(text));
			}
		}
	}
}