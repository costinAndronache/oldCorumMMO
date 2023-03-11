#ifndef _SOUNDCONTROL_H_
#define _SOUNDCONTROL_H_

#include "GlobalSoundHandles.h"
#include "SoundLib.h"
#include "VoidList.h"

class SoundEffectList : public CVoidList
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


class SoundFileList : public CVoidList
{
public:
	void FreeData(void* pData)
	{
		if (pData)
		{
			SAFE_REL_SOUND_FILE((SOUND_FILE_HANDLE)pData);
		}
	}
};

// -----------------------------------------------------
// Name			: CSoundControl
// Desc			: ���� ����Ʈ�� ���� ����� �����Ѵ�.
// Purpose		:
// Date/Code By : 040406 / Seven
// -----------------------------------------------------
class CSoundControl
{
public:
	CSoundControl() 
	{
	}

	~CSoundControl()
	{
		Release();
	}

// void Init(DWORD dwSoundEffectHandles, DWORD dwSoundFileHandles);
// ����� ����Ʈ, ���� ���� ������ �������� �ʴ´�.

	ISoundEffect * GetSoundEffect(const char * szSoundFileName, VECTOR3 * pPos, BOOL bLoop);
	// ���� ����Ʈ�� ��´�.
	// ���� �̸�(��� ����)�� ���尡 ���� �ɰ��� �����Ѵ�.
	// ���ο��� ���� �ڵ��� ����.

	void Release();
	// ��� ����Ʈ�� ���� ���带 �����Ѵ�.

private:
//	DWORD			m_dwSoundEffectHandles;
//	DWORD			m_dwMaxSoundFileHandles;

	SoundEffectList	m_SoundEffectList;
	SoundFileList	m_SoundFileList;
};

#endif
// _SOUNDCONTROL_H_
