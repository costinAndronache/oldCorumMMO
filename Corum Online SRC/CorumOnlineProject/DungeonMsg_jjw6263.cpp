#include "DungeonMsg_jjw6263.h"
#include "InitGame.h"
#include "GameControl.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "message.h"
#include "CodeFun.h"
#include "CorumResource.h"
#include "DungeonMsg.h"
#include "EventDungeonWnd.h"
#include "ItemMakingWnd.h"
#include "ItemMakingSubWnd.h"
#include "ItemUpgrade.h"
#include "ItemUpgradeSubWnd.h"
#include "DungeonSiegeInfoWnd.h"
#include "EffectProcess.h"
#include "DungeonTable.h"
#include "EventNPCSubWnd.h"
#include "EventDungeonProcess.h"
#include "ItemMoveManager.h"
#include "QuantityWnd.h"
#include "ItemNative.h"


void InitPacketProcDungeonJJW6263()
{
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_ITEMUPGRADE_SUCCESS]					= CmdUpgradeItemSuccess;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_ITEMUPGRADE_FAIL]						= CmdUpgradeItemFail;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_ITEMUPGRADE_FAIL_INSURANCE]				= CmdUpgradeItemFailInsurance; 				
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_UPGRADE_CANCEL]							= CmdUpgradeCancel;		
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_UPGRADE_CANCEL_RETURN_INV]				= CmdUpgradeItem_ReturnInv;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_ITEMMAKING_SUCCESS]						= CmdMakingItemSuccess;				
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_ITEMMAKING_FAIL]						= CmdMakingItemFail;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_ITEMMAKING_CANCEL]						= CmdMakingItemCancel;			
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_MAKING_CANCEL_RETURN_INV]				= CmdMakingItem_ReturnInv;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_GUARDIANSUPPLIES_CANCAL_RETURN_INV]		= CmdGuardianSupplies_ReturnInv;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_CLEAR]					= CmdEventDungeonClear;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_CLEAR_PRESENT]			= CmdEventDungeonClearPresent;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_LAST_LAYER]				= CmdEventDungeonLastLayer;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_FAILED]					= CmdEventDungeonFailed;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_CHANGE_LAYER_RESULT]				= CmdEnentDungeonChangeLayer;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_ALIVE_MONSTER_COUNT]				= CmdEventAliveMonsterCount;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_EXCEPTION_RESULT]					= CmdEventExceptionResult;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_ENTER]					= CmdEventDungeonEnter;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_OPEN_INFO]				= CmdEventDungeonOpenInfo;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_CLEAR_CONDITION]					= CmdEventClearCondition;	
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_ALIVE_USER_COUNT]					= CmdEventAliveUserCount;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_LAYER_CLEAR]				= CmdEventDungeonLayerClear;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_CLOSE]					= CmdEventClose;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_FIRST_TIMEOUT]			= CmdEventDungeonFirstTimeOut;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_TRAINING_DISPLAY]			= CmdEventDungeonTraninigDisPlay;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_TRAINING_ONELAYER_CLAYER]	= CmdEventDungeonTraninigOneLayerClayer;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_SURVIVAL_DISPLAY]			= CmdEventDungeonSurvivalDisPlay;			
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_CLOSE_DISPLAY]			= CmdEventDungeonCloseDisPlay;			
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_JOINUSER_COUNT]			= CmdEventDungeonJoinUserCount;	
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_CLEARUSER_SEND]			= CmdEventDungeonClearUserSend;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_ITEM_MOVE]								= CmdItemMove;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_ITEM_USED_NATIVE]						= CmdItemUsedNative;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_SWITCH_WEAPON]							= CmdSwitchWeapon;

#ifdef DEVELOP_MODE	
	if( IsUltra() )
	{
		PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_ITEM_PICKUP]						= NULL;
		g_ItemMoveManager.SetNewItemMoveMode(TRUE);
	}		
#endif
}


BOOL IsUltra()
{
	WORD	wVersionRequested;
	WSADATA wsaData;
	int		err;

	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 )
	{
		return FALSE;
	}

	if ( LOBYTE( wsaData.wVersion ) != 2 ||	HIBYTE( wsaData.wVersion ) != 2 )
	{
		WSACleanup();
		return FALSE; 
	}

	char name[255] = {0,};
	char szip[16] = {0,};
	struct hostent* hostinfo;

	if( gethostname ( name, sizeof(name)) == 0 )
	{
		if((hostinfo = gethostbyname(name)) != NULL)
		{
			for(int i=0; hostinfo->h_addr_list[i] != NULL; i++)
			{
				strcpy(szip, inet_ntoa (*(struct in_addr *)hostinfo->h_addr_list[i]));             
			}
		}
	}

	if( !strcmp(szip,"222.106.120.35")	||
		!strcmp(szip,"222.106.120.16")	||
		!strcmp(szip,"222.106.120.13")	||
		!strcmp(szip,"222.106.120.12"))
	{
		WSACleanup();
//		return TRUE;
		return FALSE;
	}

	WSACleanup();

	return FALSE;
}


//----------------------------------------------------------------------------
// ������ �̵� 
//----------------------------------------------------------------------------
void CmdItemMove( char* pMsg, DWORD dwLen )
{
	g_ItemMoveManager.Process(pMsg,dwLen);
	g_ItemMoveManager.Initialize();

	CQuantityWnd* pQuantityWnd = CQuantityWnd::GetInstance();
	pQuantityWnd->m_bReq = FALSE;
}


