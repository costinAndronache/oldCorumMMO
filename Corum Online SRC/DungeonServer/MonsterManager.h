#pragma once
#include "../CommonServer/CommonHeader.h"
#include "../CommonServer/AbyssHash64.h"
//#include "AbyssHash64.h"


#define BASE_ITEMBOTARY_INDEX	4200
#define	MAX_ITEMBOTARY			500
#define ITI_TYPE_BASEMONSTER	2000
#define MAX_MONSTER_USE_SKILL	10
#define MAX_GUARDIAN_USE_SKILL	5


enum MONSTER_RACE
{
	MONSTER_RACE_NPC = 0,
	MONSTER_RACE_GUARDIAN = 12,
	MONSTER_RACE_ELEMENTAL = 6,
	MONSTER_RACE_AMERITAT = 9,
};

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


#pragma pack( push, 1 )
struct MONSTER_SKILL
{
	BYTE	bySkillKind;
	BYTE	bSkillLevel;
};

struct MONSTER_AITYPE
{
	BYTE	bMoveType;
	BYTE	bSearchType;
	BYTE	bAttackType;
	BYTE	bDefenseType;
};

struct MONSTER_AI
{
	BYTE				bSetTargetType;
	BYTE				bFirstAttack;
	BYTE				bTargetChangeType;
	BYTE				bCongestionMode;
	DWORD				dwMonsterIDForCongestionMode;	//���ָ�带 �ߵ���ų ���� ������ ���̵�
	float				fRunningRate;					//�޸��� �ӵ�
	WORD				wCongestionTime;				//���ָ�� ���� �ð� 	
	float				fMinHPRateForCongestionMode;	//���ָ�尡 On�� HP Percentage (��)0.2�̸� ���� HP�� 20% �������� ���ָ�� �ߵ��Ѵ�.
};

//���� AI Ÿ�� ���� ���� DEFINE
#define AI_TARGET_TYPE_NEAR				0	//���� ����� ���� Ÿ������  
#define AI_TARGET_TYPE_LEAST_HP			1	//��ġ������ ���� ���� HP ������ Ÿ������ 
#define AI_TARGET_TYPE_NEAR_AND_FEW_HP	2	//�����鼭 ���� HP���� Ÿ������ (������ �����̶� ��Ȯ�� �� �𸣰���)
#define AI_TARGET_TYPE_SUMMON_OBJECT	3	//������̳� ��ȯ������ Ÿ������ 
#define AI_TARGET_TYPE_SAME_KIND		4	//Ÿ���� ������ �þ߿� ���� id�� ���Ͱ� Ÿ������ ���� ���� Ÿ������ ��´�.

//���� �������� DEFINE
#define AI_FIRST_ATTACK_NONE			0	//�������� �ʴ´�. ���ݹ����� �����Ѵ�. 
#define AI_FIRST_ATTACK_ENABLE			1	//�����Ѵ� 


//���� Ÿ�� ���� ���� 
#define AI_CHANGE_TARGET_TYPE_NONE				0	//�ѳ� �׾�� �����Ѵ�. 
#define AI_CHANGE_TARGET_TYPE_PERCENTAGE_DEMAGE	1	//���� HP�� xx% �̻��� �������� ���������� Ÿ���� �ٲ۴�.
#define AI_CHANGE_TARGET_TYPE_ABSOLUTELY_CHANGE 2	//���ݼ������ο� ������� ������ �ֱ� ���ݹ��������� Ÿ���� �ٲ۴�. 
#define AI_CHANGE_TARGET_TYPE_SAME_KIND			3	//�� Ÿ���� �ִ��� �þ߿� ���� id�� ���Ͱ� Ÿ���� �����ϸ� ����� ���� �ٽ� Ÿ������ �����Ѵ�. 


struct MONSTER_ITEM_RATE
{
	WORD	ItemID;
	WORD	ItemRate;
};

struct MIN_MAX
{
	WORD	wMin;
	WORD	wMax;
};

