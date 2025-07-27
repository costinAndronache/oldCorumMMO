#pragma once
#include "../InitGame.h"
#include "CustomUIBase.h"
#include "Button.h"
#include <string>

namespace CustomUI {
	class InputField: public Renderable {
	public:
		typedef std::function<void(const char*)> InputTextUpdateHandler;

		InputField(Rect frameInParent, SpriteModel bgSpriteModel);
		InputField(Rect frameInParent, SpriteModel bgSpriteModel, Button::Sprites clearButtonSprites);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order);

		void onTextUpdate(InputTextUpdateHandler handler) { _handler = handler; }
		const char* currentText();
		static const int maxChars = 124;

	protected:
		void onMouseStateChange(MouseState newState, MouseState oldState) override;
		void processKeyUp(WPARAM wparam, LPARAM lparam) override;
		bool swallowsKeyEvents() override { return _isActive; }
		bool swallowsMouseEvents() override;
	private:
		void onButtonPressRelease(Button* button);
		void notifyClient();

		std::string _buffer;
		SpriteModel _bgSpriteModel;
		InputTextUpdateHandler _handler;
		bool _isActive;
		bool _caretStateON;
		DWORD _lastCaretUpdateTime;
		Button* _clearButton;
	};

	struct InputFieldResources {
		static void initialize();
		static SpriteModel bgSpriteModel;
	};
}


