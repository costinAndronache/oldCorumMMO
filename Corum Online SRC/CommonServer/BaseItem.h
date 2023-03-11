#pragma once


#include "CommonServer.h"


#pragma pack(push,1)


struct SET_ITEM_OPTION_1
{
	WORD	wKind;
	WORD	wMin;
	WORD	wMax;
};


struct SET_ITEM_OPTION_2
{
	WORD	wKind;
	WORD	wValue;
};


typedef struct BASEITEM_WEAPON
{	
	BYTE				bWeaponKind;
	BYTE				bHand;
	BYTE				bItemGrade;
	BYTE				bClassGrade;
	WORD				wSetID;
	BYTE				byDropBox;
	BYTE				bRaceID;
	BYTE				bClassID;
	WORD				wWeight;		
	WORD				wMin_Damage;
	WORD                wMax_Damage;
	WORD				wMana_Damage;
	WORD				wMin_Str;
	WORD				wMin_Dex;
	WORD				wMin_Level;
	WORD				wMin_Slot;
	WORD				wMax_Slot;
	WORD				wBase_Speed;
	WORD				wMax_Speed;
	WORD				wRange;	
	SET_ITEM_OPTION_1	pBaseSetItemOption[6];
	SET_ITEM_OPTION_2	pPartOption[4];	
	WORD				wM_QTT;
	WORD				wG_QTT;
	WORD				wW_QTT;
	WORD				wL_QTT;
	BYTE				bMaterial;
	DWORD				dwPrice;
	DWORD				dwSellPrice;	
	BYTE				bIVIconCount;			
	BYTE				bIVIndex;				
	BYTE				bModCount;				
	
} *LPBASEITEM_WEAPON;


typedef struct BASEITEM_ARMOR
{
	BYTE				bArmorKind;
	BYTE				bItemGrade;
	BYTE				bClassGrade;
	WORD				wSetID;
	BYTE				byDropBox;
	BYTE				bRaceID;
	BYTE				bClassID;	
	WORD				wWeight;
	WORD				wMin_Damage;
	WORD                wMax_Damage;
	WORD				wMin_Str;
	WORD				wMin_Level;
	WORD				wMin_Slot;
	WORD				wMax_Slot;	
	SET_ITEM_OPTION_1	pBaseSetItemOption[6];
	SET_ITEM_OPTION_2	pPartOption[4];
	WORD				wM_QTT;
	WORD				wG_QTT;
	WORD				wW_QTT;
	WORD				wL_QTT;
	BYTE				bMaterial;
	DWORD				dwPrice;
	DWORD				dwSellPrice;	
	BYTE				bIVIconCount;
	BYTE				bIVIndex;	
	BYTE				bModCount;

} *LPBASEITEM_ARMOR;


typedef struct BASEITEM_SPECIAL
{
	BYTE				bClassGrade;	
	WORD				wWeight;
	DWORD				dwPrice;
	DWORD				dwSellPrice;
	BYTE				bIVIconCount;			
	BYTE				bIVIndex;				
	BYTE				bModCount;
	BYTE				bOverCnt;			
	BYTE				bQuantityCnt;			

} *LPBASEITEM_SPECIAL;


struct BASEITEM_CONSUMABLE_ITEMATTR
{
	WORD				wAttr_Code;
	WORD				wAttr_Value;
};


typedef struct BASEITEM_CONSUMABLE
{
	BASEITEM_CONSUMABLE_ITEMATTR	sItem_Attr[5];
	WORD				wLast_Time;
	WORD				wMin_Lev;
	WORD				wMax_Lev;
	WORD				wWeight;
	DWORD				dwPrice;
	DWORD				dwSellPrice;	
	BYTE				bIVIconCount;			
	BYTE				bIVIndex;				
	BYTE				bModCount;
	BYTE				bOverCnt;				
	BYTE				bQuantityCnt;			

} *LPBASEITEM_CONSUMABLE;


typedef struct BASEITEM_SUPPLIES
{
	BYTE				bType;					
	WORD				wMin;					
	WORD				wMax;
	WORD				wWeight;	
	DWORD				dwPrice;
	DWORD				dwSellPrice;		
	BYTE				bIVIconCount;			
	BYTE				bIVIndex;				
	BYTE				bModCount;
	BYTE				bOverCnt;				
	BYTE				bQuantityCnt;			
	WORD				wMin_Lev;
	WORD				wMax_Lev;
} *LPBASEITEM_SUPPLIES;


