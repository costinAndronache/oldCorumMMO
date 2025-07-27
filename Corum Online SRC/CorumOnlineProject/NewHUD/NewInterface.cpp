#include "NewInterface.h"
#include "../ItemWnd.h"
#include "../SkillWnd.h"
#include "../CharWnd.h"
#include "../DungeonInterfaceLayout.h"
#include "HUDSpriteCollection.h"
#include "ViewManager/DragNDropManager.h"

using namespace CustomUI;
using namespace NewInterface;

static void checkOldInterfaces(std::vector<CMenu*> oldInterfaces) {
	for (auto intf : oldInterfaces) {
		if (intf->GetActive()) {
			intf->Render();
			intf->RenderText();
		}
	}
}

static void renderOldInterfaces(std::vector<CMenu*> oldInterfaces) {
	for (auto intf : oldInterfaces) {
		if (intf->GetActive()) {
			intf->Render();
			intf->RenderText();
		}
	}
}

#pragma region Constructor
Interface::Interface(CustomUI::Size screenSize,
	CMainUser* mainUser,
	const LP_SKILL_LIST_MANAGER skillListManager,
	CItemResourceHash* resourceHash) {
	_mainUser = mainUser;
	_skillListManager = skillListManager;
	_frameInParent = CustomUI::Rect{ {0, 0}, screenSize };

	const auto hudSize = NewHUDResources::newHUDSize;
	const auto hudOriginY = (long)(screenSize.height - hudSize.height);
	_leftHUD = registerChildRenderable<LeftHUD>([&]() {
		return new LeftHUD(Point{ 0,  hudOriginY });
	});

	const auto rightHudOriginX = (long)(screenSize.width - hudSize.width);
	_rightHUD = registerChildRenderable<RightHUD>([=]() {
		return new RightHUD({ rightHudOriginX, hudOriginY }, resourceHash);
	});

	_skillSelectionView = registerChildRenderable<NewSkillSelectionView>([&]() {
		return new NewSkillSelectionView(
			{ {0, hudOriginY - 200}, {200, 200} },
			MatrixContainer::VerticalGrowthDirection::upwards,
			skillListManager
		);
	});


	_thisAsShared = std::shared_ptr<Interface>(this, [](Interface*) {
		// no destruction
	});

	mainUser->addUpdateListener(std::weak_ptr<Interface>(_thisAsShared));

	_leftHUD->updateEXPScale(mainUser->percentageEXP());
	_leftHUD->updateHPAttributes(mainUser->currentHP(), mainUser->maxHP());

	LeftHUD::EventHandlers handlers;
	handlers.statsHandler = [=]() {
		_statsView->toggleHiddenState();
	};

	handlers.itemHandler = [=]() {
		_newItemsWindow->toggleHiddenState();
	};

	handlers.skillsHandler = [=]() {
		_userSkillsView->toggleHiddenState();
	};

	handlers.leftSkillHandler = [=]() {
		_skillSelectionView->switchToActiveSelection(NewSkillSelectionView::ActiveSkillSelection::leftSkills);
	};

	handlers.rightSkillHandler = [=]() {
		_skillSelectionView->switchToActiveSelection(NewSkillSelectionView::ActiveSkillSelection::rightSkills);
	};

	_leftHUD->setEventHandlers(handlers);
	updateLeftHUDWithSelectedLeftRightSkills();

	_skillSelectionView->updateCurrentSkills({
		mainUser->skillsAvailableOnLeft(),
		mainUser->skillsAvailableOnRight(),
		std::vector<BYTE>()
		});

	_skillSelectionView->setHandlers({
		[=](BYTE leftSkillKind) {
			_mainUser->SetSkillChangeLR(leftSkillKind, 0);
			_skillSelectionView->switchToActiveSelection(NewSkillSelectionView::ActiveSkillSelection::none);
		},
		[=](BYTE rightSkillKind) {
			_mainUser->SetSkillChangeLR(rightSkillKind, 1);
			_skillSelectionView->switchToActiveSelection(NewSkillSelectionView::ActiveSkillSelection::none);
		},
		[=](BYTE guardianSkillKind) {
		// not yet
		_skillSelectionView->switchToActiveSelection(NewSkillSelectionView::ActiveSkillSelection::none);
	},
		});

	//

	_newItemsWindow = registerChildRenderable<NewItemsWindow>([=]() {
		return new NewItemsWindow({ 200, 200 }, resourceHash);
	});

	_newItemsWindow->onClose([=]() {
		_newItemsWindow->setHidden(true);
	});

	_newItemsWindow->setHidden(true);

	_mouseTracking = registerChildRenderable<MouseTrackingSpriteRenderable>([=]() {
		return new MouseTrackingSpriteRenderable(bounds());
	});
	_mouseTracking->updateZIndexOffsetForce(1000);

	_dragNDropSystem = new DragNDropSystem(this);
	_userInventoryManager = new UserInventoryManager(
		_newItemsWindow->groupedInventoryView(),
		ItemUsageManager::sharedInstance()
	);
	_equipItemsManager = new EquipItemsManager(_newItemsWindow->equipItemsView());

	_dragNDropManager = new DragNDropManager(
		mainUser,
		SharedNetwork::sharedInstance(),
		_dragNDropSystem
	);

	_dragNDropManager->setupRoutes(
		_rightHUD->beltDragNDropParticipant(),
		_userInventoryManager,
		_equipItemsManager
	);
	updatedItemInventory(mainUser);

	auto entriesCount = CharacterStatsManager::maxEntryCount();
	_statsView = registerChildRenderable<CharacterStatsView>([=]() {
		return new CharacterStatsView({
			{ 200, 200},
			{ 400, CharacterStatsView::appropriateSizeForElementsCountOnPage(entriesCount)}
			});
	});

	_statsView->onClose([=]() {
		_statsView->setHidden(true);
	});
	_statsView->setHidden(true);

	auto statusPointManager = new StatusPointManager(SharedNetwork::sharedInstance());
	_statsManager = new CharacterStatsManager(
		_statsView, statusPointManager, _mainUser
	);

	_statsManager->refreshCharacterStats();

	/// test skills
	auto skillsViewSize = UserSkillsView::appropriateSizeForMaxNumberOfSkillsPerList(UserSkillsManager::maxNumOfSkillsInList());

	_userSkillsView = registerChildRenderable<UserSkillsView>([=]() {
		return new UserSkillsView({ {200, 200}, skillsViewSize });
	});

	_userSkillsView->onClose([=]() {
		_userSkillsView->setHidden(true);
	});

	_userSkillsManager = new UserSkillsManager(
		_userSkillsView, 
		_mainUser, 
		SharedNetwork::sharedInstance(), 
		&g_sSkillListManager,
		g_pEffectLayer
	);

	_userSkillsManager->refreshUserSkillsView();
}


