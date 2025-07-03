#include "MatrixContainer.h"

using namespace CustomUI;

MatrixContainer::MatrixContainer(Rect frameInParent, VerticalGrowthDirection direction, Size itemSize) {
	_frameInParent = frameInParent;
	_direction = direction;
	_itemSize = itemSize;
}

Point MatrixContainer::originForIndex(int index, int maxColumnsPerRow) {
	const auto yCount = index / maxColumnsPerRow;
	const auto xCount = index % maxColumnsPerRow;

	const std::pair<int, Point> signAndGrowthOrigin = [this]() {
		switch (_direction) {
		case VerticalGrowthDirection::downwards:
			return std::make_pair<int, Point>(1, { 0, 0 });
		case VerticalGrowthDirection::upwards:
			return std::make_pair<int, Point>(-1, 
				{ 0, (long)(bounds().size.height - _itemSize.height) });
		}
	}();
	

	return Point {
		xCount * (int)_itemSize.width + signAndGrowthOrigin.second.x,
		(yCount * (int)_itemSize.width) * signAndGrowthOrigin.first + signAndGrowthOrigin.second.y
	};
}

