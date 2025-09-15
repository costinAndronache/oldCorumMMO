#include "DragNDropManager.h"
#include "../../InitGame.h"
#include "../../NetworkClient.h"

using namespace NewInterface;
using namespace CustomUI;

static CTDS_ITEM_MOVE movePacket(
	ITEM_NATIVE from, int fromIndex,
	ITEM_NATIVE to, int toIndex,
	int quantity
) {
	CTDS_ITEM_MOVE packet;

	packet.bSrc = from;
	packet.bDest = to;
	packet.bSrcIndex = fromIndex;
	packet.bDestIndex = toIndex;

	packet.bSectionNum = 1;
	packet.bQuantity = quantity;
	packet.dwUnitID = 0;
	packet.dwMoney = 0;
	packet.wPlayerShopItemID = 0;
	return packet;
}

static CTDS_ITEM_MOVE dropToTilePacket(ITEM_NATIVE from, int fromIndex, int quantity) {
	CTDS_ITEM_MOVE packet;

	packet.bSrc = from;
	packet.bDest = ITEM_NATIVE_TILE;
	packet.bSrcIndex = fromIndex;
	packet.bDestIndex = 0xff;

	packet.bSectionNum = 1;
	packet.bQuantity = quantity;
	packet.dwUnitID = 0;
	packet.dwMoney = 0;
	packet.wPlayerShopItemID = 0;

	return packet;
}

DragNDropManager::DragNDropManager(
	CMainUser* mainUser, 
	std::shared_ptr<SharedNetwork> network, 
	std::shared_ptr<CustomUI::DragNDropSystem> dragNDropSystem,
	SoundLibrary* soundLibrary
) {
	_mainUser = mainUser;
	_network = network;
	_dragNDropSystem = dragNDropSystem;
	_soundLibrary = soundLibrary;
}

void DragNDropManager::setupRoutes(
	std::shared_ptr<BeltManager> belt,
	std::shared_ptr<UserInventoryManager> userItemsInventory,
	std::shared_ptr<EquipItemsManager> equipItems
) {
	setupRoutesFromBelt(belt, userItemsInventory);
	setupRoutesFromUserInventory(userItemsInventory, belt, equipItems);
	setupRoutesFromEquipInventory(equipItems, userItemsInventory);
}

void DragNDropManager::setupRoutesFromBelt(
	std::shared_ptr<BeltManager> fromBelt,
	std::shared_ptr<UserInventoryManager> toUserItemsInventory
) {
	const auto toReceivers = std::vector<std::shared_ptr<DragNDropReceiver>>{ fromBelt, toUserItemsInventory };
	auto onNoRouteMatched = [=]() -> void {
		dropOnTileFromBelt(fromBelt);
	};

	_dragNDropSystem->registerRoute(
		fromBelt,
		toReceivers,
	[=](Rect frameOnEnd, std::set<unsigned int> indexesOfReceivers) {
		for (auto index : indexesOfReceivers) {
			const auto r = toReceivers[index];
			if (r == fromBelt) {
				swapBeltItems(fromBelt, frameOnEnd);
				return;
			}

			if (r == toUserItemsInventory) {
				moveFromBeltToSmallItemsInventory(
					fromBelt,
					toUserItemsInventory,
					frameOnEnd
				);
			}

		}

		onNoRouteMatched();
	},
		onNoRouteMatched
	);
}

void DragNDropManager::setupRoutesFromUserInventory(
	std::shared_ptr<UserInventoryManager> fromUserItemsInventory,
	std::shared_ptr<BeltManager> toBelt,
	std::shared_ptr<EquipItemsManager> toEquipItems
) {
	const auto toReceivers = std::vector<std::shared_ptr<DragNDropReceiver>>{ fromUserItemsInventory, toBelt, toEquipItems };
	auto onNoRouteMatched = [=]() -> void {
		dropOnTileFromUserInventory(fromUserItemsInventory);
	};

	_dragNDropSystem->registerRoute(
		fromUserItemsInventory,
		toReceivers,
		[=](Rect frameOnEnd, std::set<unsigned int> indexesOfReceivers) {
		for (auto index : indexesOfReceivers) {
			const auto r = toReceivers[index];
			if (r == fromUserItemsInventory) {
				swapUserInventoryITems(fromUserItemsInventory, frameOnEnd);
				return;
			}

			if (r == toBelt) {
				moveFromUserInventoryToBelt(fromUserItemsInventory, toBelt, frameOnEnd);
				return;
			}

			if (r == toEquipItems) {
				moveFromUserInventoryToEquip(fromUserItemsInventory, toEquipItems, frameOnEnd);
				return;
			}

		}

		onNoRouteMatched();
	},
		onNoRouteMatched
	);

}

