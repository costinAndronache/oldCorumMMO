#include "stdafx.h"
#include "WorldMsg_minbobo.h"
#include "InitGame.h"
#include "GameControl.h"
#include "message.h"
#include "WorldProcess.h"
#include "Chat.h"
#include "WorldWnd.h"
#include "Interface.h"
#include "UserInterface.h"
#include "GameNoticeWnd.h"
#include "CommandWnd.h"
#include "InputManager.h"
#include "ErrorWnd.h"
#include "NetworkClient.h"
#include "DUNGEON_DATA_EX.h"
#include "Map.h"
#include "GMTool.h"
#include "resource.h"
#include "CodeFun.h"
#include "Chat.h"


//==============================================================//
// ��ųŰ ���� �ϴ� ��Ŷ.										//
//==============================================================//
void CmdSkillKey( char* pMsg, DWORD dwLen )
{
	WSTC_SKILL_KEY* pPacket = (WSTC_SKILL_KEY*)pMsg;

	for(int i = 0; i < 16; i++)
	{
		if(pPacket->bSkillKey[i]==0)
			g_pMainPlayer->m_nSkillKey[i] = -1;	
		else
		{
			if(pPacket->bSkillKey[i]==__SKILL_ATTACK__)
				g_pMainPlayer->m_nSkillKey[i] = pPacket->bSkillKey[i];			
			else
			{
				if(g_pEffectLayer->m_Effect[pPacket->bSkillKey[i]].wProperty!=0)
				{				
					if(g_pEffectLayer->m_Effect[pPacket->bSkillKey[i]].bSkillType!=0)
						g_pMainPlayer->m_nSkillKey[i] = pPacket->bSkillKey[i];
					else
						g_pMainPlayer->m_nSkillKey[i] = -1;	
				}
				else
					g_pMainPlayer->m_nSkillKey[i] = -1;
			}
		}
		g_pMainPlayer->m_nPosSK[i] = pPacket->bPosKey[i];
	}
}

//==============================================================//
// �Ӹ� ���/�ź� ���� �ϴ� ��Ŷ.								//
//==============================================================//
void CmdCommandMsg(char* pMsg, DWORD dwLen)
{
	WSTC_COMMAND_MSG*	pPacket			= (WSTC_COMMAND_MSG*)pMsg;
	char				szInfo[0xff]	= {0,};
	
	if(pPacket->bCommand==TRUE)
		lstrcpy(szInfo, g_Message[ETC_MESSAGE413].szMessage);	// MSG_ID : 413 ; �Ӹ���� ���·� ����Ǿ����ϴ�.
	else
		lstrcpy(szInfo, g_Message[ETC_MESSAGE414].szMessage);	// MSG_ID : 414 ; �Ӹ��ź� ���·� ����Ǿ����ϴ�.

	DisplayMessageAdd(szInfo, 0xffff9a63, TRUE, DEF_CHAT_TYPE_WHISPER);

	CUserInterface::GetInstance()->m_bCommand = pPacket->bCommand;
}

//==============================================================//
// �������� �޴� ��Ŷ.											//
//==============================================================//
void CmdNotice(char* pMsg, DWORD dwLen)
{
	CTWS_GM_NOTICE*			pPacket		= (CTWS_GM_NOTICE*)pMsg;	
	CGameNoticeMessageWnd*	pNoticeWnd	= CGameNoticeMessageWnd::GetInstance();
	
	int		nCount			= 1;
	int		nSize			= 0;
	char*	szTemp			= 0;
	char	szChat[1024]	= {0,};

	memset(szChat, 0, 1024);
	memcpy(szChat, pPacket->szMsg, pPacket->wMsgLen);
	
	szTemp	= szChat;
	nSize	= pPacket->wMsgLen;

	if(0 == nSize)
		return;

	for(int iLoop = 0; iLoop < MAX_NOTICE_COUNT; iLoop++)
	{	
		memset(g_sNoticeMessage[iLoop], 0, MAX_NOTICE_SIZE);
	}
	
	// MSG_ID : 415 ; ��������
	lstrcpy(g_sNoticeMessage[0], g_Message[ETC_MESSAGE415].szMessage);	
	g_byLen[0] = (BYTE)lstrlen(g_sNoticeMessage[0]);	

	while(nSize > 0)
	{
		if(nSize > MAX_NOTICE_SIZE)
		{
			for(;;)
			{
				if(nSize > MAX_NOTICE_SIZE)
				{
					char* szPrev = CharPrevExA(0, szTemp, szTemp+MAX_NOTICE_SIZE, 0);
					nSize = lstrlen(szTemp) - lstrlen(szPrev);

					__lstrcpyn(g_sNoticeMessage[nCount], szTemp, nSize);
					g_byLen[nCount] = (BYTE)nSize;

					szTemp = szPrev;
					nCount++;
					nSize = lstrlen(szTemp);
				}
				else
				{
					// ���� ���ڿ��� ä�� ���ο� ����, ���̸� ����Ѵ�.
					lstrcpy(g_sNoticeMessage[nCount], szTemp);
					g_byLen[nCount] = (BYTE)lstrlen(szTemp);
					nCount++;								
					nSize = 0;
					break;
				}
			}
		}
		else
		{	
			memset(g_sNoticeMessage[nCount], 0, MAX_NOTICE_SIZE);
			__lstrcpyn(g_sNoticeMessage[nCount], szTemp, lstrlen(szTemp));	
			g_byLen[nCount] = (BYTE)lstrlen(g_sNoticeMessage[nCount]);
			nCount++;
			nSize = 0;
		}		
	}
	goto lb_Notice;
	
lb_Notice:

	g_byNoticeCount = (BYTE)nCount;
	g_bNoticeFlag	= TRUE;
	g_dwPrevTime	= timeGetTime();
	g_dwNextTime	= timeGetTime();

	if(GetGameStatus()==UPDATE_GAME_WORLD)
		g_bPosType = UPDATE_GAME_WORLD;
	else if(GetGameStatus()==UPDATE_GAME_PLAY)
		g_bPosType = UPDATE_GAME_PLAY;
		
	pNoticeWnd->SetPosition(420, 0);
	pNoticeWnd->SetActive(TRUE);
}


