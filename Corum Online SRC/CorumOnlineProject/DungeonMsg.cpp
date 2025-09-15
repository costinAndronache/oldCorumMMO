#include "DungeonMsg.h"
#include "NetworkClient.h"
#include "EffectCommand.h"
#include "Party.h"
#include "Chat.h"
#include "Guild.h"
#include "Trade.h"
#include "CodeFun.h"
#include "message.h"
#include "GameControl.h"
#include "DungeonProcess.h"
#include "UserInterface.h"
#include "ItemWnd.h"
#include "Interface.h"
#include "ChatWnd.h"
#include "DSMiniMap.h"
#include "QuantityWnd.h"
#include "StoreWnd.h"
#include "DungeonTable.h"
#include "HelpWnd.h"
#include "EffectProcess.h"
#include "GameMenuWnd.h"
#include "ObjectManager.h"
#include "SkillWnd.h"
#include "CharWnd.h"
#include "NoticeWnd.h"
#include "BankWnd.h"
#include "GuildExWnd.h"
#include "ItemTradeShopWnd.h"
#include "PlayerShopWnd.h"
#include "CDungeonManagerWnd.h"
#include "ItemUpgrade.h"
#include "Errorwnd.h"
#include "Map.h"
#include "ChrInfo.h"
#include "Effect.h"
#include "InputManager.h"
#include "WorldProcess.h"
#include "CorumResource.h"
#include "PartyBoardWnd.h"
#include "Filter.h"
#include "DungeonSiegeInfoWnd.h"
#include "MinboboWork.h"
#include "ItemMakingWnd.h"
#include "ItemNative.h"
#include "GuildWarFinalSettingWnd.h"
#include "GuildWarStatusWnd.h"
#include "DungeonProcess.h"

extern LPBASEITEM_HASH		g_pBaseItemHash;

void InitPacketProcDungeon()
{				
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_JOIN]				= CmdJoinDungeon;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_CHANGE_LAYER]		= CmdChangeLayer;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_CHANGE_SECTION]		= CmdChangeSection;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_LEVELUP]			= CmdLevelUP;	
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_APPEAR_ITEM]		= CmdAppearItem;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_DISAPPEAR_ITEM]		= CmdDisappearItem;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_ITEM_PICKUP]				= CmdPickupItem;		
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_ERROR_CODE]			= CmdDungeonErrorCode;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_MESSAGE]			= CmdDungeonMessage;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_JOIN_FAILED]		= CmdDungeonJoinFailedForDungeon;		
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_GUILDWAR_DIE_MSG]	= CmdDungeonGuildWarDieMsg;
							
	InitPacketProcDungeonMinbobo();
	InitPacketProcDungeonSeven();
	InitPacketProcDungeonDeepDark();
	InitPacketProcDungeonJJW6263();
	InitPacketProcDungeonYak();	
}


//==============================================================//
// Icon�� �ε��ϰ� �����ϴ� �Լ�.								//
//==============================================================//
void CmdPickupItem( char* pMsg, DWORD dwLen )
{
	char				szInfo[0xff] = {0,};
//	LP_ITEM_TABLE		lpItemTable;
	CBaseItem*			lpItemTable;
	DSTC_ITEM_PICKUP*	pPacket				= (DSTC_ITEM_PICKUP*)pMsg;	
	CItemWnd*			pItemWnd			= CItemWnd::GetInstance();
	CBankWnd*			pBankWnd			= CBankWnd::GetInstance();
	CQuantityWnd*		pQuantityWnd		= CQuantityWnd::GetInstance();
	CGuildExWnd*		pGuildExWnd			= CGuildExWnd::GetInstance();		
	CChatWnd*			pChatWnd			= CChatWnd::GetInstance();
	CPlayerShopWnd*		pPlayerShopWnd		= CPlayerShopWnd::GetInstance();
	CGameMenuWnd*		pGameMenuWnd		= CGameMenuWnd::GetInstance();
	WORD				wWeight				= g_pMainPlayer->GetSumWeight();

	if(!(pPacket->bInv>=57 && pPacket->bInv<=60))
		g_pMainPlayer->m_dwPlayserShop = 0;

	if(pPacket->bInv!=27)
		g_bSmallBankChk = FALSE;

 	switch(pPacket->bInv)
	{
	case 1:		// ���� ������ ����, Weapon, Armor �������� ������ ���(�ٷ� ĭ����)
		{
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_GETITEM, g_v3InterfaceSoundPos, FALSE);

			CGameMenuWnd* pGameMenuWnd = CGameMenuWnd::GetInstance();
				
			if((pPacket->Item.m_wItemID/ITEM_DISTRIBUTE)==ITEM_MONEY_INDEX)
			{				
				g_pMainPlayer->m_dwMoney = pPacket->dwMoney;

				// MSG_ID : 79 ; %d Karz�� ������ϴ�.
				wsprintf(szInfo, g_Message[ETC_MESSAGE79].szMessage, pPacket->Item.m_Item_Money.dwMoney);	
				DisplayMessageAdd(szInfo, 0xFFFF2CFF, pGameMenuWnd->m_bSystemMsgFlag);

				if(pPacket->bZipCode==1)
				{
					// MSG_ID : 80 ; ���̻� Karz�� ������ �� �����ϴ�.
					lstrcpy(szInfo, g_Message[ETC_MESSAGE80].szMessage);	
					DisplayMessageAdd(szInfo, 0xFFFFC309, pGameMenuWnd->m_bSystemMsgFlag);
				}
			}
			else
			{
				if(pPacket->bZipCode==255)
				{
					//"���� ��ȣ�� ������ �ִ� �������� �����Ͽ� ������� ��å�� ��ȯ����
					//���� ��ȣ�� �������������Ͽ����� �˷��帳�ϴ�."
					DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
				}
				else
				{
					memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));

					lpItemTable = g_pItemTableHash_get()->GetData(pPacket->Item.GetID());

					if(lpItemTable)
					{
						// MSG_ID : 81 ; %s �������� ȹ���Ͽ����ϴ�.
						wsprintf(szInfo, g_Message[ETC_MESSAGE81].szMessage, lpItemTable->szItemName_Kor);	
						DisplayMessageAdd(szInfo, 0xFFFF2CFF, pGameMenuWnd->m_bSystemMsgFlag);
					}
				}				
			}
		}
		break;
	case 2:		// ���� �������� ������ ���(�ٷ� ĭ����)
		{
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_GETITEM, g_v3InterfaceSoundPos, FALSE);

		//	int	nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;

			if(pPacket->Item.GetType()!=ITEM_SMALL)			
				return;

			if(pPacket->bZipCode==255)
			{
				//"���� ��ȣ�� ������ �ִ� �������� �����Ͽ� ������� ��å�� ��ȯ����
				// ���� ��ȣ�� �������������Ͽ����� �˷��帳�ϴ�."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}
			else
			{
				Insert_SmallInvItem(&pPacket->Item, pPacket->bZipCode, pPacket->Item.GetQuantity());

				lpItemTable = g_pItemTableHash_get()->GetData(pPacket->Item.GetID());

				if(lpItemTable)
				{
					// MSG_ID : 82 ; %s �������� %d�� ȹ���Ͽ����ϴ�.
					wsprintf(szInfo, g_Message[ETC_MESSAGE82].szMessage, lpItemTable->szItemName_Kor
						, pPacket->Item.GetQuantity());	
					DisplayMessageAdd(szInfo, 0xFFFF2CFF, pGameMenuWnd->m_bSystemMsgFlag);
				}
			}	
			
			pQuantityWnd->m_bReq = FALSE;
		}
		break;
	case 3:		// ���� �������� ���콺���� ��Ʈ�� ���� ���.
		{			
		//	int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;

			if(pPacket->Item.GetType()!=ITEM_SMALL)			
				return;
			
			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"���� ��ȣ�� ������ �ִ� �������� �����Ͽ� ������� ��å�� ��ȯ����
				// ���� ��ȣ�� �������������Ͽ����� �˷��帳�ϴ�."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}
			else
			{
				Insert_SmallBeltItem(&pPacket->Item, pPacket->bZipCode, pPacket->Item.GetQuantity(), FALSE);
			
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));			
				
				// ���� ���� //
				PlayItemSound(pPacket->Item.m_wItemID);
			}
			
			pQuantityWnd->m_bReq = FALSE;
		}			
		break;
	case 4:		// ���콺�� �ٴ����� �Һ������ �̵�.
		{
			PlayItemSound(g_pMainPlayer->m_MouseItem.m_wItemID);

			memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
			g_pMainPlayer->m_nItemSelect	= 0;
			pQuantityWnd->m_bReq			= FALSE;				
		}
		break;
	case 5:		// �ٴڿ��� ���콺��(���� ������).
		{
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_GETITEM, g_v3InterfaceSoundPos, FALSE);
			
			int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;			

			if(nValue>__MAX_ITEM_LARGE_INDEX__)
				return;

			memcpy(&g_pMainPlayer->m_MouseItem, &pPacket->Item, sizeof(CItem));

			if(g_Mouse.bLDown)
				g_pMainPlayer->m_nItemSelect = 1;
		}
		break;
	case 6:		// ���콺���� ���� ����������(���� ������).
		{	
			int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;
			
			if(nValue>__MAX_ITEM_LARGE_INDEX__)
				return;

			CQuantityWnd* pQuantityWnd = CQuantityWnd::GetInstance();

			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"���� ��ȣ�� ������ �ִ� �������� �����Ͽ� ������� ��å�� ��ȯ����
				// ���� ��ȣ�� �������������Ͽ����� �˷��帳�ϴ�."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}
			else
			{
				memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				g_pMainPlayer->m_nItemSelect = 0;

				if(pItemWnd->m_byItemChk==1)
					pItemWnd->m_byItemChk = 0;

				PlayItemSound(pPacket->Item.m_wItemID);
				
				CItemMakingWnd* pItemMakingWnd = CItemMakingWnd::GetInstance();
				if(pItemMakingWnd)
				{
					if(pItemMakingWnd->GetActive())
					{
						char szInfo[0xff] = {0,};
						// "�������� �κ��丮�� �̵��Ǿ����ϴ�."	
						wsprintf(szInfo, g_Message[ETC_MESSAGE1209].szMessage); 
						DisplayMessageAdd(szInfo, 0xffffff00);
					}
				}
			}
			
			pQuantityWnd->m_bReq = FALSE;			
		}
		break;
	case 7:		// ���� �����ۿ��� ���콺��(���� ������).
		{	
			int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;

			if(nValue>__MAX_ITEM_LARGE_INDEX__)
				return;

			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ITEMCLICK, g_v3InterfaceSoundPos, FALSE);

			memcpy(&g_pMainPlayer->m_MouseItem, &pPacket->Item, sizeof(CItem));
			memset(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], 0, sizeof(CItem));

			if(g_Mouse.bLDown)
				g_pMainPlayer->m_nItemSelect = 1;

			pQuantityWnd->m_bReq = FALSE;
		}
		break;
	case 8:
	case 9:
		{	
			int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;
			
			PlayItemSound(pPacket->Item.m_wItemID);

			if(nValue>__MAX_ITEM_LARGE_INDEX__)
				return;

			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"���� ��ȣ�� ������ �ִ� �������� �����Ͽ� ������� ��å�� ��ȯ����
				// ���� ��ȣ�� �������������Ͽ����� �˷��帳�ϴ�."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}

			switch(pPacket->dwMoney)
			{
			
			case 1 :
				{
					memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));
					memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
					g_pMainPlayer->m_nItemSelect = 0;
					// "�����Ҽ� ���� �������Դϴ�."
					DisplayMessageAdd(g_Message[ETC_MESSAGE510].szMessage,  0xffff0000); 
				}
				break;

			case 2 :
				{
					memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
					g_pMainPlayer->m_nItemSelect = 0;
					//"�����Ҽ� ���� �������Դϴ�."
					DisplayMessageAdd(g_Message[ETC_MESSAGE510].szMessage,  0xffff0000); 
				}
				break;
			case 3:
				//2005.01.20 �迵��
				//"�����Ҽ� ���� �������Դϴ�."
				DisplayMessageAdd(g_Message[ETC_MESSAGE510].szMessage,  0xffff0000); 						
				break;
			case 4 :
				// MSG_ID : 53 ; ���������� �κ��丮�� ����á���ϴ�
				DisplayMessageAdd(g_Message[ETC_MESSAGE53].szMessage, 0xFFFFC309);	
				_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				break;
			default :
			{
				switch(nValue)
				{		
				case 0:	case 1:	case 2: case 3: case 4: case 5: case 6: case 7:		// ���� //
					SetPickupItemWeaponR(pPacket);
					break;
				case 10:	// ��� //
					SetPickupItemHelmet(pPacket);
					break;
				case 11:	// ���� //
					SetPickupItemArmor(pPacket);
					break;
				case 12:	// ���� //
					SetPickupItemShield(pPacket);
					break;				
				case 13:	// ��Ʈ //
					SetPickupItemBelt(pPacket);
					break;
				case 14:	// �尩 //				
					SetPickupItemGlove(pPacket);
					break;
				case 15:	// �Ź� //
					SetPickupItemBoots(pPacket);
					break;
				case 16:	// �� //
					SetPickupItemRing(pPacket);
					break;
				case 17:	// �ƹķ� //
					SetPickupItemAmule(pPacket);
					break;
				case 18:	// ���� //
					SetPickupItemBag(pPacket);
					break;
				case 20:	// ����� //
					SetPickupItemRide(pPacket);
					break;
				}		
				
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				g_pMainPlayer->m_nItemSelect = 0;
			}
			}
		}
		break;	
	case 10:	// ���콺���� �ٴ����� �̵��Ҷ�(���� ������).
		{			
			PlayItemSound(g_pMainPlayer->m_MouseItem.m_wItemID);

			memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
			g_pMainPlayer->m_nItemSelect = 0;								
			pQuantityWnd->m_bReq = FALSE;
		}
		break;
	case 11:	// ���������� ���Կ��� ���콺�� �̵�.
		{
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ITEMCLICK, g_v3InterfaceSoundPos, FALSE);

			int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;

			if(nValue>__MAX_ITEM_LARGE_INDEX__)
				return;

			if(nValue<=__MAX_ITEM_LARGE_INDEX__ && pPacket->Item.m_wItemID!=0)
			{
				memcpy(&g_pMainPlayer->m_MouseItem, &pPacket->Item, sizeof(CItem));
				memset(&g_pMainPlayer->m_pEquip[pPacket->bEquipCode], 0, sizeof(CItem));												
			}

			if(g_Mouse.bLDown)
				g_pMainPlayer->m_nItemSelect = 1;
			
			if(!g_Mouse.bLDown)
				ItemChk();			
		}
		break;
	case 12:	// �ٴڿ��� ���콺�� �̵��Ҷ�(�Һ������).
		{
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ITEMCLICK, g_v3InterfaceSoundPos, FALSE);

		//	int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;			
												
			if(pPacket->Item.GetType()!=ITEM_SMALL)
				return;

			memcpy(&g_pMainPlayer->m_MouseItem, &pPacket->Item, sizeof(CItem));

			if(g_Mouse.bLDown)
				g_pMainPlayer->m_nItemSelect = 1;
		}
		break;

	case 13:	// ���� ���� ���.
		{
// ��Ŭ�� ��û���� Karz ���� ��������� ���ϰ� 2005.02.05 �迵�� 
#if  IS_JAPAN_LOCALIZING()
			DisplayMessageAdd(g_Message[ETC_MESSAGE1372].szMessage, 0xffff0000);		
#else
			g_pMainPlayer->m_dwMoney = pPacket->dwMoney;			
#endif
		}
		break;
	case 14:	// ���� �������� ���콺���� �Һ�ĭ���� ���� ���.
		{			
		//	int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;
				
			PlayItemSound(pPacket->Item.m_wItemID);

			if(pPacket->Item.GetType()!=ITEM_SMALL)
				return;

			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"���� ��ȣ�� ������ �ִ� �������� �����Ͽ� ������� ��å�� ��ȯ����
				// ���� ��ȣ�� �������������Ͽ����� �˷��帳�ϴ�."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}
			else
			{
				Insert_SmallInvItem(&pPacket->Item, pPacket->bZipCode, pPacket->Item.GetQuantity());

				pItemWnd->m_byItemChk			= 1;
				g_pMainPlayer->m_nItemSelect	= 0;			
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
			}
			
			pQuantityWnd->m_bReq = FALSE;
		}
		break;
	case 15:	// �Һ� â���� ���콺�� �̵��� ��(���� ������).
		{
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ITEMCLICK, g_v3InterfaceSoundPos, FALSE);

		//	int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;

			if(pPacket->Item.GetType()!=ITEM_SMALL)
				return;
						
			if(g_Mouse.bLDown)
				g_pMainPlayer->m_nItemSelect = 1;

			memcpy(&g_pMainPlayer->m_MouseItem, &pPacket->Item, sizeof(CItem));
			memset(&g_pMainPlayer->m_pInv_Small[pPacket->bZipCode], 0, sizeof(CItem));							

			CQuantityWnd::GetInstance()->m_bReq = FALSE;
		}
		break;
	case 16:	// ��Ʈ���� ���콺��(�Һ� ������).
		{			
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ITEMCLICK, g_v3InterfaceSoundPos, FALSE);

		//	int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;

			if(pPacket->Item.GetType()!=ITEM_SMALL)
				return;
			
			if(g_Mouse.bLDown)
				g_pMainPlayer->m_nItemSelect = 1;

			memcpy(&g_pMainPlayer->m_MouseItem, &pPacket->Item, sizeof(CItem));
			g_pMainPlayer->nullifyBeltItemAtIndex(pPacket->bZipCode);

		}
		break;
	case 17:	// Item Swap //
		{
			g_pMainPlayer->m_bCurrnetHand = pPacket->bZipCode;						
		}
		break;
	case 18:	// ������ �������� �������� �ȶ� //
		{
		//	int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;
									
			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"���� ��ȣ�� ������ �ִ� �������� �����Ͽ� ������� ��å�� ��ȯ����
				// ���� ��ȣ�� �������������Ͽ����� �˷��帳�ϴ�."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}
			else
			{
				if(pPacket->Item.GetType()==ITEM_SMALL)
				{				
					if(pPacket->bEquipCode==0)
					{
						PlayItemSound(pPacket->Item.m_wItemID);

						if(pPacket->Item.GetQuantity()!=pPacket->bZipCode2)
						{
							if(pPacket->Item.GetQuantity()<pPacket->bZipCode2)
								return;

							BYTE byPackCnt = BYTE(pPacket->Item.GetQuantity()-pPacket->bZipCode2);

							pPacket->Item.SetQuantity(byPackCnt);
							Insert_SmallInvItem(&pPacket->Item, pPacket->bZipCode, byPackCnt);
						}					
						
						DWORD dwMoney = pPacket->dwMoney - g_pMainPlayer->m_dwMoney;

						// MSG_ID : 83 ; %u Karz�� ȹ���߽��ϴ�.
						wsprintf(szInfo, g_Message[ETC_MESSAGE83].szMessage, dwMoney);
						DisplayMessageAdd(szInfo, 0xFFFFC309);

						g_pMainPlayer->m_dwMoney = pPacket->dwMoney;
						memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));					
					}
					else if(pPacket->bEquipCode==1)
					{
						Insert_SmallInvItem(&pPacket->Item, pPacket->bZipCode, pPacket->Item.GetQuantity());
						
						memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));

