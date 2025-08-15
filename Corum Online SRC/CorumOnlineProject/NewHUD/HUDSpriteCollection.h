#pragma once
#include "../NewUI/CustomUIBase.h"
#include "../NewUI/Button.h"
#include "../NewUI/BorderRenderable.h"

typedef CustomUI::SpriteModel CUISpriteModel;

struct NewHUDResources {
	static CustomUI::Size newHUDSize;

	static CUISpriteModel hpBarSprite;
	static CUISpriteModel spBarSprite;
	static CUISpriteModel expBarSprite;
	static CUISpriteModel cooldownBarSprite;
	static CUISpriteModel attackSkillSprite;
	static CUISpriteModel leftInterfaceHUDSprite;
	static CUISpriteModel rightInterfaceHUDSprite;
	static CUISpriteModel genericBackgroundSprite;
	static CUISpriteModel borderedBlackBackgroundSolid;
	static CUISpriteModel borderedBlackBackgroundFaded;
	static CUISpriteModel blueDot;
	static CUISpriteModel displacementIcon;
	static CustomUI::BorderRenderable::BorderLineSprites goldBorder;

	static CustomUI::Button::Sprites
		trade, shop, item, stats, skill,
		group, lair, chat, match, rest, pk, system;

	static CustomUI::Button::Sprites plus, up, xClose, next, previous;
	static CustomUI::Button::Sprites genericButtonBackgroundSprites;

	static CUISpriteModel inventoryItemUnderlaySprite;

	static struct InventoryItemUnderlays {
		CUISpriteModel sword, key, helm, doubleHelm,
			shield, gloveLeft, armor, doubleArmor,
			necklace, bag, belt, gloveRight, boot, ring,
			inventoryBackground;
	} inventoryItemUnderlays ;

	static void initialize(I4DyuchiGXRenderer *renderer);

	static CustomUI::SpriteModel spriteForSkill(BYTE skillKind, LP_SKILL_LIST_MANAGER skillListManager);
};

