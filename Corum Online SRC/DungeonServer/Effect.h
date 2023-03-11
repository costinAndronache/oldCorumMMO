/////////////////////////////////////
//
//
//		yakolla
//		skill.iti ������ ���� ����Ÿ������ �־� �д�.
/////////////////////////////////////
#pragma once
#include "..\\4DyuchiGRX_common\\math.inl"
#define     MAX_SKILL					151
#define MAX_SKILL_COUNT					200		// ���۸� ���ؼ�.
#define MAX_SKILL_LEVEL				50
#define ITI_TYPE_BASESKILL			3000
#define MAX_KIND_OF_CLASS_EX		MAX_KIND_OF_CLASS + 2	// Ŭ������ ������ �ʴ� ����Ʈ�� �ʿ� �ؼ�.
#define MAX_STATUS_SKILLVALUE		5		// ���½�ų�� ���� ��ȭ�Ҽ� �ִ°�.
#define PROPERTY_AURA	100
#define PROPERTY_DIVINE	200
#define PROPERTY_SUMMON	300
#define PROPERTY_CHAKRA	400
#define PROPERTY_MAGIC	500

#define TARGET_MONSTER		1		// ��ų�� Ŭ���ؾ� �ϴ� ���(����)
#define TARGET_TILE			2		// ��ų�� Ŭ���ؾ� �ϴ� ���(Ÿ��)
#define TARGET_PLAYER		4		// ��ų�� Ŭ���ؾ� �ϴ� ���(�÷��̾�)
#define TARGET_ALL			7		// ��ų�� Ŭ���ؾ� �ϴ� ���(���)
#define TARGET_PKPLAYER		8		// ��ų�� Ŭ���ؾ� �ϴ� ���(PK����϶� PLAYER)

#define TARGETTYPE_NONE		0
#define TARGETTYPE_TILE		1
#define TARGETTYPE_ENEMY_MONSTER  2	// ����� ����
#define TARGETTYPE_FRIEND_MONSTER  4	// ����� ����
#define	TARGETTYPE_ENEMY_PLAYER	8		// ����� �÷��̾�
#define	TARGETTYPE_FRIEND_PLAYER	16		// ����� �÷��̾�

#define TARGETTYPE_OBJECT	100		// ��ü
#define	TARGETTYPE_LINE		110		// ����
#define	TARGETTYPE_CIRCLE	120		// ���ȿ� �ִ� ���
#define	TARGETTYPE_X		130		// X ��翡 �ִ³��
#define TARGETTYPE_BOX		 140	// �ڽ�
#define TARGETTYPE_MULTICOLLITION 150	// 3�� �浹
#define TARGETTYPE_TIMEZEROTILE 160	// Ÿ�Ͽ��ٰ� ���ڸ� �ڴ´�. �ð�����.
#define TARGETTYPE_SUMMONTILE	170 // Ÿ�Ͽ��ٰ� ��ȯ�� �Ѵ�.
#define	TARGETTYPE_ARC		180		// ��ä��
#define	TYPE_ACTIVE		1
#define	TYPE_DRIVE		2
#define	TYPE_PASSIVE		3
#define TYPE_TIMEZERO	4
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

// ���� �迭
#define	__SKILL_MANAMASTERY__			1
#define	__SKILL_FIREMISSILE__			2
#define	__SKILL_FIREBLAZE__				3
#define	__SKILL_MAGMAWALL__				4
#define	__SKILL_ICEMISSILE__			5
#define	__SKILL_ICEWAVE__				6
#define	__SKILL_ICEWALL__				7
	
