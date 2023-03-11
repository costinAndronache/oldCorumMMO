// class CMonster
#pragma once
#include "ObjectDefine.h"
//#include "DamageNumber.h"
//#include "Map.h"
#include "SoundControl.h"
#include "SoundLib.h"
#include "MonsterManager.h"

// Proto Type
class CUser;

#define		MONSTER_TEMP_PUSHING					1		// ���� �и�ó���� �����ٴ� �÷���.
#define		MONSTER_TEMP_FRAMECOUNT					2		// �и�ó���� ���� �����ӱ�� ����.
#define		MONSTER_TEMP_PUSHPOWER					3		// �и����� �Ŀ��� �����ϴ� 
#define 	MONSTER_TEMP_PUSHPOWER1					4		// �и����� �Ŀ��� �����ϴ� 
#define		MONSTER_TEMP_KILLERINDEX				5		//	���� ���� �����ε���
#define		MONSTER_TEMP_SKILL_POISON_RESIST		6		// �� ���׷�
#define		MONSTER_TEMP_TARGET_TYPE				11		// Ÿ���� Ÿ��. ����? ����?
#define		MONSTER_TEMP_TARGET_INDEX				12		// Ÿ���� �ε�����ȣ.
#define		MONSTER_TEMP_SELECTED					13		// ����� ���� ���� �÷���
#define		MONSTER_TEMP_DYINGFRAME					14		// ���� �״�ó���� ���� �ð�üũ�� �ض�.
#define		MONSTER_TEMP_FINISHDYINGFRAME			15		// ���� �״�ó���� ���� ������ �ð�üũ�� �ض�.

#define		MONSTER_TEMP_MONMON_DAMAGE_TYPE			26		// ���ͳ��� �οﶧ ���̴� ������ Ÿ��
#define		MONSTER_TEMP_MONMON_DAMAGE				27		// ���ͳ��� �οﶧ ���̴� ������ ��
#define		MONSTER_TEMP_DAMAGE_TYPE				28		// �̽����� ������� ��������
#define		MONSTER_TEMP_DAMAGE						29		// ��������								

#define		MONSTER_MAXHP			1
#define		MONSTER_MAXMP			2
#define		MONSTER_HP				3
#define		MONSTER_MP				4
#define		MONSTER_EXP				5

#define		MAX_GUARDIAN_USE_SKILL	5

enum GUARDIAN_CREATE_STATUS
{
	GUARDIAN_CREATE_STATUS_NONE = 0,		// �ʱ� ����
	GUARDIAN_CREATE_STATUS_ALREADYCALL = 1,	// �̹� ���ѳ��̱���.
	GUARDIAN_CREATE_STATUS_CREATING = 3,	// �������̾�.
};

enum GUARDIAN_ITEM_TYPE
{
	GUARDIAN_ITEM_TYPE_EGG = 1,				// �ʶ�
	GUARDIAN_ITEM_TYPE_ADULT = 2,			// ����
	GUARDIAN_ITEM_TYPE_DIE = 3,				// ������
	GUARDIAN_ITEM_TYPE_BROKEN = 4,			// ������
	GUARDIAN_ITEM_TYPE_SOUL = 5,			// ��ȥ
};
//#pragma pack( push, 1 )


class CMonster			// ������� �����ϴ� Ŭ�����̴�.
{
private:
	BYTE				m_bStatus;						// Monster Status..

public:
	char				m_szName[50];
	DWORD				m_dwMonsterIndex;				// Monster Index..
	BYTE				m_bClass;				
	DWORD				m_dwMonsterKind;				// Monster? or Guardian?

	WORD				m_wCurSectionNum;				// ������ PrevSectionNum�� ���� ����.
	
	VECTOR3				m_v3CurPos;						// Current Position..
	VECTOR3				m_v3DestPos;
	VECTOR3				m_v3Direction;					// Current Direction..

	OBJECT_HANDLE_DESC	m_hMonster;						// 20

	EffectDesc*			m_pEffectTarget;						// �ڽ��� ���� ���͸� ǥ���ҋ� ����.
	EffectDesc*			m_pEffectSelect;						// �ڽ��� ���� ���͸� �����ҋ� ����.
	
	GXOBJECT_HANDLE		m_hShadowHandle;
	
