//==========================================================//
// Code by Jang.								2002.12.17	//
//==========================================================//
#include "UserInterface.h"
#include "CodeFun.h"
#include "ItemWnd.h"
#include "CharWnd.h"
#include "SkillWnd.h"
#include "GameMenuWnd.h"
#include "DSMiniMap.h"
#include "Interface.h"
#include "ChatWnd.h"
#include "SpriteManager.h"
#include "User.h"
#include "GameControl.h"
#include "DefResource.h"
#include "QuantityWnd.h"
#include "ItemWnd.h"
#include "ObjectManager.h"
#include "GMTool.h"
#include "GuildExWnd.h"
#include "GuildRankWnd.h"
#include "CorumResource.h"
#include "HashTable.h"
#include "dungeontable.h"
#include "dungeon_data_ex.h"
#include "message.h"
#include "GroupWnd.h"
#include "PlayerShopWnd.h"
#include "Chat.h"
#include "CDungeonInfoWnd.h"
#include "RequestMatchWnd.h"
#include "InputManager.h"
#include "ChatBackground.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "Effect.h"
#include "InitGame.h"
#include "ItemTradeShopWnd.h"
#include "ItemNative.h"
#include "GuardianWnd.h"
#include "SkillSelectionView.h"

//////////////////////////////////////////////////////////////////////////
// Temporary File for Interface Process
#include "InterfaceManager.h"
#include "InterfaceProcTemp.h"
#include "DungeonInterfaceLayout.h"

std::shared_ptr<CUserInterface> CUserInterface::_shared(nullptr);

void GXClickProc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{
	if(bFrameFlag==FRAME_FLAG_FINISHED || bFrameFlag == FRAME_FLAG_CHANGE_NEXT )	
		HideObject(handle);
}

void GXMoveProc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{	
}

//==========================================================//
// Construction/Destruction.								//
//==========================================================//
CUserInterface::CUserInterface()
{ 
	for(int i = 0; i < 2; i++)
	{
		m_fEnIndex[i]	= 0;
		m_fEnSpeed[i]	= 0;
		m_fManaIndex[i]	= 0;
		m_fManaSpeed[i]	= 0;
		
		m_bEn[i]		= FALSE;
		m_bMana[i]		= FALSE;
	}	

	for(int i = 0; i < 4; i++)
		m_bUIChk[i] = FALSE;

	m_dwMagicArray		= 0;
	m_byMoveType		= 0;
	m_byOrderCheck		= 0;
	m_byOrder			= 0;
	m_fExpIndex			= 0;
	m_fExpSpeed			= 0;
	m_fEnDecSpeed		= 0;
	m_fEnIncSpeed		= 0;	
	m_fManaIncDec		= 0;
	m_fManaDecSpeed		= 0;
	m_fManaIncSpeed		= 0;
	m_nChatChk			= 0;		
	m_nPK				= 0;
	m_wMaxSkillLevel	= __MAX_SKILL_UP__;
	m_wMaxMasteryLevel	= __MAX_MASTERYSKILL_UP__;
	m_byPointerType		= 255;
	m_bTrade			= FALSE;
	m_bMatch			= FALSE;
	m_bRGW				= FALSE;		// RivalGuildWar
	m_bCommand			= TRUE;
	m_bParty			= TRUE;
	m_bGuardian			= FALSE;
	m_bSoundChk			= FALSE;
	m_bUserInterface	= FALSE;
	m_bMouseIcon		= FALSE;
	m_bSmall			= FALSE;
	m_bClick			= FALSE;
	m_bUp				= FALSE;
	m_bDown				= FALSE;	
	m_bSkillRender		= 0;
	m_bMonster			= FALSE;
	m_bMoveChr			= FALSE;
	m_fMonMaxHp			= 0;
	m_fMonHp			= 0;
	m_dwMonCurTime		= 0;
	m_dwMonPrevTime		= 0;
	m_dwUserCurTime		= 0;
	m_dwUserPrevTime		= 0;
	m_bMatchUserHPShowFlag = 0;
	m_bMatchUserHPShowEnable = FALSE;
	m_dwMagicArray	= 0;

	m_pMonster			= NULL;
	m_pUser				= NULL;	

	int nMousePos[__MAX_MOUSE_POINTER__*2] = 
	{
		2, 3, 2, 3, 2, 3, 10, 2, 10, 2, 15, 8, 15, 8, 15, 8, 1, 1, 1, 1, 1, 1, 7, 19, 7, 19, 10, 10, 10, 10, 10, 10, 16, 16, 16, 16
	};
	
	for(int i = 0; i < __MAX_MOUSE_POINTER__*2; i++)
		m_nMousePos[i]	= nMousePos[i];	
	
	// Mouse Move Effect //
}

CUserInterface::~CUserInterface()
{
}
//==========================================================//
// Class Member Function.									//
//==========================================================//
BOOL CUserInterface::Init()
{
	m_pStatusWnd = CGuildWarStatusWnd::GetInstance();

	for(int i = 0; i < 2; i++)
	{
		m_fManaIndex[i]	= 0;
		m_fEnIndex[i]	= 0;		
		m_bEn[i]		= FALSE;
		m_bMana[i]		= FALSE;		
	}	

	m_fEnSpeed[0]	= 1.2f;
	m_fEnSpeed[1]	= 1.5f;
	m_fEnIncSpeed	= 1.0f;
	m_fEnDecSpeed	= 1.0f;
	m_fExpIndex		= 0.0f;
	m_fManaSpeed[0]	= 1.2f;
	m_fManaSpeed[1]	= 1.5f;
	m_fManaIncSpeed	= 0.6f;
	m_fManaDecSpeed	= 0.6f;
	m_fExpSpeed		= 1.3f;	
	m_bGuardian		= FALSE;
	m_dwMagicArray	= 0;
	
	InsertUIData();				// Insert User Interface Sprite Data

	SetRenderDefaultUI();		// Set Default User Interface Sprite to be Rendered


	SetScalingObjX(SPR_OBJ_CAST, 1023 - (float)g_pMainPlayer->percentageCoolPoints() * 300);	

	// 버그 //	
	if(m_nPK==0)
		SetRender(BUTTON_OBJ_PKOFF1, TRUE);
	else
		SetRender(BUTTON_OBJ_PKON1, TRUE);	
		
	m_pUserClickHandle.pDesc				= AllocObjDesc();	
	m_pUserClickHandle.pDesc->ObjectFunc	= GXClickProc;
	m_pUserClickHandle.pHandle	= CreateHandleObject(g_pObjManager->GetFile(MOD_PLAYERCLICK), GXPlayerPROC, m_pUserClickHandle.pDesc, GXOBJECT_CREATE_TYPE_EFFECT);		
	HideObject(m_pUserClickHandle.pHandle);
				
	char szFile[0xff]={0,};
	
	lstrcpy(szFile, "e0121000.chr");
	m_pUserMouseHandle[0].pDesc				= AllocObjDesc();			
	m_pUserMouseHandle[0].pHandle			= CreateHandleObject(GetFile(szFile, DATA_TYPE_EFFECT), GXPlayerPROC, m_pUserMouseHandle[0].pDesc, GXOBJECT_CREATE_TYPE_EFFECT);
	HideObject(m_pUserMouseHandle[0].pHandle);

	lstrcpy(szFile, "e0125000.chr");
	m_pUserMouseHandle[1].pDesc				= AllocObjDesc();			
	m_pUserMouseHandle[1].pHandle			= CreateHandleObject(GetFile(szFile, DATA_TYPE_EFFECT), GXPlayerPROC, m_pUserMouseHandle[1].pDesc, GXOBJECT_CREATE_TYPE_EFFECT);
	HideObject(m_pUserMouseHandle[1].pHandle);

	lstrcpy(szFile, "e0128000.chr");
	m_pUserMouseHandle[2].pDesc				= AllocObjDesc();			
	m_pUserMouseHandle[2].pHandle			= CreateHandleObject(GetFile(szFile, DATA_TYPE_EFFECT), GXPlayerPROC, m_pUserMouseHandle[2].pDesc, GXOBJECT_CREATE_TYPE_EFFECT);
	HideObject(m_pUserMouseHandle[2].pHandle);

	m_pSprPlayerShop[0] = g_pRenderer->CreateSpriteObject(GetFile("menu_5.tga", DATA_TYPE_UI), 128, 48, 128, 19, 0);
	m_pSprPlayerShop[1] = g_pRenderer->CreateSpriteObject(GetFile("menu_5.tga", DATA_TYPE_UI), 128, 67, 128, 32, 0);			

	SetMousePointerType(__MOUSE_POINTER_DEFAULT__);
	SetMousePointerPos((float)g_Mouse.MousePos.x , (float)g_Mouse.MousePos.y);	
	
	m_pMonster	= NULL;
	m_pUser		= NULL;
	m_bInit		= TRUE;

	m_pStatusWnd->Init();
	return TRUE;
}

