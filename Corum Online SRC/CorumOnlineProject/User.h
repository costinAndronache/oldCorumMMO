#pragma once


#include "Define.h"
#include "Struct.h"
#include "GameDefine.h"
#include "ObjectDefine.h"
#include "SpriteManager.h"	
#include "../CommonServer/CommonHeader.h"
#include "../CommonServer/CommonClientDungeon.h"
#include "Effect.h"
#include <vector>
#include "CMainUserUpdateInterested.h"
#include "../BaseLibrary/Timer.h"

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

	AppliedSkill*			m_pEffectDesc[MAX_SKILL_COUNT];			
	AppliedSkill*			m_pPKEffect;
	AppliedSkill*			m_pOwnerEffect;
	AppliedSkill*			m_pEffectMagicArray;	// ������ ����Ʈ ������ �ֱ�..
	AppliedSkill*			m_pEffectAttackMode;
	
	BYTE				m_bOverDriveCount;	
	
	COnlyList*			m_pEffectList;				// ����Ʈ�� �� ��� ���� ����Ʈ�� �°��� ������ ť
	COnlyList*			skillsAppliedOnThisUnit;	// ���� ����ް� �ִ� ��������Ʈ ����Ʈ
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
	CMonster*			servantMonsters[ MAX_USER_GUARDIAN ];
	
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
	void				SetMotion( WORD wAct, int nFrame = 0, BYTE bFlag = ACTION_LOOP );
	void				SetMotionSequence( WORD wAct, WORD wNextAct, BYTE bNextFlag = ACTION_LOOP, int nNextFrame = 0 );
	void				SetMatchUser(DWORD	dwMatchUserIndex);
	void				ShowMoveStartEffect();
	void				ShowMoveStopEffect();
	BYTE				GetStatus();
	void				SetStatus(BYTE bStatus, BOOL bLive = FALSE);		

public:
	virtual void				OnCastPhaseBegin(BYTE bSkillKind, VECTOR3& vecTarget, BOOL bDirection, int spOffsetPerSecond);
	virtual void				OnCastPhaseEndExecutionPhaseBegin(BYTE bSkillKind);

	void				UserObjectAlpha(BYTE bAlpha);
	void				UserObjectEnable(BYTE bEnable);
	void				SetPlayerShop(BOOL bPlayerShop);	
	void				SetAttackMode(BYTE bAttackMode);
	void				SetSiegePKCount(int nCount);
	void				DungeonOwnerEffect();
	void				SetCharacterMotionForSkillExecution(BYTE bSkillKind);
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
private:
	std::vector<CMainUserUpdateInterestedWeakRef> _updateListeners;
	Timer			*continousSkillCastSPUpdateTimer;
	DWORD			m_dwHP;
	DWORD			m_dwMP;
	DWORD			m_dwMaxHP;
	DWORD			m_dwMaxMP;
	DWORD			m_dwPoint;				// �������� �߻��ϴ� ���ʽ� ����Ʈ.
	DWORD			m_dwPointSkill;

	DWORD			m_dwExp;
	DWORD			m_dwLevel;
	DWORD			m_dwHonor;
	DWORD			m_dwEgo;
	DWORD			m_dwStr;
	DWORD			m_dwInt;
	DWORD			m_dwDex;
	DWORD			m_dwVit;
	DWORD			m_dwLuck;

	float			m_fMaxCoolPoint;
	float			m_fCurCoolPoint;



	void			SendCasting();
	BOOL			IsWithContinousSkillSelected();

