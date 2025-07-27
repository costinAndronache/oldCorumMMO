#include "SpriteCollection.h"

using namespace CustomUI;

IDISpriteObject* SpriteCollection::downArrow = NULL;
IDISpriteObject* SpriteCollection::downArrowPressed = NULL;
Size SpriteCollection::downArrowSize = { 14, 14 };

IDISpriteObject* SpriteCollection::genericButtonBackground = NULL;
IDISpriteObject* SpriteCollection::genericButtonPressedBackground = NULL;
Size SpriteCollection::genericButtonBackgroundSize = { 62, 18 };

IDISpriteObject* SpriteCollection::xClose = NULL;
IDISpriteObject* SpriteCollection::xClosePressed = NULL;
Size SpriteCollection::xCloseSize = { 13, 13 };

IDISpriteObject* SpriteCollection::xRedBlack = NULL;
IDISpriteObject* SpriteCollection::xRedBlackPressed = NULL;
Size SpriteCollection::xRedBlackSize = { 33, 33 };

SpriteModel SpriteCollection::xCloseSprite;
SpriteModel SpriteCollection::xClosePressedSprite;

void SpriteCollection::initialize(I4DyuchiGXRenderer* renderer) {
	if (downArrow == NULL) {
		char* resourceFile = GetFile("menu_1.tga", DATA_TYPE_UI);
		downArrow = renderer->CreateSpriteObject(resourceFile,
			213, 109,
			downArrowSize.width, downArrowSize.height,
			0);
		downArrowPressed = renderer->CreateSpriteObject(resourceFile,
			213 + downArrowSize.width, 109,
			downArrowSize.width, downArrowSize.height,
			0);
	}

	if (genericButtonBackground == NULL) {
		char* resourceFile = GetFile("menu_7.tga", DATA_TYPE_UI);
		genericButtonBackground = renderer->CreateSpriteObject(resourceFile,
			194, 175,
			genericButtonBackgroundSize.width, genericButtonBackgroundSize.height,
			0);
		genericButtonPressedBackground = renderer->CreateSpriteObject(resourceFile,
			194, 191,
			genericButtonBackgroundSize.width, genericButtonBackgroundSize.height,
			0);
	}

	if (xClose == NULL) {
		char* resourceFile = GetFile("menu_1.tga", DATA_TYPE_UI);
		xClose = renderer->CreateSpriteObject(resourceFile,
			187, 16,
			xCloseSize.width, xCloseSize.height,
			0);
		xClosePressed = renderer->CreateSpriteObject(resourceFile,
			200, 16,
			xCloseSize.width, xCloseSize.height,
			0);
	}

	if (xRedBlack == NULL) {
		char* resourceFile = GetFile("menu_4.tif", DATA_TYPE_UI);
		xRedBlack = renderer->CreateSpriteObject(resourceFile,
			38, 38,
			xRedBlackSize.width, xRedBlackSize.height,
			0);
		xRedBlackPressed = renderer->CreateSpriteObject(resourceFile,
			37, 42,
			xRedBlackSize.width, 28,
			0);
	}

	xCloseSprite = { xClose, xCloseSize };
	xClosePressedSprite = { xClosePressed, xCloseSize };
}