void DragNDropManager::setupRoutesFromEquipInventory(
	std::shared_ptr<EquipItemsManager> fromEquipItems,
	std::shared_ptr<UserInventoryManager> toInventory
) {
	const auto toReceivers = std::vector<std::shared_ptr<DragNDropReceiver>>{ toInventory };
	auto onNoRouteMatched = [=]() -> void {
		fromEquipItems->resetIndexOnCurrentDragNDropItem();
	};

	_dragNDropSystem->registerRoute(
		fromEquipItems,
		toReceivers,
		[=](Rect frameOnEnd, std::set<unsigned int> indexesOfReceivers) {
		for (auto index : indexesOfReceivers) {
			const auto r = toReceivers[index];
			if (r == toInventory) {
				moveFromEquipToItemInventory(fromEquipItems, toInventory, frameOnEnd);
				return;
			}
		}

		onNoRouteMatched();
	},
		onNoRouteMatched
	);

}

void DragNDropManager::swapBeltItems(
	std::shared_ptr<BeltManager> belt, 
	Rect dragNDropEndFrame) 
{
	const auto fromIndex = belt->indexOnCurrentDragNDropItem();
	const auto toIndex = belt->itemIndexForGlobalPoint(dragNDropEndFrame.center());

	if (!(fromIndex >= 0 && toIndex >= 0)) {
		belt->resetIndexOnCurrentDragNDropItem();
		return;
	}

	const auto item = _mainUser->beltItemAtIndex(fromIndex);
	auto packet = movePacket(ITEM_NATIVE_BELT, fromIndex, ITEM_NATIVE_BELT, toIndex, item.GetQuantity());

	_network->onNextItemMove([=](DSTC_ITEM_MOVE incomingPacket) {
		if (incomingPacket.bErrorCode != DSTC_ITEM_MOVE::ERROR_CODE_NO_ERROR) {
			belt->resetIndexOnCurrentDragNDropItem();
		}
	});
	_network->send(packet);
}

void DragNDropManager::moveFromBeltToSmallItemsInventory(
	std::shared_ptr<BeltManager> belt,
	std::shared_ptr<UserInventoryManager> userInventory,
	Rect dragNDropEndFrame) 
{
	const auto fromIndex = belt->indexOnCurrentDragNDropItem();
	const auto toIndex = userInventory->itemIndexForGlobalPoint(dragNDropEndFrame.center());

	if (!(toIndex.tab == GroupedItemInventoryView::Tab::smallItems &&
		  fromIndex >= 0 && toIndex.index >= 0)) {
		belt->resetIndexOnCurrentDragNDropItem();
		return;
	}
	const auto item = _mainUser->beltItemAtIndex(fromIndex);
	auto packet = movePacket(
		ITEM_NATIVE_BELT, fromIndex, 
		ITEM_NATIVE_INV_SMALL, toIndex.index, 
		item.GetQuantity()
	);

	_network->onNextItemMove([=](DSTC_ITEM_MOVE incomingPacket) {
		if (incomingPacket.bErrorCode != DSTC_ITEM_MOVE::ERROR_CODE_NO_ERROR) {
			belt->resetIndexOnCurrentDragNDropItem();
			_soundLibrary->playItemMouseDropInInterface();

		}
	});
	_network->send(packet);
}

void DragNDropManager::swapUserInventoryITems(
	std::shared_ptr<UserInventoryManager> inv,
	CustomUI::Rect dragNDropEndFrame
) {
	const auto fromIndex = inv->indexOnCurrentDragNDropItem();
	const auto toIndex = inv->itemIndexForGlobalPoint(dragNDropEndFrame.center());
	if (!(fromIndex.index >= 0 && toIndex.index >= 0 && fromIndex.tab == toIndex.tab)) {
		inv->resetIndexOnCurrentDragNDropItem();
		return;
	}

	CTDS_ITEM_MOVE packet = [=]() {
		switch (fromIndex.tab) {
		case GroupedItemInventoryView::Tab::largeItems:
			const auto largeItem = _mainUser->m_pInv_Large[fromIndex.index];
			return movePacket(
				ITEM_NATIVE_INV_LARGE, fromIndex.index,
				ITEM_NATIVE_INV_LARGE, toIndex.index,
				largeItem.GetQuantity()
			);
		case GroupedItemInventoryView::Tab::smallItems:
			const auto smallItem = _mainUser->m_pInv_Small[fromIndex.index];
			return movePacket(
				ITEM_NATIVE_INV_SMALL, fromIndex.index,
				ITEM_NATIVE_INV_SMALL, toIndex.index,
				smallItem.GetQuantity()
			);
		}
	}();

	_network->onNextItemMove([=](DSTC_ITEM_MOVE incomingPacket) {
		if (incomingPacket.bErrorCode != DSTC_ITEM_MOVE::ERROR_CODE_NO_ERROR) {
			inv->resetIndexOnCurrentDragNDropItem();
			_soundLibrary->playItemMouseDropInInterface();
		}
	});
	_network->send(packet);

}