void	CUserInterface::CreateMenu(
		char*	pszName, 
		float	fWidth, 
		float	fHeight, 
		BYTE	byType, 
		int		nIndex, 
		BOOL	bActive, 
		BOOL	bScroll
		)
{
	m_pStatusWnd = CGuildWarStatusWnd::GetInstance();
	
	m_pStatusWnd->CreateMenu( 
		pszName, 
		fWidth, 
		fHeight, 
		byType, 
		nIndex, 
		bActive, 
		false );

	CMenu::CreateMenu( 
		pszName, 
		fWidth, 
		fHeight, 
		byType, 
		nIndex, 
		bActive, 
		bScroll );
}


void CUserInterface::InsertUIData()
{
	int Order = __ORDER_USERINTERFACE_START_;

	InsertData(SPR_OBJ_UI_LEFT, SPR_INTERFACE_UI_LEFT, hudLeft.x, hudLeft.y, 1.0f, 1.0f, Order, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_UI_RIGHT, SPR_INTERFACE_UI_RIGHT, hudRight.x, hudRight.y, 1.0f, 1.0f, Order, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_EN2, SPR_INTERFACE_EN2, hp.x, hp.y, 0.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_EN1, SPR_INTERFACE_EN1, hp.x, hp.y, 0.0f, 1.0f, Order + 2, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_EXP, SPR_INTERFACE_EXP, expPos.x, expPos.y, 1.0f, 1.0f, Order + 3, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_MANA2, SPR_INTERFACE_MANA2, 1023, 709, 0.0f, 1.0f, Order + 1, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_MANA1, SPR_INTERFACE_MANA1, sp.x, sp.y, 0.0f, 1.0f, Order + 2, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_CAST, SPR_INTERFACE_CAST, cool.x, cool.y, (float)g_pMainPlayer->percentageCoolPoints() * 300 / 2, 1.0f, Order + 3, FALSE, FALSE, FALSE);

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
}

void CUserInterface::SetRenderDefaultUI()
{
	SetRender(SPR_OBJ_UI_TRADE1, TRUE);
	SetRender(SPR_OBJ_UI_SHOP1, TRUE);
	SetRender(SPR_OBJ_UI_REST1, TRUE);
	SetRender(SPR_OBJ_UI_MATCH1, TRUE);	
	SetRender(SPR_OBJ_UI_LEFT, TRUE);
	SetRender(SPR_OBJ_UI_RIGHT, TRUE);
	SetRender(SPR_OBJ_EN2, TRUE);
	SetRender(SPR_OBJ_EN1, TRUE);
	SetRender(SPR_OBJ_EXP, TRUE);
	SetRender(SPR_OBJ_MANA2, TRUE);
	SetRender(SPR_OBJ_MANA1, TRUE);
	SetRender(SPR_OBJ_CAST, TRUE);
}

void CUserInterface::SetPointerChk(BOOL bChk)
{
	m_bPointerChk = bChk;
}

BOOL CUserInterface::GetPointerChk()
{
	return m_bPointerChk;
}

void CUserInterface::SetOrder()
{
}

int	CUserInterface::GetChkOrderIndex()
{
	return m_byOrderCheck;
}

void CUserInterface::Remove()
{
	SetMousePointerType(__MOUSE_POINTER_DEFAULT__);

	RemoveAllData();
	
	if(m_pUserClickHandle.pHandle)
	{
		DeleteHandleObject(m_pUserClickHandle.pHandle);
		m_pUserClickHandle.pHandle = NULL;
	}
	if(m_pUserClickHandle.pDesc)	
	{
		FreeObjDesc( m_pUserClickHandle.pDesc);
		m_pUserClickHandle.pDesc = NULL;
	}
	
	if(m_pUserMouseHandle[0].pHandle)
	{
		DeleteHandleObject(m_pUserMouseHandle[0].pHandle);
		m_pUserMouseHandle[0].pHandle = NULL;
	}
	if(m_pUserMouseHandle[0].pDesc)	
	{
		FreeObjDesc(m_pUserMouseHandle[0].pDesc);
		m_pUserMouseHandle[0].pDesc = NULL;
	}
	
	if(m_pUserMouseHandle[1].pHandle)
	{
		DeleteHandleObject(m_pUserMouseHandle[1].pHandle);
		m_pUserMouseHandle[1].pHandle = NULL;
	}
	if(m_pUserMouseHandle[1].pDesc)	
	{
		FreeObjDesc(m_pUserMouseHandle[1].pDesc);
		m_pUserMouseHandle[1].pDesc = NULL;
	}
	
	if(m_pUserMouseHandle[2].pHandle)
	{
		DeleteHandleObject(m_pUserMouseHandle[2].pHandle);
		m_pUserMouseHandle[2].pHandle = NULL;
	}
	if(m_pUserMouseHandle[2].pDesc)	
	{
		FreeObjDesc(m_pUserMouseHandle[2].pDesc);
		m_pUserMouseHandle[2].pDesc = NULL;
	}

	if(m_pSprPlayerShop[0])
		m_pSprPlayerShop[0]->Release();
	if(m_pSprPlayerShop[1])
		m_pSprPlayerShop[1]->Release();

	m_byPointerType	= 255;
	
	m_bActive	= FALSE;
	m_bInit		= FALSE;

	
}

