// MSoundManager.h: interface for the MSoundManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSOUNDMANAGER_H__6CD50D69_6736_4921_A117_7F63088E919F__INCLUDED_)
#define AFX_MSOUNDMANAGER_H__6CD50D69_6736_4921_A117_7F63088E919F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SoundLib.h"
#include "MSoundControl.h"



class MSoundManager  
{
public:

	BOOL		Init(DWORD dwMaxSoundControlCount,  DWORD dwMaxSoundFileHandleCount, DWORD dwMaxSoundEffectCount, BOOL bDebugMode);
	// �ʱ�ȭ.
	// �ִ� ��Ʈ�� ����.
	// �ִ� �����ڵ� ����,
	// �ִ� ����Ʈ ����,
	// ����׸��.

	BOOL		CreateSoundControl( MSoundControl** ppSoundControl ,DWORD dwMaxSoundHandle);
	// ��Ʈ���� �����Ѵ�.

	
	
	void		Delete();

	
	
	MSoundManager();
	~MSoundManager();

protected:

	BOOL		ReleaseSoundLib();
	BOOL		InitSoundLib();
	
	// ��������.
	BOOL				m_bDebugMode;
	IMilesSoundLib*		m_pSoundLib;

	// ��Ʈ�ѵ�.
	DWORD				m_dwMaxSoundControlCount;


	// ���� �ڵ� ����.
	DWORD				m_dwMaxSoundFileHandleCount;

	// ����Ʈ ����.
	DWORD				m_dwMaxSoundEffectCount;

};

MSoundManager*			g_pSoundManager		=	0;

MSoundControl*			g_pInterfaceSound	=	0;
MSoundControl*			g_pPCSound			=	0;

#endif // !defined(AFX_MSOUNDMANAGER_H__6CD50D69_6736_4921_A117_7F63088E919F__INCLUDED_)
