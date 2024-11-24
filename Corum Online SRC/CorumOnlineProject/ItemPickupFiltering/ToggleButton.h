#pragma once
#include "CustomUIBase.h"
#include "Button.h"

namespace CustomUI {
	class ToggleButton;

	class ToggleButtonClient {
	public:
		virtual void toggleButtonDidSwitchState(ToggleButton* button, bool isOn) = 0;
	};

	class ToggleButton: public Renderable, public ButtonClient {
	public:
		ToggleButton(SpriteModel spriteModel, SpriteModel pressedSpriteModel, Button::LabelModel labelModel, Rect frame, ToggleButtonClient* client);
		ToggleButton(SpriteModel spriteModel, SpriteModel pressedSpriteModel, Rect frame, ToggleButtonClient* client);
		
		void setState(bool isToggled);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order);
	private:
		void onButtonPress(Button* button) override;
		void onButtonPressRelease(Button* button) override;

		void adjustButtonSprites(bool isToggled);

		Button* _button;
		ToggleButtonClient* _client;
		SpriteModel _spriteModel, _pressedSpriteModel;
		bool _isToggled;
	};
}


