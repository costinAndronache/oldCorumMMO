#pragma once


#include "Define.h"
#include "Struct.h"
#include "GameDefine.h"
#include "ObjectDefine.h"
#include "SpriteManager.h"	
#include "../CommonServer/CommonHeader.h"
#include "Effect.h"


#define		USER_TEMP_DAMAGE_TYPE				0		// �̽����� ������� ��������
#define		USER_TEMP_DAMAGE					1		// ������ ��
#define		USER_TEMP_5SECTICK					5		// 5������ ���� ����
#define		USER_TEMP_IDLETIME					6		// ������ �ƹ����� ������ ���ְ� ����� ���� ����
#define		USER_TEMP_100MILLI					7		// 100�и������� ���� ����
#define		USER_TEMP_1SECTICK					8		// 1������ ���� ����
#define		USER_TEMP_COMMANDGUARDIAN			10
#define		USER_TEMP_TARGET_TYPE				11		// Ÿ���� Ÿ��. ����? ����?
#define		USER_TEMP_TARGET_INDEX				12		// Ÿ���� �ε�����ȣ.
#define		USER_TEMP_AUTO_TARGET_TYPE			13		// �ڵ� Ÿ�� Ÿ��. ����? ����?
#define     USER_TEMP_AUTO_TARGET_INDEX			14		// �ڵ� Ÿ���� �ε���.
#define		USER_TEMP_LASTATTACKTICK			15		// AttackWaiting.
#define		USER_TEMP_SKILL_POISON_RESIST		16		// �ߵ� ���׷�

#define		WEIGTH_100PER_OVER					101		// ����ǰ ���԰� 100 �� �̻��ΰ�� 
#define		WEIGTH_80PER_OVER					81		// ����ǰ ���԰� 80 �� �̻��ΰ�� 


enum USER_PROPERTY_CODE
{	
	USER_MAXHP	=			1,
	USER_MAXMP	=			2,
	USER_HONOR	=			3,
	USER_EGO	=			4,
	USER_STR	=			5,
	USER_INT	=			6,
	USER_DEX	=			7,
	USER_VIT	=			8,
	USER_LUCK	=			9,
	USER_ATTACK_R	=		10,
	USER_ATTACK_L	=		11,
	USER_AA			=		12,
	USER_AVOID		=		13,
	USER_BLOCKRATE	=		14,
	USER_FIRE_RESIST=		15,
	USER_ICE_RESIST	=		16,
	USER_LIGHT_RESIST=		17,
	USER_POI_RESIST	=		18,
	USER_PHY_RESIST	=		19,
	USER_MOVESPEED	=		20,
	USER_EXP		=		21,
	USER_HP			=		22,
	USER_MP			=		23,
	USER_CASTINGTIME=		24,
	USER_COOLTIME	=		25,
	USER_ATTACKRATE_PERCENT	=26,
	USER_ATTACKACURACY_PERCENT	=27,
	USER_OVERDRIVE_DELAY=	28,
	USER_HEALHPSPEED	=	29,
	USER_HEALMPSPEED	=	30,
	USER_POISONING		=	31,
	USER_DEC_SKILLLEVEL	=	32,
	USER_SKILLATTACK_R_PERCENT	=	33,
	USER_SKILLATTACK_L_PERCENT	=	34,
	USER_CTTIME		=		35,
	USER_ALL_RESIST	=		36,
	USER_MAX_RESIST	=		37,
	USER_MOVESPEED_PERCENT	=38,
	USER_HEALHPSPEED_PERCENT=	39,
	USER_PHY_RESIST_PERCENT=	40,
	USER_MP_PERCENT		=	41,
	USER_MAXHP_PERCENT	=	42,
	USER_ATTACK_R_PERCENT=	43,
	USER_BLOCKRATE_PERCENT=	44,
	USER_AVOID_PERCENT=		45,
	USER_HP_PERCENT	=		46,
	USER_POI_RESIST_PERCENT	=47,
	USER_ITEM_ATTACK		=48,
	USER_ITEM_DEFENSE		=49,
	USER_BLIND				=50,
	USER_MAXMP_PERCENT		=51,
	USER_ATTACK_L_PERCENT	=52,
	USER_DETECTION			=53,
	USER_MAX_STATUS
};


