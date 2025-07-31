#include "TooltipHelper.h"
#include "../../InitGame.h"
#include "../../message.h"

using namespace CustomUI;
using namespace NewInterface;


TooltipHelper::TooltipHelper(
	CMessagePool* messagePool,
	CMainUser* mainPlayer,
	CItemOptionHash* itemOptionHash,
	EffectLayer* effectLayer,
	CItemAttrLayer* itemAttrLayer,
	CDungeonTable* dungeonTable
) {
	_messagePool = messagePool;
	_mainUser = mainPlayer;
	_itemOptionHash = itemOptionHash;
	_effectLayer = _effectLayer;
	_itemAttrLayer = _itemAttrLayer;
	_dungeonTable = dungeonTable;
}

std::vector<DynamicInfoBox::InfoLine> TooltipHelper::generateItemTooltip(const CItem& item) {



}


//
static void GetDueDayText(char* pszItemInfo, DWORD dwDueDay);


void TooltipHelper::ItemInfoRender_Weapon(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, LPSET_ITEM_INFO pSetItemInfo
	, DWORD dwItemSetID, DWORD dwSetNum, DWORD* pdwTextColor, BOOL isEquip, int iOption, int* nMaxSize)
{
	int		nSize = 0;
	char	szInfo[0xff] = { 0, };

	if (pSetItemInfo && dwItemSetID != 0 && dwItemSetID != 32767)
	{
		strcpy(szItemInfo[*iInfoRow], pSetItemInfo->szItemName_Kor);
	}

	strcat(szItemInfo[*iInfoRow], "(");
	strcat(szItemInfo[*iInfoRow], pItemInfo->szItemName_Kor);

	if (isEquip)
	{
		if (pSetItemInfo && dwItemSetID != 0 && dwItemSetID != 32767)
		{
			char szTmp[50] = { 0, };
			wsprintf(szTmp, " : %d/%d)", dwSetNum, pSetItemInfo->bFullSet);
			strcat(szItemInfo[*iInfoRow], szTmp);
		}
		else
		{
			strcat(szItemInfo[*iInfoRow], ")");
		}
	}
	else
	{
		strcat(szItemInfo[*iInfoRow], ")");
	}

	if (pItem->m_bSlot.uUsedSlot > 0)
	{
		memset(szInfo, 0, sizeof(szInfo));
		wsprintf(szInfo, " + %u", pItem->m_bSlot.uUsedSlot);
		strcat(szItemInfo[*iInfoRow], szInfo);
	}

	switch (pItemInfo->BaseItem_Weapon.byDropBox)
	{
	case 1:
		if (iOption != 0)
			pdwTextColor[*iInfoRow] = TEXT_COLOR_BLUE;
		break;
	case 2:
		pdwTextColor[*iInfoRow] = TEXT_COLOR_GREEN;
		break;
	case 3:
		pdwTextColor[*iInfoRow] = TEXT_COLOR_YELLOW;
		break;
	}

	if (pItem->m_bSlot.uUsedSlot > 0)
	{
		// 업그레이드 된 아이템은 노랑색으로...
		pdwTextColor[*iInfoRow] = TEXT_COLOR_YELLOW;
	}

	nSize = lstrlen(szItemInfo[*iInfoRow]);

	if (*nMaxSize < nSize)
		*nMaxSize = nSize;

	if (pItemInfo->GetMovable() != 0)// (귀속아이템) 표시
	{
		*iInfoRow += 1;
		strcpy(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE1428].szMessage);
		pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;
		nSize = lstrlen(szItemInfo[*iInfoRow]);
		if (*nMaxSize < nSize)
			*nMaxSize = nSize;

		*iInfoRow += 1;
	}
	else
	{
		*iInfoRow += 2;
	}

	if (pItemInfo->BaseItem_Weapon.bClassID != 0)
	{
		BOOL bClass = FALSE;

		if (g_pMainPlayer && pItemInfo->BaseItem_Weapon.bClassID & (BYTE)(1 << (g_pMainPlayer->m_wClass - 1)))
			bClass = TRUE;

		if (bClass)
		{
			// "클래스착용 가능"
			strcpy(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE995].szMessage);
			pdwTextColor[*iInfoRow] = 0xffffffff;
		}
		else
		{
			// "클래스착용 불가"
			strcpy(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE996].szMessage);
			pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;
			pdwTextColor[*iInfoRow - 2] = TEXT_COLOR_RED;
		}

		nSize = lstrlen(szItemInfo[*iInfoRow]);

		if (*nMaxSize < nSize)
			*nMaxSize = nSize;

		*iInfoRow += 1;
	}

	wsprintf(szItemInfo[*iInfoRow], "       ");

	nSize = lstrlen(szItemInfo[*iInfoRow]);

	if (*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;

	// "Damage %d ~ %d"
	wsprintf(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE858].szMessage
		, (int)pItem->m_Item_Weapon.wMin_Damage, (int)pItem->m_Item_Weapon.wMax_Damage);

	nSize = lstrlen(szItemInfo[*iInfoRow]);

	if (*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;

	// "Grade : %u"
	wsprintf(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE997].szMessage
		, pItemInfo->BaseItem_Weapon.bItemGrade);

	nSize = lstrlen(szItemInfo[*iInfoRow]);

	if (*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;

	// "스킬공격력 : %u"
	wsprintf(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE998].szMessage, pItem->m_Item_Weapon.wMana_Damage);
	nSize = lstrlen(szItemInfo[*iInfoRow]);

	if (*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;

	// "Slot : %u"
	wsprintf(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE860].szMessage, pItem->m_bSlot.uMaxSlot);
	nSize = lstrlen(szItemInfo[*iInfoRow]);

	if (*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;

	// "무게 : %u"
	wsprintf(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE999].szMessage, pItemInfo->GetWeight());
	nSize = lstrlen(szItemInfo[*iInfoRow]);

	if (*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;

#ifdef __USE_STAT_LIMIT					
	if (g_pMainPlayer->m_dwLevel >= pItemInfo->BaseItem_Weapon.wMin_Level)
		pdwTextColor[*iInfoRow] = 0xffffffff;
	else
		pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;

	wsprintf(szItemInfo[*iInfoRow], "Level : %u", pItemInfo->BaseItem_Weapon.wMin_Level);
	nSize = lstrlen(szItemInfo[*iInfoRow]);

	if (*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;

	if (g_pMainPlayer->m_dwStr >= pItemInfo->BaseItem_Weapon.wMin_Str)
		pdwTextColor[*iInfoRow] = 0xffffffff;
	else
		pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;

	wsprintf(szItemInfo[*iInfoRow], "Str : %u", pItemInfo->BaseItem_Weapon.wMin_Str);
	nSize = lstrlen(szItemInfo[*iInfoRow]);

	if (*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;
#endif
}


void TooltipHelper::ItemInfoRender_Armor(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, LPSET_ITEM_INFO pSetItemInfo
	, DWORD dwItemSetID, DWORD dwSetNum, DWORD* pdwTextColor, BOOL isEquip, int iOption, int* nMaxSize, BYTE byZipCode)
{
	int		nSize = 0;
	char	szInfo[0xff] = { 0, };

	if (pSetItemInfo && dwItemSetID != 0 && dwItemSetID != 32767)
	{
		wsprintf(szItemInfo[*iInfoRow], "%s", pSetItemInfo->szItemName_Kor);
	}

	strcat(szItemInfo[*iInfoRow], "(");
	strcat(szItemInfo[*iInfoRow], pItemInfo->szItemName_Kor);

	if (isEquip)
	{
		if (pSetItemInfo && dwItemSetID != 0 && dwItemSetID != 32767)
		{
			char szTmp[50] = { 0, };
			wsprintf(szTmp, " : %d/%d)", dwSetNum, pSetItemInfo->bFullSet);
			strcat(szItemInfo[*iInfoRow], szTmp);
		}
		else
		{
			strcat(szItemInfo[*iInfoRow], ")");
		}
	}
	else
	{
		strcat(szItemInfo[*iInfoRow], ")");
	}

	if (pItem->m_bSlot.uUsedSlot > 0)
	{
		memset(szInfo, 0, sizeof(szInfo));
		wsprintf(szInfo, " + %u", pItem->m_bSlot.uUsedSlot);
		strcat(szItemInfo[*iInfoRow], szInfo);
	}

	nSize = lstrlen(szItemInfo[*iInfoRow]);

	if (*nMaxSize < nSize)
		*nMaxSize = nSize;

	switch (pItemInfo->BaseItem_Armor.byDropBox)// : 050108 hwoarang
	{
	case 1:
		if (iOption != 0)
			pdwTextColor[*iInfoRow] = TEXT_COLOR_BLUE;
		break;
	case 2:
		pdwTextColor[*iInfoRow] = TEXT_COLOR_GREEN;
		break;
	case 3:
		pdwTextColor[*iInfoRow] = TEXT_COLOR_YELLOW;
		break;
	}

	if (pItem->m_bSlot.uUsedSlot > 0)
	{
		// 업그레이드 된 아이템은 노랑색으로...
		pdwTextColor[*iInfoRow] = TEXT_COLOR_YELLOW;
	}

	if (isEquip)
	{
		switch (byZipCode)
		{
		case EQUIP_TYPE_AMULE1:
		case EQUIP_TYPE_AMULE3:
		case EQUIP_TYPE_LRING1:
		case EQUIP_TYPE_LRING2:
		case EQUIP_TYPE_LRING3:
			pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;
			break;
		}
	}

	if (pItemInfo->GetMovable() != 0)// (귀속아이템) 표시
	{
		*iInfoRow += 1;
		strcpy(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE1428].szMessage);
		pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;
		nSize = lstrlen(szItemInfo[*iInfoRow]);
		if (*nMaxSize < nSize)
			*nMaxSize = nSize;

		*iInfoRow += 1;
	}
	else
	{
		*iInfoRow += 2;
	}

	if (pItemInfo->BaseItem_Armor.bClassID != 0)
	{
		BOOL bClass = FALSE;

		if (g_pMainPlayer && pItemInfo->BaseItem_Armor.bClassID & (BYTE)(1 << (g_pMainPlayer->m_wClass - 1)))
			bClass = TRUE;

		if (bClass)
		{
			// "클래스착용 가능"
			strcpy(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE995].szMessage);
			pdwTextColor[*iInfoRow] = 0xffffffff;
		}
		else
		{
			strcpy(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE996].szMessage);
			pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;
			pdwTextColor[*iInfoRow - 2] = TEXT_COLOR_RED;
		}

		nSize = lstrlen(szItemInfo[*iInfoRow]);

		if (*nMaxSize < nSize)
			*nMaxSize = nSize;

		*iInfoRow += 1;
	}

	wsprintf(szItemInfo[*iInfoRow], "       ");

	nSize = lstrlen(szItemInfo[*iInfoRow]);

	if (*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;

	// MSG_ID : 104 ; Defense %d
	wsprintf(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE104].szMessage, pItem->m_Item_Armor.wDefense);
	nSize = lstrlen(szItemInfo[*iInfoRow]);

	if (*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;

	// "Grade : %u"
	wsprintf(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE997].szMessage, pItemInfo->BaseItem_Armor.bItemGrade);
	nSize = lstrlen(szItemInfo[*iInfoRow]);

	if (*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;

	// "Slot : %u"
	wsprintf(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE860].szMessage, pItem->m_bSlot.uMaxSlot);
	nSize = lstrlen(szItemInfo[*iInfoRow]);

	if (*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;

	// "무게 : %u"
	wsprintf(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE999].szMessage, pItemInfo->GetWeight());
	nSize = lstrlen(szItemInfo[*iInfoRow]);

	if (*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;

#ifdef __USE_STAT_LIMIT						
	if (g_pMainPlayer->m_dwLevel >= pItemInfo->BaseItem_Armor.wMin_Level)
		pdwTextColor[*iInfoRow] = 0xffffffff;
	else
		pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;

	wsprintf(szItemInfo[*iInfoRow], "Level : %u", pItemInfo->BaseItem_Armor.wMin_Level);

	nSize = lstrlen(szItemInfo[*iInfoRow]);

	if (*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;

	if (g_pMainPlayer->m_dwStr >= pItemInfo->BaseItem_Armor.wMin_Str)
		pdwTextColor[*iInfoRow] = 0xffffffff;
	else
		pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;

	wsprintf(szItemInfo[*iInfoRow], "Str : %u", pItemInfo->BaseItem_Armor.wMin_Str);

	nSize = lstrlen(szItemInfo[*iInfoRow]);

	if (*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;
#endif
}


void TooltipHelper::ItemInfoRender_Guardian(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize)
{
	int nSize = 0;

	wsprintf(szItemInfo[*iInfoRow], "%s", pItemInfo->szItemName_Kor);
	nSize = lstrlen(szItemInfo[*iInfoRow]);

	if (*nMaxSize < nSize)
		*nMaxSize = nSize;

	if (pItemInfo->GetMovable() != 0)// (귀속아이템) 표시
	{
		*iInfoRow += 1;
		strcpy(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE1428].szMessage);
		pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;
		nSize = lstrlen(szItemInfo[*iInfoRow]);
		if (*nMaxSize < nSize)
			*nMaxSize = nSize;

		*iInfoRow += 1;
	}
	else
	{
		*iInfoRow += 2;
	}

	// "무게 : %u"
	wsprintf(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE999].szMessage, pItemInfo->GetWeight());
	nSize = lstrlen(szItemInfo[*iInfoRow]);

	if (*nMaxSize < nSize)
		*nMaxSize = nSize;

	if (GUARDIAN_ITEM_TYPE_EGG == pItemInfo->BaseItem_Guardian.bType)
	{
		const auto isCurrentGuardian = true;
		//CUserInterface::GetInstance()->m_bGuardian && CUserInterface::GetInstance()->m_i64ActiveGuardianItemID == pItem->GetSerial()->i64Serial;
		if (isCurrentGuardian)
		{
			// "부화중입니다..."
			lstrcpy(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE1250].szMessage);
			pdwTextColor[*iInfoRow] = TEXT_COLOR_YELLOW;
			nSize = lstrlen(szItemInfo[*iInfoRow]);

			if (*nMaxSize < nSize)
				*nMaxSize = nSize;

			*iInfoRow += 1;
		}

		// "부화율 %d%%"
		wsprintf(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE1251].szMessage
			, DWORD((float)pItem->m_Item_Guardian.dwCompleteTime / pItemInfo->BaseItem_Guardian.wCompleteTime * 100.f));
		nSize = lstrlen(szItemInfo[*iInfoRow]);

		if (*nMaxSize < nSize)
			*nMaxSize = nSize;

		*iInfoRow += 1;

		if (pItem->m_Item_Guardian.dwEndLife)
		{
			GetDueDayText(szItemInfo[*iInfoRow], pItem->m_Item_Guardian.dwEndLife);

			nSize = lstrlen(szItemInfo[*iInfoRow]);
			pdwTextColor[*iInfoRow] = TEXT_COLOR_YELLOW;
			if (*nMaxSize < nSize)
				*nMaxSize = nSize;

			*iInfoRow += 1;
		}
	}
	else if (GUARDIAN_ITEM_TYPE_ADULT == pItemInfo->BaseItem_Guardian.bType)
	{
		DWORD dwGuardianLevel = GetGuardianItemLevel(pItem->m_Item_Guardian.dwCompleteTime);

		wsprintf(szItemInfo[*iInfoRow], "Level %d", dwGuardianLevel);
		nSize = lstrlen(szItemInfo[*iInfoRow]);

		if (*nMaxSize < nSize)
			*nMaxSize = nSize;

		*iInfoRow += 1;

		// "경험치 %d/%d"
		wsprintf(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE1252].szMessage, pItem->m_Item_Guardian.dwCompleteTime
			, GetExpTableOfLevel(OBJECT_TYPE_MONSTER, dwGuardianLevel + 1));
		nSize = lstrlen(szItemInfo[*iInfoRow]);

		if (*nMaxSize < nSize)
			*nMaxSize = nSize;

		*iInfoRow += 1;

		if (pItem->m_Item_Guardian.dwEndLife)
		{
			GetDueDayText(szItemInfo[*iInfoRow], pItem->m_Item_Guardian.dwEndLife);

			nSize = lstrlen(szItemInfo[*iInfoRow]);
			pdwTextColor[*iInfoRow] = TEXT_COLOR_YELLOW;
			if (*nMaxSize < nSize)
				*nMaxSize = nSize;

			*iInfoRow += 1;
		}

		*iInfoRow += 1;

		pdwTextColor[*iInfoRow] = TEXT_COLOR_ORANGE;
		wsprintf(szItemInfo[*iInfoRow], "EGO(%d) STR(%d) INT(%d) DEX(%d) VIT(%d)", pItem->Guardian.wStat[0], pItem->Guardian.wStat[1], pItem->Guardian.wStat[2], pItem->Guardian.wStat[3], pItem->Guardian.wStat[4]);
		*iInfoRow += 1;

		WORD wSkillLevel = pItem->bLevelH << 8 | pItem->Guardian.bLevelL;
		for (int i = 0;i < MAX_GUARDIAN_USE_SKILL;++i)
		{
			BYTE bKind = pItem->Guardian.bSkill[i];
			BYTE bLevel = (7 & (wSkillLevel >> 3 * i)) * 5;

			if (bKind == 0)
				continue;

			if (bLevel == 0)
				bLevel = 1;

			pdwTextColor[*iInfoRow] = TEXT_COLOR_GREEN;
			wsprintf(szItemInfo[*iInfoRow], "%s Lv. %d", g_pEffectLayer->m_Effect[bKind].szName, bLevel);
			*iInfoRow += 1;
		}

		*iInfoRow += 1;
	}

	LP_ITEM_OPTION lpItemOption = (LP_ITEM_OPTION)g_pItemOptionHash->GetData(pItem->GetID());

	if (lpItemOption)
	{
		for (int i = 0; i < lpItemOption->byCount; i++)
		{
			__lstrcpyn(szItemInfo[*iInfoRow], lpItemOption->szOption[i], sizeof(lpItemOption->szOption[i]));

			nSize = lstrlen(szItemInfo[*iInfoRow]);

			if (*nMaxSize < nSize)
				*nMaxSize = nSize;

			*iInfoRow += 1;
		}
	}
}


