#pragma once
#include "../InitGame.h"
#include "CustomUIBase.h"
#include "Label.h"

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
		struct LabelModel {
			const char* text;
			SingleLineLabel::Appearance appearance;
		};
		Button(SpriteModel spriteModel, SpriteModel pressedSpriteModel, Rect frame, ButtonClient* client);
		Button(SpriteModel spriteModel, SpriteModel pressedSpriteModel, LabelModel labelModel, Rect frame, ButtonClient* client);
		void renderWithRenderer(I4DyuchiGXRenderer *renderer, int order);

		void updateSpriteModelTo(SpriteModel newModel);
		void updatePressedSpriteModelTo(SpriteModel newPressedStateSpriteModel);
	private:
		SpriteModel _spriteModel;
		SpriteModel _pressedSpriteModel;
		Rect _frame;
		ButtonClient* _client;
		bool _detectedPress;
		DWORD _lastPressNotifyTime;
		SingleLineLabel* _label;
	};

	struct ButtonResources {
		static void initialize();

		static IDISpriteObject* downArrow;
		static IDISpriteObject* downArrowPressed;
		static Size downArrowSize;

		static IDISpriteObject* genericBackground;
		static IDISpriteObject* genericPressedBackground;
		static Size genericBackgroundSize;

		static IDISpriteObject* xClose;
		static IDISpriteObject* xClosePressed;
		static Size xCloseSize;

		static IDISpriteObject* xRedBlack;
		static IDISpriteObject* xRedBlackPressed;
		static Size xRedBlackSize;
	};
}