//==============================================================//
// ���� ���� ���� ���� �޴� ��Ŷ.								//
//==============================================================//
void CmdDungeonJoinFailed( char* pMsg, DWORD dwLen )
{
	WSTC_DUNGEON_JOIN_FAILED* pPacket = (WSTC_DUNGEON_JOIN_FAILED*)pMsg;

	g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_DUNGEON_JOIN_FAIL);
	// MSG_ID : 417 ; [Dungeon Join Failed!]
	g_pInputManager->InsertCharacter(INPUT_BUFFER_WORLD_DUNGEON_JOIN_FAIL,g_Message[ETC_MESSAGE417].szMessage
		, lstrlen(g_Message[ETC_MESSAGE417].szMessage)); 
		
	char szInfo[0xff]={0,};

	switch(pPacket->bFailCode)
	{
	case DUNGOEN_JOIN_FAIL_IMPOSSIBILITY_AREA:			
		{
			// "[World Error] ������ ��ҷδ� �̵��Ͻ� �� �����ϴ�."
			lstrcpy(szInfo, g_Message[ETC_MESSAGE1236].szMessage);
		}
		break;
	case DUNGEON_JOIN_FAIL_SERVER_IS_NOT_CONNECTED:		
		{
			//"[World Error] ���� ������ ���� ������ ���� �Ǿ� ���� �ʽ��ϴ�."
			lstrcpy(szInfo, g_Message[ETC_MESSAGE696].szMessage);
		}
		break;
	case DUNGEON_JOIN_FAIL_SERVER_INFO_IS_NOT_FOUND:		
		{
			//"[World Error] ���弭�� �������������� �����ϴ�."
			lstrcpy(szInfo, g_Message[ETC_MESSAGE697].szMessage);
		}
		break;
	case DUNGEON_JOIN_FAIL_USER_ACCEPT_FAIL:		
		{
			//"[World Error] ���������κ��� ������ �źδ��߽��ϴ�."
			lstrcpy(szInfo, g_Message[ETC_MESSAGE698].szMessage);
		}
		break;
	case DUNGEON_JOIN_FAIL_NOT_ENOUGH_ENTER_KARZ:		
		{			
			//"[World Error] ����� ����"
			lstrcpy(szInfo, g_Message[ETC_MESSAGE699].szMessage);	
		}break;
	case DUNGEON_JOIN_FAIL_UNKNOWN_DUNGEOIN_ID:		
		{
			//"[World Error] ���弭���� �ش� ���������� �����ϴ�!"
			lstrcpy(szInfo, g_Message[ETC_MESSAGE700].szMessage);
		}
		break;
	case DUNGEON_JOIN_FAIL_NOT_MINMAX_LEVEL:
		{
			// [World Error] ���尡���� ������ �ƴմϴ�.
			lstrcpy(szInfo, g_Message[ETC_MESSAGE1257].szMessage); 
		}break;
	case DUNGEON_JOIN_FAIL_EVENT_DUNGEON_OPEN_FULL:	
		{
			//"[World Error] �����ο� �ʰ�"
			lstrcpy(szInfo, g_Message[ETC_MESSAGE701].szMessage);
		}
		break;
	case DUNGEON_JOIN_FAIL_EVENT_DUNGEON_OPEN_TIME_UP:	
		{			
			//"[World Error] ���� ���� �ð� ���"
			lstrcpy(szInfo, g_Message[ETC_MESSAGE702].szMessage);
		}
		break;
	case DUNGEON_JOIN_FAIL_INVALID_EVENT_ENTER_LEVEL:	
		{
			//"[World Error] �̺�Ʈ ������ ������ �� ���� �����Դϴ�."
			lstrcpy(szInfo, g_Message[ETC_MESSAGE703].szMessage);		
		}
		break;
	default:	
		{
			//"[World Error] ���弭������ �������ο� �����߽��ϴ�! (Code:%d)"
			wsprintf(szInfo, g_Message[ETC_MESSAGE704].szMessage, pPacket->bFailCode);			
		}
		break;		
	}

	DisplayMessageAdd(szInfo, 0xFFFFC309);
	_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_Camera.v3CameraPos, FALSE);
	MoveToUserPrevPos();
}