void TooltipHelper::ItemInfoRender_Supplies(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize)
{
	int nSize = 0;

	wsprintf(szItemInfo[*iInfoRow], "%s", pItemInfo->szItemName_Kor);
	nSize = lstrlen(szItemInfo[*iInfoRow]);

	if (*nMaxSize < nSize)
		*nMaxSize = nSize;

	if (pItemInfo->GetMovable() != 0)// (귀속아이템) 표시
	{
		*iInfoRow += 1;
		strcpy(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE1428].szMessage);
		pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;
		nSize = lstrlen(szItemInfo[*iInfoRow]);
		if (*nMaxSize < nSize)
			*nMaxSize = nSize;

		*iInfoRow += 1;
	}
	else
	{
		*iInfoRow += 2;
	}

	if (pItemInfo->BaseItem_Supplies.bType == 7)
	{
		wsprintf(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE1419].szMessage
			, pItemInfo->BaseItem_Supplies.wMin_Lev);
	}
	else if (pItemInfo->BaseItem_Supplies.bType == 1 && pItemInfo->BaseItem_Supplies.bType == 2)
	{
		wsprintf(szItemInfo[*iInfoRow], "%d%% / %d%%", pItemInfo->BaseItem_Supplies.wMin
			, pItemInfo->BaseItem_Supplies.wMax);
	}
	else
	{
		wsprintf(szItemInfo[*iInfoRow], "%d / %d", pItemInfo->BaseItem_Supplies.wMin
			, pItemInfo->BaseItem_Supplies.wMax);
	}

	nSize = lstrlen(szItemInfo[*iInfoRow]);

	if (*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;

	// "무게 : %u"
	wsprintf(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE999].szMessage, pItemInfo->GetWeight());
	nSize = lstrlen(szItemInfo[*iInfoRow]);

	if (*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;

	LP_ITEM_OPTION lpItemOption = (LP_ITEM_OPTION)g_pItemOptionHash->GetData(pItem->GetID());

	if (lpItemOption)
	{
		for (int i = 0; i < lpItemOption->byCount; i++)
		{
			__lstrcpyn(szItemInfo[*iInfoRow], lpItemOption->szOption[i], sizeof(lpItemOption->szOption[i]));

			nSize = lstrlen(szItemInfo[*iInfoRow]);

			if (*nMaxSize < nSize)
				*nMaxSize = nSize;

			*iInfoRow += 1;
		}
	}
}


