#pragma once


#include "PathFinding\\BlockLineManager.h"
#include "Unit.h"
#include "Linkedlist.h"
#include "MonsterManager.h"


#define GUARDIAN_TYPE_NORMAL		1		// ������ ��ȯ��
#define GUARDIAN_TYPE_DUNGEON		2		// ������ ��ȣ�ϱ� ����.

#define MONSTER_CLASS_BEAST			1
#define MONSTER_CLASS_UNDEAD		2
#define MONSTER_CLASS_DEMON			3
#define MONSTER_CLASS_PLANT			4
#define MONSTER_CLASS_ELEMENTAL		5
#define MONSTER_RACE_BOSS			7
#define	MONSTER_RANK_DRAGON			9

#define NPC_AIFLAG_SLEEP			1
#define NPC_AIFLAG_DEEPSLEEP		2

#define MAX_AI_SEARCH_TYPE_NUM	4				// AI�� ������Ʈ�� ã�� ����� ����
#define	MAX_AI_DEFENSE_TYPE_NUM	5				// AI�� ������ ������ �ൿ �ϴ� ���


#define AI_KIND_MOVETYPE			1		// �����̴� ���ȿ� AI
#define	AI_KIND_SEARCHTYPE			2		// �˻��ϴ� AI
#define	AI_KIND_ATTACKTYPE			3		// �����Ҷ��� AI
#define	AI_KIND_DEFENSETYPE			4		// ����Ҷ��� AI
#define MAX_ASTRT_CURVE				20

struct MAP_TILE;
class CSection;
struct BASEMONSTER;
class CItem;
class CClassMemoryPool;

enum MONSTER_TEMP
{
	NPC_TEMP_DAMAGE				=0	,	// ���Ͱ� ���������� ���� �������̴�. AI���� ����Ѵ�.
	NPC_TEMP_DAMAGETICK			=1	,	// ���������� ����� �������� ƽ.
	NPC_TEMP_FINDUSER			=2	,	// ������ ã����? ����?
	NPC_TEMP_TARGET				=3	,	// ���� Ÿ���� �����ΰ�.? ����? ����? �Ѵ�? 
	NPC_TEMP_AIFLAG				=4	,	// �� �÷��װ� 0�ϋ��� AI�� �����ϴ�.
	NPC_TEMP_CREATETYPE			=5	,	// �̰��� 0�̸� �����Ǵ� ���� �̰��� 1�̸� �����Ǵ� ���Ͱ� �ƴϴ�.
	NPC_TEMP_SKILLMANY			=6	,	// �̰��� 0�̸� ��ų�� ���� ���³��̴�.
	NPC_TEMP_ITEM				=7	,	// ���� �̰��� 0�� �ƴ϶�� �� ���Ͱ� �׾����� �� �������� ��������.
	NPC_TEMP_CONGESTION_MODE	=8	,	// ���ָ��; ���� �̰��� 0�� �ƴ϶�� ���ָ�尡 �ߵ��� ������ Tick�̴�. 

	NPC_TEMP_MOVE				=10	,	// �������� ��� �÷���.
	NPC_TEMP_AIKIND				=11	,	// AI�� ��� ���ͳ� ������.
	NPC_TEMP_SLOT				=12	,	// ������̶�� ������ ����� �����ΰ�.
	NPC_TEMP_PEACE				=13,
	NPC_TEMP_DUNGEONGUARDIAN	=14	,	// ���� ���� ������̶�� ������ �ִ� ������̳� ���� ������̳�.
	NPC_TEMP_LASTATTACKTICK		=15,	// ���������� ������ ƽ
	NPC_TEMP_LASTSKILLTICK		=16,	// ���������� ��ų����� ƽ
	NPC_TEMP_LASTSTATUSTICK		=17,	// ���������� ���� ���� ƽ
	NPC_TEMP_KILLTICK			=18,	// �׾����� ƽ.
	MAX_NPC_TEMP	
};
enum	AI_DEFENSE_ENUM
{
	AI_DEFENSE_TEMP_DAMAGE = 0,
};

