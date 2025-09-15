#include "ToggleButton.h"
using namespace CustomUI;

ToggleButton::ToggleButton(Button::Sprites sprites, Button::LabelModel labelModel, Rect frameInParent): _sprites(sprites), _isToggled(false) {
	_frameInParent = frameInParent;
	_button = registerChildRenderable<Button>([=]() { 
		return std::make_shared<Button>(sprites, labelModel, bounds()); 
	});

	_button->onClickEndLEFT([this]() {
		onButtonPressRelease(_button);
	});
}

ToggleButton::ToggleButton(Button::Sprites sprites, Rect frameInParent) :
	_sprites(sprites), _isToggled(false) {
	_frameInParent = frameInParent;
	_button = registerChildRenderable<Button>([=]() { 
		return std::make_shared<Button>(sprites, bounds()); 
	});

	_button->onClickEndLEFT([this]() {
		onButtonPressRelease(_button);
	});
}

void ToggleButton::setState(bool isToggled) {
	_isToggled = isToggled;
	adjustButtonSprites(isToggled);
}


void ToggleButton::onButtonPress(std::shared_ptr<Button> button) {

}

void ToggleButton::onButtonPressRelease(std::shared_ptr<Button> button) {
	_isToggled = !_isToggled;
	adjustButtonSprites(_isToggled);
	if (_stateSwitchHandler) {
		_stateSwitchHandler(_isToggled);
	}
}

void ToggleButton::adjustButtonSprites(bool isToggled) {
	if (isToggled) {
		_button->updateSpriteModelTo({
			_sprites.pressed,
			SpriteModel::zero,
			_sprites.normal
		});
	}
	else {
		_button->updateSpriteModelTo(_sprites);
	}
}

void ToggleButton::renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) {
	_button->renderWithRenderer(renderer, order);
}
