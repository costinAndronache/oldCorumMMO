#pragma once
#include "../NewUI/CustomUIBase.h"

struct HUDSpriteCollection {
	static CustomUI::SpriteModel hpBarSprite;
	static CustomUI::SpriteModel spBarSprite;
	static CustomUI::SpriteModel expBarSprite;
	static CustomUI::SpriteModel cooldownBarSprite;

	static void initialize(I4DyuchiGXRenderer *renderer);
};

