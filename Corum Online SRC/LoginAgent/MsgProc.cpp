#include "StdAfx.h"
#include "MsgProc.h"
#include "ThrowQuery.h"
#include "OwnServer.h"
#include "UserTable.h"
#include "WorldServerTable.h"
#include "ServerFunction.h"
#include "recvmsg.h"
#include "Version.h"
#include "PacketEncrypt.h"
#include "Filter.h"
#include "User.h"
#include "ChinaBillingProc.h"

#include "CPSSOLib.h"
#include "../CommonServer/CommonHeader.h"

extern IOleDBThread*	g_pDb;
extern DWORD			g_dwVersion;
extern MACHINENAME		g_MachineName;

void CmdLoginNormal(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	CTWS_LOGIN *			packet = (CTWS_LOGIN*)pMsg;						

	// �ñ״��� üũ.
	if(	packet->bySigniture[0] != '@' || packet->bySigniture[1] != 'S' || 
		packet->bySigniture[2] != 'A' || packet->bySigniture[3] != 'D' )
	{
		WSTC_LOGIN_FAIL	packet;
		packet.bResult = LOGIN_FAIL_INVALID_VERSION;		//������ ����, ����
		SendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
		return;
	}

	int nNationalCode = 0;	

	switch( MY_NATION_CODE )
	{
	case NC_JAPAN:
		{
			nNationalCode = National_Code_Japan;
		}break;
	case NC_TAIWAN:
		{
			nNationalCode = National_Code_Taiwan;
		}break;
	case NC_CHINA:
		{
			nNationalCode = National_Code_China;
		}break;
	case NC_KOREA:
	default:
		{
			nNationalCode = National_Code_Korea;
		}break;
	}

	if ( nNationalCode != packet->byNationCode )
	{ 
		// �ٸ� ���󿡼� �����ҷ��� ���.. �ϴ� ������ ���ٰ� ǥ�� ..
		WSTC_LOGIN_FAIL	packet;
		packet.bResult = LOGIN_FAIL_INVALID_VERSION;		//������ ���� 
		SendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
		return;
	}

	if(packet->dwVersion < g_dwVersion)
	{
		WSTC_LOGIN_FAIL	packet;
		packet.bResult = LOGIN_FAIL_INVALID_VERSION;		//������ ���� 
		SendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
		return;
	}	

	// �ִ� ���� ������ �ʰ� 
	if(g_pUserTable->GetCount() >= g_pThis->GetMaxUserAllowedNum())		
	{
		WSTC_LOGIN_FAIL	packet;
		packet.bResult = LOGIN_FAIL_USER_OVERFLOW;				//���� �ִ� �ο� �ʰ�...  
		SendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
		Log(LOG_NORMAL, "User Overflow ~ ~ ~ !!");
		return;
	}
	
	//���� Data Alloc
	CUser* pUser = g_pUserTable->AllocNewUser();
	
	if(!pUser)
	{
		Log(LOG_IMPORTANT, "User MemoryPool Overflow!");
		g_pNet->CompulsiveDisconnectUser(dwConnectionIndex);
		return;
	}
	
	pUser->SetConnectionIndex(dwConnectionIndex);

#ifdef __PACKET_ENCRYPTION
	// �ϴ� �������� �⺻Ű�� ����.
	pUser->SetBaseEncryptKey();

	// �������� �� �༮�� ���� Ű�� �Ҵ��Ѵ�.
	WSTC_ENCRYPTION_KEY Packet;
	RandomKeyGenerator(Packet.byServerKey);
	
	// Ŭ���̾�Ʈ Ű�� �����Ͽ� ���� ����� Ű�� �����Ѵ�.
	BYTE btEncryptionKey[ENCRYPT_KEY_LEN] = {0,};
	CalculateKey(packet->byClientKey, Packet.byServerKey, btEncryptionKey);

	// Ŭ���̾�Ʈ���� ��Ŷ ��ȣ Ű�� ����!
	SendToUser(pUser, (char*)&Packet, Packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

	pUser->SetEncryptKey(btEncryptionKey);	
#endif 	

	WORD wPort = 0;
	// �켱 �ӽ÷� User ����ü�� ID ���� 
	__lstrcpyn(pUser->m_szLoginID, packet->szID, MAX_ID_REAL_LENGTH);				
	__lstrcpyn(pUser->m_szLoginPassword, packet->szPassword, MAX_PASSWORD_REAL_LENGTH);
	g_pNet->SetUserInfo(pUser->GetConnectionIndex(), (void*)pUser);	
	g_pNet->GetUserAddress(pUser->GetConnectionIndex(), (LPSTR)pUser->GetIPString(), &wPort);

	ThrowLoginQuery(pUser, packet->szID, packet->szPassword);
}

void CmdLoginNetMarble(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	CTWS_NETMARBLE_LOGIN *	packet = (CTWS_NETMARBLE_LOGIN *)pMsg;
	
	// �ñ״��� üũ.
	if(	packet->bySigniture[0] != '@' || packet->bySigniture[1] != 'S' || 
		packet->bySigniture[2] != 'A' || packet->bySigniture[3] != 'D' )
	{
		WSTC_LOGIN_FAIL	packet;
		packet.bResult = LOGIN_FAIL_INVALID_VERSION;		//������ ����, ����
		SendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
		return;
	}

	int nNationalCode = 0;	

	switch( MY_NATION_CODE )
	{
	case NC_JAPAN:
		{
			nNationalCode = National_Code_Japan;
		}break;
	case NC_TAIWAN:
		{
			nNationalCode = National_Code_Taiwan;
		}break;
	case NC_CHINA:
		{
			nNationalCode = National_Code_China;
		}break;
	case NC_KOREA:
	default:
		{
			nNationalCode = National_Code_Korea;
		}break;
	}

	if ( nNationalCode != packet->byNationCode )
	{ 
		// �ٸ� ���󿡼� �����ҷ��� ���.. �ϴ� ������ ���ٰ� ǥ�� ..
		WSTC_LOGIN_FAIL	packet;
		packet.bResult = LOGIN_FAIL_INVALID_VERSION;		//������ ���� 
		SendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
		return;
	}

	if(packet->dwVersion < g_dwVersion)
	{
		WSTC_LOGIN_FAIL	packet;
		packet.bResult = LOGIN_FAIL_INVALID_VERSION;		//������ ���� 
		SendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
		return;
	}	

	// �ִ� ���� ������ �ʰ� 
	if(g_pUserTable->GetCount() >= g_pThis->GetMaxUserAllowedNum())		
	{
		WSTC_LOGIN_FAIL	packet;
		packet.bResult = LOGIN_FAIL_USER_OVERFLOW;				//���� �ִ� �ο� �ʰ�...  
		SendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
		Log(LOG_NORMAL, "User Overflow ~ ~ ~ !!");
		return;
	}
	
	//���� Data Alloc
	CUser* pUser = g_pUserTable->AllocNewUser();
	
	if(!pUser)
	{
		Log(LOG_IMPORTANT, "User MemoryPool Overflow!");
		g_pNet->CompulsiveDisconnectUser(dwConnectionIndex);
		return;
	}
	
	pUser->SetConnectionIndex(dwConnectionIndex);

#ifdef __PACKET_ENCRYPTION
	// �ϴ� �������� �⺻Ű�� ����.
	pUser->SetBaseEncryptKey();

	// �������� �� �༮�� ���� Ű�� �Ҵ��Ѵ�.
	WSTC_ENCRYPTION_KEY Packet;
	RandomKeyGenerator(Packet.byServerKey);
	
	// Ŭ���̾�Ʈ Ű�� �����Ͽ� ���� ����� Ű�� �����Ѵ�.
	BYTE btEncryptionKey[ENCRYPT_KEY_LEN] = {0,};
	CalculateKey(packet->byClientKey, Packet.byServerKey, btEncryptionKey);

	// Ŭ���̾�Ʈ���� ��Ŷ ��ȣ Ű�� ����!
	SendToUser(pUser, (char*)&Packet, Packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

	pUser->SetEncryptKey(btEncryptionKey);	
#endif 	

	if (strlen(packet->szID) != 0)
	{
		goto pass_login;
	}
	else
	{
		char szVal[1024] = {0,};
		if (TRUE == GetCPCookieValue(packet->szCPCookie,"UserID", szVal))
		{ 
			// ����
			//memset(packet->szID, 0, sizeof(packet->szID));
			__lstrcpyn(packet->szID, szVal, MAX_ID_REAL_LENGTH);
		}
		else
		{
			// ����
			Log(LOG_IMPORTANT, "GetCPCookieValue[UserID] failed");
			return;
		}

		// �н����� (����ť id�� ��´�.)
		if (TRUE == GetCPCookieValue(packet->szCPCookie,"UniID", szVal))
		{ 
			// ����
			//memset(packet->szPassword, 0, sizeof(packet->szPassword));
			__lstrcpyn(packet->szPassword, szVal, MAX_PASSWORD_REAL_LENGTH);
		}
		else
		{
			// ����	
			Log(LOG_IMPORTANT, "GetCPCookieValue[UniID] failed");
			return;
		}
		
		int iHex;
		char buf[1024] = {0,};
		char buf2[1024] = {0,};

		__lstrcpyn(buf, packet->szPassword+2, MAX_PASSWORD_REAL_LENGTH-2);
		sscanf(buf,"%d", &iHex);
		itoa(iHex, buf, 16);
		iHex = atoi(buf);
		wsprintf(buf2, "NM%010s", buf);
		__lstrcpyn(packet->szPassword, buf2, MAX_PASSWORD_REAL_LENGTH);
	}

pass_login:

	WORD wPort = 0;
	// �켱 �ӽ÷� User ����ü�� ID ���� 
	__lstrcpyn(pUser->m_szLoginID, packet->szID, MAX_ID_REAL_LENGTH);				
	__lstrcpyn(pUser->m_szLoginPassword, packet->szPassword, MAX_PASSWORD_REAL_LENGTH);
	g_pNet->SetUserInfo(pUser->GetConnectionIndex(), (void*)pUser);	
	g_pNet->GetUserAddress(pUser->GetConnectionIndex(), (LPSTR)pUser->GetIPString(), &wPort);

	ThrowLoginQuery(pUser, packet->szID, packet->szPassword);
}

void CmdLogin(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	if( IS_ABLE_SERVICE_TYPE( ST_NETMARBLE) )
	{
		CmdLoginNetMarble(dwConnectionIndex, pMsg,dwLength);
	}
	else
	{
		CmdLoginNormal(dwConnectionIndex, pMsg,dwLength);
	}
}

void CmdCheckMonitoring(CUser* pUser ,char* pMsg,DWORD dwLength)
{
	pMsg;dwLength;

	if (NULL == pUser)
	{
		Log(LOG_JUST_DISPLAY, "pUser is NULL at CmdCheckMonitoring function.. ");
		return;
	}	

	// ������ ����,
	char szIp[16]	= {0,};	
	WORD wPort		= 0;
	g_pNet->GetUserAddress(pUser->GetConnectionIndex(), szIp, &wPort);

	// �ش� �����ǰ� ��� �Ǿ� �ִ��� üũ �Ѵ�.,
	char szQuery[QUERY_STRING_SIZE]={0,};
	memset(szQuery, 0, QUERY_STRING_SIZE);
	wsprintf(szQuery, "Select * From MONITORING_LIST where ip = '%s'", szIp);
	g_pDb->THOpenRecord(szQuery, QUERY_TYPE_CHECK_MONITOR, (void*)pUser->m_dwConnectionIndex, 1, (BYTE)TOTAL_DB);	
}


void CmdCMKickUser(CUser* pUser, char* pMsg,DWORD dwLength)
{
	dwLength;

	if( pUser && TRUE == pUser->m_bIsMonitor )
	{
		CTAS_KICK_USER* pRecievePacket = (CTAS_KICK_USER*)pMsg;
		CUser* pPrevUser = g_pUserTable->GetUserInfo(pRecievePacket->dwPropID);

		if( NULL == pPrevUser )
		{
			return;
		}

		if(pPrevUser->m_dwChar_Index)
		{
			// �ٸ� ���峪 ���� ������ ������ �ִٸ� ��� ������� ������ 
			ASTWS_KICK_USER	KickPacket;
			KickPacket.dwCharIndex = pPrevUser->m_dwChar_Index;
			KickPacket.btKickCode = 0;
			g_pServerTable->BroadCastWorldServer((char*)&KickPacket, KickPacket.GetPacketSize());
		}
		
		// Agent�� ���ӵǾ� �ִ� ���� 
		if(pPrevUser->m_dwCurWorldServerPos == SERVER_POS_AGENT)	
		{
			DWORD dwConnectionIndex = pPrevUser->GetConnectionIndex();
			OnDisconnectUser(dwConnectionIndex);
			g_pNet->SetUserInfo(dwConnectionIndex,NULL);
			g_pNet->CompulsiveDisconnectUser(dwConnectionIndex);			
		}
		else
		{
			ThrowLogoutQuery(pPrevUser);

			Log(LOG_JUST_DISPLAY, "Monitoring Tool User Kick...(ID:%s, Name:%s)",pPrevUser->m_szLoginID, pPrevUser->GetSelectedCharacter()->szName);

			// ���� ���̺��� �� ���� ���� Free�� �� 
			g_pUserTable->Remove(pPrevUser); 
		}
	}
}

void CmdCMToolNotice(CUser* pUser ,char* pMsg,DWORD dwLength)
{
	dwLength;

	if( pUser && TRUE == pUser->m_bIsMonitor )
	{
		CTAS_CMTOOL_NOTICE* pRecievePacket = (CTAS_CMTOOL_NOTICE*) pMsg;
		ASTWS_CMTOOL_NOTICE NoticePacket;

		__lstrcpyn(NoticePacket.szMsg, pRecievePacket->szMsg, 1024);	
		NoticePacket.wMsgLen = pRecievePacket->wMsgLen;

		g_pServerTable->BroadCastWorldServer((char*)&NoticePacket,NoticePacket.GetPacketSize());
	}
}

void CmdIsAliveUser(CUser* pUser ,char* pMsg,DWORD dwLength)
{
	dwLength;

	if( pUser && TRUE == pUser->m_bIsMonitor )
	{
		CTAS_IS_ALIVE_USER* pRecievePacket = (CTAS_IS_ALIVE_USER*) pMsg;
		
		char szQuery[QUERY_STRING_SIZE] = {0,};

		wsprintf(szQuery, "Select * From LoginTable Where user_id = '%s' AND kind = '%d'", pRecievePacket->szID, pRecievePacket->Kind);

		// ���� ó���� RecieveDB ����.
		g_pDb->THOpenRecord(szQuery, QUERY_TYPE_IS_ALIVE_USER, (void*)pUser->m_dwConnectionIndex, 1, (BYTE)MEMBER_DB);
		
	}
}

void CmdRequestServerAlive(CUser* pUser ,char* pMsg,DWORD dwLength)
{
	dwLength; pMsg;

	if (pUser && TRUE == pUser->m_bIsMonitor)
	{
		// �پ� �ִ� ���� �����鿡�� ������ �����޶� ��û.
		ASTWS_REQUEST_SERVER_ALIVE RequestServerAlivePacket;
		g_pServerTable->BroadCastWorldServer((char*)&RequestServerAlivePacket, RequestServerAlivePacket.GetPacketSize());		

		// ������
		// �������� ���� ������ ������ ����.
		ASTC_SERVER_ALIVE_INFO		SendPacket;

		SendPacket.wServerType			= g_pThis->GetServerType();
		SendPacket.dwServerID			= g_pThis->GetServerID();
		SendPacket.wConnectedUserCount	= g_pUserTable->GetCount();
		SendPacket.dwExpGainRate		= 0;
		SendPacket.dwMagicItemDropRate	= 0;

		g_pUserTable->BroadCastToMonitoringTool((char*)&SendPacket, SendPacket.GetPacketSize());
	}
}


// pUser�� Alloc ���� ���¿��� �� �ѹ� Login �޼����� ���� ��� 
void CmdLoginOverlapped(CUser* pUser ,char* pMsg,DWORD dwLength)
{
	dwLength;

	CTWS_LOGIN*	pPacket = (CTWS_LOGIN*)pMsg;

	WSTC_LOGIN_FAIL	packet;
	packet.bResult = LOGIN_FAIL_NOW_TRY_TO_LOGIN;
	SendToUser( pUser, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);

	Log(LOG_NORMAL, "Login Message Overlapped!! (pUser->m_szLoginID:%s, pPacket->szLoginID:%s)", pUser->m_szLoginID, pPacket->szID);
}

void CmdCharacterSelect(CUser* pUser ,char* pMsg,DWORD dwLength)
{
	dwLength;

	CTWS_CHARACTER_SELECT* pPacket = (CTWS_CHARACTER_SELECT*)pMsg;

	if(pPacket->bCharacterIndex > 3)
	{
		Log(LOG_IMPORTANT, "Invalid CharacterIndex Entered! (Login ID:%s)", pUser->m_szLoginID);
		ASTC_CHARACTER_SELECT_FAIL FailPacket;
		FailPacket.bErrorCode = 1;
		SendToUser(pUser, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		return;
	}

	if(pUser->m_dwStatus == USER_STATUS_CHARACTER_SELECTED)	
	{
		Log(LOG_NORMAL, "Character is already selected!");
		return;	
	}
	
	if(pUser->m_EnumChar[ pPacket->bCharacterIndex ].dwRecentWorldmapID == 0)
	{
		pUser->m_EnumChar[ pPacket->bCharacterIndex ].dwRecentWorldmapID = 1;
	}	

	// Ȥ�� �� ĳ���Ͱ� �ٸ� ������ ����������츦 ����Ͽ� ��� ������� ������~
	ASTWS_KICK_USER	KickPacket;
	KickPacket.dwCharIndex = pUser->m_EnumChar[ pPacket->bCharacterIndex ].dwCharacterIndex;
	g_pServerTable->BroadCastWorldServer((char*)&KickPacket, KickPacket.GetPacketSize());
	
	if( pUser->m_EnumChar[ pPacket->bCharacterIndex ].dwRecentWorldmapID )
	{
		LPWorldServerData pServer = g_pServerTable->GetServer( 1 );
		
		if(!pServer)
		{
			Log(LOG_IMPORTANT, "WorldServer Data is Not Found or Invalid WorldmapID Entered! (WorldMapID=%d)"
				, pUser->m_EnumChar[ pPacket->bCharacterIndex ].dwRecentWorldmapID );
			
			ASTC_CHARACTER_SELECT_FAIL FailPacket;
			FailPacket.bErrorCode = 2;
			SendToUser(pUser, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			return;
		}

		// �ش� ���弭���� ���ӵǾ� ���� ���� 
		if(!pServer->dwConnectionIndex)		
		{
			Log(LOG_IMPORTANT, "WorldServer is Not Attached! (WorldMapID=%d)"
				, pUser->m_EnumChar[ pPacket->bCharacterIndex ].dwRecentWorldmapID );

			ASTC_CHARACTER_SELECT_FAIL FailPacket;
			FailPacket.bErrorCode = 3;
			SendToUser(pUser, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			return;
		}
		
		pUser->m_bSelectedChrIndex	= pPacket->bCharacterIndex;	// ������ �迭 Index ����
		pUser->m_dwChar_Index		= pUser->m_EnumChar[ pPacket->bCharacterIndex ].dwCharacterIndex;
		
		if( IS_ABLE_SERVICE_TYPE(ST_CHINA_BILLING) )//�߱����� Test : 050104 hwoarang
		{
			if(pUser->m_EnumChar[ pPacket->bCharacterIndex ].dwLevel<10)
			{
				pUser->m_sChinaBillingData.dwChinaBillingPoint		= 0;
				pUser->m_sChinaBillingData.eChinaBillingUserData	= china_billing_info_limit;
			}
			else
			{
				SendChinaBillingLogin(pUser->GetPropID(), pUser->GetUserID(), pUser->GetIP());
			}
		}

		// World ���� ���� ��� ����Ʈ�� ĳ���� �߰� ��Ű�� 
		ASTWS_ADD_NEW_USER packet;
		packet.dwPropID		= pUser->GetPropID();
		packet.dwChar_Index = pUser->m_dwChar_Index = pUser->m_EnumChar[ pPacket->bCharacterIndex ].dwCharacterIndex;	//DB���� Character Index��ȣ 
		packet.dwSerialCode = pUser->m_dwSerialCode = g_pThis->GetNewSerial( pPacket->bCharacterIndex );
		packet.bBillingType = pUser->m_bBillingType;
		packet.BillingEday  = pUser->m_BillingEday;
		packet.dwRemainTime = pUser->m_dwRemainTime;

		if( IS_ABLE_NATION(NC_TAIWAN) )
		{
			if( IS_ABLE_SERVICE_TYPE(ST_BILLING) )//�Ϲݺ��� :050104 hwoarang
			{
				packet.BillingInfo = pUser->m_BillingInfo;	//���� ���� 
			}
		} // if( IS_ABLE_NATION(NC_TAIWAN) )

		g_pNet->SendToServer(pServer->dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

		// ĳ���͸� �����ϰ� ���弭���� Accept����� ����ϰ� �ִ»���
		pUser->m_dwStatus = USER_STATUS_CHARACTER_SELECTED; 
	}
	else
	{
		ASTC_CHARACTER_SELECT_FAIL FailPacket;
		FailPacket.bErrorCode = 4;
		SendToUser(pUser, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

		Log(LOG_IMPORTANT, "Character WorldMapID is not found! (Name:%s)"
			, pUser->m_EnumChar[ pPacket->bCharacterIndex ].szName );
	}
}

void CmdCreateNewCharacter(CUser* pUser ,char* pMsg,DWORD dwLength)
{
	dwLength;

	CTWS_CREATE_NEW_CHARACTER* pPacket = (CTWS_CREATE_NEW_CHARACTER*)pMsg;

	if(pPacket->bChrNum > 3)
	{
		Log(LOG_IMPORTANT, "Invalid ChrNum Entered! at CmdCreateNewCharacter() bChrNum=%d", pPacket->bChrNum);
		return;
	}
	
	if(lstrlen(pUser->m_EnumChar[ pPacket->bChrNum ].szName))
	{
		Log(LOG_IMPORTANT, "Character Place Overlapped! at CmdCreateNewCharacter() bChrNum=%d", pPacket->bChrNum);
		return;
	}

	switch( MY_NATION_CODE )
	{
	case NC_CHINA:
		{
			if(FilterWordsIncluded(pPacket->szCharacterName)) 
			{
				Log(LOG_IMPORTANT, "Prohibited Character Name! at CmdCreateNewCharacter() Requested Name: %s", pPacket->szCharacterName);

				WSTC_CREATE_CHARACTER_FAIL FailPacket;
				FailPacket.nResultValue = -3;

				SendToUser(pUser, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				return;
			} // if(FilterWordsIncluded(pPacket->szCharacterName))
		}break;
	}

	// �켱 �ӽ÷� ������ѳ��� 
	pUser->m_bSelectedChrIndex = pPacket->bChrNum;
	pUser->m_bClass = pPacket->bClass;
	pUser->m_EnumChar[ pPacket->bChrNum].dwRecentWorldmapID = 1;
	lstrcpyn(pUser->m_EnumChar[ pPacket->bChrNum ].szName, pPacket->szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH+1);
	pUser->m_bTryCreateCharSlot = pPacket->bChrNum;

	ThrowInsertCharQuery(pUser, pPacket->szCharacterName,(BYTE)(pPacket->bChrNum + 1),(WORD)(pPacket->wHead + 1000), pPacket->bClass, pPacket->wStartMapID);
}

void CmdDeleteCharacter(CUser* pUser ,char* pMsg,DWORD dwLength)
{
	dwLength;

	CTWS_DELETE_CHARACTER* pPacket = (CTWS_DELETE_CHARACTER*)pMsg;
	
	char szJumin[8] = {0,};	
	lstrcpyn(szJumin, pPacket->szJumin, 7+1);	szJumin[7] = '\0';

	if(IS_ABLE_NATION(NC_KOREA))//�ֹε�� ��ȣ�� �´��� üũ  :050104 hwoarang
	{	
		char szQuery[ QUERY_STRING_SIZE ]={0,};
		wsprintf(szQuery, "CZP_JUMIN_CHECK '%s', '%s', '%s'", pUser->m_szLoginID, pPacket->szCharacterName, szJumin);
		g_pDb->THOpenRecord(szQuery, QUERY_TYPE_JUMIN_CHECK, (void*)pUser->m_dwConnectionIndex, 1, MEMBER_DB);
	} // if(IS_ABLE_NATION(NC_KOREA))
	else
	{
		// ���� ��¥ üũ //
		if(	pUser->m_EnumChar[pPacket->bChrNum].sCreateTime.year==0 || 
			pUser->m_EnumChar[pPacket->bChrNum].sCreateTime.month==0 || 
			pUser->m_EnumChar[pPacket->bChrNum].sCreateTime.day==0)
		{
			ThrowDeleteCharQuery(pUser, pPacket->szCharacterName);
		}
		else
		{		
			CTime cCreateTime(pUser->m_EnumChar[pPacket->bChrNum].sCreateTime.year, 
				pUser->m_EnumChar[pPacket->bChrNum].sCreateTime.month, 
				pUser->m_EnumChar[pPacket->bChrNum].sCreateTime.day, 
				pUser->m_EnumChar[pPacket->bChrNum].sCreateTime.hour, 
				pUser->m_EnumChar[pPacket->bChrNum].sCreateTime.minute, 
				pUser->m_EnumChar[pPacket->bChrNum].sCreateTime.second);
			
			CTime cCurrentTime = CTime::GetCurrentTime();

			CTimeSpan ts = cCurrentTime - cCreateTime;

			if(ts.GetTotalHours()>=24)
			{
				if(pUser->m_EnumChar[pPacket->bChrNum].dwGuildId!=0)
				{
					// WorldServer Message //
					ASTWS_GUILD_CHARACTER_DELETE pSendServerPacket;
					memset(pSendServerPacket.szCharacterName, 0, sizeof(pSendServerPacket.szCharacterName));
					memcpy(pSendServerPacket.szCharacterName, pUser->m_EnumChar[pPacket->bChrNum].szName, MAX_CHARACTER_NAME_REAL_LENGTH);
					pSendServerPacket.dwGuildId	= pUser->m_EnumChar[pPacket->bChrNum].dwGuildId;
					pSendServerPacket.byRank	= pUser->m_EnumChar[pPacket->bChrNum].byRank;
					pSendServerPacket.dwIndex	= pUser->m_EnumChar[pPacket->bChrNum].dwCharacterIndex;		
					g_pServerTable->SendToWorldServer((char*)&pSendServerPacket, pSendServerPacket.GetPacketSize(), 1);
				}
				ThrowDeleteCharQuery(pUser, pPacket->szCharacterName);
			}
			else
			{
				WSTC_DELETE_CHARACTER_RESULT pPacket;
				pPacket.bSuccess		= FALSE;
				pPacket.nResultValue	= 10;
				SendToUser(pUser, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
			}
		}
	}
}

void CmdCheckGM(CUser* pUser ,char* pMsg,DWORD dwLength)
{
	dwLength; pMsg; pUser;
/*
	CTWS_CHECK_GM* pPacket = (CTWS_CHECK_GM*)pMsg;

	char szQuery[ 0xff ];
	wsprintf(szQuery, "Select [Ip] from GM_LIST where [Name]= '%s'", pUser->m_szCharacterName);
	g_pDb->THOpenRecord(szQuery, QUERY_TYPE_GM_CHECK, pUser, 1, TOTAL_DB);	
*/
}


