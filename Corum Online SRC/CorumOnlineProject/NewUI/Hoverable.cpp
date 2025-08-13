#include "Hoverable.h"

using namespace CustomUI;

Hoverable::Hoverable(Rect frameInParent) {
	_onMouseMove = nullptr;
	_onMouseStateChange = nullptr;
	_frameInParent = frameInParent;
}

void Hoverable::onHover(
	OnHovering onHovering,
	OnHoveringEnd onHoveringEnd
) {
	_onMouseStateChange = [=](MouseState newState) {
		if (newState == MouseState::none) {
			_onMouseMove = nullptr;
			if (onHoveringEnd) { onHoveringEnd(); }
		}
		else {
			_onMouseMove = onHovering;
		}
	};
}

void Hoverable::onMouseStateChange(MouseState newState, MouseState oldState, Point) {
	if (_onMouseStateChange) {
		_onMouseStateChange(newState);
	}
}

void Hoverable::onMouseMove(Point mouseGlobalOrigin) {
	if (_onMouseMove) {
		_onMouseMove(mouseGlobalOrigin);
	}
}