#pragma region Internals
void Interface::updateLeftHUDWithSelectedLeftRightSkills() {
	int nOrder = __ORDER_USERINTERFACE_START_ + 4;

	BYTE skillKind_left = _mainUser->GetSkillKind(SELECT_ATTACK_TYPE_LBUTTON);
	BYTE skillKind_right = _mainUser->GetSkillKind(SELECT_ATTACK_TYPE_RBUTTON);

	BYTE skillKind_guardian = [=]() {
		CMonster* pGuardian = _mainUser->m_pGuardian[0];
		if (pGuardian) {
			return pGuardian->GetSelectedSkill();
		}
		return (BYTE)MAX_SKILL;
	}();


	const auto leftSkillSprite = NewHUDResources::spriteForSkill (skillKind_left, _skillListManager);
	const auto rightSkillSprite = NewHUDResources::spriteForSkill(skillKind_right, _skillListManager);;

	_leftHUD->updateSkillSpriteLEFT(leftSkillSprite);
	_leftHUD->updateSkillSpriteRIGHT(rightSkillSprite);
}


//CMainUserUpdateInterested

void Interface::updatedSkills(CMainUser* mainUser) {
	_skillSelectionView->updateCurrentSkills({
		mainUser->skillsAvailableOnLeft(),
		mainUser->skillsAvailableOnRight(),
		std::vector<BYTE>() // guardian skills handled later
	});

	_userSkillsManager->refreshUserSkillsView();
}

