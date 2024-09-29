#pragma once
#include "../InitGame.h"
#include "CustomUIBase.h"
#include <string>

namespace CustomUI {
	class InputField;

	class InputFieldClient {
	public:
		virtual void onInputFieldTextChange(InputField* inputField, const char* text) = 0;
	};

	class InputField {
	public:
		InputField(Rect frame, SpriteModel bgSpriteModel, InputFieldClient* client);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order);
		bool handleKeyUp(WPARAM wparam, LPARAM lparam);
		bool handleKeyDown(WPARAM wparam, LPARAM lparam);

		const char* currentText();
		static const int maxChars = 124;
	private:
		void notifyClient();

		std::string _buffer;
		Rect _frame;
		SpriteModel _bgSpriteModel;
		InputFieldClient* _client;
		bool _isActive;
		bool _caretStateON;
		DWORD _lastCaretUpdateTime;
	};

	struct InputFieldResources {
		static void initialize();
		static SpriteModel bgSpriteModel;
	};
}


