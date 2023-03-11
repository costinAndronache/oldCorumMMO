#pragma once

#define MAX_ITEM_ATTR				8		// 2Byte * 8 = 16Byte

#define	__ITEM_WEAPON__				0		
#define __ITEM_AROMR__				1		
#define	__ITEM_SUPPLIES__			2		
#define __ITEM_ZODIAC__				3			
#define	__ITEM_RIDE__				4		
#define	__ITEM_SPECIAL__			5
#define	__ITEM_GUARDIAN__			6		
#define __ITEM_MAGICARRAY__			7
#define __ITEM_MATERIALS__			8		
#define __ITEM_MIX_UPGRADE__		9
#define __ITEM_MAGICFIELD_ARRAY__	10
#define __ITEM_CONSUMABLE__			11
#define __ITEM_UPGRADE__			12
#define __ITEM_LIQUID__				13
#define __ITEM_EDITION__			14
#define __ITEM_BAG__				15

#define MAX_BELT					8
#define MAX_EQUIP					21
#define MAX_INV_LARGE				21
#define MAX_INV_SMALL				42	
#define MAX_TOTAL_BANK				70
#define	MAX_BANK_LARGE				63			
#define	MAX_BANK_SMALL				70
#define MAX_GUARDIAN				5
#define	MAX_INV_GUARDIAN			5
#define MAX_ATTR_PER_ITEM_TYPE		6
#define MAX_ITEM_OPTION				106

#define ITEM_DISTRIBUTE				200

#define	ITEM_WEAPONST_INDEX			0
#define	ITEM_WEAPONEND_INDEX		7
#define	ITEM_ARMORST_INDEX			10
#define	ITEM_ARMOREND_INDEX			17
#define ITEM_BAG_INDEX				18
#define	ITEM_GUARDIAN_INDEX			19
#define	ITEM_RIDE_INDEX				20
#define ITEM_MIXUPGRADEST_INDEX		31
#define ITEM_MIXUPGRADEEND_INDEX	33
#define ITEM_UPGRADE_INDEX			34
#define ITEM_LIQUID_INDEX			35
#define ITEM_EDITION_INDEX			36
#define	ITEM_ZODIACST_INDEX			40
#define ITEM_ZODIACEND_INDEX		41
#define	ITEM_SUPPLIES_INDEX			50
#define	ITEM_SPECIALST_INDEX		60
#define	ITEM_SPECIALEND_INDEX		69
#define ITEM_MATERIALST_INDEX		70
#define ITEM_MATERIALEND_INDEX		72
#define	ITEM_MAGICARRAY_INDEX		80
#define ITEM_MAGICFIELDARRAY_INDEX	90
#define	ITEM_MONEY_INDEX			100
#define ITEM_CONSUMABLE_INDEX		101
#define ITEM_CONSUMABLEST_INDEX		101
#define ITEM_CONSUMABLEEND_INDEX	102

struct ITEM_SERIAL
{
	union
	{
		struct 	
		{
			BYTE	bServerSet;
			BYTE	bServerSerialID;
			WORD	wRestartCount;
			DWORD	dwSerialNum;
		};

		__int64 i64Serial;
	};
};

struct ITEM_WEAPON_EX
{
	unsigned __int64	wMin_Damage		: 8;
	unsigned __int64	wMax_Damage		: 8;
	unsigned __int64	wMana_Damage	: 10;
	unsigned __int64	i64Reserved		: 38;

	DWORD	dwReserved		: 32;
	BYTE	bReserved		: 8;	
};

struct ITEM_ARMOR_EX
{
	WORD	wDefense		: 10;		
	WORD	wReserved		: 6;
};

struct ITEM_ZODIAC_EX
{
	WORD				wPosX			: 16;	
	WORD				wPosZ			: 16;

	BYTE				bQuantity		: 7;		
	BYTE				bType			: 1;

	unsigned __int64	wMapId			: 14;		
	unsigned __int64	bLayer			: 5;		
	unsigned __int64	bInvIndex		: 6;		
	unsigned __int64	bReserved		: 39;
}; 

struct ITEM_SPECIAL_EX
{
	BYTE	bQuantity		: 4;
	BYTE	bReserved		: 4;
};

struct ITEM_CONSUMABLE_EX
{
	DWORD	dwEndDate		: 32;		// 아이템 끝나는 시간을 기록한다.
	BYTE	bInvIndex		: 7;		// 사용했을때의 인벤위치 : 사용한 후에는 이동하지 못한다. base : 1부터	
	BYTE	bQuantity		: 1;	
};