// ��ã�� ����� ������ ���� ����ü�̴�. Client�� �ٸ���.
struct A_STAR
{
	DWORD	dwCurveNum;							// ��ã�� �߿��� ������ �޶����� ������ ����.
	DWORD	dwCurCurve;							// ������� ����� Ŀ��.

	BlockLocation	pLocation[MAX_ASTRT_CURVE];	// Ŀ���Ұ��� ��ġ���� ���� �迭.
};

struct AI_DEFENSE_PARAM
{
	BYTE bObjectType;
	const void* pObject;
	DWORD dwTemp[10];
};

typedef struct GUARDIAN_CREATION_INFO
{
	CUser*		pUser;			// �θ𰡵� ������ ����.
	CDungeon*	pDungeon;		// ���߿� �ϳ��� NULL.
	BYTE		bZipCode;		// ������ ����� �迭������ �ε���.
	BOOL		bUserGuardian;
	CItem*		pItem;
} * LPGUARDIAN_CREATION_INFO;


#pragma pack( push, 1 )
struct UPDATE_GUARDIAN
{
	DWORD	dwLevel;
	DWORD	dwExp;
	DWORD	dwHP;
	DWORD	dwMP;
	WORD	wEgo;
	WORD	wStr;
	WORD	wInt;
	WORD	wDex;
	WORD	wVit;
	WORD	wStatPoint;
	MONSTER_SKILL monsterSkill[5];
	DWORD	dwID;

};
typedef struct GUARDIAN_INFO
{
	DWORD	dwID;
	char	szName[50];
	DWORD	dwKind;
	DWORD	dwLevel;
	DWORD	dwExp;
	DWORD	dwHP;
	DWORD	dwMP;
	BYTE	bOldGuardian;	// ������?
	WORD	wStatPoint;
	WORD	wEgo;
	WORD	wStr;
	WORD	wInt;
	WORD	wDex;
	WORD	wVit;
	MONSTER_SKILL monsterSkill[5];
} * LPGUARDIAN_INFO;
#pragma pack( pop )

class CMonster : public CUnit					// ����� ���� Ŭ������ ���̾���.
{
	WORD				m_wInfoIndex;			// �̸��Ͱ� ������ ������ ������ �ִ� �����迭�� �ε���
	MAP_TILE*			m_pPrevTile;			//�ٷ� ���� Ÿ�� ������ 
	VECTOR2				m_v2DestPos;			// ���� ��ǥ
	CSection*			m_pCurSection;			// ���� ���Ͱ� �ִ¼���.
	POSITION_			m_pLayerListNode;		//�� ���� ����Ʈ���� ���� ������ ���� ��� ������ 
	POSITION_			m_pPrevSectionNode;		//������ ��������Ʈ������ ���� ������ ���� ��� ������ ��ġ
	DWORD				m_dwSerial_NO;			// ������ϋ� ����� ������ȣ.
	CUnit*				m_pUnitForAI;			// AI�� ����� �Ǵ� ����.
	DWORD				m_dwAI;
	float				m_fSight;				// ������ �þ�(Cm).
	float				m_fAttackRange;			// ���ݱ�..
	short				m_wAll_resist;
	char				m_szGuardianName[50];
	DWORD				m_dwTemp[MAX_NPC_TEMP];	// 0 - 9������ ���� ����� ����. 10-19������ �����.
	A_STAR				m_PathFinder;
	BASEMONSTER*		m_pBaseMonster;			// �����ϋ��� ���̽� ����.
	POSITION_			m_pResponListPos;
	SKILLDESC			m_SkillDesc;			// ��ų ��� �ִ� ��Ŷ
	VECTOR2				m_v2TargetAIPos;		// Ÿ���� ��ġ
	VECTOR2				m_v2TargetAIDir;
	MONSTER_SKILL		m_MonsterSkill[MAX_MONSTER_USE_SKILL];
	BOOL				m_bSpeedUp;				// ���Ͱ� �ι�� ����ΰ�? ������ : �ִ���
	BOOL				m_bCongestionMode;		// ���Ͱ� ���ָ�� : �ִ���
	
protected:
	// AI
	void*				FindUser_Guardian();				// ������.

