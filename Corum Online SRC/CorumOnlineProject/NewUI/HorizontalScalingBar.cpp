#include "HorizontalScalingBar.h"

using namespace CustomUI;

void HorizontalScalingBar::renderWithRenderer(I4DyuchiGXRenderer* renderer, int zIndex) {
	_barSprite.renderWith(renderer, _currentScaledRect, zIndex);
}

void HorizontalScalingBar::updateScale(float scale) {
	_currentScaledRect = globalFrame().scaled(scale, 1.0);
	switch (_direction) {
	case Direction::leftRight:
		break;
	case Direction::rightLeft:
		_currentScaledRect.origin.x = globalFrame().maxX() - _currentScaledRect.size.width;
		break;
	}
}