/////////////////////////////////////
//
//
//		yakolla
//		skill.iti 파일을 던전 데이타폴더에 넣어 둔다.
/////////////////////////////////////
#pragma once
#include "../SS3D_0719/4DYUCHIGX/4DyuchiGRX_common/math.inl"
#include "../CommonServer/CommonClientDungeon.h"

#define	NONE			0
#define	SKILL_PROPERTY_KIND_FIRE		1	// 스킬 속성 불
#define	SKILL_PROPERTY_KIND_ICE			2	// 스킬 속성 얼음
#define	SKILL_PROPERTY_KIND_EARTH		3	// 스킬 속성 불1/2 물리1/2
#define	SKILL_PROPERTY_KIND_LIGHT		4	// 스킬 속성 전기
#define	SKILL_PROPERTY_KIND_PHY			5	// 스킬 속성 물리
#define SKILL_PROPERTY_KIND_POI			6	// 스킬 속성 독
#define CRIME_APPLY_ENEMY		1	// 스킬사용될때 적용되는 대상이 적군에게
#define CRIME_APPLY_FRIENDLY	2	// 스킬사용될때 적용되는 대상이 아군에게
#define CRIME_APPLY_ENEMY_FRIENDLY_MONSTER 3 // 스킬사용될때 적용되는 공격스킬이지만 나가 소환한 몬스터에게만 사용할수 있게 한다.
											// 물론 PK일때는 전부다 적용시킬수 있다.


class CUnit;
class CUser;
class CMonster;
class CDungeonLayer;
struct SKILL_TARGETINFO;
struct SKILL_RESULT;
struct DSTC_USER_STATUS;
//--> 스크립트 구조들
#pragma pack( push, 1 )

typedef struct BASESKILL
{
	BYTE			skillKind;				// 아이디.
	DWORD			dwResourceID;
	DWORD			dwStatusResourceID;
	WORD			wProperty;			// none : 0, 마법 : 500, Aura : 100, Divine, : 200, Summon : 300, Chakra : 400
	BYTE			bType;				// none : 0, Active : 1, Drive : 2, Passive : 3
	BYTE			bAbleClass;			// 스킬을 사용할수 있는 클래스
	BYTE			bSkillTarget;		// none : 0, monster : 1, 지역 : 2, player : 3
	BYTE			bSkillType;			// none : 0, player : 1, monster : object : 100, 2 직선 : 110, 원 : 120, X범위 : 130, 직교 : 140, 3중충돌 : 150, 타일셋 : 160
	DWORD			dwRange;
	DWORD			dwMinMastery;		// 최소 마스터리
	DWORD			dwCastingTime;
	DWORD			dwCoolTime;
	DWORD			dwEffectPosition;	// 31~29 bit none : 0, 머리위 : 1, 몸통 : 2, 바닥 : 4,  28~0 bit subkey
	BYTE			bJointEffect;
	BYTE			bFormula;			// 공식 
	BYTE			bSuccessFormula;	// 스킬 성공률에 관한 공식
	WORD			wSoundNum[8];		// 사운드 넘버들.
	STATUSVALUE		sStatusValue[MAX_STATUS_SKILLVALUE];	
	BYTE			bCrime;				// 적용대상이 적이면 1, 아군이면 2
	BYTE			bEfficiency;
//	char			szInfo[128];	// 스킬에 대한 간단한 설명.
	VALUE			Value[MAX_SKILL_LEVEL+1];	
} *LPBASESKILL;




//<-- 스크립트 구조들
struct Effect : public BASESKILL
{
public:
	
	int GetEffectPosition();
	int	GetMaxMastery(DWORD dwUserLevel, BYTE bSkillLevel, WORD wClass);	
};

enum
{
	EFFECT_DESC_TEMP_POISON_DAMAGE_MIN		=	0,	// 데미지 사용한다.
	EFFECT_DESC_TEMP_POISON_DAMAGE_MAX		=	1,	// 데미지 사용한다.
	EFFECT_DESC_TEMP_POISON_OWNER_TYPE		=	2,	// 중독에서 사용한다.
	EFFECT_DESC_TEMP_POISON_OWNER_INDEX		=	3,	// 중독에서 사용한다.
	EFFECT_DESC_TEMP_MAX 
};

struct AppliedSkill
{
	float		fParamPlus[5];					// 상태변화값.
	DWORD		dwDestTick;						// 끝날 시간.
	DWORD		dwLastUpdateTick;				// 마지막 으로 상태를 저용한 시간( 중독등의 스킬 적용시 사용 )
	DWORD		dwAmfleafireTime;				// 엠플리파이어 영향을 받아 시간이 늘어난시간을 계산.
	Effect*		pEffect;						// 이펙트 정보
	void*		pos;							// 리스트에 연결할때의 포지션.
	CUser*		pOwnUser;						// 스킬 쏜놈.
	DWORD		dwTemp[EFFECT_DESC_TEMP_MAX];	// 여러가지 용도로 사용할 공간
	BYTE		bSkillLevel;					// 스킬 레벨	
};

struct SkillCast
{
	BYTE			bSkillKind;					// 스킬 아이디
	char			bSkillLevel;				// 스킬 레벨 : zero base
	DWORD			dwSkillKeepTime;			// 스킬 지속시간 (단, 오버드라이브만 사용가능하다)
	BYTE			casterType;					// 스킬을 쏘는 객체 타입
	DWORD			casterDungeonID;					// 스킬을 쏘는 객체 인덱스
	BYTE			bTargetType;				// 스킬을 당하는 객체 타입
	DWORD			dwTargetIndex;				// 스킬을 당하는 객체 인덱스
	WORD			wTileIndex_X;				// 스킬 발동할 위치
	WORD			wTileIndex_Z;				// 스킬 발동할 위치
	VECTOR2			casterPosition;				// 스킬 쏘는 객체의 위치
	BOOL			casterPlayerPKFlagEnabled;						// PK 모드인가.
	WORD			wDamageMinMax[2];			// 스킬 데미지 최소 최대값을 기록
	CDungeonLayer*	pDungeonLayer;				// 쏘려고 하는 던전 층포인터
	BYTE			bSectionNum;				// 쏘려고 하는 섹션 넘버.
	CUser*			pMonsterMaster;				// 몬스터 소환시 사용되어진다.소환할 몬스터의 주인이 될 놈의 포인터를 넣어 준다.
};

struct EffectLayer
{	
	Effect			m_Effect[MAX_SKILL_COUNT];	
	
private:
	
	DWORD			Load(char* szFileName); // 파일 읽어.
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
	BYTE			GetSkillMasteryKind(BYTE bSkillKind);	// 해당종류의 마스터리 아이디를 얻어온다.
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
	void			SendSkillStatusUserMon(CUser* pSendUser, CMonster *pMonster, BYTE bSkillKind, BYTE bSkillLevel,DWORD dwEndTime); // 상태스킬을 만들어서 클라이언트에게 보내줘라.
	void			SendSkillStatusUserUser(CUser *pSendUser, CUser *pTargetUser, BYTE bSkillKind, BYTE bSkillLevel,DWORD dwEndTime);// 상태스킬을 만들어서 클라이언트에게 보내줘라.
		
	void			Init();			// 초기화
	Effect*			GetEffectInfo(BYTE bSkillKind);
	void			LoadScript();		// 스크립트 파일을 읽어 와라.
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
	BYTE	bPK;			// pk 모드인지..
	DWORD	dwTargetIndex;
	DWORD	dwTime;			// 적용되어질 타임.	
};


#pragma pack( pop )
extern EffectLayer*	g_pEffectLayer;