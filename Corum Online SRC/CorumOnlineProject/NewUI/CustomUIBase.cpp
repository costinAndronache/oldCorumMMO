#include "CustomUIBase.h"
#include "../QuantityWnd.h"
#include "../DungeonProcess.h"

using namespace CustomUI;

SpriteModel SpriteModel::zero = { NULL, { 0, 0 }, 0 };

char CustomUI::getASCII(WPARAM wparam, LPARAM lParam) {
	BYTE keyState[256];

	GetKeyboardState(keyState);
	WORD wascii[3];
	int scanCode = (lParam >> 16) & 0xff;
	if (ToAscii(wparam, scanCode, keyState, wascii, 0) != 1) {
		return 0;
	}

	return wascii[0];
}

bool CustomUI::safeToHandleKeyEvents() {
	return (!g_pGVDungeon->bChatMode && !CQuantityWnd::GetInstance()->GetActive());
}