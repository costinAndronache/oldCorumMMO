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
#define CRIME_APPLY_ENEMY		1	// 스킬사용될때 적용되는 대상이 적군에게
#define CRIME_APPLY_FRIENDLY	2	// 스킬사용될때 적용되는 대상이 아군에게
#define CRIME_APPLY_ENEMY_FRIENDLY_MONSTER 3 // 스킬사용될때 적용되는 공격스킬이지만 나가 소환한 몬스터에게만 사용할수 있게 한다.
											// 물론 PK일때는 전부다 적용시킬수 있다.

#define __CHR_EFFECT_NONE__				(__SKILL_ATTACK__+1)
#define	__CHR_EFFECT_GUARDIAN_DYING__	(__CHR_EFFECT_NONE__+1)

#define SKILL_TEMP_EFFECT_TIME_FINISH	0		// 이펙트가 진행중일때 끝낼수 있는 플래그.
#define SKILL_TEMP_EFFECT_TIME_5SEC		1		// 이펙트가 끝나기 5초전에 소리 내라는 플래그.
#define	SKILL_TEMP_EFFECT_TIME_30SEC	2		// 이펙트가 끝나기 30초전에 소리 내라는 플래그.
#define SKILL_TEMP_CREATECOUNT			3		// 똑같은 이펙트가 여러개 만들어질때 만들어진 순서.
#define SKILL_TEMP_HOAMINGRANDOM		4		// 유도 계산에서 사용하는 랜덤중인지 플래그.
#define SKILL_TEMP_HOAMINGFRAMECOUNT	5		// 유도 계산에서의 진행 카운트
#define SKILL_TEMP_HOAMINGARROW			6		// 유도 계산에서의 방향플래그.
#define SKILL_TEMP_HOAMINGDRUNKENCOUNT	7		// 유도 게산에서 드렁큰모드로 전환된 카운트
#define SKILL_TEMP_EFFECT_TIME_1SEC		8		// 1초마다 처리하기 위한
#define SKILL_TEMP_EFFECT_ALPHA			9		// POTION에서 쓰일 알파 초기상태값.
#define	SKILL_TEMP_POSONING				10
#define	SKILL_TEMP_CAMERA_FRAMECOUNT	11		// 카메라 흔들린 프레임
#define	SKILL_TEMP_AMFLEAFIRETIME		12		// 엠플리 적용시켰을때의 오버드라이브의 남은 시간을 저장하여 엠플리가 사라졋을때 기준삼아 다시 시간을 깍아준다.
class CUser;
class CMonster;
class CEffectHash;
interface ISoundEffect;
//--> 스크립트 구조들
#pragma pack( push, 1 )


typedef struct BASESKILL
{
	BYTE			skillKind;		// 아이디.
	DWORD			dwResourceID;
	DWORD			dwStatusResourceID;
	WORD			wProperty; // none : 0, 마법 : 500, Aura : 100, Divine, : 200, Summon : 300, Chakra : 400
	BYTE			bType; // none : 0, Active : 1, Drive : 2, Passive : 3
	BYTE			bAbleClass;	// 스킬을 사용할수 있는 클래스
	char			szName[50];
	char			szDescription[255];	// 스킬에 대한 간단한 설명.
	char			szDescription2[2][64];		
	BYTE			bSkillTarget; // 0 = none, 1 = 땅, 2 = 몬스터(적군), 4 = 몬스터(아군), 8 = 유저(적군), 16 = 유저(아군), 32 = 적군에게는 pk모드일때만 사용가능한가?
	BYTE			bSkillType; // none : 0, player : 1, monster : 2, object : 100, 2 직선 : 110, 원 : 120, X범위 : 130, 직교 : 140, 3중충돌 : 150, 타일셋 : 160
	DWORD			dwRange;
	DWORD			dwMinMastery;	// 최소 마스터리
	DWORD			dwCastingTimeMilliseconds;
	DWORD			dwCoolTimeMilliseconds;
	DWORD			dwEffectPosition; // none : 0, 머리위 : 1, 몸통 : 2, 바닥 : 4
	BYTE			bJointEffect;
	BYTE			bFormula;		// 공식 
	BYTE			bSuccessFormula;	// 스킬 성공률에 관한 공식
	WORD			wSoundNum[8];	// 사운드 넘버들.
	STATUSVALUE		sStatusValue[MAX_STATUS_SKILLVALUE];
	BYTE			bCrime;			// 적용대상이 적이면 1, 아군이면 2
	BYTE			bEfficiency;
	
	VALUE			Value[MAX_SKILL_LEVEL+1];
	
	
} *LPBASESKILL;
#pragma pack( pop )
//<-- 스크립트 구조들
struct Effect : public BASESKILL
{
	