public:
	std::vector<BYTE>	skillsAvailableOnLeft();
	std::vector<BYTE>	skillsAvailableOnRight();

	void				OnCastPhaseBegin(BYTE bSkillKind, VECTOR3& vecTarget, BOOL bDirection, int spOffsetPerSecond) override;
	void				OnCastPhaseEndExecutionPhaseBegin(BYTE bSkillKind) override;

	void			addUpdateListener(CMainUserUpdateInterestedWeakRef);

	float			percentageHP() const;
	float			percentageMP() const; 
	float			percentageCoolPoints() const;

	float			currentCoolPoints() const;
	void			updateCurrentCoolPoints(float);

	float			maxCoolPoints() const;
	void			updateMaxCoolPoints(float);

	DWORD			currentHP() const;
	void			updateCurrentHP(DWORD hp);

	DWORD			currentSP() const;
	void			updateCurrentSP(DWORD sp);

	DWORD			maxHP() const;
	void			updateMaxHP(DWORD);

	DWORD			maxSP() const;
	void			updateMaxSP(DWORD);


	DWORD			currentStatPoints() const;
	void			updateCurrentStatPoints(DWORD);

	DWORD			currentSkillPoints() const;
	void			updateCurrentSkillPoints(DWORD);

	DWORD			currentEXP() const;
	void			updateCurrentEXP(DWORD);

	DWORD			currentLevel() const;
	void			updateCurrentLevel(DWORD);

	DWORD			currentHonor() const;
	void			updateCurrentHonor(DWORD);


	DWORD			currentEGO() const;
	void			updateCurrentEGO(DWORD);

	DWORD			currentSTR() const;
	void			updateCurrentSTR(DWORD);

	DWORD			currentINT() const;
	void			updateCurrentINT(DWORD);

	DWORD			currentDEX() const;
	void			updateCurrentDEX(DWORD);

	DWORD			currentVIT() const;
	void			updateCurrentVIT(DWORD);

	DWORD			currentLUCK() const;
	void			updateCurrentLUCK(DWORD);

	//

	DWORD			m_wRace;
	DWORD			m_wGrade;

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

	WORD			m_wClassRank;

	DWORD			m_wAttackSpeed;
	DWORD			m_dwHealHPSec;
	DWORD			m_dwHealMPSec;
	int				m_nPoisonSec;				// �ߵ�
	
	DWORD			m_pwAttackDamage_R[2];
	DWORD			m_pwAttackDamage_L[2];		// 0 - MIN,  1 - MAX
	DWORD			m_wAttackAcuracy;
	DWORD			m_wMagicDamage;
	DWORD			m_wAvoid;
	DWORD			m_wBlockRate;
	
	DWORD			m_wFireResist;
	DWORD			m_wIceResist;
	DWORD			m_wLightResist;
	DWORD			m_wPoiResist;
	DWORD			m_wPhyResist;
	DWORD			m_bMaxResist;
	
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
	
	BOOL			IsGMUser() const;
	short			GetPhyResist();
	void			GetAttackDamage_L(WORD& wAttackDamageMin, WORD& wAttackDamageMax);
	void			GetAttackDamage_R(WORD& wAttackDamageMin, WORD& wAttackDamageMax);

	void			SetSkillChangeLR(BYTE bySkillKind, BYTE byLR);//left = 0, right = 1
	void			BeginSkillCastOn(CMonster* pTargetMonster, BYTE bSkillKindLR);
	void			BeginSkillCastOn(CUser* pTargetUser, BYTE bSkillKindLR);
	void			BeginSkillCastAtTile(WORD wTileIndexX, WORD wTileIndexZ, BYTE bSkillKindLR, BYTE skillKind);
	void			SignalStartProcessingContinousSkillAtCurrentState();

	void			SwitchBetweenIdleCharacterMotions();	// ��û�� ������ ���ֱ�� ����.
	BOOL			CheckItem(CItem* pItem);
	BYTE			GetSkillKind(BYTE byLR);
	float			GetODC() const;
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


private:
	BYTE			_skillLevel[MAX_SKILL];
public:
	void			initializeSkillLevelsFrom(const BYTE skillLevels[MAX_SKILL]);
	BYTE			GetSkillLevel(BYTE bSkillKind);
	void			applyOffsetForSkills(int offset);

};

