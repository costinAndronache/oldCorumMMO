//==============================================================//
// Code by Jang.									2003.08.01	//
//==============================================================//
#include "Trade.h"
#include "ItemTradeShopWnd.h"
#include "ItemWnd.h"
#include "Interface.h"
#include "UserInterface.h"
#include "Message.h"
#include "ItemTradeWnd.h"
#include "User.h"
#include "../CommonServer/CommonHeader.h"
#include "QuantityWnd.h"
#include "DungeonProcess.h"
#include "GameControl.h"
#include "HashTable.h"
#include "Chat.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "BankWnd.h"
#include "StoreWnd.h"
#include "CodeFun.h"
#include "ItemNative.h"


//==============================================================//
// ������ ��ȯ�� ��ȯ��ư�� �������� ��� ó��.					//
//==============================================================//
void CmdTradeOk( char* pMsg, DWORD dwLen )
{
	// �ڱ� �ڽ� Ok�� ���� ��� //
	DSTC_TRADE_OK*		pPacket				= (DSTC_TRADE_OK*)pMsg;
	CItemTradeShopWnd*	pItemTradeShopWnd	= CItemTradeShopWnd::GetInstance();
	char				szInfo[0xff]		= {0,};

	if(pPacket->byType==1)
	{
		// Ȯ�θ� �Ǿ��� ��� //
		// "%s �Կ� ������ ��ٸ��� �ֽ��ϴ�."
		wsprintf(szInfo, g_Message[ETC_MESSAGE647].szMessage, pPacket->szCharacterName); 
		DisplayMessageAdd(szInfo, 0xFFFFFF00);
		pItemTradeShopWnd->m_bOkChk = TRUE;
		pItemTradeShopWnd->SetOk();
	}
	else if(pPacket->byType==3)
	{
		// Ȯ�θ� �Ǿ��� ��� //
		// "%s ���� ������ ��ȯ�� Ȯ���ϼ̽��ϴ�."
		wsprintf(szInfo, g_Message[ETC_MESSAGE648].szMessage, pPacket->szCharacterName); 
		DisplayMessageAdd(szInfo, 0xFFFFFF00);		
	}
	else if(pPacket->byType==2)
	{
		// ��ȯ Ȱ��ȭ //
		// "��ȯ��ư�� Ȱ��ȭ �Ǿ����ϴ�."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE649].szMessage); 
		DisplayMessageAdd(szInfo, 0xFFFFFF00);

		// ��ư ���� //
		pItemTradeShopWnd->m_bTradeChk = TRUE;
		pItemTradeShopWnd->SetTrade();
		pItemTradeShopWnd->m_bOkChk = TRUE;
		pItemTradeShopWnd->SetOk();
	}
}


