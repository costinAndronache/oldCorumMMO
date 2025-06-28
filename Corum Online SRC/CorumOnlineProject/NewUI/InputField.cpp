#include "InputField.h"
#include <d3d8.h>
using namespace CustomUI;

#define WHITE(alpha)         D3DCOLOR_ARGB(alpha, 255, 255, 255)

SpriteModel InputFieldResources::bgSpriteModel = { NULL, {53, 12}, 0 };
// menu1 tga, 74,0 53x12

void InputFieldResources::initialize() {
	if (bgSpriteModel.sprite == NULL) {
		char* resourceFile = GetFile("menu_1.tga", DATA_TYPE_UI);
		bgSpriteModel.sprite = g_pRenderer->CreateSpriteObject(
			resourceFile,
			74, 0,
			bgSpriteModel.size.width, bgSpriteModel.size.height,
			0
		);
	}
}

InputField::InputField(Rect frame, SpriteModel bgSpriteModel): _bgSpriteModel(bgSpriteModel) {
	_isActive = false;
	_caretStateON = false;
	_lastCaretUpdateTime = 0;
	InputFieldResources::initialize();
	_buffer = "";
	_frame = frame;
	_clearButton = NULL;
}

InputField::InputField(Rect frame, SpriteModel bgSpriteModel, SpriteModel clearBtnModel, SpriteModel clearBtnPressedModel) : _bgSpriteModel(bgSpriteModel) {
	_frame = frame;

	_isActive = false;
	_caretStateON = false;
	_lastCaretUpdateTime = 0;
	InputFieldResources::initialize();
	_buffer = "";

	Rect clearBtnFrame = frame
		.fromMaxXOrigin(-clearBtnModel.size.width-5)
		.withSize(clearBtnModel.size)
		.scaled(0.8, 0.8)
		.centeredVerticallyWith(frame);

	_clearButton = registerChildRenderable<Button>([&]() {
		return new Button(clearBtnModel, clearBtnPressedModel, clearBtnFrame);
	});

	_clearButton->onRelease([this]() {
		onButtonPressRelease(this->_clearButton);
	});
}


void InputField::onButtonPress(Button* button) { }

void InputField::onButtonPressRelease(Button* button) { 
	_buffer.erase(_buffer.begin(), _buffer.end());
	notifyClient();
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
		if (_frame.isMouseInside(g_Mouse.MousePos)) {
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
		if (now - _lastCaretUpdateTime > 600) {
			_lastCaretUpdateTime = now;
			_caretStateON = !_caretStateON;
		}
	}

	if (_clearButton) {
		_clearButton->renderWithRenderer(renderer, order + 1);
	}
}

bool InputField::handleKeyDown(WPARAM wparam, LPARAM lparam) {
	return _isActive;
} 

void InputField::notifyClient() {
	if (_handler) {
		const char* text = _buffer.c_str();
		_handler(text);
	}
}

bool InputField::handleKeyUp(WPARAM wparam, LPARAM lParam) {
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

	if (wparam == VK_SPACE) {
		_buffer.append(" ");
		notifyClient();
		return true;
	}

	const short asciiResult = getASCII(wparam, lParam);
	if (asciiResult < 0) {
		return true;
	}

	char key = (char)asciiResult;

	if (('a' <= key && key <= 'z') ||
		('A' <= key && key <= 'Z') ||
		('0' <= key && key <= '9')) {
		char str[2] = { key, '\0' };

		if (_buffer.size() < maxChars) {
			_buffer.append(str);
			notifyClient();
		}
	}

	return true;
}

const char* InputField::currentText() {
	return _buffer.c_str();
}