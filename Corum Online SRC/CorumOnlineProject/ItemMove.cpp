#include "StdAfx.h"
#include "ItemMove.h"
#include "GameControl.h"
#include "User.h"
#include "InitGame.h"
#include "message.h"
#include "Chat.h"
#include "GameMenuWnd.h"


BOOL IsItemDungeonManager(ITEM_NATIVE eItemNative)
{
	return ITEM_NATIVE_DUNGEONMANAGER == eItemNative;
}


BOOL IsItemNativeTile(ITEM_NATIVE eItemNative)
{
	return ITEM_NATIVE_TILE == eItemNative;
}


BOOL IsItemNativePlayer(ITEM_NATIVE eItemNative)
{
	return ITEM_NATIVE_PLAYER_START < eItemNative && ITEM_NATIVE_PLAYER_END > eItemNative;
}


ITEM_NATIVE_TYPE GetItemNativeType(ITEM_NATIVE eItemNative)
{
	if( IsItemNativePlayer(eItemNative) )
	{
		if(eItemNative >= ITEM_NATIVE_EQUIP && eItemNative <= ITEM_NATIVE_BANK_SMALL)
		{
			return ITEM_NATIVE_TYPE_REAL;
		}
		else if(eItemNative >= ITEM_NATIVE_TRADE && eItemNative <= ITEM_NATIVE_PLAYERSHOP)
		{
			return ITEM_NATIVE_TYPE_VIRTUAL;
		}
	}
	else
	{
		return ITEM_NATIVE_TYPE_NONE;
	}

	return ITEM_NATIVE_TYPE_NONE;
}


BOOL IsItemNativeTraderMonster(ITEM_NATIVE eItemNative)
{
	return ITEM_NATIVE_TRADERMONSTER_START < eItemNative && ITEM_NATIVE_TRADERMONSTER_END > eItemNative;
}


BOOL IsItemNativeInvLargeAndSmall(ITEM_NATIVE eItemNative)
{
	return ITEM_NATIVE_INV_LARGE == eItemNative || ITEM_NATIVE_INV_SMALL == eItemNative;
}


BOOL IsItemNativeSrcRemove(ITEM_NATIVE eItemNativeDest)
{
	if(	eItemNativeDest == ITEM_NATIVE_TRADE	||	eItemNativeDest == ITEM_NATIVE_UPGRADE ||
		eItemNativeDest == ITEM_NATIVE_MAKING	||	eItemNativeDest == ITEM_NATIVE_PLAYERSHOP )
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}


BYTE GetItemNativePlayerBufferCount()
{
	return ITEM_NATIVE_PLAYER_END - ITEM_NATIVE_PLAYER_START;
}


BYTE GetItemNativeTileBufferCount()
{
	return ITEM_NATIVE_TILE_END - ITEM_NATIVE_TILE_START;
}


BYTE GetItemNativeTraderMonsterCount()
{
	return ITEM_NATIVE_TRADERMONSTER_END - ITEM_NATIVE_TRADERMONSTER_START;
}


