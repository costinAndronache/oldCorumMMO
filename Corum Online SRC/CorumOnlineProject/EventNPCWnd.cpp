//======================================================//
// Code by Jang.							2003.05.09	//
//======================================================//
#include "Menu.h"
#include "InitGame.h"
#include "Message.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "GameNoticeWnd.h"
#include "EventNPCWnd.h"
#include "EventNPCSubWnd.h"
#include "WorldProcess.h"
#include "CodeFun.h"
#include "Interface.h"


CEventNPCWnd* CEventNPCWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CEventNPCWnd::CEventNPCWnd()
{ 	
}

CEventNPCWnd::~CEventNPCWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CEventNPCWnd::Init()
{	
	/*
	InsertData(SPR_OBJ_EVENT_NPC_WND1, SPR_INTERFACE_DF_WND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_EVENT_NPC_WND2, SPR_INTERFACE_DF_WND2, 0, 32, 1.0f, 8.0f, 0);
	InsertData(SPR_OBJ_EVENT_NPC_WND3, SPR_INTERFACE_DF_WND3, 0, 160, 1.0f, 1.0f, 0);

	InsertData(SPR_OBJ_EVENT_NPC_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_EVENT_NPC_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);

	InsertData(SPR_OBJ_EVENT_NPC_OK1, SPR_INTERFACE_OK1, 121, 160, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_EVENT_NPC_OK2, SPR_INTERFACE_OK2, 121, 160, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_EVENT_NPC_OK3, SPR_INTERFACE_OK3, 121, 160, 1.0f, 1.0f, 2);

	InsertData(SPR_OBJ_EVENT_NPC_CANCEL1, SPR_INTERFACE_CANCEL1, 189, 160, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_EVENT_NPC_CANCEL2, SPR_INTERFACE_CANCEL2, 189, 160, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_EVENT_NPC_CANCEL3, SPR_INTERFACE_CANCEL3, 189, 160, 1.0f, 1.0f, 2);
	
	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);

	InsertCheckInterface(121, 160, 185, 186, 4, CHECK_BTN);
	InsertCheckInterface(189, 160, 253, 186, 5, CHECK_BTN);
*/
	m_bInit	= TRUE;

	// ��� ���� �κ�. (���� �Ʒ��� ����.)

	// �� ���� �츮�� ���� ���ϴ� ����
	// �ź��� ������ ���ܳ��⵵ �Ѵٳ�.
	// Ư���ϰ� �ź��� �� ������ ã�� ����
	// ���� ���谡���� ����� ������ �ٳ���...
	// ���� ���� �׵��� �ϳ�����.  ��� ����
	// �� ������ ������ ���Ͽ�����
	// ��ġ�� �˷��� ���� ����..
	// �ź��� ������ ������ ���ٰ�?

	__lstrcpyn(m_szRenderText[0], g_Message[ETC_MESSAGE1024].szMessage, lstrlen(g_Message[ETC_MESSAGE1024].szMessage)); // "�� ���� �츮�� ���� ���ϴ� ����"
	__lstrcpyn(m_szRenderText[1], g_Message[ETC_MESSAGE1025].szMessage, lstrlen(g_Message[ETC_MESSAGE1025].szMessage)); // "�ź��� ������ ���ܳ��⵵ �Ѵٳ�."
	__lstrcpyn(m_szRenderText[2], g_Message[ETC_MESSAGE1026].szMessage, lstrlen(g_Message[ETC_MESSAGE1026].szMessage)); // "Ư���ϰ� �ź��� �� ������ ã�� ����"
	__lstrcpyn(m_szRenderText[3], g_Message[ETC_MESSAGE1027].szMessage, lstrlen(g_Message[ETC_MESSAGE1027].szMessage)); // "���� ���谡���� ����� ������ �ٳ���..."
	__lstrcpyn(m_szRenderText[4], g_Message[ETC_MESSAGE1028].szMessage, lstrlen(g_Message[ETC_MESSAGE1028].szMessage)); // "���� ���� �׵��� �ϳ�����.  ��� ����"
	__lstrcpyn(m_szRenderText[5], g_Message[ETC_MESSAGE1029].szMessage, lstrlen(g_Message[ETC_MESSAGE1029].szMessage)); // "�� ������ ������ ���Ͽ�����"
	__lstrcpyn(m_szRenderText[6], g_Message[ETC_MESSAGE1030].szMessage, lstrlen(g_Message[ETC_MESSAGE1030].szMessage)); // "��ġ�� �˷��� ���� ����.."
	__lstrcpyn(m_szRenderText[7], g_Message[ETC_MESSAGE1031].szMessage, lstrlen(g_Message[ETC_MESSAGE1031].szMessage)); // "�ź��� ������ ������ ���ٰ�?"

	
	// ���ö���¡ �� ��, ����� �� ���� �ٲ�ų� �ϸ� �Ʒ� ���� ���� �ٲ��ֽʽÿ�.

	m_iTextLineNumber = 8;

	return TRUE;
}

