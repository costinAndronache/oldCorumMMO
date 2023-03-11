#pragma once


#include "../CommonServer/CommonHeader.h"
#include "../CommonServer/AbyssHash64.h"
#include "Effect.h"
#include "unit.h"
#include "Linkedlist.h"
#include "ItemMove.h"
#include "ItemNative.h"


class	CDungeon;
struct	EffectDesc;
struct	CTDS_DUNGEON_CASTING;
struct	MAP_TILE;
class	CClassMemoryPool;
class	CItemNativeManager;
class	CVirtualLinkItemNativeHash;
enum	ITEM_NATIVE_TYPE;


#define		MAX_BASICSKILL_LEVEL		25
#define		MAX_MASTERYSKILL_LEVEL		50
#define		MAX_PLAYER_SHOP_INV			10
#define		MAX_MONEY_PLAYERSHOP		4000000000 // 40��

#ifdef JAPAN_LOCALIZING
#define		MAX_PLAYER_SHOP_TITLE		30
#elif defined CHINA_LOCALIZING
#define		MAX_PLAYER_SHOP_TITLE		28
#else
#define		MAX_PLAYER_SHOP_TITLE		40
#endif

#define		ITEM_MAGICARRAY_DESC_TEMP_STARTTICK		0				// ������ ���� �ð�. 
#define		MAX_REAL_PLAYER_SHOP_TITLE	30
#define		MAX_USE_MAGIC_ARRAY			10
#define		MAX_USE_MAGICFIELD_ARRAY	10
#define		ITEM_SKILL_ATTACK			7			// �����Ҷ� ������ ��ų
#define		ITEM_SKILL_DEFENSE			8			// ����Ҷ� ������ ��ų
#define		ITEM_SKILL_DIE				9			// ������ ������ ��ų
#define		__GCTYPE_NONE__				0
#define		__GCTYPE_GUILD__			1
#define		__GCTYPE_CLEN__				2
#define		MAX_ITEMPRESENT				100
#define		MAX_ITEMPRESENT_RATE		100
#define		BASE_ITEMPRESENT_INDEX		12402
#define		__MAX_MONEY__				4200000000
#define		MAX_USER_GUARDIAN			5

#define		WEIGTH_100PER_OVER			101
#define		WEIGTH_80PER_OVER			81



enum EN_ReLive_Flag
{
	USER_RELIVE_MODE_WITH_HP	= 0,
	USER_RELIVE_MODE_HALF		= 1,
	USER_RELIVE_MODE_FULL		= 2,
};


enum ENUM_GODMODE_STATUS
{
	GODMODE_STATUS_RELEASE		= 0,
	GODMODE_STATUS_EVENT_TIMER	= 1,
	GODMODE_STATUS_MAPLOADING	= 2,
};


enum GOTOWORLD_STATUS
{
	GOTOWORLD_STATUS_UPPER_LAYER = 1,
	GOTOWORLD_STATUS_GOTOWORLD = 2,
	GOTOWORLD_STATUS_GOTOWORLD_OUTSTATUS_GUILDWAR = 3	// Remark
};


enum USER_TEMP
{
	USER_TEMP_HEALTICKLAST				= 0,
	USER_TEMP_STATUSSKILLTICK			= 1,
	USER_TEMP_USEDAMAGESP				= 2,	// �������� SP�� ����Ѵ�.
	USER_TEMP_USERDYINGSTARTTICK		= 3,	// ���� ���� �ð�.
	USER_TEMP_DAMAGE					= 4,	// ������ ���������� ���� ������. ����ū��.
	USER_TEMP_DAMAGETICK				= 5,	// ���������� ����� �������� ƽ.
	USER_TEMP_CASTINGDESTTICK			= 6,	// ĳ���� ���� ƽ
	USER_TEMP_CASTINGSTARTTICK			= 7,	// ĳ���� ���� ƽ
	USER_TEMP_LASTATTACKTARGET			= 8,	// ���������� ������ ���. 0 - Monster 1 - User.
	USER_TEMP_LASTTARGETINDEX			= 9,	// ������ Ÿ���� �ε���.
	USER_TEMP_LAST_ITEM_CASTING			= 10,	// ���������� ������ ���� �ð��̴�.
	USER_TEMP_FLAG_UPDATEUSERFORAI		= 11,	// ������ �׾����� ���� AI�� Ÿ�ٿ��� �������� ������ �Ǵ��ϴ� �÷��״�..
	USER_TEMP_FLAG_LOBBY				= 12,	// ���� ���۽� �κ�� ������ �³��ΰ�?
	USER_TEMP_BADSTARTTICK				= 13,	// ���۳��� �� ƽ�� �����Ѵ�.
	USER_TEMP_LASTCLIENTTICK			= 14,	// Ŭ���̾�Ʈ���� ������ Tick.
	USER_TEMP_LASTSERVERTICK			= 15,	// Ŭ���̾�Ʈ���� ������ Tick�� ����� ��ÿ� ������ Tick.
	USER_TEMP_LASTATTACKTICK			= 16,	// ���ǵ� �ٶ�����...
	USER_TEMP_LASTMOVETICK				= 17,	// ���ǵ� �ً�����...
	USER_TEMP_LASTDBUPDATE				= 18,	// ���������� DB�� ������Ʈ �� TICK
	USER_TEMP_HACK						= 19,	// ���� ������ �Ѽ�.. �������̵Ǹ� ��������.
	USER_TEMP_LASTITEMLISTCHECKTICK		= 20,	//
	USER_TEMP_LASTGODMODETICK			= 21,	// �Ÿ�� ƽ
	USER_TEMP_LASTSPEEDHACKTICK			= 22,	// ���������� ���ǵ��� üũ�� �� ƽ
	USER_TEMP_GOTOWORLD_OUTSTAUS		= 23,	// �������� �׾����� ���͵ɶ� ��ġ�� ��Ÿ����.
	USER_TEMP_GM_DRIVED_OBSERVE			= 24,	// GM ��ɿ��� OBSERVE�� �ϸ� Ÿ�������Դ� GM �� ������ �ʴ´�
	USER_TEMP_GM_DRIVED_SILENCE_FLAG	= 25,	// GM �� �ش�ĳ������ ��ȭ ������ ������ ���� 
	
