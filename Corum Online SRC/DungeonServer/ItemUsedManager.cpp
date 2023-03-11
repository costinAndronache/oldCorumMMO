#include "stdafx.h"
#include "ItemUsedManager.h"
#include "ItemMoveManager.h"
#include "ItemMove.h"
#include "ItemNative.h"
#include "User.h"
#include "OwnServer.h"
#include "Dungeon.h"
#include "DungeonLayer.h"
#include "Map.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "DUNGEON_DATA_EX.h"
#include "MonsterManager.h"
#include "TraderMonster.h"
#include "ItemNativeManager.h"
#include "DungeonTable.h"
#include "ServerFunction.h"
#include "RecordSet.h"
#include "GameSystemNative.h"
#include "../CommonServer/CommonHeader.h"


CItemUsedManager::CItemUsedManager()
{
	memset(CItemUsedManager::ItemUsedTypeProc,NULL,sizeof(CItemUsedManager::ItemUsedTypeProc));
	memset(CItemUsedManager::ItemUsedValueProc,NULL,sizeof(CItemUsedManager::ItemUsedValueProc));

	InitItemUsedTypeProc();
	InitItemUsedValueProc();
}


CItemUsedManager::~CItemUsedManager()
{

}


void CItemUsedManager::InitItemUsedTypeProc()
{
	ItemUsedTypeProc[ITEM_USED_NATIVE_TYPE_ZODIAC]		= ItemUsedNativeTypeZodiac;
	ItemUsedTypeProc[ITEM_USED_NATIVE_TYPE_SUPPLIES]	= ItemUsedNativeTypeSupplies;
	ItemUsedTypeProc[ITEM_USED_NATIVE_TYPE_SPECIAL]		= ItemUsedNativeTypeSpecial;
	ItemUsedTypeProc[ITEM_USED_NATIVE_TYPE_MAGICARRAY]	= ItemUsedNativeTypeMagicarray;
	ItemUsedTypeProc[ITEM_USED_NATIVE_TYPE_CONSUMABLE]	= ItemUsedNativeTypeConsumable;
}


void CItemUsedManager::InitItemUsedValueProc()
{
	ItemUsedValueProc[ITEM_USED_NATIVE_VALUE_COORDINATE_SAVE]		= Value_ItemUsedNative_CoordinateSave;
	ItemUsedValueProc[ITEM_USED_NATIVE_VALUE_GOTO_COORDINATE]		= Value_ItemUsedNative_Goto_Coordinate;
	ItemUsedValueProc[ITEM_USED_NATIVE_VALUE_TOWNPORTALSCROLL]		= Value_ItemUsedNative_TownPortalScroll;
	ItemUsedValueProc[ITEM_USED_NATIVE_VALUE_INSURANCE]				= Value_ItemUsedNative_Insurance;
	
	ItemUsedValueProc[ITEM_USED_NATIVE_VALUE_HP]					= Value_ItemUsedNative_HP;
	ItemUsedValueProc[ITEM_USED_NATIVE_VALUE_MP]					= Value_ItemUsedNative_MP;
	ItemUsedValueProc[ITEM_USED_NATIVE_VALUE_HP_POINT]				= Value_ItemUsedNative_HP_Point;
	ItemUsedValueProc[ITEM_USED_NATIVE_VALUE_SP_HP_POINT]			= Value_ItemUsedNative_SP_HP_Point;
	ItemUsedValueProc[ITEM_USED_NATIVE_VALUE_HATCH_TIME_SHORTENING] = Value_ItemUsedNative_HatchTimeShortening;

	ItemUsedValueProc[ITEM_USED_NATIVE_VALUE_GUILD_CLEN_MAKE]		= Value_ItemUsedNative_Guild_Clen_Make;
	ItemUsedValueProc[ITEM_USED_NATIVE_VALUE_STATUS_SKILL_INIT]		= Value_ItemUsedNative_Status_Skill_Init;
	ItemUsedValueProc[ITEM_USED_NATIVE_VALUE_VOUCHER_OF_HERO]		= Value_ItemUsedNative_Voucher_Of_Hero;
	
	ItemUsedValueProc[ITEM_USED_NATIVE_VALUE_PERSONAL_RECALL]		= Value_ItemUsedNative_PersonalRecall;
	ItemUsedValueProc[ITEM_USED_NATIVE_VALUE_ALL_PARTY_RECALL]		= Value_ItemUsedNative_AllPartyRecall;
	ItemUsedValueProc[ITEM_USED_NATIVE_VALUE_GUILD_RECALL]			= Value_ItemUsedNative_Guild_Recall;
	ItemUsedValueProc[ITEM_USED_NATIVE_VALUE_MAGICARRAY_TO_PARTY]	= Value_ItemUsedNative_MagicArrayToParty;
} 


//============================================================================================
// ������ ����Ҽ� �ִ� ������?
//============================================================================================
BOOL CItemUsedManager::IsAvailability(CUser* pUser,CDungeonLayer** pLayer,CTDS_ITEM_USED_NATIVE* const pItemUsed)
{
	if (!pUser)												return FALSE;
	if (!pUser->GetCurDungeon() )							return FALSE;
	if ( pUser->GetUnitStatus() == UNIT_STATUS_DEAD )		return FALSE;
	if ( pUser->GetUnitStatus() == UNIT_STATUS_CASTING )	return FALSE;	

	*pLayer =  pUser->GetCurDungeonLayer();

	if ( NULL == *pLayer )									return FALSE;
	if ( pUser->m_byRest == 1 )								return FALSE;
	if ( pUser->m_bPortalMoving == 1 )						return FALSE;
	if ( CheckShop(pUser) )									return FALSE;
	if ( CheckTrade(pUser->m_nItemTradeSeriel) )			return FALSE;
	if ( pUser->IsCurDungeonMatchInLive() )					return FALSE;
		
	return TRUE;
}


//============================================================================================
// Ŭ���̾�Ʈ�� ������ ��Ŷ ����� 
//============================================================================================
void CItemUsedManager::InitItemMoveResultPacket(CTDS_ITEM_USED_NATIVE* const pItemUsed
												,DSTC_ITEM_USED_NATIVE* const pItemUsedResult)
{
	pItemUsedResult->bSrc				= pItemUsed->bSrc;
	pItemUsedResult->bSrcIndex			= pItemUsed->bSrcIndex;
	pItemUsedResult->bCommonIndex		= pItemUsed->bCommonIndex; 

	pItemUsedResult->dwCommonId			= pItemUsed->dwCommonId;
	pItemUsedResult->dwPortalUserIndex	= pItemUsed->dwPortalUserIndex; 
}


//============================================================================================
// ������ ��� �α� 
//============================================================================================
void CItemUsedManager::ItemMoveLogProcess()
{
	 
}


