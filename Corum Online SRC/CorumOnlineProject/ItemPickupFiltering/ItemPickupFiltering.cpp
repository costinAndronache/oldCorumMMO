#include "ItemPickupFiltering.h"
#include "../InitGame.h"
#include "../CorumResource.h"
#include "ItemFilteringView.h"
#include <iostream>
using namespace CustomUI;


std::vector<CItem*> getDebugItemList();

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
		switch (head->pData->dwCode_ID)
		{
		case ITEM_KIND_WEAPON:
			result.push_back(createItemWithBase(head->pData));
			break;
		case ITEM_KIND_AROMR:
			result.push_back(createItemWithBase(head->pData));
			break;
		case ITEM_KIND_GUARDIAN:
			//result.push_back(createItemWithBase(head->pData));
			break;
		case ITEM_KIND_SUPPLIES:
			result.push_back(createItemWithBase(head->pData));
			break;
		case ITEM_KIND_CONSUMABLE:
			result.push_back(createItemWithBase(head->pData));
			break;
		case ITEM_KIND_ZODIAC:
			result.push_back(createItemWithBase(head->pData));
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
			result.push_back(createItemWithBase(head->pData));
			break;
		default:
			result.push_back(createItemWithBase(head->pData));
			break;
		}
		head = head->pNext;
	}

	return result;
}

static std::string filePath() {
	std::string base(g_szBasePath);
	base.append("\\ItemFiltering.fil");
	return base;
}

static std::set<DWORD> readStoredSelectedIDs() {
	std::set<DWORD> result;

	std::ifstream ifs;
	std::string path = filePath();
	ifs.open(path.c_str(), std::ifstream::in);
	if (ifs.good()) {
		int count = 0;
		ifs >> count;
		for (int i = 0; i < count; i++) {
			DWORD id = 0;
			ifs >> id;
			result.insert(id);
		}
	}

	ifs.close();
	return result;
}

static void storeSelectedIDs(std::set<DWORD> ids) {
	std::ofstream ofs;
	std::string path = filePath();
	ofs.open(path, std::ofstream::out);
	if (ofs.good()) {
		ofs << ids.size() << '\n';
		for (auto it = ids.begin(); it != ids.end(); it++) {
			ofs << *it << '\n';
		}
	}

	ofs.close();
}

ItemPickupFiltering::ItemPickupFiltering() {
	auto items = getDebugItemList();
	Rect frame = { { 100, 100}, {380, 395} };
	std::set<DWORD> ids = readStoredSelectedIDs();
	_view = new ItemFilteringView(frame, items, ids, this);
}

static ItemPickupFiltering *shared = nullptr;

ItemPickupFiltering* ItemPickupFiltering::sharedInstance() {
	
	if (shared == nullptr) {
		shared = new ItemPickupFiltering();
	}

	return shared;
}

bool ItemPickupFiltering::isInterfaceFocused() {
	return _isInterfaceFocused;
}

bool ItemPickupFiltering::handleMouseDown() {
	_isInterfaceFocused = _view->handleMouseDown();
	return _isInterfaceFocused;
}

bool ItemPickupFiltering::handleMouseUp() {
	_isInterfaceFocused = _view->handleMouseUp();
	return _isInterfaceFocused;
}

bool ItemPickupFiltering::handleKeyUp(WPARAM wparam, LPARAM lparam) {
	return _view->handleKeyUp(wparam, lparam);
}

bool ItemPickupFiltering::handleKeyDown(WPARAM wparam, LPARAM lparam) {
	return _view->handleKeyDown(wparam, lparam);
}

void ItemPickupFiltering::render() {
	_view->renderWithRenderer(g_pRenderer, 1);
}

void ItemPickupFiltering::openView() {
	_view->setHidden(false);
}

std::set<DWORD> ItemPickupFiltering::currentSelectedIDs() {
	return _view->currentSelectedIDs();
}

void ItemPickupFiltering::itemFilteringViewDidUpdateSelection(ItemFilteringView*, std::set<DWORD> selectedItemIDs) {
	storeSelectedIDs(selectedItemIDs);
}