//==============================================================//
// �������� ���� ��Ŷ.											//
//==============================================================//
void CmdConnectDungeonServer( char* pMsg, DWORD dwLen )
{
	WSTC_CONNECT_DUNGEON_SERVER*	pPacket		= (WSTC_CONNECT_DUNGEON_SERVER*)pMsg;
	CErrorWnd*						pErrorWnd	= CErrorWnd::GetInstance();
	char							szIP[ MAX_IP_LENGTH ]={0,};

	ChangeAddressType(pPacket->dwDungeonIP, szIP);	
	SendLoadingStatus(TRUE);

	if( !g_pNet->ConnectServer( szIP, pPacket->wPort, SERVER_INDEX_ZONE ) ) 
	{
		MoveToUserPrevPos();		
		
		//"�������� ���ӿ� �����߽��ϴ�."			
		//"�ش� ���������� ���� �� �� �����ϴ�."	
		//"����� �����ϼ���!"					
		pErrorWnd->SetError(g_Message[ETC_MESSAGE705].szMessage, g_Message[ETC_MESSAGE706].szMessage, g_Message[ETC_MESSAGE483].szMessage, 3);
		pErrorWnd->SetActive(TRUE);
		pErrorWnd->m_byType		= 0;						
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_Camera.v3CameraPos, FALSE);
		SendLoadingStatus(FALSE);
		return;
	}
	
	g_pMainPlayer->m_bReceivedConnectMsg = 1;
	SendLoadingStatus(FALSE);
	StopBGM();
	
	CTDS_DUNGEON_JOIN join;
	memset(join.szChrName, 0, sizeof(join.szChrName));		
	join.dwPartyID		= pPacket->dwPartyID;
	join.dwUserIndex	= g_pMainPlayer->m_dwUserIndex;
	join.wDungeonID		= (WORD)pPacket->wDungeonID;		
	join.bIsPortalJoin	= 0;

	if(g_pThisDungeon != NULL)
	{
		const bool cant_move = ((g_pThisDungeon->GetDungeonType() == DUNGEON_TYPE_EVENT) || (g_pThisDungeon->GetDungeonType() == DUNGEON_TYPE_MATCH_STATIDUM));

		join.wPrevDungeonID				= WORD(cant_move ? 0 : g_pThisDungeon->m_dwID);
		g_pMainPlayer->wPrevDungeonID	= WORD(cant_move ? 0 : g_pThisDungeon->m_dwID);
	}
			
	__lstrcpyn( join.szChrName, g_pMainPlayer->m_szName, MAX_CHARACTER_NAME_REAL_LENGTH + 1);	
	g_pNet->SendMsg( (char*)&join, join.GetPacketSize(), SERVER_INDEX_ZONE );
	
	InitWorldMessage();
	CCommandWnd* pCommandWnd = CCommandWnd::GetInstance();
	pCommandWnd->SetActive(FALSE);

#ifndef DEVELOP_MODE			
	if(VAR->bChatMode)
	{
		g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_CHAT);
		g_pInputManager->ClearInput(INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME);
		
		g_pInputManager->SetFocusInput(INPUT_BUFFER_WORLD_CHAT);
		g_nChatId			= 0;		
		VAR->bChatMode		= 0;
		::KillTimer(g_hMainWnd, 1);
	}	
#endif
}