enum NRET {
	NRET_OUTSIDE_HUD = 0,
	NRET_JUST_INSIDE_HUD = 1,
	NRET_BUTTON_ITEM = 2,
	NRET_BUTTON_CHR = 3,
	NRET_BUTTON_SKILL = 4,
	NRET_BUTTON_GUILD = 5,
	NRET_BUTTON_LAIR = 6,
	NRET_BUTTON_CHAT = 7,
	NRET_BUTTON_SYSTEM = 8,
	NRET_BUTTON_PK = 13,
	NRET_GUARDIAN_INTERFACE1 = 23,
	NRET_GUARDIAN_POTION = 15,
	NRET_GUARDIAN_SKILL = 21,
	NRET_ITEMBELT_AREA = 14,
	NRET_BUTTON_TRADE = 16,
	NRET_BUTTON_SHOP = 17,
	NRET_BUTTON_REST = 18,
	NRET_BUTTON_MATCH = 19
};

int	CUserInterface::GetChk()
{
	const bool m_bGuardian = true;

	const auto mousePoint = VECTOR2{ (float)g_Mouse.MousePos.x, (float)g_Mouse.MousePos.y };

	CSkillWnd* pSkillWnd = CSkillWnd::GetInstance();
	CMonster* pGuardian = g_pMainPlayer->m_pGuardian[0];
	if (SkillSelectionView::sharedInstance()->isMouseHovering()) {
		return 22;
	}

	if (m_bGuardian) {
		// 가디언 정보창 버튼
		if (pointInsideRect(mousePoint,
			guardianInterface1.origin,
			guardianInterface1.size.x,
			guardianInterface1.size.y)) {
			return NRET_GUARDIAN_INTERFACE1;
		}


		// 가디언 아이템
		if (pointInsideRect(mousePoint, guardianPotionPos)) {
			return NRET_GUARDIAN_POTION;
		}


		// 가디언 스킬 선택 : 최덕석 2005.3.3
		if (pointInsideRect(mousePoint, guardianSkillPos)) {
			return NRET_GUARDIAN_SKILL;
		}

	}

	if ( pointInsideRect(mousePoint, leftHUD) || pointInsideRect(mousePoint, rightHUD)) {
		if (pointInsideRect(mousePoint, btnItem)) return NRET_BUTTON_ITEM;

		if (pointInsideRect(mousePoint, btnStats)) return NRET_BUTTON_CHR;

		if (pointInsideRect(mousePoint, btnSkill)) return NRET_BUTTON_SKILL;

		if (pointInsideRect(mousePoint, btnGroup)) return NRET_BUTTON_GUILD;

		if (pointInsideRect(mousePoint, btnLair)) return NRET_BUTTON_LAIR;

		if (pointInsideRect(mousePoint, btnChat)) return NRET_BUTTON_CHAT;

		if (pointInsideRect(mousePoint, btnOptions)) return NRET_BUTTON_SYSTEM;


		if (pointInsideRect(mousePoint, btnPK))
			return NRET_BUTTON_PK;

		else if (g_Mouse.MousePos.x >= 727 && g_Mouse.MousePos.x <= 1024 && g_Mouse.MousePos.y >= 733 && g_Mouse.MousePos.y <= 768)
			return NRET_ITEMBELT_AREA;

		if (pointInsideRect(mousePoint, btnTrade))
			return NRET_BUTTON_TRADE;	// 교환.

		if (pointInsideRect(mousePoint, btnShop))
			return NRET_BUTTON_SHOP;	// 앉기.

		if (pointInsideRect(mousePoint, btnRest))
			return NRET_BUTTON_REST;

		if (pointInsideRect(mousePoint, btnMatch))
			return NRET_BUTTON_MATCH;	// 대결.

		return NRET_JUST_INSIDE_HUD;
	}

	return NRET_OUTSIDE_HUD;
}


