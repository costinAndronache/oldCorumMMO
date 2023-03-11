//======================================================//
// Code by Jang.							2003.07.21	//
//======================================================//
#include "GroupWnd.h"
#include "Interface.h"
#include "UserInterface.h"
#include "SpriteManager.h"
#include "GameControl.h"
#include "DungeonTable.h"
#include "HashTable.h"
#include "Chat.h"
#include "DungeonProcess.h"
#include "message.h"
#include "ChatWnd.h"
#include "GuildEditWnd.h"
#include "GuildDeleteWnd.h"
#include "MessengerDeleteWnd.h"
#include "InputManager.h"
#include "IMEEdit.h"
#include "GuildMasterWnd.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "CorumResource.h"
#include "PartyEntryWnd.h"
#include "CodeFun.h"
#include "ChatBackground.h"
#include "RivalGuildWar.h"
#include "PartyMatchingWnd.h"
#include "GuildWarRequest.h"



CGroupWnd* CGroupWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CGroupWnd::CGroupWnd()
{	
	m_byMsgCnt				= 0;	
	m_nPartyUserIndex		= -1;
	m_byLayerIndex			= 0;	
	m_dwPrevTime			= 0;
	m_dwCurTime				= 0;
	m_wDungeonID			= 0;
	m_byCurPage				= 1;
	m_byMaxPage				= 1;
	m_bPartyMove			= FALSE;
	m_bMessengerMove		= FALSE;
	m_bGuildWarMove			= FALSE;
	m_bInfo					= FALSE;
	m_bInit					= FALSE;
	m_bGuildBtn[0]			= FALSE;
	m_bGuildBtn[1]			= FALSE;
	m_bGuildBtn[2]			= FALSE;
	m_bGuildBtn[3]			= FALSE;
	m_bGuildBtn[4]			= FALSE;
	m_bGuildBtn[5]			= FALSE;
	m_bGuildBtn[6]			= FALSE;
	m_bPartyBtn[0]			= FALSE;
	m_bPartyBtn[1]			= FALSE;
	m_bPartyBtn[2]			= FALSE;
	m_bPartyBtn[3]			= FALSE;
	m_bFriednBtn[0]			= FALSE;
	m_bFriednBtn[1]			= FALSE;
	m_bGuildWarBtn			= FALSE;
	m_bPartyMsg				= TRUE;
	m_bGuildMsg				= FALSE;
	m_byGroupTab			= GROUP_TAB_PARTY;
	m_byStatus				= GROUP_PARTY_DEFAULT;	
	m_byUserType			= GUILD_USER_ONLINE;
	m_pGuildUserPos			= NULL;
	m_pGuildOfflineUserPos	= NULL;	
	m_pMessengerOnlinePos	= NULL;	
	m_pMessengerOfflinePos	= NULL;	
	m_byMessengerType		= FRIEND_USER_ONLINE;
	m_pSprStatus[0]			= NULL;
	m_pSprStatus[1]			= NULL;
	m_pSprStatus[2]			= NULL;
	m_pSprStatus[3]			= NULL;

	m_bynGuildWarForm = 0;
	m_byGuildWarNumber = 0;
	m_byGuildWarType = 0;
	m_bGuildWarMatchEnable = FALSE;

	//��Ƽ��ȭ�ý���(����â 2005.02.15)
	m_byPMatchStatus		= 0;
	m_nPMatchResult			= -1;
//	m_nPMatchEscapeResult   = -1;
	m_dwPartyCount			= 0;  //��Ƽ��Ȳ(����â 2005.03.03)
}

CGroupWnd::~CGroupWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CGroupWnd::Init()
{
/*
	InsertData(SPR_OBJ_GROUPWND, SPR_INTERFACE_GROUPWND, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_GROUPWND_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GROUPWND_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);	
	InsertData(SPR_OBJ_GROUPWND_PARTY1, SPR_INTERFACE_GROUPWND_PARTY1, 6, 24, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GROUPWND_PARTY2, SPR_INTERFACE_GROUPWND_PARTY2, 6, 24, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GROUPWND_GUILD1, SPR_INTERFACE_GROUPWND_GUILD1, 68, 24, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GROUPWND_GUILD2, SPR_INTERFACE_GROUPWND_GUILD2, 68, 24, 1.0f, 1.0f, 1);		
	InsertData(SPR_OBJ_GROUPWND_FRIEND1, SPR_INTERFACE_GROUPWND_FRIEND1, 130, 24, 1.0f, 1.0f, 1);	
	InsertData(SPR_OBJ_GROUPWND_FRIEND2, SPR_INTERFACE_GROUPWND_FRIEND2, 130, 24, 1.0f, 1.0f, 1);	
	InsertData(SPR_OBJ_GROUPWND_GUILDWAR1, SPR_INTERFACE_GROUPWND_GUILDWAR1, 192, 24, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GROUPWND_GUILDWAR2, SPR_INTERFACE_GROUPWND_GUILDWAR2, 192, 24, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GROUPWND_ADD1, SPR_INTERFACE_GROUPWND_ADD1, 5, 100, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GROUPWND_ADD2, SPR_INTERFACE_GROUPWND_ADD2, 5, 100, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GROUPWND_ADD3, SPR_INTERFACE_GROUPWND_ADD3, 5, 100, 1.0f, 1.0f, 1);	
	InsertData(SPR_OBJ_GROUPWND_DELETE1, SPR_INTERFACE_GROUPWND_DELETE1, 5, 135, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GROUPWND_DELETE2, SPR_INTERFACE_GROUPWND_DELETE2, 5, 135, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GROUPWND_DELETE3, SPR_INTERFACE_GROUPWND_DELETE3, 5, 135, 1.0f, 1.0f, 1);	
	InsertData(SPR_OBJ_GROUPWND_INFO1, SPR_INTERFACE_GROUPWND_INFO1, 5, 170, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GROUPWND_INFO2, SPR_INTERFACE_GROUPWND_INFO2, 5, 170, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GROUPWND_INFO3, SPR_INTERFACE_GROUPWND_INFO3, 5, 170, 1.0f, 1.0f, 1);	
	InsertData(SPR_OBJ_GROUPWND_OUT1, SPR_INTERFACE_GROUPWND_OUT1, 5, 240, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GROUPWND_OUT2, SPR_INTERFACE_GROUPWND_OUT2, 5, 240, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GROUPWND_OUT3, SPR_INTERFACE_GROUPWND_OUT3, 5, 240, 1.0f, 1.0f, 1);	
	InsertData(SPR_OBJ_GROUPWND_MSG1, SPR_INTERFACE_GROUPWND_MSG1, 5, 275, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GROUPWND_MSG2, SPR_INTERFACE_GROUPWND_MSG2, 5, 275, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GROUPWND_MSG3, SPR_INTERFACE_GROUPWND_MSG3, 5, 275, 1.0f, 1.0f, 1);	
	InsertData(SPR_OBJ_GROUPWND_SELECT, SPR_INTERFACE_GROUPWND_SELECT, 0, 0, 104.0f, 16.0f, 2);
	InsertData(SPR_OBJ_GROUPWND_GUILDWARADD1, SPR_INTERFACE_GROUPWND_GUILDWARADD1, 5, 100, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GROUPWND_GUILDWARADD2, SPR_INTERFACE_GROUPWND_GUILDWARADD2, 5, 100, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GROUPWND_GUILDWARADD3, SPR_INTERFACE_GROUPWND_GUILDWARADD3, 5, 100, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GROUPWND_GUILDWAREXIT1, SPR_INTERFACE_GROUPWND_GUILDWAREXIT1, 5, 135, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GROUPWND_GUILDWAREXIT2, SPR_INTERFACE_GROUPWND_GUILDWAREXIT2, 5, 135, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GROUPWND_GUILDWAREXIT3, SPR_INTERFACE_GROUPWND_GUILDWAREXIT3, 5, 135, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILD_PAGE, SPR_INTERFACE_PAGEBOX, 110, 380, 1.0f, 1.0f, 3);
	InsertData(SPR_OBJ_GUILD_PAGEPREV1, SPR_INTERFACE_PAGEPRE1, 93, 380, 1.0f, 1.0f, 3);
	InsertData(SPR_OBJ_GUILD_PAGEPREV2, SPR_INTERFACE_PAGEPRE2, 93, 380, 1.0f, 1.0f, 3);
	InsertData(SPR_OBJ_GUILD_PAGEPREV3, SPR_INTERFACE_PAGEPRE3, 93, 380, 1.0f, 1.0f, 3);
	InsertData(SPR_OBJ_GUILD_PAGENEXT1, SPR_INTERFACE_PAGENEXT1, 175, 380, 1.0f, 1.0f, 3);
	InsertData(SPR_OBJ_GUILD_PAGENEXT2, SPR_INTERFACE_PAGENEXT2, 175, 380, 1.0f, 1.0f, 3);
	InsertData(SPR_OBJ_GUILD_PAGENEXT3, SPR_INTERFACE_PAGENEXT3, 175, 380, 1.0f, 1.0f, 3);
	InsertData(SPR_OBJ_GUILD_ONOFF1, SPR_INTERFACE_GUILD_ONOFF1, 5, 310, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILD_ONOFF2, SPR_INTERFACE_GUILD_ONOFF2, 5, 310, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILD_ONOFF3, SPR_INTERFACE_GUILD_ONOFF3, 5, 310, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILD_INFO1, SPR_INTERFACE_GUILD_INFO1, 5, 275, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILD_INFO2, SPR_INTERFACE_GUILD_INFO2, 5, 275, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILD_INFO3, SPR_INTERFACE_GUILD_INFO3, 5, 275, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILD_LEVEL1, SPR_INTERFACE_GUILD_LEVEL, 110, 491, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILD_LEVEL2, SPR_INTERFACE_GUILD_LEVEL, 142, 491, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILD_LEVEL3, SPR_INTERFACE_GUILD_LEVEL, 174, 491, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILD_LEVEL4, SPR_INTERFACE_GUILD_LEVEL, 206, 491, 1.0f, 1.0f, 1);	
	InsertData(SPR_OBJ_GUILD_MARK, SPR_INTERFACE_GUILD_MARK, 20, 472, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GROUPWND_STATUS, SPR_INTERFACE_GROUPWND_STATUS, 107, 81, 1.0f, 1.0f, 2);	
	InsertData(SPR_OBJ_GROUP_MASTER1, SPR_INTERFACE_GROUP_MASTER1, 5, 345, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GROUP_MASTER2, SPR_INTERFACE_GROUP_MASTER2, 5, 345, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GROUP_MASTER3, SPR_INTERFACE_GROUP_MASTER3, 5, 345, 1.0f, 1.0f, 1);		
	InsertData(SPR_OBJ_GUILDWAR_GUILD, SPR_INTERFACE_GUILDWAR_GUILD, 32, 81, 1.0f, 1.0f, 1);					
	InsertData(SPR_OBJ_GROUPWND_PARTYBAR1, SPR_INTERFACE_GROUPWND_PARTYBAR, 0, 96, 1.0f, 1.0f, __ORDER_USERINTERFACE_START_+2, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GROUPWND_PARTYBAR2, SPR_INTERFACE_GROUPWND_PARTYBAR, 0, 132, 1.0f, 1.0f, __ORDER_USERINTERFACE_START_+2, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GROUPWND_PARTYBAR3, SPR_INTERFACE_GROUPWND_PARTYBAR, 0, 168, 1.0f, 1.0f, __ORDER_USERINTERFACE_START_+2, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GROUPWND_PARTYBAR4, SPR_INTERFACE_GROUPWND_PARTYBAR, 0, 200, 1.0f, 1.0f, __ORDER_USERINTERFACE_START_+2, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GROUPWND_PARTYBAR5, SPR_INTERFACE_GROUPWND_PARTYBAR, 0, 240, 1.0f, 1.0f, __ORDER_USERINTERFACE_START_+2, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GROUPWND_PARTYBAR6, SPR_INTERFACE_GROUPWND_PARTYBAR, 0, 276, 1.0f, 1.0f, __ORDER_USERINTERFACE_START_+2, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GROUPWND_PARTYBAR7, SPR_INTERFACE_GROUPWND_PARTYBAR, 0, 312, 1.0f, 1.0f, __ORDER_USERINTERFACE_START_+2, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GROUPWND_PARTYGAGE1, SPR_INTERFACE_GROUPWND_PARTYGAGE, 0, 113, 1.0f, 1.0f, __ORDER_USERINTERFACE_START_+3, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GROUPWND_PARTYGAGE2, SPR_INTERFACE_GROUPWND_PARTYGAGE, 0, 149, 1.0f, 1.0f, __ORDER_USERINTERFACE_START_+3, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GROUPWND_PARTYGAGE3, SPR_INTERFACE_GROUPWND_PARTYGAGE, 0, 185, 1.0f, 1.0f, __ORDER_USERINTERFACE_START_+3, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GROUPWND_PARTYGAGE4, SPR_INTERFACE_GROUPWND_PARTYGAGE, 0, 221, 1.0f, 1.0f, __ORDER_USERINTERFACE_START_+3, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GROUPWND_PARTYGAGE5, SPR_INTERFACE_GROUPWND_PARTYGAGE, 0, 257, 1.0f, 1.0f, __ORDER_USERINTERFACE_START_+3, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GROUPWND_PARTYGAGE6, SPR_INTERFACE_GROUPWND_PARTYGAGE, 0, 293, 1.0f, 1.0f, __ORDER_USERINTERFACE_START_+3, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GROUPWND_PARTYGAGE7, SPR_INTERFACE_GROUPWND_PARTYGAGE, 0, 329, 1.0f, 1.0f, __ORDER_USERINTERFACE_START_+3, FALSE, FALSE, FALSE);	

	InsertData(SPR_OBJ_GROUPWND_PBCREATE1, SPR_INTERFACE_PARTYHELP_CREATE1, 5, 275, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GROUPWND_PBCREATE2, SPR_INTERFACE_PARTYHELP_CREATE2, 5, 275, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GROUPWND_PBCREATE3, SPR_INTERFACE_PARTYHELP_CREATE3, 5, 275, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GROUPWND_PBDESTROY1, SPR_INTERFACE_PARTYHELP_DESTROY1, 5, 310, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GROUPWND_PBDESTROY2, SPR_INTERFACE_PARTYHELP_DESTROY2, 5, 310, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GROUPWND_PBDESTROY3, SPR_INTERFACE_PARTYHELP_DESTROY3, 5, 310, 1.0f, 1.0f, 2);
		
	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(6, 25, 64, 44, 4, CHECK_BTN);
	InsertCheckInterface(68, 25, 126, 44, 5, CHECK_BTN);
	InsertCheckInterface(130, 25, 188, 44, 6, CHECK_BTN);		
	InsertCheckInterface(5, 100, 37, 132, 7, CHECK_BTN);
	InsertCheckInterface(5, 135, 37, 167, 8, CHECK_BTN);
	InsertCheckInterface(5, 170, 37, 202, 9, CHECK_BTN);
	InsertCheckInterface(5, 205, 37, 237, 10, CHECK_BTN);
	InsertCheckInterface(5, 240, 37, 272, 11, CHECK_BTN);		
	InsertCheckInterface(45, 110, 253, 132, 12, CHECK_BTN);
	InsertCheckInterface(45, 135, 253, 167, 13, CHECK_BTN);
	InsertCheckInterface(45, 170, 253, 202, 14, CHECK_BTN);
	InsertCheckInterface(45, 205, 253, 237, 15, CHECK_BTN);
	InsertCheckInterface(45, 240, 253, 272, 16, CHECK_BTN);
	InsertCheckInterface(45, 275, 253, 307, 17, CHECK_BTN);
	InsertCheckInterface(45, 310, 253, 342, 18, CHECK_BTN);
	InsertCheckInterface(45, 345, 253, 377, 19, CHECK_BTN);	
	InsertCheckInterface(93, 380, 112, 396, 20, CHECK_BTN);
	InsertCheckInterface(175, 380, 191, 396, 21, CHECK_BTN);
	InsertCheckInterface(5, 275, 37, 307, 22, CHECK_BTN);
	InsertCheckInterface(5, 310, 37, 342, 23, CHECK_BTN);
	InsertCheckInterface(5, 345, 37, 377, 24, CHECK_BTN);
	InsertCheckInterface(192, 25, 250, 44, 25, CHECK_BTN);
	*/
	InsertData(SPR_OBJ_GROUPWND_PARTYBAR1, SPR_INTERFACE_GROUPWND_PARTYBAR, 0, 116, 1.0f, 1.0f, __ORDER_USERINTERFACE_START_+2, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GROUPWND_PARTYBAR2, SPR_INTERFACE_GROUPWND_PARTYBAR, 0, 152, 1.0f, 1.0f, __ORDER_USERINTERFACE_START_+2, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GROUPWND_PARTYBAR3, SPR_INTERFACE_GROUPWND_PARTYBAR, 0, 188, 1.0f, 1.0f, __ORDER_USERINTERFACE_START_+2, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GROUPWND_PARTYBAR4, SPR_INTERFACE_GROUPWND_PARTYBAR, 0, 220, 1.0f, 1.0f, __ORDER_USERINTERFACE_START_+2, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GROUPWND_PARTYBAR5, SPR_INTERFACE_GROUPWND_PARTYBAR, 0, 260, 1.0f, 1.0f, __ORDER_USERINTERFACE_START_+2, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GROUPWND_PARTYBAR6, SPR_INTERFACE_GROUPWND_PARTYBAR, 0, 296, 1.0f, 1.0f, __ORDER_USERINTERFACE_START_+2, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GROUPWND_PARTYBAR7, SPR_INTERFACE_GROUPWND_PARTYBAR, 0, 332, 1.0f, 1.0f, __ORDER_USERINTERFACE_START_+2, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GROUPWND_PARTYGAGE1, SPR_INTERFACE_GROUPWND_PARTYGAGE, 0, 133, 1.0f, 1.0f, __ORDER_USERINTERFACE_START_+3, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GROUPWND_PARTYGAGE2, SPR_INTERFACE_GROUPWND_PARTYGAGE, 0, 169, 1.0f, 1.0f, __ORDER_USERINTERFACE_START_+3, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GROUPWND_PARTYGAGE3, SPR_INTERFACE_GROUPWND_PARTYGAGE, 0, 205, 1.0f, 1.0f, __ORDER_USERINTERFACE_START_+3, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GROUPWND_PARTYGAGE4, SPR_INTERFACE_GROUPWND_PARTYGAGE, 0, 241, 1.0f, 1.0f, __ORDER_USERINTERFACE_START_+3, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GROUPWND_PARTYGAGE5, SPR_INTERFACE_GROUPWND_PARTYGAGE, 0, 277, 1.0f, 1.0f, __ORDER_USERINTERFACE_START_+3, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GROUPWND_PARTYGAGE6, SPR_INTERFACE_GROUPWND_PARTYGAGE, 0, 313, 1.0f, 1.0f, __ORDER_USERINTERFACE_START_+3, FALSE, FALSE, FALSE);
	InsertData(SPR_OBJ_GROUPWND_PARTYGAGE7, SPR_INTERFACE_GROUPWND_PARTYGAGE, 0, 349, 1.0f, 1.0f, __ORDER_USERINTERFACE_START_+3, FALSE, FALSE, FALSE);	
	m_pSprStatus[0] = g_pRenderer->CreateSpriteObject(GetFile("menu_5.tga", DATA_TYPE_UI), 0, 144, 18, 18, 0);
	m_pSprStatus[1] = g_pRenderer->CreateSpriteObject(GetFile("menu_5.tga", DATA_TYPE_UI), 0, 162, 18, 18, 0);
	m_pSprStatus[2] = g_pRenderer->CreateSpriteObject(GetFile("menu_5.tga", DATA_TYPE_UI), 0, 180, 18, 18, 0);
	m_pSprStatus[3] = g_pRenderer->CreateSpriteObject(GetFile("menu_5.tga", DATA_TYPE_UI), 0, 198, 18, 18, 0);
	
	m_bInit = TRUE;

//��Ƽ ��ȭ �ý��� (����â 2005.01.28)
	m_byColor = 0;
	m_nExpDistrib	 = PARTY_CONFIG_EXP_SHARE; 
	m_nItemRouting	 = PARTY_CONFIG_ITEM_FIRST;
	m_bLevelMatch	 = 3;
	m_bExpMatch		 = 1;
	m_bItemMatch	 = 1;
	m_bMasterMatch	 = 0;    
//��Ƽ ��ȭ �ý���(����â 2005.02.01)	
	m_byBackBoard			= 0;
//	m_byPMatchStatus		= 0;
//	m_nPMatchResult			= -1;
	m_nPMatchEscapeResult   = -1;
    m_bMasterMatchStatus    = 0;

	m_byUserType = GUILD_USER_ONLINE;
	
	m_nGuildWarRankListPage = 1;
	m_nGuildWarRankListSelect = -1;
	m_nGuildWarRankListTotalCount = 0;
	m_nGuildWarRankListMaxPage = 0;
	m_byBtnChallange = 0;
	m_byBtnLeftDouble = 0;
	m_byBtnRightDouble = 0;

	ZeroMemory(m_GuildWarList, sizeof(m_GuildWarList));
	m_nGuildWarListPage = 1;
	m_nGuildWarListSelect = -1;
	m_nGuildWarListTotalCount = 0;
	m_nGuildWarListMaxPage = 0;
	m_byBtnOvserve = 0;
	
	m_byGuildWarDropDownEnableNum = 0;
	m_byGuildWarDropDownButtonStatus = MOUSE_NONE;
	ZeroMemory(m_szGuildWarMatch, sizeof(m_szGuildWarMatch));
	ZeroMemory(m_szGuildWarNumber, sizeof(m_szGuildWarNumber));
	ZeroMemory(m_szGuildWarType, sizeof(m_szGuildWarType));

//	wsprintf(m_szGuildWarMatch[0], "All");
	wsprintf(m_szGuildWarMatch[0], g_Message[ETC_MESSAGE1349].szMessage);//ų���ʵ�

//	wsprintf(m_szGuildWarNumber[0], "All");
	wsprintf(m_szGuildWarNumber[0], "2");
//	wsprintf(m_szGuildWarNumber[2], "3");
//	wsprintf(m_szGuildWarNumber[3], "4");

	wsprintf(m_szGuildWarType[0], g_Message[ETC_MESSAGE1350].szMessage);//���ı����
	wsprintf(m_szGuildWarType[1], g_Message[ETC_MESSAGE1351].szMessage);//������
	wsprintf(m_szGuildWarType[2], g_Message[ETC_MESSAGE1352].szMessage);//��嵵����

	m_bGuildWarChallenge = FALSE;

	m_bGuildWarPacketSendEnable = TRUE;
	
	return TRUE;
}

void CGroupWnd::Remove()
{
	RemoveAllData();

	for(int i = 0; i < 4; i++)
	{
		if(m_pSprStatus[i])
			m_pSprStatus[i]->Release();

	}	
	m_bInit		= FALSE;
	m_bActive	= FALSE;

	
}

DWORD CGroupWnd::GetGageResource(BYTE byIndex)
{
	switch(byIndex)
	{
	case 0:
		return SPR_OBJ_GROUPWND_PARTYGAGE1;
	case 1:
		return SPR_OBJ_GROUPWND_PARTYGAGE2;
	case 2:
		return SPR_OBJ_GROUPWND_PARTYGAGE3;
	case 3:
		return SPR_OBJ_GROUPWND_PARTYGAGE4;
	case 4:
		return SPR_OBJ_GROUPWND_PARTYGAGE5;
	case 5:
		return SPR_OBJ_GROUPWND_PARTYGAGE6;
	case 6:
		return SPR_OBJ_GROUPWND_PARTYGAGE7;
	}
	return MAX_PARTY_USER;
}

DWORD CGroupWnd::GetBarResource(BYTE byIndex)
{
	switch(byIndex)
	{
	case 0:
		return SPR_OBJ_GROUPWND_PARTYBAR1;
	case 1:
		return SPR_OBJ_GROUPWND_PARTYBAR2;
	case 2:
		return SPR_OBJ_GROUPWND_PARTYBAR3;
	case 3:
		return SPR_OBJ_GROUPWND_PARTYBAR4;
	case 4:
		return SPR_OBJ_GROUPWND_PARTYBAR5;
	case 5:
		return SPR_OBJ_GROUPWND_PARTYBAR6;
	case 6:
		return SPR_OBJ_GROUPWND_PARTYBAR7;
	}
	return MAX_PARTY_USER;
}

void CGroupWnd::SetPartyUI()
{
	if(m_bInit==FALSE)
		return;

	BYTE byIndex = 0;
	
	// Party UI //
	SetRender(SPR_OBJ_GROUPWND_PARTYBAR1, FALSE);
	SetRender(SPR_OBJ_GROUPWND_PARTYBAR2, FALSE);
	SetRender(SPR_OBJ_GROUPWND_PARTYBAR3, FALSE);
	SetRender(SPR_OBJ_GROUPWND_PARTYBAR4, FALSE);
	SetRender(SPR_OBJ_GROUPWND_PARTYBAR5, FALSE);
	SetRender(SPR_OBJ_GROUPWND_PARTYBAR6, FALSE);
	SetRender(SPR_OBJ_GROUPWND_PARTYBAR7, FALSE);
	SetRender(SPR_OBJ_GROUPWND_PARTYGAGE1, FALSE);
	SetRender(SPR_OBJ_GROUPWND_PARTYGAGE2, FALSE);
	SetRender(SPR_OBJ_GROUPWND_PARTYGAGE3, FALSE);
	SetRender(SPR_OBJ_GROUPWND_PARTYGAGE4, FALSE);
	SetRender(SPR_OBJ_GROUPWND_PARTYGAGE5, FALSE);
	SetRender(SPR_OBJ_GROUPWND_PARTYGAGE6, FALSE);
	SetRender(SPR_OBJ_GROUPWND_PARTYGAGE7, FALSE);
	SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE);		

	ListNode<PARTY_USER>* pNode = g_pPartyUserHash->GetHead();

	while(pNode)
	{
		LP_PARTY_USER sPartyNode = pNode->pData;

		CUser* pUser = g_pUserHash->GetData(sPartyNode->dwUserIndex);

		if(pUser)
		{
			// Render //						
			SetScalingObj(GetGageResource(byIndex), (float)sPartyNode->wHp/(float)sPartyNode->wMaxHp*50.0f, 1.0f);
			SetRender(GetBarResource(byIndex), TRUE);
			SetRender(GetGageResource(byIndex), TRUE);
			byIndex++;
		}

		pNode = pNode->pNext;
	}	
}

void CGroupWnd::SetOrder()
{
}

void CGroupWnd::RenderText()
{		
	if(m_byGroupTab==GROUP_TAB_PARTY)
		RenderPartyInfo();
	else if(m_byGroupTab==GROUP_TAB_GUILD)
		RenderGuildInfo();
	else if(m_byGroupTab==GROUP_TAB_FRIEND)
		RenderFriendInfo();
	else if(m_byGroupTab==GROUP_TAB_GUILDWAR)
		RenderGuildWarInfoNew();

	// ���� �̸�, ���� //
	if(m_bInfo==TRUE)
	{
		DUNGEON_DATA_EX* pDungeonInfo = g_pDungeonTable->GetDungeonInfo(m_wDungeonID);
		
		if(pDungeonInfo)
		{
			if(!IsEmptyString(pDungeonInfo->m_szDungeonName))
			{
				if(pDungeonInfo->m_dwID/1000!=0)
				{
					if(m_byLayerIndex==0)
					{
#if IS_JAPAN_LOCALIZING()
						wsprintf(m_szInfo[0], g_Message[ETC_MESSAGE568].szMessage, m_szName, 
							lstrlen(pDungeonInfo->m_szSchoolName) ? pDungeonInfo->m_szSchoolName : pDungeonInfo->m_szDungeonName); // "%s ���� ���� %s ����"
#else
						wsprintf(m_szInfo[0], g_Message[ETC_MESSAGE568].szMessage, m_szName, pDungeonInfo->m_szDungeonName); // "%s ���� ���� %s ����"
#endif
//						wsprintf(m_szInfo[1], g_Message[ETC_MESSAGE569].szMessage ); // "���� 1���� �ֽ��ϴ�."
						lstrcpy(m_szInfo[1], g_Message[ETC_MESSAGE569].szMessage ); // "���� 1���� �ֽ��ϴ�."
					}
					else
					{
#if IS_JAPAN_LOCALIZING()
						wsprintf(m_szInfo[0], g_Message[ETC_MESSAGE568].szMessage, m_szName, 
							lstrlen(pDungeonInfo->m_szSchoolName) ? pDungeonInfo->m_szSchoolName : pDungeonInfo->m_szDungeonName, m_byLayerIndex); // "%s ���� ���� %s ����"
#else
						wsprintf(m_szInfo[0], g_Message[ETC_MESSAGE568].szMessage, m_szName, pDungeonInfo->m_szDungeonName, m_byLayerIndex); // "%s ���� ���� %s ����"
#endif
						wsprintf(m_szInfo[1], g_Message[ETC_MESSAGE570].szMessage, m_byLayerIndex); // "���� %u���� �ֽ��ϴ�."
					}											
				}
				else
				{
#if IS_JAPAN_LOCALIZING()
					wsprintf(m_szInfo[0], g_Message[ETC_MESSAGE571].szMessage, m_szName, 
						lstrlen(pDungeonInfo->m_szSchoolName) ? pDungeonInfo->m_szSchoolName : pDungeonInfo->m_szDungeonName);	//"%s ���� ���� %s ������"											
#else
					wsprintf(m_szInfo[0], g_Message[ETC_MESSAGE571].szMessage, m_szName, pDungeonInfo->m_szDungeonName);	//"%s ���� ���� %s ������"											
#endif
					lstrcpy(m_szInfo[1], g_Message[ETC_MESSAGE572].szMessage); // " �ֽ��ϴ�."
				}

				for(int i = 0; i < 2; i++)
				{						
					int nSize = lstrlen(m_szInfo[i]);
					RenderFont(m_szInfo[i], m_fPosX+10, m_fPosX+10+(nSize*7), m_fPosZ+419+(i*20), m_fPosZ+433+(i*20), GetStartOrder()+4);
				}
			}
		}
		else
		{
			wsprintf(m_szInfo[0], g_Message[ETC_MESSAGE732].szMessage, m_szName); // "%s ���� ���� ���忡"
			lstrcpy(m_szInfo[1], g_Message[ETC_MESSAGE572].szMessage); // "�ֽ��ϴ�."

			for(int i = 0; i < 2; i++)
			{						
				int nSize = lstrlen(m_szInfo[i]);				
				RenderFont(m_szInfo[i], m_fPosX+10, m_fPosX+10+(nSize*7), m_fPosZ+458+(i*20), m_fPosZ+472+(i*20), GetStartOrder()+4);				
			}
		}

		// ���� �ð� üũ /
		m_dwCurTime	= timeGetTime();

		if((m_dwCurTime-m_dwPrevTime)>3000)
			m_bInfo	= FALSE;
	}	
}

void CGroupWnd::SetActive(BOOL bActive)
{
	m_bActive = bActive;
	
	if(bActive==FALSE)
	{
		m_bGuildBtn[0]	= FALSE;
		m_bGuildBtn[1]	= FALSE;
		m_bGuildBtn[2]	= FALSE;
		m_bGuildBtn[3]	= FALSE;
		m_bGuildBtn[4]	= FALSE;
		m_bPartyBtn[0]	= FALSE;
		m_bPartyBtn[1]	= FALSE;
		m_bPartyBtn[2]	= FALSE;		
		m_bPartyBtn[3]	= FALSE;		
		ShowSpriteAll();
		
		ListNode<PARTY_USER>* pNode = g_pPartyUserHash->GetHead();

		BYTE byIndex = 0;
		
		while(pNode)
		{
			LP_PARTY_USER sPartyNode = pNode->pData;

			CUser* pUser = g_pUserHash->GetData(sPartyNode->dwUserIndex);

			if(pUser)
			{
				// Render //						
				SetScalingObj(GetGageResource(byIndex), (float)sPartyNode->wHp/(float)sPartyNode->wMaxHp*50.0f, 1.0f);
				SetRender(GetBarResource(byIndex), TRUE);
				SetRender(GetGageResource(byIndex), TRUE);
				byIndex++;
			}

			pNode = pNode->pNext;
		}
		
		m_nPartyUserIndex = -1;		

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
//		CInterface::GetInstance()->SetWnd(m_nIndex);

		SetRender(SPR_OBJ_GROUPWND, TRUE);
		SetRender(SPR_OBJ_GROUPWND_CLOSE1, TRUE);
		OpenTab();
		SetPosGroup();

		if(m_byGroupTab==GROUP_TAB_PARTY)
		{
			if(g_pMainPlayer->m_dwPartyId==0)
				SetStatus(GROUP_PARTY_CREATEADD);
			else		
			{		
				if(m_nPartyUserIndex==-1)
					SetStatus(GROUP_PARTY_JOIN);			
			}		
			m_byBackBoard = 0;
		}	
		else if(m_byGroupTab==GROUP_TAB_GUILD)
		{
			int nPage = 0;

			if(m_byUserType==GUILD_USER_ONLINE)
			{
				nPage = (g_pGuildUserHash->GetCount()+1)/8;
					
				if(((g_pGuildUserHash->GetCount()+1)%8)==0)
					nPage -= 1;
				
				m_byMaxPage	= 1+nPage;
			}
			else
			{
				nPage = g_pGuildOfflineHash->GetCount()/8;

				if(nPage!=0 && nPage%8==0)
					nPage -= 1;
					
				m_byMaxPage	= 1+nPage;
			}

			if(m_byCurPage>m_byMaxPage)
				m_byCurPage = 1;
			else if(m_byCurPage<1)
				m_byCurPage = 1;		

			SetStatus(GROUP_GUILD_DEFAULT);
		}
		else if(m_byGroupTab==GROUP_TAB_FRIEND)
		{
			int nPage = 0;

			if(m_byMessengerType==FRIEND_USER_ONLINE)
			{
				nPage = g_pMessengerOnLineHash->GetCount()/8;
				
				if((g_pMessengerOnLineHash->GetCount()%8)==0)
					nPage -= 1;
				
				m_byMaxPage	= 1+nPage;
			}
			else
			{
				nPage = g_pMessengerOffLineHash->GetCount()/8;

				if(nPage!=0 && nPage%8==0)
					nPage -= 1;
					
				m_byMaxPage	= 1+nPage;
			}

			if(m_byCurPage>m_byMaxPage)
				m_byCurPage = 1;
			else if(m_byCurPage<1)
				m_byCurPage = 1;
			
			SetStatus(GROUP_FRIEND_DEFAULT);		
			SetPrevNext(__FRIEND_PAGE_CURR__);
		}
		else if(m_byGroupTab==GROUP_TAB_GUILDWAR)
		{
			if(m_bGuildWarPacketSendEnable)
			{
				m_bGuildWarPacketSendEnable = FALSE;
				TEAM_MATCH_PACKET TeamMatch;
				TeamMatch.Body.PageNum.iPageNumber = m_nGuildWarRankListPage = 1;
				Cmd_CTWS_GuildWarPacket(TeamMatch, CTWS_TM_LADDER_LIST);
//				TempSetRandomData();//�ӽ�
				SetStatus(GROUP_GUILDWAR_MATCH);
				m_nGuildWarRankListSelect = -1;
				m_byBtnChallange = 0;
				m_byBtnOvserve = 0;
				m_byBtnLeftDouble = 0;
				m_byBtnRightDouble = 0;

				if(!g_pMainPlayer->m_dwGuildId)//��� ������ ������ �� �����.
				{
					m_byGuildWarType = 1;
				}
			}
			else
			{
				m_byGroupTab = GROUP_TAB_PARTY;
				if(g_pMainPlayer->m_dwPartyId==0)
					SetStatus(GROUP_PARTY_CREATEADD);
				else		
				{		
					if(m_nPartyUserIndex==-1)
						SetStatus(GROUP_PARTY_JOIN);			
				}
			}	
		}
				
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}	
}

void CGroupWnd::SetStatus(BYTE byStatus)
{
	m_byStatus = byStatus;

	if(!GetActive())
		return;
	
	SetRender(SPR_OBJ_GUILD_PAGE, FALSE);
	SetRender(SPR_OBJ_GUILD_PAGEPREV1, FALSE);
	SetRender(SPR_OBJ_GUILD_PAGEPREV2, FALSE);
	SetRender(SPR_OBJ_GUILD_PAGEPREV3, FALSE);
	SetRender(SPR_OBJ_GUILD_PAGENEXT1, FALSE);
	SetRender(SPR_OBJ_GUILD_PAGENEXT2, FALSE);
	SetRender(SPR_OBJ_GUILD_PAGENEXT3, FALSE);
	
	SetRender(SPR_OBJ_GUILD_ONOFF1, FALSE);
	SetRender(SPR_OBJ_GUILD_ONOFF2, FALSE);
	SetRender(SPR_OBJ_GUILD_ONOFF3, FALSE);
	SetRender(SPR_OBJ_GUILD_INFO1, FALSE);
	SetRender(SPR_OBJ_GUILD_INFO2, FALSE);
	SetRender(SPR_OBJ_GUILD_INFO3, FALSE);

	SetRender(SPR_OBJ_GROUPWND_ADD1, FALSE);
	SetRender(SPR_OBJ_GROUPWND_ADD2, FALSE);
	SetRender(SPR_OBJ_GROUPWND_ADD3, FALSE);
	SetRender(SPR_OBJ_GROUPWND_DELETE1, FALSE);
	SetRender(SPR_OBJ_GROUPWND_DELETE2, FALSE);
	SetRender(SPR_OBJ_GROUPWND_DELETE3, FALSE);
	SetRender(SPR_OBJ_GROUPWND_INFO1, FALSE);
	SetRender(SPR_OBJ_GROUPWND_INFO2, FALSE);
	SetRender(SPR_OBJ_GROUPWND_INFO3, FALSE);		
	SetRender(SPR_OBJ_GROUPWND_OUT1, FALSE);
	SetRender(SPR_OBJ_GROUPWND_OUT2, FALSE);
	SetRender(SPR_OBJ_GROUPWND_OUT3, FALSE);
	SetRender(SPR_OBJ_GROUPWND_MSG1, FALSE);
	SetRender(SPR_OBJ_GROUPWND_MSG2, FALSE);
	SetRender(SPR_OBJ_GROUPWND_MSG3, FALSE);

	SetRender(SPR_OBJ_GROUP_MASTER1, FALSE);
	SetRender(SPR_OBJ_GROUP_MASTER2, FALSE);
	SetRender(SPR_OBJ_GROUP_MASTER3, FALSE);

	SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);

	SetRender(SPR_OBJ_GROUPWND_GUILDWARADD1, FALSE);
	SetRender(SPR_OBJ_GROUPWND_GUILDWARADD2, FALSE);
	SetRender(SPR_OBJ_GROUPWND_GUILDWARADD3, FALSE);
	SetRender(SPR_OBJ_GROUPWND_GUILDWAREXIT1, FALSE);
	SetRender(SPR_OBJ_GROUPWND_GUILDWAREXIT2, FALSE);
	SetRender(SPR_OBJ_GROUPWND_GUILDWAREXIT3, FALSE);

	SetRender(SPR_OBJ_GROUPWND_PBCREATE1, FALSE);
	SetRender(SPR_OBJ_GROUPWND_PBCREATE2, FALSE);
	SetRender(SPR_OBJ_GROUPWND_PBCREATE3, FALSE);
	SetRender(SPR_OBJ_GROUPWND_PBDESTROY1, FALSE);
	SetRender(SPR_OBJ_GROUPWND_PBDESTROY2, FALSE);
	SetRender(SPR_OBJ_GROUPWND_PBDESTROY3, FALSE);

//	SetRender(SPR_OBJ_GUILDWAR_GUILD, FALSE);		
	
	//��Ƽ ��ȭ �ý��� (����â 2005.01.25)
//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, FALSE);     
	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, FALSE);
	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO3, FALSE);
//	SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION1, FALSE);
	SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, FALSE);
	SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION3, FALSE);
//	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION1, FALSE);  
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION2, FALSE);
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION3, FALSE);
//	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK, FALSE);
//	SetRender(SPR_OBJ_GROUPWND_PARTYFRIENDADD, FALSE);
	SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP1, FALSE);
	SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP2, FALSE);
	SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP3, FALSE);
	SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP4, FALSE);
	SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP5, FALSE);
	SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP6, FALSE);   
	SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM1, FALSE);
	SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM2, FALSE);
	SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM3, FALSE);
	SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM4, FALSE);
	SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM5, FALSE);
	SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM6, FALSE); 
	SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITI_OK1, FALSE);
	SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITI_OK2, FALSE);
	SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITI_CANCEL1, FALSE);
//	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK1, FALSE);
//	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL1, FALSE);
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, FALSE);
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV1, FALSE);					
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV2, FALSE);						
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV3, FALSE);						 
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV4, FALSE);						 
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV5, FALSE);					
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV6, FALSE);						
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP1, FALSE);						
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP2, FALSE);						 
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP3, FALSE);						 
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP4, FALSE);						 
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP5, FALSE);						 
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP6, FALSE);						 
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM1, FALSE);						
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM2, FALSE);						 
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM3, FALSE);						 
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM4, FALSE);						 
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM5, FALSE);						 
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM6, FALSE);						 
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO1, FALSE);					
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO2, FALSE);					
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO3, FALSE);					 
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO4, FALSE);					 
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO5, FALSE);					
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO6, FALSE);
	//��Ƽ �ý��� ģ�� �߰� (����â 2005.02.22)
	SetRender(SPR_OBJ_GROUPWND_FRIENDADD_OFF1, FALSE);
	SetRender(SPR_OBJ_GROUPWND_FRIENDADD_OFF2, FALSE);
	SetRender(SPR_OBJ_GROUPWND_FRIENDADD_OFF3, FALSE);
	SetRender(SPR_OBJ_GROUPWND_FRIENDADD_OFF4, FALSE);
	SetRender(SPR_OBJ_GROUPWND_FRIENDADD_OFF5, FALSE);
	SetRender(SPR_OBJ_GROUPWND_FRIENDADD_OFF6, FALSE);
	SetRender(SPR_OBJ_GROUPWND_FRIENDADD_OFF7, FALSE);
	SetRender(SPR_OBJ_GROUPWND_FRIENDADD_ON1, FALSE);
	SetRender(SPR_OBJ_GROUPWND_FRIENDADD_ON2, FALSE);
	SetRender(SPR_OBJ_GROUPWND_FRIENDADD_ON3, FALSE);
	SetRender(SPR_OBJ_GROUPWND_FRIENDADD_ON4, FALSE);
	SetRender(SPR_OBJ_GROUPWND_FRIENDADD_ON5, FALSE);
	SetRender(SPR_OBJ_GROUPWND_FRIENDADD_ON6, FALSE);
	SetRender(SPR_OBJ_GROUPWND_FRIENDADD_ON7, FALSE);


	if(m_byGroupTab==GROUP_TAB_PARTY)
	{
		switch(m_byStatus)
		{
		case GROUP_PARTY_DEFAULT:
			SetRender(SPR_OBJ_GROUPWND_ADD1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			//��Ƽ ��ȭ �ý��� (����â 2005.01.25)
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1,TRUE);  
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2,TRUE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2,TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION2,TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
		


			
#ifdef PARTY_BOARD
			
			if(g_pMainPlayer->m_dwPartyId==0)
			{
				SetRender(SPR_OBJ_GROUPWND_PBCREATE1, TRUE);	
				SetRender(SPR_OBJ_GROUPWND_PBDESTROY1, TRUE);	
			}
			else
			{
				
				if(!g_pPartyBoardHash->GetData(g_pMainPlayer->m_dwPartyId))
				{
					SetRender(SPR_OBJ_GROUPWND_PBCREATE2, TRUE);	
					SetRender(SPR_OBJ_GROUPWND_PBDESTROY1, TRUE);	
				}
				else
				{
					SetRender(SPR_OBJ_GROUPWND_PBCREATE1, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PBDESTROY2, TRUE);	
				}
			}
			
#endif
			break;		
		case GROUP_PARTY_CREATEADD:
			if(m_byPMatchStatus==0)
			{
				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, FALSE);
			
		//		if(m_nPMatchResult==0 && g_pMainPlayer->m_bPartyLeader==1)
		//		{
		//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
		//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
		//		}
			}
			else if(m_byPMatchStatus==1)
			{
				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
			
				if(m_nPMatchResult==0 && g_pMainPlayer->m_dwPartyId != 0)    // ��Ī�ؼ� ��Ƽ�� ��� �� ��� ��Ƽ���� �� Ȱ��ȭ
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, FALSE);
	//				m_byPMatchStatus = 0;
				}
				if(m_nPMatchResult==0 && g_pMainPlayer->m_bPartyLeader==1)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
				}
				else if(m_nPMatchResult==0 && g_pMainPlayer->m_dwPartyId == 0)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
				}
	//			else if(m_nPMatchResult!=0 && g_pMainPlayer->m_dwPartyId == 0)
	//			{
	//				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
	//				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, FALSE);
	//			}
				if(m_bMasterMatchStatus==1 && g_pMainPlayer->m_bPartyLeader==1)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
				}
				if(m_bMasterMatchStatus==1 && g_pMainPlayer->m_bPartyLeader==1 && g_pMainPlayer->m_dwPartyId == 0)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
				}
				if(m_bMasterMatchStatus==1 && g_pMainPlayer->m_dwPartyId == 0)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
				}
			}
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
			//��Ƽ ��ȭ �ý��� (����â 2005.01.25)
	//		SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1,TRUE);  
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2,TRUE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2,TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION2,TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
	

#ifdef PARTY_BOARD
			
			if(g_pMainPlayer->m_dwPartyId==0)
			{
				SetRender(SPR_OBJ_GROUPWND_PBCREATE1, TRUE);	
				SetRender(SPR_OBJ_GROUPWND_PBDESTROY1, TRUE);	
			}
			else
			{
				
				if(!g_pPartyBoardHash->GetData(g_pMainPlayer->m_dwPartyId))
				{
					SetRender(SPR_OBJ_GROUPWND_PBCREATE2, TRUE);	
					SetRender(SPR_OBJ_GROUPWND_PBDESTROY1, TRUE);	
				}
				else
				{
					SetRender(SPR_OBJ_GROUPWND_PBCREATE1, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PBDESTROY2, TRUE);	
				}
			}
			
#endif
			break;
		case GROUP_PARTY_CREATEADDCLK:
			SetRender(SPR_OBJ_GROUPWND_ADD3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
			//��Ƽ ��ȭ �ý��� (����â 2005.01.25)
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1,TRUE);                  
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2,TRUE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2,TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION2,TRUE); 
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);

#ifdef PARTY_BOARD
			
			SetRender(SPR_OBJ_GROUPWND_PBCREATE1, TRUE);	
			SetRender(SPR_OBJ_GROUPWND_PBDESTROY1, TRUE);				

#endif
			break;		
		case GROUP_PARTY_JOIN:
			if(m_byPMatchStatus==0)
			{
				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, FALSE);
			

		//		if(m_nPMatchResult==0 && g_pMainPlayer->m_bPartyLeader==1)
		//		{
		//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
		//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
		//		}

			}
	//		if(m_byPMatchStatus==0 && g_pMainPlayer->m_bPartyLeader==1)
	//		{
	//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
	//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
	//		}
			else if(m_byPMatchStatus==1)
			{
				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
			

				if(m_nPMatchResult==0 && g_pMainPlayer->m_dwPartyId != 0)  // ��Ī�ؼ� ��Ƽ�� ��� �� ��� ��Ƽ���� �� Ȱ��ȭ 
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, FALSE);
					m_byPMatchStatus = 0;
				}
				if(m_nPMatchResult==0 && g_pMainPlayer->m_bPartyLeader==1)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
					m_byPMatchStatus = 1;//�߰� 
				}
				else if(m_nPMatchResult==0 && g_pMainPlayer->m_dwPartyId == 0)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
				}
				if(m_bMasterMatchStatus==1 && g_pMainPlayer->m_bPartyLeader==1)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
				}
				if(m_bMasterMatchStatus==1 && g_pMainPlayer->m_bPartyLeader==1 && g_pMainPlayer->m_dwPartyId == 0)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
				}
				if(m_bMasterMatchStatus==1 && g_pMainPlayer->m_dwPartyId == 0)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
				}
			}
			
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);						
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
			//��Ƽ ��ȭ �ý��� (����â 2005.01.25)
	//		SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1,TRUE);  
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2,TRUE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2,TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION2,TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);


#ifdef PARTY_BOARD
			
			if(g_pMainPlayer->m_dwPartyId==0)
			{
				SetRender(SPR_OBJ_GROUPWND_PBCREATE1, TRUE);	
				SetRender(SPR_OBJ_GROUPWND_PBDESTROY1, TRUE);	
			}
			else
			{
				
				if(!g_pPartyBoardHash->GetData(g_pMainPlayer->m_dwPartyId))
				{
					SetRender(SPR_OBJ_GROUPWND_PBCREATE2, TRUE);	
					SetRender(SPR_OBJ_GROUPWND_PBDESTROY1, TRUE);	
				}
				else
				{
					SetRender(SPR_OBJ_GROUPWND_PBCREATE1, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PBDESTROY2, TRUE);	
				}
			}
			
#endif
			break;
		case GROUP_PARTY_SELECT:
			if(g_pMainPlayer->m_bPartyLeader==1)			
				SetRender(SPR_OBJ_GROUPWND_DELETE2, TRUE);
			else
				SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);

			SetRender(SPR_OBJ_GROUPWND_ADD1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG2, TRUE);	
			//��Ƽ ��ȭ �ý��� (����â 2005.01.25)
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1,TRUE); 
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2,TRUE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2,TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION2,TRUE); 
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
	//		SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);

			if(m_byPMatchStatus==0)
			{
				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, FALSE);			
			}
			else if(m_byPMatchStatus==1)
			{
				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
			

				if(m_nPMatchResult==0 && g_pMainPlayer->m_dwPartyId != 0)     // ��Ī�ؼ� ��Ƽ�� ��� �� ��� ��Ƽ���� �� Ȱ��ȭ
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, FALSE);
		//			m_byPMatchStatus = 0;
				}
				if(m_nPMatchResult==0 && g_pMainPlayer->m_bPartyLeader==1)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
				}
				else if(m_nPMatchResult==0 && g_pMainPlayer->m_dwPartyId == 0)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
				}
				if(m_bMasterMatchStatus==1 && g_pMainPlayer->m_bPartyLeader==1)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
				}
				if(m_bMasterMatchStatus==1 && g_pMainPlayer->m_bPartyLeader==1 && g_pMainPlayer->m_dwPartyId == 0)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
				}
				if(m_bMasterMatchStatus==1 && g_pMainPlayer->m_dwPartyId == 0)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
				}
			}
			
#ifdef PARTY_BOARD

			SetRender(SPR_OBJ_GROUPWND_PBCREATE1, TRUE);	
			SetRender(SPR_OBJ_GROUPWND_PBDESTROY1, TRUE);	
#endif
			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.26) 
		case GROUP_PARTY_PARTYUSERINFO1:       
			if(m_byPMatchStatus==0)
			{
				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, FALSE);
			}
			else if(m_byPMatchStatus==1)
			{	
				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
				
				if(m_nPMatchResult==0 && g_pMainPlayer->m_dwPartyId != 0)       // ��Ī�ؼ� ��Ƽ�� ��� �� ��� ��Ƽ���� �� Ȱ��ȭ
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, FALSE);
	//				m_byPMatchStatus = 0;
				}
				if(m_nPMatchResult==0 && g_pMainPlayer->m_bPartyLeader==1)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
//					m_byPMatchStatus = 1;
				}
				else if(m_nPMatchResult==0 && g_pMainPlayer->m_dwPartyId == 0)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
				}
				if(m_bMasterMatchStatus==1 && g_pMainPlayer->m_bPartyLeader==1)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
				}
				if(m_bMasterMatchStatus==1 && g_pMainPlayer->m_bPartyLeader==1 && g_pMainPlayer->m_dwPartyId == 0)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
				}
				if(m_bMasterMatchStatus==1 && g_pMainPlayer->m_dwPartyId == 0)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
				}
			}			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
			SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE);

			break;

		case GROUP_PARTY_PARTYUSERINFO2:
			if(m_byPMatchStatus==0)
			{
				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, FALSE);
			}
			else if(m_byPMatchStatus==1)
			{	
				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
				
				if(m_nPMatchResult==0 && g_pMainPlayer->m_dwPartyId != 0)       // ��Ī�ؼ� ��Ƽ�� ��� �� ��� ��Ƽ���� �� Ȱ��ȭ
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, FALSE);
	//				m_byPMatchStatus = 0;
				}
				if(m_nPMatchResult==0 && g_pMainPlayer->m_bPartyLeader==1)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
//					m_byPMatchStatus = 1;
				}
				else if(m_nPMatchResult==0 && g_pMainPlayer->m_dwPartyId == 0)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
				}
				if(m_bMasterMatchStatus==1 && g_pMainPlayer->m_bPartyLeader==1)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
				}
				if(m_bMasterMatchStatus==1 && g_pMainPlayer->m_bPartyLeader==1 && g_pMainPlayer->m_dwPartyId == 0)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
				}
				if(m_bMasterMatchStatus==1 && g_pMainPlayer->m_dwPartyId == 0)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
				}
			}
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
			//��Ƽ �ý���(����â 2005.03.02)
			SetRender(SPR_OBJ_GROUPWND_STATUS, TRUE);
			SetRender(SPR_OBJ_GUILD_PAGE, TRUE);
			SetRender(SPR_OBJ_GUILD_PAGEPREV1, TRUE);
			SetRender(SPR_OBJ_GUILD_PAGENEXT1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE);

			break;   
			
		//��Ƽ ��ȭ �ý��� (����â 2005.01.26)
		case GROUP_PARTY_EXPITEMCONDITION1:
			if(m_byPMatchStatus==0)
			{
				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, FALSE);
			}
			else if(m_byPMatchStatus==1)
			{	
			
				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
				
				

				if(m_nPMatchResult==0 && g_pMainPlayer->m_dwPartyId != 0)       // ��Ī�ؼ� ��Ƽ�� ��� �� ��� ��Ƽ���� �� Ȱ��ȭ
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, FALSE);
	//				m_byPMatchStatus = 0;
				}
				if(m_nPMatchResult==0 && g_pMainPlayer->m_bPartyLeader==1)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
//					m_byPMatchStatus = 1;
				}
				else if(m_nPMatchResult==0 && g_pMainPlayer->m_dwPartyId == 0)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
				}
				if(m_bMasterMatchStatus==1 && g_pMainPlayer->m_bPartyLeader==1)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
				}
				if(m_bMasterMatchStatus==1 && g_pMainPlayer->m_bPartyLeader==1 && g_pMainPlayer->m_dwPartyId == 0)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
				}
				if(m_bMasterMatchStatus==1 && g_pMainPlayer->m_dwPartyId == 0)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
				}
			}
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP6, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM6, FALSE);   
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITI_OK1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITI_CANCEL1, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
			
			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.26)
		case GROUP_PARTY_EXPITEMCONDITION2:
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
			

			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.26)
		case GROUP_PARTY__CONDITIONLIST_EXP1:						//��ӹڽ� ������ DOWN Ŭ���� ����Ʈ ����.
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP4, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP5, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP6, TRUE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITI_OK1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITI_CANCEL1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
			
			
			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.26)
		case GROUP_PARTY__CONDITIONLIST_EXP2:						//ó�� ��ӹڽ� ������ DOWN->UP���� ��� ����Ʈ ���� ���·� �����ϰ� ��ӹڽ� �����ܸ� ����  
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP4, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP5, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP6, TRUE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
	//		SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM6, FALSE);  
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITI_OK1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITI_CANCEL1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
		

			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.26)
		case GROUP_PARTY__CONDITIONLIST_EXP3:						//�ٽ� ��ӹڽ� ������ DOWN Ŭ���� ����Ʈ ����.
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP4, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP5, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP6, FALSE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
	//		SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM6, FALSE);   
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITI_OK1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITI_CANCEL1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
		

			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.26)
		case GROUP_PARTY__CONDITIONLIST_EXP4:						//��ӹڽ� �����ܿ� over���� ��� 
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP5, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP6, FALSE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM6, FALSE);  
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITI_OK1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITI_CANCEL1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
		
			
			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.26)
		case GROUP_PARTY__CONDITIONLIST_EXP5:						//��ӹڽ� ����Ʈ ���ý� ����Ʈ ������ ������ ���� 
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP6, FALSE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM6, FALSE);  
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITI_OK1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITI_CANCEL1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
			

			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.26)
		case GROUP_PARTY__CONDITIONLIST_ITEM1:						//��ӹڽ� ������ DOWN Ŭ���� ����Ʈ ����.
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP6, FALSE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);	
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM4, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM5, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM6, TRUE);   
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITI_OK1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITI_CANCEL1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
		

			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.26)
		case GROUP_PARTY__CONDITIONLIST_ITEM2:						//ó�� ��ӹڽ� ������ DOWN->UP���� ��� ����Ʈ ���� ���·� �����ϰ� ��ӹڽ� �����ܸ� ����  
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP6, FALSE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM4, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM5, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM6, TRUE);   
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITI_OK1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITI_CANCEL1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
		
			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.26)
		case GROUP_PARTY__CONDITIONLIST_ITEM3:						//�ٽ� ��ӹڽ� ������ DOWN Ŭ���� ����Ʈ ����.
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP6, FALSE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);	
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM4, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM5, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM6, FALSE);  
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITI_OK1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITI_CANCEL1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
		

			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.26)
		case GROUP_PARTY__CONDITIONLIST_ITEM4:						//��ӹڽ� �����ܿ� over���� ��� 
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP6, FALSE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM5, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITI_OK1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITI_CANCEL1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
		
			
			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.26)
		case GROUP_PARTY__CONDITIONLIST_ITEM5:							//��ӹڽ� ����Ʈ ���ý� ����Ʈ ������ ������ ���� 
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP6, FALSE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITI_OK1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITI_CANCEL1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
		

			break;
	
		//��Ƽ ��ȭ �ý��� (����â 2005.01.31)
/*		case GROUP_PARTY__EXPITEMCONDITI_OK:						//EXPITEMCONDITI Ȯ�� ��ư 	
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_EXP6, FALSE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST_ITEM6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITI_OK1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITI_CANCEL1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);

			break; */  

		//��Ƽ ��ȭ �ý��� (����â 2005.01.27)
		case GROUP_PARTY_PMATCHCONDITION1:                         //��Ƽ �˻� ���� �޴� ��ư (������ ���)
			if(m_byPMatchStatus==0)
			{
				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, FALSE);
			}
			else if(m_byPMatchStatus==1)
			{
			//	if(g_pMainPlayer->m_bPartyLeader==1 && g_pMainPlayer->m_dwPartyId != 0)
			//	{
				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
				SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
				
	
			//	}

				if(m_nPMatchResult==0 && g_pMainPlayer->m_bPartyLeader!=1 && g_pMainPlayer->m_dwPartyId != 0) // ��Ī�ؼ� ��Ƽ�� ��� �� ��� ��Ƽ���� �� Ȱ��ȭ
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, FALSE);
		//			m_byPMatchStatus = 0;
				}		
				if(m_nPMatchResult==0 && g_pMainPlayer->m_bPartyLeader==1)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
			
				}
				else if(m_nPMatchResult==0 && g_pMainPlayer->m_dwPartyId == 0)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);   
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
				}
				
				if(m_bMasterMatchStatus==1 && g_pMainPlayer->m_bPartyLeader==1)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
				}
				if(m_bMasterMatchStatus==1 && g_pMainPlayer->m_bPartyLeader==1 && g_pMainPlayer->m_dwPartyId == 0)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
				}
				if(m_bMasterMatchStatus==1 && g_pMainPlayer->m_dwPartyId == 0)
				{
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
					SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
				}
			}
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK1, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV1, TRUE);					
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV2, TRUE);						
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV3, FALSE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV4, FALSE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV5, TRUE);					
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV6, FALSE);						
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP1, TRUE);						
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP2, TRUE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP3, FALSE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP4, FALSE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP5, TRUE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP6, FALSE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM1, TRUE);						
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM2, TRUE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM3, FALSE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM4, FALSE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM5, TRUE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM6, FALSE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO1, TRUE);					
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO2, TRUE);					
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO3, FALSE);					 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO4, FALSE);					 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO5, TRUE);					
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO6, FALSE);
				
	       break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.27)
		case GROUP_PARTY_PMATCHCONDITION_LEV1:							//��ӹڽ� ������ DOWN Ŭ���� ����Ʈ ����.
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV4, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV5, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV6, TRUE);  
			m_byBackBoard = 1;
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK1, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL1, TRUE);
		
			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.27)
		case GROUP_PARTY_PMATCHCONDITION_LEV2:							//ó�� ��ӹڽ� ������ DOWN->UP���� ��� ����Ʈ ���� ���·� �����ϰ� ��ӹڽ� �����ܸ� ����  
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV4, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV5, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV6, TRUE);  
			m_byBackBoard = 1;
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK1, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL1, TRUE);
		
			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.27)
		case GROUP_PARTY_PMATCHCONDITION_LEV3:							//�ٽ� ��ӹڽ� ������ DOWN Ŭ���� ����Ʈ ����.
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV4, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV5, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV6, FALSE);  
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK1, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL1, TRUE);
			
			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.27)
		case GROUP_PARTY_PMATCHCONDITION_LEV4:								//��ӹڽ� �����ܿ� over���� ��� 
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV5, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV6, FALSE);  
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK1, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL1, TRUE);

			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.30)
		case GROUP_PARTY_PMATCHCONDITION_LEV5:							//��ӹڽ� ����Ʈ ���ý� ����Ʈ ������ ������ ���� 
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV6, FALSE);  
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK1, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL1, TRUE);

			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.27)
		case GROUP_PARTY_PMATCHCONDITION_EXP1:
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV6, FALSE);  
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP4, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP5, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP6, TRUE); 
			m_byBackBoard = 2;
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK1, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL1, TRUE);

			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.27)
		case GROUP_PARTY_PMATCHCONDITION_EXP2:
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV6, FALSE);  
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP4, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP5, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP6, TRUE); 
			m_byBackBoard = 2;
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK1, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL1, TRUE);

			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.27)
		case GROUP_PARTY_PMATCHCONDITION_EXP3:
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV6, FALSE);  
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP4, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP5, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK1, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL1, TRUE);

			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.27)
		case GROUP_PARTY_PMATCHCONDITION_EXP4:
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV6, FALSE);  
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP5, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK1, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL1, TRUE);

			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.30)
		case GROUP_PARTY_PMATCHCONDITION_EXP5:							//��ӹڽ� ����Ʈ ���ý� ����Ʈ ������ ������ ���� 
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV6, FALSE);  
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK1, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL1, TRUE);

			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.27)
		case GROUP_PARTY_PMATCHCONDITION_ITEM1:
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV6, FALSE);  
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM4, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM5, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM6, TRUE); 
			m_byBackBoard = 3;
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK1, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL1, TRUE);

			break;  

		//��Ƽ ��ȭ �ý��� (����â 2005.01.27)
		case GROUP_PARTY_PMATCHCONDITION_ITEM2:
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV2, TRUE);  
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV6, FALSE);  
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM4, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM5, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM6, TRUE); 
			m_byBackBoard = 3;
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK1, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL1, TRUE);

			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.27)
		case GROUP_PARTY_PMATCHCONDITION_ITEM3:
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV6, FALSE);  
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM4, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM5, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK1, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL1, TRUE);

			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.27)
		case GROUP_PARTY_PMATCHCONDITION_ITEM4:
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV6, FALSE);  
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM5, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK1, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL1, TRUE);

			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.30)
		case GROUP_PARTY_PMATCHCONDITION_ITEM5:							//��ӹڽ� ����Ʈ ���ý� ����Ʈ ������ ������ ���� 
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV6, FALSE);  
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK1, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL1, TRUE);

			break;


		//��Ƽ ��ȭ �ý��� (����â 2005.01.27)
		case GROUP_PARTY_PMATCHCONDITION_PMAINCHO1:
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV6, FALSE);  
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO4, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO5, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO6, TRUE); 
			m_byBackBoard = 4;
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK1, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL1, TRUE);

			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.27)
		case GROUP_PARTY_PMATCHCONDITION_PMAINCHO2:
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV6, FALSE);  
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO4, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO5, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO6, TRUE); 
			m_byBackBoard = 4;
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK1, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL1, TRUE);
			
			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.27)
		case GROUP_PARTY_PMATCHCONDITION_PMAINCHO3:
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV6, FALSE);  
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO4, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO5, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK1, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL1, TRUE);

			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.27)
		case GROUP_PARTY_PMATCHCONDITION_PMAINCHO4:
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV6, FALSE);  
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO5, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK1, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL1, TRUE);

			break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.30)
		case GROUP_PARTY_PMATCHCONDITION_PMAINCHO5:							//��ӹڽ� ����Ʈ ���ý� ����Ʈ ������ ������ ���� 
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV6, FALSE);  
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO3, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO4, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO5, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO6, FALSE); 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK1, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL1, TRUE);

			break;
			
		case GROUP_PARTY_PMATCHCONDITION_OK_ON:								//��Ī ���� �� Ȯ�� ������ ��� Ȱ��ȭ		
   			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, TRUE);
//			m_byPMatchStatus=1;
//			m_nPMatchResult=0;
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK1, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, FALSE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV1, TRUE);					
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV2, TRUE);						
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV3, FALSE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV4, FALSE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV5, TRUE);					
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV6, FALSE);						
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP1, TRUE);						
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP2, TRUE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP3, FALSE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP4, FALSE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP5, TRUE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP6, FALSE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM1, TRUE);						
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM2, TRUE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM3, FALSE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM4, FALSE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM5, TRUE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM6, FALSE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO1, TRUE);					
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO2, TRUE);					
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO3, FALSE);					 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO4, FALSE);					 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO5, TRUE);					
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO6, FALSE);	
		


			break;

		case GROUP_PARTY_PMATCHCONDITION_OK_OFF:							//��Ī ���� �� Ȯ�� ���� �Ŀ� ��� �� ��� �� Ȱ��ȭ
			if(m_byPMatchStatus == 0)
			{	
				m_bLevelMatch    = 3; 
				m_bExpMatch      = 1;
				m_bItemMatch     = 1;
				m_bMasterMatch	 = 0;
			}   
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
//			m_byPMatchStatus=0;
//			m_nPMatchEscapeResult=0;
		    SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, FALSE);
			SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
		//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, TRUE);			
			SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_STATUS, FALSE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK1, TRUE);
//			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_CANCEL1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV1, TRUE);					
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV2, TRUE);						
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV3, FALSE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV4, FALSE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV5, TRUE);					
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_LEV6, FALSE);						
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP1, TRUE);						
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP2, TRUE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP3, FALSE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP4, FALSE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP5, TRUE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_EXP6, FALSE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM1, TRUE);						
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM2, TRUE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM3, FALSE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM4, FALSE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM5, TRUE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_ITEM6, FALSE);						 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO1, TRUE);					
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO2, TRUE);					
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO3, FALSE);					 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO4, FALSE);					 
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO5, TRUE);					
			SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_PMAINCHO6, FALSE);	 


			break;


		}
	}
	else if(m_byGroupTab==GROUP_TAB_GUILD)
	{	
		SetRender(SPR_OBJ_GUILD_PAGE, TRUE);
		SetRender(SPR_OBJ_GUILD_PAGEPREV1, TRUE);
		SetRender(SPR_OBJ_GUILD_PAGENEXT1, TRUE);		
		
		switch(m_byStatus)
		{
		case GROUP_GUILD_DEFAULT:
			if(g_pMainPlayer->m_dwGuildId==0)
			{
				SetRender(SPR_OBJ_GROUPWND_ADD1, TRUE);
				SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
				SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);
				SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
				SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
				SetRender(SPR_OBJ_GUILD_ONOFF1, TRUE);
				SetRender(SPR_OBJ_GUILD_INFO1, TRUE);
				SetRender(SPR_OBJ_GROUP_MASTER1, TRUE);
			}
			else
			{
				if(g_pMainPlayer->m_byRank==1)
				{
					SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);
					SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
					SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);
					SetRender(SPR_OBJ_GROUPWND_OUT2, TRUE);
					SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
					SetRender(SPR_OBJ_GUILD_ONOFF2, TRUE);
					SetRender(SPR_OBJ_GUILD_INFO1, TRUE);
					SetRender(SPR_OBJ_GROUP_MASTER1, TRUE);
				}				
				else
				{
					SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);
					SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
					SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);
					SetRender(SPR_OBJ_GROUPWND_OUT2, TRUE);
					SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
					SetRender(SPR_OBJ_GUILD_ONOFF1, TRUE);
					SetRender(SPR_OBJ_GUILD_INFO1, TRUE);
					SetRender(SPR_OBJ_GROUP_MASTER1, TRUE);
				}
			}
			break;
		case GROUP_GUILD_ADD:
			if(g_pMainPlayer->m_dwGuildId!=0)
			{				
				SetRender(SPR_OBJ_GROUPWND_ADD3, TRUE);
				SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
				SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);
				SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
				SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
				SetRender(SPR_OBJ_GUILD_ONOFF1, TRUE);
				SetRender(SPR_OBJ_GUILD_INFO1, TRUE);
				SetRender(SPR_OBJ_GROUP_MASTER1, TRUE);
			}
			break;
		case GROUP_GUILD_SELECT:
			if(g_pMainPlayer->m_dwGuildId!=0)
			{
				if(g_pMainPlayer->m_byRank==1)
				{
					SetRender(SPR_OBJ_GROUPWND_ADD1, TRUE);
					SetRender(SPR_OBJ_GROUPWND_DELETE2, TRUE);
					SetRender(SPR_OBJ_GROUPWND_INFO2, TRUE);
					SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
					SetRender(SPR_OBJ_GROUPWND_MSG2, TRUE);
					SetRender(SPR_OBJ_GUILD_ONOFF1, TRUE);
					SetRender(SPR_OBJ_GUILD_INFO2, TRUE);
					SetRender(SPR_OBJ_GROUP_MASTER2, TRUE);
				}
				else
				{
					SetRender(SPR_OBJ_GROUPWND_ADD1, TRUE);
					SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
					SetRender(SPR_OBJ_GROUPWND_INFO2, TRUE);
					SetRender(SPR_OBJ_GROUPWND_OUT1, TRUE);
					SetRender(SPR_OBJ_GROUPWND_MSG2, TRUE);
					SetRender(SPR_OBJ_GUILD_ONOFF1, TRUE);
					SetRender(SPR_OBJ_GUILD_INFO1, TRUE);
					SetRender(SPR_OBJ_GROUP_MASTER1, TRUE);
				}
			}
			break;
		}
	}
	else if(m_byGroupTab==GROUP_TAB_FRIEND)
	{
		SetRender(SPR_OBJ_GROUPWND_STATUS, TRUE);
		SetRender(SPR_OBJ_GUILD_PAGE, TRUE);
		SetRender(SPR_OBJ_GUILD_PAGEPREV1, TRUE);
		SetRender(SPR_OBJ_GUILD_PAGENEXT1, TRUE);

		switch(m_byStatus)
		{
		case GROUP_FRIEND_DEFAULT:
			SetRender(SPR_OBJ_GROUPWND_ADD2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
			SetRender(SPR_OBJ_GUILD_ONOFF2, TRUE);			
			break;
		case GROUP_FRIEND_ADD:
			SetRender(SPR_OBJ_GROUPWND_ADD3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_DELETE1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);
			SetRender(SPR_OBJ_GUILD_ONOFF1, TRUE);			
			break;
		case GROUP_FRIEND_SELECT:
			SetRender(SPR_OBJ_GROUPWND_ADD1, TRUE);
			SetRender(SPR_OBJ_GROUPWND_DELETE2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG2, TRUE);
			SetRender(SPR_OBJ_GUILD_ONOFF1, TRUE);			
			break;
		}		
	}
	else if(m_byGroupTab==GROUP_TAB_GUILDWAR)
	{
		SetRender(SPR_OBJ_GUILD_PAGE, TRUE);
		SetRender(SPR_OBJ_GUILD_PAGEPREV1, TRUE);
		SetRender(SPR_OBJ_GUILD_PAGENEXT1, TRUE);

		ReSetGuildWarRenderStatus();
		SetGuildWarRenderStatus();

		/*
		SetRender(SPR_OBJ_GUILDWAR_GUILD, TRUE);		

		switch(m_byStatus)
		{
		case GROUP_GUILDWAR_DEFAULT:
			SetRender(SPR_OBJ_GROUPWND_GUILDWARADD2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_GUILDWAREXIT1, TRUE);			
			break;
		case GROUP_GUILDWAR_ADD:
			SetRender(SPR_OBJ_GROUPWND_GUILDWARADD3, TRUE);
			SetRender(SPR_OBJ_GROUPWND_GUILDWAREXIT1, TRUE);			
			break;
		case GROUP_GUILDWAR_SELECT:
			if(g_pMainPlayer->m_byRank==1)
			{
				SetRender(SPR_OBJ_GROUPWND_GUILDWARADD1, TRUE);
				SetRender(SPR_OBJ_GROUPWND_GUILDWAREXIT2, TRUE);
			}	
			else
			{
				SetRender(SPR_OBJ_GROUPWND_GUILDWARADD1, TRUE);
				SetRender(SPR_OBJ_GROUPWND_GUILDWAREXIT1, TRUE);
			}
			break;
		}
		*/
	}
}

void CGroupWnd::OpenTab()
{
	ReSetGuildWarRenderStatus();

	SetRender(SPR_OBJ_GROUPWND_ADD1, FALSE);
	SetRender(SPR_OBJ_GROUPWND_ADD2, FALSE);
	SetRender(SPR_OBJ_GROUPWND_ADD3, FALSE);
	SetRender(SPR_OBJ_GROUPWND_DELETE1, FALSE);
	SetRender(SPR_OBJ_GROUPWND_DELETE2, FALSE);
	SetRender(SPR_OBJ_GROUPWND_DELETE3, FALSE);
	SetRender(SPR_OBJ_GROUPWND_INFO1, FALSE);
	SetRender(SPR_OBJ_GROUPWND_INFO2, FALSE);
	SetRender(SPR_OBJ_GROUPWND_INFO3, FALSE);				
	SetRender(SPR_OBJ_GROUPWND_OUT1, FALSE);
	SetRender(SPR_OBJ_GROUPWND_OUT2, FALSE);
	SetRender(SPR_OBJ_GROUPWND_OUT3, FALSE);
	SetRender(SPR_OBJ_GROUPWND_MSG1, FALSE);
	SetRender(SPR_OBJ_GROUPWND_MSG2, FALSE);
	SetRender(SPR_OBJ_GROUPWND_MSG3, FALSE);
	//��Ƽ ��ȭ �ý��� (����â 2005.01.25)
//	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO1, FALSE);  
	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO2, FALSE);
	SetRender(SPR_OBJ_GROUPWND_PARTYUSERINFO3, FALSE);
//	SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION1, FALSE);
	SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, FALSE);
	SetRender(SPR_OBJ_GROUPWND_EXPITEMCONDITION3, FALSE);
//	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION1, FALSE);   
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION2, FALSE);
	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION3, FALSE);
/*	SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK, FALSE);
	SetRender(SPR_OBJ_GROUPWND_PARTYFRIENDADD, FALSE);
	SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST1, FALSE);
	SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST2, FALSE);
	SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST3, FALSE);
	SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST4, FALSE);
	SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST5, FALSE);
	SetRender(SPR_OBJ_GROUPWND_CONDITIONLIST6, FALSE);   */


	SetRender(SPR_OBJ_GUILD_ONOFF1, FALSE);
	SetRender(SPR_OBJ_GUILD_ONOFF2, FALSE);
	SetRender(SPR_OBJ_GUILD_ONOFF3, FALSE);
	SetRender(SPR_OBJ_GUILD_INFO1, FALSE);
	SetRender(SPR_OBJ_GUILD_INFO2, FALSE);
	SetRender(SPR_OBJ_GUILD_INFO3, FALSE);
		
	SetRender(SPR_OBJ_GUILD_LEVEL1, FALSE);
	SetRender(SPR_OBJ_GUILD_LEVEL2, FALSE);
	SetRender(SPR_OBJ_GUILD_LEVEL3, FALSE);
	SetRender(SPR_OBJ_GUILD_LEVEL4, FALSE);
	SetRender(SPR_OBJ_GUILD_MARK, FALSE);	

	SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE);
	m_nPartyUserIndex = -1;		

	switch(m_byGroupTab)
	{
	case GROUP_TAB_PARTY:
		SetRender(SPR_OBJ_GROUPWND_PARTY1, TRUE);
		SetRender(SPR_OBJ_GROUPWND_PARTY2, FALSE);
		SetRender(SPR_OBJ_GROUPWND_GUILD1, FALSE);
		SetRender(SPR_OBJ_GROUPWND_GUILD2, TRUE);
		SetRender(SPR_OBJ_GROUPWND_FRIEND1, FALSE);
		SetRender(SPR_OBJ_GROUPWND_FRIEND2, TRUE);
		SetRender(SPR_OBJ_GROUPWND_GUILDWAR1, FALSE);
		SetRender(SPR_OBJ_GROUPWND_GUILDWAR2, TRUE);				
		SetScalingObj(SPR_OBJ_GROUPWND_SELECT, 104.0f, 18.0f);
		break; 
	case GROUP_TAB_GUILD:
		SetRender(SPR_OBJ_GROUPWND_PARTY1, FALSE);
		SetRender(SPR_OBJ_GROUPWND_PARTY2, TRUE);
		SetRender(SPR_OBJ_GROUPWND_GUILD1, TRUE);
		SetRender(SPR_OBJ_GROUPWND_GUILD2, FALSE);
		SetRender(SPR_OBJ_GROUPWND_FRIEND1, FALSE);
		SetRender(SPR_OBJ_GROUPWND_FRIEND2, TRUE);
		SetRender(SPR_OBJ_GROUPWND_GUILDWAR1, FALSE);
		SetRender(SPR_OBJ_GROUPWND_GUILDWAR2, TRUE);
		SetScalingObj(SPR_OBJ_GROUPWND_SELECT, 104.0f, 18.0f);

		if(g_pMainPlayer->m_dwGuildId!=0)
		{
			if(m_byGuildLevel>=0)
				SetRender(SPR_OBJ_GUILD_LEVEL1, TRUE);
			if(m_byGuildLevel>=1)
				SetRender(SPR_OBJ_GUILD_LEVEL2, TRUE);
			if(m_byGuildLevel>=2)
				SetRender(SPR_OBJ_GUILD_LEVEL3, TRUE);
			if(m_byGuildLevel>=3)
				SetRender(SPR_OBJ_GUILD_LEVEL4, TRUE);

			SetRender(SPR_OBJ_GUILD_MARK, TRUE);
		}						
		break;
	case GROUP_TAB_FRIEND:
		SetRender(SPR_OBJ_GROUPWND_PARTY1, FALSE);
		SetRender(SPR_OBJ_GROUPWND_PARTY2, TRUE);
		SetRender(SPR_OBJ_GROUPWND_GUILD1, FALSE);
		SetRender(SPR_OBJ_GROUPWND_GUILD2, TRUE);
		SetRender(SPR_OBJ_GROUPWND_FRIEND1, TRUE);
		SetRender(SPR_OBJ_GROUPWND_FRIEND2, FALSE);
		SetRender(SPR_OBJ_GROUPWND_GUILDWAR1, FALSE);
		SetRender(SPR_OBJ_GROUPWND_GUILDWAR2, TRUE);
		break;
	case GROUP_TAB_GUILDWAR:
		SetGuildWarRenderStatus();
		if(!g_pMainPlayer->m_dwGuildId)//��� ������ ������ �� �����.
		{
			m_byGuildWarType = 1;
		}
		// sung-han 2005-02-16 ��� ��ġ�� �Ⱦ���.
		/*---------------------------------------------------------------------
		if(m_bGuildWarPacketSendEnable)
		{
			m_bGuildWarPacketSendEnable = FALSE;
			TEAM_MATCH_PACKET TeamMatch;
			TeamMatch.Body.PageNum.iPageNumber = 1;
			Cmd_CTWS_GuildWarPacket(TeamMatch, CTWS_TM_LADDER_LIST);
			TempSetRandomData();//�ӽ�
		}
		---------------------------------------------------------------------*/
		break;
	}
}

int CGroupWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();	

	if(nRt!=20 && g_pMainPlayer->m_dwGuildId!=0 && (m_byGroupTab>=GROUP_TAB_GUILD && m_byGroupTab<=GROUP_TAB_GUILDWAR))
	{
		SetRender(SPR_OBJ_GUILD_PAGEPREV1, TRUE);
		SetRender(SPR_OBJ_GUILD_PAGEPREV2, FALSE);
		SetRender(SPR_OBJ_GUILD_PAGEPREV3, FALSE);
	}
	if(nRt!=21 && g_pMainPlayer->m_dwGuildId!=0 && (m_byGroupTab>=GROUP_TAB_GUILD && m_byGroupTab<=GROUP_TAB_GUILDWAR))
	{
		SetRender(SPR_OBJ_GUILD_PAGENEXT1, TRUE);
		SetRender(SPR_OBJ_GUILD_PAGENEXT2, FALSE);
		SetRender(SPR_OBJ_GUILD_PAGENEXT3, FALSE);
	}

	if(nRt!=24 && g_pMainPlayer->m_dwGuildId!=0 && m_byGroupTab==GROUP_TAB_GUILD && m_bGuildBtn[6]==TRUE)
	{
		m_bGuildBtn[6] = FALSE;
		SetRender(SPR_OBJ_GROUP_MASTER1, FALSE);
		SetRender(SPR_OBJ_GROUP_MASTER2, TRUE);
		SetRender(SPR_OBJ_GROUP_MASTER3, FALSE);
	}
	
	if(nRt!=22 && m_bGuildBtn[5] && m_byGroupTab==GROUP_TAB_GUILD)
	{
		SetRender(SPR_OBJ_GUILD_INFO1, FALSE);
		SetRender(SPR_OBJ_GUILD_INFO2, TRUE);
		SetRender(SPR_OBJ_GUILD_INFO3, FALSE);
	}

#ifdef PARTY_BOARD

	if(nRt!=22 && m_bPartyBtn[4] && m_byGroupTab==GROUP_TAB_PARTY)
	{
		SetRender(SPR_OBJ_GROUPWND_PBCREATE3, FALSE);

		if(g_pMainPlayer->m_dwPartyId==0)
		{
			SetRender(SPR_OBJ_GROUPWND_PBCREATE1, TRUE);	
			SetRender(SPR_OBJ_GROUPWND_PBCREATE2, FALSE);			
		}
		else
		{			
			if(!g_pPartyBoardHash->GetData(g_pMainPlayer->m_dwPartyId))
			{
				SetRender(SPR_OBJ_GROUPWND_PBCREATE1, FALSE);	
				SetRender(SPR_OBJ_GROUPWND_PBCREATE2, TRUE);	
			}
			else
			{
				SetRender(SPR_OBJ_GROUPWND_PBCREATE1, TRUE);	
				SetRender(SPR_OBJ_GROUPWND_PBCREATE2, FALSE);	
			}
		}
		m_bPartyBtn[4] = FALSE;
	}

	if(nRt!=23 && m_bPartyBtn[5] && m_byGroupTab==GROUP_TAB_PARTY)
	{
		SetRender(SPR_OBJ_GROUPWND_PBDESTROY3, FALSE);

		if(g_pMainPlayer->m_dwPartyId==0)
		{
			SetRender(SPR_OBJ_GROUPWND_PBDESTROY1, TRUE);	
			SetRender(SPR_OBJ_GROUPWND_PBDESTROY2, FALSE);			
		}
		else
		{			
			if(g_pPartyBoardHash->GetData(g_pMainPlayer->m_dwPartyId))
			{
				SetRender(SPR_OBJ_GROUPWND_PBDESTROY1, FALSE);	
				SetRender(SPR_OBJ_GROUPWND_PBDESTROY2, TRUE);	
			}
			else
			{
				SetRender(SPR_OBJ_GROUPWND_PBDESTROY1, TRUE);	
				SetRender(SPR_OBJ_GROUPWND_PBDESTROY2, FALSE);	
			}
		}
		m_bPartyBtn[5] = FALSE;
	}
	
#endif

	if(nRt!=9)
	{
		if(m_bPartyBtn[2] && m_byGroupTab==GROUP_TAB_PARTY)
		{
			m_bPartyBtn[2]	= FALSE;
			SetRender(SPR_OBJ_GROUPWND_INFO1, FALSE);
			SetRender(SPR_OBJ_GROUPWND_INFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO3, FALSE);
		}
		else if(m_bGuildBtn[3] && m_byGroupTab==GROUP_TAB_GUILD)
		{
			m_bGuildBtn[3]	= FALSE;
			SetRender(SPR_OBJ_GROUPWND_INFO1, FALSE);
			SetRender(SPR_OBJ_GROUPWND_INFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO3, FALSE);
		}		
		else if(m_bFriednBtn[2] && m_byGroupTab==GROUP_TAB_FRIEND)
		{
			m_bFriednBtn[2]	= FALSE;
			SetRender(SPR_OBJ_GROUPWND_INFO1, FALSE);
			SetRender(SPR_OBJ_GROUPWND_INFO2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_INFO3, FALSE);
		}				
	}
	
	if(nRt!=8 && m_bGuildBtn[4] && m_byGroupTab==GROUP_TAB_GUILD)
	{
		m_bGuildBtn[4]	= FALSE;
		SetRender(SPR_OBJ_GROUPWND_DELETE1, FALSE);
		SetRender(SPR_OBJ_GROUPWND_DELETE2, TRUE);
		SetRender(SPR_OBJ_GROUPWND_DELETE3, FALSE);
	}
	
	if(nRt!=8 && m_bPartyBtn[3] && m_byGroupTab==GROUP_TAB_PARTY)
	{
		m_bPartyBtn[3]	= FALSE;
		SetRender(SPR_OBJ_GROUPWND_DELETE1, FALSE);
		SetRender(SPR_OBJ_GROUPWND_DELETE2, TRUE);
		SetRender(SPR_OBJ_GROUPWND_DELETE3, FALSE);
	}		
/*
	if(nRt!=8 && m_bGuildWarBtn && m_byGroupTab==GROUP_TAB_GUILDWAR)
	{
		m_bGuildWarBtn	= FALSE;
		SetRender(SPR_OBJ_GROUPWND_GUILDWAREXIT1, FALSE);
		SetRender(SPR_OBJ_GROUPWND_GUILDWAREXIT2, TRUE);
		SetRender(SPR_OBJ_GROUPWND_GUILDWAREXIT3, FALSE);
	}		
*/
	if(nRt!=10)
	{
		if(m_bPartyBtn[1] && m_byGroupTab==GROUP_TAB_PARTY)
		{
			m_bPartyBtn[1]	= FALSE;
			SetRender(SPR_OBJ_GROUPWND_OUT1, FALSE);
			SetRender(SPR_OBJ_GROUPWND_OUT2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_OUT3, FALSE);
		}
		else if(m_bGuildBtn[1] && m_byGroupTab==GROUP_TAB_GUILD)
		{
			m_bGuildBtn[1]	= FALSE;
			SetRender(SPR_OBJ_GROUPWND_OUT1, FALSE);
			SetRender(SPR_OBJ_GROUPWND_OUT2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_OUT3, FALSE);
		}				
	}		

	if(nRt!=11)
	{
		if(m_bPartyBtn[0] || m_bGuildBtn[2])
		{
			m_bGuildBtn[2] = FALSE;
			m_bPartyBtn[0] = FALSE;
			SetRender(SPR_OBJ_GROUPWND_MSG1, FALSE);
			SetRender(SPR_OBJ_GROUPWND_MSG2, TRUE);
			SetRender(SPR_OBJ_GROUPWND_MSG3, FALSE);
		}
	}	
	
	if(nRt!=10 && m_bFriednBtn[1])
	{		
		m_bFriednBtn[1] = FALSE;
		SetRender(SPR_OBJ_GROUPWND_MSG1, FALSE);
		SetRender(SPR_OBJ_GROUPWND_MSG2, TRUE);
		SetRender(SPR_OBJ_GROUPWND_MSG3, FALSE);
	}	

	if(m_bGuildBtn[0] && nRt!=23 && m_byGroupTab==GROUP_TAB_GUILD)
	{
		m_bGuildBtn[0] = FALSE;
		SetRender(SPR_OBJ_GUILD_ONOFF1, FALSE);
		SetRender(SPR_OBJ_GUILD_ONOFF2, TRUE);
		SetRender(SPR_OBJ_GUILD_ONOFF3, FALSE);		
	}

	if(m_bFriednBtn[0] && nRt!=21 && m_byGroupTab==GROUP_TAB_FRIEND)
	{
		m_bFriednBtn[0] = FALSE;
		SetRender(SPR_OBJ_GUILD_ONOFF1, FALSE);
		SetRender(SPR_OBJ_GUILD_ONOFF2, TRUE);
		SetRender(SPR_OBJ_GUILD_ONOFF3, FALSE);		
	}
	

	m_byColor = 0;

	//��Ƽ ��ȭ �ý��� (����â 2005.02.01)
	if( m_byGroupTab==GROUP_TAB_PARTY && (nRt != 0 || nRt != 1)) 
	{
		int iTempRt = GetChk( 0, FALSE, 1);
		if( iTempRt != 0 && iTempRt != 1 )
		{
			nRt = iTempRt;
		}
	
		if(m_byBackBoard == 2 || m_byBackBoard == 4)
		{
			iTempRt = GetChk( 0, FALSE, 2);
			if( iTempRt != 0 && iTempRt != 1 )
			{
				nRt = iTempRt;
			}
		}
		else if(m_byBackBoard == 1 || m_byBackBoard == 3 || m_byStatus == GROUP_PARTY_JOIN || m_byStatus == GROUP_PARTY_SELECT)
		{
			iTempRt = GetChk( 0, FALSE, 3);
			if( iTempRt != 0 && iTempRt != 1 )
			{
				nRt = iTempRt;
			}
		} 
		
	}
	


	// sung-han 2005-01-29 ���̹� ����� ����
	ReSetGuildWarRenderStatus();
	if(m_byGroupTab == GROUP_TAB_GUILDWAR)
	{
		if( nRt != 0 && nRt != 1 )
		{
			int nTempRt = 0;
			if(m_byStatus == GROUP_GUILDWAR_MATCH && m_byGuildWarDropDownEnableNum == 2)
			{
				nTempRt = GetChk(0, FALSE, 5);
				if(nTempRt != 0 && nTempRt != 1)
				{
					nRt = nTempRt;
				}
			}
			
			if(m_byStatus == GROUP_GUILDWAR_MATCH && nTempRt < 2 )
			{
				nTempRt = GetChk(0, FALSE, 4);
				if(nTempRt != 0 && nTempRt != 1)
				{
					nRt = nTempRt;
				}
			}
			else if(m_byStatus == GROUP_GUILDWAR_RANK || m_byStatus == GROUP_GUILDWAR_LIST)
			{
				nTempRt = GetChk(0, FALSE, 6);
				if(nTempRt != 0 && nTempRt != 1)
				{
					nRt = nTempRt;
				}
			}
		}

		SetGuildWarRenderStatus();
	}


	InitValue(nRt);

	MOUSECHECK eMouseCheck = GetMouseCheck(nRt);
	
	switch(nRt)
	{
	case 0:	
		{			
			if(eMouseCheck==MOUSE_DOWN)
			{
				if(g_pMainPlayer->m_dwPartyId==0)
				{
					if(m_byStatus==GROUP_PARTY_CREATEADD)
						SetStatus(GROUP_PARTY_CREATEADD);
					else if(m_byStatus==GROUP_PARTY_CREATEADDCLK)
						SetStatus(GROUP_PARTY_CREATEADDCLK);
					
					m_byRenderType = GROUP_RENDER_NONE;
				}
				else
					m_byRenderType = GROUP_RENDER_NONE;
			}
		}
		break;
	case 1:
		{
			if(g_pMainPlayer->m_dwPartyId==0)
			{
				if(m_byStatus==GROUP_PARTY_CREATEADD)
					SetStatus(GROUP_PARTY_CREATEADD);
				else if(m_byStatus==GROUP_PARTY_CREATEADDCLK)
					SetStatus(GROUP_PARTY_CREATEADDCLK);

				m_byRenderType = GROUP_RENDER_NONE;
			}				
			else
				m_byRenderType = GROUP_RENDER_NONE;

			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 2:
		{			
			if(eMouseCheck==MOUSE_UP)
				SetActive(FALSE);
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_GROUPWND_CLOSE1, FALSE);
				SetRender(SPR_OBJ_GROUPWND_CLOSE2, TRUE);
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 3:	
		{
			SetMouseCheck(nRt, eMouseCheck);
		}		
		break;
	case 4:
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_byGroupTab	= GROUP_TAB_PARTY;				
				m_byStatus		= GROUP_PARTY_DEFAULT;
				
				OpenTab();
				SetActive();
				SetPosGroup();
			}
			SetMouseCheck(nRt, eMouseCheck);
		}		
		break;
	case 5:
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_byGroupTab	= GROUP_TAB_GUILD;			
				m_byStatus		= GROUP_GUILD_DEFAULT;
				
				OpenTab();
				SetActive();
				SetPosGroup();
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 6:
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_byStatus		= GROUP_FRIEND_DEFAULT;
				m_byGroupTab	= GROUP_TAB_FRIEND;
			
				OpenTab();
				SetActive();
				SetPosGroup();
			}
			SetMouseCheck(nRt, eMouseCheck);
		}		
		break;
	case 25:
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				if(!m_bGuildWarPacketSendEnable)
					break;
				m_byGroupTab	= GROUP_TAB_GUILDWAR;
				m_byStatus		= GROUP_GUILDWAR_DEFAULT;
			
				OpenTab();
				SetActive();
				SetPosGroup();
			}
			SetMouseCheck(nRt, eMouseCheck);
		}		
		break;
	case 7://�޴� ù��°
		{
			BYTE byChk = 0;

			if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_CREATEADD)
					{
						if(!g_pMainPlayer->m_bMatching)
						{
							if(g_pMainPlayer->m_dwPartyId==0)
								g_pMainPlayer->m_bPartyLeader = 1;

							CUserInterface::GetInstance()->SetPointerChk( TRUE );
							CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_GUARDIAN__);
							m_byRenderType = GROUP_RENDER_PARTYUSERADDCLK;
							SetStatus(GROUP_PARTY_CREATEADDCLK);
						}
						else
						{
							// "��� �߿��� ����� �� �����ϴ�."
							DisplayMessageAdd(g_Message[ETC_MESSAGE501].szMessage, 0xffffff00); 
						}
					}
					else if(m_byStatus==GROUP_PARTY_JOIN)
					{
						if(!g_pMainPlayer->m_bMatching)
						{
							CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_GUARDIAN__);
							CUserInterface::GetInstance()->SetPointerChk( TRUE );							
							SetStatus(GROUP_PARTY_CREATEADDCLK);

							if(g_pMainPlayer->m_bPartyLeader==1)
								m_bPartyMsg = FALSE;
							else
								m_bPartyMsg = TRUE;	
						}
						else
						{
							// "��� �߿��� ����� �� �����ϴ�."
							DisplayMessageAdd(g_Message[ETC_MESSAGE501].szMessage, 0xff00ffff); 
						}
					}
					else
						byChk = __MOUSE_POINTER_DEFAULTCLICK__;						
				}
				else if(m_byGroupTab==GROUP_TAB_GUILD)
				{
					if(!g_pMainPlayer->m_bMatching)
					{
						if(m_byStatus==GROUP_GUILD_DEFAULT)
						{
							BOOL bChk = FALSE;

							if(g_pMainPlayer->m_byRank==1)
							{
								m_bGuildMsg = TRUE;
								bChk		= TRUE;
							}
							else if(g_pMainPlayer->m_byRank != 0) 
							{							
								m_bGuildMsg = FALSE;

								ListNode<GUILD_USER>* pGuildUserPos = g_pGuildUserHash->GetHead();															

								while(pGuildUserPos)
								{
									LP_GUILD_USER sGuildUser = pGuildUserPos->pData;
									
									if(!sGuildUser)
										continue;

									if(sGuildUser->byRank==1)
										bChk = TRUE;
							
									pGuildUserPos = pGuildUserPos->pNext;
								}

								if(!bChk)
								{
									// "���ӻ� �����Ͱ� �����ϴ�."
									DisplayMessageAdd(g_Message[ETC_MESSAGE164].szMessage, 0xff00ffff); 
								}
							}

							if(bChk)
							{
								CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_GUARDIAN__);
								CUserInterface::GetInstance()->SetPointerChk( TRUE );					
								SetStatus(GROUP_GUILD_ADD);
								m_byRenderType = GROUP_RENDER_GUILDUSERADDCLK;
							}
							else
								byChk = __MOUSE_POINTER_DEFAULTCLICK__;
						}					
						else
							byChk = __MOUSE_POINTER_DEFAULTCLICK__;
					}
					else
					{
						// "��� �߿��� ����� �� �����ϴ�."
						DisplayMessageAdd(g_Message[ETC_MESSAGE501].szMessage, 0xff00ffff); 
					}
				}
				else if(m_byGroupTab==GROUP_TAB_FRIEND)
				{
					if(m_byStatus==GROUP_FRIEND_DEFAULT)
					{
						if(g_pMessengerOnLineHash->GetCount()+g_pMessengerOffLineHash->GetCount()<MAX_MESSENGER_USER)
						{
							CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_GUARDIAN__);
							CUserInterface::GetInstance()->SetPointerChk( TRUE );					
							SetStatus(GROUP_FRIEND_ADD);
							m_byRenderType = GROUP_RENDER_FRIENDUSERADDCLK;
						}
						else
						{
							// "�� �̻� ģ���� ����� �� �����ϴ�."
							DisplayMessageAdd(g_Message[ETC_MESSAGE163].szMessage, 0xff92D5FF, DEF_CHAT_TYPE_FRIEND);
							byChk = __MOUSE_POINTER_DEFAULTCLICK__;
						}
					}
				}
				else if(m_byGroupTab==GROUP_TAB_GUILDWAR)//���콺 �ٿ��϶�
				{
					// sung-han 2005-02-16 ��� ��ġ�� �Ⱦ���.
					if(m_bGuildWarPacketSendEnable)
					{					
						//������ ��ũ ����Ʈ�� �����޶�� ��û�ؾ� �Ѵ�.
						m_bGuildWarPacketSendEnable = FALSE;
						TEAM_MATCH_PACKET TeamMatch;
						TeamMatch.Body.PageNum.iPageNumber = m_nGuildWarRankListPage = 1;
						Cmd_CTWS_GuildWarPacket(TeamMatch, CTWS_TM_LADDER_LIST);
//						TempSetRandomData();//�ӽ�
						SetStatus(GROUP_GUILDWAR_RANK);
					}

					/*
					if(m_byStatus==GROUP_GUILDWAR_DEFAULT)
					{
						if(g_pGuildWarHash->GetCount()<MAX_GUILDWAR)
						{
							CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_GUARDIAN__);
							CUserInterface::GetInstance()->SetPointerChk( TRUE );					
							SetStatus(GROUP_GUILDWAR_ADD);
							m_byRenderType = GROUP_RENDER_GUILDWARUSERADDCLK;
						}
						else
						{
							// "�ϳ��� ���� �ִ� 5���� �ٸ� ���� ���ÿ� ������� �����մϴ�."
							DisplayMessageAdd(g_Message[ETC_MESSAGE876].szMessage, 0xffffa2ff, DEF_CHAT_TYPE_GUILD);
							byChk = __MOUSE_POINTER_DEFAULTCLICK__;
						}
					}
					*/
				}			
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_CREATEADD)				
					{
						CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_BUTTON__);
						m_byRenderType = GROUP_RENDER_PARTYUSERADD;
					}
					else if(m_byStatus==GROUP_PARTY_CREATEADDCLK)
					{
						m_byRenderType = GROUP_RENDER_PARTYUSERADDCLK;				
					}
					else if(m_byStatus==GROUP_PARTY_JOIN)
					{
						if(g_pMainPlayer->m_bPartyLeader==1)
						{
							CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_BUTTON__);
							m_byRenderType = GROUP_RENDER_PARTYUSERADD;				
						}					
					}
					else
						CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_DEFAULT__);
				}
				else if(m_byGroupTab==GROUP_TAB_GUILD)
				{
					if(m_byStatus==GROUP_GUILD_DEFAULT)				
					{
						if(g_pMainPlayer->m_dwGuildId!=0)
							m_byRenderType = GROUP_RENDER_GUILDUSERADD;								
						else
							byChk = __MOUSE_POINTER_DEFAULT__;
					}
					else if(m_byStatus==GROUP_GUILD_ADD)
						m_byRenderType = GROUP_RENDER_GUILDUSERADDCLK;
				}
				else if(m_byGroupTab==GROUP_TAB_FRIEND)
				{
					if(m_byStatus==GROUP_FRIEND_DEFAULT)					
						m_byRenderType = GROUP_RENDER_FRIENDUSERADD;	
					else if(m_byStatus==GROUP_FRIEND_ADD)
						m_byRenderType = GROUP_RENDER_FRIENDUSERADDCLK;	
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}
				else if(m_byGroupTab==GROUP_TAB_GUILDWAR)
				{
					m_byRenderType = GROUP_RENDER_TEXT__GUILDWAR_RANK;
					/*
					if(m_byStatus==GROUP_GUILDWAR_DEFAULT)					
						m_byRenderType = GROUP_RENDER_GUILDWARUSERADD;	
					else if(m_byStatus==GROUP_GUILDWAR_ADD)				
						m_byRenderType = GROUP_RENDER_GUILDWARUSERADDCLK;	
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
					*/
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}		
		break;
	case 8://�޴� �ι�°
		{
			BYTE byChk = 0;

			if(eMouseCheck==MOUSE_UP)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_SELECT)
					{
						if(g_pMainPlayer->m_bPartyLeader==1)
						{							
							if(m_nPartyUserIndex==0)
								DisplayMessageAdd(g_Message[ETC_MESSAGE576].szMessage, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY); // "�ڱ� �ڽ��� ������ �� �����ϴ�."
							else
							{
								// Ż�� //													
								if(g_pMainPlayer->m_dwPartyId!=0)
								{
									ListNode<PARTY_USER>* pNode = g_pPartyUserHash->GetHead();

									BYTE byIndex = 0;

									while(pNode)
									{
										LP_PARTY_USER sPartyNode = pNode->pData;
					
										if(byIndex==m_nPartyUserIndex-1)
										{
											CTWS_PARTY_LOGOUTRT pPacket;
											pPacket.dwPartyId	= g_pMainPlayer->m_dwPartyId;
											pPacket.dwUserIndex	= sPartyNode->dwUserIndex;
											g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);							
											m_bClick			= TRUE;
											m_bDown				= TRUE;
											m_bUpChk			= TRUE;
											m_nPartyUserIndex	= -1;
											SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE);
											SetMouseCheck(nRt, eMouseCheck, byChk);
											return CInterface::GetInstance()->GetSetWnd();
										}
										byIndex++;
										pNode = pNode->pNext;
									}																
								}
								else
									DisplayMessageAdd(g_Message[ETC_MESSAGE69].szMessage, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY); // "�������� ��Ƽ�� �����ϴ�."
							}
							
							m_bPartyBtn[3]	= FALSE;
							SetRender(SPR_OBJ_GROUPWND_DELETE1, FALSE);
							SetRender(SPR_OBJ_GROUPWND_DELETE2, TRUE);
							SetRender(SPR_OBJ_GROUPWND_DELETE3, FALSE);
						}
						else
							byChk = __MOUSE_POINTER_DEFAULT__;
					}				
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}
				else if(m_byGroupTab==GROUP_TAB_GUILD)
				{
					if(m_byStatus==GROUP_GUILD_SELECT)
					{
						if(g_pMainPlayer->m_byRank==1)
						{
							if(m_byUserType==GUILD_USER_OFFLINE)
							{
								BYTE							byIndex					= 0;
								ListNode<GUILD_OFFLINE_USER>*	pGuildOffLineUserPos	= m_pGuildOfflineUserPos;							

								while(pGuildOffLineUserPos)
								{
									LP_GUILD_OFFLINE_USER sGuildOfflineUser = pGuildOffLineUserPos->pData;
									
									if(!sGuildOfflineUser)
										continue;

									if(m_nPartyUserIndex==byIndex)
									{
										CGuildDeleteWnd* pGuildDeleteWnd = CGuildDeleteWnd::GetInstance();
										memset(pGuildDeleteWnd->m_szName, 0, sizeof(pGuildDeleteWnd->m_szName));
										pGuildDeleteWnd->SetActive();
										pGuildDeleteWnd->m_byType		= 3;
										pGuildDeleteWnd->m_dwUserIndex	= sGuildOfflineUser->dwUserIndex;
										__lstrcpyn(pGuildDeleteWnd->m_szName, sGuildOfflineUser->szName, MAX_CHARACTER_NAME_REAL_LENGTH);									
										CInterface::GetInstance()->SetWnd(GUILDDELETE_WND);									

										m_bGuildBtn[4]	= FALSE;
										SetRender(SPR_OBJ_GROUPWND_DELETE1, FALSE);
										SetRender(SPR_OBJ_GROUPWND_DELETE2, TRUE);
										SetRender(SPR_OBJ_GROUPWND_DELETE3, FALSE);										
										break;
									}										
									pGuildOffLineUserPos = pGuildOffLineUserPos->pNext;
									byIndex++;
								}
							}
							else
							{
								if(m_nPartyUserIndex==0 && m_byCurPage==1)
								{
									//"�ڱ� �ڽ��� �����Ҽ� �����ϴ�."						
									DisplayMessageAdd(g_Message[ETC_MESSAGE180].szMessage
										, 0xff00ffff, TRUE, DEF_CHAT_TYPE_PARTY);	
								}
								else
								{
									BYTE					byIndex			= 0;
									ListNode<GUILD_USER>*	pGuildUserPos	= m_pGuildUserPos;

									if(m_byCurPage==1)
									{
										byIndex = 1;					
										pGuildUserPos = g_pGuildUserHash->GetHead();
									}

									while(pGuildUserPos)
									{
										LP_GUILD_USER sGuildUser = pGuildUserPos->pData;
										
										if(!sGuildUser)
											continue;

										if(m_nPartyUserIndex==byIndex)
										{
											if(sGuildUser->dwUserIndex!=g_pMainPlayer->m_dwUserIndex)
											{
												CGuildDeleteWnd* pGuildDeleteWnd = CGuildDeleteWnd::GetInstance();
												memset(pGuildDeleteWnd->m_szName, 0, sizeof(pGuildDeleteWnd->m_szName));											
												pGuildDeleteWnd->SetActive();
												pGuildDeleteWnd->m_byType		= 3;
												pGuildDeleteWnd->m_dwUserIndex	= sGuildUser->dwUserIndex;
												__lstrcpyn(pGuildDeleteWnd->m_szName, sGuildUser->szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);									
												CInterface::GetInstance()->SetWnd(GUILDDELETE_WND);
											}
											else
												DisplayMessageAdd(g_Message[ETC_MESSAGE180].szMessage, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD); // "�ڱ� �ڽ��� �����Ҽ� �����ϴ�."

											break;
										}
										
										pGuildUserPos = pGuildUserPos->pNext;
										byIndex++;
									}
								}
							}
							
							m_bGuildBtn[4]	= FALSE;
							SetRender(SPR_OBJ_GROUPWND_DELETE1, FALSE);
							SetRender(SPR_OBJ_GROUPWND_DELETE2, TRUE);
							SetRender(SPR_OBJ_GROUPWND_DELETE3, FALSE);
						}
						else
							byChk = __MOUSE_POINTER_DEFAULT__;
					}
				}
				else if(m_byGroupTab==GROUP_TAB_FRIEND)
				{				
					if(m_byStatus==GROUP_FRIEND_SELECT)
					{					
						m_bFriednBtn[3]	= FALSE;
						SetRender(SPR_OBJ_GROUPWND_DELETE1, FALSE);
						SetRender(SPR_OBJ_GROUPWND_DELETE2, TRUE);
						SetRender(SPR_OBJ_GROUPWND_DELETE3, FALSE);

						if(m_byMessengerType==GUILD_USER_OFFLINE)
						{
							BYTE						byIndex					= 0;
							ListNode<MESSENGER_USER>*	pMessengerOffLinePos	= m_pMessengerOfflinePos;							

							while(pMessengerOffLinePos)
							{
								LP_MESSENGER_USER sMessengerUser = pMessengerOffLinePos->pData;
								
								if(!sMessengerUser)
									continue;

								if(m_nPartyUserIndex==byIndex)
								{
									CMessengerDeleteWnd* pMessengerDeleteWnd = CMessengerDeleteWnd::GetInstance();
									memset(pMessengerDeleteWnd->m_szName, 0, sizeof(pMessengerDeleteWnd->m_szName));
									pMessengerDeleteWnd->SetActive();
									pMessengerDeleteWnd->m_byType		= 3;
									pMessengerDeleteWnd->m_dwUserIndex	= sMessengerUser->dwUserIndex;
									__lstrcpyn(pMessengerDeleteWnd->m_szName, sMessengerUser->szName, MAX_CHARACTER_NAME_REAL_LENGTH);									
									CInterface::GetInstance()->SetWnd(MESSENGERDELETE_WND);
									break;
								}	
								
								pMessengerOffLinePos = pMessengerOffLinePos->pNext;
								byIndex++;
							}
						}
						else
						{							
							BYTE						byIndex				= 0;
							ListNode<MESSENGER_USER>*	pMessengerOnLinePos	= m_pMessengerOnlinePos;
							
							while(pMessengerOnLinePos)
							{
								LP_MESSENGER_USER sMessengerUser = pMessengerOnLinePos->pData;
								
								if(!sMessengerUser)
									continue;

								if(m_nPartyUserIndex==byIndex)
								{									
									CMessengerDeleteWnd* pMessengerDeleteWnd = CMessengerDeleteWnd::GetInstance();
									memset(pMessengerDeleteWnd->m_szName, 0, sizeof(pMessengerDeleteWnd->m_szName));
									pMessengerDeleteWnd->SetActive();
									pMessengerDeleteWnd->m_byType		= 3;
									pMessengerDeleteWnd->m_dwUserIndex	= sMessengerUser->dwUserIndex;
									__lstrcpyn(pMessengerDeleteWnd->m_szName, sMessengerUser->szName, MAX_CHARACTER_NAME_REAL_LENGTH);									
									CInterface::GetInstance()->SetWnd(MESSENGERDELETE_WND);						
									break;
								}
								pMessengerOnLinePos = pMessengerOnLinePos->pNext;
								byIndex++;
							}							
						}														
					}					
				}
				else if(m_byGroupTab==GROUP_TAB_GUILDWAR)//���콺 ���϶�
				{
					if(m_byStatus==GROUP_GUILDWAR_SELECT)
					{
						//���� ������
					}
					/*
					if(m_byStatus==GROUP_GUILDWAR_SELECT)
					{
						if(g_pMainPlayer->m_byRank==1)
						{
							m_bGuildWarBtn	= FALSE;
							SetRender(SPR_OBJ_GROUPWND_GUILDWAREXIT1, FALSE);
							SetRender(SPR_OBJ_GROUPWND_GUILDWAREXIT2, TRUE);
							SetRender(SPR_OBJ_GROUPWND_GUILDWAREXIT3, FALSE);

							ListNode<SGUILD_WAR>* lpGuildWarPos = g_pGuildWarHash->GetHead();
			
							struct tm	*when;
							time_t		now;
							BYTE		byIndex = 0;
							
							time(&now);
							when = localtime(&now);
							

							if(when)//�� ������ ���� ���� : 050103 hwoarang    
							{
								while(lpGuildWarPos)
								{
									LP_GUILD_WAR lpGuildWar = lpGuildWarPos->pData;

									if(lpGuildWar)
									{			
										if(m_nPartyUserIndex==byIndex)
										{
											int nTime	= (int)difftime(now, lpGuildWar->sTime);
											int nHou	= (nTime/60)/60;
											
											if(nHou>=GUILDWAR_TIME)
											{
												// ������ ���� ��Ŷ ������.
												CTWS_GUILDWAR_EXIT pPacket;
												pPacket.dwGuildId		= g_pMainPlayer->m_dwGuildId;
												pPacket.dwGuildIdDst	= lpGuildWar->dwGuildId;
												g_pNet->SendMsg( (char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
											}
											else
											{
												char szInfo[0xff]={0,};
												//"����� �¶��� 3��(72�ð�)���Ŀ� ���ᰡ �����մϴ�."
	//											wsprintf(szInfo, g_Message[ETC_MESSAGE905].szMessage);
												lstrcpy(szInfo, g_Message[ETC_MESSAGE905].szMessage);
												DisplayMessageAdd(szInfo, 0xffffa2ff, TRUE, DEF_CHAT_TYPE_GUILD);										
											}
											break;
										}																
									}
									lpGuildWarPos = lpGuildWarPos->pNext;
									byIndex++;
								}
							}
						}
						else
							byChk = __MOUSE_POINTER_DEFAULT__;
					}
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
					*/
				}
				else
					byChk = __MOUSE_POINTER_DEFAULT__;
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{				
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_SELECT)
					{
						if(g_pMainPlayer->m_bPartyLeader==1)
						{
							m_bPartyBtn[3]	= TRUE;
							SetRender(SPR_OBJ_GROUPWND_DELETE1, FALSE);
							SetRender(SPR_OBJ_GROUPWND_DELETE2, FALSE);
							SetRender(SPR_OBJ_GROUPWND_DELETE3, TRUE);
						}
						else
							byChk = __MOUSE_POINTER_DEFAULTCLICK__;
					}				
					else
						byChk = __MOUSE_POINTER_DEFAULTCLICK__;
				}
				else if(m_byGroupTab==GROUP_TAB_GUILD)
				{
					if(m_byStatus==GROUP_GUILD_SELECT)
					{
						if(g_pMainPlayer->m_byRank==1)
						{							
							m_bGuildBtn[4]	= TRUE;
							SetRender(SPR_OBJ_GROUPWND_DELETE1, FALSE);
							SetRender(SPR_OBJ_GROUPWND_DELETE2, FALSE);
							SetRender(SPR_OBJ_GROUPWND_DELETE3, TRUE);
						}
						else
							byChk = __MOUSE_POINTER_DEFAULTCLICK__;
					}
					else
						byChk = __MOUSE_POINTER_DEFAULTCLICK__;
				}
				else if(m_byGroupTab==GROUP_TAB_FRIEND)
				{
					if(m_byStatus==GROUP_FRIEND_SELECT)
					{
						m_bFriednBtn[3]	= TRUE;
						SetRender(SPR_OBJ_GROUPWND_DELETE1, FALSE);
						SetRender(SPR_OBJ_GROUPWND_DELETE2, FALSE);
						SetRender(SPR_OBJ_GROUPWND_DELETE3, TRUE);					
					}				
					else
						byChk = __MOUSE_POINTER_DEFAULTCLICK__;
				}
				else if(m_byGroupTab==GROUP_TAB_GUILDWAR)//���콺 �ٿ��϶�
				{
					if(m_bGuildWarPacketSendEnable)
					{
						//������ ����� ����Ʈ�� �����޶�� ��û�ؾ� �Ѵ�.
						m_bGuildWarPacketSendEnable = FALSE;
						TEAM_MATCH_PACKET TeamMatch;
						TeamMatch.Body.PageNum.iPageNumber = 1;
						Cmd_CTWS_GuildWarPacket(TeamMatch, CTWS_TM_GUILD_WAR_LIST);
//						TempSetRandomData();//�ӽ�
						SetStatus(GROUP_GUILDWAR_LIST);
					}

					/*
					if(m_byStatus==GROUP_GUILDWAR_SELECT)
					{
						if(g_pMainPlayer->m_byRank==1)
						{
							m_bGuildWarBtn	= TRUE;
							SetRender(SPR_OBJ_GROUPWND_GUILDWAREXIT1, FALSE);
							SetRender(SPR_OBJ_GROUPWND_GUILDWAREXIT2, FALSE);
							SetRender(SPR_OBJ_GROUPWND_GUILDWAREXIT3, TRUE);
						}
						else
							byChk = __MOUSE_POINTER_DEFAULTCLICK__;
					}
					else
						byChk = __MOUSE_POINTER_DEFAULTCLICK__;
					*/
				}						
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_SELECT)
					{
						if(g_pMainPlayer->m_bPartyLeader==1)						
							m_byRenderType = GROUP_RENDER_PARTYUSERDEL;
						else
							byChk = __MOUSE_POINTER_DEFAULT__;
					}				
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}
				else if(m_byGroupTab==GROUP_TAB_GUILD)
				{
					if(m_byStatus==GROUP_GUILD_SELECT)
					{
						if(g_pMainPlayer->m_byRank==1)						
							m_byRenderType = GROUP_RENDER_GUILDUSERDELETE;
						else
							byChk = __MOUSE_POINTER_DEFAULT__;
					}
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}
				else if(m_byGroupTab==GROUP_TAB_FRIEND)
				{
					if(m_byStatus==GROUP_FRIEND_SELECT)					
						m_byRenderType = GROUP_RENDER_FRIENDUSERDELETE;
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}
				else if(m_byGroupTab==GROUP_TAB_GUILDWAR)
				{
					m_byRenderType = GROUP_RENDER_TEXT__GUILDWAR_LIST;
					if(m_byStatus==GROUP_GUILDWAR_SELECT)
					{
						if(g_pMainPlayer->m_byRank!=1)
							byChk = __MOUSE_POINTER_DEFAULT__;						
					}
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}			
				else
					byChk = __MOUSE_POINTER_DEFAULT__;
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}				
		break;
	case 9://�޴� ����°
		{
			BYTE byChk = 0;

			if(eMouseCheck==MOUSE_UP)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_SELECT)
					{
						if(m_nPartyUserIndex!=-1)
						{
							if(m_nPartyUserIndex!=0)
							{													
								ListNode<PARTY_USER>* pNode = g_pPartyUserHash->GetHead();

								BYTE byIndex = 0;

								while(pNode)
								{
									LP_PARTY_USER sPartyNode = pNode->pData;

									if(byIndex==m_nPartyUserIndex-1)
									{		
										CTWS_PARTY_INFO pPacket;
										memset(pPacket.szCharName, 0, sizeof(pPacket.szCharName));
										lstrcpy(pPacket.szCharName, sPartyNode->szName);
										pPacket.dwUserIndex	= sPartyNode->dwUserIndex;
										g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);																			
									}
									byIndex++;

									pNode = pNode->pNext;
								}						
							}
							else
								DisplayMessageAdd(g_Message[ETC_MESSAGE577].szMessage, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY); // "�ڽſ� ������ ���� �����ϴ�."
						}					
						SetRender(SPR_OBJ_GROUPWND_INFO1, FALSE);
						SetRender(SPR_OBJ_GROUPWND_INFO2, TRUE);
						SetRender(SPR_OBJ_GROUPWND_INFO3, FALSE);
						m_bPartyBtn[2] = FALSE;
						byChk = __MOUSE_POINTER_DEFAULT__;
					}
				}
				else if(m_byGroupTab==GROUP_TAB_GUILD)
				{
					if(m_byStatus==GROUP_GUILD_SELECT)
					{
						if(m_nPartyUserIndex==0 && m_byCurPage==1 && m_byUserType==GUILD_USER_ONLINE)
							DisplayMessageAdd(g_Message[ETC_MESSAGE577].szMessage, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD); // "�ڽſ� ������ ���� �����ϴ�."
						else if(m_byUserType==GUILD_USER_OFFLINE)
							DisplayMessageAdd(g_Message[ETC_MESSAGE733].szMessage, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD); // "�������� ĳ���Ϳ� ������ ���� �����ϴ�. "
						else
						{						
							BYTE					byIndex			= 0;
							ListNode<GUILD_USER>*	pGuildUserPos	= m_pGuildUserPos;

							if(m_byCurPage==1)
							{
								byIndex = 1;					
								pGuildUserPos = g_pGuildUserHash->GetHead();
							}

							while(pGuildUserPos)
							{
								LP_GUILD_USER sGuildUser = pGuildUserPos->pData;
								
								if(!sGuildUser)
									continue;

								if(m_nPartyUserIndex==byIndex)
								{
									CTWS_GUILDUSER_INFO pPacket;								
									pPacket.dwUserIndex	= sGuildUser->dwUserIndex;
									g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);								
								}										
								pGuildUserPos = pGuildUserPos->pNext;
								byIndex++;
							}
						}														
						SetRender(SPR_OBJ_GROUPWND_INFO1, FALSE);
						SetRender(SPR_OBJ_GROUPWND_INFO2, TRUE);
						SetRender(SPR_OBJ_GROUPWND_INFO3, FALSE);
						m_bGuildBtn[3] = FALSE;
						byChk = __MOUSE_POINTER_DEFAULT__;
					}
				}
				else if(m_byGroupTab==GROUP_TAB_FRIEND)
				{
					if(m_byStatus==GROUP_FRIEND_SELECT)
					{					
						if(m_byMessengerType==FRIEND_USER_OFFLINE)
							DisplayMessageAdd(g_Message[ETC_MESSAGE733].szMessage, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND); // "�������� ĳ���Ϳ� ������ ���� �����ϴ�. "
						else
						{						
							BYTE						byIndex				= 0;
							ListNode<MESSENGER_USER>*	pMessengerUserPos	= m_pMessengerOnlinePos;						

							while(pMessengerUserPos)
							{
								LP_MESSENGER_USER sMessengerUser = pMessengerUserPos->pData;
								
								if(!sMessengerUser)
									continue;

								if(m_nPartyUserIndex==byIndex)
								{
									CTWS_FRIENDUSER_INFO pPacket;
									pPacket.dwUserIndex	= sMessengerUser->dwUserIndex;
									g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
									CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_BUTTON__);
								}										
								pMessengerUserPos = pMessengerUserPos->pNext;
								byIndex++;
							}
						}														
						SetRender(SPR_OBJ_GROUPWND_INFO1, FALSE);
						SetRender(SPR_OBJ_GROUPWND_INFO2, TRUE);
						SetRender(SPR_OBJ_GROUPWND_INFO3, FALSE);
						m_bFriednBtn[3] = FALSE;
						byChk = __MOUSE_POINTER_DEFAULT__;
					}
				}
				else if(m_byGroupTab==GROUP_TAB_GUILDWAR)//���콺 ���϶�
				{
					//������ ������
				}
				else
					byChk = __MOUSE_POINTER_DEFAULT__;
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_SELECT)
					{					
						SetRender(SPR_OBJ_GROUPWND_INFO1, FALSE);
						SetRender(SPR_OBJ_GROUPWND_INFO2, FALSE);
						SetRender(SPR_OBJ_GROUPWND_INFO3, TRUE);
						m_bPartyBtn[2] = TRUE;
					}
					else
						byChk = __MOUSE_POINTER_DEFAULTCLICK__;
				}
				else if(m_byGroupTab==GROUP_TAB_GUILD)
				{
					if(m_byStatus==GROUP_GUILD_SELECT)
					{
						SetRender(SPR_OBJ_GROUPWND_INFO1, FALSE);
						SetRender(SPR_OBJ_GROUPWND_INFO2, FALSE);
						SetRender(SPR_OBJ_GROUPWND_INFO3, TRUE);
						m_bGuildBtn[3] = TRUE;
					}
					else
						byChk = __MOUSE_POINTER_DEFAULTCLICK__;
				}
				else if(m_byGroupTab==GROUP_TAB_FRIEND)
				{
					if(m_byStatus==GROUP_FRIEND_SELECT)
					{					
						SetRender(SPR_OBJ_GROUPWND_INFO1, FALSE);
						SetRender(SPR_OBJ_GROUPWND_INFO2, FALSE);
						SetRender(SPR_OBJ_GROUPWND_INFO3, TRUE);
						m_bFriednBtn[2] = TRUE;
					}
					else
						byChk = __MOUSE_POINTER_DEFAULTCLICK__;
				}
				else if(m_byGroupTab==GROUP_TAB_GUILDWAR)//���콺 �ٿ��϶�
				{
					SetStatus(GROUP_GUILDWAR_MATCH);
				}
				else
					byChk = __MOUSE_POINTER_DEFAULT__;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_SELECT)
						m_byRenderType = GROUP_RENDER_PARTYUSERINFO;					
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}
				else if(m_byGroupTab==GROUP_TAB_GUILD)
				{
					if(m_byStatus==GROUP_GUILD_SELECT)				
						m_byRenderType = GROUP_RENDER_GUILDUSERINFO;
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}
				else if(m_byGroupTab==GROUP_TAB_FRIEND)
				{
					if(m_byStatus==GROUP_FRIEND_SELECT)
						m_byRenderType = GROUP_RENDER_FRIENDSERINFO;
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}
				else if(m_byGroupTab==GROUP_TAB_GUILDWAR)
				{
					m_byRenderType = GROUP_RENDER_TEXT__GUILDWAR_MATCH;
				}
			}				
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}													
		break;
	case 10://�޴� �׹�°
		{
			BYTE byChk = 0;

			if(eMouseCheck==MOUSE_UP)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_JOIN || m_byStatus==GROUP_PARTY_SELECT)
					{
						CTWS_PARTY_LOGOUT pPacket;
						
						if(g_pMainPlayer->m_dwPartyId!=0)
						{
							pPacket.dwPartyId = g_pMainPlayer->m_dwPartyId;
							g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
							g_pMainPlayer->m_bPartyLeader = 0;

						}
						else
						{
							// "�������� ��Ƽ�� �����ϴ�."
							DisplayMessageAdd(g_Message[ETC_MESSAGE69].szMessage, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY); 
						}

					
						m_bPartyBtn[1]	= FALSE;					

						SetActive(FALSE);					
					}
					else
					{
						byChk = __MOUSE_POINTER_DEFAULT__;
					}
				}
				else if(m_byGroupTab==GROUP_TAB_GUILD)
				{
					if(m_byStatus==GROUP_GUILD_DEFAULT)
					{
						if(g_pMainPlayer->m_dwGuildId!=0)
						{	
							if(g_pMainPlayer->m_byRank==1)
							{
								// �޼��� ������ //
								CGuildDeleteWnd* pGuildDeleteWnd = CGuildDeleteWnd::GetInstance();
								pGuildDeleteWnd->SetActive();
								pGuildDeleteWnd->m_byType = 1;
								CInterface::GetInstance()->SetWnd(GUILDDELETE_WND);
															
								m_bGuildBtn[1]	= FALSE;
								SetRender(SPR_OBJ_GROUPWND_OUT1, FALSE);
								SetRender(SPR_OBJ_GROUPWND_OUT2, TRUE);
								SetRender(SPR_OBJ_GROUPWND_OUT3, FALSE);
							}
							else if(g_pMainPlayer->m_byRank!=1)
							{
								CGuildDeleteWnd* pGuildDeleteWnd = CGuildDeleteWnd::GetInstance();
								pGuildDeleteWnd->SetActive();
								pGuildDeleteWnd->m_byType		= 2;
								pGuildDeleteWnd->m_dwUserIndex	= g_pMainPlayer->m_dwUserIndex;
								CInterface::GetInstance()->SetWnd(GUILDDELETE_WND);
															
								m_bGuildBtn[1]	= FALSE;
								SetRender(SPR_OBJ_GROUPWND_OUT1, FALSE);
								SetRender(SPR_OBJ_GROUPWND_OUT2, TRUE);
								SetRender(SPR_OBJ_GROUPWND_OUT3, FALSE);
							}
							else
								byChk = __MOUSE_POINTER_DEFAULT__;
						}				
						else
							byChk = __MOUSE_POINTER_DEFAULT__;
					}
				}
				else if(m_byGroupTab==GROUP_TAB_FRIEND)
				{
					if(m_byStatus==GROUP_FRIEND_SELECT)
					{													
						if(m_byMessengerType==FRIEND_USER_ONLINE)
						{
							BYTE						byIndex				= 0;
							ListNode<MESSENGER_USER>*	pMessengerOnlinePos	= m_pMessengerOnlinePos;
							
							while(pMessengerOnlinePos)
							{
								LP_MESSENGER_USER sMessengerUser = pMessengerOnlinePos->pData;
								
								if(!sMessengerUser)
									continue;

								if(m_nPartyUserIndex==byIndex)
								{	
									CChatWnd*	pChatWnd	= CChatWnd::GetInstance();
									CInterface*	pInterface	= CInterface::GetInstance();
									
									g_pInputManager->ClearInput(INPUT_BUFFER_19);
									g_pInputManager->ClearInput(INPUT_BUFFER_6);
									g_pInputManager->InsertCharacter(INPUT_BUFFER_6, sMessengerUser->szName, lstrlen(sMessengerUser->szName));
									
	#ifdef _USE_IME
				
									POINT pChatPoint;
									pChatPoint.x	= 10;
									pChatPoint.y	= 635;
									GET_IMEEDIT()->SetEditIndex(1);
						//			GET_IMEEDIT()->ClearImeEditString();
									GET_IMEEDIT()->ActivateIME(pChatPoint, __MAX_BUUFER_SIZE__);
	#endif
									pChatWnd->m_byChatType	= CHAT_TYPE_WHISPER;									
									g_nChatModeExt			= 1;
									pChatWnd->SetActive();
									pInterface->SetWnd(CHAT_WND);

									ChatModeOn(INPUT_BUFFER_19, FALSE);
									ShowChatEditBackground(TRUE, GET_IMEEDIT()->GetOrder()-1);
								}										
								pMessengerOnlinePos = pMessengerOnlinePos->pNext;
								byIndex++;
							}
						}
						else
						{
							BYTE						byIndex				= 0;
							ListNode<MESSENGER_USER>*	pMessengerOnlinePos	= m_pMessengerOfflinePos;						

							while(pMessengerOnlinePos)
							{
								LP_MESSENGER_USER sMessengerUser = pMessengerOnlinePos->pData;
								
								if(!sMessengerUser)
									continue;

								if(m_nPartyUserIndex==byIndex)
								{	
									CChatWnd*	pChatWnd	= CChatWnd::GetInstance();
									CInterface*	pInterface	= CInterface::GetInstance();
									
									g_pInputManager->ClearInput(INPUT_BUFFER_19);
									g_pInputManager->ClearInput(INPUT_BUFFER_6);
									g_pInputManager->InsertCharacter(INPUT_BUFFER_6, sMessengerUser->szName, lstrlen(sMessengerUser->szName));
									
	#ifdef _USE_IME
					
									POINT pChatPoint;
									pChatPoint.x	= 10;
									pChatPoint.y	= 635;
									GET_IMEEDIT()->SetEditIndex(1);
						//			GET_IMEEDIT()->ClearImeEditString();
									GET_IMEEDIT()->ActivateIME(pChatPoint, __MAX_BUUFER_SIZE__);
	#endif

									pChatWnd->m_byChatType	= CHAT_TYPE_WHISPER;									
									g_nChatModeExt			= 1;
									pChatWnd->SetActive();
									pInterface->SetWnd(CHAT_WND);

									ChatModeOn(INPUT_BUFFER_19, FALSE);
									ShowChatEditBackground(TRUE, GET_IMEEDIT()->GetOrder()-1);
								}										
								pMessengerOnlinePos = pMessengerOnlinePos->pNext;
								byIndex++;
							}
						}
						m_bFriednBtn[1]	= FALSE;
						SetRender(SPR_OBJ_GROUPWND_MSG1, FALSE);
						SetRender(SPR_OBJ_GROUPWND_MSG2, TRUE);
						SetRender(SPR_OBJ_GROUPWND_MSG3, FALSE);						
					}											
				}
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_JOIN || m_byStatus==GROUP_PARTY_SELECT)    //��â (2005.02.14)
					{
						m_bPartyBtn[1]	= TRUE;
						SetRender(SPR_OBJ_GROUPWND_OUT1, FALSE);
						SetRender(SPR_OBJ_GROUPWND_OUT2, FALSE);
						SetRender(SPR_OBJ_GROUPWND_OUT3, TRUE);
						
					}
					else
						byChk = __MOUSE_POINTER_DEFAULTCLICK__;

					//��Ƽ ��ȭ �ý��� (����â 2005.02.14)
						if(g_pMainPlayer->m_bPartyLeader==1)
						{
							SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, FALSE);
							SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
							m_byPMatchStatus=0;
						}
						else if(g_pMainPlayer->m_bPartyLeader!=1 && g_pMainPlayer->m_dwPartyId != 0)
						{
							SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_ON, FALSE);
							SetRender(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK_OFF, TRUE);
							m_byPMatchStatus=0;
						}			
				}
				else if(m_byGroupTab==GROUP_TAB_GUILD)
				{
					if(m_byStatus==GROUP_GUILD_DEFAULT)
					{
						if(g_pMainPlayer->m_dwGuildId!=0)
						{							
							m_bGuildBtn[1]	= TRUE;
							SetRender(SPR_OBJ_GROUPWND_OUT1, FALSE);
							SetRender(SPR_OBJ_GROUPWND_OUT2, FALSE);
							SetRender(SPR_OBJ_GROUPWND_OUT3, TRUE);													
						}				
						else
							byChk = __MOUSE_POINTER_DEFAULTCLICK__;
					}
				}
				else if(m_byGroupTab==GROUP_TAB_FRIEND)
				{
					if(m_byStatus==GROUP_FRIEND_SELECT)
					{
						SetRender(SPR_OBJ_GROUPWND_MSG1, FALSE);
						SetRender(SPR_OBJ_GROUPWND_MSG2, FALSE);
						SetRender(SPR_OBJ_GROUPWND_MSG3, TRUE);
						m_bFriednBtn[1]	= TRUE;						
					}				
					else
						byChk = __MOUSE_POINTER_DEFAULTCLICK__;
				}
				else if(m_byGroupTab==GROUP_TAB_GUILDWAR)//���콺 �ٿ��϶�
				{
					if(g_pMainPlayer->m_byRank == 1 || g_pMainPlayer->m_byRank == 2)
					{
						SetRender(SPR_OBJ_GUILDWAR_CHALLENGE1, FALSE);
						SetRender(SPR_OBJ_GUILDWAR_CHALLENGE2, TRUE);
						CUserInterface::GetInstance()->SetPointerChk( TRUE );
						CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_GUARDIAN__);

						m_bGuildWarChallenge = TRUE;
					}
					else
					{
						DisplayMessageAdd(g_Message[ETC_MESSAGE1386].szMessage, 0xffffff00); // "�α�帶����(2���)�̻� ���� ��û�� �� �� �ֽ��ϴ�."
					}
				}
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_JOIN || m_byStatus==GROUP_PARTY_SELECT)					
						m_byRenderType = GROUP_RENDER_PARTYUSERLOGOUT;
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}
				else if(m_byGroupTab==GROUP_TAB_GUILD)
				{
					if(m_byStatus==GROUP_GUILD_DEFAULT)
					{
						if(g_pMainPlayer->m_dwGuildId!=0)						
							m_byRenderType = GROUP_RENDER_GUILDDELETE;
						else
							byChk = __MOUSE_POINTER_DEFAULT__;
					}
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}
				else if(m_byGroupTab==GROUP_TAB_FRIEND)
				{
					if(m_byStatus==GROUP_FRIEND_SELECT)					
						m_byRenderType = GROUP_RENDER_FRIENDUSERMSG;
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}
				else if(m_byGroupTab==GROUP_TAB_GUILDWAR)
				{
					m_byRenderType = GROUP_RENDER_TEXT__GUILDWAR_CHALLENGE;
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);		
		}				
		break;
	case 11://�޴� �ټ���°
		{
			BYTE byChk = 0;

			if(eMouseCheck==MOUSE_UP)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_SELECT)
					{
						if(m_nPartyUserIndex==0)
						{
							// "�ڱ� �ڽ��� �Ӹ��� ���� �� �����ϴ�."
							DisplayMessageAdd(g_Message[ETC_MESSAGE578].szMessage, 0xffff9a63, TRUE, DEF_CHAT_TYPE_PARTY); 
						}
						else if(m_nPartyUserIndex>0)
						{								
							ListNode<PARTY_USER>* pNode = g_pPartyUserHash->GetHead();

							BYTE byIndex = 0;

							while(pNode)
							{
								LP_PARTY_USER sPartyNode = pNode->pData;

								byIndex++;

								if(m_nPartyUserIndex==byIndex)
								{
									CChatWnd*	pChatWnd	= CChatWnd::GetInstance();
									CInterface*	pInterface	= CInterface::GetInstance();
									
									g_pInputManager->ClearInput(INPUT_BUFFER_19);
									g_pInputManager->ClearInput(INPUT_BUFFER_6);
									g_pInputManager->InsertCharacter(INPUT_BUFFER_6, sPartyNode->szName, lstrlen(sPartyNode->szName));


	#ifdef _USE_IME
				
									POINT pChatPoint;
									pChatPoint.x	= 10;
									pChatPoint.y	= 635;
									GET_IMEEDIT()->SetEditIndex(1);
						//			GET_IMEEDIT()->ClearImeEditString();
									GET_IMEEDIT()->ActivateIME(pChatPoint, __MAX_BUUFER_SIZE__);
	#endif

									pChatWnd->m_byChatType	= CHAT_TYPE_WHISPER;									
									g_nChatModeExt			= 1;
									pChatWnd->SetActive();
									pInterface->SetWnd(CHAT_WND);
									
									ChatModeOn(INPUT_BUFFER_19, FALSE);
									ShowChatEditBackground(TRUE, GET_IMEEDIT()->GetOrder()-1);
								}			
								pNode = pNode->pNext;
							}
						}
						m_bPartyBtn[0]	= FALSE;
						SetRender(SPR_OBJ_GROUPWND_MSG1, FALSE);
						SetRender(SPR_OBJ_GROUPWND_MSG2, TRUE);
						SetRender(SPR_OBJ_GROUPWND_MSG3, FALSE);					
					}
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}
				else if(m_byGroupTab==GROUP_TAB_GUILD)
				{
					if(m_byStatus==GROUP_GUILD_SELECT)
					{
						if(g_pMainPlayer->m_byRank==1)
						{
							if(m_nPartyUserIndex==0 && m_byCurPage==1 && m_byUserType==GUILD_USER_ONLINE)
							{
								// "�ڱ� �ڽ��� �Ӹ��� ���� �� �����ϴ�."						
								DisplayMessageAdd(g_Message[ETC_MESSAGE578].szMessage, 0xffff9a63, TRUE, DEF_CHAT_TYPE_GUILD); 
							}
							else
							{
								if(m_byUserType==GUILD_USER_ONLINE)
								{								
									BYTE					byIndex			= 0;
									ListNode<GUILD_USER>*	pGuildUserPos	= m_pGuildUserPos;

									if(m_byCurPage==1)
									{
										byIndex = 1;					
										pGuildUserPos = g_pGuildUserHash->GetHead();
									}

									while(pGuildUserPos)
									{
										LP_GUILD_USER sGuildUser = pGuildUserPos->pData;
										
										if(!sGuildUser)
											continue;

										if(m_nPartyUserIndex==byIndex)
										{	
											CChatWnd*	pChatWnd	= CChatWnd::GetInstance();
											CInterface*	pInterface	= CInterface::GetInstance();
											
											g_pInputManager->ClearInput(INPUT_BUFFER_19);
											g_pInputManager->ClearInput(INPUT_BUFFER_6);
											g_pInputManager->InsertCharacter(INPUT_BUFFER_6, sGuildUser->szCharacterName, lstrlen(sGuildUser->szCharacterName));

											

	#ifdef _USE_IME
							
											POINT pChatPoint;
											pChatPoint.x	= 10;
											pChatPoint.y	= 635;
											GET_IMEEDIT()->SetEditIndex(1);
								//			GET_IMEEDIT()->ClearImeEditString();
											GET_IMEEDIT()->ActivateIME(pChatPoint, __MAX_BUUFER_SIZE__);
	#endif

											pChatWnd->m_byChatType	= CHAT_TYPE_WHISPER;											
											g_nChatModeExt			= 1;
											pChatWnd->SetActive();
											pInterface->SetWnd(CHAT_WND);
											
											ChatModeOn(INPUT_BUFFER_19, FALSE);
											ShowChatEditBackground(TRUE, GET_IMEEDIT()->GetOrder()-1);
										}										
										pGuildUserPos = pGuildUserPos->pNext;
										byIndex++;
									}
								}
								else
								{
									BYTE							byIndex					= 0;
									ListNode<GUILD_OFFLINE_USER>*	pGuildOffLineUserPos	= m_pGuildOfflineUserPos;

									while(pGuildOffLineUserPos)
									{
										LP_GUILD_OFFLINE_USER sGuildOfflineUser = pGuildOffLineUserPos->pData;
										
										if(!sGuildOfflineUser)
											continue;

										if(m_nPartyUserIndex==byIndex)
										{	
											CChatWnd*	pChatWnd	= CChatWnd::GetInstance();
											CInterface*	pInterface	= CInterface::GetInstance();
											
											g_pInputManager->ClearInput(INPUT_BUFFER_19);
											g_pInputManager->ClearInput(INPUT_BUFFER_6);
											g_pInputManager->InsertCharacter(INPUT_BUFFER_6, sGuildOfflineUser->szName, lstrlen(sGuildOfflineUser->szName));
											
	#ifdef _USE_IME
				
											POINT pChatPoint;
											pChatPoint.x	= 10;
											pChatPoint.y	= 635;
											GET_IMEEDIT()->SetEditIndex(1);
								//			GET_IMEEDIT()->ClearImeEditString();
											GET_IMEEDIT()->ActivateIME(pChatPoint, __MAX_BUUFER_SIZE__);
	#endif

											pChatWnd->m_byChatType	= CHAT_TYPE_WHISPER;											
											g_nChatModeExt			= 1;
											pChatWnd->SetActive();
											pInterface->SetWnd(CHAT_WND);
											
											ChatModeOn(INPUT_BUFFER_19, FALSE);
											ShowChatEditBackground(TRUE, GET_IMEEDIT()->GetOrder()-1);
										}										
										pGuildOffLineUserPos = pGuildOffLineUserPos->pNext;
										byIndex++;
									}
								}
							}
						}
						else
						{
							if(m_nPartyUserIndex==0)
							{
								// "�ڱ� �ڽ��� �Ӹ��� ���� �� �����ϴ�."
								DisplayMessageAdd(g_Message[ETC_MESSAGE578].szMessage, 0xffff9a63, TRUE, DEF_CHAT_TYPE_GUILD); 
							}
							else
							{
								BYTE					byIndex			= 0;
								ListNode<GUILD_USER>*	pGuildUserPos	= m_pGuildUserPos;

								if(m_byCurPage==1)
								{
									byIndex = 1;					
									pGuildUserPos = g_pGuildUserHash->GetHead();
								}

								while(pGuildUserPos)
								{
									LP_GUILD_USER sGuildUser = pGuildUserPos->pData;
									
									if(!sGuildUser)
										continue;

									if(m_nPartyUserIndex==byIndex)
									{	
										CChatWnd*	pChatWnd	= CChatWnd::GetInstance();
										CInterface*	pInterface	= CInterface::GetInstance();
										
										g_pInputManager->ClearInput(INPUT_BUFFER_19);
										g_pInputManager->ClearInput(INPUT_BUFFER_6);
										g_pInputManager->InsertCharacter(INPUT_BUFFER_6, sGuildUser->szCharacterName, lstrlen(sGuildUser->szCharacterName));									

	#ifdef _USE_IME
				
										POINT pChatPoint;
										pChatPoint.x	= 10;
										pChatPoint.y	= 635;
										GET_IMEEDIT()->SetEditIndex(1);
							//			GET_IMEEDIT()->ClearImeEditString();
										GET_IMEEDIT()->ActivateIME(pChatPoint, __MAX_BUUFER_SIZE__);
	#endif

										pChatWnd->m_byChatType	= CHAT_TYPE_WHISPER;										
										g_nChatModeExt			= 1;
										pChatWnd->SetActive();
										pInterface->SetWnd(CHAT_WND);								

										ChatModeOn(INPUT_BUFFER_19, FALSE);
										ShowChatEditBackground(TRUE, GET_IMEEDIT()->GetOrder()-1);

									}										
									pGuildUserPos = pGuildUserPos->pNext;
									byIndex++;
								}
							}
						}										
						m_bGuildBtn[2]	= FALSE;
						SetRender(SPR_OBJ_GROUPWND_MSG1, FALSE);
						SetRender(SPR_OBJ_GROUPWND_MSG2, TRUE);
						SetRender(SPR_OBJ_GROUPWND_MSG3, FALSE);					
					}
				}
				else if(m_byGroupTab==GROUP_TAB_FRIEND)
				{
					if(m_byStatus==GROUP_FRIEND_DEFAULT)
					{
						if(m_byMessengerType==FRIEND_USER_ONLINE)
						{
							m_byCurPage	= 1;
							m_byMessengerType = FRIEND_USER_OFFLINE;
							SetPrevNext(__GUILD_PAGE_CURR__);
						}
						else
						{
							m_byCurPage	= 1;
							m_byMessengerType  = FRIEND_USER_ONLINE;
							SetPrevNext(__GUILD_PAGE_CURR__);								
						}
						m_bFriednBtn[0] = FALSE;

						SetRender(SPR_OBJ_GUILD_ONOFF1, FALSE);
						SetRender(SPR_OBJ_GUILD_ONOFF2, TRUE);
						SetRender(SPR_OBJ_GUILD_ONOFF3, FALSE);												
					}
					else
						byChk = __MOUSE_POINTER_DEFAULT__;						
				}

				//��Ƽ��Ȳ (����â 2005.03.03)
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_PARTYUSERINFO2)
					{  
			//			BYTE byIndex = nRt-12;
						int iRealSelect = 0;

						if((DWORD)((m_byCurPage-1)*__FRIEND_MAX_USER_DISPLAY__+ m_nPartyUserIndex)<(m_dwPartyCount))
						{		
						//	for(int i = 0; i < __FRIEND_MAX_USER_DISPLAY__; i++)
						//	{
								iRealSelect = m_nPartyUserIndex + ((m_byCurPage-1)*__FRIEND_MAX_USER_DISPLAY__);
				
								if(m_nPartyUserIndex == iRealSelect)
								{
									if(m_nPartyFriend[iRealSelect].bOnline == 0)
									{
								//		m_nPartyUserIndex = nRt-12;

								//		if(m_nPartyUserIndex==byIndex)
								//		{	
											CChatWnd*	pChatWnd	= CChatWnd::GetInstance();
											CInterface*	pInterface	= CInterface::GetInstance();
											
											g_pInputManager->ClearInput(INPUT_BUFFER_19);
											g_pInputManager->ClearInput(INPUT_BUFFER_6);
											g_pInputManager->InsertCharacter(INPUT_BUFFER_6, m_nPartyFriend[iRealSelect].szCharacterName, lstrlen(m_nPartyFriend[iRealSelect].szCharacterName));

	#ifdef _USE_IME
				
											POINT pChatPoint;
											pChatPoint.x	= 10;
											pChatPoint.y	= 635;
											GET_IMEEDIT()->SetEditIndex(1);
								//			GET_IMEEDIT()->ClearImeEditString();
											GET_IMEEDIT()->ActivateIME(pChatPoint, __MAX_BUUFER_SIZE__);
	#endif

											pChatWnd->m_byChatType	= CHAT_TYPE_WHISPER;									
											g_nChatModeExt			= 1;
											pChatWnd->SetActive();
											pInterface->SetWnd(CHAT_WND);
									
											ChatModeOn(INPUT_BUFFER_19, FALSE);
											ShowChatEditBackground(TRUE, GET_IMEEDIT()->GetOrder()-1);
								//		}
									}
								}
								else if(m_nPartyUserIndex == iRealSelect-8)
								{
									if(m_nPartyFriend[iRealSelect].bOnline == 0)
									{
								//		m_nPartyUserIndex = nRt-12;

								//		if(m_nPartyUserIndex==byIndex)
								//		{	
											CChatWnd*	pChatWnd	= CChatWnd::GetInstance();
											CInterface*	pInterface	= CInterface::GetInstance();
											
											g_pInputManager->ClearInput(INPUT_BUFFER_19);
											g_pInputManager->ClearInput(INPUT_BUFFER_6);
											g_pInputManager->InsertCharacter(INPUT_BUFFER_6, m_nPartyFriend[iRealSelect].szCharacterName, lstrlen(m_nPartyFriend[iRealSelect].szCharacterName));

	#ifdef _USE_IME
				
											POINT pChatPoint;
											pChatPoint.x	= 10;
											pChatPoint.y	= 635;
											GET_IMEEDIT()->SetEditIndex(1);
								//			GET_IMEEDIT()->ClearImeEditString();
											GET_IMEEDIT()->ActivateIME(pChatPoint, __MAX_BUUFER_SIZE__);
	#endif

											pChatWnd->m_byChatType	= CHAT_TYPE_WHISPER;									
											g_nChatModeExt			= 1;
											pChatWnd->SetActive();
											pInterface->SetWnd(CHAT_WND);
									
											ChatModeOn(INPUT_BUFFER_19, FALSE);
											ShowChatEditBackground(TRUE, GET_IMEEDIT()->GetOrder()-1);
								//		}							
									}									
								}
								else if(m_nPartyUserIndex == iRealSelect-16)
								{
									if(m_nPartyFriend[iRealSelect].bOnline == 0)
									{
							//			m_nPartyUserIndex = nRt-12;

							//			if(m_nPartyUserIndex==byIndex)
							//			{						
											CChatWnd*	pChatWnd	= CChatWnd::GetInstance();
											CInterface*	pInterface	= CInterface::GetInstance();
											
											g_pInputManager->ClearInput(INPUT_BUFFER_19);
											g_pInputManager->ClearInput(INPUT_BUFFER_6);
											g_pInputManager->InsertCharacter(INPUT_BUFFER_6, m_nPartyFriend[iRealSelect].szCharacterName, lstrlen(m_nPartyFriend[iRealSelect].szCharacterName));

	#ifdef _USE_IME
				
											POINT pChatPoint;
											pChatPoint.x	= 10;
											pChatPoint.y	= 635;
											GET_IMEEDIT()->SetEditIndex(1);
							//				GET_IMEEDIT()->ClearImeEditString();
											GET_IMEEDIT()->ActivateIME(pChatPoint, __MAX_BUUFER_SIZE__);
	#endif

											pChatWnd->m_byChatType	= CHAT_TYPE_WHISPER;									
											g_nChatModeExt			= 1;
											pChatWnd->SetActive();
											pInterface->SetWnd(CHAT_WND);
									
											ChatModeOn(INPUT_BUFFER_19, FALSE);
											ShowChatEditBackground(TRUE, GET_IMEEDIT()->GetOrder()-1);
							//			}
									}									
								}
							}
							m_bPartyBtn[0]	= FALSE;
							SetRender(SPR_OBJ_GROUPWND_MSG1, FALSE);
							SetRender(SPR_OBJ_GROUPWND_MSG2, TRUE);
							SetRender(SPR_OBJ_GROUPWND_MSG3, FALSE);
					//	}
					}
				}

			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_SELECT)
					{
						SetRender(SPR_OBJ_GROUPWND_MSG1, FALSE);
						SetRender(SPR_OBJ_GROUPWND_MSG2, FALSE);
						SetRender(SPR_OBJ_GROUPWND_MSG3, TRUE);
						m_bPartyBtn[0]	= TRUE;						
					}
					else
						byChk = __MOUSE_POINTER_DEFAULTCLICK__;
				}
				else if(m_byGroupTab==GROUP_TAB_GUILD)
				{
					if(m_byStatus==GROUP_GUILD_SELECT)
					{
						SetRender(SPR_OBJ_GROUPWND_MSG1, FALSE);
						SetRender(SPR_OBJ_GROUPWND_MSG2, FALSE);
						SetRender(SPR_OBJ_GROUPWND_MSG3, TRUE);
						m_bGuildBtn[2]	= TRUE;
					}				
					else
						byChk = __MOUSE_POINTER_DEFAULTCLICK__;
				}
				else if(m_byGroupTab==GROUP_TAB_FRIEND)
				{
					if(m_byStatus==GROUP_FRIEND_DEFAULT)
					{
						m_bFriednBtn[0] = TRUE;
						SetRender(SPR_OBJ_GUILD_ONOFF2, FALSE);
						SetRender(SPR_OBJ_GUILD_ONOFF3, TRUE);
					}					
					else
						byChk = __MOUSE_POINTER_DEFAULTCLICK__;
				}
				//��Ƽ��Ȳ (2005.03.03)
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_PARTYUSERINFO2)
					{
						SetRender(SPR_OBJ_GROUPWND_MSG1, FALSE);
						SetRender(SPR_OBJ_GROUPWND_MSG2, FALSE);
						SetRender(SPR_OBJ_GROUPWND_MSG3, TRUE);
						m_bPartyBtn[0]	= TRUE;	
					}
					else
						byChk = __MOUSE_POINTER_DEFAULTCLICK__;
				}
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_SELECT)					
						m_byRenderType = GROUP_RENDER_PARTYMSG;
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}
				else if(m_byGroupTab==GROUP_TAB_GUILD)
				{
					if(m_byStatus==GROUP_GUILD_SELECT)
						m_byRenderType = GROUP_RENDER_GUILDMSG;
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}
				else if(m_byGroupTab==GROUP_TAB_FRIEND)
				{
					if(m_byStatus==GROUP_FRIEND_DEFAULT)
						m_byRenderType = GROUP_RENDER_FRIENDUSERONOFF;						
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}
				//��Ƽ��Ȳ (2005. 03. 03)
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_PARTYUSERINFO2)
						m_byRenderType = GROUP_RENDER_PARTYMSG;
				    else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}

			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;	
	case 22:
		{
			BYTE byChk = 0;

			if(eMouseCheck==MOUSE_UP)
			{
				if(m_byGroupTab==GROUP_TAB_GUILD)
				{
					if(m_byStatus==GROUP_GUILD_SELECT)
					{
						if(g_pMainPlayer->m_dwGuildId!=0)
						{
							if(g_pMainPlayer->m_byRank==1)
							{							
								m_bGuildBtn[5] = FALSE;
								SetRender(SPR_OBJ_GUILD_INFO2, TRUE);
								SetRender(SPR_OBJ_GUILD_INFO3, FALSE);

								if(m_byCurPage==1 && m_nPartyUserIndex==0 && m_byUserType==GUILD_USER_ONLINE)
								{
									// Open //
									CGuildEditWnd* pGuildEditWnd = CGuildEditWnd::GetInstance();									
																									

									RECT rt;
									rt.left = long(pGuildEditWnd->m_fPosX+32);	rt.right = long(pGuildEditWnd->m_fPosX+150);
									rt.top = long(pGuildEditWnd->m_fPosZ+42);	rt.bottom = long(pGuildEditWnd->m_fPosZ+54);
									int nOrder = pGuildEditWnd->GetStartOrder()+1;
									g_pInputManager->InitializeInput(INPUT_BUFFER_12, FONT_SS3D_GAME, rt, 0xffffffff, nOrder);
									
									ChatModeOn(INPUT_BUFFER_12, FALSE);
									ShowChatEditBackground(TRUE, GET_IMEEDIT()->GetOrder()-1);

									pGuildEditWnd->SetData(g_pMainPlayer->m_szName, g_pMainPlayer->m_szNickName, g_pMainPlayer->m_byRank, g_pMainPlayer->m_dwUserIndex);
									pGuildEditWnd->SetActive(TRUE);									
									CInterface::GetInstance()->SetWnd(GUILDEDIT_WND);
#ifdef _USE_IME
			
									POINT pChatPoint;
									pChatPoint.x	= long(pGuildEditWnd->m_fPosX+32);
									pChatPoint.y	= long(pGuildEditWnd->m_fPosZ+42);
									GET_IMEEDIT()->SetEditIndex(1);
									GET_IMEEDIT()->ActivateIME(pChatPoint, MAX_NICK_REAL_NAME, pGuildEditWnd->GetStartOrder()+2);
#endif
								
									CInterface::GetInstance()->SetWnd(GUILDEDIT_WND);
									break;
								}
								else if(m_byUserType==GUILD_USER_ONLINE)
								{
									BYTE byIndex = 0; 

									ListNode<GUILD_USER>* pGuildUserPos = m_pGuildUserPos;

									if(m_byCurPage==1)
									{
										pGuildUserPos	= g_pGuildUserHash->GetHead();
										byIndex			= 1; 
									}

									while(pGuildUserPos)
									{
										LP_GUILD_USER sGuildUser = pGuildUserPos->pData;
										
										if(!sGuildUser)
											continue;										

										if(m_nPartyUserIndex==byIndex)
										{
											// Open //
											CGuildEditWnd* pGuildEditWnd = CGuildEditWnd::GetInstance();
											RECT rt;
											rt.left = long(pGuildEditWnd->m_fPosX)+32;	rt.right = long(pGuildEditWnd->m_fPosX)+150;
											rt.top = long(pGuildEditWnd->m_fPosZ)+42;	rt.bottom = long(pGuildEditWnd->m_fPosZ)+54;
											int nOrder = pGuildEditWnd->GetStartOrder()+1;
											g_pInputManager->InitializeInput(INPUT_BUFFER_12, FONT_SS3D_GAME, rt, 0xffffffff, nOrder);
											
											ChatModeOn(INPUT_BUFFER_12, FALSE);
											ShowChatEditBackground(TRUE, GET_IMEEDIT()->GetOrder()-1);

											pGuildEditWnd->SetData(sGuildUser->szCharacterName, sGuildUser->szNickName, sGuildUser->byRank, sGuildUser->dwUserIndex);
											pGuildEditWnd->SetActive(TRUE);
										
											CInterface::GetInstance()->SetWnd(GUILDEDIT_WND);											
#ifdef _USE_IME			
											POINT pChatPoint;
											pChatPoint.x	= long(pGuildEditWnd->m_fPosX+32);
											pChatPoint.y	= long(pGuildEditWnd->m_fPosZ+42);
											GET_IMEEDIT()->SetEditIndex(1);
											GET_IMEEDIT()->ActivateIME(pChatPoint, MAX_NICK_REAL_NAME, pGuildEditWnd->GetStartOrder()+2);
#endif		
											break;
										}							
										pGuildUserPos = pGuildUserPos->pNext;
										byIndex++;										
									}
								}	
								else
								{
									BYTE byIndex = 0; 

									ListNode<GUILD_OFFLINE_USER>* pGuildOfflineUserPos = m_pGuildOfflineUserPos;
									
									while(pGuildOfflineUserPos)
									{
										LP_GUILD_OFFLINE_USER sGuildOfflineUser = pGuildOfflineUserPos->pData;
										
										if(!sGuildOfflineUser)
											continue;										

										if(m_nPartyUserIndex==byIndex)
										{
											// Open //
											CGuildEditWnd* pGuildEditWnd = CGuildEditWnd::GetInstance();

											RECT rt;
											rt.left = long(pGuildEditWnd->m_fPosX)+32;	rt.right = long(pGuildEditWnd->m_fPosX)+150;
											rt.top = long(pGuildEditWnd->m_fPosZ)+42;	rt.bottom = long(pGuildEditWnd->m_fPosZ)+54;
											int nOrder = pGuildEditWnd->GetStartOrder()+1;
											g_pInputManager->InitializeInput(INPUT_BUFFER_12, FONT_SS3D_GAME, rt, 0xffffffff, nOrder);
											
											ChatModeOn(INPUT_BUFFER_12, FALSE);
											ShowChatEditBackground(TRUE, GET_IMEEDIT()->GetOrder()-1);

											pGuildEditWnd->SetData(sGuildOfflineUser->szName, sGuildOfflineUser->szNickName, sGuildOfflineUser->byRank, sGuildOfflineUser->dwUserIndex);
											pGuildEditWnd->SetActive(TRUE);
											CInterface::GetInstance()->SetWnd(GUILDEDIT_WND);
#ifdef _USE_IME			
											POINT pChatPoint;
											pChatPoint.x	= long(pGuildEditWnd->m_fPosX+32);
											pChatPoint.y	= long(pGuildEditWnd->m_fPosZ+42);
											GET_IMEEDIT()->SetEditIndex(1);
											GET_IMEEDIT()->ActivateIME(pChatPoint, MAX_NICK_REAL_NAME, pGuildEditWnd->GetStartOrder()+2);
#endif												
											CInterface::GetInstance()->SetWnd(GUILDEDIT_WND);											
											break;
										}							
										pGuildOfflineUserPos = pGuildOfflineUserPos->pNext;
										byIndex++;										
									}
								}
							}	
							else
								byChk = __MOUSE_POINTER_DEFAULT__;
						}
						else
							byChk = __MOUSE_POINTER_DEFAULT__;
					}
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}
				else if(m_byGroupTab==GROUP_TAB_FRIEND)
				{
					if(m_byStatus==GROUP_FRIEND_DEFAULT)
					{
						if(m_byMessengerType==FRIEND_USER_ONLINE)
						{
							m_byCurPage	= 1;
							m_byMessengerType = FRIEND_USER_OFFLINE;
							SetPrevNext(__GUILD_PAGE_CURR__);
						}
						else
						{
							m_byCurPage	= 1;
							m_byMessengerType  = FRIEND_USER_ONLINE;
							SetPrevNext(__GUILD_PAGE_CURR__);								
						}
						m_bFriednBtn[0] = FALSE;

						SetRender(SPR_OBJ_GUILD_ONOFF1, FALSE);
						SetRender(SPR_OBJ_GUILD_ONOFF2, TRUE);
						SetRender(SPR_OBJ_GUILD_ONOFF3, FALSE);												
					}
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}



#ifdef PARTY_BOARD

				else if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(	m_byStatus==GROUP_PARTY_DEFAULT		||
						m_byStatus==GROUP_PARTY_CREATEADD	||
						m_byStatus==GROUP_PARTY_JOIN)
					{
						if(!g_pPartyBoardHash->GetData(g_pMainPlayer->m_dwPartyId))
						{
							SetRender(SPR_OBJ_GROUPWND_PBCREATE1, FALSE);	
							SetRender(SPR_OBJ_GROUPWND_PBCREATE2, TRUE);	
							SetRender(SPR_OBJ_GROUPWND_PBCREATE3, FALSE);
							m_bPartyBtn[4] = TRUE;

							if(g_pMainPlayer->m_dwPartyId!=0)
							{
								CPartyEntryWnd* pPartyEntryWnd	= CPartyEntryWnd::GetInstance();
								pPartyEntryWnd->m_byEntryType	= PARTYENTRY_INPUT;
								pPartyEntryWnd->SetActive();
							}
							else
							{
								DisplayMessageAdd(g_Message[ETC_MESSAGE69].szMessage, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY); 
							}
						}
						else
						{
							byChk = __MOUSE_POINTER_DEFAULT__;
						}
					}
					else
					{
						byChk = __MOUSE_POINTER_DEFAULT__;					
					}
				}
#endif				
				else
				{
					byChk = __MOUSE_POINTER_DEFAULT__;
				}
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_byGroupTab==GROUP_TAB_GUILD)
				{			
					if(m_byStatus==GROUP_GUILD_SELECT)
					{
						if(g_pMainPlayer->m_dwGuildId!=0)
						{
							if(g_pMainPlayer->m_byRank==1)
							{
								m_bGuildBtn[5] = TRUE;
								SetRender(SPR_OBJ_GUILD_INFO2, FALSE);
								SetRender(SPR_OBJ_GUILD_INFO3, TRUE);								
							}
							else
								byChk = __MOUSE_POINTER_DEFAULTCLICK__;
						}
						else
							byChk = __MOUSE_POINTER_DEFAULTCLICK__;
					}					
					else
						byChk = __MOUSE_POINTER_DEFAULTCLICK__;
				}		

#ifdef PARTY_BOARD

				else if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_DEFAULT || m_byStatus==GROUP_PARTY_CREATEADD || m_byStatus==GROUP_PARTY_JOIN)
					{
						if(!g_pPartyBoardHash->GetData(g_pMainPlayer->m_dwPartyId))
						{
							SetRender(SPR_OBJ_GROUPWND_PBCREATE1, FALSE);	
							SetRender(SPR_OBJ_GROUPWND_PBCREATE2, FALSE);	
							SetRender(SPR_OBJ_GROUPWND_PBCREATE3, TRUE);
							m_bPartyBtn[4] = TRUE;							
						}
						else
							byChk = __MOUSE_POINTER_DEFAULTCLICK__;
					}
					else
						byChk = __MOUSE_POINTER_DEFAULTCLICK__;					
				}	
				
#endif

				else
					byChk = __MOUSE_POINTER_DEFAULTCLICK__;				
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(m_byGroupTab==GROUP_TAB_GUILD)
				{
					if(g_pMainPlayer->m_dwGuildId!=0)
					{
						if(g_pMainPlayer->m_byRank==1)
						{
							if(m_byStatus==GROUP_GUILD_SELECT)							
								m_byRenderType = GROUP_RENDER_GUILDUSERRANK;								
							else
								byChk = __MOUSE_POINTER_DEFAULT__;
						}
						else
							byChk = __MOUSE_POINTER_DEFAULT__;
					}
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}
				
				

#ifdef PARTY_BOARD

				else if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_DEFAULT || m_byStatus==GROUP_PARTY_CREATEADD || m_byStatus==GROUP_PARTY_JOIN)
					{
						if(!g_pPartyBoardHash->GetData(g_pMainPlayer->m_dwPartyId))
							m_byRenderType = GROUP_RENDER_PARTYBOARDCREATE;
						else
							byChk = __MOUSE_POINTER_DEFAULT__;
					}
					else
						byChk = __MOUSE_POINTER_DEFAULT__;					
				}

#endif

				else
					byChk = __MOUSE_POINTER_DEFAULT__;
			}

			//��Ƽ ��ȭ �ý��� (����â 2005.01.25)     
			if(eMouseCheck==MOUSE_DOWN)					 // ���콺�� �������� ������ ��ȭ  
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{		
					SetStatus(GROUP_PARTY_PARTYUSERINFO1);
					byChk = __MOUSE_POINTER_BUTTONCLICK__;
					SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE);
				}
				else
					byChk = __MOUSE_POINTER_DEFAULT__;

			}
			else if(eMouseCheck==MOUSE_UP)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					SetStatus(GROUP_PARTY_PARTYUSERINFO2);
					byChk = __MOUSE_POINTER_BUTTON__;	
					SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE); 

					m_byCurPage	= 1;
//					SetPrevNext(__FRIEND_PAGE_CURR__);

					CTWS_PARTY_FRIEND pPacket;
					g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
				}
				else 
					byChk = __MOUSE_POINTER_DEFAULT__;  
			} 

			if(eMouseCheck==MOUSE_OVER)			// ���콺�� �����ܿ� �÷� �������� ������ ���� �� ���� ǥ�� 
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					m_byRenderType = GROUP_RENDER_PARTYUSERINFO_LIST;
					byChk = __MOUSE_POINTER_BUTTON__;
				}
				else 
					byChk = __MOUSE_POINTER_DEFAULT__;
			}

			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;
	case 23:
		{
			BYTE byChk = 0;

			if(eMouseCheck==MOUSE_UP)
			{
				if(m_byGroupTab==GROUP_TAB_GUILD)
				{
					if(m_byStatus==GROUP_GUILD_DEFAULT)
					{
						if(g_pMainPlayer->m_dwGuildId!=0)
						{
							if(g_pMainPlayer->m_byRank==1)
							{
								if(m_byUserType==GUILD_USER_ONLINE)
								{
									m_byCurPage	= 1;
									m_byUserType = GUILD_USER_OFFLINE;
									SetPrevNext(__GUILD_PAGE_CURR__);
								}
								else
								{
									m_byCurPage	= 1;
									m_byUserType = GUILD_USER_ONLINE;
									SetPrevNext(__GUILD_PAGE_CURR__);								
								}
								m_bGuildBtn[0] = FALSE;

								SetRender(SPR_OBJ_GUILD_ONOFF1, FALSE);
								SetRender(SPR_OBJ_GUILD_ONOFF2, TRUE);
								SetRender(SPR_OBJ_GUILD_ONOFF3, FALSE);																
							}	
							else
								byChk = __MOUSE_POINTER_DEFAULT__;
						}
						else
							byChk = __MOUSE_POINTER_DEFAULT__;
					}
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}				
#ifdef PARTY_BOARD

				else if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_DEFAULT || m_byStatus==GROUP_PARTY_CREATEADD || m_byStatus==GROUP_PARTY_JOIN)
					{
						if(g_pPartyBoardHash->GetData(g_pMainPlayer->m_dwPartyId))
						{
							SetRender(SPR_OBJ_GROUPWND_PBDESTROY1, FALSE);	
							SetRender(SPR_OBJ_GROUPWND_PBDESTROY2, TRUE);	
							SetRender(SPR_OBJ_GROUPWND_PBDESTROY3, FALSE);
							m_bPartyBtn[4] = TRUE;

							if(g_pMainPlayer->m_dwPartyId!=0)
							{
								CTWS_PARTY_BOARDDELETE pPacket;
								pPacket.dwPartyId = g_pMainPlayer->m_dwPartyId;
								g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
							}
							else
								DisplayMessageAdd(g_Message[ETC_MESSAGE1123].szMessage, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY); // "�Խ��ǿ� ��ϵǾ� �ִ� ��Ƽ�� �����ϴ�."
						}
						else
							byChk = __MOUSE_POINTER_DEFAULT__;
					}
					else
						byChk = __MOUSE_POINTER_DEFAULT__;					
				}	

#endif
				else
					byChk = __MOUSE_POINTER_DEFAULT__;
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_byGroupTab==GROUP_TAB_GUILD)
				{			
					if(m_byStatus==GROUP_GUILD_DEFAULT)
					{
						if(g_pMainPlayer->m_dwGuildId!=0)
						{
							if(g_pMainPlayer->m_byRank==1)
							{
								m_bGuildBtn[0] = TRUE;
								SetRender(SPR_OBJ_GUILD_ONOFF2, FALSE);
								SetRender(SPR_OBJ_GUILD_ONOFF3, TRUE);								
							}
							else
								byChk = __MOUSE_POINTER_DEFAULTCLICK__;
						}
						else
							byChk = __MOUSE_POINTER_DEFAULTCLICK__;
					}					
					else
						byChk = __MOUSE_POINTER_DEFAULTCLICK__;   
				}	
#ifdef PARTY_BOARD

				else if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_DEFAULT || m_byStatus==GROUP_PARTY_CREATEADD || m_byStatus==GROUP_PARTY_JOIN)
					{
						if(g_pPartyBoardHash->GetData(g_pMainPlayer->m_dwPartyId))
						{
							SetRender(SPR_OBJ_GROUPWND_PBDESTROY1, FALSE);	
							SetRender(SPR_OBJ_GROUPWND_PBDESTROY2, FALSE);	
							SetRender(SPR_OBJ_GROUPWND_PBDESTROY3, TRUE);
							m_bPartyBtn[5] = TRUE;							
						}
						else
							byChk = __MOUSE_POINTER_DEFAULTCLICK__;
					}
					else
						byChk = __MOUSE_POINTER_DEFAULTCLICK__;					
				}	
				
#endif
				else
					byChk = __MOUSE_POINTER_DEFAULTCLICK__;				
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(m_byGroupTab==GROUP_TAB_GUILD)
				{
					if(g_pMainPlayer->m_dwGuildId!=0)
					{
						if(g_pMainPlayer->m_byRank==1)
						{
							if(m_byStatus==GROUP_GUILD_DEFAULT)
								m_byRenderType = GROUP_RENDER_GUILDUSERONOFF;								
							else
								byChk = __MOUSE_POINTER_DEFAULT__;
						}
						else
							byChk = __MOUSE_POINTER_DEFAULT__;
					}
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}		
#ifdef PARTY_BOARD

				else if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_DEFAULT || m_byStatus==GROUP_PARTY_CREATEADD || m_byStatus==GROUP_PARTY_JOIN)
					{
						if(g_pPartyBoardHash->GetData(g_pMainPlayer->m_dwPartyId))
							m_byRenderType = GROUP_RENDER_PARTYBOARDDESTROY;
						else
							byChk = __MOUSE_POINTER_DEFAULT__;
					}
					else
						byChk = __MOUSE_POINTER_DEFAULT__;					
				}

#endif
				else
					byChk = __MOUSE_POINTER_DEFAULT__;
			}

			//��Ƽ ��ȭ �ý��� (����â 2005.01.26) -- ����ġ �й�� ������ ���� ��� ����    
			if(eMouseCheck==MOUSE_DOWN)					 // ���콺�� �������� ������ ��ȭ 
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus!=GROUP_PARTY_EXPITEMCONDITION1)
					{
						SetStatus(GROUP_PARTY_EXPITEMCONDITION1);
						byChk = __MOUSE_POINTER_BUTTONCLICK__;

						SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE);  //��Ƽ ��ȭ �ý��� (����â 2005. 02.14)

					}
					else if(m_byStatus==GROUP_PARTY_EXPITEMCONDITION1)
					{
						byChk = __MOUSE_POINTER_BUTTONCLICK__;
					}
				}
				else
					byChk = __MOUSE_POINTER_DEFAULT__;

			}
			else if(eMouseCheck==MOUSE_UP)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus!=GROUP_PARTY_EXPITEMCONDITION1)
					{
							SetStatus(GROUP_PARTY_EXPITEMCONDITION1);
							byChk = __MOUSE_POINTER_BUTTON__;
					}
				
				}
				else 
						byChk = __MOUSE_POINTER_DEFAULT__;

			} 

			else if(eMouseCheck==MOUSE_OVER)			// ���콺�� �����ܿ� �÷� �������� ������ ���� �� ���� ǥ�� 
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					m_byRenderType = GROUP_RENDER_EXPITEMCONDITION_MSG;
					byChk = __MOUSE_POINTER_BUTTON__;

				}
				else 
					byChk = __MOUSE_POINTER_DEFAULT__;
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;
	case 24:
		{
			BYTE byChk = 0;

			if(eMouseCheck==MOUSE_UP)
			{
				if(m_byGroupTab==GROUP_TAB_GUILD)
				{
					if(m_byStatus==GROUP_GUILD_SELECT)
					{		
						if(g_pMainPlayer->m_byRank==1)
						{
							m_bGuildBtn[6] = FALSE;
							SetRender(SPR_OBJ_GROUP_MASTER1, FALSE);
							SetRender(SPR_OBJ_GROUP_MASTER2, TRUE);
							SetRender(SPR_OBJ_GROUP_MASTER3, FALSE);							

							if(m_byCurPage==1 && m_nPartyUserIndex==0 && m_byUserType==GUILD_USER_ONLINE)
							{				
								DisplayMessageAdd(g_Message[ETC_MESSAGE800].szMessage, 0xffffff00, TRUE, DEF_CHAT_TYPE_GUILD); // "�ڱ� �ڽſ��Դ� ������ ������ �̾��Ҽ� �����ϴ�."
								break;
							}
							else if(m_byUserType==GUILD_USER_ONLINE)
							{
								BYTE byIndex = 0; 

								ListNode<GUILD_USER>* pGuildUserPos = m_pGuildUserPos;

								if(m_byCurPage==1)
								{
									pGuildUserPos	= g_pGuildUserHash->GetHead();
									byIndex			= 1; 
								}

								while(pGuildUserPos)
								{
									LP_GUILD_USER sGuildUser = pGuildUserPos->pData;
									
									if(!sGuildUser)
										continue;										

									if(m_nPartyUserIndex==byIndex)
									{
										CGuildMasterWnd* pGuildMasterWnd = CGuildMasterWnd::GetInstance();
										memset(pGuildMasterWnd->m_szName, 0, sizeof(pGuildMasterWnd->m_szName));
										pGuildMasterWnd->SetActive();										
										pGuildMasterWnd->m_dwUserIndex	= sGuildUser->dwUserIndex;
										__lstrcpyn(pGuildMasterWnd->m_szName, sGuildUser->szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);									
										CInterface::GetInstance()->SetWnd(GUILDMASTER_WND);
										break;
									}							
									pGuildUserPos = pGuildUserPos->pNext;
									byIndex++;										
								}
							}	
							else
							{
								BYTE byIndex = 0; 

								ListNode<GUILD_OFFLINE_USER>* pGuildOfflineUserPos = m_pGuildOfflineUserPos;
								
								while(pGuildOfflineUserPos)
								{
									LP_GUILD_OFFLINE_USER sGuildOfflineUser = pGuildOfflineUserPos->pData;
									
									if(!sGuildOfflineUser)
										continue;										

									if(m_nPartyUserIndex==byIndex)
									{
										CGuildMasterWnd* pGuildMasterWnd = CGuildMasterWnd::GetInstance();
										memset(pGuildMasterWnd->m_szName, 0, sizeof(pGuildMasterWnd->m_szName));
										pGuildMasterWnd->SetActive();										
										pGuildMasterWnd->m_dwUserIndex	= sGuildOfflineUser->dwUserIndex;
										__lstrcpyn(pGuildMasterWnd->m_szName, sGuildOfflineUser->szName, MAX_CHARACTER_NAME_REAL_LENGTH);									
										CInterface::GetInstance()->SetWnd(GUILDMASTER_WND);
										break;
									}							
									pGuildOfflineUserPos = pGuildOfflineUserPos->pNext;
									byIndex++;										
								}
							}							
						}
						else
							byChk = __MOUSE_POINTER_DEFAULT__;
					}
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}				
				else
					byChk = __MOUSE_POINTER_DEFAULT__;
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_byGroupTab==GROUP_TAB_GUILD)
				{			
					if(m_byStatus==GROUP_GUILD_SELECT)
					{
						if(g_pMainPlayer->m_byRank==1)
						{
							m_bGuildBtn[6] = TRUE;
							SetRender(SPR_OBJ_GROUP_MASTER2, FALSE);
							SetRender(SPR_OBJ_GROUP_MASTER3, TRUE);							
						}
						else
							byChk = __MOUSE_POINTER_DEFAULTCLICK__;
					}					
					else
						byChk = __MOUSE_POINTER_DEFAULTCLICK__;
				}				
				else
					byChk = __MOUSE_POINTER_DEFAULTCLICK__;				
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(m_byGroupTab==GROUP_TAB_GUILD)
				{
					if(m_byStatus==GROUP_GUILD_SELECT)
					{
						if(g_pMainPlayer->m_byRank==1)
							m_byRenderType = GROUP_RENDER_GUILDUSERMASTER;							
						else
							byChk = __MOUSE_POINTER_DEFAULT__;
					}
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}								
				else
					byChk = __MOUSE_POINTER_DEFAULT__;
			}

			//��Ƽ ��ȭ �ý��� (����â 2005.01.27) -- �˻��� ��Ƽ�� ���� ����    
			if(eMouseCheck==MOUSE_DOWN)					 // ���콺�� �������� ������ ��ȭ 
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
						SetStatus(GROUP_PARTY_PMATCHCONDITION1);
						byChk = __MOUSE_POINTER_BUTTONCLICK__;

						SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE);  //��Ƽ ��ȭ �ý��� (����â 2005.02.14)
				}
				else
					byChk = __MOUSE_POINTER_DEFAULT__;

			}
			else if(eMouseCheck==MOUSE_UP)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					SetStatus(GROUP_PARTY_PMATCHCONDITION1);
					byChk = __MOUSE_POINTER_BUTTON__;

					SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE);    //��Ƽ ��ȭ �ý��� (����â 2005.02.14)

				}
				else 
						byChk = __MOUSE_POINTER_DEFAULT__;

			} 

			if(eMouseCheck==MOUSE_OVER)			// ���콺�� �����ܿ� �÷� �������� ������ ���� �� ���� ǥ�� 
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					m_byRenderType = GROUP_RENDER_PMATCHCONDITION_MSG;
					byChk = __MOUSE_POINTER_BUTTON__;

				}
				else 
					byChk = __MOUSE_POINTER_DEFAULT__;
			} 
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;   
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19://�� ���� ���ý�(�ؽ�Ʈ)
		{
			BYTE byChk = 0;

			if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus!=GROUP_PARTY_PMATCHCONDITION1 && m_byStatus!=GROUP_PARTY_EXPITEMCONDITION1 
						&& !(m_byStatus>=GROUP_PARTY__CONDITIONLIST_EXP1 && m_byStatus<=GROUP_PARTY__CONDITIONLIST_ITEM4)
						&& !(m_byStatus>=GROUP_PARTY_PMATCHCONDITION_LEV1 && m_byStatus<=GROUP_PARTY_PMATCHCONDITION_PMAINCHO4)
						&& !(m_byStatus>=GROUP_PARTY__CONDITIONLIST_EXP5 && m_byStatus<=GROUP_PARTY_PMATCHCONDITION_PMAINCHO5) 
						&& m_byStatus!=GROUP_PARTY_PARTYUSERINFO2 && m_byStatus!=GROUP_PARTY_PARTYUSERINFO1) //��Ƽ��ȭ�ý��� (����â 2005.01.14)
					{
						if(g_pMainPlayer->m_dwPartyId!=0)
						{
							BYTE byIndex = nRt-12;
						
							if(g_pPartyUserHash->GetCount() >= byIndex)
							{							
								// ó�� //
								if(byIndex==m_nPartyUserIndex)
								{
									m_nPartyUserIndex = -1;
									SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE);

									// ���� ���� //
									SetStatus(GROUP_PARTY_JOIN);
								}
								else
								{
									m_nPartyUserIndex = nRt-12;
									SetRender(SPR_OBJ_GROUPWND_SELECT, TRUE);
									SetPosObjX(SPR_OBJ_GROUPWND_SELECT, 45);
									SetPosObjY(SPR_OBJ_GROUPWND_SELECT, float(97+m_nPartyUserIndex*35));

									// ���� ���� //
									SetStatus(GROUP_PARTY_SELECT);
								}
							}
						}
					}				
				}								
				else if(m_byGroupTab==GROUP_TAB_GUILD)
				{
					if(g_pMainPlayer->m_dwGuildId!=0)
					{										
						BYTE byIndex = nRt-12;										
						
						if(g_pMainPlayer->m_byRank==1)
						{
							if(m_byUserType==GUILD_USER_ONLINE)
							{
								if((DWORD)((m_byCurPage-1)*__GUILD_MAX_USER_DISPLAY__+ byIndex)<(g_pGuildUserHash->GetCount()+1))
								{
									// ó�� //
									if(byIndex==m_nPartyUserIndex)
									{
										m_nPartyUserIndex = -1;
										SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE);									
										SetStatus(GROUP_GUILD_DEFAULT);
									}
									else
									{
										m_nPartyUserIndex = nRt-12;
										SetRender(SPR_OBJ_GROUPWND_SELECT, TRUE);
										SetPosObjX(SPR_OBJ_GROUPWND_SELECT, 45);
										SetPosObjY(SPR_OBJ_GROUPWND_SELECT, float(97+m_nPartyUserIndex*35));
										// ���� ���� //
										SetStatus(GROUP_GUILD_SELECT);
									}
								}
								else
									byChk = __MOUSE_POINTER_DEFAULTCLICK__;
							}
							else
							{							
								if((DWORD)((m_byCurPage-1)*__GUILD_MAX_USER_DISPLAY__+ byIndex)<(g_pGuildOfflineHash->GetCount()))
								{									
									// ó�� //
									if(byIndex==m_nPartyUserIndex)
									{
										m_nPartyUserIndex = -1;
										SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE);									
										SetStatus(GROUP_GUILD_DEFAULT);
									}
									else
									{
										m_nPartyUserIndex = nRt-12;
										SetRender(SPR_OBJ_GROUPWND_SELECT, TRUE);
										SetPosObjX(SPR_OBJ_GROUPWND_SELECT, 45);
										SetPosObjY(SPR_OBJ_GROUPWND_SELECT, float(97+m_nPartyUserIndex*35));

										// ���� ���� //
										SetStatus(GROUP_GUILD_SELECT);
									}
								}
								else
									byChk = __MOUSE_POINTER_DEFAULTCLICK__;
							}
						}
						else
						{
							if((DWORD)((m_byCurPage-1)*__GUILD_MAX_USER_DISPLAY__+ byIndex)<(g_pGuildUserHash->GetCount()+1))
							{																												
								// ó�� //
								if(byIndex==m_nPartyUserIndex)
								{
									m_nPartyUserIndex = -1;
									SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE);									
									SetStatus(GROUP_GUILD_DEFAULT);
								}
								else
								{
									m_nPartyUserIndex = nRt-12;
									SetRender(SPR_OBJ_GROUPWND_SELECT, TRUE);
									SetPosObjX(SPR_OBJ_GROUPWND_SELECT, 45);
									SetPosObjY(SPR_OBJ_GROUPWND_SELECT, float(97+m_nPartyUserIndex*35));

									// ���� ���� //
									SetStatus(GROUP_GUILD_SELECT);
								}
							}
							else
								byChk = __MOUSE_POINTER_DEFAULTCLICK__;
						}					
					}
					else
						byChk = __MOUSE_POINTER_DEFAULTCLICK__;
				}
				else if(m_byGroupTab==GROUP_TAB_FRIEND)
				{
					if(m_byStatus==GROUP_FRIEND_DEFAULT || m_byStatus==GROUP_FRIEND_SELECT)
					{										
						BYTE byIndex = nRt-12;										
											
						if(m_byMessengerType==FRIEND_USER_ONLINE)
						{
							if((DWORD)((m_byCurPage-1)*__FRIEND_MAX_USER_DISPLAY__+ byIndex)<(g_pMessengerOnLineHash->GetCount()))
							{																				
								// ó�� //
								if(byIndex==m_nPartyUserIndex)
								{
									m_nPartyUserIndex = -1;
									SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE);									
									SetStatus(GROUP_FRIEND_DEFAULT);
								}
								else
								{
									m_nPartyUserIndex = nRt-12;
									SetRender(SPR_OBJ_GROUPWND_SELECT, TRUE);
									SetPosObjX(SPR_OBJ_GROUPWND_SELECT, 45);
									SetPosObjY(SPR_OBJ_GROUPWND_SELECT, float(97+m_nPartyUserIndex*35));

									// ���� ���� //
									SetStatus(GROUP_FRIEND_SELECT);
								}
							}
							else
								byChk = __MOUSE_POINTER_DEFAULTCLICK__;
						}
						else
						{							
							if((DWORD)((m_byCurPage-1)*__FRIEND_MAX_USER_DISPLAY__+ byIndex)<(g_pMessengerOffLineHash->GetCount()))
							{																				
								// ó�� //
								if(byIndex==m_nPartyUserIndex)
								{
									m_nPartyUserIndex = -1;
									SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE);									
									SetStatus(GROUP_FRIEND_DEFAULT);
								}
								else
								{
									m_nPartyUserIndex = nRt-12;
									SetRender(SPR_OBJ_GROUPWND_SELECT, TRUE);
									SetPosObjX(SPR_OBJ_GROUPWND_SELECT, 45);
									SetPosObjY(SPR_OBJ_GROUPWND_SELECT, float(97+m_nPartyUserIndex*35));

									// ���� ���� //
									SetStatus(GROUP_FRIEND_SELECT);
								}
							}
							else
								byChk = __MOUSE_POINTER_DEFAULTCLICK__;
						}
					}
				}
				else if(m_byGroupTab==GROUP_TAB_GUILDWAR)
				{
					/*
					if(m_byStatus==GROUP_GUILDWAR_DEFAULT || m_byStatus==GROUP_GUILDWAR_SELECT)
					{										
						BYTE byIndex = nRt-12;										
						
						if(g_pGuildWarHash->GetCount()>byIndex)					
						{
							// ó�� //
							if(byIndex==m_nPartyUserIndex)
							{
								m_nPartyUserIndex = -1;
								SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE);									
								SetStatus(GROUP_GUILDWAR_DEFAULT);
							}
							else
							{
								m_nPartyUserIndex = nRt-12;
								SetRender(SPR_OBJ_GROUPWND_SELECT, TRUE);
								SetPosObjX(SPR_OBJ_GROUPWND_SELECT, 45);
								SetPosObjY(SPR_OBJ_GROUPWND_SELECT, float(97+m_nPartyUserIndex*35));

								// ���� ���� //
								SetStatus(GROUP_GUILDWAR_SELECT);
							}
						}
						else							
							byChk = __MOUSE_POINTER_DEFAULTCLICK__;
					}
					*/
				}
				//��Ƽ��Ȳ (����â 2005.03.03)
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_PARTYUSERINFO2)
					{
						BYTE byIndex = nRt-12;
						int iRealSelect = 0;

						if((DWORD)((m_byCurPage-1)*__FRIEND_MAX_USER_DISPLAY__+ byIndex)<(m_dwPartyCount))
						{		
							for(int i = 0; i < __FRIEND_MAX_USER_DISPLAY__; i++)
							{
								iRealSelect = i + ((m_byCurPage-1)*__FRIEND_MAX_USER_DISPLAY__);
				
								if(byIndex == iRealSelect)
								{
									if(m_nPartyFriend[iRealSelect].bOnline == 0)
									{
										m_nPartyUserIndex = nRt-12;
										SetRender(SPR_OBJ_GROUPWND_SELECT, TRUE);
										SetPosObjX(SPR_OBJ_GROUPWND_SELECT, 45);
										SetPosObjY(SPR_OBJ_GROUPWND_SELECT, float(97+m_nPartyUserIndex*35));
										SetRender(SPR_OBJ_GROUPWND_MSG1, FALSE);	
										SetRender(SPR_OBJ_GROUPWND_MSG2, TRUE);	

									}
									else if(m_nPartyFriend[iRealSelect].bOnline == 3)
									{
										SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE);
										SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);	
										SetRender(SPR_OBJ_GROUPWND_MSG2, FALSE);	
									}
								}
								else if(byIndex == iRealSelect-8)
								{
									if(m_nPartyFriend[iRealSelect].bOnline == 0)
									{
										m_nPartyUserIndex = nRt-12;
										SetRender(SPR_OBJ_GROUPWND_SELECT, TRUE);
										SetPosObjX(SPR_OBJ_GROUPWND_SELECT, 45);
										SetPosObjY(SPR_OBJ_GROUPWND_SELECT, float(97+m_nPartyUserIndex*35));
										SetRender(SPR_OBJ_GROUPWND_MSG1, FALSE);	
										SetRender(SPR_OBJ_GROUPWND_MSG2, TRUE);	
									}
									else if(m_nPartyFriend[iRealSelect].bOnline == 3)
									{
										SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE);
										SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);	
										SetRender(SPR_OBJ_GROUPWND_MSG2, FALSE);
									}

								}
								else if(byIndex == iRealSelect-16)
								{
									if(m_nPartyFriend[iRealSelect].bOnline == 0)
									{
										m_nPartyUserIndex = nRt-12;
										SetRender(SPR_OBJ_GROUPWND_SELECT, TRUE);
										SetPosObjX(SPR_OBJ_GROUPWND_SELECT, 45);
										SetPosObjY(SPR_OBJ_GROUPWND_SELECT, float(97+m_nPartyUserIndex*35));
										SetRender(SPR_OBJ_GROUPWND_MSG1, FALSE);	
										SetRender(SPR_OBJ_GROUPWND_MSG2, TRUE);	
									}
									else if(m_nPartyFriend[iRealSelect].bOnline == 3)
									{
										SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE);
										SetRender(SPR_OBJ_GROUPWND_MSG1, TRUE);	
										SetRender(SPR_OBJ_GROUPWND_MSG2, FALSE);
									}

								}
							
						//	SetStatus(GROUP_PARTY_PARTYUSERINFO2);	
							}
						}
					}

				}
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(g_pMainPlayer->m_dwPartyId!=0)
					{
						BYTE byIndex = nRt-12;
						
						if(g_pPartyUserHash->GetCount() < byIndex)
							byChk = __MOUSE_POINTER_DEFAULT__;							
					}
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}
			
				else if(m_byGroupTab==GROUP_TAB_GUILD)
				{
					if(g_pMainPlayer->m_dwGuildId!=0)
					{
						BYTE byIndex = nRt-12;
						
						if(g_pMainPlayer->m_byRank==1)
						{
							if(m_byUserType==GUILD_USER_ONLINE)
							{
								if((DWORD)((m_byCurPage-1)*__GUILD_MAX_USER_DISPLAY__+ byIndex)>=(g_pGuildUserHash->GetCount()+1))
									byChk = __MOUSE_POINTER_DEFAULT__;								
							}
							else
							{							
								if((DWORD)((m_byCurPage-1)*__GUILD_MAX_USER_DISPLAY__+ byIndex)>=(g_pGuildOfflineHash->GetCount()+1))
									byChk = __MOUSE_POINTER_DEFAULT__;
							}
						}
						else
						{
							if((DWORD)((m_byCurPage-1)*__GUILD_MAX_USER_DISPLAY__+ byIndex)>=(g_pGuildUserHash->GetCount()+1))
								byChk = __MOUSE_POINTER_DEFAULT__;							
						}					
					}
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}
				else if(m_byGroupTab==GROUP_TAB_FRIEND)
				{
					if(m_byStatus==GROUP_FRIEND_DEFAULT || m_byStatus==GROUP_FRIEND_SELECT)
					{
						BYTE byIndex = nRt-12;
											
						if(m_byMessengerType==FRIEND_USER_ONLINE)
						{
							if((DWORD)((m_byCurPage-1)*__FRIEND_MAX_USER_DISPLAY__+ byIndex)>=(g_pMessengerOnLineHash->GetCount()))
								byChk = __MOUSE_POINTER_DEFAULT__;;
						}
						else
						{							
							if((DWORD)((m_byCurPage-1)*__FRIEND_MAX_USER_DISPLAY__+ byIndex)>=(g_pMessengerOffLineHash->GetCount()))
								byChk = __MOUSE_POINTER_DEFAULT__;;
						}							
					}
					else
						CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_DEFAULT__);			
				}
				else if(m_byGroupTab==GROUP_TAB_GUILDWAR)
				{
					if(m_byStatus==GROUP_GUILDWAR_DEFAULT || m_byStatus==GROUP_GUILDWAR_SELECT)
					{
						BYTE byIndex = nRt-12;

						if(g_pGuildWarHash->GetCount()<=byIndex)
							byChk = __MOUSE_POINTER_DEFAULT__;
					}
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}
				//��Ƽ��Ȳ (����â 2005.03.03)
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_PARTYUSERINFO2)
					{
						int i;

						for(i=0;i<20;i++)
						{
							if(m_nPartyFriend[i].bOnline == 0)
							{
								BYTE byIndex = nRt-12;
						
								if((DWORD)((m_byCurPage-1)*__FRIEND_MAX_USER_DISPLAY__+ byIndex)>=(m_dwPartyCount))
									byChk = __MOUSE_POINTER_DEFAULT__;;						
							}
							else
								byChk = __MOUSE_POINTER_DEFAULT__;				
						}
					}
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}			
		break;
	case 20://������ ������
		{
			BYTE byChk = 0;

			if(eMouseCheck==MOUSE_UP)
			{
				if(m_byGroupTab==GROUP_TAB_GUILD)
				{
					if(g_pMainPlayer->m_dwGuildId!=0)
					{										
						SetRender(SPR_OBJ_GUILD_PAGEPREV1, FALSE);
						SetRender(SPR_OBJ_GUILD_PAGEPREV2, TRUE);
						SetRender(SPR_OBJ_GUILD_PAGEPREV3, FALSE);
						SetPrevNext(__GUILD_PAGE_PREV__);
					}
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}
				else if(m_byGroupTab==GROUP_TAB_FRIEND)
				{					
					SetRender(SPR_OBJ_GUILD_PAGEPREV1, FALSE);
					SetRender(SPR_OBJ_GUILD_PAGEPREV2, TRUE);
					SetRender(SPR_OBJ_GUILD_PAGEPREV3, FALSE);
					SetPrevNext(__FRIEND_PAGE_PREV__);
				}
				else if(m_byGroupTab==GROUP_TAB_PARTY)        //��Ƽ �ý���(����â 2005.03.02)
				{
					if(m_byStatus==GROUP_PARTY_PARTYUSERINFO2)
					{
						SetRender(SPR_OBJ_GUILD_PAGEPREV1, FALSE);
						SetRender(SPR_OBJ_GUILD_PAGEPREV2, TRUE);
						SetRender(SPR_OBJ_GUILD_PAGEPREV3, FALSE);
				//		SetPrevNext(__FRIEND_PAGE_PREV__);
						SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE);

						m_byCurPage--;
						if(m_byCurPage<1)
							m_byCurPage = 1;
					}
				}
				else if(m_byGroupTab==GROUP_TAB_GUILDWAR)
				{
					if( m_byStatus == GROUP_GUILDWAR_RANK )
					{
						SetRender(SPR_OBJ_GUILD_PAGEPREV1, FALSE);
						SetRender(SPR_OBJ_GUILD_PAGEPREV2, TRUE);
						SetRender(SPR_OBJ_GUILD_PAGEPREV3, FALSE);

						m_nGuildWarRankListPage--;
						if(m_nGuildWarRankListPage < 1)
							m_nGuildWarRankListPage = 1;
						TEAM_MATCH_PACKET TeamMatch;
						TeamMatch.Body.PageNum.iPageNumber = m_nGuildWarRankListPage;
						Cmd_CTWS_GuildWarPacket(TeamMatch, CTWS_TM_LADDER_LIST);
//						TempSetRandomData();//�ӽ�
					}
					else if(m_byStatus == GROUP_GUILDWAR_LIST)
					{
						SetRender(SPR_OBJ_GUILD_PAGEPREV1, FALSE);
						SetRender(SPR_OBJ_GUILD_PAGEPREV2, TRUE);
						SetRender(SPR_OBJ_GUILD_PAGEPREV3, FALSE);

						m_nGuildWarListPage--;
						if(m_nGuildWarListPage < 1)
							m_nGuildWarListPage = 1;
						TEAM_MATCH_PACKET TeamMatch;
						TeamMatch.Body.PageNum.iPageNumber = m_nGuildWarListPage;
						Cmd_CTWS_GuildWarPacket(TeamMatch, CTWS_TM_GUILD_WAR_LIST);
//						TempSetRandomData();//�ӽ�
					}
				}
				else
					byChk = __MOUSE_POINTER_DEFAULT__;
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_byGroupTab==GROUP_TAB_GUILD)
				{
					if(g_pMainPlayer->m_dwGuildId!=0)
					{										
						SetRender(SPR_OBJ_GUILD_PAGEPREV1, FALSE);
						SetRender(SPR_OBJ_GUILD_PAGEPREV2, FALSE);
						SetRender(SPR_OBJ_GUILD_PAGEPREV3, TRUE);						
					}
					else
						byChk = __MOUSE_POINTER_DEFAULTCLICK__;
				}
				else if(m_byGroupTab==GROUP_TAB_FRIEND)
				{					
					SetRender(SPR_OBJ_GUILD_PAGEPREV1, FALSE);
					SetRender(SPR_OBJ_GUILD_PAGEPREV2, FALSE);
					SetRender(SPR_OBJ_GUILD_PAGEPREV3, TRUE);
				}
				else if(m_byGroupTab==GROUP_TAB_PARTY)        //��Ƽ �ý���(����â 2005.03.02)
				{
					if(m_byStatus==GROUP_PARTY_PARTYUSERINFO2)
					{
						SetRender(SPR_OBJ_GUILD_PAGEPREV1, FALSE);
						SetRender(SPR_OBJ_GUILD_PAGEPREV2, FALSE);
						SetRender(SPR_OBJ_GUILD_PAGEPREV3, TRUE);				
					}
				}
				else if(m_byGroupTab==GROUP_TAB_GUILDWAR)
				{
					if(GROUP_GUILDWAR_RANK == m_byStatus || GROUP_GUILDWAR_LIST == m_byStatus)
					{
						SetRender(SPR_OBJ_GUILD_PAGEPREV1, FALSE);
						SetRender(SPR_OBJ_GUILD_PAGEPREV2, FALSE);
						SetRender(SPR_OBJ_GUILD_PAGEPREV3, TRUE);
					}
				}
				else
					byChk = __MOUSE_POINTER_DEFAULTCLICK__;				
			}			
			else if(eMouseCheck==MOUSE_OVER)
			{	
				if(m_byGroupTab==GROUP_TAB_GUILD)
				{
					if(g_pMainPlayer->m_dwGuildId)
					{
						SetRender(SPR_OBJ_GUILD_PAGEPREV1, FALSE);
						SetRender(SPR_OBJ_GUILD_PAGEPREV2, FALSE);
						SetRender(SPR_OBJ_GUILD_PAGEPREV3, TRUE);
					}
					else			
						byChk = __MOUSE_POINTER_DEFAULT__;
				}
				else if(m_byGroupTab==GROUP_TAB_FRIEND)
				{					
					SetRender(SPR_OBJ_GUILD_PAGEPREV1, FALSE);
					SetRender(SPR_OBJ_GUILD_PAGEPREV2, TRUE);
					SetRender(SPR_OBJ_GUILD_PAGEPREV3, FALSE);					
				}
				else if(m_byGroupTab==GROUP_TAB_PARTY)        //��Ƽ �ý���(����â 2005.03.02)
				{
					if(m_byStatus==GROUP_PARTY_PARTYUSERINFO2)
					{
						SetRender(SPR_OBJ_GUILD_PAGEPREV1, FALSE);
						SetRender(SPR_OBJ_GUILD_PAGEPREV2, TRUE);
						SetRender(SPR_OBJ_GUILD_PAGEPREV3, FALSE);
					}
				}
				else if(m_byGroupTab==GROUP_TAB_GUILDWAR)
				{
					if(GROUP_GUILDWAR_RANK == m_byStatus || GROUP_GUILDWAR_LIST == m_byStatus)
					{
						SetRender(SPR_OBJ_GUILD_PAGEPREV1, FALSE);
						SetRender(SPR_OBJ_GUILD_PAGEPREV2, TRUE);
						SetRender(SPR_OBJ_GUILD_PAGEPREV3, FALSE);
					}
				}
				else			
					byChk = __MOUSE_POINTER_DEFAULT__;
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;
	case 21://������ ��������
		{
			BYTE byChk = 0;

			if(eMouseCheck==MOUSE_UP)
			{
				if(m_byGroupTab==GROUP_TAB_GUILD)
				{
					if(g_pMainPlayer->m_dwGuildId!=0)
					{										
						SetRender(SPR_OBJ_GUILD_PAGENEXT1, FALSE);
						SetRender(SPR_OBJ_GUILD_PAGENEXT2, TRUE);
						SetRender(SPR_OBJ_GUILD_PAGENEXT3, FALSE);
						SetPrevNext(__GUILD_PAGE_NEXT__);
					}
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}
				else if(m_byGroupTab==GROUP_TAB_FRIEND)
				{					
					SetRender(SPR_OBJ_GUILD_PAGENEXT1, FALSE);
					SetRender(SPR_OBJ_GUILD_PAGENEXT2, TRUE);
					SetRender(SPR_OBJ_GUILD_PAGENEXT3, FALSE);
					SetPrevNext(__FRIEND_PAGE_NEXT__);					
				}
				else if(m_byGroupTab==GROUP_TAB_PARTY)        //��Ƽ �ý���(����â 2005.03.02)
				{
					if(m_byStatus==GROUP_PARTY_PARTYUSERINFO2)
					{
						SetRender(SPR_OBJ_GUILD_PAGENEXT1, FALSE);
						SetRender(SPR_OBJ_GUILD_PAGENEXT2, TRUE);
						SetRender(SPR_OBJ_GUILD_PAGENEXT3, FALSE);
				//		SetPrevNext(__FRIEND_PAGE_NEXT__);
						SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE);
						
				
						m_byCurPage++;

						if(m_byCurPage>PARTY_FRIEND_MAX_PAGE)
							m_byCurPage = PARTY_FRIEND_MAX_PAGE;				
					}
				}
				else if(m_byGroupTab==GROUP_TAB_GUILDWAR)
				{
					if( m_byStatus == GROUP_GUILDWAR_RANK )
					{
						SetRender(SPR_OBJ_GUILD_PAGENEXT1, FALSE);
						SetRender(SPR_OBJ_GUILD_PAGENEXT2, TRUE);
						SetRender(SPR_OBJ_GUILD_PAGENEXT3, FALSE);

						m_nGuildWarRankListPage++;
						if(m_nGuildWarRankListPage > m_nGuildWarRankListMaxPage)
							m_nGuildWarRankListPage = m_nGuildWarRankListMaxPage;
						TEAM_MATCH_PACKET TeamMatch;
						TeamMatch.Body.PageNum.iPageNumber = m_nGuildWarRankListPage;
						Cmd_CTWS_GuildWarPacket(TeamMatch, CTWS_TM_LADDER_LIST);
//						TempSetRandomData();//�ӽ�
					}
					else if(m_byStatus == GROUP_GUILDWAR_LIST)
					{
						SetRender(SPR_OBJ_GUILD_PAGENEXT1, FALSE);
						SetRender(SPR_OBJ_GUILD_PAGENEXT2, TRUE);
						SetRender(SPR_OBJ_GUILD_PAGENEXT3, FALSE);

						m_nGuildWarListPage++;
						if(m_nGuildWarListPage > m_nGuildWarListMaxPage)
							m_nGuildWarListPage = m_nGuildWarListMaxPage;
						TEAM_MATCH_PACKET TeamMatch;
						TeamMatch.Body.PageNum.iPageNumber = m_nGuildWarListPage;
						Cmd_CTWS_GuildWarPacket(TeamMatch, CTWS_TM_GUILD_WAR_LIST);
//						TempSetRandomData();//�ӽ�
					}
				}
				else
					byChk = __MOUSE_POINTER_DEFAULT__;
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_byGroupTab==GROUP_TAB_GUILD)
				{
					if(g_pMainPlayer->m_dwGuildId!=0)
					{										
						SetRender(SPR_OBJ_GUILD_PAGENEXT1, FALSE);
						SetRender(SPR_OBJ_GUILD_PAGENEXT2, FALSE);
						SetRender(SPR_OBJ_GUILD_PAGENEXT3, TRUE);
					}
					else
						byChk = __MOUSE_POINTER_DEFAULTCLICK__;
				}
				else if(m_byGroupTab==GROUP_TAB_FRIEND)
				{					
					SetRender(SPR_OBJ_GUILD_PAGENEXT1, FALSE);
					SetRender(SPR_OBJ_GUILD_PAGENEXT2, FALSE);
					SetRender(SPR_OBJ_GUILD_PAGENEXT3, TRUE);
				}
				else if(m_byGroupTab==GROUP_TAB_PARTY)        //��Ƽ �ý���(����â 2005.03.02)
				{
					if(m_byStatus==GROUP_PARTY_PARTYUSERINFO2)
					{
						SetRender(SPR_OBJ_GUILD_PAGENEXT1, FALSE);
						SetRender(SPR_OBJ_GUILD_PAGENEXT2, FALSE);
						SetRender(SPR_OBJ_GUILD_PAGENEXT3, TRUE);
					}
				}
				else if(m_byGroupTab==GROUP_TAB_GUILDWAR)
				{
					if(GROUP_GUILDWAR_RANK == m_byStatus || GROUP_GUILDWAR_LIST == m_byStatus)
					{
						SetRender(SPR_OBJ_GUILD_PAGENEXT1, FALSE);
						SetRender(SPR_OBJ_GUILD_PAGENEXT2, FALSE);
						SetRender(SPR_OBJ_GUILD_PAGENEXT3, TRUE);
					}
				}
				else
					byChk = __MOUSE_POINTER_DEFAULTCLICK__;
			}					
			else if(eMouseCheck==MOUSE_OVER)
			{	
				if(m_byGroupTab==GROUP_TAB_GUILD)
				{
					if(g_pMainPlayer->m_dwGuildId)
					{						
						SetRender(SPR_OBJ_GUILD_PAGENEXT1, FALSE);
						SetRender(SPR_OBJ_GUILD_PAGENEXT2, TRUE);
						SetRender(SPR_OBJ_GUILD_PAGENEXT3, FALSE);
					}
					else			
						byChk = __MOUSE_POINTER_DEFAULT__;
				}
				else if(m_byGroupTab==GROUP_TAB_FRIEND)
				{					
					SetRender(SPR_OBJ_GUILD_PAGENEXT1, FALSE);
					SetRender(SPR_OBJ_GUILD_PAGENEXT2, TRUE);
					SetRender(SPR_OBJ_GUILD_PAGENEXT3, FALSE);
				}
				else if(m_byGroupTab==GROUP_TAB_PARTY)        //��Ƽ �ý���(����â 2005.03.02)
				{
					if(m_byStatus==GROUP_PARTY_PARTYUSERINFO2)
					{
						SetRender(SPR_OBJ_GUILD_PAGENEXT1, FALSE);
						SetRender(SPR_OBJ_GUILD_PAGENEXT2, TRUE);
						SetRender(SPR_OBJ_GUILD_PAGENEXT3, FALSE);
					}
				}
				else if(m_byGroupTab==GROUP_TAB_GUILDWAR)
				{
					if(GROUP_GUILDWAR_RANK == m_byStatus || GROUP_GUILDWAR_LIST == m_byStatus)
					{
						SetRender(SPR_OBJ_GUILD_PAGENEXT1, FALSE);
						SetRender(SPR_OBJ_GUILD_PAGENEXT2, TRUE);
						SetRender(SPR_OBJ_GUILD_PAGENEXT3, FALSE);
					}
				}
				else
					byChk = __MOUSE_POINTER_DEFAULT__;
			}
		}
		break;	
		
		//��Ƽ ��ȭ �ý��� (����â 2005.01.26)  -- 	GROUP_PARTY__CONDITIONLIST_EXP ��Ӵٿ� ��ư 
		case 26:
		{
			BYTE byChk = 0;

			if(eMouseCheck==MOUSE_UP)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY__CONDITIONLIST_EXP1 || m_byStatus==GROUP_PARTY__CONDITIONLIST_ITEM5 || m_byStatus==GROUP_PARTY__CONDITIONLIST_EXP5
						|| (m_byStatus>=GROUP_PARTY__CONDITIONLIST_ITEM1 && GROUP_PARTY__CONDITIONLIST_ITEM4 >= m_byStatus))
					{
						SetStatus(GROUP_PARTY__CONDITIONLIST_EXP2);
						byChk = __MOUSE_POINTER_BUTTON__;
					}
					else if(m_byStatus==GROUP_PARTY__CONDITIONLIST_EXP3)
					{
						SetStatus(GROUP_PARTY_EXPITEMCONDITION1);
						byChk = __MOUSE_POINTER_BUTTON__;
					}

				}
				else
					byChk = __MOUSE_POINTER_DEFAULT__;
				
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(g_pMainPlayer->m_bPartyLeader==1 && g_pMainPlayer->m_dwPartyId != 0)
					{
						if(m_byStatus==GROUP_PARTY__CONDITIONLIST_EXP4 || m_byStatus==GROUP_PARTY__CONDITIONLIST_ITEM5 || m_byStatus==GROUP_PARTY__CONDITIONLIST_EXP5
							|| (m_byStatus>=GROUP_PARTY__CONDITIONLIST_ITEM1 && GROUP_PARTY__CONDITIONLIST_ITEM4 >= m_byStatus))
						{
							SetStatus(GROUP_PARTY__CONDITIONLIST_EXP1);
							byChk = __MOUSE_POINTER_BUTTONCLICK__;
						}
						else if(m_byStatus==GROUP_PARTY__CONDITIONLIST_EXP2)
						{
							SetStatus(GROUP_PARTY__CONDITIONLIST_EXP3);
							byChk = __MOUSE_POINTER_BUTTONCLICK__;
						}
					}


				}
				else
					byChk = __MOUSE_POINTER_DEFAULT__;

				}			
			else if(eMouseCheck==MOUSE_OVER)
			{	
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_EXPITEMCONDITION1 || m_byStatus==GROUP_PARTY__CONDITIONLIST_ITEM5 || m_byStatus==GROUP_PARTY__CONDITIONLIST_EXP5
						|| (m_byStatus>=GROUP_PARTY__CONDITIONLIST_ITEM1 && GROUP_PARTY__CONDITIONLIST_ITEM4 >= m_byStatus))
					{
						SetStatus(GROUP_PARTY__CONDITIONLIST_EXP4);
						byChk = __MOUSE_POINTER_BUTTON__;
					}
					else if(m_byStatus==GROUP_PARTY__CONDITIONLIST_EXP2)
					{
						byChk = __MOUSE_POINTER_BUTTON__;
					}
				}
				else 
					byChk = __MOUSE_POINTER_DEFAULT__;
				
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.27) -- GROUP_PARTY__CONDITIONLIST_ITEM ��Ӵٿ� ��ư 
		case 27:
		{
			BYTE byChk = 0;

			if(eMouseCheck==MOUSE_UP)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY__CONDITIONLIST_EXP5 || m_byStatus==GROUP_PARTY__CONDITIONLIST_ITEM1 || m_byStatus==GROUP_PARTY__CONDITIONLIST_ITEM5
						|| (m_byStatus>=GROUP_PARTY__CONDITIONLIST_EXP1 && GROUP_PARTY__CONDITIONLIST_EXP4 >= m_byStatus))
					{
						SetStatus(GROUP_PARTY__CONDITIONLIST_ITEM2);
						byChk = __MOUSE_POINTER_BUTTON__;
					}
					else if(m_byStatus==GROUP_PARTY__CONDITIONLIST_ITEM3)
					{
						SetStatus(GROUP_PARTY_EXPITEMCONDITION1);
						byChk = __MOUSE_POINTER_BUTTON__;
					}

				}
				else
					byChk = __MOUSE_POINTER_DEFAULT__;
				
			} 
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)				
				{
					if(g_pMainPlayer->m_bPartyLeader==1 && g_pMainPlayer->m_dwPartyId != 0)
					{
						if(m_byStatus==GROUP_PARTY__CONDITIONLIST_EXP5 || m_byStatus==GROUP_PARTY__CONDITIONLIST_ITEM4 || m_byStatus==GROUP_PARTY__CONDITIONLIST_ITEM5
							|| (m_byStatus>=GROUP_PARTY__CONDITIONLIST_EXP1 && GROUP_PARTY__CONDITIONLIST_EXP4 >= m_byStatus))
						{
							SetStatus(GROUP_PARTY__CONDITIONLIST_ITEM1);
							byChk = __MOUSE_POINTER_BUTTONCLICK__;
						}
						else if(m_byStatus==GROUP_PARTY__CONDITIONLIST_ITEM2)
						{
							SetStatus(GROUP_PARTY__CONDITIONLIST_ITEM3);
							byChk = __MOUSE_POINTER_BUTTONCLICK__;
						}
					}

				}
				else
					byChk = __MOUSE_POINTER_DEFAULT__;

				}		
			else if(eMouseCheck==MOUSE_OVER)
			{	
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY__CONDITIONLIST_EXP5 || m_byStatus==GROUP_PARTY_EXPITEMCONDITION1 || m_byStatus==GROUP_PARTY__CONDITIONLIST_ITEM5
						|| (m_byStatus>=GROUP_PARTY__CONDITIONLIST_EXP1 && GROUP_PARTY__CONDITIONLIST_EXP4 >= m_byStatus))
					{
						SetStatus(GROUP_PARTY__CONDITIONLIST_ITEM4);
						byChk = __MOUSE_POINTER_BUTTON__;
					}
					else if(m_byStatus==GROUP_PARTY__CONDITIONLIST_EXP2)
					{
						byChk = __MOUSE_POINTER_BUTTON__;
					}
				}
				else 
					byChk = __MOUSE_POINTER_DEFAULT__;
				
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;
		
		//��Ƽ ��ȭ �ý��� (����â 2005.01.26)  
		case 28:								//����й� 
		{
			BYTE byChk = 0;
		
			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(eMouseCheck==MOUSE_DOWN)
				{ 
					if(m_byStatus==GROUP_PARTY__CONDITIONLIST_EXP1 || m_byStatus==GROUP_PARTY__CONDITIONLIST_EXP2)
					{
						m_nExpDistrib = PARTY_CONFIG_EXP_SHARE;
						SetStatus(GROUP_PARTY__CONDITIONLIST_EXP5);
					}
				}
				else if(eMouseCheck==MOUSE_OVER)
				{
			//		m_byColor = 1;
					byChk = __MOUSE_POINTER_BUTTON__;
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.26) 
		case 29:                               //������ �й� 
		{
			BYTE byChk = 0;

			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(eMouseCheck==MOUSE_DOWN)
				{
					if(m_byStatus==GROUP_PARTY__CONDITIONLIST_EXP1 || m_byStatus==GROUP_PARTY__CONDITIONLIST_EXP2)
					{
						m_nExpDistrib = PARTY_CONFIG_EXP_LEVEL;
						SetStatus(GROUP_PARTY__CONDITIONLIST_EXP5);
					}
				}
				else if(eMouseCheck==MOUSE_OVER)
				{
			//		m_byColor = 2;
					byChk = __MOUSE_POINTER_BUTTON__;
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.26) 
		case 30:							//���ʽ� ����ġ�� �й� 
		{
			BYTE byChk = 0;
			
			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(eMouseCheck==MOUSE_DOWN)
				{
					if(m_byStatus==GROUP_PARTY__CONDITIONLIST_EXP1 || m_byStatus==GROUP_PARTY__CONDITIONLIST_EXP2)
					{
						m_nExpDistrib = PARTY_CONFIG_EXP_KILLER;
						SetStatus(GROUP_PARTY__CONDITIONLIST_EXP5);
					}
				}
				else if(eMouseCheck==MOUSE_OVER)
				{
			//		m_byColor = 3;
					byChk = __MOUSE_POINTER_BUTTON__;
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.26) 
		case 31:							//������ �켱  
		{
			BYTE byChk = 0;
			
			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(eMouseCheck==MOUSE_DOWN)
				{
					if(m_byStatus==GROUP_PARTY__CONDITIONLIST_ITEM1 || m_byStatus==GROUP_PARTY__CONDITIONLIST_ITEM2)
					{
						m_nItemRouting = PARTY_CONFIG_ITEM_FIRST;
						SetStatus(GROUP_PARTY__CONDITIONLIST_ITEM5);
					}
				}
				else if(eMouseCheck==MOUSE_OVER)
				{
			//		m_byColor = 3;
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.26) 
		case 32:							//������ Ÿ���� �켱 
		{
			BYTE byChk = 0;
			
			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(eMouseCheck==MOUSE_DOWN)
				{
					if(m_byStatus==GROUP_PARTY__CONDITIONLIST_ITEM1 || m_byStatus==GROUP_PARTY__CONDITIONLIST_ITEM2)
					{
						m_nItemRouting = PARTY_CONFIG_ITEM_KILL;
						SetStatus(GROUP_PARTY__CONDITIONLIST_ITEM5);
					}
				}
				else if(eMouseCheck==MOUSE_OVER)
				{
			//		m_byColor = 3;
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.26) 
		case 33:							//���� ���� 
		{
			BYTE byChk = 0;
			
			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(eMouseCheck==MOUSE_DOWN)
				{
					if(m_byStatus==GROUP_PARTY__CONDITIONLIST_ITEM1 || m_byStatus==GROUP_PARTY__CONDITIONLIST_ITEM2)
					{
						m_nItemRouting = PARTY_CONFIG_ITEM_RANDOM;
						SetStatus(GROUP_PARTY__CONDITIONLIST_ITEM5);
					}
				}
				else if(eMouseCheck==MOUSE_OVER)
				{
			//		m_byColor = 3;
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.26) 
		case 34:									//EXPITEMCONDITI_OK ��ư 
		{
			BYTE byChk = 0;

			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(m_byStatus==GROUP_PARTY_EXPITEMCONDITION1 || (m_byStatus>=GROUP_PARTY__CONDITIONLIST_EXP1 && GROUP_PARTY__CONDITIONLIST_ITEM4<=m_byStatus) 
					|| m_byStatus==GROUP_PARTY__CONDITIONLIST_EXP5 || m_byStatus==GROUP_PARTY__CONDITIONLIST_ITEM5)
				{
					if(eMouseCheck==MOUSE_DOWN)
					{
					
					}
					else if(eMouseCheck==MOUSE_UP)
					{
						if(g_pMainPlayer->m_bPartyLeader==1 && g_pMainPlayer->m_dwPartyId != 0)
						{
							if(m_nItemRouting>0 && m_nExpDistrib>0)
							{
								CTWS_PARTY_CONFIG pPacket;
								pPacket.pConfig.item = m_nItemRouting;
								pPacket.pConfig.exp = m_nExpDistrib;
								g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);							
							}
						}
					}
					else if(eMouseCheck==MOUSE_OVER)
					{
						if(g_pMainPlayer->m_bPartyLeader==1 && g_pMainPlayer->m_dwPartyId != 0)
						{
		//					SetStatus(GROUP_PARTY__EXPITEMCONDITI_OK);
							byChk = __MOUSE_POINTER_BUTTON__;
						}
					}
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.26) 
		case 35:									//EXPITEMCONDITI_CANCEL ��ư 
		{
			BYTE byChk = 0;

			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(m_byStatus==GROUP_PARTY_EXPITEMCONDITION1 || (m_byStatus>=GROUP_PARTY__CONDITIONLIST_EXP1 && GROUP_PARTY__CONDITIONLIST_ITEM4<=m_byStatus) 
					|| m_byStatus==GROUP_PARTY__CONDITIONLIST_EXP5 || m_byStatus==GROUP_PARTY__CONDITIONLIST_ITEM5)
				{
					if(eMouseCheck==MOUSE_DOWN)
					{
				
					}
					else if(eMouseCheck==MOUSE_UP)
					{						
						SetStatus(GROUP_PARTY_JOIN);
					}
					else if(eMouseCheck==MOUSE_OVER)
					{
						byChk = __MOUSE_POINTER_BUTTON__;
					}
				}

			}

			SetMouseCheck(nRt, eMouseCheck, byChk);

		}
		break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.26) 
		case 36:										//GROUP_PARTY_PMATCHCONDITION1 Ŭ���� ��Ӵٿ� GROUP_PARTY_PMATCHCONDITION_LEV �ڽ� 
		{
			BYTE byChk = 0;
		
			if(m_byStatus!=GROUP_PARTY_EXPITEMCONDITION1 && !(m_byStatus>=GROUP_PARTY__CONDITIONLIST_EXP1 && m_byStatus<=GROUP_PARTY__CONDITIONLIST_ITEM4)
				&& m_byStatus!=GROUP_PARTY__CONDITIONLIST_EXP5 && m_byStatus!=GROUP_PARTY__CONDITIONLIST_ITEM5 
				&& !(m_byStatus>=GROUP_PARTY_DEFAULT &&  m_byStatus<=GROUP_PARTY_SELECT) && m_byStatus!=GROUP_PARTY_PARTYUSERINFO1 && m_byStatus!=GROUP_PARTY_PARTYUSERINFO2) // ��Ƽ ��ȭ �ý��� �߰� - if���� �߰�  (����â 2005.02.18)
			{
			if(eMouseCheck==MOUSE_UP)        
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_LEV1 || (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_EXP5 && GROUP_PARTY_PMATCHCONDITION_PMAINCHO5 >= m_byStatus) 
						|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_LEV5 || (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_EXP1 && GROUP_PARTY_PMATCHCONDITION_PMAINCHO4 >= m_byStatus))
					{
						if( (m_byPMatchStatus != 1 && g_pMainPlayer->m_dwPartyId == 0) || (m_byPMatchStatus != 1 &&g_pMainPlayer->m_bPartyLeader==1))
						{
							SetStatus(GROUP_PARTY_PMATCHCONDITION_LEV2);
							byChk = __MOUSE_POINTER_BUTTON__;
						}
					}
					else if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_LEV3 || m_byPMatchStatus == 0)
					{
						SetStatus(GROUP_PARTY_PMATCHCONDITION1);
						byChk = __MOUSE_POINTER_BUTTON__;
					}
				}
				else
					byChk = __MOUSE_POINTER_DEFAULT__;
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_LEV4 || (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_EXP5 && GROUP_PARTY_PMATCHCONDITION_PMAINCHO5 >= m_byStatus) 
						|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_LEV5 || (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_EXP1 && GROUP_PARTY_PMATCHCONDITION_PMAINCHO4 >= m_byStatus))
					{
						if( (m_byPMatchStatus != 1 && g_pMainPlayer->m_dwPartyId == 0) || (m_byPMatchStatus != 1 &&g_pMainPlayer->m_bPartyLeader==1))
						{
							SetStatus(GROUP_PARTY_PMATCHCONDITION_LEV1);
							byChk = __MOUSE_POINTER_BUTTONCLICK__;
						}
					}
					else if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_LEV2)
					{
						m_byBackBoard	 = 0;
						SetStatus(GROUP_PARTY_PMATCHCONDITION_LEV3);
						byChk = __MOUSE_POINTER_BUTTONCLICK__;
					}
				}
				else
					byChk = __MOUSE_POINTER_DEFAULT__;

				}			
			else if(eMouseCheck==MOUSE_OVER)
			{	
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_PMATCHCONDITION1 || (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_EXP5 && GROUP_PARTY_PMATCHCONDITION_PMAINCHO5 >= m_byStatus) 
						|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_LEV5 || (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_EXP1 && GROUP_PARTY_PMATCHCONDITION_PMAINCHO4 >= m_byStatus))
					{
						if((m_byPMatchStatus != 1 && g_pMainPlayer->m_dwPartyId == 0) || (m_byPMatchStatus != 1 &&g_pMainPlayer->m_bPartyLeader==1))
						{
							SetStatus(GROUP_PARTY_PMATCHCONDITION_LEV4);
							byChk = __MOUSE_POINTER_BUTTON__;
						}
					}
					else if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_LEV2)
					{
						byChk = __MOUSE_POINTER_BUTTON__;
					}
				}
				else 
					byChk = __MOUSE_POINTER_DEFAULT__;
				}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		}
		break;
		//��Ƽ ��ȭ �ý��� (����â 2005.01.28) 
		case 37:										//GROUP_PARTY_PMATCHCONDITION1 Ŭ���� ��Ӵٿ� GROUP_PARTY_PMATCHCONDITION_EXP �ڽ� 
		{
			BYTE byChk = 0;

			if(m_byStatus!=GROUP_PARTY_EXPITEMCONDITION1 && !(m_byStatus>=GROUP_PARTY__CONDITIONLIST_EXP1 && m_byStatus<=GROUP_PARTY__CONDITIONLIST_ITEM4)
				&& m_byStatus!=GROUP_PARTY__CONDITIONLIST_EXP5 && m_byStatus!=GROUP_PARTY__CONDITIONLIST_ITEM5 
				&& !(m_byStatus>=GROUP_PARTY_DEFAULT &&  m_byStatus<=GROUP_PARTY_SELECT) && m_byStatus!=GROUP_PARTY_PARTYUSERINFO1 && m_byStatus!=GROUP_PARTY_PARTYUSERINFO2) // ��Ƽ ��ȭ �ý��� �߰� - if���� �߰�  (����â 2005.02.18)
			{

			if(eMouseCheck==MOUSE_UP)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_EXP1 || (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_ITEM5  && GROUP_PARTY_PMATCHCONDITION_PMAINCHO5 >= m_byStatus) 
						|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_EXP5 || (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_ITEM1 &&  GROUP_PARTY_PMATCHCONDITION_PMAINCHO4  >= m_byStatus)
						|| (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_LEV1 && GROUP_PARTY_PMATCHCONDITION_LEV4 >= m_byStatus) || m_byStatus==GROUP_PARTY_PMATCHCONDITION_LEV5)
					{ 
						if((m_byPMatchStatus != 1 && g_pMainPlayer->m_dwPartyId == 0) || (m_byPMatchStatus != 1 &&g_pMainPlayer->m_bPartyLeader==1))
						{
							SetStatus(GROUP_PARTY_PMATCHCONDITION_EXP2);
							byChk = __MOUSE_POINTER_BUTTON__;
						}
					}
					else if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_EXP3 || m_byPMatchStatus == 0)
					{
						SetStatus(GROUP_PARTY_PMATCHCONDITION1);
						byChk = __MOUSE_POINTER_BUTTON__;
					}

				}
				else
					byChk = __MOUSE_POINTER_DEFAULT__;
				
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_EXP4 || (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_ITEM5  && GROUP_PARTY_PMATCHCONDITION_PMAINCHO5 >= m_byStatus) 
						|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_EXP5 || (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_ITEM1 && GROUP_PARTY_PMATCHCONDITION_PMAINCHO4 >= m_byStatus)
						|| (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_LEV1 && GROUP_PARTY_PMATCHCONDITION_LEV4 >= m_byStatus) || m_byStatus==GROUP_PARTY_PMATCHCONDITION_LEV5)
					{	
						if((m_byPMatchStatus != 1 && g_pMainPlayer->m_dwPartyId == 0) || (m_byPMatchStatus != 1 &&g_pMainPlayer->m_bPartyLeader==1))
						{
							SetStatus(GROUP_PARTY_PMATCHCONDITION_EXP1);
							byChk = __MOUSE_POINTER_BUTTONCLICK__;
						}
					}
					else if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_EXP2)
					{	
						m_byBackBoard	 = 0;
						SetStatus(GROUP_PARTY_PMATCHCONDITION_EXP3);
						byChk = __MOUSE_POINTER_BUTTONCLICK__;
					}

				}
				else
					byChk = __MOUSE_POINTER_DEFAULT__;

				}			
			else if(eMouseCheck==MOUSE_OVER)
			{	
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_PMATCHCONDITION1 || (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_ITEM5  && GROUP_PARTY_PMATCHCONDITION_PMAINCHO5 >= m_byStatus) 
						|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_EXP5 || (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_ITEM1 && GROUP_PARTY_PMATCHCONDITION_PMAINCHO4 >= m_byStatus)
						|| (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_LEV1 && GROUP_PARTY_PMATCHCONDITION_LEV4 >= m_byStatus) || m_byStatus==GROUP_PARTY_PMATCHCONDITION_LEV5)
					{
						if((m_byPMatchStatus != 1 && g_pMainPlayer->m_dwPartyId == 0) || (m_byPMatchStatus != 1 &&g_pMainPlayer->m_bPartyLeader==1))
						{
							SetStatus(GROUP_PARTY_PMATCHCONDITION_EXP4);
							byChk = __MOUSE_POINTER_BUTTON__;
						}
					}
					else if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_EXP2)
					{
						byChk = __MOUSE_POINTER_BUTTON__;
					}
				}
				else 
					byChk = __MOUSE_POINTER_DEFAULT__;
				
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		}
		break;
	
		//��Ƽ ��ȭ �ý��� (����â 2005.01.28) 
		case 38:										//GROUP_PARTY_PMATCHCONDITION1 Ŭ���� ��Ӵٿ� GROUP_PARTY_PMATCHCONDITION_ITEM �ڽ� 
		{
			BYTE byChk = 0;

			if(m_byStatus!=GROUP_PARTY_EXPITEMCONDITION1 && !(m_byStatus>=GROUP_PARTY__CONDITIONLIST_EXP1 && m_byStatus<=GROUP_PARTY__CONDITIONLIST_ITEM4)
				&& m_byStatus!=GROUP_PARTY__CONDITIONLIST_EXP5 && m_byStatus!=GROUP_PARTY__CONDITIONLIST_ITEM5 
				&& !(m_byStatus>=GROUP_PARTY_DEFAULT &&  m_byStatus<=GROUP_PARTY_SELECT) && m_byStatus!=GROUP_PARTY_PARTYUSERINFO1 && m_byStatus!=GROUP_PARTY_PARTYUSERINFO2) // ��Ƽ ��ȭ �ý��� �߰� - if���� �߰�  (����â 2005.02.18)
			{

			if(eMouseCheck==MOUSE_UP)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_ITEM1 || m_byStatus==GROUP_PARTY_PMATCHCONDITION_ITEM5 || m_byStatus==GROUP_PARTY_PMATCHCONDITION_PMAINCHO5  
						|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_EXP5 || (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_LEV1 &&  GROUP_PARTY_PMATCHCONDITION_EXP4  >= m_byStatus)
						|| (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_PMAINCHO1 && GROUP_PARTY_PMATCHCONDITION_PMAINCHO4 >= m_byStatus) || m_byStatus==GROUP_PARTY_PMATCHCONDITION_LEV5)
					{
						if((m_byPMatchStatus != 1 && g_pMainPlayer->m_dwPartyId == 0) || (m_byPMatchStatus != 1 &&g_pMainPlayer->m_bPartyLeader==1))
						{
							SetStatus(GROUP_PARTY_PMATCHCONDITION_ITEM2);
							byChk = __MOUSE_POINTER_BUTTON__;
						}
					}
					else if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_ITEM3 || m_byPMatchStatus == 0)
					{
						SetStatus(GROUP_PARTY_PMATCHCONDITION1);
						byChk = __MOUSE_POINTER_BUTTON__;
					}

				}
				else
					byChk = __MOUSE_POINTER_DEFAULT__;
				
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_ITEM4 || m_byStatus==GROUP_PARTY_PMATCHCONDITION_ITEM5 || m_byStatus==GROUP_PARTY_PMATCHCONDITION_PMAINCHO5  
						|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_EXP5 || (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_LEV1 &&  GROUP_PARTY_PMATCHCONDITION_EXP4  >= m_byStatus)
						|| (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_PMAINCHO1 && GROUP_PARTY_PMATCHCONDITION_PMAINCHO4 >= m_byStatus) || m_byStatus==GROUP_PARTY_PMATCHCONDITION_LEV5)
					{
						if((m_byPMatchStatus != 1 && g_pMainPlayer->m_dwPartyId == 0) || (m_byPMatchStatus != 1 &&g_pMainPlayer->m_bPartyLeader==1))
						{
							SetStatus(GROUP_PARTY_PMATCHCONDITION_ITEM1);
							byChk = __MOUSE_POINTER_BUTTONCLICK__;
						}
					}
					else if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_ITEM2)
					{
						m_byBackBoard	 = 0;
						SetStatus(GROUP_PARTY_PMATCHCONDITION_ITEM3);
						byChk = __MOUSE_POINTER_BUTTONCLICK__;
					}

				}
				else
					byChk = __MOUSE_POINTER_DEFAULT__;

				}			
			else if(eMouseCheck==MOUSE_OVER)
			{	
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_PMATCHCONDITION1 || m_byStatus==GROUP_PARTY_PMATCHCONDITION_ITEM5 || m_byStatus==GROUP_PARTY_PMATCHCONDITION_PMAINCHO5  
						|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_EXP5 || (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_LEV1 &&  GROUP_PARTY_PMATCHCONDITION_EXP4  >= m_byStatus)
						|| (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_PMAINCHO1 && GROUP_PARTY_PMATCHCONDITION_PMAINCHO4 >= m_byStatus) || m_byStatus==GROUP_PARTY_PMATCHCONDITION_LEV5)
					{
						if((m_byPMatchStatus != 1 && g_pMainPlayer->m_dwPartyId == 0) || (m_byPMatchStatus != 1 &&g_pMainPlayer->m_bPartyLeader==1))
						{
							SetStatus(GROUP_PARTY_PMATCHCONDITION_ITEM4);
							byChk = __MOUSE_POINTER_BUTTON__;
						}
					}
					else if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_ITEM2)
					{
						byChk = __MOUSE_POINTER_BUTTON__;
					}
				}
				else 
					byChk = __MOUSE_POINTER_DEFAULT__;
				
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		}
		break;

		//��Ƽ ��ȭ �ý��� (����â 2005.01.28) 
		case 39:										//GROUP_PARTY_PMATCHCONDITION1 Ŭ���� ��Ӵٿ� GROUP_PARTY_PMATCHCONDITION_PMAINCHO �ڽ� 
		{
			BYTE byChk = 0;

			if(m_byStatus!=GROUP_PARTY_EXPITEMCONDITION1 && !(m_byStatus>=GROUP_PARTY__CONDITIONLIST_EXP1 && m_byStatus<=GROUP_PARTY__CONDITIONLIST_ITEM4)
				&& m_byStatus!=GROUP_PARTY__CONDITIONLIST_EXP5 && m_byStatus!=GROUP_PARTY__CONDITIONLIST_ITEM5 
				&& !(m_byStatus>=GROUP_PARTY_DEFAULT &&  m_byStatus<=GROUP_PARTY_SELECT) && m_byStatus!=GROUP_PARTY_PARTYUSERINFO1 && m_byStatus!=GROUP_PARTY_PARTYUSERINFO2) // ��Ƽ ��ȭ �ý��� �߰� - if���� �߰�  (����â 2005.02.18)
			{

			if(eMouseCheck==MOUSE_UP)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_PMAINCHO1 || m_byStatus==GROUP_PARTY_PMATCHCONDITION_ITEM5 || m_byStatus==GROUP_PARTY_PMATCHCONDITION_PMAINCHO5  
						|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_EXP5 || (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_LEV1 &&  GROUP_PARTY_PMATCHCONDITION_ITEM4  >= m_byStatus)
						|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_LEV5)
					{	
						if((m_byPMatchStatus != 1 && g_pMainPlayer->m_dwPartyId == 0) || (m_byPMatchStatus != 1 &&g_pMainPlayer->m_bPartyLeader==1))
						{
							SetStatus(GROUP_PARTY_PMATCHCONDITION_PMAINCHO2);
							byChk = __MOUSE_POINTER_BUTTON__;
						}
					}
					else if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_PMAINCHO3 || m_byPMatchStatus == 0)
					{
						SetStatus(GROUP_PARTY_PMATCHCONDITION1);
						byChk = __MOUSE_POINTER_BUTTON__;
					}

				}
				else
					byChk = __MOUSE_POINTER_DEFAULT__;
				
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_PMAINCHO4 || m_byStatus==GROUP_PARTY_PMATCHCONDITION_ITEM5 || m_byStatus==GROUP_PARTY_PMATCHCONDITION_PMAINCHO5  
						|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_EXP5 || (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_LEV1 &&  GROUP_PARTY_PMATCHCONDITION_ITEM4  >= m_byStatus)
						|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_LEV5)
					{
						if((m_byPMatchStatus != 1 && g_pMainPlayer->m_dwPartyId == 0) || (m_byPMatchStatus != 1 &&g_pMainPlayer->m_bPartyLeader==1))
						{
							SetStatus(GROUP_PARTY_PMATCHCONDITION_PMAINCHO1);
							byChk = __MOUSE_POINTER_BUTTONCLICK__;
						}
					}
					else if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_PMAINCHO2)
					{
						m_byBackBoard	 = 0;
						SetStatus(GROUP_PARTY_PMATCHCONDITION_PMAINCHO3);
						byChk = __MOUSE_POINTER_BUTTONCLICK__;
					}

				}
				else
					byChk = __MOUSE_POINTER_DEFAULT__;

				}			
			else if(eMouseCheck==MOUSE_OVER)
			{	
				if(m_byGroupTab==GROUP_TAB_PARTY)
				{
					if(m_byStatus==GROUP_PARTY_PMATCHCONDITION1 || m_byStatus==GROUP_PARTY_PMATCHCONDITION_ITEM5 || m_byStatus==GROUP_PARTY_PMATCHCONDITION_PMAINCHO5  
						|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_EXP5 || (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_LEV1 &&  GROUP_PARTY_PMATCHCONDITION_ITEM4  >= m_byStatus)
						|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_LEV5)
					{
						if((m_byPMatchStatus != 1 && g_pMainPlayer->m_dwPartyId == 0) || (m_byPMatchStatus != 1 &&g_pMainPlayer->m_bPartyLeader==1))
						{
							SetStatus(GROUP_PARTY_PMATCHCONDITION_PMAINCHO4);
							byChk = __MOUSE_POINTER_BUTTON__;
						}
					}
					else if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_PMAINCHO2)
					{
						byChk = __MOUSE_POINTER_BUTTON__;
					}
				}
				else 
					byChk = __MOUSE_POINTER_DEFAULT__;
				
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		}
		break;

		//��Ƽ��ȭ�ý���(����â 2005.01.31)
		case 42:								//3���� 
		{
			BYTE byChk = 0;

			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(eMouseCheck==MOUSE_DOWN)
				{ 
					if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_LEV1|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_LEV2)
					{
						m_bLevelMatch	= 3;
						m_byBackBoard	 = 0;	
						SetStatus(GROUP_PARTY_PMATCHCONDITION_LEV5);
					}
				}
				else if(eMouseCheck==MOUSE_OVER)
				{
			//		m_byColor = 1;
					byChk = __MOUSE_POINTER_BUTTON__;
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;

		//��Ƽ��ȭ�ý���(����â 2005.01.31)
		case 43:								//5���� 
		{
			BYTE byChk = 0;
			
			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(eMouseCheck==MOUSE_DOWN)
				{ 
					if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_LEV1|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_LEV2)
					{
						m_bLevelMatch	= 5;
						m_byBackBoard	 = 0;
						SetStatus(GROUP_PARTY_PMATCHCONDITION_LEV5);
					}
				}
				else if(eMouseCheck==MOUSE_OVER)
				{
			//		m_byColor = 1;
					byChk = __MOUSE_POINTER_BUTTON__;
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;

		//��Ƽ��ȭ�ý���(����â 2005.01.31)
		case 44:								//10���� 
		{
			BYTE byChk = 0;

			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(eMouseCheck==MOUSE_DOWN)
				{ 
					if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_LEV1|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_LEV2)
					{
						m_bLevelMatch	= 10;
						m_byBackBoard	 = 0;
						SetStatus(GROUP_PARTY_PMATCHCONDITION_LEV5);
					}
				}
				else if(eMouseCheck==MOUSE_OVER)
				{
			//		m_byColor = 1;
					byChk = __MOUSE_POINTER_BUTTON__;
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;

		//��Ƽ��ȭ�ý���(����â 2005.01.31)
		case 45:								//����й� 
		{
			BYTE byChk = 0;
			
			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(eMouseCheck==MOUSE_DOWN)
				{ 
					if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_EXP1|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_EXP2)
					{
						m_bExpMatch = 1;
						m_byBackBoard	 = 0;
						SetStatus(GROUP_PARTY_PMATCHCONDITION_EXP5);
					}
				}
				else if(eMouseCheck==MOUSE_OVER)
				{
			//		m_byColor = 1;
					byChk = __MOUSE_POINTER_BUTTON__;
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;

		//��Ƽ��ȭ�ý���(����â 2005.01.31)
		case 46:								//�������й� 
		{
			BYTE byChk = 0;

			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(eMouseCheck==MOUSE_DOWN)
				{ 
					if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_EXP1|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_EXP2)
					{
						m_bExpMatch = 2;
						m_byBackBoard	 = 0;
						SetStatus(GROUP_PARTY_PMATCHCONDITION_EXP5);
					}
				}
				else if(eMouseCheck==MOUSE_OVER)
				{
			//		m_byColor = 1;
					byChk = __MOUSE_POINTER_BUTTON__;
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;

		//��Ƽ��ȭ�ý���(����â 2005.01.31)
		case 47:								//���ʽ�����ġ �й� 
		{
			BYTE byChk = 0;

			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(eMouseCheck==MOUSE_DOWN)
				{ 
					if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_EXP1|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_EXP2)
					{
						m_bExpMatch	= 3;
						m_byBackBoard	 = 0;
						SetStatus(GROUP_PARTY_PMATCHCONDITION_EXP5);
					}
				}
				else if(eMouseCheck==MOUSE_OVER)
				{
			//		m_byColor = 1;
					byChk = __MOUSE_POINTER_BUTTON__;
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;

		//��Ƽ��ȭ�ý���(����â 2005.01.31)
		case 48:								//������� 
		{
			BYTE byChk = 0;

			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(eMouseCheck==MOUSE_DOWN)
				{ 
					if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_EXP1|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_EXP2)
					{
						m_bExpMatch	= 4;
						m_byBackBoard	 = 0;
						SetStatus(GROUP_PARTY_PMATCHCONDITION_EXP5);
					}
				}
				else if(eMouseCheck==MOUSE_OVER)
				{
			//		m_byColor = 1;
					byChk = __MOUSE_POINTER_BUTTON__;
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;

		//��Ƽ��ȭ�ý���(����â 2005.01.31)
		case 49:								//�����ڿ켱  
		{
			BYTE byChk = 0;

			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(eMouseCheck==MOUSE_DOWN)
				{ 
					if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_ITEM1|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_ITEM2)
					{
						m_bItemMatch = 1;
						m_byBackBoard	 = 0;
						SetStatus(GROUP_PARTY_PMATCHCONDITION_ITEM5);
					}
				}
				else if(eMouseCheck==MOUSE_OVER)
				{
			//		m_byColor = 1;
					byChk = __MOUSE_POINTER_BUTTON__;
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;

		//��Ƽ��ȭ�ý���(����â 2005.01.31)
		case 50:								//������ Ÿ���� �켱 
		{
			BYTE byChk = 0;

			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(eMouseCheck==MOUSE_DOWN)
				{ 
					if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_ITEM1|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_ITEM2)
					{
						m_bItemMatch = 2;
						m_byBackBoard	 = 0;
						SetStatus(GROUP_PARTY_PMATCHCONDITION_ITEM5);
					}
				}
				else if(eMouseCheck==MOUSE_OVER)
				{
			//		m_byColor = 1;
					byChk = __MOUSE_POINTER_BUTTON__;
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;

		//��Ƽ��ȭ�ý���(����â 2005.01.31)
		case 51:								//��������  
		{
			BYTE byChk = 0;

			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(eMouseCheck==MOUSE_DOWN)
				{ 
					if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_ITEM1|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_ITEM2)
					{
						m_bItemMatch = 3;
						m_byBackBoard	 = 0;
						SetStatus(GROUP_PARTY_PMATCHCONDITION_ITEM5);
					}
				}
				else if(eMouseCheck==MOUSE_OVER)
				{
			//		m_byColor = 1;
					byChk = __MOUSE_POINTER_BUTTON__;
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;

		//��Ƽ��ȭ�ý���(����â 2005.01.31)
		case 52:								//������� 
		{
			BYTE byChk = 0;

			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(eMouseCheck==MOUSE_DOWN)
				{ 
					if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_ITEM1|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_ITEM2)
					{
						m_bItemMatch = 4;
						m_byBackBoard	 = 0;
						SetStatus(GROUP_PARTY_PMATCHCONDITION_ITEM5);
					}
				}
				else if(eMouseCheck==MOUSE_OVER)
				{
			//		m_byColor = 1;
					byChk = __MOUSE_POINTER_BUTTON__;
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;

		//��Ƽ��ȭ�ý���(����â 2005.01.31)
		case 53:								//������ ��Ƽ�� ���� 
		{
			BYTE byChk = 0;

			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(eMouseCheck==MOUSE_DOWN)
				{ 
					if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_PMAINCHO1|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_PMAINCHO2)
					{
						m_bMasterMatch	= 0;
						m_byBackBoard	 = 0;
						SetStatus(GROUP_PARTY_PMATCHCONDITION_PMAINCHO5);
					}
				}
				else if(eMouseCheck==MOUSE_OVER)
				{
			//		m_byColor = 1;
					byChk = __MOUSE_POINTER_BUTTON__;
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;

		//��Ƽ��ȭ�ý���(����â 2005.01.31)
		case 54:								//�ڽ��� ��Ƽ������ ĳ���� �ʴ� 
		{
			BYTE byChk = 0;

			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(eMouseCheck==MOUSE_DOWN)
				{ 
					if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_PMAINCHO1|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_PMAINCHO2)
					{
						m_bMasterMatch	= 1;
						m_byBackBoard	 = 0;
						SetStatus(GROUP_PARTY_PMATCHCONDITION_PMAINCHO5);
					}
				}
				else if(eMouseCheck==MOUSE_OVER)
				{
			//		m_byColor = 1;
					byChk = __MOUSE_POINTER_BUTTON__;
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break; 
	
		//��Ƽ��ȭ�ý���(����â 2005.02.01)
		case 55:											// PMATCHCONDITION_OK ��ư ������ ���  
		{
			BYTE byChk = 0;
			
			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(eMouseCheck==MOUSE_DOWN)    
				{ 
				
				} 
				else if(eMouseCheck==MOUSE_UP)
				{
					if(!(m_byStatus>=GROUP_PARTY_CREATEADD && GROUP_PARTY__CONDITIONLIST_ITEM4 >= m_byStatus) || m_byStatus==GROUP_PARTY_PMATCHCONDITION_OK_ON 
						 || m_byStatus==GROUP_PARTY_PMATCHCONDITION_OK_OFF)       
					
					{
						if(g_pMainPlayer->m_bPartyLeader==1 || g_pMainPlayer->m_dwPartyId == 0)
						{
							if(m_byPMatchStatus == 0) 
							{
								m_byPMatchStatus = 1;
								SetStatus(GROUP_PARTY_PMATCHCONDITION_OK_ON); 

								if(m_bLevelMatch>=3 && m_bExpMatch>=1 && m_bItemMatch>=1 && m_bMasterMatch>=0)
								{
									CTWS_PMATCH_REGISTER pPacket;
									pPacket.condition.bLevel  = m_bLevelMatch;
									pPacket.condition.bExp	  = m_bExpMatch;   
									pPacket.condition.bItem   = m_bItemMatch;      
									pPacket.condition.bMaster = m_bMasterMatch;	 
									g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);	
								}

								DisplayMessageAdd(g_Message[ETC_MESSAGE1377].szMessage, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY); // ��Ƽ ��Ī�� ���۵Ǿ����ϴ�.

							}
							else if(m_byPMatchStatus == 1 || g_pMainPlayer->m_dwPartyId != 0) 
							{
								m_byPMatchStatus = 0;
								SetStatus(GROUP_PARTY_PMATCHCONDITION_OK_OFF);

								CTWS_PMATCH_ESCAPE pPacket;
								g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);

								DisplayMessageAdd(g_Message[ETC_MESSAGE1378].szMessage, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY); //��Ƽ ��Ī�� ����Ǿ����ϴ�.
							
							}
						}

					}
					else if((m_byStatus>=GROUP_PARTY_CREATEADD && GROUP_PARTY_PMATCHCONDITION_PMAINCHO5 >= m_byStatus))
					{
						if(g_pMainPlayer->m_bPartyLeader==1 || g_pMainPlayer->m_dwPartyId == 0 || g_pMainPlayer->m_dwPartyId != 0)
						{
							if(m_byPMatchStatus == 1)
							{
								m_byPMatchStatus = 0;
								SetStatus(GROUP_PARTY_PMATCHCONDITION_OK_OFF);

								CTWS_PMATCH_ESCAPE pPacket;
								g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);

								DisplayMessageAdd(g_Message[ETC_MESSAGE1378].szMessage, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY); //��Ƽ ��Ī�� ����Ǿ����ϴ�.
							

							}
						}
					}
				}

				else if(eMouseCheck==MOUSE_OVER)						//��Ƽ ��ȭ �ý��� (����â 2005.02.07 �߰�)
				{
					if(m_byStatus==GROUP_PARTY_PMATCHCONDITION1)
					{
						if(m_byPMatchStatus == 1)
						{
							m_byRenderType = GROUP_RENDER_PMATCHBUTTON_ON_MSG;	
							byChk = __MOUSE_POINTER_BUTTON__;
						}
						else if(m_byPMatchStatus == 0)
						{
							m_byRenderType = GROUP_RENDER_PMATCHBUTTON_OFF_MSG;	
							byChk = __MOUSE_POINTER_BUTTON__;
						}
					}
					else if(m_byStatus==GROUP_PARTY_PMATCHCONDITION1 || m_byPMatchStatus == 0)
					{
						m_byRenderType = GROUP_RENDER_PMATCHBUTTON_OFF_MSG;	
						byChk = __MOUSE_POINTER_BUTTON__;
					}
					else if(m_byStatus!=GROUP_PARTY_PMATCHCONDITION1)
					{
						if(m_byPMatchStatus == 1)
						{
							m_byRenderType = GROUP_RENDER_PMATCHBUTTON_ON_MSG;	
							byChk = __MOUSE_POINTER_BUTTON__;
						}
					
					}
					
				}
			}
			else if(m_byGroupTab==GROUP_TAB_GUILDWAR)
			{
				if(m_byStatus==GROUP_GUILDWAR_MATCH)
				{
					if(eMouseCheck==MOUSE_DOWN)
					{
						if(!g_pThisDungeon->IsStadium() && !g_pMainPlayer->m_bPlayerShop && 
							!g_pMainPlayer->m_bMatching)//����� ���� �ƴϰ� �����ߵ� �ƴϰ� pvp�ߵ� �ƴϰ�
						{
							if(m_bGuildWarPacketSendEnable)
							{
								if(m_bGuildWarMatchEnable)
								{
									SetRender(SPR_OBJ_GUILDWAR_AUTOMATCHING1, FALSE);
									SetRender(SPR_OBJ_GUILDWAR_AUTOMATCHING2, TRUE);
									SetRender(SPR_OBJ_GUILDWAR_AUTOMATCH_V, TRUE);
									m_bGuildWarMatchEnable = !m_bGuildWarMatchEnable;//�ӽ�, �������� ���� �������� ������ ����

									//������ ��Ī ���縦 ������.
									m_bGuildWarPacketSendEnable = FALSE;
									TEAM_MATCH_PACKET TeamMatch;
									TeamMatch.Body.Condtion.iMatchType = M_T_NONE;
									TeamMatch.Body.Condtion.iMatchGuildNumber = 0;
									TeamMatch.Body.Condtion.iMatchGuildType = T_T_NONE;
									Cmd_CTWS_GuildWarPacket(TeamMatch, CTWS_TM_REMOVE);
								}
								else
								{
									SetRender(SPR_OBJ_GUILDWAR_AUTOMATCHING1, TRUE);
									SetRender(SPR_OBJ_GUILDWAR_AUTOMATCHING2, FALSE);
									SetRender(SPR_OBJ_GUILDWAR_AUTOMATCH_V, FALSE);
									m_bGuildWarMatchEnable = !m_bGuildWarMatchEnable;//�ӽ�, �������� ���� �������� ������ ����

									//������ ��Ī ������ ������.
									m_bGuildWarPacketSendEnable = FALSE;
									TEAM_MATCH_PACKET TeamMatch;
									TeamMatch.Body.Condtion.iMatchType = GetFormConvert((int)m_bynGuildWarForm);
									TeamMatch.Body.Condtion.iMatchGuildNumber = GetWarNumber((int)m_byGuildWarNumber);
									TeamMatch.Body.Condtion.iMatchGuildType = GetTypeConvert((int)m_byGuildWarType);
									Cmd_CTWS_GuildWarPacket(TeamMatch, CTWS_TM_INSERT);
								}
							}
						}
					}
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;
		
		//��Ƽ �ý��� ģ�� �߰� (����â 2005.02.28)
		case 102:											 
		{
			BYTE byChk = 0;
			
			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(g_pPartyUserHash->GetCount() == 1)  
				{
					if(eMouseCheck==MOUSE_DOWN)    
					{ 
				
					} 
					else if(eMouseCheck==MOUSE_UP)   
					{
						if(g_pMessengerOnLineHash->GetCount()+g_pMessengerOffLineHash->GetCount()<MAX_MESSENGER_USER)
						{
							ListNode<PARTY_USER>* pNode = g_pPartyUserHash->GetHead();

							LP_PARTY_USER sPartyNode = pNode->pData;
							
							LP_MESSENGER_USER lpMessengerUser = g_pMessengerOnLineHash->GetData(sPartyNode->dwUserIndex);

							if(lpMessengerUser)	
							{
								// "�̹� ��ϵǾ� �ֽ��ϴ�."
								DisplayMessageAdd(g_Message[ETC_MESSAGE251].szMessage, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND); 
							}
							else
							{
								CTWS_MESSENGER_USERADD	pMessengerPacket;							
								memset(pMessengerPacket.szCharacterName, 0, sizeof(pMessengerPacket.szCharacterName));
								pMessengerPacket.dwUserIndex	= sPartyNode->dwUserIndex;
								__lstrcpyn(pMessengerPacket.szCharacterName, sPartyNode->szName, MAX_CHARACTER_NAME_REAL_LENGTH);
								g_pNet->SendMsg((char*)&pMessengerPacket, pMessengerPacket.GetPacketSize(), SERVER_INDEX_WORLD);
					
								char szInfo[0xff] = {0,};
								wsprintf(szInfo, g_Message[ETC_MESSAGE830].szMessage, sPartyNode->szName); // "%s �Կ��� ģ�� ��� �޼����� ���½��ϴ�."
								DisplayMessageAdd(szInfo, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);		// "�̹� ��ϵǾ� �ֽ��ϴ�."
							}														
						}
						else
						{
							// "�� �̻� ģ���� ����� �� �����ϴ�."					
							DisplayMessageAdd(g_Message[ETC_MESSAGE165].szMessage, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);	
						}
				
					}				
					
					else if(eMouseCheck==MOUSE_OVER)					
					{
						byChk = __MOUSE_POINTER_BUTTON__;
					}
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;

		//��Ƽ �ý��� ģ�� �߰� (����â 2005.02.28)
		case 103:											 
		{
			BYTE byChk = 0;
			
			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(g_pPartyUserHash->GetCount() == 2)
				{
					if(eMouseCheck==MOUSE_DOWN)    
					{ 
				
					} 
					else if(eMouseCheck==MOUSE_UP)
					{
						if(g_pMessengerOnLineHash->GetCount()+g_pMessengerOffLineHash->GetCount()<MAX_MESSENGER_USER)
						{
							ListNode<PARTY_USER>* pNode = g_pPartyUserHash->GetHead();
							pNode = pNode->pNext;

							LP_PARTY_USER sPartyNode = (pNode->pData);
							
							LP_MESSENGER_USER lpMessengerUser = g_pMessengerOnLineHash->GetData(sPartyNode->dwUserIndex);
							
							if(lpMessengerUser)	
							{
								// "�̹� ��ϵǾ� �ֽ��ϴ�."
								DisplayMessageAdd(g_Message[ETC_MESSAGE251].szMessage, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND); 
							}
							else  
							{
								CTWS_MESSENGER_USERADD	pMessengerPacket;							
								memset(pMessengerPacket.szCharacterName, 0, sizeof(pMessengerPacket.szCharacterName));
								pMessengerPacket.dwUserIndex	= sPartyNode->dwUserIndex;  
								__lstrcpyn(pMessengerPacket.szCharacterName, sPartyNode->szName, MAX_CHARACTER_NAME_REAL_LENGTH); 
								g_pNet->SendMsg((char*)&pMessengerPacket, pMessengerPacket.GetPacketSize(), SERVER_INDEX_WORLD);
					
								char szInfo[0xff] = {0,};
								wsprintf(szInfo, g_Message[ETC_MESSAGE830].szMessage, sPartyNode->szName); // "%s �Կ��� ģ�� ��� �޼����� ���½��ϴ�."sPartyNode->szName
								DisplayMessageAdd(szInfo, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);		// "�̹� ��ϵǾ� �ֽ��ϴ�."
							}														
						}
						else
						{
							// "�� �̻� ģ���� ����� �� �����ϴ�."					
							DisplayMessageAdd(g_Message[ETC_MESSAGE165].szMessage, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);	
						}
					}
					else if(eMouseCheck==MOUSE_OVER)					
					{
						byChk = __MOUSE_POINTER_BUTTON__;
					}
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;

		//��Ƽ �ý��� ģ�� �߰� (����â 2005.02.28)
		case 104:											 
		{
			BYTE byChk = 0;
			
			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(g_pPartyUserHash->GetCount() == 3)
				{
					if(eMouseCheck==MOUSE_DOWN)    
					{ 
				
					} 
					else if(eMouseCheck==MOUSE_UP)
					{
						if(g_pMessengerOnLineHash->GetCount()+g_pMessengerOffLineHash->GetCount()<MAX_MESSENGER_USER)
						{
							ListNode<PARTY_USER>* pNode = g_pPartyUserHash->GetHead();
							pNode = pNode->pNext;
							pNode = pNode->pNext;

							LP_PARTY_USER sPartyNode = (pNode->pData);
							
							LP_MESSENGER_USER lpMessengerUser = g_pMessengerOnLineHash->GetData(sPartyNode->dwUserIndex);
							
							if(lpMessengerUser)	
							{
								// "�̹� ��ϵǾ� �ֽ��ϴ�."
								DisplayMessageAdd(g_Message[ETC_MESSAGE251].szMessage, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND); 
							}
							else  
							{
								CTWS_MESSENGER_USERADD	pMessengerPacket;							
								memset(pMessengerPacket.szCharacterName, 0, sizeof(pMessengerPacket.szCharacterName));
								pMessengerPacket.dwUserIndex	= sPartyNode->dwUserIndex;  
								__lstrcpyn(pMessengerPacket.szCharacterName, sPartyNode->szName, MAX_CHARACTER_NAME_REAL_LENGTH); 
								g_pNet->SendMsg((char*)&pMessengerPacket, pMessengerPacket.GetPacketSize(), SERVER_INDEX_WORLD);
					
								char szInfo[0xff] = {0,};
								wsprintf(szInfo, g_Message[ETC_MESSAGE830].szMessage, sPartyNode->szName); // "%s �Կ��� ģ�� ��� �޼����� ���½��ϴ�."sPartyNode->szName
								DisplayMessageAdd(szInfo, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);		// "�̹� ��ϵǾ� �ֽ��ϴ�."
							}														
						}
						else
						{
							// "�� �̻� ģ���� ����� �� �����ϴ�."					
							DisplayMessageAdd(g_Message[ETC_MESSAGE165].szMessage, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);	
						}
					
					}

					else if(eMouseCheck==MOUSE_OVER)					
					{
						byChk = __MOUSE_POINTER_BUTTON__;
					}
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;

		//��Ƽ �ý��� ģ�� �߰� (����â 2005.02.28)
		case 105:											 
		{
			BYTE byChk = 0;
			
			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(g_pPartyUserHash->GetCount() == 4)
				{
					if(eMouseCheck==MOUSE_DOWN)    
					{ 
				
					} 
					else if(eMouseCheck==MOUSE_UP)
					{
						if(g_pMessengerOnLineHash->GetCount()+g_pMessengerOffLineHash->GetCount()<MAX_MESSENGER_USER)
						{
							ListNode<PARTY_USER>* pNode = g_pPartyUserHash->GetHead();
							pNode = pNode->pNext;
							pNode = pNode->pNext;
							pNode = pNode->pNext;

							LP_PARTY_USER sPartyNode = (pNode->pData);
							
							LP_MESSENGER_USER lpMessengerUser = g_pMessengerOnLineHash->GetData(sPartyNode->dwUserIndex);
							
							if(lpMessengerUser)	
							{
								// "�̹� ��ϵǾ� �ֽ��ϴ�."
								DisplayMessageAdd(g_Message[ETC_MESSAGE251].szMessage, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND); 
							}
							else  
							{
								CTWS_MESSENGER_USERADD	pMessengerPacket;							
								memset(pMessengerPacket.szCharacterName, 0, sizeof(pMessengerPacket.szCharacterName));
								pMessengerPacket.dwUserIndex	= sPartyNode->dwUserIndex;  
								__lstrcpyn(pMessengerPacket.szCharacterName, sPartyNode->szName, MAX_CHARACTER_NAME_REAL_LENGTH); 
								g_pNet->SendMsg((char*)&pMessengerPacket, pMessengerPacket.GetPacketSize(), SERVER_INDEX_WORLD);
					
								char szInfo[0xff] = {0,};
								wsprintf(szInfo, g_Message[ETC_MESSAGE830].szMessage, sPartyNode->szName); // "%s �Կ��� ģ�� ��� �޼����� ���½��ϴ�."sPartyNode->szName
								DisplayMessageAdd(szInfo, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);		// "�̹� ��ϵǾ� �ֽ��ϴ�."
							}														
						}
						else
						{
							// "�� �̻� ģ���� ����� �� �����ϴ�."					
							DisplayMessageAdd(g_Message[ETC_MESSAGE165].szMessage, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);	
						}
					
					}

					else if(eMouseCheck==MOUSE_OVER)					
					{
						byChk = __MOUSE_POINTER_BUTTON__;
					}
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;

		//��Ƽ �ý��� ģ�� �߰� (����â 2005.02.28)
		case 106:											 
		{
			BYTE byChk = 0;
			
			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(g_pPartyUserHash->GetCount() == 5)
				{
					if(eMouseCheck==MOUSE_DOWN)    
					{ 
				
					} 
					else if(eMouseCheck==MOUSE_UP)
					{
						if(g_pMessengerOnLineHash->GetCount()+g_pMessengerOffLineHash->GetCount()<MAX_MESSENGER_USER)
						{
							ListNode<PARTY_USER>* pNode = g_pPartyUserHash->GetHead();
							pNode = pNode->pNext;
							pNode = pNode->pNext;
							pNode = pNode->pNext;
							pNode = pNode->pNext;

							LP_PARTY_USER sPartyNode = (pNode->pData);
							
							LP_MESSENGER_USER lpMessengerUser = g_pMessengerOnLineHash->GetData(sPartyNode->dwUserIndex);
							
							if(lpMessengerUser)	
							{
								// "�̹� ��ϵǾ� �ֽ��ϴ�."
								DisplayMessageAdd(g_Message[ETC_MESSAGE251].szMessage, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND); 
							}
							else  
							{
								CTWS_MESSENGER_USERADD	pMessengerPacket;							
								memset(pMessengerPacket.szCharacterName, 0, sizeof(pMessengerPacket.szCharacterName));
								pMessengerPacket.dwUserIndex	= sPartyNode->dwUserIndex;  
								__lstrcpyn(pMessengerPacket.szCharacterName, sPartyNode->szName, MAX_CHARACTER_NAME_REAL_LENGTH); 
								g_pNet->SendMsg((char*)&pMessengerPacket, pMessengerPacket.GetPacketSize(), SERVER_INDEX_WORLD);
					
								char szInfo[0xff] = {0,};
								wsprintf(szInfo, g_Message[ETC_MESSAGE830].szMessage, sPartyNode->szName); // "%s �Կ��� ģ�� ��� �޼����� ���½��ϴ�."sPartyNode->szName
								DisplayMessageAdd(szInfo, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);		// "�̹� ��ϵǾ� �ֽ��ϴ�."
							}														
						}
						else
						{
							// "�� �̻� ģ���� ����� �� �����ϴ�."					
							DisplayMessageAdd(g_Message[ETC_MESSAGE165].szMessage, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);	
						}
					
					}

					else if(eMouseCheck==MOUSE_OVER)					
					{
						byChk = __MOUSE_POINTER_BUTTON__;
					}
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;

		//��Ƽ �ý��� ģ�� �߰� (����â 2005.02.28)
		case 107:											 
		{
			BYTE byChk = 0;
			
			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(g_pPartyUserHash->GetCount() == 6)
				{
					if(eMouseCheck==MOUSE_DOWN)    
					{ 
				
					} 
					else if(eMouseCheck==MOUSE_UP)
					{
						if(g_pMessengerOnLineHash->GetCount()+g_pMessengerOffLineHash->GetCount()<MAX_MESSENGER_USER)
						{
							ListNode<PARTY_USER>* pNode = g_pPartyUserHash->GetHead();
							pNode = pNode->pNext;
							pNode = pNode->pNext;
							pNode = pNode->pNext;
							pNode = pNode->pNext;
							pNode = pNode->pNext;

							LP_PARTY_USER sPartyNode = (pNode->pData);
							
							LP_MESSENGER_USER lpMessengerUser = g_pMessengerOnLineHash->GetData(sPartyNode->dwUserIndex);
							
							if(lpMessengerUser)	
							{
								// "�̹� ��ϵǾ� �ֽ��ϴ�."
								DisplayMessageAdd(g_Message[ETC_MESSAGE251].szMessage, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND); 
							}
							else  
							{
								CTWS_MESSENGER_USERADD	pMessengerPacket;							
								memset(pMessengerPacket.szCharacterName, 0, sizeof(pMessengerPacket.szCharacterName));
								pMessengerPacket.dwUserIndex	= sPartyNode->dwUserIndex;  
								__lstrcpyn(pMessengerPacket.szCharacterName, sPartyNode->szName, MAX_CHARACTER_NAME_REAL_LENGTH); 
								g_pNet->SendMsg((char*)&pMessengerPacket, pMessengerPacket.GetPacketSize(), SERVER_INDEX_WORLD);
					
								char szInfo[0xff] = {0,};
								wsprintf(szInfo, g_Message[ETC_MESSAGE830].szMessage, sPartyNode->szName); // "%s �Կ��� ģ�� ��� �޼����� ���½��ϴ�."sPartyNode->szName
								DisplayMessageAdd(szInfo, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);		// "�̹� ��ϵǾ� �ֽ��ϴ�."
							}														
						}
						else
						{
							// "�� �̻� ģ���� ����� �� �����ϴ�."					
							DisplayMessageAdd(g_Message[ETC_MESSAGE165].szMessage, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);	
						}
					
					}

					else if(eMouseCheck==MOUSE_OVER)					
					{
						byChk = __MOUSE_POINTER_BUTTON__;
					}
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;

		//��Ƽ �ý��� ģ�� �߰� (����â 2005.02.28)
		case 108:											 
		{
			BYTE byChk = 0;
			
			if(m_byGroupTab==GROUP_TAB_PARTY)
			{
				if(g_pPartyUserHash->GetCount() == 7)
				{
					if(eMouseCheck==MOUSE_DOWN)    
					{ 
				
					} 
					else if(eMouseCheck==MOUSE_UP)
					{
						if(g_pMessengerOnLineHash->GetCount()+g_pMessengerOffLineHash->GetCount()<MAX_MESSENGER_USER)
						{
							ListNode<PARTY_USER>* pNode = g_pPartyUserHash->GetHead();
							pNode = pNode->pNext;
							pNode = pNode->pNext;
							pNode = pNode->pNext;
							pNode = pNode->pNext;
							pNode = pNode->pNext;
							pNode = pNode->pNext;

							LP_PARTY_USER sPartyNode = (pNode->pData);
							
							LP_MESSENGER_USER lpMessengerUser = g_pMessengerOnLineHash->GetData(sPartyNode->dwUserIndex);
							
							if(lpMessengerUser)	
							{
								// "�̹� ��ϵǾ� �ֽ��ϴ�."
								DisplayMessageAdd(g_Message[ETC_MESSAGE251].szMessage, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND); 
							}
							else  
							{
								CTWS_MESSENGER_USERADD	pMessengerPacket;							
								memset(pMessengerPacket.szCharacterName, 0, sizeof(pMessengerPacket.szCharacterName));
								pMessengerPacket.dwUserIndex	= sPartyNode->dwUserIndex;  
								__lstrcpyn(pMessengerPacket.szCharacterName, sPartyNode->szName, MAX_CHARACTER_NAME_REAL_LENGTH); 
								g_pNet->SendMsg((char*)&pMessengerPacket, pMessengerPacket.GetPacketSize(), SERVER_INDEX_WORLD);
					
								char szInfo[0xff] = {0,};
								wsprintf(szInfo, g_Message[ETC_MESSAGE830].szMessage, sPartyNode->szName); // "%s �Կ��� ģ�� ��� �޼����� ���½��ϴ�."sPartyNode->szName
								DisplayMessageAdd(szInfo, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);		// "�̹� ��ϵǾ� �ֽ��ϴ�."
							}														
						}
						else
						{
							// "�� �̻� ģ���� ����� �� �����ϴ�."					
							DisplayMessageAdd(g_Message[ETC_MESSAGE165].szMessage, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);	
						}
					
					}

					else if(eMouseCheck==MOUSE_OVER)					
					{
						byChk = __MOUSE_POINTER_BUTTON__;
					}
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;  
	
	
	case GUILDWAR_CHECK_RETURN_APPLY:
		{
			if(m_byGroupTab==GROUP_TAB_GUILDWAR)
			{
				if(m_byStatus==GROUP_GUILDWAR_RANK)
				{
					if(eMouseCheck==MOUSE_UP)
					{
						SetRender(SPR_OBJ_GUILDWAR_APPLY1, FALSE);//�ӽ�
						SetRender(SPR_OBJ_GUILDWAR_APPLY2, TRUE);
						SetRender(SPR_OBJ_GUILDWAR_APPLY3, FALSE);

						/*
						if(!g_pThisDungeon->IsStadium() && !g_pMainPlayer->m_bPlayerShop && 
							!g_pMainPlayer->m_bMatching)//����� ���� �ƴϰ� �����ߵ� �ƴϰ� pvp�ߵ� �ƴϰ�
						{
							SetRender(SPR_OBJ_GUILDWAR_APPLY1, FALSE);
							SetRender(SPR_OBJ_GUILDWAR_APPLY2, TRUE);
							SetRender(SPR_OBJ_GUILDWAR_APPLY3, FALSE);
						
							if(m_nGuildWarRankListSelect > -1 )
							{
								CGuildWarRequest* pGuildWarRequest = CGuildWarRequest::GetInstance();
								if(!pGuildWarRequest->GetActive())
								{
									ZeroMemory(pGuildWarRequest->m_szRequestName, sizeof(pGuildWarRequest->m_szRequestName));
									lstrcpy(pGuildWarRequest->m_szRequestName, m_LadderList[m_nGuildWarRankListSelect].szGuildMaster);
									pGuildWarRequest->SetRankData(m_LadderList[m_nGuildWarRankListSelect]);
									pGuildWarRequest->m_bReRequestEnable = FALSE;
									pGuildWarRequest->SetActive(TRUE);
								}
							}
						}
						*/
						//�����̴�., �ϴ��� �Ⱦ���...
					}
					else if(eMouseCheck==MOUSE_DOWN)
					{
						SetRender(SPR_OBJ_GUILDWAR_APPLY1, FALSE);
						SetRender(SPR_OBJ_GUILDWAR_APPLY2, FALSE);
						SetRender(SPR_OBJ_GUILDWAR_APPLY3, TRUE);
						m_byBtnChallange = 1;
					}
					else if(eMouseCheck==MOUSE_OVER)
					{
						SetRender(SPR_OBJ_GUILDWAR_APPLY1, FALSE);
						SetRender(SPR_OBJ_GUILDWAR_APPLY2, TRUE);
						SetRender(SPR_OBJ_GUILDWAR_APPLY3, FALSE);
						m_byBtnChallange = 2;
					}
				}
				else if(m_byStatus==GROUP_GUILDWAR_LIST)
				{
					if(eMouseCheck==MOUSE_UP)
					{
						SetRender(SPR_OBJ_GUILDWAR_OBSERVE1, FALSE);//�ӽ�
						SetRender(SPR_OBJ_GUILDWAR_OBSERVE2, TRUE);
						SetRender(SPR_OBJ_GUILDWAR_OBSERVE3, FALSE);

						if(!g_pThisDungeon->IsStadium() && !g_pMainPlayer->m_bPlayerShop && 
							!g_pMainPlayer->m_bMatching)//����� ���� �ƴϰ� �����ߵ� �ƴϰ� pvp�ߵ� �ƴϰ�
						{
							SetRender(SPR_OBJ_GUILDWAR_OBSERVE1, FALSE);
							SetRender(SPR_OBJ_GUILDWAR_OBSERVE2, TRUE);
							SetRender(SPR_OBJ_GUILDWAR_OBSERVE3, FALSE);

							if(m_nGuildWarListSelect > -1)
							{
								TEAM_MATCH_PACKET TeamMatch;
								TeamMatch.Body.ObserveIndex.iObserveIndex = m_nGuildWarListSelect+1;
								TeamMatch.Body.ObserveIndex.wDungeonID = m_GuildWarList[m_nGuildWarListSelect].wDungeonID;
								TeamMatch.Body.ObserveIndex.byLayerIndex = m_GuildWarList[m_nGuildWarListSelect].byLayerIndex;
								TeamMatch.Body.ObserveIndex.match_type = m_GuildWarList[m_nGuildWarListSelect].match_type;
								Cmd_CTWS_GuildWarPacket(TeamMatch, CTWS_TM_GUILD_WAR_OBSERVE);
							}
						}
						//�����̴�., �ϴ��� �Ⱦ���...
					}
					else if(eMouseCheck==MOUSE_DOWN)
					{
						SetRender(SPR_OBJ_GUILDWAR_OBSERVE1, FALSE);
						SetRender(SPR_OBJ_GUILDWAR_OBSERVE2, FALSE);
						SetRender(SPR_OBJ_GUILDWAR_OBSERVE3, TRUE);
						m_byBtnOvserve = 1;
					}
					else if(eMouseCheck==MOUSE_OVER)
					{
						SetRender(SPR_OBJ_GUILDWAR_OBSERVE1, FALSE);
						SetRender(SPR_OBJ_GUILDWAR_OBSERVE2, TRUE);
						SetRender(SPR_OBJ_GUILDWAR_OBSERVE3, FALSE);
						m_byBtnOvserve = 2;
					}
				}
			}
		}
		break;

	case GUILDWAR_CHECK_RETURN_MATCH:
		{
			if(m_byGroupTab==GROUP_TAB_GUILDWAR)
			{
				if(m_byStatus==GROUP_GUILDWAR_MATCH)
				{
					if(eMouseCheck==MOUSE_DOWN)
					{
						if(!g_pThisDungeon->IsStadium() && !g_pMainPlayer->m_bPlayerShop && 
							!g_pMainPlayer->m_bMatching)//����� ���� �ƴϰ� �����ߵ� �ƴϰ� pvp�ߵ� �ƴϰ�
						{
							if(m_bGuildWarPacketSendEnable)
							{
								if(m_bGuildWarMatchEnable)
								{
									SetRender(SPR_OBJ_GUILDWAR_AUTOMATCHING1, FALSE);
									SetRender(SPR_OBJ_GUILDWAR_AUTOMATCHING2, TRUE);
									SetRender(SPR_OBJ_GUILDWAR_AUTOMATCH_V, TRUE);
									m_bGuildWarMatchEnable = !m_bGuildWarMatchEnable;//�ӽ�, �������� ���� �������� ������ ����

									//������ ��Ī ���縦 ������.
									m_bGuildWarPacketSendEnable = FALSE;
									TEAM_MATCH_PACKET TeamMatch;
									TeamMatch.Body.Condtion.iMatchType = M_T_NONE;
									TeamMatch.Body.Condtion.iMatchGuildNumber = 0;
									TeamMatch.Body.Condtion.iMatchGuildType = T_T_NONE;
									Cmd_CTWS_GuildWarPacket(TeamMatch, CTWS_TM_REMOVE);
								}
								else
								{
									SetRender(SPR_OBJ_GUILDWAR_AUTOMATCHING1, TRUE);
									SetRender(SPR_OBJ_GUILDWAR_AUTOMATCHING2, FALSE);
									SetRender(SPR_OBJ_GUILDWAR_AUTOMATCH_V, FALSE);
									m_bGuildWarMatchEnable = !m_bGuildWarMatchEnable;//�ӽ�, �������� ���� �������� ������ ����

									//������ ��Ī ������ ������.
									m_bGuildWarPacketSendEnable = FALSE;
									TEAM_MATCH_PACKET TeamMatch;
									TeamMatch.Body.Condtion.iMatchType = GetFormConvert((int)m_bynGuildWarForm);
									TeamMatch.Body.Condtion.iMatchGuildNumber = GetWarNumber((int)m_byGuildWarNumber);
									TeamMatch.Body.Condtion.iMatchGuildType = GetTypeConvert((int)m_byGuildWarType);
									Cmd_CTWS_GuildWarPacket(TeamMatch, CTWS_TM_INSERT);
								}
							}
						}
					}
				}
			}
		}
		break;

	case GUILDWAR_CHECK_DROPDOWN_BUTTON_FORM:
		{
			if(m_byGroupTab==GROUP_TAB_GUILDWAR)
			{
				if(eMouseCheck==MOUSE_UP)
				{
					m_byGuildWarDropDownButtonStatus = MOUSE_NONE;
				}
				else if(eMouseCheck==MOUSE_DOWN)
				{
					if(!m_bGuildWarMatchEnable)//��Ī ���۵����� ����
					{
						if(!g_pThisDungeon->IsStadium() && !g_pMainPlayer->m_bPlayerShop && 
								!g_pMainPlayer->m_bMatching)//����� ���� �ƴϰ� �����ߵ� �ƴϰ� pvp�ߵ� �ƴϰ�
						{
							m_byGuildWarDropDownButtonStatus = MOUSE_DOWN;
							if(m_byGuildWarDropDownEnableNum == 1)
								m_byGuildWarDropDownEnableNum = 0;
							else
								m_byGuildWarDropDownEnableNum = 1;
							SetStatus(GROUP_GUILDWAR_MATCH);
						}
					}
				}
				else if(eMouseCheck==MOUSE_OVER)
				{
					m_byGuildWarDropDownButtonStatus = MOUSE_OVER;
				}
			}
		}
		break;

	case GUILDWAR_CHECK_DROPDOWN_BUTTON_NUMBER:
		{
			if(m_byGroupTab==GROUP_TAB_GUILDWAR)
			{
				if(eMouseCheck==MOUSE_UP)
				{
					m_byGuildWarDropDownButtonStatus = MOUSE_NONE;
				}
				else if(eMouseCheck==MOUSE_DOWN)
				{
					if(!m_bGuildWarMatchEnable)//��Ī ���۵����� ����
					{
						if(!g_pThisDungeon->IsStadium() && !g_pMainPlayer->m_bPlayerShop && 
								!g_pMainPlayer->m_bMatching)//����� ���� �ƴϰ� �����ߵ� �ƴϰ� pvp�ߵ� �ƴϰ�
						{
							m_byGuildWarDropDownButtonStatus = MOUSE_DOWN;
							if(m_byGuildWarDropDownEnableNum == 2)
								m_byGuildWarDropDownEnableNum = 0;
							else
								m_byGuildWarDropDownEnableNum = 2;
							SetStatus(GROUP_GUILDWAR_MATCH);
						}
					}
				}
				else if(eMouseCheck==MOUSE_OVER)
				{
					m_byGuildWarDropDownButtonStatus = MOUSE_OVER;
				}
			}
		}
		break;

	case GUILDWAR_CHECK_DROPDOWN_BUTTON_TYPE:
		{
			if(m_byGroupTab==GROUP_TAB_GUILDWAR)
			{
				if(eMouseCheck==MOUSE_UP)
				{
					m_byGuildWarDropDownButtonStatus = MOUSE_NONE;
				}
				else if(eMouseCheck==MOUSE_DOWN)
				{
					if(!m_bGuildWarMatchEnable)//��Ī ���۵����� ����
					{
						if(!g_pThisDungeon->IsStadium() && !g_pMainPlayer->m_bPlayerShop && 
								!g_pMainPlayer->m_bMatching)//����� ���� �ƴϰ� �����ߵ� �ƴϰ� pvp�ߵ� �ƴϰ�
						{
							m_byGuildWarDropDownButtonStatus = MOUSE_DOWN;
							if(m_byGuildWarDropDownEnableNum == 3)
								m_byGuildWarDropDownEnableNum = 0;
							else
								m_byGuildWarDropDownEnableNum = 3;
							SetStatus(GROUP_GUILDWAR_MATCH);
						}
					}
				}
				else if(eMouseCheck==MOUSE_OVER)
				{
					m_byGuildWarDropDownButtonStatus = MOUSE_OVER;
				}
			}
		}
		break;

		//�������� +7���� ����Ǿ� �ִ�(�ϴ��� 2,4,3)
	case GUILDWAR_CHECK_DROPDOWN_FORM:
//	case GUILDWAR_CHECK_DROPDOWN_FORM+1:
	case GUILDWAR_CHECK_DROPDOWN_NUMBER:
//	case GUILDWAR_CHECK_DROPDOWN_NUMBER+1:
//	case GUILDWAR_CHECK_DROPDOWN_NUMBER+2:
//	case GUILDWAR_CHECK_DROPDOWN_NUMBER+3:
	case GUILDWAR_CHECK_DROPDOWN_TYPE:
	case GUILDWAR_CHECK_DROPDOWN_TYPE+1:
	case GUILDWAR_CHECK_DROPDOWN_TYPE+2:
		{
			if(m_byGroupTab==GROUP_TAB_GUILDWAR)
			{
				if(m_byGuildWarDropDownEnableNum != 0)
				{
					if(m_byGuildWarDropDownEnableNum == 1)
					{
						if(eMouseCheck==MOUSE_DOWN)
						{
							m_bynGuildWarForm = nRt - GUILDWAR_CHECK_DROPDOWN_FORM;
						}
						else if(eMouseCheck==MOUSE_OVER)
						{
						}
					}
					else if(m_byGuildWarDropDownEnableNum == 2)
					{
						if(eMouseCheck==MOUSE_DOWN)
						{
							m_byGuildWarNumber = nRt - GUILDWAR_CHECK_DROPDOWN_NUMBER;
						}
						else if(eMouseCheck==MOUSE_OVER)
						{
						}
					}
					else if(m_byGuildWarDropDownEnableNum == 3)
					{
						if(eMouseCheck==MOUSE_DOWN)
						{
							if(g_pMainPlayer->m_dwGuildId)
							{
								m_byGuildWarType = nRt - GUILDWAR_CHECK_DROPDOWN_TYPE;
							}
							else
							{
								m_byGuildWarType = 1;//��� ������ ������ �� �����.
							}
						}
						else if(eMouseCheck==MOUSE_OVER)
						{
						}
					}
					if(eMouseCheck==MOUSE_DOWN)
					{
						m_byGuildWarDropDownEnableNum = 0;
						SetStatus(GROUP_GUILDWAR_MATCH);
					}
				}
			}
		}
		break;
	//��� ��ũ�� ����� ����Ʈ�϶� �� �ؽ�Ʈ ����
	case GUILDWAR_CHECK_TEXT_LINE_NUMBER:
	case GUILDWAR_CHECK_TEXT_LINE_NUMBER+1:
	case GUILDWAR_CHECK_TEXT_LINE_NUMBER+2:
	case GUILDWAR_CHECK_TEXT_LINE_NUMBER+3:
	case GUILDWAR_CHECK_TEXT_LINE_NUMBER+4:
	case GUILDWAR_CHECK_TEXT_LINE_NUMBER+5:
	case GUILDWAR_CHECK_TEXT_LINE_NUMBER+6:
	case GUILDWAR_CHECK_TEXT_LINE_NUMBER+7:
	case GUILDWAR_CHECK_TEXT_LINE_NUMBER+8:
	case GUILDWAR_CHECK_TEXT_LINE_NUMBER+9:
	case GUILDWAR_CHECK_TEXT_LINE_NUMBER+10:
	case GUILDWAR_CHECK_TEXT_LINE_NUMBER+11:
	case GUILDWAR_CHECK_TEXT_LINE_NUMBER+12:
	case GUILDWAR_CHECK_TEXT_LINE_NUMBER+13:
	case GUILDWAR_CHECK_TEXT_LINE_NUMBER+14:
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_byGroupTab==GROUP_TAB_GUILDWAR)
				{
					if( m_byStatus == GROUP_GUILDWAR_RANK)
					{
						if(m_nGuildWarRankListTotalCount > nRt - GUILDWAR_CHECK_TEXT_LINE_NUMBER)
						{
							m_nGuildWarRankListSelect = nRt - GUILDWAR_CHECK_TEXT_LINE_NUMBER;
						}
					}
					else if( m_byStatus == GROUP_GUILDWAR_LIST)
					{
						if(m_nGuildWarListTotalCount > nRt - GUILDWAR_CHECK_TEXT_LINE_NUMBER)
						{
							m_nGuildWarListSelect = nRt - GUILDWAR_CHECK_TEXT_LINE_NUMBER;
						}
					}
				}
			}
		}
		break;

	case GUILDWAR_CHAGE_DOUBLE_PAGEPREV://������ ������ ����(10)
		{
			BYTE byChk = 0;

			if(m_byGroupTab==GROUP_TAB_GUILDWAR && (m_byStatus == GROUP_GUILDWAR_RANK || m_byStatus == GROUP_GUILDWAR_LIST))
			{
				if(eMouseCheck==MOUSE_UP)
				{
					SetRender(SPR_OBJ_GUILDWAR_PAGEPREV_DOUBLE1, FALSE);
					SetRender(SPR_OBJ_GUILDWAR_PAGEPREV_DOUBLE2, TRUE);
					SetRender(SPR_OBJ_GUILDWAR_PAGEPREV_DOUBLE3, FALSE);
					if( m_byStatus == GROUP_GUILDWAR_RANK )
					{
						m_nGuildWarRankListPage-=10;
						if(m_nGuildWarRankListPage < 1)
							m_nGuildWarRankListPage = 1;
						TEAM_MATCH_PACKET TeamMatch;
						TeamMatch.Body.PageNum.iPageNumber = m_nGuildWarRankListPage;
						Cmd_CTWS_GuildWarPacket(TeamMatch, CTWS_TM_LADDER_LIST);
//						TempSetRandomData();//�ӽ�
					}
					else if(m_byStatus == GROUP_GUILDWAR_LIST)
					{
						m_nGuildWarListPage-=10;
						if(m_nGuildWarListPage < 1)
							m_nGuildWarListPage = 1;
						TEAM_MATCH_PACKET TeamMatch;
						TeamMatch.Body.PageNum.iPageNumber = m_nGuildWarListPage;
						Cmd_CTWS_GuildWarPacket(TeamMatch, CTWS_TM_GUILD_WAR_LIST);
//						TempSetRandomData();//�ӽ�
					}
					byChk = __MOUSE_POINTER_BUTTON__;
				}
				else if(eMouseCheck==MOUSE_DOWN)
				{
					SetRender(SPR_OBJ_GUILDWAR_PAGEPREV_DOUBLE1, FALSE);
					SetRender(SPR_OBJ_GUILDWAR_PAGEPREV_DOUBLE2, FALSE);
					SetRender(SPR_OBJ_GUILDWAR_PAGEPREV_DOUBLE3, TRUE);
					m_byBtnLeftDouble = 1;
					byChk = __MOUSE_POINTER_BUTTONCLICK__;
				}
				else if(eMouseCheck==MOUSE_OVER)
				{
					SetRender(SPR_OBJ_GUILDWAR_PAGEPREV_DOUBLE1, FALSE);
					SetRender(SPR_OBJ_GUILDWAR_PAGEPREV_DOUBLE2, TRUE);
					SetRender(SPR_OBJ_GUILDWAR_PAGEPREV_DOUBLE3, FALSE);
					m_byBtnLeftDouble = 2;
					byChk = __MOUSE_POINTER_BUTTON__;
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;

	case GUILDWAR_CHAGE_DOUBLE_PAGENEXT://������ �������� ����(10)
		{
			BYTE byChk = 0;

			if(m_byGroupTab==GROUP_TAB_GUILDWAR && (m_byStatus == GROUP_GUILDWAR_RANK || m_byStatus == GROUP_GUILDWAR_LIST))
			{
				if(eMouseCheck==MOUSE_UP)
				{
					SetRender(SPR_OBJ_GUILDWAR_PAGENEXT_DOUBLE1, FALSE);
					SetRender(SPR_OBJ_GUILDWAR_PAGENEXT_DOUBLE2, TRUE);
					SetRender(SPR_OBJ_GUILDWAR_PAGENEXT_DOUBLE3, FALSE);
					if( m_byStatus == GROUP_GUILDWAR_RANK )
					{
						m_nGuildWarRankListPage+=10;
						if(m_nGuildWarRankListPage > m_nGuildWarRankListMaxPage)
							m_nGuildWarRankListPage = m_nGuildWarRankListMaxPage;
						TEAM_MATCH_PACKET TeamMatch;
						TeamMatch.Body.PageNum.iPageNumber = m_nGuildWarRankListPage;
						Cmd_CTWS_GuildWarPacket(TeamMatch, CTWS_TM_LADDER_LIST);
//						TempSetRandomData();//�ӽ�
					}
					else if(m_byStatus == GROUP_GUILDWAR_LIST)
					{
						m_nGuildWarListPage+=10;
						if(m_nGuildWarListPage > m_nGuildWarListMaxPage)
							m_nGuildWarListPage = m_nGuildWarListMaxPage;
						TEAM_MATCH_PACKET TeamMatch;
						TeamMatch.Body.PageNum.iPageNumber = m_nGuildWarListPage;
						Cmd_CTWS_GuildWarPacket(TeamMatch, CTWS_TM_GUILD_WAR_LIST);
//						TempSetRandomData();//�ӽ�
					}
					byChk = __MOUSE_POINTER_BUTTON__;
				}
				else if(eMouseCheck==MOUSE_DOWN)
				{
					SetRender(SPR_OBJ_GUILDWAR_PAGENEXT_DOUBLE1, FALSE);
					SetRender(SPR_OBJ_GUILDWAR_PAGENEXT_DOUBLE2, FALSE);
					SetRender(SPR_OBJ_GUILDWAR_PAGENEXT_DOUBLE3, TRUE);
					m_byBtnRightDouble = 1;
					byChk = __MOUSE_POINTER_BUTTONCLICK__;
				}
				else if(eMouseCheck==MOUSE_OVER)
				{
					SetRender(SPR_OBJ_GUILDWAR_PAGENEXT_DOUBLE1, FALSE);
					SetRender(SPR_OBJ_GUILDWAR_PAGENEXT_DOUBLE2, TRUE);
					SetRender(SPR_OBJ_GUILDWAR_PAGENEXT_DOUBLE3, FALSE);
					m_byBtnRightDouble = 2;
					byChk = __MOUSE_POINTER_BUTTON__;
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;
	}
	
	if(MouseUp() && GetActive())
	{
		SetRender(SPR_OBJ_GROUPWND_CLOSE1, TRUE);
		SetRender(SPR_OBJ_GROUPWND_CLOSE2, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
		return CInterface::GetInstance()->GetSetWnd();

	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();

	return 0;
}

void CGroupWnd::SetPrevNext(BYTE byType)
{
	if(m_byGroupTab==GROUP_TAB_GUILD)
	{	
		if(m_byUserType==GUILD_USER_ONLINE)
		{
			int nPage = 0;

			nPage = (g_pGuildUserHash->GetCount()+1)/8;
					
			if(((g_pGuildUserHash->GetCount()+1)%8)==0)
				nPage -= 1;
				
			m_byMaxPage	= 1+nPage;		

			if(byType==__GUILD_PAGE_PREV__)
			{
				if(m_byCurPage==1)
					return;

				m_byCurPage -= 1;
			}
			else if(byType==__GUILD_PAGE_NEXT__)
			{
				if(m_byCurPage==m_byMaxPage)
					return;

				m_byCurPage += 1;
			}
			else
			{
				if(m_byCurPage==1)
					return;
			}	

			if(m_byCurPage>1)
			{
				ListNode<GUILD_USER>* pGuildUserPos = g_pGuildUserHash->GetHead();

				BYTE byIndex = 1;

				while(pGuildUserPos)
				{
					LP_GUILD_USER lpGuildUser = pGuildUserPos->pData;

					if(!lpGuildUser)
					{
						byIndex++;
						continue;
					}

					if((m_byCurPage-1)*__GUILD_MAX_USER_DISPLAY__==byIndex)
					{
						m_pGuildUserPos = pGuildUserPos;
						return;
					}
					else
						byIndex++;

					pGuildUserPos = pGuildUserPos->pNext;
				}
			}
			else
			{
				m_byCurPage		= 1;
				m_pGuildUserPos	= NULL;
			}
		}
		else if(m_byUserType==GUILD_USER_OFFLINE)
		{
			int nPage = 0;

			nPage = g_pGuildOfflineHash->GetCount()/8;

			if(nPage!=0 && nPage%8==0)
				nPage -= 1;
				
			m_byMaxPage	= 1+nPage;

			if(byType==__GUILD_PAGE_PREV__)
			{
				if(m_byCurPage==1)
					return;

				m_byCurPage -= 1;
			}
			else if(byType==__GUILD_PAGE_NEXT__)
			{
				if(m_byCurPage==m_byMaxPage)
					return;

				m_byCurPage += 1;
			}	
						
			if(m_byCurPage>1)		
			{
				BYTE byIndex = 0;

				ListNode<GUILD_OFFLINE_USER>* pGuildOfflineUserPos = g_pGuildOfflineHash->GetHead();
				
				while(pGuildOfflineUserPos)
				{
					LP_GUILD_OFFLINE_USER lpGuildOfflineUser = pGuildOfflineUserPos->pData;

					if(!lpGuildOfflineUser)
					{
						byIndex++;
						continue;
					}

					if((m_byCurPage-1)*__GUILD_MAX_USER_DISPLAY__==byIndex)
					{
						m_pGuildOfflineUserPos = pGuildOfflineUserPos;
						return;
					}
					else
						byIndex++;

					pGuildOfflineUserPos = pGuildOfflineUserPos->pNext;
				}
			}
			else
			{
				m_pGuildOfflineUserPos = g_pGuildOfflineHash->GetHead();
			}
		}
	}
	else if(m_byGroupTab==GROUP_TAB_FRIEND)
	{
		if(m_byMessengerType==FRIEND_USER_ONLINE)
		{
			int nPage = 0;

			nPage = g_pMessengerOnLineHash->GetCount()/8;
					
			if(nPage!=0 && nPage%8==0)
				nPage -= 1;
				
			m_byMaxPage	= 1+nPage;		

			if(byType==__FRIEND_PAGE_PREV__)
			{
				if(m_byCurPage==1)
					return;

				m_byCurPage -= 1;
			}
			else if(byType==__FRIEND_PAGE_NEXT__)
			{
				if(m_byCurPage==m_byMaxPage)
					return;

				m_byCurPage += 1;
			}			

			if(m_byCurPage>1)
			{
				ListNode<MESSENGER_USER>* pFriendUserPos = g_pMessengerOnLineHash->GetHead();

				BYTE byIndex = 1;

				while(pFriendUserPos)
				{
					LP_MESSENGER_USER lpFriendUser = pFriendUserPos->pData;

					if(!lpFriendUser)
					{
						byIndex++;
						continue;
					}

					if((m_byCurPage-1)*__FRIEND_MAX_USER_DISPLAY__==byIndex)
					{
						m_pMessengerOnlinePos = pFriendUserPos;
						return;
					}
					else
						byIndex++;

					pFriendUserPos = pFriendUserPos->pNext;
				}
			}
			else
			{
				m_byCurPage				= 1;
				m_pMessengerOnlinePos	= g_pMessengerOnLineHash->GetHead();
			}
		}
		else
		{
			int nPage = 0;

			nPage = g_pMessengerOffLineHash->GetCount()/8;

			if(nPage!=0 && nPage%8==0)
				nPage -= 1;
				
			m_byMaxPage	= 1+nPage;

			if(byType==__FRIEND_PAGE_PREV__)
			{
				if(m_byCurPage==1)
					return;

				m_byCurPage -= 1;
			}
			else if(byType==__FRIEND_PAGE_NEXT__)
			{
				if(m_byCurPage==m_byMaxPage)
					return;

				m_byCurPage += 1;
			}	
						
			if(m_byCurPage>1)		
			{
				BYTE byIndex = 0;

				ListNode<MESSENGER_USER>* pMessengerOfflineUserPos = g_pMessengerOffLineHash->GetHead();
				
				while(pMessengerOfflineUserPos)
				{
					LP_MESSENGER_USER lpMessengerOfflineUser = pMessengerOfflineUserPos->pData;

					if(!lpMessengerOfflineUser)
					{
						byIndex++;
						continue;
					}

					if((m_byCurPage-1)*__FRIEND_MAX_USER_DISPLAY__==byIndex)
					{
						m_pMessengerOfflinePos = pMessengerOfflineUserPos;
						return;
					}
					else
						byIndex++;

					pMessengerOfflineUserPos = pMessengerOfflineUserPos->pNext;
				}
			}
			else
			{
				m_pMessengerOfflinePos = g_pMessengerOffLineHash->GetHead();
			}
		}
	}

	SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE);
	m_nPartyUserIndex = -1;	
}

void CGroupWnd::RenderPartyInfo()
{
	// ��Ƽ ���� //
	switch(m_byRenderType)
	{
	case GROUP_RENDER_PARTYCREATE:
		lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE558].szMessage); // "���ο� ��Ƽ�� �Ἲ�մϴ�."
		m_byMsgCnt	= 1;
		break;
	case GROUP_RENDER_PARTYCREATEADD:
		lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE559].szMessage); // "��Ƽ�� �߰���ư�� ��������."
		m_byMsgCnt	= 1;		
		break;
	case GROUP_RENDER_PARTYUSERADD:
		lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE560].szMessage); // "��Ƽ���� �߰��մϴ�."
		m_byMsgCnt	= 1;		
		break;
	case GROUP_RENDER_PARTYUSERADDCLK:
		lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE561].szMessage); // "��Ƽ�� �߰��ϰ���� ĳ���͸�"
		m_byMsgCnt	= 1;
#if !IS_CHINA_LOCALIZING()
		lstrcpy(m_szInfo[1], g_Message[ETC_MESSAGE562].szMessage); // "Ŭ���ϼ���."	
		m_byMsgCnt++;
#endif		
		break;
	case GROUP_RENDER_PARTYUSERLOGOUT:
		lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE563].szMessage); // "���� �ΰ��ִ� ��Ƽ���� Ż���մϴ�."
		m_byMsgCnt	= 1;
		break;		
	case GROUP_RENDER_PARTYUSERINFO:
		lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE564].szMessage); // "��Ƽ���� ������ �����ɴϴ�."
		m_byMsgCnt	= 1;
		break;
	case GROUP_RENDER_PARTYUSERDEL:
		lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE565].szMessage); // "��Ƽ���� �����մϴ�."
		m_byMsgCnt	= 1;
		break;
	case GROUP_RENDER_PARTYMSG:
		lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE566].szMessage); // "��뿡�� �Ӹ��� �����ϴ�."
		m_byMsgCnt	= 1;
		break;
	case GROUP_RENDER_PARTYBOARDCREATE:
		lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1124].szMessage); // "�Ἲ�� ��Ƽ�� �Խ��ǿ� ��Ͻ��� ��Ƽ����"
		lstrcpy(m_szInfo[1], g_Message[ETC_MESSAGE1125].szMessage); // "������ �� �ֽ��ϴ�."
		m_byMsgCnt	= 2;
		break;
	case GROUP_RENDER_PARTYBOARDDESTROY:
		lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1126].szMessage); // "�Խ��ǿ� ��ϵǾ� �ִ� ��Ƽ�� ������ ����"
		lstrcpy(m_szInfo[1], g_Message[ETC_MESSAGE1127].szMessage); // "��ŵ�ϴ�."
		m_byMsgCnt	= 2;
		break;
	//��Ƽ ��ȭ �ý��� (����â 2005.01.25)
	case GROUP_RENDER_PARTYUSERINFO_LIST:
		lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1284].szMessage); // "�ֱ� �ڽ��� �ξ��� ��Ƽ���� "
		lstrcpy(m_szInfo[1], g_Message[ETC_MESSAGE1285].szMessage); // "����Ʈ�� �����ɴϴ�"
		m_byMsgCnt	= 2;
		break;
	//��Ƽ ��ȭ �ý��� (����â 2005.01.28) 
	case GROUP_RENDER_EXPITEMCONDITION_MSG:
		lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1286].szMessage); // "����ġ �й� �� ������ ���� �����"
		lstrcpy(m_szInfo[1], g_Message[ETC_MESSAGE1288].szMessage); // "�����մϴ�."
		m_byMsgCnt	= 2;
		break;
	//��Ƽ ��ȭ �ý��� (����â 2005.01.28) 
	case GROUP_RENDER_PMATCHCONDITION_MSG:
		lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1287].szMessage); // "���� �ϰ� ���� ��Ƽ�� ������"
		lstrcpy(m_szInfo[1], g_Message[ETC_MESSAGE1288].szMessage); // "�����մϴ�."
		m_byMsgCnt	= 2;
		break;
	//��Ƽ ��ȭ �ý��� (����â 2005.02.07)
	case GROUP_RENDER_PMATCHBUTTON_ON_MSG:
		lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1375].szMessage); //��ư�� Ŭ��(��Ȱ��ȭ)�ϸ� ��Ƽ ��Ī��
		lstrcpy(m_szInfo[1], g_Message[ETC_MESSAGE1376].szMessage); // �����մϴ�.
		m_byMsgCnt	= 2;
		break;
	//��Ƽ ��ȭ �ý��� (����â 2005.02.07)
	case GROUP_RENDER_PMATCHBUTTON_OFF_MSG:
		lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1373].szMessage); //��ư�� Ŭ��(Ȱ��ȭ)�ϸ� ��Ƽ ��Ī��
		lstrcpy(m_szInfo[1], g_Message[ETC_MESSAGE1374].szMessage); //�����մϴ�.
		m_byMsgCnt	= 2;
		break;
		
		
	}	

	if(m_byRenderType!=GROUP_RENDER_NONE)
	{
		if(m_bInfo!=TRUE)
		{
			for(int i = 0; i < m_byMsgCnt; i++)
			{
				int nSize = lstrlen(m_szInfo[i]);
				RenderFont(m_szInfo[i], m_fPosX+10, m_fPosX+10+(nSize*7), m_fPosZ+419+(i*20), m_fPosZ+433+(i*20), GetStartOrder()+4);
			}		
		}
	}

	//��Ƽ �ý���(����â 2005.03.02)
	if(m_byGroupTab==GROUP_TAB_PARTY)
	{
		if(m_byStatus==GROUP_PARTY_PARTYUSERINFO2)
		{
			wsprintf(m_szInfo[0], "%u / %u", m_byCurPage, PARTY_FRIEND_MAX_PAGE);
			
			int nLen = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+140-(nLen*3), m_fPosX+140+(nLen*7), m_fPosZ+385, m_fPosZ+399, GetStartOrder()+4);	
		}
	}

	// �ο� //
	BYTE byParty = 0;
	
	if(g_pMainPlayer->m_dwPartyId!=0)			
		byParty = BYTE(g_pPartyUserHash->GetCount() + 1);	//KBS 030827

	wsprintf(m_szInfo[0], g_Message[ETC_MESSAGE567].szMessage, byParty); // "��Ƽ�ο� : %u / 8"

	int nSize = lstrlen(m_szInfo[0]);	
	RenderFont(m_szInfo[0], m_fPosX+10, m_fPosX+10+(nSize*7), m_fPosZ+54, m_fPosZ+68, GetStartOrder()+4);

	DWORD dwLev = 0;

	// ��Ƽ�� ���� //
	
	if(g_pMainPlayer->m_dwPartyId!=0)
	{
		
		DWORD dwColor = 0xffffffff;
		
	//  m_byStatus!=GROUP_PARTY__CONDITIONLIST_EXP5 && m_byStatus!=GROUP_PARTY__CONDITIONLIST_ITEM5
		if(m_byStatus!=GROUP_PARTY_EXPITEMCONDITION1 && !(m_byStatus>=GROUP_PARTY__CONDITIONLIST_EXP1 && GROUP_PARTY__CONDITIONLIST_ITEM4 >= m_byStatus)
			&& !(m_byStatus>=GROUP_PARTY_PMATCHCONDITION1 && GROUP_PARTY_PMATCHCONDITION_PMAINCHO5 >= m_byStatus) && m_byStatus!=GROUP_PARTY_PMATCHCONDITION_OK_ON 
			&& m_byStatus!=GROUP_PARTY_PMATCHCONDITION_OK_OFF && m_byStatus!=GROUP_PARTY_PARTYUSERINFO2 && m_byStatus!=GROUP_PARTY_PARTYUSERINFO1)         //��Ƽ ��ȭ �ý��� ����â(2005.01.29)
	{
			
			if(g_pMainPlayer->m_bPartyLeader==1) 

			dwColor = 0xFFFFC309;

		// �ڱ��ڽ� //
		switch(g_pMainPlayer->m_wClass)
		{
		case 1: lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE7].szMessage);	break; // "������"
		case 2: lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE9].szMessage);	break; // "������Ʈ"
		case 3: lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE10].szMessage);	break; // "���ӳ�"
		case 4: lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE11].szMessage);	break; // "������"	
		case 5: lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE12].szMessage);	break; //"�Ҽ�����"		
		}
		
		int nSize = lstrlen(m_szInfo[0]);
		RenderFont(m_szInfo[0], m_fPosX+50, m_fPosX+50+(nSize*7), m_fPosZ+108, m_fPosZ+122, GetStartOrder()+4, dwColor);
		
		dwLev += g_pMainPlayer->m_dwLevel;

		wsprintf(m_szInfo[0], "%u", g_pMainPlayer->m_dwLevel);
		
		nSize = lstrlen(m_szInfo[0]);
		RenderFont(m_szInfo[0], m_fPosX+116-(nSize*1), m_fPosX+116+(nSize*7), m_fPosZ+108, m_fPosZ+122, GetStartOrder()+4, dwColor);		
		
		lstrcpy(m_szInfo[0], g_pMainPlayer->m_szName);

		nSize = lstrlen(m_szInfo[0]);
		RenderFont(m_szInfo[0], m_fPosX+190-(nSize*3), m_fPosX+190+(nSize*7), m_fPosZ+108, m_fPosZ+122, GetStartOrder()+4, dwColor);
							
		ListNode<PARTY_USER>* pNode = g_pPartyUserHash->GetHead();

		BYTE byIndex = 0;
		BYTE byFriendAddIndex = 0;	//��Ƽ �ý��� ģ�� �߰� (����â 2005.02.24)
		
		while(pNode)
		{
			LP_PARTY_USER sPartyNode = pNode->pData;

			dwColor = 0xffffffff;

			if(sPartyNode->byLeader==1)
				dwColor = 0xFFFFC309;

			// �ڱ��ڽ� //
			switch(sPartyNode->wClass)
			{
			case 1: wsprintf(m_szInfo[0], g_Message[ETC_MESSAGE7].szMessage);	break; // "������"
			case 2: wsprintf(m_szInfo[0], g_Message[ETC_MESSAGE9].szMessage);	break; // "������Ʈ"
			case 3: wsprintf(m_szInfo[0], g_Message[ETC_MESSAGE10].szMessage);	break; // "���ӳ�"
			case 4: wsprintf(m_szInfo[0], g_Message[ETC_MESSAGE11].szMessage);	break; // "������"	
			case 5: wsprintf(m_szInfo[0], g_Message[ETC_MESSAGE12].szMessage);	break; // "�Ҽ�����"			
			}
		
			int nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+50, m_fPosX+50+(nSize*7), m_fPosZ+143+(byIndex*35), m_fPosZ+157+(byIndex*35), GetStartOrder()+4, dwColor);		
			
			dwLev += sPartyNode->wLevel;

			wsprintf(m_szInfo[0], "%u", sPartyNode->wLevel);
		
			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+116-(nSize*1), m_fPosX+116+(nSize*7), m_fPosZ+143+(byIndex*35), m_fPosZ+157+(byIndex*35), GetStartOrder()+4, dwColor);
						
			lstrcpy(m_szInfo[0], sPartyNode->szName);

			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+190-(nSize*3), m_fPosX+190+(nSize*7), m_fPosZ+143+(byIndex*35), m_fPosZ+157+(byIndex*35), GetStartOrder()+4, dwColor);
			
			//��Ƽ �ý��� ģ�� �߰� (����â 2005.02.22)
		    
			LP_MESSENGER_USER lpMessengerUser = g_pMessengerOnLineHash->GetData(sPartyNode->dwUserIndex);

			//��Ƽ �ý��� ģ�� �߰� (����â 2005.02.24)
			if(lpMessengerUser)
			{
				SetRender(SPR_OBJ_GROUPWND_FRIENDADD_OFF1 + byFriendAddIndex, TRUE);
			}
			else
			{	
				SetRender(SPR_OBJ_GROUPWND_FRIENDADD_ON1 + byFriendAddIndex, TRUE);
				
			}
			

			byIndex++;
			byFriendAddIndex++;	//��Ƽ �ý��� ģ�� �߰� (����â 2005.02.24)
			
			pNode = pNode->pNext;
		}
	}
	
		

		// ���� //
		if(byParty!=0)
		{
			WORD wLev = (WORD)dwLev/(WORD)byParty;

			wsprintf(m_szInfo[0], "%s : %u", g_Message[ETC_MESSAGE472].szMessage, wLev); // "Level"

			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+160, m_fPosX+160+(nSize*7), m_fPosZ+54, m_fPosZ+68, GetStartOrder()+4, dwColor);
		}						
	}
	
	//��Ƽ ��ȭ �ý��� (����â 2005.01.26)
	if(m_byGroupTab==GROUP_TAB_PARTY)
	{
	
		if(m_byStatus==GROUP_PARTY_EXPITEMCONDITION1 || (m_byStatus>=GROUP_PARTY__CONDITIONLIST_EXP1 && GROUP_PARTY__CONDITIONLIST_ITEM4 >= m_byStatus)
			|| m_byStatus==GROUP_PARTY__CONDITIONLIST_EXP5 || m_byStatus==GROUP_PARTY__CONDITIONLIST_ITEM5)
		{
			DWORD dwColor = 0xffffffff;

			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1289].szMessage);  // "��Ƽ �÷��̽� ����ġ �й� ��İ� "
			lstrcpy(m_szInfo[1], g_Message[ETC_MESSAGE1290].szMessage);  // "������ ���� ����� �����մϴ�."

			int nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+50, m_fPosX+50+(nSize*7), m_fPosZ+108, m_fPosZ+108+14, GetStartOrder()+4, dwColor);
			nSize = lstrlen(m_szInfo[1]);
			RenderFont(m_szInfo[1], m_fPosX+50, m_fPosX+50+(nSize*7), m_fPosZ+125, m_fPosZ+125+14, GetStartOrder()+4, dwColor);

			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1292].szMessage);  // "����ġ �й� ��� ����"

			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+50, m_fPosX+50+(nSize*7), m_fPosZ+175, m_fPosZ+175+14, GetStartOrder()+4, dwColor);

			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1293].szMessage);  // "������ ���� ��� ����"

			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+50, m_fPosX+50+(nSize*7), m_fPosZ+255, m_fPosZ+255+14, GetStartOrder()+4, dwColor);

		}
		else if(m_byStatus==GROUP_PARTY_PMATCHCONDITION1 || (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_LEV1 && GROUP_PARTY_PMATCHCONDITION_PMAINCHO4 >= m_byStatus)
			|| (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_LEV5 && GROUP_PARTY_PMATCHCONDITION_PMAINCHO5 >= m_byStatus) || m_byStatus==GROUP_PARTY_PMATCHCONDITION_OK_ON 
			|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_OK_OFF)
		{
			DWORD dwColor = 0xffffffff;

			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1301].szMessage);  // "�˻��� ��Ƽ�� ������ �����մϴ�."
		
			int nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+50, m_fPosX+50+(nSize*7), m_fPosZ+108, m_fPosZ+108+14, GetStartOrder()+4, dwColor);
			
			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1302].szMessage);  // "��Ƽ�� ���� ���� ����"

			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+50, m_fPosX+50+(nSize*7), m_fPosZ+138, m_fPosZ+138+14, GetStartOrder()+4, dwColor);

			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1292].szMessage); //  "����ġ �й� ��� ����"
			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+50, m_fPosX+50+(nSize*7), m_fPosZ+190, m_fPosZ+190+14, GetStartOrder()+4, dwColor);

			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1293].szMessage); // "������ ���� ��� ����"
			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+50, m_fPosX+50+(nSize*7), m_fPosZ+245, m_fPosZ+245+14, GetStartOrder()+4, dwColor);

			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1303].szMessage); // "��Ƽ ���� ��� ����"
			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+50, m_fPosX+50+(nSize*7), m_fPosZ+300, m_fPosZ+300+14, GetStartOrder()+4, dwColor);


		}
	}

	//��Ƽ ��ȭ �ý��� (����â 2005.01.28) 
	if(m_byGroupTab==GROUP_TAB_PARTY)
	{
		if(m_byStatus==GROUP_PARTY_EXPITEMCONDITION1 || (m_byStatus>=GROUP_PARTY__CONDITIONLIST_EXP1 && GROUP_PARTY__CONDITIONLIST_ITEM4 >= m_byStatus)
			|| m_byStatus==GROUP_PARTY__CONDITIONLIST_EXP5 || m_byStatus==GROUP_PARTY__CONDITIONLIST_ITEM5 )
		{	
			if(m_nExpDistrib == PARTY_CONFIG_EXP_SHARE)
			{
				DWORD dwColor = 0xffffffff;

				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1295].szMessage);  //"���� �й�"
				int nSize = lstrlen(m_szInfo[0]);
				RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+202, m_fPosZ+202+14, GetStartOrder()+7, dwColor);

			}
			else if(m_nExpDistrib == PARTY_CONFIG_EXP_LEVEL)
			{
				DWORD dwColor = 0xffffffff;

				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1296].szMessage);  // "������ �й�"
				nSize = lstrlen(m_szInfo[0]);
				RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+202, m_fPosZ+202+14, GetStartOrder()+7, dwColor);

			}
			else if(m_nExpDistrib == PARTY_CONFIG_EXP_KILLER)
			{
				DWORD dwColor = 0xffffffff;

				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1297].szMessage);  // "���ʽ� ����ġ�� �й�"
				nSize = lstrlen(m_szInfo[0]);
				RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+202, m_fPosZ+202+14, GetStartOrder()+7, dwColor);

			}			
			
		}
		if(g_pMainPlayer->m_dwPartyId ==0)
		{	
			m_nExpDistrib = PARTY_CONFIG_EXP_SHARE; 
			m_nItemRouting = PARTY_CONFIG_ITEM_FIRST;
		}
	

		if(m_byStatus==GROUP_PARTY__CONDITIONLIST_EXP2 || m_byStatus==GROUP_PARTY__CONDITIONLIST_EXP1)
		{ 
			DWORD dwColor = 0xffffffff;

			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1295].szMessage);  //"���� �й�"

			int nSize = lstrlen(m_szInfo[0]);


			RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+217, m_fPosZ+217+14, GetStartOrder()+7, dwColor);


			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1296].szMessage);  // "������ �й�"

			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+235, m_fPosZ+235+14, GetStartOrder()+7, dwColor);


			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1297].szMessage);  // "���ʽ� ����ġ�� �й�"

			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+253, m_fPosZ+253+14, GetStartOrder()+7, dwColor);

		}
	}

	//��Ƽ ��ȭ �ý��� (����â 2005.01.28) 
	if(m_byGroupTab==GROUP_TAB_PARTY)
	{
		if(m_byStatus==GROUP_PARTY_EXPITEMCONDITION1 || (m_byStatus>=GROUP_PARTY__CONDITIONLIST_EXP1 && GROUP_PARTY__CONDITIONLIST_ITEM4 >= m_byStatus)
				|| m_byStatus==GROUP_PARTY__CONDITIONLIST_EXP5 || m_byStatus==GROUP_PARTY__CONDITIONLIST_ITEM5 )
		{
			if(m_nItemRouting == PARTY_CONFIG_ITEM_FIRST)
			{
				DWORD dwColor = 0xffffffff;

				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1298].szMessage);  // "������ �켱"
				nSize = lstrlen(m_szInfo[0]);
				RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+282, m_fPosZ+282+14, GetStartOrder()+7, dwColor);

			}
			else if(m_nItemRouting == PARTY_CONFIG_ITEM_KILL)
			{
				DWORD dwColor = 0xffffffff;
				
				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1299].szMessage);  // "������ Ÿ���� �켱"
				nSize = lstrlen(m_szInfo[0]);
				RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+282, m_fPosZ+282+14, GetStartOrder()+7, dwColor);

			}
			else if(m_nItemRouting == PARTY_CONFIG_ITEM_RANDOM)
			{
				DWORD dwColor = 0xffffffff;

				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1300].szMessage);  // "���� ����"
				nSize = lstrlen(m_szInfo[0]);
				RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+282, m_fPosZ+282+14, GetStartOrder()+7, dwColor);

			}
		}

		if(m_byStatus==GROUP_PARTY__CONDITIONLIST_ITEM2 || m_byStatus==GROUP_PARTY__CONDITIONLIST_ITEM1)
		{ 
			DWORD dwColor = 0xffffffff;

			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1298].szMessage);  // "������ �켱"

			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+297, m_fPosZ+297+14, GetStartOrder()+7, dwColor);


			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1299].szMessage);  // "������ Ÿ���� �켱"

			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+315, m_fPosZ+315+14, GetStartOrder()+7, dwColor);


			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1300].szMessage);  // "���� ����"

			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+333, m_fPosZ+333+14, GetStartOrder()+7, dwColor);


		}

		if(m_byStatus==GROUP_PARTY_PMATCHCONDITION1 || (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_LEV1 && GROUP_PARTY_PMATCHCONDITION_PMAINCHO4 >= m_byStatus)
			||(m_byStatus>=GROUP_PARTY_PMATCHCONDITION_LEV5 && GROUP_PARTY_PMATCHCONDITION_PMAINCHO5 >= m_byStatus) )
		{
			if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_LEV2 || m_byStatus==GROUP_PARTY_PMATCHCONDITION_LEV1)
			{

			DWORD dwColor = 0xffffffff;

			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1304].szMessage);  // "3������"

			int nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+180, m_fPosZ+180+14, GetStartOrder()+7, dwColor);


			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1305].szMessage);  // "5������"

			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+198, m_fPosZ+198+14, GetStartOrder()+7, dwColor);


			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1306].szMessage);  // "10������"

			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+216, m_fPosZ+216+14, GetStartOrder()+7, dwColor);


			}
		else if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_EXP2 || m_byStatus==GROUP_PARTY_PMATCHCONDITION_EXP1)
		{
			DWORD dwColor = 0xffffffff;

			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1295].szMessage);  // "���� �й�"

			int nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+232, m_fPosZ+232+14, GetStartOrder()+7, dwColor);


			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1296].szMessage);  // "������ �й�"

			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+250, m_fPosZ+250+14, GetStartOrder()+7, dwColor);


			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1297].szMessage);  // "���ʽ� ����ġ�� �й�"

			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+268, m_fPosZ+268+14, GetStartOrder()+7, dwColor);

			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1307].szMessage);  // "�������"

			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+286, m_fPosZ+286+14, GetStartOrder()+7, dwColor);


		}
		else if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_ITEM2 || m_byStatus==GROUP_PARTY_PMATCHCONDITION_ITEM1)
		{
			DWORD dwColor = 0xffffffff;

			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1298].szMessage);  // "������ �켱"

			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+287, m_fPosZ+287+14, GetStartOrder()+7, dwColor);


			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1299].szMessage);  // "������ Ÿ���� �켱"

			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+305, m_fPosZ+305+14, GetStartOrder()+7, dwColor);


			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1300].szMessage);  // "���� ����"

			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+323, m_fPosZ+323+14, GetStartOrder()+7, dwColor);

			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1307].szMessage);  // "�������"

			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+341, m_fPosZ+341+14, GetStartOrder()+7, dwColor);
		}
		else if(m_byStatus==GROUP_PARTY_PMATCHCONDITION_PMAINCHO2 || m_byStatus==GROUP_PARTY_PMATCHCONDITION_PMAINCHO1)
		{
			DWORD dwColor = 0xffffffff;

			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1308].szMessage);  // "������ ��Ƽ�� ����"

			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+342, m_fPosZ+342+14, GetStartOrder()+7, dwColor);


			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1309].szMessage);  // "�ڽ��� ��Ƽ������ �ʴ� "

			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+360, m_fPosZ+360+14, GetStartOrder()+7, dwColor);

			}
		}
	}

	//��Ƽ ��ȭ �ý��� (����â 2005.01.31)
	if(m_byGroupTab==GROUP_TAB_PARTY)
	{
		if(m_byStatus==GROUP_PARTY_PMATCHCONDITION1 || (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_LEV1 && GROUP_PARTY_PMATCHCONDITION_PMAINCHO4 >= m_byStatus)
			|| (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_LEV5 && GROUP_PARTY_PMATCHCONDITION_PMAINCHO5 >= m_byStatus) || m_byStatus==GROUP_PARTY_PMATCHCONDITION_OK_ON 
			|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_OK_OFF)
		{	
			if(m_bLevelMatch == 3)
			{
				DWORD dwColor = 0xffffffff;

				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1304].szMessage);  //3������ 
				int nSize = lstrlen(m_szInfo[0]);
				RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+165, m_fPosZ+165+14, GetStartOrder()+7, dwColor);

			}
			else if(m_bLevelMatch == 5)
			{
				DWORD dwColor = 0xffffffff;

				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1305].szMessage);  // 5������ 
				nSize = lstrlen(m_szInfo[0]);
				RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+165, m_fPosZ+165+14, GetStartOrder()+7, dwColor);

			}
			else if(m_bLevelMatch == 10)
			{
				DWORD dwColor = 0xffffffff;

				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1306].szMessage);  // 10������ 
				nSize = lstrlen(m_szInfo[0]);
				RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+165, m_fPosZ+165+14, GetStartOrder()+7, dwColor);

			}
		
		}

		if(m_byStatus==GROUP_PARTY_PMATCHCONDITION1 || (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_LEV1 && GROUP_PARTY_PMATCHCONDITION_PMAINCHO4 >= m_byStatus)
			|| (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_LEV5 && GROUP_PARTY_PMATCHCONDITION_PMAINCHO5 >= m_byStatus) || m_byStatus==GROUP_PARTY_PMATCHCONDITION_OK_ON 
			|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_OK_OFF)
		{
			if(m_bExpMatch == 1)
			{
				DWORD dwColor = 0xffffffff;

				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1295].szMessage);  // ����й� 
				int nSize = lstrlen(m_szInfo[0]);
				RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+217, m_fPosZ+217+14, GetStartOrder()+4, dwColor);

			}
			else if(m_bExpMatch == 2)
			{
				DWORD dwColor = 0xffffffff;

				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1296].szMessage);  // ������ �й�  
				nSize = lstrlen(m_szInfo[0]);
				RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+217, m_fPosZ+217+14, GetStartOrder()+4, dwColor);

			}
			else if(m_bExpMatch == 3)
			{
				DWORD dwColor = 0xffffffff;

				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1297].szMessage);  // ���ʽ� ����ġ�� �й� 
				nSize = lstrlen(m_szInfo[0]);
				RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+217, m_fPosZ+217+14, GetStartOrder()+4, dwColor);

			}
			else if(m_bExpMatch == 4)
			{
				DWORD dwColor = 0xffffffff;

				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1307].szMessage);  // ��� ���� 
				nSize = lstrlen(m_szInfo[0]);
				RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+217, m_fPosZ+217+14, GetStartOrder()+4, dwColor);

			}
		}

	
		if(m_byStatus==GROUP_PARTY_PMATCHCONDITION1 || (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_LEV1 && GROUP_PARTY_PMATCHCONDITION_PMAINCHO4 >= m_byStatus)
			|| (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_LEV5 && GROUP_PARTY_PMATCHCONDITION_PMAINCHO5 >= m_byStatus) || m_byStatus==GROUP_PARTY_PMATCHCONDITION_OK_ON 
			|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_OK_OFF)
		{
			if(m_bItemMatch	== 1)
			{
				DWORD dwColor = 0xffffffff;

				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1298].szMessage);  // ������ �켱 
				int nSize = lstrlen(m_szInfo[0]);
				RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+272, m_fPosZ+272+14, GetStartOrder()+4, dwColor);

			}
			else if(m_bItemMatch == 2)
			{
				DWORD dwColor = 0xffffffff;

				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1299].szMessage);  // ������ Ÿ���� �켱   
				nSize = lstrlen(m_szInfo[0]);
				RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+272, m_fPosZ+272+14, GetStartOrder()+4, dwColor);

			}
			else if(m_bItemMatch == 3)
			{
				DWORD dwColor = 0xffffffff;

				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1300].szMessage);  // ���� ����  
				nSize = lstrlen(m_szInfo[0]);
				RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+272, m_fPosZ+272+14, GetStartOrder()+4, dwColor);

			}
			else if(m_bItemMatch == 4)
			{
				DWORD dwColor = 0xffffffff;

				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1307].szMessage);  // ��� ���� 
				nSize = lstrlen(m_szInfo[0]);
				RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+272, m_fPosZ+272+14, GetStartOrder()+4, dwColor);

			}
		}

		if(m_byStatus==GROUP_PARTY_PMATCHCONDITION1 || (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_LEV1 && GROUP_PARTY_PMATCHCONDITION_PMAINCHO4 >= m_byStatus)
			|| (m_byStatus>=GROUP_PARTY_PMATCHCONDITION_LEV5 && GROUP_PARTY_PMATCHCONDITION_PMAINCHO5 >= m_byStatus) || m_byStatus==GROUP_PARTY_PMATCHCONDITION_OK_ON 
			|| m_byStatus==GROUP_PARTY_PMATCHCONDITION_OK_OFF )
		{
			if(m_bMasterMatch == 1)
			{
				DWORD dwColor = 0xffffffff;

				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1309].szMessage);  // �ڽ��� ��Ƽ ������ 
				int nSize = lstrlen(m_szInfo[0]);
				RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+327, m_fPosZ+327+14, GetStartOrder()+4, dwColor);

			}
			else if(m_bMasterMatch == 0)
			{
				DWORD dwColor = 0xffffffff;

				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1308].szMessage);  // ���� ��Ƽ�� ����  
				nSize = lstrlen(m_szInfo[0]);
				RenderFont(m_szInfo[0], m_fPosX+62, m_fPosX+62+(nSize*7), m_fPosZ+327, m_fPosZ+327+14, GetStartOrder()+4, dwColor);

			}
	
		}

	}

	//��Ƽ��Ȳ (����â 2005. 03. 03)
	if(m_byGroupTab==GROUP_TAB_PARTY)
	{
		if(m_byStatus==GROUP_PARTY_PARTYUSERINFO2)
		{
			BYTE byPartyFriendCount = 0;	
			int iRealSelect = 0;
			int nLen = lstrlen(m_szInfo[0]);

			if(m_dwPartyCount==0)
				return;

			for(int i = 0; i < __FRIEND_MAX_USER_DISPLAY__; i++)
			{
				iRealSelect = i + ((m_byCurPage-1)*__FRIEND_MAX_USER_DISPLAY__);
				
				if(iRealSelect>=m_dwPartyCount)
					return;

				switch(m_nPartyFriend[iRealSelect].bClass)
				{
				case 1: 
					lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE7].szMessage);	// "������"	
					break; 
				case 2: 
					lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE9].szMessage);	// "������Ʈ"
					break; 
				case 3: 
					lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE10].szMessage);	// "���ӳ�"
					break; 
				case 4: 
					lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE11].szMessage);	// "������"	
					break; 
				case 5: 
					lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE12].szMessage);	// "�Ҽ�����"	
					break; 		
				}

				int nSize = lstrlen(m_szInfo[0]);
				RenderFont(m_szInfo[0], m_fPosX+50, m_fPosX+50+(nLen*7), m_fPosZ+108+(i*35), m_fPosZ+122+(i*35), GetStartOrder()+4);
		
				lstrcpy(m_szInfo[0], m_nPartyFriend[iRealSelect].szCharacterName);

				nSize = lstrlen(m_szInfo[0]);
				RenderFont(m_szInfo[0], m_fPosX+190-(nSize*3), m_fPosX+190+(nLen*7), m_fPosZ+108+(i*35), m_fPosZ+122+(i*35), GetStartOrder()+4);
				
				VECTOR2 vPos;						 
				vPos.x		= m_fPosX+120;
				vPos.y		= m_fPosZ+106+(i*35);							
				g_pRenderer->RenderSprite(m_pSprStatus[m_nPartyFriend[iRealSelect].bOnline], NULL, 0.0f, &vPos, NULL, 0xffffffff, GetStartOrder()+2, RENDER_TYPE_DISABLE_TEX_FILTERING);

				byPartyFriendCount++;
			}			
		}
	}

}

void CGroupWnd::RenderFriendInfo()
{
	switch(m_byRenderType)
	{
	case GROUP_RENDER_FRIENDUSERADD:
		{			
			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE166].szMessage); // "���ο� ģ���� ����մϴ�."
		}
		m_byMsgCnt	= 1;
		break;
	case GROUP_RENDER_FRIENDUSERADDCLK:
		{
			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE167].szMessage);	// "�߰��� ĳ���͸� Ŭ���ϼ���."		
		}
		m_byMsgCnt	= 1;
		break;
	case GROUP_RENDER_FRIENDUSERONOFF:
		{
			if(m_byMessengerType==FRIEND_USER_OFFLINE)
			{
				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE255].szMessage); // "�¶��� ������ ģ���� ����Ʈ��"
				lstrcpy(m_szInfo[1], g_Message[ETC_MESSAGE183].szMessage); // "�����ɴϴ�."
			}
			else
			{
				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE256].szMessage); // "�������� ������ ģ���� ����Ʈ��"
				lstrcpy(m_szInfo[1], g_Message[ETC_MESSAGE183].szMessage); // "�����ɴϴ�."
			}
			m_byMsgCnt	= 2;
		}		
		break;
	case GROUP_RENDER_FRIENDUSERMSG:
		{
			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE741].szMessage); // "������ ģ������ �Ӹ��� �����ϴ�."
			m_byMsgCnt	= 1;
		}
		break;
	case GROUP_RENDER_FRIENDSERINFO:
		{
			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE742].szMessage); // "������ ģ���� ���� ������ �����ɴϴ�."
			m_byMsgCnt	= 1;			
		}
		break;
	case GROUP_RENDER_FRIENDUSERDELETE:
		{
			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE743].szMessage); // "������ ģ���� ����Ʈ���� �����մϴ�."
			m_byMsgCnt	= 1;
		}
		break;
	}

	if(m_byRenderType!=GROUP_RENDER_NONE)
	{
		if(m_bInfo!=TRUE)
		{
			for(int i = 0; i < m_byMsgCnt; i++)
			{
				int nSize = lstrlen(m_szInfo[i]);
				RenderFont(m_szInfo[i], m_fPosX+10, m_fPosX+10+(nSize*7), m_fPosZ+419+(i*20), m_fPosZ+433+(i*20), GetStartOrder()+4);				
			}		
		}
	}

	wsprintf(m_szInfo[0], "%u / %u", m_byCurPage, m_byMaxPage);
			
	int nLen = lstrlen(m_szInfo[0]);
	RenderFont(m_szInfo[0], m_fPosX+140-(nLen*3), m_fPosX+140+(nLen*7), m_fPosZ+385, m_fPosZ+399, GetStartOrder()+4);	
		
	VECTOR2 vPos;						 
	vPos.x		= m_fPosX+15;
	vPos.y		= m_fPosZ+50;
	g_pRenderer->RenderSprite(m_pSprStatus[g_byStatusMessenger], NULL, 0.0f, &vPos, NULL, 0xffffffff, GetStartOrder()+2, RENDER_TYPE_DISABLE_TEX_FILTERING);
	
	if(g_byStatusMessenger==0)
		lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE257].szMessage); // "������"
	else if(g_byStatusMessenger==1)
		lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE258].szMessage); // "�ڸ����"
	else if(g_byStatusMessenger==2)	
		lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE757].szMessage); // "�������"

	nLen = lstrlen(m_szInfo[0]);
	RenderFont(m_szInfo[0], m_fPosX+40, m_fPosX+40+(nLen*7), m_fPosZ+53, m_fPosZ+67, GetStartOrder()+4);
	
	if(m_byMessengerType==FRIEND_USER_ONLINE)
		wsprintf(m_szInfo[0], g_Message[ETC_MESSAGE168].szMessage, g_pMessengerOnLineHash->GetCount(), g_pMessengerOffLineHash->GetCount()+g_pMessengerOnLineHash->GetCount()); // "�¶��� ģ�� : %u / %u"
	else
		wsprintf(m_szInfo[0], g_Message[ETC_MESSAGE744].szMessage, g_pMessengerOffLineHash->GetCount(), g_pMessengerOffLineHash->GetCount()+g_pMessengerOnLineHash->GetCount()); // "�������� ģ�� : %u / %u"
			
	nLen = lstrlen(m_szInfo[0]);
	
#if IS_JAPAN_LOCALIZING()
	RenderFont(m_szInfo[0], m_fPosX+100, m_fPosX+100+(nLen*7), m_fPosZ+53, m_fPosZ+67, GetStartOrder()+4);
#else
	RenderFont(m_szInfo[0], m_fPosX+110, m_fPosX+110+(nLen*7), m_fPosZ+53, m_fPosZ+67, GetStartOrder()+4);
#endif			
	
	if(m_byMessengerType==FRIEND_USER_ONLINE)
	{
		BYTE byFriendIndex = 0;

		ListNode<MESSENGER_USER>* pMessengerUserPos;

		pMessengerUserPos	= m_pMessengerOnlinePos;
		byFriendIndex		= 0;
					
		if(g_pMessengerOnLineHash->GetCount()==0)
			return;

		for(int i = byFriendIndex; i < __FRIEND_MAX_USER_DISPLAY__; i++)
		{			
			if(byFriendIndex==g_pMessengerOnLineHash->GetCount()-((m_byCurPage-1)*__FRIEND_MAX_USER_DISPLAY__))
				return;
			
			LP_MESSENGER_USER lpMessengerUser = pMessengerUserPos->pData;
			
			if(!lpMessengerUser)
				continue;
											
			switch(lpMessengerUser->wClass)
			{
			case 1: 
				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE7].szMessage);	// "������"	
				break; 
			case 2: 
				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE9].szMessage);	// "������Ʈ"
				break; 
			case 3: 
				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE10].szMessage);	// "���ӳ�"
				break; 
			case 4: 
				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE11].szMessage);	// "������"	
				break; 
			case 5: 
				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE12].szMessage);	// "�Ҽ�����"	
				break; 		
			}
		
			int nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+50, m_fPosX+50+(nLen*7), m_fPosZ+108+(i*35), m_fPosZ+122+(i*35), GetStartOrder()+4);
		
			lstrcpy(m_szInfo[0], lpMessengerUser->szName);

			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+190-(nSize*3), m_fPosX+190+(nLen*7), m_fPosZ+108+(i*35), m_fPosZ+122+(i*35), GetStartOrder()+4);
				
			VECTOR2 vPos;						 
			vPos.x		= m_fPosX+120;
			vPos.y		= m_fPosZ+106+(i*35);							
			g_pRenderer->RenderSprite(m_pSprStatus[lpMessengerUser->byStatus], NULL, 0.0f, &vPos, NULL, 0xffffffff, GetStartOrder()+2, RENDER_TYPE_DISABLE_TEX_FILTERING);

			pMessengerUserPos = pMessengerUserPos->pNext;
			byFriendIndex++;
		}		
	}
	else if(m_byMessengerType==FRIEND_USER_OFFLINE)
	{
		BYTE byFriendIndex = 0;

		ListNode<MESSENGER_USER>* pMessengerUserPos;

		pMessengerUserPos	= m_pMessengerOfflinePos;
		byFriendIndex		= 0;
					
		if(g_pMessengerOffLineHash->GetCount()==0)
			return;

		for(int i = byFriendIndex; i < __FRIEND_MAX_USER_DISPLAY__; i++)
		{			
			if(byFriendIndex==g_pMessengerOffLineHash->GetCount()-((m_byCurPage-1)*__FRIEND_MAX_USER_DISPLAY__))
				return;
			
			LP_MESSENGER_USER lpMessengerUser = pMessengerUserPos->pData; 
			
			if(!lpMessengerUser)
				continue;
											
			switch(lpMessengerUser->wClass)
			{
			case 1: 
				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE7].szMessage);	// "������"	
				break; 
			case 2: 
				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE9].szMessage);	// "������Ʈ"
				break; 
			case 3: 
				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE10].szMessage);	// "���ӳ�"
				break; 
			case 4: 
				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE11].szMessage);	// "������"	
				break; 
			case 5: 
				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE12].szMessage);	// "�Ҽ�����"	
				break;			
			}
		
			int nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+50, m_fPosX+50+(nLen*7), m_fPosZ+108+(i*35), m_fPosZ+122+(i*35), GetStartOrder()+4);
					
			lstrcpy(m_szInfo[0], lpMessengerUser->szName);

			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+190-(nSize*3), m_fPosX+190+(nLen*7), m_fPosZ+108+(i*35), m_fPosZ+122+(i*35), GetStartOrder()+4);
		
			VECTOR2 vPos;						 
			vPos.x		= m_fPosX+120;
			vPos.y		= m_fPosZ+106+(i*35);							
			g_pRenderer->RenderSprite(m_pSprStatus[3], NULL, 0.0f, &vPos, NULL, 0xffffffff, GetStartOrder()+2, RENDER_TYPE_DISABLE_TEX_FILTERING);
					
			pMessengerUserPos = pMessengerUserPos->pNext;
			byFriendIndex++;
		}		
	}
}

void CGroupWnd::RenderGuildInfo()
{
	switch(m_byRenderType)
	{
	case GROUP_RENDER_GUILDUSERADD:
		{
			if(g_pGuildInfo.byType==__GCTYPE_CLEN__)
				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE715].szMessage); // "Ŭ������ �߰��մϴ�."
			else
				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE716].szMessage); // "������ �߰��մϴ�."
		}			
		m_byMsgCnt	= 1;
		break;		
	case GROUP_RENDER_GUILDUSERADDCLK:
		{
			m_byMsgCnt	= 1;
			if(g_pGuildInfo.byType==__GCTYPE_CLEN__)
				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE717].szMessage); // "Ŭ���� �߰��ϰ���� ĳ���͸�"
			else
				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE718].szMessage); // "��忡 �߰��ϰ���� ĳ���͸�"

#if !IS_CHINA_LOCALIZING()
			
			lstrcpy(m_szInfo[1], g_Message[ETC_MESSAGE562].szMessage); // "Ŭ���ϼ���."
			m_byMsgCnt++;

#endif
		}		
		break;
	case GROUP_RENDER_GUILDUSERONOFF:
		{
			if(g_pGuildInfo.byType==__GCTYPE_CLEN__)
			{
				if(m_byUserType==GUILD_USER_ONLINE)
					lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE181].szMessage);
				else
					lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE182].szMessage); // "���� �¶��� ���� Ŭ������ �����"				

				lstrcpy(m_szInfo[1], g_Message[ETC_MESSAGE183].szMessage); // "�����ɴϴ�."				
			}
			else
			{
				if(m_byUserType==GUILD_USER_ONLINE)
					lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE184].szMessage); // "���� �������� ���� ������ �����"					
				else
					lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE185].szMessage); // "���� �¶��� ���� ������ �����"				

				lstrcpy(m_szInfo[1], g_Message[ETC_MESSAGE183].szMessage); // "�����ɴϴ�."
			}			
		}
		m_byMsgCnt	= 2;
		break;
	case GROUP_RENDER_GUILDDELETE:
		{
			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE186].szMessage);	// "���� �׷쿡�� Ż��(��ü) �մϴ�."		
			m_byMsgCnt	= 1;
		}		
		break;
	case GROUP_RENDER_GUILDMSG:
		{
			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE566].szMessage); // "��뿡�� �Ӹ��� �����ϴ�."
			m_byMsgCnt	= 1;
		}
		break;
	case GROUP_RENDER_GUILDUSERINFO:
		{
			if(g_pGuildInfo.byType==__GCTYPE_CLEN__)
				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE187].szMessage); // "������ Ŭ������ ������ ������ �ɴϴ�."
			else
				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE188].szMessage); // "������ ������ ������ ������ �ɴϴ�."

			m_byMsgCnt	= 1;
		}
		break;
	case GROUP_RENDER_GUILDUSERDELETE:
		{
			if(g_pGuildInfo.byType==__GCTYPE_CLEN__)
				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE189].szMessage); // "������ Ŭ������ �߹��մϴ�."
			else
				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE190].szMessage); // "������ ������ �߹��մϴ�."

			m_byMsgCnt	= 1;			
		}
		break;
	case GROUP_RENDER_GUILDUSERRANK:
		{
			if(g_pGuildInfo.byType==__GCTYPE_CLEN__)				
				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE191].szMessage); // "������ Ŭ������ ������ �����մϴ�"
			else
				lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE192].szMessage); // "������ ������ ������ �����մϴ�"

			m_byMsgCnt	= 1;
		}
		break;
	case GROUP_RENDER_GUILDUSERMASTER:
		{			
			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE801].szMessage);	// "��� ������ ������ �̾��մϴ�."
			m_byMsgCnt	= 1;
		}
		break;
	}	

	if(m_byRenderType!=GROUP_RENDER_NONE)
	{
		if(m_bInfo!=TRUE)
		{
			for(int i = 0; i < m_byMsgCnt; i++)
			{
				int nSize = lstrlen(m_szInfo[i]);
				RenderFont(m_szInfo[i], m_fPosX+10, m_fPosX+10+(nSize*7), m_fPosZ+419+(i*20), m_fPosZ+433+(i*20), GetStartOrder()+4);
			}		
		}
	}
					
	if(g_pMainPlayer->m_dwGuildId!=0)
	{	
		wsprintf(m_szInfo[0], "%u / %u", m_byCurPage, m_byMaxPage);
			
		int nLen = lstrlen(m_szInfo[0]);
		RenderFont(m_szInfo[0], m_fPosX+140-(nLen*3), m_fPosX+140+(nLen*7), m_fPosZ+385, m_fPosZ+399, GetStartOrder()+4);		

		if(g_pMainPlayer->m_byType==__GCTYPE_GUILD__)
			wsprintf(m_szInfo[0], g_Message[ETC_MESSAGE573].szMessage, g_pMainPlayer->m_szGuildName); // "%s ���"
		else
			wsprintf(m_szInfo[0], g_Message[ETC_MESSAGE574].szMessage, g_pMainPlayer->m_szGuildName); // "%s Ŭ��"
		
		nLen = lstrlen(m_szInfo[0]);
		RenderFont(m_szInfo[0], m_fPosX+10, m_fPosX+10+(nLen*7), m_fPosZ+54, m_fPosZ+68, GetStartOrder()+4);		
				
		LP_GROUPINFO_TABLE lpGroupInfo = g_pGroupInfoHash->GetData(g_pGuildInfo.wLevel);
		
		if( lpGroupInfo && g_pMainPlayer->m_byType > 0 )
		{
			wsprintf(m_szInfo[0], g_Message[ETC_MESSAGE575].szMessage, g_pGuildInfo.byCount, lpGroupInfo->sGroupInfo[g_pMainPlayer->m_byType-1].wMemeber[0]);//"�ο� : %u / %u"			

			nLen = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+155, m_fPosX+155+(nLen*7), m_fPosZ+46, m_fPosZ+60, GetStartOrder()+4);
		}
		
		wsprintf(m_szInfo[0], "%s : %u", g_Message[ETC_MESSAGE472].szMessage, g_pGuildInfo.wLevel); // "����"
		
		nLen = lstrlen(m_szInfo[0]);
		RenderFont(m_szInfo[0], m_fPosX+155, m_fPosX+155+(nLen*7), m_fPosZ+60, m_fPosZ+74, GetStartOrder()+4);
				
		lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE170].szMessage); // "����ܰ�:"
		
		nLen = lstrlen(m_szInfo[0]);
		RenderFont(m_szInfo[0], m_fPosX+54, m_fPosX+54+(nLen*7), m_fPosZ+492, m_fPosZ+506, GetStartOrder()+4);		

		if(m_byUserType==GUILD_USER_ONLINE)
		{
			if(m_byCurPage==1)
			{
				// �ڱ� �ڽ� //
				DWORD dwColor = 0xffffffff;

				if(g_pMainPlayer->m_byRank==1)
					dwColor = 0xFFFFC309;
				
				switch(g_pMainPlayer->m_wClass)
				{
					case 1: 
						lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE7].szMessage);		// "������"	
						break; 
					case 2: 
						lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE9].szMessage);		// "������Ʈ"
						break; 
					case 3: 
						lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE10].szMessage);	// "���ӳ�"
						break; 
					case 4: 
						lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE11].szMessage);	// "������"	
						break; 
					case 5: 
						lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE12].szMessage);	// "�Ҽ�����"	
						break; 	
				}
				
				nLen = lstrlen(m_szInfo[0]);	
				RenderFont(m_szInfo[0], m_fPosX+50, m_fPosX+50+(nLen*7), m_fPosZ+102, m_fPosZ+116, GetStartOrder()+4, dwColor);

				wsprintf(m_szInfo[0], "%u", g_pMainPlayer->m_dwLevel);
				
				nLen = lstrlen(m_szInfo[0]);
				RenderFont(m_szInfo[0], m_fPosX+116-(nLen*1), m_fPosX+116+(nLen*7), m_fPosZ+102, m_fPosZ+116, GetStartOrder()+4, dwColor);

				lstrcpy(m_szInfo[0], g_pMainPlayer->m_szName);
				
				nLen = lstrlen(m_szInfo[0]);
				RenderFont(m_szInfo[0], m_fPosX+190-(nLen*3), m_fPosX+190+(nLen*7), m_fPosZ+102, m_fPosZ+116, GetStartOrder()+4, dwColor);

				if(!IsEmptyString(g_pMainPlayer->m_szNickName))
				{
					lstrcpy(m_szInfo[0], g_pMainPlayer->m_szNickName);
					
					nLen = lstrlen(m_szInfo[0]);
					RenderFont(m_szInfo[0], m_fPosX+190-(nLen*3), m_fPosX+190+(nLen*7), m_fPosZ+118, m_fPosZ+132, GetStartOrder()+4, dwColor);
				}
				
				lstrcpy(m_szInfo[0], g_pGuildInfo.szRankName[g_pMainPlayer->m_byRank]);	
							
				nLen = lstrlen(m_szInfo[0]);			
				RenderFont(m_szInfo[0], m_fPosX+50, m_fPosX+50+(nLen*7), m_fPosZ+118, m_fPosZ+132, GetStartOrder()+4, dwColor);							
			}

			BYTE byGuildIndex = 0;

			ListNode<GUILD_USER>* pGuildUserPos;

			if(m_byCurPage==1)
			{
				pGuildUserPos	= g_pGuildUserHash->GetHead();
				byGuildIndex	= 1;
			}
			else
				pGuildUserPos = m_pGuildUserPos;

			for(int i = byGuildIndex; i < __GUILD_MAX_USER_DISPLAY__; i++)
			{			
				if(byGuildIndex==g_pGuildUserHash->GetCount()+1-((m_byCurPage-1)*__GUILD_MAX_USER_DISPLAY__))
					return;
				
				LP_GUILD_USER pGuildUser = pGuildUserPos->pData;
				
				// �ڱ� �ڽ� //
				DWORD dwColor = 0xffffffff;

				if(pGuildUser->byRank==1)
					dwColor = 0xFFFFC309;
				
				switch(pGuildUser->wClass)
				{
					case 1: 
						lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE7].szMessage);		// "������"	
						break; 
					case 2: 
						lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE9].szMessage);		// "������Ʈ"
						break; 
					case 3: 
						lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE10].szMessage);	// "���ӳ�"
						break; 
					case 4: 
						lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE11].szMessage);	// "������"	
						break; 
					case 5: 
						lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE12].szMessage);	// "�Ҽ�����"	
						break; 	
		
				}
				
				nLen = lstrlen(m_szInfo[0]);	
				RenderFont(m_szInfo[0], m_fPosX+50, m_fPosX+50+(nLen*7), m_fPosZ+102+i*35, m_fPosZ+116+i*35, GetStartOrder()+4, dwColor);

				wsprintf(m_szInfo[0], "%u", pGuildUser->dwLev);
				
				nLen = lstrlen(m_szInfo[0]);
				RenderFont(m_szInfo[0], m_fPosX+116-(nLen*1), m_fPosX+116+(nLen*7), m_fPosZ+102+i*35, m_fPosZ+116+i*35, GetStartOrder()+4, dwColor);
				
				lstrcpy(m_szInfo[0], pGuildUser->szCharacterName);
				
				nLen = lstrlen(m_szInfo[0]);
				RenderFont(m_szInfo[0], m_fPosX+190-(nLen*3), m_fPosX+190+(nLen*7), m_fPosZ+102+i*35, m_fPosZ+116+i*35, GetStartOrder()+4, dwColor);

				if(!IsEmptyString(pGuildUser->szNickName))
				{
					lstrcpy(m_szInfo[0], pGuildUser->szNickName);
					
					nLen = lstrlen(m_szInfo[0]);
					RenderFont(m_szInfo[0], m_fPosX+190-(nLen*3), m_fPosX+190+(nLen*7), m_fPosZ+118+i*35, m_fPosZ+132+i*35, GetStartOrder()+4, dwColor);								
				}

				lstrcpy(m_szInfo[0], g_pGuildInfo.szRankName[pGuildUser->byRank]);	
							
				nLen = lstrlen(m_szInfo[0]);
				RenderFont(m_szInfo[0], m_fPosX+50, m_fPosX+50+(nLen*7), m_fPosZ+118+i*35, m_fPosZ+132+i*35, GetStartOrder()+4, dwColor);
		
				pGuildUserPos = pGuildUserPos->pNext;
				byGuildIndex++;
			}		
		}
		else if(m_byUserType==GUILD_USER_OFFLINE)
		{
			BYTE byGuildIndex = 0;

			ListNode<GUILD_OFFLINE_USER>* pGuildOfflineUserPos;
			
			pGuildOfflineUserPos = m_pGuildOfflineUserPos;

			for(int i = byGuildIndex; i < __GUILD_MAX_USER_DISPLAY__; i++)
			{			
				if(byGuildIndex==g_pGuildOfflineHash->GetCount()-((m_byCurPage-1)*__GUILD_MAX_USER_DISPLAY__))
					return;
				
				LP_GUILD_OFFLINE_USER pGuildOfflineUser = pGuildOfflineUserPos->pData;
				
				// �ڱ� �ڽ� //
				DWORD dwColor = 0xffffffff;

				if(pGuildOfflineUser->byRank==1)
					dwColor = 0xFFFFC309;
				
				switch(pGuildOfflineUser->wClass)
				{
					case 1: 
						lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE7].szMessage);		// "������"	
						break; 
					case 2: 
						lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE9].szMessage);		// "������Ʈ"
						break; 
					case 3: 
						lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE10].szMessage);	// "���ӳ�"
						break; 
					case 4: 
						lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE11].szMessage);	// "������"	
						break; 
					case 5: 
						lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE12].szMessage);	// "�Ҽ�����"	
						break; 	
					
				}
				
				nLen = lstrlen(m_szInfo[0]);	
				RenderFont(m_szInfo[0], m_fPosX+50, m_fPosX+50+(nLen*7), m_fPosZ+102+i*35, m_fPosZ+116+i*35, GetStartOrder()+4, dwColor);											

				wsprintf(m_szInfo[0], "%u", pGuildOfflineUser->dwLevel);
				
				nLen = lstrlen(m_szInfo[0]);			
				RenderFont(m_szInfo[0], m_fPosX+116-(nLen*1), m_fPosX+116+(nLen*7), m_fPosZ+102+i*35, m_fPosZ+116+i*35, GetStartOrder()+4, dwColor);
				
				lstrcpy(m_szInfo[0], pGuildOfflineUser->szName);
				
				nLen = lstrlen(m_szInfo[0]);
				RenderFont(m_szInfo[0], m_fPosX+190-(nLen*3), m_fPosX+190+(nLen*7), m_fPosZ+102+i*35, m_fPosZ+116+i*35, GetStartOrder()+4, dwColor);				

				if(!IsEmptyString(pGuildOfflineUser->szNickName))
				{
					lstrcpy(m_szInfo[0], pGuildOfflineUser->szNickName);
					
					nLen = lstrlen(m_szInfo[0]);
					RenderFont(m_szInfo[0], m_fPosX+190-(nLen*3), m_fPosX+190+(nLen*7), m_fPosZ+118+i*35, m_fPosZ+132+i*35, GetStartOrder()+4, dwColor);
				}

				lstrcpy(m_szInfo[0], g_pGuildInfo.szRankName[pGuildOfflineUser->byRank]);	
							
				nLen = lstrlen(m_szInfo[0]);
				RenderFont(m_szInfo[0], m_fPosX+50, m_fPosX+50+(nLen*7), m_fPosZ+118+i*35, m_fPosZ+132+i*35, GetStartOrder()+4, dwColor);
		
				pGuildOfflineUserPos = pGuildOfflineUserPos->pNext;
				
				if(!pGuildOfflineUserPos)
					return;

				byGuildIndex++;
			}	
		}
	}
}

void CGroupWnd::RenderGuildWarInfo()
{
	switch(m_byRenderType)
	{
	case GROUP_RENDER_GUILDWARUSERADD:
		{			
			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE878].szMessage); // "������� �����մϴ�."
		}
		m_byMsgCnt	= 1;
		break;
	case GROUP_RENDER_GUILDWARUSERADDCLK:
		{
			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE879].szMessage); // "����� ������ ������ Ŭ���ϼ���."
		}
		m_byMsgCnt	= 1;
		break;	
	}

	if(m_byRenderType!=GROUP_RENDER_NONE)
	{
		if(m_bInfo!=TRUE)
		{
			for(int i = 0; i < m_byMsgCnt; i++)
			{
				int nSize = lstrlen(m_szInfo[i]);
				RenderFont(m_szInfo[0], m_fPosX+10, m_fPosX+10+(nSize*7), m_fPosZ+419+i*20, m_fPosZ+433+i*20, GetStartOrder()+4);				
			}		
		}
	}
				
	wsprintf(m_szInfo[0], g_Message[ETC_MESSAGE573].szMessage, g_pGuildInfo.szGuildName); // "%s ���"
	
	int nLen = lstrlen(m_szInfo[0]);
	RenderFont(m_szInfo[0], m_fPosX+10, m_fPosX+10+(nLen*7), m_fPosZ+53, m_fPosZ+67, GetStartOrder()+4);				
			
	wsprintf(m_szInfo[0], g_Message[ETC_MESSAGE899].szMessage, g_pGuildInfo.dwPoint); // "Ladder ���� : %u"
	
	nLen = lstrlen(m_szInfo[0]);
	RenderFont(m_szInfo[0], m_fPosX+128, m_fPosX+128+(nLen*7), m_fPosZ+53, m_fPosZ+67, GetStartOrder()+4);

	ListNode<SGUILD_WAR>* lpGuildWarPos = g_pGuildWarHash->GetHead();
		
	struct tm	*when;
	time_t		now;
	
	time(&now);
	when = localtime(&now);
	
	BYTE byGuildWarIndex = 0;

	while(lpGuildWarPos)
	{
		LP_GUILD_WAR lpGuildWar = lpGuildWarPos->pData;

		if(lpGuildWar)
		{
			lstrcpy(m_szInfo[0], lpGuildWar->szGuildName);

			int nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+50, m_fPosX+50+(nLen*7), m_fPosZ+108+(byGuildWarIndex*35), m_fPosZ+122+(byGuildWarIndex*35), GetStartOrder()+4);			

			wsprintf(m_szInfo[0],  "%u : %u", lpGuildWar->wPoint1, lpGuildWar->wPoint2);

			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+120, m_fPosX+120+(nLen*7), m_fPosZ+108+(byGuildWarIndex*35), m_fPosZ+122+(byGuildWarIndex*35), GetStartOrder()+4);			

			int nTime = (int)difftime(now, lpGuildWar->sTime);

			int nHou	= (nTime/60)/60;	
			int nMit	= (nTime/60)%60;	

			if(nMit>0)
				nHou += 1;
			else if(nMit==0)
				nMit = 60;

			if(nHou>=GUILDWAR_TIME)
				wsprintf(m_szInfo[0],  g_Message[ETC_MESSAGE904].szMessage); // "�ð� �ʰ�"
			else
				wsprintf(m_szInfo[0],  "%u : %u", GUILDWAR_TIME-nHou, 60-nMit);

			nSize = lstrlen(m_szInfo[0]);
			RenderFont(m_szInfo[0], m_fPosX+190, m_fPosX+190+(nLen*7), m_fPosZ+108+(byGuildWarIndex*35), m_fPosZ+122+(byGuildWarIndex*35), GetStartOrder()+4);			

			byGuildWarIndex++;
		}
		lpGuildWarPos = lpGuildWarPos->pNext;
	}			
}

void CGroupWnd::SetPosGroup()
{
	if(m_byGroupTab==GROUP_TAB_PARTY)
	{
		SetPosObjX(SPR_OBJ_GROUPWND_ADD1, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_ADD1, 100);
		SetPosObjX(SPR_OBJ_GROUPWND_ADD2, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_ADD2, 100);
		SetPosObjX(SPR_OBJ_GROUPWND_ADD3, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_ADD3, 100);
		SetPosObjX(SPR_OBJ_GROUPWND_DELETE1, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_DELETE1, 135);
		SetPosObjX(SPR_OBJ_GROUPWND_DELETE2, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_DELETE2, 135);
		SetPosObjX(SPR_OBJ_GROUPWND_DELETE3, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_DELETE3, 135);
		SetPosObjX(SPR_OBJ_GROUPWND_INFO1, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_INFO1, 170);
		SetPosObjX(SPR_OBJ_GROUPWND_INFO2, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_INFO2, 170);
		SetPosObjX(SPR_OBJ_GROUPWND_INFO3, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_INFO3, 170);
		SetPosObjX(SPR_OBJ_GROUPWND_OUT1, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_OUT1, 205);
		SetPosObjX(SPR_OBJ_GROUPWND_OUT2, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_OUT2, 205);
		SetPosObjX(SPR_OBJ_GROUPWND_OUT3, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_OUT3, 205);
		SetPosObjX(SPR_OBJ_GROUPWND_MSG1, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_MSG1, 240);
		SetPosObjX(SPR_OBJ_GROUPWND_MSG2, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_MSG2, 240);
		SetPosObjX(SPR_OBJ_GROUPWND_MSG3, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_MSG3, 240);

		//��Ƽ ��ȭ �ý��� (����â 2005.01.25)
//		SetPosObjX(SPR_OBJ_GROUPWND_PARTYUSERINFO1, 7);    
//		SetPosObjY(SPR_OBJ_GROUPWND_PARTYUSERINFO1, 275);
		SetPosObjX(SPR_OBJ_GROUPWND_PARTYUSERINFO2, 7);
		SetPosObjY(SPR_OBJ_GROUPWND_PARTYUSERINFO2, 275);
		SetPosObjX(SPR_OBJ_GROUPWND_PARTYUSERINFO3, 7);
		SetPosObjY(SPR_OBJ_GROUPWND_PARTYUSERINFO3, 275);
//		SetPosObjX(SPR_OBJ_GROUPWND_EXPITEMCONDITION1, 5);
//		SetPosObjY(SPR_OBJ_GROUPWND_EXPITEMCONDITION1, 310);
		SetPosObjX(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, 7);
		SetPosObjY(SPR_OBJ_GROUPWND_EXPITEMCONDITION2, 310);
		SetPosObjX(SPR_OBJ_GROUPWND_EXPITEMCONDITION3, 7);
		SetPosObjY(SPR_OBJ_GROUPWND_EXPITEMCONDITION3, 310);
//		SetPosObjX(SPR_OBJ_GROUPWND_PMATCHCONDITION1, 7);  
//		SetPosObjY(SPR_OBJ_GROUPWND_PMATCHCONDITION1, 345);
		SetPosObjX(SPR_OBJ_GROUPWND_PMATCHCONDITION2, 7);
		SetPosObjY(SPR_OBJ_GROUPWND_PMATCHCONDITION2, 345);
		SetPosObjX(SPR_OBJ_GROUPWND_PMATCHCONDITION3, 7);
		SetPosObjY(SPR_OBJ_GROUPWND_PMATCHCONDITION3, 345);
	//	SetPosObjX(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK, 5);
	//	SetPosObjY(SPR_OBJ_GROUPWND_PMATCHCONDITION_OK, 240);


	}
	else if(m_byGroupTab==GROUP_TAB_GUILD)
	{
		SetPosObjX(SPR_OBJ_GROUPWND_ADD1, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_ADD1, 100);
		SetPosObjX(SPR_OBJ_GROUPWND_ADD2, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_ADD2, 100);
		SetPosObjX(SPR_OBJ_GROUPWND_ADD3, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_ADD3, 100);			
		SetPosObjX(SPR_OBJ_GROUPWND_DELETE1, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_DELETE1, 135);
		SetPosObjX(SPR_OBJ_GROUPWND_DELETE2, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_DELETE2, 135);
		SetPosObjX(SPR_OBJ_GROUPWND_DELETE3, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_DELETE3, 135);
		SetPosObjX(SPR_OBJ_GROUPWND_INFO1, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_INFO1, 170);
		SetPosObjX(SPR_OBJ_GROUPWND_INFO2, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_INFO2, 170);
		SetPosObjX(SPR_OBJ_GROUPWND_INFO3, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_INFO3, 170);				
		SetPosObjX(SPR_OBJ_GROUPWND_OUT1, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_OUT1, 205);
		SetPosObjX(SPR_OBJ_GROUPWND_OUT2, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_OUT2, 205);
		SetPosObjX(SPR_OBJ_GROUPWND_OUT3, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_OUT3, 205);
		SetPosObjX(SPR_OBJ_GROUPWND_MSG1, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_MSG1, 240);
		SetPosObjX(SPR_OBJ_GROUPWND_MSG2, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_MSG2, 240);
		SetPosObjX(SPR_OBJ_GROUPWND_MSG3, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_MSG3, 240);

		SetPosObjX(SPR_OBJ_GUILD_PAGE, 110);
		SetPosObjY(SPR_OBJ_GUILD_PAGE, 380);		
		SetPosObjX(SPR_OBJ_GUILD_PAGEPREV1, 93);
		SetPosObjY(SPR_OBJ_GUILD_PAGEPREV1, 380);
		SetPosObjX(SPR_OBJ_GUILD_PAGEPREV2, 93);
		SetPosObjY(SPR_OBJ_GUILD_PAGEPREV2, 380);
		SetPosObjX(SPR_OBJ_GUILD_PAGEPREV3, 93);
		SetPosObjY(SPR_OBJ_GUILD_PAGEPREV3, 380);
		SetPosObjX(SPR_OBJ_GUILD_PAGENEXT1, 175);
		SetPosObjY(SPR_OBJ_GUILD_PAGENEXT1, 380);
		SetPosObjX(SPR_OBJ_GUILD_PAGENEXT2, 175);
		SetPosObjY(SPR_OBJ_GUILD_PAGENEXT2, 380);
		SetPosObjX(SPR_OBJ_GUILD_PAGENEXT3, 175);
		SetPosObjY(SPR_OBJ_GUILD_PAGENEXT3, 380);	
		
		SetPosObjX(SPR_OBJ_GUILD_ONOFF1, 5);
		SetPosObjY(SPR_OBJ_GUILD_ONOFF1, 310);	
		SetPosObjX(SPR_OBJ_GUILD_ONOFF2, 5);
		SetPosObjY(SPR_OBJ_GUILD_ONOFF2, 310);	
		SetPosObjX(SPR_OBJ_GUILD_ONOFF3, 5);
		SetPosObjY(SPR_OBJ_GUILD_ONOFF3, 310);	
		SetPosObjX(SPR_OBJ_GUILD_INFO1, 5);
		SetPosObjY(SPR_OBJ_GUILD_INFO1, 275);	
		SetPosObjX(SPR_OBJ_GUILD_INFO2, 5);
		SetPosObjY(SPR_OBJ_GUILD_INFO2, 275);	
		SetPosObjX(SPR_OBJ_GUILD_INFO3, 5);
		SetPosObjY(SPR_OBJ_GUILD_INFO3, 275);	

		SetPosObjX(SPR_OBJ_GROUP_MASTER1, 5);
		SetPosObjY(SPR_OBJ_GROUP_MASTER1, 345);	
		SetPosObjX(SPR_OBJ_GROUP_MASTER2, 5);
		SetPosObjY(SPR_OBJ_GROUP_MASTER2, 345);	
		SetPosObjX(SPR_OBJ_GROUP_MASTER3, 5);
		SetPosObjY(SPR_OBJ_GROUP_MASTER3, 345);	
	}
	else if(m_byGroupTab==GROUP_TAB_FRIEND)
	{
		SetPosObjX(SPR_OBJ_GUILD_PAGE, 110);
		SetPosObjY(SPR_OBJ_GUILD_PAGE, 380);		
		SetPosObjX(SPR_OBJ_GUILD_PAGEPREV1, 93);
		SetPosObjY(SPR_OBJ_GUILD_PAGEPREV1, 380);
		SetPosObjX(SPR_OBJ_GUILD_PAGEPREV2, 93);
		SetPosObjY(SPR_OBJ_GUILD_PAGEPREV2, 380);
		SetPosObjX(SPR_OBJ_GUILD_PAGEPREV3, 93);
		SetPosObjY(SPR_OBJ_GUILD_PAGEPREV3, 380);
		SetPosObjX(SPR_OBJ_GUILD_PAGENEXT1, 175);
		SetPosObjY(SPR_OBJ_GUILD_PAGENEXT1, 380);
		SetPosObjX(SPR_OBJ_GUILD_PAGENEXT2, 175);
		SetPosObjY(SPR_OBJ_GUILD_PAGENEXT2, 380);
		SetPosObjX(SPR_OBJ_GUILD_PAGENEXT3, 175);
		SetPosObjY(SPR_OBJ_GUILD_PAGENEXT3, 380);
		
		SetPosObjX(SPR_OBJ_GROUPWND_ADD1, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_ADD1, 100);
		SetPosObjX(SPR_OBJ_GROUPWND_ADD2, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_ADD2, 100);
		SetPosObjX(SPR_OBJ_GROUPWND_ADD3, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_ADD3, 100);			
		SetPosObjX(SPR_OBJ_GROUPWND_DELETE1, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_DELETE1, 135);
		SetPosObjX(SPR_OBJ_GROUPWND_DELETE2, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_DELETE2, 135);
		SetPosObjX(SPR_OBJ_GROUPWND_DELETE3, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_DELETE3, 135);
		SetPosObjX(SPR_OBJ_GROUPWND_INFO1, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_INFO1, 170);
		SetPosObjX(SPR_OBJ_GROUPWND_INFO2, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_INFO2, 170);
		SetPosObjX(SPR_OBJ_GROUPWND_INFO3, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_INFO3, 170);
		SetPosObjX(SPR_OBJ_GROUPWND_MSG1, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_MSG1, 205);
		SetPosObjX(SPR_OBJ_GROUPWND_MSG2, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_MSG2, 205);
		SetPosObjX(SPR_OBJ_GROUPWND_MSG3, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_MSG3, 205);
		SetPosObjX(SPR_OBJ_GUILD_ONOFF1, 5);
		SetPosObjY(SPR_OBJ_GUILD_ONOFF1, 240);	
		SetPosObjX(SPR_OBJ_GUILD_ONOFF2, 5);
		SetPosObjY(SPR_OBJ_GUILD_ONOFF2, 240);	
		SetPosObjX(SPR_OBJ_GUILD_ONOFF3, 5);
		SetPosObjY(SPR_OBJ_GUILD_ONOFF3, 240);

		SetPosObjX(SPR_OBJ_GROUPWND_STATUS, 107);
		SetPosObjY(SPR_OBJ_GROUPWND_STATUS, 81);
	}
/*	else if(m_byGroupTab==GROUP_TAB_GUILDWAR)
	{
		SetPosObjX(SPR_OBJ_GROUPWND_GUILDWARADD1, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_GUILDWARADD1, 100);
		SetPosObjX(SPR_OBJ_GROUPWND_GUILDWARADD2, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_GUILDWARADD2, 100);
		SetPosObjX(SPR_OBJ_GROUPWND_GUILDWARADD3, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_GUILDWARADD3, 100);
		SetPosObjX(SPR_OBJ_GROUPWND_GUILDWAREXIT1, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_GUILDWAREXIT1, 135);
		SetPosObjX(SPR_OBJ_GROUPWND_GUILDWAREXIT2, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_GUILDWAREXIT2, 135);
		SetPosObjX(SPR_OBJ_GROUPWND_GUILDWAREXIT3, 5);
		SetPosObjY(SPR_OBJ_GROUPWND_GUILDWAREXIT3, 135);
	}*/
}
//======================================================//
// End.													//
//======================================================//


// sung-han 2005-01-29 ���̹������ ��ŷ ����Ʈ
void CGroupWnd::RenderTextGuildWarRankList()
{
	int i;
	int nSize;
	DWORD dwColor;

	int nPosX = 53;
	int nPosZ = 105;

	char szTmp[255];

	if( m_nGuildWarRankListTotalCount <= 0 )
		return;

	for(i=0; i<MAXLINE_PER_PAGE; i++)
	{

		if(m_nGuildWarRankListTotalCount <= i )
			break;

		dwColor = 0xffffffff;
		if(m_nGuildWarRankListSelect == i)
		{
			dwColor = 0xFFFFC309;
		}
		ZeroMemory(szTmp, sizeof(szTmp));
		wsprintf(szTmp, "%3d. %12s    %d%s", m_pGuildWarRankListInfoPage.m_GuildLadderInfo[i].m_uWeekRank, 
			m_pGuildWarRankListInfoPage.m_GuildLadderInfo[i].m_szGuildName, m_pGuildWarRankListInfoPage.m_GuildLadderInfo[i].m_uWeekPoint, 
			g_Message[ETC_MESSAGE1371].szMessage );//��, �� ����� '���� �̸� ����'
		nSize = lstrlen(szTmp);
		if( nSize < 100 )
			RenderFont(szTmp, m_fPosX+nPosX, m_fPosX+nPosX+(nSize*7), m_fPosZ+nPosZ+(i*16), m_fPosZ+nPosZ+14+(i*16), GetStartOrder()+4, dwColor);
	}

	if(g_pMainPlayer->m_dwGuildId)
	{
		ZeroMemory(szTmp, sizeof(szTmp));
		wsprintf(szTmp, "%3d. %12s    %d%s", m_pGuildWarRankListInfoPage.m_uMyGuildLadderRank, 
			m_pGuildWarRankListInfoPage.m_szMyGuildName, m_pGuildWarRankListInfoPage.m_uMyGuildLadderPoint, 
			g_Message[ETC_MESSAGE1371].szMessage );//��, �ڽ��� ����� '���� �̸� ����', �������� ?
		nSize = lstrlen(szTmp);
		if( nSize < 100 )
			RenderFont(szTmp, m_fPosX+nPosX, m_fPosX+nPosX+(nSize*7), m_fPosZ+nPosZ+(15*16), m_fPosZ+nPosZ+14+(15*16), GetStartOrder()+4, dwColor);
	}

	nPosX = 10;
	nPosZ = 418;
	if(m_nGuildWarRankListSelect > -1)
	{
		ZeroMemory(szTmp, sizeof(szTmp));
		wsprintf(szTmp, "%s (GM:%s)", m_pGuildWarRankListInfoPage.m_GuildLadderInfo[m_nGuildWarRankListSelect].m_szGuildName, 
			m_pGuildWarRankListInfoPage.m_GuildLadderInfo[m_nGuildWarRankListSelect].m_szGuildMasterName);// '���� (GM:�渶��)'
		nSize = lstrlen(szTmp);
		RenderFont(szTmp, m_fPosX+nPosX, m_fPosX+nPosX+(nSize*7), m_fPosZ+nPosZ, m_fPosZ+nPosZ+14, GetStartOrder()+4 );

		ZeroMemory(szTmp, sizeof(szTmp));
		wsprintf(szTmp, g_Message[ETC_MESSAGE1364].szMessage, m_pGuildWarRankListInfoPage.m_GuildLadderInfo[m_nGuildWarRankListSelect].m_uGuildMemberCount);//���� : %d��
		nSize = lstrlen(szTmp);
		RenderFont(szTmp, m_fPosX+nPosX, m_fPosX+nPosX+(nSize*7), m_fPosZ+nPosZ+15, m_fPosZ+nPosZ+15+14, GetStartOrder()+4 );

		ZeroMemory(szTmp, sizeof(szTmp));
		wsprintf(szTmp, g_Message[ETC_MESSAGE1365].szMessage, 1, m_pGuildWarRankListInfoPage.m_GuildLadderInfo[m_nGuildWarRankListSelect].m_uWeekPoint,
			m_pGuildWarRankListInfoPage.m_GuildLadderInfo[m_nGuildWarRankListSelect].m_uWeekRank, 
			m_pGuildWarRankListInfoPage.m_GuildLadderInfo[m_nGuildWarRankListSelect].m_uJuniorWeekPoint, 
			m_pGuildWarRankListInfoPage.m_GuildLadderInfo[m_nGuildWarRankListSelect].m_uSeniorWeekPoint);//%d�ְ� ���� %d�� %d�� (%d/%d)
		nSize = lstrlen(szTmp);
		RenderFont(szTmp, m_fPosX+nPosX, m_fPosX+nPosX+(nSize*7), m_fPosZ+nPosZ+45, m_fPosZ+nPosZ+45+14, GetStartOrder()+4 );

		ZeroMemory(szTmp, sizeof(szTmp));
		wsprintf(szTmp, g_Message[ETC_MESSAGE1365].szMessage, 1, m_pGuildWarRankListInfoPage.m_GuildLadderInfo[m_nGuildWarRankListSelect].m_uQuaterPoint,
			m_pGuildWarRankListInfoPage.m_GuildLadderInfo[m_nGuildWarRankListSelect].m_uQuaterRank, 
			m_pGuildWarRankListInfoPage.m_GuildLadderInfo[m_nGuildWarRankListSelect].m_uJuniorQuaterPoint, 
			m_pGuildWarRankListInfoPage.m_GuildLadderInfo[m_nGuildWarRankListSelect].m_uSeniorQuaterPoint);//%d�ְ� ���� %d�� %d�� (%d/%d)
		nSize = lstrlen(szTmp);
		RenderFont(szTmp, m_fPosX+nPosX, m_fPosX+nPosX+(nSize*7), m_fPosZ+nPosZ+60, m_fPosZ+nPosZ+60+14, GetStartOrder()+4 );
	}

	ZeroMemory(szTmp, sizeof(szTmp));
	if(m_nGuildWarRankListMaxPage > 0)
	{
		wsprintf(szTmp, "%3d/%d", m_nGuildWarRankListPage, m_nGuildWarRankListMaxPage);
		nSize = lstrlen(szTmp);
		RenderFont(szTmp, m_fPosX+120, m_fPosX+115+(nSize*8), m_fPosZ+384, m_fPosZ+384+14, GetStartOrder()+4 );
	}
}
/*
void CGroupWnd::RenderTextGuildWarRankList()
{
	int i;
	int nSize;
	DWORD dwColor;

	int nPosX = 53;
	int nPosZ = 105;

	char szTmp[255];

	for(i=0; i<MAXLINE_PER_PAGE; i++)
	{
		int nRealArea = i;

		if(m_nGuildWarRankListTotalCount <= nRealArea )
			break;

		dwColor = 0xffffffff;
		if(m_nGuildWarRankListSelect == i)
		{
			dwColor = 0xFFFFC309;
		}
		ZeroMemory(szTmp, sizeof(szTmp));
		wsprintf(szTmp, "%3d. %12s    %d%s", m_LadderList[nRealArea].iRank[0], m_LadderList[nRealArea].szGuildName, m_LadderList[nRealArea].iTotalPoint, g_Message[ETC_MESSAGE1371].szMessage );//��
		nSize = lstrlen(szTmp);
		RenderFont(szTmp, m_fPosX+nPosX, m_fPosX+nPosX+(nSize*7), m_fPosZ+nPosZ+(i*16), m_fPosZ+nPosZ+14+(i*16), GetStartOrder()+4, dwColor);
	}

	nPosX = 10;
	nPosZ = 418;
	if(m_nGuildWarRankListSelect > -1)
	{
		int nRealSelect = m_nGuildWarRankListSelect;
		
		ZeroMemory(szTmp, sizeof(szTmp));
		wsprintf(szTmp, "%s (GM:%s)", m_LadderList[nRealSelect].szGuildName, m_LadderList[nRealSelect].szGuildMaster);
		nSize = lstrlen(szTmp);
		RenderFont(szTmp, m_fPosX+nPosX, m_fPosX+nPosX+(nSize*7), m_fPosZ+nPosZ, m_fPosZ+nPosZ+14, GetStartOrder()+4 );

		ZeroMemory(szTmp, sizeof(szTmp));
		wsprintf(szTmp, g_Message[ETC_MESSAGE1364].szMessage, m_LadderList[nRealSelect].iGuildTotalCount);//���� : %d��
		nSize = lstrlen(szTmp);
		RenderFont(szTmp, m_fPosX+nPosX, m_fPosX+nPosX+(nSize*7), m_fPosZ+nPosZ+15, m_fPosZ+nPosZ+15+14, GetStartOrder()+4 );

		ZeroMemory(szTmp, sizeof(szTmp));
		wsprintf(szTmp, g_Message[ETC_MESSAGE1365].szMessage, 1, m_LadderList[nRealSelect].WeekPoint[0]+m_LadderList[nRealSelect].WeekPoint[1],
			m_LadderList[nRealSelect].iRank[0], m_LadderList[nRealSelect].WeekPoint[0], m_LadderList[nRealSelect].WeekPoint[1]);//%d�ְ� ���� %d�� %d�� (%d/%d)
		nSize = lstrlen(szTmp);
		RenderFont(szTmp, m_fPosX+nPosX, m_fPosX+nPosX+(nSize*7), m_fPosZ+nPosZ+45, m_fPosZ+nPosZ+45+14, GetStartOrder()+4 );

		ZeroMemory(szTmp, sizeof(szTmp));
		wsprintf(szTmp, g_Message[ETC_MESSAGE1365].szMessage, 5, m_LadderList[nRealSelect].MonthPoint[0]+m_LadderList[nRealSelect].MonthPoint[1],
			m_LadderList[nRealSelect].iRank[1], m_LadderList[nRealSelect].MonthPoint[0], m_LadderList[nRealSelect].MonthPoint[1]);//%d�ְ� ���� %d�� %d�� (%d/%d)
		nSize = lstrlen(szTmp);
		RenderFont(szTmp, m_fPosX+nPosX, m_fPosX+nPosX+(nSize*7), m_fPosZ+nPosZ+60, m_fPosZ+nPosZ+60+14, GetStartOrder()+4 );
	}

	ZeroMemory(szTmp, sizeof(szTmp));
	if(m_nGuildWarRankListMaxPage == 0)
		wsprintf(szTmp, "%3d/%d", 0, m_nGuildWarRankListMaxPage);
	else
		wsprintf(szTmp, "%3d/%d", m_nGuildWarRankListPage, m_nGuildWarRankListMaxPage);
	nSize = lstrlen(szTmp);
	RenderFont(szTmp, m_fPosX+120, m_fPosX+115+(nSize*8), m_fPosZ+384, m_fPosZ+384+14, GetStartOrder()+4 );
}
*/

void CGroupWnd::RenderTextGuildWarList()
{
	int i;
	int nSize;
	DWORD dwColor;
	char szTmp[100];

	int iIsUse[MAX_MATCH_TEAM] ={0,};
	int iIsUseCount = 0;

	int nPosX = 53;
	int nPosZ = 105;

	for(i=0; i<MAXLINE_PER_PAGE; i++)
	{
		int nRealArea = i;

		if(m_nGuildWarListTotalCount <= nRealArea )
			break;

		dwColor = 0xffffffff;
		if(m_nGuildWarListSelect == i)
		{
			dwColor = 0xFFFFC309;
		}

		char cMatchtype = 'K';
		if(m_GuildWarList[nRealArea].match_type == M_T_DEAH_MATCH)
			cMatchtype = 'D';
		else if(m_GuildWarList[nRealArea].match_type == M_T_KING_OF_THE_HILL)
			cMatchtype = 'K';
		else if(m_GuildWarList[nRealArea].match_type == M_T_CAPTURE_THE_FLAG)
			cMatchtype = 'C';
		else if(m_GuildWarList[nRealArea].match_type == M_T_LEADER_KILL)
			cMatchtype = 'L';
		else if(m_GuildWarList[nRealArea].match_type == M_T_KILLING_FIELD)
			cMatchtype = 'K';

		char cMatchGrade = 'S';
		if(m_GuildWarList[nRealArea].match_level == M_L_HIGH)
			cMatchGrade = 'S';
		else if(m_GuildWarList[nRealArea].match_level == M_L_LOW)
			cMatchGrade = 'J';
		
		for(int j=1; j<MAX_MATCH_TEAM; j++)
		{
			if(m_GuildWarList[nRealArea].aGLD[j].bIsUse)
			{
				iIsUse[iIsUseCount] = j;
				iIsUseCount++;
			}
		}

		ZeroMemory(szTmp, sizeof(szTmp));
		wsprintf(szTmp, "%c %c %s : %s", cMatchtype, cMatchGrade, m_GuildWarList[nRealArea].aGLD[iIsUse[0]].szGuildName, m_GuildWarList[nRealArea].aGLD[iIsUse[1]].szGuildName );
		nSize = lstrlen(szTmp);
		RenderFont(szTmp, m_fPosX+nPosX, m_fPosX+nPosX+(nSize*7), m_fPosZ+nPosZ+(i*16), m_fPosZ+nPosZ+14+(i*16), GetStartOrder()+4, dwColor);
	}

	nPosX = 10;
	nPosZ = 418;
	if(m_nGuildWarListSelect > -1)
	{
		int nRealSelect = m_nGuildWarListSelect;

		int iGrade = 0;
		ZeroMemory(szTmp, sizeof(szTmp));
		if(m_GuildWarList[nRealSelect].match_level == ML_HIGH)
		{
			wsprintf(szTmp, "%s : Junior (J)", g_Message[ETC_MESSAGE296].szMessage);
			iGrade = 0;
		}
		else
		{
			wsprintf(szTmp, "%s : Senior (S)", g_Message[ETC_MESSAGE296].szMessage);
			iGrade = 1;
		}
		nSize = lstrlen(szTmp);
		RenderFont(szTmp, m_fPosX+nPosX, m_fPosX+nPosX+(nSize*7), m_fPosZ+nPosZ, m_fPosZ+nPosZ+14, GetStartOrder()+4 );


		ZeroMemory(szTmp, sizeof(szTmp));
		if(m_GuildWarList[nRealSelect].match_type == MT_DEAH_MATCH)
		{
			wsprintf(szTmp, g_Message[ETC_MESSAGE1354].szMessage);//��� : ������ġ (D)
		}
		else //if(m_GuildWarList[nRealSelect].match_type == MT_KILLING_FIELD)
		{
			wsprintf(szTmp, g_Message[ETC_MESSAGE1355].szMessage);//��� : ų���ʵ� (K)
		}
		nSize = lstrlen(szTmp);
		RenderFont(szTmp, m_fPosX+nPosX, m_fPosX+nPosX+(nSize*7), m_fPosZ+nPosZ+15, m_fPosZ+nPosZ+15+14, GetStartOrder()+4 );

		for(i=0; i<m_GuildWarList[nRealSelect].player_count; i++)
		{
			ZeroMemory(szTmp, sizeof(szTmp));
			wsprintf(szTmp, g_Message[ETC_MESSAGE1353].szMessage, m_GuildWarList[nRealSelect].aGLD[iIsUse[i]].szGuildName, m_GuildWarList[nRealSelect].aGLD[iIsUse[i]].iRank[0],
				m_GuildWarList[nRealSelect].aGLD[iIsUse[i]].iTotalPoint, m_GuildWarList[nRealSelect].aGLD[iIsUse[i]].WeekPoint[iGrade]);//%4s %d�� %d�� (���:%d��)
			nSize = lstrlen(szTmp);
			RenderFont(szTmp, m_fPosX+nPosX, m_fPosX+nPosX+(nSize*7), m_fPosZ+nPosZ+30+i*15, m_fPosZ+nPosZ+30+14+i*15, GetStartOrder()+4 );
		}
	}

	ZeroMemory(szTmp, sizeof(szTmp));
	if( m_nGuildWarListMaxPage == 0)
		wsprintf(szTmp, "%3d/%d", 0, m_nGuildWarListMaxPage);
	else
		wsprintf(szTmp, "%3d/%d", m_nGuildWarListPage, m_nGuildWarListMaxPage);
	nSize = lstrlen(szTmp);
	RenderFont(szTmp, m_fPosX+120, m_fPosX+115+(nSize*8), m_fPosZ+384, m_fPosZ+384+14, GetStartOrder()+4 );
}

void CGroupWnd::RenderGuildWarInfoNew()
{
	char szGuildWarTemp[100];
	int i = 0;
	int nSize = 0;

	switch(m_byRenderType)
	{
	case GROUP_RENDER_TEXT__GUILDWAR_RANK:
		{
			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1420].szMessage); // "������ ��巩ŷ�� �����ݴϴ�."
			m_byMsgCnt = 1;
		}
		break;
	case GROUP_RENDER_TEXT__GUILDWAR_LIST:
		{
			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1421].szMessage); // "���� �������� ����� ����Ʈ��"
			lstrcpy(m_szInfo[1], g_Message[ETC_MESSAGE1422].szMessage); // "�����ݴϴ�."
			m_byMsgCnt = 2;
		}
		break;
	case GROUP_RENDER_TEXT__GUILDWAR_MATCH:
		{
			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1423].szMessage); // "����� ��Ī �ɼ��� �����մϴ�."
			m_byMsgCnt = 1;
		}
		break;
	case GROUP_RENDER_TEXT__GUILDWAR_CHALLENGE:
		{
			lstrcpy(m_szInfo[0], g_Message[ETC_MESSAGE1424].szMessage); // "���ϴ� ��忡 ���� ������û��"
			lstrcpy(m_szInfo[1], g_Message[ETC_MESSAGE1425].szMessage); // "�Ҽ� �ֽ��ϴ�."
			m_byMsgCnt = 2;
		}
		break;
	}	

	if(m_byRenderType!=GROUP_RENDER_NONE)
	{
		if(m_bInfo!=TRUE)
		{
			for(i = 0; i < m_byMsgCnt; i++)
			{
				nSize = lstrlen(m_szInfo[i]);
				RenderFont(m_szInfo[i], m_fPosX+10, m_fPosX+10+(nSize*7), m_fPosZ+419+15+(i*20), m_fPosZ+433+15+(i*20), GetStartOrder()+4);
			}		
		}
	}
	
	switch(m_byStatus)
	{
	case GROUP_GUILDWAR_RANK:
		{
			RenderTextGuildWarRankList();
		}
		break;

	case GROUP_GUILDWAR_LIST:
		{
			RenderTextGuildWarList();
		}
		break;

	case GROUP_GUILDWAR_MATCH:
		{
			ZeroMemory(szGuildWarTemp, sizeof(szGuildWarTemp));
			
			//�� �����..
			if( g_pMainPlayer->m_dwLevel <= 55 )
				wsprintf(szGuildWarTemp, g_Message[ETC_MESSAGE1356].szMessage);//��   ��    Junior(Lv55����)
			else
				wsprintf(szGuildWarTemp, g_Message[ETC_MESSAGE1357].szMessage);//��   ��    Senior(Lv56�̻�)
			nSize = lstrlen(szGuildWarTemp);
			RenderFont(szGuildWarTemp, m_fPosX+50, m_fPosX+50+(nSize*7), m_fPosZ+3+110, m_fPosZ+3+124, GetStartOrder()+4);

			ZeroMemory(szGuildWarTemp, sizeof(szGuildWarTemp));
			wsprintf(szGuildWarTemp, g_Message[ETC_MESSAGE1358].szMessage);//��������
			nSize = lstrlen(szGuildWarTemp);
			RenderFont(szGuildWarTemp, m_fPosX+10+50, m_fPosX+10+50+(nSize*7), m_fPosZ-16+170, m_fPosZ-16+184, GetStartOrder()+4);

			ZeroMemory(szGuildWarTemp, sizeof(szGuildWarTemp));
			wsprintf(szGuildWarTemp, g_Message[ETC_MESSAGE1359].szMessage);//��������
			nSize = lstrlen(szGuildWarTemp);
			RenderFont(szGuildWarTemp, m_fPosX+10+50, m_fPosX+10+50+(nSize*7), m_fPosZ-16+230, m_fPosZ-16+244, GetStartOrder()+4);

			ZeroMemory(szGuildWarTemp, sizeof(szGuildWarTemp));
			wsprintf(szGuildWarTemp, g_Message[ETC_MESSAGE1360].szMessage);//���������
			nSize = lstrlen(szGuildWarTemp);
			RenderFont(szGuildWarTemp, m_fPosX+10+50, m_fPosX+10+50+(nSize*7), m_fPosZ-16+290, m_fPosZ-16+304, GetStartOrder()+4);

			//��Ӵٿ� �ڽ� �ȿ� ���õ� ����
			nSize = lstrlen(m_szGuildWarMatch[m_bynGuildWarForm]);
			RenderFont(m_szGuildWarMatch[m_bynGuildWarForm], m_fPosX+127, m_fPosX+127+(nSize*7), m_fPosZ+3+170, m_fPosZ+3+184, GetStartOrder()+4);

			nSize = lstrlen(m_szGuildWarNumber[m_byGuildWarNumber]);
			RenderFont(m_szGuildWarNumber[m_byGuildWarNumber], m_fPosX+127, m_fPosX+127+(nSize*7), m_fPosZ+3+230, m_fPosZ+3+244, GetStartOrder()+4);

			nSize = lstrlen(m_szGuildWarType[m_byGuildWarType]);
			RenderFont(m_szGuildWarType[m_byGuildWarType], m_fPosX+127, m_fPosX+127+(nSize*7), m_fPosZ+3+290, m_fPosZ+3+304, GetStartOrder()+4);

			//��Ӵٿ� �ڽ��� �������� ���ǿ� ���� ���
			switch(m_byGuildWarDropDownEnableNum)
			{
			case 1:
				{
					for(i=0; i<2; i++)
					{
						nSize = lstrlen(m_szGuildWarMatch[i]);
						RenderFont(m_szGuildWarMatch[i], m_fPosX+127, m_fPosX+127+(nSize*7), m_fPosZ+3+170+(i+1)*15, m_fPosZ+3+184+(i+1)*15, GetStartOrder()+7);
					}
				}
				break;

			case 2:
				{
					for(i=0; i<4; i++)
					{
						nSize = lstrlen(m_szGuildWarNumber[i]);
						RenderFont(m_szGuildWarNumber[i], m_fPosX+127, m_fPosX+127+(nSize*7), m_fPosZ+3+230+(i+1)*15, m_fPosZ+3+244+(i+1)*15, GetStartOrder()+7);
					}
				}
				break;

			case 3:
				{
					for(i=0; i<4; i++)
					{
						nSize = lstrlen(m_szGuildWarType[i]);
						RenderFont(m_szGuildWarType[i], m_fPosX+127, m_fPosX+127+(nSize*7), m_fPosZ+3+290+(i+1)*15, m_fPosZ+3+304+(i+1)*15, GetStartOrder()+7);
					}
				}
				break;

			}
		}
		break;
	}
}

void CGroupWnd::SetGuildWarRenderStatus()
{
	SetRender(SPR_OBJ_GROUPWND_PARTY1, FALSE);
	SetRender(SPR_OBJ_GROUPWND_PARTY2, TRUE);
	SetRender(SPR_OBJ_GROUPWND_GUILD1, FALSE);
	SetRender(SPR_OBJ_GROUPWND_GUILD2, TRUE);
	SetRender(SPR_OBJ_GROUPWND_FRIEND1, FALSE);
	SetRender(SPR_OBJ_GROUPWND_FRIEND2, TRUE);
	SetRender(SPR_OBJ_GROUPWND_GUILDWAR1, TRUE);
	SetRender(SPR_OBJ_GROUPWND_GUILDWAR2, FALSE);
	
	if(m_byGroupTab == GROUP_TAB_GUILDWAR)
	{
		if(m_bGuildWarChallenge)
		{
			SetRender(SPR_OBJ_GUILDWAR_CHALLENGE2, TRUE);
		}
		else
		{
			SetRender(SPR_OBJ_GUILDWAR_CHALLENGE1, TRUE);
		}

		if(m_byStatus == GROUP_GUILDWAR_RANK)
		{
			SetRender(SPR_GUILDWAR_TITLEBAR_RANK, TRUE);

			SetRender(SPR_OBJ_GUILDWAR_RANK2, TRUE);
			SetRender(SPR_OBJ_GUILDWAR_LIST1, TRUE);
			SetRender(SPR_OBJ_GUILDWAR_MATCH1, TRUE);

			if(m_byBtnChallange == 1)
			{
				SetRender(SPR_OBJ_GUILDWAR_APPLY3, TRUE);
			}
			else if(m_byBtnChallange == 2)
			{
				SetRender(SPR_OBJ_GUILDWAR_APPLY2, TRUE);
			}
			else
			{
				SetRender(SPR_OBJ_GUILDWAR_APPLY1, TRUE);
			}
			m_byBtnChallange = 0;
		}
		else if(m_byStatus == GROUP_GUILDWAR_LIST)
		{
			SetRender(SPR_GUILDWAR_TITLEBAR_LIST, TRUE);

			SetRender(SPR_OBJ_GUILDWAR_RANK1, TRUE);
			//SetRender(SPR_OBJ_GUILDWAR_RANK3, TRUE);
			SetRender(SPR_OBJ_GUILDWAR_LIST2, TRUE);
			SetRender(SPR_OBJ_GUILDWAR_MATCH1, TRUE);

			if(m_byBtnOvserve == 1)
			{
				SetRender(SPR_OBJ_GUILDWAR_OBSERVE3, TRUE);
			}
			else if(m_byBtnOvserve == 2)
			{
				SetRender(SPR_OBJ_GUILDWAR_OBSERVE2, TRUE);
			}
			else
			{
				SetRender(SPR_OBJ_GUILDWAR_OBSERVE1, TRUE);
			}
			m_byBtnOvserve = 0;
		}
		else if(m_byStatus == GROUP_GUILDWAR_MATCH)
		{
			SetRender(SPR_GUILDWAR_TITLEBAR_MATCH, TRUE);

			SetRender(SPR_OBJ_GUILDWAR_RANK1, TRUE);
			SetRender(SPR_OBJ_GUILDWAR_LIST1, TRUE);
			//SetRender(SPR_OBJ_GUILDWAR_LIST3, TRUE);
			//SetRender(SPR_OBJ_GUILDWAR_RANK3, TRUE);
			SetRender(SPR_OBJ_GUILDWAR_MATCH2, TRUE);

			SetRender(DROPDOWN_OBJ_GUILDWAR_FORM_LEFT, TRUE);
			SetRender(DROPDOWN_OBJ_GUILDWAR_FORM_MIDDLE, TRUE);
			if(m_byGuildWarDropDownEnableNum == 1)
			{
				SetRender(DROPDOWN_OBJ_GUILDWAR_FORM_RIGHT2, TRUE);
				SetRender(DROPDOWN_OBJ_GUILDWAR_FORM_BOARD, TRUE);
			}
			else
				SetRender(DROPDOWN_OBJ_GUILDWAR_FORM_RIGHT1, TRUE);

			SetRender(DROPDOWN_OBJ_GUILDWAR_NUMBER_LEFT, TRUE);
			SetRender(DROPDOWN_OBJ_GUILDWAR_NUMBER_MIDDLE, TRUE);
			if(m_byGuildWarDropDownEnableNum == 2)
			{
				SetRender(DROPDOWN_OBJ_GUILDWAR_NUMBER_RIGHT2, TRUE);
				SetRender(DROPDOWN_OBJ_GUILDWAR_NUMBER_BOARD, TRUE);
			}
			else
				SetRender(DROPDOWN_OBJ_GUILDWAR_NUMBER_RIGHT1, TRUE);

			SetRender(DROPDOWN_OBJ_GUILDWAR_TYPE_LEFT, TRUE);
			SetRender(DROPDOWN_OBJ_GUILDWAR_TYPE_MIDDLE, TRUE);
			if(m_byGuildWarDropDownEnableNum == 3)
			{
				SetRender(DROPDOWN_OBJ_GUILDWAR_TYPE_RIGHT2, TRUE);
				SetRender(DROPDOWN_OBJ_GUILDWAR_TYPE_BOARD, TRUE);
			}
			else
				SetRender(DROPDOWN_OBJ_GUILDWAR_TYPE_RIGHT1, TRUE);


			if(m_bGuildWarMatchEnable)
			{
				SetRender(SPR_OBJ_GUILDWAR_AUTOMATCH_V, TRUE);
				SetRender(SPR_OBJ_GUILDWAR_AUTOMATCHING2, TRUE);
			}
			else
				SetRender(SPR_OBJ_GUILDWAR_AUTOMATCHING1, TRUE);

			SetRender(SPR_OBJ_GUILD_PAGE, FALSE);
			SetRender(SPR_OBJ_GUILD_PAGEPREV1, FALSE);
			SetRender(SPR_OBJ_GUILD_PAGEPREV2, FALSE);
			SetRender(SPR_OBJ_GUILD_PAGEPREV3, FALSE);
			SetRender(SPR_OBJ_GUILD_PAGENEXT1, FALSE);
			SetRender(SPR_OBJ_GUILD_PAGENEXT2, FALSE);
			SetRender(SPR_OBJ_GUILD_PAGENEXT3, FALSE);
		}

		if(m_byStatus == GROUP_GUILDWAR_RANK || m_byStatus == GROUP_GUILDWAR_LIST)
		{
			SetRender(SPR_OBJ_GUILD_PAGE, TRUE);
			if(m_byBtnLeftDouble == 1)
			{
				SetRender(SPR_OBJ_GUILDWAR_PAGEPREV_DOUBLE3, TRUE);
			}
			else if(m_byBtnLeftDouble == 2)
			{
				SetRender(SPR_OBJ_GUILDWAR_PAGEPREV_DOUBLE2, TRUE);
			}
			else
			{
				SetRender(SPR_OBJ_GUILDWAR_PAGEPREV_DOUBLE1, TRUE);
			}
			m_byBtnLeftDouble = 0;

			if(m_byBtnRightDouble == 1)
			{
				SetRender(SPR_OBJ_GUILDWAR_PAGENEXT_DOUBLE3, TRUE);
			}
			else if(m_byBtnRightDouble == 2)
			{
				SetRender(SPR_OBJ_GUILDWAR_PAGENEXT_DOUBLE2, TRUE);
			}
			else
			{
				SetRender(SPR_OBJ_GUILDWAR_PAGENEXT_DOUBLE1, TRUE);
			}
			m_byBtnRightDouble = 0;
		}
	}
}

void CGroupWnd::ReSetGuildWarRenderStatus()
{
	for(int iGW = SPR_OBJ_GUILDWAR_RANK1; iGW <= SPR_GUILDWAR_TITLEBAR_MATCH; iGW++)
	{
		SetRender(iGW, FALSE);
	}
}

void CGroupWnd::TempSetRandomData()
{
	// �ӽ� ����Ÿ�� �ִ� �κ� (���߿� �����ؾ� ��)
	//-----------------------------------------------------------------------
	return;
/*

	m_nGuildWarRankListMaxPage = 15;
	m_nGuildWarRankListTotalCount = 15;
	m_nGuildWarListMaxPage = 15;
	m_nGuildWarListTotalCount = 15;
	for(int i=0; i<MAX_LADDER_LIST; i++)//��巩ŷ ����Ʈ
	{
		m_LadderList[i].iRank[0] = i+1+(m_nGuildWarRankListPage-1)*15;
		m_LadderList[i].iRank[1] = i+1;
		m_LadderList[i].iGuildTotalCount = rand()%100;
		m_LadderList[i].WeekPoint[0] = rand()%500;
		m_LadderList[i].WeekPoint[1] = rand()%500;
		m_LadderList[i].MonthPoint[0] = rand()%500;
		m_LadderList[i].MonthPoint[1] = rand()%500;
		m_LadderList[i].iTotalPoint = rand()%1000;
		ZeroMemory(m_LadderList[i].szGuildName, sizeof(m_LadderList[i].szGuildName));
		ZeroMemory(m_LadderList[i].szGuildMaster, sizeof(m_LadderList[i].szGuildMaster));
		for(int j=0; j<10; j++)
		{
			m_LadderList[i].szGuildName[j] = 'a' + i;
			m_LadderList[i].szGuildMaster[j] = 'A' + i;
		}
	}

	for(i=0; i<MAX_LADDER_LIST; i++)//����� 
	{
		if( rand()%2 == 0 )
			m_GuildWarList[i].match_type = MT_DEAH_MATCH;
		else
			m_GuildWarList[i].match_type = MT_KILLING_FIELD;

		if(rand()%2)
			m_GuildWarList[i].match_level = ML_HIGH;
		else
			m_GuildWarList[i].match_level = ML_LOW;
		m_GuildWarList[i].player_count = rand()%4;
		if(	m_GuildWarList[i].player_count < 2 )
			m_GuildWarList[i].player_count = 2;
		for(int j=0; j<2; j++)
		{
			m_GuildWarList[i].aGLD[j].iRank[0] = i+j+1+(m_nGuildWarListPage-1)*15;
			m_GuildWarList[i].aGLD[j].iRank[1] = i+j+1+(m_nGuildWarListPage-1)*15;
			m_GuildWarList[i].aGLD[j].iGuildTotalCount = rand()%100;
			m_GuildWarList[i].aGLD[j].WeekPoint[0] = rand()%500;
			m_GuildWarList[i].aGLD[j].WeekPoint[1] = rand()%500;
			m_GuildWarList[i].aGLD[j].MonthPoint[0] = rand()%500;
			m_GuildWarList[i].aGLD[j].MonthPoint[1] = rand()%500;
			m_GuildWarList[i].aGLD[j].iTotalPoint = rand()%1000;
			ZeroMemory(m_GuildWarList[i].aGLD[j].szGuildName, sizeof(m_GuildWarList[i].aGLD[j].szGuildName));
			ZeroMemory(m_GuildWarList[i].aGLD[j].szGuildMaster, sizeof(m_GuildWarList[i].aGLD[j].szGuildMaster));
			for(int k=0; k<10; k++)
			{
				m_GuildWarList[i].aGLD[j].szGuildMaster[k] = 'a' + i;
			}
		}

		for(int k=0; k<10; k++)
		{
			m_GuildWarList[i].aGLD[0].szGuildName[k] = 'a' + i;
			m_GuildWarList[i].aGLD[1].szGuildName[k] = 'A' + i;
		}
	}
	//-----------------------------------------------------------------------
	*/
}

void CGroupWnd::InitGuildWarMatching()
{
	m_bynGuildWarForm = 0;
	m_byGuildWarNumber = 0;
	if(!g_pMainPlayer->m_dwGuildId)//��� ������ ������ �� �����.
	{
		m_byGuildWarType = 1;
	}
	else if(m_byGuildWarType<0 || m_byGuildWarType>2)
	{
		m_byGuildWarType = 0;
	}
	m_bGuildWarMatchEnable = FALSE;
}