#pragma once


class CItem;
struct DSTC_ITEM_MOVE;
enum ITEM_NATIVE;


enum ITEM_NATIVE_TYPE
{
	ITEM_NATIVE_TYPE_NONE		= 0,
	ITEM_NATIVE_TYPE_REAL		= 1,
	ITEM_NATIVE_TYPE_VIRTUAL	= 2,		
};


enum ITEM_USED_NATIVE_TYPE
{
	ITEM_USED_NATIVE_TYPE_START			= 0,
	ITEM_USED_NATIVE_TYPE_ZODIAC		= 1,
	ITEM_USED_NATIVE_TYPE_SUPPLIES,
	ITEM_USED_NATIVE_TYPE_SPECIAL,
	ITEM_USED_NATIVE_TYPE_MAGICARRAY,
	ITEM_USED_NATIVE_TYPE_CONSUMABLE,
	ITEM_USED_NATIVE_TYPE_END,
};


enum ITEM_USED_NATIVE_VALUE
{
	ITEM_USED_NATIVE_VALUE_ZODIAC_START		= 0,
	ITEM_USED_NATIVE_VALUE_COORDINATE_SAVE	= 1,
	ITEM_USED_NATIVE_VALUE_GOTO_COORDINATE,
	ITEM_USED_NATIVE_VALUE_TOWNPORTALSCROLL,
	ITEM_USED_NATIVE_VALUE_INSURANCE,
	ITEM_USED_NATIVE_VALUE_ZODIAC_END,

	ITEM_USED_NATIVE_VALUE_SUPPLIES_START	= 10,
	ITEM_USED_NATIVE_VALUE_HP				= 11,
	ITEM_USED_NATIVE_VALUE_MP,
	ITEM_USED_NATIVE_VALUE_HP_POINT,
	ITEM_USED_NATIVE_VALUE_SP_HP_POINT,
	ITEM_USED_NATIVE_VALUE_HATCH_TIME_SHORTENING,
	ITEM_USED_NATIVE_VALUE_SUPPLIES_END,

	ITEM_USED_NATIVE_VALUE_SPECIAL_START	= 20,
	ITEM_USED_NATIVE_VALUE_GUILD_CLEN_MAKE	= 21,
	ITEM_USED_NATIVE_VALUE_STATUS_SKILL_INIT,
	ITEM_USED_NATIVE_VALUE_VOUCHER_OF_HERO,
	ITEM_USED_NATIVE_VALUE_SPECIAL_END,

	ITEM_USED_NATIVE_VALUE_MAGICARRAY_START	= 30,
	ITEM_USED_NATIVE_VALUE_PERSONAL_RECALL	= 31,
	ITEM_USED_NATIVE_VALUE_ALL_PARTY_RECALL,
	ITEM_USED_NATIVE_VALUE_GUILD_RECALL,
	ITEM_USED_NATIVE_VALUE_MAGICARRAY_TO_PARTY,
	ITEM_USED_NATIVE_VALUE_MAGICARRAY_END,

	ITEM_USED_NATIVE_VALUE_CONSUMABLE_START	= 40,
	ITEM_USED_NATIVE_VALUE_CONSUMABLE_END,
};


struct ITEM_NATIVE_INFO
{
	ITEM_NATIVE eSlotID;
	union
	{
		DWORD dwOwnerID;
		INT64 i64ItemID;
	};
	union
	{
		BYTE bySlotIndex;
		BYTE bySectionNum;
	};
	union 
	{
		BYTE byItemQuantityCnt;
		DWORD dwMoney;
	};	
};


BOOL			IsItemDungeonManager(ITEM_NATIVE eItemNative);
BOOL			IsItemNativeTile(ITEM_NATIVE eItemNative);
BOOL			IsItemNativePlayer(ITEM_NATIVE eItemNative);
BOOL			IsItemNativeTraderMonster(ITEM_NATIVE eItemNative);
BOOL			IsItemNativeSrcRemove(ITEM_NATIVE eItemNativeDest);
BOOL			IsItemNativeInvLargeAndSmall(ITEM_NATIVE eItemNative);
BYTE			GetItemNativePlayerBufferCount();
BYTE			GetItemNativeTileBufferCount();
BYTE			GetItemNativeTraderMonsterCount();
ITEM_NATIVE_TYPE GetItemNativeType(ITEM_NATIVE eItemNative);
void			PickupItem(DSTC_ITEM_MOVE* const pItemMove,const ITEM_NATIVE_INFO* pItemNativeInfo_Src, const ITEM_NATIVE_INFO* pItemNativeInfo_Dest);
void			DropItem(DSTC_ITEM_MOVE* const pItemMove, const ITEM_NATIVE_INFO* pItemNativeInfo_Src, BYTE byQuantityCount);
BOOL			SetSlotItemNative(const CItem* pItem_Src, const ITEM_NATIVE_INFO* pItemNativeInfo_Dest);
const CItem*	GetItemNative(const ITEM_NATIVE_INFO* pItemNativeInfo);
void			RemoveItemNative(const ITEM_NATIVE_INFO* pItemNativeInfo);
BOOL			MoveItemNative(DSTC_ITEM_MOVE* const pItemMove, const ITEM_NATIVE_INFO* pItemNativeInfo_Src, const ITEM_NATIVE_INFO* pItemNativeInfo_Dest, BYTE byQuantityCount);
BOOL			OverlapItemNative(DSTC_ITEM_MOVE* const pItemMove, const ITEM_NATIVE_INFO* pItemNativeInfo_Src, const ITEM_NATIVE_INFO* pItemNativeInfo_Dest, BYTE byQuantityCount);
BOOL			ExchangeItemNative(const ITEM_NATIVE_INFO* pItemNativeInfo_Src, const ITEM_NATIVE_INFO* pItemNativeInfo_Dest);
BOOL			PutOnItemNative(const ITEM_NATIVE_INFO* pItemNativeInfo_Src, const ITEM_NATIVE_INFO* pItemNativeInfo_Dest);
void			AllLinkSrcToDestNative(ITEM_NATIVE_INFO* pItemNativeInfo_Src, ITEM_NATIVE_INFO* pItemNativeInfo_Dest);