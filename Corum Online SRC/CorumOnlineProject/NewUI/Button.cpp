#include "Button.h"
#include "../CorumResource.h"

using namespace CustomUI;

Button::Button(Sprites sprites, Rect frameInParent): _sprites(sprites) {
	_frameInParent = frameInParent;
	_label = NULL;
}

Button::Button(Sprites sprites, LabelModel labelModel, Rect frameInParent): _sprites(sprites) {
	_frameInParent = frameInParent;

	Rect labelFrame{ {0, 0}, SingleLineLabel::fittedSize(strlen(labelModel.text)) };
	labelFrame = labelFrame.centeredWith(_frameInParent);
	_label = registerChildRenderable<SingleLineLabel>([=]() {
		return new SingleLineLabel(labelFrame, labelModel.appearance, labelModel.text);
	});
}

void Button::updateSpriteModelTo(Sprites newModel) {
	_sprites = newModel;
}

void Button::onMouseStateChange(MouseState newState, MouseState oldState) {
	if (newState == MouseState::pressedInside && _onPress) { 
		_onPress();  
	}

	if (oldState == MouseState::pressedInside && _onRelease) { 
		_onRelease(); 
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
	case MouseState::pressedInside:
		_sprites.pressed.renderWith(renderer, frame, order);
	}

	if (_label) {
		_label->renderWithRenderer(renderer, order + 1);
	}
}