	MAX_USER_TEMP				
};


enum ENUM_SKILL_CASTING_FAIL_REASON
{
	SKILL_CASTING_FAIL_REASON_LOW				= 0,
	SKILL_CASTING_FAIL_REASON_GET_SLAPPED		= 1,	// ĳ������ ��� �¾Ҵ�.
	SKILL_CASTING_FAIL_REASON_LACK_SP			= 2,	// SP ����
	SKILL_CASTING_FAIL_REASON_LACK_ZODIAC_ITEM	= 3,	// Ȳ�� ī�� ����
	SKILL_CASTING_FAIL_NONE_REASEON				= 4,	// �����ϱ� ������ ������ ����.
	SKILL_CASTING_FAIL_REASON_OVER_RANGE		= 5,	// �����Ÿ��� �Ѱ屺.
};


enum USER_TYPE
{
	USER_TYPE_NORMAL = 0,
	USER_TYPE_GM,
	USER_TYPE_PC_ROOM,
};


enum PORTAL_TYPE
{
	Portal_Type_General,		// �Ϲ� ���� ��� 
	Portal_Type_NPC,			// NPC �� ����  
	Portal_Type_GM,				// GM �� ���� 
};


// 1:1 ���� �߻��ϴ� ���� ���� 
enum MATCH_STATUS
{
	MATCH_STATUS_USER_REQUEST_AFTER_USER_GOTO_WORLD = 10,	// ������ ��� ��û�ϰ� ����� �������ȳ� ����..
	MATCH_STATUS_USER_REQUEST_AFTER_EXIT_GAME		= 11,	// ������ ��� ��û�ϰ� ���� ���� �߳� ���ϳ�..
};


struct GUARDIAN_EGG_GROWDESC
{
	DWORD dwLastTick;
	POSITION_ pos;
};

struct USER_STATUS_ARRAY
{
	DWORD			dwUsedSlot;
	USER_STATUS		pUserStatus[30];
};


#pragma pack(push,1)
struct DB_USER_CHARACTERINFO
{
	BYTE	byType;
	char	szName[MAX_CHARACTER_NAME_LENGTH];
};

struct UPDATE_USER_DATA
{
	DWORD			m_dwChar_Index;
	DWORD			m_dwCurDungeonID;
	WORD			m_wClass;				// �ɸ��� Ŭ����.
	WORD			m_wRace;
	WORD			m_wHP;					
	WORD			m_wSP;
	WORD			m_wPoint;
	WORD			m_wPointSkill;			// �������� ����ϴ� ��ų �ǳ���.
	DWORD			m_dwExp;
	DWORD			m_dwLevel;	
	DWORD			m_dwEgo;
	DWORD			m_dwStr;
	DWORD			m_dwInt;
	DWORD			m_dwDex;
	DWORD			m_dwVit;
	BYTE			m_bySelectedSkill[2];	
	CItem			m_pEquip[MAX_EQUIP_POOL];
	CItem			m_pInv_Large[MAX_INV_LARGE_POOL];
	CItem			m_pInv_Small[MAX_INV_SMALL_POOL];
	CItem			m_pBelt[MAX_BELT_POOL];
	BYTE			m_pwSkillLevel[MAX_SKILL];
	CItem			m_pInv_Guardian[MAX_INV_GUARDIAN_POOL];	
	DWORD			m_dwMoney;
	short			m_wCriminalPropensity;	// ���� Ÿ��
	WORD			m_wClassRank;
	BYTE			m_bPKCount;
	BYTE			m_bPKRepeatCount;
	DWORD			m_dwPKTime;
	DWORD			m_dwPKRepeatTime;
	DWORD			m_dwMatchRecords;
	DWORD			m_dwMatchWin;
	DWORD			m_dwMatchLose;
	BYTE			m_bCurrentHand;	
};

