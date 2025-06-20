/////////////////////////////////////
//
//
//		yakolla
//		skill.iti ������ ���� ����Ÿ������ �־� �д�.
/////////////////////////////////////
#pragma once
#include "../SS3D_0719/4DYUCHIGX/4DyuchiGRX_common/math.inl"
#include "../CommonServer/CommonClientDungeon.h"

#define	NONE			0
#define	SKILL_PROPERTY_KIND_FIRE		1	// ��ų �Ӽ� ��
#define	SKILL_PROPERTY_KIND_ICE			2	// ��ų �Ӽ� ����
#define	SKILL_PROPERTY_KIND_EARTH		3	// ��ų �Ӽ� ��1/2 ����1/2
#define	SKILL_PROPERTY_KIND_LIGHT		4	// ��ų �Ӽ� ����
#define	SKILL_PROPERTY_KIND_PHY			5	// ��ų �Ӽ� ����
#define SKILL_PROPERTY_KIND_POI			6	// ��ų �Ӽ� ��
#define CRIME_APPLY_ENEMY		1	// ��ų���ɶ� ����Ǵ� ����� ��������
#define CRIME_APPLY_FRIENDLY	2	// ��ų���ɶ� ����Ǵ� ����� �Ʊ�����
#define CRIME_APPLY_ENEMY_FRIENDLY_MONSTER 3 // ��ų���ɶ� ����Ǵ� ���ݽ�ų������ ���� ��ȯ�� ���Ϳ��Ը� ����Ҽ� �ְ� �Ѵ�.
											// ���� PK�϶��� ���δ� �����ų�� �ִ�.


class CUnit;
class CUser;
class CMonster;
class CDungeonLayer;
struct SKILL_TARGETINFO;
struct SKILL_RESULT;
struct DSTC_USER_STATUS;
//--> ��ũ��Ʈ ������
#pragma pack( push, 1 )

typedef struct BASESKILL
{
	BYTE			skillKind;				// ���̵�.
	DWORD			dwResourceID;
	DWORD			dwStatusResourceID;
	WORD			wProperty;			// none : 0, ���� : 500, Aura : 100, Divine, : 200, Summon : 300, Chakra : 400
	BYTE			bType;				// none : 0, Active : 1, Drive : 2, Passive : 3
	BYTE			bAbleClass;			// ��ų�� ����Ҽ� �ִ� Ŭ����
	BYTE			bSkillTarget;		// none : 0, monster : 1, ���� : 2, player : 3
	BYTE			bSkillType;			// none : 0, player : 1, monster : object : 100, 2 ���� : 110, �� : 120, X���� : 130, ���� : 140, 3���浹 : 150, Ÿ�ϼ� : 160
	DWORD			dwRange;
	DWORD			dwMinMastery;		// �ּ� �����͸�
	DWORD			dwCastingTime;
	DWORD			dwCoolTime;
	DWORD			dwEffectPosition;	// 31~29 bit none : 0, �Ӹ��� : 1, ���� : 2, �ٴ� : 4,  28~0 bit subkey
	BYTE			bJointEffect;
	BYTE			bFormula;			// ���� 
	BYTE			bSuccessFormula;	// ��ų �������� ���� ����
	WORD			wSoundNum[8];		// ���� �ѹ���.
	STATUSVALUE		sStatusValue[MAX_STATUS_SKILLVALUE];	
	BYTE			bCrime;				// �������� ���̸� 1, �Ʊ��̸� 2
	BYTE			bEfficiency;
//	char			szInfo[128];	// ��ų�� ���� ������ ����.
	VALUE			Value[MAX_SKILL_LEVEL+1];	
} *LPBASESKILL;




//<-- ��ũ��Ʈ ������
struct Effect : public BASESKILL
{
public:
	
	int GetEffectPosition();
	int	GetMaxMastery(DWORD dwUserLevel, BYTE bSkillLevel, WORD wClass);	
};

enum
{
	EFFECT_DESC_TEMP_POISON_DAMAGE_MIN		=	0,	// ������ ����Ѵ�.
	EFFECT_DESC_TEMP_POISON_DAMAGE_MAX		=	1,	// ������ ����Ѵ�.
	EFFECT_DESC_TEMP_POISON_OWNER_TYPE		=	2,	// �ߵ����� ����Ѵ�.
	EFFECT_DESC_TEMP_POISON_OWNER_INDEX		=	3,	// �ߵ����� ����Ѵ�.
	EFFECT_DESC_TEMP_MAX 
};