struct ITEM_SUPPLIES_EX
{
	BYTE	bQuantity		: 7;
	BYTE	bReserved		: 1;
};


struct ITEM_MAGICARRAY_EX
{
	BYTE	bQuantity		: 4;
	BYTE	bReserved		: 4;
};

struct ITEM_MONEY_EX
{
	DWORD	dwMoney			: 32;		
};

struct ITEM_RIDEOBJECT_EX
{
	float	fWorldPos_x;		
	float	fWorldPos_z;
};

struct ITEM_MATERIALS_EX
{
	BYTE	bQuantity		: 1;
	BYTE	bReserved		: 7;
};

struct ITEM_MIX_UPGRADE_EX
{
	BYTE	bQuantity		: 7;
	BYTE	bReserved		: 1;
};

struct ITEM_GUARDIAN_EX
{
	DWORD	dwEndLife		: 32;	// 가뎐 생명 끝나는 시기. 이값이전에 태어나야 해..
	DWORD	dwCompleteTime	: 32;	// 완성도
	DWORD	wID				: 32;	// 디비에 생성된 가디언의 아이디.

	BYTE	bCreate			: 1;	// 가디언이 디비에 생성됐는가. 0이라면 아직 알인 상태인것이다.
	BYTE	bReserved		: 7;
};

struct ITEM_MAGICFIELD_ARRAY_EX
{
	BYTE	bQuantity		: 7;
	BYTE	bReserved		: 1;
};

struct ITEM_UPGREADE_EX
{
	BYTE	bQuantity		: 1;
	BYTE	bReserved		: 7;
};

struct ITEM_LIQUID_EX
{
	BYTE	bQuantity		: 1;
	BYTE	bReserved		: 7;
};

struct ITEM_EDITION_EX 
{
	BYTE	bQuantity		: 1;
	BYTE	bReserved		: 7;
};

struct ITEM_BAG_EX 
{
	WORD	wProbability	: 12;	
	WORD	bType			: 4;	
};

struct ITEM_SLOT
{
	BYTE	uUsedSlot	:4;
	BYTE	uMaxSlot	:4;
};

// 아이템에 붙는 추가속성이다.
struct ITEM_ATTR
{
	WORD	uCode	:6;			// 속성 코드
	WORD	uValue	:10;		// 증가치.
};

class CItemEx
{
public:

	WORD			m_wItemID;			//<-- 2Byte	아이템 종류
	ITEM_SERIAL		m_Serial;			//<-- 8Byte	아이템 시리얼, 즉 UID

	union						
	{
		ITEM_WEAPON_EX				m_Item_Weapon;
		ITEM_ARMOR_EX				m_Item_Armor;
		ITEM_SUPPLIES_EX			m_Item_Supplies;
		ITEM_MONEY_EX				m_Item_Money;		
		ITEM_RIDEOBJECT_EX			m_Item_RideObject;		
		ITEM_SPECIAL_EX				m_Item_Special;
		ITEM_ZODIAC_EX				m_Item_Zodiac;
		ITEM_GUARDIAN_EX			m_Item_Guardian;
		ITEM_MAGICARRAY_EX			m_Item_MagicArray;
		ITEM_MATERIALS_EX			m_Item_Materials;
		ITEM_MIX_UPGRADE_EX			m_Item_Mix_Upgrade;
		ITEM_MAGICFIELD_ARRAY_EX	m_Item_MagicField_Array;
		ITEM_CONSUMABLE_EX			m_Item_Consumable;
		ITEM_UPGREADE_EX			m_Item_UpGreade;
		ITEM_LIQUID_EX				m_Item_LiQuid;
		ITEM_EDITION_EX				m_Item_Edition;
		ITEM_BAG_EX					m_Item_Bag;
	};									//<-- 13Byte

	ITEM_SLOT	m_bSlot;				//<-- 1Byte		// 슬롯 정보
	ITEM_ATTR	wAttr[MAX_ITEM_ATTR];	//<-- 16Byte	// 슬롯 속성