//==============================================================//
// ������ ��ȯ ó��.											//
//==============================================================//
void CmdTradeData( char* pMsg, DWORD dwLen )
{
	CItemTradeShopWnd*	pItemTradeShopWnd	= CItemTradeShopWnd::GetInstance();
	CItemWnd*			pItemWnd			= CItemWnd::GetInstance();
	DSTC_TRADE_DATA*	pPacket				= (DSTC_TRADE_DATA*)pMsg;
	char				szInfo[0xff]		= {0,};
	
	g_pMainPlayer->m_dwMoney = pPacket->dwMoeny;

	memset(&g_pMainPlayer->m_pItemTrade, 0, sizeof(CItem)*MAX_TRADE);
	memset(&pItemTradeShopWnd->m_pTradeItem, 0, sizeof(CItem)*MAX_TRADE);	

	if( IsUltra() )
	{
		ITEM_TYPE eType;
		ITEM_NATIVE_INFO ItemNativeInfoDest;
		
		for(int i = 0; i < pPacket->btItemCount; i++)
		{
			eType = GetItemType( pPacket->Nodes[i].cItem.GetID() );

			if( eType == ITEM_TYPE_LARGE)
			{
				ItemNativeInfoDest.eSlotID		= ITEM_NATIVE_INV_LARGE;
				ItemNativeInfoDest.bySlotIndex	= pPacket->Nodes[i].index;
				
				SetSlotItemNative( &pPacket->Nodes[i].cItem, &ItemNativeInfoDest);
			}
			else if( eType == ITEM_TYPE_SMALL )
			{
				ItemNativeInfoDest.eSlotID		= ITEM_NATIVE_INV_SMALL;
				ItemNativeInfoDest.bySlotIndex	= pPacket->Nodes[i].index;
				
				SetSlotItemNative( &pPacket->Nodes[i].cItem, &ItemNativeInfoDest);
			}
		}
	}
	else
	{
		// modified by deepdark.
		// ������ �̵� //
		for(int i = 0; i < pPacket->btItemCount; i++)
		{
			int nValue = pPacket->Nodes[i].cItem.GetID()/ITEM_DISTRIBUTE;

			if(nValue <= 20)
			{
				for(int j = 0; j < MAX_INV_LARGE_POOL; j++)
				{
					if(g_pMainPlayer->m_pInv_Large[j].GetID()==0)
					{
						memcpy(&g_pMainPlayer->m_pInv_Large[j], &pPacket->Nodes[i].cItem, sizeof(CItem));
						break;
					}
				}
			}
		}

		// �Һ� ������ //
		for(int i = 0; i < pPacket->btItemCount; i++)
		{
			int nValue = pPacket->Nodes[i].cItem.GetID()/ITEM_DISTRIBUTE;

			if(GetType((WORD)nValue)==ITEM_SMALL)
			{
				Trade_SmallItem(&pPacket->Nodes[i].cItem);
			}
		}
	}

	if(pItemWnd->GetActive()==TRUE)	
	{
		pItemWnd->SetLine(TRUE, pItemWnd->m_byItemChk);
	}

	// ItemTradeShopWnd //
	pItemTradeShopWnd->m_bTradeOkChk = FALSE;
	pItemTradeShopWnd->SetActive(FALSE);

	// "������ ��ȯ�� �����Ͽ����ϴ�."
	lstrcpy(szInfo, g_Message[ETC_MESSAGE650].szMessage); 
	DisplayMessageAdd(szInfo, 0xFFFFFF00);
}


//==============================================================//
// ��ȯ�߿� �κ��丮�� �Һ������ ����.							//
//==============================================================//
void Trade_SmallItem(CItem* pItem)
{
//	int		nValue		= pItem->GetID()/ITEM_DISTRIBUTE;
	BYTE	byPackCnt	= pItem->GetQuantity();
		
	for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
	{
		if(g_pMainPlayer->m_pInv_Small[i].GetID()==0)
		{
			memcpy(&g_pMainPlayer->m_pInv_Small[i], pItem, sizeof(CItem));
			g_pMainPlayer->m_pInv_Small[i].SetQuantity(byPackCnt);
			break;
		}
		else if(g_pMainPlayer->m_pInv_Small[i].GetID()==pItem->GetID())
		{
//			LP_ITEM_TABLE lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_pInv_Small[i].m_wItemID);
			CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_pInv_Small[i].m_wItemID);

//			int nValue			= g_pMainPlayer->m_pInv_Small[i].m_wItemID/ITEM_DISTRIBUTE;
			int nMaxQuantity	= 1;

			nMaxQuantity = lpItemTable->GetMaxQuantity();				
			
			BYTE byCnt = BYTE(nMaxQuantity - g_pMainPlayer->m_pInv_Small[i].GetQuantity());

			if(byCnt>=byPackCnt)
			{
				BYTE bQuantity = g_pMainPlayer->m_pInv_Small[i].GetQuantity();
				g_pMainPlayer->m_pInv_Small[i].SetQuantity(BYTE(bQuantity+byPackCnt));
				break;
			}
			else if(byCnt!=0)
			{
				BYTE bQuantity = g_pMainPlayer->m_pInv_Small[i].GetQuantity();
				g_pMainPlayer->m_pInv_Small[i].SetQuantity(BYTE(bQuantity+byCnt));
				byPackCnt = byPackCnt - byCnt;
			}
		}
	}
}


