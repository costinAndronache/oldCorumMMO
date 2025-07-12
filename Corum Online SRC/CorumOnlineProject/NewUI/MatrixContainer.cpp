#include "MatrixContainer.h"

using namespace CustomUI;

Size MatrixContainer::appropriateSizeFor(Sizes sizes, int elementsCount) {
	const auto rowCount = ceil((float)elementsCount / sizes.columnsPerRow);
	return Size{
		sizes.columnsPerRow * (sizes.itemSize.width + sizes.spacingX) - sizes.spacingX,
		rowCount * (sizes.itemSize.height + sizes.spacingY) - sizes.spacingY
	};
}

MatrixContainer::MatrixContainer(Rect frameInParent, Appearance appearance) {
	_frameInParent = frameInParent;
	_appearance = appearance;
}

Point MatrixContainer::originForIndex(int index, int maxColumnsPerRow) {
	const auto yCount = index / maxColumnsPerRow;
	const auto xCount = index % maxColumnsPerRow;

	const std::pair<int, Point> signAndGrowthOrigin = [this]() {
		switch (_appearance.verticalGrowthDirection) {
		case VerticalGrowthDirection::downwards:
			return std::make_pair<int, Point>(1, { 0, 0 });
		case VerticalGrowthDirection::upwards:
			return std::make_pair<int, Point>(-1, 
				{ 0, (long)(bounds().size.height - _appearance.sizes.itemSize.height) });
		}
	}();
	

	const auto spacingX = _appearance.sizes.spacingX;
	const auto spacingY = _appearance.sizes.spacingY;

	return Point {
		xCount * ((int)_appearance.sizes.itemSize.width + spacingX) + signAndGrowthOrigin.second.x,
		yCount * ((int)_appearance.sizes.itemSize.height + spacingY) * signAndGrowthOrigin.first + signAndGrowthOrigin.second.y
	};
}

