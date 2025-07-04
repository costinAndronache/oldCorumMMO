#include "RightHUD.h"
#include "HUDSpriteCollection.h"


using namespace CustomUI;
using namespace NewInterface;

RightHUD::RightHUD(CustomUI::Point originInParent) {
	_frameInParent = { originInParent, NewHUDResources::newHUDSize };

	_rightHUDSprite = registerChildRenderable<SpriteRenderable>([=]() {
		return new SpriteRenderable(bounds(), NewHUDResources::rightInterfaceHUDSprite);
	});

	const auto groupBtnSprites = NewHUDResources::group;
	const auto groupBtnFrame = Rect{ {112, 35}, groupBtnSprites.normal.size };
	_groupBtn = registerChildRenderable<Button>([=]() {
		return new Button(groupBtnSprites, groupBtnFrame);
	});

	const auto lairBtnSprites = NewHUDResources::lair;
	const auto lairBtnFrame = Rect{ {189, 35}, lairBtnSprites.normal.size };
	_lairBtn = registerChildRenderable<Button>([=]() {
		return new Button(lairBtnSprites, lairBtnFrame);
	});

	const auto chatBtnSprites = NewHUDResources::chat;
	const auto chatBtnFrame = Rect{ {272, 35}, chatBtnSprites.normal.size };
	_chatBtn = registerChildRenderable<Button>([=]() {
		return new Button(chatBtnSprites, chatBtnFrame);
	});

	const auto matchBtnSprites = NewHUDResources::match;
	const auto matchBtnFrame = Rect{ {350, 33}, matchBtnSprites.normal.size };
	_matchBtn = registerChildRenderable<Button>([=]() {
		return new Button(matchBtnSprites, matchBtnFrame);
	});

	const auto restBtnSprites = NewHUDResources::rest;
	const auto restBtnFrame = Rect{ {350, 14}, restBtnSprites.normal.size };
	_restBtn = registerChildRenderable<Button>([=]() {
		return new Button(restBtnSprites, restBtnFrame);
	});

	const auto spBarFrame = Rect{ {97, 60}, {300, 15} };
	_spBar = registerChildRenderable<HorizontalScalingBar>([=]() {
		return new HorizontalScalingBar(NewHUDResources::spBarSprite,
										spBarFrame,										
										HorizontalScalingBar::Direction::rightLeft);
	});

	const auto coolBarFrame = Rect{ {97, 75}, { 300, 5 } };
	_cooldownBar = registerChildRenderable<HorizontalScalingBar>([=]() {
		return new HorizontalScalingBar(NewHUDResources::cooldownBarSprite,
										coolBarFrame,
										HorizontalScalingBar::Direction::rightLeft);
	});

	const auto optionsBtnSprites = NewHUDResources::system;
	const auto optionsBtnFrame = Rect{ {34, 84}, optionsBtnSprites.normal.size };
	_optionsBtn = registerChildRenderable<Button>([=]() {
		return new Button(optionsBtnSprites, optionsBtnFrame);
	});
}


void RightHUD::setEventHandlers(EventHandlers handlers) {

}

void RightHUD::updateSPScale(float scale) {
	_spBar->updateScale(scale);
}

void RightHUD::updateCooldownScale(float scale) {
	_cooldownBar->updateScale(scale);
}