#define	__SKILL_EARTHQUAKE__			9
#define	__SKILL_METEOR__				10
#define	__SKILL_LIGHTNING__				11
#define	__SKILL_AIREALCOAT__			12
#define	__SKILL_THUNDERSTORM__			13
#define	__SKILL_MANAEXPAND__			14
#define	__SKILL_SPEEDCASTING__			15
#define	__SKILL_EXPLOSION__				16
#define	__SKILL_MEDITETION__			17
#define	__SKILL_MULTICASTING__			18
#define	__SKILL_CONCENTRATION__			19
#define __SKILL_BLASTING__				20
#define __SKILL_FREEZING__				21
#define __SKILL_FLASHSHOCK__			22
#define __SKILL_DISPEL__				23
#define __SKILL_FIREMASTERY__			24
#define __SKILL_ICEMASTERY__			25
#define __SKILL_LIGHTMASTERY__			26
#define __SKILL_DRAGONICFIREBLAST__		28
#define __SKILL_DRAGONICVIBRATION__		29
// ���� �迭
#define	__SKILL_AURAMASTERY__			31
#define	__SKILL_AURARECHARGE__			32
#define	__SKILL_ZEAL__					33
#define	__SKILL_BASH__					34
#define __SKILL_LIGHTFLING				35
#define	__SKILL_RAGINGSWORD__			36
#define __SKILL_LIFEBALANCE__			37
#define	__SKILL_LIGHTNINGBREAK__		38
#define	__SKILL_SILENTBRANDYSHIP__		39
#define	__SKILL_ANTAGONIZE__			41
#define	__SKILL_AURABATTLER__			42
#define	__SKILL_LIFEFORCE__				43
#define	__SKILL_REGENERATION__			44
#define	__SKILL_WEAPONMASTERY__			45
#define	__SKILL_DEFENSEMASTERY__		46
#define __SKILL_SWORDMASTERY__			47
#define __SKILL_BLUNTWEAPON_FIGTER_MASTERY__	48
#define __SKILL_SPEARMASTERY__			49
#define __SKILL_INFULX__				50
#define __SKILL_AMPLIFY__				51
#define __SKILL_BERSERKER__				52

// ����� �迭
#define	__SKILL_DIVINEMASTERY__			61
#define	__SKILL_HEAL__					62
#define	__SKILL_PRAY__					63
#define	__SKILL_BLESSING__				64
#define	__SKILL_WINDTALES__				65
#define	__SKILL_HOLDSHIELD__			66
#define	__SKILL_WINDFORCE__				67
#define	__SKILL_TREEAGION__				68
#define	__SKILL_AMFLEAFIRE__			69
#define	__SKILL_HOLYPRESSURE__			70
#define	__SKILL_APSOLUTMAGICBARRIER__	71
#define	__SKILL_SPELLBINDING__			72
#define	__SKILL_DETECT__				73
#define	__SKILL_AROSE__					74
#define	__SKILL_HOLYDIVINE__			75
#define __SKILL_ORDRISING__				77
#define __SKILL_SUITRISING__			78
#define __SKILL_ORDSLUM__				79
#define __SKILL_SUITSLUM__				80
#define __SKILL_REMEDY__				81
#define __SKILL_PROTECTSHIELD__			82
#define __SKILL_FRAIL__					83
#define __SKILL_DETECTION__				84
#define __SKILL_ROUNDRANGE__			85
#define __SKILL_BLUNTWEAPON_PREST_MASTERY__	86
#define __SKILL_HOLYBRAIN				87
#define __SKILL_INCREASING				88
#define	__SKILL_LANDINGSUPPORT__		89
// ���� �迭
#define	__SKILL_SUMMONMASTERY__			91
#define	__SKILL_CALMDOWN__				92
#define	__SKILL_SLEEP__					93
#define	__SKILL_CONFUSE__				94
#define	__SKILL_MINDCONTROL__			95
#define	__SKILL_RAGE__					96		//kk
#define	__SKILL_REDELEMENTAL__			97
#define __SKILL_SOULBLASTER__			98
#define	__SKILL_MINDEXPLOSION__			99
#define	__SKILL_SOULETER__				100
#define	__SKILL_SUMMONACCELERATOR__		101
#define	__SKILL_DOMENATION__			102
#define	__SKILL_TAMINGMASTERY__			103
#define	__SKILL_BEYONDLIMIT__			104
#define __SKILL_BLUEELEMENTAL__			105
#define __SKILL_GREENELEMENTAL__		106
#define __SKILL_MIRACULOUSHEART__		107
#define __SKILL_ARCANUMSKIN__			108
#define __SKILL_FRENZY__				109
#define __SKILL_DETONATION__			110
#define __SKILL_TURNOVER__				111
#define __SKILL_DETONATION_MONSTER__	112
// ��ũ�� �迭
#define	__SKILL_CHAKRAMASTERY__			121
#define	__SKILL_POISONING__				123
#define	__SKILL_CHAINATTACKER__			124
#define	__SKILL_IRONSKIN__				125
#define	__SKILL_SNIPING__				126
#define	__SKILL_SHADOWSQUARE__			127
#define	__SKILL_HIDING__				128
#define	__SKILL_SPELLTRAP__				129
#define	__SKILL_PRESSURE__				130
#define	__SKILL_SYLPHID__				131
#define	__SKILL_SHAUT__					132
#define	__SKILL_BERSERK__				133
#define	__SKILL_SPEEDOVER__				134
#define	__SKILL_DODGE__					135
#define	__SKILL_TRAINING__				136
#define	__SKILL_BLASTQUAKE__			137
#define	__SKILL_LIFEEXPLOSION__			138
#define __SKILL_DEATHBLOW__				139
#define __SKILL_VAMPIRE__				140
#define __SKILL_POSIONCLOUD__			141
#define __SKILL_DARK__					143
#define __SKILL_FORTIFICATIONMASTERY__	144
#define __SKILL_AMPLIFICATIONMASTERY__	145
#define __SKILL_POISONMASTERY__			146
#define __SKILL_EXTREMESPEAR__				147
#define __SKILL_ATTACK__				151
#define __SKILL_NONE_SELECT__			255

