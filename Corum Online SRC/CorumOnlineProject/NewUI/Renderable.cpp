#include "Renderable.h"

using namespace CustomUI;


Rect Renderable::globalFrame() const {
	if (_parent == nullptr) { 
		return _frameInParent; 
	}
	const auto parentGlobalFrame = _parent->globalFrame();
	return _frameInParent.withOriginOffsetBy(parentGlobalFrame.origin);
}

Rect Renderable::boundingBoxInParent() const {
	if (_childRenderables.empty()) {
		return _frameInParent;
	}

	int minX = INT_MAX, minY = INT_MAX, maxX = INT_MIN, maxY = INT_MIN;
	
	for (auto child : _childRenderables) {
		if (child) {
			const auto frame = child->frameInParent();
			if (minX > frame.origin.x) { minX = frame.origin.x; }
			if (minY > frame.origin.y) { minY = frame.origin.y; }

			if (maxX < frame.maxX()) { maxX = frame.maxX(); }
			if (maxY < frame.maxY()) { maxY = frame.maxY(); }
		}
	}

	return Rect{ {minX, minY}, { (float)(maxX - minX), (float)(maxY - minY)} };
}

void Renderable::updateMouseState(MouseState newState) {
	const auto old = _currentMouseState;
	if (old == newState) { return; }
	_currentMouseState = newState;

	onMouseStateChange(newState, old); 
}

void Renderable::handleMouseDown(Point mouse, MouseButton button) {
	if (_isHidden) { return; }
	
	if (!globalFrame().containsPoint(mouse)) {
		updateMouseState(MouseState::none);
		return;
	}

	updateMouseState(
		button == MouseButton::left ? 
			MouseState::leftButtonPressedInside : 
			MouseState::rightButtonPressedInside
	);

	if (swallowsMouseEvents()) { return; }
	for (auto child : _childRenderables) {
		child->handleMouseDown(mouse, button);
	}
}

void Renderable::handleMouseUp(Point mouse, MouseButton button) {
	if (_isHidden) { return; }
	updateMouseState(
		globalFrame().containsPoint(mouse) ? MouseState::hovering : MouseState::none
	);

	if (swallowsMouseEvents()) { return; }
	for (auto child : _childRenderables) {
		child->handleMouseUp(mouse, button);
	}
}

void Renderable::handleMouseMove(Point mouse) {
	if (_isHidden) { return; }

	if (!swallowsMouseEvents()) {
		for (auto child : _childRenderables) {
			child->handleMouseMove(mouse);
		}
	}

	onMouseMove(mouse);

	if (_currentMouseState == MouseState::leftButtonPressedInside ||
		_currentMouseState == MouseState::rightButtonPressedInside ) {
		return; 
	}

	updateMouseState(
		globalFrame().containsPoint(mouse) ? MouseState::hovering : MouseState::none
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
	return !_isHidden && globalFrame().containsPoint(mouse);
}

bool Renderable::swallowsKeyboard() {
	if (_isHidden) { return false; }
	if (swallowsKeyEvents()) { return true; }

	for (const auto child : _childRenderables) {
		if (child->swallowsKeyboard()) { return true; }
	}
	return false;
}

void Renderable::deconstructAllChildren() {
	for (auto child : _childRenderables) {
		if (child) { delete child; }
	}
	_childRenderables.clear();
}

void Renderable::deconstructChildrenWhere(std::function<bool(Renderable*)> eligibleToDeconstruct) {
	_childRenderables.erase(std::remove_if(
		_childRenderables.begin(),
		_childRenderables.end(),
		eligibleToDeconstruct), 
		_childRenderables.end()
	);
}

void Renderable::renderWithRenderer(I4DyuchiGXRenderer* renderer, int zIndex) {
	if (_backgroundSprite.sprite) {
		_backgroundSprite.renderWith(renderer, globalFrame(), zIndex + _zIndexOffsetForce);
	}

	for (int i = 0; i < _childRenderables.size(); i++) {
		auto child = _childRenderables[i];
		if (child && !child->getHidden()) {
			child->renderWithRenderer(
				renderer, 
				zIndex + i + 1 + child->_zIndexOffsetForce
			);
		}
	}
}