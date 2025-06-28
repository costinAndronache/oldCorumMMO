#pragma once
#include "../InitGame.h"
#include "CustomUIBase.h"
#include "Button.h"
#include <string>

namespace CustomUI {
	class InputField: public Renderable {
	public:
		typedef std::function<void(const char*)> InputTextUpdateHandler;

		InputField(Rect frame, SpriteModel bgSpriteModel);
		InputField(Rect frame, SpriteModel bgSpriteModel, SpriteModel clearButtonSprite, SpriteModel clearButtonPressedSprite);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order);
		bool handleKeyUp(WPARAM wparam, LPARAM lparam);
		bool handleKeyDown(WPARAM wparam, LPARAM lparam);
		
		void onTextUpdate(InputTextUpdateHandler handler) { _handler = handler; }
		const char* currentText();
		static const int maxChars = 124;
	private:
		void onButtonPress(Button* button);
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


