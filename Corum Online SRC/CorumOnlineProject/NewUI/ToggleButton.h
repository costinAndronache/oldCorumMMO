#pragma once
#include "CustomUIBase.h"
#include "Button.h"

namespace CustomUI {
	class ToggleButton;

	class ToggleButton: public Renderable {
	public:
		typedef std::function<void(bool)> StateSwitchHandler;

		ToggleButton(SpriteModel spriteModel, SpriteModel pressedSpriteModel, Button::LabelModel labelModel, Rect frameInParent);
		ToggleButton(SpriteModel spriteModel, SpriteModel pressedSpriteModel, Rect frameInParent);
		
		void setState(bool isToggled);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order);

		void onStateSwitch(StateSwitchHandler handler) { _stateSwitchHandler = handler; }
	private:
		void onButtonPress(Button* button);
		void onButtonPressRelease(Button* button);

		void adjustButtonSprites(bool isToggled);

		Button* _button;
		SpriteModel _spriteModel, _pressedSpriteModel;
		bool _isToggled;

		StateSwitchHandler _stateSwitchHandler;
	};
}


