// MSoundControl.h: interface for the MSoundControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSOUNDCONTROL_H__71FF33A4_39ED_4387_8154_78935C7E9A41__INCLUDED_)
#define AFX_MSOUNDCONTROL_H__71FF33A4_39ED_4387_8154_78935C7E9A41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SoundLib.h"

class MSoundManager;

class MSoundControl  
{
public:

	DWORD Run();
	// ���ϰ��� ���� �÷��� �ǰ� �ִ� ���� ����Ʈ ����.

	DWORD			Release();
	// ��Ʈ�� ����.



	// ���� �޴����� �ǵ帱 �̺�Ʈ�ڵ鷯. ���� �ǵ帮�� ������.
	BOOL			OnCreate( MSoundManager* pManager, DWORD dwMaxSoundHandleCount);
	// ���� �޴��Ŀ��� ÷ �����ǰ� ȣ��Ǵ� �ʱ�ȭ �Լ�.

	MSoundControl();
	~MSoundControl();

protected:
	void Delete();

	// �۷ι� �޴���.
	MSoundManager*			m_pManager;

	// ���� �ڵ��.
	DWORD					m_dwSoundHandleCount;						// ���� �ڵ� ī��Ʈ. ���� ũ��.
	SOUND_FILE_HANDLE*		m_pSoundFileHandle;							// �ڵ� ����.

	// ���� ����Ʈ.
	DWORD					m_dwMaxSoundEffectCount;					// �ִ� ���� ����Ʈ ����. ���� ũ��.
	DWORD					m_dwSoundEffectCount;						// ���� �����ִ� ���� ����Ʈ ����.
	ISoundEffect**			m_ppSoundEffect;							// ���� ����Ʈ ����.

};

#endif // !defined(AFX_MSOUNDCONTROL_H__71FF33A4_39ED_4387_8154_78935C7E9A41__INCLUDED_)
