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
#include "../CommonServer/CommonClientDungeon.h"

extern DWORD	g_dwCurTick;

#define	NONE			0
#define CRIME_APPLY_ENEMY		1	// ��ų���ɶ� ����Ǵ� ����� ��������
#define CRIME_APPLY_FRIENDLY	2	// ��ų���ɶ� ����Ǵ� ����� �Ʊ�����
#define CRIME_APPLY_ENEMY_FRIENDLY_MONSTER 3 // ��ų���ɶ� ����Ǵ� ���ݽ�ų������ ���� ��ȯ�� ���Ϳ��Ը� ����Ҽ� �ְ� �Ѵ�.
											// ���� PK�϶��� ���δ� �����ų�� �ִ�.

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


typedef struct BASESKILL
{
	BYTE			skillKind;		// ���̵�.
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
	DWORD			dwCastingTimeMilliseconds;
	DWORD			dwCoolTimeMilliseconds;
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
class AppliedSkill;
#define LIGHT_DESCEX_TEMP_SHOW 0		// SHOW�������� HIDE��������.
struct LIGHT_DESCEX
{
	GXLIGHT_HANDLE		m_handle;		// ������ �Ծ����� ��Ÿ���� ����Ʈ �ڵ�
	LIGHT_DESC			m_sLightDesc;			// ����Ʈ ����ũ
	void				(*LightFunc)(AppliedSkill*);			// ����Ʈ ���.�����Ÿ��� ���ؼ�..	
	int					m_nDestTick;
	DWORD				m_dwTemp[10];
};
// ����Ʈ ǥ���� ���� ����ü.
class AppliedSkill
{
public:
	AppliedSkill(){};
	~AppliedSkill(){};
	BYTE				skillKind;		// ǥ���ҷ��� �ϴ� ����Ʈ�� ���� ���̵�.	
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
	
	DWORD				GetRemainTime(DWORD dwCurTick)	const
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
	AppliedSkill*				CreateMagicArray(BYTE bSkillKind, VECTOR3* vecStart, BOOL bOwn);	// ������ ����Ҷ� ������ �׸���.
	void					SetSkillStatusUserUser(CUser* pTargetUser, BYTE bSkillKind);
	void					SendSkillStatusUserUser();
	BOOL					CanSkillBeCastAtPosition(BYTE bSkillKind, VECTOR3* vecTarget);
	AppliedSkill*				CreateGXObject(char *szFile, BOOL bOwn, WORD wChrNum);
	AppliedSkill*				CreateStatusEffect(BYTE bSkillKind, BYTE bJoint, BOOL bOwn);
	void					SetSkillStatusUserMon(CMonster* pMonster, BYTE bSkillKind);
	void					SendSkillStatusUserMon();
	AppliedSkill*				CreateEffect(BYTE bSkillKind, BYTE bJoint, BOOL bOwn);
	BOOL					IsEffectShow(AppliedSkill* pEffectDesc);
	void					AttachLight(AppliedSkill* pEffectDesc, BYTE bLightNum, void(*LightFunc)(AppliedSkill*));
	void					DetachLight(AppliedSkill* pEffectDesc);
		
	void					Init(BOOL bChk);			// �ʱ�ȭ
	void					LoadScript(BOOL bChk);		// ��ũ��Ʈ ������ �о� �Ͷ�.

	int						GetEffectStatusPostion(BYTE bID)		{	return m_Effect[bID].GetEffectPosition();			}
	BYTE					GetSkillMasteryKind(BYTE bSkillKind)	{	return BYTE(bSkillKind/30*30+__SKILL_MANAMASTERY__);}
	Effect*					GetEffectInfo(BYTE bSkillID)			{	return &m_Effect[bSkillID];							}
	
~EffectLayer(){};
};


