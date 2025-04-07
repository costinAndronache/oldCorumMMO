#pragma once
#include <Windows.h>

namespace ItemPickupFiltering {
	enum ActionCode {
		ActionCodeDroppedItemsTooltipRendering,
		ActionCodePickupFiltering,
		ActionCodeIncreaseFPS,
		ActionCodeDecreaseFPS,
		ActionCodeUnknown
	};

	ActionCode actionCodeFromKeyEvent(WPARAM wParam, LPARAM lParam);
}