int CUserInterface::CheckInterface()
{
	m_bSkillRender	= 0;
	m_bMouseIcon	= FALSE;
		
	int nRt = GetChk();

	if(nRt>=1 && nRt<=23)
		m_bMouseIcon = TRUE;
	
	if (g_Mouse.bLDown) {
		const VECTOR2 mousePoint = { (float)g_Mouse.MousePos.x, (float)g_Mouse.MousePos.y };
		SkillSelectionView::sharedInstance()->handleMouseButtonDownEvent(mousePoint);

		if (!m_bSoundChk) {
			if (nRt > 1)
			{
				if (nRt != 14)
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_BTNCLICK, g_Camera.v3CameraPos, FALSE);
			}
			m_bSoundChk = TRUE;
		}
	}

	// 대전중 
	if( (g_pMainPlayer->m_bMatching) || (g_pMainPlayer->m_dwGuildWarFlag == G_W_F_OBSERVER && g_pThisDungeon->IsStadium()) )
	{ 
		// 못쓰게 가려라.
		EnableQuickSlot(FALSE);	
	}
	else
	{
		EnableQuickSlot(TRUE);
	}

	if(m_bUIChk[0]==TRUE && nRt!=17)
	{
		SetRender(SPR_OBJ_UI_TRADE1, TRUE);
		SetRender(SPR_OBJ_UI_TRADE2, FALSE);
		SetRender(SPR_OBJ_UI_TRADE3, FALSE);
		m_bUIChk[0] = FALSE;
	}
	if(m_bUIChk[1]==TRUE && nRt!=18)
	{
		SetRender(SPR_OBJ_UI_REST1, TRUE);
		SetRender(SPR_OBJ_UI_REST2, FALSE);
		SetRender(SPR_OBJ_UI_REST3, FALSE);
		m_bUIChk[1] = FALSE;
	}
	if(m_bUIChk[2]==TRUE && nRt!=19)
	{
		SetRender(SPR_OBJ_UI_SHOP1, TRUE);
		SetRender(SPR_OBJ_UI_SHOP2, FALSE);
		SetRender(SPR_OBJ_UI_SHOP3, FALSE);
		m_bUIChk[2] = FALSE;
	}
	if(m_bUIChk[3]==TRUE && nRt!=20)
	{
		SetRender(SPR_OBJ_UI_MATCH1, TRUE);
		SetRender(SPR_OBJ_UI_MATCH2, FALSE);
		SetRender(SPR_OBJ_UI_MATCH3, FALSE);
		m_bUIChk[3] = FALSE;
	}

	switch(nRt)
	{
	case NRET_BUTTON_ITEM:
		{
			SetRender(BUTTON_OBJ_ITEM1, TRUE);
			SetPointer(__MOUSE_POINTER_BUTTON__);
		}		
		break;
	case NRET_BUTTON_CHR:
		{
			SetRender(BUTTON_OBJ_CHR1, TRUE);
			SetPointer(__MOUSE_POINTER_BUTTON__);
		}
		break;
	case NRET_BUTTON_SKILL:
		{
			SetRender(BUTTON_OBJ_SKILL1, TRUE);
			SetPointer(__MOUSE_POINTER_BUTTON__);
		}
		break;
	case NRET_BUTTON_GUILD:
		{
			SetRender(BUTTON_OBJ_GUILD1, TRUE);
			SetPointer(__MOUSE_POINTER_BUTTON__);
		}				
		break;
	case NRET_BUTTON_LAIR:
		{
			SetRender(BUTTON_OBJ_PARTY1, TRUE);
			SetPointer(__MOUSE_POINTER_BUTTON__);
		}		
		break;
	case NRET_BUTTON_CHAT:
		{
			SetRender(BUTTON_OBJ_CHAT1, TRUE);
			SetPointer(__MOUSE_POINTER_BUTTON__);
		}
		break;
	case NRET_BUTTON_SYSTEM:
		{
			SetRender(BUTTON_OBJ_SYSTEM1, TRUE);
			SetPointer(__MOUSE_POINTER_BUTTON__);
		}
		break;	
	case NRET_GUARDIAN_SKILL:
		if(g_Mouse.bLDown)		
			SetPointer(__MOUSE_POINTER_BUTTONCLICK__);	
		else
			SetPointer(__MOUSE_POINTER_BUTTON__);		
		break;
	case NRET_BUTTON_PK:
		{
			SetPointer(__MOUSE_POINTER_BUTTON__);
			
			if(m_nPK==0)
			{
				SetRender(BUTTON_OBJ_PKOFF1, FALSE);
				SetRender(BUTTON_OBJ_PKOFF2, TRUE);
			}
			else
			{
				SetRender(BUTTON_OBJ_PKON1, FALSE);
				SetRender(BUTTON_OBJ_PKON2, TRUE);
			}
		}
		break;
	case NRET_ITEMBELT_AREA:
		if( !g_pThisDungeon->IsStadium() || g_pMainPlayer->m_dwGuildWarFlag != G_W_F_OBSERVER )
			if ( !BeltProcForInterface() )	return 0;

		break;

	case NRET_GUARDIAN_POTION: // 가디언 
		GuardianProcForInterface();
		break;
	case NRET_GUARDIAN_INTERFACE1:	// 가디언 정보창 버튼
		if(g_Mouse.bLDown)
		{
			SetPointer(__MOUSE_POINTER_BUTTONCLICK__);
		}
		else
		{
			SetPointer(__MOUSE_POINTER_BUTTON__);
		}
		break;
	case NRET_BUTTON_TRADE:	// 트레이드 //
		{
			if(g_Mouse.bLDown)
			{
				m_bUIChk[0] = TRUE;
				SetRender(SPR_OBJ_UI_TRADE1, FALSE);
				SetRender(SPR_OBJ_UI_TRADE2, FALSE);
				SetRender(SPR_OBJ_UI_TRADE3, TRUE);
				SetPointer(__MOUSE_POINTER_BUTTONCLICK__);				
			}
			else if(!g_Mouse.bLDown && !m_bUp)
			{
				m_bUIChk[0] = TRUE;
				SetRender(SPR_OBJ_UI_TRADE1, FALSE);
				SetRender(SPR_OBJ_UI_TRADE2, TRUE);
				SetRender(SPR_OBJ_UI_TRADE3, FALSE);
				SetRender(SPR_OBJ_UI_SHOP1, TRUE);
				SetRender(SPR_OBJ_UI_SHOP2, FALSE);
				SetRender(SPR_OBJ_UI_SHOP3, FALSE);
				SetPointer(__MOUSE_POINTER_BUTTON__);
			}
		}
		break;
	case NRET_BUTTON_SHOP:	// 노점상 //
		{
			if(g_Mouse.bLDown)
			{
				m_bUIChk[2] = TRUE;
				SetRender(SPR_OBJ_UI_SHOP1, FALSE);
				SetRender(SPR_OBJ_UI_SHOP2, FALSE);
				SetRender(SPR_OBJ_UI_SHOP3, TRUE);
				SetPointer(__MOUSE_POINTER_BUTTONCLICK__);
			}
			else if(!g_Mouse.bLDown)
			{
				m_bUIChk[2] = TRUE;
				SetRender(SPR_OBJ_UI_SHOP1, FALSE);
				SetRender(SPR_OBJ_UI_SHOP2, TRUE);
				SetRender(SPR_OBJ_UI_SHOP3, FALSE);
				SetRender(SPR_OBJ_UI_TRADE1, TRUE);
				SetRender(SPR_OBJ_UI_TRADE2, FALSE);
				SetRender(SPR_OBJ_UI_TRADE3, FALSE);
				SetPointer(__MOUSE_POINTER_BUTTON__);
			}
		}
		break;
	case NRET_BUTTON_REST:	// 앉기 //
		{
			if(g_Mouse.bLDown)
			{
				m_bUIChk[1] = TRUE;
				SetRender(SPR_OBJ_UI_REST1, FALSE);
				SetRender(SPR_OBJ_UI_REST2, FALSE);
				SetRender(SPR_OBJ_UI_REST3, TRUE);
				SetPointer(__MOUSE_POINTER_BUTTONCLICK__);

			}
			else if(!g_Mouse.bLDown)
			{
				m_bUIChk[1] = TRUE;
				SetRender(SPR_OBJ_UI_REST1, FALSE);
				SetRender(SPR_OBJ_UI_REST2, TRUE);
				SetRender(SPR_OBJ_UI_REST3, FALSE);
				SetPointer(__MOUSE_POINTER_BUTTON__);
			}			
		}
		break;
	case NRET_BUTTON_MATCH:	// 1:1 대결 //
		{
			if(g_Mouse.bLDown)
			{
				m_bUIChk[3] = TRUE;
				SetRender(SPR_OBJ_UI_MATCH1, FALSE);
				SetRender(SPR_OBJ_UI_MATCH2, FALSE);
				SetRender(SPR_OBJ_UI_MATCH3, TRUE);
				SetPointer(__MOUSE_POINTER_BUTTONCLICK__);
			}
			else if(!g_Mouse.bLDown && !m_bUp)
			{
				m_bUIChk[3] = TRUE;
				SetRender(SPR_OBJ_UI_MATCH1, FALSE);
				SetRender(SPR_OBJ_UI_MATCH2, TRUE);
				SetRender(SPR_OBJ_UI_MATCH3, FALSE);
				SetPointer(__MOUSE_POINTER_BUTTON__);
			}
		}
		break;
	case NRET_OUTSIDE_HUD:
	case NRET_JUST_INSIDE_HUD:

		if(nRt == NRET_JUST_INSIDE_HUD)
			if(!g_Mouse.bLDown)
				SetPointer(__MOUSE_POINTER_DEFAULT__);
					
		SetRender(BUTTON_OBJ_ITEM1, FALSE);
		SetRender(BUTTON_OBJ_CHR1, FALSE);
		SetRender(BUTTON_OBJ_SKILL1, FALSE);
		SetRender(BUTTON_OBJ_GUILD1, FALSE);
		SetRender(BUTTON_OBJ_PARTY1, FALSE);
		SetRender(BUTTON_OBJ_CHAT1, FALSE);
		SetRender(BUTTON_OBJ_SYSTEM1, FALSE);				

		if(m_nPK==0)
		{
			SetRender(BUTTON_OBJ_PKOFF1, TRUE);
			SetRender(BUTTON_OBJ_PKOFF2, FALSE);
			SetRender(BUTTON_OBJ_PKON1, FALSE);
			SetRender(BUTTON_OBJ_PKON2, FALSE);
		}
		else
		{
			SetRender(BUTTON_OBJ_PKOFF1, FALSE);
			SetRender(BUTTON_OBJ_PKOFF2, FALSE);
			SetRender(BUTTON_OBJ_PKON1, TRUE);
			SetRender(BUTTON_OBJ_PKON2, FALSE);
		}
		break;
	}

	SetRender(BUTTON_OBJ_ITEM2, FALSE);
	SetRender(BUTTON_OBJ_CHR2, FALSE);
	SetRender(BUTTON_OBJ_SKILL2, FALSE);
	SetRender(BUTTON_OBJ_GUILD2, FALSE);
	SetRender(BUTTON_OBJ_PARTY2, FALSE);
	SetRender(BUTTON_OBJ_CHAT2, FALSE);
	SetRender(BUTTON_OBJ_SYSTEM2, FALSE);

	const auto pSkillWnd = SkillSelectionView::sharedInstance();

	m_nChatChk		= 0;
	m_byOrderCheck	= 0;
	
	if(m_bUp)
		m_bSoundChk	= FALSE;

	if( g_pThisDungeon->IsStadium() && g_pMainPlayer->m_dwGuildWarFlag == G_W_F_OBSERVER )
		goto STATUS_CHECK;

	if( m_bUp && m_bDown && nRt > 0 )
	{			
		m_bSmall = FALSE;
		
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
		
		else if(nRt == NRET_GUARDIAN_INTERFACE1)
		{
			CGuardianWnd* pGuardianWnd = CGuardianWnd::GetInstance();		
					
			if(pGuardianWnd->GetActive()==FALSE)
				pGuardianWnd->SetActive();
			else					
				pGuardianWnd->SetActive(FALSE);

			m_byOrderCheck = GUARDIAN_WND;
		}
		m_bUp	= FALSE;
		m_bDown = FALSE;
	}
	
	if(g_Mouse.bLDown && nRt>0)
	{
		if(nRt != NRET_GUARDIAN_POTION)
		{
			CInterface::GetInstance()->SetIconChk(FALSE);		
			SetPointer(__MOUSE_POINTER_BUTTONCLICK__);
		}
			
		m_bClick	= TRUE;
		m_bDown		= TRUE;

		switch ( nRt )
		{
			case NRET_BUTTON_ITEM:		SetRender(BUTTON_OBJ_ITEM2, TRUE);			break;
			case NRET_BUTTON_CHR:		SetRender(BUTTON_OBJ_CHR2, TRUE);			break;
			case 4:		SetRender(BUTTON_OBJ_SKILL2, TRUE);			break;
			case 5:		SetRender(BUTTON_OBJ_GUILD2, TRUE);			break;
			case 6:		SetRender(BUTTON_OBJ_PARTY2, TRUE);			break;
			case 7:		SetRender(BUTTON_OBJ_CHAT2, TRUE);			break;
			case 8:		SetRender(BUTTON_OBJ_SYSTEM2, TRUE);		break;
		}

		SetRender(BUTTON_OBJ_ITEM1, FALSE);
		SetRender(BUTTON_OBJ_CHR1, FALSE);
		SetRender(BUTTON_OBJ_SKILL1, FALSE);
		SetRender(BUTTON_OBJ_GUILD1, FALSE);
		SetRender(BUTTON_OBJ_PARTY1, FALSE);
		SetRender(BUTTON_OBJ_CHAT1, FALSE);
		SetRender(BUTTON_OBJ_SYSTEM1, FALSE);			
		return 1;
	}