class SHOP_TILE
{
public:
	DWORD	dwDungeonId;

	WORD	wX;
	WORD	wY;
	~SHOP_TILE(){}
};

class ITEM_SHOP
{
public:
	BYTE	byType;	
	~ITEM_SHOP(){}
};

class ITEM_STORE
{
public:
	WORD			wItemId;
	~ITEM_STORE(){}
};

class ITEM_TRADE
{
public: 

	union
	{
		struct __TradeSerialStruct
		{
			DWORD	dwHiUserIndex;
			DWORD	dwLowUserIndex;
		} TradeSerialStruct;

		__int64 nTradeSerial;
	};
	
	BYTE	byHiUserRt;
	BYTE	byLowUserRt;
	
	DWORD	dwHiMoney;
	DWORD	dwLowMoney;

	CItem pTradeLargeItem[MAX_INV_LARGE_POOL];
	CItem pTradeSmallItem[MAX_INV_SMALL_POOL];
	CItem pTradeLargeItem2[MAX_INV_LARGE_POOL];
	CItem pTradeSmallItem2[MAX_INV_SMALL_POOL];

	~ITEM_TRADE(){}
};

class GUILD_NODE
{
public:
	union
	{
		struct 	__IdStruct
		{			
			DWORD	dwHiId;
			DWORD	dwLowId;
		} IdStruct;

		__int64 i64Id;
	};
	~GUILD_NODE(){}
};

struct	CASTING_INFO
{
	BYTE	bSkillKind;
	BYTE	bOwnType;
	BYTE	bPK;
	BYTE	bTargetType;
	DWORD	dwTargetIndex;
	WORD	wTileIndex_X;
	WORD	wTileIndex_Z;
	WORD	wAttackDamage[2];	
};	

struct PLAYERSHOPINFO
{	
	BYTE		m_bPlayerShopMode;						// ������ ��屺 : 1 ���� : 0
	char		szTitle[MAX_PLAYER_SHOP_TITLE];			// ������ ����	
	DWORD		dwCustomSellPrice[MAX_PLAYER_SHOP_INV];			
	CItem		cItem[MAX_PLAYER_SHOP_INV];				// ������ �����۸��
};

struct NEWPLAYERSHOPINFO
{	
	BYTE				m_bPlayerShopMode;						// ������ ��屺 : 1 ���� : 0
	char				szTitle[MAX_PLAYER_SHOP_TITLE];			// ������ ����	
	DWORD				dwCustomSellPrice[MAX_PLAYER_SHOP_INV];			
	ITEM_NATIVE_INFO	cItem[MAX_PLAYER_SHOP_INV];				// ������ �����۸��
};

struct PLAYERSHOP_DB
{
	DWORD		dwUserIndex;
	CItem		cItem[MAX_PLAYER_SHOP_INV];	
};

class PLAYERSHOP_LOG
{
public:
	DWORD		dwUserIndex;
	DWORD		dwKey;

	CItem		cItem;
	DWORD		dwPrice;
	BYTE		byZipCode;	
	BYTE		bInv;
	DWORD		dwCnt;
	
	BYTE		byOwnZipCode;
	BYTE		byOwnZipCode2;
	DWORD		dwOwnPrice;
	DWORD		dwOwnUserIndex;	
	~PLAYERSHOP_LOG(){}
};


struct ITEM_MAGICARRAY_DESC
{
	CBaseItem*	pItem;
	POSITION_   pos;
	DWORD		dwTemp[20];
};


struct ITEM_MAGICFIELDARRAY_DESC
{
	CBaseItem*	pBaseItem;
	float		fParamPlus[5];
	WORD		wStatusID[5];
};


struct PRESENTBOX_ITEM_RATE
{
	WORD	wBotaryID;
	WORD	wBotaryRate;
	WORD	wBotaryQuantity;
};


struct BASEITEM_PRESENT_BOX
{
	PRESENTBOX_ITEM_RATE	ItemRate[ MAX_ITEMPRESENT_RATE ];
};


struct PK_DESC_INFO
{
	BYTE			m_bPKCount;
	BYTE			m_bPKRepeatCount;
	DWORD			m_dwPKTime;
	DWORD			m_dwPKRepeatTime;
	BYTE			m_bBad;					// �̷� ���۳� �� pk ���ص� �Ҹ� ���� ��.��
	POSITION_		m_pPKList;				// pkchecklist�� ���Ե��ֳ�.
	BYTE			m_bPKMode;				//pk ����
};


