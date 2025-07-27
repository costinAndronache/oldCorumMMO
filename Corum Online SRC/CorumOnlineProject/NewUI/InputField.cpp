#include "InputField.h"
#include <d3d8.h>
using namespace CustomUI;

#define WHITE(alpha)         D3DCOLOR_ARGB(alpha, 255, 255, 255)

SpriteModel InputFieldResources::bgSpriteModel = { NULL, {53, 12} };
// menu1 tga, 74,0 53x12

void InputFieldResources::initialize() {
	if (bgSpriteModel.sprite == NULL) {
		char* resourceFile = GetFile("menu_1.tga", DATA_TYPE_UI);
		bgSpriteModel.sprite = g_pRenderer->CreateSpriteObject(
			resourceFile,
			74, 0,
			bgSpriteModel.naturalSize.width, bgSpriteModel.naturalSize.height,
			0
		);
	}
}

InputField::InputField(Rect frameInParent, SpriteModel bgSpriteModel): _bgSpriteModel(bgSpriteModel) {
	_isActive = false;
	_caretStateON = false;
	_lastCaretUpdateTime = 0;
	InputFieldResources::initialize();
	_buffer = "";
	_frameInParent = frameInParent;
	_clearButton = NULL;
}

InputField::InputField(Rect frameInParent, SpriteModel bgSpriteModel, Button::Sprites clearButtonSprites) : _bgSpriteModel(bgSpriteModel) {
	_frameInParent = frameInParent;

	_isActive = false;
	_caretStateON = false;
	_lastCaretUpdateTime = 0;
	InputFieldResources::initialize();
	_buffer = "";

	const auto _bounds = bounds();
	const auto clearBtnSize = Size{ 25, 25 };
	Rect clearBtnFrame = _bounds
		.fromMaxXOrigin(-(clearBtnSize.width+5))
		.withSize(clearBtnSize)
		.centeredVerticallyWith(_bounds);

	_clearButton = registerChildRenderable<Button>([&]() {
		return new Button(clearButtonSprites, clearBtnFrame);
	});

	_clearButton->onClickEndLEFT([this]() {
		onButtonPressRelease(this->_clearButton);
	});
}

bool InputField::swallowsMouseEvents() { 
	return _clearButton == nullptr;
}


void InputField::onButtonPressRelease(Button* button) { 
	_buffer.erase(_buffer.begin(), _buffer.end());
	notifyClient();
}


void InputField::onMouseStateChange(MouseState newState, MouseState oldState) {
	if (newState == MouseState::leftButtonPressedInside) {
		_isActive = true;
		_caretStateON = true;
	}

	if (newState == MouseState::none) {
		_isActive = false;
		_caretStateON = false;
	}
}

void InputField::processKeyUp(WPARAM wparam, LPARAM lparam) {
	if (!_isActive) {
		return;
	}

	if (wparam == VK_BACK) {
		if (!_buffer.empty()) {
			_buffer.erase(_buffer.end() - 1);
			notifyClient();
		}
		return;
	}

	if (wparam == VK_SPACE) {
		_buffer.append(" ");
		notifyClient();
		return;
	}

	const short asciiResult = getASCII(wparam, lparam);
	if (asciiResult < 0) {
		return;
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
}


void InputField::renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) {
	const auto _globalFrame = globalFrame();
	_bgSpriteModel.renderWith(renderer, _globalFrame, order);

	char renderedText[maxChars + 1];
	int count = _buffer.size();
	strcpy(renderedText, _buffer.c_str());

	if (_caretStateON) {
		strcat(renderedText, "|");
		count += 1;
	}

	RECT rt = { _globalFrame.origin.x + 5, _globalFrame.origin.y + 5, 
		_globalFrame.maxX(), _globalFrame.maxY() };

	renderer->RenderFont(GetFont(),
		(TCHAR*)renderedText,
		count,
		&rt,
		WHITE(255),
		CHAR_CODE_TYPE_ASCII,
		order + 1,
		0);

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

void InputField::notifyClient() {
	if (_handler) {
		const char* text = _buffer.c_str();
		_handler(text);
	}
}

const char* InputField::currentText() {
	return _buffer.c_str();
}