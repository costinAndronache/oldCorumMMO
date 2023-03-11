//==============================================================//
// Code by Jang.									2003.08.01	//
//==============================================================//
#include "Chat.h"
#include "GameControl.h"
#include "../CommonServer/CommonHeader.h"
#include "User.h"
#include "Message.h"
#include "HashTable.h"
#include "Interface.h"
#include "GroupWnd.h"
#include "DungeonProcess.h"
#include "ChatListWnd.h"
#include "ChatWnd.h"
#include "GlobalSoundHandles.h"
#include "InitGame.h"
#include "ChatBackground.h"
#include "CodeFun.h"
#include "InputManager.h"
#include "WorldProcess.h"
#include "GameMenuWnd.h"

extern int			g_nDgIndex;
extern int			g_nDgCount;
extern SDGCHATMSG	g_sDgChatMsg[__MAX_QUEUE__];
//==============================================================//
// �������� �޼��� �ʱ�ȭ.										//
//==============================================================//
void InitDungeonMessage()
{
	for(int i = 0; i < __MAX_QUEUE__; i++)
	{
		memset(g_sDgChatMsg[i].szDungeon, 0, sizeof(g_sDgChatMsg[i].szDungeon));		
		g_sDgChatMsg[i].sTimeIndex		= 0;
		g_sDgChatMsg[i].sdwColor		= 0xffffffff;
		g_sDgChatMsg[i].sbActive		= FALSE;
		g_sDgChatMsg[i].snSize			= 0;
	}
	
	for(i = 0; i < __MAX_CHAT_LIST_QUEUE__; i++)
	{
		memset(g_sDgChatListMsg[i].szMessage, 0, sizeof(g_sDgChatListMsg[i].szMessage));
		g_sDgChatListMsg[i].snSize		= 0;
		g_sDgChatListMsg[i].sdwColor	= 0xffffffff;
	}
	g_nDgChatListIndex	= 0;
	g_nDgIndex			= 0;
	g_nDgChatListStart	= 0;
	g_nDgChatListEnd	= 0;
}
//==============================================================//
// �������� �޼��� �߰�.										//
//==============================================================//
int DungeonMessageAdd(char* pszMessage, DWORD dwCol, BOOL bChkSystemMsg, BYTE byType)
{			
	ChatListAdd(pszMessage, dwCol, byType, bChkSystemMsg);	
	return 2;
}
//==============================================================//
// �������� �޼��� �߰�.										//
//==============================================================//
int MessageAdd(char* pszMessage, DWORD dwCol, BOOL bChkSystemMsg, int nSize)
{
	// Trash String Attached by [strncpy] function. So, [strncpy] change to [__lstrcpyn]
	// [strcpy] change to [lstrcpy]
	// 2004. 08. 23.
	// Begin - minjin

//	CChatListWnd* pChatListWnd = CChatListWnd::GetInstance();

	if(bChkSystemMsg)// && pChatListWnd->GetActive())
	{
		// ä�� ����Ʈ �޼��� //
		if(g_nDgChatListIndex<__MAX_CHAT_LIST_QUEUE__)
		{
			g_sDgChatListMsg[g_nDgChatListIndex].sdwColor	= dwCol;
			
			// memory init -_- by deepdark.
		//	ZeroMemory(g_sDgChatListMsg[g_nDgChatListIndex].szMessage, sizeof(g_sDgChatListMsg[g_nDgChatListIndex].szMessage));
			
			if(nSize)
			{
//				strncat(g_sDgChatListMsg[g_nDgChatListIndex].szMessage, "\0", 1);
				__lstrcpyn(g_sDgChatListMsg[g_nDgChatListIndex].szMessage, pszMessage, nSize);
			}
			else
			{
//				::MessageBox(g_hMainWnd, "nSize is ZERO", "ERROR", MB_OK);
//				strcpy(g_sDgChatListMsg[g_nDgChatListIndex].szMessage, pszMessage);
				lstrcpy(g_sDgChatListMsg[g_nDgChatListIndex].szMessage, pszMessage);
			}

			g_sDgChatListMsg[g_nDgChatListIndex].snSize = lstrlen(g_sDgChatListMsg[g_nDgChatListIndex].szMessage);
			g_nDgChatListIndex++;
			g_nDgChatListEnd++;
		}	
		else
		{
			if(g_nDgChatListStart==__MAX_CHAT_LIST_QUEUE__-1)
				g_nDgChatListStart = 0;
			else
				g_nDgChatListStart++;

			if(g_nDgChatListEnd>=__MAX_CHAT_LIST_QUEUE__-1)
				g_nDgChatListEnd = 0;
			else
				g_nDgChatListEnd++;

			g_sDgChatListMsg[g_nDgChatListEnd].sdwColor	= dwCol;

			// memory init -_- by deepdark.
		//	ZeroMemory(g_sDgChatListMsg[g_nDgChatListIndex].szMessage, sizeof(g_sDgChatListMsg[g_nDgChatListIndex].szMessage));

			if(nSize)
			{
//				strncat(g_sDgChatListMsg[g_nDgChatListEnd].szMessage, "\0", 1);
				__lstrcpyn(g_sDgChatListMsg[g_nDgChatListEnd].szMessage, pszMessage, nSize);
			}
			else
			{
//				::MessageBox(g_hMainWnd, "nSize is ZERO", "ERROR", MB_OK);
//				strcpy(g_sDgChatListMsg[g_nDgChatListEnd].szMessage, pszMessage);
				lstrcpy(g_sDgChatListMsg[g_nDgChatListEnd].szMessage, pszMessage);
			}

			g_sDgChatListMsg[g_nDgChatListEnd].snSize = lstrlen(g_sDgChatListMsg[g_nDgChatListEnd].szMessage);
		}
		// Active == FALSE
		for(int i = g_nDgIndex; i < __MAX_QUEUE__; i++)
		{
			if(g_sDgChatMsg[i].sbActive==FALSE)
			{
				// memory init -_- by deepdark.
			//	ZeroMemory(g_sDgChatListMsg[g_nDgChatListIndex].szMessage, sizeof(g_sDgChatListMsg[g_nDgChatListIndex].szMessage));

				if(nSize)
				{
//					strncat(g_sDgChatMsg[i].szDungeon, "\0", 1);
					__lstrcpyn(g_sDgChatMsg[i].szDungeon, pszMessage, nSize);
				}
				else
				{
//					::MessageBox(g_hMainWnd, "nSize is ZERO", "ERROR", MB_OK);
//					strcpy(g_sDgChatMsg[i].szDungeon, pszMessage);
					lstrcpy(g_sDgChatMsg[i].szDungeon, pszMessage);
				}

				g_sDgChatMsg[i].sTimeIndex	= 0;		
				g_sDgChatMsg[i].sdwColor	= dwCol;
				g_sDgChatMsg[i].sbActive	= TRUE;
				g_sDgChatMsg[i].snSize		= lstrlen(pszMessage);
				
				if(g_nDgIndex<__MAX_QUEUE__-1)
					g_nDgIndex++;
				else
					g_nDgIndex = 0;

				if(g_nDgCount<__MAX_QUEUE__)
					g_nDgCount++;

				return 1;
			}		
		}
		for(i = 0; i < g_nDgIndex; i++)
		{
			if(g_sDgChatMsg[i].sbActive==FALSE)
			{
				// memory init -_- by deepdark.
			//	ZeroMemory(g_sDgChatListMsg[g_nDgChatListIndex].szMessage, sizeof(g_sDgChatListMsg[g_nDgChatListIndex].szMessage));

				if(nSize)
				{
//					strncat(g_sDgChatMsg[i].szDungeon, "\0", 1);
					__lstrcpyn(g_sDgChatMsg[i].szDungeon, pszMessage, nSize);
				}
				else
				{
//					::MessageBox(g_hMainWnd, "nSize is ZERO", "ERROR", MB_OK);
//					lstrcpy(g_sDgChatMsg[i].szDungeon, pszMessage);
					lstrcpy(g_sDgChatMsg[i].szDungeon, pszMessage);
				}

				g_sDgChatMsg[i].sTimeIndex	= 0;		
				g_sDgChatMsg[i].sdwColor		= dwCol;
				g_sDgChatMsg[i].sbActive		= TRUE;
				g_sDgChatMsg[i].snSize			= lstrlen(pszMessage);
				
				if(g_nDgIndex<__MAX_QUEUE__-1)
					g_nDgIndex++;
				else
					g_nDgIndex = 0;

				if(g_nDgCount<__MAX_QUEUE__)
					g_nDgCount++;

				return 1;
			}
		}
		// Non Active == FALSE
		
		// memory init -_- by deepdark.
	//	ZeroMemory(g_sDgChatListMsg[g_nDgChatListIndex].szMessage, sizeof(g_sDgChatListMsg[g_nDgChatListIndex].szMessage));

		if(nSize)
		{
//			strncat(g_sDgChatMsg[g_nDgIndex].szDungeon, "\0", 1);
			__lstrcpyn(g_sDgChatMsg[g_nDgIndex].szDungeon, pszMessage, nSize);
		}
		else
		{
//			::MessageBox(g_hMainWnd, "nSize is ZERO", "ERROR", MB_OK);
//			lstrcpy(g_sDgChatMsg[i].szDungeon, pszMessage);
			lstrcpy(g_sDgChatMsg[i].szDungeon, pszMessage);
		}

		g_sDgChatMsg[g_nDgIndex].sTimeIndex	= 0;
		g_sDgChatMsg[g_nDgIndex].sdwColor	= dwCol;
		g_sDgChatMsg[g_nDgIndex].sbActive	= TRUE;
		g_sDgChatMsg[g_nDgIndex].snSize		= lstrlen(pszMessage);
		
		if(g_nDgIndex<__MAX_QUEUE__-1)
			g_nDgIndex++;
		else
			g_nDgIndex = 0;

		if(g_nDgCount<__MAX_QUEUE__)
			g_nDgCount++;
	}

	// End - minjin

	return 1;
}
//==============================================================//
// �������� �޼��� ó��.										//
//==============================================================//
void ProcessMessgae()
{
	CGameMenuWnd* pGameMenuWnd = CGameMenuWnd::GetInstance();
	for(int i = 0; i < __MAX_QUEUE__; i++)
	{
		if(g_sDgChatMsg[i].sbActive==TRUE)
		{
			g_sDgChatMsg[i].sTimeIndex++;

			//if(g_sDgChatMsg[i].sTimeIndex>600)     //(����â 2005.03.03) -  ä��ť(�����ϴܿ� ä�ñ� �ö󰡴� �κ�) �ð� ���� �κ� 
			if(g_sDgChatMsg[i].sTimeIndex > pGameMenuWnd->m_byChatRemainTime * 510)// sung-han 2005-03-30 ���ä�� ǥ�� ������� �ð�
			{
				g_sDgChatMsg[i].sbActive = FALSE;
				g_nDgCount--;
			}
		}
	}
}
//==============================================================//
// ���� ������ �������� ��� ó��.								//
//==============================================================//
void CmdDGFail(char* pMsg, DWORD dwLen)
{
//	char*	szTemp	= 0;
//	int		nSize	= 0;
	char	szInfo[0xff] = {0,};
	DWORD	dwColor = 0xffff9a63;	

 	WSTC_DSF_CHAT* pPacket = (WSTC_DSF_CHAT*)pMsg;

	if(pPacket->enCommand==COMMAND_WHISPER)
		wsprintf(szInfo, g_Message[ETC_MESSAGE94].szMessage, pPacket->szId);			// MSG_ID : 94 ; %s ĳ���Ͱ� �����ϴ�. ĳ���� �̸��� �ٽ� Ȯ���� �ֽʽÿ�.
	else if(pPacket->enCommand==COMMAND_MESSAGE)
	{
		if(pPacket->nFail==1)
		{			
			wsprintf(szInfo, g_Message[ETC_MESSAGE681].szMessage, pPacket->szId);	//"%s ĳ���Ͱ� �α��� �Ǿ����� �ʽ��ϴ�.", 
			DisplayMessageAdd(szInfo, dwColor, TRUE, DEF_CHAT_TYPE_WHISPER);
			lstrcpy(szInfo, g_Message[ETC_MESSAGE682].szMessage);					//"������ �����ϰڽ��ϴ�.");			
		}
		else if(pPacket->nFail==0)
		{
			wsprintf(szInfo, g_Message[ETC_MESSAGE683].szMessage, pPacket->szId);	//"%s ĳ���Ͱ� ��� ������ ������ �� �����ϴ�.", 			
			DisplayMessageAdd(szInfo, dwColor, TRUE, DEF_CHAT_TYPE_WHISPER);
			lstrcpy(szInfo, g_Message[ETC_MESSAGE684].szMessage);					// "ĳ���� �̸��� �ٽ� Ȯ���� �ֽʽÿ�.");			 
		}
		else if(pPacket->nFail==2)
			wsprintf(szInfo, g_Message[ETC_MESSAGE426].szMessage, pPacket->szId);	// MSG_ID : 97 ; %s ĳ���Ϳ� �������� �� ���� �������� �����ϴ�.
	}		
	DisplayMessageAdd(szInfo, dwColor, TRUE, DEF_CHAT_TYPE_WHISPER);	
	_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
}
//==============================================================//
// �ӼӸ� ó��.													//
//==============================================================//
void CmdDGWhisper(char* pMsg, DWORD dwLen)
{	
	char szInfo[0xff] = {0,};
	
	WSTC_DSCHAT_WHISPER* pPacket = (WSTC_DSCHAT_WHISPER*)pMsg;

	if(pPacket->nSuccess==1)
	{		
		wsprintf(szInfo, g_Message[ETC_MESSAGE97].szMessage, pPacket->szId, pPacket->szMessage);	// MSG_ID : 97 ; �ӼӸ� %s : %s\n\n
	}
	else if(pPacket->nSuccess==2)
		wsprintf(szInfo, g_Message[ETC_MESSAGE98].szMessage, pPacket->szId);						// MSG_ID : 98 ; �ӼӸ��� %s�� ���߽��ϴ�.\n\n

	DisplayMessageAdd(szInfo, 0xffff9a63, TRUE, DEF_CHAT_TYPE_WHISPER);		
}
//==============================================================//
// ���� ó��.													//
//==============================================================//
void CmdDGMessage(char* pMsg, DWORD dwLen)
{	
	char szInfo[0xff] = {0,};
	
	CTWS_DSCHAT_COMMAND* pPacket = (CTWS_DSCHAT_COMMAND*)pMsg;

	if(pPacket->nSuccess==1)
		wsprintf(szInfo, g_Message[ETC_MESSAGE99].szMessage, pPacket->szId, pPacket->szMessage);	// MSG_ID : 99 ; ���� %s : %s\n\n
	else if(pPacket->nSuccess==2)
		wsprintf(szInfo, g_Message[ETC_MESSAGE100].szMessage, pPacket->szId);					// MSG_ID : 100 ; ������ %s�� ���߽��ϴ�.\n\n

	DisplayMessageAdd(szInfo, 0xffff9a63, TRUE, DEF_CHAT_TYPE_WHISPER);		
}
//==============================================================//
// �������� �Ϲ�ä�� ó��.										//
//==============================================================//
void CmdDGChatUser(char* pMsg, DWORD dwLen)
{
	// Modified by minjin. if statement -> switch-case statement // 2004. 10. 01.
	char szInfo[0xff] = {0,};
	WSTC_DGCHAT_USER* pPacket = (WSTC_DGCHAT_USER*)pMsg;			
	wsprintf( szInfo, "%s : %s", pPacket->szId, pPacket->szMessage );

	// �ٵ�.. ���� �� if ���� �� �ϴ°ǰ���..? ^^;;;
	if ( GetGameStatus() == UPDATE_GAME_WORLD )
	{
		switch ( pPacket->nIndex )
		{
		case 2:	DisplayMessageAdd( szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD );				break;
		case 3:	DisplayMessageAdd( szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY );				break;
		case 4:	DisplayMessageAdd( szInfo, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND );			break;
		case 5:	DisplayMessageAdd( pPacket->szMessage, 0xffffa2ff, TRUE, DEF_CHAT_TYPE_GUILD );	break;
		}
/*		if(pPacket->nIndex==2)
			DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);
		else if(pPacket->nIndex==3)
			DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);
		else if(pPacket->nIndex==4)
			DisplayMessageAdd(szInfo, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);
		else if(pPacket->nIndex==5)
			DisplayMessageAdd(pPacket->szMessage, 0xffffa2ff, TRUE, DEF_CHAT_TYPE_GUILD);
//*/}
	else
	{
		switch ( pPacket->nIndex )
		{
		case 2:	DisplayMessageAdd( szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD );				break;
		case 3:	DisplayMessageAdd( szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY );				break;
		case 4:	DisplayMessageAdd( szInfo, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND );			break;
		case 5:	DisplayMessageAdd( pPacket->szMessage, 0xffffa2ff, TRUE, DEF_CHAT_TYPE_GUILD );	break;
		}
/*		if(pPacket->nIndex==2)
			DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);			
		else if(pPacket->nIndex==3)
			DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);			
		else if(pPacket->nIndex==4)
			DisplayMessageAdd(szInfo, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);			
		else if(pPacket->nIndex==5)	
			DisplayMessageAdd(pPacket->szMessage, 0xffffa2ff, TRUE, DEF_CHAT_TYPE_GUILD);
//*/}
}
//==============================================================//
// ���忡�� �Ϲ�ä�� �޼��� ó��.								//
//==============================================================//
void WorldMessageAdd(char* pszMessage, DWORD dwChatColor)
{	
	lstrcpy(g_sWordChatMsg[g_nMsgInx].szDungeon, pszMessage);
	g_sWordChatMsg[g_nMsgInx].sdwChatColor	= dwChatColor;
	g_sWordChatMsg[g_nMsgInx].snSize = lstrlen(pszMessage);

	if(g_nMsgInx<__MAX_WORD_QUEUE__-1)
		g_nMsgInx++;
	else
		g_nMsgInx = 0;
	
	if(g_nWordEnd<__MAX_WORD_QUEUE__-1)
	{		
		if(g_nWordIndex!=0)
			g_nWordEnd++;
	}
	else
		g_nWordEnd = 0;
		
	if(g_nWordIndex==__MAX_WORD_QUEUE__)
	{
		if(g_nWordStart<__MAX_WORD_QUEUE__-1)
			g_nWordStart++;
		else
			g_nWordStart = 0;
	}

	if(g_nWordIndex<__MAX_WORD_QUEUE__)
		g_nWordIndex++;	

	if(g_nWordIndex>__MAX_CHAT_INDEX__)
	{	
		if(g_nScrollPos<__MAX_CHAT_SCROLL__)
			g_nScrollPos++;

		g_nCurScrollPos = g_nScrollPos;
	}

	if(g_nScrollPos>0)
	{
		//SPR(SPR_CHAT_SC)->vPos.y = 730-16;
	}
}
//==============================================================//
// �޼��� ����Ʈ�� �߰�(��� �޼����� DisplayMessageAdd ȣ��).	//
//==============================================================//
int DisplayMessageAdd(char* pszMessage, DWORD dwCol, BOOL bChk, BYTE byType)
{	
	if (GetGameStatus()==UPDATE_GAME_PLAY)
		return DungeonMessageAdd(pszMessage, dwCol, bChk, byType);
	else
		WorldMessageAdd(pszMessage, dwCol);

	return 0;
}
//==============================================================//
// Ư�� ä�� �޼��� ����Ʈ���� ����.							//
//==============================================================//
void RemoveChat(POSITION_ pos, BYTE byType)
{
	if(!pos)
		return;
	
	LP_CHAT_MSG	lpChatMsg = NULL;
	
	switch(byType)
	{
	case DEF_CHAT_TYPE_ALLCHAT:
		lpChatMsg	= (LP_CHAT_MSG)g_pAllChatList->GetNext(pos);	
		break;
	case DEF_CHAT_TYPE_WHISPER:
		lpChatMsg	= (LP_CHAT_MSG)g_pWhisperChatList->GetNext(pos);			
		break;
	case DEF_CHAT_TYPE_GUILD:
		lpChatMsg = (LP_CHAT_MSG)g_pGuildChatList->GetNext(pos);
		break;
	case DEF_CHAT_TYPE_PARTY:
		lpChatMsg = (LP_CHAT_MSG)g_pPartyChatList->GetNext(pos);
		break;
	case DEF_CHAT_TYPE_FRIEND:
		lpChatMsg = (LP_CHAT_MSG)g_pFriendChatList->GetNext(pos);
		break;		
	default:
		return;
	}

	if(lpChatMsg)
	{
		LALFree(g_pChatPool, lpChatMsg);
		lpChatMsg = NULL;
	}
}
//==============================================================//
// Ư�� ä�� �޼��� ����Ʈ���� �߰�.							//
//==============================================================//
void AddChat(char* pszText, DWORD dwColor, BYTE byType, int nSize)
{
	LP_CHAT_MSG lpChatMsg = (LP_CHAT_MSG)LALAlloc(g_pChatPool);
	memset(lpChatMsg->szText, 0, sizeof(lpChatMsg->szText));

	if(nSize)
		__lstrcpyn(lpChatMsg->szText, pszText, nSize);
	else
		lstrcpy(lpChatMsg->szText, pszText);

	lpChatMsg->dwColor	= dwColor;

	switch(byType)
	{
	case DEF_CHAT_TYPE_ALLCHAT:
		g_pAllChatList->AddTail(lpChatMsg);
		break;
	case DEF_CHAT_TYPE_WHISPER:
		g_pWhisperChatList->AddTail(lpChatMsg);
		break;
	case DEF_CHAT_TYPE_GUILD:
		g_pGuildChatList->AddTail(lpChatMsg);		
		break;		
	case DEF_CHAT_TYPE_PARTY:
		g_pPartyChatList->AddTail(lpChatMsg);
		break;		
	case DEF_CHAT_TYPE_FRIEND:
		g_pFriendChatList->AddTail(lpChatMsg);		
		break;		
	default:
		{
			if(lpChatMsg)
			{
				LALFree(g_pChatPool, lpChatMsg);
				lpChatMsg = NULL;
			}
		}
		return;
	}	
	CChatListWnd::GetInstance()->SetScroll();
}
//==============================================================//
// �޼��� ����Ʈ�� �� ���� ���� ���� �޼��� ����.				//
//==============================================================//
void ChkMaxChaList(BYTE byType)
{
	switch(byType)
	{
	case DEF_CHAT_TYPE_ALLCHAT:		
		{
			if(g_pAllChatList->GetCount()==__MAX_MSG_CNT__)
			{
				POSITION_ pos = g_pAllChatList->GetHeadPosition();	
				RemoveChat(pos, byType);
				g_pAllChatList->RemoveHead();
			}
		}
		break;
	case DEF_CHAT_TYPE_WHISPER:
		{
			if(g_pWhisperChatList->GetCount()==__MAX_MSG_CNT__)
			{
				POSITION_ pos = g_pWhisperChatList->GetHeadPosition();	
				RemoveChat(pos, byType);
				g_pWhisperChatList->RemoveHead();
			}
		}		
		break;
	case DEF_CHAT_TYPE_GUILD:
		{
			if(g_pGuildChatList->GetCount()==__MAX_MSG_CNT__)
			{
				POSITION_ pos = g_pGuildChatList->GetHeadPosition();	
				RemoveChat(pos, byType);
				g_pGuildChatList->RemoveHead();
			}
		}		
		break;		
	case DEF_CHAT_TYPE_PARTY:
		{
			if(g_pPartyChatList->GetCount()==__MAX_MSG_CNT__)
			{
				POSITION_ pos = g_pPartyChatList->GetHeadPosition();	
				RemoveChat(pos, byType);
				g_pPartyChatList->RemoveHead();
			}
		}		
		break;		
	case DEF_CHAT_TYPE_FRIEND:
		{
			if(g_pFriendChatList->GetCount()==__MAX_MSG_CNT__)
			{
				POSITION_ pos = g_pFriendChatList->GetHeadPosition();	
				RemoveChat(pos, byType);
				g_pFriendChatList->RemoveHead();
			}
		}		
		break;		
	default:		
		return;
	}		
}
//==============================================================//
// �޼��� ������ �����ؼ� �߰��ϴ� �κ�.						//
//==============================================================//
void ChatListAdd(char* pszText, DWORD dwCol, BYTE byType, BOOL bChkSystemMsg)
{
	CGameMenuWnd* pGameMenuWnd = CGameMenuWnd::GetInstance();

	char*	szTemp = {0, };
	int		nSize	= 0;
	
	szTemp	= pszText;	
	nSize	= lstrlen(pszText);
	
	while(nSize>0)
	{
		if(nSize>MAX_CHATLIST_TEXT)
		{
			for(;;)
			{
				if(nSize > MAX_CHATLIST_TEXT)
				{
					char* szPrev = CharPrevExA(0, szTemp, szTemp+MAX_CHATLIST_TEXT, 0);
					nSize = lstrlen(szTemp) - lstrlen(szPrev);
	
					ChkMaxChaList(byType);
					AddChat(szTemp, dwCol, byType, nSize);
					// sung-han 2005-03-30 �븻 ä��â ���͸�
					if( ( byType == DEF_CHAT_TYPE_ALLCHAT )
						|| ( pGameMenuWnd->m_bChatKindWhisper && byType == DEF_CHAT_TYPE_WHISPER )
						|| ( pGameMenuWnd->m_bChatKindGuild && byType == DEF_CHAT_TYPE_GUILD )
						|| ( pGameMenuWnd->m_bChatKindParty && byType == DEF_CHAT_TYPE_PARTY )
						|| ( pGameMenuWnd->m_bChatKindFriend && byType == DEF_CHAT_TYPE_FRIEND ) )
					{
						MessageAdd(szTemp, dwCol, bChkSystemMsg, nSize);
					}

					szTemp	= szPrev;
					nSize	= lstrlen(szTemp);
				}
				else	// ���� ���ڿ��� ä�� ���ο� ����, ���̸� ����Ѵ�.
				{
					ChkMaxChaList(byType);
					AddChat(szTemp, dwCol, byType, nSize);
					// sung-han 2005-03-30 �븻 ä��â ���͸�
					if( ( byType == DEF_CHAT_TYPE_ALLCHAT )
						|| ( pGameMenuWnd->m_bChatKindWhisper && byType == DEF_CHAT_TYPE_WHISPER )
						|| ( pGameMenuWnd->m_bChatKindGuild && byType == DEF_CHAT_TYPE_GUILD )
						|| ( pGameMenuWnd->m_bChatKindParty && byType == DEF_CHAT_TYPE_PARTY )
						|| ( pGameMenuWnd->m_bChatKindFriend && byType == DEF_CHAT_TYPE_FRIEND ) )
					{
						MessageAdd(szTemp, dwCol, bChkSystemMsg, nSize);
					}
					nSize = 0;
					break;
				}
			}
		}
		else
		{
			ChkMaxChaList(byType);
			AddChat(szTemp, dwCol, byType);
			// sung-han 2005-03-30 �븻 ä��â ���͸�
			if( ( byType == DEF_CHAT_TYPE_ALLCHAT )
				|| ( pGameMenuWnd->m_bChatKindWhisper && byType == DEF_CHAT_TYPE_WHISPER )
				|| ( pGameMenuWnd->m_bChatKindGuild && byType == DEF_CHAT_TYPE_GUILD )
				|| ( pGameMenuWnd->m_bChatKindParty && byType == DEF_CHAT_TYPE_PARTY )
				|| ( pGameMenuWnd->m_bChatKindFriend && byType == DEF_CHAT_TYPE_FRIEND ) )
			{
				MessageAdd(szTemp, dwCol, bChkSystemMsg);
			}
			nSize = 0;
		}
	}
}