struct MATCH_DESC_INFO
{
	DWORD			m_dwMatchRecords;
	DWORD			m_dwMatchWin;
	DWORD			m_dwMatchLose;
	DWORD			m_dwMatchUserIndex;		// ����ϰ� �ִ� ��� �ε���
	BYTE			m_bMatching;			// matching
};


struct ATTACK_DESC_INFO
{
	BYTE			m_bAttackType;					// ����� ���Ѱ�.
	BYTE			m_bAttackFrame;
	BYTE			m_bCurAttackType;				// ���� ��Ŷ� ���� ��.
	BYTE			m_bCurAttackFrame;
};

#pragma pack(pop)


class CUser : public CUnit
{
public:

	char						m_szName[MAX_CHARACTER_NAME_LENGTH];	
	WORD						m_wHead;
	WORD						m_wPointSkill;			
	DWORD						m_dwHonor;
	DWORD						m_dwLuck;
	CItemNativeManager*			m_pItemNativeManager;
	CVirtualLinkItemNativeHash* m_pVirtualLinkItemNativeHash;
	char						m_byItemNativeLinkCount;
	ITEM_NATIVE					m_CureItemNativeLink;
	CItem						m_pEquip[MAX_EQUIP_POOL];
	CItem						m_pInv_Large[MAX_INV_LARGE_POOL];
	CItem						m_pInv_Small[MAX_INV_SMALL_POOL];
	CItem						m_pInv_Guardian[MAX_INV_GUARDIAN_POOL];
	CItem						m_pBelt[MAX_BELT_POOL];	
	CItem						m_pBank_Large[MAX_BANK_LARGE_POOL];
	CItem						m_pBank_Small[MAX_BANK_SMALL_POOL];	
	DWORD						m_dwBankMoney;	
	CItem						m_pTradeItem[MAX_TRADE_POOL];
	CItem						m_pUpgradeItem[MAX_UPGRADE_POOL];
	CItem						m_pMakingItem[MAX_MAKING_POOL];
	PLAYERSHOPINFO				m_sPlayerShop;	
	ITEM_NATIVE_INFO			m_pTradeItemLink[MAX_TRADE_POOL];
	ITEM_NATIVE_INFO			m_pUpgradeItemLink[MAX_UPGRADE_POOL];
	ITEM_NATIVE_INFO			m_pMakingItemLink[MAX_MAKING_POOL];
	NEWPLAYERSHOPINFO			m_sPlayerShopLink;
	CItem						m_ItemMouse;	
	CItem						m_ItemGuardian;	
	BYTE						m_pwSkillLevel[MAX_SKILL];
	char						m_szGuildName[MAX_GUILD_NAME_LENGTH];
	char						m_szNickName[MAX_GUILD_NAME_LENGTH];
	BYTE						m_byGroupType;	
	BYTE						m_byRank;
	DWORD						m_dwGuildId;	
	DWORD						m_dwMoney;
	short						m_wCriminalPropensity;	
	WORD						m_wClassRank;
	PK_DESC_INFO				m_sPKDescInfo;	
	MATCH_DESC_INFO				m_sMatchDescInfo;
	BYTE						m_bCurrentHand;
	BYTE						m_byLastUsedSkillItem;
	BYTE						m_byLastUsedStatItem;
	char						m_szId[MAX_CHARACTER_NAME_LENGTH];		
	BYTE						m_bInvItemCheck;
	BYTE						m_bBankItemCheck;
	BYTE						m_byGuildFlag;	
	BYTE						m_byUpgrade;
	DWORD						m_dwHealHPSec;
	DWORD						m_dwHealMPSec;
	WORD						m_pwItemDamage[2];					// Min Max
	WORD						m_wItemDefense;						// Item Defense
	BYTE						m_byItemChk[MAX_EQUIP_POOL];	
	DWORD						m_dwAccumulationExp;				// ���� ����ġ. ������ �����ڸ� ������ ���� Ŭ���� ���� ��� ����ġ�� ���ƸԴ� ��찡 �ִ�.
	BOOL						m_bInLastLayer;						// ���� ���������� �ֽ��ϴ�.
	WORD						m_wKillMonsterCurrentLayer;			// ���� ������ óġ�� ���� ��.
	BOOL						m_bIsEventDungeonClear;				// �̺�Ʈ ���� Ŭ���� �߾�?		
	__int64						m_nItemTradeSeriel;	
	WORD						m_wPerWeight;
	BYTE						m_byRest;
	BYTE						m_byDiffLevelPointMonsterKill;		// ���� ������ ���� ����Ʈ ����.
	BYTE						m_byExTrade;	
	BYTE						m_bCount;
	DWORD						m_dwPartyId;
	BYTE						m_bPartyLeader;
	float						m_fMaxCoolPoint;
	float						m_fCurCoolPoint;		
	DB_USER_CHARACTERINFO		m_pUserCharacterInfo[4];
	CMonster*					m_pGuardian[MAX_USER_GUARDIAN];
	CMonster*					m_pMonster[MAX_USER_GUARDIAN];
	DWORD						m_dwStartSkillTick[MAX_SKILL];					// ��Ÿ���� ���ؼ�.
	float						m_fPlusParamByMagicFieldArray[USER_MAX_STATUS];	// ���� �������� ���� ���ġ�� ������ �ִ� �迭
	CVoidList*					m_pUsingMagicArrayList;
	ITEM_MAGICARRAY_DESC*		m_pUseMagicArrayDesc[MAX_USE_MAGIC_ARRAY];		// �������� �۵��ǰ� �ִ��� �˱� ���� ��
	ITEM_MAGICFIELDARRAY_DESC	m_UseMagicFieldArrayDesc;		// ���������� ����ǰ� �ִ��� �˱� ���� ��
	POSITION_					m_posDungeonUserList;			// ���� ���� ����Ʈ�� ���� ��ġ�� ������.
	ATTACK_DESC_INFO			m_sAttackDescInfo;
	DWORD						m_dwClientPrevTick;				// by deepdark. Ŭ���̾�Ʈ�� tick ��!
	GUARDIAN_EGG_GROWDESC		m_GuardianEggGrowDesc[MAX_USER_GUARDIAN];
	VECTOR2						m_v2ReservPotalPosition;
	BYTE						m_bUserType;					// USER_TYPE_GM, USER_TYPE_NORMAL
	DWORD						m_dwConnectionIndex;			//ConnectionIndex
	CVoidList*					m_pUsingItemList;				// ���Ǿ����� �ִ� ������ ����Ʈ
	BYTE						m_bDestWorldSpot;				// ��δ����϶� ���Դ����� �������� ����Ѵ�. ��) 0 �϶��� 2001, 1�϶��� 2002�� ������ �ϹǷ�..
	MAP_TILE*					m_pPrevTile;					// �ٷ� ���� Ÿ�� ������ 	
	CASTING_INFO				m_sCastingInfo;					// ĳ��������	
	MAP_TILE*					m_pTileDest;					// �ɸ��Ͱ� �̵��ϴ� ��ǥŸ��.
	POSITION_					m_pLayerUserListNode;			// �� ���� ����Ʈ���� ���� ������ ���� ��� ������ 
	POSITION_					m_pPrevSectionNode;				// ������ ��������Ʈ������ ���� ������ ���� ��� ������ ��ġ 
	DWORD						m_dwTemp[MAX_USER_TEMP];	
	WORD						m_wSiegePKCount;				// �����Ҷ� ���� ���μ�
	DWORD						m_dwKillMonCount;	
	void*						pBufForPortal;					// �ܺμ����� ��Ż �̵��� Buffer
	BYTE						m_bPortalMoving;				// ��Ż �̵����� 
	enum PORTAL_TYPE			m_ePortalType;	
	DWORD						m_dwPlayserShop;				// ���� ������ ����.
	POSITION_					m_posRemoveList;				// ���� �������� ������ �Ҹ���Ʈ�� ���ϲ���� �� �����ǿ� ���� ����Ʈ �������� ����϶�.
	ENUM_GODMODE_STATUS			m_enGodMode;					// ���� ����ΰ�?
	BOOL						m_bConnectToGameRoom;			// ���ӹ濡�� �����ߴ�.
	WORD						m_wRecentJoinDungeonID;			// �̺�Ʈ ������ ������ �ֱ� ������ ���� ���̵� 	
	PARTY_CONFIG				m_PartyConfig;					// ��Ƽ ���� : �ִ��� 2005.1.25
protected:

