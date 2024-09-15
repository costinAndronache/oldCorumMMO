#pragma once
#include "../InitGame.h"
#include "CustomUIBase.h"

namespace CustomUI {
	class Button;
	class ButtonClient 
	{
	public:
		virtual void onButtonPress(Button* button) = 0;
		virtual void onButtonPressRelease(Button* button) = 0;
	};

	class Button 
	{
	public:
		struct SpriteModel {
			IDISpriteObject* sprite;
			Size size;
			float rotation;
		};
		Button(SpriteModel spriteModel, SpriteModel pressedSpriteModel, Rect frame, ButtonClient* client);
		void renderWithRenderer(I4DyuchiGXRenderer *renderer, int order);
	private:
		SpriteModel _spriteModel;
		SpriteModel _pressedSpriteModel;
		Rect _frame;
		ButtonClient* _client;
		bool _detectedPress;
		DWORD _lastPressNotifyTime;
	};

	struct ButtonResources {
		static void initialize();
		static IDISpriteObject* downArrow;
		static IDISpriteObject* downArrowPressed;
		static Size downArrowSize;
	};
}

