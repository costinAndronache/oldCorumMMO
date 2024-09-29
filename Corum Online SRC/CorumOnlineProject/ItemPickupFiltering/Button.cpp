#include "Button.h"
#include "../CorumResource.h"

using namespace CustomUI;

IDISpriteObject* ButtonResources::downArrow = NULL;
IDISpriteObject* ButtonResources::downArrowPressed = NULL;
Size ButtonResources::downArrowSize = { 14, 14 };

IDISpriteObject* ButtonResources::genericBackground = NULL;
IDISpriteObject* ButtonResources::genericPressedBackground = NULL;
Size ButtonResources::genericBackgroundSize = { 62, 19 };

IDISpriteObject* ButtonResources::xClose = NULL;
IDISpriteObject* ButtonResources::xClosePressed = NULL;
Size ButtonResources::xCloseSize = { 13, 13 };

IDISpriteObject* ButtonResources::xRedBlack = NULL;
IDISpriteObject* ButtonResources::xRedBlackPressed = NULL;
Size ButtonResources::xRedBlackSize = { 33, 33 };

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

	if (xClose == NULL) {
		char* resourceFile = GetFile("menu_1.tga", DATA_TYPE_UI);
		xClose = g_pRenderer->CreateSpriteObject(resourceFile,
			187, 16,
			xCloseSize.width, xCloseSize.height,
			0);
		xClosePressed = g_pRenderer->CreateSpriteObject(resourceFile,
			200, 16,
			xCloseSize.width, xCloseSize.height,
			0);
	}

	if (xRedBlack == NULL) {
		char* resourceFile = GetFile("menu_4.tif", DATA_TYPE_UI);
		xRedBlack = g_pRenderer->CreateSpriteObject(resourceFile,
			38, 38,
			xRedBlackSize.width, xRedBlackSize.height,
			0);
		xRedBlackPressed = g_pRenderer->CreateSpriteObject(resourceFile,
			37, 42,
			xRedBlackSize.width, 28,
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

	Rect labelFrame = { frame.origin, SingleLineLabel::fittedSize(strlen(labelModel.text)) };
	labelFrame = labelFrame.centeredWith(frame);
	_label = new SingleLineLabel(labelFrame, labelModel.appearance, labelModel.text);
}

void Button::updateSpriteModelTo(SpriteModel newModel) {
	_spriteModel = newModel;
}

void Button::updatePressedSpriteModelTo(SpriteModel newPressedStateSpriteModel) {
	_pressedSpriteModel = newPressedStateSpriteModel;
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