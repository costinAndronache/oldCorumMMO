#include "DynamicInfoBox.h"
#include <algorithm>

#include "HUDSpriteCollection.h"
using namespace CustomUI;
using namespace NewInterface;


static MatrixContainer::Appearance appearance(Size oneLineSize) {
	return {
		MatrixContainer::VerticalGrowthDirection::downwards,
		{
			oneLineSize,
			1,
			0,
			5
		}
	};
};

DynamicInfoBox::DynamicInfoBox(CustomUI::Point pointInOrigin) {
	_frameInParent = { pointInOrigin, {0, 0} };
	updateBackground(NewHUDResources::genericBackgroundSprite);
}

void DynamicInfoBox::updateWithLines(const std::vector<InfoLine>& lines) {
	deconstructAllChildren();

	const auto maxElement = std::max_element(
		std::begin(lines),
		std::end(lines),
		[=](const InfoLine& a, const InfoLine& b) { return a.text.size() < b.text.size(); }
	);

	if (maxElement == std::end(lines)) { return; }

	auto bigInsets = Insets{ 5, 5, 5, 5 };
	auto smallInsets = Insets{ 2, 2, 2, 2 };

	const auto maxLineSize = SingleLineLabel::fittedSize((*maxElement).text);
	const auto containerAppearance = appearance(maxLineSize);
	const auto totalSize = MatrixContainer::appropriateSizeFor(
		containerAppearance.sizes, lines.size()
	).toCoverInsets(bigInsets);

	_frameInParent = { _frameInParent.origin, totalSize };
	_border = registerChildRenderable<BorderRenderable>([=]() {
		return std::make_shared<BorderRenderable>(bounds());
	});

	_border->updateSingleBorderLine(NewHUDResources::blueDot);
	_infoLinesContainer = registerChildRenderable<MatrixContainer>([=]() {
		return std::make_shared<MatrixContainer>(bounds().withInsets(smallInsets), containerAppearance);
	});

	_infoLinesContainer->rebuild<InfoLine>(
		lines,
		[=](InfoLine line, int, Rect frameInContainer) {
			auto label = std::make_shared<SingleLineLabel>(
				frameInContainer,
				SingleLineLabel::Appearance(line.color),
				line.text
			);
			return label;
		}
	);
}