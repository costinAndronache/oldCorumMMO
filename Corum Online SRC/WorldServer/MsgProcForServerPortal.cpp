#include "stdafx.h"
#include "MsgProcForServerPortal.h"
#include "MessengerHash.h"
#include "GuildHash.h"
#include "PartyHash.h"
#include "ChinaBillingProc.h"
#include "WorldUserTable.h"
#include "OwnServer.h"
#include "DUNGEON_DATA_EX.h"
#include "DungeonTable.h"
#include "recvmsg.h"
#include "AcceptTable.h"
#include "WorldUser.h"
#include "ServerFunction.h"


//=============================================
// �̺�Ʈ ���� ��Ż�� �����൵ �Ǵ°�?
//=============================================
int ChkDungeonPortalCondition( DUNGEON_DATA_EX* pDungeon )
{
	switch ( pDungeon->m_bEventState )
	{
		case EVENT_DUNGEON_READY:			return 20;
		case EVENT_DUNGEON_OPEN_FULL:		return 19;
		case EVENT_DUNGEON_OPEN_TIME_UP:	return 18;
		case EVENT_DUNGEON_CLOSE:			return 20;
		case EVENT_DUNGEON_FIRST_CLOSE:		return 20;
	}

	return 0;
}

//=============================================
// ��Ż�� ���������� �� ������ ���� ������ ��û
//=============================================
void CmdRequestUserAcceptForPortal(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSWDS_REQUEST_USER_ACCEPT_FOR_PORTAL *pPacket = (DSWDS_REQUEST_USER_ACCEPT_FOR_PORTAL*)pMsg;

	BYTE bErrorCode = 0;
	
	DUNGEON_DATA_EX *pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	DUNGEON_DATA_EX *pDungeonFrom = g_pDungeonTable->GetDungeonInfo(pPacket->wFromDungeonID);

	if(pDungeon)
	{		
		// ������ �߿��� �̵� ����
		if(pDungeon->m_bSiege)	 
		{
			if( pDungeonFrom && pDungeonFrom->IsStadium() )
			{
				bErrorCode = 254;
			}
			else
			{
				bErrorCode = 8;				
			}
			goto lb_Fail;
		}
		else if(	IS_ABLE_SERVICE_TYPE(ST_DEVELOP) 
				||	IS_ABLE_NATION(NC_JAPAN) )//hwoarang
		{
			if(pDungeon->m_byEntererMinLevel > pPacket->dwUserLevel || pDungeon->m_byEntererMaxLevel < pPacket->dwUserLevel)
			{
				// �������� �ɷǳ�?
				bErrorCode = 9;		
				goto lb_Fail;
			}
		}
		else if (pDungeon->IsConquer() && pDungeon->m_dwOwnerIndex != pPacket->dwUserIndex )
		{
			// ����� ��ƶ�.
			if (pPacket->dwUserMoney >= pDungeon->GetEntrancePay())
			{
				pPacket->dwUserMoney -= pDungeon->GetEntrancePay();				
			}
			else
			{
				bErrorCode =  17;
				goto lb_Fail;
			}
		}

		// �̺�Ʈ �����̸�
		if(pDungeon->GetDungeonType() == DUNGEON_TYPE_EVENT)
		{
			bErrorCode = (BYTE)ChkDungeonPortalCondition( pDungeon );			
		}

		if(pDungeon->m_pServer && pDungeon->m_pServer->dwServerStatus == SERVER_STATUS_CONNECTED)	
		{			
			CWorldUser* pUser = g_pUserTable->GetUserInfo(pPacket->dwUserIndex);
			
			if(pUser)
				pUser->m_byPortal = 1;
			
			g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, pMsg, dwLength, FLAG_SEND_NOT_ENCRYPTION);

			return;
		}
		else
		{
			//���� ������ ���ӵǾ� ���� ���� 
			bErrorCode = 1;		
			goto lb_Fail;
		}		
	}
	else
	{
		// ID�� �ش��ϴ� �������� ���� 
		bErrorCode = 2;		
	}

lb_Fail:
	
	DSWDS_USER_ACCEPT_FAILED_FOR_PORTAL FailPacket;	
	FailPacket.dwUserIndex = pPacket->dwUserIndex;
	FailPacket.bErrorCode = bErrorCode;
	g_pNet->SendToServer(pServer->dwConnectionIndex, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}


//=========================================================
// ��Ż�� ������  ���������� ������ Address�� Port�� ������
//=========================================================
void CmdUserAcceptAllowedForPortal(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSWDS_USER_ACCEPT_ALLOWED_FOR_PORTAL* pPacket = (DSWDS_USER_ACCEPT_ALLOWED_FOR_PORTAL*)pMsg;

	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wFromDungeonID);

	if(pDungeon)
	{
		if(pDungeon->m_pServer && pDungeon->m_pServer->dwServerStatus == SERVER_STATUS_CONNECTED)
		{
			g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, pMsg, dwLength, FLAG_SEND_NOT_ENCRYPTION);
		}
		else
		{
			Log(LOG_IMPORTANT, "Dungeon Server is not connected at CmdUserAcceptAllowedForPortal() (DungeonID:%d)", pPacket->wFromDungeonID);
		}
	}
	else
	{
		Log(LOG_IMPORTANT, "Dungeon data is not found! at CmdUserAcceptAllowedForPortal() (DungeonID:%d)", pPacket->wFromDungeonID);
	}
}


//=================================================
// ��Ż�� ������������ ���� ��� ��� �߰� ����  
//=================================================
void CmdUserAcceptFailedForPortal(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSWDS_USER_ACCEPT_FAILED_FOR_PORTAL* pPacket = (DSWDS_USER_ACCEPT_FAILED_FOR_PORTAL*)pMsg;

	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wFromDungeonID);

	if(pDungeon)
	{
		// �ش� ���������� �޼��� ������ 
		if(pDungeon->m_pServer && pDungeon->m_pServer->dwServerStatus == SERVER_STATUS_CONNECTED)	
		{
			g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, pMsg, dwLength, FLAG_SEND_NOT_ENCRYPTION);
		}
		else
		{
			Log(LOG_IMPORTANT, "Dungeon Server is not connected at CmdUserAcceptFailedForPortal() (DungeonID:%d)", pPacket->wFromDungeonID);
		}
	}
	else
	{
		Log(LOG_IMPORTANT, "Dungeon data is not found! at CmdUserAcceptFailedForPortal() (DungeonID:%d)", pPacket->wFromDungeonID);
	}
} 