STATUS_CHECK:	

	if(m_byOrderCheck!=0)
		return 1;

	m_pStatusWnd->CheckInterface();

	return 0;
}

void CUserInterface::Render()
{	
	if( g_pThisDungeon->IsStadium() && g_pMainPlayer->m_dwGuildWarFlag == G_W_F_OBSERVER )
		return;

	for(int i = 0; i < m_bySpriteDataCnt; i++)
	{
		if(m_sSprite_Data[i].dwId!=0xff && m_sSprite_Data[i].bRender)
		{
			LP_SPRITE_DATA lpSpriteData = g_pInterfaceSprHash->GetData(m_sSprite_Data[i].dwSprId);
			
			if(lpSpriteData)
			{	
				VECTOR2 vPos;

				if(!m_sSprite_Data[i].bPos)
				{
					vPos.x	= m_sSprite_Data[i].fPosX;
					vPos.y	= m_sSprite_Data[i].fPosY;
				}
				else
				{
					vPos.x	= m_fPosX + m_sSprite_Data[i].fPosX;
					vPos.y	= m_fPosZ + m_sSprite_Data[i].fPosY;
				}								

				int nOrder = 0;

				if(m_sSprite_Data[i].bOrder)	
					nOrder = __ORDER_INTERFACE_START__ + m_byOrder * 10 + m_sSprite_Data[i].nOrder;
				else
					nOrder = m_sSprite_Data[i].nOrder;

				if(m_sSprite_Data[i].v2Scaling.x==1.0f && m_sSprite_Data[i].v2Scaling.y==1.0f)
				{
					g_pRenderer->RenderSprite(lpSpriteData->pSpr, NULL
						, m_sSprite_Data[i].fRot, &vPos, NULL, m_sSprite_Data[i].dwAlpha, nOrder, 0);
				}
				else
				{
					g_pRenderer->RenderSprite(lpSpriteData->pSpr, &m_sSprite_Data[i].v2Scaling
						, m_sSprite_Data[i].fRot, &vPos, NULL, m_sSprite_Data[i].dwAlpha, nOrder, 0);				
				}
			}
		}
	}
}

