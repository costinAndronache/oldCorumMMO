#pragma once
#include "StdAfx.h"
#ifdef SS3D_TEST
#include "./../SS3D_0719/4dyuchigx/4dyuchiGRX_common/IExecutive.h"
#else
#include "./../4dyuchiGRX_common/IExecutive.h"
#endif


#define MAX_OBJECT_NUM	1000	// ���� ������ ������Ʈ(�� ������Ʈ ����)
#define MAX_OBJECT_SIZE 1024	// GameObject struct�� ������ 
#define MAX_FILE_LENGTH 32		// ������ ȭ�� �̸��� ���� 
#define UNIQNUM_SPARE   4		// �ϳ� ������ ����� �����ϴ� ����. 
#define MAX_LIGHT_NUM   8
#define MAX_TEMP_BUFFER_NUMBER		10


struct  OBJECT_DESC
{
	BYTE	bType;
	BYTE	bActionFlag;	
	BYTE	bNextFlag;		//bACtionFlag�� ACTION_NEXT�϶� ��ȿ 
	BYTE	bCrashProc;		//�浹������ CrashProc�� ���� �ϴ³� �������� ���� 
	WORD	wNextAct;		//bACtionFlag�� ACTION_NEXT�϶� ��ȿ 
	int		nFrame;			//bACtionFlag�� ACTION_PAUSE�̰ų� ACTION_NEXT�϶� ��ȿ 
	int		nCurFrame;		// ���� ������.
	int		nNextFrame;		//bACtionFlag�� ACTION_NEXT�϶� ��ȿ 
	DWORD	dwTempBuf[ MAX_TEMP_BUFFER_NUMBER ];
	VECTOR3 vDest;			//������	
	float	fDestDistance;	//������ ������ �Ÿ� 
	DWORD	dwDestTime;		//������ ���� ���µ� �����ϴ� �ð� 
	DWORD	dwStartTick;	//���� ������ TickCount;
	DWORD	dwCurTick;		//���� �̵� �������� �ð� (Start Tick���κ��� ������ Tick)
	DWORD	dwArrivalTick;	//������ ���� ���� Tick
	BOOL	bSkip;

	void*	pInfo;			//Player Information
	void	(*ObjectFunc)(GXOBJECT_HANDLE, OBJECT_DESC*, DWORD, BYTE);							// ObjectFunction �Լ� ������ 
	void	(*CrashFunc)(GXOBJECT_HANDLE, OBJECT_DESC*, GXSCHEDULE_PROC_MSG_COLLISION_ARG*);	// �浹 �Լ� 
};


typedef OBJECT_DESC* LPObjectDesc;


struct OBJECT_HANDLE_DESC
{
	DWORD				dwObjID;
	GXOBJECT_HANDLE		pHandle;
	LPObjectDesc		pDesc;

	OBJECT_HANDLE_DESC* pNext;		
	OBJECT_HANDLE_DESC* pPrev;		
};

/*
enum GAME_OBJECT_TYPE
{
	OBJECT_TYPE_MAINPLAYER		= 1,
	OBJECT_TYPE_PLAYER			= 2,
	OBJECT_TYPE_TRADER_MONSTER	= 3,
	OBJECT_TYPE_EFFECT			= 4,
	OBJECT_TYPE_ITEM			= 5,
	OBJECT_TYPE_SPRITE			= 6,
	OBJECT_TYPE_GAME_LIGHT		= 7,
	OBJECT_TYPE_MONSTER			= 8,
	OBJECT_TYPE_WEAPON			= 9,
	OBJECT_TYPE_WEAPON_EFFECT	= 10,
	OBJECT_TYPE_PC_EFFECT		= 11,
	OBJECT_TYPE_GUARDIAN		= 12,
	OBJECT_TYPE_PLANE			= 13,
	OBJECT_TYPE_TILE			= 16,		
	OBJECT_TYPE_SELCHAR			= 15,
	OBJECT_TYPE_SKILL			= 17,
	OBJECT_TYPE_DUNGEON			= 18,
	OBJECT_TYPE_DAMAGE_USER		= 19,
	OBJECT_TYPE_DAMAGE_MONSTER	= 20
};
*/

typedef struct
{
	LPVOID	pHandle;							// 3d ��ü �ڵ�
	char	szChrName[MAX_FILE_LENGTH];			// chr ȭ�� �̸� 
	GXSchedulePROC pProc;	
	BYTE	bType;								// ������Ʈ Ÿ��
	
	WORD	wId;								// Uniq ID
	void*	pBucketID;
	void*	pBucketHandle;
} GameObject, *LPGameObject;


enum ACTION_DESC_TYPE
{
	ACTION_LOOP		= 0,
	ACTION_ONCE		= 1,
	ACTION_PAUSE	= 2,
	ACTION_NEXT		= 3,
	ACTION_ONCE_HIDE= 4		
};

/*
struct ACTION_DESC
{
	BYTE	bActionFlag;	
	BYTE	bNextFlag;		//bACtionFlag�� ACTION_NEXT�϶� ��ȿ 
	WORD	wNextAct;		//bACtionFlag�� ACTION_NEXT�϶� ��ȿ 
	int		nFrame;			//bACtionFlag�� ACTION_PAUSE�̰ų� ACTION_NEXT�϶� ��ȿ 
	int		nNextFrame;		//bACtionFlag�� ACTION_NEXT�϶� ��ȿ 
	int		nTempBuf;		
	VECTOR3 vDest;			//������
	void	(*ObjectFunc)(GXOBJECT_HANDLE);	//ObjectFunction �Լ� ������ 
};
*/

struct	SOUND_DESC
{
	DWORD		dwType;
	VECTOR3		v3ListenPos;
	VECTOR3		v3SoundPos;
};

struct	SOUND_HANDLE_DESC
{
	DWORD	hSound;
	SOUND_DESC		*pDesc;
};

