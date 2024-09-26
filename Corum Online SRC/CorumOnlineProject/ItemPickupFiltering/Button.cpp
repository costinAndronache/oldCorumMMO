#include "Button.h"
#include "../CorumResource.h"

using namespace CustomUI;

IDISpriteObject* ButtonResources::downArrow = NULL;
IDISpriteObject* ButtonResources::downArrowPressed = NULL;
Size ButtonResources::downArrowSize = { 14, 14 };

IDISpriteObject* ButtonResources::genericBackground = NULL;
IDISpriteObject* ButtonResources::genericPressedBackground = NULL;
Size ButtonResources::genericBackgroundSize = { 62, 19 };

void ButtonResources::initialize() {
	if (downArrow == NULL) {
		char* resourceFile = GetFile("menu_1.tga", DATA_TYPE_UI);
		downArrow = g_pRenderer->CreateSpriteObject(resourceFile,
													213, 109,
													downArrowSize.width, downArrowSize.height,
													0);
		downArrowPressed = g_pRenderer->CreateSpriteObject(resourceFile,
														   213 + downArrowSize.width, 109,
														   downArrowSize.width, downArrowSize.height,
														   0);
	}

	if (genericBackground == NULL) {
		char* resourceFile = GetFile("menu_7.tga", DATA_TYPE_UI);
		genericBackground = g_pRenderer->CreateSpriteObject(resourceFile,
			194, 175,
			genericBackgroundSize.width, genericBackgroundSize.height,
			0);
		genericPressedBackground = g_pRenderer->CreateSpriteObject(resourceFile,
			194, 191,
			genericBackgroundSize.width, genericBackgroundSize.height,
			0);
	}
}

Button::Button(SpriteModel spriteModel, SpriteModel pressedSpriteModel, Rect frame, ButtonClient* client) :
			   _spriteModel(spriteModel), _pressedSpriteModel(pressedSpriteModel), _frame(frame), _client(client) {
	_detectedPress = false;
	_lastPressNotifyTime = 0;
	_label = NULL;
}

Button::Button(SpriteModel spriteModel, SpriteModel pressedSpriteModel, LabelModel labelModel, Rect frame, ButtonClient* client): 
	_spriteModel(spriteModel), _pressedSpriteModel(pressedSpriteModel), _frame(frame), _client(client) {
	_detectedPress = false;
	_lastPressNotifyTime = 0;

	Rect labelFrame = { frame.origin, Label::fittedSize(strlen(labelModel.text)) };
	labelFrame = labelFrame.centeredXYWith(frame);
	_label = new Label(labelFrame, labelModel.appearance, labelModel.text);
}

void Button::renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) {
	VECTOR2 pos = { _frame.origin.x, _frame.origin.y };

	if (g_Mouse.bLDown && _frame.isGlobalMouseInside()) {
		if (_pressedSpriteModel.sprite) {
			VECTOR2 scale = _frame.size.divideBy(_pressedSpriteModel.size);
			renderer->RenderSprite(_pressedSpriteModel.sprite, &scale, _pressedSpriteModel.rotation, &pos, NULL, 0xffffffff, order, RENDER_TYPE_DISABLE_TEX_FILTERING);
		}
		_detectedPress = true;
		DWORD now = timeGetTime();

		if (now - _lastPressNotifyTime > 500 && _client) {
			_client->onButtonPress(this);
			_lastPressNotifyTime = now;
		}
	}
	else {
		if (_spriteModel.sprite) {
			VECTOR2 scale = _frame.size.divideBy(_spriteModel.size);
			renderer->RenderSprite(_spriteModel.sprite, &scale, _spriteModel.rotation, &pos, NULL, 0xffffffff, order, RENDER_TYPE_DISABLE_TEX_FILTERING);
		}

		if (_detectedPress && _client) {
			_client->onButtonPressRelease(this);
			_detectedPress = false;
		}
	}

	if (_label) {
		_label->renderWithRenderer(renderer, order + 1);
	}
}