void CUserInterface::PartyUserName()
{
	BYTE byIndex = 0;

	ListNode<PARTY_USER>* pNode = g_pPartyUserHash->GetHead();

	while(pNode)
	{
		LP_PARTY_USER sPartyNode = pNode->pData;

		CUser* pUser = g_pUserHash->GetData(sPartyNode->dwUserIndex);
		
		if(pUser)
		{
			int nSize	= lstrlen(pUser->m_szName);
			RenderFont(pUser->m_szName, 3, 3+nSize*7, 116+(byIndex*36)+4, 130+(byIndex*36)+4, __ORDER_USERINTERFACE_START_+3);								
			byIndex++;
		}

		pNode = pNode->pNext;
	}	
}

void CUserInterface::RenderText()
{
	m_pStatusWnd->RenderText();

	if( g_pThisDungeon->IsStadium() && g_pMainPlayer->m_dwGuildWarFlag == G_W_F_OBSERVER )
		return;


	/// should these be replaced with continous updates?

	RenderMonsterBar();
	RenderUserBar();
	
	PartyUserName();

	RenderGuardianItem();

	CItemWnd::GetInstance()->RenderItemBelt();
	SkillSelectionView::sharedInstance()->render();

	if(g_pMainPlayer->m_GuardianItem.m_wItemID!=0)
	{		
		char	szInfo[0xff]={0,};
		WORD	wCnt	= 10;
		int		nIndex	= 0;

		for(int j = 0; j < 2; j++)
		{
			if(g_pMainPlayer->m_GuardianItem.m_Item_Supplies.bQuantity>=wCnt)
			{
				nIndex++;
				wCnt=WORD(wCnt*10);
			}			
		}
		
		if(g_ItemMoveManager.GetNewItemMoveMode())
		{
			if(	g_ItemMoveManager.GetNativeSrc() == ITEM_NATIVE_GUARDIAN )
			{
				
			}
		}
		else
		{
			wsprintf(szInfo, "%d", g_pMainPlayer->m_GuardianItem.GetQuantity());							

			if(!IsEmptyString(szInfo))
			{
				RenderFont(szInfo, 93+21-(nIndex*7), 93+21+7, 96, 110, __ORDER_USERINTERFACE_START_+2);
			}
		}		
	}

	if(m_bGuardian==TRUE)
	{		
		if(!IsEmptyString(m_szGuardianName))
		{
 			RenderFont(m_szGuardianName, 2, 100, 85, 99, __ORDER_USERINTERFACE_START_+2);
		}
	}
}


void CUserInterface::onDungeonJoin() {
	SetCool();
}

void CUserInterface::updatedCurrentHP(CMainUser* player, DWORD oldValue, DWORD newValue) {
	updateHPBar(newValue / (float)player->maxHP());
}

void CUserInterface::updatedMAXHP(CMainUser* player, DWORD oldValue, DWORD newValue) {
	updateHPBar(newValue / (float)player->maxHP());
}

void CUserInterface::updatedCurrentSP(CMainUser* player, DWORD oldValue, DWORD newValue) {
	updateSPBar(newValue / (float)player->maxSP());
}

void CUserInterface::updatedMAXSP(CMainUser* player, DWORD oldValue, DWORD newValue) {
	updateSPBar(newValue / (float)player->maxSP());
}

float expScale(DWORD currentLevel, DWORD currentCumulatedEXP) {
	const auto level = currentLevel;
	const auto cumulatedEXP = currentCumulatedEXP;
	const auto cumulatedExpForCurrentLevel = GetCumulatedExpByLevel(OBJECT_TYPE_PLAYER, level);
	const auto cumulatedExpForNextLevel = GetCumulatedExpByLevel(OBJECT_TYPE_PLAYER, level + 1);
	const auto toGather = cumulatedExpForNextLevel - cumulatedExpForCurrentLevel;
	const DWORD gatheredSoFar = cumulatedEXP - cumulatedExpForCurrentLevel;
	const auto scale = (float)gatheredSoFar / toGather;

	return scale;
}

void CUserInterface::updatedEXP(CMainUser* player, DWORD oldValue, DWORD newValue) {
	updateEXPBar(expScale(player->currentLevel(), player->currentEXP()));
}

void CUserInterface::updatedLevel(CMainUser* player, DWORD oldValue, DWORD newValue) {
	updateEXPBar(expScale(player->currentLevel(), player->currentEXP()));
}

void CUserInterface::updatedCoolPoints(CMainUser* player, float oldValue, float newValue) {
	updateCooldownBar(newValue / player->maxCoolPoints());
}


void CUserInterface::SetCool()
{
	float fSize = g_pMainPlayer->percentageCoolPoints();

	SetScalingObj(SPR_OBJ_CAST, fSize *300/2, 1.0);
	SetPosObjX(SPR_OBJ_CAST, 1023-fSize *300);
}

extern char globalDebugLine[255];


void	CUserInterface::updateSPBar(float scale) {
	SetScalingObj(SPR_OBJ_MANA2, scale * 300 / 2, 1.0);
	SetPosObjX(SPR_OBJ_MANA2, 1023 - scale * 300);

	SetScalingObj(SPR_OBJ_MANA1, scale * 300 / 2, 1.0);
	SetPosObjX(SPR_OBJ_MANA1, 1023 - scale * 300);

	SetRender(SPR_OBJ_MANA1, TRUE);
	SetRender(SPR_OBJ_MANA2, TRUE);
}

void	CUserInterface::updateHPBar(float scale) {
	SetScalingObj(SPR_OBJ_EN1, scale * 300 / 2, 1.0);
	SetScalingObj(SPR_OBJ_EN2, scale * 300 / 2, 1.0);

	SetRender(SPR_OBJ_EN1, TRUE);
}

void	CUserInterface::updateEXPBar(float scale) {
	SetScalingObj(SPR_OBJ_EXP, scale * 300 / 2, 1.0);
	SetRender(SPR_OBJ_EXP, TRUE);
}

void	CUserInterface::updateCooldownBar(float scale) {
	SetScalingObj(SPR_OBJ_CAST, scale * 300 / 2, 1.0);
	SetPosObjX(SPR_OBJ_CAST, 1023 - scale * 300);
	SetRender(SPR_OBJ_CAST, TRUE);
}

void CUserInterface::SetActive(BOOL bActive)
{
	m_bActive =  bActive;
}

void CUserInterface::RenderMonsterBar()
{
	//	sung-han 2004-12-30 : pvp 에서 캐릭터 에너지바와 엘레맨탈 에너지 바가 겹치는 부분 처리
	//-----------------------------------------------------------------------------------------
	if( m_pMatchUser && m_bMatchUserHPShowEnable )
		return;
	//-----------------------------------------------------------------------------------------

	if(m_bMonster)
	{
		m_dwMonCurTime	= timeGetTime();

		if(m_dwMonCurTime-m_dwMonPrevTime>3000)
		{
			SetRender(SPR_OBJ_MONSTERHP_BAR1, FALSE);
			SetRender(SPR_OBJ_MONSTERHP_BAR2, FALSE);
			SetRender(SPR_OBJ_MONSTER_GAGE1, FALSE);
			SetRender(SPR_OBJ_MONSTER_GAGE2, FALSE);
			m_bMonster = FALSE;
			return;
		}
		
		if(!IsEmptyString(m_szMosterName))
		{
			int nSize = lstrlen(m_szMosterName);
			RenderFont(m_szMosterName, 421, 421+(nSize*7)+10, 4, 18, __ORDER_USERINTERFACE_START_+3);			
		}
	}	
}