//==============================================================//
// �Ӹ� ���� ��Ŷ.												//
//==============================================================//
void CmdCommandFail(char* pMsg, DWORD dwLen)
{
	char	szInfo[0xff]	= {0,};
	DWORD	dwColor			= 0xFFFFB894;

	WSTC_WSF_CHAT* pPacket = (WSTC_WSF_CHAT*)pMsg;

	if(pPacket->enCommand==COMMAND_WHISPER)
	{
		// MSG_ID : 423 ; %s ĳ���Ͱ� �����ϴ�. ĳ���� �̸��� �ٽ� Ȯ���� �ֽʽÿ�.
		wsprintf(szInfo, g_Message[ETC_MESSAGE423].szMessage, pPacket->szId);	
	}
	else if(pPacket->enCommand==COMMAND_MESSAGE)
	{
		if(pPacket->nFail==1)
		{
			// MSG_ID : 424 ; %s ĳ���Ͱ� �α��� �Ǿ� ���� �ʽ��ϴ�. ������ �����ϰڽ��ϴ�.
			wsprintf(szInfo, g_Message[ETC_MESSAGE424].szMessage, pPacket->szId); 
			dwColor = 0xffff00ff;
		}
		else if(pPacket->nFail==0)
		{
			// MSG_ID : 425 ; %s�� ���� ĳ�����Դϴ�. ĳ���Ϳ� �̸��� Ȯ���� �ֽʽÿ�.
			wsprintf(szInfo, g_Message[ETC_MESSAGE425].szMessage, pPacket->szId); 
		}
		else if(pPacket->nFail==2)
		{
			// MSG_ID : 426 ; %s ĳ���Ϳ� �������� �� ���� �������� �����ϴ�.
			wsprintf(szInfo, g_Message[ETC_MESSAGE426].szMessage, pPacket->szId);	
		}
	}

	DisplayMessageAdd(szInfo, dwColor, DEF_CHAT_TYPE_WHISPER);
}

//==============================================================//
// �Ӹ� ���� ��Ŷ.												//
//==============================================================//
void CmdWhisper(char* pMsg, DWORD dwLen)
{
	char szInfo[0xff]={0,};
	
	WSTC_CHAT_WHISPER* pPacket = (WSTC_CHAT_WHISPER*)pMsg;

	if(pPacket->nSuccess==1)	
	{
		// MSG_ID : 427 ; �ӼӸ� %s : %s
		wsprintf(szInfo, g_Message[ETC_MESSAGE427].szMessage, pPacket->szId, pPacket->szMessage);	 
	}
	else if(pPacket->nSuccess==2)	
	{
		// MSG_ID : 428 ; �ӼӸ��� %s�� ���߽��ϴ�.
		wsprintf(szInfo, g_Message[ETC_MESSAGE428].szMessage, pPacket->szId);	
	}
	
	DisplayMessageAdd(szInfo, 0xffff9a63, DEF_CHAT_TYPE_WHISPER);
}

//==============================================================//
// �ӼӸ� ���� ��Ŷ.											//
//==============================================================//
void CmdCommandMessage(char* pMsg, DWORD dwLen)
{
	char szInfo[0xff]={0,};
	
	WSTC_CHAT_COMMAND* pPacket = (WSTC_CHAT_COMMAND*)pMsg;

	if(pPacket->nSuccess==1)
		wsprintf(szInfo, g_Message[ETC_MESSAGE429].szMessage, pPacket->szId, pPacket->szMessage);		// MSG_ID : 429 ; ���� %s : %s
	else if(pPacket->nSuccess==2)
		wsprintf(szInfo, g_Message[ETC_MESSAGE430].szMessage, pPacket->szId);	// MSG_ID : 430 ; ������ %s �Կ��� ���� �Ͽ����ϴ�.
	else if(pPacket->nSuccess==0)
		wsprintf(szInfo, g_Message[ETC_MESSAGE267].szMessage, pPacket->szId);	// "%s ���� �Ӹ� �ź� �����Դϴ�."
	
	DisplayMessageAdd(szInfo, 0xffff9a63, DEF_CHAT_TYPE_WHISPER);	
}

//==============================================================//
// �α׾ƿ��� ���� ���� ó�� ��Ŷ.								//
//==============================================================//
void CmdCommandMessageRQ(char* pMsg, DWORD dwLen)
{
	char			szInfo[0xff]={0,};
	WSTC_CHAT_RQ*	pPacket = (WSTC_CHAT_RQ*)pMsg;

	// �߰� //	
	if(pPacket->nCount!=0)
	{		
		__lstrcpyn(g_sMessageRQ[g_nWrIndex].szId, pPacket->szId, MAX_CHARACTER_NAME_REAL_LENGTH);
		__lstrcpyn(g_sMessageRQ[g_nWrIndex].szMessage, pPacket->szMessage, sizeof(pPacket->szMessage));

		memset(g_sMessageRQ[g_nWrIndex].szTime, 0, sizeof(g_sMessageRQ[g_nWrIndex].szTime));
		wsprintf(g_sMessageRQ[g_nWrIndex].szTime, g_Message[ETC_MESSAGE871].szMessage, // "%d�� %d�� %d�� %d�� %d�� %d��"
			pPacket->wdTime[0], pPacket->wdTime[1], pPacket->wdTime[2], pPacket->wdTime[3], pPacket->wdTime[4], pPacket->wdTime[5]);

		g_nWrIndex++;

		if(pPacket->nIndex==pPacket->nCount-1)
		{
			// Default Windowds ��� //	
			CCommandWnd* pCommandWnd = CCommandWnd::GetInstance();
			pCommandWnd->SetActive();
			pCommandWnd->SetSize(pPacket->nCount*2);

			for(int i = 0; i <  pPacket->nCount; i++)
			{				
				lstrcpy(szInfo, g_sMessageRQ[i].szTime);
				pCommandWnd->MessageAdd(szInfo, 0xffffffff);
				wsprintf(szInfo, "%s : %s",  g_sMessageRQ[i].szId, g_sMessageRQ[i].szMessage);
				pCommandWnd->MessageAdd(szInfo, 0xffffffff);
			}
			pCommandWnd->m_bMessageChk = TRUE;			
		}		
	}	
}

