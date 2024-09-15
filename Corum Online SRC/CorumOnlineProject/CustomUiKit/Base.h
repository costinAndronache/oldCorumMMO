#pragma once

namespace CustomUiKit {
	struct Point {
		int x, y;
	};

	struct Size {
		int width, height;
	};

	struct Rect {
		Point origin;
		Size size;
	};

	struct Color {
		__int8 a, r, g, b;
	};
}