//						LP_ITEM_TABLE lpItem = g_pItemTableHash_get()->GetData(pPacket->dwId);
						CBaseItem* lpItem = g_pItemTableHash_get()->GetData(pPacket->dwId);

						if(lpItem)
						{
							// MSG_ID : 84 ; %s �������� ���̻� �ȼ� �����ϴ�.
							wsprintf(szInfo, g_Message[ETC_MESSAGE84].szMessage, lpItem->szItemName_Kor);	
							DisplayMessageAdd(szInfo, 0xFFFFC309);
						}					
					}
					
					CQuantityWnd* pQuantityWnd = CQuantityWnd::GetInstance();
					pQuantityWnd->m_bReq = FALSE;   				
				}
			}
		}
		break;
	case 19:	// �Һ� �������� �춧.
		{						
		//	int nPackCount = pPacket->bEquipCode;
						
			if(pPacket->bZipCode2==0)
			{
				PlayItemSound(pPacket->Item.m_wItemID);
				
				if(pPacket->Item.GetQuantity()==0)
				{
					// "�κ��丮�� ������ �����մϴ�."
					lstrcpy(szInfo, g_Message[ETC_MESSAGE90].szMessage); 
					DisplayMessageAdd(szInfo, 0xFFFF2CFF);
				}
				else
				{
					Insert_SmallInvItem(&pPacket->Item, pPacket->bZipCode, pPacket->Item.GetQuantity());
				
					g_pMainPlayer->m_dwMoney = pPacket->dwMoney;
					
//					LP_ITEM_TABLE lpItem = g_pItemTableHash_get()->GetData(pPacket->dwId);
					CBaseItem* lpItem = g_pItemTableHash_get()->GetData(pPacket->dwId);

					if(lpItem)
					{					
						// MSG_ID : 85 ; "%s �������� %u���� �����ϼ̽��ϴ�.
						wsprintf(szInfo, g_Message[ETC_MESSAGE85].szMessage, lpItem->szItemName_Kor, pPacket->bEquipCode);	
						DisplayMessageAdd(szInfo, 0xFFFF2CFF);											
					}	
					
					if(pItemWnd->m_byItemChk==0)
						pItemWnd->m_byItemChk = 1;
				}				
			}
			else
			{
//				LP_ITEM_TABLE lpItem = g_pItemTableHash_get()->GetData(pPacket->dwId);
				CBaseItem* lpItem = g_pItemTableHash_get()->GetData(pPacket->dwId);

				if(lpItem)
				{
					// MSG_ID : 86 ; Kartz�� �����մϴ�. %s �������� ���� �Ҽ������ϴ�.
					wsprintf(szInfo, g_Message[ETC_MESSAGE86].szMessage, lpItem->szItemName_Kor);	
					DisplayMessageAdd(szInfo, 0xFFFFC309);
				}				
			}
			g_pMainPlayer->m_nItemSelect = 0;
		}
		break;
	case 20:	// �Һ� �����ۻ��.
		{
			memcpy(&g_pMainPlayer->m_pInv_Small[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));			
		}
		break;
	case 21:	// �Һ� �����ۻ��.
		{
		g_pMainPlayer->setBeltItem(pPacket->Item, pPacket->bZipCode);
			//memcpy(&g_pMainPlayer->m_pBelt[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));			
		}
		break;
	case 22:	// ���콺���� ��������â����
		{					
		}
		break;
	case 23:	// ��������â���� ���콺��.
		{		
		}
		break;
	case 24:		// ���콺���� â���(��� ������) //
		{
			int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;
										
			if(pPacket->Item.m_wItemID==0)
				return;
			if(nValue>__MAX_ITEM_LARGE_INDEX__)
				return;

			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"���� ��ȣ�� ������ �ִ� �������� �����Ͽ� ������� ��å�� ��ȯ����
				// ���� ��ȣ�� �������������Ͽ����� �˷��帳�ϴ�."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}
			else
			{
				PlayItemSound(pPacket->Item.m_wItemID);

				memcpy(&g_pMainPlayer->m_pBank_Large[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
			}			
			pBankWnd->m_byBankChk = 0;
		}
		break;
	case 25:		// ���콺���� â���(�Һ� ������) //
		{
		//	int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;				
		
			if(pPacket->Item.GetType()!=ITEM_SMALL)
				return;

			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"���� ��ȣ�� ������ �ִ� �������� �����Ͽ� ������� ��å�� ��ȯ����
				// ���� ��ȣ�� �������������Ͽ����� �˷��帳�ϴ�."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}
			else
			{
				PlayItemSound(pPacket->Item.m_wItemID);

				if(pPacket->Item.GetQuantity()>=pPacket->dwMoney)
				{			
					BYTE	byPacketCnt	= BYTE(pPacket->dwMoney);
					BYTE	byOutCnt	= BYTE(pPacket->Item.GetQuantity()-pPacket->dwMoney);
					
					if(byPacketCnt==pPacket->Item.GetQuantity())
						Insert_SmallBankItem(&pPacket->Item, pPacket->bZipCode, pPacket->Item.GetQuantity());
					else
					{
						pPacket->Item.SetQuantity(BYTE(pPacket->dwMoney));
						Insert_SmallBankItem(&pPacket->Item, pPacket->bZipCode, BYTE(pPacket->dwMoney));
						
						if(byOutCnt!=0)
						{
							pPacket->Item.SetQuantity(byOutCnt);
							Insert_SmallInvItem(&pPacket->Item, 0, byOutCnt);
						}
					}
				}

				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));				
			}	

			pQuantityWnd->m_bReq = FALSE;

			if(pBankWnd->m_byBankChk==0)
			{
				pBankWnd->m_byBankChk = 1;
			}
		}
		break;
	case 26:		// â���� ���콺��(��� ������) //
		{
			int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;

			if(pPacket->Item.m_wItemID==0)
				return;
			if(nValue>__MAX_ITEM_LARGE_INDEX__)
				return;

			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ITEMCLICK, g_v3InterfaceSoundPos, FALSE);

			memcpy(&g_pMainPlayer->m_MouseItem, &pPacket->Item, sizeof(CItem));
			memset(&g_pMainPlayer->m_pBank_Large[pPacket->bZipCode], 0, sizeof(CItem));			

			g_pMainPlayer->m_nItemSelect = 1;
			pBankWnd->m_byBankChk = 0;
		}
		break;
	case 27:		// â���� ���콺��(�Һ� ������) //
		{
		//	int nValue = pPacket->Item.GetID()/ITEM_DISTRIBUTE;

			if(pPacket->Item.GetID()==0)
				return;
			if(pPacket->Item.GetType()!=ITEM_SMALL)
				return;

			PlayItemSound(pPacket->Item.m_wItemID);
			
			memcpy(&g_pMainPlayer->m_MouseItem, &pPacket->Item, sizeof(CItem));								
			memset(&g_pMainPlayer->m_pBank_Small[pPacket->bZipCode], 0, sizeof(CItem));
				
			g_pMainPlayer->m_nItemSelect = 1;
			pBankWnd->m_byBankChk = 1;
			
			g_bSmallBankChk = TRUE;
		}
		break;
	case 28:		// ���콺���� ������� �ٴڿ� ������ �� //
		{				
		//	int		nVal		= pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;
			BYTE	byPacketCnt	= BYTE(pPacket->Item.GetQuantity() - pPacket->dwMoney);
		//	BYTE	byOutCnt	= BYTE(pPacket->dwMoney);

			pQuantityWnd->m_bReq = FALSE;			
			memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));			

			PlayItemSound(pPacket->Item.m_wItemID);

			if(byPacketCnt>0)
			{
				pPacket->Item.SetQuantity(byPacketCnt);
				Insert_SmallInvItem(&pPacket->Item, 0, pPacket->Item.GetQuantity());
			}
		}
		break;	
	case 30:		// ��� ���� ������ ���� //
		{			// �迵�� 2005.01.29
			if(pQuantityWnd->GetActive())
			{
				if(pQuantityWnd->m_bQuantityType==__QUANTITY_STORE__)
				{								
					int		nVal = g_pMainPlayer->m_MouseItem.m_wItemID/ITEM_DISTRIBUTE;				
					BOOL	bChk = FALSE;

					CTDS_ITEM_PICKUP ItemPickup;
					ItemPickup.bSectionNum	= 1;
					ItemPickup.i64ItemID	= 0;

					if(nVal==50)
					{
//						LP_ITEM_TABLE lpItemTable = g_pItemTableHash_get()->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);
						CBaseItem* lpItemTable = g_pItemTableHash_get()->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);;

						BYTE byMaxCnt = 0;

						if(lpItemTable)
							byMaxCnt = lpItemTable->GetMaxQuantity();

						for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
						{
							if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
							{
								ItemPickup.bInv		= 14;
								ItemPickup.bZipCode	= BYTE(i);
								bChk				= TRUE;
								break;
							}
							else
							{
								if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==g_pMainPlayer->m_MouseItem.m_wItemID)
								{
									// �����ۿ��� �ִ� ĭ�� ã�´� //
									if(g_pMainPlayer->m_pInv_Small[i].m_Item_Supplies.bQuantity<byMaxCnt)
									{
										ItemPickup.bInv		= 14;
										ItemPickup.bZipCode	= BYTE(i);
										bChk			= TRUE;
										break;
									}
								}															
							}
						}
						
						if(bChk==FALSE)
							ItemPickup.bInv	= 4;
						
						g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
						pQuantityWnd->m_bReq = TRUE;
					}			
				}
				else if(pQuantityWnd->m_bQuantityType==__QUANTITY_THROW__)
				{				
					int		nVal = g_pMainPlayer->m_MouseItem.m_wItemID/ITEM_DISTRIBUTE;				
					BOOL	bChk = FALSE;

					CTDS_ITEM_PICKUP ItemPickup;
					ItemPickup.bSectionNum	= 1;
					ItemPickup.i64ItemID	= 0;

					if(nVal==50)
					{
//						LP_ITEM_TABLE lpItemTable = g_pItemTableHash_get()->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);
						CBaseItem* lpItemTable = g_pItemTableHash_get()->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);

						BYTE byMaxCnt = 0;

						if(lpItemTable)
							byMaxCnt = lpItemTable->GetMaxQuantity();

						for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
						{
							if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
							{
								ItemPickup.bInv		= 14;
								ItemPickup.bZipCode	= BYTE(i);
								bChk				= TRUE;
								break;
							}
							else
							{
								if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==g_pMainPlayer->m_MouseItem.m_wItemID)
								{
									// �����ۿ��� �ִ� ĭ�� ã�´� //
									if(g_pMainPlayer->m_pInv_Small[i].m_Item_Supplies.bQuantity<byMaxCnt)
									{
										ItemPickup.bInv		= 14;
										ItemPickup.bZipCode	= BYTE(i);
										bChk				= TRUE;
										break;
									}
								}
							}
						}
					}
					
					if(bChk==FALSE)
						ItemPickup.bInv	= 4;										
					
					g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
					pQuantityWnd->m_bReq = TRUE;
				}
				else if(pQuantityWnd->m_bQuantityType==__QUANTITY_BANKINSERT__)
				{
					int		nVal = g_pMainPlayer->m_MouseItem.m_wItemID/ITEM_DISTRIBUTE;				
					BOOL	bChk = FALSE;

					CTDS_ITEM_PICKUP ItemPickup;
					ItemPickup.bSectionNum	= 1;
					ItemPickup.i64ItemID	= 0;

					if(nVal==50)
					{
//						LP_ITEM_TABLE lpItemTable = g_pItemTableHash_get()->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);
						CBaseItem* lpItemTable = g_pItemTableHash_get()->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);

						BYTE byMaxCnt = 0;

						if(lpItemTable)
							byMaxCnt = lpItemTable->GetMaxQuantity();

						for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
						{
							if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
							{
								ItemPickup.bInv		= 14;
								ItemPickup.bZipCode	= BYTE(i);
								bChk				= TRUE;
								break;
							}
							else
							{
								if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==g_pMainPlayer->m_MouseItem.m_wItemID)
								{
									// �����ۿ��� �ִ� ĭ�� ã�´� //
									if(g_pMainPlayer->m_pInv_Small[i].m_Item_Supplies.bQuantity<byMaxCnt)
									{
										ItemPickup.bInv		= 14;
										ItemPickup.bZipCode	= BYTE(i);
										bChk				= TRUE;
										break;
									}
								}															
							}
						}
						
						if(bChk==FALSE)
							ItemPickup.bInv	= 4;
						
						g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
						pQuantityWnd->m_bReq = TRUE;
					}
				}
				
				pQuantityWnd->SetActive(FALSE);				
			}
			
			// ������ ���� //
			memcpy(&g_pMainPlayer->m_pInv_Small[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));

			// ���� â //								
			pGuildExWnd->SetActive(TRUE);
			pGuildExWnd->m_byChk = 1;
			pGuildExWnd->m_byGuildType = pPacket->bZipCode2;			

			// �Է� ��� //
			RECT rt;
			rt.left		= (LONG)pGuildExWnd->m_fPosX+135;
			rt.right	= (LONG)pGuildExWnd->m_fPosX+200;
			rt.top		= (LONG)pGuildExWnd->m_fPosZ+70;
			rt.bottom	= (LONG)pGuildExWnd->m_fPosZ+82;
			int nOrder	= __ORDER_INTERFACE_START__ + pGuildExWnd->m_byOrder*10 + 2;

			if(g_pGVDungeon->bChatMode==TRUE)
			{
				g_pInputManager->ClearInput(INPUT_BUFFER_19);
				g_pInputManager->SetFocusInput(INPUT_BUFFER_0);
				
				pChatWnd->SetActive(FALSE);				
			}

			g_pInputManager->InitializeInput(INPUT_BUFFER_7, FONT_SS3D_GAME, rt, 0xffffffff, nOrder);
			
			ChatModeOn(INPUT_BUFFER_7, FALSE);