	bool	operator==( const CItemEx& rhs ) const
	{
		if( m_Serial.i64Serial == rhs.m_Serial.i64Serial )
		{
			if( m_wItemID == rhs.m_wItemID )
				return true;
		}
		return false;
	}
	bool	operator<( const CItemEx& rhs ) const
	{
		if( m_wItemID < rhs.m_wItemID )
			return true;
		else if( m_wItemID > rhs.m_wItemID )
			return false;

		if( m_Serial.i64Serial < rhs.m_Serial.i64Serial )
			return true;
		else if( m_Serial.i64Serial > rhs.m_Serial.i64Serial )
			return false;

/*
		if( m_Serial.bServerSerialID < rhs.m_Serial.bServerSerialID )
			return true;
		else if( m_Serial.bServerSerialID > rhs.m_Serial.bServerSerialID )
			return false;	

		if( m_Serial.bServerSet < rhs.m_Serial.bServerSet )
			return true;
		else if( m_Serial.bServerSet > rhs.m_Serial.bServerSet )
			return false;

		if( m_Serial.wRestartCount < rhs.m_Serial.wRestartCount )
			return true;
		else if( m_Serial.wRestartCount > rhs.m_Serial.wRestartCount )
			return false;*/


		

		return true;
	}

	WORD	GetKind()
	{
		WORD wdividing	= ITEM_DISTRIBUTE;
		WORD wID		= m_wItemID / wdividing;
		if (wID >= ITEM_WEAPONST_INDEX && wID <= ITEM_WEAPONEND_INDEX)
			return __ITEM_WEAPON__;
		else if (wID >= ITEM_ARMORST_INDEX && wID <= ITEM_ARMOREND_INDEX)
			return __ITEM_AROMR__;
		else if (wID == ITEM_BAG_INDEX)
			return __ITEM_BAG__;
		else if (wID == ITEM_SUPPLIES_INDEX)
			return __ITEM_SUPPLIES__;
		else if (wID >= ITEM_ZODIACST_INDEX && wID <= ITEM_ZODIACEND_INDEX)
			return __ITEM_ZODIAC__;
		else if (wID == ITEM_RIDE_INDEX)
			return __ITEM_RIDE__;
		else if (wID >= ITEM_SPECIALST_INDEX && wID <= ITEM_SPECIALEND_INDEX)
			return __ITEM_SPECIAL__;
		else if (wID == ITEM_GUARDIAN_INDEX)
			return __ITEM_GUARDIAN__;
		else if (wID == ITEM_MAGICARRAY_INDEX)
			return __ITEM_MAGICARRAY__;
		else if (wID >= ITEM_MATERIALST_INDEX && wID <= ITEM_MATERIALEND_INDEX)
			return __ITEM_MATERIALS__;
		else if (wID >= ITEM_MIXUPGRADEST_INDEX && wID <= ITEM_MIXUPGRADEEND_INDEX)
			return __ITEM_MIX_UPGRADE__;
		else if (wID == ITEM_MAGICFIELDARRAY_INDEX)
			return __ITEM_MAGICFIELD_ARRAY__;
		else if	(wID >=  ITEM_CONSUMABLEST_INDEX && wID <= ITEM_CONSUMABLEEND_INDEX )
			return __ITEM_CONSUMABLE__;
		else if	(wID == ITEM_UPGRADE_INDEX)
			return  __ITEM_UPGRADE__;
		else if (wID == ITEM_LIQUID_INDEX)
			return  __ITEM_LIQUID__;
		else if (wID == ITEM_EDITION_INDEX)
			return  __ITEM_EDITION__;
		else
		{
			// 여기에 드러왔다는것은 인덱스 범위를 디파인 하지 않았기때문이다...
			// 아이템 종류에 대한 범위를 디파인 해줘라 ㅡ.ㅡ
			// 각 아이템테이블이 사용하는 아이디를 10으로 나눠서 min과 max를 범위로 잡으면 된다
			//__asm int 3
			//	AfxMessageBox("[WARNING] Item Index Not Define!!!",MB_OK|MB_ICONSTOP);
			return -1;
		}
	}
};


