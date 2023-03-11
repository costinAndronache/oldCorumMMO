// Struct Effect Info....
#pragma once


#define MAX_EFFECT_INFO			200
#define MAX_EFFECT_SOUND_NUM	200
#define MAX_PLAYER_SOUND_NUM	200	
//#define MAX_MONSTER_SOUND_NUM	6
#define MAX_GAMEPLAY_SOUND_NUM  200 

#define EFFECT_TYPE_DAMAGE			11
#define EFFECT_TYPE_STATUS			20		// �б���..
#define EFFECT_TYPE_NORMAL			21		// �ѹ� ������ ������, ����������Ʈ�� ������ ����ٴѴ�.
#define EFFECT_TYPE_STATUS_TOP		1		
#define EFFECT_TYPE_STATUS_CENTER	2		
#define EFFECT_TYPE_STATUS_BOTTOM	4		

//--- ���� ��� �� ���� ��ȣ ---------//
#define MONSTER_SOUND_NEUTRAL		0
#define MONSTER_SOUND_TARGET		1
#define MONSTER_SOUND_GETHIT		2
#define MONSTER_SOUND_ATTACK		4
#define MONSTER_SOUND_DEATH			6
#define MONSTER_SOUND_MOVE			8


//--- Game Play ������ ���Ʈ ���۹�ȣ -//
#define GAMEPLAY_HEABYSTONE_WALK	5
#define GAMEPLAY_HEABYSTONE_RUN		1
#define GAMEPLAY_AIRPLANE_LANDING	102
#define GAMEPLAY_AIRPLANE_TAKE_OFF	101

/*
// ����Ʈ�� ������ �������ִ� ����ü.
struct EffectInfo
{
	char	szEffectName[100];		// ����Ʈ �̸�.
	float	fRange;					// �����Ÿ�.

	BYTE	bType;					// ����Ʈ�� Ÿ��.

	BYTE	bStartSoundNum;			// ����Ʈ�� ���۵ɶ��� ����.
	BYTE	bFinishSoundNum;		// ����Ʈ�� ���������� ����.
};

// ����Ʈ ǥ���� ���� ����ü.
struct EffectDesc
{
	DWORD	dwEffectInfoNum;		// ǥ���ҷ��� �ϴ� ����Ʈ�� ���� ���̵�.	
	
	BYTE	bTargetObjectType;		// Ÿ�� ������Ʈ�� Ÿ��.
	DWORD	dwTargetIndex;			// Ÿ���� �ε���.
//////////////////////////////////////////////////////////////////////////////////
	//slowboat add
	float	f_Radcount;
	DWORD	dwFrameCount;
	DWORD	dwTemp[10];
///////////////////////////////////////////////////////////////////////////////

	OBJECT_HANDLE_DESC	hEffect;	// ����Ʈ�� ������Ʈ �ڵ�.
};
*/