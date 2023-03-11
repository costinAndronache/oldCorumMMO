#include "stdafx.h"
#include "MsgProc.h"
#include "GuildHash.h"
#include "PartyHash.h"
#include "GlobalDefine.h"
#include "MessengerHash.h"
#include "EventDungeonManager.h"
#include "AcceptTable.h"
#include "WorldUserTable.h"
#include "DungeonTable.h"
#include "recvmsg.h"
#include "Map.h"
#include "NTOS.h"
#include "SchoolGuildDungeon.h"
#include "ChatLog.h"
#include "PartyMatcher.h"
#include "Party.h"
#include "MsgProcForServerParty.h"

void CmdWorldLogin(DWORD dwConnectionIndex ,char* pMsg,DWORD dwLength)
{
	//WS-03
	CTWS_WORLD_LOGIN *pPacket = (CTWS_WORLD_LOGIN*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	ACCEPT_USER* pAccept = g_pAcceptTable->GetAcceptInfo(pPacket->dwChar_Index);

	if(pAccept)
	{
		if(pPacket->dwSerialCode == pAccept->dwSerialCode)
		{			
			pAccept->dwConnection	= dwConnectionIndex;
			pAccept->dwPropID		= pPacket->dwPropID;
			pAccept->bEventFlag		= pPacket->bEventFlag;

			char szQuery[0xff] = {0, };
			wsprintf(szQuery, "CZP_WORLD_USER_CHK %u", pPacket->dwChar_Index);
			g_pDb->THOpenRecord(szQuery, QUERY_TYPE_WORLDCHK, (void*)pAccept, 4, (BYTE)GAME_DB);
		}
		else
		{
			//��ϵ� �ø����ȣ �ƴ� 
			WSTC_WORLD_LOGIN_FAIL FailPacket;
			FailPacket.bErrorCode = 1;
			g_pNet->SendToUser(dwConnectionIndex, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);			
			Log(LOG_IMPORTANT, "Invalid SerialCode Entered!");
		}		
	}
	else
	{
		//Accept ����Ʈ�� ���� ������ �α����Ѱ�� 
		WSTC_WORLD_LOGIN_FAIL	FailPacket;
		FailPacket.bErrorCode = 2;
		g_pNet->SendToUser(dwConnectionIndex, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		Log(LOG_IMPORTANT, "Fail to search user in acceptlist!");
	}
}


void CmdRequestDungeonInfo(CWorldUser* pUser ,char* pMsg,DWORD dwLength)
{
	CTWS_REQUEST_DUNGEON_INFO *pPacket = (CTWS_REQUEST_DUNGEON_INFO*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);

	if(!pDungeon)	return;
	if (!pUser->m_pListRefreshFromDungeon)
	{
		char szTemp[0xff];wsprintf(szTemp, "<yakolla> %x,%d CmdRequestDungeonInfo:: m_pListRefreshFromDungeon is NULL", pUser, pUser->m_dwConnectionIndex);
		Log(LOG_JUST_FILEWRITE, szTemp);
		return;
	}

	WSTC_REPLY_DUNGEON_INFO packet;		//memset(&packet, 0, sizeof(WSTC_REPLY_DUNGEON_INFO));
	packet.wDungeonID			= pPacket->wDungeonID;
	packet.wGroupID				= pDungeon->m_wGroupID;
	packet.wAreaLevel_h			= pDungeon->m_wAreaLevel_h;
	packet.wInitLevel			= pDungeon->m_wInitLevel;
	packet.wUpgradeLevel		= pDungeon->m_wUpgradeLevel;
	packet.dwOwnerGuildNum		= pDungeon->m_dwOwnerGuildNum;
	packet.wLayerCount			= pDungeon->m_wLayerCount;
	packet.bSiege				= pDungeon->m_bSiege;
	packet.bOperationType		= pDungeon->m_bOperationType;	
	packet.wProduction			= pDungeon->m_wProduction;
	packet.byProductionCount	= pDungeon->m_byProductionCount;
	packet.dwOwnerIndex			= pDungeon->m_dwOwnerIndex;
	packet.byEntererMaxLevel	= pDungeon->m_byEntererMaxLevel;
	packet.byEntererMinLevel	= pDungeon->m_byEntererMinLevel;
	memcpy(&packet.cMagicArrayItem, &pDungeon->m_cMagicFieldArrayItem, sizeof(CItem));
	memcpy(&packet.m_cGuardianItem, &pDungeon->m_cGuardianItem, sizeof(CItem));
	packet.byDefenseCount		= pDungeon->m_byDefenseCount;
	packet.byProperty			= pDungeon->m_byProperty;
	packet.dwAccEntrance		= pDungeon->m_dwAccEntrance;
	packet.dwEntrance			= pDungeon->m_dwEntrance;
	packet.dwAccExp				= pDungeon->m_dwAccExp;
	memset(packet.szOwner, 0, sizeof(packet.szOwner));
	memset(packet.szManager, 0, sizeof(packet.szManager));
	memset(packet.szDungeonName, 0, sizeof(packet.szDungeonName));
	memset(packet.szMemo, 0, sizeof(packet.szMemo));

	__lstrcpyn(packet.szManager, pDungeon->m_szManager, MAX_CHARACTER_NAME_REAL_LENGTH);	
	__lstrcpyn(packet.szOwner, pDungeon->m_szOwner, MAX_CHARACTER_NAME_REAL_LENGTH);
	__lstrcpyn(packet.szDungeonName, pDungeon->m_szDungeonName, MAX_DUNGEON_NAME_LENGTH);
	
	// �Ϻ����� ���� ������ ������ ����̸� �ֱ�!!
	if( IS_ABLE_NATION( NC_JAPAN) )
	{
		memset(packet.szSchoolName, 0, sizeof(packet.szSchoolName));
		LP_SCHOOL_GUILD_DUNGEON pGuildDungeon = g_pSchoolGuildDungeonTableHash->GetData(pPacket->wDungeonID);
		if(pGuildDungeon != NULL)
		{
			__lstrcpyn(packet.szSchoolName, pGuildDungeon->szSchoolName, MAX_CHARACTER_NAME_LENGTH);
		}
	}

	packet.bMemoLength = (BYTE)strlen(pDungeon->m_szMemo);
	memcpy(packet.szMemo, pDungeon->m_szMemo, packet.bMemoLength);

	packet.dwRemainSiegeStartTick = (pDungeon->m_dwRemainSiegeStartTick - g_dwCurTick);	//���������� ���� �ð� 
	packet.dwRemainSiegeEndTick = (pDungeon->m_dwRemainSiegeEndTick-g_dwCurTick);
	packet.dwRemainLevelStartTick = (pDungeon->m_dwRemainLevelStartTick-g_dwCurTick);
	g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
	
	// ���� ���� ���� �Ǿ����� ���Ű��� ���� ��������Ʈ.
	pDungeon->m_pListSendToUser->AddHead(pUser);
	pUser->m_pListRefreshFromDungeon->AddHead(pDungeon);
}


//====================================================================
// ������ Join�� �õ� �Ѵ�.
//====================================================================
void CmdTryToJoinDungeon(CWorldUser* pUser ,char* pMsg,DWORD dwLength)
{
	CTWS_TRY_TO_JOIN_DUNGEON	*pPacket = (CTWS_TRY_TO_JOIN_DUNGEON*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	if(pUser->m_dwCurServerPos != POS_SERVER_WORLD)
	{
		Log(LOG_NORMAL, "User Current Position is not World!(Name:%s)", pUser->m_szCharacterName);
		//return;		//��Ȳ�� ���캸�� �ּ��� Ǯ��!
	}

	if( ! IS_ABLE_SERVICE_TYPE(ST_DEVELOP) )//hwoarang
	{
#ifdef WORLD_MOVE_CHK
		BOOL bChk = FALSE;
		
		if(pUser->m_pEquip[EQUIP_TYPE_RIDE].GetID()/ITEM_DISTRIBUTE==ITEM_RIDE_INDEX)
		{
			bChk = TRUE;
		}
		else
		{		
			if(pUser->m_dwPrevDungeonId!=pPacket->wDungeonID)
			{
				LP_DUNGEON_NODE lpJoinDungeon	= (LP_DUNGEON_NODE)g_pDungeonListHash->GetData(pPacket->wDungeonID);
				LP_DUNGEON_NODE lpPrevDungeon	= (LP_DUNGEON_NODE)g_pDungeonListHash->GetData(pUser->m_dwPrevDungeonId);
				
				if(lpJoinDungeon && lpPrevDungeon)
				{
					if(lpJoinDungeon->byType==lpPrevDungeon->byType)
						bChk = TRUE;
				}
				else
				{
					if(!lpJoinDungeon)
						Log(LOG_NORMAL, "Not DungeonListHash : %u", pPacket->wDungeonID);
					if(!lpPrevDungeon)
						Log(LOG_NORMAL, "Not DungeonListHash : %u", pUser->m_dwPrevDungeonId);
				}
			}
			else
			{
				bChk = TRUE;
			}
		}

		if(!bChk)
		{
			WSTC_DUNGEON_JOIN_FAILED packet;
			packet.wDungeonID = pPacket->wDungeonID;
			packet.bFailCode   = DUNGOEN_JOIN_FAIL_IMPOSSIBILITY_AREA;
			g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			return;
	}
#endif
	}

	BYTE bDestWorldSpot = 0;

	// ��δ����� Ȧ����ȣ ������ ����ϸ� �ȴ�.
	if (pPacket->wDungeonID > 2000 && pPacket->wDungeonID < 3000) 
	{
		bDestWorldSpot = pPacket->wDungeonID%2==0;
	}
	
	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo( WORD(pPacket->wDungeonID-bDestWorldSpot) );	
	if(!pDungeon)	
	{
		Log(LOG_IMPORTANT, "CmdTryToJoinDungeon::Unknown Dungeon ID Received! (%d)", pPacket->wDungeonID);
		
		WSTC_DUNGEON_JOIN_FAILED packet;
		packet.bFailCode   = DUNGEON_JOIN_FAIL_UNKNOWN_DUNGEOIN_ID;
		packet.wDungeonID = pPacket->wDungeonID;
		
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		return;
	}
		
	// ���� �����Ϳ� �������� ������ ��ũ ���� ���� ���� 
	if(!pDungeon->m_pServer)	
	{
		WSTC_DUNGEON_JOIN_FAILED	packet;
		packet.bFailCode = DUNGEON_JOIN_FAIL_SERVER_INFO_IS_NOT_FOUND;
		packet.wDungeonID = pPacket->wDungeonID;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		return;
	}

	if(	IS_ABLE_SERVICE_TYPE(ST_DEVELOP) 
	||	IS_ABLE_NATION(NC_JAPAN) )//hwoarang
	{
	if (pDungeon->m_byEntererMinLevel > pUser->m_dwLevel || pDungeon->m_byEntererMaxLevel < pUser->m_dwLevel)
	{
		// �������� �ɷǳ�?
		WSTC_DUNGEON_JOIN_FAILED	packet;
		packet.bFailCode = DUNGEON_JOIN_FAIL_NOT_MINMAX_LEVEL;
		packet.wDungeonID = pPacket->wDungeonID;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		Log(LOG_IMPORTANT, "Invalid Level, DungeonID : %d, Name : %s", pDungeon->m_dwID, pUser->m_szCharacterName);
		return;
	}

	}

	if (pDungeon->IsConquer())
	{
		if (pUser->m_dwUserIndex != pDungeon->m_dwOwnerIndex)
		{
			if (pUser->m_dwMoney < pDungeon->GetEntrancePay())
			{
				// ����� �����ϴ�.
				WSTC_DUNGEON_JOIN_FAILED	packet;
				packet.bFailCode	= DUNGEON_JOIN_FAIL_NOT_ENOUGH_ENTER_KARZ;
				packet.wDungeonID	= pPacket->wDungeonID;
				g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				Log(LOG_IMPORTANT, "Sufficient Enterance Fee, DungeonID : %d, Name : %s", pDungeon->m_dwID, pUser->m_szCharacterName);
				return;
			}
		}	
	}
	
	// ���⼭ �ش� ������ ���� �ִ� ������ �Ǵ°��� üũ 
	if(pDungeon->m_pServer->dwServerStatus == SERVER_STATUS_CONNECTED)	
	{
		// �ش� ������������ �� ������ ������ ����϶�� �޼����� ������. 
		WSTDS_ADD_USER_ACCEPT_ALLOWED packet;
		packet.wDungeonID = pPacket->wDungeonID;		
		packet.dwUserIndex = pUser->m_dwUserIndex;
			
		if (pUser->m_bConnectGameRoom)
			packet.bType = ACCEPT_ALLOWED_TYPE_GAMEROOM;
		else
			packet.bType = ACCEPT_ALLOWED_TYPE_NORMAL;
		
		packet.bUserType	= pUser->m_bUserType;		
		pUser->m_dwStatus	= USER_STATUS_CHARACTER_SELECTED;	

		g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);				
	}
	else	 
	{
		// ������������ ���� ������ ����
		WSTC_DUNGEON_JOIN_FAILED packet;
		packet.wDungeonID	= pPacket->wDungeonID;
		packet.bFailCode	= DUNGEON_JOIN_FAIL_SERVER_IS_NOT_CONNECTED;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}	
}

void CmdCharacterInfoSearch(CWorldUser* pUser, char* pMsg,DWORD dwLength)
{		
	CTWS_CHARACTER_SEARCH* pPacket = (CTWS_CHARACTER_SEARCH*)pMsg;	
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}
	
	char szQuery[0xff]={0,};
	wsprintf(szQuery, "Select Char_Index, Name From CharacterInfo where Name = '%s'", pPacket->szName);
	g_pDb->THOpenRecord(szQuery, QUERY_TYPE_CHARACTERINFO, (void*)pUser->m_dwConnectionIndex, 1, (BYTE)GAME_DB);
}

void CmdHelpInfo(CWorldUser* pUser, char* pMsg,DWORD dwLength)
{
	WSTC_HELP_INFO* pPacket = (WSTC_HELP_INFO*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}
	
	pUser->m_byHelp[0] = pPacket->bHelpInfo[0];
	pUser->m_byHelp[1] = pPacket->bHelpInfo[1];
	pUser->m_byHelp[2] = pPacket->bHelpInfo[2];
}

void CmdCommandMsg(CWorldUser* pUser ,char* pMsg,DWORD dwLength)
{
	CTWS_COMMAND_MSG* pPacket = (CTWS_COMMAND_MSG*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	pUser->m_bCommand = pPacket->bCommand;	

	WSTC_COMMAND_MSG pSendPacket;
	pSendPacket.bCommand = pUser->m_bCommand;
	g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}

void CmdSkillKeyRt(CWorldUser* pUser ,char* pMsg,DWORD dwLength)
{
	CTWS_SKILL_KEY* pPacket = (CTWS_SKILL_KEY*)pMsg;	
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	memcpy(pUser->m_bySkillKey, pPacket->bSkillKey, 16);
	memcpy(pUser->m_byPosKey, pPacket->bPosKey, 16);
}

void CmdCheckGM(CWorldUser* pUser ,char* pMsg,DWORD dwLength)
{
	CTWS_CHECK_GM* pPacket = (CTWS_CHECK_GM*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	char szQuery[ 0xff ]={0,};
	memset(szQuery, 0, 0xff);
	
	wsprintf(szQuery, "Select [Ip] from GM_LIST where [Name] = '%s'", pUser->m_szCharacterName);	
	g_pDb->THOpenRecord(szQuery, QUERY_TYPE_GM_CHECK, (void*)pUser->m_dwConnectionIndex, 32, (BYTE)TOTAL_DB);		
}

void CmdCMKickUser(CWorldUser* pUser ,char* pMsg,DWORD dwLength)
{
	pUser;

	CTWS_KICK_USER* pPacket = (CTWS_KICK_USER*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	CWorldUser *pKUser = g_pUserTable->GetUserInfo(pPacket->dwCharIndex);
	
	if(pKUser)
	{
		Log(LOG_NORMAL, "Monitoring Tool World User Kicked! (Name:%s)", pKUser->m_szCharacterName);
		pKUser->m_bKicked = 1;	
		DWORD dwConnectionIndex = pKUser->m_dwConnectionIndex;
		OnDisconnectUser(dwConnectionIndex);
		g_pNet->SetUserInfo(dwConnectionIndex, NULL);
		g_pNet->CompulsiveDisconnectUser(dwConnectionIndex);
	}
	
	// Ȯ���ϰ� Kick �ع�����. 
	WSTDS_KICK_USER	packet;
	packet.dwCharIndex = pPacket->dwCharIndex;
	g_pServerTable->BroadCast((char*)&packet, packet.GetPacketSize());
}


void CmdCheckMonitoring(CWorldUser* pUser,char* pMsg,DWORD dwLength)
{
	CTWS_CHECK_MONITORING* pPacket = (CTWS_CHECK_MONITORING*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	// ����͸������� ������ �����̴�.
	if(pUser)
		pUser->m_bIsMonitor = TRUE;
}


// �۾� ��ȣ.
void CmdCommand(CWorldUser* pUser ,char* pMsg, DWORD dwLength)
{
	WSTC_WS_CHAT* pPacket = (WSTC_WS_CHAT*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	// ��ɾ� �˻� //
	switch(pPacket->cScript.enCommand)
	{
	case COMMAND_WHISPER:
		{
			CWorldUser* pUserDst = g_pUserTable->GetUserInfoByName(pPacket->cScript.szId);

			if(!pUserDst)
			{								
				// ������ ������.
				//char szQuery[ 0xff ];				
				char szQuery[ 1024 ]={0,};				
				memset(pUser->m_szSearchName, 0, sizeof(pUser->m_szSearchName));
				__lstrcpyn(pUser->m_szSearchName, pPacket->cScript.szId, MAX_CHARACTER_NAME_REAL_LENGTH);
				wsprintf(szQuery, "CZP_STORE_MESSAGE '%s', '%s', '%s'", pPacket->cScript.szId, pUser->m_szCharacterName, pPacket->cScript.szText);
				g_pDb->THOpenRecord(szQuery, QUERY_TYPE_MESSAGE, (void*)pUser->m_dwConnectionIndex, 1, (BYTE)GAME_DB);
				//Log(LOG_NORMAL, szQuery);
			}
			else
			{	
				if(pUserDst->m_bCommand==TRUE)
				{
					// ������ ������.
					WSTC_CHAT_WHISPER	pSendPacket;
					WSTC_DSCHAT_WHISPER pSendDSPacket;

					memset(pSendPacket.szId, 0, sizeof(pSendPacket.szId));
					memset(pSendPacket.szMessage, 0, sizeof(pSendPacket.szMessage));
					
					if(pUserDst->m_dwCurServerPos==POS_SERVER_WORLD)
					{
						pSendPacket.nSuccess	= 1;
						__lstrcpyn(pSendPacket.szId, pUser->m_szCharacterName, MAX_ID_LENGTH);
						__lstrcpyn(pSendPacket.szMessage, pPacket->cScript.szText, MAX_INPUT_BUFFER_SIZE);// text�� ���� �޽��� �������.
						g_pNet->SendToUser(pUserDst->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
					}
					else
					{
						pSendDSPacket.nSuccess	= 1;
						__lstrcpyn(pSendDSPacket.szId, pUser->m_szCharacterName, MAX_ID_LENGTH);
						__lstrcpyn(pSendDSPacket.szMessage, pPacket->cScript.szText, MAX_INPUT_BUFFER_SIZE);// text�� ���� �޽��� �������.
						g_pNet->SendToUser(pUserDst->m_dwConnectionIndex, (char*)&pSendDSPacket, pSendDSPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
					}												
					pSendPacket.nSuccess	= 2;
					__lstrcpyn(pSendPacket.szId, pPacket->cScript.szId, MAX_ID_LENGTH);
					__lstrcpyn(pSendPacket.szMessage, pPacket->cScript.szMessage, MAX_INPUT_BUFFER_SIZE);
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
				}
				else
				{
					WSTC_CHAT_COMMAND pSendPacket;
					pSendPacket.nSuccess	= 0;
					__lstrcpyn(pSendPacket.szId, pPacket->cScript.szId, MAX_ID_LENGTH);
					__lstrcpyn(pSendPacket.szMessage, pPacket->cScript.szMessage, MAX_INPUT_BUFFER_SIZE);
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
				}
			}
			break;
		}
	case COMMAND_MESSAGE:
		{
			CWorldUser* pUserDst = g_pUserTable->GetUserInfoByName(pPacket->cScript.szId);

			if(!pUserDst)
			{
				// ������ ������.
				//char szQuery[ 0xff ];				
				char szQuery[ 1024 ]={0,};				
				memset(pUser->m_szSearchName, 0, sizeof(pUser->m_szSearchName));
				__lstrcpyn(pUser->m_szSearchName, pPacket->cScript.szId, MAX_CHARACTER_NAME_REAL_LENGTH);
				wsprintf(szQuery, "CZP_STORE_MESSAGE '%s', '%s', '%s'", pPacket->cScript.szId, pUser->m_szCharacterName, pPacket->cScript.szMessage);
				g_pDb->THOpenRecord(szQuery, QUERY_TYPE_MESSAGE, (void*)pUser->m_dwConnectionIndex, 1, (BYTE)GAME_DB);
				//Log(LOG_NORMAL, szQuery);
			}
			else
			{	
				// ������ ������.
				if(pUserDst->m_bCommand==TRUE)
				{
					WSTC_CHAT_COMMAND pSendPacket;
					
					pSendPacket.nSuccess = 1;					
					memset(pSendPacket.szId, 0, sizeof(pSendPacket.szId));
					memset(pSendPacket.szMessage, 0, sizeof(pSendPacket.szMessage));

					__lstrcpyn(pSendPacket.szId, pUser->m_szSearchName, MAX_ID_LENGTH);
					__lstrcpyn(pSendPacket.szMessage, pPacket->cScript.szMessage, MAX_INPUT_BUFFER_SIZE);
					g_pNet->SendToUser(pUserDst->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
				
					pSendPacket.nSuccess = 2;
					memset(pSendPacket.szId, 0, sizeof(pSendPacket.szId));
					memset(pSendPacket.szMessage, 0, sizeof(pSendPacket.szMessage));

					__lstrcpyn(pSendPacket.szId, pPacket->cScript.szId, MAX_ID_LENGTH);
					__lstrcpyn(pSendPacket.szMessage, pPacket->cScript.szMessage, MAX_INPUT_BUFFER_SIZE);
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
				}
				else
				{
					WSTC_CHAT_COMMAND pSendPacket;
					pSendPacket.nSuccess = 0;
					memset(pSendPacket.szId, 0, sizeof(pSendPacket.szId));
					memset(pSendPacket.szMessage, 0, sizeof(pSendPacket.szMessage));

					__lstrcpyn(pSendPacket.szId, pPacket->cScript.szId, MAX_ID_LENGTH);
					__lstrcpyn(pSendPacket.szMessage, pPacket->cScript.szMessage, MAX_INPUT_BUFFER_SIZE);
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
				}				
			}
			break;
		}
		break;
	}
}

void DGCmdCommand(CWorldUser* pUser ,char* pMsg, DWORD dwLength)
{	
	WSTC_DS_CHAT* pPacket = (WSTC_DS_CHAT*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	CWorldUser* pUserDst = g_pUserTable->GetUserInfoByName(pPacket->cScript.szId);

	// ��ɾ� �˻� //
	switch(pPacket->cScript.enCommand)
	{
	case COMMAND_WHISPER:
		if(!pUserDst)
		{						
			// ������ ������.
			//char szQuery[ 0xff ];				
			char szQuery[ 1024 ]={0,};				
			memset(pUser->m_szSearchName, 0, sizeof(pUser->m_szSearchName));
			__lstrcpyn(pUser->m_szSearchName, pPacket->cScript.szId, MAX_CHARACTER_NAME_REAL_LENGTH);
			wsprintf(szQuery, "CZP_STORE_MESSAGE '%s', '%s', '%s'", pPacket->cScript.szId, pUser->m_szCharacterName, pPacket->cScript.szText);
			g_pDb->THOpenRecord(szQuery, QUERY_TYPE_MESSAGE2, (void*)pUser->m_dwConnectionIndex, 1, (BYTE)GAME_DB);
			//Log(LOG_NORMAL, szQuery);
		}
		else
		{
			if(pUserDst->m_bCommand==TRUE)
			{
				WSTC_DSCHAT_WHISPER pSendPacket;
				WSTC_CHAT_WHISPER	pSendPacketWS;

				memset(pSendPacketWS.szId, 0, sizeof(pSendPacketWS.szId));
				memset(pSendPacketWS.szMessage, 0, sizeof(pSendPacketWS.szMessage));			

				// ������ ������ //
				if(pUserDst->m_dwCurServerPos==POS_SERVER_WORLD)
				{
					pSendPacketWS.nSuccess	= 1;
					__lstrcpyn(pSendPacketWS.szId, pUser->m_szCharacterName, MAX_ID_LENGTH);
					__lstrcpyn(pSendPacketWS.szMessage, pPacket->cScript.szText, MAX_INPUT_BUFFER_SIZE);
					g_pNet->SendToUser(pUserDst->m_dwConnectionIndex, (char*)&pSendPacketWS, pSendPacketWS.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);				
				}
				else
				{
					pSendPacket.nSuccess	= 1;
					__lstrcpyn(pSendPacket.szId, pUser->m_szCharacterName, MAX_ID_LENGTH);
					__lstrcpyn(pSendPacket.szMessage, pPacket->cScript.szText, MAX_INPUT_BUFFER_SIZE);
					g_pNet->SendToUser(pUserDst->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);								
				}									
				pSendPacket.nSuccess	= 2;
				__lstrcpyn(pSendPacket.szId, pPacket->cScript.szId, MAX_ID_LENGTH);
				__lstrcpyn(pSendPacket.szMessage, pPacket->cScript.szMessage, MAX_INPUT_BUFFER_SIZE);
				g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);

				if( IS_ABLE_NATION(NC_TAIWAN) )//hwoarang
				{	
					if (pUser->IsGMUser() || 
					pUserDst->IsGMUser() ||
					!__strncmp(pUser->m_szCharacterName, "GM", lstrlen("GM")) ||
					!__strncmp(pUserDst->m_szCharacterName, "GM", lstrlen("GM")) )
					{
						// GM�̰ų� ������ GM���� �޽����� ���´�.
						SetChatLogInWorld(Chat_Type_WHISPER, pUser,pUserDst, pPacket->cScript.szMessage, strlen(pPacket->cScript.szMessage));
					}
				}
			}
			else
			{
				WSTC_CHAT_COMMAND pSendPacket;
				pSendPacket.nSuccess	= 0;
				memset(pSendPacket.szId, 0, sizeof(pSendPacket.szId));
				memset(pSendPacket.szMessage, 0, sizeof(pSendPacket.szMessage));
				__lstrcpyn(pSendPacket.szId, pPacket->cScript.szId, MAX_ID_LENGTH);
				__lstrcpyn(pSendPacket.szMessage, pPacket->cScript.szMessage, MAX_INPUT_BUFFER_SIZE);
				g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
			}
		}
		break;
		
	case COMMAND_MESSAGE:		
		if(!pUserDst)
		{
			// ������ ������.
			//char szQuery[ 0xff ];				
			char szQuery[ 1024 ]={0,};				
			memset(pUser->m_szSearchName, 0, sizeof(pUser->m_szSearchName));
			__lstrcpyn(pUser->m_szSearchName, pPacket->cScript.szId, MAX_CHARACTER_NAME_LENGTH);
			wsprintf(szQuery, "CZP_STORE_MESSAGE '%s', '%s', '%s'", pPacket->cScript.szId, pUser->m_szCharacterName, pPacket->cScript.szMessage);
			g_pDb->THOpenRecord(szQuery, QUERY_TYPE_MESSAGE2, (void*)pUser->m_dwConnectionIndex, 1, (BYTE)GAME_DB);
			//Log(LOG_NORMAL, szQuery);
		}
		else
		{	
			if(pUserDst->m_bCommand==TRUE)
			{			
				// ������ ������.
				CTWS_DSCHAT_COMMAND pSendPacket;
				pSendPacket.nSuccess = 1;				
				
				__lstrcpyn(pSendPacket.szId, pUser->m_szSearchName, MAX_ID_LENGTH);
				__lstrcpyn(pSendPacket.szMessage, pPacket->cScript.szMessage, MAX_INPUT_BUFFER_SIZE);
				g_pNet->SendToUser(pUserDst->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
				
				pSendPacket.nSuccess	= 2;
				memset(pSendPacket.szId, 0, sizeof(pSendPacket.szId));
				memset(pSendPacket.szMessage, 0, sizeof(pSendPacket.szMessage));

				__lstrcpyn(pSendPacket.szId, pPacket->cScript.szId, MAX_ID_LENGTH);
				__lstrcpyn(pSendPacket.szMessage, pPacket->cScript.szMessage, MAX_INPUT_BUFFER_SIZE);
				g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
			}
			else
			{
				WSTC_CHAT_COMMAND pSendPacket;
				
				pSendPacket.nSuccess = 0;
				memset(pSendPacket.szId, 0, sizeof(pSendPacket.szId));
				memset(pSendPacket.szMessage, 0, sizeof(pSendPacket.szMessage));

				__lstrcpyn(pSendPacket.szId, pPacket->cScript.szId, MAX_ID_LENGTH);
				__lstrcpyn(pSendPacket.szMessage, pPacket->cScript.szMessage, MAX_INPUT_BUFFER_SIZE);
				g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
			}
		}
	}
}

void CmdMessageRQ(CWorldUser* pUser ,char* pMsg, DWORD dwLength)
{	
	CTWS_CHAT_MESSAGERQ* pPacket = (CTWS_CHAT_MESSAGERQ*)pMsg;	
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}
	
	char szQuery[0xff]={0,};

	pPacket->szId[ MAX_ID_LENGTH - 1 ] = '\0';
	memset(szQuery, 0, sizeof(szQuery));
	wsprintf(szQuery, "SELECT [NameFrom], [SendTime], [Message] FROM MessageTable where NameTo = '%s'", pPacket->szId);
	g_pDb->THOpenRecord(szQuery, QUERY_TYPE_MESSAGERQ, (void*)pUser->m_dwConnectionIndex, 10, (BYTE)GAME_DB);
	//Log(LOG_NORMAL, szQuery);
}

void CmdChatUser(CWorldUser* pUser ,char* pMsg, DWORD dwLength)
{
	WSTC_CHAT_USER*		pPacket  = (WSTC_CHAT_USER*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	WSTC_DGCHAT_USER	pPacketDst;
	pPacketDst.nIndex = pPacket->nIndex;
	memset(pPacketDst.szDstId, 0, sizeof(pPacketDst.szDstId));
	memset(pPacketDst.szId, 0, sizeof(pPacketDst.szId));
	memset(pPacketDst.szMessage, 0, sizeof(pPacketDst.szMessage));

	__lstrcpyn(pPacketDst.szDstId, pPacket->szDstId, MAX_ID_LENGTH);
	__lstrcpyn(pPacketDst.szId, pPacket->szId,MAX_ID_LENGTH);
	__lstrcpyn(pPacketDst.szMessage, pPacket->szMessage, MAX_INPUT_BUFFER_SIZE);

	if(pPacket->nIndex==2)
	{
		LP_GUILD_TABLE lpGuildTable = g_pGuildTableHash->GetData(pUser->m_dwGuildId);

		CWorldUser * pExistGuildUser = NULL;	

		if(lpGuildTable)
		{
			ListNode<CWorldUser>* lpGuildUserNode = lpGuildTable->pGuildUserHash->GetHead();		
								
			while(lpGuildUserNode)
			{
				CWorldUser* pGuildUser = lpGuildUserNode->pData;
				
				if(!pGuildUser)
				{
					lpGuildUserNode = lpGuildUserNode->pNext;
					continue;
				}
				else
				{
					pExistGuildUser = pGuildUser;	// �α׸� ���ؼ� ��� ���� �ϳ��� �޾Ƶд�. ������ �߿��ϰԵȴ�.!!
				}

				pPacketDst.bSerType	= (BYTE)pGuildUser->m_dwCurServerPos;
				g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pPacketDst, pPacketDst.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

				lpGuildUserNode = lpGuildUserNode->pNext;
			}

			if( IS_ABLE_NATION(NC_TAIWAN) )//hwoarang
			{
				if (pExistGuildUser)
				{
					if (pUser->IsGMUser() || 
						pExistGuildUser->IsGMUser() ||
						!__strncmp(pUser->m_szCharacterName, "GM", lstrlen("GM")) ||
						!__strncmp(pExistGuildUser->m_szCharacterName, "GM", lstrlen("GM")) )
					{
						SetChatLogInWorld(Chat_Type_GUILD, pUser, pExistGuildUser, pPacket->szMessage, pPacket->byMessageLen);
					}
				}
			}
		}
	}
	else if(pPacket->nIndex==3)
	{
		LP_PARTY_TABLE lpPartyTable = g_pPartyTableHash->GetData(pUser->m_dwPartyId);
		
		if(!lpPartyTable)
			return;

		POSITION_ pos = lpPartyTable->pPartyUserList->GetHeadPosition();			

		CWorldUser * pExistUserRev = NULL;
		while(pos)
		{
			CWorldUser* pUserRev = (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(pos);			

			if(!pUserRev)
			{
				return;
			}
			else
			{
				pExistUserRev = (pUserRev == pUser)? pExistUserRev : pUserRev;	// �α׸� ���ؼ� ��Ƽ���� �ϳ��� �޾Ƶд�.
			}

			pPacketDst.bSerType	= (BYTE)pUserRev->m_dwCurServerPos;
			g_pNet->SendToUser(pUserRev->m_dwConnectionIndex, (char*)&pPacketDst, pPacketDst.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}

		if (pExistUserRev)
		{
			if( IS_ABLE_NATION(NC_TAIWAN) )//hwoarang
			{
				if (pUser->IsGMUser() || 
				pExistUserRev->IsGMUser() ||
				!__strncmp(pUser->m_szCharacterName, "GM", lstrlen("GM")) ||
				!__strncmp(pExistUserRev->m_szCharacterName, "GM", lstrlen("GM")) )
				{
					SetChatLogInWorld(Chat_Type_PARTY, pUser, pExistUserRev, pPacket->szMessage, pPacket->byMessageLen);
				}
			}
		}
	}
}

void CmdRequestOwnDungeonInfo(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{
	CTWS_REQUEST_OWN_DUNGEON_INFO* pPacket = (CTWS_REQUEST_OWN_DUNGEON_INFO*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	POSITION_ pos = g_pDungeonTable->m_pDungeonList->GetHeadPosition();
	while(pos)
	{
		DUNGEON_DATA_EX* pDungeon = (DUNGEON_DATA_EX*)g_pDungeonTable->m_pDungeonList->GetNext(pos);
		
		if (pDungeon->m_dwOwnerIndex == pUser->m_dwUserIndex)
		{
			BOOL	bSend = TRUE;
			POSITION_ poss = pUser->m_pListRefreshFromDungeon->GetHeadPosition();
			while(poss)
			{
				DUNGEON_DATA_EX* pTempDungeon = (DUNGEON_DATA_EX*)pUser->m_pListRefreshFromDungeon->GetNext(poss);
				if (pTempDungeon->m_dwID == pDungeon->m_dwID)
				{
					bSend = FALSE;
					break;
				}
			}

			if (bSend)
			{
				CTWS_REQUEST_DUNGEON_INFO packet;
				packet.wDungeonID = (WORD)pDungeon->m_dwID;
				CmdRequestDungeonInfo(pUser, (char*)&packet, packet.GetPacketSize());
			}	
		}	
	}
}

void CmdDSChatUser(CWorldUser* pUser ,char* pMsg, DWORD dwLength)
{
	CTWS_DGCHAT_USER* pPacket = (CTWS_DGCHAT_USER*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	WSTC_DGCHAT_USER	pPacketDst;
	pPacketDst.nIndex = pPacket->nIndex;	
	memset(pPacketDst.szDstId, 0, sizeof(pPacketDst.szDstId));
	memset(pPacketDst.szId, 0, sizeof(pPacketDst.szId));
	memset(pPacketDst.szMessage, 0, sizeof(pPacketDst.szMessage));

	__lstrcpyn(pPacketDst.szDstId, pPacket->szDstId, MAX_ID_LENGTH);
	__lstrcpyn(pPacketDst.szId, pPacket->szId, MAX_ID_LENGTH);
	__lstrcpyn(pPacketDst.szMessage, pPacket->szMessage, MAX_INPUT_BUFFER_SIZE);

	if(pPacket->nIndex==2)
	{
		LP_GUILD_TABLE lpGuildTable = g_pGuildTableHash->GetData(pUser->m_dwGuildId);

		if(lpGuildTable)
		{
			ListNode<CWorldUser>* lpGuildUserNode = lpGuildTable->pGuildUserHash->GetHead();		
									
			while(lpGuildUserNode)
			{
				CWorldUser* pGuildUser = lpGuildUserNode->pData;
				
				if(!pGuildUser)
				{
					lpGuildUserNode = lpGuildUserNode->pNext;
					continue;
				}
				
				pPacketDst.bSerType	= (BYTE)pGuildUser->m_dwCurServerPos;
				g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pPacketDst, pPacketDst.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

				lpGuildUserNode = lpGuildUserNode->pNext;
			}
		}		
	}
	else if(pPacket->nIndex==3)
	{
		LP_PARTY_TABLE lpPartyTable = g_pPartyTableHash->GetData(pUser->m_dwPartyId);
		
		if(!lpPartyTable)
			return;

		POSITION_ pos = lpPartyTable->pPartyUserList->GetHeadPosition();			

		while(pos)
		{
			CWorldUser*	pUserRev = (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(pos);			

			if(!pUserRev)
				return;

			pPacketDst.bSerType	= (BYTE)pUserRev->m_dwCurServerPos;
			g_pNet->SendToUser(pUserRev->m_dwConnectionIndex, (char*)&pPacketDst, pPacketDst.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}
	}
	else if(pPacket->nIndex==4)
	{
		LP_MESSENGER_TABLE lpMessengerTable = g_pMessengerTableHash->GetData(pUser->m_dwUserIndex);
						
		if(lpMessengerTable)
		{
			ListNode<MESSENGER_USER>* lpMessengerUserPos = lpMessengerTable->pMessengerUserHash->GetHead();

			while(lpMessengerUserPos)
			{
				LP_MESSENGER_USER lpMessengerUser = lpMessengerUserPos->pData;

				if(!lpMessengerUser)
				{
					lpMessengerUserPos = lpMessengerUserPos->pNext;
					continue;
				}

				CWorldUser* pOnLineUser = g_pUserTable->GetUserInfo(lpMessengerUser->dwUserIndex);
				
				if(!pOnLineUser)
				{
					lpMessengerUserPos = lpMessengerUserPos->pNext;
					continue;
				}

				LP_MESSENGER_TABLE lpMessengerSubTable = g_pMessengerTableHash->GetData(lpMessengerUser->dwUserIndex);

				if(lpMessengerSubTable)
				{
					LP_MESSENGER_USER lpMessengerSubUser = lpMessengerSubTable->pMessengerUserHash->GetData(pUser->m_dwUserIndex);

					if(lpMessengerSubUser)
					{
						pPacketDst.bSerType	= (BYTE)pOnLineUser->m_dwCurServerPos;
						g_pNet->SendToUser(pOnLineUser->m_dwConnectionIndex, (char*)&pPacketDst, pPacketDst.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					}
				}				
				lpMessengerUserPos = lpMessengerUserPos->pNext;
			}
		}
		pPacketDst.bSerType	= (BYTE)pUser->m_dwCurServerPos;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pPacketDst, pPacketDst.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
}

void CmdNotice(CWorldUser* pUser ,char* pMsg, DWORD dwLength)
{
	if (NULL == pUser)
	{
		Log(LOG_IMPORTANT, "Argument pUser is NULL at CmdNotice() funtion.");
		return;
	}
	
	if (USER_TYPE_GM != pUser->m_bUserType)
	{
		Log(LOG_IMPORTANT, "It happens to Attempt Notices-action who is not GM!! : %s", pUser->m_szCharacterName);
		return;
	}	

	// ���� �� ���� �����鿡�� �����϶�� �޽����� ������.
	CTWS_GM_NOTICE*		pRecievePacket = (CTWS_GM_NOTICE*)pMsg;
	WSTDS_NOTICE_ACTION NoticeActionPacket;

	memset(NoticeActionPacket.szMessage, 0, 1024);	
	__lstrcpyn(NoticeActionPacket.szMessage, pRecievePacket->szMsg, 1024);	
	NoticeActionPacket.wMsgLen = pRecievePacket->wMsgLen;

	// Ȥ�ö� ���۰� ���������� üũ.
#ifdef _DEBUG
	if (1024 <= NoticeActionPacket.wMsgLen)
	{
		Log(LOG_IMPORTANT, "Dangerous. Notice array overscope. at CmdNotice function.");
		return;
	}
#endif
	
	// �� ���� �����鿡�� ������.
	g_pServerTable->BroadCast((char*)&NoticeActionPacket, NoticeActionPacket.GetPacketSize());	

	// �׸��� ������ ������ ���� ���� ���忡�� ���� �����鿡�� ���� �޽����� �����ش�.
	g_pUserTable->SendUserInWorld(pMsg, dwLength);		
}

void CmdRefreshRecentDungeon(CWorldUser* pUser ,char* pMsg, DWORD dwLength)
{
	CTWS_REFRESH_RECENT_DUNGEON* pPacket = (CTWS_REFRESH_RECENT_DUNGEON*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pUser->m_dwDungeonID);
	if (NULL == pDungeon)
	{
		Log(LOG_IMPORTANT, "Invalid DungeonID is inputed at CmdRefreshRecentDungeon function.");
		return;
	}

	// ���尡 �ƴ϶��
	if(pUser->m_dwDungeonID != 0)
	{
		// �̺�Ʈ ������ �ƴ϶��
		if (DUNGEON_TYPE_EVENT != pDungeon->GetDungeonType())
		{
			// ������ ���� ���� ������ ������Ʈ
			pUser->SetCurDungeon(pPacket->wDungeonID);
		}
	}
	else
	{
		Log(LOG_IMPORTANT, "Invalid DungeonID Entered at CmdRefreshRecentDungeon() Function!");
	}
}

//����� �̵��� ������ 
void CmdGoToWorldSuccess(CWorldUser* pUser ,char* pMsg, DWORD dwLength)
{
	CTWS_GO_TO_WORLD_SUCCESS* pPacket = (CTWS_GO_TO_WORLD_SUCCESS*)pMsg;
	
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	if(pUser->m_ListPosInWorld)
	{
		Log(LOG_IMPORTANT, "WorldUser Already Exists in WorldUserList!! (Name:%s)", pUser->m_szCharacterName);
		return;
	}
	else
	{
		pUser->SetCurDungeon(0);		
		pUser->m_ListPosInWorld = g_pUserTable->m_pUserListInWorld->AddTail(pUser);	//���忡 �ִ� ������ List�� �߰� 
		pUser->m_dwCurServerPos = POS_SERVER_WORLD;
		pUser->m_bWorldId = pPacket->bWorldMapId;		//���� �ִ� ����� ��ȣ�� �����ϰ� 
		Log(LOG_JUST_FILEWRITE, "WorldUserList Added! in CmdGoToWorldSuccess() (Name:%s)", pUser->m_szCharacterName);
	}
	
	// ���Ե� ��Ƽ�� �������� 
	if(pUser->m_dwPartyId)
	{
		LP_PARTY_TABLE lpPartyTable = g_pPartyTableHash->GetData(pUser->m_dwPartyId);
					
		if(lpPartyTable)
		{	
			 if(lpPartyTable->pPartyUserList)
			 {				 
				WSTC_WORLD_APPEAR	packet;
				packet.AppearInfo.dwCharIndex = pUser->m_dwUserIndex;
				packet.AppearInfo.wClass = pUser->m_wClass;
				packet.AppearInfo.wHead  = pUser->m_wHead;
				
				if(pUser->m_byCurrentHand==1)
				{
					packet.AppearInfo.wHandL  = pUser->m_pEquip[ EQUIP_TYPE_LHAND1 ].m_wItemID;
					packet.AppearInfo.wHandR  = pUser->m_pEquip[ EQUIP_TYPE_RHAND1 ].m_wItemID;
				}
				else
				{
					packet.AppearInfo.wHandL  = pUser->m_pEquip[ EQUIP_TYPE_LHAND2 ].m_wItemID;
					packet.AppearInfo.wHandR  = pUser->m_pEquip[ EQUIP_TYPE_RHAND2 ].m_wItemID;
				}

				packet.AppearInfo.wHelmet = pUser->m_pEquip[ EQUIP_TYPE_HELMET ].m_wItemID;
				packet.AppearInfo.wArmor  = pUser->m_pEquip[ EQUIP_TYPE_ARMOR  ].m_wItemID; 
				
				packet.AppearInfo.fAppear_X = pUser->m_v3CurWorldPos.x;
				packet.AppearInfo.fAppear_Z = pUser->m_v3CurWorldPos.z;
				
				 BYTE	bCount = 0;
				 WSTC_WORLD_PARTY_USER_INFO MultiPacket;

				 POSITION_ pos = lpPartyTable->pPartyUserList->GetHeadPosition();

				 while(pos)
				 {
					 CWorldUser* pPartyUser =  (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(pos);																 
					 		
					 //����ʿ� �ִ� ��Ƽ ������ �ƴϸ� Skip
					 if(pPartyUser->m_dwCurServerPos != POS_SERVER_WORLD )	continue;					 

					 //��Ƽ������ ���� ����ʿ� �ִ� �������Դ� �� ������ ���� ����ʿ� ���Դٴ� Appear �޼����� ������. 
					 if(pPartyUser->m_dwUserIndex != pUser->m_dwUserIndex )
					 {
						 g_pNet->SendToUser(pPartyUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);	
					 
						 MultiPacket.AppearInfo[ bCount ].dwCharIndex = pPartyUser->m_dwUserIndex;
						 MultiPacket.AppearInfo[ bCount ].wClass = pPartyUser->m_wClass;
						 MultiPacket.AppearInfo[ bCount ].wHead  = pPartyUser->m_wHead;
						 MultiPacket.AppearInfo[ bCount ].wHelmet = pPartyUser->m_pEquip[ EQUIP_TYPE_HELMET ].m_wItemID;
						 MultiPacket.AppearInfo[ bCount ].wArmor  = pPartyUser->m_pEquip[ EQUIP_TYPE_ARMOR  ].m_wItemID; 
						 
						if(pPartyUser->m_byCurrentHand==1)
						{
							MultiPacket.AppearInfo[ bCount ].wHandL  = pPartyUser->m_pEquip[ EQUIP_TYPE_LHAND1 ].m_wItemID;
							MultiPacket.AppearInfo[ bCount ].wHandR  = pPartyUser->m_pEquip[ EQUIP_TYPE_RHAND1 ].m_wItemID;
						}
						else
						{
							MultiPacket.AppearInfo[ bCount ].wHandL  = pPartyUser->m_pEquip[ EQUIP_TYPE_LHAND2 ].m_wItemID;
							MultiPacket.AppearInfo[ bCount ].wHandR  = pPartyUser->m_pEquip[ EQUIP_TYPE_RHAND2 ].m_wItemID;
						}


						 MultiPacket.AppearInfo[ bCount ].fAppear_X = pPartyUser->m_v3CurWorldPos.x;
						 MultiPacket.AppearInfo[ bCount ].fAppear_Z = pPartyUser->m_v3CurWorldPos.z;
						
						 bCount++;
					 }
				 
				 }	
				 
				 MultiPacket.bUserCount = bCount;
				 g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&MultiPacket, MultiPacket.GetPacketSize()
					 , FLAG_SEND_NOT_ENCRYPTION);			 
			 } 		
		}	
	}
}


//����� �̵� Packet
void CmdWorldMove(CWorldUser* pUser ,char* pMsg, DWORD dwLength)
{
	if(!g_pMap[ pUser->m_bWorldId ])
	{
		Log(LOG_IMPORTANT, "User WorldPosition is invalid! (name:%s, m_bWorldID:%d)", pUser->m_szCharacterName, pUser->m_bWorldId );
		return;
	}

	CTC_WORLD_MOVE *pPacket = (CTC_WORLD_MOVE*)pMsg;

	MAP_TILE *pTile = g_pMap[ pUser->m_bWorldId ]->GetMap(pPacket->wDestX, pPacket->wDestZ);

	if(!pTile)	
	{
		return;
	}

	pUser->m_v3CurWorldPos.x = TILE_X(pPacket->wDestX);
	pUser->m_v3CurWorldPos.y = 0.0f;
	pUser->m_v3CurWorldPos.z = TILE_Z(pPacket->wDestX);

	if(!pUser->m_dwPartyId)	return;

	pPacket->dwCharIndex = pUser->m_dwUserIndex;

	LP_PARTY_TABLE lpPartyTable = g_pPartyTableHash->GetData( pUser->m_dwPartyId );
					
	if(lpPartyTable)
	{	
		 if(lpPartyTable->pPartyUserList)
		 {
			 POSITION_ pos = lpPartyTable->pPartyUserList->GetHeadPosition();

			 while(pos)
			 {
				 CWorldUser* pPartyUser =  (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(pos);

				 if(pPartyUser->m_dwUserIndex==pUser->m_dwUserIndex
					 || pPartyUser->m_dwCurServerPos != POS_SERVER_WORLD)
				 {
					 continue;	//�����̰ų� ���忡 ���� ������ Skip
				 }
				 
				  g_pNet->SendToUser( pPartyUser->m_dwConnectionIndex, pMsg, dwLength , FLAG_SEND_NOT_ENCRYPTION);
			 }
		 }
	}
}

//����� Stop Packet
void CmdWorldStop(CWorldUser* pUser ,char* pMsg, DWORD dwLength)
{
	if(!g_pMap[ pUser->m_bWorldId ])
	{
		Log(LOG_IMPORTANT, "User WorldPosition is invalid! (name:%s, m_bWorldID:%d)", pUser->m_szCharacterName, pUser->m_bWorldId );
		return;
	}

	CTC_WORLD_STOP *pPacket = (CTC_WORLD_STOP*)pMsg;

	MAP_TILE* pTile = g_pMap[ pUser->m_bWorldId ]->GetTile(pPacket->v3StopPos.x, pPacket->v3StopPos.z);
	if(!pTile)	return;

	pUser->m_v3CurWorldPos = pPacket->v3StopPos;

	if(!pUser->m_dwPartyId)	return;

	pPacket->dwCharIndex = pUser->m_dwUserIndex;

	LP_PARTY_TABLE lpPartyTable = g_pPartyTableHash->GetData( pUser->m_dwPartyId );
					
	if(lpPartyTable)
	{	
		 if(lpPartyTable->pPartyUserList)
		 {
			 POSITION_ pos = lpPartyTable->pPartyUserList->GetHeadPosition();

			 while(pos)
			 {
				 CWorldUser* pPartyUser =  (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(pos);

				 if(pPartyUser->m_dwUserIndex == pUser->m_dwUserIndex 
					 || pPartyUser->m_dwCurServerPos != POS_SERVER_WORLD)		continue;	//�����̰ų� ���忡 ���� ������ Skip
				 
				  g_pNet->SendToUser( pPartyUser->m_dwConnectionIndex, pMsg, dwLength , FLAG_SEND_NOT_ENCRYPTION);
			 }
		 }
	}
}

void CmdWorldSetPosition(CWorldUser* pUser ,char* pMsg, DWORD dwLength)
{
	CTC_MOVE_POSITION*	pPacket = (CTC_MOVE_POSITION*)pMsg;

	if(!pUser->m_dwPartyId)	return;

	pPacket->dwCharIndex = pUser->m_dwUserIndex;

	LP_PARTY_TABLE lpPartyTable = g_pPartyTableHash->GetData( pUser->m_dwPartyId );
					
	if(lpPartyTable)
	{	
		 if(lpPartyTable->pPartyUserList)
		 {
			 POSITION_ pos = lpPartyTable->pPartyUserList->GetHeadPosition();

			 while(pos)
			 {
				 CWorldUser* pPartyUser =  (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(pos);

				 if(pPartyUser->m_dwUserIndex == pUser->m_dwUserIndex 
					 || pPartyUser->m_dwCurServerPos != POS_SERVER_WORLD)		continue;	//�����̰ų� ���忡 ���� ������ Skip
				 
				  g_pNet->SendToUser( pPartyUser->m_dwConnectionIndex, pMsg, dwLength , FLAG_SEND_NOT_ENCRYPTION);
			 }
		 }
	}
}

void CmdAirshipRiding(CWorldUser* pUser ,char* pMsg, DWORD dwLength)
{
	CTC_AIRSHIP_RIDING*	pPacket = (CTC_AIRSHIP_RIDING*)pMsg;

	if(!pUser->m_dwPartyId)	return;

	pPacket->dwCharIndex = pUser->m_dwUserIndex;

	LP_PARTY_TABLE lpPartyTable = g_pPartyTableHash->GetData( pUser->m_dwPartyId );
					
	if(lpPartyTable)
	{	
		 if(lpPartyTable->pPartyUserList)
		 {
			 POSITION_ pos = lpPartyTable->pPartyUserList->GetHeadPosition();

			 while(pos)
			 {
				 CWorldUser* pPartyUser =  (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(pos);

				 if(pPartyUser->m_dwUserIndex == pUser->m_dwUserIndex 
					 || pPartyUser->m_dwCurServerPos != POS_SERVER_WORLD)		continue;	//�����̰ų� ���忡 ���� ������ Skip
				 
				  g_pNet->SendToUser( pPartyUser->m_dwConnectionIndex, pMsg, dwLength , FLAG_SEND_NOT_ENCRYPTION);
			 }
		 }
	}
}

void CmdAirshipGetoff(CWorldUser* pUser ,char* pMsg, DWORD dwLength)
{
	CTC_AIRSHIP_GETOFF*	pPacket = (CTC_AIRSHIP_GETOFF*)pMsg;

	if(!pUser->m_dwPartyId)	return;

	pPacket->dwCharIndex = pUser->m_dwUserIndex;

	LP_PARTY_TABLE lpPartyTable = g_pPartyTableHash->GetData( pUser->m_dwPartyId );
					
	if(lpPartyTable)
	{	
		 if(lpPartyTable->pPartyUserList)
		 {
			 POSITION_ pos = lpPartyTable->pPartyUserList->GetHeadPosition();

			 while(pos)
			 {
				 CWorldUser* pPartyUser =  (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(pos);

				 if(pPartyUser->m_dwUserIndex == pUser->m_dwUserIndex 
					 || pPartyUser->m_dwCurServerPos != POS_SERVER_WORLD)		continue;	//�����̰ų� ���忡 ���� ������ Skip
				 
				  g_pNet->SendToUser( pPartyUser->m_dwConnectionIndex, pMsg, dwLength , FLAG_SEND_NOT_ENCRYPTION);
			 }
		 }
	}
}


void CmdLoadingStatus(CWorldUser* pUser ,char* pMsg, DWORD dwLength)
{
	CTWS_LOADING_STATUS	*pPacket = (CTWS_LOADING_STATUS*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	pUser->m_bIsMapLoading = pPacket->bIsLoading;
}

#ifndef NEW_COMMAND
void CmdGmCommand( CWorldUser* pUser, char* pMsg, DWORD dwLength )
{
	pUser;

	CTWS_GM_COMMAND*	pCommand = ( CTWS_GM_COMMAND* )pMsg;
	if(pCommand->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pCommand->bGameStatus, pCommand->bHeader, dwLength);
		return;
	}
	
	switch( pCommand->wCommand )
	{
	case GM_COMMAND_RECALL:
		{
			CWorldUser* pPortalUser = g_pUserTable->GetUserInfoByName(pCommand->szCharName);

			if(pPortalUser)
			{
				WSTDS_USER_PORTAL pPacket;
				pPacket.wDungeonID		= (WORD)pCommand->dwTemp[0];	//Dungeon ID.
				pPacket.byLayerIndex	= (BYTE)pCommand->dwTemp[1];	// Layer.				
				pPacket.wX				= (WORD)pCommand->dwTemp[2];	// X.
				pPacket.wY				= (WORD)pCommand->dwTemp[3];	// Y.
				pPacket.dwUserIndex		= pPortalUser->m_dwUserIndex;

				DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pPortalUser->m_dwDungeonID);

				if(pDungeon)
					g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			}
		}
		break;
		
	case GM_COMMAND_USERPOTAL:
		{
			CWorldUser* pPortalUser = g_pUserTable->GetUserInfoByName(pCommand->szCharName);

			if(pPortalUser)
			{
				WSTDS_USER_PORTAL pPacket;
				pPacket.wDungeonID		= (WORD)pCommand->dwTemp[0];	//Dungeon ID.
				pPacket.byLayerIndex	= (BYTE)pCommand->dwTemp[1];	// Layer.				
				pPacket.wX				= (WORD)pCommand->dwTemp[2];	// X.
				pPacket.wY				= (WORD)pCommand->dwTemp[3];	// Y.
				pPacket.dwUserIndex		= pPortalUser->m_dwUserIndex;

				DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pPortalUser->m_dwDungeonID);

				if(pDungeon)
					g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			}
		}
	}
}

#else

void CmdGmCommand( CWorldUser* pUser, char* pMsg, DWORD dwLength )
{
	CTWS_NEW_GM_COMMAND*	pCommand = ( CTWS_NEW_GM_COMMAND * )pMsg;
	if(pCommand->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pCommand->bGameStatus, pCommand->bHeader, dwLength);
		return;
	}

	if (FALSE == pUser->IsGMUser())
	{
		// ����~~~~!! GM�� �ƴϸ鼭 GM ����� ������ �ϴٴ�..!!!
		return;
	}

	WSTDS_USER_PORTAL packet;

	switch( pCommand->sBaseGMCmdParam.eGMCommand )
	{
	case GM_Command_Where_Is_Someone:
		{
		}
		break;
	case GM_Command_Get_Recall_Someone:
		{
			CWorldUser* pPortalUser = 
				g_pUserTable->GetUserInfoByName(pCommand->sGMCmdParamRecallSomeone.szCharacterName);
			
			if ( pPortalUser )
			{
				packet.byGMCommand	= GM_Command_Get_Recall_Someone;
				packet.dwUserIndex	= pPortalUser->m_dwUserIndex;

				DUNGEON_MOVE_INFO &rDMI = packet.DMI;

				rDMI.wDungeonID		= pCommand->sGMCmdParamRecallSomeone.wDungeonID;
				rDMI.byLayerIndex	= pCommand->sGMCmdParamRecallSomeone.bLayer;	
				rDMI.wX				= pCommand->sGMCmdParamRecallSomeone.wX;
				rDMI.wY				= pCommand->sGMCmdParamRecallSomeone.wZ;

				DUNGEON_DATA_EX* pDungeon = 
					g_pDungeonTable->GetDungeonInfo((WORD)pPortalUser->m_dwDungeonID);

				if ( pDungeon )
				{
					g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, 
						(char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}
			}
		}
		break;
		
	case GM_Command_Get_Move_Someone:
		{
			CWorldUser * pTargetToUser = 
				g_pUserTable->GetUserInfoByName(pCommand->sGMCmdParamGetMoveSomeone.szCharacterName);
				
			if (!pTargetToUser) return;

			DUNGEON_MOVE_INFO &rDMI = packet.DMI;

			packet.byGMCommand			= GM_Command_Get_Move_Someone;
			packet.dwUserIndex			= pUser->m_dwUserIndex;					// ��Ż �� �� 
			packet.dwTargetToUserIndex	= pTargetToUser->m_dwUserIndex;			// ��� �� ���� 
			
			rDMI.wDungeonID			= (WORD)pTargetToUser->m_dwDungeonID;	// ��� ������	
																				// m_dwDungeonID�� ��ȿ������ �׽�Ʈ�� �˾ƺ��� �� �� �ϴ�.
			rDMI.byLayerIndex			= 255;	// ���̾ �𸥴�. 

			DUNGEON_DATA_EX* pDungeon = 
				g_pDungeonTable->GetDungeonInfo((WORD)pUser->m_dwDungeonID);

			if (pDungeon)
			{
				g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, 
					(char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			}
		}
		break;

	case GM_Command_Dungeon_Portal:
		{
			DUNGEON_MOVE_INFO &rDMI = packet.DMI;
			
			packet.byGMCommand			= GM_Command_Dungeon_Portal;
			packet.dwUserIndex			= pUser->m_dwUserIndex;									// ��Ż �� �� 
			packet.dwTargetToUserIndex	= 0;

			rDMI.wDungeonID			= (WORD)pCommand->sGMCmdParamDungeonPortal.wDungeonID;	// ��� ������	
			rDMI.byLayerIndex			= (BYTE)pCommand->sGMCmdParamDungeonPortal.bLayerFloor;		
			rDMI.wX = 0;
			rDMI.wY = 0;

			if (rDMI.byLayerIndex > 255)
			{
				return;
			}

			DUNGEON_DATA_EX* pDungeon = 
				g_pDungeonTable->GetDungeonInfo((WORD)pUser->m_dwDungeonID);

			if (pDungeon && pDungeon->m_pServer)
			{
				g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, 
					(char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			}		
		}
		break;

	case GM_Command_Kick_User:
		{
			CWorldUser * pTargetToUser = 
				g_pUserTable->GetUserInfoByName(pCommand->sGMCmdParamKickUser.szCharacterName);

			if (!pTargetToUser) return;

//			CTWS_KICK_USER* pPacket = (CTWS_KICK_USER*)pMsg;
			
			if(pTargetToUser)
			{
				Log(LOG_NORMAL, "GM World User Kicked! (Name:%s)", pTargetToUser->m_szCharacterName);
				pTargetToUser->m_bKicked = 1;	
				DWORD dwConnectionIndex = pTargetToUser->m_dwConnectionIndex;
				OnDisconnectUser(dwConnectionIndex);
				g_pNet->SetUserInfo(dwConnectionIndex, NULL);
				g_pNet->CompulsiveDisconnectUser(dwConnectionIndex);
			}
			
			// Ȯ���ϰ� Kick �ع�����. 
			WSTDS_KICK_USER	packet;
			packet.dwCharIndex = pTargetToUser->m_dwUserIndex;
			g_pServerTable->BroadCast((char*)&packet, packet.GetPacketSize());
			
		}
		break;

	default:
		break;
	}
}

#endif


//============================================
// �̺�Ʈ �������� ���¿� ������ ����.
//============================================
void CmdQueryEventInfo(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{
	CTWS_QUERY_EVENT_DUNGEON_INFO* pPacket = (CTWS_QUERY_EVENT_DUNGEON_INFO*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	if (1 == pPacket->bRequestType)		
	{
		// �̺�Ʈ ���� ��ü�� ������ ���� ��.
		SendAllEventInfo(pUser->m_dwConnectionIndex);
	}
	else								
	{
		// �̺�Ʈ ������ ���¸��� ���� ��.
		SendAllEventState(pUser->m_dwConnectionIndex);
	}	
}


//============================================
// �̺�Ʈ �������� ������ ����.
//============================================
void SendAllEventInfo(DWORD dwConnectionIndex)
{	
	if(NULL == g_pEventDungeonManager)						return;
	if(NULL == g_pEventDungeonManager->m_pEventDungeonList)	return;	
	
	POSITION_ pos = g_pEventDungeonManager->m_pEventDungeonList->GetHeadPosition();
	
	WSTC_EVENT_DUNGEON_INFO	packet;
	DUNGEON_DATA_EX*		pDungeon = NULL;
	EVENT_DUNGEON_PROPERTY* pEventProperty = NULL;

	while(pos)
	{
		pDungeon = (DUNGEON_DATA_EX*)g_pEventDungeonManager->m_pEventDungeonList->GetNext(pos);
		
		if(DUNGEON_TYPE_EVENT != pDungeon->GetDungeonType())
			continue;		

		pEventProperty = g_pEventDungeonManager->GetEventProperty(pDungeon->m_dwID);
		if(NULL == pEventProperty)
			continue;

		packet.wDungeonID		= (WORD)pDungeon->m_dwID;		
		packet.byEventState		= pDungeon->m_bEventState;			
		packet.byEventType		= (BYTE)pEventProperty->byDungeonType;
		packet.byWorldMapID		= (BYTE)pDungeon->m_wWorldMapID;		
		packet.byEnterMinLevel	= pEventProperty->byEnterLevelMin;
		packet.byEnterMaxLevel	= pEventProperty->byEnterLevelMax;
		packet.dwReadyOpenTime	= (DWORD)pEventProperty->dwReadyOpenTime;
		memset(packet.szDungeonName, 0, MAX_DUNGEON_NAME_LENGTH);
		__lstrcpyn(packet.szDungeonName, pDungeon->m_szDungeonName, MAX_DUNGEON_NAME_LENGTH);
		packet.dwEventEndTick	= pEventProperty->dwCloseTime; 

		g_pNet->SendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
}


//============================================
// �̺�Ʈ �������� ���¸� ����.
//============================================
void SendAllEventState(DWORD dwConnectionIndex)
{
	if(NULL == g_pEventDungeonManager)						return;
	if(NULL == g_pEventDungeonManager->m_pEventDungeonList)	return;	
	
	WSTC_EVENT_DUNGEON_STATE_INFO	packet;
	DUNGEON_DATA_EX*				pDungeon = NULL;
	EVENT_DUNGEON_PROPERTY*			pEventProperty = NULL;

	POSITION_ pos = g_pEventDungeonManager->m_pEventDungeonList->GetHeadPosition();

	while(pos)
	{
		pDungeon = (DUNGEON_DATA_EX*)g_pEventDungeonManager->m_pEventDungeonList->GetNext(pos);
		
		if(DUNGEON_TYPE_EVENT != pDungeon->GetDungeonType())
			continue;
		
		pEventProperty = g_pEventDungeonManager->GetEventProperty(pDungeon->m_dwID);
		if(NULL == pEventProperty)
			continue;

		packet.wDungeonID		= (WORD)pDungeon->m_dwID;		
		packet.byEventState		= pDungeon->m_bEventState;	
		packet.dwReadyOpenTime = pEventProperty->dwReadyOpenTime;

		g_pNet->SendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
}


//============================================
// ��Ƽ ��Ī : �ִ��� 2005.1.24
//============================================
void CmdPMatchRegister(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{
	CTWS_PMATCH_REGISTER* pPacket = (CTWS_PMATCH_REGISTER*)pMsg;
	WSTC_PMATCH_REGISTER_RESULT RegisterResult;
	MATCH_CONDITION_PARTY cnd;

	// �ߺ� ���ɼ��� �����Ƿ� �ѹ� ������
	CmdPMatchEscape(pUser, NULL, 0);

	// ��Ī ���� �ʱ�ȭ
	pUser->m_PMatchCondition = pPacket->condition;
	pUser->m_bLevelDist = 0;
	pUser->m_iSign = 1;

	// ������ ���
	cnd.bLevel = (BYTE)pUser->m_dwLevel;
	cnd.bExp = (BYTE)pUser->m_PMatchCondition.bExp;
	cnd.bItem = pUser->m_PMatchCondition.bItem;
	
	// ����� ��Ͻ�Ű�� �����Ǹ�
	if(g_PartyMatcher.Insert(cnd, pUser->m_dwUserIndex, ((pUser->m_PMatchCondition.bMaster == 1) ? false : true)) )
	{
		RegisterResult.bResult = 0;
	}
	// �����ϸ�
	else
	{
		RegisterResult.bResult = 1;
	}

	// Ŭ���̾�Ʈ���� ��� ����� �뺸
	g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&RegisterResult, RegisterResult.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);

}

void CmdPMatchEscape(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{
	WSTC_PMATCH_ESCAPE_RESULT EscapeResult;
	MATCH_CONDITION_PARTY cnd;

	// ������ ��Ī �Ƿ� ���̸�
	if(pUser->m_dwPMatchIndex != 0)
	{
	//	DWORD dwOwnIndex, dwGuestIndex;
//		CWorldUser *pOwnUser, *pGuestUser;

		// �� ��Ī�� ������ ���� ��Ŵ
		g_PartyMatcher.MatchFail(pUser->m_dwPMatchIndex);
	}

	// ����� ������ ���� ������ �����
	cnd.bLevel = (BYTE)pUser->m_dwLevel;
	cnd.bExp = (BYTE)pUser->m_PMatchCondition.bExp;
	cnd.bItem = (BYTE)pUser->m_PMatchCondition.bItem;

	// ����ڿ��� Ż��
	if(g_PartyMatcher.Remove(cnd, pUser->m_dwUserIndex))
	{
		EscapeResult.bResult = 0;
	}
	else
	{
		// Ż�� �Ұ��� �ϸ�, �����ڵ�
		EscapeResult.bResult = 1;

		// �켱 ���� ���� ���� ������ ����
		return;
	}
	
	// Ŭ���̾�Ʈ���� Ż�� ����� �뺸
	g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&EscapeResult, EscapeResult.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
}

void CmdPMatchRequestResult(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{
	CTWS_PMATCH_REQUEST_RESULT* pPacket = (CTWS_PMATCH_REQUEST_RESULT*)pMsg;
	DWORD dwOwnIndex, dwGuestIndex;

	// ����� ��Ī �Ƿ� ���� �ƴ϶��
	if(pUser->m_dwPMatchIndex == 0)
	{
		return;
	}

	// ������ ��Ī ������ �޾�����, ��Ī �Ƿ� ���� �ƴ϶��
	if(g_PartyMatcher.GetReadyDB(pUser->m_dwPMatchIndex, dwOwnIndex, dwGuestIndex) == false)
	{
		// ���� ������
		// ������ ��Ŷ�� ���ų�, ��Ŷ�� ������
		Log(LOG_JUST_DISPLAY, "PM : ��Ī�� ��𼱰� �ߴܵ� %d", pUser->m_dwPMatchIndex);

		// ��Ī ���� ��ȣ�� ����
		pUser->m_dwPMatchIndex = 0;

		return;
	}
	
	CWorldUser *pOwnUser = g_pUserTable->GetUserInfo(dwOwnIndex);
	CWorldUser *pGuestUser = g_pUserTable->GetUserInfo(dwGuestIndex);

	// ���� �ϳ��� �α׾ƿ� �ߴٸ�
	if(pOwnUser == NULL || pGuestUser == NULL)
	{
		// ��Ī�� ��� ��Ų��
		g_PartyMatcher.MatchFail(pUser->m_dwPMatchIndex);

		return;
	}

	/////////////////////////////////////////////
	// ���忡�� ���� �Ƿڸ� �ϰ�,
	// ������ �³��� �Ѵٸ� ��󿡰� �Ƿڸ� �Ѵ�.

	// �� ��� �� �ϳ��� �³��� �ϸ�
	if(pPacket->bResult == 0)
	{
		// �����̶��
		if(pUser == pOwnUser)
		{
			// ��󿡰� ��Ī ���Ǹ� ����
			WSTC_PMATCH_REQUEST packet;
			packet.bLevel = (BYTE)pOwnUser->m_dwLevel;
			packet.bClass = (BYTE)pOwnUser->m_wClass;
			strcpy(packet.szName, pOwnUser->m_szCharacterName);
			packet.bMaster = 0;
			g_pNet->SendToUser(pGuestUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
		}
		// ��� �̶��
		else
		{
			// ��Ƽ�� �ξ��ش�
			CTWS_PARTY_APPROVE PartyApprove;
			PartyApprove.dwUserIndex = pOwnUser->m_dwUserIndex;
			__lstrcpyn(PartyApprove.szName, pOwnUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
			CmdPartyApprove(pGuestUser, (char*)&PartyApprove, PartyApprove.GetPacketSize());

			// ��Ƽ�� �ξ����ٸ�
			if(pOwnUser->m_dwPartyId == pGuestUser->m_dwPartyId)
			{
				// ��Ī �޴������� ������ �˸�
				g_PartyMatcher.MatchSuccess(pUser->m_dwPMatchIndex);
				
				// ������ ���忡�� ��Ż��Ŵ
				WSTDS_PARTY_PORTAL Portal;
				Portal.dwUserIndex = pOwnUser->m_dwUserIndex;
				Portal.dwPartalUserIndex = pGuestUser->m_dwUserIndex;
				Portal.dwPartyId = pOwnUser->m_dwPartyId;
				Portal.byIndex = 255;	// �������� ������� �ʰ� �̵�
				__lstrcpyn(Portal.szName, pOwnUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
				CmdPartyPortal(NULL, (char*)&Portal, Portal.GetPacketSize());
			}
			// ���� ���ߴٸ�
			else
			{
				// ��Ī �޴������� ���и� �˸�
				g_PartyMatcher.MatchFail(pUser->m_dwPMatchIndex);
			}

			// ������ ��Ī �Ƿ� ���� �ƴ����� ����
			pOwnUser->m_dwPMatchIndex = 0;
			pGuestUser->m_dwPMatchIndex = 0;
		}
	}
	// �� ��� �� �ϳ��� �źθ� �ϸ�
	else
	{
		// ��Ī�� ��ҽ�Ű�� ������ �Ƿ� ���¸� �����
		g_PartyMatcher.MatchFail(pUser->m_dwPMatchIndex);
	}
}

void CmdPMatchChange(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{
	/*
	WSTC_PMATCH_CHANGE_RESULT ChangeResult;

	// ��Ī ���� ������ ���� �̱���
	// ������ ��Ī ������ ����
	pUser->m_PMatchCondition = pPacket->condition;

	// ��Ī ������ �����
	cnd.bLevel = (BYTE)pUser->m_dwLevel;
	cnd.bExp = (BYTE)pUser->m_PMatchCondition.bExp;
	cnd.bItem = (BYTE)pUser->m_PMatchCondition.bItem;

	if(g_PartyMatcher.Remove(cnd, pUser->m_dwUserIndex) && g_PartyMatcher.Insert(cnd, pUser->m_dwUserIndex, false))
	{
		packet.bResult = 0;
	}
	else
	{
		packet.bResult = 1;
	}

	g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&ChangeResult, ChangeResult.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
	*/
}
