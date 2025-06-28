#include "Button.h"
#include "../CorumResource.h"

using namespace CustomUI;

Button::Button(SpriteModel spriteModel, SpriteModel pressedSpriteModel, Rect frameInParent) :
			   _spriteModel(spriteModel), _pressedSpriteModel(pressedSpriteModel) {
	_frameInParent = frameInParent;
	_label = NULL;
}

Button::Button(SpriteModel spriteModel, SpriteModel pressedSpriteModel, LabelModel labelModel, Rect frameInParent): 
	_spriteModel(spriteModel), _pressedSpriteModel(pressedSpriteModel) {
	_frameInParent = frameInParent;

	Rect labelFrame{ {0, 0}, SingleLineLabel::fittedSize(strlen(labelModel.text)) };
	labelFrame = labelFrame.centeredWith(_frameInParent);
	_label = registerChildRenderable<SingleLineLabel>([=]() {
		return new SingleLineLabel(labelFrame, labelModel.appearance, labelModel.text);
	});
}

void Button::updateSpriteModelTo(SpriteModel newModel) {
	_spriteModel = newModel;
}

void Button::updatePressedSpriteModelTo(SpriteModel newPressedStateSpriteModel) {
	_pressedSpriteModel = newPressedStateSpriteModel;
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
	case MouseState::hovering:
		_spriteModel.renderWith(renderer, frame, order);
		break;
	case MouseState::pressedInside:
		_pressedSpriteModel.renderWith(renderer, frame, order);

	}

	if (_label) {
		_label->renderWithRenderer(renderer, order + 1);
	}
}