//==============================================================//
// ��ȯ ���� �޼���.											//
//==============================================================//
void CmdTradeRetrun( char* pMsg, DWORD dwLen )
{
	DSTC_TRADE_RETURN*	pPacket			= (DSTC_TRADE_RETURN*)pMsg;
	char				szInfo[0xff]	= {0,};

	switch(pPacket->byType)
	{	
	case 0:
		// "��ȯ�� �������� �����ϴ�."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE651].szMessage); 
		break;
	case 1:
		// "Karz�� �ʰ��Ͽ����ϴ�."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE652].szMessage); 
		break;
	case 2:
		// "%s �Կ� Karz�� �ʰ��Ͽ����ϴ�."
		wsprintf(szInfo, g_Message[ETC_MESSAGE653].szMessage, pPacket->szCharacterName); 
		break;
	case 3:
		// "��� �����ۿ� �� ������ �����ϴ�."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE654].szMessage); 
		break;
	case 4:
		// "%s �Կ� ��� �����ۿ� �� ������ �����ϴ�."
		wsprintf(szInfo, g_Message[ETC_MESSAGE655].szMessage, pPacket->szCharacterName); 
		break;
	case 5:
		// "�Һ� �����ۿ� �� ������ �����ϴ�."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE656].szMessage); 
		break;
	case 6:
		// "%s �Կ� �Һ� �����ۿ� �� ������ �����ϴ�."
		wsprintf(szInfo, g_Message[ETC_MESSAGE657].szMessage, pPacket->szCharacterName); 
		break;
	case 7:
		// "DB�� ������Ʈ�� ������ ������ϴ�."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE658].szMessage); 
		break;
	case 8:
		//"������ ��ȣ�� �����ϴ� ����������� ���ɼ��� �־� ��ȯ�Ͻ� ���� �����ϴ�."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE993].szMessage);
		break;
	}
	
	DisplayMessageAdd(szInfo, 0xFFFFFF00);
}


//==============================================================//
// ��ȯ ����.													//
//==============================================================//
void CmdTradeTrade( char* pMsg, DWORD dwLen )
{
	CItemTradeShopWnd*	pItemTradeShopWnd	= CItemTradeShopWnd::GetInstance();
	DSTC_TRADE_TRADE*	pPacket				= (DSTC_TRADE_TRADE*)pMsg;
	char				szInfo[0xff]		= {0,};
	
	if(pPacket->byType==0)
	{
		// �ڱ� �ڽ��� ��ȯ�� �������� ��� //
		// "%s �Կ� ������ ��ٸ��� �ֽ��ϴ�."
		wsprintf(szInfo, g_Message[ETC_MESSAGE647].szMessage, pPacket->szCharacterName); 
		DisplayMessageAdd(szInfo, 0xFFFFFF00);
		pItemTradeShopWnd->m_bTradeOneChk = TRUE;
		pItemTradeShopWnd->SetTradeOne();
	}
	else
	{
		// ���� ��ȯ�� �������� ��� //
		// "%s���� ��ȯ�� ���� �ϼ̽��ϴ�."
		wsprintf(szInfo, g_Message[ETC_MESSAGE659].szMessage, pPacket->szCharacterName); 
		DisplayMessageAdd(szInfo, 0xFFFFFF00);
	}
}


//==============================================================//
// ����� ������ ���� �ϰ� �����ش�.							//
//==============================================================//
void CmdTradeItem( char* pMsg, DWORD dwLen )
{
	DSTC_TRADE_ITEM*	pPacket				= (DSTC_TRADE_ITEM*)pMsg;
	CItemTradeShopWnd*	pItemTradeShopWnd	= CItemTradeShopWnd::GetInstance();

	// ������� Ok�� ���� ��� //
	memset(&pItemTradeShopWnd->m_pTradeItem, 0, sizeof(pItemTradeShopWnd->m_pTradeItem));
	
	for(int i = 0; i < pPacket->bItemCount; i++)
	{
		memcpy(&pItemTradeShopWnd->m_pTradeItem[pPacket->Nodes[i].index], &pPacket->Nodes[i].cItem, sizeof(CItem));
	}

	pItemTradeShopWnd->m_dwDstMoney = pPacket->dwMoney;	
}


