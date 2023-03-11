#include "stdafx.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
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
#include "GameSystemNative.h"


void CmdRequestUserAcceptForPortal(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	DSWDS_REQUEST_USER_ACCEPT_FOR_PORTAL* pPacket = (DSWDS_REQUEST_USER_ACCEPT_FOR_PORTAL*)pMsg;

	ACCEPT_USER* pUser = (ACCEPT_USER*)0xffffffff;

	while(pUser)
	{
		pUser = g_pAcceptTable->GetAcceptInfo(pPacket->dwUserIndex);

		if(pUser)
		{
			g_pAcceptTable->Remove(pUser);
			Log(LOG_IMPORTANT, "Request UserIndex is already accepted! (CmdRequestUserAcceptForPortal Function!)");
		}
		else 
		{
			goto lb_Continue;	
		}
	}

lb_Continue:
	
	// ���� ��� ��ܿ� �߰� 
	pUser = g_pAcceptTable->AllocNewAccept(pPacket->wDungeonID, pPacket->dwUserIndex);
	pUser->bConnectToGameRoom = pPacket->bConnectionToGameRoom;

	if(!g_pAcceptTable->Add(pUser))
	{
		Log(LOG_IMPORTANT, "Adding User to AcceptTable is Failed! (CmdRequestUserAcceptForPortal Function!)");

		DSWDS_USER_ACCEPT_FAILED_FOR_PORTAL FailPacket;
		FailPacket.wFromDungeonID = pPacket->wFromDungeonID;
		FailPacket.dwUserIndex = pPacket->dwUserIndex;
		FailPacket.bErrorCode = 3;				//Accept Table�� Add ���� 
		g_pNet->SendToServer(dwConnectionIndex, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		return;
	}
	
	// �ٽ� ������������ ������ Address�� �Բ� �˸� 
	DSWDS_USER_ACCEPT_ALLOWED_FOR_PORTAL packet;	
	packet.wFromDungeonID = pPacket->wFromDungeonID;
	packet.dwUserIndex = pPacket->dwUserIndex;
#if defined TAIWAN_LOCALIZING
	// �븸�� ���ξ����Ƿ� ���������ϱ� ������ �ܺ� IP�� �������� IP�� ����.
	packet.dwConnectIP = inet_addr(g_pThis->GetIPForExtern());
#else
	packet.dwConnectIP = inet_addr(g_pThis->GetIPForUser());
#endif
	packet.wPort = g_pThis->GetPortForUser();

	// Append by seven / [12/9/2004]
	// GM ��� �����Ͽ� �����̵���
	// Ÿ���� ���� ��Ż�� ��ġ�� �𸦰�찡 �ִ�.
	// �̸� ���ؼ� �̵��� ���� ��ġ�� �˾Ƽ� �����ش�.
	if (255 != pPacket->bLayerIndex)
	{
		if (Portal_Type_GM == pPacket->bPortalType)
		{
			// �ٽø��ؼ� Ư�������� 0�� �������� �����̴�.
			CDungeon * pDungeonDest = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
			if (pDungeonDest )
			{
				CDungeonLayer * pLayer = pDungeonDest->GetDungeonLayer(pPacket->bLayerIndex);
				if (pLayer)
				{
					pLayer->GetStartTile(0, &packet.v2Pos);
				}
			}
		}
		else
		{
			// �Ϲ� / NPC ��Ż �̵��̴�.
			// ��Ż ������ ����� �� ������ ���̴�. ��ġ�κ��� �˰��ֱ� ������ 
			// �ƹ��͵� ���� �ʾƵ� �ȴ�. 
		}
	}
	else
	{
		// pUser�� ��ġ�� �����̴�.
		CUser * pUser = g_pUserHash->GetData(pPacket->dwTargetUserIndex);
		if (!pUser)
		{
			// ��� ������ ����. 
			// �̷� ���� ��ġ �ʰ����� ������ �ִ�.
			// bErrorCode�� �߰�����
			DSWDS_USER_ACCEPT_FAILED_FOR_PORTAL FailPacket;
			FailPacket.wFromDungeonID = pPacket->wFromDungeonID;
			FailPacket.dwUserIndex	  = pPacket->dwUserIndex;
			FailPacket.bErrorCode	  = 3;				//Accept Table�� Add ���� 
			g_pNet->SendToServer(dwConnectionIndex, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}

		packet.bLayerIndex = pUser->GetCurLayerIndex();

		packet.v2Pos.x = pUser->GetCurPosition()->x;
		packet.v2Pos.y = pUser->GetCurPosition()->y;
	}
	
	g_pNet->SendToServer(dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );	
}


void CmdUserAcceptAllowedForPortal(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	DSWDS_USER_ACCEPT_ALLOWED_FOR_PORTAL* pPacket = (DSWDS_USER_ACCEPT_ALLOWED_FOR_PORTAL*)pMsg;

	CUser* pUser = g_pUserHash->GetData( pPacket->dwUserIndex );

	if(pUser)
	{
		if(pUser->pBufForPortal)
		{			
			PORTAL_BUCKET* pPortal = (PORTAL_BUCKET*)(pUser->pBufForPortal);
			pPortal->dwConnectIP = pPacket->dwConnectIP;
			pPortal->wPort = pPacket->wPort;

			// ��Ż �̵��� ��ġ ���� 
			if (0 == pPortal->wPortalItemID &&
				0.f == pPortal->v2Pos.x && 0.f == pPortal->v2Pos.y)
			{ 
				// �Ϲ���Ż�� �ƴϴ�. 
				pPortal->v2Pos = pPacket->v2Pos;	// append by seven / [12/9/2004]
			}


			if (255 == pPortal->bLayerNo)
			{
				// ���߿� ���� �ޱ����ؼ� ���� 255�� ���صξ����� 
				// ����ε� ������ �Է����ش�.
				pPortal->bLayerNo = pPacket->bLayerIndex;				
			}

			pUser->ChangeGodMode(GODMODE_STATUS_MAPLOADING);

			// ���� ������ ������ �����Ŀ� ���������� ��Ż �̵��϶�� ��Ŷ�� ������. 
			UpdateUserDBThread(pUser);	
			// sung-han 2005-02-28 ��Ż���� ����Ʈ �ȳ����°� ����-----------------------------
			// ���� ��Ʈ(����)�� ó���ž� �ִ����� �ٸ� ��Ʈ�� ����Ű� ����
			CDungeon * pDungeonDest = g_pDungeonTable->GetDungeonInfo(pUser->GetCurDungeonID());
			pDungeonDest->RemoveUserForInsidePortal(pUser);
			// ------------------------------------------------------------------------------------

			Log( LOG_JUST_FILEWRITE, "UpdateUserDB Name : %s, Level : %u, Exp : %u", pUser->m_szName, pUser->GetLevel(), pUser->GetExp());
		}
		else
		{			
			SendPortalFailMessage(pUser, 0, 4, 0);						
			Log(LOG_IMPORTANT, "Portal info is not found at CmdUserAcceptAllowedForPortal");
		}
	}
}


void CmdUserAcceptFailedForPortal(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	DSWDS_USER_ACCEPT_FAILED_FOR_PORTAL* pPacket = (DSWDS_USER_ACCEPT_FAILED_FOR_PORTAL*)pMsg;

	CUser* pUser = g_pUserHash->GetData( pPacket->dwUserIndex );

	if(pUser)
	{	
		// ����� �Ǵ� PVP���϶� �����ڵ尡 254�� ���ƿ���, ���������� ������ǥ�� �̵����� �ش�.
		//
		if( pPacket->bErrorCode == 254 )
		{
			WORD	wRecentJoinDungeonID = pUser->m_wRecentJoinDungeonID;
			
			if( wRecentJoinDungeonID > 2000 && wRecentJoinDungeonID < 3000 )
			{
				pUser->m_bDestWorldSpot = (wRecentJoinDungeonID % 2) == 0;
                pUser->m_wRecentJoinDungeonID -= pUser->m_bDestWorldSpot;                
			}

			// Remark
			pUser->GoToWorld( pUser->m_bDestWorldSpot, GOTOWORLD_STATUS_GOTOWORLD_OUTSTATUS_GUILDWAR/*USER_TEMP_GOTOWORLD_OUTSTAUS_GUILDWAR*/ );
		}
		else
		{
			pUser->m_bPortalMoving = 0;

			if(pUser->pBufForPortal)
			{
				PORTAL_BUCKET* pPortal = (PORTAL_BUCKET*)(pUser->pBufForPortal);
				SendPortalFailMessage(pUser, pPortal->wToDungeonID, pPacket->bErrorCode, pPortal->wPortalItemID );
				
				LALFree(g_pPortalBufferPool, pUser->pBufForPortal);
				pUser->pBufForPortal = NULL;
			}
			else
			{
				SendPortalFailMessage(pUser, 0, 4, 0);
				Log(LOG_IMPORTANT, "Portal info is not found at CmdUserAcceptFailedForPortal");
			}
		}
	}
}

void CmdUserPortal(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)//: hwoarang 050129
{
	WSTDS_USER_PORTAL* pPacket = (WSTDS_USER_PORTAL*)pMsg;

	CUser * pPortalUser		= (CUser *)g_pUserHash->GetData(pPacket->dwUserIndex);
	CUser * pTargetToUser	= (CUser *)g_pUserHash->GetData(pPacket->dwTargetToUserIndex); 

	DSTC_USER_PORTAL pSendPacket;
	
	DUNGEON_MOVE_INFO &rSendDMI = pSendPacket.DMI;

	if (!pPortalUser)
	{
		return;
	}

	pPortalUser->m_ePortalType = Portal_Type_GM;	// GM �� ���� ��Ż�̴�.

	switch( pPacket->byGMCommand )
	{
	case GM_Command_Get_Recall_Someone:
		{
			rSendDMI = pPacket->DMI;	// �̵��� ���� ����

			NetSendToUser(pPortalUser->m_dwConnectionIndex, 
				(char*)&pSendPacket, pSendPacket.GetPacketSize(),
				FLAG_SEND_NOT_ENCRYPTION);
		}
		break;
	case GM_Command_Get_Move_Someone:
		{		
			rSendDMI = pPacket->DMI;	// �̵��� ���� ����

			if (pTargetToUser)
			{
				// ���� ��Ʈ�� ���� ��Ż�̴�
				rSendDMI.byLayerIndex = pTargetToUser->m_bInLastLayer;	// �� 
				rSendDMI.wX = (WORD)pTargetToUser->GetCurPosition()->x;				// ��ġ 
				rSendDMI.wY = (WORD)pTargetToUser->GetCurPosition()->y;
			}
			else
			{
//				rSendDMI.byLayerIndex = pPacket->byLayerIndex;	// �� 
				rSendDMI.wX = 0;									// ��ġ�� �𸥴�. 
				rSendDMI.wY = 0;
				pSendPacket.dwTargetUserIndex = pPacket->dwTargetToUserIndex;
			}

			NetSendToUser(pPortalUser->m_dwConnectionIndex, 
				(char*)&pSendPacket, pSendPacket.GetPacketSize(),
				FLAG_SEND_NOT_ENCRYPTION);
		}

		break;
	case GM_Command_Dungeon_Portal:
		{
			rSendDMI = pPacket->DMI;	// �̵��� ���� ����

//			rSendDMI.wDungeonID		= pPacket->wDungeonID;	// �̵��� ����
		
//			rSendDMI.byLayerIndex = pPacket->byLayerIndex;	// �� 
//			rSendDMI.wX = (WORD)pPacket->wX;	// ��ġ 
//			rSendDMI.wY = (WORD)pPacket->wY;

			NetSendToUser(pPortalUser->m_dwConnectionIndex, 
				(char*)&pSendPacket, pSendPacket.GetPacketSize(),
				FLAG_SEND_NOT_ENCRYPTION);
		}
		break;
	}
}
