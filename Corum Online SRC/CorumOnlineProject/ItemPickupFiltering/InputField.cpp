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
	_buffer = "TEST";
}

void InputField::renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) {
	VECTOR2 pos = { _frame.origin.x, _frame.origin.y };
	VECTOR2 scale = _frame.size.divideBy(_bgSpriteModel.size);

	renderer->RenderSprite(_bgSpriteModel.sprite,
		&scale, 0, &pos,
		NULL, 0xffffffff,
		order, RENDER_TYPE_DISABLE_TEX_FILTERING);

	RECT rt = { _frame.origin.x + 5, _frame.origin.y + 5, _frame.maxX(), _frame.maxY() };
	g_pGeometry->RenderFont(GetFont(), 
							(TCHAR*)_buffer.c_str(), _buffer.size(), 
							&rt, WHITE(255), CHAR_CODE_TYPE_ASCII, 
							order + 1, 0
	);

	if (g_Mouse.bLDown) {
		if (_frame.isGlobalMouseInside()) {
			_isActive = !_isActive;
		}
		else {
			_isActive = false;
			if (_caretStateON) {
				_buffer.erase(_buffer.end() - 1);
				_caretStateON = false;
			}
		}
	}

	if (_isActive) {
		DWORD now = timeGetTime();
		if (now - _lastCaretUpdateTime > 1200) {
			_lastCaretUpdateTime = now;
			_caretStateON = !_caretStateON;

			if (_caretStateON) {
				_buffer.append("|");
			}
			else {
				_buffer.erase(_buffer.end() - 1);
			}
		}
	}
}

bool InputField::handleKeyUp(WORD keyCode) {
	if (!_isActive) {
		return false;
	}
	if (('a' <= keyCode && keyCode <= 'z') ||
		('A' <= keyCode && keyCode <= 'Z')) {
		char str[2] = { keyCode, '\0' };

		if (_caretStateON) {
			_buffer.erase(_buffer.end() - 1);
			_buffer.append(str);
			_buffer.append("|");
		}
		else {
			_buffer.append(str);
		}
	}
	else {
		_isActive = false;
	}

	return true;
}