#include "Renderable.h"

using namespace CustomUI;

void Renderable::updateMouseState(MouseState newState) {
	const auto old = _currentMouseState;
	if (old == newState) { return; }
	_currentMouseState = newState;

	onMouseStateChange(newState, old); 
}

void Renderable::handleMouseDown(Point mouse) {
	if (_isHidden) { return; }
	for (auto child : _childRenderables) {
		child->handleMouseDown(mouse);
	}

	if (!_frame.isMouseInside(mouse)) {
		updateMouseState(MouseState::none);
		return;
	}
	updateMouseState(MouseState::pressedInside);
}

void Renderable::handleMouseUp(Point mouse) {
	if (_isHidden) { return; }
	for (auto child : _childRenderables) {
		child->handleMouseUp(mouse);
	}

	updateMouseState(
		_frame.isMouseInside(mouse) ? MouseState::hovering : MouseState::none
	);
}

void Renderable::handleMouseMove(Point mouse) {
	if (_isHidden) { return; }
	for (auto child : _childRenderables) {
		child->handleMouseMove(mouse);
	}

	if (_currentMouseState == MouseState::pressedInside) { return; }

	updateMouseState(
		_frame.isMouseInside(mouse) ? MouseState::hovering: MouseState::none
	);
}

bool Renderable::swallowsMouse(Point mouse) {
	return !_isHidden && _frame.isMouseInside(mouse);
}