#pragma once
#include "StdAfx.h"
#ifdef SS3D_TEST
#include "./../SS3D_0719/4dyuchigx/4dyuchiGRX_common/IExecutive.h"
#else
#include "./../4dyuchiGRX_common/IExecutive.h"
#endif


#define MAX_OBJECT_NUM	1000	// 생성 가능한 오브젝트(맵 오브젝트 제외)
#define MAX_OBJECT_SIZE 1024	// GameObject struct의 사이즈 
#define MAX_FILE_LENGTH 32		// 데이터 화일 이름의 길이 
#define UNIQNUM_SPARE   4		// 하나 있지만 예비로 셋팅하는 숫자. 
#define MAX_LIGHT_NUM   8
#define MAX_TEMP_BUFFER_NUMBER		10


struct  OBJECT_DESC
{
	BYTE	bType;
	BYTE	bActionFlag;	
	BYTE	bNextFlag;		//bACtionFlag가 ACTION_NEXT일때 유효 
	BYTE	bCrashProc;		//충돌했을때 CrashProc을 실행 하는냐 마느냐의 여부 
	WORD	wNextAct;		//bACtionFlag가 ACTION_NEXT일때 유효 
	int		nFrame;			//bACtionFlag가 ACTION_PAUSE이거나 ACTION_NEXT일때 유효 
	int		nCurFrame;		// 현재 프레임.
	int		nNextFrame;		//bACtionFlag가 ACTION_NEXT일때 유효 
	DWORD	dwTempBuf[ MAX_TEMP_BUFFER_NUMBER ];
	VECTOR3 vDest;			//목적지	
	float	fDestDistance;	//목적지 까지의 거리 
	DWORD	dwDestTime;		//목적지 까지 가는데 도달하는 시간 
	DWORD	dwStartTick;	//시작 시점의 TickCount;
	DWORD	dwCurTick;		//현재 이동 진행중인 시간 (Start Tick으로부터 누적된 Tick)
	DWORD	dwArrivalTick;	//목적지 도달 예상 Tick
	BOOL	bSkip;

	void*	pInfo;			//Player Information
	void	(*ObjectFunc)(GXOBJECT_HANDLE, OBJECT_DESC*, DWORD, BYTE);							// ObjectFunction 함수 포인터 
	void	(*CrashFunc)(GXOBJECT_HANDLE, OBJECT_DESC*, GXSCHEDULE_PROC_MSG_COLLISION_ARG*);	// 충돌 함수 
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
	LPVOID	pHandle;							// 3d 객체 핸들
	char	szChrName[MAX_FILE_LENGTH];			// chr 화일 이름 
	GXProcedureHandler* pProc;
	BYTE	bType;								// 오브젝트 타입
	
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
	BYTE	bNextFlag;		//bACtionFlag가 ACTION_NEXT일때 유효 
	WORD	wNextAct;		//bACtionFlag가 ACTION_NEXT일때 유효 
	int		nFrame;			//bACtionFlag가 ACTION_PAUSE이거나 ACTION_NEXT일때 유효 
	int		nNextFrame;		//bACtionFlag가 ACTION_NEXT일때 유효 
	int		nTempBuf;		
	VECTOR3 vDest;			//목적지
	void	(*ObjectFunc)(GXOBJECT_HANDLE);	//ObjectFunction 함수 포인터 
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