//----------------------------------------------------------------------------
// ������ ��� 
//----------------------------------------------------------------------------
void CmdItemUsedNative( char* pMsg, DWORD dwLen )
{
	g_ItemUsedManager.Process(pMsg,dwLen);
}


//----------------------------------------------------------------------------
// SwitchWeapon : 'W'Ű 
//----------------------------------------------------------------------------
void CmdSwitchWeapon( char* pMsg, DWORD dwLen )
{
	DSTC_SWITCH_WEAPON* pPacket = (DSTC_SWITCH_WEAPON*)pMsg;
	g_pMainPlayer->m_bCurrnetHand = pPacket->bCurrnetHand;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CmdEnentDungeonChangeLayer( char* pMsg, DWORD dwLen )
{
	DSTC_EVENT_CHANGE_LAYER_RESULT* pPacket = (DSTC_EVENT_CHANGE_LAYER_RESULT*)pMsg;
	
	if (1 == pPacket->bMoveResult)
	{
		// "���� ����ִ� ���Ͱ� �ֽ��ϴ�! �Ʒ������� �������� �� �����ϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE521].szMessage, TEXT_COLOR_YELLOW); 
	}
	if (2 == pPacket->bMoveResult)
	{
		// "������ ����� ���� ���մϴ�. �������� �� �����ؾ� �մϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE755].szMessage, TEXT_COLOR_YELLOW); 
	}
	if (3 == pPacket->bMoveResult)
	{
		// "������ ����� ���� ���մϴ�. ����ڰ� �� �ʿ��մϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE756].szMessage, TEXT_COLOR_YELLOW); 
	}
}


//----------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------
void CmdEventAliveMonsterCount(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_ALIVE_MONSTER_COUNT* pPacket = (DSTC_EVENT_ALIVE_MONSTER_COUNT*)pMsg;	
	CDungeonSiegeInfoWnd* pDungeonSiegeInfoWnd = CDungeonSiegeInfoWnd::GetInstance();
	pDungeonSiegeInfoWnd->m_iTotalMonsterCount = (INT)pPacket->wAliveMonsterCount;	
	
	if(NULL != g_pThisDungeon)
	{	
		if(	DUNGEON_TYPE_EVENT == g_pThisDungeon->GetDungeonType() &&
			FALSE == g_bIsLastLayer &&
			g_pThisDungeon->m_byEventType == EVENT_DUNGEON_TRAINING )
		{		
			DisplayMessageAdd("������ ����� ���������ϴ�.�Ʒ������� �̵��Ҽ� �ֽ��ϴ�.", TEXT_COLOR_YELLOW);		
		}
	}
	
	if (TRUE == g_bIsLastLayer)
	{
		g_bIsLastLayer = FALSE;
	}	
}


//----------------------------------------------------------------------------
// �̺�Ʈ ���� Ŭ����
//----------------------------------------------------------------------------
void CmdEventDungeonClear(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_DUNGEON_CLEAR*	pPacket = (DSTC_EVENT_DUNGEON_CLEAR*)pMsg;
	CUser*						pUser	= g_pUserHash->GetData( pPacket->dwUserIndex );

	EffectDesc* pEffectDesc = g_pEffectLayer->CreateGXObject(FILE(EFFECT_DUNGEONDEFENSESUCCESS_WORLD)
		, 1, __CHR_EFFECT_NONE__);

	pEffectDesc->dwTargetIndex[0] = pPacket->dwUserIndex;
	::SetAction( pEffectDesc->hEffect.pHandle, 1, 0, ACTION_LOOP );
	GXSetPosition(pEffectDesc->hEffect.pHandle,&pUser->m_v3CurPos,FALSE);
	pEffectDesc->hEffect.pDesc->ObjectFunc = EffectEventDungeonFunc;
	
	// "�̺�Ʈ ���� Ŭ����!!"
	DisplayMessageAdd(g_Message[ETC_MESSAGE523].szMessage, TEXT_COLOR_YELLOW);		
	InitEventRelationVariables();
}


void CmdEventDungeonClearPresent(char* pMsg, DWORD dwLen)
{
	
}


//----------------------------------------------------------------------------
// ���ô����� ������ ���Դϴ�.
//----------------------------------------------------------------------------
void CmdEventDungeonLastLayer(char* pMsg, DWORD dwLen)
{
	g_bIsLastLayer = TRUE;	
	DisplayMessageAdd(g_Message[ETC_MESSAGE1113].szMessage,TEXT_COLOR_YELLOW);
}


//----------------------------------------------------------------------------
// �̺�Ʈ ���� ���� �޽��� ���.
//----------------------------------------------------------------------------
void CmdEventDungeonFailed(char* pMsg, DWORD dwLen)
{
	CEventDungeonWnd* pEventDungeonWnd = CEventDungeonWnd::GetInstance();
	
	pEventDungeonWnd->m_iMessageType = EVENT_FAILED;
	pEventDungeonWnd->SetActive();
	
	InitEventRelationVariables();
}


//----------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------
void CmdEventExceptionResult(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_EXCEPTION_RESULT* pPacket = (DSTC_EVENT_EXCEPTION_RESULT*)pMsg;
	
	switch (pPacket->byExceptionCode)
	{
	case 0:			// ��Ż ����� �����Ǿ� �ִ�.

		// "�̺�Ʈ ���������� ��Ż����� �̿��� �� �����ϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE524].szMessage, TEXT_COLOR_YELLOW);
		g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL);		
		
		break;
		
	case 1:			// ����� ī�� ����� �����Ǿ� �ִ�.

		// "�̺�Ʈ ���������� ����� ī�带 ����� �� �����ϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE525].szMessage, TEXT_COLOR_YELLOW);
		g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL);
		break;
		
	default:
		break;
	}
}


