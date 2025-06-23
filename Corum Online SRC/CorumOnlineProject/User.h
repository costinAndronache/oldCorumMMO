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

#define		USER_TEMP_DAMAGE_TYPE				0		// 미스인지 블록인지 성공인지
#define		USER_TEMP_DAMAGE					1		// 데미지 값
#define		USER_TEMP_5SECTICK					5		// 5초턴을 위한 템프
#define		USER_TEMP_IDLETIME					6		// 유저가 아무반응 없을때 서있게 만들기 위한 템프
#define		USER_TEMP_100MILLI					7		// 100밀리초턴을 위한 템프
#define		USER_TEMP_1SECTICK					8		// 1초턴을 위한 템프
#define		USER_TEMP_COMMANDGUARDIAN			10
#define		USER_TEMP_TARGET_TYPE				11		// 타겟의 타입. 몬스터? 유저?
#define		USER_TEMP_TARGET_INDEX				12		// 타켓의 인덱스번호.
#define		USER_TEMP_AUTO_TARGET_TYPE			13		// 자동 타겟 타입. 몬스터? 유저?
#define     USER_TEMP_AUTO_TARGET_INDEX			14		// 자동 타겟의 인덱스.
#define		USER_TEMP_LASTATTACKTICK			15		// AttackWaiting.
#define		USER_TEMP_SKILL_POISON_RESIST		16		// 중독 저항력


struct CTDS_SKILL;
class CMonster;
class CItemNativeManager;


class CUser
{
public:

	DWORD				m_dwUserIndex;	
	
	char				m_szName[MAX_CHARACTER_NAME_LENGTH];			// Name	
	WORD				m_wHead;				// 머리 번호.
	WORD				m_wClass;				// 케릭터 클래스.
	float				m_fMoveSpeed;

	VECTOR3				m_v3CurPos;				// 현재나의 좌표.
	VECTOR3				m_v3Direction;			// 움직이는 방향.	단위백터.

	int					m_nCharNameSize;		// 캐릭터 이름 사이즈.
	int					m_nGuildNameSize;		// 길드이름 사이즈.
	
private:

	BYTE				m_bStatus;				// 현재의 상태.

public:

	BYTE				m_bMoveType;
	BYTE				m_bSkillTmp;		// 시작될 스킬을 잠깐 가지고 있는 변수.
	BYTE				m_bPlayerShop;		// 노점상 중인가?

	
	WORD				m_wCurSectionNum;	// 서버의 PrevSectionNum과 같은 변수.

	BOOL				m_bCollision;

	BOOL				m_bIsRequestedMatch;
	DWORD				m_dwMatchUserIndex;		// 대결중 상대편 
		
	char				m_szPlayerShopTitle[MAX_PLAYER_SHOP_TITLE];

	OBJECT_HANDLE_DESC	m_hPlayer;			// 케릭터 바디 핸들.
	OBJECT_HANDLE_DESC	m_hPlayerHead;
	OBJECT_HANDLE_DESC	m_hPlayerRHand;
	OBJECT_HANDLE_DESC	m_hPlayerLHand;
	OBJECT_HANDLE_DESC	m_hPlayerHelmet;
	OBJECT_HANDLE_DESC	m_hPlayerFlag;	
	OBJECT_HANDLE_DESC	m_hPlayerEmoticon;
	OBJECT_HANDLE_DESC  m_RideObject;			//무엇인가 타고 있는 오브젝트의 핸들 (비공정, 초코보, 배.. 등등등..)
	OBJECT_HANDLE_DESC	m_hSelfPortalEffect;	//본인 이동용 포탈 Effect 핸들 		
	OBJECT_HANDLE_DESC	m_hMoveStartEffect;		
	OBJECT_HANDLE_DESC	m_hMoveStopEffect;		
	OBJECT_HANDLE_DESC	m_hPlayerShop;

	GXOBJECT_HANDLE		m_hShadowHandle;		
	GXOBJECT_HANDLE		m_hEventClearObject;
	GXLIGHT_HANDLE		m_hShadowLightHandle;

	AppliedSkill*			m_pEffectDesc[MAX_SKILL_COUNT];			
	AppliedSkill*			m_pPKEffect;
	AppliedSkill*			m_pOwnerEffect;
	AppliedSkill*			m_pEffectMagicArray;	// 마법진 이펙트 가지고 있기..
	AppliedSkill*			m_pEffectAttackMode;
	
	BYTE				m_bOverDriveCount;	
	
