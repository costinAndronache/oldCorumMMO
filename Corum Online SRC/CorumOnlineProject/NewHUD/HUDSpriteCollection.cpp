#include "HUDSpriteCollection.h"
#include "../DungeonInterfaceLayout.h"

using namespace CustomUI;

SpriteModel NewHUDResources::hpBarSprite;
SpriteModel NewHUDResources::spBarSprite;
SpriteModel NewHUDResources::expBarSprite;
SpriteModel NewHUDResources::cooldownBarSprite;
SpriteModel NewHUDResources::attackSkillSprite;
SpriteModel NewHUDResources::leftInterfaceHUDSprite;
SpriteModel NewHUDResources::rightInterfaceHUDSprite;
SpriteModel NewHUDResources::inventoryItemUnderlaySprite;
SpriteModel NewHUDResources::genericBackgroundSprite;
CUISpriteModel NewHUDResources::borderedBlackBackgroundSolid;
CUISpriteModel NewHUDResources::borderedBlackBackgroundFaded;

CustomUI::Button::Sprites
NewHUDResources::trade, NewHUDResources::shop, NewHUDResources::item,
NewHUDResources::stats, NewHUDResources::skill, NewHUDResources::group,
NewHUDResources::lair, NewHUDResources::chat, NewHUDResources::match,
NewHUDResources::rest, NewHUDResources::pk, NewHUDResources::system,
NewHUDResources::plus, NewHUDResources::up, NewHUDResources::xClose,
NewHUDResources::next, NewHUDResources::previous;

Size NewHUDResources::newHUDSize{ 400, 121 };

NewHUDResources::InventoryItemUnderlays NewHUDResources::inventoryItemUnderlays;

#define from SpriteModel::from

static void initNewHUDElements(I4DyuchiGXRenderer* r) {
	char* rf = GetFile("newHUDElements.tif", DATA_TYPE_UI);
	const auto smallerSize = Size{ 51, 20 };

	NewHUDResources::plus = {
		from(r, rf, {{63, 523}, {9, 9}}),
		from(r, rf, {{63, 523}, {9, 9}}),
		from(r, rf, {{73, 523}, {9, 8}})
	};

	NewHUDResources::up = {
		from(r, rf, {{0, 565}, {32, 16}}),
		from(r, rf, {{0, 565}, {32, 16}}),
		from(r, rf, {{33, 565}, {32, 16}})
	};

	NewHUDResources::trade = {
		from(r, rf, { { 0, 0}, smallerSize}),
		from(r, rf, { { 0, 20}, smallerSize}),
		from(r, rf, { { 0, 40}, smallerSize})
	};

	NewHUDResources::shop = {
		from(r, rf, { { 52, 0}, smallerSize}),
		from(r, rf, { { 52, 20}, smallerSize}),
		from(r, rf, { { 52, 40}, smallerSize}),
	};

	NewHUDResources::item = {
		from(r, rf, { { 103, 0}, smallerSize}),
		from(r, rf, { { 103, 20}, smallerSize}),
		from(r, rf, { { 103, 40}, smallerSize}),
	};

	NewHUDResources::stats = {
		from(r, rf, { { 161, 0}, smallerSize}),
		from(r, rf, { { 161, 20}, smallerSize}),
		from(r, rf, { { 161, 40}, smallerSize}),
	};

	NewHUDResources::skill = {
		from(r, rf, { { 221, 0}, smallerSize}),
		from(r, rf, { { 221, 20}, smallerSize}),
		from(r, rf, { { 221, 40}, smallerSize}),
	};

	const auto biggerSize = Size{ 60, 18 };

	NewHUDResources::group = {
		from(r, rf, { { 0, 72}, biggerSize}),
		from(r, rf, { { 0, 91}, biggerSize}),
		from(r, rf, { { 0, 110}, biggerSize}),
	};

	NewHUDResources::lair = {
		from(r, rf, { { 60, 72}, biggerSize}),
		from(r, rf, { { 60, 91}, biggerSize}),
		from(r, rf, { { 60, 110}, biggerSize}),
	};

	NewHUDResources::chat = {
		from(r, rf, { { 119, 72}, biggerSize}),
		from(r, rf, { { 119, 91}, biggerSize}),
		from(r, rf, { { 119, 110}, biggerSize}),
	};

	NewHUDResources::match = {
		from(r, rf, { { 177, 72}, biggerSize}),
		from(r, rf, { { 177, 91}, biggerSize}),
		from(r, rf, { { 177, 110}, biggerSize}),
	};

	const auto size3 = Size{ 30, 33 };
	NewHUDResources::pk = {
		from(r, rf, { { 0, 143}, {30, 33} }),
		from(r, rf, { { 33, 143}, {30, 33} }),
		from(r, rf, { { 66, 143}, {30, 33} }),
	};

	NewHUDResources::system = {
		from(r, rf, { { 0, 180}, {32, 32} }),
		from(r, rf, { { 34, 180}, {32, 32} }),
		from(r, rf, { { 68, 180}, {32, 32} }),
	};

	NewHUDResources::borderedBlackBackgroundSolid = from(r, rf, { {0, 523}, {62, 20} });
	NewHUDResources::borderedBlackBackgroundFaded = from(r, rf, { {0, 544}, {166, 20} });

	NewHUDResources::xClose = {
		{ SpriteCollection::xClose, SpriteCollection::xCloseSize},
		{ SpriteCollection::xClose, SpriteCollection::xCloseSize},
		{ SpriteCollection::xClosePressed, SpriteCollection::xCloseSize}
	};

	NewHUDResources::next = {
		from(r, rf, {{85, 524}, {16, 17}}),
		from(r, rf, {{85, 524}, {16, 17}}),
		from(r, rf, {{102, 524}, {16, 17}})
	};

	NewHUDResources::previous = {
		from(r, rf, {{119, 524}, {16, 17}}),
		from(r, rf, {{119, 524}, {16, 17}}),
		from(r, rf, {{136, 524}, {16, 17}})
	};
}

