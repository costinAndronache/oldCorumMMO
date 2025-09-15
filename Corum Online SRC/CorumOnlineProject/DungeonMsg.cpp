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
// Icon을 로드하고 삭제하는 함수.								//
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
	case 1:		// 돈이 들어왔을 경우랑, Weapon, Armor 아이템이 들어왔을 경우(바로 칸으로)
		{
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_GETITEM, g_v3InterfaceSoundPos, FALSE);

			CGameMenuWnd* pGameMenuWnd = CGameMenuWnd::GetInstance();
				
			if((pPacket->Item.m_wItemID/ITEM_DISTRIBUTE)==ITEM_MONEY_INDEX)
			{				
				g_pMainPlayer->m_dwMoney = pPacket->dwMoney;

				// MSG_ID : 79 ; %d Karz를 얻었습니다.
				wsprintf(szInfo, g_Message[ETC_MESSAGE79].szMessage, pPacket->Item.m_Item_Money.dwMoney);	
				DisplayMessageAdd(szInfo, 0xFFFF2CFF, pGameMenuWnd->m_bSystemMsgFlag);

				if(pPacket->bZipCode==1)
				{
					// MSG_ID : 80 ; 더이상 Karz를 습득할 수 없습니다.
					lstrcpy(szInfo, g_Message[ETC_MESSAGE80].szMessage);	
					DisplayMessageAdd(szInfo, 0xFFFFC309, pGameMenuWnd->m_bSystemMsgFlag);
				}
			}
			else
			{
				if(pPacket->bZipCode==255)
				{
					//"같은 번호를 가지고 있는 아이템이 존재하여 복사방지 정책의 일환으로
					//같은 번호의 아이템은제거하였음을 알려드립니다."
					DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
				}
				else
				{
					memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));

					lpItemTable = g_pItemTableHash_get()->GetData(pPacket->Item.GetID());

					if(lpItemTable)
					{
						// MSG_ID : 81 ; %s 아이템을 획득하였습니다.
						wsprintf(szInfo, g_Message[ETC_MESSAGE81].szMessage, lpItemTable->szItemName_Kor);	
						DisplayMessageAdd(szInfo, 0xFFFF2CFF, pGameMenuWnd->m_bSystemMsgFlag);
					}
				}				
			}
		}
		break;
	case 2:		// 물약 아이템이 들어왔을 경우(바로 칸으로)
		{
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_GETITEM, g_v3InterfaceSoundPos, FALSE);

		//	int	nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;

			if(pPacket->Item.GetType()!=ITEM_SMALL)			
				return;

			if(pPacket->bZipCode==255)
			{
				//"같은 번호를 가지고 있는 아이템이 존재하여 복사방지 정책의 일환으로
				// 같은 번호의 아이템은제거하였음을 알려드립니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}
			else
			{
				Insert_SmallInvItem(&pPacket->Item, pPacket->bZipCode, pPacket->Item.GetQuantity());

				lpItemTable = g_pItemTableHash_get()->GetData(pPacket->Item.GetID());

				if(lpItemTable)
				{
					// MSG_ID : 82 ; %s 아이템을 %d개 획득하였습니다.
					wsprintf(szInfo, g_Message[ETC_MESSAGE82].szMessage, lpItemTable->szItemName_Kor
						, pPacket->Item.GetQuantity());	
					DisplayMessageAdd(szInfo, 0xFFFF2CFF, pGameMenuWnd->m_bSystemMsgFlag);
				}
			}	
			
			pQuantityWnd->m_bReq = FALSE;
		}
		break;
	case 3:		// 작은 아이템이 마우스에서 벨트로 갔을 경우.
		{			
		//	int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;

			if(pPacket->Item.GetType()!=ITEM_SMALL)			
				return;
			
			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"같은 번호를 가지고 있는 아이템이 존재하여 복사방지 정책의 일환으로
				// 같은 번호의 아이템은제거하였음을 알려드립니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}
			else
			{
				Insert_SmallBeltItem(&pPacket->Item, pPacket->bZipCode, pPacket->Item.GetQuantity(), FALSE);
			
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));			
				
				// 물약 사운드 //
				PlayItemSound(pPacket->Item.m_wItemID);
			}
			
			pQuantityWnd->m_bReq = FALSE;
		}			
		break;
	case 4:		// 마우스에 바닥으로 소비아이템 이동.
		{
			PlayItemSound(g_pMainPlayer->m_MouseItem.m_wItemID);

			memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
			g_pMainPlayer->m_nItemSelect	= 0;
			pQuantityWnd->m_bReq			= FALSE;				
		}
		break;
	case 5:		// 바닥에서 마우스로(장착 아이템).
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
	case 6:		// 마우스에서 장착 아이템으로(장착 아이템).
		{	
			int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;
			
			if(nValue>__MAX_ITEM_LARGE_INDEX__)
				return;

			CQuantityWnd* pQuantityWnd = CQuantityWnd::GetInstance();

			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"같은 번호를 가지고 있는 아이템이 존재하여 복사방지 정책의 일환으로
				// 같은 번호의 아이템은제거하였음을 알려드립니다."
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
						// "아이템이 인벤토리로 이동되었습니다."	
						wsprintf(szInfo, g_Message[ETC_MESSAGE1209].szMessage); 
						DisplayMessageAdd(szInfo, 0xffffff00);
					}
				}
			}
			
			pQuantityWnd->m_bReq = FALSE;			
		}
		break;
	case 7:		// 장착 아이템에서 마우스로(장착 아이템).
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
				//"같은 번호를 가지고 있는 아이템이 존재하여 복사방지 정책의 일환으로
				// 같은 번호의 아이템은제거하였음을 알려드립니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}

			switch(pPacket->dwMoney)
			{
			
			case 1 :
				{
					memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));
					memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
					g_pMainPlayer->m_nItemSelect = 0;
					// "착용할수 없는 아이템입니다."
					DisplayMessageAdd(g_Message[ETC_MESSAGE510].szMessage,  0xffff0000); 
				}
				break;

			case 2 :
				{
					memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
					g_pMainPlayer->m_nItemSelect = 0;
					//"착용할수 없는 아이템입니다."
					DisplayMessageAdd(g_Message[ETC_MESSAGE510].szMessage,  0xffff0000); 
				}
				break;
			case 3:
				//2005.01.20 김영대
				//"착용할수 없는 아이템입니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE510].szMessage,  0xffff0000); 						
				break;
			case 4 :
				// MSG_ID : 53 ; 장착아이템 인벤토리가 가득찼습니다
				DisplayMessageAdd(g_Message[ETC_MESSAGE53].szMessage, 0xFFFFC309);	
				_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				break;
			default :
			{
				switch(nValue)
				{		
				case 0:	case 1:	case 2: case 3: case 4: case 5: case 6: case 7:		// 웨폰 //
					SetPickupItemWeaponR(pPacket);
					break;
				case 10:	// 헬멧 //
					SetPickupItemHelmet(pPacket);
					break;
				case 11:	// 갑옷 //
					SetPickupItemArmor(pPacket);
					break;
				case 12:	// 방패 //
					SetPickupItemShield(pPacket);
					break;				
				case 13:	// 벨트 //
					SetPickupItemBelt(pPacket);
					break;
				case 14:	// 장갑 //				
					SetPickupItemGlove(pPacket);
					break;
				case 15:	// 신발 //
					SetPickupItemBoots(pPacket);
					break;
				case 16:	// 링 //
					SetPickupItemRing(pPacket);
					break;
				case 17:	// 아뮬렛 //
					SetPickupItemAmule(pPacket);
					break;
				case 18:	// 가방 //
					SetPickupItemBag(pPacket);
					break;
				case 20:	// 비공정 //
					SetPickupItemRide(pPacket);
					break;
				}		
				
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				g_pMainPlayer->m_nItemSelect = 0;
			}
			}
		}
		break;	
	case 10:	// 마우스에서 바닥으로 이동할때(장착 아이템).
		{			
			PlayItemSound(g_pMainPlayer->m_MouseItem.m_wItemID);

			memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
			g_pMainPlayer->m_nItemSelect = 0;								
			pQuantityWnd->m_bReq = FALSE;
		}
		break;
	case 11:	// 아이템장착 슬롯에서 마우스로 이동.
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
	case 12:	// 바닥에서 마우스로 이동할때(소비아이템).
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

	case 13:	// 돈이 버릴 경우.
		{
// 넷클루 요청으로 Karz 땅에 떨어뜨지지 못하게 2005.02.05 김영대 
#if  IS_JAPAN_LOCALIZING()
			DisplayMessageAdd(g_Message[ETC_MESSAGE1372].szMessage, 0xffff0000);		
#else
			g_pMainPlayer->m_dwMoney = pPacket->dwMoney;			
#endif
		}
		break;
	case 14:	// 작은 아이템이 마우스에서 소비칸으로 갔을 경우.
		{			
		//	int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;
				
			PlayItemSound(pPacket->Item.m_wItemID);

			if(pPacket->Item.GetType()!=ITEM_SMALL)
				return;

			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"같은 번호를 가지고 있는 아이템이 존재하여 복사방지 정책의 일환으로
				// 같은 번호의 아이템은제거하였음을 알려드립니다."
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
	case 15:	// 소비 창에서 마우스로 이동할 때(작은 아이템).
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
	case 16:	// 벨트에서 마우스로(소비 아이템).
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
	case 18:	// 아이템 상점에서 아이템을 팔때 //
		{
		//	int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;
									
			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"같은 번호를 가지고 있는 아이템이 존재하여 복사방지 정책의 일환으로
				// 같은 번호의 아이템은제거하였음을 알려드립니다."
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

						// MSG_ID : 83 ; %u Karz를 획득했습니다.
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
							// MSG_ID : 84 ; %s 아이템을 더이상 팔수 없습니다.
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
	case 19:	// 소비 아이템을 살때.
		{						
		//	int nPackCount = pPacket->bEquipCode;
						
			if(pPacket->bZipCode2==0)
			{
				PlayItemSound(pPacket->Item.m_wItemID);
				
				if(pPacket->Item.GetQuantity()==0)
				{
					// "인벤토리에 공간이 부족합니다."
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
						// MSG_ID : 85 ; "%s 아이템을 %u개를 구매하셨습니다.
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
					// MSG_ID : 86 ; Kartz가 부족합니다. %s 아이템을 구매 할수없습니다.
					wsprintf(szInfo, g_Message[ETC_MESSAGE86].szMessage, lpItem->szItemName_Kor);	
					DisplayMessageAdd(szInfo, 0xFFFFC309);
				}				
			}
			g_pMainPlayer->m_nItemSelect = 0;
		}
		break;
	case 20:	// 소비 아이템사용.
		{
			memcpy(&g_pMainPlayer->m_pInv_Small[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));			
		}
		break;
	case 21:	// 소비 아이템사용.
		{
		g_pMainPlayer->setBeltItem(pPacket->Item, pPacket->bZipCode);
			//memcpy(&g_pMainPlayer->m_pBelt[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));			
		}
		break;
	case 22:	// 마우스에서 던전관리창으로
		{					
		}
		break;
	case 23:	// 던전관리창에서 마우스로.
		{		
		}
		break;
	case 24:		// 마우스에서 창고로(장비 아이템) //
		{
			int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;
										
			if(pPacket->Item.m_wItemID==0)
				return;
			if(nValue>__MAX_ITEM_LARGE_INDEX__)
				return;

			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"같은 번호를 가지고 있는 아이템이 존재하여 복사방지 정책의 일환으로
				// 같은 번호의 아이템은제거하였음을 알려드립니다."
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
	case 25:		// 마우스에서 창고로(소비 아이템) //
		{
		//	int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;				
		
			if(pPacket->Item.GetType()!=ITEM_SMALL)
				return;

			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"같은 번호를 가지고 있는 아이템이 존재하여 복사방지 정책의 일환으로
				// 같은 번호의 아이템은제거하였음을 알려드립니다."
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
	case 26:		// 창고에서 마우스로(장비 아이템) //
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
	case 27:		// 창고에서 마우스로(소비 아이템) //
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
	case 28:		// 마우스에서 나누기로 바닥에 떨어질 때 //
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
	case 30:		// 길드 생성 아이템 삭제 //
		{			// 김영대 2005.01.29
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
									// 아이템에서 있는 칸을 찾는다 //
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
									// 아이템에서 있는 칸을 찾는다 //
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
									// 아이템에서 있는 칸을 찾는다 //
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
			
			// 아이템 삭제 //
			memcpy(&g_pMainPlayer->m_pInv_Small[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));

			// 생성 창 //								
			pGuildExWnd->SetActive(TRUE);
			pGuildExWnd->m_byChk = 1;
			pGuildExWnd->m_byGuildType = pPacket->bZipCode2;			

			// 입력 모드 //
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
	case 32:		// 아이템에서 뱅크로 돈 이동.
	case 33:		// 뱅크에서 아이템으로 돈 이동.
		{
			g_pMainPlayer->m_dwMoney		= pPacket->dwMoney;
			g_pMainPlayer->m_dwBankMoney	= pPacket->dwId;

			if(g_pMainPlayer->m_dwBankMoney==__MAX_MONEY__)
			{
				// MSG_ID :	87 ; 더이상 들어갈수 없습니다.
				DisplayMessageAdd(g_Message[ETC_MESSAGE87].szMessage, 0xFFFFC309);	
				_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
			}				
			if(g_pMainPlayer->m_dwMoney==__MAX_MONEY__)
			{
				// MSG_ID :	87 ; 더이상 들어갈수 없습니다.
				DisplayMessageAdd(g_Message[ETC_MESSAGE87].szMessage, 0xFFFFC309);	
				_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
			}				
		}
		break;
	case 34:		// Weapon 아이템 팔 때.
		{
			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"같은 번호를 가지고 있는 아이템이 존재하여 복사방지 정책의 일환으로
				// 같은 번호의 아이템은제거하였음을 알려드립니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}
			else
			{
				if(pPacket->bEquipCode==0)
				{
					DWORD dwMoney = pPacket->dwMoney-g_pMainPlayer->m_dwMoney;
					
					g_pMainPlayer->m_dwMoney = pPacket->dwMoney;				
					// 메세지 //
					// MSG_ID : 83 ; %u Karz를 획득했습니다.
					wsprintf(szInfo, g_Message[ETC_MESSAGE83].szMessage, dwMoney);	
					DisplayMessageAdd(szInfo, 0xFFFF2CFF);
				}
				else if(pPacket->bEquipCode==1)
				{					
					memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));
					// 메세지 //				
					// MSG_ID : 88 ; 소지할수 있는 Karz 최대량을 가지고 있어서 아이템을 팔 수 없습니다.
					DisplayMessageAdd(g_Message[ETC_MESSAGE88].szMessage, 0xFFFFC309);	
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				}
				else
				{
					// 메세지 //
					// MSG_ID : 88 ; 소지할수 있는 Karz 최대량을 가지고 있어서 아이템을 팔 수 없습니다.
					DisplayMessageAdd(g_Message[ETC_MESSAGE88].szMessage, 0xFFFFC309);	
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				}

				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
			}
		}
		break;
	case 35:		// Weapon 아이템 살 때.
		{																	
			if(pPacket->bEquipCode==1)
			{					
				memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));
				g_pMainPlayer->m_dwMoney = pPacket->dwMoney;

//				LP_ITEM_TABLE lpItem = g_pItemTableHash_get()->GetData(pPacket->dwId);
				CBaseItem* lpItem = g_pItemTableHash_get()->GetData(pPacket->dwId);

				if(lpItem)
				{
					// MSG_ID : 89 ; %s 아이템을 구매하였습니다.			
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
					// MSG_ID : 90 ; 인벤토리에 공간이 부족합니다.
					DisplayMessageAdd(g_Message[ETC_MESSAGE90].szMessage, 0xFFFFC309);		
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				}
				else
				{
//					LP_ITEM_TABLE lpItem = g_pItemTableHash_get()->GetData(pPacket->dwId);
					CBaseItem* lpItem = g_pItemTableHash_get()->GetData(pPacket->dwId);

					if(lpItem)
					{						
						// MSG_ID : 86 ; Karz가 부족합니다. %s 아이템을 구매 할수없습니다.
						wsprintf(szInfo, g_Message[ETC_MESSAGE86].szMessage, lpItem->szItemName_Kor);	
						DisplayMessageAdd(szInfo, 0xFFFFC309);
						_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
					}
				}								
			}
		}
		break;
	case 84:
	case 36:		// Armor 아이템 팔 때.
		{
			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"같은 번호를 가지고 있는 아이템이 존재하여 복사방지 정책의 일환으로
				// 같은 번호의 아이템은제거하였음을 알려드립니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}
			else
			{
				if(pPacket->bEquipCode==0)
				{
					DWORD dwMoney = pPacket->dwMoney-g_pMainPlayer->m_dwMoney;
					
					g_pMainPlayer->m_dwMoney = pPacket->dwMoney;				
					// 메세지 //
					// MSG_ID : 83 ; %u Karz를 획득했습니다.
					wsprintf(szInfo, g_Message[ETC_MESSAGE83].szMessage, dwMoney);	
					DisplayMessageAdd(szInfo, 0xFFFF2CFF);
				}
				else if(pPacket->bEquipCode==1)
				{					
					memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));
					// 메세지 //				
					// MSG_ID : 88 ; 소지할수 있는 Karz 최대량을 가지고 있어서 아이템을 팔 수 없습니다.
					DisplayMessageAdd(g_Message[ETC_MESSAGE88].szMessage, 0xFFFFC309);	
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				}
				else
				{
					// 메세지 //
					// MSG_ID : 88 ; 소지할수 있는 Karz 최대량을 가지고 있어서 아이템을 팔 수 없습니다.
					DisplayMessageAdd(g_Message[ETC_MESSAGE88].szMessage, 0xFFFFC309);	
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				}

				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
			}
		}
		break;
	case 85:	
	case 37:		// Armor 아이템 살 때.
		{
			if(pPacket->bEquipCode==1)
			{					
				memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));
				g_pMainPlayer->m_dwMoney = pPacket->dwMoney;

