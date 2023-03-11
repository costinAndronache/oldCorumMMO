#include "DungeonMsg_seven.h"
#include "InitGame.h"
#include "GameControl.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "UserInterface.h"
#include "message.h"
#include "DungeonTable.h"
#include "Chat.h"
#include "CodeFun.h"
#include "CorumResource.h"
#include "DungeonMsg.h"
#include "RequestMatchWnd.h"
#include "InputManager.h"
#include "SevenWork.h"
#include "GroupWnd.h"
#include "RivalGuildWar.h"

void InitPacketProcDungeonSeven()
{
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_DSTC::CMD_DUNGEON_EMOTICON ]				= CmdEmoticon;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_DSTC::CMD_DUNGEON_REQUEST_MATCH_MAN2MAN]	= CmdDungeonRequestMatchM2M;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_DSTC::CMD_DUNGEON_REPLY_MATCH_MAN2MAN]		= CmdDungeonReplyMatchM2M;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_DSTC::CMD_READY_MATCH_MAN2MAN]				= CmdReadyMatchM2M;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_DSTC::CMD_START_MATCH_MAN2MAN]				= CmdStartMatchM2M;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_DSTC::CMD_END_MATCH_MAN2MAN ]				= CmdEndMatchM2M;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_DSTC::CMD_MATCH_USER_INFO ]				= CmdMatchUserInfo;	
}

void CmdEmoticon( char* pMsg, DWORD dwLen )
{
	DSTC_DUNGEON_EMOTICON * pEmoticon = (DSTC_DUNGEON_EMOTICON *)pMsg;
	char	szBuf[255] = {0,};

	CUser* pUser = g_pUserHash->GetData(pEmoticon->dwUserIndex);
	if (!pUser)	return;
	
	if (!pUser->m_hPlayerEmoticon.pHandle)
	{
		pUser->m_hPlayerEmoticon.pDesc				= AllocObjDesc();
		pUser->m_hPlayerEmoticon.pDesc->bType		= OBJECT_TYPE_EFFECT;
		pUser->m_hPlayerEmoticon.pDesc->ObjectFunc	= GXEmoticonProc;
		pUser->m_hPlayerEmoticon.pDesc->pInfo		= pUser;

		lstrcpy(szBuf, "e0202000.chr");
		pUser->m_hPlayerEmoticon.pHandle = 
			CreateHandleObject(GetFile(szBuf, DATA_TYPE_EFFECT), GXPlayerPROC, pUser->m_hPlayerEmoticon.pDesc, GXOBJECT_CREATE_TYPE_EFFECT);
	}

	GXSetPosition(pUser->m_hPlayerEmoticon.pHandle, &pUser->m_v3CurPos, FALSE, TRUE);
		
	g_pExecutive->SetAlphaFlag(pUser->m_hPlayerEmoticon.pHandle, 255);
	ShowObject(pUser->m_hPlayerEmoticon.pHandle);
	::SetAction(pUser->m_hPlayerEmoticon.pHandle, pEmoticon->bWhatEmoticon, ACTION_ONCE);
}