void TooltipHelper::ItemInfoRender_Consumable(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize)
{
	int nSize = 0;

	wsprintf(szItemInfo[*iInfoRow], "%s", pItemInfo->szItemName_Kor);
	nSize = lstrlen(szItemInfo[*iInfoRow]);

	if (*nMaxSize < nSize)
		*nMaxSize = nSize;

	if (pItemInfo->GetMovable() != 0)// (귀속아이템) 표시
	{
		*iInfoRow += 1;
		strcpy(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE1428].szMessage);
		pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;
		nSize = lstrlen(szItemInfo[*iInfoRow]);
		if (*nMaxSize < nSize)
			*nMaxSize = nSize;

		*iInfoRow += 1;
	}
	else
	{
		*iInfoRow += 2;
	}

	// "무게 : %u"
	wsprintf(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE999].szMessage, pItemInfo->GetWeight());
	nSize = lstrlen(szItemInfo[*iInfoRow]);

	if (*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 2;

	if (pItem->m_Item_Consumable.bInvIndex)
	{
		// "사용중입니다."
		__lstrcpyn(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE844].szMessage, 0xff);
		nSize = lstrlen(szItemInfo[*iInfoRow]);
		pdwTextColor[*iInfoRow] = TEXT_COLOR_ORANGE;

		if (*nMaxSize < nSize)
			*nMaxSize = nSize;

		*iInfoRow += 1;

		if (pItemInfo->BaseItem_Consumable.wLast_Time)
		{
			if (ITEM_LAND_TYPE_SERVERTIME == pItemInfo->bCode_Type)// : 050108 hwoarang
				GetDueDayText(szItemInfo[*iInfoRow], pItem->m_Item_Consumable.dwEndDate);
			else if (ITEM_LAND_TYPE_PLAYTIME == pItemInfo->bCode_Type)

				nSize = lstrlen(szItemInfo[*iInfoRow]);
			pdwTextColor[*iInfoRow] = TEXT_COLOR_YELLOW;
			if (*nMaxSize < nSize)
				*nMaxSize = nSize;

			*iInfoRow += 1;
		}
	}

	LP_ITEM_OPTION lpItemOption = (LP_ITEM_OPTION)g_pItemOptionHash->GetData(pItem->GetID());

	if (lpItemOption)
	{
		for (int i = 0; i < lpItemOption->byCount; i++)
		{
			__lstrcpyn(szItemInfo[*iInfoRow], g_pItemAttrLayer->GetItemDescription(pItem->GetID(), i), 0xff);

			nSize = lstrlen(szItemInfo[*iInfoRow]);

			if (*nMaxSize < nSize)
				*nMaxSize = nSize;

			*iInfoRow += 1;
		}
	}
}