enum ENUM_SKILL_CASTING_FAIL_REASON
{
	SKILL_CASTING_FAIL_REASON_LOW = 0,
	SKILL_CASTING_FAIL_REASON_GET_SLAPPED = 1,			// ĳ������ ��� �¾Ҵ�.
	SKILL_CASTING_FAIL_REASON_LACK_SP = 2,				// SP ����
	SKILL_CASTING_FAIL_REASON_LACK_ZODIAC_ITEM = 3,		// Ȳ�� ī�� ����
	SKILL_CASTING_FAIL_NONE_REASEON = 4,				// �����ϱ� ������ ������ ����.
	SKILL_CASTING_FAIL_REASON_OVER_RANGE = 5,			// �����Ÿ��� �Ѱ屺.
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


struct CTDS_SKILL;
class CMonster;
class CItemNativeManager;


class CUser
{
public:

	DWORD				m_dwUserIndex;	
	
	char				m_szName[MAX_CHARACTER_NAME_LENGTH];			// Name	
	WORD				m_wHead;				// �Ӹ� ��ȣ.
	WORD				m_wClass;				// �ɸ��� Ŭ����.
	float				m_fMoveSpeed;

	VECTOR3				m_v3CurPos;				// ���糪�� ��ǥ.
	VECTOR3				m_v3Direction;			// �����̴� ����.	��������.

	int					m_nCharNameSize;		// ĳ���� �̸� ������.
	int					m_nGuildNameSize;		// ����̸� ������.
	
private:

	BYTE				m_bStatus;				// ������ ����.

public:

	BYTE				m_bMoveType;
	BYTE				m_bSkillTmp;		// ���۵� ��ų�� ��� ������ �ִ� ����.
	BYTE				m_bPlayerShop;		// ������ ���ΰ�?

	
	WORD				m_wCurSectionNum;	// ������ PrevSectionNum�� ���� ����.

	BOOL				m_bCollision;

	enum MATCH_STATUS
	{
		MATCH_STATUS_USER_REQUEST_AFTER_USER_GOTO_WORLD = 10,	// ������ ��� ��û�ϰ� ����� �������ȳ� ����..
		MATCH_STATUS_USER_REQUEST_AFTER_EXIT_GAME		= 11,	// ������ ��� ��û�ϰ� ���� ���� �߳� ���ϳ�..
	};

	BOOL				m_bIsRequestedMatch;
	DWORD				m_dwMatchUserIndex;		// ����� ����� 
		
	char				m_szPlayerShopTitle[MAX_PLAYER_SHOP_TITLE];

	OBJECT_HANDLE_DESC	m_hPlayer;			// �ɸ��� �ٵ� �ڵ�.
	OBJECT_HANDLE_DESC	m_hPlayerHead;
	OBJECT_HANDLE_DESC	m_hPlayerRHand;
	OBJECT_HANDLE_DESC	m_hPlayerLHand;
	OBJECT_HANDLE_DESC	m_hPlayerHelmet;
	OBJECT_HANDLE_DESC	m_hPlayerFlag;	
	OBJECT_HANDLE_DESC	m_hPlayerEmoticon;
	OBJECT_HANDLE_DESC  m_RideObject;			//�����ΰ� Ÿ�� �ִ� ������Ʈ�� �ڵ� (�����, ���ں�, ��.. ����..)
	OBJECT_HANDLE_DESC	m_hSelfPortalEffect;	//���� �̵��� ��Ż Effect �ڵ� 		
	OBJECT_HANDLE_DESC	m_hMoveStartEffect;		
	OBJECT_HANDLE_DESC	m_hMoveStopEffect;		
	OBJECT_HANDLE_DESC	m_hPlayerShop;

	GXOBJECT_HANDLE		m_hShadowHandle;		
	GXOBJECT_HANDLE		m_hEventClearObject;
	GXLIGHT_HANDLE		m_hShadowLightHandle;

	EffectDesc*			m_pEffectDesc[MAX_SKILL_COUNT];			
	EffectDesc*			m_pPKEffect;
	EffectDesc*			m_pOwnerEffect;
	EffectDesc*			m_pEffectMagicArray;	// ������ ����Ʈ ������ �ֱ�..
	EffectDesc*			m_pEffectAttackMode;
	
