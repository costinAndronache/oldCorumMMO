#pragma once
#include "CustomUIBase.h"
#include "Button.h"

namespace CustomUI {
	class ToggleButton;

	class ToggleButton: public Renderable {
	public:
		typedef std::function<void(bool)> StateSwitchHandler;

		ToggleButton(Button::Sprites _sprites, Button::LabelModel labelModel, Rect frameInParent);
		ToggleButton(Button::Sprites _sprites, Rect frameInParent);
		
		void setState(bool isToggled);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order);

		void onStateSwitch(StateSwitchHandler handler) { _stateSwitchHandler = handler; }
	private:
		void onButtonPress(std::shared_ptr<Button> button);
		void onButtonPressRelease(std::shared_ptr<Button> button);

		void adjustButtonSprites(bool isToggled);

		std::shared_ptr<Button> _button;
		Button::Sprites _sprites;

		bool _isToggled;

		StateSwitchHandler _stateSwitchHandler;
	};
}