void TooltipHelper::ItemInfoRender_Zodiac(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize)
{
	int nSize = 0;

	strcpy(szItemInfo[*iInfoRow], pItemInfo->szItemName_Kor);
	nSize = lstrlen(szItemInfo[*iInfoRow]);

	if (*nMaxSize < nSize)
		*nMaxSize = nSize;

	if (pItemInfo->GetMovable() != 0)// (귀속아이템) 표시
	{
		*iInfoRow += 1;
		strcpy(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE1428].szMessage);
		pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;
		nSize = lstrlen(szItemInfo[*iInfoRow]);
		if (*nMaxSize < nSize)
			*nMaxSize = nSize;

		*iInfoRow += 1;
	}
	else
	{
		*iInfoRow += 2;
	}

	// "무게 : %u"
	wsprintf(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE999].szMessage, pItemInfo->GetWeight());
	nSize = lstrlen(szItemInfo[*iInfoRow]);

	if (*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 2;

	if (pItem->GetID() == __ITEM_INSURANCE_INDEX__)
	{
		if (pItem->m_Item_Zodiac.bInvIndex)
		{
			// 아이템 보험 사용중일때의 메시지출력
			// "사용중입니다."
			__lstrcpyn(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE844].szMessage, 0xff);
			nSize = lstrlen(szItemInfo[*iInfoRow]);
			pdwTextColor[*iInfoRow] = TEXT_COLOR_ORANGE;
			if (*nMaxSize < nSize)
				*nMaxSize = nSize;
		}
	}

	// 마을 귀환 스크롤 추가 2005.02.14 김영대 
	WORD wItemID = pItem->GetID();
	if ((wItemID == __ITEM_PORTAL_INDEX__) ||
		(wItemID >= __ITEM_PORTAL_VILL1__ && wItemID <= __ITEM_PORTAL_VILL6__))
		//if(	pItem->GetID()==__ITEM_PORTAL_INDEX__ || pItem->GetID()==__ITEM_PORTAL_VILL1__ ||
		//	pItem->GetID()==__ITEM_PORTAL_VILL2__ || pItem->GetID()==__ITEM_PORTAL_VILL3__ ||
		//	pItem->GetID()==__ITEM_PORTAL_VILL4__ || pItem->GetID()==__ITEM_PORTAL_VILL5__)
	{
		if (pItem->m_Item_Zodiac.bType == 0)
		{
			// "기록된 좌표 없음"
			strcpy(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE462].szMessage);
			nSize = lstrlen(szItemInfo[*iInfoRow]);

			if (*nMaxSize < nSize)
				*nMaxSize = nSize;

			*iInfoRow += 1;
		}
		else
		{
			DUNGEON_DATA_EX* pDungeonInfo = g_pDungeonTable->GetDungeonInfo(pItem->m_Item_Zodiac.wMapId);

			if (pDungeonInfo)
			{
#if IS_JAPAN_LOCALIZING()
				wsprintf(szItemInfo[*iInfoRow]
					, lstrlen(pDungeonInfo->m_szSchoolName) ? pDungeonInfo->m_szSchoolName : pDungeonInfo->m_szDungeonName);
#else
				__lstrcpyn(szItemInfo[*iInfoRow], pDungeonInfo->m_szDungeonName, lstrlen(pDungeonInfo->m_szDungeonName));
#endif
				nSize = lstrlen(szItemInfo[*iInfoRow]);

				if (*nMaxSize < nSize)
					*nMaxSize = nSize;

				*iInfoRow += 1;

				if (pItem->m_Item_Zodiac.wMapId / 1000 != 0)
				{
					// 던젼 층수 //
					if (pItem->m_Item_Zodiac.bLayer == 0)
					{
						// MSG_ID : 16 ; 지상 1층
						strcpy(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE16].szMessage);
					}
					else
					{
						// MSG_ID : 17 ; 지하 %d층
						wsprintf(szItemInfo[*iInfoRow]
							, g_Message[ETC_MESSAGE17].szMessage, pItem->m_Item_Zodiac.bLayer);
					}

					nSize = lstrlen(szItemInfo[*iInfoRow]);

					if (*nMaxSize < nSize)
						*nMaxSize = nSize;

					*iInfoRow += 1;
				}

				if (pItem->GetID() == __ITEM_PORTAL_INDEX__)
				{
					// "X : %u, Y : %u"
					wsprintf(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE867].szMessage, pItem->m_Item_Zodiac.wPosX, pItem->m_Item_Zodiac.wPosZ);
					nSize = lstrlen(szItemInfo[*iInfoRow]);

					if (*nMaxSize < nSize)
						*nMaxSize = nSize;

					*iInfoRow += 1;
				}
			}
			else
			{
				// "사용할 수 없습니다."
				strcpy(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE211].szMessage);
				nSize = lstrlen(szItemInfo[*iInfoRow]);

				if (*nMaxSize < nSize)
					*nMaxSize = nSize;

				*iInfoRow += 1;
			}
		}
	}
	else
	{
		LP_ITEM_OPTION lpItemOption = (LP_ITEM_OPTION)g_pItemOptionHash->GetData(pItem->GetID());

		if (lpItemOption)
		{
			for (int i = 0; i < lpItemOption->byCount; i++)
			{
				__lstrcpyn(szItemInfo[*iInfoRow], lpItemOption->szOption[i], sizeof(lpItemOption->szOption[i]));

				nSize = lstrlen(szItemInfo[*iInfoRow]);

				if (*nMaxSize < nSize)
					*nMaxSize = nSize;

				*iInfoRow += 1;
			}
		}
	}
}


