#include "EquipItemsManager.h"
#include "../../../CommonServer/ItemManagerDefine.h"

using namespace NewInterface;
using namespace CustomUI;

static SpriteModel underlays[MAX_EQUIP_POOL] = { SpriteModel::zero };

static void buildUnderlays();

EquipItemsManager::EquipItemsManager(GenericItemsContainerView* managedView) {
	_managedView = managedView;
	_indexOnCurrentDragNDropItem = -1;
	buildUnderlays();
}


void EquipItemsManager::updateWithItems(const CItem equipVector[MAX_EQUIP_POOL]) {
	std::vector<GenericItemsContainerView::ItemWithUnderlay> models;
	for (int i = 0; i < MAX_EQUIP_POOL; i++) {
		models.push_back({ equipVector[i], underlays[i] });
	}

	_managedView->updateWithItems(
		models,
	[=](CItem item, SpriteModel sprite, int index, Rect globalFrame) {
		if (item.m_wItemID == 0) { return; } // empty item 
		if (!_handler) { return; }

		_indexOnCurrentDragNDropItem = index;
		_managedView->setHiddenStateForItemAtIndex(index, true);

		SpriteRenderable* sprr = new SpriteRenderable(globalFrame, sprite);
		_handler(sprr, globalFrame);
	}
	);
}


static void buildUnderlays() {
	auto underlaySprites = NewHUDResources::inventoryItemUnderlays;
	const auto put = [=](int where, SpriteModel what) -> void {
		underlays[where] = what;
	};

	put(EQUIP_TYPE_LHAND1, underlaySprites.shield);
	put(EQUIP_TYPE_LHAND2, underlaySprites.shield);
	put(EQUIP_TYPE_RHAND1, underlaySprites.sword);
	put(EQUIP_TYPE_RHAND2, underlaySprites.sword);
	put(EQUIP_TYPE_HELMET, underlaySprites.helm);
	put(EQUIP_TYPE_RIDE, underlaySprites.key);
	put(EQUIP_TYPE_LGLOVE, underlaySprites.gloveLeft);
	put(EQUIP_TYPE_RGLOVE, underlaySprites.gloveRight);
	put(EQUIP_TYPE_ARMOR, underlaySprites.armor);
	put(EQUIP_TYPE_AMULE2, underlaySprites.necklace);
	put(EQUIP_TYPE_BAG, underlaySprites.bag);
	put(EQUIP_TYPE_BELT, underlaySprites.belt);
	put(EQUIP_TYPE_BOOTS, underlaySprites.boot);
	put(EQUIP_TYPE_LRING1, underlaySprites.ring);
	put(EQUIP_TYPE_LRING2, underlaySprites.ring);
	put(EQUIP_TYPE_LRING3, underlaySprites.ring);
	put(EQUIP_TYPE_RRING1, underlaySprites.ring);
	put(EQUIP_TYPE_RRING2, underlaySprites.ring);
	put(EQUIP_TYPE_RRING3, underlaySprites.ring);
}

void EquipItemsManager::resetIndexOnCurrentDragNDropItem() {
	_managedView->setHiddenStateForItemAtIndex(_indexOnCurrentDragNDropItem, false);
	_indexOnCurrentDragNDropItem = -1;
}