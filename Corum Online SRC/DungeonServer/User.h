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
#define		MAX_MONEY_PLAYERSHOP		4000000000 // 40억

#ifdef JAPAN_LOCALIZING
#define		MAX_PLAYER_SHOP_TITLE		30
#elif defined CHINA_LOCALIZING
#define		MAX_PLAYER_SHOP_TITLE		28
#else
#define		MAX_PLAYER_SHOP_TITLE		40
#endif

#define		ITEM_MAGICARRAY_DESC_TEMP_STARTTICK		0				// 아이템 적용 시간. 
#define		MAX_REAL_PLAYER_SHOP_TITLE	30
#define		MAX_USE_MAGIC_ARRAY			10
#define		MAX_USE_MAGICFIELD_ARRAY	10
#define		ITEM_SKILL_ATTACK			7			// 공격할때 나가는 스킬
#define		ITEM_SKILL_DEFENSE			8			// 방어할때 나가는 스킬
#define		ITEM_SKILL_DIE				9			// 죽을때 나가는 스킬
#define		__GCTYPE_NONE__				0
#define		__GCTYPE_GUILD__			1
#define		__GCTYPE_CLEN__				2
#define		MAX_ITEMPRESENT				100
#define		MAX_ITEMPRESENT_RATE		100
#define		BASE_ITEMPRESENT_INDEX		12402
#define		__MAX_MONEY__				4200000000
#define		MAX_USER_GUARDIAN			5

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
	USER_TEMP_USEDAMAGESP				= 2,	// 데미지를 SP로 대신한다.
	USER_TEMP_USERDYINGSTARTTICK		= 3,	// 유저 죽은 시간.
	USER_TEMP_DAMAGE					= 4,	// 유저가 마지막으로 받은 데미지. 제일큰것.
	USER_TEMP_DAMAGETICK				= 5,	// 마지막으로 기억한 데미지의 틱.
	USER_TEMP_CASTINGDESTTICK			= 6,	// 캐스팅 끝날 틱
	USER_TEMP_CASTINGSTARTTICK			= 7,	// 캐스팅 시작 틱
	USER_TEMP_LASTATTACKTARGET			= 8,	// 마지막으로 공격한 대상. 0 - Monster 1 - User.
	USER_TEMP_LASTTARGETINDEX			= 9,	// 마지막 타켓의 인덱스.
	USER_TEMP_LAST_ITEM_CASTING			= 10,	// 마지막으로 물약을 먹은 시간이다.
	USER_TEMP_FLAG_UPDATEUSERFORAI		= 11,	// 유저가 죽었을때 몬스터 AI에 타겟에서 지워져야 할지를 판단하는 플래그당..
	USER_TEMP_FLAG_LOBBY				= 12,	// 공성 시작시 로비로 나갔다 온놈인가?
	USER_TEMP_BADSTARTTICK				= 13,	// 나쁜놈이 된 틱을 저장한다.
	USER_TEMP_LASTCLIENTTICK			= 14,	// 클라이언트에서 보내온 Tick.
	USER_TEMP_LASTSERVERTICK			= 15,	// 클라이언트에서 보내온 Tick를 기록할 당시에 서버의 Tick.
	USER_TEMP_LASTATTACKTICK			= 16,	// 스피드 핵때문에...
	USER_TEMP_LASTMOVETICK				= 17,	// 스피드 핵떄문에...
	USER_TEMP_LASTDBUPDATE				= 18,	// 마지막으로 DB에 업데이트 한 TICK
	USER_TEMP_HACK						= 19,	// 핵을 감지한 총수.. 일정량이되면 연결종료.
	USER_TEMP_LASTITEMLISTCHECKTICK		= 20,	//
	USER_TEMP_LASTGODMODETICK			= 21,	// 신모드 틱
	USER_TEMP_LASTSPEEDHACKTICK			= 22,	// 마지막으로 스피드핵 체크를 한 틱
	USER_TEMP_GOTOWORLD_OUTSTAUS		= 23,	// 던전에서 죽었을때 복귀될때 위치를 나타낸다.
	USER_TEMP_GM_DRIVED_OBSERVE			= 24,	// GM 명령에서 OBSERVE를 하면 타유저에게는 GM 이 보이지 않는다
	USER_TEMP_GM_DRIVED_SILENCE_FLAG	= 25,	// GM 이 해당캐릭에게 대화 유무를 결정한 상태 
	
	MAX_USER_TEMP				
};