void TooltipHelper::ItemInfoRender_Default(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize)
{
	int nSize = 0;

	wsprintf(szItemInfo[*iInfoRow], "%s", pItemInfo->szItemName_Kor);
	nSize = lstrlen(szItemInfo[*iInfoRow]);

	if (*nMaxSize < nSize)
		*nMaxSize = nSize;

	if (pItemInfo->GetMovable() != 0)// (귀속아이템) 표시
	{
		*iInfoRow += 1;
		strcpy(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE1428].szMessage);
		pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;
		nSize = lstrlen(szItemInfo[*iInfoRow]);
		if (*nMaxSize < nSize)
			*nMaxSize = nSize;

		*iInfoRow += 1;
	}
	else
	{
		*iInfoRow += 2;
	}

	// "무게 : %u"
	//wsprintf( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE999].szMessage,  pItemInfo->GetWeight()); 
	if ((pItem->m_wItemID >= BAG_ITEM_NUM_FRIST && pItem->m_wItemID <= BAG_ITEM_NUM_LAST))
	{
		switch (pItem->m_Item_Bag.bType)
		{
		case ITEM_BAG_TYPE_DECREASE_SMALL_BAG:
		{
			wsprintf(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE1412].szMessage, pItem->m_Item_Bag.wProbability);
		}
		break;
		case ITEM_BAG_TYPE_INCREASE_TOTAL_BAG:
		{
			wsprintf(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE1413].szMessage, pItem->m_Item_Bag.wProbability);
		}
		break;
		case ITEM_BAG_TYPE_INCREASE_TOTALPER_BAG:
		{
			wsprintf(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE1414].szMessage, pItem->m_Item_Bag.wProbability);
		}
		break;
		default:
		{
			wsprintf(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE999].szMessage, pItem->m_Item_Bag.wProbability);
		}
		}
		nSize = lstrlen(szItemInfo[*iInfoRow]);
	}
	else
	{
		wsprintf(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE999].szMessage, pItem->m_Item_Bag.wProbability);
		nSize = lstrlen(szItemInfo[*iInfoRow]);
	}

	//pdwTextColor[*iInfoRow] = TEXT_COLOR_ORANGE;

	if (*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 2;

	LP_ITEM_OPTION lpItemOption = (LP_ITEM_OPTION)g_pItemOptionHash->GetData(pItem->GetID());

	if (lpItemOption)
	{
		for (int i = 0; i < lpItemOption->byCount; i++)
		{
			__lstrcpyn(szItemInfo[*iInfoRow], lpItemOption->szOption[i], sizeof(lpItemOption->szOption[i]));

			nSize = lstrlen(szItemInfo[*iInfoRow]);
			//pdwTextColor[*iInfoRow] = TEXT_COLOR_ORANGE;
			if (*nMaxSize < nSize)
				*nMaxSize = nSize;

			*iInfoRow += 1;
		}
	}
	if ((pItem->m_wItemID >= BAG_ITEM_NUM_FRIST && pItem->m_wItemID <= BAG_ITEM_NUM_LAST))
	{
		wsprintf(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE347].szMessage, pItemInfo->BaseItem_Bag.wMin_Lev);
		if (g_pMainPlayer && g_pMainPlayer->currentLevel() < pItemInfo->BaseItem_Bag.wMin_Lev)	pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;
		*iInfoRow += 1;
		nSize = lstrlen(szItemInfo[*iInfoRow]);
		if (*nMaxSize < nSize)	*nMaxSize = nSize;
	}

}


void TooltipHelper::ItemInfoRender_Option(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor
	, int* nMaxSize, int* iOption)
{
	int nSize = 0;

	if (*iOption != 0)
	{
		wsprintf(szItemInfo[*iInfoRow], "    ");
		nSize = lstrlen(szItemInfo[*iInfoRow]);

		if (*nMaxSize < nSize)
			*nMaxSize = nSize;

		*iInfoRow += 1;

		for (BYTE i = 0; i < MAX_ITEM_ATTR; i++)
		{
			// 효과 수치가 0이면 무시!
			if (0 == pItem->wAttr[i].uValue) continue;

			__lstrcpyn(szItemInfo[*iInfoRow]
				, g_pItemAttrLayer->GetAttrOptionDescription(pItem->wAttr[i].uCode, pItem->wAttr[i].uValue), 0xff);

			pdwTextColor[*iInfoRow] = TEXT_COLOR_BLUE;
			nSize = lstrlen(szItemInfo[*iInfoRow]);

			if (*nMaxSize < nSize)
				*nMaxSize = nSize;

			*iInfoRow += 1;
		}
	}
}


