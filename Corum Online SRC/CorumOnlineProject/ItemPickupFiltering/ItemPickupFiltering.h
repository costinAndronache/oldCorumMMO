#pragma once
#include "ItemFilteringView.h"
#include "ItemPickupFilteringBase.h"
#include <vector>
#include <set>

extern class CItem;
namespace ItemPickupFiltering {
	class ItemPickupFilteringSystem : private ItemFilteringViewClient {
	public:
		ItemPickupFilteringSystem();
		static ItemPickupFilteringSystem* sharedInstance();

		void render();
		bool handleKeyUp(WPARAM wparam, LPARAM lparam);
		bool handleKeyDown(WPARAM wparam, LPARAM lparam);
		bool handleMouseDown();
		bool handleMouseUp();
		bool isInterfaceFocused();
		std::set<DWORD> currentSelectedIDs();

		void setViewActive(bool active);
		bool isViewActive();
	private:
		ItemFilteringView* _view;
		void itemFilteringViewDidUpdateSelection(ItemFilteringView*, std::set<DWORD> selectedItemIDs);
		std::vector<CItem*> totalItemsList;
		bool _isInterfaceFocused;
	};
}