enum ENUM_SKILL_CASTING_FAIL_REASON
{
	SKILL_CASTING_FAIL_REASON_LOW				= 0,
	SKILL_CASTING_FAIL_REASON_GET_SLAPPED		= 1,	// 캐스팅중 얻어 맞았다.
	SKILL_CASTING_FAIL_REASON_LACK_SP			= 2,	// SP 부족
	SKILL_CASTING_FAIL_REASON_LACK_ZODIAC_ITEM	= 3,	// 황소 카드 부족
	SKILL_CASTING_FAIL_NONE_REASEON				= 4,	// 실패하긴 했지만 이유가 없다.
	SKILL_CASTING_FAIL_REASON_OVER_RANGE		= 5,	// 사정거리를 넘겼군.
};


enum USER_TYPE
{
	USER_TYPE_NORMAL = 0,
	USER_TYPE_GM,
	USER_TYPE_PC_ROOM,
};


enum PORTAL_TYPE
{
	Portal_Type_General,		// 일반 아템 사용 
	Portal_Type_NPC,			// NPC 에 의해  
	Portal_Type_GM,				// GM 에 의해 
};


// 1:1 대결시 발생하는 각종 문제 
enum MATCH_STATUS
{
	MATCH_STATUS_USER_REQUEST_AFTER_USER_GOTO_WORLD = 10,	// 유저가 대결 신청하고 월드로 나가버렸네 쓰글..
	MATCH_STATUS_USER_REQUEST_AFTER_EXIT_GAME		= 11,	// 유저가 대결 신청하고 게임 종료 했네 둑일넘..
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
	WORD			m_wClass;				// 케릭터 클래스.
	WORD			m_wRace;
	WORD			m_wHP;					
	WORD			m_wSP;
	WORD			m_wPoint;
	WORD			m_wPointSkill;			// 레벨업당 방생하는 스킬 뽀나리.
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
	short			m_wCriminalPropensity;	// 범죄 타임
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
	BYTE		m_bPlayerShopMode;						// 노점상 모드군 : 1 보통 : 0
	char		szTitle[MAX_PLAYER_SHOP_TITLE];			// 노점상 제목	
	DWORD		dwCustomSellPrice[MAX_PLAYER_SHOP_INV];			
	CItem		cItem[MAX_PLAYER_SHOP_INV];				// 노점상 아이템목록
};

struct NEWPLAYERSHOPINFO
{	
	BYTE				m_bPlayerShopMode;						// 노점상 모드군 : 1 보통 : 0
	char				szTitle[MAX_PLAYER_SHOP_TITLE];			// 노점상 제목	
	DWORD				dwCustomSellPrice[MAX_PLAYER_SHOP_INV];			
	ITEM_NATIVE_INFO	cItem[MAX_PLAYER_SHOP_INV];				// 노점상 아이템목록
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
	BYTE			m_bBad;					// 이런 나쁜놈 넌 pk 당해도 할말 없다 ㅡ.ㅡ
	POSITION_		m_pPKList;				// pkchecklist에 포함되있나.
	BYTE			m_bPKMode;				//pk 유무
};


struct MATCH_DESC_INFO
{
	DWORD			m_dwMatchRecords;
	DWORD			m_dwMatchWin;
	DWORD			m_dwMatchLose;
	DWORD			m_dwMatchUserIndex;		// 대결하고 있는 상대 인덱스
	BYTE			m_bMatching;			// matching
};