//==============================================================//
// ä�� ó�� ��Ŷ.												//
//==============================================================//
void CmdChatUser(char* pMsg, DWORD dwLen)
{
	char szInfo[0xff]={0,};
	WSTC_CHAT_USER* pPacket = (WSTC_CHAT_USER*)pMsg;
	wsprintf(szInfo, "%s : %s", pPacket->szId, pPacket->szMessage);

	if(GetGameStatus()==UPDATE_GAME_WORLD)
	{
		if(pPacket->nIndex==1)
			DisplayMessageAdd(szInfo, 0xFFFFFFFF);
		else if(pPacket->nIndex==2)
			DisplayMessageAdd(szInfo, 0xff00ffff);
	}
	else
	{
		if(pPacket->nIndex==1)
			DisplayMessageAdd(szInfo, 0xFFFFFFFF);
		else if(pPacket->nIndex==2)
			DisplayMessageAdd(szInfo, 0xff00ffff);
	}	
}

//==============================================================//
// ��Ƽ ���� ����󿡼� �����ִ� ó�� ��Ŷ.						//
//==============================================================//
void CmdWorldAppear(char* pMsg, DWORD dwLen)
{
	if(GetGameStatus() != UPDATE_GAME_WORLD)	
	{
		asm_int3();
		return;
	}
	
	WSTC_WORLD_APPEAR*	pPacket		= (WSTC_WORLD_APPEAR*)pMsg;
	LP_PARTY_USER		pPartyUser	= g_pPartyUserHash->GetData( pPacket->AppearInfo.dwCharIndex );
	
	if(!pPartyUser)
		return;

	InitWorldPlayer( &pPacket->AppearInfo, pPartyUser );	
}

