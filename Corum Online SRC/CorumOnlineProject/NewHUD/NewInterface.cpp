#include "NewInterface.h"
#include "../ItemWnd.h"
#include "../SkillWnd.h"
#include "../DungeonInterfaceLayout.h"
#include "HUDSpriteCollection.h"

using namespace CustomUI;
using namespace NewInterface;

Interface::Interface(CustomUI::Size screenSize,
					 CMainUser* mainUser,
					 LP_SKILL_LIST_MANAGER skillListManager) {
	_mainUser = mainUser;
	_skillListManager = skillListManager;
	_frameInParent = CustomUI::Rect{ {0, 0}, screenSize };
	const auto leftHUDOriginY = (long)(screenSize.height - LeftHUD::preferredSize().height);
	_leftHUD = registerChildRenderable<LeftHUD>([&]() {
		return new LeftHUD(Point{ 0,  leftHUDOriginY });
	});

	_skillSelectionView = registerChildRenderable<NewSkillSelectionView>([&]() {
		return new NewSkillSelectionView(
			{ {0, leftHUDOriginY - 200}, {200, 200} },
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
	handlers.tradeHandler = []() {

	};

	handlers.itemHandler = []() {
		CItemWnd* pItemWnd = CItemWnd::GetInstance();
		pItemWnd->SetActive(!pItemWnd->GetActive());
	};

	handlers.skillsHandler = []() {
		CSkillWnd* skillWnd = CSkillWnd::GetInstance();
		skillWnd->SetActive(!skillWnd->GetActive());
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
}

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

void Interface::renderWithRenderer(I4DyuchiGXRenderer* renderer, int zIndex) {
	Renderable::renderWithRenderer(renderer, zIndex);

	CItemWnd* pItemWnd = CItemWnd::GetInstance();
	if (pItemWnd->GetActive()) {
		pItemWnd->Render();
	}

	CSkillWnd* skillWnd = CSkillWnd::GetInstance();
	if (skillWnd->GetActive()) {
		skillWnd->Render();
	}
}



//CMainUserUpdateInterested

void Interface::updatedSkills(CMainUser* mainUser) {
	_skillSelectionView->updateCurrentSkills({
		mainUser->skillsAvailableOnLeft(),
		mainUser->skillsAvailableOnRight(),
		std::vector<BYTE>()
		});
}

void Interface::updatedLeftRightSkillSelection(CMainUser*) {
	updateLeftHUDWithSelectedLeftRightSkills();
}

void Interface::updatedCurrentHP(CMainUser* mainUser, DWORD oldValue, DWORD newValue) {
	_leftHUD->updateHPAttributes(mainUser->currentHP(), mainUser->maxHP());
}



void Interface::updatedMAXHP(CMainUser* mainUser, DWORD oldValue, DWORD newValue) {
	_leftHUD->updateHPAttributes(mainUser->currentHP(), mainUser->maxHP());
}

void Interface::updatedCurrentSP(CMainUser* mainUser, DWORD oldValue, DWORD newValue) {
	_leftHUD->updateSPAttributes(mainUser->currentSP(), mainUser->maxSP());
}

void Interface::updatedMAXSP(CMainUser* mainUser, DWORD oldValue, DWORD newValue) {
	_leftHUD->updateSPAttributes(mainUser->currentSP(), mainUser->maxSP());

}

void Interface::updatedEXP(CMainUser* mainUser, DWORD oldValue, DWORD newValue) {
	_leftHUD->updateEXPScale(mainUser->percentageEXP());
}

void Interface::updatedStatPoints(CMainUser*, DWORD oldValue, DWORD newValue) {

}

void Interface::updatedSkillPoints(CMainUser*, DWORD oldValue, DWORD newValue) {

}

void Interface::updatedCoolPoints(CMainUser*, float oldValue, float newValue) {

}

void Interface::updatedLevel(CMainUser* mainUser, DWORD oldValue, DWORD newValue) {
	_leftHUD->updateEXPScale(mainUser->percentageEXP());
}

///

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