void DragNDropManager::moveFromUserInventoryToBelt(
	std::shared_ptr<UserInventoryManager> inv,
	std::shared_ptr<BeltManager> belt,
	CustomUI::Rect dragNDropEndFrame
) {
	const auto fromIndex = inv->indexOnCurrentDragNDropItem();
	const auto toIndex = belt->itemIndexForGlobalPoint(dragNDropEndFrame.center());

	if (!(fromIndex.tab == GroupedItemInventoryView::Tab::smallItems &&
		  fromIndex.index >= 0 && toIndex >= 0)) {
		inv->resetIndexOnCurrentDragNDropItem();
		return;
	}

	const auto item = _mainUser->m_pInv_Small[fromIndex.index];
	auto packet = movePacket(
		ITEM_NATIVE_INV_SMALL, fromIndex.index,
		ITEM_NATIVE_BELT, toIndex,
		item.GetQuantity()
	);

	_network->onNextItemMove([=](DSTC_ITEM_MOVE incomingPacket) {
		if (incomingPacket.bErrorCode != DSTC_ITEM_MOVE::ERROR_CODE_NO_ERROR) {
			inv->resetIndexOnCurrentDragNDropItem();
			_soundLibrary->playItemMouseDropInInterface();
		}
	});
	_network->send(packet);
}

void DragNDropManager::moveFromUserInventoryToEquip(
	std::shared_ptr<UserInventoryManager> inv,
	std::shared_ptr<EquipItemsManager> equipItems,
	CustomUI::Rect dragNDropEndFrame
) {
	const auto fromIndex = inv->indexOnCurrentDragNDropItem();
	const auto toIndex = equipItems->itemIndexForGlobalPoint(dragNDropEndFrame.center());

	const auto item = _mainUser->m_pInv_Large[fromIndex.index];

	if (!(fromIndex.tab == GroupedItemInventoryView::Tab::largeItems &&
		fromIndex.index >= 0 && toIndex >= 0)) {
		inv->resetIndexOnCurrentDragNDropItem();
		return;
	}

	auto packet = movePacket(
		ITEM_NATIVE_INV_LARGE, fromIndex.index,
		ITEM_NATIVE_EQUIP, toIndex,
		item.GetQuantity()
	);

	_network->onNextItemMove([=](DSTC_ITEM_MOVE incomingPacket) {
		if (incomingPacket.bErrorCode != DSTC_ITEM_MOVE::ERROR_CODE_NO_ERROR) {
			inv->resetIndexOnCurrentDragNDropItem();
			_soundLibrary->playItemMouseDropInInterface();

		}
	});
	_network->send(packet);
}

void DragNDropManager::moveFromEquipToItemInventory(
	std::shared_ptr<EquipItemsManager> fromEquipItems,
	std::shared_ptr<UserInventoryManager> toInventory,
	CustomUI::Rect dragNDropEndFrame
) {
	const auto fromIndex = fromEquipItems->indexOnCurrentDragNDropItem();
	const auto toIndex = toInventory->itemIndexForGlobalPoint(dragNDropEndFrame.center());

	const auto item = _mainUser->m_pEquip[fromIndex];

	if (!(toIndex.tab == GroupedItemInventoryView::Tab::largeItems &&
		toIndex.index >= 0 && fromIndex >= 0)) {
		fromEquipItems->resetIndexOnCurrentDragNDropItem();
		return;
	}

	auto packet = movePacket(
		ITEM_NATIVE_EQUIP, fromIndex,
		ITEM_NATIVE_INV_LARGE, toIndex.index,
		item.GetQuantity()
	);

	_network->send(packet);
	_soundLibrary->playItemMouseDropInInterface();
}

void DragNDropManager::dropOnTileFromBelt(std::shared_ptr<BeltManager> belt) {
	const auto fromIndex = belt->indexOnCurrentDragNDropItem();
	if (!(fromIndex >= 0)) { return;  }
	const auto item = _mainUser->beltItemAtIndex(fromIndex);

	auto packet = dropToTilePacket(
		ITEM_NATIVE_BELT, fromIndex, item.GetQuantity()
	);

	_network->send(packet);
	_soundLibrary->playItemMouseDropInTile();
}

void DragNDropManager::dropOnTileFromUserInventory(std::shared_ptr<UserInventoryManager> inventory) {
	const auto fromIndex = inventory->indexOnCurrentDragNDropItem();
	if (!(fromIndex.index >= 0)) { return; }
	
	typedef struct { ITEM_NATIVE source; CItem item; } Data;

	auto data = [=]() -> Data {
		switch (fromIndex.tab) {
		case GroupedItemInventoryView::Tab::smallItems:
			return {
				ITEM_NATIVE_INV_SMALL, _mainUser->m_pInv_Small[fromIndex.index]
			};
		case GroupedItemInventoryView::Tab::largeItems:
			return {
				ITEM_NATIVE_INV_LARGE, _mainUser->m_pInv_Large[fromIndex.index]
			};
		}
	}();

	auto packet = dropToTilePacket(
		data.source, fromIndex.index, data.item.GetQuantity()
	);

	_network->send(packet);
	_soundLibrary->playItemMouseDropInTile();

}