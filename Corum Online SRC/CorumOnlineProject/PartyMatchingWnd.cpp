// PartyMatchingWnd.cpp: implementation of the CPartyMatchingWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartyMatchingWnd.h"
#include "SpriteManager.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "Chat.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "CodeFun.h"
#include "Party.h"



CPartyMatchingWnd* CPartyMatchingWnd::c_pThis = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction   (��Ƽ��ȭ�ý��� ����â 2005.02.15)
//////////////////////////////////////////////////////////////////////

CPartyMatchingWnd::CPartyMatchingWnd()
{
	m_dwUserIndex	= 0;		
	m_bBtn[0]		= FALSE;
	m_bBtn[1]		= FALSE;

}

CPartyMatchingWnd::~CPartyMatchingWnd()
{

}


BOOL CPartyMatchingWnd::Init()
{
	m_bInit	= TRUE;

	m_byPMatchResultTrue  = 0;
	m_byPMatchResultFalse = 1;

	return TRUE;
}

void CPartyMatchingWnd::Remove()
{
	RemoveAllData();	
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CPartyMatchingWnd::SetOrder()
{
}

void CPartyMatchingWnd::RenderText()
{
	char szTmp[50];
	ZeroMemory(szTmp, sizeof(szTmp));
	lstrcpy(szTmp, g_Message[ETC_MESSAGE1366].szMessage);//��Ƽ ��û �޽���
		
	RenderFont(szTmp, (int)m_fPosX+25, (int)m_fPosX+85, (int)m_fPosZ+6, (int)m_fPosZ+18, GetStartOrder()+1);

	int nSize = strlen(m_szInfo[0]);
	RenderFont(m_szInfo[0], m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+40, m_fPosZ+52, GetStartOrder()+1);	
	nSize = strlen(m_szInfo[1]);
	RenderFont(m_szInfo[1], m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+60, m_fPosZ+72, GetStartOrder()+1);	
}

void CPartyMatchingWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(bActive==FALSE)
	{
		ShowSpriteAll();

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_PARTYMATCHAING_WND, TRUE);
		SetRender(SPR_OBJ_PARTYMATCHAING_CLOSE1, TRUE);
		SetRender(SPR_OBJ_PARTYMATCHAING_OK1, TRUE);
		SetRender(SPR_OBJ_PARTYMATCHAING_CANCEL1, TRUE);		

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);


	}
}

int CPartyMatchingWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(m_bBtn[0] && nRt!=4)
	{
		SetRender(SPR_OBJ_PARTYMATCHAING_OK1, TRUE);
		SetRender(SPR_OBJ_PARTYMATCHAING_OK2, FALSE);
		SetRender(SPR_OBJ_PARTYMATCHAING_OK3, FALSE);								
		m_bBtn[0] = FALSE;
	}

	if(m_bBtn[1] && nRt!=5)
	{
		SetRender(SPR_OBJ_PARTYMATCHAING_CANCEL1, TRUE);
		SetRender(SPR_OBJ_PARTYMATCHAING_CANCEL2, FALSE);
		SetRender(SPR_OBJ_PARTYMATCHAING_CANCEL3, FALSE);
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
			{//������ ���� ó���� ����.
				if(m_byPartyMatchKind == PMWK_APPLY)       //���� ��Ƽ�� ���� 
				{
					//��Ƽ ��û�� �������� ���� ó��
					CTWS_PMATCH_REQUEST_RESULT pPacket;        //��Ī �Ƿ� ��� ���� ���� ��� 
					pPacket.bResult = m_byPMatchResultFalse;
					g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
					SetActive(FALSE);

				}
				else if(m_byPartyMatchKind == PMWK_TAKE)   //�ڽ��� �������� 
				{
					//��Ƽ�� ���� �޼����� �������� ���� ó��
					CTWS_PMATCH_REQUEST_RESULT pPacket;        //��Ī �Ƿ� ��� ���� ���� ��� 
					pPacket.bResult = m_byPMatchResultFalse;
					g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
					SetActive(FALSE);

				}

				SetActive(FALSE);
/*				CTWS_PARTY_JOIN pPacketSend;
				pPacketSend.bErrCode	= 2;
				pPacketSend.dwUserIndex	= m_dwUserIndex;
				__lstrcpyn(pPacketSend.szName, m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
				g_pNet->SendMsg((char*)&pPacketSend, pPacketSend.GetPacketSize(), SERVER_INDEX_WORLD);

				SetActive(FALSE);	
				
				char szInfo[0xff] = {0,};
				wsprintf(szInfo, g_Message[ETC_MESSAGE359].szMessage, m_szName);	// MSG_ID : 359 ; %s ���� ��Ƽ ��û�� �����ϼ̽��ϴ�.
				DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);
				*/
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_PARTYMATCHAING_CLOSE1, FALSE);
				SetRender(SPR_OBJ_PARTYMATCHAING_CLOSE2, TRUE);
			}						
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 3:			
		{
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 4://������ư
		{			
			if(eMouseCheck==MOUSE_UP)
			{
				if(m_byPartyMatchKind == PMWK_APPLY)        //���� ��Ƽ�� ���� 
				{
					//��Ƽ ��û�� �������� ���� ó��
					g_pMainPlayer->m_bPartyLeader = 0;
					CTWS_PMATCH_REQUEST_RESULT pPacket;        //��Ī �Ƿ� ��� ���� ���� ��� 
					pPacket.bResult = m_byPMatchResultTrue;
					g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
                    SetActive(FALSE);

				}
				else if(m_byPartyMatchKind == PMWK_TAKE)    //�ڽ��� �������� 
				{
					//��Ƽ�� ���� �޼����� �������� ���� ó��
					g_pMainPlayer->m_bPartyLeader = 1;
					CTWS_PMATCH_REQUEST_RESULT pPacket;        //��Ī �Ƿ� ��� ���� ���� ��� 
					pPacket.bResult = m_byPMatchResultTrue;
					g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
					SetActive(FALSE);

				}
/*				CTWS_PARTY_APPROVE pPacket;
				pPacket.dwUserIndex	= m_dwUserIndex;
				__lstrcpyn(pPacket.szName, m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
				g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
				SetActive(FALSE);
				*/

				}
			else if(eMouseCheck==MOUSE_DOWN)
			{				
				SetRender(SPR_OBJ_PARTYMATCHAING_OK1, FALSE);
				SetRender(SPR_OBJ_PARTYMATCHAING_OK2, FALSE);
				SetRender(SPR_OBJ_PARTYMATCHAING_OK3, TRUE);
				m_bBtn[0] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(SPR_OBJ_PARTYMATCHAING_OK1, FALSE);
				SetRender(SPR_OBJ_PARTYMATCHAING_OK2, TRUE);
				SetRender(SPR_OBJ_PARTYMATCHAING_OK3, FALSE);				
				m_bBtn[0] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 5://������ư
		{			
			if(eMouseCheck==MOUSE_UP)
			{
				if(m_byPartyMatchKind == PMWK_APPLY)          //���� ��Ƽ�� ���� 
				{
					//��Ƽ ��û�� �������� ���� ó��
					CTWS_PMATCH_REQUEST_RESULT pPacket;        //��Ī �Ƿ� ��� ���� ���� ��� 
					pPacket.bResult = m_byPMatchResultFalse;
					g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
					SetActive(FALSE);

				}
				else if(m_byPartyMatchKind == PMWK_TAKE)      //�ڽ��� �������� 
				{
					//��Ƽ�� ���� �޼����� �������� ���� ó��
					CTWS_PMATCH_REQUEST_RESULT pPacket;        //��Ī �Ƿ� ��� ���� ���� ��� 
					pPacket.bResult = m_byPMatchResultFalse;
					g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
					SetActive(FALSE);

				}
				// �̹� ���� �޼����� ���� ��� //
/*				CTWS_PARTY_JOIN pPacketSend;
				pPacketSend.bErrCode	= 2;
				pPacketSend.dwUserIndex	= m_dwUserIndex;
				__lstrcpyn(pPacketSend.szName, m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
				g_pNet->SendMsg((char*)&pPacketSend, pPacketSend.GetPacketSize(), SERVER_INDEX_WORLD);
				SetActive(FALSE);				
								
				char szInfo[0xff] = {0,};
				wsprintf(szInfo, g_Message[ETC_MESSAGE359].szMessage, m_szName);	// MSG_ID : 359 ; %s ���� ��Ƽ ��û�� �����ϼ̽��ϴ�.
				DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);

				return CInterface::GetInstance()->GetSetWnd();
				*/

			}
			else if(eMouseCheck==MOUSE_DOWN)
			{						
				SetRender(SPR_OBJ_PARTYMATCHAING_CANCEL1, FALSE);
				SetRender(SPR_OBJ_PARTYMATCHAING_CANCEL2, FALSE);
				SetRender(SPR_OBJ_PARTYMATCHAING_CANCEL3, TRUE);
				m_bBtn[1] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(SPR_OBJ_PARTYMATCHAING_CANCEL1, FALSE);
				SetRender(SPR_OBJ_PARTYMATCHAING_CANCEL2, TRUE);
				SetRender(SPR_OBJ_PARTYMATCHAING_CANCEL3, FALSE);
				m_bBtn[1] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}		
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_PARTYMATCHAING_CLOSE1, TRUE);
		SetRender(SPR_OBJ_PARTYMATCHAING_CLOSE2, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
		return CInterface::GetInstance()->GetSetWnd();

	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();		

	return 0;
}

void CPartyMatchingWnd::SetPartyMatchingWndData(PARTY_MATCHING_DATA MatchData)
{
	m_byPartyMatchKind = MatchData.byKind;

	char szTmp[50];
	ZeroMemory(szTmp, sizeof(szTmp));
	switch(MatchData.byClass)
	{
	case CLASS_TYPE_WARRIOR:
		wsprintf(szTmp, g_Message[ETC_MESSAGE7].szMessage );
		break;
	case CLASS_TYPE_PRIEST:
		wsprintf(szTmp, g_Message[ETC_MESSAGE9].szMessage );
		break;
	case CLASS_TYPE_SUMMONER:
		wsprintf(szTmp, g_Message[ETC_MESSAGE10].szMessage );
		break;
	case CLASS_TYPE_HUNTER:
		wsprintf(szTmp, g_Message[ETC_MESSAGE11].szMessage );
		break;
	case CLASS_TYPE_WIZARD:
		wsprintf(szTmp, g_Message[ETC_MESSAGE12].szMessage );
		break;
	}

	if(MatchData.byKind == PMWK_TAKE)		  //�ڽ��� �������� 
	{//�޼���cdb
//		wsprintf(m_szInfo[0], "%s(%s, Lv %d) �Կ��Լ�", MatchData.szName, szTmp, MatchData.nLevel );
//		wsprintf(m_szInfo[1], "��Ƽ ��û�� �Խ��ϴ�. �����Ͻðڽ��ϱ�?" );

		wsprintf(m_szInfo[0],  g_Message[ETC_MESSAGE1282].szMessage, MatchData.szName, szTmp, MatchData.nLevel );
		wsprintf(m_szInfo[1],  g_Message[ETC_MESSAGE1283].szMessage);




	}
	else if(MatchData.byKind == PMWK_APPLY)   //���� ��Ƽ�� ���� 
	{
//		wsprintf(m_szInfo[0], "%s(%s, Lv %d) ���� ��Ƽ��" ,MatchData.szName, szTmp, MatchData.nLevel );
//		wsprintf(m_szInfo[1], "���� �޽����� �����ðڽ��ϱ�?" );

		wsprintf(m_szInfo[0],  g_Message[ETC_MESSAGE1281].szMessage, MatchData.szName, szTmp, MatchData.nLevel );
		wsprintf(m_szInfo[1],  g_Message[ETC_MESSAGE1283].szMessage);


	}
}
//======================================================//
// End.													//
//======================================================//