typedef struct BASEITEM_ZODIAC
{
	WORD				wWeight;	
	BYTE				bGrade;
	WORD				wMin_Lev;
	WORD				wDifficulty;
	DWORD				dwRarity;
	DWORD				dwPrice;
	DWORD				dwSellPrice;
	BYTE				bIVIconCount;			
	BYTE				bIVIndex;				
	BYTE				bModCount;
	BYTE				bOverCnt;				
	BYTE				bQuantityCnt;			
	
} *LPBASEITEM_ZODIAC;


typedef struct BASEITEM_RIDE
{
	BYTE				bGrade;	
	WORD				wWeight;
	DWORD				dwPrice;
	DWORD				dwSellPrice;
	BYTE				bIVIconCount;			
	BYTE				bIVIndex;				
	BYTE				bModCount;				
	
} *LPBASEITEM_RIDE;


typedef struct BASEITEM_GUARDIAN
{
	BYTE				bClassGrade;	
	WORD				wWeight;	
	BYTE				bType;					// �ʶ� : 1
	WORD				wCompleteTime;			// ������ ����.
	DWORD				dwDueDayTime;			// ������ ����. ������
	WORD				wCreatureID;			// ��Ȱ������ ����� ���̵�
	WORD				wDieItemID;				// �׾����� ����� ���̵�
	WORD				wBrokenItemID;			// �������� ����� ���̵�
	WORD				wSoulItemID;			// �����̾����� ������ ��ȥ���� ����.
	WORD				wDyingPenalty;			// ������ �׾����� ������ ���δ�.
	WORD				wLvLifeUp;				// ������ ���� ����
	WORD				wBaseGuardianID;
	DWORD				dwPrice;
	DWORD				dwSellPrice;	
	BYTE				bIVIconCount;			
	BYTE				bIVIndex;				
	BYTE				bModCount;				

} *LPBASEITEM_GUARDIAN;


typedef struct BASEITEM_MAGICARRAY
{
	BYTE				bSubID;					// ������̶�� ����ų �迭 �ε���
	BYTE				bGrade;	
	WORD				wWeight;
	BYTE				bClassId;
	BYTE				bRaceId;
	WORD				wMinLev;
	WORD				wMinRank;
	WORD				wMagicId1;
	WORD				wPoint1;
	DWORD				dwDur1;
	WORD				wMagicId2;
	WORD				wPoint2;
	DWORD				dwDur2;
	WORD				wMagicId3;
	WORD				wPoint3;
	DWORD				dwDur3;		
	DWORD				dwPrice;
	DWORD				dwSellPrice;		
	BYTE				bIVIconCount;			
	BYTE				bIVIndex;				
	BYTE				bModCount;
	BYTE				bOverCnt;				
	BYTE				bQuantityCnt;			
	
} *LPBASEITEM_MAGICARRAY;


typedef struct BASEITEM_MATERIALS
{
	BYTE				bGrade;
	BYTE				byOrder;
	WORD				wPower;
	WORD				wBias;
	BYTE				bSmin;
	BYTE				bSmax;	
	WORD				wWeight;	
	BYTE				bLevADD;
	BYTE				bDurability;
	BYTE				bDifficulty;	
	DWORD				dwPrice;
	DWORD				dwSellPrice;	
	BYTE				bIVIconCount;			
	BYTE				bIVIndex;				
	BYTE				bModCount;	
	BYTE				bOverCnt;
	BYTE				bQuantityCnt;			
	
} *LPBASEITEM_MATERIALS;


typedef struct BASEITEM_MIX_UPGRADE
{
	BYTE				bGrade;	
	WORD				wWeight;
	WORD				wMin_Lev;
	WORD				wDurability;
	WORD				wDifficulty;	
	DWORD				dwPrice;
	DWORD				dwSellPrice;
	BYTE				bIVIconCount;			
	BYTE				bIVIndex;				
	BYTE				bModCount;	
	BYTE				bOverCnt;	
	BYTE				bQuantityCnt;			
	
} *LPBASEITEM_MIX_UPGRADE;


struct VALUE_BASEITEM_MAGICFIELD_ARRAY
{
	WORD				wID;
	BYTE				bWho;
	short				nValue;
	BYTE				bFormula;
};


typedef struct BASEITEM_MAGICFIELD_ARRAY
{
	WORD				wWeight;	
	WORD				wMin_Lv;
	VALUE_BASEITEM_MAGICFIELD_ARRAY sValue[5];
	DWORD				dwPrice;
	DWORD				dwSellPrice;
	BYTE				bIVIconCount;			
	BYTE				bIVIndex;				
	BYTE				bModCount;	
	BYTE				bOverCnt;
	BYTE				bQuantityCnt;			

} *LPBASEITEM_MAGICFIELD_ARRAY;