#ifdef	_USE_IME 
			
			POINT pChatPoint;
			pChatPoint.x	= long(pGuildExWnd->m_fPosX+135);
			pChatPoint.y	= long(pGuildExWnd->m_fPosZ+70);
			GET_IMEEDIT()->SetEditIndex(1);
//			GET_IMEEDIT()->ClearImeEditString();			
			GET_IMEEDIT()->ActivateIME(pChatPoint, MAX_GUILD_NAME_REAL_LENGTH, pGuildExWnd->GetStartOrder()+2);			
#endif      

		}
		break;
	case 32:		// �����ۿ��� ��ũ�� �� �̵�.
	case 33:		// ��ũ���� ���������� �� �̵�.
		{
			g_pMainPlayer->m_dwMoney		= pPacket->dwMoney;
			g_pMainPlayer->m_dwBankMoney	= pPacket->dwId;

			if(g_pMainPlayer->m_dwBankMoney==__MAX_MONEY__)
			{
				// MSG_ID :	87 ; ���̻� ���� �����ϴ�.
				DisplayMessageAdd(g_Message[ETC_MESSAGE87].szMessage, 0xFFFFC309);	
				_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
			}				
			if(g_pMainPlayer->m_dwMoney==__MAX_MONEY__)
			{
				// MSG_ID :	87 ; ���̻� ���� �����ϴ�.
				DisplayMessageAdd(g_Message[ETC_MESSAGE87].szMessage, 0xFFFFC309);	
				_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
			}				
		}
		break;
	case 34:		// Weapon ������ �� ��.
		{
			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"���� ��ȣ�� ������ �ִ� �������� �����Ͽ� ������� ��å�� ��ȯ����
				// ���� ��ȣ�� �������������Ͽ����� �˷��帳�ϴ�."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}
			else
			{
				if(pPacket->bEquipCode==0)
				{
					DWORD dwMoney = pPacket->dwMoney-g_pMainPlayer->m_dwMoney;
					
					g_pMainPlayer->m_dwMoney = pPacket->dwMoney;				
					// �޼��� //
					// MSG_ID : 83 ; %u Karz�� ȹ���߽��ϴ�.
					wsprintf(szInfo, g_Message[ETC_MESSAGE83].szMessage, dwMoney);	
					DisplayMessageAdd(szInfo, 0xFFFF2CFF);
				}
				else if(pPacket->bEquipCode==1)
				{					
					memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));
					// �޼��� //				
					// MSG_ID : 88 ; �����Ҽ� �ִ� Karz �ִ뷮�� ������ �־ �������� �� �� �����ϴ�.
					DisplayMessageAdd(g_Message[ETC_MESSAGE88].szMessage, 0xFFFFC309);	
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				}
				else
				{
					// �޼��� //
					// MSG_ID : 88 ; �����Ҽ� �ִ� Karz �ִ뷮�� ������ �־ �������� �� �� �����ϴ�.
					DisplayMessageAdd(g_Message[ETC_MESSAGE88].szMessage, 0xFFFFC309);	
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				}

				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
			}
		}
		break;
	case 35:		// Weapon ������ �� ��.
		{																	
			if(pPacket->bEquipCode==1)
			{					
				memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));
				g_pMainPlayer->m_dwMoney = pPacket->dwMoney;

//				LP_ITEM_TABLE lpItem = g_pItemTableHash_get()->GetData(pPacket->dwId);
				CBaseItem* lpItem = g_pItemTableHash_get()->GetData(pPacket->dwId);

				if(lpItem)
				{
					// MSG_ID : 89 ; %s �������� �����Ͽ����ϴ�.			
					wsprintf(szInfo, g_Message[ETC_MESSAGE89].szMessage, lpItem->szItemName_Kor);		
					DisplayMessageAdd(szInfo, 0xFFFF2CFF);
				}				

				if(pItemWnd->m_byItemChk==1)
					pItemWnd->m_byItemChk = 0;
			}
			else
			{
				if(pPacket->bEquipCode==2)
				{
					// MSG_ID : 90 ; �κ��丮�� ������ �����մϴ�.
					DisplayMessageAdd(g_Message[ETC_MESSAGE90].szMessage, 0xFFFFC309);		
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				}
				else
				{
//					LP_ITEM_TABLE lpItem = g_pItemTableHash_get()->GetData(pPacket->dwId);
					CBaseItem* lpItem = g_pItemTableHash_get()->GetData(pPacket->dwId);

					if(lpItem)
					{						
						// MSG_ID : 86 ; Karz�� �����մϴ�. %s �������� ���� �Ҽ������ϴ�.
						wsprintf(szInfo, g_Message[ETC_MESSAGE86].szMessage, lpItem->szItemName_Kor);	
						DisplayMessageAdd(szInfo, 0xFFFFC309);
						_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
					}
				}								
			}
		}
		break;
	case 84:
	case 36:		// Armor ������ �� ��.
		{
			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"���� ��ȣ�� ������ �ִ� �������� �����Ͽ� ������� ��å�� ��ȯ����
				// ���� ��ȣ�� �������������Ͽ����� �˷��帳�ϴ�."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}
			else
			{
				if(pPacket->bEquipCode==0)
				{
					DWORD dwMoney = pPacket->dwMoney-g_pMainPlayer->m_dwMoney;
					
					g_pMainPlayer->m_dwMoney = pPacket->dwMoney;				
					// �޼��� //
					// MSG_ID : 83 ; %u Karz�� ȹ���߽��ϴ�.
					wsprintf(szInfo, g_Message[ETC_MESSAGE83].szMessage, dwMoney);	
					DisplayMessageAdd(szInfo, 0xFFFF2CFF);
				}
				else if(pPacket->bEquipCode==1)
				{					
					memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));
					// �޼��� //				
					// MSG_ID : 88 ; �����Ҽ� �ִ� Karz �ִ뷮�� ������ �־ �������� �� �� �����ϴ�.
					DisplayMessageAdd(g_Message[ETC_MESSAGE88].szMessage, 0xFFFFC309);	
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				}
				else
				{
					// �޼��� //
					// MSG_ID : 88 ; �����Ҽ� �ִ� Karz �ִ뷮�� ������ �־ �������� �� �� �����ϴ�.
					DisplayMessageAdd(g_Message[ETC_MESSAGE88].szMessage, 0xFFFFC309);	
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				}

				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
			}
		}
		break;
	case 85:	
	case 37:		// Armor ������ �� ��.
		{
			if(pPacket->bEquipCode==1)
			{					
				memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));
				g_pMainPlayer->m_dwMoney = pPacket->dwMoney;