#define EFFECT_TYPE_STATUS_TOP		1		
#define EFFECT_TYPE_STATUS_CENTER	2		
#define EFFECT_TYPE_STATUS_BOTTOM	4		

class CUnit;
class CUser;
class CMonster;
class CDungeonLayer;
struct SKILL_TARGETINFO;
struct SKILL_RESULT;
struct DSTC_USER_STATUS;
//--> ��ũ��Ʈ ������
#pragma pack( push, 1 )

struct VALUE
{
	int		nMin;
	int		nMax;
	int		nMana;
	int		nCompass;
	int		nDuration;
	int		nProbability;
};

struct STATUSVALUE
{
	WORD			wStatusID;	// ���¸����� ������ ��ȭ�Ǵ� �ʵ� �÷���.
	BYTE			bFormula;	// �Ŀ� ���� ���� ����ϱ� ���ؼ�.
};

typedef struct BASESKILL
{
	BYTE			bID;				// ���̵�.
	DWORD			dwResourceID;
	DWORD			dwStatusResourceID;
	WORD			wProperty;			// none : 0, ���� : 500, Aura : 100, Divine, : 200, Summon : 300, Chakra : 400
	BYTE			bType;				// none : 0, Active : 1, Drive : 2, Passive : 3
	BYTE			bAbleClass;			// ��ų�� ����Ҽ� �ִ� Ŭ����
//	char			szName_Kor[50];
//	char			szName_Eng[25];
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

struct EffectDesc
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

struct SKILLDESC
{
	BYTE			bSkillKind;					// ��ų ���̵�
	char			bSkillLevel;				// ��ų ���� : zero base
	DWORD			dwSkillKeepTime;			// ��ų ���ӽð� (��, ��������̺길 ��밡���ϴ�)
	BYTE			bOwnType;					// ��ų�� ��� ��ü Ÿ��
	DWORD			dwOwnIndex;					// ��ų�� ��� ��ü �ε���
	BYTE			bTargetType;				// ��ų�� ���ϴ� ��ü Ÿ��
	DWORD			dwTargetIndex;				// ��ų�� ���ϴ� ��ü �ε���
	WORD			wTileIndex_X;				// ��ų �ߵ��� ��ġ
	WORD			wTileIndex_Z;				// ��ų �ߵ��� ��ġ
	VECTOR2			v2OwnObjectPos;				// ��ų ��� ��ü�� ��ġ
	BOOL			bPK;						// PK ����ΰ�.
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
	BOOL			MessyProcessForUser(CUser* pOwnUser, SKILLDESC* pSkillDesc);
	BOOL			MessyProcessForSystem(CUser* pOwnUser, SKILLDESC* pSkillDesc);
	BYTE			GetAppliedTargetCount(CDungeonLayer* pDungeonLayer, SKILLDESC* pSkillDesc, SKILL_TARGETINFO* pTargetInfo, VECTOR2* pV2Start);
	void			RevisionStartPositon(SKILLDESC* pSkillDesc, VECTOR2* pV2Start);
	BOOL			IsRange(SKILLDESC* pSkillDesc, VECTOR2* pV2Start);
	CUnit*			GetValiidUnit(SKILLDESC* pSkillDesc);
	BOOL			IsSkillSuccess(SKILLDESC* pSkillDesc, CUnit* pOffense, CUnit* pDefense, SKILL_RESULT* pSkill_result);
	void			AfterSkillSuccessProcess(SKILLDESC* pSkillDesc, CUnit* pOffense, CUnit* pDefense, DSTC_USER_STATUS*	pUserStatus);
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
	BOOL			IsSuccess(BYTE bSkillKind, BYTE bSkillLevel, EffectDesc* pEffectDesc_Pray);
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
	void			SendSkill(SKILLDESC* pSkillDesc);
	int				GetUsedSPSkill(CUser* pOwnUser, SKILLDESC* pSkillDesc);
	BOOL			IsUnitStatusReadySkill(BYTE bySkillKind, const CUnit* pUnit);

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