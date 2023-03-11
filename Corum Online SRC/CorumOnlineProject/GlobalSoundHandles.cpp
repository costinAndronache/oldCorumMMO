//=======================================================================
// CSoundEffectBasketCase
#include "GlobalSoundHandles.h"
#include "GameMenuWnd.h"
#include <crtdbg.h>
#include "Define.h"
#include "User.h"
#include "InitGame.h"
#include "GameControl.h"

GLOBAL_SOUND_LIB_HANDLES	g_SoundLibHandles;	
CSoundEffectBasketCase		g_SoundEffectBasketCase;

IMilesSoundLib*				g_pSoundLib				= NULL;
char*						g_pszSoundErrorString	= NULL;

VECTOR3						g_v3InterfaceSoundPos	= {0, };

#ifndef __GLOBALSOUNDHANDLES_H__
BOOL						g_bInitSoundEnalbe = TRUE;	// �α��ν� ���ϴ� �Ҹ� ����
#endif

//===================================
// Purpose : �ʱ�ȭ 
//===================================
CSoundEffectBasketCase::CSoundEffectBasketCase()
{
	memset(this, 0, sizeof(*this));
	Init();
}
//===================================
// Purpose : ������
//===================================
CSoundEffectBasketCase::~CSoundEffectBasketCase()
{
	Release();
}
//===================================
// Purpose : ���� �ʱ�ȭ
//===================================
void CSoundEffectBasketCase::Init()
{
	if ( !m_pSoundEffectList )
	{
		m_pSoundEffectList = new CSoundEffectList;
	}
	ForceAllClean();
}
//===================================
// Purpose : ���ҽ� ����
//===================================
void CSoundEffectBasketCase::Release()
{
	ForceAllClean();
	delete m_pSoundEffectList;
	m_pSoundEffectList = NULL;
}
//===================================
// Purpose : ���������� �Ҵ�� ���ҽ� ����
//===================================
void CSoundEffectBasketCase::ForceAllClean()
{
	if (g_pSoundLib == NULL)
		return;

	POSITION_	pos;
	POSITION_	PrevPos;

	pos = m_pSoundEffectList->GetHeadPosition();
	while(pos)
	{
		PrevPos = pos;
		ISoundEffect * pSoundEffect = (ISoundEffect *)m_pSoundEffectList->GetNext(pos);
		if (pSoundEffect)
		{
			pSoundEffect->Stop();
			m_pSoundEffectList->RemoveAt(PrevPos);
			pSoundEffect = NULL;
		}
	}
}
//===================================
// Purpose : �׾��ִ� ���ҽ��� ���� 
//===================================
void CSoundEffectBasketCase::UpdateToClean()
{
	if (g_pSoundLib == NULL)
		return;

	POSITION_	pos;
	POSITION_	PrevPos;

	pos = m_pSoundEffectList->GetHeadPosition();

	while(pos)
	{
		PrevPos = pos;

		ISoundEffect * pSoundEffect = (ISoundEffect *)m_pSoundEffectList->GetNext(pos);
		if (pSoundEffect && !pSoundEffect->IsAlive())
		{				
			m_pSoundEffectList->RemoveAt(PrevPos);
		}
	}
}
//===================================
// Purpose : ���ҽ� �߰� 
//===================================
void CSoundEffectBasketCase::Add(ISoundEffect * pSoundEffect )
{
	if (pSoundEffect == NULL)
		return;

	int iCount = m_pSoundEffectList->GetCount();
	if (iCount % 3 == 0)
	{
		UpdateToClean();
	}

	m_pSoundEffectList->AddTail((void*) pSoundEffect);
}
//===================================
// Purpose : ���� ����� ���´�. 
// Output : phOutFileHandle �� �޾ƿ� 
//===================================
void GetSoundEffectFile(SOUND_FILE_HANDLE * phOutFileHandle, char * pszFileName)
{
	if ( !g_pSoundLib ) 
		return;

	if (*phOutFileHandle)
	{
		return;
	}

	SOUND_ERROR_CODE	Result;
	Result = g_pSoundLib->CreateSoundEffectFile(phOutFileHandle, pszFileName); 
	if (Result != SOUND_ERROR_NOERROR) 
	{ 
#ifdef DEVELOP_MODE
		char szBuf[255];
		g_pSoundLib->GetErrorString( &g_pszSoundErrorString, Result); 
		wsprintf(szBuf, "Sound : %s(%s)\n", pszFileName, g_pszSoundErrorString );
		OutputDebugString(g_pszSoundErrorString); 
#endif
		return; 
	} 
}
//===================================
// Purpose : ���� ����Ʈ ����� ��´�.
//===================================
void GetSoundEffect(ISoundEffect ** ppOutEffect, SOUND_FILE_HANDLE & rhInFileHandle, VECTOR3 & v3Pos)
{
	if ( !g_pSoundLib ) 
		return;

	float fDis = 0;

	if (g_pMainPlayer)
	{

		fDis = CalcDistance(&g_pMainPlayer->m_v3CurPos, &v3Pos);

		if ( fDis >= SOUND_LISTEN_SCOPE_DISTANCE && !(g_v3InterfaceSoundPos == v3Pos) )
		{
			*ppOutEffect = NULL;
			return;
		}
	}

	SOUND_ERROR_CODE	Result;

	if (!rhInFileHandle)
		return;

	Result = g_pSoundLib->CreateSoundEffect(ppOutEffect, rhInFileHandle);
	if (Result != SOUND_ERROR_NOERROR)
	{
		g_pSoundLib->GetErrorString( &g_pszSoundErrorString, Result );
		OutputDebugString( g_pszSoundErrorString );
	}
}
//===================================
// Purpose : ����Ʈ ���� �ڵ��� �̿��Ͽ� �Ҹ��� ���
//===================================
void PlaySoundEffect(ISoundEffect * pSoundEffect, VECTOR3 * pV3Pos, bool bLoop)
{	
	if ( !g_pSoundLib ) 
		return;

	// sung-han 2005-03-14 �α��ν� ���ϴ� �Ҹ� ����, ���� on, off ���
	if(!g_bInitSoundEnalbe)
		return;

	static VECTOR3	v3Pos = {0.f,};
	CGameMenuWnd * pGameMenuWnd = CGameMenuWnd::GetInstance();

	if (pSoundEffect && pGameMenuWnd->m_bEffectFlag)
	{
		pSoundEffect->SetMaxMinDistance(DEFAULT_SOUND_MAX_DISTANCE, DEFAULT_SOUND_MIN_DISTANCE);

		// sung-han 2005-03-14 �Ÿ��� ���� ���� ���� ���� ----------------------------
		float fDis = 0;
		float fEffectVolume = g_fEffectVolume;
		if(g_pMainPlayer && !(*pV3Pos == v3Pos) )
		{
			fDis = CalcDistance(&g_pMainPlayer->m_v3CurPos, pV3Pos);
			fEffectVolume = g_fEffectVolume - (fDis / SOUND_LISTEN_SCOPE_DISTANCE)*g_fEffectVolume;
		}
		if(fEffectVolume < 0 || fEffectVolume > 1)
		{
			fEffectVolume = g_fEffectVolume;
		}
		//------------------------------------------------------------------------------

		
		pSoundEffect->SetVolume(fEffectVolume);
		if ( *pV3Pos == v3Pos )
			pSoundEffect->SetRelativePosition(&v3Pos);
		else
			pSoundEffect->SetPosition(pV3Pos);
		
		pSoundEffect->Play(bLoop);
	}
}
//===================================
// Purpose : 
//===================================
void PlaySoundEffectMaxMin(ISoundEffect * pSoundEffect, VECTOR3 * pV3Pos, bool bLoop, int iMaxDistance, int iMinDistance)
{	
	if ( !g_pSoundLib ) 
		return;

	CGameMenuWnd * pGameMenuWnd = CGameMenuWnd::GetInstance();

	if (pSoundEffect && pGameMenuWnd->m_bEffectFlag)
	{
		pSoundEffect->SetMaxMinDistance((float)iMaxDistance, (float)iMinDistance);
		pSoundEffect->SetPosition(pV3Pos);
		pSoundEffect->Play(bLoop);
	}
}
//===================================
// Purpose : 
//===================================
void PlayBGM(const char * szBGMName)
{
	if (!g_pSoundLib)
		return;

	if (__strcmp((char*)szBGMName, "\0") == 0)
	{
		return;
	}

	CGameMenuWnd* pGameMenuWnd = CGameMenuWnd::GetInstance();

	if (NULL == pGameMenuWnd)
	{
		return;
	}
	
	lstrcpy(pGameMenuWnd->m_szLast_BGM_Name, szBGMName);	// �ֱ� ���� ���� ������ ������ �д�.
	
	if (pGameMenuWnd ->m_bBGMFlag == FALSE)
		return;

	StopBGM();	// �ϴ� ����ǰ� �ִ� ������ ����.

	g_pSoundLib->PlayBGM(szBGMName);

	if (g_fBGMVolume > 1.0f)
		g_fBGMVolume = 1.0f;
	else if( g_fBGMVolume < 0.0f)
		g_fBGMVolume = 0.0f;
		
	SetBGMVolume(g_fBGMVolume);		
}
//===================================
// Purpose : 
//===================================
void SetBGMVolume(float fBGMvolume)
{
	if (g_pSoundLib && fBGMvolume > 0)
		g_pSoundLib->SetBGMVolume(fBGMvolume);
}
//===================================
// Purpose : 
//===================================
void StopBGM()
{
	if (!g_pSoundLib)
		return;

	g_pSoundLib->StopBGM();
}
//===================================
// Purpose : ���� ������ ���ҽ��� ����� �� �����Ǿ��� �˻��Ѵ�.
//===================================
void Check_Sound_Resource()
{
	int nSoundEffectCount;
	int	nSoundFileCount;

	nSoundEffectCount = g_pSoundLib->GetAllocatedSoundEffectCount();
	nSoundFileCount	= g_pSoundLib->GetAllocatedSoundFileCount();

	if (0 != nSoundEffectCount)
		__asm nop;
	if (0 != nSoundFileCount)
	{
		g_pSoundLib->Run();

	}


}
//===================================
// Purpose : ���� ���� 
//===================================
void Release_Sound_Resource()
{
	if (NULL == g_pSoundLib)
		return;

	g_SoundLibHandles.Release();
	g_SoundLibHandles.Init();
	
	Check_Sound_Resource();
}