void TooltipHelper::ItemInfoRender_SetItemBonusOption(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor
	, int* nMaxSize, BOOL isEquip, DWORD dwItemSetID, DWORD dwSetNum, LPSET_ITEM_INFO pSetItemInfo)
{
	int nSize = 0;

	if (isEquip && dwItemSetID)
	{
		int iLoop = 0;

		if (dwSetNum >= 2)
		{
			// "추가 옵션"
			wsprintf(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE589].szMessage);
			pdwTextColor[*iInfoRow] = TEXT_COLOR_ORANGE;
			*iInfoRow += 1;

			nSize = lstrlen(szItemInfo[*iInfoRow]);

			if (*nMaxSize < nSize)
				*nMaxSize = nSize;

			iLoop = dwSetNum - 1;
			if (dwSetNum > 4) iLoop = 4;

			for (int i = 0; i < iLoop; i++)
			{
				switch (pItemInfo->dwCode_ID)
				{
				case ITEM_KIND_WEAPON:
				{
					if (pItemInfo->BaseItem_Weapon.pPartOption[i].wKind == 0)	continue;
					if (pItemInfo->BaseItem_Weapon.pPartOption[i].wValue == 0)	continue;

					__lstrcpyn(szItemInfo[*iInfoRow]
						, g_pItemAttrLayer->GetAttrOptionDescription(pItemInfo->BaseItem_Weapon.pPartOption[i].wKind
							, pItemInfo->BaseItem_Weapon.pPartOption[i].wValue), 0xff);

					pdwTextColor[*iInfoRow] = TEXT_COLOR_GREEN;
					nSize = lstrlen(szItemInfo[*iInfoRow]);

					if (*nMaxSize < nSize)
						*nMaxSize = nSize;

					*iInfoRow += 1;
				}
				break;

				case ITEM_KIND_AROMR:
				{
					if (pItemInfo->BaseItem_Armor.pPartOption[i].wKind == 0) continue;
					if (pItemInfo->BaseItem_Armor.pPartOption[i].wValue == 0)
					{
						// 추가값이 0이어두 무시.. by deepdark.
						continue;
					}

					__lstrcpyn(szItemInfo[*iInfoRow]
						, g_pItemAttrLayer->GetAttrOptionDescription(pItemInfo->BaseItem_Armor.pPartOption[i].wKind
							, pItemInfo->BaseItem_Armor.pPartOption[i].wValue), 0xff);

					pdwTextColor[*iInfoRow] = TEXT_COLOR_GREEN;
					nSize = lstrlen(szItemInfo[*iInfoRow]);

					if (*nMaxSize < nSize)
						*nMaxSize = nSize;

					*iInfoRow += 1;
				}
				break;
				}
			}

			// Set Item Bonus 옵션.
			wsprintf(szItemInfo[*iInfoRow], "       ");
			nSize = lstrlen(szItemInfo[*iInfoRow]);

			if (*nMaxSize < nSize)
				*nMaxSize = nSize;

			*iInfoRow += 1;

			// "세트아이템 보너스 옵션"
			wsprintf(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE590].szMessage);
			pdwTextColor[*iInfoRow] = TEXT_COLOR_ORANGE;
			nSize = lstrlen(szItemInfo[*iInfoRow]);

			if (*nMaxSize < nSize)
				*nMaxSize = nSize;

			*iInfoRow += 1;

			iLoop = dwSetNum - 1;
			if (dwSetNum > 5) iLoop = 5;

			for (int i = 0; i < iLoop; i++)
			{
				if (pSetItemInfo->pBonusOption[i][0].wKind != 0 && pSetItemInfo->pBonusOption[i][0].wValue != 0)
				{
					__lstrcpyn(szItemInfo[*iInfoRow]
						, g_pItemAttrLayer->GetAttrOptionDescription(pSetItemInfo->pBonusOption[i][0].wKind
							, pSetItemInfo->pBonusOption[i][0].wValue), 0xff);

					pdwTextColor[*iInfoRow] = TEXT_COLOR_GREEN;
					nSize = lstrlen(szItemInfo[*iInfoRow]);

					if (*nMaxSize < nSize)
						*nMaxSize = nSize;

					*iInfoRow += 1;
				}

				if (pSetItemInfo->pBonusOption[i][1].wKind != 0 && pSetItemInfo->pBonusOption[i][0].wValue != 0)
				{
					__lstrcpyn(szItemInfo[*iInfoRow]
						, g_pItemAttrLayer->GetAttrOptionDescription(pSetItemInfo->pBonusOption[i][1].wKind
							, pSetItemInfo->pBonusOption[i][1].wValue), 0xff);

					pdwTextColor[*iInfoRow] = TEXT_COLOR_GREEN;
					nSize = lstrlen(szItemInfo[*iInfoRow]);

					if (*nMaxSize < nSize)
						*nMaxSize = nSize;

					*iInfoRow += 1;
				}
			}

			// Full Set Bonus 옵션.
			if (dwSetNum == pSetItemInfo->bFullSet)
			{
				for (int i = 0; i < MAX_ITEM_ATTR; i++)
				{
					if (pSetItemInfo->pFullOption[i].wKind != 0 && pSetItemInfo->pFullOption[i].wValue != 0)
					{
						__lstrcpyn(szItemInfo[*iInfoRow]
							, g_pItemAttrLayer->GetAttrOptionDescription(pSetItemInfo->pFullOption[i].wKind
								, pSetItemInfo->pFullOption[i].wValue), 0xff);

						pdwTextColor[*iInfoRow] = TEXT_COLOR_GREEN;
						nSize = lstrlen(szItemInfo[*iInfoRow]);

						if (*nMaxSize < nSize)
							*nMaxSize = nSize;

						*iInfoRow += 1;
					}
				}
			}
		}
	}
}