void CUserInterface::RenderUserBar()
{
	if( m_pMatchUser && m_bMatchUserHPShowEnable )
	{
		SetRender(SPR_OBJ_USERHP_BAR1, TRUE);
		SetRender(SPR_OBJ_USERHP_BAR2, TRUE);
		SetRender(SPR_OBJ_USER_GAGE1, TRUE);
		SetRender(SPR_OBJ_USER_GAGE2, TRUE);
		
		if(!IsEmptyString(m_szMatchUserName))
		{	
			int nSize = lstrlen(m_szMatchUserName);
			RenderFont(m_szMatchUserName, 421, 421+(nSize*7)+10, 4, 18, __ORDER_USERINTERFACE_START_+3);			
		}
	}
	else
	{
		CloseUserBar();
	}
}

void CUserInterface::CloseUserBar()
{
	SetRender(SPR_OBJ_USERHP_BAR1, FALSE);
	SetRender(SPR_OBJ_USERHP_BAR2, FALSE);
	SetRender(SPR_OBJ_USER_GAGE1, FALSE);
	SetRender(SPR_OBJ_USER_GAGE2, FALSE);
}

void CUserInterface::OpenUserBar(char *pszName, DWORD dwHP, DWORD dwMaxHP)
{
	// 데이터 //
	__lstrcpyn(m_szMatchUserName, pszName, lstrlen(pszName));
	m_fUserHp		= (float)dwHP;
	m_fUserMaxHp	= (float)dwMaxHP;
//	m_bMonster		= FALSE;
	m_dwUserPrevTime= timeGetTime();
	m_dwUserCurTime	= timeGetTime();
	// 리소트 //
	SetScalingObj(SPR_OBJ_USER_GAGE1, (m_fUserHp/m_fUserMaxHp)*100.0f, 1.0f);
	SetScalingObj(SPR_OBJ_USER_GAGE2, 100.0f, 1.0f);

	SetRender(SPR_OBJ_USERHP_BAR1, TRUE);
	SetRender(SPR_OBJ_USERHP_BAR2, TRUE);
	SetRender(SPR_OBJ_USER_GAGE1, TRUE);
	SetRender(SPR_OBJ_USER_GAGE2, TRUE);
	
}

void CUserInterface::OpenMonsterBar(char* pszName, DWORD dwHp, DWORD dwMaxHp)
{
//	sung-han 2004-12-30 : pvp 에서 캐릭터 에너지바와 엘레맨탈 에너지 바가 겹치는 부분 처리
//-----------------------------------------------------------------------------------------
	if( m_pMatchUser && m_bMatchUserHPShowEnable ) 
	{
		CloseUserBar();
		m_bMatchUserHPShowEnable = FALSE;
	}
//-----------------------------------------------------------------------------------------
	// 데이터 //
	lstrcpy(m_szMosterName, pszName);
	m_fMonHp		= (float)dwHp;
	m_fMonMaxHp		= (float)dwMaxHp;
	m_bMonster		= TRUE;
	m_dwMonPrevTime	= timeGetTime();
	m_dwMonCurTime	= timeGetTime();
	// 리소트 //
	SetScalingObj(SPR_OBJ_MONSTER_GAGE1, (m_fMonHp/m_fMonMaxHp)*100.0f, 1.0f);
	SetScalingObj(SPR_OBJ_MONSTER_GAGE2, 100.0f, 1.0f);

	SetRender(SPR_OBJ_MONSTERHP_BAR1, TRUE);
	SetRender(SPR_OBJ_MONSTERHP_BAR2, TRUE);
	SetRender(SPR_OBJ_MONSTER_GAGE1, TRUE);
	SetRender(SPR_OBJ_MONSTER_GAGE2, TRUE);	
}

void CUserInterface::RenderGuardianItem()
{		
	if(g_pMainPlayer->m_GuardianItem.m_wItemID!=0)
	{
		if(g_ItemMoveManager.GetNewItemMoveMode())
		{
			if(	g_ItemMoveManager.GetNativeSrc() == ITEM_NATIVE_GUARDIAN )
			{
				return;
			}
		}

		LP_ITEM_RESOURCE_EX lpItemResourceEx = 
			g_pItemResourceHash->GetData(g_pMainPlayer->m_GuardianItem.m_wItemID);

		if(lpItemResourceEx)
		{
			VECTOR2	vPos = {93.f, 76.f};
			g_pRenderer->RenderSprite(lpItemResourceEx->pSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, __ORDER_USERINTERFACE_START_+1, RENDER_TYPE_DISABLE_TEX_FILTERING);
		}
	}
}

void CUserInterface::SetMousePointerPos(float fPosX, float fPosZ)
{	

}


void CUserInterface::SetPKMode(BOOL bPK)
{
	m_nPK = bPK;
	if(bPK)
	{		
		SetRender(BUTTON_OBJ_PKOFF1, FALSE);
		SetRender(BUTTON_OBJ_PKOFF2, FALSE);
		SetRender(BUTTON_OBJ_PKON2, TRUE);	
	}
	else
	{		
		SetRender(BUTTON_OBJ_PKON1, FALSE);
		SetRender(BUTTON_OBJ_PKON1, FALSE);
		SetRender(BUTTON_OBJ_PKOFF2, TRUE);
	}
}

void CUserInterface::SetPointer(BYTE byPointerType)
{
	if(CInterface::GetInstance()->GetIconChk() == FALSE|| GetGameStatus() == UPDATE_GAME_PLAY)
	{

		if(  (g_pThisDungeon != NULL) &&  (g_pThisDungeon->IsStadium() && g_pMainPlayer->m_dwGuildWarFlag == G_W_F_OBSERVER ) )
			SetMousePointerType( __MOUSE_POINTER_DEFAULT__ );
		else
			SetMousePointerType(byPointerType);

		CInterface::GetInstance()->SetIconChk(TRUE);
	}
	
	if(GetGameStatus()==UPDATE_GAME_CHAR_SELECT || GetGameStatus()==UPDATE_GAME_WORLD )
	{
		SetMousePointerType(byPointerType);
	}
}


void CUserInterface::SetMousePointerType(BYTE byPointerType)
{
	if(m_bPointerChk==TRUE)
	{
		if(byPointerType!=__MOUSE_POINTER_GUARDIAN__ && byPointerType!=__MOUSE_POINTER_GUARDIANCLICK__)
		{
			return;			
		}
	}

	m_byPointerType = byPointerType;

	if(!g_hCursor[ byPointerType ] || byPointerType >= __MAX_MOUSE_POINTER__)	
	{
		//asm_int3();
		return;
	}

	if(GetCursor() != g_hCursor[ byPointerType ])
	{
		if(g_ItemMoveManager.GetNewItemMoveMode() )
			if( g_ItemMoveManager.GetMouseItemID() )
				if( byPointerType != __MOUSE_POINTER_ITEMCLICK__ )
					byPointerType = __MOUSE_POINTER_ITEMCLICK__;		

		SetCursor( g_hCursor[ byPointerType ] );					
	}
	
	CInterface::GetInstance()->SetIconChk(TRUE);
}