	void				SearchUser();						// FindUser�� ������ ������ ã������ �������� ���ƴٴѴ�.
	void				SearchUser_Guardian();

	BOOL				PathFind(const VECTOR2* v2Dest = NULL );
	void				SetResist();
	void				SetAllResist();
	BOOL				IsAlliance(const CUser* pUser);
	BOOL				IsAlliance(const CMonster* pMonster);

	void				(CMonster::*AI)();
	void				AI0();
	void				AI1();
	void				AI2();
	void				AI3();
	void				AI4();		// For Guardian.....
	void				AI5();		// For Dungeon Guardian.....
	void				AI6();		// FOr Special Monster.....
	void				AI8();		// ��ȣ��(������ ��) : �ִ��� 2005.2.7
	void				AI9();		// �Ƹ޸�ŸƮ : �ִ��� 2005.1.26
	void				AI_New();

	// AI�� ���� ���� �Լ�.
	void				SpeedUp(float fRate);
	void				SpeedNormal();
	void				Move(float fRunningRate = 1.0f);
	BOOL				AttackAfterMove();	//kk
	void				Attack();
	void				Damage();
	void				Delay();
	
	// AI�� ���� ���ǿ� ���� ���� ã�� �Լ� ���
	void*				(CMonster::*AI_FindObject[MAX_AI_SEARCH_TYPE_NUM])(BYTE bObjectType);
	void*				AI_FindObjectNear(BYTE bObjectType);	// ���� ��ó�� �ִ� ��
	void*				AI_FindObjectNearAndFewHP(BYTE bObjectType);	// ��ó�� �ְ� hp�� ���� ��
	void*				AI_FindObjectAttachDetectAndNearAndFewHP(BYTE bObjectType);// ����Ʈ ����� �ִ³��̴�.
	void*				AI_FindObjectLeastHP(BYTE bObjectType);		//HP�� ���������� 
	
	// ai�� ���� ���ǿ� ���� ������ ���� �Ŀ� �Լ� ���
	void				(CMonster::*AI_DefenseType[MAX_AI_DEFENSE_TYPE_NUM])(const AI_DEFENSE_PARAM* pParam);
	void				AI_DefenseNormal(const AI_DEFENSE_PARAM* pParam);						// �ٸ��𿡰� �¾Ƶ� Ÿ�ٺ�ȭ�� �Ͼ� ���� �ʴ´�.
	void				AI_DefenseRecovery(const AI_DEFENSE_PARAM* pParam);					// �ٸ��𿡰� ������ ��ų 10~12���� ����� ���� ���¸� �������Ѷ�.
	void				AI_Defense_ChangeTargetToFierceObject(const AI_DEFENSE_PARAM* pParam);	// �ٸ��𿡰� ������ �� �糪������� Ÿ���� �ٲ���
	
public:

	static void* operator new(size_t size);
	static void operator delete(void* pDeleteMonster);

	virtual void	Update();
	virtual void	DetachSkill(EffectDesc* pEffectDesc);	// ���� ��ų�� ����.
	virtual void	RemoveResource();
	virtual void	CreateResource();
	virtual void	UpdateMaxHP();
	virtual void	UpdateMaxSP();
	virtual void	UpdateAttackSpeed();
	virtual void	UpdateAttackAcuracy();
	virtual void	UpdateBlockRate();
	virtual void	AddExp(int iExp, BOOL bAlphaExp = TRUE);
	virtual void	UpdateFireResist();				// Resist
	virtual void	UpdateIceResist();
	virtual void	UpdateLightResist();
	virtual void	UpdatePoiResist();
	virtual void	UpdatePhyResist();
	virtual void	UpdateAvoid();
	virtual void	UpdateAD();
	
