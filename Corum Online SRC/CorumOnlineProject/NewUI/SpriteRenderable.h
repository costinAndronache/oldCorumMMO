#pragma once
#include "Renderable.h"

namespace CustomUI {
	class SpriteRenderable: public Renderable {
	public:
		SpriteRenderable(Rect frameInParent, SpriteModel sprite): _sprite(sprite){
			_frameInParent = frameInParent;
		}

		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int zIndex) override {
			const auto frame = globalFrame();
			_sprite.renderWith(renderer, globalFrame(), zIndex);
		}

		void updateSprite(SpriteModel newSprite) { _sprite = newSprite; }

	private:
		SpriteModel _sprite;
	};
}

