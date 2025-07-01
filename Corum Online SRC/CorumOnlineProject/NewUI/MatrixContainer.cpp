#include "MatrixContainer.h"

using namespace CustomUI;

MatrixContainer::MatrixContainer(Point growthOrigin, VerticalGrowthDirection direction, Size itemSize) {
	_growthOrigin = growthOrigin;
	_direction = direction;
	_itemSize = itemSize;
}

Point MatrixContainer::originForIndex(int index, int maxColumnsPerRow) {
	const auto yCount = index / maxColumnsPerRow;
	const auto xCount = index % maxColumnsPerRow;

	const int ySign = [this]() {
		switch (_direction) {
		case VerticalGrowthDirection::downwards:
			return 1;
		case VerticalGrowthDirection::upwards:
			return -1;
		}
	}();

	return Point {
		xCount * (int)_itemSize.width + _growthOrigin.x,
		(yCount * (int)_itemSize.width) * ySign + _growthOrigin.y
	};
}

