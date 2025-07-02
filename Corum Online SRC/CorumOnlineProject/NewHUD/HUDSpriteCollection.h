#pragma once
#include "../NewUI/CustomUIBase.h"
#include "../NewUI/Button.h"

typedef CustomUI::SpriteModel CUISpriteModel;

struct NewHUDResources {
	static CustomUI::Size newHUDSize;

	static CUISpriteModel hpBarSprite;
	static CUISpriteModel spBarSprite;
	static CUISpriteModel expBarSprite;
	static CUISpriteModel cooldownBarSprite;
	static CUISpriteModel attackSkillSprite;
	static CUISpriteModel leftInterfaceHUDSprite;

	static CustomUI::Button::Sprites
		trade, shop, item, stats, skill,
		group, lair, chat, match, rest, pk, system;


	static void initialize(I4DyuchiGXRenderer *renderer);
};