	virtual int		GetExpDying();
	virtual void	ReSetStatusFromSkillStatusValue(EffectDesc* pEffectDesc);
	virtual void	SetStatusFromSkillStatusValue(BYTE bSkillKind
						, BYTE bSkillLevel
						, WORD wClass
						, float* pResetValue
						, BOOL bUserStatusSend = FALSE);
	virtual void	SetDamageOver(const CUnit* pUnit, DWORD dwDamage);
	virtual BOOL	IsAlliance(const CUnit* pUnit);
	virtual BOOL	AttachSkill(BYTE bOwnType
						, DWORD dwOwnIndex
						, BYTE bSkillKind
						, BYTE bSkillLevel
						, WORD wClass
						, DWORD& dwDestTime
						, DWORD& dwAlphaDamage);
	virtual WORD	GetOriginalAttackAcuracy() const;
	virtual WORD	GetOriginalAvoid() const;
	virtual void	UpdateMoveSpeed();
	virtual void	LevelUP();
	virtual BOOL	IsBadAction(CUnit* pDefenseUnit);
	virtual void	SetCurDungeonLayer(CDungeonLayer* const pDungeonLayer);
	virtual short	GetAllResist() const;
	virtual USER_PROPERTY_CODE		SetDamage(DWORD dwOffenseLevel, DWORD dwDamage );
	virtual void	SetHP(DWORD dwHP, const CUnit* pAttacker = NULL);
	virtual BOOL	ChangeTargetObject(const CUnit* pAttackUser, int nAttackDemage= -1);
	virtual const char*	GetName() const;
	virtual void	SendStopPacket(ENUM_SEND_MODE eSendMode) const;
	virtual void	UpdateAllStatus();
	virtual void	SendAllStatus();
	virtual BYTE	GetSkillLevel(const BYTE bySkillKind) const;
	
	virtual bool	OnDie();
	
	WORD				GetOriginalAD_Min() const;
	WORD				GetOriginalAD_Max() const;
	DWORD				GetOriginalMaxHP() const;
	void				UpdateSight();
	WORD				GetInfoIndex()					{ return m_wInfoIndex; }
	void				SetInfoIndex( WORD wInfoIndex ) { m_wInfoIndex = wInfoIndex; }
	void				SetStatusStop();										// ���� ������ ���¸� ������ �����.
	BOOL				SetPathFindMove(BYTE bMonsterStatus);										// ��ã�� ����� ���������� �����Ѵ�.
	BOOL				SetMonsterAI( DWORD dwAI );								// ������ AI �Լ� �����͸� �����Ѵ�.
	void				SetStatus( BYTE	bStatus, BOOL bClearTarget = FALSE, BOOL bCompulsion = FALSE);								// ������ ���¸� �����Ѵ�.
	void				ClearTargetAI();
	void				ScheduleMoveDestPos(const VECTOR2* pDestPos );
	void				SubExp( int iExp );
	void				DisAppearUnit( DWORD dwUnitID );						// ������ �����ų� �Ҷ� ���ڷ� ���� �ε��� ���� ���Ͱ� ��� �ִ� ������ ���̶�� NULL�� �����.
	int					GetDropItem(int nMagicItemRate, int iRate);
	BYTE				GetAIType(BYTE bAIKind);