void PickupItem(DSTC_ITEM_MOVE* const pItemMove
				, const ITEM_NATIVE_INFO* pItemNativeInfo_Src
				, const ITEM_NATIVE_INFO* pItemNativeInfo_Dest)
{
	if (IsItemNativeTile(pItemNativeInfo_Src->eSlotID))
	{
		CGameMenuWnd*	pGameMenuWnd = CGameMenuWnd::GetInstance();
		char			szInfo[0xff] = {0,};

		CItem			cItemTile;
		CBaseItem*		lpItemTable;
		memcpy(&cItemTile, &pItemMove->Item, sizeof(CItem));
		
		lpItemTable = g_pItemTableHash->GetData(cItemTile.GetID());
		
		ITEM_TYPE eItemType = GetItemType(cItemTile.GetID());

		if(ITEM_TYPE_LARGE == eItemType)
		{
			SetSlotItemNative(&cItemTile, pItemNativeInfo_Dest);
			
			if(lpItemTable)
			{
				// MSG_ID : 81 ; %s �������� ȹ���Ͽ����ϴ�.
				wsprintf(szInfo, g_Message[ETC_MESSAGE81].szMessage, lpItemTable->szItemName_Kor);	
				DisplayMessageAdd(szInfo, 0xFFFF2CFF, pGameMenuWnd->m_bSystemMsgFlag);
			}			
		}
		else if(ITEM_TYPE_SMALL == eItemType)
		{
			const CItem* pItemDest = GetItemNative(pItemNativeInfo_Dest);
				
			if(pItemDest->GetID() == 0)
			{
				SetSlotItemNative(&cItemTile, pItemNativeInfo_Dest);
			}
			else if(cItemTile.GetID() == pItemDest->GetID())
			{
				CItem cItemDest;
				memcpy(&cItemDest, pItemDest, sizeof(CItem));
				cItemDest.SetQuantity(cItemDest.GetQuantity() + cItemTile.GetQuantity());
				SetSlotItemNative(&cItemDest,pItemNativeInfo_Dest);
			}

			if(lpItemTable)
			{
				// MSG_ID : 82 ; %s �������� %d�� ȹ���Ͽ����ϴ�.
				wsprintf(szInfo, g_Message[ETC_MESSAGE82].szMessage, lpItemTable->szItemName_Kor, cItemTile.GetQuantity());	
				DisplayMessageAdd(szInfo, 0xFFFF2CFF, pGameMenuWnd->m_bSystemMsgFlag);
			}
		}
		else if(ITEM_TYPE_MONEY == eItemType)
		{
			g_pMainPlayer->m_dwMoney += cItemTile.m_Item_Money.dwMoney;

			// MSG_ID : 79 ; %d Karz�� ������ϴ�.
			wsprintf(szInfo, g_Message[ETC_MESSAGE79].szMessage, cItemTile.m_Item_Money.dwMoney);	
			DisplayMessageAdd(szInfo, 0xFFFF2CFF, pGameMenuWnd->m_bSystemMsgFlag);
		}
		else
		{
			// ����!!
		}
	}
}


void DropItem(DSTC_ITEM_MOVE* const pItemMove, const ITEM_NATIVE_INFO* pItemNativeInfo_Src, BYTE byQuantityCount)
{
	if( IsItemNativeInvLargeAndSmall(pItemNativeInfo_Src->eSlotID) )
	{
		CItem cItem;
		memcpy(&cItem, g_pMainPlayer->GetItem(pItemNativeInfo_Src->eSlotID, pItemNativeInfo_Src->bySlotIndex), sizeof(CItem));

		BYTE byQuantityCnt = cItem.GetQuantity();

		assert(byQuantityCnt);
		assert(byQuantityCnt >= byQuantityCount);

		if(cItem.IsOverlapItem())
		{
			if(byQuantityCnt > byQuantityCount)
			{
				cItem.SetSerialNum(pItemMove->Item.m_Serial);
				cItem.SetQuantity(byQuantityCnt - byQuantityCount);
				SetSlotItemNative(&cItem,pItemNativeInfo_Src);
			}
			else if(byQuantityCnt == byQuantityCount)
			{
				g_pMainPlayer->RemoveItem(pItemNativeInfo_Src->eSlotID, pItemNativeInfo_Src->bySlotIndex);
			}
		}
		else
		{
			g_pMainPlayer->RemoveItem(pItemNativeInfo_Src->eSlotID, pItemNativeInfo_Src->bySlotIndex);
		}	
	}
	else if(pItemNativeInfo_Src->eSlotID == ITEM_NATIVE_INV_KARZ)
	{
		// Ÿ�Ͽ� ���� �����Ŵ�.
		g_pMainPlayer->m_dwMoney -= pItemNativeInfo_Src->dwMoney;
	}
	else
	{

	}
}