static void initMenu1Elements(I4DyuchiGXRenderer* renderer) {
	Size bigBarsSize{ 4, 15 };
	Size smallBarsSize{ 2, 5 };

	char* resourceFile = GetFile("menu_1.tga", DATA_TYPE_UI);

	NewHUDResources::hpBarSprite = SpriteModel::from(renderer, resourceFile, {
		{123, 32}, bigBarsSize
	});

	NewHUDResources::spBarSprite = SpriteModel::from(renderer, resourceFile, {
		{131, 32}, bigBarsSize
	});

	NewHUDResources::expBarSprite = SpriteModel::from(renderer, resourceFile, {
		{144, 32}, smallBarsSize
	});

	NewHUDResources::cooldownBarSprite = SpriteModel::from(renderer, resourceFile, {
		{140, 32}, smallBarsSize
	});
}

void initInventoryUnderlayElements(I4DyuchiGXRenderer* r) {
	char* rf = GetFile("newHUDElements.tif", DATA_TYPE_UI);
	Size itemSize = { 33, 64 };
	const auto y = 252;

	NewHUDResources::inventoryItemUnderlays.sword = from(r, rf, { {0, y}, itemSize });
	NewHUDResources::inventoryItemUnderlays.key = from(r, rf, { {34, y}, itemSize });
	NewHUDResources::inventoryItemUnderlays.helm = from(r, rf, { {68, y}, itemSize });
	NewHUDResources::inventoryItemUnderlays.doubleHelm = from(r, rf, { {102, y}, itemSize });
	NewHUDResources::inventoryItemUnderlays.shield = from(r, rf, { {136, y}, itemSize });
	NewHUDResources::inventoryItemUnderlays.gloveLeft = from(r, rf, { {170, y}, itemSize });
	NewHUDResources::inventoryItemUnderlays.armor = from(r, rf, { {204, y}, itemSize });
	NewHUDResources::inventoryItemUnderlays.doubleArmor = from(r, rf, { {238, y}, itemSize });
	NewHUDResources::inventoryItemUnderlays.necklace = from(r, rf, { {272, y}, itemSize });
	NewHUDResources::inventoryItemUnderlays.bag = from(r, rf, { {306, y}, itemSize });
	NewHUDResources::inventoryItemUnderlays.belt = from(r, rf, { {340, y}, itemSize });
	NewHUDResources::inventoryItemUnderlays.gloveRight = from(r, rf, { {374, y}, itemSize });
	NewHUDResources::inventoryItemUnderlays.boot = from(r, rf, { {408, y}, itemSize });
	NewHUDResources::inventoryItemUnderlays.ring = from(r, rf, { {442, y}, itemSize });


	NewHUDResources::inventoryItemUnderlays.inventoryBackground = from(
		r, rf, { {0, 317}, { 240, 205 } }
	);
}

void NewHUDResources::initialize(I4DyuchiGXRenderer* renderer) {
	initMenu1Elements(renderer);
	initNewHUDElements(renderer);
	initInventoryUnderlayElements(renderer);

	attackSkillSprite = {
		renderer->CreateSpriteObject(GetFile("skill_icon1.tga", DATA_TYPE_UI), 0, 0, 32, 32, 0),
		{ SKILL_ICON_SIZE, SKILL_ICON_SIZE }
	};
	
	leftInterfaceHUDSprite = {
		renderer->CreateSpriteObject(GetFile("newInterfaceLeft.tif", DATA_TYPE_UI), 0, 0, 400, 121, 0),
		{ 400, 121}
	};

	rightInterfaceHUDSprite = {
		renderer->CreateSpriteObject(GetFile("newInterfaceRight.tif", DATA_TYPE_UI), 0, 0, 400, 121, 0),
		{ 400, 121}
	};

	auto menu_4 = GetFile("menu_4.tif", DATA_TYPE_UI);
	inventoryItemUnderlaySprite = {
		renderer->CreateSpriteObject(menu_4, 190, 115, 34, 34, 0),
		{ 34, 34 }
	};

	char* resourceFile = GetFile("speaking_box.tif", DATA_TYPE_UI);
	genericBackgroundSprite = {
		renderer->CreateSpriteObject(resourceFile, 0, 0, 2, 2, 0),
		{ 2, 2 }
	};
}

SpriteModel NewHUDResources::spriteForSkill(BYTE skillKind, LP_SKILL_LIST_MANAGER skillListManager) {
	Button::Sprites specimen = Button::Sprites::allZero;
	if (!(0 <= skillKind && skillKind < MAX_SKILL)) { return SpriteModel::zero; }

	if (skillKind == __SKILL_ATTACK__) {
		return NewHUDResources::attackSkillSprite;
	}

	return {
		skillListManager->spriteForSkillKind[skillKind],
		{SKILL_ICON_SIZE, SKILL_ICON_SIZE}
	};
}