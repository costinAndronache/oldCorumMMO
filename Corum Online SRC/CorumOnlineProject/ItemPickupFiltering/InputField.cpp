#include "InputField.h"
#include <d3d8.h>
using namespace CustomUI;

#define WHITE(alpha)         D3DCOLOR_ARGB(alpha, 255, 255, 255)

SpriteModel InputFieldResources::bgSpriteModel = { NULL, {54, 16}, 0 };
// menu1 tga, 160,0 54x16

void InputFieldResources::initialize() {
	if (bgSpriteModel.sprite == NULL) {
		char* resourceFile = GetFile("menu_1.tga", DATA_TYPE_UI);
		bgSpriteModel.sprite = g_pRenderer->CreateSpriteObject(
			resourceFile,
			160, 0,
			bgSpriteModel.size.width, bgSpriteModel.size.height,
			0
		);
	}
}

InputField::InputField(Rect frame, SpriteModel bgSpriteModel, InputFieldClient* client):
_frame(frame), _client(client), _bgSpriteModel(bgSpriteModel) {
	_isActive = false;
	_caretStateON = false;
	_lastCaretUpdateTime = 0;
	InputFieldResources::initialize();
	_buffer = "";
}

void InputField::renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) {
	VECTOR2 pos = { _frame.origin.x, _frame.origin.y };
	VECTOR2 scale = _frame.size.divideBy(_bgSpriteModel.size);

	renderer->RenderSprite(_bgSpriteModel.sprite,
		&scale, 0, &pos,
		NULL, 0xffffffff,
		order, RENDER_TYPE_DISABLE_TEX_FILTERING);

	char renderedText[maxChars + 1];
	int count = _buffer.size();
	strcpy(renderedText, _buffer.c_str());

	if (_caretStateON) {
		strcat(renderedText, "|");
		count += 1;
	}

	RECT rt = { _frame.origin.x + 5, _frame.origin.y + 5, _frame.maxX(), _frame.maxY() };
	g_pGeometry->RenderFont(GetFont(), 
							(TCHAR*)renderedText, count, 
							&rt, WHITE(255), CHAR_CODE_TYPE_ASCII, 
							order + 1, 0
	);

	if (g_Mouse.bLDown) {
		if (_frame.isGlobalMouseInside()) {
			_isActive = true;
			_caretStateON = true;
		}
		else {
			_isActive = false;
			_caretStateON = false;
		}
	}

	if (_isActive) {
		DWORD now = timeGetTime();
		if (now - _lastCaretUpdateTime > 1200) {
			_lastCaretUpdateTime = now;
			_caretStateON = !_caretStateON;
		}
	}
}

bool InputField::handleKeyDown(WPARAM wparam, LPARAM lparam) {
	return _isActive;
} 

void InputField::notifyClient() {
	if (_client) {
		const char* text = _buffer.c_str();
		_client->onInputFieldTextChange(this, text);
	}
}

bool InputField::handleKeyUp(WPARAM wparam, LPARAM lParam) {
	BYTE keyState[256];
	if (!_isActive) {
		return false;
	}

	if (wparam == VK_BACK) {
		if (!_buffer.empty()) {
			_buffer.erase(_buffer.end() - 1);
			notifyClient();
		}
		return true;
	}

	GetKeyboardState(keyState);
	WORD wascii[3];
	int scanCode = (lParam >> 16) & 0xff;
	if (ToAscii(wparam, scanCode, keyState, wascii, 0) != 1) {
		return true;
	}

	char key = wascii[0];

	if (('a' <= key && key <= 'z') ||
		('A' <= key && key <= 'Z')) {
		char str[2] = { key, '\0' };

		if (_buffer.size() < maxChars) {
			_buffer.append(str);
			notifyClient();
		}
	}

	return true;
}