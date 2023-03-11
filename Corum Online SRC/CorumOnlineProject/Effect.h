/////////////////////////////////////
//
//
//		yakolla
//
/////////////////////////////////////
#pragma once
#include "ObjectDefine.h"
#include "Define.h"
#include "SoundLib.h"
#include "GameDefine.h"
extern DWORD	g_dwCurTick;
#define MAX_SKILL_LEVEL				50
#define ITI_TYPE_BASESKILL			3000
#define MAX_KIND_OF_CLASS_EX		MAX_KIND_OF_CLASS + 2	// Ŭ������ ������ �ʴ� ����Ʈ�� �ʿ� �ؼ�.
#define MAX_STATUS_SKILLVALUE		5		// ���½�ų�� ���� ��ȭ�Ҽ� �ִ°�.

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
#define	TYPE_PASSIVE	3
#define TYPE_TIMEZERO	4

#define	NONE			0
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
#define	__SKILL_RAGE__					96
#define	__SKILL_REDELEMENTAL__			97
#define __SKILL_SOULBLASTER				98
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
#define __SKILL_FORTIFICATIONMASTERY__	144
#define __SKILL_AMPLIFICATIONMASTERY__	145
#define __SKILL_POISONMASTERY__			146
#define __SKILL_EXTREMESPEAR__			147
#define __SKILL_ATTACK__				151
#define __SKILL_NONE_SELECT__			255

#define __CHR_EFFECT_NONE__				(__SKILL_ATTACK__+1)
#define	__CHR_EFFECT_GUARDIAN_DYING__	(__CHR_EFFECT_NONE__+1)

#define SKILL_TEMP_EFFECT_TIME_FINISH	0		// ����Ʈ�� �������϶� ������ �ִ� �÷���.
#define SKILL_TEMP_EFFECT_TIME_5SEC		1		// ����Ʈ�� ������ 5������ �Ҹ� ����� �÷���.
#define	SKILL_TEMP_EFFECT_TIME_30SEC	2		// ����Ʈ�� ������ 30������ �Ҹ� ����� �÷���.
#define SKILL_TEMP_CREATECOUNT			3		// �Ȱ��� ����Ʈ�� ������ ��������� ������� ����.
#define SKILL_TEMP_HOAMINGRANDOM		4		// ���� ��꿡�� ����ϴ� ���������� �÷���.
#define SKILL_TEMP_HOAMINGFRAMECOUNT	5		// ���� ��꿡���� ���� ī��Ʈ
#define SKILL_TEMP_HOAMINGARROW			6		// ���� ��꿡���� �����÷���.
#define SKILL_TEMP_HOAMINGDRUNKENCOUNT	7		// ���� �Ի꿡�� �巷ū���� ��ȯ�� ī��Ʈ
#define SKILL_TEMP_EFFECT_TIME_1SEC		8		// 1�ʸ��� ó���ϱ� ����
#define SKILL_TEMP_EFFECT_ALPHA			9		// POTION���� ���� ���� �ʱ���°�.
#define	SKILL_TEMP_POSONING				10
#define	SKILL_TEMP_CAMERA_FRAMECOUNT	11		// ī�޶� ��鸰 ������
#define	SKILL_TEMP_AMFLEAFIRETIME		12		// ���ø� ������������� ��������̺��� ���� �ð��� �����Ͽ� ���ø��� ������� ���ػ�� �ٽ� �ð��� ����ش�.
class CUser;
class CMonster;
class CEffectHash;
interface ISoundEffect;
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
	BYTE			bID;		// ���̵�.
	DWORD			dwResourceID;
	DWORD			dwStatusResourceID;
	WORD			wProperty; // none : 0, ���� : 500, Aura : 100, Divine, : 200, Summon : 300, Chakra : 400
	BYTE			bType; // none : 0, Active : 1, Drive : 2, Passive : 3
	BYTE			bAbleClass;	// ��ų�� ����Ҽ� �ִ� Ŭ����
	char			szName[50];
	char			szDescription[255];	// ��ų�� ���� ������ ����.
	char			szDescription2[2][64];		
	BYTE			bSkillTarget; // 0 = none, 1 = ��, 2 = ����(����), 4 = ����(�Ʊ�), 8 = ����(����), 16 = ����(�Ʊ�), 32 = �������Դ� pk����϶��� ��밡���Ѱ�?
	BYTE			bSkillType; // none : 0, player : 1, monster : 2, object : 100, 2 ���� : 110, �� : 120, X���� : 130, ���� : 140, 3���浹 : 150, Ÿ�ϼ� : 160
	DWORD			dwRange;
	DWORD			dwMinMastery;	// �ּ� �����͸�
	DWORD			dwCastingTime;
	DWORD			dwCoolTime;
	DWORD			dwEffectPosition; // none : 0, �Ӹ��� : 1, ���� : 2, �ٴ� : 4
	BYTE			bJointEffect;
	BYTE			bFormula;		// ���� 
	BYTE			bSuccessFormula;	// ��ų �������� ���� ����
	WORD			wSoundNum[8];	// ���� �ѹ���.
	STATUSVALUE		sStatusValue[MAX_STATUS_SKILLVALUE];
	BYTE			bCrime;			// �������� ���̸� 1, �Ʊ��̸� 2
	BYTE			bEfficiency;
	
	VALUE			Value[MAX_SKILL_LEVEL+1];
	
	
} *LPBASESKILL;
#pragma pack( pop )
//<-- ��ũ��Ʈ ������
struct Effect : public BASESKILL
{
	