//==============================================================//
// ������ ��ȯ ��� ó��.										//
//==============================================================//
void CmdTradeCancelFail( char* pMsg, DWORD dwLen )
{
	DSTC_TRADE_CANCELFAIL*	pPacket			= (DSTC_TRADE_CANCELFAIL*)pMsg;
	char					szInfo[0xff]	= {0,};

	if(pPacket->byType==0)
	{
		// "%s ���� ������ ��ȯ�� ����Ͽ����ϴ�."
		wsprintf(szInfo, g_Message[ETC_MESSAGE661].szMessage, pPacket->szCharacterName); 
	}
	else if(pPacket->byType==1)
	{
		// "������ ��ȯ�� ��ҵǾ����ϴ�."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE660].szMessage); 
	}

	DisplayMessageAdd(szInfo, 0xFFFFFF00);
}


//==============================================================//
// ������ ��ȯ ��� ó��(������ ó��).							//
//==============================================================//
void CmdTradeCancel(char* pMsg, DWORD dwLen)
{	
	DSTC_TRADE_CANCEL*	pPacket				= (DSTC_TRADE_CANCEL*)pMsg;
	CItemTradeShopWnd*	pItemTradeShopWnd	= CItemTradeShopWnd::GetInstance();
//	CItemWnd*			pItemWnd			= CItemWnd::GetInstance();	
	BYTE				byBankItem			= 0;	
	char				szInfo[0xff]		= {0,};

	memset(pItemTradeShopWnd->m_szName, 0, sizeof(pItemTradeShopWnd->m_szName));
	pItemTradeShopWnd->m_dwMoney = 0;

	if(pPacket->byType==0)
	{
		// "������ ��ȯ�� ��ҵǾ����ϴ�."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE660].szMessage); 		
		DisplayMessageAdd(szInfo, 0xFFFFFF00);
	}
	else if(pPacket->byType==1)
	{
		// "%s ���� ������ ��ȯ�� ����Ͽ����ϴ�."	
		wsprintf(szInfo, g_Message[ETC_MESSAGE661].szMessage, pPacket->szCharacterName);	
		DisplayMessageAdd(szInfo, 0xFFFFFF00);
	}
	else if(pPacket->byType==2)
	{
		// "%s ���� ������ �����Ͽ����ϴ�."	
		wsprintf(szInfo, g_Message[ETC_MESSAGE662].szMessage, pPacket->szCharacterName);	
		DisplayMessageAdd(szInfo, 0xFFFFFF00);
	}
	
	pItemTradeShopWnd->SetActive(FALSE);
	
	// ITEM MOVE... Modified by deepdark.
	for(int i = 0; i < pPacket->btItemCount; i++)
	{
		int nValue = pPacket->Nodes[i].cItem.GetID()/ITEM_DISTRIBUTE;
		
		// ���� ������ //
		if(GetType((WORD)nValue)==ITEM_SMALL)				
		{
			Insert_SmallInvItem(&pPacket->Nodes[i].cItem, 0, pPacket->Nodes[i].cItem.GetQuantity());
		}
		else
		{
			// ū ������ //
			if(pPacket->bInvType[i]==0)
			{
				for(int j = 0; j < MAX_INV_LARGE_POOL; j++)
				{
					if(g_pMainPlayer->m_pInv_Large[j].GetID()==0)
					{
						memcpy(&g_pMainPlayer->m_pInv_Large[j], &pPacket->Nodes[i].cItem, sizeof(CItem));
						break;
					}
				}
			}
			else
			{
				for(int j = 0; j < MAX_BANK_LARGE_POOL; j++)
				{
					if(g_pMainPlayer->m_pBank_Large[j].GetID()==0)
					{
						memcpy(&g_pMainPlayer->m_pBank_Large[j], &pPacket->Nodes[i].cItem, sizeof(CItem));
						byBankItem+=1;
						break;
					}
				}
			}
		}

		memset(&g_pMainPlayer->m_pItemTrade[pPacket->Nodes[i].index], 0, sizeof(CItem));
	}

	memset(pItemTradeShopWnd->m_pTradeItem, 0, sizeof(CItem)*MAX_TRADE);

	if(byBankItem > 0)
	{
		// "���â�� ������ �����մϴ�. %d���� �������� â��� �̵��մϴ�."
		wsprintf(szInfo, g_Message[ETC_MESSAGE1168].szMessage, byBankItem);
		DisplayMessageAdd(szInfo, 0xFFFFFF00);
	}
}


