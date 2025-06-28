#pragma once
#include "../NewUI/CustomUIBase.h"

typedef CustomUI::SpriteModel CUISpriteModel;

struct HUDSpriteCollection {
	static CUISpriteModel hpBarSprite;
	static CUISpriteModel spBarSprite;
	static CUISpriteModel expBarSprite;
	static CUISpriteModel cooldownBarSprite;

	static void initialize(I4DyuchiGXRenderer *renderer);
};