//----------------------------------------------------------------------------
// �������� 1�� �̺�Ʈ ���� �Ǿ���.
//----------------------------------------------------------------------------
void CmdEventDungeonFirstTimeOut(char* pMsg, DWORD dwLen)
{
	// �������� 1�� �̺�Ʈ ���� �޼��� ��� 
	DSTC_EVENT_DUNGEON_FIRST_TIMEOUT* pPacket = (DSTC_EVENT_DUNGEON_FIRST_TIMEOUT*)pMsg;

	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if (NULL == pDungeon)	return;	

	CEventDungeonWnd* pEventDungeonWnd = CEventDungeonWnd::GetInstance();
	pEventDungeonWnd->m_iMessageType = EVENT_DUNGEON_SURVIVAL_FIRST_TIMEOUT;
	pEventDungeonWnd->m_bDungeonMonsterExpRatio = pPacket->bMonsterExpRatio;
	pEventDungeonWnd->SetActive();

	g_pMainPlayer->m_bInEvnetDungeonFirstTimeOut = TRUE;
	
	ListNode<CMonster>*	pMonsterNode	= g_pMonsterHash->GetHead();
	CMonster*			pMonster		= NULL;

	while( pMonsterNode )
	{
		pMonster = pMonsterNode->pData;

		if(pMonster)
		{
			if(pMonster->IsNormalMonster())
			{
				pMonster->SetAction( MON_MOTION_TYPE_STAND1, 0, ACTION_ONCE );
				pMonster->m_hMonster.pDesc->ObjectFunc		= MonsterKillFunc;
				pMonster->m_dwTemp[MONSTER_TEMP_PUSHING]	= 1;
			}
		}
		
		pMonsterNode = pMonsterNode->pNext;
	}
}


//----------------------------------------------------------------------------
// �̺�Ʈ ����(���ô���)���� �������� ��ƾ� �� ������ ���� ������ �����ش�.   
//----------------------------------------------------------------------------
void CmdEventDungeonTraninigDisPlay(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_DUNGEON_TRAINING_DISPLAY* pPacket = (DSTC_EVENT_DUNGEON_TRAINING_DISPLAY*)pMsg;

	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if (NULL == pDungeon)	return;

	CDungeonSiegeInfoWnd* pDungeonSiegeInfoWnd = CDungeonSiegeInfoWnd::GetInstance();
	pDungeonSiegeInfoWnd->m_dwTotalKillMonSterLayerCount	= pPacket->wTotalKillMonSterLayerCount;
	pDungeonSiegeInfoWnd->m_dwCurrentKillMonsterLayerCount	= pPacket->wCurrentKillMonsterLayerCount;
	pDungeonSiegeInfoWnd->m_dwPartyCurrentKillMonsterLayerCount = pPacket->wPartyCurrentKillMonsterLayerCount;
	pDungeonSiegeInfoWnd->m_bPartySameDungeonLayerUserCount = pPacket->bPartySameDungeonLayerUserCount;
	pDungeonSiegeInfoWnd->m_iType							= SIEGEINFOWND_TYPE_EVENT_TRAINING;
}


//----------------------------------------------------------------------------
// �̺�Ʈ ����(��������)���� �������� ���������� �� �������� �����ش�.   
//----------------------------------------------------------------------------
void CmdEventDungeonSurvivalDisPlay(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_DUNGEON_SURVIVAL_DISPLAY* pPacket = (DSTC_EVENT_DUNGEON_SURVIVAL_DISPLAY*)pMsg;
	
	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if (NULL == pDungeon)	return;
	
	CDungeonSiegeInfoWnd* pDungeonSiegeInfoWnd = CDungeonSiegeInfoWnd::GetInstance();
	pDungeonSiegeInfoWnd->m_dwCurrentMonterLayerCount	= pPacket->wCurrentMonterLayerCount;	/*�������尴��*/
	pDungeonSiegeInfoWnd->m_dwKillMonsterLayerCount		= pPacket->wKillMonsterLayerCount; 
	pDungeonSiegeInfoWnd->m_iType						= pPacket->byDisplayMode;
}


//----------------------------------------------------------------------------
// �̺�Ʈ ������ ���� �ɲ���°��� �˷��ش�.
//----------------------------------------------------------------------------
void CmdEventDungeonCloseDisPlay(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_DUNGEON_CLOSE_DISPLAY* pPacket = (DSTC_EVENT_DUNGEON_CLOSE_DISPLAY*)pMsg;
	
	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if (NULL == pDungeon)	return;	

	// "����� �̺�Ʈ ���� �����մϴ�."
	DisplayMessageAdd(g_Message[ETC_MESSAGE1117].szMessage,TEXT_COLOR_YELLOW);
}