	CUser*						m_pPrevUser;					
	CUser*						m_pNextUser;
	
public:

	static void* operator new(size_t size);
	static void operator delete(void* pDeleteUser);
	
	CUser();
	virtual ~CUser();

	virtual const char*				Name() const {return &m_szName[0];}//�̸� �ޱ� : hwoarang 050202

	virtual void				Update();
	virtual void				RemoveResource();
	virtual void				CreateResource();
	virtual void				DetachSkill(EffectDesc* pEffectDesc);
	virtual void				UpdateMaxHP();
	virtual void				UpdateMaxSP();
	virtual void				UpdateAttackSpeed();
	virtual void				UpdateAttackAcuracy();
	virtual void				UpdateBlockRate();
	virtual void				AddExp(int iExp, BOOL bAlphaExp = TRUE);
	virtual void				UpdateFireResist();				
	virtual void				UpdateIceResist();
	virtual void				UpdateLightResist();
	virtual void				UpdatePoiResist();
	virtual void				UpdatePhyResist();
	virtual void				UpdateAvoid();
	virtual void				UpdateAD();
	virtual void				GetAttackDamage(BYTE bySelectedSkill, BYTE bSkillLevel, BYTE bWeaponKind, WORD* pwAttackDamageMin, WORD* pwAttackDamageMax, BYTE bLR) const;
	virtual int					GetExpDying();
	virtual void				GetAttackingDamage(WORD wAttackDamageMin, WORD wAttackDamageMax, WORD* pwAttackDamageMin, WORD* pwAttackDamageMax, BYTE byLR);
	virtual void				ReSetStatusFromSkillStatusValue(EffectDesc* pEffectDesc);
	virtual void				SetStatusFromSkillStatusValue(BYTE bSkillKind, BYTE bSkillLevel, WORD wClass, float* pResetValue, BOOL bUserStatusSend = FALSE);
	virtual void				SetDamageOver(const CUnit* pUnit, DWORD dwDamage);
	virtual BOOL				IsAlliance(const CUnit* pUnit);
	virtual BOOL				AttachSkill(BYTE bOwnType, DWORD dwOwnIndex, BYTE bSkillKind, BYTE bSkillLevel, WORD wClass, DWORD& dwDestTime, DWORD& dwAlphaDamage);
	virtual WORD				GetOriginalAttackAcuracy() const;
	virtual WORD				GetOriginalAvoid() const;
	virtual void				UpdateMoveSpeed();
	virtual void				LevelUP();
	virtual BOOL				IsBadAction(CUnit* pDefenseUnit);
	virtual void				SetBad(BOOL bBad, BYTE byCriminalKind, int nValue, BOOL bChk = FALSE);	
	virtual void				SetAttackTypeAndFrame( const CUnit* pUnit, DWORD dwType );
	virtual short				GetAllResist() const;
	virtual USER_PROPERTY_CODE	SetDamage(DWORD dwOffenseLevel, DWORD dwDamage );
	virtual void				SetHP(DWORD dwHP, const CUnit* pAttacker = NULL);
	virtual BYTE				GetSkillLevel(const BYTE bSkillKind) const;		
	virtual BYTE				GetSkillLevelByBonusOption(const BYTE bSkillKind) const;		
	virtual const char*			GetName() const;
	virtual void				SendStopPacket(ENUM_SEND_MODE eSendMode) const;
	virtual void				UpdateAllStatus();
	virtual void				SendAllStatus();					
	virtual DWORD				GetConnectionIndex() const;