//				LP_ITEM_TABLE lpItem = g_pItemTableHash_get()->GetData(pPacket->dwId);
				CBaseItem* lpItem = g_pItemTableHash_get()->GetData(pPacket->dwId);

				if(lpItem)
				{
					// MSG_ID : 89 ; %s �������� �����Ͽ����ϴ�.
					wsprintf(szInfo, g_Message[ETC_MESSAGE89].szMessage, lpItem->szItemName_Kor);		
					DisplayMessageAdd(szInfo, 0xFFFF2CFF);
				}				
			}
			else
			{
				if(pPacket->bEquipCode==2)
				{
					// MSG_ID : 90 ;  �κ��丮�� ������ �����մϴ�.
					DisplayMessageAdd(g_Message[ETC_MESSAGE90].szMessage, 0xFFFFC309);		
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				}
				else
				{
//					LP_ITEM_TABLE lpItem = g_pItemTableHash_get()->GetData(pPacket->dwId);
					CBaseItem* lpItem = g_pItemTableHash_get()->GetData(pPacket->dwId);

					if(lpItem)
					{						
						// MSG_ID : 86 ; Kartz�� �����մϴ�. %s �������� ���� �Ҽ������ϴ�.
						wsprintf(szInfo, g_Message[ETC_MESSAGE86].szMessage, lpItem->szItemName_Kor);	
						DisplayMessageAdd(szInfo, 0xFFFFC309);
						_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
					}
				}								
			}
		}
		break;
	case 38:		// ���콺���� Guardian â���� Guardian �������� �� ��.
		{
			int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;
			
			if(nValue!=ITEM_GUARDIAN_INDEX)
				return;

			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"���� ��ȣ�� ������ �ִ� �������� �����Ͽ� ������� ��å�� ��ȯ����
				// ���� ��ȣ�� �������������Ͽ����� �˷��帳�ϴ�."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}
			else
			{
				if(g_pMainPlayer->m_pInv_Guardian[pPacket->bZipCode].m_wItemID==0)
					memcpy(&g_pMainPlayer->m_pInv_Guardian[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));				
				else
				{
					for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
						{
							memcpy(&g_pMainPlayer->m_pInv_Large[i], &pPacket->Item, sizeof(CItem));
							break;
						}
					}
				}		
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));				
			}

			g_pMainPlayer->m_nItemSelect = 0;
		}
		break;
	case 39:	// Guardian â���� ���콺�� Guardian �������� �� ��.
		{
			int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;

			if(nValue==ITEM_GUARDIAN_INDEX)
			{								
				memcpy(&g_pMainPlayer->m_MouseItem, &pPacket->Item, sizeof(CItem));
				memset(&g_pMainPlayer->m_pInv_Guardian[pPacket->bZipCode], 0, sizeof(CItem));
				g_pMainPlayer->m_nItemSelect = 1;			
			}			
		}
		break;
	case 40:		// ���콺���� Guardian �������̽��� �Һ� �������� �� ��.
		{
			int		nValue		= pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;				
			BYTE	byPacketCnt	= 0;
 
			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));

				//"���� ��ȣ�� ������ �ִ� �������� �����Ͽ� ������� ��å�� ��ȯ����
				// ���� ��ȣ�� �������������Ͽ����� �˷��帳�ϴ�."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}
			else
			{
				if(nValue==ITEM_SUPPLIES_INDEX || (nValue >= ITEM_CONSUMABLEST_INDEX && nValue <= ITEM_CONSUMABLEEND_INDEX))
				{							
					// ������ ������ //
					if(g_pMainPlayer->m_GuardianItem.m_wItemID==0)
						memcpy(&g_pMainPlayer->m_GuardianItem.m_wItemID, &pPacket->Item, sizeof(CItem));						
					else
					{
						if(pPacket->Item.m_wItemID==g_pMainPlayer->m_GuardianItem.m_wItemID)
						{
							CBaseItem* lpItemTable = 
								g_pItemTableHash_get()->GetData(g_pMainPlayer->m_GuardianItem.m_wItemID);

							if(!lpItemTable)
								return;
							
							int nMaxQuantity	= 1;

							nMaxQuantity = lpItemTable->GetMaxQuantity();						

							int nItemQuantity	= nMaxQuantity - g_pMainPlayer->m_GuardianItem.GetQuantity();
							int nItemMouse		= pPacket->Item.GetQuantity();

							if(nItemQuantity>=nItemMouse)
							{
								g_pMainPlayer->m_GuardianItem.SetQuantity(BYTE(g_pMainPlayer->m_GuardianItem.GetQuantity()+nItemMouse));
							}
							else
							{
								g_pMainPlayer->m_GuardianItem.SetQuantity(BYTE(g_pMainPlayer->m_GuardianItem.GetQuantity()+nItemQuantity));
								byPacketCnt = BYTE(nItemMouse-nItemQuantity);
								
								for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
								{
									if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
									{						
										memcpy(&g_pMainPlayer->m_pInv_Small[i], &pPacket->Item, sizeof(CItem));
										g_pMainPlayer->m_pInv_Small[i].SetQuantity(byPacketCnt);									
										break;
									}
									else if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==pPacket->Item.m_wItemID)
									{
										CBaseItem* lpItemTable = g_pItemTableHash_get()->GetData(g_pMainPlayer->m_pInv_Small[i].m_wItemID);

										if(!lpItemTable)
											return;

										int nMaxQuantity	= 1;

										nMaxQuantity = lpItemTable->GetMaxQuantity();									

										if(g_pMainPlayer->m_pInv_Small[i].GetQuantity()<nMaxQuantity)
										{
											int nCount = nMaxQuantity - g_pMainPlayer->m_pInv_Small[i].GetQuantity();

											if(nCount>=byPacketCnt)
											{
												g_pMainPlayer->m_pInv_Small[i].m_Item_Supplies.bQuantity+=byPacketCnt;
												break;
											}
											else
											{
												g_pMainPlayer->m_pInv_Small[i].m_Item_Supplies.bQuantity+=nCount;
												byPacketCnt -= nCount;
											}						
										}
									}									
								}							
							}
						}
						else
						{
							Insert_SmallInvItem(&pPacket->Item, 0, pPacket->Item.GetQuantity());
						}
					}

					memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));							
				}
			}
		}
		break;
	case 41:		// Guardian �������̽����� ���콺�� �Һ� �������� �� ��.
		{
			int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;								

			if(nValue==ITEM_SUPPLIES_INDEX || (nValue >= ITEM_CONSUMABLEST_INDEX && nValue <= ITEM_CONSUMABLEEND_INDEX) )			
				memcpy(&g_pMainPlayer->m_MouseItem, &pPacket->Item, sizeof(CItem));
						
			if(g_Mouse.bLDown)
				g_pMainPlayer->m_nItemSelect = 1;

			memset(&g_pMainPlayer->m_GuardianItem, 0, sizeof(CItem));				
		}
		break;
	case 42:		// Guardian �������� ���콺���� ���� ����â����.
		{
			int nVal = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;

			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"���� ��ȣ�� ������ �ִ� �������� �����Ͽ� ������� ��å�� ��ȯ����
				// ���� ��ȣ�� �������������Ͽ����� �˷��帳�ϴ�."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}
			else
			{
				if(nVal==ITEM_GUARDIAN_INDEX)
				{
					CDungeonManagerWnd* pDungeonManagerWnd = CDungeonManagerWnd::GetInstance();

					if(pPacket->bZipCode2==0)
					{
						CItem* pItem = (CItem*)&pDungeonManagerWnd->m_pDungeon->m_cGuardianItem;					
						memcpy(pItem, &pPacket->Item, sizeof(CItem));
					}
					else if(pPacket->bZipCode2==1)
					{
						memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));
					}

					memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
					g_pMainPlayer->m_nItemSelect = 0;
				}
			}			
		}
		break;
	case 43:		// Guardian �������� ���� ����â���� ���콺�� �̵�.
		{
			CDungeonManagerWnd* pDungeonManagerWnd = CDungeonManagerWnd::GetInstance();

			if(!pDungeonManagerWnd->m_pDungeon)
				return;

			memcpy(&g_pMainPlayer->m_MouseItem, &pPacket->Item, sizeof(CItem));
			memset(&pDungeonManagerWnd->m_pDungeon->m_cGuardianItem, 0, sizeof(CItem));
			
			if(g_Mouse.bLDown)
				g_pMainPlayer->m_nItemSelect = 1;
		}
		break;
	case 44:		// Guardian���� ������ ���.
		{	
			// ����, ��ų�� ���� : �ִ��� 2005.1.17
			// ������ Zipcode, zipcode2, money�� ������
			// ��¼�ڴ°ž�!! -�ִ���
			char szTemp[0xff] = {0,};	// �޼��� ��¿� ����

			// HP ����
			if (pPacket->bZipCode == 0)
			{
				memcpy(&g_pMainPlayer->m_GuardianItem, &pPacket->Item, sizeof(CItem));

				CMonster *pGuardian = g_pMainPlayer->m_pGuardian[0];
				if(pGuardian)
				{
					pGuardian->m_dwHP = pPacket->dwMoney;
				}
			}
			
			// ��ȭ ����
			else if(pPacket->bZipCode == 1)
			{
				memcpy(&g_pMainPlayer->m_GuardianItem, &pPacket->Item, sizeof(CItem));
			}

			// ���� ����
			else if(pPacket->bZipCode == 2)
			{
				memcpy(&g_pMainPlayer->m_GuardianItem, &pPacket->Item, sizeof(CItem));

				// "%s �� ������ %d�� �����Ͽ����ϴ�."
				wsprintf(szTemp, g_Message[ETC_MESSAGE1277].szMessage, g_pMainPlayer->m_pGuardian[0]->m_szName, pPacket->dwMoney);
				DisplayMessageAdd(szTemp, 0xFFFFC309);

				// ���� ����Ʈ�� �Ѹ���.
				AppliedSkill*	pEffectDesc =  g_pEffectLayer->CreateGXObject(g_pObjManager->GetFile(SKILL_REMEDY), TRUE, __CHR_EFFECT_NONE__);
				pEffectDesc->byTargetObjectType[0]	= OBJECT_TYPE_MONSTER; 
				pEffectDesc->dwTargetIndex[0]		=g_pMainPlayer->m_pGuardian[0]->m_dwMonsterIndex; 
				GXSetPosition( pEffectDesc->hEffect.pHandle, &g_pMainPlayer->m_pGuardian[0]->m_v3CurPos, FALSE );
				::SetAction( pEffectDesc->hEffect.pHandle, 1, 0, ACTION_ONCE );
				pEffectDesc->hEffect.pDesc->ObjectFunc	= EffectOnceAndNormal;
			}
			
			// ��ų��
			else if(pPacket->bZipCode == 3)
			{
				if(pPacket->bZipCode2 == 0)	// ����, ���� ������ ��� : �ִ��� 2005.1.17
				{
					char *pFailMsg = NULL;
					switch(pPacket->dwMoney)
					{
					case 1:
						// "��ų�� ��� �� �ִ� ������ ���� �մϴ�."
						pFailMsg = g_Message[ETC_MESSAGE1276].szMessage;
						break;
					case 2:
						// "�̹� ��� ��ų �Դϴ�."
						pFailMsg = g_Message[ETC_MESSAGE1278].szMessage;
						break;
					case 3:
						// "��ų�� ��� �� �����ϴ�. ���� ������ ��ų�� ���� ���ž� �մϴ�."
						pFailMsg = g_Message[ETC_MESSAGE1275].szMessage;
						break;
					case 4:
						// "������� ������ ���� ��ų���� ���� �� �����ϴ�."
						pFailMsg = g_Message[ETC_MESSAGE1411].szMessage;
						break;
					default:
						break;
					}
					if(pFailMsg)
						DisplayMessageAdd(pFailMsg, 0xFFFF0000);
				}
				else if(pPacket->bZipCode2 > 0) // ����
				{
					memcpy(&g_pMainPlayer->m_GuardianItem, &pPacket->Item, sizeof(CItem));

					// "%s �� %d Lv�� %s ��ų�� ������ϴ�."
					wsprintf(szTemp, g_Message[ETC_MESSAGE1274].szMessage,
						g_pMainPlayer->m_pGuardian[0]->m_szName,
						pPacket->bZipCode2,
						g_pEffectLayer->m_Effect[pPacket->dwMoney].szName);
					DisplayMessageAdd(szTemp, 0xFFFFC309);

					// ��ų�� ����ٴ� ����Ʈ�� �Ѹ���.
					AppliedSkill*	pEffectDesc =  g_pEffectLayer->CreateGXObject(g_pObjManager->GetFile(SKILL_PRAY), TRUE, __CHR_EFFECT_NONE__);
					pEffectDesc->byTargetObjectType[0]	= OBJECT_TYPE_MONSTER; 
					pEffectDesc->dwTargetIndex[0]		=g_pMainPlayer->m_pGuardian[0]->m_dwMonsterIndex; 
					GXSetPosition( pEffectDesc->hEffect.pHandle, &g_pMainPlayer->m_pGuardian[0]->m_v3CurPos, FALSE );
					::SetAction( pEffectDesc->hEffect.pHandle, 1, 0, ACTION_ONCE );
					pEffectDesc->hEffect.pDesc->ObjectFunc	= EffectOnceAndNormal;
				}
			}

			// ���� ������
			else if(pPacket->bZipCode == 4)
			{
					memcpy(&g_pMainPlayer->m_GuardianItem, &pPacket->Item, sizeof(CItem));

					// "%s �� ������ �ʱ�ȭ�߽��ϴ�."
					wsprintf(szTemp, "%s�� ������ �ʱ�ȭ�߽��ϴ�.", g_pMainPlayer->m_pGuardian[0]->m_szName);
					DisplayMessageAdd(szTemp, 0xFFFFC309);

					// �ʱ�ȭ ����Ʈ �Ѹ���
					AppliedSkill*	pEffectDesc =  g_pEffectLayer->CreateGXObject(g_pObjManager->GetFile(SKILL_REMEDY), TRUE, __CHR_EFFECT_NONE__);
					pEffectDesc->byTargetObjectType[0]	= OBJECT_TYPE_MONSTER; 
					pEffectDesc->dwTargetIndex[0]		=g_pMainPlayer->m_pGuardian[0]->m_dwMonsterIndex; 
					GXSetPosition( pEffectDesc->hEffect.pHandle, &g_pMainPlayer->m_pGuardian[0]->m_v3CurPos, FALSE );
					::SetAction( pEffectDesc->hEffect.pHandle, 1, 0, ACTION_ONCE );
					pEffectDesc->hEffect.pDesc->ObjectFunc	= EffectOnceAndNormal;
			}

			// HP updating
			CMonster *pGuardian = g_pMainPlayer->m_pGuardian[0];//hwoarang
			if(pGuardian)
			{
				CUserInterface* pUserInterface = CUserInterface::GetInstance();
				pUserInterface->SetGauge(pGuardian->m_dwHP, pGuardian->m_dwMaxHP);
			}
		}
		break;
	case 101:
		{
			memcpy(&g_pMainPlayer->m_GuardianItem, &pPacket->Item, sizeof(CItem));
		}
		break;
	case 45:		// Guardian�� ������� ���� ������ �̵�.
		{
			int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;

			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"���� ��ȣ�� ������ �ִ� �������� �����Ͽ� ������� ��å�� ��ȯ����
				// ���� ��ȣ�� �������������Ͽ����� �˷��帳�ϴ�."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}
			else
			{
				if(nValue==ITEM_SUPPLIES_INDEX)
				Insert_SmallInvItem(&pPacket->Item, 0, pPacket->Item.GetQuantity());

				memset(&g_pMainPlayer->m_GuardianItem, 0, sizeof(CItem));			
			}		
		}
		break;
	case 46:		// ū �������� ���콺���� Ʈ���̵� â���� �̵� //
		{
			int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;
						
			if( (nValue>=ITEM_WEAPONST_INDEX && nValue<=ITEM_WEAPONEND_INDEX) ||
				(nValue>=ITEM_ARMORST_INDEX && nValue<=ITEM_ARMOREND_INDEX) ||
				nValue==ITEM_GUARDIAN_INDEX ||
				nValue==ITEM_RIDE_INDEX)
			{
				memcpy(&g_pMainPlayer->m_pItemTrade[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));
			}
				
			memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
			g_pMainPlayer->m_nItemSelect = 0;
		}
		break;
	case 47:
		{
			if(pPacket->Item.GetType()==ITEM_SMALL)
			{
				if(pPacket->Item.GetQuantity()>=pPacket->dwMoney)
				{			
					BYTE	byPacketCnt	= BYTE(pPacket->dwMoney);
					BYTE	byOutCnt	= BYTE(pPacket->Item.GetQuantity()-pPacket->dwMoney);
					
					if(byPacketCnt==pPacket->Item.GetQuantity())
						Insert_TradeItem(&pPacket->Item, pPacket->bZipCode, pPacket->Item.GetQuantity());
					else
					{
						pPacket->Item.SetQuantity(BYTE(pPacket->dwMoney));
						Insert_TradeItem(&pPacket->Item, pPacket->bZipCode, BYTE(pPacket->dwMoney));
						
						if(byOutCnt!=0)
						{
							pPacket->Item.SetQuantity(byOutCnt);
							Insert_SmallInvItem(&pPacket->Item, 0, byOutCnt);
						}
					}
				}
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				pQuantityWnd->m_bReq = FALSE;						
			}

			if(g_Mouse.bLDown)
				g_pMainPlayer->m_nItemSelect = 1;
		}
		break;
	case 48:		// ū �������� Ʈ���̵� â���� ���콺�� �̵�.
		{
			int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;

			if( (nValue>=ITEM_WEAPONST_INDEX && nValue<=ITEM_WEAPONEND_INDEX) ||
				(nValue>=ITEM_ARMORST_INDEX && nValue<=ITEM_ARMOREND_INDEX) ||
				nValue==ITEM_GUARDIAN_INDEX ||
				nValue==ITEM_RIDE_INDEX)
			{
				memcpy(&g_pMainPlayer->m_MouseItem, &g_pMainPlayer->m_pItemTrade[pPacket->bZipCode], sizeof(CItem));
				memset(&g_pMainPlayer->m_pItemTrade[pPacket->bZipCode], 0, sizeof(CItem));
			}

			if(g_Mouse.bLDown)
				g_pMainPlayer->m_nItemSelect = 1;
		}
		break;
	case 49:		// ���� �������� Ʈ���̵� â���� ���콺�� �̵�.
		{
		//	int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;

			if(pPacket->Item.GetType()==ITEM_SMALL)
			{
				memcpy(&g_pMainPlayer->m_MouseItem, &g_pMainPlayer->m_pItemTrade[pPacket->bZipCode], sizeof(CItem));
				memset(&g_pMainPlayer->m_pItemTrade[pPacket->bZipCode], 0, sizeof(CItem));
			}

			if(g_Mouse.bLDown)
				g_pMainPlayer->m_nItemSelect = 1;
		}
		break;
	case 50:
	case 55:
		{
			int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;

			if(nValue==ITEM_ZODIAC_INDEX)
			{
				if(pPacket->Item.GetID()==__ITEM_PORTAL_INDEX__)
				{
					if(pPacket->bInv==50)
					{
						if(pPacket->Item.m_Item_Zodiac.bType==1)
							memcpy(&g_pMainPlayer->m_pInv_Small[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));
					}
					else 
					{
						if (pPacket->Item.m_Item_Zodiac.bType == 1) {
							g_pMainPlayer->setBeltItem(pPacket->Item, pPacket->bZipCode);
						}
					}					
				}				
			}
									
			// "��ǥ�� ����Ǿ����ϴ�."
			DisplayMessageAdd(g_Message[ETC_MESSAGE511].szMessage, 0xFFFF2CFF); 
		}
		break;
	case 51:
	case 54:
		{
		//	int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;

			//if(nValue==ITEM_ZODIAC_INDEX)
			{
				//if(pPacket->Item.GetID()==__ITEM_PORTAL_INDEX__)
				{
					if(pPacket->bInv==51)
					{
						WORD wToDungeonID = g_pMainPlayer->m_pInv_Small[pPacket->bEquipCode].m_Item_Zodiac.wMapId;	
						SendPacketRequestDungeonInfo(wToDungeonID);						
						
						g_pMainPlayer->SetMotion(36);
						g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc = NULL;

						SendStopPacket();
						
						CTDS_PORTAL_MOVE packet;			
						packet.wPortalItemID    = (WORD)pPacket->dwMoney;
						packet.wToDungeonID		= g_pMainPlayer->m_pInv_Small[pPacket->bEquipCode].m_Item_Zodiac.wMapId;
						packet.bLayerNo			= g_pMainPlayer->m_pInv_Small[pPacket->bEquipCode].m_Item_Zodiac.bLayer;
						packet.v2Pos.x			= (float)g_pMainPlayer->m_pInv_Small[pPacket->bEquipCode].m_Item_Zodiac.wPosX;
						packet.v2Pos.y			= (float)g_pMainPlayer->m_pInv_Small[pPacket->bEquipCode].m_Item_Zodiac.wPosZ;

						g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);
						g_pMainPlayer->SetStatus(UNIT_STATUS_PORTAL_MOVING);
						
						CItemWnd* pWnd = CItemWnd::GetInstance();
						pWnd->SetActive(FALSE);

						if(pPacket->Item.GetQuantity()>1)
							memcpy(&g_pMainPlayer->m_pInv_Small[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));
						else
							memset(&g_pMainPlayer->m_pInv_Small[pPacket->bZipCode], 0, sizeof(CItem));

						memset(&g_pMainPlayer->m_GuardianItem, 0, sizeof(CItem));
					}	
					else
					{
						const auto item = g_pMainPlayer->beltItemAtIndex(pPacket->bEquipCode);
						WORD wToDungeonID = item.m_Item_Zodiac.wMapId;
						SendPacketRequestDungeonInfo(wToDungeonID);						
						
						g_pMainPlayer->SetMotion(36);
						g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc = NULL;

						SendStopPacket();

						CTDS_PORTAL_MOVE packet;			
						packet.wPortalItemID    = (WORD)pPacket->dwMoney;
						packet.wToDungeonID		= item.m_Item_Zodiac.wMapId;
						packet.bLayerNo			= item.m_Item_Zodiac.bLayer;
						packet.v2Pos.x			= (float)item.m_Item_Zodiac.wPosX;
						packet.v2Pos.y			= (float)item.m_Item_Zodiac.wPosZ;

						g_pNet->SendMsg((char*)&packet, packet.GetPacketSize
							(), SERVER_INDEX_ZONE);
						g_pMainPlayer->SetStatus(UNIT_STATUS_PORTAL_MOVING);

						CItemWnd* pWnd = CItemWnd::GetInstance();
						pWnd->SetActive(FALSE);

						if(pPacket->Item.GetQuantity()>1)
							memcpy(&g_pMainPlayer->m_pInv_Small[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));
						else
							memset(&g_pMainPlayer->m_pInv_Small[pPacket->bZipCode], 0, sizeof(CItem));												

						memset(&g_pMainPlayer->m_GuardianItem, 0, sizeof(CItem));
					}
				}
			}
		}
		break;
	case 52:
	case 53:
		{
			int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;

			if(nValue==ITEM_ZODIAC_INDEX)
			{

				// ���� ��ȯ ��ũ�� �߰� 2005.02.14 �迵�� 
				WORD wItemID = pPacket->Item.GetID(); 
				if(wItemID >= __ITEM_PORTAL_VILL1__ && wItemID <= __ITEM_PORTAL_VILL6__)
				{
					WORD wToDungeonID = pPacket->Item.m_Item_Zodiac.wMapId;	
					
					SendPacketRequestDungeonInfo(wToDungeonID);

					g_pMainPlayer->SetMotion(36);
					g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc = NULL;	

					SendStopPacket();

					CTDS_PORTAL_MOVE packet;	
					packet.wPortalItemID    = (WORD)pPacket->dwMoney;
					packet.wToDungeonID		= pPacket->Item.m_Item_Zodiac.wMapId;					
					packet.bLayerNo			= pPacket->Item.m_Item_Zodiac.bLayer;
					packet.v2Pos.x			= (float)pPacket->Item.m_Item_Zodiac.wPosX;
					packet.v2Pos.y			= (float)pPacket->Item.m_Item_Zodiac.wPosZ;

					g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);
					g_pMainPlayer->SetStatus(UNIT_STATUS_PORTAL_MOVING);

					CItemWnd* pWnd = CItemWnd::GetInstance();
					pWnd->SetActive(FALSE);

					if(pPacket->bInv==52)
					{
						if(pPacket->Item.GetQuantity()>1)
						{
							memcpy(&g_pMainPlayer->m_pInv_Small[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));
							g_pMainPlayer->m_pInv_Small[pPacket->bZipCode].SetQuantity(BYTE(pPacket->Item.GetQuantity()-1));
						}
						else
							memset(&g_pMainPlayer->m_pInv_Small[pPacket->bZipCode], 0, sizeof(CItem));
					}	
					else
					{
						if(pPacket->Item.GetQuantity() > 1) {
							g_pMainPlayer->setBeltItem(pPacket->Item, pPacket->bZipCode);
							g_pMainPlayer->m_pInv_Small[pPacket->bZipCode].SetQuantity(BYTE(pPacket->Item.GetQuantity()-1));
						}
						else {
							g_pMainPlayer->nullifyBeltItemAtIndex(pPacket->bZipCode);
						}
					}

					memset(&g_pMainPlayer->m_GuardianItem, 0, sizeof(CItem));
				}
			}
		}
		break;
	case 56:		// ��Ƽ�� �۽��� ��Ż ���.
		{
			memcpy(&g_pMainPlayer->m_pInv_Small[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));

			memset(&g_pMainPlayer->m_GuardianItem, 0, sizeof(CItem));
		}
		break;
	case 57:		// ���콺���� ���������� ���� �������̵�.
		{
			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"���� ��ȣ�� ������ �ִ� �������� �����Ͽ� ������� ��å�� ��ȯ����
				// ���� ��ȣ�� �������������Ͽ����� �˷��帳�ϴ�."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}
			else
			{
				if(pPacket->Item.GetID()!=0)
				{
					if(pPacket->bZipCode2==1)
					{
						memcpy(&g_pMainPlayer->m_sPlayerShop.cItem[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));
						memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
						g_pMainPlayer->m_sPlayerShop.m_dwCustomSellPrice[pPacket->bZipCode]	= pPacket->dwMoney;									
						g_pMainPlayer->m_nItemSelect	= 0;
						pQuantityWnd->m_bReq			= FALSE;
						g_pMainPlayer->m_dwPlayserShop	= pPacket->dwMoney;
						g_pMainPlayer->m_nItemSelect	= 0;
					}
					else if(pPacket->bZipCode2==2)
					{
						memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));
						memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));					
						g_pMainPlayer->m_nItemSelect	= 0;
						pQuantityWnd->m_bReq			= FALSE;					
						g_pMainPlayer->m_nItemSelect	= 0;
					}				
				}			
			}
		}
		break;
	case 58:		// �����󿡼� ���콺�� ���� �������̵�.
		{
			pPlayerShopWnd->m_dwMoney = g_pMainPlayer->m_sPlayerShop.m_dwCustomSellPrice[pPacket->bZipCode];
			
			memcpy(&g_pMainPlayer->m_MouseItem, &pPacket->Item, sizeof(CItem));
			memset(&g_pMainPlayer->m_sPlayerShop.cItem[pPacket->bZipCode], 0, sizeof(CItem));			
			g_pMainPlayer->m_sPlayerShop.m_dwCustomSellPrice[pPacket->bZipCode]	= 0;			
			g_pMainPlayer->m_dwPlayserShop	= pPacket->dwMoney;
			
			if(g_Mouse.bLDown)
				g_pMainPlayer->m_nItemSelect = 1;
		}
		break;
	case 59:		// ���콺���� ���������� �Һ� ������ �̵�.
		{				
		//	int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;
				
			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"���� ��ȣ�� ������ �ִ� �������� �����Ͽ� ������� ��å�� ��ȯ����
				// ���� ��ȣ�� �������������Ͽ����� �˷��帳�ϴ�."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}
			else
			{
				if(pPacket->Item.GetType()==ITEM_SMALL)
				{					
					if(pPacket->Item.GetQuantity()>=pPacket->dwMoney)
					{			
						BYTE	byPacketCnt	= BYTE(pPacket->dwMoney);
						BYTE	byOutCnt	= BYTE(pPacket->Item.GetQuantity()-pPacket->dwMoney);
						
						if(byPacketCnt==pPacket->Item.GetQuantity())
							Insert_PlayerShopItem(&pPacket->Item, pPacket->bZipCode, pPacket->Item.GetQuantity());
						else
						{
							pPacket->Item.SetQuantity(BYTE(pPacket->dwMoney));
							Insert_PlayerShopItem(&pPacket->Item, pPacket->bZipCode, BYTE(pPacket->dwMoney));
							
							if(byOutCnt!=0)
							{
								pPacket->Item.SetQuantity(byOutCnt);
								Insert_SmallInvItem(&pPacket->Item, 0, byOutCnt);
							}
						}
					}
					
					memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
					pQuantityWnd->m_bReq = FALSE;						
					g_pMainPlayer->m_dwPlayserShop	= pPacket->dwId;
					g_pMainPlayer->m_sPlayerShop.m_dwCustomSellPrice[pPacket->bZipCode2] = pPacket->dwId;
				}
			}			

			if(g_Mouse.bLDown)
				g_pMainPlayer->m_nItemSelect = 1;
		}
		break;
	case 60:		// �����󿡼� ���콺�� �Һ� �������̵�.
		{
			pPlayerShopWnd->m_dwMoney = g_pMainPlayer->m_sPlayerShop.m_dwCustomSellPrice[pPacket->bZipCode];

			memcpy(&g_pMainPlayer->m_MouseItem, &pPacket->Item, sizeof(CItem));
			memset(&g_pMainPlayer->m_sPlayerShop.cItem[pPacket->bZipCode], 0, sizeof(CItem));			
			g_pMainPlayer->m_sPlayerShop.m_dwCustomSellPrice[pPacket->bZipCode]	= 0;
			g_pMainPlayer->m_dwPlayserShop	= pPacket->dwId;
			
			if(g_Mouse.bLDown)
				g_pMainPlayer->m_nItemSelect = 1;
			
		}
		break;
	case 61:		// ��Ƽ ������ ���.
		{		
			memcpy(&g_pMainPlayer->m_pInv_Small[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));		
		}
		break;
	case 62:		// ��Ƽ ��ü ��Ż ���.
		{
			memcpy(&g_pMainPlayer->m_pInv_Small[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));	
		}
		break;
	case 63:// ���콺���� �������� â���� ������ �̵�
		{
			int nValue = pPacket->Item.GetID()/ITEM_DISTRIBUTE;

			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"���� ��ȣ�� ������ �ִ� �������� �����Ͽ� ������� ��å�� ��ȯ����
				// ���� ��ȣ�� �������������Ͽ����� �˷��帳�ϴ�."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}
			else
			{
				if(nValue==ITEM_MATERIALEND_INDEX)
					Insert_MagicFieldArrayItem(&pPacket->Item, BYTE(pPacket->dwMoney));
			
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
			}			
		}
		break;
	case 64://��������â���� �������� ���콺��
		{
			int nValue = pPacket->Item.GetID()/ITEM_DISTRIBUTE;
	
			CDungeonManagerWnd* pDungeonManagerWnd = CDungeonManagerWnd::GetInstance();

			if(nValue==ITEM_MATERIALEND_INDEX)
			{								
				memcpy(&g_pMainPlayer->m_MouseItem, &pPacket->Item, sizeof(CItem));
				g_pMainPlayer->m_MouseItem.SetQuantity(1);
				
				if(pPacket->Item.GetQuantity()>1)
				{
					memcpy(&pDungeonManagerWnd->m_pDungeon->m_cMagicFieldArrayItem, &pPacket->Item, sizeof(CItem));
					pDungeonManagerWnd->m_pDungeon->m_cMagicFieldArrayItem.SetQuantity(BYTE(pPacket->Item.GetQuantity()-1));
				}
				else
					memset(&pDungeonManagerWnd->m_pDungeon->m_cMagicFieldArrayItem, 0, sizeof(CItem));				
			}

			if(g_Mouse.bLDown)
				g_pMainPlayer->m_nItemSelect = 1;

			CQuantityWnd::GetInstance()->m_bReq = FALSE;
		}
		break;
	case 65:
		{
			int nValue = pPacket->Item.GetID()/ITEM_DISTRIBUTE;

			CItemUpgradeWnd* pItemUpgradeWnd = CItemUpgradeWnd::GetInstance();

			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"���� ��ȣ�� ������ �ִ� �������� �����Ͽ� ������� ��å�� ��ȯ����
				// ���� ��ȣ�� �������������Ͽ����� �˷��帳�ϴ�."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}
			else if(pPacket->bZipCode==254)
			{
				// ���� ���߳� 
				DisplayMessageAdd(g_Message[ETC_MESSAGE1004].szMessage, 0xffffff00);
				memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bEquipCode],&pPacket->Item,sizeof(CItem));
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				g_pMainPlayer->m_nItemSelect = 0;
			}
			else
			{
				if(GetType(WORD(nValue))==ITEM_LARGE)
				{
					if(	(nValue>=ITEM_WEAPONST_INDEX && nValue<=ITEM_WEAPONEND_INDEX) ||
						(nValue>=ITEM_ARMORST_INDEX && nValue<=ITEM_ARMOREND_INDEX))
					{
						if(pItemUpgradeWnd->m_pUpgradeItem[0].GetID()==0)
						{
							memcpy(&pItemUpgradeWnd->m_pUpgradeItem[0], &pPacket->Item, sizeof(CItem));						
							memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
							g_pMainPlayer->m_nItemSelect = 0;
						}
					}
				}
			}			
		}
		break;
	case 66:
		{
			int nValue = pPacket->Item.GetID()/ITEM_DISTRIBUTE;

			CItemUpgradeWnd* pItemUpgradeWnd = CItemUpgradeWnd::GetInstance();

			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"���� ��ȣ�� ������ �ִ� �������� �����Ͽ� ������� ��å�� ��ȯ����
				// ���� ��ȣ�� �������������Ͽ����� �˷��帳�ϴ�."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}
			else
			{
				if(GetType(WORD(nValue))==ITEM_LARGE)
				{
					if(nValue>=ITEM_WEAPONST_INDEX && nValue<=ITEM_ARMOREND_INDEX)
					{																					
						memcpy(&g_pMainPlayer->m_MouseItem, &pPacket->Item, sizeof(CItem));										
						memset(&pItemUpgradeWnd->m_pUpgradeItem[0], 0, sizeof(CItem));
					}
				}				
			}
			
			if(g_Mouse.bLDown)
				g_pMainPlayer->m_nItemSelect = 1;
		}
		break;
	case 67:
		{
			int nValue = pPacket->Item.GetID()/ITEM_DISTRIBUTE;

			if(GetType(WORD(nValue))==ITEM_SMALL)
			{
				if( nValue == ITEM_UPGRADE_INDEX || nValue == ITEM_LIQUID_INDEX || nValue == ITEM_EDITION_INDEX )
				{
					if(pPacket->bEquipCode==0)
					{
						Insert_UpgradeItem(&pPacket->Item, pPacket->bZipCode, BYTE(pPacket->dwMoney));
					}
					else if(pPacket->bEquipCode==1)						
					{
						Insert_SmallInvItem(&pPacket->Item, 0, pPacket->Item.GetQuantity());					
					}

					memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
					g_pMainPlayer->m_nItemSelect = 0;
				}
			}
		}
		break;
	case 68:
		{
			int nValue = pPacket->Item.GetID()/ITEM_DISTRIBUTE;

			CItemUpgradeWnd* pItemUpgradeWnd = CItemUpgradeWnd::GetInstance();

			if(GetType(WORD(nValue))==ITEM_SMALL)
			{
				if( nValue == ITEM_UPGRADE_INDEX || nValue == ITEM_LIQUID_INDEX || nValue == ITEM_EDITION_INDEX )
				{																					
					memcpy(&g_pMainPlayer->m_MouseItem, &pPacket->Item, sizeof(CItem));										
					memset(&pItemUpgradeWnd->m_pUpgradeItem[pPacket->bZipCode], 0, sizeof(CItem));
				}
			}

			if(g_Mouse.bLDown)
				g_pMainPlayer->m_nItemSelect = 1;
		}
		break;
	case 69:
		{			
			memcpy(&g_pMainPlayer->m_pInv_Small[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));				
		}
		break;
	case 70:
		{
			Insert_SmallInvItem(&pPacket->Item, 0, pPacket->Item.GetQuantity());
		}
		break;
	case 71:
		{
			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"���� ��ȣ�� ������ �ִ� �������� �����Ͽ� ������� ��å�� ��ȯ����
				// ���� ��ȣ�� �������������Ͽ����� �˷��帳�ϴ�."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}
			else
			{
				_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_GETITEM, g_v3InterfaceSoundPos, FALSE);

				Insert_SmallBeltItem(&pPacket->Item, 0, pPacket->Item.GetQuantity(), TRUE);

				if(pPacket->Item.GetType()!=ITEM_SMALL)			
					return;

				lpItemTable = g_pItemTableHash_get()->GetData(pPacket->Item.GetID());

				if(lpItemTable)
				{
					// MSG_ID : 82 ; %s �������� %d�� ȹ���Ͽ����ϴ�.
					wsprintf(szInfo, g_Message[ETC_MESSAGE82].szMessage, lpItemTable->szItemName_Kor
						, pPacket->Item.GetQuantity());	
					DisplayMessageAdd(szInfo, 0xFFFF2CFF, pGameMenuWnd->m_bSystemMsgFlag);
				}					
			}			
		}
		break;
	case 72:
		{
			if(pPacket->bZipCode==255)
			{	
				// "������ ��ȣ�� �����ϴ� ����������� ���ɼ��� �־� �����Ͻ� ���� �����ϴ�."
				DisplayMessageAdd(g_Message[ETC_MESSAGE993].szMessage, 0xFFFF0000);
			}
			else
			{
				g_pMainPlayer->m_dwMoney = pPacket->dwMoney;
				memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));
				memset(&pPlayerShopWnd->m_sPlayerShopInfo.cItem[pPacket->bZipCode2], 0, sizeof(CItem));
			}			
		}
		break;
	case 73:
		{
			if(pPacket->bZipCode==255)
			{	
				// "������ ��ȣ�� �����ϴ� ����������� ���ɼ��� �־� �����Ͻ� ���� �����ϴ�."
				DisplayMessageAdd(g_Message[ETC_MESSAGE993].szMessage, 0xFFFF0000);
			}
			else
			{
				g_pMainPlayer->m_dwMoney = pPacket->dwMoney;
				Insert_SmallInvItem(&pPacket->Item, 0, pPacket->Item.GetQuantity());

				if(pPacket->bEquipCode==0)
					memset(&pPlayerShopWnd->m_sPlayerShopInfo.cItem[pPacket->bZipCode2], 0, sizeof(CItem));
				else
					pPlayerShopWnd->m_sPlayerShopInfo.cItem[pPacket->bZipCode2].SetQuantity(pPacket->bEquipCode);
			}
			
			CQuantityWnd::GetInstance()->m_bReq = FALSE;
		}
		break;
	case 74:
		{
			if(pPacket->bEquipCode==0)
				memcpy(&g_pMainPlayer->m_pInv_Small[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));
			else if (pPacket->bEquipCode == 1) {
				g_pMainPlayer->setBeltItem(pPacket->Item, pPacket->bZipCode);
			}
		}
		break;
	case 75:
		{
			memcpy(&g_pMainPlayer->m_pInv_Small[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));
		}
		break;
	case 76:
		{
			memcpy(&g_pMainPlayer->m_pInv_Small[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));
			memset(&g_pMainPlayer->m_GuardianItem, 0, sizeof(CItem));
		}
		break;
	case 77:
		{
			if (pPacket->bEqiupCode2 == 1) {
				g_pMainPlayer->setBeltItem(pPacket->Item, pPacket->bZipCode);
			} else {
				memcpy(&g_pMainPlayer->m_pInv_Small[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));
			}

		}
		break;

	case 78:	//�������� ������ ����(�±ر�) 
		{	
			DWORD dwMsgId = 0;

			if(pPacket->bEqiupCode2 > 0)		//�ζǴ� Error
			{
				switch(pPacket->bEqiupCode2)
				{
				case 0xff:	//��!
					dwMsgId = ETC_MESSAGE900;break; // "��! ������ȸ��..."
				case 254:	//��������!
					dwMsgId = ETC_MESSAGE901;break; // "�̺�Ʈ �����Դϴ�. (ErrorCode:254)"
				case 253:	//�κ��� ��ĭ ����!
					dwMsgId = ETC_MESSAGE90;break; // "�κ��丮�� ������ �����մϴ�."
				case 252:	//�������� Ǯ�� ����!
					dwMsgId = ETC_MESSAGE903;break; // "�̺�Ʈ �����Դϴ�. (ErrorCode:252)"
				}

				DisplayMessageAdd(g_Message[dwMsgId].szMessage, 0xFFFF2CFF);
			}
			else
			{
				PlayItemSound(pPacket->Item.m_wItemID);
				
				int nValue = pPacket->dwId / ITEM_DISTRIBUTE;
				if(GetType(WORD(nValue)) == ITEM_SMALL)
					Insert_SmallInvItem(&pPacket->Item, pPacket->bZipCode, pPacket->Item.GetQuantity());
				
				if(GetType(WORD(nValue)) == ITEM_LARGE)
					memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));
				

