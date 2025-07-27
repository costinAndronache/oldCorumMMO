#pragma once

class CItem;

enum ITEM_USE_SOURCE {
 ITEM_USE_FROM_SMALL_INVENTORY = 0,
 ITEM_USE_FROM_BELT = 1
};


BOOL ItemUsedZodiac(CItem* pItem, BYTE indexInSource, ITEM_USE_SOURCE source);
BOOL ItemUsedSupplies(CItem* pItem, BYTE indexInSource, ITEM_USE_SOURCE source);
BOOL ItemUsedConsumable(CItem* pItem, BYTE indexInSource, ITEM_USE_SOURCE source);
BOOL ItemUsedSpecial(CItem* pItem, BYTE indexInSource, ITEM_USE_SOURCE source);
BOOL ItemUsedZodianInsurance(CItem* pItem, BYTE indexInSource, ITEM_USE_SOURCE source);
BOOL ItemUsedPresentBox(CItem* pItem, BYTE indexInSource, ITEM_USE_SOURCE source);
BOOL ItemUsedMagicArray(CItem* pItem, BYTE indexInSource, ITEM_USE_SOURCE source);
BOOL IsSamePropertyConsumableItem(CItem* pSrc, CItem* pDst);