	virtual bool				OnDie();//��� ó��: hwoarang 050202 

	float					GetMoveSpeedByStatus() const;
	void					GetAttackDamageByFormula20(float& fDamageMin, float& fDamageMax, BYTE bSkillKind, BYTE bLR, WORD wSP) const;
	DWORD					GetPrevTickCount();					
	void					SetPrevTickCount(DWORD dwTick);		
	void					ChangeGodMode(ENUM_GODMODE_STATUS eGodModeStatus);
	BOOL					IsAvailabilityAttachItem(WORD wItemID);
	void					AttachItem(CItem* pItem);			
	void					UpdateUsingItem(CItem* pItem);		
	void					DetachItem(POSITION_ pos);			
	void					AttachMagicFieldArray(CItem* pItem);
	void					DetachMagicFieldArray(ITEM_MAGICFIELDARRAY_DESC* pUseMagicFieldArrayDesc);
	void					AttachMagicArray(DWORD dwItemID);
	void					DetachMagicArray(ITEM_MAGICARRAY_DESC* pItemMagicArrayDesc);
	void					SetStatusFromMagicFieldArrayStatusValue(CItem* pItem);
	void					ReSetStatusFromMagicFieldArrayStatusValue(ITEM_MAGICFIELDARRAY_DESC* pUseMagicFieldArrayDesc);	
	void					SetStatus( BYTE bStatus, BOOL bCompulsion = FALSE);
	void					InitUsingItem();					
	void					Move();
	WORD					GetCurDungeonLayerMonsterCount();
	DWORD					GetCheckStr() const; 
	DWORD					GetCheckDex();
	WORD					GetHeadModelNo() const;
	void					SetHeadModelNo(WORD wHeadModelNo);
	WORD					GetLeftoverStatSkillPoint() const;
	void					SetLeftOverStatSkillPoint(WORD wSkillPoint);
	DWORD					GetHonor() const;
	void					SetHonor(DWORD wHonor);
	DWORD					GetLuck() const;
	void					SetLuck(DWORD dwLuck);
	PLAYERSHOPINFO*			GetPlayerShopInfo();
	const BYTE*				GetSkillLevelPtr() const;
	short					GetCriminalPropensity() const;
	WORD					GetClassRank() const;
	void					SetClassRank(WORD wClassRank);
	MATCH_DESC_INFO*		GetMatchDescInfo() const;
	PK_DESC_INFO*			GetPKDescInfo() const;
	BYTE					GetCurrentHand() const;
	DWORD					GetBankMoney() const;
	void					SetBankMoney(DWORD dwBankMoney);
	const char*				GetLoginID() const;
	void					SetLoginID(const char* szLoginID);
	void					SetSummonMonster(GAME_OBJECT_TYPE eObjectType, BYTE byIndex, CMonster* pMonster);
	CMonster*				GetSummonMonster(GAME_OBJECT_TYPE eObjectType, BYTE byIndex);
	BOOL					IsServerSetChange() const;	
	void					InitUserItemAll();
	void					InitUserItem( CItem* pItem );
	BOOL					SetEquipItem( USER_STATUS_ARRAY* pUserStatus, BYTE bEquipNum );		
	void					RemoveEquipItem( USER_STATUS* pUserStatus, BYTE bEquipNum );
	int						GetOriginalMoveSpeed() const;
	DWORD					GetSumItemEgo() const;
	DWORD					GetSumItemStr() const;
	DWORD					GetSumItemInt() const;
	DWORD					GetSumItemDex() const;
	DWORD					GetSumItemVit() const;
	DWORD					GetNewestEgo() const;
	DWORD					GetNewestStr() const;
	DWORD					GetNewestInt() const;
	DWORD					GetNewestDex() const;
	DWORD					GetNewestVit() const;
	WORD					GetOriginalMaxHP() const;
	WORD					GetOriginalMaxMP() const;
	float					GetOriginalHealMPSpeed() const;
	float					GetOriginalHealHPSpeed() const;
	short					GetPhyResist() const;	
	void					UpdateHealHPSec();
	void					UpdateHealSPSec();
	void					UpdateMaxCoolPoint();
	void					SendAllUserSkillLevel();
	void					SetPKMode(BOOL bPKMode);
	void					SubExp( int iExp );
	void					AddHackCount(BOOL bNewMethod = FALSE);
	void					SetRest(BYTE bRest);
	int						GetSubordinateIndex() const;										
	BOOL					IsChangeLayer(WORD wIndex_X, WORD wIndex_Z, BYTE bSpotNum) const;	
	void					SetCriminalPropensity(int nKind,DWORD dwDeffenseLevel);				
	float					GetODC() const;
	void					SendItemSkill(BYTE bFlag, BYTE bObjectTargetType, void* pObjectTartet);
	void					SetEventCompulsiveDisconnectUser();			
	void					CallBackCompulsiveDisconnectUser();			
	void					InitUserBonusItem(const CItem* pItem );
	void					InitStat(int nInc);
	void					InitSkill(int nInc);
	BOOL					CheckItem(CItem* pItem) const;
	void					SetExchangePKCount(int nCount, DWORD dwPKEndTime);
	void					SetExchangePKRepeatCount(int nCount, DWORD dwPKRepeatEndTime);
	BYTE					GetCriminalPropensityTableID() const;			
	BOOL					IsPKZone();
	BYTE					GetPartyLeader();
	void					SetPartyLeader(BYTE bPartyLeader);
	DWORD					GetPartyID();
	void					SetPartyID(DWORD dwPartyID);
	void					SetGuildID(DWORD dwGuildID);
	void					SetAttackMode(BYTE bAttackMode);
	void					SetGodMode(ENUM_GODMODE_STATUS eGodModeStatus);
	void					GoToWorld(WORD wMoveSpotNum, GOTOWORLD_STATUS eOutStaus);
	WORD					GetCurrentWeaponID() const;
	void					ExchangeConsumableItemRefreshUserAttr(WORD wItemID, WORD wAttrCode, short nValue, BYTE byCreate);
	void					SendToUserConsumableItemEffectListAll(CUser* pUser);
	BOOL					IsPickUpItem();
	WORD					GetAllSumWeight();
	WORD					GetSumWeight();
	WORD					GetEquipItemSumWeight();
	WORD					GetInvLargeItemSumWeight();
	WORD					GetSmallItemSumWeight();
	WORD					GetAverageWeight();
	WORD					GetArmorDefense();
	BOOL					OverDriveChk(BOOL bChk = TRUE);
	DWORD					GetSkillPassiveValueMin(BYTE bSkillKind) const;
	void					UsingItemListProcess();			
	void					InitMatch();
	void					CancelMatch(enum MATCH_STATUS eMatchStatus);
	void					InsertToDungeon(CDungeon* pDungeon);
	void					SetQueryUserInfo(char* pResult);
	enum DUNGEON_JOIN_FAIL	IsEnterDungeon();
	enum PORTAL_TYPE		GetPortalType() const;
	void					SetPortalType(enum PORTAL_TYPE	ePortalType);
	void					Recover5SecPer();
	void					BadModeReleaseTimeProcess();
	void					GodModeReleaseTimeProcess();
	void					EffectSkillTimeProcess();
	const CItem*			GetItem								(enum ITEM_NATIVE eItemNative, BYTE bySlotIndex) const;
	void					SetItem								(ITEM_NATIVE eItemNative, BYTE bySlotIndex, const CItem* pItem);
	void					RemoveItem							(ITEM_NATIVE eItemNative, BYTE bySlotIndex);
	DWORD					GetItemTotalSize					(ITEM_NATIVE eItemNative) const;
	ITEM_NATIVE_TYPE		GetItemNativeType					(BYTE byNativeID) const;
	void					ConvertItem							(ITEM_NATIVE eItemNative, BYTE bySlotIndex);
	DWORD					GetItemNativeLinkCount				();
	ITEM_NATIVE				GetCurItemNativeLink				();
	BOOL					IsUserItemNativeLink				(const CItem* pItemNative);
	void					SetPlayerShopCustomSellPrice		(DWORD dwCustomSellPrice,BYTE bySlotIndex);
	DWORD					GetPlayerShopCustomSellPrice		(BYTE bySlotIndex);
	void					RemovePlayerShopCustonSellPrice		(DWORD dwCustomSellPrice,BYTE bySlotIndex);
	void					SetPlayerShopMode					(BYTE bPlayerShopMode);
	void					SetPlayerShopTitle					(LPSTR lpTitle);
	void					RemovePlayerShopTitle				();
	BOOL					IsUserVirtualItemNativeLink			(const CItem* pItemNative);	
	BOOL					IsUserVirtualItemNativeLink			(const CItem* pItemNative,BYTE byLinkCnt);
	BOOL					SetUserVirtualItemNativeLink		(const CItem* pItemNative,BYTE byLinkCnt);
	BOOL					SetUserVirtualItemNativeLinkReturn	(const CItem* pItemNative,BYTE byLinkCnt,SET_LINKTOINV_TYPE eType);
	BOOL					SetUserVirtualItemNativeLinkSell	(const CItem* pItemNative,BYTE bySellCnt);
	BOOL					RemoveUserVirtualItemNativeLink		(const CItem* pItemNative);
	void					RemoveAllVirtualItemNativeLink		();
	BOOL					IsCorrectItemNativeLink				(ITEM_NATIVE eItemNative);
	void					SetItemLink							(ITEM_NATIVE eFromItemNative, BYTE byFromSlotIndex, const ITEM_NATIVE_INFO* pToItemNativeLinkInfo);
	void					RemoveItemLink						(ITEM_NATIVE eItemNative, BYTE bySlotIndex);
	const ITEM_NATIVE_INFO* GetItemLink							(ITEM_NATIVE eItemNative, BYTE bySlotIndex);
	void					SetMoney(DWORD dwMoney);
	DWORD					GetMoney();
	void					PKDropItem( CUser * pOffense );	// �߱��� ���� �� ���Ƽ  : ����� 2005.4.1
	BOOL					IsSummonGuardian(BYTE bySlotIndex);
	void					UnSummonGuardian(BYTE bySlotIndex);
	void					SendSkillCastingFail(ENUM_SKILL_CASTING_FAIL_REASON eFailReason);
	void					SetReservPotalPosition(VECTOR2* pv2Position);
	const VECTOR2*			GetReservPotalPosition() const;
	void					GuildMemberKill(const CUser* pTargetUser);
	float					GetMovePerMS();
	BOOL					IsCurDungeonEventInLive();
	BOOL					IsCurDungeonMatchInLive();
	BOOL					IsCurDungeonVillageInLive();
	BOOL					IsGMUser() const;
	void					ReLive( BYTE byFlag = USER_RELIVE_MODE_WITH_HP, DWORD dwHP = 1 );	
	void					SetKillMonCount(DWORD dwKillMonCount);
	DWORD					GetKillMonCount();
	CMonster*				GetGuardian(BYTE bySlotIndex);
	void					SetGuardian(BYTE bySlotIndex, CMonster* pGuardian);
private:

	void					GetAttackDamageByFormula1(float& fDamageMin, float& fDamageMax, BYTE bSkillKind, WORD wWeaponKind, BYTE bLR, BYTE bSkillAttack) const;
	void					GetAttackDamageByFormula2(float& fDamageMin, float& fDamageMax, BYTE bSkillKind, BYTE bSkillLevel, BYTE bLR, BYTE bSkillAttack) const;
	BOOL					IsAlliance(const CUser* pUser);					
	BOOL					IsAlliance(const CMonster* pMonster);			
	BOOL					IsBadAction(CUser* pDefenseUser);			
	BOOL					IsBadAction(const CMonster* pDefenseMonster);
};


typedef	CAbyssHash<CUser> USER_HASH, *LPUSER_HASH;
typedef	CAbyssHash<CUser> PARTYUSER_HASH, * LPPARTYUSER_HASH;
typedef	CAbyssHash<CUser> GUILDUSER_HASH, * LPGUILDUSER_HASH;

extern CClassMemoryPool*		g_pUserClassPool;
extern LPUSER_HASH				g_pUserHash;
extern BASEITEM_PRESENT_BOX*	g_pBasePresentBox;
