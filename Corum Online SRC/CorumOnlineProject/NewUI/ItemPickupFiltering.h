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
		void handleKeyUp(WPARAM wparam, LPARAM lparam);
		void handleKeyDown(WPARAM wparam, LPARAM lparam);
		void handleMouseDown();
		void handleMouseUp();
		std::set<DWORD> currentSelectedIDs();

		void setViewActive(bool active);
		bool isViewActive();

		bool swallowsMouse();
		bool swallowsKeyboard();

		void offsetView(CustomUI::Point byOffsets);
	private:
		ItemFilteringView* _view;
		void itemFilteringViewDidUpdateSelection(ItemFilteringView*, std::set<DWORD> selectedItemIDs);
		std::vector<CItem*> totalItemsList;
	};
}