//======================================================================================
// ��� ���� 
//======================================================================================
BOOL PutOnItemNative(const ITEM_NATIVE_INFO* pItemNativeInfo_Src, const ITEM_NATIVE_INFO* pItemNativeInfo_Dest)
{
	BOOL bSuccess = FALSE;

	CItem cItem_Src;
	memcpy(&cItem_Src, GetItemNative(pItemNativeInfo_Src), sizeof(CItem));
	assert(cItem_Src.GetID());
	
	CItem cItem_Dest;
	memcpy(&cItem_Dest, GetItemNative(pItemNativeInfo_Dest), sizeof(CItem));

	BYTE byWeaponKind = cItem_Src.GetWeaponKind();
	BYTE byHandSlotIndex = 0;
	BOOL IsDestItemExistence = FALSE;

	if(pItemNativeInfo_Dest->bySlotIndex == EQUIP_TYPE_RHAND1)
		byHandSlotIndex = EQUIP_TYPE_LHAND1;
	else if(pItemNativeInfo_Dest->bySlotIndex == EQUIP_TYPE_RHAND2)
		byHandSlotIndex = EQUIP_TYPE_LHAND2;
	else if(pItemNativeInfo_Dest->bySlotIndex == EQUIP_TYPE_LHAND1)
		byHandSlotIndex = EQUIP_TYPE_RHAND1;
	else if(pItemNativeInfo_Dest->bySlotIndex == EQUIP_TYPE_LHAND2)
		byHandSlotIndex = EQUIP_TYPE_RHAND2;	

	// �� �ڸ��� �������� �־�?
	if(cItem_Dest.GetID())
		IsDestItemExistence = TRUE;
	else
		IsDestItemExistence = FALSE;
	
	// ����� 
	if(byWeaponKind >= ITEM_CLASS_SWARD && byWeaponKind <= ITEM_CLASS_STAFF)
	{
		ITEM_WEAPON_TYPE eWeaponType = cItem_Src.GetWeaponType();

		// ���ڵ� 
		if( ITEM_WEAPON_TYPE_TWOHAND == eWeaponType )
		{
			if( pItemNativeInfo_Dest->bySlotIndex == EQUIP_TYPE_RHAND1 ||
				pItemNativeInfo_Dest->bySlotIndex == EQUIP_TYPE_RHAND2		)
			{
				CItem cItem_Hand;
				ITEM_NATIVE_INFO ItemNativeInfo_Hand;
				memcpy(&ItemNativeInfo_Hand,pItemNativeInfo_Dest,sizeof(ITEM_NATIVE_INFO));
				ItemNativeInfo_Hand.bySlotIndex = byHandSlotIndex;
				memcpy(&cItem_Hand, GetItemNative(&ItemNativeInfo_Hand), sizeof(CItem));

				// LHand�� ���� �������� ������ 
				if( cItem_Hand.GetID() )
				{
					BOOL bIsEmpty = FALSE;
					
					const CItem* pItemInvLarge = NULL;
					ITEM_NATIVE_INFO ItemNativeInfo_Large;
					ItemNativeInfo_Large.eSlotID = ITEM_NATIVE_INV_LARGE;
					ItemNativeInfo_Large.bySlotIndex = 0;
					ItemNativeInfo_Large.byItemQuantityCnt = 1;

					pItemInvLarge = GetItemNative(&ItemNativeInfo_Large);						

					for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pItemInvLarge[i].GetID() == 0)
						{
							bIsEmpty = TRUE;
							ItemNativeInfo_Large.bySlotIndex = i;
							break;
						}
					}

					// Inv_Large�� �󽽷��� �ִ�.
					if(bIsEmpty)
					{
						// Equip�� �ִ� ���и� InvLarge�� 
						bSuccess = MoveItemNative(NULL, &ItemNativeInfo_Hand, &ItemNativeInfo_Large, cItem_Hand.GetQuantity());														
						if(!bSuccess)
							return bSuccess;

						// InvLarge���ִ� ���ڵ带 Equip��
						if(IsDestItemExistence)
							bSuccess = ExchangeItemNative(pItemNativeInfo_Src, pItemNativeInfo_Dest);
						else
							bSuccess = MoveItemNative(NULL, pItemNativeInfo_Src, pItemNativeInfo_Dest, cItem_Src.GetQuantity());	
						return bSuccess;
					}
					else
					{
						// Inv_Large�� �󽽷��� ����.
						return FALSE;
					}
				}
				else
				{
					// LHand�� ���� �������� ����. 
					if(IsDestItemExistence)
						bSuccess = ExchangeItemNative(pItemNativeInfo_Src, pItemNativeInfo_Dest);
					else
						bSuccess = MoveItemNative(NULL, pItemNativeInfo_Src, pItemNativeInfo_Dest, cItem_Src.GetQuantity());	
					return bSuccess;
				}
			}				
		}
		else if( ITEM_WEAPON_TYPE_ONEHAND == eWeaponType )
		{
			// ���ڵ� 
			if(IsDestItemExistence)
				bSuccess = ExchangeItemNative(pItemNativeInfo_Src, pItemNativeInfo_Dest);
			else
				bSuccess = MoveItemNative(NULL, pItemNativeInfo_Src, pItemNativeInfo_Dest, cItem_Src.GetQuantity());	
			return bSuccess;
		}
	}
	else if(byWeaponKind == ITEM_CLASS_SHIELD)
	{
		// ����  
		if( pItemNativeInfo_Dest->bySlotIndex == EQUIP_TYPE_LHAND1 ||
			pItemNativeInfo_Dest->bySlotIndex == EQUIP_TYPE_LHAND2		)
		{
			CItem cItem_Hand;
			ITEM_NATIVE_INFO ItemNativeInfo_Hand;
			memcpy(&ItemNativeInfo_Hand,pItemNativeInfo_Dest,sizeof(ITEM_NATIVE_INFO));
			ItemNativeInfo_Hand.bySlotIndex = byHandSlotIndex;
			memcpy(&cItem_Hand, GetItemNative(&ItemNativeInfo_Hand), sizeof(CItem));

			// RHand�� �������� ������ 
			if( cItem_Hand.GetID() )
			{
				ITEM_WEAPON_TYPE eWeaponType = cItem_Hand.GetWeaponType();
				
				// ���ڵ� ������ 
				if( ITEM_WEAPON_TYPE_TWOHAND == eWeaponType )
				{
					BOOL bIsEmpty = FALSE;
					
					const CItem* pItemInvLarge = NULL;
					ITEM_NATIVE_INFO ItemNativeInfo_Large;
					ItemNativeInfo_Large.eSlotID = ITEM_NATIVE_INV_LARGE;
					ItemNativeInfo_Large.bySlotIndex = 0;
					ItemNativeInfo_Large.byItemQuantityCnt = 1;

					pItemInvLarge = GetItemNative(&ItemNativeInfo_Large);

					for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pItemInvLarge[i].GetID() == 0)
						{
							bIsEmpty = TRUE;
							ItemNativeInfo_Large.bySlotIndex = i;
							break;
						}
					}

					// Inv_Large�� �󽽷��� �ִ�.
					if(bIsEmpty)
					{
						// Equip�� �ִ� ���ڵ带 InvLarge�� 
						bSuccess = MoveItemNative(NULL, &ItemNativeInfo_Hand, &ItemNativeInfo_Large, cItem_Hand.GetQuantity());														
						if(!bSuccess)
							return bSuccess;	

						// InvLarge���ִ� ���и� Equip�� 
						if(IsDestItemExistence)
							bSuccess = ExchangeItemNative(pItemNativeInfo_Src, pItemNativeInfo_Dest);
						else
							bSuccess = MoveItemNative(NULL, pItemNativeInfo_Src, pItemNativeInfo_Dest, cItem_Src.GetQuantity());	
						return bSuccess;
					}
					else
					{
						// Inv_Large�� �󽽷��� ����.
						return FALSE;
					}
				}
				else if( ITEM_WEAPON_TYPE_ONEHAND == eWeaponType )
				{
					// ���ڵ� ������
					// InvLarge���ִ� ���и� Equip�� 
					if(IsDestItemExistence)
						bSuccess = ExchangeItemNative(pItemNativeInfo_Src, pItemNativeInfo_Dest);
					else
						bSuccess = MoveItemNative(NULL, pItemNativeInfo_Src, pItemNativeInfo_Dest, cItem_Src.GetQuantity());	
					return bSuccess;
				}
			}
			else
			{
				if(IsDestItemExistence)
					bSuccess = ExchangeItemNative(pItemNativeInfo_Src, pItemNativeInfo_Dest);
				else
					bSuccess = MoveItemNative(NULL, pItemNativeInfo_Src, pItemNativeInfo_Dest, cItem_Src.GetQuantity());	
				return bSuccess;
			}
		}	
	}
	else
	{
		// �׿��� �͵�
		if(IsDestItemExistence)
			bSuccess = ExchangeItemNative(pItemNativeInfo_Src, pItemNativeInfo_Dest);
		else
			bSuccess = MoveItemNative(NULL, pItemNativeInfo_Src, pItemNativeInfo_Dest, cItem_Src.GetQuantity());	
		return bSuccess;
	}
	
	return bSuccess;
}