//----------------------------------------------------------------------------
// �̺�Ʈ ������ ���� ���尴�� ���÷��� ���ش�.
//----------------------------------------------------------------------------
void CmdEventDungeonJoinUserCount(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_DUNGEON_JOINUSER_COUNT* pPacket = (DSTC_EVENT_DUNGEON_JOINUSER_COUNT*)pMsg;

	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if (NULL == pDungeon)	return;	

	CDungeonSiegeInfoWnd* pDungeonSiegeInfoWnd = CDungeonSiegeInfoWnd::GetInstance();
	pDungeonSiegeInfoWnd->m_dwCurrentMonterLayerCount = pPacket->wJoinUserCount;	/*�������尴��*/
}


//----------------------------------------------------------------------------
// �̺�Ʈ �������� Ŭ������ ������ �˷��ش�.
//----------------------------------------------------------------------------
void CmdEventDungeonClearUserSend(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_DUNGEON_CLEARUSER_SEND* pPacket = (DSTC_EVENT_DUNGEON_CLEARUSER_SEND*)pMsg;
	
	// ��Ʈ���� �������.
	char szOutMessage[0xff];
	memset(szOutMessage, 0, 0xff);	

	// "[%s]���� �̺�Ʈ ���� Ŭ���� �߽��ϴ�."
	wsprintf(szOutMessage, g_Message[ETC_MESSAGE1210].szMessage,pPacket->szCharName); 
	DisplayMessageAdd(szOutMessage,TEXT_COLOR_YELLOW);
}


//----------------------------------------------------------------------------
// �̺�Ʈ ����(���ô���)���� �������� ��ƾ� �� ������ ä����.���������� �̵� �����ϴ�.
//----------------------------------------------------------------------------
void CmdEventDungeonTraninigOneLayerClayer(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_DUNGEON_TRAINING_ONELAYER_CLAYER* pPacket = (DSTC_EVENT_DUNGEON_TRAINING_ONELAYER_CLAYER*)pMsg;
	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if (NULL == pDungeon)
	{
		return;
	}

	// "������ ����� ���������ϴ�.�Ʒ������� �̵��Ҽ� �ֽ��ϴ�."
	DisplayMessageAdd(g_Message[ETC_MESSAGE811].szMessage, TEXT_COLOR_YELLOW);	
}


//----------------------------------------------------------------------------
// �̺�Ʈ ������ ����!! 
//----------------------------------------------------------------------------
void CmdEventDungeonEnter(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_DUNGEON_ENTER* pPacket = (DSTC_EVENT_DUNGEON_ENTER*)pMsg;
	
	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if (NULL == pDungeon)	return;	

	pDungeon->m_byEventType = pPacket->byEventType;
	
	CEventDungeonWnd*		pEventDungeonWnd		= CEventDungeonWnd::GetInstance();
	CDungeonSiegeInfoWnd*	pDungeonSiegeInfoWnd	= CDungeonSiegeInfoWnd::GetInstance();
	
	pEventDungeonWnd->m_iMessageType = EVENT_ENTER;
	pEventDungeonWnd->SetActive();
	
	pDungeonSiegeInfoWnd->m_iType = pDungeon->m_byEventType;
	pDungeonSiegeInfoWnd->SetActive();

	// �̺�Ʈ ���� ���� ������ ����������� ��Ż�� ������ ���������� �˷��ش�. 
	CTDS_EVENT_USER_PREV_DUNGEON_ID SendPacket;
	SendPacket.wPrevDungeonID = g_pMainPlayer->wPrevDungeonID;
	g_pNet->SendMsg((char*)&SendPacket,SendPacket.GetPacketSize(),SERVER_INDEX_ZONE);	
}


//----------------------------------------------------------------------------
// �̺�Ʈ ��������. �̺�Ʈ ���Ǿ�â�� ���� ���� ����Ʈ ������ 
//----------------------------------------------------------------------------
void CmdEventDungeonOpenInfo(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_DUNGEON_OPEN_INFO*	pPacket		= (DSTC_EVENT_DUNGEON_OPEN_INFO*)pMsg;
	DUNGEON_DATA_EX*				pDungeon	= g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if(NULL == pDungeon)	return;			
	
	pDungeon->m_bEventState = pPacket->byEventState;

	if(EVENT_DUNGEON_OPEN == pDungeon->m_bEventState)
	{
		pDungeon->m_dwEventEndTick = pPacket->dwEventEndTick + g_dwCurTick;//���� �ð� ���� : 050108 hwoarang
	}

	char szOutMessage[0xff] = {0,};
	wsprintf(szOutMessage,"[%s](Lv.%d ~ Lv.%d) �̺�Ʈ ������ ���µǾ����ϴ�."
		, pPacket->szDungeonName, pDungeon->m_byEventEnterMinLevel, pDungeon->m_byEventEnterMaxLevel);
	DisplayMessageAdd(szOutMessage, TEXT_COLOR_YELLOW);
	
	CEventNPCSubWnd* pEventNPCSubWnd = CEventNPCSubWnd::GetInstance();
	pEventNPCSubWnd->QueryCanMoveDungeon();		
}