//==============================================================//
// ���� ���� �����ִ� ó�� ��Ŷ.								//
//==============================================================//
void CmdBillingInfo(char* pMsg, DWORD dwLen)
{
	WSTC_BILLING_INFO*	pPacket		= (WSTC_BILLING_INFO*)pMsg;
	CWorldWnd*			pWorldWnd	= CWorldWnd::GetInstance();

	memset(pWorldWnd->szMessageLine, 0, sizeof(pWorldWnd->szMessageLine));
	
	pWorldWnd->m_bInfoType = pPacket->bInfoType;

	char szBillingType[16];
	memset(szBillingType, 0, 16);

	switch(pPacket->bCurBillingType)
	{
	case USER_BILLING_METER_RATE:		__lstrcpyn(szBillingType, g_Message[ETC_MESSAGE824].szMessage, 15);	break;	// "����������"
	case USER_BILLING_FLAT_RATE:		__lstrcpyn(szBillingType, g_Message[ETC_MESSAGE825].szMessage, 15);	break;	// "����������"
	case USER_BILLING_1HOUR_FREE:		__lstrcpyn(szBillingType, g_Message[ETC_MESSAGE826].szMessage, 15);	break;	// "�ѽð�����"
	case USER_BILLING_ABSOLUTELY_FREE:	__lstrcpyn(szBillingType, g_Message[ETC_MESSAGE827].szMessage, 15);	break;	// "�������"
#if IS_TAIWAN_LOCALIZING()
	case PCROOM_BILLING_SOMEHOUR_FREE:	__lstrcpyn(szBillingType, g_Message[ETC_MESSAGE829].szMessage, 15);	break;	// "PC��������"
#endif		
	case PCROOM_BILLING_METER_RATE:		__lstrcpyn(szBillingType, g_Message[ETC_MESSAGE828].szMessage, 15);	break;	// "PC��������"
	case PCROOM_BILLING_FLAT_RATE:		__lstrcpyn(szBillingType, g_Message[ETC_MESSAGE829].szMessage, 15);	break;	// "PC��������"
	}

	wsprintf(pWorldWnd->szMessageLine[0], g_Message[ETC_MESSAGE814].szMessage, g_pMainPlayer->m_szName, szBillingType); // "%s���� %s ���Դϴ�."

	int nHour, nMinute, nSec;
	
	if(pPacket->bInfoType == BILLING_INFO_SMALL_TIME)
	{
		nHour = pPacket->dwRemainTime / 3600;
		nMinute = ((pPacket->dwRemainTime % 3600) / 60)%60;
		nSec = (pPacket->dwRemainTime - (3600*nHour + 60*nMinute))%60;
		
		lstrcpy(pWorldWnd->szMessageLine[1],  g_Message[ETC_MESSAGE815].szMessage); // "������ �÷��̰����� �ܿ� �ð���"
		
		if(!nHour)
			wsprintf(pWorldWnd->szMessageLine[2], g_Message[ETC_MESSAGE816].szMessage, nMinute, nSec); // "%d�� %d���Դϴ�."
		else
			wsprintf(pWorldWnd->szMessageLine[2], g_Message[ETC_MESSAGE817].szMessage, nHour, nMinute, nSec); // "%d�ð� %d�� %d���Դϴ�."

		wsprintf(pWorldWnd->szMessageLine[3], g_Message[ETC_MESSAGE818].szMessage, szBillingType); // "����� %s �÷��̽ð��� ����"
		
		lstrcpy(pWorldWnd->szMessageLine[4], g_Message[ETC_MESSAGE819].szMessage); // "�˴ϴ�. �ٸ� ���������� ������ ��"
		
		lstrcpy(pWorldWnd->szMessageLine[5], g_Message[ETC_MESSAGE820].szMessage); // "�Ե鲲���� ����ؼ� ������ ���� ��"
		
		lstrcpy(pWorldWnd->szMessageLine[6], g_Message[ETC_MESSAGE821].szMessage);	// "�ֽ��ϴ�."
	}
	else if(pPacket->bInfoType == BILLING_INFO_START_BILLING)
	{
		if(pPacket->bCurBillingType == USER_BILLING_METER_RATE || pPacket->bCurBillingType == PCROOM_BILLING_METER_RATE
			|| pPacket->bCurBillingType == USER_BILLING_1HOUR_FREE)
		{
			nHour = pPacket->dwRemainTime / 3600;
			nMinute = ((pPacket->dwRemainTime % 3600) / 60)%60;
			nSec = (pPacket->dwRemainTime - (3600*nHour + 60*nMinute))%60;
		
			lstrcpy(pWorldWnd->szMessageLine[1], g_Message[ETC_MESSAGE815].szMessage ); // "������ �÷��̰����� �ܿ� �ð���"

			if(!nHour)
				wsprintf(pWorldWnd->szMessageLine[2], g_Message[ETC_MESSAGE816].szMessage, nMinute, nSec); // "%d�� %d���Դϴ�."
			else
				wsprintf(pWorldWnd->szMessageLine[2], g_Message[ETC_MESSAGE817].szMessage, nHour, nMinute, nSec); // "%d�ð� %d�� %d���Դϴ�."
		}
		else if(pPacket->bCurBillingType == USER_BILLING_FLAT_RATE || pPacket->bCurBillingType == PCROOM_BILLING_FLAT_RATE)
		{
			wsprintf(pWorldWnd->szMessageLine[1], g_Message[ETC_MESSAGE822].szMessage, g_pMainPlayer->m_szName); // "%s ������ �����Ⱓ��"
			wsprintf(pWorldWnd->szMessageLine[2], g_Message[ETC_MESSAGE823].szMessage, // "%d�� %d�� %d�ϱ��� �Դϴ�."
				pPacket->BillingEndDate.year, pPacket->BillingEndDate.month, pPacket->BillingEndDate.day);
		}
	}
	
	pWorldWnd->m_bInfoType = 0;
	pWorldWnd->SetActive();
}

//==============================================================//
// ��Ƽ ���� ����󿡼� �����ִ� ó�� ��Ŷ.						//
//==============================================================//
void CmdWorldDisappear(char* pMsg, DWORD dwLen)
{
	if(GetGameStatus() != UPDATE_GAME_WORLD)	
	{
		asm_int3();
		return;
	}

	WSTC_WORLD_DISAPPEAR*	pPacket		= (WSTC_WORLD_DISAPPEAR*)pMsg;
	LP_PARTY_USER			pPartyUser	= g_pPartyUserHash->GetData( pPacket->dwCharIndex );
	
	if(!pPartyUser)
		return;

	FreeWorldPartyUser( pPartyUser );	
}

void CmdAirshipRiding(char* pMsg, DWORD dwLen)
{
	CTC_AIRSHIP_RIDING* pPacket = (CTC_AIRSHIP_RIDING*)pMsg;

	LP_PARTY_USER pPartyUser = g_pPartyUserHash->GetData( pPacket->dwCharIndex );
	
	if(!pPartyUser)
		return;

	pPartyUser->SetStatus(UNIT_STATUS_RIDE_AIRPLANE_DOWN);
	pPartyUser->m_hPlayer.pDesc->ObjectFunc = NULL;

	HideObject(pPartyUser->m_hPlayer.pHandle);
}

