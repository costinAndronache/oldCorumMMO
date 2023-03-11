#include "stdafx.h"
#include "ChinaBillingProc.h"
#include "ServerTable.h"
#include "WorldUser.h"
#include "WorldUserTable.h"
#include "recvmsg.h"
#include "WorldUser.h"
#include "DUNGEON_DATA_EX.h"
#include "DungeonTable.h"
//#include "LinkedlistEx.h"
//#include "Log.h"


// KickList�̴�... �� cpp������ ����.
CVoidList g_ChinaKickUserList;


void __stdcall OnChinaKickOut(DWORD dwEventIndex)
{
	// �� �� �༮�� 5�ʸ��� ȣ��Ǹ�, 
	// Kick ���� ���� ����Ʈ�� �˻��ؼ�, 
	// Kick ���Ѿ� �� �༮�� ���δ�!!!
	// �������� ���, �̹� Ŭ���̾�Ʈ�� ����Ǿ���� �ϳ�,
	// �������� ���� �������� Ŭ���̾�Ʈ�� �����Ѵ�.

	CHINA_KICK_USER_DATA* pKickData = NULL;
	DWORD dwCurrentTime = timeGetTime();

	for(;;)
	{
		pKickData = (CHINA_KICK_USER_DATA*)g_ChinaKickUserList.GetHead();
		
		if(!pKickData) break;

		// Kick ������ �� �� 5�ʰ� ���� ��츸!!!!
		if(dwCurrentTime - pKickData->dwKickDecistionTime < 5*1000)
			break;
		
		ChinaBillingKickOutProc(pKickData->dwCharacterIndex);

		g_ChinaKickUserList.RemoveHead();	
	}
}

void AddChinaBillingKickList(DWORD dwCharacterIndex)
{
	CHINA_KICK_USER_DATA* pKickData = new CHINA_KICK_USER_DATA;

	pKickData->dwCharacterIndex = dwCharacterIndex;
	pKickData->dwKickDecistionTime = 5*1000;

	g_ChinaKickUserList.AddTail(pKickData);
}

void ChinaBillingKickOutProc(DWORD dwCharacterIndex)
{
	// �̹� ������ �������� �ʴ� ������� ����.
	CWorldUser *pUser = g_pUserTable->GetUserInfo(dwCharacterIndex);
	if(!pUser) return;

	// ������ ���� ������ ���´�.
	Log(LOG_NORMAL, "World User Kicked[Billing Expired]! (Name:%s)", pUser->m_szCharacterName);

	pUser->m_bKicked = 1;	
	DWORD dwConnectionIndex = pUser->m_dwConnectionIndex;

	OnDisconnectUser(dwConnectionIndex);
	g_pNet->SetUserInfo(dwConnectionIndex, NULL);
	g_pNet->CompulsiveDisconnectUser(dwConnectionIndex);

	//Ȯ���ϰ� Kick �ع����� ���� ������ KICK�϶�� �˷��ش�.
	WSTDS_KICK_USER	packet;
	packet.dwCharIndex = dwCharacterIndex;

	g_pServerTable->BroadCast((char*)&packet, packet.GetPacketSize());
	
	// Login Server Kick //
	WSTAS_WORLD_USER_DISCONNECT pdisconnect;
	pdisconnect.dwPropID = pUser->m_dwPropID;
	g_pNet->SendToServer(g_pThis->GetAgentConnectionIndex(), (char*)&pdisconnect, pdisconnect.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
}

void AcmdChinaBillingInfo(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	if(! IS_ABLE_SERVICE_TYPE( ST_CHINA_BILLING ) )
	{
		return;
	}
	
	ASTWS_CHINA_BILLING_INFO* pPacket = (ASTWS_CHINA_BILLING_INFO*)pMsg;

	CWorldUser* pUser = g_pUserTable->GetUserInfo(pPacket->dwUserIndex);

	if(pUser)
	{
		pUser->m_sChinaBillingInfo.dwPoint	= pPacket->dwPoint;
		pUser->m_sChinaBillingInfo.eType	= (ECHINA_BILLING_CHECK)pPacket->byType;	
		
		//////////////////////////////////////////////////////////////////////////
		// Log 
		Log(LOG_IMPORTANT, "Recv(China Billing Info) : proc(%d), Type(%d)", 
			pUser->m_sChinaBillingInfo.eType, 
			pUser->m_sChinaBillingInfo.dwPoint);
		//--

		if(Billing_Info_Procedure_Type_Normal == pPacket->GetProcedureType())
		{
			if(pUser->m_sChinaBillingInfo.eType!=enum_level_approval)
			{
				DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo( pUser->m_dwDungeonID );	

				if(!pDungeon)
					return;

				// Goto Dungeon -> World //
				WSTDS_CB_LEVELCHK pLevelPacket;
				pLevelPacket.dwUserIndex = pPacket->dwUserIndex;
				g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pLevelPacket, pLevelPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);	

				// User Message Send //
				WSTC_CB_MESSAGE pCBMessagePacket;
				pCBMessagePacket.byMessageType = 1;
				g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pCBMessagePacket, pCBMessagePacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			}			
		}
		else if (Billing_Info_Procedure_Type_Level_Limit == pPacket->GetProcedureType())
		{
			// �޽��� �����ؼ� �ٲ�� �־ byBillingType 2�� �߰��� 
			// User Level UP and Over 10 level
			DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pUser->m_dwDungeonID);	

			if(!pDungeon)
				return;

			// Goto Dungeon -> World //
			WSTDS_CB_LEVELCHK pLevelPacket;
			pLevelPacket.dwUserIndex = pPacket->dwUserIndex;
			g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pLevelPacket, pLevelPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);	

			// User Message Send //
			WSTC_CB_MESSAGE pCBMessagePacket;
			pCBMessagePacket.byMessageType = 1;
			g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pCBMessagePacket, pCBMessagePacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);			
		}
	}
}