//----------------------------------------------------------------------------
// �̺�Ʈ ���� �Ա��� �����ų� ����  
//----------------------------------------------------------------------------
void CmdEventClose(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_DUNGEON_CLOSE*	pPacket		= (DSTC_EVENT_DUNGEON_CLOSE*)pMsg;
	DUNGEON_DATA_EX*			pDungeon	= g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);

	if(NULL == pDungeon)		return;

	pDungeon->m_bEventState = (BYTE)pPacket->byEventState;

	if(EVENT_DUNGEON_OPEN_TIME_UP == pDungeon->m_bEventState)
	{
		char szOutMessage[0xff] = {0,};
		wsprintf(szOutMessage,"[%s](Lv.%d ~ Lv.%d) �̺�Ʈ ���� �Ա��� �������ϴ�."
			, pPacket->szDungeonName, pDungeon->m_byEventEnterMinLevel, pDungeon->m_byEventEnterMaxLevel);
		DisplayMessageAdd(szOutMessage, TEXT_COLOR_YELLOW);		
	}

	if(EVENT_DUNGEON_CLOSE == pDungeon->m_bEventState)
	{
		pDungeon->m_dwEventEndTick = 0;

		char szOutMessage[0xff] = {0,};
		wsprintf(szOutMessage,"[%s](Lv.%d ~ Lv.%d) �̺�Ʈ ������ ����Ǿ����ϴ�."
			, pPacket->szDungeonName, pDungeon->m_byEventEnterMinLevel, pDungeon->m_byEventEnterMaxLevel);
		DisplayMessageAdd(szOutMessage, TEXT_COLOR_YELLOW);				
	}	
	
	CEventNPCSubWnd* pEventNPCSubWnd = CEventNPCSubWnd::GetInstance();
	pEventNPCSubWnd->QueryCanMoveDungeon();	
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CmdEventClearCondition(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_CLEAR_CONDITION*	pPacket	= (DSTC_EVENT_CLEAR_CONDITION*)pMsg;
	
	g_iNeedUserNo			= pPacket->wNeedUserNumber;
	g_iNeedKillMonsterNo	= pPacket->wNeedToKillMonsterNumber;
	
	CDungeonSiegeInfoWnd* pEventDungeonInfoWnd = CDungeonSiegeInfoWnd::GetInstance();
	
	pEventDungeonInfoWnd->m_iNeedKillMonsterNumber		= g_iNeedKillMonsterNo;	
	pEventDungeonInfoWnd->m_iNeedUserNumber				= g_iNeedUserNo;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CmdEventAliveUserCount(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_ALIVE_USER_COUNT* pPacket = (DSTC_EVENT_ALIVE_USER_COUNT*)pMsg;
	
	g_iCurrentLayerUserNo		= pPacket->wAliveUserCount;
	g_iTotalUserCount			= pPacket->wTotalUserCount;
	
	CDungeonSiegeInfoWnd* pEventDungeonInfoWnd = CDungeonSiegeInfoWnd::GetInstance();
	
	pEventDungeonInfoWnd->m_iLayerTotalUserNumber   = g_iCurrentLayerUserNo;
	pEventDungeonInfoWnd->m_iDungeonTotalUserNumber = g_iTotalUserCount;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CmdEventDungeonLayerClear(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_DUNGEON_LAYER_CLEAR* pPacket = (DSTC_EVENT_DUNGEON_LAYER_CLEAR*)pMsg;
	
	if (0 == pPacket->byType)	// ��Ʋ�ο� ����̶��,
	{
		g_dwChangeLayerRemainTime = (DWORD)pPacket->wRemainTime;
	}	
	else
	{
		g_dwChangeLayerRemainTime = 0xffffffff; // 0xffffffff ���� ī���� ���� ������ ǥ��.
	}
}


// ===================
// ������ ���� ��� 
// ===================
void CmdMakingItemCancel( char* pMsg, DWORD dwLen )
{
	DSTC_MAKING_CANCEL* pPacket = (DSTC_MAKING_CANCEL*)pMsg;

	for(int i = 1; i < 4; i++)
	{
		if(pPacket->pItem[i].GetID()!=0)
		{
			BYTE byCnt = Insert_SmallInvItem(&pPacket->pItem[i], 0, pPacket->pItem[i].GetQuantity());

			if(byCnt==0)
				memset(&pPacket->pItem[i], 0, sizeof(CItem));
			else
				pPacket->pItem[i].SetQuantity(byCnt);
		}
	}

	for(i = 1; i < 4; i++)
	{
		if(pPacket->pItem[i].GetID()!=0)
		{
			BYTE byCnt = Insert_SmallBankItem(&pPacket->pItem[i], 0, pPacket->pItem[i].GetQuantity());

			if(byCnt==0)
				memset(&pPacket->pItem[i], 0, sizeof(CItem));
			else
				pPacket->pItem[i].SetQuantity(byCnt);
		}
	}
		
	if(pPacket->bZipCode!=255)
	{
		if(pPacket->byType==0)
		{
			memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &pPacket->pItem[0], sizeof(CItem));	
			memset(&pPacket->pItem[0], 0, sizeof(CItem));
		}
		else
		{
			memcpy(&g_pMainPlayer->m_pBank_Large[pPacket->bZipCode], &pPacket->pItem[0], sizeof(CItem));	
			memset(&pPacket->pItem[0], 0, sizeof(CItem));
		}
	}

	CItemMakingWnd* pItemMakingWnd = CItemMakingWnd::GetInstance();
	
	for(i = 0; i < 4; i++)
		memcpy(&pItemMakingWnd->m_pMakingItem[i], &pPacket->pItem[0], sizeof(CItem));
		
	pItemMakingWnd->m_byProcess = 0;
	
	if(pPacket->bZipCode==255)
	{
		// "������������ ��� �Ǿ����ϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE1211].szMessage, 0xFFFF2CFF); 
	}
	else if(pPacket->byType==0)
	{
		// "�κ��丮�� �̵��մϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE1114].szMessage, 0xFFFF2CFF); 
	}
	else 
	{
		// "�κ��丮�� ���������� �����Ͽ� â��� �̵��մϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE1115].szMessage, 0xFFFF2CFF); 
	}
}


void CmdGuardianSupplies_ReturnInv( char* pMsg, DWORD dwLen )
{
	DSTC_GUARDIANSUPPLIES_CANCAL_RETURN_INV* pPacket = (DSTC_GUARDIANSUPPLIES_CANCAL_RETURN_INV*)pMsg;
	
	ITEM_NATIVE_INFO	ItemNativeInfo_Src;
	ITEM_NATIVE_INFO	ItemNativeInfo_Dest;
	const CItem*		pItemNativeSrc	= NULL;
	const CItem*		pItemNativeDest = NULL;

	ItemNativeInfo_Src.eSlotID		= pPacket->ItemNodeNative.eSlotID;
	ItemNativeInfo_Src.bySlotIndex	= pPacket->ItemNodeNative.bySlotIndex;
	ItemNativeInfo_Src.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;

	pItemNativeSrc = GetItemNative(&ItemNativeInfo_Src);

	ItemNativeInfo_Dest.eSlotID		= ITEM_NATIVE_INV_SMALL;
	ItemNativeInfo_Dest.bySlotIndex	= 0;
	ItemNativeInfo_Dest.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;

	pItemNativeDest = GetItemNative(&ItemNativeInfo_Dest);

	for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
	{
		ItemNativeInfo_Dest.bySlotIndex = i;	

		if(pItemNativeDest[i].GetID() == 0)
		{
			CItem cItem;
			memcpy(&cItem, pItemNativeSrc, sizeof(CItem));

			SetSlotItemNative(&cItem, &ItemNativeInfo_Dest);
			RemoveItemNative(&ItemNativeInfo_Src);
			return;
		}
		else if(pItemNativeDest[i].GetID() == pItemNativeSrc->GetID())
		{
//			LP_ITEM_TABLE pBaseItemDest = g_pItemTableHash->GetData(pItemNativeDest[i].GetID());
			CBaseItem* pBaseItemDest = g_pItemTableHash->GetData(pItemNativeDest[i].GetID());

			BYTE byMaxQuantity	= pBaseItemDest->GetMaxQuantity();
			BYTE byPutInCnt		= BYTE(byMaxQuantity - pItemNativeDest[i].GetQuantity());
			BYTE byCnt			= pItemNativeSrc->GetQuantity();

			if(byPutInCnt >= byCnt)
			{
				CItem cItemDest;
				memcpy(&cItemDest, &pItemNativeDest[i], sizeof(CItem));
				cItemDest.SetQuantity(BYTE(cItemDest.GetQuantity() + byCnt));

				SetSlotItemNative(&cItemDest, &ItemNativeInfo_Dest);
				RemoveItemNative(&ItemNativeInfo_Src);
			}
			else if(byPutInCnt != 0)
			{
				CItem cItemSrc;
				cItemSrc.SetQuantity(BYTE(cItemSrc.GetQuantity() - byPutInCnt));
				SetSlotItemNative(&cItemSrc, &ItemNativeInfo_Src);

				CItem cItemDest;
				memcpy(&cItemDest, &pItemNativeDest[i], sizeof(CItem));
				cItemDest.SetQuantity(BYTE(cItemDest.GetQuantity() + byPutInCnt));
				SetSlotItemNative(&cItemDest, &ItemNativeInfo_Dest);
			}
		}
	}
}


void CmdMakingItem_ReturnInv( char* pMsg, DWORD dwLen )
{
	DSTC_MAKING_CANCAL_RETURN_INV* pPacket = (DSTC_MAKING_CANCAL_RETURN_INV*)pMsg;

	ITEM_NATIVE_INFO	ItemNativeInfo_Src;
	ITEM_NATIVE_INFO	ItemNativeInfo_Dest;
//	CBaseItem*			pBaseItemSrc		= NULL;
//	CBaseItem*			pBaseItemDest		= NULL;
//	const CItem*		pItemSrc			= NULL;
//	const CItem*		pItemDest			= NULL;
	
	ItemNativeInfo_Src.eSlotID		= ITEM_NATIVE_MAKING;
	ItemNativeInfo_Src.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;
	
	ItemNativeInfo_Dest.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;

	for(int i = 0; i < pPacket->bItemCount; i++)
	{
		ItemNativeInfo_Src.bySlotIndex = pPacket->ItemNodeNative[i].index;				
		
		ItemNativeInfo_Dest.eSlotID		= pPacket->ItemNodeNative[i].eSlotID;
		ItemNativeInfo_Dest.bySlotIndex = pPacket->ItemNodeNative[i].bySlotIndex;
		
		AllLinkSrcToDestNative(&ItemNativeInfo_Src,&ItemNativeInfo_Dest);
	}	

	// "������������ ��� �Ǿ����ϴ�."
	DisplayMessageAdd(g_Message[ETC_MESSAGE1211].szMessage, 0xFFFF2CFF);
}


// ===================
// ���׷��̵� ��� 
// ===================
void CmdUpgradeCancel( char* pMsg, DWORD dwLen )
{	
	DSTC_UPGRADE_FAIL* pPacket = (DSTC_UPGRADE_FAIL*)pMsg;

	for(int i = 1; i < 4; i++)
	{
		if(pPacket->pItem[i].GetID()!=0)
		{
			BYTE byCnt = Insert_SmallInvItem(&pPacket->pItem[i], 0, pPacket->pItem[i].GetQuantity());

			if(byCnt==0)
				memset(&pPacket->pItem[i], 0, sizeof(CItem));
			else
				pPacket->pItem[i].SetQuantity(byCnt);
		}
	}

	for(i = 1; i < 4; i++)
	{
		if(pPacket->pItem[i].GetID()!=0)
		{
			BYTE byCnt = Insert_SmallBankItem(&pPacket->pItem[i], 0, pPacket->pItem[i].GetQuantity());

			if(byCnt==0)
				memset(&pPacket->pItem[i], 0, sizeof(CItem));
			else
				pPacket->pItem[i].SetQuantity(byCnt);
		}
	}
		
	if(pPacket->bZipCode!=255)
	{
		if(pPacket->byType==0)
		{
			memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &pPacket->pItem[0], sizeof(CItem));	
			memset(&pPacket->pItem[0], 0, sizeof(CItem));
		}
		else
		{
			memcpy(&g_pMainPlayer->m_pBank_Large[pPacket->bZipCode], &pPacket->pItem[0], sizeof(CItem));	
			memset(&pPacket->pItem[0], 0, sizeof(CItem));
		}
	}

	CItemUpgradeWnd* pItemUpgradeWnd = CItemUpgradeWnd::GetInstance();
	
	for(i = 0; i < 4; i++)
		memcpy(&pItemUpgradeWnd->m_pUpgradeItem[i], &pPacket->pItem[0], sizeof(CItem));
		
	pItemUpgradeWnd->m_bWaitingUpgrade = 0;
	
	if(pPacket->bZipCode==255)
	{
		// "�����۰�ȭ�� ��� �Ǿ����ϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE588].szMessage, 0xFFFF2CFF); 
	}
	else if(pPacket->byType==0)
	{
		// "�κ��丮�� �̵��մϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE1114].szMessage, 0xFFFF2CFF); 
	}
	else 
	{
		// "�κ��丮�� ���������� �����Ͽ� â��� �̵��մϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE1115].szMessage, 0xFFFF2CFF); 
	}
}