#ifdef _USE_IME

void EnableChatInput(BOOL bClear, DWORD dwIndex)
{
	POINT pChatPoint;
	pChatPoint.x	= 10;
	pChatPoint.y	= 635;

	if(bClear)	GET_IMEEDIT()->ClearImeEditString();
	if(dwIndex)	GET_IMEEDIT()->SetEditIndex(dwIndex);
	GET_IMEEDIT()->ActivateIME(pChatPoint, __MAX_BUUFER_SIZE__);

	ShowChatEditBackground(TRUE, GET_IMEEDIT()->GetOrder()-1);
}
#endif

void ChatModeOn(DWORD dwInputId, BOOL bClear)
{
#ifndef _USE_IME
	ChangeLanguage(TRUE);
#endif	
	
	g_pGVDungeon->bChatMode = TRUE;	
	
	if(bClear)
		g_pInputManager->ClearInput(dwInputId);

	g_pInputManager->SetFocusInput(dwInputId);

}

void ChatModeOff(DWORD dwInputId, DWORD dwFocusId, BOOL bChk, BOOL bClear)
{
#ifndef _USE_IME
	ChangeLanguage(FALSE);
#endif	

	if(bChk)
	{
		g_nChatModeExt			= 0;
		g_pGVDungeon->bChatMode = FALSE;	
	}	

	if(bClear)
		g_pInputManager->ClearInput(dwInputId);
	
	g_pInputManager->SetFocusInput(dwFocusId);	
}

void IMESetEdit(DWORD dwIndex, BOOL bChk, BOOL bBack, BOOL bNative)
{
#ifdef _USE_IME

	if(GET_IMEEDIT()->IsActivated())
	{
		GET_IMEEDIT()->DeactivateIME();

		if(bChk)
			GET_IMEEDIT()->SetEditIndex(dwIndex);

		if(bBack)
			ShowChatEditBackground(FALSE, 0);

		if(bNative)
			GET_IMEEDIT()->SetNativeLanguageMode(FALSE);
	}

#endif
}
//==============================================================//
// End.															//
//==============================================================//