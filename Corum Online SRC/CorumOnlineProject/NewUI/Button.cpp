#include "Button.h"
#include "../CorumResource.h"

using namespace CustomUI;

Button::Button(SpriteModel spriteModel, SpriteModel pressedSpriteModel, Rect frame) :
			   _spriteModel(spriteModel), _pressedSpriteModel(pressedSpriteModel) {
	_frame = frame;
	_label = NULL;
}

Button::Button(SpriteModel spriteModel, SpriteModel pressedSpriteModel, LabelModel labelModel, Rect frame): 
	_spriteModel(spriteModel), _pressedSpriteModel(pressedSpriteModel) {
	_frame = frame;
	Rect labelFrame{ frame.origin, SingleLineLabel::fittedSize(strlen(labelModel.text)) };
	labelFrame = labelFrame.centeredWith(frame);
	_label = new SingleLineLabel(labelFrame, labelModel.appearance, labelModel.text);
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
	VECTOR2 pos = { _frame.origin.x, _frame.origin.y };

	switch (_currentMouseState) {
	case MouseState::none:
	case MouseState::hovering:
		_spriteModel.renderWith(renderer, _frame);
		break;
	case MouseState::pressedInside:
		_pressedSpriteModel.renderWith(renderer, _frame);

	}

	if (_label) {
		_label->renderWithRenderer(renderer, order + 1);
	}
}

