#pragma once

#include "../GameControl.h"
#include "../InterfaceSpr.h"
#include "../NewUI/HorizontalScalingBar.h"
#include "../NewUI/CustomUIBase.h"
#include "../NewUI/Renderable.h"
#include "HUDSpriteCollection.h"
#include "../NewUI/ToggleButton.h"
#include "NewSkillSelectionView.h"
#include "../CMainUserUpdateInterested.h"
#include "../NewUI/SpriteRenderable.h"
#include "../NewUI/Label.h"

namespace NewInterface {
	typedef std::function<void()> EventHandler;
	typedef std::function<void(bool)> ToggleEventHandler;

	class LeftHUD : public CustomUI::Renderable {
	public:
		struct EventHandlers {
			EventHandler tradeHandler, shopHandler, itemHandler, statsHandler, skillsHandler, leftSkillHandler, rightSkillHandler;
			ToggleEventHandler pkHandler;
		};

		LeftHUD(CustomUI::Point originInParent);
		void updatePKToggle(bool isON);
		void updateHPScale(float scale);
		void updateHPAttributes(DWORD currentHP, DWORD maxHP);
		void updateSPAttributes(DWORD currentSP, DWORD maxSP);
		void updateEXPScale(float expScale);
		void updateSkillSpriteLEFT(CustomUI::SpriteModel leftSkillSprite);
		void updateSkillSpriteRIGHT(CustomUI::SpriteModel rightSkillSprite);

		void setEventHandlers(EventHandlers handlers);
	private:
		EventHandlers _handlers;
		std::shared_ptr<CustomUI::HorizontalScalingBar> _hpBar;
		std::shared_ptr<CustomUI::HorizontalScalingBar> _expBar;
		std::shared_ptr<CustomUI::Button> _tradeBtn, _shopBtn, _itemBtn, _statsBtn, _skillsBtn;
		std::shared_ptr<CustomUI::ToggleButton> _pkButton;
		std::shared_ptr<CustomUI::Button> _leftSkillBtn, _rightSkillBtn;
		std::shared_ptr<CustomUI::SpriteRenderable> _leftHUDSprite;
		std::shared_ptr<CustomUI::SingleLineLabel> _hpLabel, _spLabel;
	};
}