	DWORD				m_wModNo;						// �ӽ�.
	float				m_fSpeed;
	BYTE				m_byBaseScale;
	DWORD				m_dwHP;
	DWORD				m_dwMaxHP;
	DWORD				m_dwExp;
	DWORD				m_dwLevel;
	WORD				m_wStatPoint;
	WORD				m_wEgo;
	WORD				m_wStr;
	WORD				m_wInt;
	WORD				m_wDex;
	WORD				m_wVit;
	WORD				m_wAttackAcuracy;		// ������
	WORD				m_wAvoid;				// ȸ����
	short				m_wFireResist;
	short				m_wIceResist;
	short				m_wLightResist;
	short				m_wPoiResist;
	short				m_wPhyResist;
	WORD				m_pwAttackDamage_L[2];
	MAP_TILE*			m_pCurTile;	
	// slowboat add ///////////////////////
	DWORD				m_dwTemp[30];
	///////////////////////////////////////
	//=======> yakolla
	BYTE				m_bOverDriveCount;
	BYTE				m_bZipCode;
	DWORD				m_dwLordIndex;
	
	COnlyList*			m_pEffectList;				// ����Ʈ�� �� ��� ���� ����Ʈ�� �°��� ������ ť
	COnlyList*			m_pUsingStatusEffectList;	// ���� ����ް� �ִ� ��������Ʈ ����Ʈ
	EffectDesc*			m_pEffectDesc[MAX_SKILL_COUNT];
	VECTOR3				m_vecTemp;		// �ʱ� ��ġ
	float				m_radian;		// Ƣ�� ����
	float				m_fDir;			// ȸ���Ҷ� ���� ����.
	//<======== yakolla

	// ����� ���õ� ��ų : �ִ��� 2005.3.3
	MONSTER_SKILL		m_Skill[MAX_GUARDIAN_USE_SKILL];
	BYTE				m_bSelectedSkill;

	////////////////////////////////////////////////////////////
	// sound_new test
//#if !defined(SOUND_NEW)	
	// �ӽ� ����.
	SOUND_FILE_HANDLE	m_hSoundFile[10];
	ISoundEffect *		m_pSound[10];
	CSoundControl *		m_pSoundControl;	
		

//#endif
	//==

	WORD				wNeu;
	WORD				wDamage[2];
	WORD				wAttack[2];
	WORD				wDeath[2];
	WORD				wMove[2];
	POSITION_			m_pDyingList;		// ���ѽð��� ������ ���� �ʴ´ٸ� ������ �׿��� �ϱ� ���� ����Ʈ�� �߰��϶�.��.��
	
	BYTE				m_bSkillTmp;
	BYTE				m_bCongestionMode;	// ���ָ�� ���
private:
	void				RemoveAllStatusEffect();
	
public:
	BOOL				UsingStatusSkill(BYTE bSkillKind);
	void				AttachSkill(BYTE bSkillKind, BYTE bSkillLevel, DWORD dwEndTime);
	void				RemoveResource();
	void				CreateResource();
	void				SetStatus(BYTE bStatus, void (*ObjectFunc)(GXOBJECT_HANDLE, OBJECT_DESC*, DWORD, BYTE));
	BYTE				GetStatus();
	void				SetDamageIndex( DWORD dwDamage );
	void				SetDamageChar( DWORD dwChar );
	void				SetAction( WORD wAct, int nFrame = 0, BYTE bFlag = ACTION_LOOP );
	void				SetActionNext( WORD wAct, WORD wNextAct, BYTE bNextFlag = ACTION_LOOP, int nNextFrame = 0 );
	void				(*WithActionFunc)(GXOBJECT_HANDLE handle);
	void				InitDie(CUser* pKiller, BOOL bOwn, DWORD dwPower, DWORD dwStartKillFrame);
	void				ReleaseSoundHandle();

	void				ModelScaleByLevel();
	BOOL				IsNormalMonster();
	void				SetActionSkill(BYTE bSkillKind);
	

	// ����� ��ų ���� : �ִ��� 2005.3.3
	BYTE GetSkillLevel(const BYTE bySkillKind) const;
	void SetSkillLevel(const BYTE bySkillKind, const BYTE bySkillLevel);
	BYTE GetSelectedSkill() const;
	void SetSelectedSkill(BYTE bSkill);

	CMonster();
	~CMonster();
};

//#pragma pack(pop)