void CmdAirshipGetoff(char* pMsg, DWORD dwLen)
{
	CTC_AIRSHIP_GETOFF* pPacket = (CTC_AIRSHIP_GETOFF*)pMsg;

	LP_PARTY_USER pPartyUser = g_pPartyUserHash->GetData( pPacket->dwCharIndex );
	
	if(!pPartyUser)
		return;

	pPartyUser->SetStatus(UNIT_STATUS_NORMAL);
	pPartyUser->m_hPlayer.pDesc->ObjectFunc = NULL;

	GXSetPosition(pPartyUser->m_hPlayer.pHandle, &pPacket->v3Pos, FALSE);
	ShowObject(pPartyUser->m_hPlayer.pHandle);
	SetActionForPartyUser( pPartyUser->m_hPlayer.pHandle, MOTION_TYPE_VILLAGESTAND, 0, ACTION_LOOP );
}

void CmdCheckConnection(char* pMsg, DWORD dwLen)
{
	g_pNet->SendMsg(pMsg, dwLen, SERVER_INDEX_WORLD);
}

void CmdWorldPartyUserInfo(char* pMsg, DWORD dwLen)
{
	if(GetGameStatus() != UPDATE_GAME_WORLD)	
	{
		asm_int3();
		return;
	}

	WSTC_WORLD_PARTY_USER_INFO* pPacket = (WSTC_WORLD_PARTY_USER_INFO*)pMsg;

	for(int i=0; i<pPacket->bUserCount; i++)
	{
		LP_PARTY_USER pPartyUser = g_pPartyUserHash->GetData( pPacket->AppearInfo[i].dwCharIndex );
	
		if(!pPartyUser)	
		{
			asm_int3();
			continue;
		}
		InitWorldPlayer( &pPacket->AppearInfo[i], pPartyUser );
	}
}

void CmdWorldSetPosition(char* pMsg, DWORD dwLen)
{
	if(GetGameStatus() != UPDATE_GAME_WORLD)	
	{
		asm_int3();
		return;
	}

	CTC_MOVE_POSITION*	 pPacket = (CTC_MOVE_POSITION*)pMsg;

	if( g_pMainPlayer->m_dwUserIndex == pPacket->dwCharIndex )		// �� �ڽſ��� ���ڽ��� �̵� �޽����� �Դٸ�
		return;

	LP_PARTY_USER pUser = g_pPartyUserHash->GetData( pPacket->dwCharIndex );

	if(pUser)
	{
		SetWorldUserPosition( pUser, &pPacket->v3Pos );
		SetActionForPartyUser( pUser->m_hPlayer.pHandle, MOTION_TYPE_VILLAGESTAND, 0, ACTION_LOOP );
		pUser->SetStatus( UNIT_STATUS_NORMAL );
		pUser->m_hPlayer.pDesc->ObjectFunc	= NULL;
		g_pExecutive->GXOSetDirection(pUser->m_hPlayer.pHandle, &g_Camera.v3AxsiY, pPacket->fDirection);
		
		//�Ⱥ��̴� �������´� ������ ���̰� 
		ShowObject(pUser->m_hPlayer.pHandle);
	}
}

void CmdPartyUserTryToConnect(char* pMsg, DWORD dwLen)
{
	CTC_MOVE_POSITION*	 pPacket = (CTC_MOVE_POSITION*)pMsg;

	if( g_pMainPlayer->m_dwUserIndex == pPacket->dwCharIndex )		// �� �ڽſ��� ���ڽ��� �̵� �޽����� �Դٸ�
		return;

	LP_PARTY_USER pUser = g_pPartyUserHash->GetData( pPacket->dwCharIndex );

	if(pUser)
	{
		SetWorldUserPosition( pUser, &pPacket->v3Pos );
		SetActionForPartyUser( pUser->m_hPlayer.pHandle, MOTION_TYPE_VILLAGESTAND, 0, ACTION_LOOP );
		pUser->SetStatus( UNIT_STATUS_NORMAL );
		pUser->m_hPlayer.pDesc->ObjectFunc	= NULL;
		
		//�켱 ������ ������� 
		HideObject(pUser->m_hPlayer.pHandle);	
	}
}

