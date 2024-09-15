#pragma once
#include "ItemInfoView.h"
#include <vector>

extern class CItem;

class ItemPickupFiltering {
public:
	ItemPickupFiltering();
	static ItemPickupFiltering* sharedInstance();

	void render();

private: 
	std::vector<CItem*> totalItemsList;
};

