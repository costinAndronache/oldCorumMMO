#pragma once
#include "CustomUIBase.h"
#include "Renderable.h"

namespace CustomUI {
	class HorizontalScalingBar: public Renderable {
	public:
		enum class Direction { leftRight, rightLeft};
		HorizontalScalingBar(SpriteModel barSprite, Rect frameInParent, Direction direction) : _barSprite(barSprite), _direction(direction) {
			_frameInParent = frameInParent;
		}

		void updateScale(float scale);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int zIndex) override;

	private:
		SpriteModel _barSprite;
		Rect _currentScaledRect;
		Direction _direction;
	};
}