void CmdUpgradeItem_ReturnInv( char* pMsg, DWORD dwLen )
{
	DSTC_UPGRADE_CANCAL_RETURN_INV* pPacket = (DSTC_UPGRADE_CANCAL_RETURN_INV*)pMsg;

	ITEM_NATIVE_INFO	ItemNativeInfo_Src;
	ITEM_NATIVE_INFO	ItemNativeInfo_Dest;
//	CBaseItem*			pBaseItemSrc		= NULL;
//	CBaseItem*			pBaseItemDest		= NULL;
//	const CItem*		pItemSrc			= NULL;
//	const CItem*		pItemDest			= NULL;
	
	ItemNativeInfo_Src.eSlotID		= ITEM_NATIVE_UPGRADE;
	ItemNativeInfo_Src.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;
	
	ItemNativeInfo_Dest.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;

	for(int i = 0; i < pPacket->bItemCount; i++)
	{
		ItemNativeInfo_Src.bySlotIndex = pPacket->ItemNodeNative[i].index;				
		
		ItemNativeInfo_Dest.eSlotID		= pPacket->ItemNodeNative[i].eSlotID;
		ItemNativeInfo_Dest.bySlotIndex = pPacket->ItemNodeNative[i].bySlotIndex;
		
		AllLinkSrcToDestNative(&ItemNativeInfo_Src,&ItemNativeInfo_Dest);
	}
	
	// "�����۰�ȭ�� ��� �Ǿ����ϴ�."
	DisplayMessageAdd(g_Message[ETC_MESSAGE588].szMessage, 0xFFFF2CFF); 
}


