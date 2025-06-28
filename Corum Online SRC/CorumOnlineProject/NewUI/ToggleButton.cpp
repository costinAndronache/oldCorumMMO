#include "ToggleButton.h"
using namespace CustomUI;

ToggleButton::ToggleButton(SpriteModel spriteModel, SpriteModel pressedSpriteModel, Button::LabelModel labelModel, Rect frame) : 
 _spriteModel(spriteModel), _pressedSpriteModel(pressedSpriteModel), _isToggled(false) {
	_button = registerChildRenderable<Button>([=]() { return new Button(spriteModel, pressedSpriteModel, labelModel, frame); });

	_button->onRelease([this]() {
		onButtonPressRelease(_button);
	});
}

ToggleButton::ToggleButton(SpriteModel spriteModel, SpriteModel pressedSpriteModel, Rect frame) :
	_spriteModel(spriteModel), _pressedSpriteModel(pressedSpriteModel), _isToggled(false) {
	_button = registerChildRenderable<Button>([=]() { return new Button(spriteModel, pressedSpriteModel, frame); });

	_button->onRelease([this]() {
		onButtonPressRelease(_button);
	});
}

void ToggleButton::setState(bool isToggled) {
	_isToggled = isToggled;
	adjustButtonSprites(isToggled);
}


void ToggleButton::onButtonPress(Button* button) {

}

void ToggleButton::onButtonPressRelease(Button* button) {
	_isToggled = !_isToggled;
	adjustButtonSprites(_isToggled);
	if (_stateSwitchHandler) {
		_stateSwitchHandler(_isToggled);
	}
}

void ToggleButton::adjustButtonSprites(bool isToggled) {
	if (isToggled) {
		_button->updateSpriteModelTo(_pressedSpriteModel);
		_button->updatePressedSpriteModelTo(_spriteModel);
	}
	else {
		_button->updateSpriteModelTo(_spriteModel);
		_button->updatePressedSpriteModelTo(_pressedSpriteModel);
	}
}

void ToggleButton::renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) {
	_button->renderWithRenderer(renderer, order);
}