// ===================
// ������ 1:1 ����� ��û�Ͽ���.
// ===================
void CmdDungeonRequestMatchM2M(char* pMsg, DWORD dwLen )
{
	DSTC_DUNGEON_REQUEST_MATCH_MAN2MAN	* pPacket = (DSTC_DUNGEON_REQUEST_MATCH_MAN2MAN *) pMsg;
	
	CUser* pUser = g_pUserHash->GetData(pPacket->dwRequestUserIndex);
	if (!pUser)	return;
	if (pUser->m_bMatching == TRUE)
	{
		// �������̴�
		return;
	}

	// ��û���� �ε����� ������ �־�� ����Ҷ� �� �������ִ�.
	CRequestMatchWnd * pRequestMatchWnd = CRequestMatchWnd::GetInstance();
	pRequestMatchWnd->m_dwRequestMatchUserIndex = pPacket->dwRequestUserIndex;

	__lstrcpyn(pRequestMatchWnd->m_szName, pUser->m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
	pRequestMatchWnd->SetActive(TRUE);

	CUserInterface::GetInstance()->m_bMatch = FALSE;	// MATCH ��ư Ǯ���ش�.
	CUserInterface::GetInstance()->ClearPVPFlag();	// PVP ���� �ʱ�ȭ!!!
}

// ===================
// ������ ����� �������� ����Դ�. 
// ===================
void CmdDungeonReplyMatchM2M(char* pMsg, DWORD dwLen)
{
	DSTC_DUNGEON_REPLY_MATCH_MAN2MAN * pPacket = (DSTC_DUNGEON_REPLY_MATCH_MAN2MAN *) pMsg;
	char		szBuf[255] = {0,};
	CRequestMatchWnd * pRequestMatchWnd = CRequestMatchWnd::GetInstance();
	CGroupWnd*	pGroupWnd = CGroupWnd::GetInstance();
	
	switch(pPacket->bReplyCode)
	{
	case 0:	// ��Ÿ ���� 
		break;
	case 1: // ��� 
		g_pMainPlayer->SetAction(MOTION_TYPE_WARSTAND, 0, ACTION_LOOP );		
		g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL);
		g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc = NULL;
		SendStopPacket();
		g_pMainPlayer->SetStatus(UNIT_STATUS_PORTAL_MOVING);
		
		g_pMainPlayer->m_dwMatchRecords += 1;

		CUserInterface::GetInstance()->m_nPK = __PK_MODE__;
		g_pMainPlayer->m_bMatching = TRUE;	
		g_pMainPlayer->m_dwMatchUserIndex = pPacket->dwMatchUserIndex;

		if (g_pMainPlayer->m_dwUserIndex != pPacket->dwMatchUserIndex)
		{
			CUser * pMatchUser = (CUser *)g_pUserHash->GetData(pPacket->dwMatchUserIndex);
			if (pMatchUser)
			{
				pMatchUser->m_bMatching = TRUE;
			}
		}

		// sung-han 2005-02-15 pvp�� �Ұ�� �������Ī ��Ҹ� ������--------------------------------
		if(pGroupWnd->m_bGuildWarMatchEnable)
		{
			pGroupWnd->m_bGuildWarMatchEnable = FALSE;
			pGroupWnd->m_bGuildWarPacketSendEnable = FALSE;
			TEAM_MATCH_PACKET TeamMatch;
			TeamMatch.Body.Condtion.iMatchType = M_T_NONE;
			TeamMatch.Body.Condtion.iMatchGuildNumber = 0;
			TeamMatch.Body.Condtion.iMatchGuildType = T_T_NONE;
			Cmd_CTWS_GuildWarPacket(TeamMatch, CTWS_TM_REMOVE);
		}
		CUserInterface::GetInstance()->ClearPVPFlag();	// PVP ���� �ʱ�ȭ!!!
		//--------------------------------------------------------------------------------------------------
		goto Not_Init_Match;
	case 2: // �ź�
		{
			CUser* pUser = g_pUserHash->GetData(pPacket->dwMatchUserIndex);
			if (!pUser)	return;

			wsprintf(szBuf, g_Message[ETC_MESSAGE531].szMessage, pUser->m_szName); // "%s ���� ����� �ź� �ϼ̽��ϴ�!"
			DisplayMessageAdd(szBuf, 0xFFFF0000);

			g_pMainPlayer->m_bMatching = FALSE;		
		}
		break;
	case 3:	//������ �����
		{
			CUser* pUser = g_pUserHash->GetData(pPacket->dwMatchUserIndex);
			if (!pUser)	return;

			wsprintf(szBuf, g_Message[ETC_MESSAGE532].szMessage, pUser->m_szName); // "%s ���� �������Դϴ�!"
			DisplayMessageAdd(szBuf, 0xFFFF0000);
			
			goto Not_Init_Match;
		}
		break;
	case 4: // ������
		{
			// "%s ���� ���� �������Դϴ�!"
			wsprintf(szBuf, g_Message[ETC_MESSAGE533].szMessage, g_pMainPlayer->m_szName); 
			DisplayMessageAdd(szBuf, 0xFFFF0000);			
		}
		break;
	case 5: // ������Ұ� ��á��.
		{
			// "������Ұ� ��á���ϴ�. ����Ŀ� ������ ��û�Ͻñ� �ٶ��ϴ�."
			DisplayMessageAdd(g_Message[ETC_MESSAGE534].szMessage, 0xFFFF0000); 

		}
		break;
	case 6:	// ��ȯ�߿� ����� �Ϸ����Ѵ�.
		{
			// "��ȯ���� ���� ����� �Ͻ� �� �����ϴ�. ��ȯ �Ϸ��� ����Ͻñ� �ٶ��ϴ�."
			DisplayMessageAdd(g_Message[ETC_MESSAGE535].szMessage, 0xFFFF0000); 
		}
		break;
	case 7:
		{
			//"�������� ���� ����� �Ͻ� �� �����ϴ�. ���� ������ ����Ͻñ� �ٶ��ϴ�."
			DisplayMessageAdd(g_Message[ETC_MESSAGE1052].szMessage, 0xFFFF0000);
		}
		break;
	case 8:
		{
			//"�ɱ����� ���� ����� �Ͻ� �� �����ϴ�. �ɱ� ������ ����Ͻñ� �ٶ��ϴ�."
			DisplayMessageAdd(g_Message[ETC_MESSAGE1053].szMessage, 0xFFFF0000);
		}
		break;
	case 9:
		{
			//"������ �������� ���� ����� �Ͻ� �� �����ϴ�."
			DisplayMessageAdd(g_Message[ETC_MESSAGE1054].szMessage, 0xFFFF0000);
		}
		break;
	case 12:
		{
			//"������ �ɱ����� ���� ����� �Ͻ� �� �����ϴ�."
			DisplayMessageAdd(g_Message[ETC_MESSAGE1055].szMessage, 0xFFFF0000);
		}
		break;

	case CUser::MATCH_STATUS_USER_REQUEST_AFTER_USER_GOTO_WORLD:
	default: // �������� ���� ����
		{
			char	szMsg[255] = {0,};
			wsprintf(szMsg, g_Message[ETC_MESSAGE536].szMessage, pPacket->bReplyCode); // "�ϴ��� ���� ���� �ڵ� : %d"
			DisplayMessageAdd(szMsg, 0xFFFF0000);
		}
		break;
	}

	// ���� �߻��� �ʱ�ȭ 
	pRequestMatchWnd->SetActive(FALSE);
	g_pMainPlayer->m_bMatching = FALSE;
	CRequestMatchWnd::GetInstance()->m_dwRequestMatchUserIndex = 0;

	if(pPacket->bReplyCode>=7 && pPacket->bReplyCode<=9 && pPacket->bReplyCode==12)
	{
		g_pMainPlayer->SetAction(MOTION_TYPE_WARSTAND, 0, ACTION_LOOP );		
		g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL);
	}	