	void				(*SkillFunc)(GXOBJECT_HANDLE, OBJECT_DESC*, DWORD, BYTE);		// ��ų ���.
	void				(*MonsterDamageFunc)(GXOBJECT_HANDLE, OBJECT_DESC*, DWORD, BYTE);		// ���� �´� ���.
	void				(*MonsterKillFunc)(GXOBJECT_HANDLE, OBJECT_DESC*, DWORD, BYTE);		// ���� �״� ���.
	void				(*MonsterKillWithAction)(GXOBJECT_HANDLE);		// ���� �״� �ױ����� �׼� ���.
	void				(*MonsterSKillDamageWithAction)(GXOBJECT_HANDLE);		// ���� ��ų ������ �׼� ���.
	
public:
	int					GetEffectPosition()				{	return dwEffectPosition;	}
	DWORD				GetClassType()					{	return wProperty/100;		}
	BOOL				IsRangeSkill()					{	return bSkillType/10 > 10;	}
	int					GetMaxMastery(BYTE bSkillLevel)
	{
	/*
		if (wProperty/100 == g_pMainPlayer->m_wClass)
			return (Value[bSkillLevel].nMax*(*((int*)&g_sBaseClassInfo[g_pMainPlayer->m_wClass-1].iMax_Aura+(wProperty/100-1)))/100.) + (g_pMainPlayer->m_dwLevel * 0.5);
		else
			return (Value[bSkillLevel].nMax*(*((int*)&g_sBaseClassInfo[g_pMainPlayer->m_wClass-1].iMax_Aura+(wProperty/100-1)))/100.);
			*/
		//return (Value[bSkillLevel].nMax*(*((int*)&g_sBaseClassInfo[g_pMainPlayer->m_wClass-1].iMax_Aura+(wProperty/100-1)))/100.);
		return bSkillLevel+1;
	}

};
class EffectDesc;
#define LIGHT_DESCEX_TEMP_SHOW 0		// SHOW�������� HIDE��������.
struct LIGHT_DESCEX
{
	GXLIGHT_HANDLE		m_handle;		// ������ �Ծ����� ��Ÿ���� ����Ʈ �ڵ�
	LIGHT_DESC			m_sLightDesc;			// ����Ʈ ����ũ
	void				(*LightFunc)(EffectDesc*);			// ����Ʈ ���.�����Ÿ��� ���ؼ�..	
	int					m_nDestTick;
	DWORD				m_dwTemp[10];
};
// ����Ʈ ǥ���� ���� ����ü.
class EffectDesc
{
public:
	EffectDesc(){};
	~EffectDesc(){};
	BYTE				bEffectInfoNum;		// ǥ���ҷ��� �ϴ� ����Ʈ�� ���� ���̵�.	
	BYTE				bJoint;				// �ΰ� ����Ʈ ���ҽ� ����.
	WORD				wChrNum;			// chr ���� ���� �ε���
	Effect*				pEffect;				// ����Ʈ ����

