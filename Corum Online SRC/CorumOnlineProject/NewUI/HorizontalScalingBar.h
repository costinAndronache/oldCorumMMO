#pragma once
#include "CustomUIBase.h"

namespace CustomUI {
	class HorizontalScalingBar {
	public:
		enum class Direction { leftRight, rightLeft};
		HorizontalScalingBar(SpriteModel barSprite, Rect frame, Direction direction) : _barSprite(barSprite), _frame(frame), _direction(direction) {}

		void updateScale(float scale);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer);

	private:
		SpriteModel _barSprite;
		Rect _frame;
		Rect _currentScaledRect;
		Direction _direction;
	};
}