Not_Init_Match:
	return;
}

// ========
// ��� �غ� 
// ========
void CmdReadyMatchM2M(char* pMsg, DWORD dwLen)
{
	DSTC_READY_MATCH_MAN2MAN * pPacket = (DSTC_READY_MATCH_MAN2MAN *)pMsg;
	char	szBuf[255] = {0,};

	CUser * pReadyUser = g_pUserHash->GetData(pPacket->dwReadyUserIndex);
	if ( !pReadyUser )
	{
		return;	
	}

	if( pReadyUser->m_dwUserIndex == g_pMainPlayer->m_dwUserIndex )
	{
		CGuildWarStatusWnd::GetInstance()->SetActive();
		CGuildWarStatusWnd::GetInstance()->SetMark_Ready();
	}
	
	wsprintf(szBuf, g_Message[ETC_MESSAGE537].szMessage, pReadyUser->m_szName); // "%s ���� ��� �غ� �Ǿ����ϴ�."
	DisplayMessageAdd(szBuf, 0xFF00FF1E);	
}

// ========
// ��� ����
// ========
void CmdStartMatchM2M(char* pMsg, DWORD dwLen)
{
	DSTC_START_MATCH_MAN2MAN * pPacket = (DSTC_START_MATCH_MAN2MAN *)pMsg;

	CUser * pUser = (CUser *)g_pUserHash->GetData(pPacket->dwMatchUserIndex);
	
	CUserInterface * pUserInterface = CUserInterface::GetInstance();
	if (pUser)
	{
		pUserInterface->m_bMatchUserHPShowFlag = TRUE;
		pUserInterface->m_bMatchUserHPShowEnable = TRUE;
		pUserInterface->m_pMatchUser = pUser;
		g_pMainPlayer->m_dwMatchUserIndex = pUser->m_dwUserIndex;

		g_MatchInfo.SetDisplayMatchFlag(TRUE);
		g_MatchInfo.SetMatchTimeTick(pPacket->dwMatchTimeTick);

		pUserInterface->OpenUserBar(pUser->m_szName, 100, 100);	// �ϴ� ��ä���.		
	}

	// PVP ���� �� ��� �غ� �Ϸ�ȴٸ�
	// ������ �� �ְ� ����� �ش�.
	// Start�� �� �ʰ� ���ְ�,
	CUserInterface::GetInstance()->IncPVPCount();	
	CUserInterface::GetInstance()->IncPVPCount();	

	CGuildWarStatusWnd::GetInstance()->SetMark_Start();

	SendPKMode(true);

	DisplayMessageAdd(g_Message[ETC_MESSAGE538].szMessage, 0xFF00FF1E); // "������ GG!!"
}

