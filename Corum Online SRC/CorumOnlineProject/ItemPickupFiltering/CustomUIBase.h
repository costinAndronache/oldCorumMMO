#pragma once
#include "../InitGame.h"

#include <d3d8.h>

#define WHITE(alpha)         D3DCOLOR_ARGB(alpha, 255, 255, 255)
#define BLACK(alpha)         D3DCOLOR_ARGB(alpha, 0, 0, 0)
#define RED(alpha)           D3DCOLOR_ARGB(alpha, 255, 0, 0)
#define GREEN(alpha)         D3DCOLOR_ARGB(alpha, 0, 128, 0)
#define LAWNGREEN(alpha)     D3DCOLOR_ARGB(alpha, 124, 252, 0)
#define BLUE(alpha)          D3DCOLOR_ARGB(alpha, 0, 200, 255)
#define DEEPSKYBLUE(alpha)   D3DCOLOR_ARGB(alpha, 0, 191, 255)
#define SKYBLUE(alpha)       D3DCOLOR_ARGB(alpha, 0, 122, 204)
#define YELLOW(alpha)        D3DCOLOR_ARGB(alpha, 255, 255, 0)
#define ORANGE(alpha)        D3DCOLOR_ARGB(alpha, 255, 165, 0)
#define DARKORANGE(alpha)    D3DCOLOR_ARGB(alpha, 255, 140, 0)
#define PURPLE(alpha)        D3DCOLOR_ARGB(alpha, 125, 0, 255)
#define CYAN(alpha)          D3DCOLOR_ARGB(alpha, 0, 255, 255)
#define PINK(alpha)          D3DCOLOR_ARGB(alpha, 255, 20, 147)
#define GRAY(alpha)          D3DCOLOR_ARGB(alpha, 128, 128, 128)
#define DARKGRAY(alpha)      D3DCOLOR_ARGB(alpha, 73, 73, 73)
#define DARKERGRAY(alpha)    D3DCOLOR_ARGB(alpha, 31, 31, 31)

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
		int midX() const { return origin.x + (size.width / 2); }
		int midY() const { return origin.y + (size.height / 2); }

		bool isGlobalMouseInside() const {
			return (origin.x < g_Mouse.MousePos.x && g_Mouse.MousePos.x < maxX() &&
					origin.y < g_Mouse.MousePos.y && g_Mouse.MousePos.y < maxY());
		}

		Rect centeredWith(Rect other) const {
			Rect result = {
				{ other.midX() - (size.width/2), other.midY() - (size.height/2) },
				size
			};
			return result;
		}

		Rect centeredVerticallyWith(Rect other) const {
			Rect result = {
				{ origin.x,  other.midY() - (size.height / 2)}, size
			};
			return result;
		}

		Rect padded(int padding) const {
			Rect result = {
				{ origin.x + padding, origin.y + padding },
				{ size.width - padding, size.height - padding }
			};

			return result;
		}

		Rect offsetX(int offset) const {
			Rect result = *this;
			result.origin.x += offset;
			return result;
		}

		Rect fromMaxXOrigin(int offset) const {
			Rect result = *this;
			result.origin.x = result.maxX() + offset;
			return result;
		}

		Rect horizontallyAlignedWith(Rect other) const {
			Rect result = { { other.origin.x, origin.y }, {other.size.width, size.height} };
			return result;
		}

		Rect positionedBelow(Rect other, int offset = 0) const {
			Rect result = { {origin.x, other.maxY() + offset}, size };
			return result;
		}

		Rect withHeight(int height) const {
			Rect result = *this;
			result.size.height = height;
			return result;
		}

		Rect withSize(Size otherSize) const {
			Rect result = { origin, otherSize };
			return result;
		}

		Rect withWidth(int width) const {
			Rect result = *this;
			result.size.width = width;
			return result;
		}

		Rect scaled(float xScale, float yScale = 1.0) const {
			Rect result = *this;
			result.size.width = float(result.size.width) * xScale;
			result.size.height = float(result.size.height) * yScale;
			return result;
		}

		static Rect zero() {
			Rect result = { {0,0}, {0,0} };
			return result;
		}
	};

	struct SpriteModel {
		IDISpriteObject* sprite;
		Size size;
		float rotation;

		static SpriteModel zero;
	};

	struct Color {
		unsigned char a, r, g, b;
		DWORD asDXColor() {
			return D3DCOLOR_ARGB(a, r, g, b);
		}
	};

	char getASCII(WPARAM wparam, LPARAM lParam); // 0 if not from keyboard
	bool safeToHandleKeyEvents();
}

