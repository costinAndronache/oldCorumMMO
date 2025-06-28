#pragma once
#include "CustomUIBase.h"

namespace CustomUI {
	struct SpriteCollection {
		static void initialize(I4DyuchiGXRenderer* renderer);

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

