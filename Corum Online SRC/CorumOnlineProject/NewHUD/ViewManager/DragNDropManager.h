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
			SharedNetwork*, 
			CustomUI::DragNDropSystem*,
			SoundLibrary*
		);

		void setupRoutes(
			BeltManager* belt,
			UserInventoryManager* userItemsInventory,
			EquipItemsManager* equipItems
		);

	private:
		CMainUser* _mainUser;
		SharedNetwork* _network;
		SoundLibrary* _soundLibrary;
		CustomUI::DragNDropSystem* _dragNDropSystem;

		void setupRoutesFromBelt(
			BeltManager* fromBelt,
			UserInventoryManager* toUserItemsInventory
		);

		void setupRoutesFromUserInventory(
			UserInventoryManager* fromUserItemsInventory,
			BeltManager* toBelt,
			EquipItemsManager* toEquipItems
		);

		void setupRoutesFromEquipInventory(
			EquipItemsManager* fromEquipItems,
			UserInventoryManager* toInventory
		);

		void swapBeltItems(BeltManager* belt, CustomUI::Rect dragNDropEndFrame);
		void moveFromBeltToSmallItemsInventory(
			BeltManager* belt,
			UserInventoryManager* userInventory,
			CustomUI::Rect dragNDropEndFrame
		);

		void swapUserInventoryITems(
			UserInventoryManager* inv,
			CustomUI::Rect dragNDropEndFrame
		);

		void moveFromUserInventoryToBelt(
			UserInventoryManager* inv,
			BeltManager* belt,
			CustomUI::Rect dragNDropEndFrame
		);

		void moveFromUserInventoryToEquip(
			UserInventoryManager* inv,
			EquipItemsManager* equipItems,
			CustomUI::Rect dragNDropEndFrame
		);

		void moveFromEquipToItemInventory(
			EquipItemsManager* fromequipItems,
			UserInventoryManager* toInventory,
			CustomUI::Rect dragNDropEndFrame
		);

		void dropOnTileFromBelt(BeltManager* belt);
		void dropOnTileFromUserInventory(UserInventoryManager* inventory);
	};
}

