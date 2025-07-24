#pragma once
#include "../ItemUsed.h"

class ItemUsageManager {
public:
	static ItemUsageManager* sharedInstance();
	ItemUsageManager();

	bool tryUseBeltItem(CItem item, int index);
	bool tryUseSmallInventoryItem(CItem item, int index);
};

