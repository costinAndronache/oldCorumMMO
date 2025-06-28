#include "HorizontalScalingBar.h"

using namespace CustomUI;

void HorizontalScalingBar::renderWithRenderer(I4DyuchiGXRenderer* renderer) {
	_barSprite.renderWith(renderer, _currentScaledRect);
}

void HorizontalScalingBar::updateScale(float scale) {
	_currentScaledRect = _frame.scaled(scale, 1.0);
	switch (_direction) {
	case Direction::leftRight:
		break;
	case Direction::rightLeft:
		_currentScaledRect.origin.x = _frame.maxX() - _currentScaledRect.size.width;
		break;
	}
}