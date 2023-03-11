// MSoundManager.cpp: implementation of the MSoundManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MSoundManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MSoundManager::MSoundManager()
{

}

MSoundManager::~MSoundManager()
{

}

BOOL MSoundManager::Init(DWORD dwMaxSoundControlCount,  DWORD dwMaxSoundFileHandleCount, DWORD dwMaxSoundEffectCount, BOOL bDebugMode)
{
	BOOL	bResult;

	m_dwMaxSoundControlCount	=	dwMaxSoundControlCount;

	// �ڵ� ī��Ʈ.
	m_dwMaxSoundFileHandleCount		=	dwMaxSoundFileHandleCount;

	// ����Ʈ ī��Ʈ.
	m_dwMaxSoundEffectCount			=	dwMaxSoundEffectCount;

	// �����?
	m_bDebugMode	=	bDebugMode;
	
	// ���� ���̺귯�� �ʱ�ȭ.
	bResult	=	InitSoundLib();
	if( bResult == FALSE)
	{
		// redist ���丮�� �ȸ´���, ����ī�尡 ���ų� �ؼ� �����ߴ�.
		// �Ҹ� �ȳ����� ��.
		return	FALSE;
	}



	return	TRUE;
}

BOOL MSoundManager::InitSoundLib()
{
	BOOL	bResult;

	IMilesSoundLib*			pSoundLib		=	0;

	bResult	=	CreateSoundLib( &pSoundLib);
	if( bResult == FALSE)
		_asm int 3;

	SOUND_ERROR_CODE	result	=	pSoundLib->Init( m_dwMaxSoundFileHandleCount, m_dwMaxSoundEffectCount, "redist", FALSE, FALSE);

	if( result != SOUND_ERROR_NOERROR)
	{
		// �����ڵ尡 ���ϵǾ���.
		
		// ���� ���̺귯�� ������Ű�� ����.
		pSoundLib->Delete();
		return	FALSE;
	}

	m_pSoundLib		=	pSoundLib;
	

	return	TRUE;
}

BOOL MSoundManager::ReleaseSoundLib()
{
	m_pSoundLib->Delete();
	
	return	TRUE;
}

void MSoundManager::Delete()
{

	if( m_pSoundLib)
	{
		ReleaseSoundLib();
	}



}

BOOL MSoundManager::CreateSoundControl(MSoundControl** ppSoundControl, DWORD dwMaxSoundHandle)
{
	MSoundControl*		pControl	=	new	MSoundControl;

	BOOL	bResult;

	bResult	=	pControl->OnCreate( this, dwMaxSoundHandle);
	if( bResult == FALSE)
	{
		pControl->Release();
		return	FALSE;
	}

	*ppSoundControl	=	pControl;

	return	TRUE;


}