void CmdWorldMove(char* pMsg, DWORD dwLen)
{
	if(GetGameStatus() != UPDATE_GAME_WORLD)
		return;
	
	CTC_WORLD_MOVE* pPacket = (CTC_WORLD_MOVE*)pMsg;

	if( g_pMainPlayer->m_dwUserIndex == pPacket->dwCharIndex )		// �� �ڽſ��� ���ڽ��� �̵� �޽����� �Դٸ�
		return;

	CInterface* pInterface = CInterface::GetInstance();

	if(pInterface->GetClick())
		return;
	
	LP_PARTY_USER pUser = g_pPartyUserHash->GetData( pPacket->dwCharIndex );

	if(pUser)
	{
		int nFrame = 0;	// �Ȱų� �޸��� �ִ� ���� ���ٸ� �������� �ε巴�� �̿��ֱ� ����...
		
		if( pUser->GetStatus() == UNIT_STATUS_WALKING || pUser->GetStatus() == UNIT_STATUS_RUNNING ) 
			nFrame = g_pExecutive->GXOGetCurrentFrame( pUser->m_hPlayer.pHandle );

		switch( pPacket->bMoveType )
		{
		case UNIT_STATUS_WALKING:
			SetActionForPartyUser( pUser->m_hPlayer.pHandle, MOTION_TYPE_WALK, nFrame, ACTION_LOOP );
			break;
		case UNIT_STATUS_RUNNING:
			SetActionForPartyUser( pUser->m_hPlayer.pHandle, MOTION_TYPE_RUN, nFrame, ACTION_LOOP );						
			break;
		}

		pUser->m_hPlayer.pDesc->ObjectFunc	= PlayerWorldMoveFunc;
		pUser->m_hPlayer.pDesc->dwStartTick	= g_dwCurTick;
	
		SetWorldUserPosition( pUser, &pPacket->v3MoveStartPos );
	
		// �̵� ������ �Է��ϰ�
		pUser->SetStatus(pPacket->bMoveType);
		pUser->m_v3Direction	= pPacket->v3MoveDirection;
		pUser->m_pDestTile		= g_pMap->GetMap( pPacket->wDestX, pPacket->wDestZ );		
		g_pExecutive->GXOSetDirection( pUser->m_hPlayer.pHandle, &g_Camera.v3AxsiY, (float)(atan2(pPacket->v3MoveDirection.z, pPacket->v3MoveDirection.x) + DEG90 ) );		
	}
	else
	{
		asm_int3();
	}
}

void CmdWorldStop(char* pMsg, DWORD dwLen)
{
	if(GetGameStatus() != UPDATE_GAME_WORLD)	return;

	CTC_WORLD_STOP* pPacket = (CTC_WORLD_STOP*)pMsg;

	if( g_pMainPlayer->m_dwUserIndex == pPacket->dwCharIndex )		// �� �ڽſ��� ���ڽ��� �̵� �޽����� �Դٸ�
		return;

	LP_PARTY_USER pUser = g_pPartyUserHash->GetData( pPacket->dwCharIndex );

	if(pUser)
	{
		SetWorldUserPosition( pUser, &pPacket->v3StopPos );
		SetActionForPartyUser( pUser->m_hPlayer.pHandle, MOTION_TYPE_VILLAGESTAND, 0, ACTION_LOOP );
		pUser->SetStatus( UNIT_STATUS_NORMAL );
		pUser->m_hPlayer.pDesc->ObjectFunc	= NULL;		
	}
}

void CmdInvalidGM( char* pMsg, DWORD dwLen )
{
	if (g_hGMToolTab1)
	{
		ShowWindow(g_hGMToolTab1,SW_HIDE);
	}

	g_bGM = FALSE;

	g_pMainPlayer->m_bUserType = USER_TYPE_NORMAL;
}

void CmdGMLoginSuccess( char* pMsg, DWORD dwLen )
{
	RECT rect;
	GetWindowRect(g_hMainWnd, &rect);
	SetWindowPos(g_hMainWnd, HWND_TOP, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOSIZE | SWP_SHOWWINDOW);

	if (!g_hGMToolDlg)
	{
		g_hGMToolDlg = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_GMTOOL), g_hMainWnd, GMDialog);
	}
	
	if (!g_hGMToolTab1)
	{
		g_hGMToolTab1 = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_GMTOOL_TAB1), g_hGMToolDlg, GMToolTab1);
	}
	
	ShowWindow(g_hGMToolTab1, SW_SHOW);

	if(g_pGVGm)
	{
		delete g_pGVGm;
		g_pGVGm = NULL;
	}

	g_pGVGm = new GLOBAL_VARIABLE_GM;
	memset(g_pGVGm, 0, sizeof(GLOBAL_VARIABLE_GM));

	g_bGM = TRUE;

	g_pMainPlayer->m_bUserType = USER_TYPE_GM;
}

void CmdGmCommandResult( char* pMsg, DWORD dwLen )
{
	WSTC_GM_COMMAND_RESULT*	pResult = (WSTC_GM_COMMAND_RESULT*)pMsg;
	pResult;
}

void CmdCBMessage( char* pMsg, DWORD dwLen )
{
	WSTC_CB_MESSAGE* pPacket = (WSTC_CB_MESSAGE*)pMsg;

	switch(pPacket->byMessageType)
	{
	case 1:
		WorldMessageAdd(g_Message[ETC_MESSAGE1242].szMessage, 0xffff0000);
		break;
	case 2:
		WorldMessageAdd(g_Message[ETC_MESSAGE1243].szMessage, 0xffff0000);
		break;
	}
}