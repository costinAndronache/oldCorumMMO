#include "LeftHUD.h"
#include "../InterfaceSpr.h"
#include "../InterfaceSprDef.h"
#include "HUDSpriteCollection.h"

using namespace CustomUI;
using namespace NewInterface;

LeftHUD::LeftHUD(Point originInParent) {
	_frameInParent = { originInParent, NewHUDResources::newHUDSize };

	const auto hpBarFrame = Rect{ { 0, 60 }, {302, 13} };

	_hpBar = registerChildRenderable<HorizontalScalingBar>([=]() {
		return 	new HorizontalScalingBar(
			NewHUDResources::spBarSprite,
			hpBarFrame,
			HorizontalScalingBar::Direction::leftRight);
	});

	const auto expBarFrame = Rect{ {0, 75}, {302, 5} };
	_expBar = registerChildRenderable<HorizontalScalingBar>([=]() {
		return new HorizontalScalingBar(
			NewHUDResources::expBarSprite,
			expBarFrame,
			HorizontalScalingBar::Direction::leftRight
		);
	});

	const auto tradeBtnFrame = Rect{ {0, 14}, {52, 18} };
	_tradeBtn = registerChildRenderable<Button>([=]() {
		return new Button(NewHUDResources::trade, tradeBtnFrame);
	});

	const auto shopBtnFrame = Rect{ {0, 33}, {52, 18} };
	_shopBtn = registerChildRenderable<Button>([=]() {
		return new Button(NewHUDResources::shop, shopBtnFrame);
	});

	const auto itemBtnFrame = Rect{ {72, 35}, {57, 18} };
	_itemBtn = registerChildRenderable<Button>([=]() {
		return new Button(NewHUDResources::item, itemBtnFrame);
	});

	const auto statsBtnFrame = Rect{ {152, 35}, {59, 18} };
	_statsBtn = registerChildRenderable<Button>([=]() {
		return new Button(NewHUDResources::stats, statsBtnFrame);
	});

	const auto skillsBtnFrame = Rect{ { 228, 35}, {59, 18} };
	_skillsBtn = registerChildRenderable<Button>([=]() {
		return new Button(NewHUDResources::skill, skillsBtnFrame);
	});

	const auto pkBtnFrame = Rect{ {330, 83}, {32, 32} };
	_pkButton = registerChildRenderable<ToggleButton>([=]() {
		return new ToggleButton(NewHUDResources::pk, pkBtnFrame);
	});

	_pkButton->setState(false);
}

void LeftHUD::renderWithRenderer(I4DyuchiGXRenderer* renderer, int zIndex) {
	
}

void LeftHUD::updatePKToggle(bool isON) {
	_pkButton->setState(isON);
}

void LeftHUD::updateEXPScale(float scale) {
	_expBar->updateScale(scale);
}

void LeftHUD::updateHPScale(float scale) {
	_hpBar->updateScale(scale);
}

void LeftHUD::setEventHandlers(EventHandlers handlers) {
	_tradeBtn->onClickEnd(handlers.tradeHandler);
	_shopBtn->onClickEnd(handlers.shopHandler);
	_itemBtn->onClickEnd(handlers.itemHandler);
	_statsBtn->onClickEnd(handlers.statsHandler);
	_skillsBtn->onClickEnd(handlers.skillsHandler);
	_pkButton->onStateSwitch(handlers.pkHandler);
}