void CmdTradeCancelReturnInv(char* pMsg, DWORD dwLen)
{
	DSTC_TRADE_CANCEL_RETURN_INV* pPacket = (DSTC_TRADE_CANCEL_RETURN_INV*)pMsg;

	CItemTradeShopWnd*	pItemTradeShopWnd	= CItemTradeShopWnd::GetInstance();
//	CItemWnd*			pItemWnd			= CItemWnd::GetInstance();
	char				szInfo[0xff]		= {0,};

	memset(pItemTradeShopWnd->m_szName, 0, sizeof(pItemTradeShopWnd->m_szName));
	pItemTradeShopWnd->m_dwMoney = 0;

	if(pPacket->byType==0)
	{
		// "������ ��ȯ�� ��ҵǾ����ϴ�."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE660].szMessage); 		
		DisplayMessageAdd(szInfo, 0xFFFFFF00);
	}
	else if(pPacket->byType==1)
	{
		// "%s ���� ������ ��ȯ�� ����Ͽ����ϴ�."	
		wsprintf(szInfo, g_Message[ETC_MESSAGE661].szMessage, pPacket->szCharacterName);	
		DisplayMessageAdd(szInfo, 0xFFFFFF00);
	}
	else if(pPacket->byType==2)
	{
		// "%s ���� ������ �����Ͽ����ϴ�."	
		wsprintf(szInfo, g_Message[ETC_MESSAGE662].szMessage, pPacket->szCharacterName);	
		DisplayMessageAdd(szInfo, 0xFFFFFF00);
	}
	
	pItemTradeShopWnd->SetActive(FALSE);

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

	memset(pItemTradeShopWnd->m_pTradeItem, 0, sizeof(CItem)*MAX_TRADE);
}


//==============================================================//
// ������ ��ȯ ��û ���.										//
//==============================================================//
void CmdTradeResult(char* pMsg, DWORD dwLen)
{
	DSTC_TRADE_RESULT*	pPacket				= (DSTC_TRADE_RESULT*)pMsg;
	CItemTradeShopWnd*	pItemTradeShopWnd	= CItemTradeShopWnd::GetInstance();
	CItemWnd*			pItemWnd			= CItemWnd::GetInstance();
	CStoreWnd*			pStoreWnd			= CStoreWnd::GetInstance();
	CBankWnd*			pBankWnd			= CBankWnd::GetInstance();

	char szInfo[0xff] = {0,};

	if(pPacket->byCode == 0)
	{
		// ��ȯâ //
		// MSG_ID : 107 ; %s ���� ������ ��ȯ�� �����ϼ̽��ϴ�.
		wsprintf(szInfo, g_Message[ETC_MESSAGE107].szMessage, pPacket->szCharacterName);	
		DisplayMessageAdd(szInfo, 0xFFFFC309);

		// Open Windows //		
		pItemTradeShopWnd->SetActive(TRUE);
		pItemTradeShopWnd->m_bTradeOkChk = TRUE;

		memset(pItemTradeShopWnd->m_szName, 0, sizeof(pItemTradeShopWnd->m_szName));
		__lstrcpyn(pItemTradeShopWnd->m_szName, pPacket->szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);				
		
		if(!pItemWnd->GetActive())
			pItemWnd->SetActive(TRUE);				
		if(pStoreWnd->GetActive())
			pStoreWnd->SetActive(FALSE);		
		if(pBankWnd->GetActive())
			pBankWnd->SetActive(FALSE);
			
		return;
	}
	else if(pPacket->byCode == 1)
	{
		// MSG_ID : 108 ; %s ���� ������ ��ȯ�� �ź��ϼ̽��ϴ�.
		wsprintf(szInfo, g_Message[ETC_MESSAGE108].szMessage, pPacket->szCharacterName);	
		DisplayMessageAdd(szInfo, 0xFFFFC309);

		return;
	}
}


