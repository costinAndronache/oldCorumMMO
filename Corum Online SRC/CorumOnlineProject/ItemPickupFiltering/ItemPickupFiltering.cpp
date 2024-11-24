#include "ItemPickupFiltering.h"
#include "../InitGame.h"
#include "../CorumResource.h"
#include "ItemFilteringView.h"
#include <iostream>
using namespace CustomUI;
using namespace ItemPickupFiltering;

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

ItemPickupFilteringSystem::ItemPickupFilteringSystem() {
	auto items = getDebugItemList();
	Rect screen = { { 0, 0}, { windowWidth(), windowHeight() } };
	Rect frame = { { 100, 100}, {380, 395} };
	frame = frame.centeredWith(screen);

	std::set<DWORD> ids = readStoredSelectedIDs();
	_view = new ItemFilteringView(frame, items, ids, this);
}

static ItemPickupFilteringSystem *shared = nullptr;

ItemPickupFilteringSystem* ItemPickupFilteringSystem::sharedInstance() {
	
	if (shared == nullptr) {
		shared = new ItemPickupFilteringSystem();
	}

	return shared;
}

bool ItemPickupFilteringSystem::isInterfaceFocused() {
	return _isInterfaceFocused;
}

bool ItemPickupFilteringSystem::handleMouseDown() {
	_isInterfaceFocused = _view->handleMouseDown();
	return _isInterfaceFocused;
}

bool ItemPickupFilteringSystem::handleMouseUp() {
	_isInterfaceFocused = _view->handleMouseUp();
	return _isInterfaceFocused;
}

bool ItemPickupFilteringSystem::handleKeyUp(WPARAM wparam, LPARAM lparam) {
	return _view->handleKeyUp(wparam, lparam);
}

bool ItemPickupFilteringSystem::handleKeyDown(WPARAM wparam, LPARAM lparam) {
	return _view->handleKeyDown(wparam, lparam);
}

void ItemPickupFilteringSystem::render() {
	_view->renderWithRenderer(g_pRenderer, 1);
}

void ItemPickupFilteringSystem::setViewActive(bool active) {
	_view->setHidden(!active);

	if (active) {
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
	else {
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
}

bool ItemPickupFilteringSystem::isViewActive() {
	return !_view->getHidden();
}

std::set<DWORD> ItemPickupFilteringSystem::currentSelectedIDs() {
	return _view->currentSelectedIDs();
}

void ItemPickupFilteringSystem::itemFilteringViewDidUpdateSelection(ItemFilteringView*, std::set<DWORD> selectedItemIDs) {
	storeSelectedIDs(selectedItemIDs);
}

