#pragma once
#include "stdafx.h"
#include <GdiPlus.h>

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

	class PagedTableWindow {
	private:
		HWND windowHandle;
		Gdiplus::Graphics* graphics;
		Rect frame;

	public:
		PagedTableWindow(Rect initialFrame, HWND windowHandle);
		void repaintInWindow();
	};
}

