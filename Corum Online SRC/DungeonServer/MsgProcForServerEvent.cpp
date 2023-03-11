#include "stdafx.h"
#include "MsgProcForServerEvent.h"
#include "DungeonTable.h"
#include "Dungeon.h"
#include "OwnServer.h"
#include "DungeonLayer.h"
#include "DUNGEON_DATA_EX.h"
#include "ServerFunction.h"
#include "AcceptTable.h"
#include "ItemMove.h"
#include "ItemNative.h"
#include "recvmsg.h"
#include "DBProc.h"
#include "RecordSet.h"


//------------------------------------------------------------------------------
// �̺�Ʈ ���� ��ȴ�.
//------------------------------------------------------------------------------
void CmdEventDungeonOpenInfo(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_EVENT_DUNGEON_OPEN_INFO* pPacket = (WSTDS_EVENT_DUNGEON_OPEN_INFO*)pMsg;	
	
	DSTC_EVENT_DUNGEON_OPEN_INFO SendPacket;
	
	memset(&SendPacket.szDungeonName, 0, MAX_DUNGEON_NAME_LENGTH);
	__lstrcpyn(SendPacket.szDungeonName, pPacket->szDungeonName, MAX_DUNGEON_NAME_LENGTH);

	SendPacket.byEventState		= pPacket->byEventState; 
	SendPacket.wDungeonID		= pPacket->wDungeonID;
	SendPacket.dwEventEndTick	= pPacket->dwEventEndTick; 
	
	g_pDungeonTable->BroadCast((char*)&SendPacket, SendPacket.GetPacketSize());
	
	return;
}


//------------------------------------------------------------------------------
// �̺�Ʈ ���� ������.
//------------------------------------------------------------------------------
void CmdEventDungeonCloseInfo(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_EVENT_DUNGEON_CLOSE_INFO* pPacket = (WSTDS_EVENT_DUNGEON_CLOSE_INFO*)pMsg;

	DSTC_EVENT_DUNGEON_CLOSE SendPacket;
	memset(&SendPacket.szDungeonName, 0, MAX_DUNGEON_NAME_LENGTH);
	__lstrcpyn(SendPacket.szDungeonName, pPacket->szDungeonName, MAX_DUNGEON_NAME_LENGTH);
	SendPacket.byEventState = pPacket->byEventState;
	SendPacket.wDungeonID	= pPacket->wDungeonID;
	g_pDungeonTable->BroadCast((char*)&SendPacket, SendPacket.GetPacketSize());
	
	CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if(NULL == pDungeon)	return;

	if(pDungeon->GetEventDungeonProperty())
	{
		if( EVENT_DUNGEON_SURVIVAL == pDungeon->GetEventDungeonProperty()->byDungeonType )
		{
			if( EVENT_DUNGEON_CLOSE == pPacket->byEventState )
			{
				pDungeon->AllUserPKMode(FALSE);
			}			
		}
	}

	return;
}


//------------------------------------------------------------------------------
//	1�� �̺�Ʈ ���� �Ǿ������� ó��...
//------------------------------------------------------------------------------
void CmdEventDungeonFirstTimeOut(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	WSTDS_EVENT_DUNGEON_FIRST_TIMEOUT* pPacket = (WSTDS_EVENT_DUNGEON_FIRST_TIMEOUT*)pMsg;
	
	DSTC_EVENT_DUNGEON_FIRST_TIMEOUT SendPacket;
	SendPacket.wDungeonID = pPacket->wDungeonID;
	memset(&SendPacket.szDungeonName, 0, MAX_DUNGEON_NAME_LENGTH);
	__lstrcpyn(SendPacket.szDungeonName, pPacket->szDungeonName, MAX_DUNGEON_NAME_LENGTH);
	
	CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if (NULL == pDungeon)
	{
		return;
	}
	
	SendPacket.bMonsterExpRatio = pDungeon->GetEventDungeonProperty()->byMonsterExpRatio;
	
	pDungeon->EventFirstTimeUpProcess();
	pDungeon->BroadCast((char*)&SendPacket,SendPacket.GetPacketSize());	
	pDungeon->AllUserPKMode();
}


//------------------------------------------------------------------------------
//	�̺�Ʈ ���� ���尴�� �̺�Ʈ ������ �ִ� ��� �����鿡�� �˷��ش�.
//------------------------------------------------------------------------------
void CmdEventDungeonJoinUserCount(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	WSTDS_EVENT_JOINUSER_COUNT* pPacket = (WSTDS_EVENT_JOINUSER_COUNT*)pMsg;

	DSTC_EVENT_DUNGEON_JOINUSER_COUNT SendPacket;
	SendPacket.wDungeonID		= pPacket->wDungeonID;
	SendPacket.wJoinUserCount	= pPacket->wJoinUserCount; 

	CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if (NULL == pDungeon)
	{
		return;
	}

	pDungeon->BroadCast((char*)&SendPacket,SendPacket.GetPacketSize());	
}


//------------------------------------------------------------------------------
//	�̺�Ʈ ���� �ݱ����� �����鿡�� �˸���.
//------------------------------------------------------------------------------
void CmdEventDungeonClose(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_EVENT_DUNGEON_CLOSE* pPacket = (WSTDS_EVENT_DUNGEON_CLOSE*)pMsg;

	DSTC_EVENT_DUNGEON_CLOSE_DISPLAY SendPacket;
	SendPacket.wDungeonID = pPacket->wDungeonID;

	CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if (NULL == pDungeon)
	{
		return;
	}

	pDungeon->BroadCast((char*)&SendPacket,SendPacket.GetPacketSize());	
}


//------------------------------------------------------------------------------
//	�̺�Ʈ ���� ������ �Ѵ�. 
//------------------------------------------------------------------------------
void CmdEventDungeonInit(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_EVENT_DUNGEON_INIT* pPacket = (WSTDS_EVENT_DUNGEON_INIT*)pMsg;

	CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if(NULL == pDungeon)	return;		

	if(FALSE == pPacket->bTimeUp)
	{
		// �̺�Ʈ ���� �������� �Ѹ� �ȵ�� ������ 
		pDungeon->EventClearProcess();
	}
	else
	{
		pDungeon->EventTimeUpProcess();
		pDungeon->EventClearProcess();
	}	
}