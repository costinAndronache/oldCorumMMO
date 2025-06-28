#include "HUDSpriteCollection.h"
using namespace CustomUI;

SpriteModel HUDSpriteCollection::hpBarSprite;
SpriteModel HUDSpriteCollection::spBarSprite;
SpriteModel HUDSpriteCollection::expBarSprite;
SpriteModel HUDSpriteCollection::cooldownBarSprite;

void HUDSpriteCollection::initialize(I4DyuchiGXRenderer* renderer) {
	Size bigBarsSize{ 2, 15 };
	Size smallBarsSize{ 4, 5 };

	char* resourceFile = GetFile("menu_1.tga", DATA_TYPE_UI);

	const auto hpBar = renderer->CreateSpriteObject(resourceFile,
		123, 33,
		bigBarsSize.width, bigBarsSize.height,
		0);
	const auto spBar = renderer->CreateSpriteObject(resourceFile,
		131, 33,
		bigBarsSize.width, bigBarsSize.height,
		0);

	const auto expBar = renderer->CreateSpriteObject(resourceFile,
		143, 32,
		smallBarsSize.width, smallBarsSize.height,
		0);

	const auto cooldownBar = renderer->CreateSpriteObject(resourceFile,
		139, 32,
		smallBarsSize.width, smallBarsSize.height,
		0);

	hpBarSprite = { hpBar, bigBarsSize, 0 };
	spBarSprite = { spBar, bigBarsSize, 0 };
	expBarSprite = { expBar, smallBarsSize, 0 };
	cooldownBarSprite = { cooldownBar, smallBarsSize, 0 };
}