void CEventNPCWnd::Remove()
{
	RemoveAllData();	
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CEventNPCWnd::SetOrder()
{
}

void CEventNPCWnd::RenderText()
{
	int		nSize = 0, i = 0;		

	//"��"
	nSize = lstrlen(g_Message[ETC_MESSAGE1062].szMessage); // "�̺�Ʈ �����"
	RenderFont(g_Message[ETC_MESSAGE1062].szMessage, m_fPosX+20, m_fPosX+20+(nSize*6), m_fPosZ+5, m_fPosZ+19, GetStartOrder()+1);	

	// ��� ��� �κ�. 
	for (i = 0; i < m_iTextLineNumber; i++)
	{
		nSize = lstrlen(m_szRenderText[i]);
		RenderFont(m_szRenderText[i], m_fPosX+10, m_fPosX+10+(nSize*7), m_fPosZ+35+(i*15), m_fPosZ+49+(i*15), GetStartOrder()+1);			
	}
}

void CEventNPCWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(bActive==FALSE)
	{
		if (m_bChk)
		{
			;
		}
		else
		{
			;
		}

		m_bChk			= FALSE;		
		m_byType		= 0;
		ShowSpriteAll();	
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);		

		//SetRender(SPR_OBJ_EVENT_NPC_WND, TRUE);

		SetRender(SPR_OBJ_EVENT_NPC_WND1, TRUE);
		SetRender(SPR_OBJ_EVENT_NPC_WND2, TRUE);
		SetRender(SPR_OBJ_EVENT_NPC_WND3, TRUE);

		SetRender(SPR_OBJ_EVENT_NPC_CLOSE1, TRUE);
		SetRender(SPR_OBJ_EVENT_NPC_OK1, TRUE);
		SetRender(SPR_OBJ_EVENT_NPC_CANCEL1, TRUE);		
	}
}

int CEventNPCWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(m_bBtn[0] && nRt!=4)
	{
		SetRender(SPR_OBJ_EVENT_NPC_OK1, TRUE);
		SetRender(SPR_OBJ_EVENT_NPC_OK2, FALSE);
		SetRender(SPR_OBJ_EVENT_NPC_OK3, FALSE);								
		m_bBtn[0] = FALSE;
	}

	if(m_bBtn[1] && nRt!=5)
	{
		SetRender(SPR_OBJ_EVENT_NPC_CANCEL1, TRUE);
		SetRender(SPR_OBJ_EVENT_NPC_CANCEL2, FALSE);
		SetRender(SPR_OBJ_EVENT_NPC_CANCEL3, FALSE);
		m_bBtn[1] = FALSE;
	}

	InitValue(nRt);

	MOUSECHECK eMouseCheck = GetMouseCheck(nRt);

	switch(nRt)
	{		
	case 1:
		{			
			SetMouseCheck(nRt, eMouseCheck);						
		}
		break;
	case 2:
		{			
			if(eMouseCheck==MOUSE_UP)
				SetActive(FALSE);				
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_EVENT_NPC_CLOSE1, FALSE);
				SetRender(SPR_OBJ_EVENT_NPC_CLOSE2, TRUE);
			}						
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 3:			
		{
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 4:
		{			
			if(eMouseCheck==MOUSE_UP)
			{															
				m_bChk		= TRUE;
				SetActive(FALSE);

				// ���������츦 ����ش�.

				CEventNPCSubWnd* pEventNPCSubWnd = CEventNPCSubWnd::GetInstance();
				
				pEventNPCSubWnd->SetText(EVENTNPC_SUB_TYPE_OK_SELECT);
				pEventNPCSubWnd->QueryCanMoveDungeon();
				pEventNPCSubWnd->SetActive(TRUE);
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{				
				SetRender(SPR_OBJ_EVENT_NPC_OK1, FALSE);
				SetRender(SPR_OBJ_EVENT_NPC_OK2, FALSE);
				SetRender(SPR_OBJ_EVENT_NPC_OK3, TRUE);				
				m_bBtn[0] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{								
				SetRender(SPR_OBJ_EVENT_NPC_OK1, FALSE);
				SetRender(SPR_OBJ_EVENT_NPC_OK2, TRUE);
				SetRender(SPR_OBJ_EVENT_NPC_OK3, FALSE);
				m_bBtn[0] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 5:
		{			
			if(eMouseCheck==MOUSE_UP)
			{
				m_bChk	= FALSE;								
				SetActive(FALSE);	
				
				// ���� �����츦 ����ش�.

				// ���������츦 ����ش�.

				CEventNPCSubWnd* pEventNPCSubWnd = CEventNPCSubWnd::GetInstance();
				
				pEventNPCSubWnd->SetText(EVENTNPC_SUB_TYPE_CANCEL);				
				pEventNPCSubWnd->SetActive(TRUE);

			}
			else if(eMouseCheck==MOUSE_DOWN)
			{				
				SetRender(SPR_OBJ_EVENT_NPC_CANCEL1, FALSE);
				SetRender(SPR_OBJ_EVENT_NPC_CANCEL2, FALSE);
				SetRender(SPR_OBJ_EVENT_NPC_CANCEL3, TRUE);				
				m_bBtn[1] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{				
				SetRender(SPR_OBJ_EVENT_NPC_CANCEL1, FALSE);
				SetRender(SPR_OBJ_EVENT_NPC_CANCEL2, TRUE);
				SetRender(SPR_OBJ_EVENT_NPC_CANCEL3, FALSE);
				m_bBtn[1] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_EVENT_NPC_CLOSE1, TRUE);
		SetRender(SPR_OBJ_EVENT_NPC_CLOSE2, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
		return CInterface::GetInstance()->GetSetWnd();

	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();		

	return 0;
}
//======================================================//
// End.													//
//======================================================//