void Interface::updatedItemInventory(CMainUser* user) {
	CItem items[MAX_BELT_POOL];
	user->copyBeltItemsInto(items);
	_rightHUD->beltDragNDropParticipant()->updateBeltWithItems(items);

	//
	std::vector<CItem> inventoryItemsSMALL(
		std::begin(user->m_pInv_Small), std::end(user->m_pInv_Small)
	);

	std::vector<CItem> inventoryItemsLARGE(
		std::begin(user->m_pInv_Large), std::end(user->m_pInv_Large)
	);

	_userInventoryManager->rebuildInventoryViewWith(
		inventoryItemsSMALL, inventoryItemsLARGE
	);

	//
	_equipItemsManager->updateWithItems(user->m_pEquip);
}

void Interface::updatedLeftRightSkillSelection(CMainUser*) {
	updateLeftHUDWithSelectedLeftRightSkills();
	_statsManager->refreshCharacterStats();
}

void Interface::updatedCurrentHP(CMainUser* mainUser, DWORD oldValue, DWORD newValue) {
	_leftHUD->updateHPAttributes(mainUser->currentHP(), mainUser->maxHP());
	_statsManager->refreshCharacterStats();
}


void Interface::updatedMAXHP(CMainUser* mainUser, DWORD oldValue, DWORD newValue) {
	_leftHUD->updateHPAttributes(mainUser->currentHP(), mainUser->maxHP());
	_statsManager->refreshCharacterStats();
}

void Interface::updatedCurrentSP(CMainUser* mainUser, DWORD oldValue, DWORD newValue) {
	_leftHUD->updateSPAttributes(mainUser->currentSP(), mainUser->maxSP());
	_rightHUD->updateSPScale(mainUser->percentageMP());
	_statsManager->refreshCharacterStats();
}

void Interface::updatedMAXSP(CMainUser* mainUser, DWORD oldValue, DWORD newValue) {
	_leftHUD->updateSPAttributes(mainUser->currentSP(), mainUser->maxSP());
	_statsManager->refreshCharacterStats();
}

void Interface::updatedEXP(CMainUser* mainUser, DWORD oldValue, DWORD newValue) {
	_leftHUD->updateEXPScale(mainUser->percentageEXP());
	_statsManager->refreshCharacterStats();
}

void Interface::updatedStatPoints(CMainUser*, DWORD oldValue, DWORD newValue) {
	_statsManager->refreshCharacterStats();
}

void Interface::updatedSkillPoints(CMainUser*, DWORD oldValue, DWORD newValue) {
	_userSkillsManager->refreshUserSkillsView();
}

void Interface::updatedCoolPoints(CMainUser* mainUser, float oldValue, float newValue) {
	_rightHUD->updateCooldownScale(mainUser->percentageCoolPoints());
	_statsManager->refreshCharacterStats();
}

void Interface::updatedLevel(CMainUser* mainUser, DWORD oldValue, DWORD newValue) {
	_leftHUD->updateEXPScale(mainUser->percentageEXP());
	_statsManager->refreshCharacterStats();
}

///

void Interface::handleMouseDown(Point mouseGlobalOrigin, MouseButton button) {
	Renderable::handleMouseDown(mouseGlobalOrigin, button);
}

void Interface::handleMouseUp(Point mouseGlobalOrigin, MouseButton button) {
	Renderable::handleMouseUp(mouseGlobalOrigin, button);
}

void Interface::handleMouseMove(Point mouseGlobalOrigin) {
	Renderable::handleMouseMove(mouseGlobalOrigin);
}

bool Interface::swallowsMouse(CustomUI::Point mouse) {
	if (_mouseTracking->isCurrentlyTracking()) { return true; }

	for (auto child : _childRenderables) {
		if (child->swallowsMouse(mouse) && child != _mouseTracking) {
			return true;
		}
	}
	return false;
}

void Interface::renderWithRenderer(I4DyuchiGXRenderer* renderer, int zIndex) {
	Renderable::renderWithRenderer(renderer, zIndex);
}


///

void Interface::renderOnMouseCursorAvatar(Renderable* avatar,
	std::function<void(CustomUI::Rect avatarCurrentGlobalFrame)> onLeftMouseButtonUP
) {
	_mouseTracking->trackWithNewRenderable(avatar, onLeftMouseButtonUP);
}

void Interface::clearCurrentMouseCursorAvatar() {
	_mouseTracking->trackWithNewRenderable(nullptr, nullptr);
}

