#pragma once
#include "../InitGame.h"

namespace CustomUI {
	struct Size { int width; int height; };
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
}