//				LP_ITEM_TABLE lpItem = g_pItemTableHash_get()->GetData(pPacket->dwId);
				CBaseItem* lpItem = g_pItemTableHash_get()->GetData(pPacket->dwId);

				if(lpItem)
				{
					// MSG_ID : 89 ; %s 아이템을 구매하였습니다.
					wsprintf(szInfo, g_Message[ETC_MESSAGE89].szMessage, lpItem->szItemName_Kor);		
					DisplayMessageAdd(szInfo, 0xFFFF2CFF);
				}				
			}
			else
			{
				if(pPacket->bEquipCode==2)
				{
					// MSG_ID : 90 ;  인벤토리에 공간이 부족합니다.
					DisplayMessageAdd(g_Message[ETC_MESSAGE90].szMessage, 0xFFFFC309);		
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				}
				else
				{
//					LP_ITEM_TABLE lpItem = g_pItemTableHash_get()->GetData(pPacket->dwId);
					CBaseItem* lpItem = g_pItemTableHash_get()->GetData(pPacket->dwId);

					if(lpItem)
					{						
						// MSG_ID : 86 ; Kartz가 부족합니다. %s 아이템을 구매 할수없습니다.
						wsprintf(szInfo, g_Message[ETC_MESSAGE86].szMessage, lpItem->szItemName_Kor);	
						DisplayMessageAdd(szInfo, 0xFFFFC309);
						_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
					}
				}								
			}
		}
		break;
	case 38:		// 마우스에서 Guardian 창으로 Guardian 아이템이 갈 때.
		{
			int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;
			
			if(nValue!=ITEM_GUARDIAN_INDEX)
				return;

			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"같은 번호를 가지고 있는 아이템이 존재하여 복사방지 정책의 일환으로
				// 같은 번호의 아이템은제거하였음을 알려드립니다."
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
	case 39:	// Guardian 창에서 마우스로 Guardian 아이템이 갈 때.
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
	case 40:		// 마우스에서 Guardian 인터페이스로 소비 아이템이 갈 때.
		{
			int		nValue		= pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;				
			BYTE	byPacketCnt	= 0;
 
			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));

				//"같은 번호를 가지고 있는 아이템이 존재하여 복사방지 정책의 일환으로
				// 같은 번호의 아이템은제거하였음을 알려드립니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}
			else
			{
				if(nValue==ITEM_SUPPLIES_INDEX || (nValue >= ITEM_CONSUMABLEST_INDEX && nValue <= ITEM_CONSUMABLEEND_INDEX))
				{							
					// 아이템 나누기 //
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
	case 41:		// Guardian 인터페이스에서 마우스로 소비 아이템이 갈 때.
		{
			int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;								

			if(nValue==ITEM_SUPPLIES_INDEX || (nValue >= ITEM_CONSUMABLEST_INDEX && nValue <= ITEM_CONSUMABLEEND_INDEX) )			
				memcpy(&g_pMainPlayer->m_MouseItem, &pPacket->Item, sizeof(CItem));
						
			if(g_Mouse.bLDown)
				g_pMainPlayer->m_nItemSelect = 1;

			memset(&g_pMainPlayer->m_GuardianItem, 0, sizeof(CItem));				
		}
		break;
	case 42:		// Guardian 아이템을 마우스에서 던젼 관리창으로.
		{
			int nVal = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;

			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"같은 번호를 가지고 있는 아이템이 존재하여 복사방지 정책의 일환으로
				// 같은 번호의 아이템은제거하였음을 알려드립니다."
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
	case 43:		// Guardian 아이템을 던젼 관리창에서 마우스로 이동.
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
	case 44:		// Guardian에게 아이템 사용.
		{	
			// 보약, 스킬북 적용 : 최덕석 2005.1.17
			// 도무지 Zipcode, zipcode2, money를 가지고
			// 어쩌자는거야!! -최덕석
			char szTemp[0xff] = {0,};	// 메세지 출력용 버퍼

			// HP 물약
			if (pPacket->bZipCode == 0)
			{
				memcpy(&g_pMainPlayer->m_GuardianItem, &pPacket->Item, sizeof(CItem));

				CMonster *pGuardian = g_pMainPlayer->m_pGuardian[0];
				if(pGuardian)
				{
					pGuardian->m_dwHP = pPacket->dwMoney;
				}
			}
			
			// 부화 단축
			else if(pPacket->bZipCode == 1)
			{
				memcpy(&g_pMainPlayer->m_GuardianItem, &pPacket->Item, sizeof(CItem));
			}

			// 생명 연장
			else if(pPacket->bZipCode == 2)
			{
				memcpy(&g_pMainPlayer->m_GuardianItem, &pPacket->Item, sizeof(CItem));

				// "%s 의 수명이 %d분 증가하였습니다."
				wsprintf(szTemp, g_Message[ETC_MESSAGE1277].szMessage, g_pMainPlayer->m_pGuardian[0]->m_szName, pPacket->dwMoney);
				DisplayMessageAdd(szTemp, 0xFFFFC309);

				// 보약 이펙트를 뿌린다.
				AppliedSkill*	pEffectDesc =  g_pEffectLayer->CreateGXObject(g_pObjManager->GetFile(SKILL_REMEDY), TRUE, __CHR_EFFECT_NONE__);
				pEffectDesc->byTargetObjectType[0]	= OBJECT_TYPE_MONSTER; 
				pEffectDesc->dwTargetIndex[0]		=g_pMainPlayer->m_pGuardian[0]->m_dwMonsterIndex; 
				GXSetPosition( pEffectDesc->hEffect.pHandle, &g_pMainPlayer->m_pGuardian[0]->m_v3CurPos, FALSE );
				::SetAction( pEffectDesc->hEffect.pHandle, 1, 0, ACTION_ONCE );
				pEffectDesc->hEffect.pDesc->ObjectFunc	= EffectOnceAndNormal;
			}
			
			// 스킬북
			else if(pPacket->bZipCode == 3)
			{
				if(pPacket->bZipCode2 == 0)	// 실패, 실패 요인을 출력 : 최덕석 2005.1.17
				{
					char *pFailMsg = NULL;
					switch(pPacket->dwMoney)
					{
					case 1:
						// "스킬을 배울 수 있는 공간이 부족 합니다."
						pFailMsg = g_Message[ETC_MESSAGE1276].szMessage;
						break;
					case 2:
						// "이미 배운 스킬 입니다."
						pFailMsg = g_Message[ETC_MESSAGE1278].szMessage;
						break;
					case 3:
						// "스킬을 배울 수 없습니다. 하위 레벨의 스킬을 먼저 배우셔야 합니다."
						pFailMsg = g_Message[ETC_MESSAGE1275].szMessage;
						break;
					case 4:
						// "가디언의 레벨이 낮아 스킬북을 익힐 수 없습니다."
						pFailMsg = g_Message[ETC_MESSAGE1411].szMessage;
						break;
					default:
						break;
					}
					if(pFailMsg)
						DisplayMessageAdd(pFailMsg, 0xFFFF0000);
				}
				else if(pPacket->bZipCode2 > 0) // 성공
				{
					memcpy(&g_pMainPlayer->m_GuardianItem, &pPacket->Item, sizeof(CItem));

					// "%s 이 %d Lv의 %s 스킬을 배웠습니다."
					wsprintf(szTemp, g_Message[ETC_MESSAGE1274].szMessage,
						g_pMainPlayer->m_pGuardian[0]->m_szName,
						pPacket->bZipCode2,
						g_pEffectLayer->m_Effect[pPacket->dwMoney].szName);
					DisplayMessageAdd(szTemp, 0xFFFFC309);

					// 스킬을 배웠다는 이펙트를 뿌린다.
					AppliedSkill*	pEffectDesc =  g_pEffectLayer->CreateGXObject(g_pObjManager->GetFile(SKILL_PRAY), TRUE, __CHR_EFFECT_NONE__);
					pEffectDesc->byTargetObjectType[0]	= OBJECT_TYPE_MONSTER; 
					pEffectDesc->dwTargetIndex[0]		=g_pMainPlayer->m_pGuardian[0]->m_dwMonsterIndex; 
					GXSetPosition( pEffectDesc->hEffect.pHandle, &g_pMainPlayer->m_pGuardian[0]->m_v3CurPos, FALSE );
					::SetAction( pEffectDesc->hEffect.pHandle, 1, 0, ACTION_ONCE );
					pEffectDesc->hEffect.pDesc->ObjectFunc	= EffectOnceAndNormal;
				}
			}

			// 스탯 포멧터
			else if(pPacket->bZipCode == 4)
			{
					memcpy(&g_pMainPlayer->m_GuardianItem, &pPacket->Item, sizeof(CItem));

					// "%s 의 스탯을 초기화했습니다."
					wsprintf(szTemp, "%s의 스탯을 초기화했습니다.", g_pMainPlayer->m_pGuardian[0]->m_szName);
					DisplayMessageAdd(szTemp, 0xFFFFC309);

					// 초기화 이펙트 뿌리고
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
	case 45:		// Guardian이 사라지면 물약 아이템 이동.
		{
			int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;

			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"같은 번호를 가지고 있는 아이템이 존재하여 복사방지 정책의 일환으로
				// 같은 번호의 아이템은제거하였음을 알려드립니다."
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
	case 46:		// 큰 아이템이 마우스에서 트레이드 창으로 이동 //
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
	case 48:		// 큰 아이템이 트레이드 창에서 마우스로 이동.
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
	case 49:		// 작은 아이템이 트레이드 창에서 마우스로 이동.
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
									
			// "좌표가 저장되었습니다."
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

				// 마을 귀환 스크롤 추가 2005.02.14 김영대 
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
	case 56:		// 파티원 퍼스널 포탈 사용.
		{
			memcpy(&g_pMainPlayer->m_pInv_Small[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));

			memset(&g_pMainPlayer->m_GuardianItem, 0, sizeof(CItem));
		}
		break;
	case 57:		// 마우스에서 노점상으로 장착 아이템이동.
		{
			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"같은 번호를 가지고 있는 아이템이 존재하여 복사방지 정책의 일환으로
				// 같은 번호의 아이템은제거하였음을 알려드립니다."
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
	case 58:		// 노점상에서 마우스로 장착 아이템이동.
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
	case 59:		// 마우스에서 노점상으로 소비 아이템 이동.
		{				
		//	int nValue = pPacket->Item.m_wItemID/ITEM_DISTRIBUTE;
				
			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"같은 번호를 가지고 있는 아이템이 존재하여 복사방지 정책의 일환으로
				// 같은 번호의 아이템은제거하였음을 알려드립니다."
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
	case 60:		// 노점상에서 마우스로 소비 아이템이동.
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
	case 61:		// 파티 마법진 사용.
		{		
			memcpy(&g_pMainPlayer->m_pInv_Small[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));		
		}
		break;
	case 62:		// 파티 전체 포탈 사용.
		{
			memcpy(&g_pMainPlayer->m_pInv_Small[pPacket->bZipCode], &pPacket->Item, sizeof(CItem));	
		}
		break;
	case 63:// 마우스에서 던전관리 창으로 마법진 이동
		{
			int nValue = pPacket->Item.GetID()/ITEM_DISTRIBUTE;

			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"같은 번호를 가지고 있는 아이템이 존재하여 복사방지 정책의 일환으로
				// 같은 번호의 아이템은제거하였음을 알려드립니다."
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
	case 64://던전관리창에서 마법진을 마우스로
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
				//"같은 번호를 가지고 있는 아이템이 존재하여 복사방지 정책의 일환으로
				// 같은 번호의 아이템은제거하였음을 알려드립니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}
			else if(pPacket->bZipCode==254)
			{
				// 조또 씨발놈 
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
				//"같은 번호를 가지고 있는 아이템이 존재하여 복사방지 정책의 일환으로
				// 같은 번호의 아이템은제거하였음을 알려드립니다."
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
				//"같은 번호를 가지고 있는 아이템이 존재하여 복사방지 정책의 일환으로
				// 같은 번호의 아이템은제거하였음을 알려드립니다."
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
					// MSG_ID : 82 ; %s 아이템을 %d개 획득하였습니다.
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
				// "동일한 번호가 존재하는 복사아이템일 가능성이 있어 구매하실 수가 없습니다."
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
				// "동일한 번호가 존재하는 복사아이템일 가능성이 있어 구매하실 수가 없습니다."
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

	case 78:	//선물상자 아이템 지급(태극기) 
		{	
			DWORD dwMsgId = 0;

			if(pPacket->bEqiupCode2 > 0)		//꽝또는 Error
			{
				switch(pPacket->bEqiupCode2)
				{
				case 0xff:	//꽝!
					dwMsgId = ETC_MESSAGE900;break; // "꽝! 다음기회에..."
				case 254:	//쿼리에러!
					dwMsgId = ETC_MESSAGE901;break; // "이벤트 오류입니다. (ErrorCode:254)"
				case 253:	//인벤에 빈칸 없음!
					dwMsgId = ETC_MESSAGE90;break; // "인벤토리에 공간이 부족합니다."
				case 252:	//선물상자 풀기 실패!
					dwMsgId = ETC_MESSAGE903;break; // "이벤트 오류입니다. (ErrorCode:252)"
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
					// "%s 아이템을 %u개 얻었습니다"
#ifdef TAIWAN_LOCALIZING // 대만의 경우 주어동사의 위치가 우리나라와 달라서 위치가 변경 된다.
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
		{  // 아이템 생성 창에서 마우스로 아이템 이동 2005.01.25 김영대
			int nValue = pPacket->Item.GetID()/ITEM_DISTRIBUTE;

			CItemMakingWnd* pItemMakingWnd = CItemMakingWnd::GetInstance();

			if(pPacket->bZipCode==255)
			{
				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
				//"같은 번호를 가지고 있는 아이템이 존재하여 복사방지 정책의 일환으로
				// 같은 번호의 아이템은제거하였음을 알려드립니다."
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
	case 82:// 가던 알 갱신될때 
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
				//"같은 번호를 가지고 있는 아이템이 존재하여 복사방지 정책의 일환으로
				// 같은 번호의 아이템은제거하였음을 알려드립니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE992].szMessage, 0xFFFF0000);
			}
			else
			{
				if(pPacket->bEquipCode==0)
				{
					DWORD dwMoney = pPacket->dwMoney-g_pMainPlayer->m_dwMoney;
					
					g_pMainPlayer->m_dwMoney = pPacket->dwMoney;				
					// 메세지 //
					// MSG_ID : 83 ; %u Karz를 획득했습니다.
					wsprintf(szInfo, g_Message[ETC_MESSAGE83].szMessage, dwMoney);	
					DisplayMessageAdd(szInfo, 0xFFFF2CFF);
				}
				else if(pPacket->bEquipCode==1)
				{					
					memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));
					// 메세지 //				
					// MSG_ID : 88 ; 소지할수 있는 Karz 최대량을 가지고 있어서 아이템을 팔 수 없습니다.
					DisplayMessageAdd(g_Message[ETC_MESSAGE88].szMessage, 0xFFFFC309);	
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				}
				else
				{
					// 메세지 
					// MSG_ID : 88 ; 소지할수 있는 Karz 최대량을 가지고 있어서 아이템을 팔 수 없습니다.
					DisplayMessageAdd(g_Message[ETC_MESSAGE88].szMessage, 0xFFFFC309);	
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				}

				memset(&g_pMainPlayer->m_MouseItem, 0, sizeof(CItem));
			}
		}
		break;
	// 아이템 보험  	
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

	// 레벨업 이펙트를 뿌린다.
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
		// MSG_ID : 78 ;%s 님이 레벨업 했습니다. 현재 레벨은 %d 입니다.
		wsprintf(szInfo, g_Message[ETC_MESSAGE78].szMessage, g_pMainPlayer->m_szName, pLevelUP->dwLevel);		
		DisplayMessageAdd(szInfo, 0xFFFF2CFF);
	}

	// 처음 레벱업 	
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
	
	// 아이디에 따라 모델 파일을 다르게 로드해야한다.
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
					//__ITEM_DROPBOX_ENABLE 카피
					switch(lpItemTalbe->BaseItem_Weapon.byDropBox)
					{
					case 1:
						// 옵션 붙은 아이템 
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
						// 새로 생성된 것이다.  컬링되었다가 나온게 아님..ㅡㅡ;
						if (pAppearItem->bCreateByType == OBJECT_TYPE_MONSTER)	
						{
							// 몬스터가 아이템을 떨구었다.
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
					//__ITEM_DROPBOX_ENABLE 카피
					switch(lpItemTalbe->BaseItem_Armor.byDropBox)
					{
					case 1:
						// 옵션 붙은 아이템 
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
						// 새로 생성된 것이다.  컬링되었다가 나온게 아님..ㅡㅡ;
						if (pAppearItem->bCreateByType == OBJECT_TYPE_MONSTER)	
						{
							// 몬스터가 아이템을 떨구었다.
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
						// 보석 상자
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
		// 아이템이 새로 생성된 것이다.
		if (dwResourceID == MOD_ITEM_EXTERAMBOTARI || dwResourceID == MOD_ITEM_SETBOTARI)
			SetActionNext( pItem->hItem.pHandle, bAction, 2, ACTION_LOOP );
		else
			SetAction( pItem->hItem.pHandle, bAction, 1, ACTION_ONCE );

		pItem->hItem.pDesc->bSkip = FALSE;
	}
	else if( pAppearItem->bType == 0 )	
	{
		// 기존에 있던 아이템이다.
		if (dwResourceID == MOD_ITEM_EXTERAMBOTARI || dwResourceID == MOD_ITEM_SETBOTARI)
			SetAction( pItem->hItem.pHandle, 2, 1, ACTION_LOOP);
		else
			SetAction( pItem->hItem.pHandle, 1, 20, ACTION_PAUSE );		
	}
	
	LOGOUT( "AppearItem CreateItem Type %d", pAppearItem->bType );
	
	// 아이템 해쉬에 삽입한다.
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
	case 1:		// 스피드 핵 으로 판단 .....
		{		
			CNoticeWnd* pNoticeWnd = CNoticeWnd::GetInstance();
			pNoticeWnd->SetActive(TRUE);
		}
		break;

	case 2:		// 새로운 스피드핵으로 판단...
		{
#if IS_CHINA_LOCALIZING()
			MessageBox(NULL, _T("醵똑嫩끽,퓻齡藁놔踏狗!"), _T("醵똑嫩끽!"), MB_OK|MB_ICONWARNING);
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

	// 여기서 타입에 대한 예외처리를 박아주셔도 됩니다.

	DisplayMessageAdd(pPacket->szMessage, pPacket->dwMessageColor);
}

void CmdDungeonGuildWarDieMsg( char* pMsg, DWORD dwLen )
{
	DSTC_GUILDWAR_DIE_MSG *pPacket = (DSTC_GUILDWAR_DIE_MSG*)pMsg;
	char szTmp[255] = {0,};
	wsprintf(szTmp, g_Message[ETC_MESSAGE1385].szMessage, pPacket->szName );//"'%s'님이 쓰러졌습니다."
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
			pErrorWnd->SetError(g_Message[ETC_MESSAGE686].szMessage, //"던전서버 로그인에 실패했습니다.", 
				g_Message[ETC_MESSAGE687].szMessage, //"서버에 해당 던전정보가 없습니다.", 
				g_Message[ETC_MESSAGE483].szMessage, //"운영팀에 문의하세요!", 
				3);
		}
		break;

	case DUNGEON_JOIN_FAIL_USER_OVERFLOW:
		{
			pErrorWnd->SetError(g_Message[ETC_MESSAGE686].szMessage, //"던전서버 로그인에 실패했습니다.", 
				g_Message[ETC_MESSAGE688].szMessage, //"이 던전의 최대유저수를 초과했습니다.", 
				g_Message[ETC_MESSAGE483].szMessage, //"운영팀에 문의하세요!", 
				3);
		}
		break;

	case DUNGEON_JOIN_FAIL_UNAUTHORIZED_USER:
		{
			pErrorWnd->SetError(g_Message[ETC_MESSAGE686].szMessage, //"던전서버 로그인에 실패했습니다.", 
				g_Message[ETC_MESSAGE689].szMessage, //"유저인증에 실패했습니다.", 
				g_Message[ETC_MESSAGE600].szMessage,//"운영자에게 문의하세요!", 
				3);
		}
		break;

	case DUNGEON_JOIN_FAIL_INVALID_CLASS_OR_HEAD:
		{
			pErrorWnd->SetError(g_Message[ETC_MESSAGE686].szMessage, //"던전서버 로그인에 실패했습니다.", 
				g_Message[ETC_MESSAGE690].szMessage, //"캐릭터 정보가 올바르지 않습니다.", 
				g_Message[ETC_MESSAGE600].szMessage,//"운영자에게 문의하세요!", 
				3);
		}
		break;
	case DUNGEON_JOIN_FAIL_NOT_MINMAX_LEVEL:
		{
			pErrorWnd->SetError(g_Message[ETC_MESSAGE1244].szMessage, //"레벨이 맞지 않아 조인할수 없습니다."
				g_Message[ETC_MESSAGE1245].szMessage, // "레벨을 확인하세요."
				"", 
				3);
		}break;
	case DUNGEON_JOIN_FAIL_INVALID_START_SPOT:
		{
			pErrorWnd->SetError(g_Message[ETC_MESSAGE1246].szMessage, // "들어갈 위치가 없습니다."
				g_Message[ETC_MESSAGE1247].szMessage,//"다시 시도하세요."
				g_Message[ETC_MESSAGE600].szMessage,//"운영자에게 문의하세요!", 
				3);
		}break;
	case DUNGEON_JOIN_FAIL_NOT_ENOUGH_ENTER_KARZ:
		{
			pErrorWnd->SetError(g_Message[ETC_MESSAGE699].szMessage,
				g_Message[ETC_MESSAGE1247].szMessage, // "다시 시도하세요."
				g_Message[ETC_MESSAGE600].szMessage,//"운영자에게 문의하세요!", 
				3);
		}break;
	}

	g_pNet->DisconnectServer( SERVER_INDEX_ZONE );
	pErrorWnd->SetActive(TRUE);
	pErrorWnd->m_byType		= 0;
}


