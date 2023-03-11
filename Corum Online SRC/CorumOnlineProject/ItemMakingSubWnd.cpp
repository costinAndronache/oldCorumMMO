#include "ItemMakingSubWnd.h"
#include "ItemMakingWnd.h"
#include "GameControl.h"
#include "User.h"
#include "QuantityWnd.h"
#include "dungeon_data_ex.h"
#include "hashtable.h"
#include "../CommonServer/CommonHeader.h"
#include "Chat.h"
#include "UserInterface.h"
#include "Message.h"
#include "DungeonTable.h"
#include "Interface.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "CodeFun.h"

CItemMakingSubWnd* CItemMakingSubWnd::c_pThis = NULL;

CItemMakingSubWnd::CItemMakingSubWnd()
{
	memset(&m_rtPos,0,sizeof(m_rtPos));
	m_byItemMakingProcess	= 0;
	m_dwNpcID				= 0;
	m_bBtn					= FALSE;
}

CItemMakingSubWnd::~CItemMakingSubWnd()
{

}


BOOL CItemMakingSubWnd::Init()
{
/*
	InsertData(SPR_OBJ_ITEM_MAKING_SUB_WND, SPR_INTERFACE_OKWND2, 0, 0, 1.0f, 1.0f, 0);
	InsertData(BUTTON_OBJ_ITEMMAKING_SUB_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_ITEMMAKING_SUB_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_ITEMMAKING_SUB_OK1, SPR_INTERFACE_OK1, 189, 110, 1.0f, 1.0f, 2);
	InsertData(BUTTON_OBJ_ITEMMAKING_SUB_OK2, SPR_INTERFACE_OK2, 189, 110, 1.0f, 1.0f, 2);
	InsertData(BUTTON_OBJ_ITEMMAKING_SUB_OK3, SPR_INTERFACE_OK3, 189, 110, 1.0f, 1.0f, 2);

	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(189, 110, 253, 126, 4, CHECK_BTN);
*/
	m_bInit	= TRUE;

	return TRUE;
}


void CItemMakingSubWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}


void CItemMakingSubWnd::RenderText()
{
	RenderFont(g_NPCTable[m_dwNpcID].szName, m_fPosX+25, m_fPosX+95, m_fPosZ+6, m_fPosZ+20, GetStartOrder()+1);

	if(m_byItemMakingProcess == ITEM_MAKING_PROCESS_START)
	{
		// "ȣ��.. �̰��� ��� ���ε� �������̱�."		
		RenderFont(g_Message[ETC_MESSAGE1215].szMessage, m_fPosX+15, m_fPosX+250, m_fPosZ+40, m_fPosZ+54, GetStartOrder()+1);

		// "������ Ǯ�� ������ ��ġ�ڳ�?"		
		RenderFont(g_Message[ETC_MESSAGE1216].szMessage, m_fPosX+15, m_fPosX+250, m_fPosZ+57, m_fPosZ+71, GetStartOrder()+1);
	}
	else if	(m_byItemMakingProcess == ITEM_MAKING_PROCESS_READY)
	{
		// "����..�Ѱ��� �˾ƵѰ� �ֳ�."
		RenderFont(g_Message[ETC_MESSAGE1217].szMessage, m_fPosX+15, m_fPosX+250, m_fPosZ+40, m_fPosZ+54, GetStartOrder()+1);
		
		// "������ �Ҿ����Ͽ� �����ϴٰ� ��ĩ�ϸ�"		
		RenderFont(g_Message[ETC_MESSAGE1218].szMessage, m_fPosX+15, m_fPosX+250, m_fPosZ+57, m_fPosZ+71, GetStartOrder()+1);

		// "�Ҹ�ɼ��� �ֳ�.�׷��� �غ��ٰ�?"		
		RenderFont(g_Message[ETC_MESSAGE1219].szMessage, m_fPosX+15, m_fPosX+250, m_fPosZ+74, m_fPosZ+88, GetStartOrder()+1);
		
		// "�Ҹ�ǵ� ��å���� ���� ���Գ�."		
		RenderFont(g_Message[ETC_MESSAGE1220].szMessage, m_fPosX+15, m_fPosX+250, m_fPosZ+91, m_fPosZ+105, GetStartOrder()+1);
	}
	else if	(m_byItemMakingProcess == ITEM_MAKING_PROCESS_SUCCESS)
	{
		// "�� �ϼ�ü�� ���������."
		RenderFont(g_Message[ETC_MESSAGE1221].szMessage, m_fPosX+15, m_fPosX+250, m_fPosZ+40, m_fPosZ+54, GetStartOrder()+1);
		
		// "���� �������̷α���.."		
		RenderFont(g_Message[ETC_MESSAGE1222].szMessage, m_fPosX+15, m_fPosX+250, m_fPosZ+57, m_fPosZ+71, GetStartOrder()+1);

		// "�ڳ� ���� ���� ����."
		RenderFont(g_Message[ETC_MESSAGE1223].szMessage, m_fPosX+15, m_fPosX+250, m_fPosZ+74, m_fPosZ+88, GetStartOrder()+1);
	}
	else if	(m_byItemMakingProcess == ITEM_MAKING_PROCESS_FAIL)
	{
		// "�̷�..���տ� �����߳�."	
		RenderFont(g_Message[ETC_MESSAGE1224].szMessage, m_fPosX+15, m_fPosX+250, m_fPosZ+40, m_fPosZ+54, GetStartOrder()+1);
		
		// "�����ϰ� ��谡 �ɷ��־�"		
		RenderFont(g_Message[ETC_MESSAGE1225].szMessage, m_fPosX+15, m_fPosX+250, m_fPosZ+57, m_fPosZ+71, GetStartOrder()+1);

		// "�������� �Ҹ��߾�."		
		RenderFont(g_Message[ETC_MESSAGE1226].szMessage, m_fPosX+15, m_fPosX+250, m_fPosZ+74, m_fPosZ+88, GetStartOrder()+1);
	}	
}

