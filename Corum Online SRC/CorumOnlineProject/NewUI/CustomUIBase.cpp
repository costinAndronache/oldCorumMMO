#include "CustomUIBase.h"
#include "../QuantityWnd.h"
#include "../DungeonProcess.h"

using namespace CustomUI;

DWORD Color::asDXColor() const {
	DWORD result = D3DCOLOR_ARGB((int)a, (int)r, (int)g, (int)b);
	return result;
}

Color Color::withAlpha(unsigned char alpha) const {
	return { r, g, b, alpha };
}

Color Color::fromARGB(DWORD argb) {
	unsigned char* charv = (unsigned char*)&argb;
	return Color{ charv[2], charv[1], charv[0] , charv[3] };
}

SpriteModel SpriteModel::zero = { NULL, { 0, 0 } };
Color Color::white = { 255, 255, 255, 255 };
Color Color::red = { 255, 0, 0, 255 };
Color Color::green = { 0, 255, 0, 255 };
Color Color::blue = { 0, 0, 255, 255 };
Color Color::yellow = { 255, 255, 0, 255 };
Color Color::magenta = { 255, 0, 255, 255 };

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

std::vector<std::string> CustomUI::strtok(const std::string& text, const char* byChars) {
	static char copy[255] = { 0 };

	memset(copy, 0, 255);
	strncpy(copy, text.c_str(), min(text.size(), 254));

	std::vector<std::string> result;

	char* next = ::strtok(copy, byChars);
	while (next != nullptr) {
		result.push_back(std::string(next));
		next = ::strtok(nullptr, byChars);
	}
	return result;
}