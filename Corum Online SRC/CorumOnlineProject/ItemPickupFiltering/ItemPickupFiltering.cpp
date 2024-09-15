#include "ItemPickupFiltering.h"
#include "../InitGame.h"
#include "../CorumResource.h"
#include "PagedItemViewTable.h"

using namespace CustomUI;

static PagedItemViewTable* table = nullptr;

std::vector<CItem*> getDebugItemList();

static PagedItemViewTable* debugView() {
	if (table == NULL) {
		auto items = getDebugItemList();
		Rect frame = { { 100, 100}, {340, 340} };
		table = new PagedItemViewTable(frame, items);
	}

	return table;
}

CItem* createItemWithBase(CBaseItem* baseItem) {
	CItem* item = new CItem;
	memset((void*)item, 0, sizeof(CItem));
	item->SetID(baseItem->wID);
	return item;
}

std::vector<CItem*> getDebugItemList() {
	std::vector<CItem*> result;

	auto head = g_pItemTableHash_get()->GetHead();

	while (head) {
		result.push_back(createItemWithBase(head->pData));
		switch (head->pData->dwCode_ID)
		{
		case ITEM_KIND_WEAPON:
			
			break;
		case ITEM_KIND_AROMR:
			//result.push_back(createItemWithBase(head->pData));
			break;
		case ITEM_KIND_GUARDIAN:
			//result.push_back(createItemWithBase(head->pData));
			break;
		case ITEM_KIND_SUPPLIES:
			//result.push_back(createItemWithBase(head->pData));
			break;
		case ITEM_KIND_CONSUMABLE:
			break;
		case ITEM_KIND_ZODIAC:
			//result.push_back(createItemWithBase(head->pData));
			break;
		case ITEM_KIND_SPECIAL:
		case ITEM_KIND_UPGRADE:
		case ITEM_KIND_LIQUID:
		case ITEM_KIND_EDITION:
		case ITEM_KIND_MAGICFIELDARRAY:
		case ITEM_KIND_MIXUPGRADE:
		case ITEM_KIND_MATERIALS:
		case ITEM_KIND_MAGICARRAY:
		case ITEM_KIND_RIDE:
		case ITEM_KIND_BAG:

			break;
		default:
			//result.push_back(createItemWithBase(head->pData));
			break;
		}
		head = head->pNext;
	}

	return result;
}

ItemPickupFiltering::ItemPickupFiltering() { }

static ItemPickupFiltering *shared = nullptr;

ItemPickupFiltering* ItemPickupFiltering::sharedInstance() {
	
	if (shared == nullptr) {
		shared = new ItemPickupFiltering();
	}

	return shared;
}

void ItemPickupFiltering::render() {
	debugView()->render();
}
