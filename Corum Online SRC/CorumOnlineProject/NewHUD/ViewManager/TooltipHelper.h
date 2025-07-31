#pragma once
// Add these includes if not already present
#include "../../message.h"
#include "../../InitGame.h"
#include "../DynamicInfoBox.h"
#include "../../DungeonTable.h"

namespace NewInterface {
	class TooltipHelper {
		TooltipHelper(
			CMessagePool* messagePool,
			CMainUser* mainPlayer,
			CItemOptionHash* itemOptionHash,
			EffectLayer* effectLayer,
			CItemAttrLayer* itemAttrLayer,
			CDungeonTable* _dungeonTable
		);

		std::vector<DynamicInfoBox::InfoLine> generateItemTooltip(const CItem& item);


	private:
		CMessagePool* _messagePool;
		CMainUser* _mainUser;
		CItemOptionHash* _itemOptionHash;
		EffectLayer* _effectLayer;
		CItemAttrLayer* _itemAttrLayer;
		CDungeonTable* _dungeonTable;

	private:
		void	ItemInfoRender(CItem* pItem, BOOL isEquip, BOOL bPrice = FALSE, DWORD dwPrice = 0, BYTE byZipCode = 0, BYTE byOption = 0);
		void	ItemInfoRender_Weapon(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, LPSET_ITEM_INFO pSetItemInfo, DWORD dwItemSetID, DWORD dwSetNum, DWORD* pdwTextColor, BOOL isEquip, int iOption, int* nMaxSize);
		void	ItemInfoRender_Armor(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, LPSET_ITEM_INFO pSetItemInfo, DWORD dwItemSetID, DWORD dwSetNum, DWORD* pdwTextColor, BOOL isEquip, int iOption, int* nMaxSize, BYTE byZipCode);
		void	ItemInfoRender_Guardian(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize);
		void	ItemInfoRender_Supplies(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize);
		void	ItemInfoRender_Consumable(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize);
		void	ItemInfoRender_Zodiac(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize);
		void	ItemInfoRender_Default(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize);
		void	ItemInfoRender_Option(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize, int* iOption);
		void	ItemInfoRender_SetItemBonusOption(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize, BOOL isEquip, DWORD dwItemSetID, DWORD dwSetNum, LPSET_ITEM_INFO pSetItemInfo);
	};
}

