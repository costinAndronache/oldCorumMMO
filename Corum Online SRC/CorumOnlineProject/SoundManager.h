#ifndef _SOUNDMANAGER_H_
#define _SOUNDMANAGER_H_

class CSoundControlList : public CVoidList
{
public:
	void FreeData(void* pData)
	{
		if (pData)
		{
			((CSoundControl *)pData)->Release();
			pData = NULL;
		}
	}
};

class CSoundManager
{
public:
	CSoundManager();
	~CSoundManager();

	void	Init(DWORD dwMaxSoundControls);
	// �ʱ�ȭ

	void	DeleteAllSoundControl();
	// �޴����� �����Ҷ� 


	CSoundControl *			GetSoundControl();
	
	bool					ReleaseSoundControl(CSoundControl * pSoundControl);
	// ������ �������� �ʿ䰡 ������ ����ϴ� �Լ�
	
private:
	DWORD					m_dwMaxSoundControls;
		
	CSoundControlList		m_SoundControlList;
	// ���� ��Ʈ�� ����Ʈ
};

#endif
// _SOUNDMANAGER_H_
