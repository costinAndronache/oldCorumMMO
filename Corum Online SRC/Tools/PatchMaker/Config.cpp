#include "stdafx.h"
#include "Config.h"


static DWORD __GetPrivateProfileString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPTSTR lpReturn)
{
	return GetPrivateProfileString(lpAppName, lpKeyName, _T(""), lpReturn, _MAX_PATH, _T("./PatchMaker.Ini"));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CPatchMakerConfig* CPatchMakerConfig::s_pPatchMakerConfigInstance = NULL;

CPatchMakerConfig* CPatchMakerConfig::GetInstance()
{
	if(s_pPatchMakerConfigInstance == NULL)
	{
		s_pPatchMakerConfigInstance = new CPatchMakerConfig;
	}

	return s_pPatchMakerConfigInstance;
}

void CPatchMakerConfig::DeleteInstance()
{
	if(s_pPatchMakerConfigInstance)
	{
		delete s_pPatchMakerConfigInstance;
		s_pPatchMakerConfigInstance = NULL;
	}
}

CPatchMakerConfig::CPatchMakerConfig()
{
	memset(this, 0, sizeof(CPatchMakerConfig));
}

CPatchMakerConfig::~CPatchMakerConfig()
{
	
}

BOOL CPatchMakerConfig::Initialize()
{
	__GetPrivateProfileString(_T("BASE_CONFIG"),	_T("GAME_NAME"),		m_szGameName);	
	__GetPrivateProfileString(_T("BASE_CONFIG"),	_T("SERVICE_STATUS"),	m_szGameState);
	__GetPrivateProfileString(_T("BASE_CONFIG"),	_T("LOCALE_NAME"),		m_szLocale);

	m_nPoolSize = GetPrivateProfileInt(_T("BASE_CONFIG"), _T("VDF_POOL_SIZE"), 10240, _T("./PatchMaker.Ini"));
	if(m_nPoolSize == 0) m_nPoolSize = 10240;

	m_bCreatManualPatch = GetPrivateProfileInt(_T("BASE_CONFIG"), _T("CREATE_MANUAL_PATCH"), 1, _T("./PatchMaker.Ini"));

	m_bMigrated = GetPrivateProfileInt(_T("BASE_CONFIG"), _T("MIGRATED"), 0, _T("./PatchMaker.Ini"));

	__GetPrivateProfileString(_T("FOLDER_SETTING"), _T("TEMP_FOLDER"),		m_szTempFolder);
	__GetPrivateProfileString(_T("FOLDER_SETTING"), _T("HELPER_FOLDER"),	m_szHelperFolder);
	__GetPrivateProfileString(_T("FOLDER_SETTING"), _T("SOURCE_FOLDER"),	m_szSourceFolder);
	__GetPrivateProfileString(_T("FOLDER_SETTING"), _T("TARGET_FOLDER"),	m_szTargetFolder);
	__GetPrivateProfileString(_T("FOLDER_SETTING"), _T("VDF_FOLDER"),		m_szVDFFolder);
		
	__GetPrivateProfileString(_T("BINARY_SETTING"), _T("VERSIONFILE"),		m_szVesionFileName);
	__GetPrivateProfileString(_T("BINARY_SETTING"), _T("AUTOPATCHER"),		m_szAutoPatchFileName);
	__GetPrivateProfileString(_T("BINARY_SETTING"), _T("SELFUPDATE"),		m_szSelfUpdateFileName);
	__GetPrivateProfileString(_T("BINARY_SETTING"), _T("LAUNCHER"),			m_szLauncherFileName);
	__GetPrivateProfileString(_T("BINARY_SETTING"), _T("GAME"),				m_szGameFileName);
	__GetPrivateProfileString(_T("BINARY_SETTING"), _T("AUTOPATCHER_DLL"),	m_szAutoPatchDllFileName);
	__GetPrivateProfileString(_T("BINARY_SETTING"), _T("STORAGE_DLL"),		m_szStorageDllFileName);
	

	// 모든 TXT 관련 값이 정상으로 들어와야 한다.
	if(	lstrlen(m_szTempFolder) == 0		|| 	lstrlen(m_szHelperFolder) == 0			||
		lstrlen(m_szSourceFolder) == 0		|| 	lstrlen(m_szTargetFolder) == 0			||
		lstrlen(m_szVDFFolder) == 0			|| 	lstrlen(m_szVesionFileName) == 0		||
		lstrlen(m_szAutoPatchFileName) == 0 || 	lstrlen(m_szSelfUpdateFileName) == 0	||
		lstrlen(m_szLauncherFileName) == 0	||	lstrlen(m_szGameFileName) == 0			||
		lstrlen(m_szGameName) == 0			||	lstrlen(m_szGameState) == 0 			||
		lstrlen(m_szAutoPatchDllFileName) == 0 )

		return FALSE;

	GetCurrentDirectory(_MAX_PATH, m_szCurrentPath); 
	wsprintf(m_szTempFullPath,	 _T("%s%s"), GetCurrentFolderPath(), m_szTempFolder+1);
	wsprintf(m_szHelperFullPath, _T("%s%s"), GetCurrentFolderPath(), m_szHelperFolder+1);
	wsprintf(m_szSourceFullPath, _T("%s%s"), GetCurrentFolderPath(), m_szSourceFolder+1);
	wsprintf(m_szTargetFullPath, _T("%s%s"), GetCurrentFolderPath(), m_szTargetFolder+1);
	wsprintf(m_szVDFFullPath,	 _T("%s%s"), GetCurrentFolderPath(), m_szVDFFolder+1);

	return TRUE;
}

DWORD CPatchMakerConfig::__GetPrivateProfileString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPTSTR lpReturn)
{
	return GetPrivateProfileString(lpAppName, lpKeyName, _T(""), lpReturn, _MAX_PATH, _T("./PatchMaker.Ini"));
}