//				LP_ITEM_TABLE lpItem = g_pItemTableHash_get()->GetData(pPacket->dwId);
				CBaseItem* lpItem = g_pItemTableHash_get()->GetData(pPacket->dwId);

				if(lpItem)
				{	
					// "%s �������� %u�� ������ϴ�"
#ifdef TAIWAN_LOCALIZING // �븸�� ��� �־���� ��ġ�� �츮����� �޶� ��ġ�� ���� �ȴ�.
					wsprintf(szInfo, g_Message[ETC_MESSAGE902].szMessage, pPacket->Item.GetQuantity(), lpItem->szItemName_Kor );	
#else
					wsprintf(szInfo, g_Message[ETC_MESSAGE902].szMessage, lpItem->szItemName_Kor, pPacket->Item.GetQuantity());	
#endif
					DisplayMessageAdd(szInfo, 0xFFFF2CFF);											
				}	
				
				if(pItemWnd->m_byItemChk==0)
				{
					pItemWnd->m_byItemChk = 1;
				}
			}
		}
		break;
	case 79:
		{
			CItemMakingWnd* pItemMakingWnd = CItemMakingWnd::GetInstance();
			int nValue = pPacket->Item.GetID()/ITEM_DISTRIBUTE;

			if(GetType(WORD(nValue))==ITEM_SMALL)
			{
				if( (nValue >= ITEM_MATERIALST_INDEX && nValue <= ITEM_MATERIALEND_INDEX) ||
					pPacket->Item.GetID() == __ITEM_MAKING_INDEX__ )
				{
					if(pPacket->bEquipCode==0)
					{
						Insert_MakingItem(&pPacket->Item, pPacket->bZipCode, BYTE(pPacket->dwMoney));
						pItemMakingWnd->m_byProcess = ITEM_MAKING_PROCESS_DEFAULT;
					}
					else if(pPacket->bEquipCode==1)						
					{
						Insert_SmallInvItem(&pPacket->Item, 0, pPacket->Item.GetQuantity());					
					}

					memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
					g_pMainPlayer->m_nItemSelect = 0;
				}
			}
		}
		break;
	case 80:
		{
			int nValue = pPacket->Item.GetID()/ITEM_DISTRIBUTE;

			CItemMakingWnd* pItemMakingWnd = CItemMakingWnd::GetInstance();

			if(GetType(WORD(nValue))==ITEM_SMALL)
			{
				if( (nValue >= ITEM_MATERIALST_INDEX && nValue <= ITEM_MATERIALEND_INDEX) ||
					pPacket->Item.GetID() == __ITEM_MAKING_INDEX__ )
				{																					
					memcpy(&g_pMainPlayer->m_MouseItem, &pPacket->Item, sizeof(CItem));										
					memset(&pItemMakingWnd->m_pMakingItem[pPacket->bZipCode], 0, sizeof(CItem));
				}
			}

			if(g_Mouse.bLDown)
			{
				g_pMainPlayer->m_nItemSelect = 1;
			}
		}
		break;
	case 81:
		{  // ������ ���� â���� ���콺�� ������ �̵� 2005.01.25 �迵��
			int nValue = pPacket->Item.GetID()/ITEM_DISTRIBUTE;

			CItemMakingWnd* pItemMakingWnd = CItemMakingWnd::GetInstance();

			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"���� ��ȣ�� ������ �ִ� �������� �����Ͽ� ������� ��å�� ��ȯ����
				// ���� ��ȣ�� �������������Ͽ����� �˷��帳�ϴ�."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}
			else
			{
				if(GetType(WORD(nValue))==ITEM_LARGE)
				{
					if(	(nValue>=ITEM_WEAPONST_INDEX && nValue<=ITEM_WEAPONEND_INDEX) ||
						(nValue>=ITEM_ARMORST_INDEX && nValue<=ITEM_ARMOREND_INDEX) ||
						nValue == ITEM_RIDE_INDEX ||
						nValue == ITEM_GUARDIAN_INDEX ||
						nValue == ITEM_BAG_INDEX )
					{																					
						memcpy(&g_pMainPlayer->m_MouseItem, &pPacket->Item, sizeof(CItem));										
						memset(&pItemMakingWnd->m_pMakingItem[0], 0, sizeof(CItem));

						pItemMakingWnd->m_byProcess = ITEM_MAKING_PROCESS_DEFAULT;
					}
				}				
			}
			
			if(g_Mouse.bLDown)
				g_pMainPlayer->m_nItemSelect = 1;			
		}
		break;
	case 82:// ���� �� ���ŵɶ� 
		{
			if (!memcmp(g_pMainPlayer->m_pInv_Guardian[pPacket->bZipCode].GetSerial()
				,pPacket->Item.GetSerial(), sizeof(ITEM_SERIAL)))
			{
				CItem* pItem = &g_pMainPlayer->m_pInv_Guardian[pPacket->bZipCode];
				memcpy(pItem, &pPacket->Item, sizeof(CItem));
				
				CBaseItem* lpItemTable = g_pItemTableHash_get()->GetData(pItem->m_wItemID);

				CUserInterface* pUserInterface = CUserInterface::GetInstance();

				if (lpItemTable->BaseItem_Guardian.bType == GUARDIAN_ITEM_TYPE_EGG)
				{
					pUserInterface->SetGauge(pItem->m_Item_Guardian.dwCompleteTime
						, lpItemTable->BaseItem_Guardian.wCompleteTime);
				}
			}
		}
		break;
	case 83:
		{
			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"���� ��ȣ�� ������ �ִ� �������� �����Ͽ� ������� ��å�� ��ȯ����
				// ���� ��ȣ�� �������������Ͽ����� �˷��帳�ϴ�."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}
			else
			{
				if(pPacket->bEquipCode==0)
				{
					DWORD dwMoney = pPacket->dwMoney-g_pMainPlayer->m_dwMoney;
					
					g_pMainPlayer->m_dwMoney = pPacket->dwMoney;				
					// �޼��� //
					// MSG_ID : 83 ; %u Karz�� ȹ���߽��ϴ�.
					wsprintf(szInfo, g_Message[ETC_MESSAGE83].szMessage, dwMoney);	
					DisplayMessageAdd(szInfo, 0xFFFF2CFF);
				}
				else if(pPacket->bEquipCode==1)
				{					
					memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));
					// �޼��� //				
					// MSG_ID : 88 ; �����Ҽ� �ִ� Karz �ִ뷮�� ������ �־ �������� �� �� �����ϴ�.
					DisplayMessageAdd(g_Message[ETC_MESSAGE88].szMessage, 0xFFFFC309);	
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				}
				else
				{
					// �޼��� 
					// MSG_ID : 88 ; �����Ҽ� �ִ� Karz �ִ뷮�� ������ �־ �������� �� �� �����ϴ�.
					DisplayMessageAdd(g_Message[ETC_MESSAGE88].szMessage, 0xFFFFC309);	
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				}

				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
			}
		}
		break;
	// ������ ����  	
	case 100:
		{
			memcpy(&g_pMainPlayer->m_pInv_Small[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));
		}
		break;
	}

	if(pItemWnd->GetActive())
	{
		if(pItemWnd->m_byItemChk==0)
		{		
			pItemWnd->SetRender(SPR_OBJ_ITEM_CHK1, FALSE);
			pItemWnd->SetRender(SPR_OBJ_ITEM_CHK2, TRUE);
		}		
	}
	
	if(g_pMainPlayer->m_nItemSelect==1)
	{
		CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_ITEMCLICK__);		
	}
		
	pItemWnd->SetLine(FALSE, 0);
	
	if(pBankWnd->GetActive()==TRUE)	
	{	
		pBankWnd->SetRender(SPR_OBJ_BANK_CHK1, FALSE);
		pBankWnd->SetRender(SPR_OBJ_BANK_CHK2, FALSE);
		
		if(pBankWnd->m_byBankChk==0)
			pBankWnd->SetRender(SPR_OBJ_BANK_CHK1, TRUE);
		else
			pBankWnd->SetRender(SPR_OBJ_BANK_CHK2, TRUE);
	}			

	if(pItemWnd->GetActive()==TRUE)
		pItemWnd->SetLine(TRUE, pItemWnd->m_byItemChk);	

	if(pQuantityWnd->GetActive()==FALSE)
		pQuantityWnd->m_bReq = FALSE;

	g_pMainPlayer->WeightProcess(TRUE, wWeight);	
	g_pMainPlayer->GetCheckUpgrade();

	g_pMainPlayer->notifyForInventoryUpdates();
}


