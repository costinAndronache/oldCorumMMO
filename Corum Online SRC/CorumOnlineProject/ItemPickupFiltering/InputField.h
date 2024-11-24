#pragma once
#include "../InitGame.h"
#include "CustomUIBase.h"
#include "Button.h"
#include <string>

namespace CustomUI {
	class InputField;

	class InputFieldClient {
	public:
		virtual void onInputFieldTextChange(InputField* inputField, const char* text) = 0;
	};

	class InputField: private ButtonClient {
	public:
		InputField(Rect frame, SpriteModel bgSpriteModel, InputFieldClient* client);
		InputField(Rect frame, SpriteModel bgSpriteModel, SpriteModel clearButtonSprite, SpriteModel clearButtonPressedSprite, InputFieldClient* client);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order);
		bool handleKeyUp(WPARAM wparam, LPARAM lparam);
		bool handleKeyDown(WPARAM wparam, LPARAM lparam);

		const char* currentText();
		static const int maxChars = 124;
	private:
		void onButtonPress(Button* button);
		void onButtonPressRelease(Button* button);

		void notifyClient();

		std::string _buffer;
		Rect _frame;
		SpriteModel _bgSpriteModel;
		InputFieldClient* _client;
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


