#include "soundcontrol.h"

void CSoundControl::Release()
{
	// ����Ʈ ����� ������.
	POSITION_	pos;
	POSITION_	PrevPos;

	pos = m_SoundEffectList.GetHeadPosition();
	while(pos)
	{
		PrevPos = pos;
		ISoundEffect * pSoundEffect = (ISoundEffect *)m_SoundEffectList.GetNext(pos);
		if (pSoundEffect)
		{
			pSoundEffect->Stop();
			m_SoundEffectList.RemoveAt(PrevPos);
			pSoundEffect = NULL;
		}
	}

	pos = m_SoundFileList.GetHeadPosition();
	while(pos)
	{
		PrevPos = pos;
		SOUND_FILE_HANDLE pSoundFile = m_SoundFileList.GetNext(pos);
		if (pSoundFile )
		{			
			m_SoundFileList.RemoveAt(PrevPos);
			pSoundFile = NULL;
		}
	}
}
// -----------------------------------------------------
// Name			: CSoundControl::GetSoundEffect(const char * szSoundFileName, bool bLoop)
// Desc			: ���� ����� ������ ����Ʈ ����� �ѱ��.
// Purpose		: ���� ����Ʈ �ڵ��� ��´�.
// return		:
// Date/Code By : 040406 / seven
// -----------------------------------------------------
ISoundEffect * CSoundControl::GetSoundEffect(const char * szSoundFileName, VECTOR3 * pPos, BOOL bLoop)
{
	// ���� ���� ����� ��´�.
	SOUND_FILE_HANDLE pSoundFileHandle = new SOUND_FILE_HANDLE;
	if (NULL == pSoundFileHandle)
	{	
		return NULL;
	}

	::GetSoundEffectFile(&pSoundFileHandle, (char *)(const char*)szSoundFileName);
	if (0xcdcdcdcd == (unsigned int)pSoundFileHandle ||
		NULL == pSoundFileHandle)
	{
		return NULL;
	}
	m_SoundFileList.AddHead( pSoundFileHandle );

	// ���� ����Ʈ ����� ��´�.

	if (m_SoundEffectList.GetCount() >= DEFAULT_SOUND_MAX_ASYNC_COUNT-1)
	{ // ���� ���Ҽ� �ִ� ����Ʈ ����� ������ ���� ���� �ڵ��� 8���� ���ѵǾ��ִ°����
	  // 7�� �̻��� ���� �˾Ƽ� �����ش�. ���� ĥ�� define���� ����.
	  // ������ ����Ʈ ����� ���д�.

		POSITION_	pos;
		POSITION_	PrevPos;

		pos = m_SoundEffectList.GetHeadPosition();
		while(pos)
		{
			PrevPos = pos;
			ISoundEffect * pSoundEffect = (ISoundEffect *)m_SoundEffectList.GetNext(pos);
			if (pSoundEffect)
			{
				if ( !pSoundEffect->IsLoop() && !pSoundEffect->IsAlive())
				{ // ������ �ƴϰ� �߻����� �����ѻ��´�. (�ڿ������� ����� ���� ����)
					m_SoundEffectList.RemoveAt(PrevPos);
					pSoundEffect = NULL;
				}
			}
		}		
	}

	ISoundEffect * pSoundEffect;
	::GetSoundEffect(&pSoundEffect, pSoundFileHandle, *pPos);
	if ( 0xcccccccc == (unsigned int)pSoundEffect || 
		 NULL == pSoundEffect)
	{
		return NULL;
	}
	m_SoundEffectList.AddHead( pSoundEffect );
	
	return pSoundEffect;
}