struct AppliedSkill
{
	float		fParamPlus[5];					// ���º�ȭ��.
	DWORD		dwDestTick;						// ���� �ð�.
	DWORD		dwLastUpdateTick;				// ������ ���� ���¸� ������ �ð�( �ߵ����� ��ų ����� ��� )
	DWORD		dwAmfleafireTime;				// ���ø����̾� ������ �޾� �ð��� �þ�ð��� ���.
	Effect*		pEffect;						// ����Ʈ ����
	void*		pos;							// ����Ʈ�� �����Ҷ��� ������.
	CUser*		pOwnUser;						// ��ų ���.
	DWORD		dwTemp[EFFECT_DESC_TEMP_MAX];	// �������� �뵵�� ����� ����
	BYTE		bSkillLevel;					// ��ų ����	
};

struct SkillCast
{
	BYTE			bSkillKind;					// ��ų ���̵�
	char			bSkillLevel;				// ��ų ���� : zero base
	DWORD			dwSkillKeepTime;			// ��ų ���ӽð� (��, ��������̺길 ��밡���ϴ�)
	BYTE			casterType;					// ��ų�� ��� ��ü Ÿ��
	DWORD			casterDungeonID;					// ��ų�� ��� ��ü �ε���
	BYTE			bTargetType;				// ��ų�� ���ϴ� ��ü Ÿ��
	DWORD			dwTargetIndex;				// ��ų�� ���ϴ� ��ü �ε���
	WORD			wTileIndex_X;				// ��ų �ߵ��� ��ġ
	WORD			wTileIndex_Z;				// ��ų �ߵ��� ��ġ
	VECTOR2			casterPosition;				// ��ų ��� ��ü�� ��ġ
	BOOL			casterPlayerPKFlagEnabled;						// PK ����ΰ�.
	WORD			wDamageMinMax[2];			// ��ų ������ �ּ� �ִ밪�� ���
	CDungeonLayer*	pDungeonLayer;				// ����� �ϴ� ���� ��������
	BYTE			bSectionNum;				// ����� �ϴ� ���� �ѹ�.
	CUser*			pMonsterMaster;				// ���� ��ȯ�� ���Ǿ�����.��ȯ�� ������ ������ �� ���� �����͸� �־� �ش�.
};

struct EffectLayer
{	
	Effect			m_Effect[MAX_SKILL_COUNT];	
	
private:
	
