//==============================================================//
// Code by Jang.									2003.11.10	//
//==============================================================//
#include "Messenger.h"
#include "GameControl.h"
#include "User.h"
#include "Message.h"
#include "HashTable.h"
#include "Interface.h"
#include "MessengerWnd.h"
#include "Chat.h"
#include "GroupWnd.h"
//==============================================================//
// �޽����� ��ϵ� ģ���� ����Ѵ�.								//
//==============================================================//
void CmdMessengerUser( char* pMsg, DWORD dwLen )
{
	WSTC_MESSENGER_USER* pPacket = (WSTC_MESSENGER_USER*)pMsg;

	LP_MESSENGER_USER lpMessengerUser = (LP_MESSENGER_USER)LALAlloc(g_pMessengerUserPool);
	
	if(lpMessengerUser)
	{		
		memset(lpMessengerUser, 0, sizeof(MESSENGER_USER));
		lpMessengerUser->byType			= pPacket->byType;
		lpMessengerUser->dwUserIndex	= pPacket->dwUserIndex;
		lpMessengerUser->wClass			= pPacket->wClass;	
		lpMessengerUser->byStatus		= pPacket->byStatus;
		__lstrcpyn(lpMessengerUser->szName, pPacket->szName, MAX_CHARACTER_NAME_LENGTH);
		
		if(lpMessengerUser->byType==0)
			g_pMessengerOnLineHash->Insert(lpMessengerUser, lpMessengerUser->dwUserIndex);
		else
			g_pMessengerOffLineHash->Insert(lpMessengerUser, lpMessengerUser->dwUserIndex);
	}

	CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();

	if(pGroupWnd->GetActive()==TRUE)
	{
		if(pGroupWnd->m_byGroupTab==GROUP_TAB_FRIEND)
		{
			pGroupWnd->SetPrevNext(__FRIEND_PAGE_CURR__);
			pGroupWnd->SetStatus(GROUP_FRIEND_DEFAULT);
		}
		pGroupWnd->m_nPartyUserIndex = -1;		
		SPR(SPR_OBJ_GROUPWND_SELECT)->ShowSprite(FALSE);		
	}
}
//==============================================================//
// �޽����� ��ϵ� ģ�� �α���/�α׾ƿ� ����.					//
//==============================================================//
void CmdMessengerUserInfo( char* pMsg, DWORD dwLen )
{
	WSTC_MESSENGER_USERINFO* pPacket = (WSTC_MESSENGER_USERINFO*)pMsg;

	LP_MESSENGER_USER lpMessengerUser		= NULL;
	LP_MESSENGER_USER lpMessengerCopyUser	= NULL;

	char szInfo[0xff] = {0,};	

	if(pPacket->byType==0)
	{
		lpMessengerUser = g_pMessengerOffLineHash->GetData(pPacket->dwUserIndex);

		if(lpMessengerUser)
		{
			lpMessengerCopyUser = (LP_MESSENGER_USER)LALAlloc(g_pMessengerUserPool);
			
			memcpy(lpMessengerCopyUser, lpMessengerUser, sizeof(MESSENGER_USER));
			lpMessengerCopyUser->byType	= pPacket->byType;
			g_pMessengerOffLineHash->Delete(lpMessengerUser, lpMessengerUser->dwUserIndex);
			g_pMessengerOnLineHash->Insert(lpMessengerCopyUser, lpMessengerCopyUser->dwUserIndex);			
			memset(lpMessengerUser, 0, sizeof(lpMessengerUser));
			
			wsprintf(szInfo, g_Message[ETC_MESSAGE796].szMessage, lpMessengerUser->szName); // "%s ���� �α��� �ϼ̽��ϴ�."
			DisplayMessageAdd(szInfo, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);
		}
	}
	else
	{
		lpMessengerUser = g_pMessengerOnLineHash->GetData(pPacket->dwUserIndex);

		if(lpMessengerUser)
		{
			lpMessengerCopyUser = (LP_MESSENGER_USER)LALAlloc(g_pMessengerUserPool);

			memcpy(lpMessengerCopyUser, lpMessengerUser, sizeof(MESSENGER_USER));
			lpMessengerCopyUser->byType	= pPacket->byType;
			g_pMessengerOnLineHash->Delete(lpMessengerUser, lpMessengerUser->dwUserIndex);
			g_pMessengerOffLineHash->Insert(lpMessengerCopyUser, lpMessengerCopyUser->dwUserIndex);
			memset(lpMessengerUser, 0, sizeof(lpMessengerUser));
			
			wsprintf(szInfo, g_Message[ETC_MESSAGE109].szMessage, lpMessengerUser->szName); // "%s ���� �α� �ƿ� �ϼ̽��ϴ�."
			DisplayMessageAdd(szInfo, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);
		}
	}

	CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();

	if(pGroupWnd->GetActive()==TRUE)
	{
		if(pGroupWnd->m_byGroupTab==GROUP_TAB_FRIEND)
		{
			pGroupWnd->SetPrevNext(__FRIEND_PAGE_CURR__);
			pGroupWnd->SetStatus(GROUP_FRIEND_DEFAULT);
		}
		pGroupWnd->m_nPartyUserIndex = -1;
		pGroupWnd->SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE);		
	}
}
//==============================================================//
// �޽����� ��� �������� ��� ���� �޼���.						//
//==============================================================//
void CmdMessengerUserFail( char* pMsg, DWORD dwLen )
{
	WSTC_MESSENGER_USERFAIL* pPacket = (WSTC_MESSENGER_USERFAIL*)pMsg;

	DWORD dwMsgId = 0;
	switch(pPacket->byFail)
	{
	case 0: dwMsgId = ETC_MESSAGE273; break; // "������ �����ϴ�."
	case 1: dwMsgId = ETC_MESSAGE272; break; // "������ �� �̻� ģ���� ����� �� �����ϴ�."
	case 2: dwMsgId = ETC_MESSAGE163; break; // "�� �̻� ģ���� ����� �� �����ϴ�."
	case 3: dwMsgId = ETC_MESSAGE831; break; // "���濡 ģ�� ��Ͽ� ��ϵǾ��ֽ��ϴ�."
	}

	DisplayMessageAdd(g_Message[dwMsgId].szMessage, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);
}

