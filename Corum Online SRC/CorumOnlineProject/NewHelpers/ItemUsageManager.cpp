#include "../InitGame.h"
#include "ItemUsageManager.h"


ItemUsageManager* ItemUsageManager::sharedInstance() {
	static ItemUsageManager* _shared = nullptr;
	if (!_shared) {
		_shared = new ItemUsageManager();
	}

	return _shared;
}

ItemUsageManager::ItemUsageManager() {}

bool ItemUsageManager::tryUseBeltItem(CItem item, int index) {
	return
		ItemUsedSupplies(&item, index, ITEM_USE_FROM_SMALL_INVENTORY) ||
		ItemUsedConsumable(&item, index, ITEM_USE_FROM_SMALL_INVENTORY) ||
		ItemUsedZodiac(&item, index, ITEM_USE_FROM_SMALL_INVENTORY);
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