// =========
// ��� ����
// =========
void CmdEndMatchM2M(char* pMsg, DWORD dwLen)
{
	DSTC_END_MATCH_MAN2MAN * pPacket = (DSTC_END_MATCH_MAN2MAN *) pMsg;

	CUserInterface::GetInstance()->CloseUserBar();
	CUserInterface::GetInstance()->m_bMatchUserHPShowFlag = FALSE;
	CUserInterface::GetInstance()->m_bMatchUserHPShowEnable = FALSE;
	CUserInterface::GetInstance()->m_szMatchUserName[0] = '\0';
	CUserInterface::GetInstance()->m_pMatchUser = NULL;
	

	// ��� ������ �����ش�.
	g_pInputManager->ClearInput(INPUT_BUFFER_13);
	g_pInputManager->ClearInput(INPUT_BUFFER_14);
	
	g_MatchInfo.SetDisplayMatchFlag(FALSE);
	g_MatchInfo.Init();

//	CUser * pWinner = NULL, * pLoser = NULL;
	char	szBuf[255] = {0,};
	if (pPacket->bJudgmentCode == 0)//
	{ 
		// KO		 
		wsprintf(szBuf, g_Message[ETC_MESSAGE539].szMessage, // // "%s �԰� %s ���� ��ῡ�� %s �Բ��� KO���� �ŵ׽��ϴ�.",
						pPacket->szWinnerName, pPacket->szLoserName, pPacket->szWinnerName);
		DisplayMessageAdd(szBuf, 0xFF00FF1E);
	}
	else if (pPacket->bJudgmentCode == 1)
	{ 
		// ���� 
		wsprintf(szBuf, g_Message[ETC_MESSAGE540].szMessage, // "%s �԰� %s ���� ��ῡ�� %s �Բ��� �������� �ŵ׽��ϴ�."
						pPacket->szWinnerName, pPacket->szLoserName, pPacket->szWinnerName);
		DisplayMessageAdd(szBuf, 0xFF00FF1E);
	}
	else if (pPacket->bJudgmentCode == 2)
	{ 
		// ���º� 
		wsprintf(szBuf, g_Message[ETC_MESSAGE541].szMessage, // "%s �԰� %s ���� ��ῡ�� ���º� ���� �����ϴ�."
						pPacket->szWinnerName, pPacket->szLoserName, pPacket->szWinnerName);
		DisplayMessageAdd(szBuf, 0xFF00FF1E);
	}
	else if (pPacket->bJudgmentCode == 3)
	{
		// ������ ���� 
		wsprintf(szBuf, g_Message[ETC_MESSAGE542].szMessage, // "%s �԰� %s ���� ��ῡ�� %s �Բ��� �¸��ϼ̽��ϴ�.(������)"
			pPacket->szLoserName, pPacket->szWinnerName, pPacket->szWinnerName);
		DisplayMessageAdd(szBuf, 0xFF00FF1E);
	}

	if (!__strcmp(pPacket->szWinnerName, g_pMainPlayer->m_szName) )
	{ 
		// ���� ����ڰ� �¸� 
		if (2 != pPacket->bJudgmentCode)
		{ 
			// ���ºΰ� �ƴϴ�.
			g_pMainPlayer->m_dwMatchWin += 1;	
		}
	} 
	else
	if (!__strcmp(pPacket->szLoserName, g_pMainPlayer->m_szName) )	
	{ 
		// ���� ����ڰ� �й�
		if (2 != pPacket->bJudgmentCode)
		{ 
			// ���ºΰ� �ƴϴ�.
			g_pMainPlayer->m_dwMatchLose += 1;	
		}
	}

	CGuildWarStatusWnd::GetInstance()->SetActive(FALSE);
}


void  CmdMatchUserInfo(char* pMsg, DWORD dwLen)
{
	CUserInterface * pUserInterface = CUserInterface::GetInstance();
	DSTC_MATCH_USER_INFO * pPacket = (DSTC_MATCH_USER_INFO *)pMsg;
	
	CUser * pUser = g_pUserHash->GetData(pPacket->dwMatchUserIndex);
	if (NULL != pUser)
	{
		if (pUserInterface->m_pMatchUser == pUser)
		{
//			pUserInterface->OpenUserBar(pUser->m_szName, pPacket->dwHP, pPacket->dwMaxHP);//�� �۾����� 
//	sung-han 2004-12-30 : pvp ���� ĳ���� �������ٿ� ������Ż ������ �ٰ� ��ġ�� �κ� ó��
//-----------------------------------------------------------------------------------------
			pUserInterface->m_fUserHp		= (float)pPacket->dwHP;
			pUserInterface->m_fUserMaxHp	= (float)pPacket->dwMaxHP;
//-----------------------------------------------------------------------------------------
			
		}
	}
}