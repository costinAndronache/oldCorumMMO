#include "LeftHUD.h"
#include "../InterfaceSpr.h"
#include "../InterfaceSprDef.h"
#include "HUDSpriteCollection.h"

using namespace CustomUI;
using namespace NewInterface;

LeftHUD::LeftHUD(CustomUI::Point originInParent) {
	_frameInParent = { originInParent, NewHUDResources::newHUDSize };

	_leftHUDSprite = registerChildRenderable<SpriteRenderable>([=]() {
		return new SpriteRenderable(bounds(), NewHUDResources::leftInterfaceHUDSprite);
	});

	const auto hpBarFrame = Rect{ { 0, 60 }, {302, 13} };

	_hpBar = registerChildRenderable<HorizontalScalingBar>([=]() {
		return 	new HorizontalScalingBar(
			NewHUDResources::hpBarSprite,
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

	const auto leftSkillBtnFrame = Rect{ {132, 85}, {34, 34} };
	_leftSkillBtn = registerChildRenderable<Button>([&]() {
		return new Button(Button::Sprites::allZero, leftSkillBtnFrame);
	});

	const auto rightSkillBtnFrame = Rect{ {247, 85}, {34, 34} };
	_rightSkillBtn = registerChildRenderable<Button>([&]() {
		return new Button(Button::Sprites::allZero, rightSkillBtnFrame);
	});

	_pkButton->setState(false);

	const auto labelsSize = Size{ 100, 15 };
	const auto labelsAppearance = SingleLineLabel::Appearance{
		{ 255, 255, 255, 255}
	};

	_hpLabel = registerChildRenderable<SingleLineLabel>([=]() {
		return new SingleLineLabel(
			{ {0, 90 }, labelsSize },
			labelsAppearance,
			""
		);
	});

	_spLabel = registerChildRenderable<SingleLineLabel>([=]() {
		return new SingleLineLabel(
			{ {0, 104 }, labelsSize },
			labelsAppearance,
			""
		);
	});

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
	_tradeBtn->onClickEndLEFT(handlers.tradeHandler);
	_shopBtn->onClickEndLEFT(handlers.shopHandler);
	_itemBtn->onClickEndLEFT(handlers.itemHandler);
	_statsBtn->onClickEndLEFT(handlers.statsHandler);
	_skillsBtn->onClickEndLEFT(handlers.skillsHandler);
	_pkButton->onStateSwitch(handlers.pkHandler);
	_leftSkillBtn->onClickEndLEFT(handlers.leftSkillHandler);
	_rightSkillBtn->onClickEndLEFT(handlers.rightSkillHandler);
}


void LeftHUD::updateHPAttributes(DWORD currentHP, DWORD maxHP) {
	const auto scale = (float)currentHP / (float)maxHP;
	_hpBar->updateScale(scale);

	char hpText[100] = { 0 };
	wsprintf(hpText, "HP: %d / %d", currentHP, maxHP);
	_hpLabel->updateTextTo(std::string(hpText));
}

void LeftHUD::updateSPAttributes(DWORD currentSP, DWORD maxSP) {
	/*const auto scale = (float)currentSP / (float)maxSP;
	_hpBar->updateScale(scale);*/
	char hpText[100] = { 0 };
	wsprintf(hpText, "SP: %d / %d", currentSP, maxSP);
	_spLabel->updateTextTo(std::string(hpText));
}

void LeftHUD::updateSkillSpriteLEFT(CustomUI::SpriteModel leftSkillSprite) {
	_leftSkillBtn->updateSpriteModelTo({
		leftSkillSprite,
		leftSkillSprite,
		leftSkillSprite
	});
}

void LeftHUD::updateSkillSpriteRIGHT(CustomUI::SpriteModel rightSkillSprite) {
	_rightSkillBtn->updateSpriteModelTo({
		rightSkillSprite,
		rightSkillSprite,
		rightSkillSprite
	});
}
