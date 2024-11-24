#pragma once

#include "../Base.h"
#include "../Renderer.h"

namespace CustomUiKit {
	class PagedTableWindow {
	private:
		const Renderer* renderer;
		Rect frame;

	public:
		PagedTableWindow(Rect initialFrame, const Renderer* renderer);
		void repaintInWindow();
	};
}

