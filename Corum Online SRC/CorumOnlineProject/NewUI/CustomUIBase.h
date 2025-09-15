#pragma once
#include "../InitGame.h"

#include <d3d8.h>
#include <vector>

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
	struct Insets {
		int top, bottom, left, right;
	};

	struct Size { 
		float width; 
		float height;
		VECTOR2 divideBy(Size other) const {
			VECTOR2 result = { (float)width / other.width, (float)height / other.height };
			return result;
		}

		Size toCoverInsets(Insets insets) const {
			return {
				width + insets.left + insets.right,
				height + insets.top + insets.bottom
			};
		}
	};

	typedef POINT Point;
	inline Point operator+(const Point& lhs, const Point& rhs) {
		return {
			lhs.x + rhs.x, lhs.y + rhs.y
		};
	}

	struct Rect {
		Point origin;
		Size size;

		float maxX() const { return origin.x + size.width; }
		float maxY() const { return origin.y + size.height; }
		float midX() const { return origin.x + (size.width / 2); }
		float midY() const { return origin.y + (size.height / 2); }

		bool containsPoint(Point point) const {
			return (origin.x < point.x && point.x < maxX() &&
					origin.y < point.y && point.y < maxY());
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

		Rect centeredHorizontallyWith(Rect other) const {
			Rect result = {
				{ other.midX() - (size.width/2),  origin.y}, size
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

		Rect fromMaxYOrigin(int offset) const {
			Rect result = *this;
			result.origin.y = result.maxY() + offset;
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

		Rect withHeight(float height) const {
			Rect result = *this;
			result.size.height = height;
			return result;
		}

		Rect withHeightOffset(float offset) const {
			Rect result = *this;
			result.size.height += offset;
			return result;
		}

		Rect withSize(Size otherSize) const {
			Rect result = { origin, otherSize };
			return result;
		}

		Rect withWidth(float width) const {
			Rect result = *this;
			result.size.width = width;
			return result;
		}

		Rect withWidthOffset(float offset) const {
			Rect result = *this;
			result.size.width += offset;
			return result;
		}

		Rect withX(int x) const {
			Rect result = *this;
			result.origin.x = x;
			return result;
		}

		Rect withY(int y) const {
			Rect result = *this;
			result.origin.y = y;
			return result;
		}

		Rect scaled(float widthScale, float heightScale = 1.0) const {
			return Rect{ origin, { (size.width) * widthScale, (size.height) * heightScale } };
		}

		Rect withOriginOffsetBy(Point offset) const {
			return Rect{ {origin.x + offset.x, origin.y + offset.y }, size };
		}

		Rect centeredOnPoint(Point p) const {
			return Rect{
				{ p.x - (int)size.width / 2, p.y - (int)size.height / 2 },
				size
			};
		}

		Point center() const {
			return {
				origin.x + (int)size.width / 2,
				origin.y + (int)size.height / 2
			};
		}

		bool fullyContains(Rect other) const {
			return (
				(origin.x <= other.origin.x && other.origin.x <= maxX())
				&&
				(origin.y <= other.origin.y && other.origin.y <= maxY())
			);
		}

		Rect withInsets(Insets insets) const {
			auto newOrigin = origin;
			auto newSize = size; 

			newOrigin.x += insets.left;
			newSize.width -= insets.left;
			
			newSize.width -= insets.right;

			newOrigin.y += insets.top;
			newSize.height -= insets.top;

			newSize.height -= insets.bottom;

			return { newOrigin, newSize };
		}

		static Rect zero() {
			Rect result = { {0,0}, {0,0} };
			return result;
		}
	};

	struct SpriteModel {
		static SpriteModel zero;

		IDISpriteObject* sprite;
		Size naturalSize;

		void renderWith(I4DyuchiGXRenderer* renderer, Rect withinGlobalFrame, int order = 0) const {
			if (!sprite) { return; }


			VECTOR2 scale = withinGlobalFrame.size.divideBy(naturalSize);
			VECTOR2 pos{ withinGlobalFrame.origin.x, withinGlobalFrame.origin.y };

			renderer->RenderSprite(sprite, &scale, 0, &pos, NULL, 0xffffffff, order, RENDER_TYPE_DISABLE_TEX_FILTERING);
		}

		static SpriteModel from(I4DyuchiGXRenderer* renderer, 
								const char* resourceFile, 
								Rect frameInFile) {
			const auto sprite = renderer->CreateSpriteObject((char*)resourceFile,
				(DWORD)frameInFile.origin.x, (DWORD)frameInFile.origin.y,
				(DWORD)frameInFile.size.width, (DWORD)frameInFile.size.height,
				0);
			return {
				sprite,
				frameInFile.size,
			};
		}

	};

	struct Color {
		unsigned char r, g, b, a;
		
		DWORD asDXColor() const;
		Color withAlpha(unsigned char alpha) const;
		static Color fromARGB(DWORD argb);

		static Color white;
		static Color red;
		static Color green;
		static Color blue;
		static Color yellow;
		static Color magenta;
	};

	char getASCII(WPARAM wparam, LPARAM lParam); // 0 if not from keyboard
	bool safeToHandleKeyEvents();

	std::vector<std::string> strtok(const std::string& text, const char* byChars);
	inline void ltrim(std::string& s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
			return !std::isspace(ch);
		}));
	}

	// Trim from the end (in place)
	inline void rtrim(std::string& s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
			return !std::isspace(ch);
		}).base(), s.end());
	}
}