	void				(*SkillFunc)(GXOBJECT_HANDLE, OBJECT_DESC*, DWORD, BYTE);		// 스킬 펑션.
	void				(*MonsterDamageFunc)(GXOBJECT_HANDLE, OBJECT_DESC*, DWORD, BYTE);		// 몬스터 맞는 펑션.
	void				(*MonsterKillFunc)(GXOBJECT_HANDLE, OBJECT_DESC*, DWORD, BYTE);		// 몬스터 죽는 펑션.
	void				(*MonsterKillWithAction)(GXOBJECT_HANDLE);		// 몬스터 죽는 죽기전에 액션 펑션.
	void				(*MonsterSKillDamageWithAction)(GXOBJECT_HANDLE);		// 몬스터 스킬 데미지 액션 펑션.
	
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
#define LIGHT_DESCEX_TEMP_SHOW 0		// SHOW상태인지 HIDE상태인지.
struct LIGHT_DESCEX
{
	GXLIGHT_HANDLE		m_handle;		// 데미지 입었을때 나타나는 라이트 핸들
	LIGHT_DESC			m_sLightDesc;			// 라이트 데스크
	void				(*LightFunc)(AppliedSkill*);			// 라이트 펑션.깜빡거리기 위해서..	
	int					m_nDestTick;
	DWORD				m_dwTemp[10];
};
// 이펙트 표현을 위한 구조체.
class AppliedSkill
{
public:
	AppliedSkill(){};
	~AppliedSkill(){};
	BYTE				skillKind;		// 표현할려고 하는 이펙트의 정보 아이디.	
	BYTE				bJoint;				// 부가 이펙트 리소스 정보.
	WORD				wChrNum;			// chr 정보 참조 인덱스
	Effect*				pEffect;				// 이펙트 정보

	DWORD				dwCount;				// 이펙트 맞은놈의 수
	DWORD				dwOwnType;				// 스킬쓴놈의 타입
	DWORD				dwOwnIndex;				// 스킬쓴놈의 인덱스
	DWORD				dwFrameCount;			// 진행 카운트
	DWORD				dwFrameFinish;			// 멈춤 카운트
	DWORD				dwHashKey;
	
	float				f_Radian;				// 이펙트 진행 라디안.
	float				f_Radcount;
		
	DWORD				dwSkillLevel;
	DWORD				dwTemp[20];				
	DWORD				dwTargetIndex[20];		// 이펙트 맞은놈들의 인덱스
	BYTE				byTargetObjectType[20];	// 타겟속성. 몬스터인지 유저인지.
	void*				pUsingStatus;	
	BOOL				bOwn;					// 이펙트를 쏜게 자신이면 1 아니면 0 , 0xff = 이펙트 옵션오프 해도 적용받지 않는것들이다.
	VECTOR3				vecOwn;					// 스킬시직위치.
	VECTOR3				vecBasePosion;			// 땅을 클릭했을때를 위한 위치.

	LIGHT_DESCEX		m_sLightDescEx;
	OBJECT_HANDLE_DESC	hEffect;	// 이펙트의 오브젝트 핸들.
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
	
	DWORD					Load(char* szFileName); // 파일 읽어.
	DWORD					ParsingSkillDescription(BYTE bSkillKind);
	BOOL					SearchString(BYTE bSkillKind, int nLen, int nIndex);
	DWORD					m_dwCount;

public:
	AppliedSkill*				CreateMagicArray(BYTE bSkillKind, VECTOR3* vecStart, BOOL bOwn);	// 마법을 사용할때 마법원 그리기.
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
		
	void					Init(BOOL bChk);			// 초기화
	void					LoadScript(BOOL bChk);		// 스크립트 파일을 읽어 와라.

	int						GetEffectStatusPostion(BYTE bID)		{	return m_Effect[bID].GetEffectPosition();			}
	BYTE					GetSkillMasteryKind(BYTE bSkillKind)	{	return BYTE(bSkillKind/30*30+__SKILL_MANAMASTERY__);}
	Effect*					GetEffectInfo(BYTE bSkillID)			{	return &m_Effect[bSkillID];							}
	
~EffectLayer(){};
};


