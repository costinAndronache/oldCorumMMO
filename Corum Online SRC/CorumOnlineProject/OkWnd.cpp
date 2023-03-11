//======================================================//
// Code by Jang.							2002.12.23	//
//======================================================//
#include "OkWnd.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "DSMiniMap.h"
#include "Chat.h"
#include "DUNGEON_DATA_EX.h"
#include "ItemTradeShopWnd.h"
#include "InputManager.h"
#include "DungeonTable.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "WorldProcess.h"
#include "InitGame.h"
#include "CodeFun.h"
#include "ItemNative.h"


COkWnd* COkWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
COkWnd::COkWnd()
{ 
	m_byRenderType	= 0;	
	m_bPointer		= FALSE;		
	m_dwDungeonID	= 0;
	m_bBtn[0]		= FALSE;
	m_bBtn[1]		= FALSE;
}

COkWnd::~COkWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL COkWnd::Init()
{	
/*	InsertData(SPR_OBJ_OK_CREATE_WND, SPR_INTERFACE_OKWND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_OK_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_OK_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_OK_OK1, SPR_INTERFACE_OK1, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_OK_OK2, SPR_INTERFACE_OK2, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_OK_OK3, SPR_INTERFACE_OK3, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_OK_CANCEL1, SPR_INTERFACE_CANCEL1, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_OK_CANCEL2, SPR_INTERFACE_CANCEL2, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_OK_CANCEL3, SPR_INTERFACE_CANCEL3, 189, 110, 1.0f, 1.0f, 2);
	
	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(121, 110, 185, 126, 4, CHECK_BTN);
	InsertCheckInterface(189, 110, 253, 126, 5, CHECK_BTN);
*/
	m_bInit	= TRUE;

	return TRUE;
}

void COkWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void COkWnd::SetOrder()
{
}