	DWORD			Load(char* szFileName); // ���� �о�.
	BOOL			ProcessUsersSkillCast(CUser* pOwnUser, SkillCast* pSkillDesc);
	BOOL			MessyProcessForSystem(CUser* pOwnUser, SkillCast* pSkillDesc);
	BYTE			GetAppliedTargetCount(CDungeonLayer* pDungeonLayer, SkillCast* pSkillDesc, SKILL_TARGETINFO* pTargetInfo, VECTOR2* pV2Start);
	void			RevisionStartPositon(SkillCast* pSkillDesc, VECTOR2* pV2Start);
	BOOL			IsRange(SkillCast* pSkillDesc, VECTOR2* pV2Start);
	CUnit*			ProcessCasterForSkillCast(SkillCast* pSkillDesc);
	BOOL			IsSkillSuccess(SkillCast* pSkillDesc, CUnit* pOffense, CUnit* pDefense, SKILL_RESULT* pSkill_result);
	void			AfterSkillSuccessProcess(SkillCast* pSkillDesc, CUnit* pOffense, CUnit* pDefense, DSTC_USER_STATUS*	pUserStatus);
public:
	int				GetBaseClassSkillMax(WORD wClass, BYTE bSkillKind);
	BYTE			GetSkillMasteryKind(BYTE bSkillKind);	// �ش������� �����͸� ���̵� ���´�.
	BOOL			IsSuccessByFormula0(BYTE bSkillKind, BYTE bSkillLevel);
	BOOL			IsSuccessByFormula1(WORD wAA, WORD wDP,  WORD wBL, DWORD dwOffenseLevel, DWORD dwDefenceLevel);
	BOOL			IsSuccessByFormula2(BYTE bSkillKind, BYTE bSkillLevel, DWORD dwOffenseLevel, DWORD dwDefenceLevel);
	BOOL			IsSuccessByFormula13(BYTE bSkillKind, BYTE bSkillLevel, BYTE bSenderLevel, BYTE bRecverLevel);
	BOOL			IsSuccessByFormula9(BYTE bSkillKind, BYTE bSkillLevel, BYTE bSummonMasteryLevel, BYTE bSenderLevel, BYTE bRecverLevel);
	BOOL			IsSuccessByFormula5(BYTE bSkillKind, BYTE bSkillLevel, BYTE bSummonMasterySkillLevel, DWORD dwOffenseLevel, DWORD dwDefenceLevel);
	BOOL			IsSuccessByFormula6(BYTE bSkillKind, BYTE bSkillLevel);
	BOOL			IsSuccess(BYTE bSkillKind, BYTE bSkillLevel, AppliedSkill* pEffectDesc_Pray);
	float			GetFormula8(BYTE bSkillKind, BYTE bSkillLevel, float fOriginaParam);
	int				GetFormula9(BYTE bSkillKind, BYTE bSkillLevel, BYTE bOwnLevel, BYTE bTargetLevel, BYTE bSummonMasteryLevel);
	int				GetFormula16(BYTE bSkillKind, BYTE bSkillLevel, WORD wClass);
	int				GetFormula19(BYTE bSkillKind, BYTE bSkillLevel, WORD wClass);
	float			GetFormula15(BYTE bSkillKind, BYTE bSkillLevel, float fOriginalParam);
	int				GetFormula14(BYTE bSkillKind, BYTE bSkillLevel, BYTE bMasterySkillKind, BYTE bMasterySkillLevel);
	float			GetFormula3(BYTE bSkillKind, BYTE bSkillLevel, WORD wClass);
	float			GetFormula4(BYTE bSkillKind, BYTE bSkillLevel, WORD wClass, float fOriginalParam);
	float			GetFormula5(BYTE bSkillKind, BYTE bSkillLevel, float fOriginalParam);
	float			GetFormula17(BYTE bSkillKind, BYTE bSkillLevel, WORD wClass);
	float			GetFormula18(BYTE bSkillKind, BYTE bSkillLevel, WORD wClass);
	float			GetFormula23(BYTE bSkillLevel);
	int				GetFormula24(BYTE bSkillKind, BYTE bSkillLevel, WORD wClass);
	int				GetFormula25(BYTE bSkillKind, BYTE bSkillLevel, WORD wClass);
	void			SendSkillStatusUserMon(CUser* pSendUser, CMonster *pMonster, BYTE bSkillKind, BYTE bSkillLevel,DWORD dwEndTime); // ���½�ų�� ���� Ŭ���̾�Ʈ���� �������.
	void			SendSkillStatusUserUser(CUser *pSendUser, CUser *pTargetUser, BYTE bSkillKind, BYTE bSkillLevel,DWORD dwEndTime);// ���½�ų�� ���� Ŭ���̾�Ʈ���� �������.
		
	void			Init();			// �ʱ�ȭ
	Effect*			GetEffectInfo(BYTE bSkillKind);
	void			LoadScript();		// ��ũ��Ʈ ������ �о� �Ͷ�.
	int				GetEffectStatusPostion(BYTE bID);
	void			SendSkill(SkillCast* pSkillDesc);
	int				spOffsetPerSecondIfContinousSkill(const Effect* skill, DWORD skillLevel, const CUser* user);
	int				UserManaOffsetForSkillUsage(const CUser* pOwnUser, const SkillCast* pSkillDesc);
	void			updateKeepTimeForContinousSkill(const CUser* pOwnUser, SkillCast* pSkillDesc);
	BOOL			CanUnitCastSkill(BYTE bySkillKind, const CUnit* pUnit);

	~EffectLayer();
};

struct SKILL_COMMAND
{
	BYTE	bSkillKind;
	BYTE	bOwnType;
	BYTE	bTargetType;	// OBJECT_TYPE_PLAYER, OBJECT_TYPE_MONSTER, OBJECT_TYPE_TILE
	BYTE	bPK;			// pk �������..
	DWORD	dwTargetIndex;
	DWORD	dwTime;			// ����Ǿ��� Ÿ��.	
};


#pragma pack( pop )
extern EffectLayer*	g_pEffectLayer;