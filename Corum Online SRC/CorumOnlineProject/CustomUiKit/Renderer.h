#pragma once
#include "Base.h"

namespace CustomUiKit {
	class Renderer {
	public:
		virtual void fillFrame(Rect frame, Color color) const = 0;
		virtual void drawLine(Point from, Point to, Color color) const = 0;
		virtual void drawTextParagraph(const char* text, Rect box) const = 0;
	};
}


