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
}

Button::Button(Sprites sprites, LabelModel labelModel, Rect frameInParent): _sprites(sprites) {
	_frameInParent = frameInParent;
	_longPressTimerLEFT = new Timer(WorkQueue::mainThreadQueue());

	Rect labelFrame{ {0, 0}, SingleLineLabel::fittedSize(strlen(labelModel.text)) };
	labelFrame = labelFrame.centeredWith(bounds());
	_label = registerChildRenderable<SingleLineLabel>([=]() {
		return new SingleLineLabel(labelFrame, labelModel.appearance, labelModel.text);
	});
}

void Button::updateSpriteModelTo(Sprites newModel) {
	_sprites = newModel;
}

void Button::onMouseStateChange(MouseState newState, MouseState oldState) {
	const auto frame = globalFrame();

	if (newState == MouseState::leftButtonPressedInside) {
		if(_onPressLEFT) { _onPressLEFT(); }
		if (_onLongPressDetectedLEFT) {
			_longPressTimerLEFT->launchAfter(1500, false, [=]() {
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