struct ATTACK_DESC_INFO
{
	BYTE			m_bAttackType;					// 계산을 위한값.
	BYTE			m_bAttackFrame;
	BYTE			m_bCurAttackType;				// 실제 패킷등에 쓰일 값.
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
	DWORD						m_dwAccumulationExp;				// 누적 경험치. 사족을 붙이자면 던젼에 따라 클리어 했을 경우 경험치를 몰아먹는 경우가 있다.
	BOOL						m_bInLastLayer;						// 현재 마지막층에 있습니다.
	WORD						m_wKillMonsterCurrentLayer;			// 현재 층에서 처치한 몬스터 수.
	BOOL						m_bIsEventDungeonClear;				// 이벤트 던젼 클리어 했어?		
	__int64						m_nItemTradeSeriel;	
	WORD						m_wPerWeight;
	BYTE						m_byRest;
	BYTE						m_byDiffLevelPointMonsterKill;		// 몬스터 랩차에 의한 포인트 누적.
	BYTE						m_byExTrade;	
	BYTE						m_bCount;
	DWORD						m_dwPartyId;
	BYTE						m_bPartyLeader;
	float						m_fMaxCoolPoint;
	float						m_fCurCoolPoint;		
	DB_USER_CHARACTERINFO		m_pUserCharacterInfo[4];
	CMonster*					m_pGuardian[MAX_USER_GUARDIAN];
	CMonster*					m_pMonster[MAX_USER_GUARDIAN];
	DWORD						m_dwStartSkillTick[MAX_SKILL];					// 쿨타임을 위해서.
	float						m_fPlusParamByMagicFieldArray[USER_MAX_STATUS];	// 던전 마법진에 의한 상승치를 가지고 있는 배열
	CVoidList*					m_pUsingMagicArrayList;
	ITEM_MAGICARRAY_DESC*		m_pUseMagicArrayDesc[MAX_USE_MAGIC_ARRAY];		// 마법진이 작동되고 있는지 알기 위한 것
	ITEM_MAGICFIELDARRAY_DESC	m_UseMagicFieldArrayDesc;		// 던전마법진 적용되고 있는지 알기 위한 것
	POSITION_					m_posDungeonUserList;			// 던전 유저 리스트에 내가 위치한 포인터.
	ATTACK_DESC_INFO			m_sAttackDescInfo;
	DWORD						m_dwClientPrevTick;				// by deepdark. 클라이언트의 tick 수!
	GUARDIAN_EGG_GROWDESC		m_GuardianEggGrowDesc[MAX_USER_GUARDIAN];
	VECTOR2						m_v2ReservPotalPosition;
	BYTE						m_bUserType;					// USER_TYPE_GM, USER_TYPE_NORMAL
	DWORD						m_dwConnectionIndex;			//ConnectionIndex
	CVoidList*					m_pUsingItemList;				// 사용되어지고 있는 아이템 리스트
	BYTE						m_bDestWorldSpot;				// 통로던전일때 들어왔던곳을 스폿으로 기억한다. 예) 0 일때는 2001, 1일때는 2002로 나가야 하므로..
	MAP_TILE*					m_pPrevTile;					// 바로 이전 타일 포인터 	
	CASTING_INFO				m_sCastingInfo;					// 캐스팅인포	
	MAP_TILE*					m_pTileDest;					// 케릭터가 이동하는 목표타일.
	POSITION_					m_pLayerUserListNode;			// 방 유저 리스트에서 빠른 삭제를 위한 노드 포인터 
	POSITION_					m_pPrevSectionNode;				// 섹션의 유저리스트에서의 빠른 삭제를 위한 노드 포인터 위치 
	DWORD						m_dwTemp[MAX_USER_TEMP];	
	WORD						m_wSiegePKCount;				// 공성할때 유저 죽인수
	DWORD						m_dwKillMonCount;	
	void*						pBufForPortal;					// 외부서버로 포탈 이동용 Buffer
	BYTE						m_bPortalMoving;				// 포탈 이동중임 
	enum PORTAL_TYPE			m_ePortalType;	
	DWORD						m_dwPlayserShop;				// 노점 아이템 가격.
	POSITION_					m_posRemoveList;				// 내가 던전에서 나가야 할리스트에 끼일꺼라면 이 포지션에 나의 리스트 포지션을 기억하라.
	ENUM_GODMODE_STATUS			m_enGodMode;					// 무적 모드인가?
	BOOL						m_bConnectToGameRoom;			// 게임방에서 접속했다.
	WORD						m_wRecentJoinDungeonID;			// 이벤트 던전을 제외한 최근 입장한 던전 아이디 	
	PARTY_CONFIG				m_PartyConfig;					// 파티 설정 : 최덕석 2005.1.25
protected:

	CUser*						m_pPrevUser;					
	CUser*						m_pNextUser;
	
public:

	static void* operator new(size_t size);
	static void operator delete(void* pDeleteUser);
	
	CUser();
	virtual ~CUser();

	virtual const char*				Name() const {return &m_szName[0];}//이름 받기 : hwoarang 050202

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

	virtual bool				OnDie();//사망 처리: hwoarang 050202 

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
	void					PKDropItem( CUser * pOffense );	// 중국쪽 죽을 때 페널티  : 엄재원 2005.4.1
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
