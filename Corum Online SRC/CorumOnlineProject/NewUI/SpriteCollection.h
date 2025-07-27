#pragma once
#include "CustomUIBase.h"

namespace CustomUI {
	struct SpriteCollection {
		static void initialize(I4DyuchiGXRenderer* renderer);

		static IDISpriteObject* downArrow;
		static IDISpriteObject* downArrowPressed;
		static Size downArrowSize;

		static IDISpriteObject* genericButtonBackground;
		static IDISpriteObject* genericButtonPressedBackground;
		static Size genericButtonBackgroundSize;

		static IDISpriteObject* xClose;
		static IDISpriteObject* xClosePressed;
		static Size xCloseSize;

		static SpriteModel xCloseSprite;
		static SpriteModel xClosePressedSprite;

		static IDISpriteObject* xRedBlack;
		static IDISpriteObject* xRedBlackPressed;
		static Size xRedBlackSize;
	};
}

