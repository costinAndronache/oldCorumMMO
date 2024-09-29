#include "ToggleButton.h"
using namespace CustomUI;

ToggleButton::ToggleButton(SpriteModel spriteModel, SpriteModel pressedSpriteModel, Button::LabelModel labelModel, Rect frame, ToggleButtonClient* client) : 
 _spriteModel(spriteModel), _pressedSpriteModel(pressedSpriteModel), _client(client), _isToggled(false) {
	_button = new Button(spriteModel, pressedSpriteModel, labelModel, frame, this);
}

ToggleButton::ToggleButton(SpriteModel spriteModel, SpriteModel pressedSpriteModel, Rect frame, ToggleButtonClient* client) :
	_spriteModel(spriteModel), _pressedSpriteModel(pressedSpriteModel), _client(client), _isToggled(false) {
	_button = new Button(spriteModel, pressedSpriteModel, frame, this);
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
	if (_client) {
		_client->toggleButtonDidSwitchState(this, _isToggled);
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