typedef struct BASEITEM_UPGRADE
{
	WORD				wWeight;	
	BYTE				bclassfication;	
	WORD				wProbability_minus;	
	WORD				wR1_plus;
	WORD				wR2_plus;	
	WORD				wW_Grade;	
	WORD				wFormula[18];	
	DWORD				dwPrice;
	DWORD				dwSellPrice;	
	BYTE				bIVIconCount;			
	BYTE				bIVIndex;				
	BYTE				bModCount;	
	BYTE				bOverCnt;
	BYTE				bQuantityCnt;			
} *LPBASEITEM_UPGRADE;


typedef struct BASEITEM_LIQUID 
{
	WORD				wWeight;	
	BYTE				bclassfication;	
	WORD				w1_slot;
	WORD				wfluent_a;	
	WORD				w2_slot;
	WORD				wfluent_b;	
	WORD				wfluent_c;	
	WORD				wW_Grade;	
	DWORD				dwPrice;
	DWORD				dwSellPrice;	
	BYTE				bIVIconCount;			
	BYTE				bIVIndex;				
	BYTE				bModCount;	
	BYTE				bOverCnt;
	BYTE				bQuantityCnt;			
} *LPBASEITEM_LIQUID;


typedef struct BASEITEM_EDITION 
{
	WORD				wWeight;	
	WORD				wOption;
	WORD				wProbability_plus;	
	WORD				wLiquid_a_min;
	WORD				wLiquid_a_max;	
	WORD				wLiquid_b_min;
	WORD				wLiquid_b_max;	
	WORD				wFormula[18];	
	DWORD				dwPrice;
	DWORD				dwSellPrice;	
	BYTE				bIVIconCount;			
	BYTE				bIVIndex;				
	BYTE				bModCount;	
	BYTE				bOverCnt;
	BYTE				bQuantityCnt;			
} *LPBASEITEM_EDITION;


typedef struct BASEITEM_BAG
{
	BYTE				bType;
	WORD				wValue_Min;
	WORD				WValue_Max;
	WORD				wMin_Lev;
	DWORD				dwPrice;
	DWORD				dwSellPrice;	
	BYTE				bIVIconCount;
	BYTE				bIVIndex;	
	BYTE				bModCount;
	
} *LPBASEITEM_BAG;


typedef struct BASEITEM_LAND
{
	WORD				BaseItemID;
	BYTE				byLandType;		// 1: �÷��� �ð� 2: ���� �ð� 
	WORD				wLandTime;		// �д��� 
} *LPBASEITEM_LAND;


class DECLSPECIFIER CBaseItem
{
public:

	WORD	wID;
	char	szItemName_Kor[50];		
	char	szItemName_Eng[35];
	DWORD	dwCode_ID;
	BYTE	bCode_Type;
	BYTE	bRandItem;
	BYTE	bMovable;// 0:�������, 1:â��, ����, ���� �������̽��� �̵� ����, �ǸŰ���(�ŷ�,����,��� ����), 2:������ �ȱ⸸ ����(ĳ���� �������� �̵�)

	union
	{
		BASEITEM_WEAPON				BaseItem_Weapon;
		BASEITEM_ARMOR				BaseItem_Armor;
		BASEITEM_SUPPLIES			BaseItem_Supplies;		
		BASEITEM_RIDE				BaseItem_Ride;
		BASEITEM_SPECIAL			BaseItem_Special;		
		BASEITEM_ZODIAC				BaseItem_Zodiac;
		BASEITEM_GUARDIAN			BaseItem_Guardian;
		BASEITEM_MAGICARRAY			BaseItem_MagicArray;
		BASEITEM_MATERIALS			BaseItem_Materials;
		BASEITEM_MIX_UPGRADE		BaseItem_MixUpgrade;
		BASEITEM_MAGICFIELD_ARRAY	BaseItem_MagicFieldArray;
		BASEITEM_CONSUMABLE			BaseItem_Consumable;
		BASEITEM_UPGRADE			BaseItem_UpGrade;
		BASEITEM_LIQUID				BaseItem_LiQuid;
		BASEITEM_EDITION			BaseItem_Edition;
		BASEITEM_BAG				BaseItem_Bag;
		BASEITEM_LAND				BaseItem_Land;
	};

	WORD	GetID			();
	BYTE	GetClass		();
	DWORD	GetSellPrice	();
	DWORD	GetPrice		();
	BYTE	GetMaxQuantity	();
	BYTE	GetGrade		();
	BYTE	GetWeaponKind	();
	BYTE	GetWeaponHand	();
	WORD	GetSetID		();
	WORD	GetWeight		();
	BYTE	GetOverlap		();
	BYTE	GetLandItemType	();
	WORD	GetLastTime		();
	BYTE	GetMovable		();
};


#pragma pack(pop)