	COnlyList*			m_pEffectList;				// 이펙트가 를 쏘기 전에 이펙트가 온것을 저장할 큐
	COnlyList*			skillsAppliedOnThisUnit;	// 현재 적용받고 있는 상태이펙트 리스트
	CTDS_SKILL*			m_pSkillPacket;
	
	WORD				m_wAttackType;
	BOOL				m_bBad;				// 이놈은 나쁜놈이에요...
	DWORD				m_dwTemp[30];
	
	WORD				m_wArmor;
	WORD				m_wHandR;
	WORD				m_wHandL;	
	WORD				m_wHelmet;
	short				m_wCriminalTime;	// 범죄 타임
	WORD				m_wSiegePKCount;	// 공성전일때의 pk수.
	CMonster*			m_pGuardian[ MAX_USER_GUARDIAN ];
	CMonster*			servantMonsters[ MAX_USER_GUARDIAN ];
	
public:		
	
	V2_SPRITE			m_pBaloonChat;				//말풍선 021105
	V2_SPRITE			m_MatchInfoSpr;
	DWORD				m_dwChatTick;				//021105
	DWORD				m_dwCastingTick;
	char				m_szChatBuf[ MAX_CHAT_INPUT_LENGTH ];	//Chat Buffer	021105
	SDGCHAT				m_sDgChat[3];

	BYTE				m_byChatLine;
	BYTE				m_byItemType;
	BYTE				m_bAttackMode;			// 방어자 : 1, 공격자 : 2
	DWORD				m_dwGuildWarFlag;		// 0 : 기본, 1 : 길드전중, 2 : 관전//길드전 플래그: hwoarang 050202 
	size_t				m_team_index;			// 길드전 팀 인덱스

	DWORD				m_dwPartyId;
	DWORD				m_dwGuildId;	

	char				m_szNickName[MAX_GUILD_NAME_LENGTH];
	
	BYTE				m_byGuildFlag;

	BYTE				m_bMatching;			// 비대전 : 0, 대전 : 1
	BOOL				m_bInEventDungeon;
	BOOL				m_bInEvnetDungeonFirstTimeOut;
	BYTE				m_byEventEffectCount;
	DWORD				m_dwMatchRecords;		// 전 
	DWORD				m_dwMatchWin;			// 승 
	DWORD				m_dwMatchLose;			// 패
	
	BYTE				m_byUpgrade;

	DWORD				m_dwDUMMY1;
	BOOL				m_bIllusion;
	DWORD				m_dwDummy2;

	MAP_TILE*			m_pCurTile;								//현재 서있는타일.
	MAP_TILE*			m_pStartTile;							//이동을 시작한 타일.
	MAP_TILE*			m_pDestTile;							//이동의 목표타일.
	
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
	DWORD			m_dwPoint;				// 레벨업시 발생하는 보너스 포인트.
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
	CItem			m_InfoItem;		// 아이템 정보를 출력할 아이템.
	CItem			m_pItemTrade[MAX_TRADE_POOL];

	int				m_nItemSelect;
	int				m_nInterfaceChk;

	WORD			m_wClassRank;

	DWORD			m_wAttackSpeed;
	DWORD			m_dwHealHPSec;
	DWORD			m_dwHealMPSec;
	int				m_nPoisonSec;				// 중독
	
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
	
	DWORD			m_dwRecentOutMap;			//최근 로그아웃한 WorldMap ID
	DWORD			m_dwRecentOutPos;			//최근 로그아웃한 Dungeon ID

	BYTE			m_bContinueWorld;			//던전에서 월드로 나올때 1 셋팅됨 
	WORD			m_wContinueDungeonID;		//던전에서 월드로 나올때의 메인플레이어 위치할 던전 ID
		
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
	
	BYTE			m_bySkill[2];			// 스킬 

public:
	
	int					m_nSkillKey[16];		// 스킬 단축키에 저장되어 있는 스킬 인덱스.
	int					m_nPosSK[16];			// 스킬 단축키에 왼쪽, 오른쪽.
			
	BYTE				m_bCurLayer;			//현재 있는 던전의 층 
	

	OBJECT_HANDLE_DESC	m_hMonster;				// 20
	WORD				wPrevDungeonID;			// 들어가기전 던전 아이디

	BYTE				m_bUserType;	

	// 옵저버를 위한 백업 데이터
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

	void			SwitchBetweenIdleCharacterMotions();	// 멍청히 있으면 서있기로 변신.
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