	BYTE				m_bOverDriveCount;	
	
	COnlyList*			m_pEffectList;				// ����Ʈ�� �� ��� ���� ����Ʈ�� �°��� ������ ť
	COnlyList*			m_pUsingStatusEffectList;	// ���� ����ް� �ִ� ��������Ʈ ����Ʈ
	CTDS_SKILL*			m_pSkillPacket;
	
	WORD				m_wAttackType;
	BOOL				m_bBad;				// �̳��� ���۳��̿���...
	DWORD				m_dwTemp[30];
	
	WORD				m_wArmor;
	WORD				m_wHandR;
	WORD				m_wHandL;	
	WORD				m_wHelmet;
	short				m_wCriminalTime;	// ���� Ÿ��
	WORD				m_wSiegePKCount;	// �������϶��� pk��.
	CMonster*			m_pGuardian[ MAX_USER_GUARDIAN ];
	CMonster*			m_pMonster[ MAX_USER_GUARDIAN ];
	
public:		
	
	V2_SPRITE			m_pBaloonChat;				//��ǳ�� 021105
	V2_SPRITE			m_MatchInfoSpr;
	DWORD				m_dwChatTick;				//021105
	DWORD				m_dwCastingTick;
	char				m_szChatBuf[ MAX_CHAT_INPUT_LENGTH ];	//Chat Buffer	021105
	SDGCHAT				m_sDgChat[3];

	BYTE				m_byChatLine;
	BYTE				m_byItemType;
	BYTE				m_bAttackMode;			// ����� : 1, ������ : 2
	DWORD				m_dwGuildWarFlag;		// 0 : �⺻, 1 : �������, 2 : ����//����� �÷���: hwoarang 050202 
	size_t				m_team_index;			// ����� �� �ε���

	DWORD				m_dwPartyId;
	DWORD				m_dwGuildId;	

	char				m_szNickName[MAX_GUILD_NAME_LENGTH];
	
	BYTE				m_byGuildFlag;

	BYTE				m_bMatching;			// ����� : 0, ���� : 1
	BOOL				m_bInEventDungeon;
	BOOL				m_bInEvnetDungeonFirstTimeOut;
	BYTE				m_byEventEffectCount;
	DWORD				m_dwMatchRecords;		// �� 
	DWORD				m_dwMatchWin;			// �� 
	DWORD				m_dwMatchLose;			// ��
	
	BYTE				m_byUpgrade;

	DWORD				m_dwDUMMY1;
	BOOL				m_bIllusion;
	DWORD				m_dwDummy2;

	MAP_TILE*			m_pCurTile;								//���� ���ִ�Ÿ��.
	MAP_TILE*			m_pStartTile;							//�̵��� ������ Ÿ��.
	MAP_TILE*			m_pDestTile;							//�̵��� ��ǥŸ��.
	
	void				SetDamageIndex( DWORD dwDamage );
	void				SetDamageChar( DWORD dwChar );
	void				SetAction( WORD wAct, int nFrame = 0, BYTE bFlag = ACTION_LOOP );
	void				SetActionNext( WORD wAct, WORD wNextAct, BYTE bNextFlag = ACTION_LOOP, int nNextFrame = 0 );
	void				SetMatchUser(DWORD	dwMatchUserIndex);
	void				ShowMoveStartEffect();
	void				ShowMoveStopEffect();
	BYTE				GetStatus();
	void				SetStatus(BYTE bStatus, BOOL bLive = FALSE);		

public:
	void				UserObjectAlpha(BYTE bAlpha);
	void				UserObjectEnable(BYTE bEnable);
	void				SetPlayerShop(BOOL bPlayerShop);	
	void				SetAttackMode(BYTE bAttackMode);
	void				SetSiegePKCount(int nCount);
	void				DungeonOwnerEffect();
	void				SetActionSkill(BYTE bSkillKind);
	void				SetActionCasting(BYTE bSkillKind, VECTOR3& vecTarget, BOOL bDirection);
	void				AttachSkill(BYTE bSkillKind, BYTE bSkillLevel, DWORD dwEndTime);
	void				SetSkillFailAction(ENUM_SKILL_CASTING_FAIL_REASON eFailReason);
	void				DeleteMagicArray();
	BOOL				UsingStatusSkill(BYTE bSkillKind);
	void				RemoveResource();
	void				CreateResource();
	void				RemoveAllStatusEffect();
	BOOL				IsSameUser( CUser* pUser );		// added by minjin.
	CUser();
	~CUser();
};


class CMainUser : public CUser
{

public:
	
