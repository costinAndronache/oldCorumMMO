#include "Hoverable.h"

using namespace CustomUI;

Hoverable::Hoverable() {
	_onMouseMove = nullptr;
	_onMouseStateChange = nullptr;
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

void Hoverable::onMouseStateChange(MouseState newState, MouseState oldState) {
	if (_onMouseStateChange) {
		_onMouseStateChange(newState);
	}
}

void Hoverable::onMouseMove(Point mouseGlobalOrigin) {
	if (_onMouseMove) {
		_onMouseMove(mouseGlobalOrigin);
	}
}