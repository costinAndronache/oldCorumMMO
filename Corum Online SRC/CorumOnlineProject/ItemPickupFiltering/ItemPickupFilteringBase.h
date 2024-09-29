#pragma once
#include <Windows.h>

namespace ItemPickupFiltering {
	enum ActionCode {
		ActionCodeDroppedItemsTooltipRendering,
		ActionCodePickupFiltering,
		ActionCodeUnknown
	};

	ActionCode actionCodeFromKeyEvent(WPARAM wParam, LPARAM lParam);
}