//======================================================================================
// ���� â������ �̵�.
//======================================================================================
BOOL ExchangeItemNative(const ITEM_NATIVE_INFO* pItemNativeInfo_Src
						,const ITEM_NATIVE_INFO* pItemNativeInfo_Dest)
{
	BOOL bSuccess = FALSE;

	CItem cItem_Src;
	memcpy(&cItem_Src, GetItemNative(pItemNativeInfo_Src), sizeof(CItem));
	assert(cItem_Src.GetID());
	
	CItem cItem_Dest;
	memcpy(&cItem_Dest, GetItemNative(pItemNativeInfo_Dest), sizeof(CItem));

	if(cItem_Src.GetID() == cItem_Dest.GetID())
	{
//		LP_ITEM_TABLE pBaseItemDest = g_pItemTableHash->GetData(cItem_Dest.GetID());
		CBaseItem* pBaseItemDest = g_pItemTableHash->GetData(cItem_Dest.GetID());
		assert(pBaseItemDest);

		BYTE byMaxQuantity = pBaseItemDest->GetMaxQuantity();

		if(byMaxQuantity == 1)
		{
			bSuccess = SetSlotItemNative(&cItem_Src, pItemNativeInfo_Dest);
			bSuccess = SetSlotItemNative(&cItem_Dest, pItemNativeInfo_Src);
		}
		else
		{
			BYTE bySrcCnt = cItem_Src.GetQuantity();
			BYTE byDestCnt = cItem_Dest.GetQuantity();

			BYTE byPackInCnt = byMaxQuantity - byDestCnt;
			
			// ����Ʈ ������ ������ �� á��. ��� �ڸ��� �ٲ���. 
			if(byPackInCnt == 0)
			{
				bSuccess = SetSlotItemNative(&cItem_Src, pItemNativeInfo_Dest);
				bSuccess = SetSlotItemNative(&cItem_Dest, pItemNativeInfo_Src);
				return bSuccess;
			}

			if(byPackInCnt >= bySrcCnt)
			{
				cItem_Dest.SetQuantity(byDestCnt + bySrcCnt);
				bSuccess = SetSlotItemNative(&cItem_Dest, pItemNativeInfo_Dest);
				RemoveItemNative(pItemNativeInfo_Src);
			}
			else if(byPackInCnt != 0)
			{
				cItem_Dest.SetQuantity(byDestCnt + byPackInCnt);
				bSuccess = SetSlotItemNative(&cItem_Dest, pItemNativeInfo_Dest);

				cItem_Src.SetQuantity(bySrcCnt - byPackInCnt);
				bSuccess = SetSlotItemNative(&cItem_Src, pItemNativeInfo_Src);
			}
		}
	}
	else 
	{
		bSuccess = SetSlotItemNative(&cItem_Src, pItemNativeInfo_Dest);
		bSuccess = SetSlotItemNative(&cItem_Dest, pItemNativeInfo_Src);
	}

	return bSuccess;
}