/*

if ( nRt == NRET_BUTTON_ITEM )				// Item Inventory Wnd
		{
			if (g_pMainPlayer->m_bMatching)
				return 0;
			if(g_pMainPlayer->GetStatus()==UNIT_STATUS_PORTAL_MOVING)
				return 0;

			CItemWnd* pItemWnd = CItemWnd::GetInstance();
			pItemWnd->SetActive(!pItemWnd->GetActive());

			m_byOrderCheck = ITEM_WND;
		}
		else if ( nRt == NRET_BUTTON_CHR )		// Character Info Wnd
		{
			CCharWnd* pCharWnd = CCharWnd::GetInstance();
			pCharWnd->SetActive(!pCharWnd->GetActive());

			m_byOrderCheck = CHAR_WND;
		}
		else if ( nRt == NRET_BUTTON_SKILL )		// Skill Info Wnd
		{
			CSkillWnd* pSkillWnd = CSkillWnd::GetInstance();
			pSkillWnd->SetActive(!pSkillWnd->GetActive());

			m_byOrderCheck = SKILL_WND;
		}
		else if ( nRt == NRET_BUTTON_GUILD )		// Group Wnd
		{
			CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();
			pGroupWnd->SetActive(!pGroupWnd->GetActive());
		}
		else if ( nRt == NRET_BUTTON_LAIR )		// Dungeon Info Wnd
		{
			DungeonInfoProcForInterface();
		}
		else if ( nRt == NRET_BUTTON_CHAT )		// Chatting Wnd
		{
			ChattingProcForInterface();
		}
		else if ( nRt == NRET_BUTTON_SYSTEM )		// Game Option Wnd
		{
			CGameMenuWnd* pGameMenuWnd = CGameMenuWnd::GetInstance();
			pGameMenuWnd->SetActive(!pGameMenuWnd->GetActive());

			if(pGameMenuWnd->GetActive()==TRUE)
				pGameMenuWnd->OpenWnd();
		}
		else if ( nRt == NRET_BUTTON_PK )		// PK MODE ON / OFF
		{
			(m_nPK==0) ? SendPKMode( TRUE ) : SendPKMode( FALSE );
		}
		else if ( nRt == NRET_BUTTON_TRADE )		// TRADE
		{
			if(!g_pMainPlayer->m_bMatching)
			{
				SetPointer(__MOUSE_POINTER_GUARDIAN__);
				SetPointerChk( TRUE );
				m_bTrade	= TRUE;
			}
			else
			{
				// "대결 중에는 사용할 수 없습니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE501].szMessage, 0xFFFF2CFF);
			}

		}
		else if ( nRt == NRET_BUTTON_SHOP )		// PLAYER SHOP
		{
			if ( !PlayerShopProcForInterface() )	return 0;
		}
		else if ( nRt == NRET_BUTTON_REST )		// REST
		{
			if ( !RestProcForInterface() )			return 0;
		}
		else if ( nRt == NRET_BUTTON_MATCH )		// MATCH
		{
			if ( !MatchProcForInterface() )			return 0;
		}

*/