	WORD			m_wRace;
	WORD			m_wGrade;
	WORD			m_wHP;					
	WORD			m_wMP;
	WORD			m_wPoint;				// �������� �߻��ϴ� ���ʽ� ����Ʈ.
	WORD			m_wPointSkill;

	DWORD			m_dwExp;
	DWORD			m_dwLevel;
	DWORD			m_dwHonor;
	DWORD			m_dwEgo;
	DWORD			m_dwStr;
	DWORD			m_dwInt;
	DWORD			m_dwDex;
	DWORD			m_dwVit;
	DWORD			m_dwLuck;
	
	CItem			m_pEquip[ MAX_EQUIP_POOL ];
	CItem			m_pInv_Large[ MAX_INV_LARGE_POOL ];
	CItem			m_pInv_Small[ MAX_INV_SMALL_POOL ];
	CItem			m_pBelt[ MAX_BELT_POOL ];
	
	DWORD			m_dwMoney;

	CItem			m_MouseItem;
	CItem			m_GuardianItem;
	CItem			m_InfoItem;		// ������ ������ ����� ������.
	CItem			m_pItemTrade[MAX_TRADE_POOL];

	int				m_nItemSelect;
	int				m_nInterfaceChk;

	SSKILL_TABLE	sSkillTable[MAX_SKILL];
	WORD			m_wClassRank;

	WORD			m_wAttackSpeed;
	DWORD			m_dwHealHPSec;
	DWORD			m_dwHealMPSec;
	int				m_nPoisonSec;				// �ߵ�
	
	WORD			m_wMaxHP;
	WORD			m_wMaxMP;
	WORD			m_pwAttackDamage_R[2];
	WORD			m_pwAttackDamage_L[2];		// 0 - MIN,  1 - MAX
	WORD			m_wAttackAcuracy;
	WORD			m_wMagicDamage;
	WORD			m_wAvoid;
	WORD			m_wBlockRate;
	
	short			m_wFireResist;
	short			m_wIceResist;
	short			m_wLightResist;
	short			m_wPoiResist;
	short			m_wPhyResist;
	BYTE			m_bMaxResist;
	
	float			m_fMaxCoolPoint;
	float			m_fCurCoolPoint;
	
	BYTE			m_byPKCount;
	BYTE			m_byPKRepeatCount;
	
	DWORD			m_dwRecentOutMap;			//�ֱ� �α׾ƿ��� WorldMap ID
	DWORD			m_dwRecentOutPos;			//�ֱ� �α׾ƿ��� Dungeon ID

	BYTE			m_bContinueWorld;			//�������� ����� ���ö� 1 ���õ� 
	WORD			m_wContinueDungeonID;		//�������� ����� ���ö��� �����÷��̾� ��ġ�� ���� ID
		
	char			m_szClassName[16];
	char			m_szGroupName[16];
	int				m_nClassNameSize;
	int				m_nGroupNameSize;
	
	char			m_szGuildName[MAX_GUILD_NAME_LENGTH];	

	BYTE			m_byType;
	BYTE			m_byRank;		

	CItem			m_pInv_Guardian[MAX_INV_GUARDIAN_POOL];

	CItem			m_pBank_Large[ MAX_BANK_LARGE_POOL ];
	CItem			m_pBank_Small[ MAX_BANK_SMALL_POOL ];

	DWORD			m_dwBankMoney;