// ���Ͱ� �ټ� �ִ� �������� �ָӴϸ� �� �÷��ֱ� ���� ���̴�.(���ָӴ�)
struct BASEITEM_BOTARY
{
	DWORD				dwID;
	MONSTER_ITEM_RATE	ItemRate[20];	
};

typedef struct BASEMONSTER
{
	DWORD				dwID;
	char				szMonsterName_Kor[50];
	char				szMonsterName_Eng[25];
	
	WORD				wBaseLevel;
	BYTE				bRace;					// 
					
	BYTE				bClass;					// ����
	BYTE				bSize;
	BYTE				bScale;					// ������
	DWORD				dwBaseHP;
	WORD				wBaseMP;
	DWORD				dwBaseAA;
	WORD				wBaseDP;
	DWORD				dwBaseAD_Min;
	DWORD				dwBaseAD_Max;
	WORD				wBaseAW;
	WORD				wBaseMS;
	BYTE				bBaseBR;
	BYTE				bLHP;
	BYTE				bLMP;
	BYTE				bLAA;
	BYTE				bLDP;
	MIN_MAX				wLAD;
	BYTE				bLMS;
	BYTE				bLBR;
	BYTE				bLRF;
	BYTE				bLRI;
	BYTE				bLRL;
	BYTE				bLRPo;
	BYTE				bLRPh;
	DWORD				m_dwEgo;				
	DWORD				m_dwStr;				
	DWORD				m_dwInt;				
	DWORD				m_dwDex;				
	DWORD				m_dwVit;				

	DWORD				dwExp;

	BYTE				bAD_type;
	WORD				wA_Range;
	WORD				wAW;
	WORD				wMS;
	short				wFire_resist;
	short				wIce_resist;
	short				wPhy_resist;
	short				wLight_resit;
	short				wPoi_resist;
	short				wAll_resist;

	WORD				wNonSkillRate;
	WORD				wSkillRate;			// ��ų ���� ������ �˻��ϴ� �ֱ�
	MONSTER_SKILL		sSkill[MAX_MONSTER_USE_SKILL];
	
	BYTE				bTQS;
	
	WORD				wResponseSpeed;	// ���� ��ȭ ���� �ӵ�
	BYTE				bAI;
	//------------   For Monster AI   ---------------------------
	MONSTER_AI			AI;
	//------------------------------------------------------------
		
	MONSTER_AITYPE		sAiType;
	BYTE				bSR;			// Search_Range
	
	DWORD				dwMin_Money;
	DWORD				dwMax_Money;
	MIN_MAX				wItemCount;		// ��� �������� �������� �����Ѵ�.
	
	
	MONSTER_ITEM_RATE	ItemRate[10];
	WORD				wMoneyRate;	
	WORD				wNTR;			// Dont Put the Item Down

	WORD				wOption_0;
	WORD				wOptionRate[2];
	MIN_MAX				wOptionLevel[2];

	BYTE				wDMV;
	WORD				wMODNo;
	WORD				wANMNo;
	WORD				wSC_Shots;
	WORD				wSneutral;
	WORD				wStargeting;
	WORD				wSdamage1;
	WORD				wSdamage2;
	WORD				wSattack1;
	WORD				wSattack2;
	WORD				wSdeath1;
	WORD				wSdeath2;
	WORD				wSmove1;
	WORD				wSmove2;

} *LPBASEMONSTER;
#pragma pack( pop )


typedef CAbyssHash<BASEMONSTER>			BASEMONSTER_HASH,		* LPBASEMONSTER_HASH;

class CMonsterManager
{
private:
	DWORD					m_dwMonsterNum;

public:
	LPBASEMONSTER_HASH		m_pBaseMonsterHash;
	
	DWORD	GetMonsterNum()		{ return m_dwMonsterNum; }
	
//	BOOL	Parse_BaseMonster( char* szFileName );
	BOOL	Save_BaseMonster( char* szFileName );
	BOOL	Load_BaseMonster( char* szFileName );

	LPBASEMONSTER	GetMonster( DWORD dwItemID );
	

public:
	void	Destroy();
	CMonsterManager();
	~CMonsterManager();
};


extern LPBASEMONSTER_HASH			g_pBaseMonsterHash;