/*
m_pMenu[COMMAND_WND]			= CCommandWnd::GetInstance();
	m_pMenu[INTERFACE_USER]			= CUserInterface::GetInstance();
	m_pMenu[ITEM_WND]				= CItemWnd::GetInstance();
	m_pMenu[CHAR_WND]				= CCharWnd::GetInstance();
	m_pMenu[SKILL_WND]				= CSkillWnd::GetInstance();
	m_pMenu[GAMEMENU_WND]			= CGameMenuWnd::GetInstance();
	m_pMenu[MINIMAP_WND]			= CMiniMapWnd::GetInstance();
	m_pMenu[CHAT_WND]				= CChatWnd::GetInstance();
	m_pMenu[CHATLIST_WND]			= CChatListWnd::GetInstance();
	m_pMenu[ITEMSHOP_WND]			= CItemShopWnd::GetInstance();
	m_pMenu[NPCDUNGEON_INFO_WND]	= CNpcDungeonInfoWnd::GetInstance();
	m_pMenu[QUANTITY_WND]			= CQuantityWnd::GetInstance();
	m_pMenu[EXIT_WND]				= CExitWnd::GetInstance();
	m_pMenu[STORE_WND]				= CStoreWnd::GetInstance();
	m_pMenu[DUNGEONMANAGER_WND]		= CDungeonManagerWnd::GetInstance();
	m_pMenu[DUNGEONINFO_WND]		= CDungeonInfoWnd::GetInstance();
	m_pMenu[BANK_WND]				= CBankWnd::GetInstance();
	m_pMenu[GUILD_WND]				= CGuildWnd::GetInstance();
	m_pMenu[GUILDEX_WND]			= CGuildExWnd::GetInstance();
	m_pMenu[GUILDJOIN_WND]			= CGuildJoinWnd::GetInstance();
	m_pMenu[GUILDRANK_WND]			= CGuildRankWnd::GetInstance();
	m_pMenu[GUILDEDIT_WND]			= CGuildEditWnd::GetInstance();
	m_pMenu[PARTYJOIN_WND]			= CPartyJoinWnd::GetInstance();
	m_pMenu[DEFAULT_WND]			= CDefaultWnd::GetInstance();
	m_pMenu[HELP_WND]				= CHelpWnd::GetInstance();
	m_pMenu[ERROR_WND]				= CErrorWnd::GetInstance();
	m_pMenu[ITEMTRADE_WND]			= CItemTradeWnd::GetInstance();
	m_pMenu[ITEMTRADESHOP_WND]		= CItemTradeShopWnd::GetInstance();
	m_pMenu[OK_WND]					= COkWnd::GetInstance();
	m_pMenu[GROUP_WND]				= CGroupWnd::GetInstance();
	m_pMenu[PLAYERSHOP_WND]			= CPlayerShopWnd::GetInstance();
	m_pMenu[PARTYPORTAL_WND]		= CPartyPortalWnd::GetInstance();
	m_pMenu[MATCH_WND]				= CRequestMatchWnd::GetInstance();
	m_pMenu[DUNGEONSIEGEINFO_WND]	= CDungeonSiegeInfoWnd::GetInstance();
	m_pMenu[ITEMUPGRADE_WND]		= CItemUpgradeWnd::GetInstance();
	m_pMenu[EVENTDUNGEON_WND]		= CEventDungeonWnd::GetInstance();
	m_pMenu[NOTICE_WND]				= CNoticeWnd::GetInstance();
	m_pMenu[PARTYREQUEST_WND]		= CPartyRequestWnd::GetInstance();
	m_pMenu[INITITEM_WND]			= CInitItemWnd::GetInstance();
	m_pMenu[GUILDREQUEST_WND]		= CGuildRequestWnd::GetInstance();
	m_pMenu[MESSENGER_WND]			= CMessengerWnd::GetInstance();
	m_pMenu[GUILDDELETE_WND]		= CGuildDeleteWnd::GetInstance();
	m_pMenu[GAME_NOTICE_WND]		= CGameNoticeMessageWnd::GetInstance();
	m_pMenu[MESSENGERDELETE_WND]	= CMessengerDeleteWnd::GetInstance();
	m_pMenu[GUILDMASTER_WND]		= CGuildMasterWnd::GetInstance();
	m_pMenu[WORLD_WND]				= CWorldWnd::GetInstance();
	m_pMenu[CHARDIE_WND]			= CCharDieWnd::GetInstance();
	m_pMenu[GUILDPORTAL_WND]		= CGuildPortalWnd::GetInstance();
	m_pMenu[GUILDWAR_WND]			= CGuildWarWnd::GetInstance();
	m_pMenu[RANKNPC_WND]			= CRankNpcWnd::GetInstance();
	m_pMenu[RANK_WND]				= CRankWnd::GetInstance();
	m_pMenu[EVENT_NPC_WND]			= CEventNPCWnd::GetInstance();
	m_pMenu[EVENT_NPC_SUB_WND]		= CEventNPCSubWnd::GetInstance();
	m_pMenu[ITEMUPGRADE_SUB_WND]	= CItemUpgradeSubWnd::GetInstance();
	m_pMenu[ITEMUPGRADE_HELP_WND]	= CItemUpgradeHelpWnd::GetInstance();
	m_pMenu[CHAR_MOVE_WND]			= CCharMoveWnd::GetInstance();
	m_pMenu[CHAR_MOVERT_WND]		= CCharMoveRtWnd::GetInstance();
	m_pMenu[CHAR_MOVEEDIT_WND]		= CCharMoveEditWnd::GetInstance();
	m_pMenu[PARTY_BOARD_WND]		= CPartyBoardWnd::GetInstance();
	m_pMenu[PARTY_ENTRY_WND]		= CPartyEntryWnd::GetInstance();
	m_pMenu[PARTY_HELP_WND]			= CPartyHelpWnd::GetInstance();
	m_pMenu[PARTY_BOARDINFO_WND]	= CPartyBoardInfoWnd::GetInstance();
	m_pMenu[BLOCK_WND]				= CBlockWnd::GetInstance();
	m_pMenu[GAMEDEFAULT_WND]		= CGameDefaultWnd::GetInstance();
	m_pMenu[ITEMMAKING_WND]			= CItemMakingWnd::GetInstance();
	m_pMenu[ITEMMAKINGSUB_WND]		= CItemMakingSubWnd::GetInstance();
	m_pMenu[ITEMMAKINGHELP_WND]		= CItemMakingHelpWnd::GetInstance();
	m_pMenu[TOPDUNGEONHELP_WND]		= CTopDungeonHelpWnd::GetInstance();
	m_pMenu[GUARDIAN_WND]			= CGuardianWnd::GetInstance();
	m_pMenu[PARTYMATCHING_WND]		= CPartyMatchingWnd::GetInstance();
	m_pMenu[GUILDWAR_REQUEST_WND]	= CGuildWarRequest::GetInstance();
	// 전병렬->
	m_pMenu[GUILDWAR_FINALSETTING_WND] = CGuildWarFinalSettingWnd::GetInstance();
	m_pMenu[GUILDWAR_INVITE_WND] = CGuildWarInvite::GetInstance();
	// Menu 생성 //
	//LoadInterfaceFrameInfoCDB("InterfaceFrameInfo.cdb"); bad data in that file
	float genericWidth = 200.f;
	float genericHeight = 100.f;
	m_pMenu[COMMAND_WND]->CreateMenu("COMMAND", genericWidth, genericHeight, __MENU_TYPE_WORLD__, COMMAND_WND);
	m_pMenu[INTERFACE_USER]->CreateMenu("INTERFACE", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, INTERFACE_USER, TRUE);
	m_pMenu[ITEM_WND]->CreateMenu("ITEM", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, ITEM_WND);
	m_pMenu[CHAR_WND]->CreateMenu("CHAR", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, CHAR_WND);
	m_pMenu[SKILL_WND]->CreateMenu("SKILL", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, SKILL_WND);
	m_pMenu[GAMEMENU_WND]->CreateMenu("GAMEMENU", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, GAMEMENU_WND);
	m_pMenu[MINIMAP_WND]->CreateMenu("MINIMAP", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, MINIMAP_WND, TRUE);
	m_pMenu[CHAT_WND]->CreateMenu("CHAT", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, CHAT_WND);
	m_pMenu[CHATLIST_WND]->CreateMenu("CHATLIST", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, CHATLIST_WND);
	m_pMenu[ITEMSHOP_WND]->CreateMenu("ITEMSHOP", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, ITEMSHOP_WND);
	m_pMenu[NPCDUNGEON_INFO_WND]->CreateMenu("NPCDUNGEONINFO", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, NPCDUNGEON_INFO_WND);
	m_pMenu[QUANTITY_WND]->CreateMenu("QUANTITY", genericWidth, genericHeight, __MENU_TYPE_CHARACTER__, QUANTITY_WND);
	m_pMenu[STORE_WND]->CreateMenu("STORE", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, STORE_WND);
	m_pMenu[DUNGEONMANAGER_WND]->CreateMenu("DUNGEONMANAGER", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, DUNGEONMANAGER_WND);
	m_pMenu[DUNGEONINFO_WND]->CreateMenu("DUNGEONINFO", genericWidth, genericHeight, __MENU_TYPE_WORLD__, DUNGEONINFO_WND);
	m_pMenu[BANK_WND]->CreateMenu("BANK", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, BANK_WND);
	m_pMenu[GUILD_WND]->CreateMenu("GUILD", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, GUILD_WND);
	m_pMenu[GUILDEX_WND]->CreateMenu("GUILDEX", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, GUILDEX_WND);
	m_pMenu[GUILDJOIN_WND]->CreateMenu("GUILDJOIN", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, GUILDJOIN_WND);
	m_pMenu[GUILDRANK_WND]->CreateMenu("GUILDRANK", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, GUILDRANK_WND);
	m_pMenu[GUILDEDIT_WND]->CreateMenu("GUILDEDIT", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, GUILDEDIT_WND);
	m_pMenu[PARTYJOIN_WND]->CreateMenu("PRATYJOIN", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, PARTYJOIN_WND);
	m_pMenu[DEFAULT_WND]->CreateMenu("DEFAULT", genericWidth, genericHeight, __MENU_TYPE_CHARACTER__, DEFAULT_WND);
	m_pMenu[HELP_WND]->CreateMenu("HELP", genericWidth, genericHeight, __MENU_TYPE_WORLD__, HELP_WND);
	m_pMenu[ERROR_WND]->CreateMenu("ERROR", genericWidth, genericHeight, __MENU_TYPE_CHARACTER__, ERROR_WND);
	m_pMenu[ITEMTRADE_WND]->CreateMenu("ITEMTRADE", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, ITEMTRADE_WND);
	m_pMenu[ITEMTRADESHOP_WND]->CreateMenu("ITEMTRADESHOP", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, ITEMTRADESHOP_WND);
	m_pMenu[OK_WND]->CreateMenu("OK", genericWidth, genericHeight, __MENU_TYPE_WORLD__, OK_WND);
	m_pMenu[GROUP_WND]->CreateMenu("GROUP", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, GROUP_WND);
	m_pMenu[PLAYERSHOP_WND]->CreateMenu("PLAYERSHOP", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, PLAYERSHOP_WND);
	m_pMenu[PARTYPORTAL_WND]->CreateMenu("PARTYPORTAL", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, PARTYPORTAL_WND);
	m_pMenu[MATCH_WND]->CreateMenu("MATCH", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, MATCH_WND);
	m_pMenu[DUNGEONSIEGEINFO_WND]->CreateMenu("DUNGEONSIEGEINFO", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, DUNGEONSIEGEINFO_WND);
	m_pMenu[ITEMUPGRADE_WND]->CreateMenu("ITEMUPGRADE", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, ITEMUPGRADE_WND);
	m_pMenu[EVENTDUNGEON_WND]->CreateMenu("EVENTDUNGEON", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, EVENTDUNGEON_WND);
	m_pMenu[NOTICE_WND]->CreateMenu("NOTICE", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, NOTICE_WND);
	m_pMenu[PARTYREQUEST_WND]->CreateMenu("PARTYREQUEST", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, PARTYREQUEST_WND);
	m_pMenu[INITITEM_WND]->CreateMenu("INITITEM", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, INITITEM_WND);
	m_pMenu[GUILDREQUEST_WND]->CreateMenu("GUILDREQUEST", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, GUILDREQUEST_WND);
	m_pMenu[MESSENGER_WND]->CreateMenu("MESSENGER", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, MESSENGER_WND);
	m_pMenu[GUILDDELETE_WND]->CreateMenu("GUILDDELETE", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, GUILDDELETE_WND);
	m_pMenu[GAME_NOTICE_WND]->CreateMenu("GAMENOTICE", genericWidth, genericHeight, __MENU_TYPE_WORLD__, GAME_NOTICE_WND);
	m_pMenu[MESSENGERDELETE_WND]->CreateMenu("MESSENGERDELETE", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, MESSENGERDELETE_WND);
	m_pMenu[GUILDMASTER_WND]->CreateMenu("GUILDMASTER", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, GUILDMASTER_WND);
	m_pMenu[WORLD_WND]->CreateMenu("WORLD", genericWidth, genericHeight, __MENU_TYPE_CHARACTER__, WORLD_WND);
	m_pMenu[CHARDIE_WND]->CreateMenu("CHARDIE", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, CHARDIE_WND);
	m_pMenu[GUILDPORTAL_WND]->CreateMenu("GUILDPORTAL", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, GUILDPORTAL_WND);
	m_pMenu[GUILDWAR_WND]->CreateMenu("GUILDWAR", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, GUILDWAR_WND);
	m_pMenu[RANKNPC_WND]->CreateMenu("RANKNPC", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, RANKNPC_WND);
	m_pMenu[RANK_WND]->CreateMenu("RANK", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, RANK_WND);
	m_pMenu[EVENT_NPC_WND]->CreateMenu("EVENTNPC", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, EVENT_NPC_WND);
	m_pMenu[EVENT_NPC_SUB_WND]->CreateMenu("EVENTNPCSUB", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, EVENT_NPC_SUB_WND);
	m_pMenu[ITEMUPGRADE_SUB_WND]->CreateMenu("ITEMUPGRADESUB", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, ITEMUPGRADE_SUB_WND);
	m_pMenu[ITEMUPGRADE_HELP_WND]->CreateMenu("ITEMUPGRADEHELP", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, ITEMUPGRADE_HELP_WND);
	m_pMenu[CHAR_MOVE_WND]->CreateMenu("CHARMOVE", genericWidth, genericHeight, __MENU_TYPE_CHARACTER__, CHAR_MOVE_WND);
	m_pMenu[CHAR_MOVERT_WND]->CreateMenu("CHARMOVERT", genericWidth, genericHeight, __MENU_TYPE_CHARACTER__, CHAR_MOVERT_WND);
	m_pMenu[CHAR_MOVEEDIT_WND]->CreateMenu("CHARMOVEEDIT", genericWidth, genericHeight, __MENU_TYPE_CHARACTER__, CHAR_MOVEEDIT_WND);
	m_pMenu[PARTY_BOARD_WND]->CreateMenu("PARTYBOARD", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, PARTY_BOARD_WND);
	m_pMenu[PARTY_ENTRY_WND]->CreateMenu("PARTYENTRY", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, PARTY_ENTRY_WND);
	m_pMenu[PARTY_HELP_WND]->CreateMenu("PARTYHELP", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, PARTY_HELP_WND);
	m_pMenu[PARTY_BOARDINFO_WND]->CreateMenu("PARTYBOARDINFO", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, PARTY_BOARDINFO_WND);
	m_pMenu[BLOCK_WND]->CreateMenu("BLOCK", genericWidth, genericHeight, __MENU_TYPE_CHARACTER__, BLOCK_WND);
	m_pMenu[GAMEDEFAULT_WND]->CreateMenu("GAMEDEFAULT", genericWidth, genericHeight, __MENU_TYPE_CHARACTER__, GAMEDEFAULT_WND);
	m_pMenu[ITEMMAKING_WND]->CreateMenu("ITEMMAKING", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__,ITEMMAKING_WND);
	m_pMenu[ITEMMAKINGSUB_WND]->CreateMenu("ITEMMAKINGSUB", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, ITEMMAKINGSUB_WND);
	m_pMenu[ITEMMAKINGHELP_WND]->CreateMenu("ITEMMAKINGHELP", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, ITEMMAKINGHELP_WND);
	m_pMenu[TOPDUNGEONHELP_WND]->CreateMenu("TOPDUNGEONHELP", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, TOPDUNGEONHELP_WND);
	m_pMenu[GUARDIAN_WND]->CreateMenu("TOPDUNGEONHELP", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, GUARDIAN_WND);
	m_pMenu[PARTYMATCHING_WND]->CreateMenu("PARTYMATCHING_WND", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, PARTYMATCHING_WND);
	m_pMenu[GUILDWAR_REQUEST_WND]->CreateMenu("GUILDWAR_REQUEST_WND", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, GUILDWAR_REQUEST_WND);
	m_pMenu[GUILDWAR_FINALSETTING_WND]->CreateMenu("GUILDWAR_FINALSETTING_WND", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, GUILDWAR_FINALSETTING_WND);
	m_pMenu[GUILDWAR_INVITE_WND]->CreateMenu("GUILDWAR_INVITE_WND", genericWidth, genericHeight, __MENU_TYPE_DEFAULT__, GUILDWAR_INVITE_WND);
	m_pMenu[EXIT_WND]->CreateMenu("EXIT", EXITWND_SIZE_X, EXITWND_SIZE_Y, __MENU_TYPE_WORLD__, EXIT_WND);
*/