//======================================================================================
// ��ũ��Ų ������(ex:PlayerShop,Trade,ItemUpgrade,ItemMakink) ��� �κ����� �����϶�  
//======================================================================================
void AllLinkSrcToDestNative(ITEM_NATIVE_INFO* pItemNativeInfo_Src, ITEM_NATIVE_INFO* pItemNativeInfo_Dest)
{
	CItem cItem_Src;
	memcpy(&cItem_Src, GetItemNative(pItemNativeInfo_Src), sizeof(CItem));

	CItem cItem_Dest;
	memcpy(&cItem_Dest, GetItemNative(pItemNativeInfo_Dest), sizeof(CItem));

	if( cItem_Src.IsOverlapItem() )
	{
		if(cItem_Dest.GetID() == 0)
		{
			SetSlotItemNative(&cItem_Src,pItemNativeInfo_Dest);
			RemoveItemNative(pItemNativeInfo_Src);
		}
		else
		{
			BYTE byCntSrc	= cItem_Src.GetQuantity(); 
			BYTE byCntDest	= cItem_Dest.GetQuantity();
			cItem_Dest.SetQuantity(byCntDest + byCntSrc);			
			SetSlotItemNative(&cItem_Dest,pItemNativeInfo_Dest);
			RemoveItemNative(pItemNativeInfo_Src);
		}
	}
	else
	{
		SetSlotItemNative(&cItem_Src,pItemNativeInfo_Dest);
		RemoveItemNative(pItemNativeInfo_Src);
	}
}


