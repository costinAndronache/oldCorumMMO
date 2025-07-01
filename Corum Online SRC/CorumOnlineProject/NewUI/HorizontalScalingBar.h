#pragma once
#include "CustomUIBase.h"
#include "Renderable.h"

namespace CustomUI {
	class HorizontalScalingBar: public Renderable {
	public:
		enum class Direction { leftRight, rightLeft};
		HorizontalScalingBar(SpriteModel barSprite, Rect frame, Direction direction) : _barSprite(barSprite), _frame(frame), _direction(direction) {}

		void updateScale(float scale);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int zIndex) override;

	private:
		SpriteModel _barSprite;
		Rect _frame;
		Rect _currentScaledRect;
		Direction _direction;
	};
}

