#include "Renderable.h"

using namespace CustomUI;


Rect Renderable::globalFrame() const {
	if (_parent == nullptr) { return _frameInParent; }
	return _frameInParent.withOriginOffsetBy(_parent->globalFrame().origin);
}

void Renderable::updateMouseState(MouseState newState) {
	const auto old = _currentMouseState;
	if (old == newState) { return; }
	_currentMouseState = newState;

	onMouseStateChange(newState, old); 
}

void Renderable::handleMouseDown(Point mouse) {
	if (_isHidden) { return; }
	
	if (!globalFrame().isMouseInside(mouse)) {
		updateMouseState(MouseState::none);
		return;
	}
	updateMouseState(MouseState::pressedInside);

	if (swallowsMouseEvents()) { return; }
	for (auto child : _childRenderables) {
		child->handleMouseDown(mouse);
	}
}

void Renderable::handleMouseUp(Point mouse) {
	if (_isHidden) { return; }
	updateMouseState(
		globalFrame().isMouseInside(mouse) ? MouseState::hovering : MouseState::none
	);

	if (swallowsMouseEvents()) { return; }
	for (auto child : _childRenderables) {
		child->handleMouseUp(mouse);
	}
}

void Renderable::handleMouseMove(Point mouse) {
	if (_isHidden) { return; }

	if (!swallowsMouseEvents()) {
		for (auto child : _childRenderables) {
			child->handleMouseMove(mouse);
		}
	}


	if (_currentMouseState == MouseState::pressedInside) { return; }

	updateMouseState(
		globalFrame().isMouseInside(mouse) ? MouseState::hovering : MouseState::none
	);
}

void Renderable::handleKeyUp(WPARAM wparam, LPARAM lparam) {
	if (_isHidden) { return; }
	processKeyUp(wparam, lparam);

	if (swallowsKeyEvents()) { return; }

	for (auto child : _childRenderables) {
		child->handleKeyUp(wparam, lparam);
	}
}

void Renderable::handleKeyDown(WPARAM wparam, LPARAM lparam) {
	if (_isHidden) { return; }
	processKeyDown(wparam, lparam);

	if (swallowsKeyEvents()) { return; }
	for (auto child : _childRenderables) {
		child->handleKeyDown(wparam, lparam);
	}
}

bool Renderable::swallowsMouse(Point mouse) {
	return !_isHidden && globalFrame().isMouseInside(mouse);
}

bool Renderable::swallowsKeyboard() {
	if (_isHidden) { return false; }
	if (swallowsKeyEvents()) { return true; }

	for (const auto child : _childRenderables) {
		if (child->swallowsKeyboard()) { return true; }
	}
	return false;
}