//======================================================================================
// Dest�� �������� ������ ���
//======================================================================================
BOOL MoveItemNative(DSTC_ITEM_MOVE* const pItemMove
					, const ITEM_NATIVE_INFO* pItemNativeInfo_Src
					, const ITEM_NATIVE_INFO* pItemNativeInfo_Dest
					, BYTE byQuantityCount)
{
	CItem cItem_Src;
	memcpy(&cItem_Src, GetItemNative(pItemNativeInfo_Src), sizeof(CItem));
//	LP_ITEM_TABLE pBaseItemSrc = g_pItemTableHash->GetData(cItem_Src.GetID());
	CBaseItem* pBaseItemSrc = g_pItemTableHash->GetData(cItem_Src.GetID());
	
	CItem cItem_Dest;
	memcpy(&cItem_Dest, GetItemNative(pItemNativeInfo_Dest), sizeof(CItem));

	assert(pBaseItemSrc);
	assert(cItem_Src.GetID());
	assert(byQuantityCount);
	assert(cItem_Src.GetQuantity() >= byQuantityCount);

	BYTE byMaxQuantity = pBaseItemSrc->GetMaxQuantity();

	// ������ ���� �ʴ� �������̴�.
	if(byMaxQuantity == 1 && byQuantityCount == 1)
	{
		if( SetSlotItemNative(&cItem_Src, pItemNativeInfo_Dest) )
		{
			RemoveItemNative(pItemNativeInfo_Src);
			return TRUE;
		}
		else
		{
			return FALSE;
		}		
	}
	else
	{
		// ���� ����.
		BYTE bCntSrc		= cItem_Src.GetQuantity();
		BYTE bPackCntSrc	= bCntSrc - byQuantityCount;
		cItem_Src.SetQuantity(bPackCntSrc);

		memcpy(&cItem_Dest,&cItem_Src,sizeof(CItem));
		cItem_Dest.SetQuantity(byQuantityCount);

		if( SetSlotItemNative(&cItem_Src, pItemNativeInfo_Src) )
		{
			if( SetSlotItemNative(&cItem_Dest, pItemNativeInfo_Dest) )
			{
				// ����.. �ҽ� �����ۼ��� ��ü�� ����Ʈ�� �ᱸ��.
				if(bPackCntSrc == 0)
				{
					RemoveItemNative(pItemNativeInfo_Src);
					return TRUE;	
				}
				else
				{
					// ����.. �ҽ� ������ ������ ���� �ֱ���. ����Ʈ �����ۿ� �� �ø����̴�.
					cItem_Dest.SetSerialNum(*pItemMove->Item.GetSerial());
					SetSlotItemNative(&cItem_Dest, pItemNativeInfo_Dest);
					return TRUE;	
				}
			}			
		}
		else
		{
			return FALSE;
		}
	}

	return FALSE;
}


