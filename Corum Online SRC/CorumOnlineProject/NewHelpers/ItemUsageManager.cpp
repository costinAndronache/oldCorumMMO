#include "../InitGame.h"
#include "ItemUsageManager.h"

ItemUsageManager::ItemUsageManager() {}

bool ItemUsageManager::tryUseBeltItem(CItem item, int index) {
	return
		ItemUsedSupplies(&item, index, ITEM_USE_FROM_BELT) ||
		ItemUsedConsumable(&item, index, ITEM_USE_FROM_BELT) ||
		ItemUsedZodiac(&item, index, ITEM_USE_FROM_BELT);
}

bool ItemUsageManager::tryUseSmallInventoryItem(CItem item, int index) {
	return
		ItemUsedSupplies(&item, index, ITEM_USE_FROM_SMALL_INVENTORY) ||
		ItemUsedConsumable(&item, index, ITEM_USE_FROM_SMALL_INVENTORY) ||
		ItemUsedSpecial(&item, index, ITEM_USE_FROM_SMALL_INVENTORY) ||
		ItemUsedMagicArray(&item, index, ITEM_USE_FROM_SMALL_INVENTORY) ||
		ItemUsedZodianInsurance(&item, index, ITEM_USE_FROM_SMALL_INVENTORY) ||
		ItemUsedZodiac(&item, index, ITEM_USE_FROM_SMALL_INVENTORY);
}