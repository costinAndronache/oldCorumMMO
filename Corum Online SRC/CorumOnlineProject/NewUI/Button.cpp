#include "Button.h"
#include "../CorumResource.h"

using namespace CustomUI;

Button::Sprites Button::Sprites::allZero{
	SpriteModel::zero,
	SpriteModel::zero,
	SpriteModel::zero
};

Button::Button(Sprites sprites, Rect frameInParent): _sprites(sprites) {
	_frameInParent = frameInParent;
	_label = NULL;
	_longPressTimerLEFT = std::make_shared<Timer>(RunLoop::mainRunLoop());

}

Button::Button(Sprites sprites, LabelModel labelModel, Rect frameInParent): _sprites(sprites) {
	_frameInParent = frameInParent;
	_longPressTimerLEFT = std::make_shared<Timer>(RunLoop::mainRunLoop());

	auto labelFrame = bounds();

	_label = registerChildRenderable<SingleLineLabel>([=]() {
		return std::make_shared<SingleLineLabel>(labelFrame, labelModel.appearance, labelModel.text);
	});
	_label->updateRenderingModeToCentered();

}

void Button::updateSpriteModelTo(Sprites newModel) {
	_sprites = newModel;
}

void Button::processKeyDown(WPARAM wparam, LPARAM lparam) {
	if(_currentMouseState == MouseState::hovering && _onKeyDown) {
		_onKeyDown(wparam, lparam);
	}
}

void Button::onMouseStateChange(MouseState newState, MouseState oldState, Point) {
	const auto frame = globalFrame();

	if (newState == MouseState::leftButtonPressedInside) {
		if(_onPressLEFT) { _onPressLEFT(); }
		if (_onLongPressDetectedLEFT) {
			_longPressTimerLEFT->launchAfter(300, false, [=]() {
				if (_currentMouseState == MouseState::leftButtonPressedInside) {
					_onLongPressDetectedLEFT();
				}
			});
		}
	}

	if (newState == MouseState::rightButtonPressedInside && _onPressRIGHT) {
		_onPressRIGHT();
	}

	if (oldState == MouseState::leftButtonPressedInside &&
		newState != MouseState::rightButtonPressedInside &&
		_onReleaseLEFT) { 
		_onReleaseLEFT(); 
	}

	if (oldState == MouseState::rightButtonPressedInside &&
		newState != MouseState::leftButtonPressedInside && 
		_onReleaseRIGHT) {
		_onReleaseRIGHT();
	}

}

void Button::renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) {
	const auto frame = globalFrame();

	switch (_currentMouseState) {
	case MouseState::none:
		_sprites.normal.renderWith(renderer, frame, order);
	case MouseState::hovering:
		_sprites.hovered.renderWith(renderer, frame, order);
		if (!_sprites.hovered.sprite) {
			_sprites.normal.renderWith(renderer, frame, order);
		}
		break;
	case MouseState::leftButtonPressedInside:
	case MouseState::rightButtonPressedInside:
		_sprites.pressed.renderWith(renderer, frame, order);
		if (!_sprites.pressed.sprite) {
			_sprites.normal.renderWith(renderer, frame, order);
		}
	}

	if (_label) {
		_label->renderWithRenderer(renderer, order + 1);
	}
}