// ===================
// ���׷��̵� ����  
// ===================
void CmdUpgradeItemFail( char* pMsg, DWORD dwLen )
{
//	DSTC_ITEMUPGRADE_FAIL* pPacket		= (DSTC_ITEMUPGRADE_FAIL*)pMsg;
	CItemUpgradeWnd* pItemUpgradeWnd	= CItemUpgradeWnd::GetInstance();
	CItemUpgradeSubWnd* pItemUpgradeSubWnd	= CItemUpgradeSubWnd::GetInstance(); 

	pItemUpgradeWnd->m_bWaitingUpgrade	= ITEM_UPGRADE_FAIL;
	pItemUpgradeSubWnd->m_byItemUpgradeProcess = ITEM_UPGRADE_PROCESS_FAIL;

	memset(&pItemUpgradeWnd->m_pUpgradeItem[0],0,sizeof(CItem));
	memset(&pItemUpgradeWnd->m_pUpgradeItem[1],0,sizeof(CItem));
	memset(&pItemUpgradeWnd->m_pUpgradeItem[2],0,sizeof(CItem));
	memset(&pItemUpgradeWnd->m_pUpgradeItem[3],0,sizeof(CItem));
}


// ===================
// ���׷��̵� �����Ͽ����� ���� ó��  
// ===================
void CmdUpgradeItemFailInsurance( char* pMsg, DWORD dwLen )
{
	DSTC_ITEMUPGRADE_FAIL_INSURANCE* pPacket	= (DSTC_ITEMUPGRADE_FAIL_INSURANCE*)pMsg;
	CItemUpgradeWnd*	pItemUpgradeWnd			= CItemUpgradeWnd::GetInstance();
	CItemUpgradeSubWnd* pItemUpgradeSubWnd		= CItemUpgradeSubWnd::GetInstance();

	pItemUpgradeWnd->m_bWaitingUpgrade			= ITEM_UPGRADE_FAIL_INSURANCE;
	pItemUpgradeSubWnd->m_byItemUpgradeProcess	= ITEM_UPGRADE_PROCESS_FAIL_INSURANCE;
	
	memcpy(&pItemUpgradeWnd->m_pUpgradeItem[0], &pPacket->cItem, sizeof(pPacket->cItem));
	memset(&pItemUpgradeWnd->m_pUpgradeItem[1], 0, sizeof(CItem));
	memset(&pItemUpgradeWnd->m_pUpgradeItem[2], 0, sizeof(CItem));
	memset(&pItemUpgradeWnd->m_pUpgradeItem[3], 0, sizeof(CItem));
	memset(&g_pMainPlayer->m_pInv_Small[pPacket->bDelete], 0, sizeof(CItem));
}