void CmdJoinDungeon( char* pMsg, DWORD dwLen ) {
	DSTC_JOIN* pJoin = (DSTC_JOIN*)pMsg;
	DungeonProcessHandleDungeonJoinEvent(pJoin);
}

void CmdChangeLayer( char* pMsg, DWORD dwLen ) {		
	DSTC_CHANGE_LAYER*	pChangeLayer = (DSTC_CHANGE_LAYER*)pMsg;
	DungeonProcessHandleLayerChangeEvent(pChangeLayer);
}


void CmdChangeSection( char* pMsg, DWORD dwLen )
{
	DSTC_CHANGE_SECTION*	pSection = (DSTC_CHANGE_SECTION*)pMsg;

	int			i			= 0;
	CMonster*	pMonster	= 0;
	CUser*		pUser		= 0;

	UpdateMainPlayer( pSection->wChageSectionNum );

	for(i = 0; i < pSection->bDisAppearMonsterCount + pSection->bDisAppearUserCount; i++)
	{
		if(i < pSection->bDisAppearUserCount)
		{
			// user!
			pUser = g_pUserHash->GetData( pSection->pdwDisAppearIndex[i] );
			if( !pUser ) continue;
			if( pUser == g_pMainPlayer ) continue;
			
			LOGOUT( "Remove User %d", pUser->m_dwUserIndex );
			RemovePlayer( pUser );
		}
		else
		{
			// monster!
			pMonster = g_pMonsterHash->GetData( pSection->pdwDisAppearIndex[i] );
			if( !pMonster ) continue;

			RemoveMonster( pMonster );
		}
	}
}