//============================================================================================
// ��밡���� �������� �ִ��� �˻��Ѵ�.
//============================================================================================
BOOL CItemUsedManager::FindItemUsedNativeValue(CTDS_ITEM_USED_NATIVE* const pItemUsed, BYTE* pbSrc, CUser* const pUser)
{
	ITEM_NATIVE_INFO	ItemNativeInfo;
	const CItem*		pItemNative		= NULL;
	CBaseItem*			pBaseItemUsed	= NULL;

	ItemNativeInfo.eSlotID = (ITEM_NATIVE)pItemUsed->bSrc;

	if(::IsItemNativePlayer(ItemNativeInfo.eSlotID))
	{
		ItemNativeInfo.dwOwnerID	= pUser->GetID();
		ItemNativeInfo.bySlotIndex	= pItemUsed->bSrcIndex;
	}
	else
	{
		return FALSE;
	}

	pItemNative		= GetItemNative(NULL, &ItemNativeInfo);
	pBaseItemUsed	= pItemNative->GetBaseItem();
	
	if(NULL == pBaseItemUsed)
	{
		return FALSE;
	}

	if( pItemNative->GetQuantity() == 0 )
	{
		// ����.. ������ �ϳ��� ���ݾ�. ���� 
		return FALSE;
	}

	if(pItemNative && pItemNative->GetID() != 0)
	{
		DWORD wKind = GetItemKind(pItemNative->GetID());

		switch(wKind)
		{
			case ITEM_KIND_ZODIAC		:	*pbSrc = ITEM_USED_NATIVE_TYPE_ZODIAC;		break;
			case ITEM_KIND_SUPPLIES		:	*pbSrc = ITEM_USED_NATIVE_TYPE_SUPPLIES;	break;
			case ITEM_KIND_SPECIAL		:	*pbSrc = ITEM_USED_NATIVE_TYPE_SPECIAL;		break;
			case ITEM_KIND_MAGICARRAY	:	*pbSrc = ITEM_USED_NATIVE_TYPE_MAGICARRAY;	break;
			case ITEM_KIND_CONSUMABLE	:	*pbSrc = ITEM_USED_NATIVE_TYPE_CONSUMABLE;	break;
			default:	return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}


//============================================================================================
// ������ ��� ��Ŷ ó���Ѵ�.
//============================================================================================
void CItemUsedManager::Process(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	BYTE bSrc							= 0;
	CUser* pUser						= NULL;
	CDungeonLayer* pLayer				= NULL;
	CTDS_ITEM_USED_NATIVE*	pItemUsed	= NULL;
	DSTC_ITEM_USED_NATIVE	ItemUsedResult;

	pUser		= (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
	pItemUsed	= (CTDS_ITEM_USED_NATIVE*)pMsg;

	if( !IsAvailability(pUser,&pLayer,pItemUsed) )
	{
		ItemUsedResult.bErrorCode = GET_NOT_AVAILABILITY;
		SendToItemMoveResult(dwConnectionIndex, &ItemUsedResult);
		return;
	}

	if( !FindItemUsedNativeValue(pItemUsed, &bSrc, pUser) )
	{
		ItemUsedResult.bErrorCode = CANNOT_FIND_NATIVE_VALUE;
		SendToItemMoveResult(dwConnectionIndex, &ItemUsedResult);
		return;
	}

	ItemUsedResult.bItemUsedNativeType = bSrc;

	InitItemMoveResultPacket(pItemUsed, &ItemUsedResult);

	if( pUser->GetItemNativeLinkCount() )
	{
		// ��ũ �ɷ��ִ� �������� ������ 
		ITEM_NATIVE eNative			= pUser->GetCurItemNativeLink();
		ItemUsedResult.bErrorCode	= ITEM_LINKING;
		ItemUsedResult.bSrc			= (BYTE)eNative;		
		SendToItemMoveResult(dwConnectionIndex, &ItemUsedResult);
		return;
	}

	if(ItemUsedTypeProc[bSrc])
	{
		(this->*ItemUsedTypeProc[bSrc])(dwConnectionIndex, pUser, pLayer, pItemUsed, &ItemUsedResult);		
		SendToItemMoveResult(dwConnectionIndex, &ItemUsedResult);
	}
	else
	{
		ItemUsedResult.bErrorCode = CANNOT_FIND_NATIVE_VALUE;
		SendToItemMoveResult(dwConnectionIndex, &ItemUsedResult);
		return;
	}
	
	if(ItemUsedResult.bErrorCode == 0xff)
	{
		CheckSpeed(pUser);
	}
}


//============================================================================================
// ������ ��� ����� Ŭ���̾�Ʈ�� �����ش�.
//============================================================================================
void CItemUsedManager::SendToItemMoveResult(DWORD dwConnectionIndex, DSTC_ITEM_USED_NATIVE* const pItemUsedResult)
{
	NetSendToUser( dwConnectionIndex, (char*)pItemUsedResult, pItemUsedResult->GetPacketSize()
		, FLAG_SEND_NOT_ENCRYPTION );
}


//============================================================================================
// ����� ī��� ��ȯ ��ũ�� ���  
//============================================================================================
void CItemUsedManager::ItemUsedNativeTypeZodiac(DWORD dwConnectionIndex
												, CUser* const pUser 
												, CDungeonLayer* const pLayer
												, CTDS_ITEM_USED_NATIVE* const pItemUsed
												, DSTC_ITEM_USED_NATIVE* const pItemUsedResult)
{
	const CItem*		pItemNative			= NULL;
	BYTE				bItemNativeValue	= NULL;
	BOOL				bSuccess			= FALSE;
	ITEM_NATIVE_INFO	ItemNativeInfo_Used;
	
	ItemNativeInfo_Used.eSlotID		= (ITEM_NATIVE)pItemUsed->bSrc;
	ItemNativeInfo_Used.dwOwnerID	= pUser->GetID();
	ItemNativeInfo_Used.bySlotIndex	= pItemUsed->bSrcIndex;

	pItemNative	= GetItemNative(NULL, &ItemNativeInfo_Used);

	// �α׸� ���� ī���Ѵ�.
	CItem cItem;
	memcpy(&cItem, pItemNative, sizeof(CItem));
	WORD wItemID = pItemNative->GetID();
	
	if( wItemID == __ITEM_PORTAL_INDEX__)
	{
		if(FALSE == pItemNative->m_Item_Zodiac.bType)
		{
			// ��ǥ ���� �ȵǾ� �ִ�.
			bItemNativeValue = ITEM_USED_NATIVE_VALUE_COORDINATE_SAVE;
		}
		else if(TRUE == pItemNative->m_Item_Zodiac.bType)
		{
			// ��ǥ ���� �Ǿ� �ִ�.
			const CItem*		pItemNativeAquarius = NULL;
			ITEM_NATIVE_INFO	ItemNativeInfo_Aquarius;
			
			ItemNativeInfo_Aquarius.eSlotID		= ITEM_NATIVE_INV_SMALL;
			ItemNativeInfo_Aquarius.dwOwnerID	= pUser->GetID();
			ItemNativeInfo_Aquarius.bySlotIndex = pItemUsed->bCommonIndex;

			pItemNativeAquarius = GetItemNative(NULL, &ItemNativeInfo_Aquarius);

			if(pItemNativeAquarius && pItemNativeAquarius->GetID() == __ITEM_PORTALUSED_INDEX__)
			{
				bItemNativeValue = ITEM_USED_NATIVE_VALUE_GOTO_COORDINATE;	
			}
			else
			{
				// ���� ã��...
				BOOL bIsFindWater = FALSE;
				ItemNativeInfo_Aquarius.bySlotIndex = 0;	

				pItemNativeAquarius = GetItemNative(NULL, &ItemNativeInfo_Aquarius);
				
				for( BYTE i = 0; i < MAX_INV_SMALL_POOL; i++ )
				{
					if(pItemNativeAquarius[i].GetID() == __ITEM_PORTALUSED_INDEX__)
					{
						bIsFindWater = TRUE;
						pItemUsedResult->bCommonIndex = i;
						break;
					}
				}

				if(bIsFindWater)
				{
					bItemNativeValue = ITEM_USED_NATIVE_VALUE_GOTO_COORDINATE;					
				}
				else
				{
					// ���� ���ݾ�..
					pItemUsedResult->bErrorCode = CANNOT_FIND_AQUARIUS;
					return;
				}
			}
		}
	}
	// ���� ��ȯ ��ũ�� �߰� �迵�� 2004.02.14
	else if (wItemID >= __ITEM_PORTAL_VILL1__  &&  wItemID <= __ITEM_PORTAL_VILL6__ )
	//else if(pItemNative->GetID() == __ITEM_PORTAL_VILL1__ || pItemNative->GetID() == __ITEM_PORTAL_VILL2__ ||
	//		pItemNative->GetID() == __ITEM_PORTAL_VILL3__ || pItemNative->GetID() == __ITEM_PORTAL_VILL4__ ||
	//		pItemNative->GetID() == __ITEM_PORTAL_VILL5__)
	{
		bItemNativeValue = ITEM_USED_NATIVE_VALUE_TOWNPORTALSCROLL;
	}

	if(ItemUsedValueProc[bItemNativeValue])
	{
		bSuccess = (this->*ItemUsedValueProc[bItemNativeValue])(dwConnectionIndex, pUser, pLayer, pItemNative
			, &ItemNativeInfo_Used, pItemUsedResult);

		if(bSuccess)
		{
			g_ItemMoveManager.ItemMoveLogProcess(&cItem, ITEM_LOG_USED, pUser->GetCurDungeonID(), (char*)pUser->GetName());
			pItemUsedResult->bItemUsedNativeValue = bItemNativeValue;
		}			
	}
	else
	{
		pItemUsedResult->bErrorCode = CANNOT_FIND_VALUE_PROC;
	}
}


//============================================================================================
// ���� ��� 
//============================================================================================
void CItemUsedManager::ItemUsedNativeTypeSupplies(DWORD dwConnectionIndex
												  , CUser* const pUser 
												  , CDungeonLayer* const pLayer
												  , CTDS_ITEM_USED_NATIVE* const pItemUsed
												  , DSTC_ITEM_USED_NATIVE* const pItemUsedResult)
{
	const CItem*		pItemNative			= NULL;
	CBaseItem*			pBaseItemUsed		= NULL;
	int					iItemDelay			= NULL;
	BYTE				bItemNativeValue	= NULL;
	BOOL				bSuccess			= FALSE;
	ITEM_NATIVE_INFO	ItemNativeInfo_Used;
	
	ItemNativeInfo_Used.eSlotID		= (ITEM_NATIVE)pItemUsed->bSrc;
	ItemNativeInfo_Used.dwOwnerID	= pUser->GetID();
	ItemNativeInfo_Used.bySlotIndex	= pItemUsed->bSrcIndex;

	pItemNative		= GetItemNative(NULL, &ItemNativeInfo_Used);
	pBaseItemUsed	= pItemNative->GetBaseItem();

	// �α׸� ���� ī���Ѵ�.
	CItem cItem;
	memcpy(&cItem, pItemNative, sizeof(CItem));
	
	iItemDelay = g_pBaseClassInfo[pUser->GetClass()].wPotion_Delay + g_pBaseRaceInfo[pUser->GetClass()].wPotion_Delay;
			
	if( g_dwTickCount - pUser->m_dwTemp[USER_TEMP_LAST_ITEM_CASTING] < (DWORD)iItemDelay )
	{
		pItemUsedResult->bErrorCode = USER_LAST_ITEM_CASTING;
		return;
	}
			
	pUser->m_dwTemp[USER_TEMP_LAST_ITEM_CASTING] = g_dwTickCount;

	// ���ν��� �迭 �ε����� ���߾� �ش�.
	bItemNativeValue = pBaseItemUsed->BaseItem_Supplies.bType + ITEM_USED_NATIVE_VALUE_SUPPLIES_START;

	if(ItemUsedValueProc[bItemNativeValue])
	{
		bSuccess = (this->*ItemUsedValueProc[bItemNativeValue])(dwConnectionIndex, pUser, pLayer, pItemNative
			, &ItemNativeInfo_Used, pItemUsedResult);

		if(bSuccess)
		{
			g_ItemMoveManager.ItemMoveLogProcess(&cItem, ITEM_LOG_USED, pUser->GetCurDungeonID(), (char*)pUser->GetName());
			pItemUsedResult->bItemUsedNativeValue = bItemNativeValue;
		}	
	}
	else
	{
		pItemUsedResult->bErrorCode = CANNOT_FIND_VALUE_PROC;
	}
}


//============================================================================================
// ����� ������ ��� 
//============================================================================================
void CItemUsedManager::ItemUsedNativeTypeSpecial(DWORD dwConnectionIndex
												 , CUser* const pUser 
												 , CDungeonLayer* const pLayer
												 , CTDS_ITEM_USED_NATIVE* const pItemUsed
												 , DSTC_ITEM_USED_NATIVE* const pItemUsedResult)
{
	const CItem*		pItemNative			= NULL;
	BYTE				bItemNativeValue	= NULL;
	BOOL				bSuccess			= FALSE;
	WORD				wItemId				= NULL;
	ITEM_NATIVE_INFO	ItemNativeInfo_Used;
		
	ItemNativeInfo_Used.eSlotID		= (ITEM_NATIVE)pItemUsed->bSrc;
	ItemNativeInfo_Used.dwOwnerID	= pUser->GetID();
	ItemNativeInfo_Used.bySlotIndex	= pItemUsed->bSrcIndex;

	pItemNative	= GetItemNative(NULL, &ItemNativeInfo_Used);
	wItemId		= pItemNative->GetID();

	// �α׸� ���� ī���Ѵ�.
	CItem cItem;
	memcpy(&cItem, pItemNative, sizeof(CItem));

	if( wItemId == __ITEM_GUILD_INDEX__ || wItemId == __ITEM_CLEN_INDEX__ )
	{
		bItemNativeValue = ITEM_USED_NATIVE_VALUE_GUILD_CLEN_MAKE;
	}
	else if( wItemId == __ITEM_STATUS_INT__ ||
			 (wItemId >= __ITEM_STATUS_INTSTART__ && wItemId <= __ITEM_STATUS_INTEND__) ||
			 wItemId==__ITEM_SKILL_INT__	||
			 (wItemId>=__ITEM_SKILL_INTSTART__ && wItemId<=__ITEM_SKILL_INTEND__) )
	{
		bItemNativeValue = ITEM_USED_NATIVE_VALUE_STATUS_SKILL_INIT;
	}
	else if( wItemId == __ITEM_HEROITEM_INDEX__ )
	{
		bItemNativeValue = ITEM_USED_NATIVE_VALUE_VOUCHER_OF_HERO;
	}

	if(ItemUsedValueProc[bItemNativeValue])
	{
		bSuccess = (this->*ItemUsedValueProc[bItemNativeValue])(dwConnectionIndex, pUser, pLayer, pItemNative
			, &ItemNativeInfo_Used, pItemUsedResult);

		if(bSuccess)
		{
			g_ItemMoveManager.ItemMoveLogProcess(&cItem, ITEM_LOG_USED, pUser->GetCurDungeonID(), (char*)pUser->GetName());
			pItemUsedResult->bItemUsedNativeValue = bItemNativeValue;
		}			
	}
	else
	{
		pItemUsedResult->bErrorCode = CANNOT_FIND_VALUE_PROC;
	}
}


//============================================================================================
// ������ ��ũ,����,�ںν�,�ó�����ƽ Ʈ���̴� ��� 
//============================================================================================
void CItemUsedManager::ItemUsedNativeTypeMagicarray(DWORD dwConnectionIndex
													, CUser* const pUser 
													, CDungeonLayer* const pLayer
													, CTDS_ITEM_USED_NATIVE* const pItemUsed
													, DSTC_ITEM_USED_NATIVE* const pItemUsedResult)
{
	const CItem*		pItemNative			= NULL;
	BYTE				bItemNativeValue	= NULL;
	BOOL				bSuccess			= FALSE;
	WORD				wItemId				= NULL;
	ITEM_NATIVE_INFO	ItemNativeInfo_Used;
		
	ItemNativeInfo_Used.eSlotID		= (ITEM_NATIVE)pItemUsed->bSrc;
	ItemNativeInfo_Used.dwOwnerID	= pUser->GetID();
	ItemNativeInfo_Used.bySlotIndex	= pItemUsed->bSrcIndex;

	pItemNative	= GetItemNative(NULL, &ItemNativeInfo_Used);
	wItemId		= pItemNative->GetID();

	// �α׸� ���� ī���Ѵ�.
	CItem cItem;
	memcpy(&cItem, pItemNative, sizeof(CItem));

	if(wItemId == __ITEM_PARTY_PORTAL1__)
	{
		bItemNativeValue = ITEM_USED_NATIVE_VALUE_PERSONAL_RECALL;
	}
	else if(wItemId == __ITEM_PARTY_PORTAL2__)
	{
		bItemNativeValue = ITEM_USED_NATIVE_VALUE_ALL_PARTY_RECALL;
	}
	else if(wItemId == __ITEM_GUILD_RECALL_INDEX__)
	{
		bItemNativeValue = ITEM_USED_NATIVE_VALUE_GUILD_RECALL;
	}
	else if((wItemId>=__ITEM_PARTY_LIFELINK1__	&&	wItemId<=__ITEM_PARTY_LIFELINK5__)	||
			(wItemId>=__ITEM_PARTY_AMBUSH1__	&&	wItemId<=__ITEM_PARTY_AMBUSH5__)	||
			(wItemId>=__ITEM_PARTY_ST1__		&&	wItemId<=__ITEM_PARTY_ST5__))
	{
		bItemNativeValue = ITEM_USED_NATIVE_VALUE_MAGICARRAY_TO_PARTY;
	}

	if(ItemUsedValueProc[bItemNativeValue])
	{
		bSuccess = (this->*ItemUsedValueProc[bItemNativeValue])(dwConnectionIndex, pUser, pLayer, pItemNative
			, &ItemNativeInfo_Used, pItemUsedResult);

		if(bSuccess)
		{
			g_ItemMoveManager.ItemMoveLogProcess(&cItem, ITEM_LOG_USED, pUser->GetCurDungeonID(), (char*)pUser->GetName());
			pItemUsedResult->bItemUsedNativeValue = bItemNativeValue;
		}	
	}
	else
	{
		pItemUsedResult->bErrorCode = CANNOT_FIND_VALUE_PROC;
	}
}


//============================================================================================
// ���ϸ��� ������ ��� 
//============================================================================================
void CItemUsedManager::ItemUsedNativeTypeConsumable(DWORD dwConnectionIndex
													, CUser* const pUser 
													, CDungeonLayer* const pLayer
													, CTDS_ITEM_USED_NATIVE* const pItemUsed
													, DSTC_ITEM_USED_NATIVE* const pItemUsedResult)
{
	CItem*				pAttachItem			= NULL;
	CBaseItem*			pBaseItemUsed		= NULL;
	WORD				wItemID				= NULL;
	ITEM_NATIVE_INFO	ItemNativeInfo_Used;
	
	ItemNativeInfo_Used.eSlotID		= (ITEM_NATIVE)pItemUsed->bSrc;
	ItemNativeInfo_Used.dwOwnerID	= pUser->GetID();
	ItemNativeInfo_Used.bySlotIndex	= pItemUsed->bSrcIndex;

	pAttachItem		= (CItem*)GetItemNative(NULL, &ItemNativeInfo_Used);
	pBaseItemUsed	= pAttachItem->GetBaseItem();
	wItemID			= pAttachItem->GetID();
	
	// �ӽ÷� ��ų�϶��� �̷��� �ߴ�. ��ĥ����. <-- ������?
	if(wItemID >= 20519 && wItemID <= 20578)	// ����� ��ų�� ID�� �ٷ� ���ٴ� �����̱� : �ִ��� 2005.1.17
	{
		CMonster* pMonster = pUser->GetGuardian(0);
		if(pMonster)
		{
			for(int i = 0; i < MAX_BASEITEM_CONSUMABLE_ITEMATTR; ++i)
			{
				if(pBaseItemUsed->BaseItem_Consumable.sItem_Attr[i].wAttr_Code)
				{
					pMonster->SetItemAttr((ITEM_ATTR_CODE)pBaseItemUsed->BaseItem_Consumable.sItem_Attr[i].wAttr_Code
					, pBaseItemUsed->BaseItem_Consumable.sItem_Attr[i].wAttr_Value);
				}
			}		
		}

		memset(pAttachItem, NULL, sizeof(CItem));
		memcpy(&pItemUsedResult->cItem, pAttachItem, sizeof(CItem));
		pItemUsedResult->bItemUsedNativeValue = ITEM_USED_NATIVE_VALUE_CONSUMABLE_START;

		g_ItemMoveManager.ItemMoveLogProcess(pAttachItem, ITEM_LOG_USED, pUser->GetCurDungeonID(), (char*)pUser->GetName());
		return;
	}

	if( !pUser->IsAvailabilityAttachItem(wItemID) )
	{
		pItemUsedResult->bErrorCode = THIS_ITEM_IS_OCCUPIABLE;
		return;
	}
	else if( pAttachItem->m_Item_Consumable.bInvIndex || pUser->GetLevel() > pBaseItemUsed->BaseItem_Consumable.wMax_Lev )
	{
		pItemUsedResult->bErrorCode = CANNOT_ENOUGH_LEVEL;
		return;
	}

	if(ITEM_LAND_TYPE_SERVERTIME == pBaseItemUsed->GetLandItemType())
	{
		pAttachItem->m_Item_Consumable.dwEndDate = GetDueDay(pBaseItemUsed->BaseItem_Consumable.wLast_Time);
	}
	else if(ITEM_LAND_TYPE_PLAYTIME == pBaseItemUsed->GetLandItemType())
	{
		pAttachItem->m_Item_Consumable.dwEndDate = GetLandTime(pBaseItemUsed->BaseItem_Consumable.wLast_Time);
	}
	
	pAttachItem->m_Item_Consumable.bInvIndex = ItemNativeInfo_Used.bySlotIndex + 1;

	if( !SetSlotItemNative(NULL, pAttachItem, &ItemNativeInfo_Used) )
	{
		pItemUsedResult->bErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
		return;
	}

	pUser->AttachItem(pAttachItem);

	memcpy(&pItemUsedResult->cItem, pAttachItem, sizeof(CItem));
	pItemUsedResult->bItemUsedNativeValue = ITEM_USED_NATIVE_VALUE_CONSUMABLE_START;

	g_ItemMoveManager.ItemMoveLogProcess(pAttachItem, ITEM_LOG_USED, pUser->GetCurDungeonID(), (char*)pUser->GetName());
}


//========================
// HP
//========================
BOOL CItemUsedManager::Value_ItemUsedNative_HP(DWORD dwConnectionIndex
											   , CUser* const pUser 
											   , CDungeonLayer* const pLayer
											   , const CItem* pItemNative
											   , ITEM_NATIVE_INFO* pItemNativeInfo_Used
											   , DSTC_ITEM_USED_NATIVE* const pItemUsedResult)
{
	float						fPer			= NULL;
	CBaseItem*					pBaseItemUsed	= NULL;
	DWORD						dwRand			= NULL;
	WORD						wMax			= NULL;
	WORD						wMin			= NULL;
	
	CItem cItem;
	memcpy(&cItem, pItemNative, sizeof(CItem));
	pBaseItemUsed = cItem.GetBaseItem();

	cItem.SetQuantity( cItem.GetQuantity() - 1 );

	if( cItem.GetQuantity() == 0 )
	{
		if( !RemoveItemNative(pItemNativeInfo_Used) )
		{
			pItemUsedResult->bErrorCode = REMOVE_ITEMNATIVE_FAIL;
			return FALSE;
		}
	}
	else
	{
		if( !SetSlotItemNative(pLayer, &cItem, pItemNativeInfo_Used) )
		{
			pItemUsedResult->bErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
			return FALSE;
		}
	}

	memcpy(&pItemUsedResult->cItem, pItemNative, sizeof(CItem));

	if(pItemUsedResult->bSrc == ITEM_NATIVE_INV_SMALL || pItemUsedResult->bSrc == ITEM_NATIVE_BELT)
	{
		// ������ 
		fPer = float(g_pBaseClassInfo[ pUser->GetClass() ].wPotion_Recovery +
			g_pBaseRaceInfo[ pUser->GetClass() ].wPotion_Recovery);

		wMax	= pBaseItemUsed->BaseItem_Supplies.wMax;
		wMin	= pBaseItemUsed->BaseItem_Supplies.wMin;

		dwRand	= ( rand() % ( wMax - wMin  + 1) ) + wMin;
		dwRand  = DWORD( (float)dwRand * (fPer / 100.f) );

		pUser->SetHP( WORD(pUser->GetHP() + ( pUser->GetMaxHP() * ( (float)dwRand  / 100.f ) ) ) );
	
		// �������� HP�����ְ� 
		DSTC_USER_STATUS UserStatus;
		UserStatus.bStatusMany			= 1;
		UserStatus.pStatus[0].dwCode	= USER_HP;
		UserStatus.pStatus[0].dwMin		= pUser->GetHP();
		NetSendToUser( dwConnectionIndex, (char*)&UserStatus, UserStatus.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

		// ���໡�� �׼� �ض�
		DSTC_DUNGEON_USED_POTION packet;
		packet.dwUserIndex	= pUser->GetID();
		packet.bType		= cItem.GetID() - 5000;
		pLayer->BroadCastSectionMsg((char*)&packet, packet.GetPacketSize(), pUser->GetPrevSectionNum());
	}
	else if(pItemUsedResult->bSrc == ITEM_NATIVE_GUARDIAN)
	{
		// �����
		if( !pUser->GetGuardian(0) )
		{
			pItemUsedResult->bErrorCode = UN_SUMMON_GURADIAN;
			return FALSE;
		}

		fPer = float(g_pBaseClassInfo[ pUser->GetClass() ].wPotion_Recovery +
			g_pBaseRaceInfo[ pUser->GetClass() ].wPotion_Recovery);

		wMax	= pBaseItemUsed->BaseItem_Supplies.wMax;
		wMin	= pBaseItemUsed->BaseItem_Supplies.wMin;
		dwRand	= ( rand() % ( wMax - wMin + 1 ) ) + wMin;
		dwRand  = DWORD( (float)dwRand * (fPer / 100.f) );
		
		CMonster* pGuardian = pUser->GetGuardian(0);
		if (pGuardian)
		{
			pGuardian->SetHP(DWORD(pGuardian->GetHP() +
				( pGuardian->GetMaxHP() * ( (float)dwRand  / 100.f ) ) ));
		
			// ����� HP�� Ŭ���̾�Ʈ�� �����ش�.. ��.-;;
			if(pGuardian->GetHP() > pGuardian->GetMaxHP())
			{
				pGuardian->SetHP(pGuardian->GetMaxHP());
			}

			pItemUsedResult->dwCommonId = pGuardian->GetHP();
		}
	}

	return TRUE;
}


//========================
// MP
//========================
BOOL CItemUsedManager::Value_ItemUsedNative_MP(DWORD dwConnectionIndex
											   , CUser* const pUser 
											   , CDungeonLayer* const pLayer
											   , const CItem* pItemNative
											   , ITEM_NATIVE_INFO* pItemNativeInfo_Used
											   , DSTC_ITEM_USED_NATIVE* const pItemUsedResult)
{
	float						fPer			= NULL;
	CBaseItem*					pBaseItemUsed	= NULL;
	DWORD						dwRand			= NULL;
	WORD						wMax			= NULL;
	WORD						wMin			= NULL;
	DSTC_USER_STATUS			UserStatus;

	CItem cItem;
	memcpy(&cItem, pItemNative, sizeof(CItem));
	pBaseItemUsed = cItem.GetBaseItem();

	cItem.SetQuantity( cItem.GetQuantity() - 1 );

	if( cItem.GetQuantity() == 0 )
	{
		if( !RemoveItemNative(pItemNativeInfo_Used) )
		{
			pItemUsedResult->bErrorCode = REMOVE_ITEMNATIVE_FAIL;
			return FALSE;
		}
	}
	else
	{
		if( !SetSlotItemNative(pLayer, &cItem, pItemNativeInfo_Used) )
		{
			pItemUsedResult->bErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
			return FALSE;
		}
	}

	memcpy(&pItemUsedResult->cItem, pItemNative, sizeof(CItem));

	fPer = float(g_pBaseClassInfo[ pUser->GetClass() ].wPotion_Recovery +
		g_pBaseRaceInfo[ pUser->GetClass() ].wPotion_Recovery);

	wMax	= pBaseItemUsed->BaseItem_Supplies.wMax;
	wMin	= pBaseItemUsed->BaseItem_Supplies.wMin;

	dwRand	= ( rand() % (wMax - wMin + 1) ) + wMin;
	dwRand	= DWORD( (float)dwRand * (fPer / 100.f ) );

	pUser->SetSP( WORD(pUser->GetSP() + ( pUser->GetMaxSP() * ( (float)dwRand / 100.f ) )) );
	
	UserStatus.bStatusMany			= 1;
	UserStatus.pStatus[0].dwCode	= USER_MP;
	UserStatus.pStatus[0].dwMin		= pUser->GetSP();

	NetSendToUser( dwConnectionIndex, (char*)&UserStatus, UserStatus.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

	return TRUE;
}


//========================
// HP Point
//========================
BOOL CItemUsedManager::Value_ItemUsedNative_HP_Point(DWORD dwConnectionIndex
													 , CUser* const pUser 
													 , CDungeonLayer* const pLayer
													 , const CItem* pItemNative
													 , ITEM_NATIVE_INFO* pItemNativeInfo_Used
													 , DSTC_ITEM_USED_NATIVE* const pItemUsedResult)
{
	float						fPer			= NULL;
	CBaseItem*					pBaseItemUsed	= NULL;
	DWORD						dwRand			= NULL;
	WORD						wMax			= NULL;
	WORD						wMin			= NULL;
			
	CItem cItem;
	memcpy(&cItem, pItemNative, sizeof(CItem));
	pBaseItemUsed = cItem.GetBaseItem();
	
	cItem.SetQuantity( cItem.GetQuantity() - 1 );

	if( cItem.GetQuantity() == 0 )
	{
		if( !RemoveItemNative(pItemNativeInfo_Used) )
		{
			pItemUsedResult->bErrorCode = REMOVE_ITEMNATIVE_FAIL;
			return FALSE;
		}
	}
	else
	{
		if( !SetSlotItemNative(pLayer, &cItem, pItemNativeInfo_Used) )
		{
			pItemUsedResult->bErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
			return FALSE;
		}
	}

	memcpy(&pItemUsedResult->cItem, pItemNative, sizeof(CItem));

	if(pItemUsedResult->bSrc == ITEM_NATIVE_INV_SMALL || pItemUsedResult->bSrc == ITEM_NATIVE_BELT)
	{
		// ������ 
		fPer = float(g_pBaseClassInfo[ pUser->GetClass() ].wPotion_Recovery +
			g_pBaseRaceInfo[ pUser->GetClass() ].wPotion_Recovery);

		wMax	= pBaseItemUsed->BaseItem_Supplies.wMax;
		wMin	= pBaseItemUsed->BaseItem_Supplies.wMin;
		dwRand	= ( rand() % ( wMax - wMin + 1 ) ) + wMin;

		pUser->SetHP( WORD(pUser->GetHP() + ( dwRand * ( fPer / 100.f ) ) ) );

		if( pUser->GetHP() > pUser->GetMaxHP() )
		{
			pUser->SetHP( (WORD)pUser->GetMaxHP() );
		}

		// �������� HP�����ְ� 
		DSTC_USER_STATUS UserStatus;
		UserStatus.bStatusMany			= 1;
		UserStatus.pStatus[0].dwCode	= USER_HP;
		UserStatus.pStatus[0].dwMin		= pUser->GetHP();
		NetSendToUser( dwConnectionIndex, (char*)&UserStatus, UserStatus.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

		// ���໡�� �׼� �ض�
		DSTC_DUNGEON_USED_POTION	packet;
		packet.dwUserIndex	= pUser->GetID();
		packet.bType		= pItemNative->GetID() - 5000;
		pLayer->BroadCastSectionMsg((char*)&packet, packet.GetPacketSize(), pUser->GetPrevSectionNum());
	}
	else if(pItemUsedResult->bSrc == ITEM_NATIVE_GUARDIAN)
	{
		// �����
		CMonster* pGuardian = pUser->GetGuardian(0);
		if( !pGuardian )
		{
			pItemUsedResult->bErrorCode = UN_SUMMON_GURADIAN;
			return FALSE;
		}

		fPer = float(g_pBaseClassInfo[ pUser->GetClass() ].wPotion_Recovery +
			g_pBaseRaceInfo[ pUser->GetClass() ].wPotion_Recovery);

		wMax	= pBaseItemUsed->BaseItem_Supplies.wMax;
		wMin	= pBaseItemUsed->BaseItem_Supplies.wMin;
		dwRand	= ( rand() % ( wMax - wMin + 1 ) ) + wMin;

		pGuardian->SetHP( DWORD(pGuardian->GetHP() + ( dwRand * ( fPer / 100.f ) ) ));

		if(pGuardian->GetHP() > pGuardian->GetMaxHP())
		{
			pGuardian->SetHP(pGuardian->GetMaxHP());
		}
		
		// ����� HP�� Ŭ���̾�Ʈ�� �����ش�.. ��.-;;
		pItemUsedResult->dwCommonId = pGuardian->GetHP();
	}

	return TRUE;
}


//========================
// SP,HP Point
//========================
BOOL CItemUsedManager::Value_ItemUsedNative_SP_HP_Point(DWORD dwConnectionIndex
														, CUser* const pUser 
														, CDungeonLayer* const pLayer
														, const CItem* pItemNative
														, ITEM_NATIVE_INFO* pItemNativeInfo_Used
														, DSTC_ITEM_USED_NATIVE* const pItemUsedResult)
{
	float						fPer			= NULL;
	CBaseItem*					pBaseItemUsed	= NULL;
	DWORD						dwRand			= NULL;
	WORD						wMax			= NULL;
	WORD						wMin			= NULL;
	DSTC_USER_STATUS			UserStatus;

	CItem cItem;
	memcpy(&cItem, pItemNative, sizeof(CItem));
	pBaseItemUsed = cItem.GetBaseItem();

	cItem.SetQuantity( cItem.GetQuantity() - 1 );

	if( cItem.GetQuantity() == 0 )
	{
		if( !RemoveItemNative(pItemNativeInfo_Used) )
		{
			pItemUsedResult->bErrorCode = REMOVE_ITEMNATIVE_FAIL;
			return FALSE;
		}
	}
	else
	{
		if( !SetSlotItemNative(pLayer, &cItem, pItemNativeInfo_Used) )
		{
			pItemUsedResult->bErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
			return FALSE;
		}
	}

	memcpy(&pItemUsedResult->cItem, pItemNative, sizeof(CItem));

	fPer = float(g_pBaseClassInfo[ pUser->GetClass() ].wPotion_Recovery +
		g_pBaseRaceInfo[ pUser->GetClass() ].wPotion_Recovery);

	wMax	= pBaseItemUsed->BaseItem_Supplies.wMax;
	wMin	= pBaseItemUsed->BaseItem_Supplies.wMin;
	dwRand	= ( rand() % ( wMax - wMin + 1 ) ) + wMin;

	pUser->SetHP( WORD(pUser->GetHP() + ( dwRand * ( fPer / 100.f ) ) ) );

	if( pUser->GetHP() > pUser->GetMaxHP() ) 
	{
		pUser->SetHP( (WORD)pUser->GetMaxHP() );
	}
	
	UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_HP;
	UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= pUser->GetHP();

	dwRand = ( rand() % ( wMax - wMin + 1) ) + wMin;

	pUser->SetSP( WORD(pUser->GetSP() + ( dwRand * ( fPer / 100.f ) ) ));

	if( pUser->GetSP() > pUser->GetMaxSP()) 
	{
		pUser->SetSP( (WORD)pUser->GetMaxSP() );
	}

	UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_MP;
	UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= pUser->GetSP();
	NetSendToUser( dwConnectionIndex, (char*)&UserStatus, UserStatus.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

	return TRUE;
}


//========================
// ����� ��ȭ �ð� ���� 
//========================
BOOL CItemUsedManager::Value_ItemUsedNative_HatchTimeShortening(DWORD dwConnectionIndex
																, CUser* const pUser 
																, CDungeonLayer* const pLayer
																, const CItem* pItemNative
																, ITEM_NATIVE_INFO* pItemNativeInfo_Used
																, DSTC_ITEM_USED_NATIVE* const pItemUsedResult)
{
	CItem*				pItemGuardian		= NULL;
	CBaseItem*			pBaseItemUsed		= NULL;
	CBaseItem*			pBaseItemGuardian	= NULL;
	ITEM_NATIVE_INFO	ItemNativeInfo;
	DWORD				dwCompleteTime		= NULL;
	DWORD				dwRand				= NULL;
	WORD				wMax				= NULL;
	WORD				wMin				= NULL;
	
	if( !pUser->IsSummonGuardian(0) )
	{
		// �˺�ȭ ���� ���� �ƴ��ݾ�.
		pItemUsedResult->bErrorCode = UN_SUMMON_GURADIANEGG;
		return FALSE;
	}

	CItem cItem;
	memcpy(&cItem, pItemNative, sizeof(CItem));
	pBaseItemUsed = cItem.GetBaseItem();

	// ����� ������ 
	ItemNativeInfo.eSlotID		= ITEM_NATIVE_INV_GUARDIAN;
	ItemNativeInfo.dwOwnerID	= pUser->GetID();
	ItemNativeInfo.bySlotIndex	= 0;

	pItemGuardian = (CItem*)GetItemNative(pLayer, &ItemNativeInfo);

	if(NULL == pItemGuardian)
	{
		pItemUsedResult->bErrorCode = CANNOT_FIND_GUARDIANITEM;
		return FALSE;
	}
	else if( GetItemKind(pItemGuardian->GetID() != ITEM_KIND_GUARDIAN) )
	{
		pItemUsedResult->bErrorCode = CANNOT_FIND_GUARDIANITEM;
		return FALSE;
	}

	pBaseItemGuardian = pItemGuardian->GetBaseItem();

	if(GUARDIAN_ITEM_TYPE_EGG == pBaseItemGuardian->BaseItem_Guardian.bType)
	{
		if( ITEM_USED_NATIVE_VALUE_HATCH_TIME_SHORTENING == pBaseItemUsed->BaseItem_Supplies.bType )
		{
			// ������ ���� 
			BYTE byCnt = cItem.GetQuantity();

			if(byCnt > 1)
			{
				cItem.SetQuantity(byCnt - 1);

				if( !SetSlotItemNative(pLayer, &cItem, pItemNativeInfo_Used) )
				{
					pItemUsedResult->bErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
					return FALSE;
				}
			}
			else
			{
				if( !RemoveItemNative(pItemNativeInfo_Used) )
				{
					pItemUsedResult->bErrorCode = REMOVE_ITEMNATIVE_FAIL;
					return FALSE;
				}
			}

			memcpy(&pItemUsedResult->cItem, pItemNative, sizeof(CItem));

			wMax			= pBaseItemUsed->BaseItem_Supplies.wMax;
			wMin			= pBaseItemUsed->BaseItem_Supplies.wMin;
			dwRand			= ( rand() % ( wMax - wMin + 1 ) ) + wMin;
			dwCompleteTime	= pItemGuardian->m_Item_Guardian.dwCompleteTime + (WORD)dwRand;
					
			pItemGuardian->SetGuardianEggCompleteTime(dwCompleteTime);

			// ����� ������ ������Ʈ �Ѵ�.
			SendToUpdate_GuardianItem(dwConnectionIndex, pItemGuardian);			
		}
	}
	else
	{
		pItemUsedResult->bErrorCode = NOT_GUARDIAN_ITEM_TYPE_EGG;
		return FALSE;
	}
	
	return TRUE;
}


//========================
// ����� ������ 
//========================
BOOL CItemUsedManager::Value_ItemUsedNative_LifeExtension(DWORD dwConnectionIndex
														  , CUser* const pUser 
														  , CDungeonLayer* const pLayer
														  , const CItem* pItemNative
														  , ITEM_NATIVE_INFO* pItemNativeInfo_Used
														  , DSTC_ITEM_USED_NATIVE* const pItemUsedResult)
{
	// SendExchangeGuardianLife
	return TRUE;
}


//==============================
// ����� ī��(�ֵ���) ��ǥ ���� 
//==============================
BOOL CItemUsedManager::Value_ItemUsedNative_CoordinateSave(DWORD dwConnectionIndex
														   , CUser* const pUser 
														   , CDungeonLayer* const pLayer
														   , const CItem* pItemNative
														   , ITEM_NATIVE_INFO* pItemNativeInfo_Used
														   , DSTC_ITEM_USED_NATIVE* const pItemUsedResult)
{
	BOOL bPortalChk = FALSE;

	if( !pUser->GetCurDungeon()->GetDungeonDataEx()->IsDungeonOwner(pUser) )
	{

#if defined TAIWAN_LOCALIZING
		// �븸�� ��� ���� 0������ ����� ��ġ�� ����Ҽ��ִ�.	���� �� �ȵǰ�
		if( pUser->GetCurDungeon()->GetID() > 10 &&
			pUser->GetCurDungeon()->GetDungeonDataEx()->GetDungeonType() != DUNGEON_TYPE_EVENT)
#else
		if(pUser->GetCurDungeon()->GetID() >= 2000 && pUser->GetCurDungeon()->GetID() < 4000)
#endif
		{
			if(pUser->GetCurLayerIndex() == 0)
			{
				bPortalChk = TRUE;
			}
		}
	}
	else
	{
		bPortalChk = TRUE;
	}

	if( pUser->GetCurDungeon()->GetDungeonDataEx()->IsStadium() )// ����� Stadium�� �ֵ��̷� ��� ����: hwoarang 050202
	{
		bPortalChk = FALSE;
	}

	if(bPortalChk)
	{						
		CItem cItem;
		memcpy(&cItem, pItemNative, sizeof(CItem));

		cItem.m_Item_Zodiac.bType	= 1;
		cItem.m_Item_Zodiac.bLayer	= pUser->GetCurLayerIndex();
		cItem.m_Item_Zodiac.wMapId	= pUser->GetCurDungeonID();
		cItem.m_Item_Zodiac.wPosX	= (WORD)pUser->GetCurPosition()->x;
		cItem.m_Item_Zodiac.wPosZ	= (WORD)pUser->GetCurPosition()->y;

		if( SetSlotItemNative(NULL, &cItem, pItemNativeInfo_Used) )
		{
			memcpy(&pItemUsedResult->cItem, &cItem, sizeof(CItem));
		}
		else
		{
			pItemUsedResult->bErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
			return FALSE;
		}									
	}
	else
	{
		pItemUsedResult->bErrorCode = CANNOT_COORDINATESAVE;
		return FALSE;
	}
	
	return TRUE;
}


//=======================================
// ����� ī��(�ֵ���) ����� ��ǥ�� �̵� 
//=======================================
BOOL CItemUsedManager::Value_ItemUsedNative_Goto_Coordinate(DWORD dwConnectionIndex
															, CUser* const pUser 
														    , CDungeonLayer* const pLayer
														    , const CItem* pItemNative
														    , ITEM_NATIVE_INFO* pItemNativeInfo_Used
														    , DSTC_ITEM_USED_NATIVE* const pItemUsedResult)
{
	BOOL bPortalChk = FALSE;

	CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo( pItemNative->m_Item_Zodiac.wMapId );	

	if(pDungeon)
	{
		if(!pDungeon->GetDungeonDataEx()->IsDungeonOwner(pUser))
		{
#if defined TAIWAN_LOCALIZING
			// �븸�� ��� ���� 0������ ����� ��ġ�� ����Ҽ��ִ�.	���� �� �ȵǰ�
			if( pUser->GetCurDungeon()->GetID() > 10 &&
				pUser->GetCurDungeon()->GetDungeonDataEx()->GetDungeonType() != DUNGEON_TYPE_EVENT)
#else
			if(pDungeon->GetID() >= 2000 && pDungeon->GetID() < 4000)
#endif
			{
				if(pItemNative->m_Item_Zodiac.bLayer == 0)
				{
					bPortalChk = TRUE;
				}
			}
		}
		else
		{
			bPortalChk = TRUE;
		}
	}
	else
	{
		bPortalChk = TRUE;
	}

	if(bPortalChk)
	{
		// ���� ���� ���� 
		const CItem*		pItemNativeAquarius = NULL;
		ITEM_NATIVE_INFO	ItemNativeInfo_Aquarius;

		ItemNativeInfo_Aquarius.eSlotID		= ITEM_NATIVE_INV_SMALL;
		ItemNativeInfo_Aquarius.dwOwnerID	= pUser->GetID();
		ItemNativeInfo_Aquarius.bySlotIndex = pItemUsedResult->bCommonIndex;

		pItemNativeAquarius = GetItemNative(NULL, &ItemNativeInfo_Aquarius);

		CItem cItem;
		memcpy( &cItem, pItemNativeAquarius, sizeof(CItem) );
		
		cItem.SetQuantity( cItem.GetQuantity() - 1 );

		if( cItem.GetQuantity() == 0 )
		{
			if( !RemoveItemNative(&ItemNativeInfo_Aquarius) )
			{
				pItemUsedResult->bErrorCode = REMOVE_ITEMNATIVE_FAIL;
				return FALSE;	
			}			
		}
		else
		{
			if( !SetSlotItemNative(NULL, &cItem, &ItemNativeInfo_Aquarius) )
			{
				pItemUsedResult->bErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
				return FALSE;
			}			
		}

		memcpy(&pItemUsedResult->cItem, pItemNativeAquarius, sizeof(CItem));
		
		// ��Ż�� �غ� ����.
		GuardianSupplies_Cancel(pUser);
	}
	else
	{
		pItemUsedResult->bErrorCode = CANNOT_GOTO_COORDINATE;
		return FALSE;
	}

	return TRUE;
}


//=======================================
// ���� ��Ż ��ũ�� 
//=======================================
BOOL CItemUsedManager::Value_ItemUsedNative_TownPortalScroll(DWORD dwConnectionIndex
															 , CUser* const pUser 
															 , CDungeonLayer* const pLayer
														     , const CItem* pItemNative
														     , ITEM_NATIVE_INFO* pItemNativeInfo_Used
														     , DSTC_ITEM_USED_NATIVE* const pItemUsedResult)
{
	CItem cItem;
	memcpy( &cItem, pItemNative, sizeof(CItem) );

	SetPortal(&cItem);

	memcpy(&pItemUsedResult->cItem, &cItem, sizeof(CItem));

	cItem.SetQuantity( cItem.GetQuantity() - 1 );
	
	if( cItem.GetQuantity() == 0 )
	{
		if( !RemoveItemNative(pItemNativeInfo_Used) )
		{
			return FALSE;
		}
	}
	else
	{
		if( !SetSlotItemNative(NULL, &cItem, pItemNativeInfo_Used) )
		{
			pItemUsedResult->bErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
			return FALSE;
		}			
	}

	// ��Ż�� �غ� ����.
	GuardianSupplies_Cancel(pUser);

	return TRUE;
}


//=======================================
// ������ ���� 
//=======================================
BOOL CItemUsedManager::Value_ItemUsedNative_Insurance(DWORD dwConnectionIndex
													  , CUser* const pUser 
													  , CDungeonLayer* const pLayer
													  , const CItem* pItemNative
													  , ITEM_NATIVE_INFO* pItemNativeInfo_Used
													  , DSTC_ITEM_USED_NATIVE* const pItemUsedResult)
{
	CItem*				pAttachItem			= NULL;
	CBaseItem*			pBaseItemUsed		= NULL;
	WORD				wItemID				= NULL;

	pAttachItem		= (CItem*)pItemNative;
	pBaseItemUsed	= pAttachItem->GetBaseItem();
	wItemID			= pAttachItem->GetID();

	if( !pUser->IsAvailabilityAttachItem(wItemID) )
	{
		pItemUsedResult->bErrorCode = THIS_ITEM_IS_OCCUPIABLE;
		return FALSE;
	}

	CItem cItem;
	memcpy(&cItem, pAttachItem,sizeof(CItem));
	cItem.m_Item_Zodiac.bInvIndex = pItemNativeInfo_Used->bySlotIndex + 1;

	if( !SetSlotItemNative(NULL, &cItem, pItemNativeInfo_Used) )
	{
		pItemUsedResult->bErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
		return FALSE;
	}

	pUser->AttachItem(pAttachItem);

	memcpy(&pItemUsedResult->cItem, pItemNative, sizeof(CItem));

	return TRUE;
}


//=======================================
// ���,Ŭ�� ���� 
//=======================================
BOOL CItemUsedManager::Value_ItemUsedNative_Guild_Clen_Make(DWORD dwConnectionIndex
															, CUser* const pUser 
															, CDungeonLayer* const pLayer
														    , const CItem* pItemNative
														    , ITEM_NATIVE_INFO* pItemNativeInfo_Used
														    , DSTC_ITEM_USED_NATIVE* const pItemUsedResult)
{
	CItem cItem;
	memcpy( &cItem, pItemNative, sizeof(CItem) );

	if(pUser->m_byGroupType != __GCTYPE_NONE__)
	{
		if(cItem.GetID() == __ITEM_GUILD_INDEX__)
		{
			pItemUsedResult->bErrorCode = ALREADY_GUILD_JOIN;
			return FALSE;
		}
		else if(cItem.GetID() == __ITEM_CLEN_INDEX__)
		{
			pItemUsedResult->bErrorCode = ALREADY_CLEN_JOIN;
			return FALSE;
		}
	}
	
	BYTE byCnt = cItem.GetQuantity();

	if(byCnt > 1)
	{
		cItem.SetQuantity(byCnt - 1);

		if( !SetSlotItemNative(pLayer, &cItem, pItemNativeInfo_Used) )
		{
			pItemUsedResult->bErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
			return FALSE;
		}
	}
	else
	{
		if( !RemoveItemNative(pItemNativeInfo_Used) )
		{
			pItemUsedResult->bErrorCode = REMOVE_ITEMNATIVE_FAIL;
			return FALSE;
		}
	}

	memcpy(&pItemUsedResult->cItem, pItemNative, sizeof(CItem));

	return TRUE;
}


//=======================================
// ��ų,���� �ʱ�ȭ  
//=======================================
BOOL CItemUsedManager::Value_ItemUsedNative_Status_Skill_Init(DWORD dwConnectionIndex
															  , CUser* const pUser 
															  , CDungeonLayer* const pLayer
														      , const CItem* pItemNative
														      , ITEM_NATIVE_INFO* pItemNativeInfo_Used
														      , DSTC_ITEM_USED_NATIVE* const pItemUsedResult)
{
	CBaseItem*	pBaseItemUsed	= NULL;
	BYTE		byCnt			= NULL;
	WORD		wId				= NULL;

	CItem cItem;
	memcpy( &cItem, pItemNative, sizeof(CItem) );

	pBaseItemUsed = cItem.GetBaseItem();

	if(pUser->GetLevel() < 10)
	{
		pItemUsedResult->bErrorCode = CANNOT_ENOUGH_LEVEL;
		return FALSE;
	}

	byCnt = cItem.GetQuantity();
	
	if(byCnt == 1)
	{
		if( !RemoveItemNative( pItemNativeInfo_Used ) )
		{
			return FALSE;
		}
	}
	else
	{
		cItem.SetQuantity( byCnt - 1 );

		if( !SetSlotItemNative( NULL, &cItem, pItemNativeInfo_Used) )
		{
			return FALSE;
		}
	}
	
	memcpy(&pItemUsedResult->cItem, pItemNative, sizeof(CItem));

	if( wId==__ITEM_STATUS_INT__ || (wId>=__ITEM_STATUS_INTSTART__ && wId<=__ITEM_STATUS_INTEND__) )
	{
		pUser->InitStat(pBaseItemUsed->BaseItem_Special.bClassGrade - 50);
	}
	else if(wId==__ITEM_SKILL_INT__ || (wId>=__ITEM_SKILL_INTSTART__ && wId<=__ITEM_SKILL_INTEND__))
	{
		pUser->InitSkill(pBaseItemUsed->BaseItem_Special.bClassGrade - 50);
	}

	return TRUE;
}


//=======================================================
// ������ ��ǥ(�±ر�) 
//=======================================================
BOOL CItemUsedManager::Value_ItemUsedNative_Voucher_Of_Hero(DWORD dwConnectionIndex
															, CUser* const pUser 
															, CDungeonLayer* const pLayer
														    , const CItem* pItemNative
														    , ITEM_NATIVE_INFO* pItemNativeInfo_Used
														    , DSTC_ITEM_USED_NATIVE* const pItemUsedResult)
{
	WORD				wItemQuantity		= NULL;
	WORD				wPresentID			= NULL;
	BOOL				bIsEmpty			= FALSE;
	DWORD				dwMaxCount			= 0;
	DWORD				dwNowConnt			= 0;
	const CItem*		pItemNativePresent	= NULL;
	char				szQuery[0xff]		= {0,};
	ITEM_EVENT*			pItemEvent			= NULL;	
	ITEM_NATIVE_INFO	ItemNativeInfo_Present;

	CItem cItem;
	memcpy(&cItem, pItemNative, sizeof(CItem));
		
	wPresentID = UnpackPresentBox(&cItem, &wItemQuantity);

	if(!wPresentID)	
	{
		// �������� Ǯ�� ����
		pItemUsedResult->bErrorCode = UNPACK_PRESENTBOX_FAIL;		
		return FALSE;
	}

	// �κ��� �� ������ ã�´�.
	if( GetItemType(wPresentID) == ITEM_TYPE_SMALL )
	{
		ItemNativeInfo_Present.eSlotID		= ITEM_NATIVE_INV_SMALL;
		ItemNativeInfo_Present.dwOwnerID	= pUser->GetID();
		ItemNativeInfo_Present.bySlotIndex	= 0;

		pItemNativePresent = GetItemNative(NULL, &ItemNativeInfo_Present);

		for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
		{
			if(pItemNativePresent[i].GetID() == 0)
			{
				bIsEmpty = TRUE;
				ItemNativeInfo_Present.bySlotIndex = i; 
				break;
			}
		}
	}
	else if( GetItemType(wPresentID) == ITEM_TYPE_LARGE )
	{
		ItemNativeInfo_Present.eSlotID		= ITEM_NATIVE_INV_LARGE;
		ItemNativeInfo_Present.dwOwnerID	= pUser->GetID();
		ItemNativeInfo_Present.bySlotIndex	= 0;

		pItemNativePresent = GetItemNative(NULL, &ItemNativeInfo_Present);

		for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
		{
			if(pItemNativePresent[i].GetID() == 0)
			{
				bIsEmpty = TRUE;
				ItemNativeInfo_Present.bySlotIndex = i; 
				break;
			}
		}
	}

	if(!bIsEmpty)
	{
		// �κ��丮�� ��ĭ�� ����
		pItemUsedResult->bErrorCode = QUITE_FULL_SLOT;		
		return FALSE;
	}

	pItemEvent = g_pItemEventHash->GetData(wPresentID);

	if(NULL == pItemEvent)
	{
		// ������ ������ ����..
		pItemUsedResult->bErrorCode = NOT_CONFORM_ITEM;
		return FALSE;
	}

	// �̺�Ʈ �������� ���޷��� �����ϰ� ������ 
#ifdef SUPPLY_EXCEED_USED 
	dwMaxCount = pItemEvent->dwmax_count;
	dwNowConnt = pItemEvent->dwnow_count;

	if(dwMaxCount <= dwNowConnt)
	{
		// �ִ� ���޷� �ʰ� 
		pItemUsedResult->bErrorCode = SUPPLY_EXCEED;
		return FALSE;
	}
#endif

	// �� ������� ������ ��������.
	CItem cPresentItem;	
	memset(&cPresentItem, NULL, sizeof(CItem));
	CreateItem(&cPresentItem, pItemEvent->witem_id, (BYTE)pItemEvent->dwitem_quantity);

	if( !SetSlotItemNative(pLayer, &cPresentItem, &ItemNativeInfo_Present) )
	{
		pItemUsedResult->bErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
		return FALSE;
	}

	memcpy(&pItemUsedResult->cItem, &cPresentItem, sizeof(CItem));
	pItemUsedResult->bCommonIndex = ItemNativeInfo_Present.bySlotIndex;

	// ������ ��ǥ ������ �����
	BYTE byCnt = cItem.GetQuantity();

	if(byCnt > 1)
	{
		cItem.SetQuantity(byCnt - 1);
		SetSlotItemNative(pLayer, &cItem, pItemNativeInfo_Used);
	}
	else
	{
		RemoveItemNative(pItemNativeInfo_Used);
	}

	// �̺�Ʈ �������� ���޷��� �����ϰ� ������ 
#ifdef SUPPLY_EXCEED_USED
	pItemEvent->dwnow_count += 1;

	wsprintf(szQuery, " Update item_event_maxcount set now_count = %d where item_id = %d"
		, pItemEvent->dwnow_count, pItemEvent->witem_id);
	g_pDb->THExecuteSQL(szQuery, FALSE, NULL, NULL, (BYTE)GAME_DB);
#endif

	return TRUE;
}


//=======================================================
// �۽��� ���� 
//=======================================================
BOOL CItemUsedManager::Value_ItemUsedNative_PersonalRecall(DWORD dwConnectionIndex
														   , CUser* const pUser 
														   , CDungeonLayer* const pLayer
														   , const CItem* pItemNative
														   , ITEM_NATIVE_INFO* pItemNativeInfo_Used
														   , DSTC_ITEM_USED_NATIVE* const pItemUsedResult)
{
	if( !pUser->GetPartyID() )
	{
		// ��Ƽ���� �ƴϾ�.
		pItemUsedResult->bErrorCode = IS_NOT_PARTY;
		return FALSE;
	}
	else if( !pUser->GetPartyLeader() )
	{
		// ��Ƽ�������� �Ҽ� �ֽ�!
		pItemUsedResult->bErrorCode = IS_NOT_PARTY_LEADER;
		return FALSE;
	}
	else if( pUser->IsCurDungeonSiege() && pUser->GetAttackMode() != ATTACK_MODE_DEFENCE )
	{
		// �������϶��� ����ڸ� ��Ƽ �۽��� ���� �����ϴ�.
		pItemUsedResult->bErrorCode = UNIT_STATUS_IN_SIEGE;
		return FALSE;
	}
	else if( pUser->IsCurDungeonEventInLive() )
	{
		// �̺�Ʈ ���������� ����Ҽ� ����.
		pItemUsedResult->bErrorCode = CUR_DUNGEON_EVENT_INLIVE;
		return FALSE;
	}
	else if( pUser->GetID() == pItemUsedResult->dwPortalUserIndex )
	{
		// �ڱ� �ڽſ��Դ� ����Ҽ� ����.
		pItemUsedResult->bErrorCode = CANNOT_MYSELF_USED;
		return FALSE;
	}

	WSTDS_PARTY_PORTAL pSendPacket;

	__lstrcpyn(pSendPacket.szName, (LPSTR)pUser->GetName(), MAX_CHARACTER_REAL_LENGTH + 1);
	pSendPacket.dwUserIndex			= pUser->GetID();
	pSendPacket.byIndex				= pItemNativeInfo_Used->bySlotIndex;
	pSendPacket.dwPartyId			= pItemUsedResult->dwCommonId;
	pSendPacket.dwPartalUserIndex	= pItemUsedResult->dwPortalUserIndex;
	
	g_pNet->SendToServer(WSINDEX, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	
	return TRUE;
}


//=======================================================
// ��Ƽ ���� ����
//=======================================================
BOOL CItemUsedManager::Value_ItemUsedNative_AllPartyRecall(DWORD dwConnectionIndex
														   , CUser* const pUser 
														   , CDungeonLayer* const pLayer
														   , const CItem* pItemNative
														   , ITEM_NATIVE_INFO* pItemNativeInfo_Used
														   , DSTC_ITEM_USED_NATIVE* const pItemUsedResult)
{
	if( !pUser->GetPartyID() )
	{
		// ��Ƽ���� �ƴϾ�.
		pItemUsedResult->bErrorCode = IS_NOT_PARTY;
		return FALSE;
	}
	else if( !pUser->GetPartyLeader() )
	{
		// ��Ƽ�������� �Ҽ� �ֽ�!
		pItemUsedResult->bErrorCode = IS_NOT_PARTY_LEADER;
		return FALSE;
	}
	else if( pUser->IsCurDungeonSiege() && pUser->GetAttackMode() != ATTACK_MODE_DEFENCE )
	{
		// �������϶��� ����ڸ� ��Ƽ ���� ���ݰ����ϴ�.
		pItemUsedResult->bErrorCode = UNIT_STATUS_IN_SIEGE;
		return FALSE;
	}
	else if( pUser->IsCurDungeonEventInLive() )
	{
		// �̺�Ʈ ���������� ����Ҽ� ����.
		pItemUsedResult->bErrorCode = CUR_DUNGEON_EVENT_INLIVE;
		return FALSE;
	}

	CItem cItem;
	memcpy(&cItem, pItemNative, sizeof(CItem));

	BYTE byCnt = cItem.GetQuantity();
	byCnt -= 1;

	if(byCnt == 0)
	{
		if( !RemoveItemNative(pItemNativeInfo_Used) )
		{
			pItemUsedResult->bErrorCode = REMOVE_ITEMNATIVE_FAIL;
			return FALSE;
		}
	}
	else if(byCnt > 0 )
	{
		cItem.SetQuantity( byCnt );

		if( !SetSlotItemNative(pLayer, &cItem, pItemNativeInfo_Used) )
		{
			pItemUsedResult->bErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
			return FALSE;
		}
	}

	memcpy(&pItemUsedResult->cItem, pItemNative, sizeof(CItem));
	
	WSTDS_PARTY_ALLPORTAL pSendPacket;
	pSendPacket.dwPartyId	= pUser->m_dwPartyId;
	pSendPacket.dwUserIndex	= pUser->GetID();
	pSendPacket.byIndex		= pItemNativeInfo_Used->bySlotIndex;

	g_pNet->SendToServer(WSINDEX, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	
	return TRUE;
}


//=======================================================
// ��� ���� 
//=======================================================
BOOL CItemUsedManager::Value_ItemUsedNative_Guild_Recall(DWORD dwConnectionIndex
														 , CUser* const pUser 
														 , CDungeonLayer* const pLayer
														 , const CItem* pItemNative
														 , ITEM_NATIVE_INFO* pItemNativeInfo_Used
														 , DSTC_ITEM_USED_NATIVE* const pItemUsedResult)
{
	if( pUser->IsCurDungeonSiege() )
	{
		// �������϶��� ������..
		pItemUsedResult->bErrorCode = UNIT_STATUS_IN_SIEGE;
		return FALSE;
	}
	else if( pUser->IsCurDungeonEventInLive() )
	{
		// �̺�Ʈ ���������� ����Ҽ� ����.
		pItemUsedResult->bErrorCode = CUR_DUNGEON_EVENT_INLIVE;
		return FALSE;
	}

	CItem cItem;
	memcpy(&cItem, pItemNative, sizeof(CItem));

	BYTE byCnt = cItem.GetQuantity();
	byCnt -= 1;

	if(byCnt == 0)
	{
		if( !RemoveItemNative(pItemNativeInfo_Used) )
		{
			pItemUsedResult->bErrorCode = REMOVE_ITEMNATIVE_FAIL;
			return FALSE;
		}
	}
	else if(byCnt > 0 )
	{
		cItem.SetQuantity( byCnt );

		if( !SetSlotItemNative(pLayer, &cItem, pItemNativeInfo_Used) )
		{
			pItemUsedResult->bErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
			return FALSE;
		}
	}

	WSTDS_GUILD_ALLPORTAL pSendPacket;						
	pSendPacket.dwGuildId	= pUser->m_dwGuildId;						
	pSendPacket.dwUserIndex	= pUser->GetID();
	pSendPacket.byIndex		= pItemUsedResult->bSrcIndex;
	
	g_pNet->SendToServer(WSINDEX, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

	return TRUE;
}


//=======================================================
// ��Ƽ ������ ��� 
//=======================================================
BOOL CItemUsedManager::Value_ItemUsedNative_MagicArrayToParty(DWORD dwConnectionIndex
															  , CUser* const pUser 
															  , CDungeonLayer* const pLayer
														      , const CItem* pItemNative
														      , ITEM_NATIVE_INFO* pItemNativeInfo_Used
														      , DSTC_ITEM_USED_NATIVE* const pItemUsedResult)
{
	if( !pUser->GetPartyID() )
	{
		// ��Ƽ���� �ƴϾ�.
		pItemUsedResult->bErrorCode = IS_NOT_PARTY;
		return FALSE;
	}
	else if( !pUser->GetPartyLeader() )
	{
		// ��Ƽ�������� �Ҽ� �ֽ�!
		pItemUsedResult->bErrorCode = IS_NOT_PARTY_LEADER;
		return FALSE;
	}
	else if(pUser->GetCurDungeon()->GetDungeonDataEx()->IsVillage())
	{
		// ���� ���ݾ�. ��.-;;
		pItemUsedResult->bErrorCode = IN_VILLAGE;
		return FALSE;				
	}

	if(pUser->m_pUsingMagicArrayList->GetCount() == 0)
	{
		CItem cItem;
		memcpy(&cItem, pItemNative, sizeof(CItem));

		BYTE byCnt = cItem.GetQuantity();

		if(byCnt > 1)
		{
			cItem.SetQuantity(byCnt - 1);

			if( !SetSlotItemNative(pLayer, &cItem, pItemNativeInfo_Used) )
			{
				pItemUsedResult->bErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
				return FALSE;
			}
		}
		else
		{
			if( !RemoveItemNative(pItemNativeInfo_Used) )
			{
				pItemUsedResult->bErrorCode = REMOVE_ITEMNATIVE_FAIL;
				return FALSE;
			}
		}

		pLayer->AttachMagicArrayToParty( pUser, cItem.GetID() );										
	}
	else
	{
		pItemUsedResult->bErrorCode = MAGICARRAY_TO_PARTY_OCCUPIABLE;
		return FALSE;
	}
			
	return TRUE;
}