void COkWnd::RenderText()
{
/*
	int nOrder = __ORDER_INTERFACE_START__ + m_byOrder*10;

	if(m_byRenderType==0)
	{
		wsprintf(m_szInfo, g_Message[ETC_MESSAGE612].szMessage); // "��Ż"
		m_rtPos.left	= (LONG)m_fPosX+25;
#if IS_JAPAN_LOCALIZING()
		m_rtPos.right	= (LONG)m_fPosX+85;
#else
		m_rtPos.right	= (LONG)m_fPosX+55;
#endif
		m_rtPos.top		= (LONG)m_fPosZ+6;
		m_rtPos.bottom	= (LONG)m_fPosZ+20;
		
		g_pGeometry->RenderFont(GetFont(), m_szInfo, lstrlen(m_szInfo), &m_rtPos, 0xffffffff,
						CHAR_CODE_TYPE_ASCII, nOrder+1, 0);

		if(m_bType==0)
			wsprintf(m_szInfo, g_Message[ETC_MESSAGE613].szMessage); // "��ǥ�� ���� �Ͻðڽ��ϱ�?"
		else if(m_bType==1 || m_bType==2)
			wsprintf(m_szInfo, g_Message[ETC_MESSAGE614].szMessage); // "��Ż�� ����Ͻðڽ��ϱ�?"

		m_rtPos.left	= (LONG)m_fPosX+15;
		m_rtPos.right	= (LONG)m_fPosX+250;
		m_rtPos.top		= (LONG)m_fPosZ+40;
		m_rtPos.bottom	= (LONG)m_fPosZ+54;
		
		g_pGeometry->RenderFont(GetFont(), m_szInfo, lstrlen(m_szInfo), &m_rtPos, 0xffffffff,
						CHAR_CODE_TYPE_ASCII, nOrder+1, 0);
	}
	else
	{
		if (m_pDungeon)
		{
		
			wsprintf(m_szInfo, g_Message[ETC_MESSAGE615].szMessage, m_pDungeon->m_szDungeonName); // "[%s] �����"
			m_rtPos.left	= (LONG)m_fPosX+25;
#if IS_JAPAN_LOCALIZING()
			m_rtPos.right	= (LONG)m_fPosX+250;
#else
			m_rtPos.right	= (LONG)m_fPosX+55;
#endif
			m_rtPos.top		= (LONG)m_fPosZ+6;
			m_rtPos.bottom	= (LONG)m_fPosZ+20;
			
			g_pGeometry->RenderFont(GetFont(), m_szInfo, lstrlen(m_szInfo), &m_rtPos, 0xffffffff,
							CHAR_CODE_TYPE_ASCII, nOrder+1, 0);

			wsprintf(m_szInfo, g_Message[ETC_MESSAGE616].szMessage, m_pDungeon->GetEntrancePay()); // "%u���� ����Ḧ"

			m_rtPos.left	= (LONG)m_fPosX+15;
			m_rtPos.right	= (LONG)m_fPosX+250;
			m_rtPos.top		= (LONG)m_fPosZ+40;
			m_rtPos.bottom	= (LONG)m_fPosZ+54;
			
			g_pGeometry->RenderFont(GetFont(), m_szInfo, lstrlen(m_szInfo), &m_rtPos, 0xffffffff,
							CHAR_CODE_TYPE_ASCII, nOrder+1, 0);
			
			wsprintf(m_szInfo, g_Message[ETC_MESSAGE617].szMessage); // "�����ϰ� �����Ͻðڽ��ϱ�?"

			m_rtPos.left	= (LONG)m_fPosX+35;
			m_rtPos.right	= (LONG)m_fPosX+250;
			m_rtPos.top		= (LONG)m_fPosZ+57;
			m_rtPos.bottom	= (LONG)m_rtPos.top+17;
			
			g_pGeometry->RenderFont(GetFont(), m_szInfo, lstrlen(m_szInfo), &m_rtPos, 0xffffffff,
							CHAR_CODE_TYPE_ASCII, nOrder+1, 0);
		}
	}
	*/
	if (m_bType&__OKWND_TYPE_DUNGEON_ENTER)
	{
		DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(m_dwDungeonID);
		if (pDungeon)
		{
#if IS_JAPAN_LOCALIZING()
			
			if(lstrlen(pDungeon->m_szSchoolName))
				lstrcpy(m_szInfo, pDungeon->m_szSchoolName);
			else
				lstrcpy(m_szInfo, pDungeon->m_szDungeonName);

#else

			lstrcpy(m_szInfo, pDungeon->m_szDungeonName);

#endif
			 
			RenderFont(m_szInfo, m_fPosX+25, m_fPosX+95, m_fPosZ+6, m_fPosZ+20, GetStartOrder()+1);
			
			lstrcpy(m_szInfo, g_Message[ETC_MESSAGE897].szMessage); // "�����Ͻðڽ��ϱ�?"

#if IS_JAPAN_LOCALIZING()
// ���� ����� ���� ���� ǥ��
// �Ϻ��� �켱 ���� minjin. 2004. 10. 05.

			if ( pDungeon->m_byEntererMaxLevel != 255 || pDungeon->m_byEntererMinLevel != 1 )
			{
				char szTemp[64] = {0,};
				wsprintf( szTemp, "  [ Lv : %d - %d ]", pDungeon->m_byEntererMinLevel, pDungeon->m_byEntererMaxLevel );
				lstrcat( m_szInfo, szTemp );
			}
#endif

			RenderFont(m_szInfo, m_fPosX+15, m_fPosX+250, m_fPosZ+40, m_fPosZ+54, GetStartOrder()+1);			
		}
	}
	else if(m_bType&__OKWND_TYPE_UNPACK_PRESENT_BOX)
	{
		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE895].szMessage); // "�±ر�"
		RenderFont(m_szInfo, m_fPosX+25, m_fPosX+95, m_fPosZ+6, m_fPosZ+20, GetStartOrder()+1);

		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE896].szMessage); // "������ �±ر⸦ ���ĺ��ðڽ��ϱ�?"
		RenderFont(m_szInfo, m_fPosX+15, m_fPosX+250, m_fPosZ+40, m_fPosZ+54, GetStartOrder()+1);		
	}
	else if (m_bType & __OKWND_TYPE_POTAL_ENTRANCE)
	{
		DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(m_dwDungeonID);
		
		if (pDungeon)
		{		
			wsprintf(m_szInfo, g_Message[ETC_MESSAGE615].szMessage, pDungeon->m_szDungeonName); // "[%s] �����"
			RenderFont(m_szInfo, m_fPosX+25, m_fPosX+250, m_fPosZ+6, m_fPosZ+20, GetStartOrder()+1);
			
			wsprintf(m_szInfo, g_Message[ETC_MESSAGE616].szMessage, pDungeon->GetEntrancePay()); // "%u���� ����Ḧ"
			RenderFont(m_szInfo, m_fPosX+25, m_fPosX+250, m_fPosZ+40, m_fPosZ+54, GetStartOrder()+1);			
			
			lstrcpy(m_szInfo, g_Message[ETC_MESSAGE617].szMessage); // "�����ϰ� �����Ͻðڽ��ϱ�?"
			RenderFont(m_szInfo, m_fPosX+35, m_fPosX+250, m_fPosZ+57, m_fPosZ+71, GetStartOrder()+1);			
		}
	}
	else if(m_bType & __OKWND_TYPE_POTAL_VILLAGE_USE ||
			m_bType & __OKWND_TYPE_POTAL_DUNGEON_USE ||
			m_bType & __OKWND_TYPE_POTAL_SAVE)
	{
		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE612].szMessage); // "��Ż"
		