void TooltipHelper::ItemInfoRender(CItem* pItem, BOOL isEquip, BOOL bPrice, DWORD dwPrice, BYTE byZipCode, BYTE byOption)
{
	char			szItemInfo[100][255] = { 0, };
	char			szUpgradeOption[10][255] = { 0, };

	int				iOption = 0;
	int				iInfoRow = 0;
	int				nMaxSize = 0;
	int				nSize = 0;

	DWORD			dwSetNum = 0;
	DWORD			dwItemSetID = 0;
	DWORD			dwEquipSetID = 0;
	DWORD			dwSetID[10] = { 0, };
	DWORD			pdwTextColor[100] = { 0, };
	DWORD			dwUpgradeIndex = 0;
	DWORD			dwUpgradeTextColor[10] = { 0, };

	CBaseItem* pItemInfo = NULL;
	LPSET_ITEM_INFO pSetItemInfo = NULL;
	CBaseItem* pItemTmp = NULL;

	if (g_Mouse.bLDown == TRUE)
		return;

	for (int i = 0; i < 50; i++)
		memset(szItemInfo[i], 0, sizeof(szItemInfo[i]));
	memset(dwSetID, 0, sizeof(dwSetID));
	memset(pdwTextColor, 0xff, sizeof(DWORD) * 50);

	pItemInfo = g_pItemTableHash_get()->GetData(pItem->m_wItemID);
	if (NULL == pItemInfo)
		return;

	if (pItem->GetType() == ITEM_LARGE)
		dwItemSetID = pItemInfo->GetSetID();

	if (dwItemSetID != 0 && dwItemSetID != 32767)
	{
		pSetItemInfo = g_pSetItemInfoHash->GetData(dwItemSetID);

		if (isEquip)
		{
			BOOL isChk = FALSE;
			memset(dwSetID, 0, sizeof(DWORD) * 10);

			for (int i = 0; i < MAX_EQUIP_POOL; i++)
			{
				if (g_pMainPlayer && g_pMainPlayer->m_bCurrnetHand == 1)
				{
					if (i == EQUIP_TYPE_RHAND2 || i == EQUIP_TYPE_LHAND2)
						continue;
				}
				else if (g_pMainPlayer && g_pMainPlayer->m_bCurrnetHand == 2)
				{
					if (i == EQUIP_TYPE_RHAND1 || i == EQUIP_TYPE_LHAND1)
						continue;
				}

				if (i == EQUIP_TYPE_AMULE1 || i == EQUIP_TYPE_AMULE3 || i == EQUIP_TYPE_LRING1 ||
					i == EQUIP_TYPE_LRING2 || i == EQUIP_TYPE_LRING3)
					continue;

				if (g_pMainPlayer && g_pMainPlayer->CheckItem(&g_pMainPlayer->m_pEquip[i]) == FALSE)
					continue;

				if (g_pMainPlayer && g_pMainPlayer->m_pEquip[i].m_wItemID == NULL)
					continue;

				pItemTmp = g_pItemTableHash_get()->GetData(g_pMainPlayer->m_pEquip[i].m_wItemID);
				dwEquipSetID = pItemTmp->GetSetID();

				if (dwItemSetID != dwEquipSetID) continue;

				isChk = FALSE;

				for (int j = 0; j < 10; j++)
				{
					if (dwSetID[j] == 0) break;
					if (dwSetID[j] == pItemTmp->wID)
					{
						isChk = TRUE;
						break;
					}
				}

				if (!isChk)
				{
					dwSetID[dwSetNum] = pItemTmp->wID;
					dwSetNum++;
				}
			}
		}
	}

	int i = 0;

	while (i < MAX_ITEM_ATTR)
	{
		if (pItem->wAttr[i].uCode != 0)
			iOption++;

		i++;
	}

	switch (pItemInfo->dwCode_ID)
	{
	case ITEM_KIND_WEAPON:
		ItemInfoRender_Weapon(szItemInfo, &iInfoRow, pItem, pItemInfo, pSetItemInfo
			, dwItemSetID, dwSetNum, pdwTextColor, isEquip, iOption, &nMaxSize);
		break;
	case ITEM_KIND_AROMR:
		ItemInfoRender_Armor(szItemInfo, &iInfoRow, pItem, pItemInfo, pSetItemInfo
			, dwItemSetID, dwSetNum, pdwTextColor, isEquip, iOption, &nMaxSize, byZipCode);
		break;
	case ITEM_KIND_GUARDIAN:
		ItemInfoRender_Guardian(szItemInfo, &iInfoRow, pItem, pItemInfo, pdwTextColor, &nMaxSize);
		break;
	case ITEM_KIND_SUPPLIES:
		ItemInfoRender_Supplies(szItemInfo, &iInfoRow, pItem, pItemInfo, pdwTextColor, &nMaxSize);
		break;
	case ITEM_KIND_CONSUMABLE:
		ItemInfoRender_Consumable(szItemInfo, &iInfoRow, pItem, pItemInfo, pdwTextColor, &nMaxSize);
		break;
	case ITEM_KIND_ZODIAC:
		ItemInfoRender_Zodiac(szItemInfo, &iInfoRow, pItem, pItemInfo, pdwTextColor, &nMaxSize);
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
		ItemInfoRender_Default(szItemInfo, &iInfoRow, pItem, pItemInfo, pdwTextColor, &nMaxSize);
		break;
	default:
		return;
	}

	if (pItemInfo->GetMovable() != 0)// (귀속아이템) 표시
	{
		strcpy(szItemInfo[iInfoRow], g_Message[ETC_MESSAGE1429].szMessage);//본인계정 외 캐릭터에게는 양도가 금지됩니다.
		nSize = lstrlen(szItemInfo[iInfoRow]);
		if (nMaxSize < nSize)
			nMaxSize = nSize;

		iInfoRow += 1;
	}

	if (pItemInfo->dwCode_ID != ITEM_KIND_GUARDIAN)
	{
		ItemInfoRender_Option(szItemInfo, &iInfoRow, pItem, pItemInfo, pdwTextColor, &nMaxSize, &iOption);
	}

	ItemInfoRender_SetItemBonusOption(szItemInfo, &iInfoRow, pItem, pItemInfo, pdwTextColor, &nMaxSize, isEquip, dwItemSetID, dwSetNum, pSetItemInfo);

	if (isEquip && byOption)
	{
		BYTE byOptionValue[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

		switch (byOption)
		{
		case 6:
			byOptionValue[0] = 31;	byOptionValue[1] = 10;	byOptionValue[2] = 12;	byOptionValue[3] = 5;	byOptionValue[4] = 14;	byOptionValue[5] = 5;
			byOptionValue[6] = 3;	byOptionValue[7] = 8;
			break;
		case 7:
			byOptionValue[0] = 31;	byOptionValue[1] = 15;	byOptionValue[2] = 12;	byOptionValue[3] = 7;	byOptionValue[4] = 14;	byOptionValue[5] = 7;
			byOptionValue[6] = 3;	byOptionValue[7] = 10;	byOptionValue[8] = 23;	byOptionValue[9] = 1;
			break;
		case 8:
			byOptionValue[0] = 31;	byOptionValue[1] = 25;	byOptionValue[2] = 12;	byOptionValue[3] = 10;	byOptionValue[4] = 14;	byOptionValue[5] = 10;
			byOptionValue[6] = 3;	byOptionValue[7] = 14;	byOptionValue[8] = 23;	byOptionValue[9] = 2;
			break;
		case 9:
			byOptionValue[0] = 31;	byOptionValue[1] = 40;	byOptionValue[2] = 12;	byOptionValue[3] = 15;	byOptionValue[4] = 14;	byOptionValue[5] = 15;
			byOptionValue[6] = 3;	byOptionValue[7] = 20;	byOptionValue[8] = 23;	byOptionValue[9] = 3;
			break;
		case 10:
			byOptionValue[0] = 31;	byOptionValue[1] = 60;	byOptionValue[2] = 12;	byOptionValue[3] = 25;	byOptionValue[4] = 14;	byOptionValue[5] = 25;
			byOptionValue[6] = 3;	byOptionValue[7] = 30;	byOptionValue[8] = 23;	byOptionValue[9] = 4;
			break;
		}

		for (int i = 0; i < 5; i++)
		{
			if (byOptionValue[i * 2] == 0)
				continue;

			__lstrcpyn(szUpgradeOption[dwUpgradeIndex]
				, g_pItemAttrLayer->GetAttrOptionDescription(byOptionValue[i * 2], byOptionValue[i * 2 + 1]), 0xff);

			dwUpgradeTextColor[dwUpgradeIndex] = TEXT_COLOR_GREEN;

			nSize = lstrlen(szUpgradeOption[dwUpgradeIndex]);

			if (nMaxSize < nSize)
				nMaxSize = nSize;

			dwUpgradeIndex++;
		}
	}

	CStoreWnd* pStoreWnd = CStoreWnd::GetInstance();

	if (pStoreWnd->GetActive() == TRUE)
	{
		char		szInfo[0xff] = { 0, };
		char		szPrice[0xff] = { 0, };
		NUMBERFMT	nFmt = { 0, 0, 3, ".", ",", 1 };

		wsprintf(szInfo, "%u", pItemInfo->GetSellPrice());
		::GetNumberFormat(NULL, NULL, szInfo, &nFmt, szPrice, 15);

		// "%s Karz"
		wsprintf(szItemInfo[iInfoRow], g_Message[ETC_MESSAGE870].szMessage, szPrice);

		if (pItemInfo->dwCode_ID != ITEM_KIND_RIDE)
		{
			nSize = lstrlen(szItemInfo[iInfoRow]);

			if (nMaxSize < nSize)
				nMaxSize = nSize;

			iInfoRow++;
		}
	}

	int nOverXSize = 0;
	int nOverYSize = 0;
	int nInfoIndex = iInfoRow;

	if (bPrice)
		nInfoIndex += 2;

	if (isEquip && byOption)
		nInfoIndex += 8;

	if (g_Mouse.MousePos.y + 5 + (nInfoIndex * 18) + 14 > windowHeight())
		nOverYSize = g_Mouse.MousePos.y + 5 + (nInfoIndex * 18) + 14 - windowHeight();

	if ((g_Mouse.MousePos.x + ITEMINFO_RENDER_POSITION) + (float)(nMaxSize * 6.5f + 10) > windowWidth())
		nOverXSize = (int)((g_Mouse.MousePos.x + ITEMINFO_RENDER_POSITION) + (float)(nMaxSize * 6.5f + 10) - windowWidth());

	if (bPrice)
	{
		iInfoRow++;

		char		szInfo[0xff] = { 0, };
		char		szPrice[0xff] = { 0, };
		NUMBERFMT	nFmt = { 0, 0, 3, ".", ",", 1 };

		wsprintf(szInfo, "%u", dwPrice);
		::GetNumberFormat(NULL, NULL, szInfo, &nFmt, szPrice, 15);

		if (GetType(pItem->GetID() / ITEM_DISTRIBUTE) == ITEM_SMALL)
		{
			// "1 개당 %s Karz"
			wsprintf(szItemInfo[iInfoRow], g_Message[ETC_MESSAGE812].szMessage, szPrice);
		}
		else
		{
			// "%s Karz"
			wsprintf(szItemInfo[iInfoRow], g_Message[ETC_MESSAGE870].szMessage, szPrice);
		}

		nSize = lstrlen(szItemInfo[iInfoRow]);

		if (nMaxSize < nSize)
			nMaxSize = nSize;

		iInfoRow++;
	}

	for (i = 0; i < iInfoRow; i++)
	{
		if (__strcmp(szItemInfo[i], "") != 0)
		{
			nSize = lstrlen(szItemInfo[i]);

			RenderFont(szItemInfo[i]
				, g_Mouse.MousePos.x + 10 + ITEMINFO_RENDER_POSITION - nOverXSize
				, g_Mouse.MousePos.x + 10 + ITEMINFO_RENDER_POSITION + nSize * 7 - nOverXSize
				, g_Mouse.MousePos.y + 5 + (i * 18) - nOverYSize
				, g_Mouse.MousePos.y + 5 + (i * 18) + 14 - nOverYSize
				, __ORDER_ITEM_DESC__ + 3
				, pdwTextColor[i]);
		}
	}

	if (isEquip && byOption)
	{
		switch (byOption)
		{
		case 6:		wsprintf(szUpgradeOption[9], g_Message[ETC_MESSAGE1132].szMessage);		break; // "+6 페오 마스터"
		case 7:		wsprintf(szUpgradeOption[9], g_Message[ETC_MESSAGE1133].szMessage);		break; // "+7 윈 마스터"
		case 8:		wsprintf(szUpgradeOption[9], g_Message[ETC_MESSAGE1134].szMessage);		break; // "+8 오셀 마스터"
		case 9:		wsprintf(szUpgradeOption[9], g_Message[ETC_MESSAGE1135].szMessage);		break; // "+9 제라 마스터"
		case 10:	wsprintf(szUpgradeOption[9], g_Message[ETC_MESSAGE1136].szMessage);		break; // "+10 베오르크 마스터"
		}

		nSize = lstrlen(szUpgradeOption[9]);

		RenderFont(szUpgradeOption[9]
			, g_Mouse.MousePos.x + 10 + ITEMINFO_RENDER_POSITION - nOverXSize
			, g_Mouse.MousePos.x + 10 + ITEMINFO_RENDER_POSITION + nSize * 7 - nOverXSize
			, g_Mouse.MousePos.y + 5 + ((iInfoRow + 1) * 18) - nOverYSize
			, g_Mouse.MousePos.y + 5 + ((iInfoRow + 1) * 18) + 14 - nOverYSize
			, __ORDER_ITEM_DESC__ + 3
			, TEXT_COLOR_GREEN);

		if (nMaxSize < nSize)
			nMaxSize = nSize;

		iInfoRow += 2;

		for (i = iInfoRow + 1; i < iInfoRow + 6; i++)
		{
			if (__strcmp(szUpgradeOption[i - (iInfoRow + 1)], "") != 0)
			{
				nSize = lstrlen(szUpgradeOption[i - (iInfoRow + 1)]);

				RenderFont(szUpgradeOption[i - (iInfoRow + 1)]
					, g_Mouse.MousePos.x + 10 + ITEMINFO_RENDER_POSITION - nOverXSize
					, g_Mouse.MousePos.x + 10 + ITEMINFO_RENDER_POSITION + nSize * 7 - nOverXSize
					, g_Mouse.MousePos.y + 5 + (i * 18) - nOverYSize
					, g_Mouse.MousePos.y + 5 + (i * 18) + 14 - nOverYSize
					, __ORDER_ITEM_DESC__ + 3
					, dwUpgradeTextColor[i - (iInfoRow + 1)]);
			}
		}

		iInfoRow += 6;
	}

	RenderInfoBox(float(g_Mouse.MousePos.x + ITEMINFO_RENDER_POSITION) - nOverXSize
		, float(g_Mouse.MousePos.y - 5) - nOverYSize
		, (float)(nMaxSize * 6.5f + 10)
		, (float)(iInfoRow * 18 + 10)
		, __ORDER_ITEM_DESC__);
}



static void GetDueDayText(char* pszItemInfo, DWORD dwDueDay)
{
	struct tm* when;
	when = localtime((time_t*)&dwDueDay);

	if (when)//널 포인터 참조 수정 : 050103 hwoarang
	{
		// "유효기간 %04d/%02d/%02d %02d:%02d분까지입니다."
		wsprintf(pszItemInfo, g_Message[ETC_MESSAGE845].szMessage, when->tm_year + 1900, when->tm_mon + 1, when->tm_mday, when->tm_hour, when->tm_min);
	}
}