	DWORD				dwCount;				// ����Ʈ �������� ��
	DWORD				dwOwnType;				// ��ų������ Ÿ��
	DWORD				dwOwnIndex;				// ��ų������ �ε���
	DWORD				dwFrameCount;			// ���� ī��Ʈ
	DWORD				dwFrameFinish;			// ���� ī��Ʈ
	DWORD				dwHashKey;
	
	float				f_Radian;				// ����Ʈ ���� ����.
	float				f_Radcount;
		
	DWORD				dwSkillLevel;
	DWORD				dwTemp[20];				
	DWORD				dwTargetIndex[20];		// ����Ʈ ��������� �ε���
	BYTE				byTargetObjectType[20];	// Ÿ�ټӼ�. �������� ��������.
	void*				pUsingStatus;	
	BOOL				bOwn;					// ����Ʈ�� ��� �ڽ��̸� 1 �ƴϸ� 0 , 0xff = ����Ʈ �ɼǿ��� �ص� ������� �ʴ°͵��̴�.
	VECTOR3				vecOwn;					// ��ų������ġ.
	VECTOR3				vecBasePosion;			// ���� Ŭ���������� ���� ��ġ.

	LIGHT_DESCEX		m_sLightDescEx;
	OBJECT_HANDLE_DESC	hEffect;	// ����Ʈ�� ������Ʈ �ڵ�.
	ISoundEffect*		m_pSound[8];
	SOUND_FILE_HANDLE	m_hSoundFile[8];
	
	DWORD				GetRemainTime(DWORD dwCurTick)	
	{
		return (hEffect.pDesc->dwDestTime > dwCurTick) ?
			((hEffect.pDesc->dwDestTime - dwCurTick)/1000) : 0;
	}
};


struct EffectLayer
{
	Effect					m_Effect[MAX_SKILL_COUNT];
	
	BYTE					m_nEffectSize[MAX_SKILL_COUNT];
	
private:
	
	DWORD					Load(char* szFileName); // ���� �о�.
	DWORD					ParsingSkillDescription(BYTE bSkillKind);
	BOOL					SearchString(BYTE bSkillKind, int nLen, int nIndex);
	DWORD					m_dwCount;

public:
	EffectDesc*				CreateMagicArray(BYTE bSkillKind, VECTOR3* vecStart, BOOL bOwn);	// ������ ����Ҷ� ������ �׸���.
	void					SetSkillStatusUserUser(CUser* pTargetUser, BYTE bSkillKind);
	void					SendSkillStatusUserUser();
	BOOL					IsEffectUse(BYTE bSkillKind, VECTOR3* vecTarget, DWORD dwStartSkillTick);
	EffectDesc*				CreateGXObject(char *szFile, BOOL bOwn, WORD wChrNum);
	EffectDesc*				CreateStatusEffect(BYTE bSkillKind, BYTE bJoint, BOOL bOwn);
	void					SetSkillStatusUserMon(CMonster* pMonster, BYTE bSkillKind);
	void					SendSkillStatusUserMon();
	EffectDesc*				CreateEffect(BYTE bSkillKind, BYTE bJoint, BOOL bOwn);
	BOOL					IsEffectShow(EffectDesc* pEffectDesc);
	void					AttachLight(EffectDesc* pEffectDesc, BYTE bLightNum, void(*LightFunc)(EffectDesc*));
	void					DetachLight(EffectDesc* pEffectDesc);
		
	void					Init(BOOL bChk);			// �ʱ�ȭ
	void					LoadScript(BOOL bChk);		// ��ũ��Ʈ ������ �о� �Ͷ�.

	int						GetEffectStatusPostion(BYTE bID)		{	return m_Effect[bID].GetEffectPosition();			}
	BYTE					GetSkillMasteryKind(BYTE bSkillKind)	{	return BYTE(bSkillKind/30*30+__SKILL_MANAMASTERY__);}
	Effect*					GetEffectInfo(BYTE bSkillID)			{	return &m_Effect[bSkillID];							}
	
~EffectLayer(){};
};


