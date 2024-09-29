#pragma once
#include "ItemFilteringView.h"
#include <vector>
#include <set>

extern class CItem;

class ItemPickupFiltering: private ItemFilteringViewClient {
public:
	ItemPickupFiltering();
	static ItemPickupFiltering* sharedInstance();

	void render();
	bool handleKeyUp(WPARAM wparam, LPARAM lparam);
	bool handleKeyDown(WPARAM wparam, LPARAM lparam);
	bool handleMouseDown();
	bool handleMouseUp();
	bool isInterfaceFocused();
	std::set<DWORD> currentSelectedIDs();

	void openView();
private:
	ItemFilteringView* _view;
	void itemFilteringViewDidUpdateSelection(ItemFilteringView*, std::set<DWORD> selectedItemIDs);
	std::vector<CItem*> totalItemsList;
	bool _isInterfaceFocused;
};

