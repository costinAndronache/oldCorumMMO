#include "HUDSpriteCollection.h"
using namespace CustomUI;

SpriteModel NewHUDResources::hpBarSprite;
SpriteModel NewHUDResources::spBarSprite;
SpriteModel NewHUDResources::expBarSprite;
SpriteModel NewHUDResources::cooldownBarSprite;
SpriteModel NewHUDResources::attackSkillSprite;

CustomUI::Button::Sprites
NewHUDResources::trade, NewHUDResources::shop, NewHUDResources::item, 
NewHUDResources::stats, NewHUDResources::skill, NewHUDResources::group, 
NewHUDResources::lair, NewHUDResources::chat, NewHUDResources::match, 
NewHUDResources::rest, NewHUDResources::pk, NewHUDResources::system;

Size NewHUDResources::newHUDSize{ 400, 121 };

#define from SpriteModel::from

static void initNewHUDElements(I4DyuchiGXRenderer* r) {
	char* rf = GetFile("newHUDElements.tiff", DATA_TYPE_UI);
	const auto smallerSize = Size{ 51, 18 };

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

	NewHUDResources::match = {
		from(r, rf, { { 229, 72}, biggerSize}),
		from(r, rf, { { 229, 91}, biggerSize}),
		from(r, rf, { { 229, 110}, biggerSize}),
	};

	NewHUDResources::match = {
		from(r, rf, { { 229, 72}, biggerSize}),
		from(r, rf, { { 229, 91}, biggerSize}),
		from(r, rf, { { 229, 110}, biggerSize}),
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
}

static void initMenu1Elements(I4DyuchiGXRenderer* renderer) {
	Size bigBarsSize{ 2, 15 };
	Size smallBarsSize{ 4, 5 };

	char* resourceFile = GetFile("menu_1.tga", DATA_TYPE_UI);

	NewHUDResources::hpBarSprite = SpriteModel::from(renderer, resourceFile, {
		{123, 33}, bigBarsSize
	});

	NewHUDResources::spBarSprite = SpriteModel::from(renderer, resourceFile, {
		{131, 33}, bigBarsSize
	});

	NewHUDResources::expBarSprite = SpriteModel::from(renderer, resourceFile, {
		{143, 32}, smallBarsSize
	});

	NewHUDResources::cooldownBarSprite = SpriteModel::from(renderer, resourceFile, {
		{149, 32}, smallBarsSize
	});
}

void NewHUDResources::initialize(I4DyuchiGXRenderer* renderer) {
	initMenu1Elements(renderer);
	initNewHUDElements(renderer);

	attackSkillSprite = {
		renderer->CreateSpriteObject(GetFile("skill_icon1.tga", DATA_TYPE_UI), 0, 0, 32, 32, 0),
		{ 32, 32},
		0
	};
	
}