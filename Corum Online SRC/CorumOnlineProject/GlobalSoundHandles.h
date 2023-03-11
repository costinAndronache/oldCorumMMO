// ===============
// GlobalSoundHandles.h
// ���� ��� ���� ���� / �Լ� / ��ũ�� ���� 
// by seven			03/08/22
// ===============


#pragma once

#include "EffectInfo.h"
#include "GlobalSoundHandles.h"
#include "SoundLib.h"
#include "VoidList.h"

// ���� ���� ������ 
#define DEFAULT_SOUND_MAX_DISTANCE			1500
#define DEFAULT_SOUND_MIN_DISTANCE			100
#define SOUND_LISTEN_SCOPE_DISTANCE			1800		// ���� : 

#define DEFAULT_SOUND_MAX_ASYNC_COUNT		8			// �� ������ ���ÿ� �߻��Ҽ� �ִ� ����

// ���� ����Ʈ ����� ������ 
#define SAFE_REL_SOUND(handle)				{ if (handle){ (handle)->Stop(); (handle)->Release(); (handle)=NULL; } }
//
// ���� ����Ʈ ���� ��� ����Ʈ�� ������
#define SAFE_REL_SOUND_FILE(Handle)			{ if(Handle) { g_pSoundLib->ReleaseSoundEffectFile(Handle); (Handle)=NULL; } }									

// 
struct global_sound_handle_t;
struct global_sound_lib_handle_t;
class CGameMenuWnd;
class CSoundEffectBasketCase;
//
typedef global_sound_handle_t		GLOBAL_SOUND_HANDLES;
typedef global_sound_lib_handle_t	GLOBAL_SOUND_LIB_HANDLES;
//
extern CSoundEffectBasketCase		g_SoundEffectBasketCase;
extern GLOBAL_SOUND_HANDLES			g_SoundHandles;			// 2D sound handle
extern GLOBAL_SOUND_LIB_HANDLES		g_SoundLibHandles;		// 3D sound handle
extern char *						g_pszSoundErrorString;
extern IMilesSoundLib*				g_pSoundLib;			// MilesSoundLIb Handle Manager

extern BOOL							g_bInitSoundEnalbe;	// �α��ν� ���ϴ� �Ҹ� ����


// ������ ���� ���� �Լ� 
void GetSoundEffectFile(SOUND_FILE_HANDLE * phOutFileHandle, char * pszFileName);
void GetSoundEffect(ISoundEffect ** ppOutEffect, SOUND_FILE_HANDLE & rhInFileHandle, VECTOR3 & v3Pos);

void PlaySoundEffect(ISoundEffect * pSoundEffect, VECTOR3 * pV3Pos, bool bLoop);
void PlaySoundEffectMaxMin(ISoundEffect * pSoundEffect, VECTOR3 * pV3Pos, bool bLoop, int iMaxDistance, int iMinDistance);
void PlayBGM(const char * szBGMName);
void StopBGM();
void SetBGMVolume(float fBGMvolume);

// purpose :���� ���� 
void Release_Sound_Resource();

// ���� ������ ���ҽ��� ����� �� �����Ǿ��� �˻��Ѵ�.
void Check_Sound_Resource();

struct global_sound_lib_handle_t
{
	SOUND_FILE_HANDLE	hSoundEffectFile[MAX_EFFECT_SOUND_NUM];			
	SOUND_FILE_HANDLE 	hSoundGamePlayFile[MAX_GAMEPLAY_SOUND_NUM];		
//	SOUND_FILE_HANDLE 	hSoundMonsterFile[400];							
	SOUND_FILE_HANDLE 	hSoundPlayerFile[200];							
	SOUND_FILE_HANDLE 	hSoundWeaponFile[200];							
	SOUND_FILE_HANDLE 	hSoundWeaponDamageFile[200];					
	SOUND_FILE_HANDLE	hSoundSystemFile[200];							
	SOUND_FILE_HANDLE	hSoundItemFile[200];							
	
	global_sound_lib_handle_t()
	{
		Init();
	}

	~global_sound_lib_handle_t()
	{
// �޴������� �������� ������.
//		Release();
	}

	void Init()
	{
		memset(this, 0, sizeof(global_sound_lib_handle_t));
	}

	void Release()
	{
		// ����Ʈ ��� ���� ���� �Ѵ����� 
		// ��������� �����ؾ���!!
		int i;

		if (NULL == g_pSoundLib)
			return;

		// Effect File Handle Release
		for (i = 0; i < MAX_EFFECT_SOUND_NUM; ++i)
			SAFE_REL_SOUND_FILE(hSoundEffectFile[i]);
		for (i = 0; i < MAX_GAMEPLAY_SOUND_NUM; ++i)
			SAFE_REL_SOUND_FILE(hSoundGamePlayFile[i]);
//		for (i = 0; i < 400; ++i)
//			SAFE_REL_SOUND_FILE(hSoundMonsterFile[i]);
		for (i = 0; i < 200; ++i)
			SAFE_REL_SOUND_FILE(hSoundPlayerFile[i]);
		for (i = 0; i < 200; ++i)
			SAFE_REL_SOUND_FILE(hSoundWeaponFile[i]);
		for (i = 0; i < 200; ++i)
			SAFE_REL_SOUND_FILE(hSoundWeaponDamageFile[i]);
		for (i = 0; i < 200; ++i)
			SAFE_REL_SOUND_FILE(hSoundGamePlayFile[i]);
		for (i = 0; i < 200; ++i)
			SAFE_REL_SOUND_FILE(hSoundItemFile[i]);		
		for (i = 0; i < 200; ++i)
			SAFE_REL_SOUND_FILE(hSoundSystemFile[i]);
	}
};


typedef struct SOUNDEFFECT
{
	ISoundEffect * pSoundEffect;

	SOUNDEFFECT() { pSoundEffect = NULL; }
	~SOUNDEFFECT() { SAFE_REL_SOUND(pSoundEffect); }

	void Release()
	{
		SAFE_REL_SOUND(pSoundEffect);
	}
} * LPSOUNDEFFECT;


// ------------------------------------------------------
// Desc			: ���� ����Ʈ ��ü���� ������ �ִٰ� �ʿ���� ��ü���� ������.
// Purpose		:
// return		:
// Parameter	:
// Date/Code By : seven
// ------------------------------------------------------
class CSoundEffectList : public CVoidList
{
public:
	void FreeData(void* pData)
	{
		if (pData)
		{
			((ISoundEffect *)pData)->Release();
			pData = NULL;
		}
	}
};

// ���� ������ / ����� ����Ʈ ��� ����
class CSoundEffectBasketCase
{
private:
	enum { SQUEEZE_SOUND_EFFECT_NUM = 5 , /* SQUEEZE_SOUND_FILE_NUM = 50 */};
	CSoundEffectList *				m_pSoundEffectList;

public:
	CSoundEffectBasketCase();
	~CSoundEffectBasketCase();

	void Init();
	void Release();

	int GetSoundEffectNum() const
	{ return m_pSoundEffectList->GetCount(); }

	void UpdateToClean();
	void ForceAllClean();

	void Add( ISoundEffect * pSoundEffect );
};

extern VECTOR3	g_v3InterfaceSoundPos;


				