/*
int Order = __ORDER_USERINTERFACE_START_;

	InsertData(BUTTON_OBJ_ITEM1, BUTTON_INTERFACE_ITEM1, btnItem.x, btnItem.y, 1.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_CHR1, BUTTON_INTERFACE_CHR1, btnStats.x, btnStats.y, 1.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_SKILL1, BUTTON_INTERFACE_SKILL1, btnSkill.x, btnSkill.y, 1.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_GUILD1, BUTTON_INTERFACE_GUILD1, btnGroup.x, btnGroup.y, 1.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_PARTY1, BUTTON_INTERFACE_PARTY1, btnLair.x, btnLair.y, 1.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_CHAT1, BUTTON_INTERFACE_CHAT1, btnChat.x, btnChat.y, 1.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_ITEM2, BUTTON_INTERFACE_ITEM2, btnItem.x, btnItem.y, 1.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_CHR2, BUTTON_INTERFACE_CHR2, btnStats.x, btnStats.y, 1.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_SKILL2, BUTTON_INTERFACE_SKILL2, btnSkill.x, btnSkill.y, 1.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_GUILD2, BUTTON_INTERFACE_GUILD2, btnGroup.x, btnGroup.y, 1.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_PARTY2, BUTTON_INTERFACE_PARTY2, btnLair.x, btnLair.y, 1.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_CHAT2, BUTTON_INTERFACE_CHAT2, btnChat.x, btnChat.y, 1.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_SYSTEM1, BUTTON_INTERFACE_SYSTEM1, btnOptions.x, btnOptions.y, 1.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_SYSTEM2, BUTTON_INTERFACE_SYSTEM2, btnOptions.x, btnOptions.y, 1.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_PKON1, BUTTON_INTERFACE_PKON1, btnPK.x, btnPK.y, 1.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_PKON2, BUTTON_INTERFACE_PKON2, btnPK.x, btnPK.y, 1.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_PKOFF1, BUTTON_INTERFACE_PKOFF1, btnPK.x, btnPK.y, 1.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);
	InsertData(BUTTON_OBJ_PKOFF2, BUTTON_INTERFACE_PKOFF2, btnPK.x, btnPK.y, 1.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_MONSTERHP_BAR1, SPR_INTERFACE_MONSTERHP_BAR1, monsterHP1.x, monsterHP1.y, 1.0f, 1.0f, Order, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_MONSTERHP_BAR2, SPR_INTERFACE_MONSTERHP_BAR2, monsterHP2.x, monsterHP2.y, 1.0f, 1.0f, Order, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_MONSTER_GAGE1, SPR_INTERFACE_MONSTER_GAGE1, monsterGage.x, monsterGage.y, 1.0f, 1.0f, Order + 2, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_MONSTER_GAGE2, SPR_INTERFACE_MONSTER_GAGE2, monsterGage.x, monsterGage.y, 1.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);

	InsertData(SPR_OBJ_USERHP_BAR1, SPR_INTERFACE_USERHP_BAR1, monsterHP1.x, monsterHP1.y, 1.0f, 1.0f, Order, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_USERHP_BAR2, SPR_INTERFACE_USERHP_BAR2, monsterHP2.x, monsterHP2.y, 1.0f, 1.0f, Order, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_USER_GAGE1, SPR_INTERFACE_USER_GAGE1, monsterGage.x, monsterGage.y, 1.0f, 1.0f, Order + 2, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_USER_GAGE2, SPR_INTERFACE_USER_GAGE2, monsterGage.x, monsterGage.y, 1.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GUARDIAN_WND1, SPR_INTERFACE_GUARDIAN_WND1, 0, 0, 1.0f, 1.0f, Order, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GUARDIAN_EN, SPR_INTERFACE_GUARDIAN_EN, 0, 0, 1.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_UI_TRADE1, SPR_INTERFACE_UI_TRADE1, btnTrade.x, btnTrade.y, 1.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_UI_TRADE2, SPR_INTERFACE_UI_TRADE2, btnTrade.x, btnTrade.y, 1.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_UI_TRADE3, SPR_INTERFACE_UI_TRADE3, btnTrade.x, btnTrade.y, 1.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_UI_SHOP1, SPR_INTERFACE_UI_SHOP1, btnShop.x, btnShop.y, 1.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_UI_SHOP2, SPR_INTERFACE_UI_SHOP2, btnShop.x, btnShop.y, 1.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_UI_SHOP3, SPR_INTERFACE_UI_SHOP3, btnShop.x, btnShop.y, 1.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);

	InsertData(SPR_OBJ_UI_REST1, SPR_INTERFACE_UI_REST1, btnRest.x, btnRest.y, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_UI_REST2, SPR_INTERFACE_UI_REST2, btnRest.x, btnRest.y, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_UI_REST3, SPR_INTERFACE_UI_REST3, btnRest.x, btnRest.y, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_UI_MATCH1, SPR_INTERFACE_UI_MATCH1, btnMatch.x, btnMatch.y, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_UI_MATCH2, SPR_INTERFACE_UI_MATCH2, btnMatch.x, btnMatch.y, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_UI_MATCH3, SPR_INTERFACE_UI_MATCH3, btnMatch.x, btnMatch.y, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);

	InsertData(SPR_OBJ_UI_QUICKSLOT_INABLE1, SPR_INTERFACE_UI_QUICKSLOT_INABLE, beltItemPos(0).x, beltItemPos(0).y, 1.0f, 1.0f, __ORDER_USER_BLET__ + 1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_UI_QUICKSLOT_INABLE2, SPR_INTERFACE_UI_QUICKSLOT_INABLE, beltItemPos(1).x, beltItemPos(0).y, 1.0f, 1.0f, __ORDER_USER_BLET__+1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_UI_QUICKSLOT_INABLE3, SPR_INTERFACE_UI_QUICKSLOT_INABLE, beltItemPos(2).x, beltItemPos(0).y, 1.0f, 1.0f, __ORDER_USER_BLET__+1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_UI_QUICKSLOT_INABLE4, SPR_INTERFACE_UI_QUICKSLOT_INABLE, beltItemPos(3).x, beltItemPos(0).y, 1.0f, 1.0f, __ORDER_USER_BLET__+1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_UI_QUICKSLOT_INABLE5, SPR_INTERFACE_UI_QUICKSLOT_INABLE, beltItemPos(4).x, beltItemPos(0).y, 1.0f, 1.0f, __ORDER_USER_BLET__+1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_UI_QUICKSLOT_INABLE6, SPR_INTERFACE_UI_QUICKSLOT_INABLE, beltItemPos(5).x, beltItemPos(0).y, 1.0f, 1.0f, __ORDER_USER_BLET__+1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_UI_QUICKSLOT_INABLE7, SPR_INTERFACE_UI_QUICKSLOT_INABLE, beltItemPos(6).x, beltItemPos(0).y, 1.0f, 1.0f, __ORDER_USER_BLET__+1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_UI_QUICKSLOT_INABLE8, SPR_INTERFACE_UI_QUICKSLOT_INABLE, beltItemPos(7).x, beltItemPos(0).y, 1.0f, 1.0f, __ORDER_USER_BLET__+1, FALSE, FALSE, FALSE);

	// 가디언 스탯창 열기 버튼 : 최덕석 2005.3.29
	InsertData(SPR_OBJ_UI_GUARDIAN_STATUS, SPR_INTERFACE_UI_GUARDIAN_STATUS, 0, 49, 1.0f, 1.0f, Order+1, FALSE, FALSE, FALSE);
*/