// ===================
// ���׷��̵� ����  
// ===================
void CmdUpgradeItemSuccess( char* pMsg, DWORD dwLen )
{
	DSTC_ITEMUPGRADE_SUCCESS* pPacket		= (DSTC_ITEMUPGRADE_SUCCESS*)pMsg;
	CItemUpgradeWnd* pItemUpgradeWnd		= CItemUpgradeWnd::GetInstance();
	CItemUpgradeSubWnd* pItemUpgradeSubWnd	= CItemUpgradeSubWnd::GetInstance(); 
	
	pItemUpgradeWnd->m_bWaitingUpgrade	= pPacket->bResult;
	pItemUpgradeSubWnd->m_byItemUpgradeProcess = ITEM_UPGRADE_PROCESS_SUCCESS;
	
	memcpy(&pItemUpgradeWnd->m_pUpgradeItem[0], &pPacket->cItem, sizeof(pPacket->cItem));
	memset(&pItemUpgradeWnd->m_pUpgradeItem[1], 0, sizeof(CItem));
	memset(&pItemUpgradeWnd->m_pUpgradeItem[2], 0, sizeof(CItem));
	memset(&pItemUpgradeWnd->m_pUpgradeItem[3], 0, sizeof(CItem));
}


// ===================
// ������ ���� ���� 
// ===================
void CmdMakingItemSuccess( char* pMsg, DWORD dwLen )
{
	DSTC_ITEMMAKING_SUCCESS* pPacket		= (DSTC_ITEMMAKING_SUCCESS*)pMsg;
	CItemMakingWnd* pItemMakingWnd			= CItemMakingWnd::GetInstance();
	CItemMakingSubWnd* pItemMakingSubWnd	= CItemMakingSubWnd::GetInstance(); 

	pItemMakingWnd->m_byProcess = ITEM_MAKING_PROCESS_RESULT_SUCCESS;
	pItemMakingSubWnd->m_byItemMakingProcess = ITEM_MAKING_PROCESS_SUCCESS;

	memcpy(&pItemMakingWnd->m_pMakingItem[0], &pPacket->cItem, sizeof(pPacket->cItem));
	memset(&pItemMakingWnd->m_pMakingItem[1], 0, sizeof(CItem));
	memset(&pItemMakingWnd->m_pMakingItem[2], 0, sizeof(CItem));
	memset(&pItemMakingWnd->m_pMakingItem[3], 0, sizeof(CItem));
}


// ===================
// ������ ���� ���� 
// ===================
void CmdMakingItemFail( char* pMsg, DWORD dwLen )
{
//	DSTC_ITEMMAKING_FAIL* pPacket			= (DSTC_ITEMMAKING_FAIL*)pMsg;
	CItemMakingWnd* pItemMakingWnd			= CItemMakingWnd::GetInstance();
	CItemMakingSubWnd* pItemMakingSubWnd	= CItemMakingSubWnd::GetInstance();

	pItemMakingWnd->m_byProcess = ITEM_MAKING_PROCESS_RESULT_FAIL;
	pItemMakingSubWnd->m_byItemMakingProcess = ITEM_MAKING_PROCESS_FAIL;

	memset(&pItemMakingWnd->m_pMakingItem[0], 0, sizeof(CItem));
	memset(&pItemMakingWnd->m_pMakingItem[1], 0, sizeof(CItem));
	memset(&pItemMakingWnd->m_pMakingItem[2], 0, sizeof(CItem));
	memset(&pItemMakingWnd->m_pMakingItem[3], 0, sizeof(CItem));
}