//==============================================================//
// ������ ��ȯ ��û ����(��û�� ������ ������ ���� �޼���).		//
//==============================================================//
void CmdTradeRequest(char* pMsg, DWORD dwLen)
{
	DSTC_TRADE_REQUEST* pPacket				= (DSTC_TRADE_REQUEST*)pMsg;	
	CItemTradeWnd*		pItemTradeWnd		= CItemTradeWnd::GetInstance();
	CItemTradeShopWnd*	pItemTradeShopWnd	= CItemTradeShopWnd::GetInstance();

	char szInfo[0xff] = {0,};	

	if(pPacket->byCode == 1)
	{
		// MSG_ID : 109 ; %s ���� �α� �ƿ� �ϼ̽��ϴ�.
		wsprintf(szInfo, g_Message[ETC_MESSAGE109].szMessage, pPacket->szCharacterName);	
		DisplayMessageAdd(szInfo, 0xFFFFC309);
		return;
	}
	else if(pPacket->byCode == 2)
	{
		// MSG_ID : 110 ; %s ���� �̹� ������ ��ȯ ��û�� �����̽��ϴ�.
		wsprintf(szInfo, g_Message[ETC_MESSAGE110].szMessage, pPacket->szCharacterName);	
		DisplayMessageAdd(szInfo, 0xFFFFC309);
		return;
	}
	else if(pPacket->byCode == 3)
	{
		// "���ÿ� ��ȯ�� �Ҽ��� �����ϴ�."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE663].szMessage); 
		DisplayMessageAdd(szInfo, 0xFFFFC309);
		return;
	}
	
	if(pItemTradeWnd->GetActive() == TRUE || pItemTradeShopWnd->GetActive() == TRUE)
	{
		// �̹� Ʈ���̵� ��û�� �޾����� ���� //
		CTDS_TRADE_REQUEST pSendPacket;
		memset(pSendPacket.szCharacterName, 0, sizeof(pSendPacket.szCharacterName));
		__lstrcpyn(pSendPacket.szCharacterName, pPacket->szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);

		pSendPacket.dwUserIndex	= pPacket->dwUserIndex;
		pSendPacket.byCode		= 1;

		g_pNet->SendMsg((char*)&pSendPacket, pSendPacket.GetPacketSize(), SERVER_INDEX_ZONE);
		return;
	}

	memset(pItemTradeWnd->m_szName, 0, sizeof(pItemTradeWnd->m_szName));
	__lstrcpyn(pItemTradeWnd->m_szName, pPacket->szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
	pItemTradeWnd->m_dwUserIndex	= pPacket->dwUserIndex;
	pItemTradeWnd->SetActive(TRUE);

	// �޼��� //	
	// MSG_ID : 111 ; %s �Կ��Լ� ��ȯ ��û�� ���Խ��ϴ�.
	wsprintf(szInfo, g_Message[ETC_MESSAGE111].szMessage, pPacket->szCharacterName);	
	DisplayMessageAdd(szInfo, 0xFFFFC309);
}


//==============================================================//
// End.															//
//==============================================================//