void CPatchMakerConfig::SetMigrated()					
{
	WritePrivateProfileString(_T("BASE_CONFIG"), _T("MIGRATED"), _T("1"), _T("./PatchMaker.Ini"));
}

LPCTSTR CPatchMakerConfig::GetCurrentFolderPath()		{ return m_szCurrentPath; }
LPCTSTR CPatchMakerConfig::GetTempFolderFullPath()		{ return m_szTempFullPath; }
LPCTSTR CPatchMakerConfig::GetHelperFolderFullPath()	{ return m_szHelperFullPath; }
LPCTSTR CPatchMakerConfig::GetSourceFolderFullPath()	{ return m_szSourceFullPath; }
LPCTSTR CPatchMakerConfig::GetTargetFolderFullPath()	{ return m_szTargetFullPath; }
LPCTSTR CPatchMakerConfig::GetVDFFolderFullPath()		{ return m_szVDFFullPath; }

LPCTSTR CPatchMakerConfig::GetTempFolderPath()			{ return m_szTempFolder; }
LPCTSTR CPatchMakerConfig::GetHelperFolderPath()		{ return m_szHelperFolder; }
LPCTSTR CPatchMakerConfig::GetSourceFolderPath()		{ return m_szSourceFolder; }
LPCTSTR CPatchMakerConfig::GetTargetFolderPath()		{ return m_szTargetFolder; }
LPCTSTR CPatchMakerConfig::GetVDFFolderPath()			{ return m_szVDFFolder; }

LPCTSTR CPatchMakerConfig::GetVesionFileName()			{ return m_szVesionFileName; }
LPCTSTR CPatchMakerConfig::GetAutoPatcherFileName()		{ return m_szAutoPatchFileName; }
LPCTSTR CPatchMakerConfig::GetSelfUpdateFileName()		{ return m_szSelfUpdateFileName; }
LPCTSTR CPatchMakerConfig::GetLauncherFileName()		{ return m_szLauncherFileName; }
LPCTSTR CPatchMakerConfig::GetGameBinFileName()			{ return m_szGameFileName; }
LPCTSTR CPatchMakerConfig::GetAutoPatcherDllFileName()	{ return m_szAutoPatchDllFileName; }
LPCTSTR CPatchMakerConfig::GetStorageDllFileName()		{ return m_szStorageDllFileName;}

LPCTSTR CPatchMakerConfig::GetGameName()				{ return m_szGameName; }
LPCTSTR CPatchMakerConfig::GetGameStatus()				{ return m_szGameState; }
LPCTSTR CPatchMakerConfig::GetLocale()					{ return m_szLocale; }
UINT CPatchMakerConfig::GetPoolSize()					{ return m_nPoolSize; }
BOOL CPatchMakerConfig::ManualPatchNeeded()				{ return m_bCreatManualPatch; }
BOOL CPatchMakerConfig::IsMigrated()					{ return m_bMigrated; }

