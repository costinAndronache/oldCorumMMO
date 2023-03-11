//==============================================================//
// Code by Jang.									2003.08.01	//
//==============================================================//
#include "Shop.h"
#include "GameControl.h"
#include "Chat.h"
#include "PlayerShopWnd.h"
#include "Message.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "CodeFun.h"
#include "Filter.h"
#include "ItemNative.h"
#include "InputManager.h"


//==============================================================//
// ���� ���� ���� ���� �޼���.									//
//==============================================================//
void CmdShopFail(char* pMsg, DWORD dwLen)
{
	DSTC_SHOP_FAIL* pPacket = (DSTC_SHOP_FAIL*)pMsg;
	
	CPlayerShopWnd* pPlayerShopWnd = CPlayerShopWnd::GetInstance();

	if(pPacket->byType==1)
	{
		// "���������� ������ ������ �� �ֽ��ϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE758].szMessage, 0xFFFFC309); 
	}		
	else if(pPacket->byType==2 || pPacket->byType==3 || pPacket->byType==4)
	{
		// "���� ��ġ���� ������ ������ �� �����ϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE759].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==5)
	{
		// "�κ��丮�� �� ������ �����Ͽ� ����� �� �����ϴ�."	
		DisplayMessageAdd(g_Message[ETC_MESSAGE760].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==6)
	{
		// "�Ǹ��� �������� �������� �ʾҽ��ϴ�."		
		DisplayMessageAdd(g_Message[ETC_MESSAGE761].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==7)
	{
		// "������ ��ҵǾ����ϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE763].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==8)
	{
		// "������ ��ҵǾ����ϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE763].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==9)
	{
		// "�������� �������� �ʽ��ϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE764].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==10)
	{
		// "������ ǰ���� ����Ǿ����ϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE765].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==11)
	{
		// "ī������ �����Ͽ� ������ �� �����ϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE766].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==12)
	{
		// "���������� �ִ� ī���� �������� �ʰ��Ǿ� �Ǹ��� �� �����ϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE767].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==13)
	{
		// "�κ��丮�� ���������� �����Ͽ� ������ �� �����ϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE768].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==14)
	{
		// "��ȯ�߿��� ������ ���⸦ �Ҽ� �����ϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE772].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==15)
	{
		// "�����߿��� ��ȯ�� �Ҽ� �����ϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE773].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==16)
	{
		// "����߿��� ��ȯ�� �Ҽ� �����ϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE774].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==17)
	{
		// "��Ż �߿��� ��ȯ�� �Ҽ� �����ϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE775].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==18)
	{
		// "�ɱ��߿��� ��ȯ�� �Ҽ� �����ϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE797].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==19)
	{
		// "���������� �Ǹ����� ���ݺ������� ���Ű� �ȵ˴ϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE1042].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==20)
	{
		// "�ǸŰ����� �����Ǿ����� �ʽ��ϴ�."
		DisplayMessageAdd(g_Message[ETC_MESSAGE1256].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==21)
	{
		DisplayMessageAdd(g_Message[ETC_MESSAGE1426].szMessage, 0xFFFFC309); //�������� ������ �� �����ϴ�.
	}
	else if(pPacket->byType==22)
	{
		DisplayMessageAdd(g_Message[ETC_MESSAGE1427].szMessage, 0xFFFFC309); //�������Ī�ÿ��� ������ ���� �����ϴ�.
	}
	else if(pPacket->byType==0)
	{
		__lstrcpyn(g_pMainPlayer->m_szPlayerShopTitle
			, g_pMainPlayer->m_sPlayerShop.szTitle, MAX_REAL_PLAYER_SHOP_TITLE);
		pPlayerShopWnd->PlayerShopStart();
	}
}


//==============================================================//
// �������� ������ �ȷ��� ��� ó��.							//
//==============================================================//
void CmdPlayerShopSell(char* pMsg, DWORD dwLen)
{
	DSTC_PLAYERSHOP_SELL* pPacket = (DSTC_PLAYERSHOP_SELL*)pMsg;

	if( IsUltra() )
	{
		if( pPacket->byBuyRealCnt > 0 )
		{
			BYTE byCnt = g_pMainPlayer->m_sPlayerShop.cItem[pPacket->byZipCode].GetQuantity();
			g_pMainPlayer->m_sPlayerShop.cItem[pPacket->byZipCode].SetQuantity(BYTE(byCnt - pPacket->byBuyRealCnt));
		}
		else
		{
			memcpy(&g_pMainPlayer->m_sPlayerShop.cItem[pPacket->byZipCode], &pPacket->cItem, sizeof(CItem));

			if(g_pMainPlayer->m_sPlayerShop.cItem[pPacket->byZipCode].GetID()==0)
				g_pMainPlayer->m_sPlayerShop.m_dwCustomSellPrice[pPacket->byZipCode] = 0;
		}

		g_pMainPlayer->m_dwMoney = pPacket->dwMoney;
	}
	else
	{
		memcpy(&g_pMainPlayer->m_sPlayerShop.cItem[pPacket->byZipCode], &pPacket->cItem, sizeof(CItem));

		if(g_pMainPlayer->m_sPlayerShop.cItem[pPacket->byZipCode].GetID()==0)
			g_pMainPlayer->m_sPlayerShop.m_dwCustomSellPrice[pPacket->byZipCode] = 0;

		g_pMainPlayer->m_dwMoney = pPacket->dwMoney;
	}
}


//==============================================================//
// �������� �������� �����Ǿ����� ó��.							//
//==============================================================//
void CmdPlayerShopSoldOut(char* pMsg, DWORD dwLen)
{
//	DSTC_PLAYERSHOP_SOLDOUT* pPacket = (DSTC_PLAYERSHOP_SOLDOUT*)pMsg;

	g_byStatusMessenger	= 0;
						
	// ��Ŷ ������ //			
	CTWS_MESSENGER_STATUS pSendPacket;
	pSendPacket.byType		= 0;
	pSendPacket.dwUserIndex	= g_pMainPlayer->m_dwUserIndex;
	g_pNet->SendMsg((char*)&pSendPacket, pSendPacket.GetPacketSize(), SERVER_INDEX_WORLD);

	// Close //
	CPlayerShopWnd* pPlayerShopWnd = CPlayerShopWnd::GetInstance();

	if(pPlayerShopWnd->GetActive()==TRUE)
	{
		pPlayerShopWnd->SetRender(BUTTON_OBJ_PLAYERSHOP_OPEN1, TRUE);
		pPlayerShopWnd->SetRender(BUTTON_OBJ_PLAYERSHOP_CLOSE1, FALSE);
		pPlayerShopWnd->SetRender(BUTTON_OBJ_PLAYERSHOP_CLOSE2, FALSE);
		pPlayerShopWnd->SetRender(BUTTON_OBJ_PLAYERSHOP_CLOSE3, FALSE);
	}	

	pPlayerShopWnd->m_byMode = 0;
	g_pMainPlayer->SetPlayerShop(0);
}


//==============================================================//
// ���� ������ ó��.											//
//==============================================================//
void CmdPlayerShopItem(char* pMsg, DWORD dwLen)
{
	DSTC_PLAYERSHOP_ITEM* pPacket = (DSTC_PLAYERSHOP_ITEM*)pMsg;
	
	for(int i = 0; i < pPacket->bItemCount; i++)
	{
		int index	= pPacket->ItemNode[i].index; // ���� cItem�� �ε���.
		int nValue	= pPacket->ItemNode[i].cItem.GetID()/ITEM_DISTRIBUTE;		

		if(GetType((WORD)nValue)==ITEM_SMALL)
		{
			BYTE byCnt = Insert_SmallInvItem(&pPacket->ItemNode[i].cItem, 0, pPacket->ItemNode[i].cItem.GetQuantity());

			if(byCnt==0)
				memset(&g_pMainPlayer->m_sPlayerShop.cItem[index], 0, sizeof(CItem));
			else
				g_pMainPlayer->m_sPlayerShop.cItem[index].SetQuantity(byCnt);
		}
		else if(GetType((WORD)nValue)==ITEM_LARGE)
		{
			for(int j = 0; j < MAX_INV_LARGE_POOL; j++)
			{
				if(g_pMainPlayer->m_pInv_Large[j].GetID()==0)
				{
					memcpy(&g_pMainPlayer->m_pInv_Large[j], &pPacket->ItemNode[i].cItem, sizeof(CItem));
					memset(&g_pMainPlayer->m_sPlayerShop.cItem[index], 0, sizeof(CItem));
					break;
				}
			}
		}
	}
}


void CmdPlayerShopItem_ReturnInv(char* pMsg, DWORD dwLen)
{
	DSTC_PLAYERSHOPITEM_RETURN_INV* pPacket = (DSTC_PLAYERSHOPITEM_RETURN_INV*)pMsg;

	ITEM_NATIVE_INFO	ItemNativeInfo_Src;
	ITEM_NATIVE_INFO	ItemNativeInfo_Dest;
//	CBaseItem*			pBaseItemSrc		= NULL;
//	CBaseItem*			pBaseItemDest		= NULL;
//	const CItem*		pItemSrc			= NULL;
//	const CItem*		pItemDest			= NULL;
	
	ItemNativeInfo_Src.eSlotID		= ITEM_NATIVE_PLAYERSHOP;
	ItemNativeInfo_Src.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;
	
	ItemNativeInfo_Dest.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;

	for(int i = 0; i < pPacket->bItemCount; i++)
	{
		ItemNativeInfo_Src.bySlotIndex = pPacket->ItemNodeNative[i].index;				
		
		ItemNativeInfo_Dest.eSlotID		= pPacket->ItemNodeNative[i].eSlotID;
		ItemNativeInfo_Dest.bySlotIndex = pPacket->ItemNodeNative[i].bySlotIndex;
		
		AllLinkSrcToDestNative(&ItemNativeInfo_Src,&ItemNativeInfo_Dest);
	}	
}


//==============================================================//
// ���� ��� ó���� �޴� ��Ŷ.									//
//==============================================================//
void CmdShopItem(char* pMsg, DWORD dwLen)
{
	DSTC_SHOP_ITEM* pPacket = (DSTC_SHOP_ITEM*)pMsg;
	
	memset(&g_pMainPlayer->m_sPlayerShop.cItem, 0, sizeof(g_pMainPlayer->m_sPlayerShop.cItem));

	for(int i = 0; i < pPacket->bItemCount; i++)
		memcpy(&g_pMainPlayer->m_sPlayerShop.cItem[pPacket->Nodes[i].index], &pPacket->Nodes[i].cItem, sizeof(CItem));
}


void CmdPlayerShopOpen( char* pMsg, DWORD dwLen )
{
	DSTC_PLAYERSHOP_OPEN* pPacket = (DSTC_PLAYERSHOP_OPEN*)pMsg;
	
	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);
	
	if(!pUser)
		return;
	
	__lstrcpyn(pUser->m_szPlayerShopTitle, pPacket->szTitle, MAX_REAL_PLAYER_SHOP_TITLE);
	pUser->SetPlayerShop(1);
}


void CmdPlayerShopClose( char* pMsg, DWORD dwLen )
{
	DSTC_PLAYERSHOP_CLOSE* pPacket = (DSTC_PLAYERSHOP_CLOSE*)pMsg;
	
	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);
	
	CPlayerShopWnd* pPlayerShopWnd = CPlayerShopWnd::GetInstance();

	if(pPlayerShopWnd->m_pOwnerPlayerShop && pPlayerShopWnd->GetActive())
	{
		if(	pPlayerShopWnd->m_pOwnerPlayerShop->m_dwUserIndex == pPacket->dwUserIndex &&
			g_pMainPlayer->m_dwUserIndex != pPacket->dwUserIndex)
		{
			// "�Ǹ����� ������ ���Ǿ����ϴ�."
			DisplayMessageAdd(g_Message[ETC_MESSAGE1010].szMessage, 0xFFFFC309); 
			pPlayerShopWnd->SetActive(FALSE);
		}
	}
	
	if(!pUser)
		return;

	if(pUser->m_dwUserIndex == g_pMainPlayer->m_dwUserIndex)
	{
		pPlayerShopWnd->SetInputMode(0);
		return;
	}
			
	pUser->SetPlayerShop(0);
}


void CmdPlayerShopShowAll( char* pMsg, DWORD dwLen )
{
	DSTC_PLAYERSHOP_SHOWALL* pPacket = (DSTC_PLAYERSHOP_SHOWALL*)pMsg;
	
	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);

	if(!pUser)
		return;

	CPlayerShopWnd* pPlayerShopWnd = CPlayerShopWnd::GetInstance();
	
	// �ʱ�ȭ.
	memset(&pPlayerShopWnd->m_sPlayerShopInfo, 0, sizeof(pPlayerShopWnd->m_sPlayerShopInfo));
	
	// title ����!!
	__lstrcpyn(pPlayerShopWnd->m_sPlayerShopInfo.szTitle, pPacket->szTitle, MAX_PLAYER_SHOP_TITLE + 1);

	// item ���� ����!!
	for(int i = 0; i < pPacket->bItemCount; i++)
	{
		pPlayerShopWnd->m_sPlayerShopInfo.m_dwCustomSellPrice[pPacket->ItemNode[i].index] = pPacket->ItemNode[i].m_dwCustomSellPrice;
		memcpy(&pPlayerShopWnd->m_sPlayerShopInfo.cItem[pPacket->ItemNode[i].index], &pPacket->ItemNode[i].cItem, sizeof(CItem));
	}
			
	pPlayerShopWnd->m_pOwnerPlayerShop = pUser;
	pPlayerShopWnd->SetActive();
}

//==============================================================//
// End.															//
//==============================================================//