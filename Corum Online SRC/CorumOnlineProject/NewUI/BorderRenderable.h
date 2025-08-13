#pragma once

#include "Renderable.h"
namespace CustomUI {
	class BorderRenderable: public CustomUI::Renderable {
	public:
		struct BorderLineSprites {
			SpriteModel horizontal;
			SpriteModel vertical;
		};

		BorderRenderable(Rect frameInParent);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int zIndex) override;
		void updateSingleBorderLine(SpriteModel borderLineSprite);
		void updateBorderLines(BorderLineSprites sprites);
		void updateThickness(float thickness);

	private:
		SpriteModel _horizontalLine, _verticalLine;
		float _thickness;
	};
}