void CmdLevelUP( char* pMsg, DWORD dwLen )
{
	DSTC_LEVELUP*	pLevelUP = (DSTC_LEVELUP*)pMsg;
	
	CUser*	pUser = g_pUserHash->GetData( pLevelUP->dwUserIndex );
	if( !pUser ) 
	{
		RequestDungeonInfo( 1, pLevelUP->dwUserIndex );
		return;
	}

	_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_LEVELUP, g_v3InterfaceSoundPos, FALSE);

	// ������ ����Ʈ�� �Ѹ���.
	AppliedSkill*	pEffectDesc =  g_pEffectLayer->CreateGXObject(g_pObjManager->GetFile(SKILL_LEVELUP), pUser == g_pMainPlayer, __CHR_EFFECT_NONE__);
	
	pEffectDesc->byTargetObjectType[0]	= OBJECT_TYPE_PLAYER; 
	pEffectDesc->dwTargetIndex[0]		= pUser->m_dwUserIndex; 
	
	GXSetPosition( pEffectDesc->hEffect.pHandle, pUser->currentPositionReadOnly(), FALSE );
	::SetAction( pEffectDesc->hEffect.pHandle, 1, 0, ACTION_ONCE );
	pEffectDesc->hEffect.pDesc->ObjectFunc	= EffectOnceAndNormal;

	if(pUser->m_dwUserIndex==g_pMainPlayer->m_dwUserIndex)
	{		

		CSkillWnd*		pSkillWnd		= CSkillWnd::GetInstance();
		CCharWnd*		pCharWnd		= CCharWnd::GetInstance();
		CUserInterface*	pUserInterface	= CUserInterface::GetInstance();
		
		if(pSkillWnd->GetActive()==TRUE)
		{
			pSkillWnd->SetSkillUpBtn();
			pSkillWnd->SetSkillButtonPos();
		}
		
			
		char szInfo[0xff] = {0,};
		// MSG_ID : 78 ;%s ���� ������ �߽��ϴ�. ���� ������ %d �Դϴ�.
		wsprintf(szInfo, g_Message[ETC_MESSAGE78].szMessage, g_pMainPlayer->m_szName, pLevelUP->dwLevel);		
		DisplayMessageAdd(szInfo, 0xFFFF2CFF);
	}

	// ó�� ������ 	
	if(pUser->m_dwUserIndex==g_pMainPlayer->m_dwUserIndex)
	{
		g_pMainPlayer->updateCurrentLevel(pLevelUP->dwLevel);
		if(g_pThisDungeon->IsVillage()==FALSE)
		{		
			if(pLevelUP->dwLevel==2)
			{
				CHelpWnd* pHelpWnd = CHelpWnd::GetInstance();

				pHelpWnd->m_byType			= 10;
				g_pMainPlayer->m_byHelp[1]	= 10;
				pHelpWnd->SetClear();
				pHelpWnd->SetActive();

				WSTC_HELP_INFO pHelpInfoPacket;
				pHelpInfoPacket.bHelpInfo[0] = g_pMainPlayer->m_byHelp[0];
				pHelpInfoPacket.bHelpInfo[1] = g_pMainPlayer->m_byHelp[1];
				pHelpInfoPacket.bHelpInfo[2] = g_pMainPlayer->m_byHelp[2];
				g_pNet->SendMsg((char*)&pHelpInfoPacket, pHelpInfoPacket.GetPacketSize(), SERVER_INDEX_WORLD);
			}
		}	
	}
}