	//Packet..
	void				SendMovePacket();
	void				SendRunningPacket();
	void				SendWalkingPacket();
	void				SendAttackPacket();
	BOOL				SendSkillPacket(BYTE bSkillArrayIndex, BOOL bSkillTick = TRUE);
	void				SkillToMonster(BYTE bSkillKind
							, BYTE bSkillLevel
							, const CMonster* pTargetMonster
							, const WORD* pwMinMax);// bSkillKind : ��ų ����, bSkillLevel : ��ų ����, pTargetMonster : ��ų������
	void				SkillToUser(BYTE bSkillKind
							, BYTE bSkillLevel
							, const CUser* pTargetUser);// bSkillKind : ��ų ����, bSkillLevel : ��ų ����, pTargetUser : ��ų������
	void				SkillToTile(BYTE bSkillKind
							, BYTE bSkillLevel
							, const MAP_TILE* pTile);// bSkillKind : ��ų ����, bSkillLevel : ��ų ����, pTile : ��ų������
	
	
	BOOL				IsElemental(VOID) const;
	BOOL				IsMindControll(VOID) const; 
	BOOL				IsGuardian(VOID) const;
	BOOL				IsNormalMonster() const;
	BOOL				IsNPC() const;

	BOOL				IsCongestionMode() const;	
	void				CheckCongestionMode(BOOL bOnCheck);	//True�̸� ������ �������� ���ָ�带 On ��Ű�� 
															//False�̸� ������ �������� ���ָ�带 Off ��Ų��.			
	BYTE				GetAI_FirstAttack() const;
	BYTE				GetAI_TargetChangeType() const;
	float				GetAI_MinHPRateForCongestionMode() const;
	float				GetAI_RunningRate() const;
	DWORD				GetAI_MonsterIDForCongestionMode() const;
	WORD				GetAI_CongestionTime() const;
	void				SetTargetAI(const CUnit* pUnit);
	void				RemoveCurDungeonLayer(BOOL bSendDisapper = FALSE);
	void				SetDestPosition(const VECTOR2* v2Dest);
	const VECTOR2*		GetDestPosition() const;
	CSection*			GetCurSection() const;
	void				RemoveCurSection();
	void				SetCurSection(CSection* pSection);
	DWORD				GetTemp(MONSTER_TEMP eMonster_Temp) const;
	void				SetTemp(MONSTER_TEMP eMonster_Temp, DWORD dwValue);
	void				AddResponList();
	void				RemoveResponList();
	void				SetBaseMonsterInfo(BASEMONSTER* pBaseMonster);
	const BASEMONSTER* 	GetBaseMonsterInfo() const;
	void				SetGuardianDBID(DWORD dwGuardianDBID);
	DWORD				GetGuardianDBID() const;
	void				SetName(char* pszName);
	CUnit*				GetUnitForAI() const;
	void				ReSearchPath();
	void				Initialize(
									BASEMONSTER* pBaseMonster
								  , GUARDIAN_INFO* pGuardian
								  , CUser* pLord
								  , BYTE byZipCode
								  , DWORD dwLevel
								  , DWORD dwExp
								  );
	void				SetSkillLevel(BYTE bySkillKind, BYTE bySkillLevel);
	const MONSTER_SKILL*		GetMonsterSkillPtr() const;
	void*				FindObject(BYTE bSearchType);			// ������ ������ ã�� m_pUserForAI�� �ִ´�. AI1 AI2

	void  GetAttackSkillDamage(BYTE bySelectedSkill, BYTE bSkillLevel, WORD* pwAttackDamageMin, WORD* pwAttackDamageMax) const;
	void  GetAttackDamageByFormula1(float& fDamageMin, float& fDamageMax, BYTE bSkillKind) const;
	void  GetAttackDamageByFormula2(float& fDamageMin, float& fDamageMax, BYTE bSkillKind, BYTE bSkillLevel) const;
	void  GetAttackDamageByFormula20(float& fDamageMin, float& fDamageMax, BYTE bSkillKind) const;
	CMonster();
	~CMonster();

public:
	
	CMonster*			m_pPrevData;
	CMonster*			m_pNextData;
};
	

void UpdateUserForAI( CUser* pUser );
void UpdateMonsterForAI( CMonster* pMonster );