BOOL CUserInterface::PosCollision(int nPosX1, int nPosX2, int nPosY1, int nPosY2)
{
	CInterface* pInterface = CInterface::GetInstance();	
	
	for(int i = 0; i < CUR_INTERFACE; i++)
	{
		if(i==COMMAND_WND || i==INTERFACE_USER)
			continue;	
	
		if(pInterface->m_pMenu[i]->GetActive()==TRUE)
		{
			int nStartPosX	= pInterface->m_pMenu[i]->GetPosX();
			int nStartPosY	= pInterface->m_pMenu[i]->GetPosY();
			int nEndPosX	= pInterface->m_pMenu[i]->GetPosX() + pInterface->m_nSize[i*2];
			int nEndPosY	= pInterface->m_pMenu[i]->GetPosY() + pInterface->m_nSize[i*2+1];
						
			if((nPosX1>=nStartPosX && nPosX1<=nEndPosX) || (nPosX2>=nStartPosX && nPosX2<=nEndPosX))
			{		
				if((nPosY1>=nStartPosY && nPosY1<=nEndPosY) || (nPosY2>=nStartPosY && nPosY2<=nEndPosY))
					return FALSE;
			}			
		}
	}	
	return TRUE;
}

BOOL CUserInterface::InterfaceCollision(int nIndex, int nPosX1, int nPosX2, int nPosY1, int nPosY2)
{
	CInterface* pInterface = CInterface::GetInstance();	
	
	for(int i = 0; i < CUR_INTERFACE; i++)
	{
		int nOrderDst = pInterface->m_pMenu[i]->GetOrderIndex();
		int nOrderSrc = pInterface->m_pMenu[nIndex]->GetOrderIndex();

		if(i==nIndex)
			continue;
		if(i==COMMAND_WND || i==INTERFACE_USER)
			continue;
		if(nOrderDst<nOrderSrc)
			continue;		
	
		if(pInterface->m_pMenu[i]->GetActive()==TRUE)
		{
			int nStartPosX	= pInterface->m_pMenu[i]->GetPosX();
			int nStartPosY	= pInterface->m_pMenu[i]->GetPosY();
			int nEndPosX	= pInterface->m_pMenu[i]->GetPosX() + pInterface->m_nSize[i*2];
			int nEndPosY	= pInterface->m_pMenu[i]->GetPosY() + pInterface->m_nSize[i*2+1];
						
			if((nPosX1>=nStartPosX && nPosX1<=nEndPosX) || (nPosX2>=nStartPosX && nPosX2<=nEndPosX))
			{		
				if((nPosY1>=nStartPosY && nPosY1<=nEndPosY) || (nPosY2>=nStartPosY && nPosY2<=nEndPosY))
					return FALSE;
			}			
		}
	}	
	return TRUE;
}

// seven
// 대중중일때 퀵슬롯의 쓰지못하게 가린다.
// 이는 가리는 효과만 있다.
void CUserInterface::EnableQuickSlot(BOOL bEnAble)
{
	if (!bEnAble)
	{
		// 가려라.
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE1, TRUE);
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE2, TRUE);
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE3, TRUE);
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE4, TRUE);
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE5, TRUE);
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE6, TRUE);
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE7, TRUE);
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE8, TRUE);
	}
	else
	{
		// 보여라.
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE1, FALSE);
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE2, FALSE);
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE3, FALSE);
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE4, FALSE);
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE5, FALSE);
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE6, FALSE);
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE7, FALSE);
		SetRender(SPR_OBJ_UI_QUICKSLOT_INABLE8, FALSE);
	}
}


void CUserInterface::OpenGuardianDescriptionWnd(INT64 i64ID
											, char* pszName
											, DWORD dwGauge
											, DWORD dwMaxGauge)
{
	// Interface update		
	
	m_bGuardian = TRUE;
	m_i64ActiveGuardianItemID = i64ID;
	lstrcpy(m_szGuardianName, pszName);			

	SetPosObj(SPR_OBJ_GUARDIAN_WND1, 0, 69);			
	SetRender(SPR_OBJ_GUARDIAN_WND1, TRUE);			
	SetGauge(dwGauge, dwMaxGauge);

	SetPosObj(SPR_OBJ_GUARDIAN_EN, 0, 97);
	SetRender(SPR_OBJ_GUARDIAN_EN, TRUE);

	CItemWnd* pItemWnd = CItemWnd::GetInstance();
	pItemWnd->m_bGuarInterface = TRUE;

	SetRender(SPR_OBJ_UI_GUARDIAN_STATUS, TRUE);

	if(pItemWnd->GetActive())
	{
		pItemWnd->SetRender(SPR_ITEMWND_GUARDIAN_ON1, TRUE);
		pItemWnd->SetRender(SPR_ITEMWND_GUARDIAN_ON2, FALSE);
		pItemWnd->SetRender(SPR_ITEMWND_GUARDIAN_OFF1, FALSE);
		pItemWnd->SetRender(SPR_ITEMWND_GUARDIAN_OFF2, FALSE);			
	}		
}


void CUserInterface::CloseGuardianDescriptionWnd()
{	
	m_bGuardian = FALSE;
	m_i64ActiveGuardianItemID = 0;
	
	SetRender(SPR_OBJ_GUARDIAN_EN, FALSE);
	SetRender(SPR_OBJ_GUARDIAN_WND1, FALSE);

	CItemWnd* pItemWnd = CItemWnd::GetInstance();
	pItemWnd->m_bGuarInterface = FALSE;

	SetRender(SPR_OBJ_UI_GUARDIAN_STATUS, FALSE);

	if(pItemWnd->GetActive())
	{
		pItemWnd->SetRender(SPR_OBJ_ITEMWND_GUARDIAN_ON1, FALSE);
		pItemWnd->SetRender(SPR_OBJ_ITEMWND_GUARDIAN_ON2, FALSE);
		pItemWnd->SetRender(SPR_OBJ_ITEMWND_GUARDIAN_OFF1, TRUE);
		pItemWnd->SetRender(SPR_OBJ_ITEMWND_GUARDIAN_OFF2, FALSE);
	}
}


void CUserInterface::SetGauge(DWORD dwGauge, DWORD dwMaxGauge)
{
	SetScalingObj(SPR_OBJ_GUARDIAN_EN, ((float)dwGauge/(float)dwMaxGauge)*46.0f, 1.0f);
}
// --
//==========================================================//
// End.														//
//==========================================================//