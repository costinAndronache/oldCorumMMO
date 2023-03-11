// MSoundControl.cpp: implementation of the MSoundControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MSoundControl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MSoundControl::MSoundControl()
{

}

MSoundControl::~MSoundControl()
{

}

BOOL MSoundControl::OnCreate(MSoundManager *pManager, DWORD dwMaxSoundHandleCount)
{
	// �𺧷��� ��� ������
	if( pManager == 0 || dwMaxSoundHandleCount == 0 || dwMaxSoundHandleCount >= 0xbbbbbbbb)
		_asm int 3;



	// �޴���.
	m_pManager		=	pManager;

	// ���� �ڵ��.
	m_dwSoundHandleCount		=	dwMaxSoundHandleCount;									// ���� �ڵ� ī��Ʈ. ���� ũ��.
	m_pSoundFileHandle			=	new	SOUND_FILE_HANDLE[m_dwSoundHandleCount];			// �ڵ� ����.
	memset( m_pSoundFileHandle, 0, sizeof(SOUND_FILE_HANDLE)*m_dwSoundHandleCount);

	// ���� ����Ʈ.
	m_dwMaxSoundEffectCount		=	10;					// �ִ� ���� ����Ʈ ����. ���� ũ��. �ӽ÷� 10�̴�. ���߿� ��ũ�帮��Ʈ�� ���� ���� ����....
	m_dwSoundEffectCount		=	0;						// ���� �����ִ� ���� ����Ʈ ����.
	m_ppSoundEffect				=	new ISoundEffect*[m_dwMaxSoundEffectCount];							// ���� ����Ʈ ����.
	
	return TRUE;
}

DWORD MSoundControl::Release()
{
	return	0;
}

void MSoundControl::Delete()
{
	
}

DWORD MSoundControl::Run()
{
	return	0;
}
