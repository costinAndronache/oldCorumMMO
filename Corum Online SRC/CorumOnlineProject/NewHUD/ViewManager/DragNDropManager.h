#pragma once

#include "BeltManager.h"
#include "UserInventoryManager.h"
#include "EquipItemsManager.h"

#include "../../NewHelpers/SharedNetwork.h"
#include "../../NewHelpers/SoundLibrary.h"

namespace NewInterface {
	class DragNDropManager {
	public:
		DragNDropManager(
			CMainUser*, 
			std::shared_ptr<SharedNetwork>, 
			std::shared_ptr<CustomUI::DragNDropSystem>,
			SoundLibrary*
		);

		void setupRoutes(
			std::shared_ptr<BeltManager> belt,
			std::shared_ptr<UserInventoryManager> userItemsInventory,
			std::shared_ptr<EquipItemsManager> equipItems
		);

	private:
		CMainUser* _mainUser;
		std::shared_ptr<SharedNetwork>  _network;
		SoundLibrary* _soundLibrary;
		std::shared_ptr<CustomUI::DragNDropSystem> _dragNDropSystem;

		void setupRoutesFromBelt(
			std::shared_ptr<BeltManager> fromBelt,
			std::shared_ptr<UserInventoryManager> toUserItemsInventory
		);

		void setupRoutesFromUserInventory(
			std::shared_ptr<UserInventoryManager> fromUserItemsInventory,
			std::shared_ptr<BeltManager> toBelt,
			std::shared_ptr<EquipItemsManager> toEquipItems
		);

		void setupRoutesFromEquipInventory(
			std::shared_ptr<EquipItemsManager> fromEquipItems,
			std::shared_ptr<UserInventoryManager> toInventory
		);

		void swapBeltItems(
			std::shared_ptr<BeltManager> belt, 
			CustomUI::Rect dragNDropEndFrame
		);

		void moveFromBeltToSmallItemsInventory(
			std::shared_ptr<BeltManager> belt,
			std::shared_ptr<UserInventoryManager> userInventory,
			CustomUI::Rect dragNDropEndFrame
		);

		void swapUserInventoryITems(
			std::shared_ptr<UserInventoryManager> inv,
			CustomUI::Rect dragNDropEndFrame
		);

		void moveFromUserInventoryToBelt(
			std::shared_ptr<UserInventoryManager> inv,
			std::shared_ptr<BeltManager> belt,
			CustomUI::Rect dragNDropEndFrame
		);

		void moveFromUserInventoryToEquip(
			std::shared_ptr<UserInventoryManager> inv,
			std::shared_ptr<EquipItemsManager> equipItems,
			CustomUI::Rect dragNDropEndFrame
		);

		void moveFromEquipToItemInventory(
			std::shared_ptr<EquipItemsManager> fromequipItems,
			std::shared_ptr<UserInventoryManager> toInventory,
			CustomUI::Rect dragNDropEndFrame
		);

		void dropOnTileFromBelt(std::shared_ptr<BeltManager> belt);
		void dropOnTileFromUserInventory(std::shared_ptr<UserInventoryManager> inventory);
	};
}

