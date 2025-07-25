#include "CharacterStatsView.h"
#include "HUDSpriteCollection.h"

using namespace CustomUI;
using namespace NewInterface;

static const int entryHeight = 40;

CharacterAttributeView::CharacterAttributeView(CustomUI::Rect frameInParent, Model model) {
	_frameInParent = frameInParent;
	const auto _bounds = bounds();

	auto appearance = SingleLineLabel::Appearance(CustomUI::Color::white, GetFont(), {5, entryHeight / 3 - 1});

	const auto nameFrame = _bounds.withWidth(100);

	_nameLabel = registerChildRenderable<SingleLineLabel>([=]() {
		return new SingleLineLabel(nameFrame, appearance, model.name);
	});

	const auto valueFrame = _bounds
		.withOriginOffsetBy({ (int)nameFrame.size.width, 0 })
		.withWidthOffset(-nameFrame.size.width);

	_valueLabel = registerChildRenderable<SingleLineLabel>([=]() {
		return new SingleLineLabel(valueFrame, appearance, model.value);
	});

	if (model.increaseAction) {
		auto btnFrame = _bounds
			.fromMaxXOrigin(-_bounds.size.height)
			.withWidth(_bounds.size.height)
			.withInsets({ 5, 5, 5, 5 });

		_increaseButton = registerChildRenderable<Button>([=]() {
			return new Button(
				NewHUDResources::plus,
				btnFrame
			);
		});
		_increaseButton->onClickEndLEFT(model.increaseAction);
	}

	_nameLabel->updateBackground(NewHUDResources::borderedBlackBackgroundSolid);
	_valueLabel->updateBackground(NewHUDResources::borderedBlackBackgroundFaded);
}


static const int spacing = 3;
static const int closeButtonHeight = 30;
float CharacterStatsView::appropriateSizeForElementsCount(int count) {
	return closeButtonHeight + count * (entryHeight + spacing) - spacing;
}

CharacterStatsView::CharacterStatsView(CustomUI::Rect frameInParent) {
	_frameInParent = frameInParent;
	const auto _bounds = bounds();

	_titleLabel = registerChildRenderable<SingleLineLabel>([=]() {
		return new SingleLineLabel(
			_bounds.withHeight(closeButtonHeight),
			SingleLineLabel::Appearance(Color::white, nullptr, { 3, 1 }),
			"Character"
		);
	});
	auto closeBtnFrame = _bounds
		.fromMaxXOrigin(-closeButtonHeight)
		.withHeight(closeButtonHeight)
		.withWidth(closeButtonHeight);
	_closeButton = registerChildRenderable<Button>([=]() {
		return new Button(NewHUDResources::xClose, closeBtnFrame);
	});

	auto appearance = MatrixContainer::Appearance{
		MatrixContainer::VerticalGrowthDirection::downwards,
		{
			{_bounds.size.width, entryHeight},
			1,
			0,
			spacing
		}
	};
	auto containerFrame = _bounds.withOriginOffsetBy({ 0, (int)closeBtnFrame.size.height });
	_mc = registerChildRenderable<MatrixContainer>([=]() {
		return new MatrixContainer(containerFrame, appearance);
	});

	updateBackground(NewHUDResources::genericBackgroundSprite);
}

void CharacterStatsView::rebuildWithModels(const std::vector<Model>& models) {
	_mc->rebuild<Model>(
		models,
		[=](Model m, int, Rect frame) {
			return new CharacterAttributeView(frame, m);
		}
	);
}

void CharacterStatsView::onClose(std::function<void()> handler) {
	_closeButton->onClickEndLEFT(handler);
}