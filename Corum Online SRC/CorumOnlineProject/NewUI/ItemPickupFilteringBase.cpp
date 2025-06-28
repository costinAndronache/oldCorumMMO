#include "ItemPickupFilteringBase.h"
#include "CustomUIBase.h"

using namespace ItemPickupFiltering;

#define __ASCII_CODE___KEY_SEE_ALL_DROPPED_ITEMS '`'
#define __ASCII_CODE___KEY_OPEN_ITEM_FILTERING 'u'
#define __ASCII_CODE___INCREASE_FPS '>'
#define __ASCII_CODE___DECREASE_FPS '<'

ActionCode ItemPickupFiltering::actionCodeFromKeyEvent(WPARAM wParam, LPARAM lParam) {
	switch (tolower(CustomUI::getASCII(wParam, lParam))) {
	case __ASCII_CODE___KEY_OPEN_ITEM_FILTERING:
		return ActionCode::ActionCodePickupFiltering;
		break;
	case __ASCII_CODE___KEY_SEE_ALL_DROPPED_ITEMS:
		return ActionCode::ActionCodeDroppedItemsTooltipRendering;
		break;
	case __ASCII_CODE___INCREASE_FPS:
		return ActionCode::ActionCodeIncreaseFPS;
		break;
	case __ASCII_CODE___DECREASE_FPS:
		return ActionCode::ActionCodeDecreaseFPS;
		break;
	default:
		return ActionCode::ActionCodeUnknown;
	}
}