void CmdAppearItem( char* pMsg, DWORD dwLen )
{
	DSTC_APPEAR_ITEM*	pAppearItem = (DSTC_APPEAR_ITEM*)pMsg;

	if(pAppearItem->Item.GetID() == 0)
		return;

	MAP_TILE*	pTile;
	ITEM*	pItem = new ITEM;
	BYTE	bAction = 1;
	DWORD dwResourceID = MOD_ITEM_BOTARI;
	if(!pItem)
		return;

	VECTOR2_TO_VECTOR3(pAppearItem->v2ItemPos, pItem->v3ItemPos)
	
	memcpy( &pItem->Item, &pAppearItem->Item, sizeof( CItem ) );

	pTile = g_pMap->GetTileBy3DPosition( pAppearItem->v2ItemPos.x, pAppearItem->v2ItemPos.y );

	pItem->hItem.pDesc				= AllocObjDesc();
	memset( pItem->hItem.pDesc, 0, sizeof( OBJECT_DESC ) );

	pItem->hItem.pDesc->bType		= OBJECT_TYPE_ITEM;
	pItem->hItem.pDesc->pInfo		= (void*)pItem;
	pItem->bSectionNum				= pTile->wAttr.uSection;
	pItem->hItem.pDesc->ObjectFunc	= NULL;
	
	// ���̵� ���� �� ������ �ٸ��� �ε��ؾ��Ѵ�.
	switch( ( pItem->Item.m_wItemID / ITEM_DISTRIBUTE ) )
	{
	case ITEM_MONEY_INDEX:
		pItem->hItem.pHandle = CreateHandleObject( FILE( MOD_ITEM_MONEY ), GXPlayerPROC, pItem->hItem.pDesc, 0 );
		_PlaySound(0, SOUND_TYPE_ITEM, SOUND_ITEM_THROW_MONEY, pItem->v3ItemPos, FALSE);
		break;

	default:
		{
			CBaseItem* lpItemTalbe = g_pItemTableHash_get()->GetData(pItem->Item.GetID());		
			
			if( NULL == lpItemTalbe )
				return;

			switch( lpItemTalbe->dwCode_ID)
			{
			case ITEM_KIND_WEAPON:
				{
					//__ITEM_DROPBOX_ENABLE ī��
					switch(lpItemTalbe->BaseItem_Weapon.byDropBox)
					{
					case 1:
						// �ɼ� ���� ������ 
						if( pItem->Item.GetOptionCount() > 0 )
							dwResourceID = MOD_ITEM_MAGICBOTARI;
						break;
					case 2:
						dwResourceID = MOD_ITEM_SETBOTARI;
						break;
					case 3:
						dwResourceID = MOD_ITEM_EXTERAMBOTARI;
						break;
					}

					if (pAppearItem->bType == 1)
					{
						// ���� ������ ���̴�.  �ø��Ǿ��ٰ� ���°� �ƴ�..�Ѥ�;
						if (pAppearItem->bCreateByType == OBJECT_TYPE_MONSTER)	
						{
							// ���Ͱ� �������� ��������.
							_PlaySound(0, SOUND_TYPE_ITEM, SOUND_ITEM_THROW_MONSTERBYMAGICITEM, g_v3InterfaceSoundPos, FALSE);
						}
						else
						{
							_PlaySound(0, SOUND_TYPE_ITEM, SOUND_ITEM_THROW_SETITEM, pItem->v3ItemPos, FALSE);
						}
					}					
				}
				break;

			case ITEM_KIND_AROMR:
				{
					//__ITEM_DROPBOX_ENABLE ī��
					switch(lpItemTalbe->BaseItem_Armor.byDropBox)
					{
					case 1:
						// �ɼ� ���� ������ 
						if( pItem->Item.GetOptionCount() > 0 )
							dwResourceID = MOD_ITEM_MAGICBOTARI;
						break;
					case 2:
						dwResourceID = MOD_ITEM_SETBOTARI;
						break;
					case 3:
						dwResourceID = MOD_ITEM_EXTERAMBOTARI;
						break;
					}
					
					if (pAppearItem->bType == 1)
					{
						// ���� ������ ���̴�.  �ø��Ǿ��ٰ� ���°� �ƴ�..�Ѥ�;
						if (pAppearItem->bCreateByType == OBJECT_TYPE_MONSTER)	
						{
							// ���Ͱ� �������� ��������.
							_PlaySound(0, SOUND_TYPE_ITEM, SOUND_ITEM_THROW_MONSTERBYMAGICITEM, g_v3InterfaceSoundPos, FALSE);
						}
						else
						{
							_PlaySound(0, SOUND_TYPE_ITEM, SOUND_ITEM_THROW_SETITEM, pItem->v3ItemPos, FALSE);
						}
					}					
				}
				break;

			case ITEM_KIND_SUPPLIES:
				{
					dwResourceID = MOD_ITEM_POTION;
					_PlaySound(0, SOUND_TYPE_ITEM, SOUND_ITEM_THROW_POTION, pItem->v3ItemPos, FALSE);
				}
				break;

			case ITEM_KIND_SPECIAL:
				{					
					switch(pItem->Item.m_wItemID)
					{
					case 6503:
						dwResourceID = MOD_ITEM_MONEY_BOXC;
						break;
					case 6504:
						dwResourceID = MOD_ITEM_MONEY_BOXB;
						break;
					case 6505:
						dwResourceID = MOD_ITEM_MONEY_BOXA;
						break;
					case 6601:
						dwResourceID = MOD_ITEM_EVENT;
					}

					if ( pItem->Item.m_wItemID >= 6503 && pItem->Item.m_wItemID <= 6505)
					{ 
						// ���� ����
						bAction = BYTE(1 + GetRandom(6));
					}

					_PlaySound(0, SOUND_TYPE_ITEM, SOUND_ITEM_THROW_JEWEL_BOX, pItem->v3ItemPos, FALSE);
					
				}
				break;
			}

			pItem->hItem.pHandle = CreateHandleObject( GetFile(g_pObjManager->GetFile(dwResourceID), DATA_TYPE_ITEM), GXPlayerPROC, pItem->hItem.pDesc, 0 );
		}	
		break;
	}

	g_pExecutive->DisableSendShadow( pItem->hItem.pHandle );
	GXSetPosition( pItem->hItem.pHandle, &pItem->v3ItemPos, FALSE );
	
	if( pAppearItem->bType == 1 )		
	{
		// �������� ���� ������ ���̴�.
		if (dwResourceID == MOD_ITEM_EXTERAMBOTARI || dwResourceID == MOD_ITEM_SETBOTARI)
			SetActionNext( pItem->hItem.pHandle, bAction, 2, ACTION_LOOP );
		else
			SetAction( pItem->hItem.pHandle, bAction, 1, ACTION_ONCE );

		pItem->hItem.pDesc->bSkip = FALSE;
	}
	else if( pAppearItem->bType == 0 )	
	{
		// ������ �ִ� �������̴�.
		if (dwResourceID == MOD_ITEM_EXTERAMBOTARI || dwResourceID == MOD_ITEM_SETBOTARI)
			SetAction( pItem->hItem.pHandle, 2, 1, ACTION_LOOP);
		else
			SetAction( pItem->hItem.pHandle, 1, 20, ACTION_PAUSE );		
	}
	
	LOGOUT( "AppearItem CreateItem Type %d", pAppearItem->bType );
	
	// ������ �ؽ��� �����Ѵ�.
	DWORD asDWORD = pItem->Item.m_Serial.i64Serial;
	g_pItemHash->Insert( pItem, pItem->Item.m_Serial.i64Serial );	
}

void CmdDisappearItem( char* pMsg, DWORD dwLen )
{
	DSTC_DISAPPEAR_ITEM* pDisAppearItem = (DSTC_DISAPPEAR_ITEM*)pMsg;

	ITEM* pItem = g_pItemHash->GetData( pDisAppearItem->i64ItemID );
	if( !pItem ) return;

	RemoveItem( pItem );
}

void CmdDungeonErrorCode( char* pMsg, DWORD dwLen )
{
	DSTC_DUNGEON_ERROR_CODE* pError = ( DSTC_DUNGEON_ERROR_CODE* )pMsg;
	
	switch( pError->dwErrorCode )
	{
	case 1:		// ���ǵ� �� ���� �Ǵ� .....
		{		
			CNoticeWnd* pNoticeWnd = CNoticeWnd::GetInstance();
			pNoticeWnd->SetActive(TRUE);
		}
		break;

	case 2:		// ���ο� ���ǵ������� �Ǵ�...
		{
#if IS_CHINA_LOCALIZING()
			MessageBox(NULL, _T("�ٶ��쳣,ǿ���˳���Ϸ!"), _T("�ٶ��쳣!"), MB_OK|MB_ICONWARNING);
#else
#ifndef DEVELOP_MODE			
			MessageBox(NULL, _T("DON'T USE SPEEDHACK!! ErrCode: 0"), _T("SPEEDHACK!"), MB_OK|MB_ICONWARNING);
#endif
#endif		

#ifndef DEVELOP_MODE
			g_bIsRunning = FALSE;
#endif
		}
		break;
	}
}


void CmdDungeonMessage( char* pMsg, DWORD dwLen )
{
	DSTC_DUNGEON_MESSAGE* pPacket = (DSTC_DUNGEON_MESSAGE*)pMsg;

	// ���⼭ Ÿ�Կ� ���� ����ó���� �ھ��ּŵ� �˴ϴ�.

	DisplayMessageAdd(pPacket->szMessage, pPacket->dwMessageColor);
}

void CmdDungeonGuildWarDieMsg( char* pMsg, DWORD dwLen )
{
	DSTC_GUILDWAR_DIE_MSG *pPacket = (DSTC_GUILDWAR_DIE_MSG*)pMsg;
	char szTmp[255] = {0,};
	wsprintf(szTmp, g_Message[ETC_MESSAGE1385].szMessage, pPacket->szName );//"'%s'���� ���������ϴ�."
	DisplayMessageAdd(szTmp, TEXT_COLOR_YELLOW);
}

void CmdDungeonJoinFailedForDungeon( char* pMsg, DWORD dwLen )
{
	DSTC_DUNGEON_JOIN_FAILED *pPacket = (DSTC_DUNGEON_JOIN_FAILED*)pMsg;

	_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_Camera.v3CameraPos, FALSE);
	MoveToUserPrevPos();
	
	g_pMainPlayer->SetMotion(MOTION_TYPE_VILLAGESTAND, 0, ACTION_LOOP);
	g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL);

	CErrorWnd* pErrorWnd = CErrorWnd::GetInstance();
	switch(pPacket->bErrorCode)
	{
	case DUNGEON_JOIN_FAIL_DUNGEON_INFO_IS_NOT_FOUND:
		{
			pErrorWnd->SetError(g_Message[ETC_MESSAGE686].szMessage, //"�������� �α��ο� �����߽��ϴ�.", 
				g_Message[ETC_MESSAGE687].szMessage, //"������ �ش� ���������� �����ϴ�.", 
				g_Message[ETC_MESSAGE483].szMessage, //"����� �����ϼ���!", 
				3);
		}
		break;

	case DUNGEON_JOIN_FAIL_USER_OVERFLOW:
		{
			pErrorWnd->SetError(g_Message[ETC_MESSAGE686].szMessage, //"�������� �α��ο� �����߽��ϴ�.", 
				g_Message[ETC_MESSAGE688].szMessage, //"�� ������ �ִ��������� �ʰ��߽��ϴ�.", 
				g_Message[ETC_MESSAGE483].szMessage, //"����� �����ϼ���!", 
				3);
		}
		break;

	case DUNGEON_JOIN_FAIL_UNAUTHORIZED_USER:
		{
			pErrorWnd->SetError(g_Message[ETC_MESSAGE686].szMessage, //"�������� �α��ο� �����߽��ϴ�.", 
				g_Message[ETC_MESSAGE689].szMessage, //"���������� �����߽��ϴ�.", 
				g_Message[ETC_MESSAGE600].szMessage,//"��ڿ��� �����ϼ���!", 
				3);
		}
		break;

	case DUNGEON_JOIN_FAIL_INVALID_CLASS_OR_HEAD:
		{
			pErrorWnd->SetError(g_Message[ETC_MESSAGE686].szMessage, //"�������� �α��ο� �����߽��ϴ�.", 
				g_Message[ETC_MESSAGE690].szMessage, //"ĳ���� ������ �ùٸ��� �ʽ��ϴ�.", 
				g_Message[ETC_MESSAGE600].szMessage,//"��ڿ��� �����ϼ���!", 
				3);
		}
		break;
	case DUNGEON_JOIN_FAIL_NOT_MINMAX_LEVEL:
		{
			pErrorWnd->SetError(g_Message[ETC_MESSAGE1244].szMessage, //"������ ���� �ʾ� �����Ҽ� �����ϴ�."
				g_Message[ETC_MESSAGE1245].szMessage, // "������ Ȯ���ϼ���."
				"", 
				3);
		}break;
	case DUNGEON_JOIN_FAIL_INVALID_START_SPOT:
		{
			pErrorWnd->SetError(g_Message[ETC_MESSAGE1246].szMessage, // "�� ��ġ�� �����ϴ�."
				g_Message[ETC_MESSAGE1247].szMessage,//"�ٽ� �õ��ϼ���."
				g_Message[ETC_MESSAGE600].szMessage,//"��ڿ��� �����ϼ���!", 
				3);
		}break;
	case DUNGEON_JOIN_FAIL_NOT_ENOUGH_ENTER_KARZ:
		{
			pErrorWnd->SetError(g_Message[ETC_MESSAGE699].szMessage,
				g_Message[ETC_MESSAGE1247].szMessage, // "�ٽ� �õ��ϼ���."
				g_Message[ETC_MESSAGE600].szMessage,//"��ڿ��� �����ϼ���!", 
				3);
		}break;
	}

	g_pNet->DisconnectServer( SERVER_INDEX_ZONE );
	pErrorWnd->SetActive(TRUE);
	pErrorWnd->m_byType		= 0;
}