void CItemMakingSubWnd::SetOrder()
{
	
}


void CItemMakingSubWnd::SetActive(BOOL bActive)
{
	m_bActive = bActive;

	if(bActive==FALSE)
	{
		ShowSpriteAll();
		
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_ITEM_MAKING_SUB_WND, bActive);
		SetRender(BUTTON_OBJ_ITEMMAKING_SUB_CLOSE1, bActive);
		SetRender(BUTTON_OBJ_ITEMMAKING_SUB_OK1, bActive);
//		SetRender(BUTTON_OBJ_ITEMMAKING_SUB_OK2, bActive);
//		SetRender(BUTTON_OBJ_ITEMMAKING_SUB_OK3, bActive);		

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}


int CItemMakingSubWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;
	
	int nRt = GetChk();

	if(m_bBtn && nRt != 4)
	{
		SetRender(BUTTON_OBJ_ITEMMAKING_SUB_OK1, TRUE);
		SetRender(BUTTON_OBJ_ITEMMAKING_SUB_OK2, FALSE);
		SetRender(BUTTON_OBJ_ITEMMAKING_SUB_OK3, FALSE);
		m_bBtn = FALSE;
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

		// CHECK_CLOSE
		case 2:
			{			
				if(eMouseCheck == MOUSE_UP)
				{
					SetActive(FALSE);
					CItemMakingWnd* pItemMaking = CItemMakingWnd::GetInstance();
					pItemMaking->m_byProcess = 0;
				}
				else if(eMouseCheck == MOUSE_DOWN)
				{
					SetRender(BUTTON_OBJ_ITEMMAKING_SUB_CLOSE1, FALSE);
					SetRender(BUTTON_OBJ_ITEMMAKING_SUB_CLOSE2, TRUE);
				}
				
				SetMouseCheck(nRt, eMouseCheck);
			}
			break;

		// CHECK_MOVEWND
		case 3:			
			{
				SetMouseCheck(nRt, eMouseCheck);
			}
			break;

		// CHECK_BTN
		case 4:
			{
				if(eMouseCheck == MOUSE_UP)
				{
					SetRender(BUTTON_OBJ_ITEMMAKING_SUB_OK1, TRUE);
					SetRender(BUTTON_OBJ_ITEMMAKING_SUB_OK2, FALSE);
					SetRender(BUTTON_OBJ_ITEMMAKING_SUB_OK3, FALSE);

					if(m_byItemMakingProcess == ITEM_MAKING_PROCESS_START)
					{
						m_byItemMakingProcess++;
					}
					else if(m_byItemMakingProcess == ITEM_MAKING_PROCESS_READY)
					{
						CTDS_ITEMMAKING packet;
						g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);						
					}
					else if(m_byItemMakingProcess == ITEM_MAKING_PROCESS_SUCCESS	||
							m_byItemMakingProcess == ITEM_MAKING_PROCESS_FAIL)
					{
						m_byItemMakingProcess = 0;
						SetActive(FALSE);
					}
				}
				else if(eMouseCheck == MOUSE_OVER)
				{
					SetRender(BUTTON_OBJ_ITEMMAKING_SUB_OK1, FALSE);
					SetRender(BUTTON_OBJ_ITEMMAKING_SUB_OK2, TRUE);
					SetRender(BUTTON_OBJ_ITEMMAKING_SUB_OK3, FALSE);
					m_bBtn = TRUE;
				}
				else if(eMouseCheck == MOUSE_DOWN)
				{
					SetRender(BUTTON_OBJ_ITEMMAKING_SUB_OK1, FALSE);
					SetRender(BUTTON_OBJ_ITEMMAKING_SUB_OK2, FALSE);
					SetRender(BUTTON_OBJ_ITEMMAKING_SUB_OK3, TRUE);
					m_bBtn = TRUE;
				}
				
				SetMouseCheck(nRt, eMouseCheck);
			}
			break;
	}

	if(MouseUp())
	{
		SetRender(BUTTON_OBJ_ITEMMAKING_SUB_CLOSE1, TRUE);
		SetRender(BUTTON_OBJ_ITEMMAKING_SUB_CLOSE2, FALSE);		
	}
	
	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
	{
		return CInterface::GetInstance()->GetSetWnd();
	}
	
	if(IsReturn(nRt, eMouseCheck))
	{
		return CInterface::GetInstance()->GetSetWnd();	
	}

	return 0;
}
