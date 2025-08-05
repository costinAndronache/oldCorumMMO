#pragma once
// Add these includes if not already present
#include "../../message.h"
#include "../../InitGame.h"
#include "../DynamicInfoBox.h"
#include "../../DungeonTable.h"

namespace NewInterface {
	class TooltipHelper {
	public:
		TooltipHelper(
			CMessagePool& messagePool,
			CMainUser* mainPlayer,
			CItemOptionHash* itemOptionHash,
			EffectLayer* effectLayer,
			CItemAttrLayer* itemAttrLayer,
			CDungeonTable* _dungeonTable,
			CSetItemInfoHash* setItemInfoHash,
			LPBASEITEM_HASH itemTableHash,
			SSKILL_DPINFO sSkillInfoDP[MAX_SKILL]
		);

		std::vector<DynamicInfoBox::InfoLine> tooltipForItem(const CItem& item);
		std::vector<DynamicInfoBox::InfoLine> tooltipForEquippedItem(const CItem& item, BYTE EQUIP_TYPE);

		std::vector<DynamicInfoBox::InfoLine> tooltipForSoldItem(const CItem& item, int sellerPrice);

		std::vector<DynamicInfoBox::InfoLine> tooltipForSkill(BYTE skillKIND);
	private:
		CMessagePool& _messagePool;
		CMainUser* _mainUser;
		CItemOptionHash* _itemOptionHash;
		EffectLayer* _effectLayer;
		CItemAttrLayer* _itemAttrLayer;
		CDungeonTable* _dungeonTable;
		CSetItemInfoHash* _setItemInfoHash;
		LPBASEITEM_HASH _itemTableHash;
		SSKILL_DPINFO* _sSkillInfoDP;

	private:
		std::vector<DynamicInfoBox::InfoLine>	ItemInfoRender(CItem* pItem, BOOL isCurrentlyEquipped, BOOL isCurrentlySold = FALSE, DWORD dwPrice = 0, BYTE EQUIP_TYPE = 0, BYTE playerEquipUpgradeLevel = 0);
		void	ItemInfoRender_Weapon(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, LPSET_ITEM_INFO pSetItemInfo, DWORD dwItemSetID, DWORD dwSetNum, DWORD* pdwTextColor, BOOL isEquip, int iOption, int* nMaxSize);
		void	ItemInfoRender_Armor(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, LPSET_ITEM_INFO pSetItemInfo, DWORD dwItemSetID, DWORD dwSetNum, DWORD* pdwTextColor, BOOL isEquip, int iOption, int* nMaxSize, BYTE EQUIP_TYPE);
		void	ItemInfoRender_Guardian(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize);
		void	ItemInfoRender_Supplies(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize);
		void	ItemInfoRender_Consumable(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize);
		void	ItemInfoRender_Zodiac(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize);
		void	ItemInfoRender_Default(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize);
		void	ItemInfoRender_Option(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize, int* iOption);
		void	ItemInfoRender_SetItemBonusOption(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize, BOOL isEquip, DWORD dwItemSetID, DWORD dwSetNum, LPSET_ITEM_INFO pSetItemInfo);
	};
}

