#include "CharacterStatsView.h"
#include "HUDSpriteCollection.h"

using namespace CustomUI;
using namespace NewInterface;

static const int entryHeight = 30;

CharacterAttributeView::CharacterAttributeView(CustomUI::Rect frameInParent, Model model) {
	_frameInParent = frameInParent;
	const auto _bounds = bounds();

	auto appearance = SingleLineLabel::Appearance(
		CustomUI::Color::white, GetFont()
	);


	const auto nameFrame = _bounds.withWidth(130);

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

	_nameLabel->updateRenderingModeToCentered();
	_valueLabel->updateRenderingModeToCentered();

}


static const int spacing = 3;
static const int closeButtonHeight = 30;
float CharacterStatsView::appropriateSizeForElementsCountOnPage(int count) {
	const auto pageHeight = count * (entryHeight + spacing) - spacing;
	return closeButtonHeight + PagedContainer::appropriateHeightFor(pageHeight);
}

CharacterStatsView::CharacterStatsView(CustomUI::Rect frameInParent) {
	_frameInParent = frameInParent;
	const auto _bounds = bounds();

	_titleLabel = registerChildRenderable<SingleLineLabel>([=]() {
		return new SingleLineLabel(
			_bounds.withHeight(closeButtonHeight),
			SingleLineLabel::Appearance(Color::white, nullptr),
			"Character"
		);
	});
	_titleLabel->updateRenderingModeToCentered();

	auto closeBtnFrame = _bounds
		.fromMaxXOrigin(-closeButtonHeight)
		.withHeight(closeButtonHeight)
		.withWidth(closeButtonHeight);
	_closeButton = registerChildRenderable<Button>([=]() {
		return new Button(NewHUDResources::xClose, closeBtnFrame);
	});

	auto containerFrame = _bounds
		.withOriginOffsetBy({ 0, (int)closeBtnFrame.size.height })
		.withHeightOffset(-closeBtnFrame.size.height);

	_container = registerChildRenderable<PagedContainer>([=]() {
		return new PagedContainer(containerFrame);
	});

	updateBackground(NewHUDResources::genericBackgroundSprite);
}

void CharacterStatsView::rebuildWithModels(
	const std::vector< std::vector<Model> >& pageModels,
	int availableStatPoints
) {
	auto appearance = MatrixContainer::Appearance{
		MatrixContainer::VerticalGrowthDirection::downwards,
		{
			{bounds().size.width, entryHeight},
			1,
			0,
			spacing
		}
	};

	auto current = _container->activePageIndex();
	_container->rebuildPages<std::vector<Model>>(
		pageModels,
		[=](Rect frameInParent, std::vector<Model> modelsForPage, int) {
			auto mc = new MatrixContainer(frameInParent, appearance);
			mc->rebuild<Model>(
				modelsForPage,
				[=](Model model, int, Rect itemFrame) {
					return new CharacterAttributeView(itemFrame, model);
				}
			);
			return mc;
		}
	);
	_container->setActivePage(current);

	char title[50] = { 0 };
	if (availableStatPoints > 0) {
		wsprintf(title, "Character (%d)", availableStatPoints);
	} else {
		wsprintf(title, "Character");
	}

	_titleLabel->updateTextTo(std::string(title));
}

void CharacterStatsView::onClose(std::function<void()> handler) {
	_closeButton->onClickEndLEFT(handler);
}