//======================================================================================
// Dest�� �������� �ְ� �ҽ� �����۰� ����Ʈ �������� ���̵� ������ �����̵� �Ҷ� ��� 
//======================================================================================
BOOL OverlapItemNative(DSTC_ITEM_MOVE* const pItemMove
					   , const ITEM_NATIVE_INFO* pItemNativeInfo_Src
					   , const ITEM_NATIVE_INFO* pItemNativeInfo_Dest
					   , BYTE byQuantityCount)
{
	CItem cItem_Src;
	memcpy(&cItem_Src, GetItemNative(pItemNativeInfo_Src), sizeof(CItem));
//	LP_ITEM_TABLE pBaseItem_Src = g_pItemTableHash->GetData(cItem_Src.GetID());
	CBaseItem* pBaseItem_Src = g_pItemTableHash->GetData(cItem_Src.GetID());
		
	CItem cItem_Dest;
	memcpy(&cItem_Dest, GetItemNative(pItemNativeInfo_Dest), sizeof(CItem));
//	LP_ITEM_TABLE pBaseItem_Dest = g_pItemTableHash->GetData(cItem_Src.GetID());
	CBaseItem* pBaseItem_Dest = g_pItemTableHash->GetData(cItem_Src.GetID());

	assert(pBaseItem_Src);
	assert(pBaseItem_Dest);
	assert(byQuantityCount);
	assert(cItem_Src.GetID());
	assert(cItem_Dest.GetID());
	assert(cItem_Src.GetID() == cItem_Dest.GetID());
	assert(cItem_Src.GetQuantity() >= byQuantityCount);	

	BYTE byMaxQuantity = pBaseItem_Src->GetMaxQuantity();

	// ������ �ߺ��Ǵ� �������̴�.
	if(byMaxQuantity > 1)
	{
		BYTE bCntSrc = cItem_Src.GetQuantity();
		BYTE bPackCntSrc = bCntSrc - byQuantityCount;
		cItem_Src.SetQuantity(bPackCntSrc);

		BYTE bCntDest = cItem_Dest.GetQuantity();
		BYTE bPackCntDest = bCntDest + byQuantityCount;
		cItem_Dest.SetQuantity(bPackCntDest);

		if( SetSlotItemNative(&cItem_Src, pItemNativeInfo_Src) )
		{
			if(bPackCntSrc == 0)
			{
				RemoveItemNative(pItemNativeInfo_Src);
			}

			if( SetSlotItemNative(&cItem_Dest, pItemNativeInfo_Dest) )
			{
				return TRUE;	
			}
		}
		else
		{
			return FALSE;
		}
	}

	return FALSE;
}


void RemoveItemNative(const ITEM_NATIVE_INFO* pItemNativeInfo)
{
	if (IsItemNativePlayer(pItemNativeInfo->eSlotID))
	{
		g_pMainPlayer->RemoveItem(pItemNativeInfo->eSlotID, pItemNativeInfo->bySlotIndex);
	}	
}


BOOL SetSlotItemNative(const CItem* pItem_Src, const ITEM_NATIVE_INFO* pItemNativeInfo_Dest)
{
	if (IsItemNativePlayer(pItemNativeInfo_Dest->eSlotID))
	{
		g_pMainPlayer->SetItem(pItemNativeInfo_Dest->eSlotID, pItemNativeInfo_Dest->bySlotIndex, pItem_Src);
	}

	return TRUE;
}


const CItem* GetItemNative(const ITEM_NATIVE_INFO* pItemNativeInfo)
{
	// ������ �����ϸ� �ȴ�.
	if (IsItemNativePlayer(pItemNativeInfo->eSlotID))
	{
		return g_pMainPlayer->GetItem(pItemNativeInfo->eSlotID, pItemNativeInfo->bySlotIndex);
	}
	else if (IsItemNativeTile(pItemNativeInfo->eSlotID))
	{
		
	}
	else if (IsItemNativeTraderMonster(pItemNativeInfo->eSlotID))
	{
		
	}
	
	return NULL;
}