#pragma once
#include "ItemInfoView.h"
#include <vector>

extern class CItem;

class ItemPickupFiltering {
public:
	ItemPickupFiltering();
	static ItemPickupFiltering* sharedInstance();

	void render();
	bool handleKeyUp(WPARAM wparam, LPARAM lparam);
	bool handleKeyDown(WPARAM wparam, LPARAM lparam);
	bool handleMouseDown();
	bool handleMouseUp();
	bool isInterfaceFocused();
private: 
	std::vector<CItem*> totalItemsList;
	bool _isInterfaceFocused;
};