/*
class CItemExCustom : public CItemEx
{
public:
	enum ItemPosition
	{
		IP_BANK = 0,
		IP_NOT_BANK
	} m_eItemPosition;

	DWORD	m_dwCharIndex;
	char	m_strID[20];

	CItemExCustom()
	{
		m_strID[0] = 0;
	}

	CItemExCustom( const CItemEx& rVal, DWORD dwCharIndex )
		: CItemEx( rVal ), m_dwCharIndex(dwCharIndex), m_eItemPosition( IP_NOT_BANK )
	{
	}

	CItemExCustom( const CItemEx& rVal, const string& strID )
		: CItemEx( rVal ), m_eItemPosition( IP_BANK ), m_dwCharIndex(0xffffffff)
	{
		if( strID.empty() == false )
			strcpy( m_strID, strID.c_str() );
		else
			m_strID[0] = 0;
	}

	bool	operator<( const CItemExCustom& rhs ) const
	{
		if( m_eItemPosition == IP_BANK && rhs.m_eItemPosition == IP_BANK )
		{
			int	iComp = strcmp( m_strID, rhs.m_strID );

			if( iComp < 0 )
				return true;
			else if( iComp > 0 )
				return false;
		}
		else
		{
			if( m_dwCharIndex < rhs.m_dwCharIndex )
				return true;
			else if( m_dwCharIndex > rhs.m_dwCharIndex )
				return false;
		}		

		if( m_Serial.bServerSerialID < rhs.m_Serial.bServerSerialID )
		return true;
		else if( m_Serial.bServerSerialID > rhs.m_Serial.bServerSerialID )
		return false;


		if( m_Serial.dwSerialNum < rhs.m_Serial.dwSerialNum )
			return true;
		else if( m_Serial.dwSerialNum > rhs.m_Serial.dwSerialNum )
			return false;

		if( m_Serial.bServerSet < rhs.m_Serial.bServerSet )
		return true;
		else if( m_Serial.bServerSet > rhs.m_Serial.bServerSet )
		return false;


		if( m_Serial.wRestartCount < rhs.m_Serial.wRestartCount )
		return true;
		else if( m_Serial.wRestartCount > rhs.m_Serial.wRestartCount )
		return false;

		if( m_wItemID < rhs.m_wItemID )
			return true;
		else if( m_wItemID > rhs.m_wItemID )
			return false;

		return true;
	}

	WORD	GetKind()
	{
		WORD wdividing	= ITEM_DISTRIBUTE;
		WORD wID		= m_wItemID / wdividing;
		if (wID >= ITEM_WEAPONST_INDEX && wID <= ITEM_WEAPONEND_INDEX)
			return __ITEM_WEAPON__;
		else if (wID >= ITEM_ARMORST_INDEX && wID <= ITEM_ARMOREND_INDEX)
			return __ITEM_AROMR__;
		else if (wID == ITEM_BAG_INDEX)
			return __ITEM_BAG__;
		else if (wID == ITEM_SUPPLIES_INDEX)
			return __ITEM_SUPPLIES__;
		else if (wID >= ITEM_ZODIACST_INDEX && wID <= ITEM_ZODIACEND_INDEX)
			return __ITEM_ZODIAC__;
		else if (wID == ITEM_RIDE_INDEX)
			return __ITEM_RIDE__;
		else if (wID >= ITEM_SPECIALST_INDEX && wID <= ITEM_SPECIALEND_INDEX)
			return __ITEM_SPECIAL__;
		else if (wID == ITEM_GUARDIAN_INDEX)
			return __ITEM_GUARDIAN__;
		else if (wID == ITEM_MAGICARRAY_INDEX)
			return __ITEM_MAGICARRAY__;
		else if (wID >= ITEM_MATERIALST_INDEX && wID <= ITEM_MATERIALEND_INDEX)
			return __ITEM_MATERIALS__;
		else if (wID >= ITEM_MIXUPGRADEST_INDEX && wID <= ITEM_MIXUPGRADEEND_INDEX)
			return __ITEM_MIX_UPGRADE__;
		else if (wID == ITEM_MAGICFIELDARRAY_INDEX)
			return __ITEM_MAGICFIELD_ARRAY__;
		else if	(wID >=  ITEM_CONSUMABLEST_INDEX && wID <= ITEM_CONSUMABLEEND_INDEX )
			return __ITEM_CONSUMABLE__;
		else if	(wID == ITEM_UPGRADE_INDEX)
			return  __ITEM_UPGRADE__;
		else if (wID == ITEM_LIQUID_INDEX)
			return  __ITEM_LIQUID__;
		else if (wID == ITEM_EDITION_INDEX)
			return  __ITEM_EDITION__;
		else
		{
			// 여기에 드러왔다는것은 인덱스 범위를 디파인 하지 않았기때문이다...
			// 아이템 종류에 대한 범위를 디파인 해줘라 ㅡ.ㅡ
			// 각 아이템테이블이 사용하는 아이디를 10으로 나눠서 min과 max를 범위로 잡으면 된다
			//__asm int 3
			//	AfxMessageBox("[WARNING] Item Index Not Define!!!",MB_OK|MB_ICONSTOP);
			return -1;
		}
	}
};

*/