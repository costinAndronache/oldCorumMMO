#ifndef __PATCHMAKER_CONFIG_H__
#define __PATCHMAKER_CONFIG_H__

#pragma once

class CPatchMakerConfig
{
public:
	~CPatchMakerConfig();
	
	static CPatchMakerConfig* GetInstance();
	static void DeleteInstance();

	BOOL		Initialize();

	// full path...
	LPCTSTR		GetCurrentFolderPath();
	LPCTSTR		GetTempFolderFullPath();
	LPCTSTR		GetHelperFolderFullPath();
	LPCTSTR		GetSourceFolderFullPath();
	LPCTSTR		GetTargetFolderFullPath();
	LPCTSTR		GetVDFFolderFullPath();

	// relative path..
	LPCTSTR		GetTempFolderPath();
	LPCTSTR		GetHelperFolderPath();
	LPCTSTR		GetSourceFolderPath();
	LPCTSTR		GetTargetFolderPath();
	LPCTSTR		GetVDFFolderPath();

	// file names.
	LPCTSTR		GetVesionFileName();
	LPCTSTR		GetAutoPatcherFileName();
	LPCTSTR		GetSelfUpdateFileName();
	LPCTSTR		GetLauncherFileName();
	LPCTSTR		GetGameBinFileName();
	LPCTSTR		GetAutoPatcherDllFileName();
	LPCTSTR		GetStorageDllFileName();
	
	LPCTSTR		GetGameName();
	LPCTSTR		GetGameStatus();
	LPCTSTR		GetLocale();
	UINT		GetPoolSize();
	BOOL		ManualPatchNeeded();
	BOOL		IsMigrated();

	void		SetMigrated();

protected:
	CPatchMakerConfig();

private:
	
	DWORD	__GetPrivateProfileString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPTSTR lpReturn);

	TCHAR	m_szGameName[_MAX_PATH];
	TCHAR	m_szGameState[_MAX_PATH];
	TCHAR	m_szLocale[_MAX_PATH];
	UINT	m_nPoolSize;
	BOOL	m_bCreatManualPatch;
	BOOL	m_bMigrated;

	TCHAR	m_szTempFolder[_MAX_PATH];
	TCHAR	m_szHelperFolder[_MAX_PATH];
	TCHAR	m_szSourceFolder[_MAX_PATH];
	TCHAR	m_szTargetFolder[_MAX_PATH];
	TCHAR	m_szVDFFolder[_MAX_PATH];

	TCHAR	m_szVesionFileName[_MAX_PATH];
	TCHAR	m_szAutoPatchFileName[_MAX_PATH];
	TCHAR	m_szSelfUpdateFileName[_MAX_PATH];
	TCHAR	m_szLauncherFileName[_MAX_PATH];
	TCHAR	m_szGameFileName[_MAX_PATH];
	TCHAR	m_szAutoPatchDllFileName[_MAX_PATH];
	TCHAR	m_szStorageDllFileName[_MAX_PATH];

	TCHAR	m_szCurrentPath[_MAX_PATH]; 
	TCHAR	m_szTempFullPath[_MAX_PATH];
	TCHAR	m_szHelperFullPath[_MAX_PATH];
	TCHAR	m_szSourceFullPath[_MAX_PATH];
	TCHAR	m_szTargetFullPath[_MAX_PATH];
	TCHAR	m_szVDFFullPath[_MAX_PATH];


	static CPatchMakerConfig* s_pPatchMakerConfigInstance;
};


#endif // __PATCHMAKER_CONFIG_H__