	DWORD			m_dwItemInfoType;
	BYTE			m_bPartyLeader;
	BYTE			m_bCurrnetHand;	
	PLAYERSHOPINFO	m_sPlayerShop;
	DWORD			m_dwPlayserShop;	
	__int64			m_i64PickupItem;	
	BYTE			m_byHelp[3];	
	int				m_nDecLevel[MAX_SKILL];
	time_t			m_PotionTime;
	MATCH_MARK		m_sMatchMark;	
	BYTE			m_bReceivedConnectMsg;
	CItemNativeManager* m_pItemNativeManager;
	
	CItem			m_pUpgradeItem[MAX_UPGRADE_POOL];
	CItem			m_pMakingItem[MAX_MAKING_POOL];
	
private:
	
	BYTE			m_bySkill[2];			// ��ų 

public:
	
	int					m_nSkillKey[16];		// ��ų ����Ű�� ����Ǿ� �ִ� ��ų �ε���.
	int					m_nPosSK[16];			// ��ų ����Ű�� ����, ������.
			
	DWORD				m_dwStartSkillTick[MAX_SKILL];	// ��ų��ð��� �����ϴ� ����.
	BYTE				m_bCurLayer;			//���� �ִ� ������ �� 
	

	OBJECT_HANDLE_DESC	m_hMonster;				// 20
	WORD				wPrevDungeonID;			// ������ ���� ���̵�

	BYTE				m_bUserType;	

	// �������� ���� ��� ������
	size_t				m_match_type;
	BYTE				m_byLayerIndex;
	WORD				m_wDungeonID;
	// 
public:

public:	
	
	BOOL			IsGMUser() const;
	short			GetPhyResist();
	void			GetAttackDamage_L(WORD& wAttackDamageMin, WORD& wAttackDamageMax);
	void			GetAttackDamage_R(WORD& wAttackDamageMin, WORD& wAttackDamageMax);
	BYTE			GetSkillLevel(BYTE bSkillKind);
	BOOL			IsSkilling();	// ���� ��ų��������ΰ�? ���콺 �ٿ��� �ߴµ� ���� ���ߴٴ� �Ҹ���.
	void			SendSkill();	// ������ ��ų�� ������.
	void			SendCasting();
	void			SetActionDummy();	// ��û�� ������ ���ֱ�� ����.
	void			SetPacketSkillMonster(CMonster* pTargetMonster, BYTE bSkillKindLR);//���Ϳ��� ���� ��ų�� ������.
	void			SetPacketSkillUser(CUser* pTargetUser,  BYTE bSkillKindLR);//�÷��̾�� ���� ��ų�� �����϶�.
	void			SetPacketSkillTile(WORD wTileIndexX, WORD wTileIndexZ,  BYTE bSkillKindLR);//Ÿ�Ͽ��� ���� ��ų�� �����϶�.
	void			SetSkillChangeLR(BYTE bySkillKind, BYTE byLR);//left = 0, right = 1
	BOOL			CheckItem(CItem* pItem);
	BYTE			GetSkillKind(BYTE byLR);
	float			GetODC();
	BOOL			IsAlliance(CUser* pUser);
	BOOL			IsAlliance(CMonster* pMonster);
	BOOL			IsMatching() const;
	BOOL			IsAttack(CUser* pUser, BOOL bAuto);
	BOOL			IsAttack(CMonster* pMonster, BOOL bAuto);
	WORD			GetAllSumWeight();
	WORD			GetSumWeight();
	FLOAT			GetAverageWeight(WORD wAppendWeight = 0);
	WORD			GetLargeSumWeight();
	WORD			GetEquipItemSumWeight();
	WORD			GetInvLargeItemSumWeight();
	WORD			GetSmallItemSumWeight();
	void			GetCheckUpgrade();
	void			WeightProcess(BOOL bChk = FALSE, WORD wWeight = 0);
	void			RegistItemNativeInfo();
	const CItem*	GetItem(enum ITEM_NATIVE eItemNative, BYTE bySlotIndex) const;
	void			SetItem(ITEM_NATIVE eItemNative, BYTE bySlotIndex, const CItem* pItem);
	void			RemoveItem(ITEM_NATIVE eItemNative, BYTE bySlotIndex);
	DWORD			GetItemTotalSize(ITEM_NATIVE eItemNative) const;
	void			ConvertItem(ITEM_NATIVE eItemNative, BYTE bySlotIndex);

public:
	CMainUser();
	~CMainUser();
};

