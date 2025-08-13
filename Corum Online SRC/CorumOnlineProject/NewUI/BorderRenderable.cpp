#include "BorderRenderable.h"

using namespace CustomUI;

BorderRenderable::BorderRenderable(Rect frameInParent) {
	_frameInParent = frameInParent;
	_thickness = 1;
}

void BorderRenderable::renderWithRenderer(I4DyuchiGXRenderer* renderer, int zIndex) {
	if (!_horizontalLine.sprite || !_verticalLine.sprite) { return; }
	auto _globalFrame = globalFrame();

	auto leftFrame = _globalFrame.withWidth(_thickness);
	auto rightFrame = _globalFrame
		.fromMaxXOrigin(-_thickness)
		.withWidth(_thickness);

	auto topFrame = _globalFrame.withHeight(_thickness);
	auto bottomFrame = _globalFrame
		.fromMaxYOrigin(-_thickness)
		.withHeight(_thickness);

	_verticalLine.renderWith(renderer, leftFrame, zIndex + 1);
	_verticalLine.renderWith(renderer, rightFrame, zIndex + 1);
	_horizontalLine.renderWith(renderer, topFrame, zIndex + 1);
	_horizontalLine.renderWith(renderer, bottomFrame, zIndex + 1);
}

void BorderRenderable::updateSingleBorderLine(SpriteModel borderLineSprite) {
	_verticalLine = _horizontalLine = borderLineSprite;
}

void BorderRenderable::updateBorderLines(BorderLineSprites sprites) {
	_verticalLine = sprites.vertical;
	_horizontalLine = sprites.horizontal;
}
void BorderRenderable::updateThickness(float thickness) {
	_thickness = thickness;
}