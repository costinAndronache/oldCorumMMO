#include "HPInfoBarView.h"
#include "HUDSpriteCollection.h"

using namespace CustomUI;
using namespace NewInterface;

HPInfoBarView::HPInfoBarView(CustomUI::Rect frameInParent) {
	_frameInParent = frameInParent;

	updateBackground(NewHUDResources::borderedBlackBackgroundSolid);

	auto hpBarFrame = bounds()
		.withInsets({5, 5, 5, 5});
	_hpBar = registerChildRenderable<HorizontalScalingBar>([=](){
		return std::make_shared<HorizontalScalingBar>(
			NewHUDResources::hpBarSprite,
			hpBarFrame,
			HorizontalScalingBar::Direction::leftRight
		);
	});

	_nameLabel = registerChildRenderable<SingleLineLabel>([=](){
		return std::make_shared<SingleLineLabel>(
			hpBarFrame,
			SingleLineLabel::Appearance::defaultAppearance(),
			""
		);
	});
	_nameLabel->updateRenderingModeToCentered();
}

void HPInfoBarView::updateNameWithScale(std::string name, float hpScale) {
	_nameLabel->updateTextTo(name);
	_hpBar->updateScale(hpScale);
}