//==============================================================//
// �޽����� ��� �������� ��� ���� �޼���.						//
//==============================================================//
void CmdMessengerUserAdd( char* pMsg, DWORD dwLen )
{
	WSTC_MESSENGER_USERADD* pPacket = (WSTC_MESSENGER_USERADD*)pMsg;
	
	CMessengerWnd* pMessengerWnd = CMessengerWnd::GetInstance();
	memset(pMessengerWnd->m_szName, 0, sizeof(pMessengerWnd->m_szName));	
	__lstrcpyn(pMessengerWnd->m_szName, pPacket->szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH );
	pMessengerWnd->m_bChk			= TRUE;
	pMessengerWnd->m_dwUserIndex	= pPacket->dwUserIndex;	
	pMessengerWnd->SetActive();
}
//==============================================================//
// �޽����� ��� �ź� �޼���.									//
//==============================================================//
void CmdMessengerFail( char* pMsg, DWORD dwLen )
{
	WSTC_MESSENGER_FAIL* pPacket = (WSTC_MESSENGER_FAIL*)pMsg;

	char szInfo[0xff] = {0,};
	wsprintf(szInfo, g_Message[ETC_MESSAGE745].szMessage, pPacket->szName); // "%s ���� ģ�� ����� �ź��ϼ̽��ϴ�."
	DisplayMessageAdd(szInfo, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);
}
//==============================================================//
// �޽������� ģ�� ���� ó��.									//
//==============================================================//
void CmdFriendDelete( char* pMsg, DWORD dwLen )
{
	WSTS_FRIENDUSER_DELETE* pPacket = (WSTS_FRIENDUSER_DELETE*)pMsg;

	LP_MESSENGER_USER lpMessengerUser = g_pMessengerOnLineHash->GetData(pPacket->dwUserIndex);

	char szInfo[0xff] = {0,};

	if(lpMessengerUser)
	{		
		wsprintf(szInfo, g_Message[ETC_MESSAGE746].szMessage, lpMessengerUser->szName); // "%s ���� ģ�� ����Ʈ���� �����Ǿ����ϴ�."
		DisplayMessageAdd(szInfo, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);

		g_pMessengerOnLineHash->Delete(lpMessengerUser, lpMessengerUser->dwUserIndex);
		memset(lpMessengerUser, 0, sizeof(lpMessengerUser));
		lpMessengerUser = NULL;
	}

	lpMessengerUser = g_pMessengerOffLineHash->GetData(pPacket->dwUserIndex);

	if(lpMessengerUser)
	{
		wsprintf(szInfo, g_Message[ETC_MESSAGE746].szMessage, lpMessengerUser->szName); // "%s ���� ģ�� ����Ʈ���� �����Ǿ����ϴ�."
		DisplayMessageAdd(szInfo, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);
		
		g_pMessengerOffLineHash->Delete(lpMessengerUser, lpMessengerUser->dwUserIndex);
		memset(lpMessengerUser, 0, sizeof(lpMessengerUser));
		lpMessengerUser = NULL;
	}

	CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();

	if(pGroupWnd->GetActive()==TRUE)
	{
		if(pGroupWnd->m_byGroupTab==GROUP_TAB_FRIEND)
		{
			pGroupWnd->SetPrevNext(__FRIEND_PAGE_CURR__);
			pGroupWnd->SetStatus(GROUP_FRIEND_DEFAULT);
		}
		pGroupWnd->m_nPartyUserIndex = -1;
		pGroupWnd->SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE);
	}	
}
//==============================================================//
// �޽������� ģ�� ��� ���� ó��.								//
//==============================================================//
void CmdMessengerSuccess( char* pMsg, DWORD dwLen )
{
	WSTC_MESSENGER_SUCCESS* pPacket = (WSTC_MESSENGER_SUCCESS*)pMsg;

	char szInfo[0xff] = {0,};
	wsprintf(szInfo, g_Message[ETC_MESSAGE748].szMessage, pPacket->szName); // "%s ���� ģ�� ����Ʈ�� ��ϵǾ����ϴ�."
	DisplayMessageAdd(szInfo, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);

	LP_MESSENGER_USER lpMessengerUser = (LP_MESSENGER_USER)LALAlloc(g_pMessengerUserPool);
	
	if(lpMessengerUser)
	{		
		
		memset(lpMessengerUser, 0, sizeof(MESSENGER_USER));
		lpMessengerUser->byType			= pPacket->byType;
		lpMessengerUser->dwUserIndex	= pPacket->dwUserIndex;
		lpMessengerUser->wClass			= pPacket->wClass;		
		__lstrcpyn(lpMessengerUser->szName, pPacket->szName, MAX_CHARACTER_NAME_LENGTH);
		
		LP_MESSENGER_USER lpDeleteUser  = g_pMessengerOffLineHash->GetData(lpMessengerUser->dwUserIndex);

		if(lpDeleteUser)
			g_pMessengerOffLineHash->Delete(lpDeleteUser, lpMessengerUser->dwUserIndex, TRUE);

		if(lpMessengerUser->byType==0)
			g_pMessengerOnLineHash->Insert(lpMessengerUser, lpMessengerUser->dwUserIndex);
		else
			g_pMessengerOffLineHash->Insert(lpMessengerUser, lpMessengerUser->dwUserIndex);
	}

	CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();

	if(pGroupWnd->GetActive()==TRUE)
	{
		if(pGroupWnd->m_byGroupTab==GROUP_TAB_FRIEND)
		{
			pGroupWnd->SetPrevNext(__FRIEND_PAGE_CURR__);
			pGroupWnd->SetStatus(GROUP_FRIEND_DEFAULT);
		}
		pGroupWnd->m_nPartyUserIndex = -1;
		pGroupWnd->SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE);
	}
}
//==============================================================//
// �޽������� ģ�� ���� ����.									//
//==============================================================//
void CmdFriendInfo( char* pMsg, DWORD dwLen )
{
	WSTC_FRIENDUSER_INFO* pPacket = (WSTC_FRIENDUSER_INFO*)pMsg;

	CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();
	
	if(pGroupWnd->GetActive()==TRUE)
	{
		memset(pGroupWnd->m_szName, 0, sizeof(pGroupWnd->m_szName));
		__lstrcpyn(pGroupWnd->m_szName, pPacket->szCharName, MAX_CHARACTER_NAME_REAL_LENGTH);
		pGroupWnd->m_wDungeonID		= pPacket->wDungeonID;
		pGroupWnd->m_byLayerIndex		= pPacket->byLayerIndex;
		pGroupWnd->m_bInfo				= TRUE;
		pGroupWnd->m_dwPrevTime			= timeGetTime();
		pGroupWnd->m_dwCurTime			= timeGetTime();
	}
}
//==============================================================//
// �޽������� ģ�� ���� ����.									//
//==============================================================//
void CmdFriendStaus( char* pMsg, DWORD dwLen )
{
	WSTC_MESSENGER_STATUS* pPacket = (WSTC_MESSENGER_STATUS*)pMsg;
	
	LP_MESSENGER_USER lpMessengerUser = g_pMessengerOnLineHash->GetData(pPacket->dwUserIndex);

	if(lpMessengerUser)
	{
		lpMessengerUser->byStatus	= pPacket->byType;

		CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();

		if(pGroupWnd->GetActive()==TRUE)
		{
			if(pGroupWnd->m_byGroupTab==GROUP_TAB_FRIEND)
				pGroupWnd->SetPrevNext(__FRIEND_PAGE_CURR__);		
		}
	}	
}
//==============================================================//
// End.															//
//==============================================================//