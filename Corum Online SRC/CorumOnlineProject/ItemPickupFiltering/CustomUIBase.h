#pragma once
#include "../InitGame.h"

namespace CustomUI {
	class Renderable {
	public:
		virtual void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) = 0;
	};

	struct Size { 
		int width; int height;
		VECTOR2 divideBy(Size other) const {
			VECTOR2 result = { (float)width / other.width, (float)height / other.height };
			return result;
		}
	};
	struct Point { int x; int y; };
	struct Rect {
		Point origin;
		Size size;

		int maxX() const { return origin.x + size.width; }
		int maxY() const { return origin.y + size.height; }

		bool isGlobalMouseInside() const {
			return (origin.x < g_Mouse.MousePos.x && g_Mouse.MousePos.x < maxX() &&
					origin.y < g_Mouse.MousePos.y && g_Mouse.MousePos.y < maxY());
		}
	};

	struct SpriteModel {
		IDISpriteObject* sprite;
		Size size;
		float rotation;

		static SpriteModel zero;
	};
}