#if IS_JAPAN_LOCALIZING()

		int nRight = (LONG)m_fPosX+85;

#else

		int nRight = (LONG)m_fPosX+55;

#endif
		
		RenderFont(m_szInfo, (int)m_fPosX+25, nRight, (int)m_fPosZ+6, (int)m_fPosZ+20, GetStartOrder()+1);
		
		if(m_bType & __OKWND_TYPE_POTAL_SAVE)
			lstrcpy(m_szInfo, g_Message[ETC_MESSAGE613].szMessage); // "��ǥ�� ���� �Ͻðڽ��ϱ�?"
		else
			lstrcpy(m_szInfo, g_Message[ETC_MESSAGE614].szMessage); // "��Ż�� ����Ͻðڽ��ϱ�?"

		RenderFont(m_szInfo, m_fPosX+15, m_fPosX+250, m_fPosZ+40, m_fPosZ+54, GetStartOrder()+1);
	}
}

void COkWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(bActive==FALSE)
	{
		m_byRenderType = 0;
		m_bSoundChk	= FALSE;
		ShowSpriteAll();

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd(BYTE(m_nIndex));

		SetPos(384, 320);

		SetRender(SPR_OBJ_OK_CREATE_WND, TRUE);
		SetRender(SPR_OBJ_OK_OK1, TRUE);
		SetRender(SPR_OBJ_OK_CANCEL1, TRUE);
		SetRender(SPR_OBJ_OK_CLOSE1, TRUE);	

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int COkWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(m_bBtn[0] && nRt!=4)
	{
		SetRender(SPR_OBJ_OK_OK1, TRUE);
		SetRender(SPR_OBJ_OK_OK2, FALSE);
		SetRender(SPR_OBJ_OK_OK3, FALSE);								
		m_bBtn[0] = FALSE;
	}

	if(m_bBtn[1] && nRt!=5)
	{
		SetRender(SPR_OBJ_OK_CANCEL1, TRUE);
		SetRender(SPR_OBJ_OK_CANCEL2, FALSE);
		SetRender(SPR_OBJ_OK_CANCEL3, FALSE);
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
				SetRender(SPR_OBJ_OK_CLOSE1, FALSE);
				SetRender(SPR_OBJ_OK_CLOSE2, TRUE);
			}						
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 3:			
		{
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 4:	// Ȯ��
		{			
			if(eMouseCheck==MOUSE_UP)
			{				
				// Process //
				if (m_bType&__OKWND_TYPE_DUNGEON_ENTER)
				{
					DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(m_dwDungeonID);
					if (!pDungeon)	return CInterface::GetInstance()->GetSetWnd();

					DungeonEnter((WORD)pDungeon->m_dwID);
				}
				else if(m_bType&__OKWND_TYPE_UNPACK_PRESENT_BOX)
				{	
					CTDS_ITEM_PICKUP pPacket;
					pPacket.bSectionNum	= 1;
					pPacket.i64ItemID	= 0;

					// �������� �̺�Ʈ �ּ� 
					// 2005.02.02 �迵�� 
					SetItemPacket(&pPacket, 77, m_bZipCode, 0, 0, 0);

					// �κ����� ����Ѱų�, ��Ʈ���� ����Ѱų� ������
					pPacket.bEqiupCode2 = (BYTE)m_dwDungeonID;	 

					g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);

					if(g_ItemMoveManager.GetNewItemMoveMode())
					{
						g_ItemUsedManager.SendItemUsedNativePacket(ITEM_NATIVE_INV_SMALL,m_bZipCode,0xff);
					}
				}
				else if (m_bType&__OKWND_TYPE_POTAL_ENTRANCE)
				{
					DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(m_dwDungeonID);

					if(!pDungeon)
						return CInterface::GetInstance()->GetSetWnd();
					
					if (g_pMainPlayer->m_dwMoney >= pDungeon->GetEntrancePay())
					{
						if (m_bType == __OKWND_TYPE_POTAL_ENTRANCE)
						{
							// ���忡�� ����� ���� �߱���.							
							DungeonEnter((WORD)pDungeon->m_dwID);
						}
						else
						{
							// ��Ż �ҷ��� �ϴµ� ����� ok �޲�
							m_bType &= ~__OKWND_TYPE_POTAL_ENTRANCE;
							return CInterface::GetInstance()->GetSetWnd();
						}
					}
					else
					{
						// "karz�� �����մϴ�."
						DisplayMessageAdd(g_Message[ETC_MESSAGE618].szMessage, 0xFFFF2CFF); 
					}					
				}
				else if(m_bType&__OKWND_TYPE_POTAL_DUNGEON_USE)
				{
					CItemTradeShopWnd* pItemTradeShopWnd = CItemTradeShopWnd::GetInstance();

					if(pItemTradeShopWnd->GetActive())
					{
						// "��ȯ�߿��� ����Ҽ� �����ϴ�."
						DisplayMessageAdd(g_Message[ETC_MESSAGE179].szMessage, 0xffff2cff); 
						SetActive(FALSE);
						return CInterface::GetInstance()->GetSetWnd();
					}
					
					CMiniMapWnd* pMiniMapWnd = CMiniMapWnd::GetInstance();

					if(g_pMainPlayer->m_pInv_Small[m_bZipCode].GetID()==__ITEM_PORTAL_INDEX__)
					{
						// �ֵ��� ����� ī�� ��Ʈ ���� ��Ż �ȵǴ� ���� ����
						// �迵�� 2005.02.07
						for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
						{
							if(g_pMainPlayer->m_pInv_Small[i].GetID()==__ITEM_PORTALUSED_INDEX__)
							{
								if(	(pMiniMapWnd->m_wMapId != g_pMainPlayer->m_pInv_Small[m_bZipCode].m_Item_Zodiac.wMapId) ||
									(pMiniMapWnd->m_byMapLayer!=g_pMainPlayer->m_pInv_Small[m_bZipCode].m_Item_Zodiac.bLayer))
								{
									CTDS_ITEM_PICKUP pPacket;
									pPacket.bSectionNum	= 1;
									pPacket.i64ItemID	= 0;
									SetItemPacket(&pPacket, 51, m_bZipCode, i, 0, 0);
									g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);

									if(g_ItemMoveManager.GetNewItemMoveMode())
									{
										g_ItemUsedManager.SendItemUsedNativePacket(ITEM_NATIVE_INV_SMALL,m_bZipCode,i);
									}

									SetActive(FALSE);
									return CInterface::GetInstance()->GetSetWnd();	
								}
								else
								{
									// "���� ���������� �̵��� �Ҽ��� �����ϴ�."
									DisplayMessageAdd(g_Message[ETC_MESSAGE503].szMessage, 0xFFFF2CFF); 
									m_bClose = FALSE;
									SetActive(FALSE);
									return CInterface::GetInstance()->GetSetWnd();
								}
							}
						}

						// "����� �� �ִ� �������� �����ϴ�."			
						DisplayMessageAdd(g_Message[ETC_MESSAGE52].szMessage, 0xFFFF2CFF); 
					}
					else
					{
						// "����� �� �ִ� �������� �����ϴ�."
						DisplayMessageAdd(g_Message[ETC_MESSAGE52].szMessage, 0xFFFF2CFF); 
					}
				}
				else if(m_bType&__OKWND_TYPE_POTAL_SAVE)
				{
					CItemTradeShopWnd* pItemTradeShopWnd = CItemTradeShopWnd::GetInstance();

					if(pItemTradeShopWnd->GetActive())
					{
						// "��ȯ�߿��� ����Ҽ� �����ϴ�."
						DisplayMessageAdd(g_Message[ETC_MESSAGE179].szMessage, 0xffff2cff); 
						SetActive(FALSE);
						return CInterface::GetInstance()->GetSetWnd();
					}

					if(g_pMainPlayer->m_pInv_Small[m_bZipCode].m_wItemID==__ITEM_PORTAL_INDEX__)
					{							
						if(g_pMainPlayer->m_pInv_Small[m_bZipCode].m_Item_Zodiac.bType==0)
						{
							CTDS_ITEM_PICKUP pPacket;
							pPacket.bSectionNum	= 1;
							pPacket.i64ItemID	= 0;
							SetItemPacket(&pPacket, 50, m_bZipCode, 0, 0, 0);
							g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);

							if(g_ItemMoveManager.GetNewItemMoveMode())
							{
								g_ItemUsedManager.SendItemUsedNativePacket(ITEM_NATIVE_INV_SMALL,m_bZipCode,0xff);
							}
							
							SetActive(FALSE);

							return CInterface::GetInstance()->GetSetWnd();
						}
					}
					else
					{
						// "�������� �����ϴ�."
						DisplayMessageAdd(g_Message[ETC_MESSAGE144].szMessage, 0xFF21DBFF); 
					}
				}
				else if(m_bType & __OKWND_TYPE_POTAL_VILLAGE_USE)
				{
					CItemTradeShopWnd* pItemTradeShopWnd = CItemTradeShopWnd::GetInstance();

					if(pItemTradeShopWnd->GetActive())
					{
						// "��ȯ�߿��� ����Ҽ� �����ϴ�."
						DisplayMessageAdd(g_Message[ETC_MESSAGE179].szMessage, 0xffff2cff); 
						SetActive(FALSE);
						return CInterface::GetInstance()->GetSetWnd();
					}

					// ���� ��ȯ ��ũ�� �߰� 2005.02.14 �迵�� 
					WORD wItemID = g_pMainPlayer->m_pInv_Small[m_bZipCode].GetID();
					if(wItemID >= __ITEM_PORTAL_VILL1__ && wItemID <= __ITEM_PORTAL_VILL6__)
					{
						CMiniMapWnd* pMiniMapWnd = CMiniMapWnd::GetInstance();

						if(	(pMiniMapWnd->m_wMapId!=g_pMainPlayer->m_pInv_Small[m_bZipCode].m_Item_Zodiac.wMapId) ||
							(pMiniMapWnd->m_byMapLayer!=g_pMainPlayer->m_pInv_Small[m_bZipCode].m_Item_Zodiac.bLayer))
						{
							CTDS_ITEM_PICKUP pPacket;
							pPacket.bSectionNum	= 1;
							pPacket.i64ItemID	= 0;
							SetItemPacket(&pPacket, 52, m_bZipCode, 0, 0, 0);
							g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);
							
							if(g_ItemMoveManager.GetNewItemMoveMode())
							{
								g_ItemUsedManager.SendItemUsedNativePacket(ITEM_NATIVE_INV_SMALL,m_bZipCode,0xff);
							}

							SetActive(FALSE);
							return CInterface::GetInstance()->GetSetWnd();
						}
						else
						{
							// "���� ���������� �̵��Ҽ��� �����ϴ�."
							DisplayMessageAdd(g_Message[ETC_MESSAGE503].szMessage, 0xFFFF2CFF); 
						}
					}
					else
					{
						// "�������� �����ϴ�."
						DisplayMessageAdd(g_Message[ETC_MESSAGE144].szMessage, 0xFF21DBFF); 
					}
				}
				
				SetActive(FALSE);				
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(SPR_OBJ_OK_OK1, FALSE);
				SetRender(SPR_OBJ_OK_OK2, TRUE);
				SetRender(SPR_OBJ_OK_OK3, FALSE);
				m_bBtn[0] = TRUE;
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_OK_OK1, FALSE);
				SetRender(SPR_OBJ_OK_OK2, FALSE);
				SetRender(SPR_OBJ_OK_OK3, TRUE);				
				m_bBtn[0] = TRUE;
			}
			
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 5:			//��� 
		{			
			if(eMouseCheck==MOUSE_UP)
				SetActive(FALSE);
			else if(eMouseCheck==MOUSE_DOWN)
			{								
				SetRender(SPR_OBJ_OK_CANCEL1, FALSE);
				SetRender(SPR_OBJ_OK_CANCEL2, FALSE);
				SetRender(SPR_OBJ_OK_CANCEL3, TRUE);
				m_bBtn[1] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{				
				SetRender(SPR_OBJ_OK_CANCEL1, FALSE);
				SetRender(SPR_OBJ_OK_CANCEL2, TRUE);
				SetRender(SPR_OBJ_OK_CANCEL3, FALSE);
				m_bBtn[1] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}
					
	if(MouseUp())
	{
		SetRender(SPR_OBJ_OK_CLOSE1, TRUE);
		SetRender(SPR_OBJ_OK_CLOSE2, FALSE);		
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
