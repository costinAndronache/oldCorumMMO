#include "stdafx.h"
#include "PatchEngine.h"
#include "DFXForm.h"
#include "FolderOp.h"
#include "VDFNodePool.h"

#ifdef _DEBUG
#pragma comment( lib, "./ZipArchive/Unicode_Debug/ZipArchive.lib" )
#else
#pragma comment( lib, "./ZipArchive/Unicode_Release/ZipArchive.lib" )
#endif

CPatchEngine::CPatchEngine()
{
	m_hUIWnd = NULL;

	CPtrList lit;
}

CPatchEngine::~CPatchEngine()
{
	UnloadVDFs();

	CPatchMakerConfig::DeleteInstance();
	CVDFNodePool::DeleteInstance();
}

BOOL CPatchEngine::Initialize(HWND hUIWnd,CProgressCtrlX* pProgressCtrl,CProgressCtrlX* pProgressCtrlSub)
{	
	m_hUIWnd = hUIWnd;
	m_pProgressCtrlX = pProgressCtrl;
	m_pProgressCtrlXSub = pProgressCtrlSub;

	SetCurrentDirectoryToFolderOfExcutable();

	if(!LoadConfiguration()) 
	{
		MessageBox(m_hUIWnd, _T("Could not load configuration file!"), _T("Configuration"), MB_ICONERROR|MB_OK);
		return FALSE;
	}

	CVDFNodePool::GetInstance(CPatchMakerConfig::GetInstance()->GetPoolSize()); 

	if(!CheckSystemFolderExistance())
	{
		if(IDYES == MessageBox(m_hUIWnd, _T("Cannot find folders. Do you want to make folders?"), 
			_T("Folder"), MB_ICONINFORMATION|MB_YESNO))
		{
			CreateSystemFolders();
		}
	}

	return TRUE;
}

void CPatchEngine::__MigrateVDF(LPCTSTR szVDFFile)
{
	CVDBrowser browser;

	browser.InitBrowserFromVDF(szVDFFile);

	CMapStringToPtr& Map = browser.GetMap();

	CString strKey;
	LPVDFNode pNode = NULL;
	TCHAR szNewPath[0xff]={0,};
	int nSourceFolderLen = lstrlen(CPatchMakerConfig::GetInstance()->GetSourceFolderFullPath());

	POSITION pos = Map.GetStartPosition();
	while(pos)
	{
		Map.GetNextAssoc(pos, strKey, (void*&)pNode);

		// 절대 경로가 세팅되어 있지 않았다면 무시!
		if(_tcsnicmp((LPCTSTR)pNode->szFilePath, CPatchMakerConfig::GetInstance()->GetSourceFolderFullPath(), nSourceFolderLen) != 0) continue;

		// 상대 경로를 구한다.
		wsprintf(szNewPath, _T(".%s"), pNode->szFilePath + nSourceFolderLen); // pNode->szFilePath;

		// 기존 노드에 복사.
		ZeroMemory(pNode->szFilePath, sizeof(pNode->szFilePath));
		lstrcpy(pNode->szFilePath, szNewPath);
	}
}

void CPatchEngine::MigrateVDF()
{
	WIN32_FIND_DATA Find;
	TCHAR szFiles[_MAX_PATH]={0,};
	wsprintf(szFiles, _T("%s\\*.vdf"), CPatchMakerConfig::GetInstance()->GetVDFFolderFullPath());
	
	HANDLE hFind = FindFirstFile(szFiles, &Find);

	if(hFind == INVALID_HANDLE_VALUE) return;

	for(;;)
	{
		if(Find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;

		TCHAR szFilePath[_MAX_PATH]={0,};
		wsprintf(szFilePath, _T("%s\\%s"), CPatchMakerConfig::GetInstance()->GetVDFFolderFullPath(), Find.cFileName);

		__MigrateVDF(szFilePath);

		if(!FindNextFile(hFind, &Find)) break;
	}

	FindClose(hFind);

	CPatchMakerConfig::GetInstance()->SetMigrated();	
}

BOOL CPatchEngine::AddNewVersion(LPVERSION_DATA pData)
{
	// 같은 이름의 버전이 존재하는 지 확인.
	if(IsThereSameVersion(pData->szVersion)) 
	{
		MessageBox(m_hUIWnd, _T("Same Version Exists!!"), _T("Version Checking"), MB_OK|MB_ICONWARNING);
		return FALSE;
	}

	if(lstrcmpi(GetLatestVersionSting(), pData->szVersion) > 0)
	{
		MessageBox(m_hUIWnd, _T("Not Latest Version Name"), _T("Version Checking"), MB_OK|MB_ICONWARNING);
		return FALSE;
	}

	// 마지막 버전이 LOCK된 상태에서만 다음 버전 추가 가능
	if(!IsThereNoPatch())
	{
		if(!GetLastVersionVDF()->IsLocked())
		{
			MessageBox(m_hUIWnd, _T("Last Version is not locked. Lock it FIRST!"), 
				_T("Adding New Version"), MB_OK|MB_ICONWARNING);

			return FALSE;
		}
	}
	
	// new version updated file copy.
	if(!CreateVersionSourceFolder(pData->szVersion))
	{
		return FALSE;
	}

	// file들을 체크!!!!!!!
	CheckAndModifySource(pData->szVersion, IsThereNoPatch());

	// Browse and make VDF File.
	if(!BrowseAndMakeVDF(pData))
	{
		MessageBox(m_hUIWnd, _T("Failed to browse and make vdf"), 
			_T("Adding New Version"), MB_OK|MB_ICONERROR);

		return FALSE;
	}

	// 저장후, 다시 로드? 흠..
	TCHAR szVDF[_MAX_PATH]={0,};
	wsprintf(szVDF, _T("%s\\%s.vdf"), CPatchMakerConfig::GetInstance()->GetVDFFolderFullPath(), pData->szVersion);

	return LoadVDF(szVDF);
}

LPCTSTR CPatchEngine::GetLatestVersionSting()
{
	if(m_PatchList.GetCount() == 0) return NULL;
	CVDBrowser* pLastVersion = GetLastVersionVDF();
	LPVERSION_DATA pVerData = pLastVersion->GetPatchVersionData();
	return pVerData->szVersion;
}

BOOL CPatchEngine::RemoveLastVersion()
{
	if(m_PatchList.GetCount() == 0) return FALSE;

	CVDBrowser* pLastVersion = GetLastVersionVDF();
	if(pLastVersion->IsLocked())
	{
		MessageBox(m_hUIWnd, _T("Last Version Is Locked!"), _T("Deleting Last Version"), MB_OK|MB_ICONWARNING);
		return FALSE;
	}

	// 리스트에서 브라우저 제거.
	m_PatchList.RemoveTail();

	LPVERSION_DATA pData = pLastVersion->GetPatchVersionData();

	ASSERT(pData != NULL);
	
	// source 제거.
	DeleteVersionSourceFolder(pData->szVersion);
	
	TCHAR szVDF[_MAX_PATH]={0,};
	wsprintf(szVDF, _T("%s\\%s.vdf"), CPatchMakerConfig::GetInstance()->GetVDFFolderFullPath(), pData->szVersion);

	// 브라우저 제거.
	delete pLastVersion;

	// vdf 제거.
	DeleteFile(szVDF);

	return TRUE;
}

BOOL CPatchEngine::LockLastVersion(BOOL bLock)								// 마지막 버전을 Lock!
{
	CVDBrowser* pLastVersion = GetLastVersionVDF();
	if(pLastVersion)
	{
		pLastVersion->SetLock(bLock);
		return TRUE;
	}

	return FALSE;
}

BOOL CPatchEngine::ResetPatchAfterBreaking()
{
	BOOL bRet = TRUE;
	VERSION_DATA VersionData;
	
	TCHAR szTempPath[_MAX_PATH]={0,};

	memcpy(&VersionData, GetLastVersionVDF()->GetPatchVersionData(), sizeof(VERSION_DATA));

	// ./Target/04043101/Full
	wsprintf(szTempPath, _T("%s\\%s\\TempForBreak"), 
		CPatchMakerConfig::GetInstance()->GetTempFolderFullPath(),
		VersionData.szVersion);

	// 소스폴더의 절대경로 길이.
	int nSourceFolderLen = lstrlen(CPatchMakerConfig::GetInstance()->GetSourceFolderFullPath());
	int nTargetFolderLen = lstrlen(szTempPath);

	// 기존 존재한 폴더는 제거.
	if(CheckFolderExistance(szTempPath))
	{
		DeleteFolderEx(szTempPath);
	}

	CreateDirectory(szTempPath, NULL);

	// 가상의 VDBrowser 생성... 최신 버전으로 세팅.
	CVDBrowser* pLatest = MakeVirtualLatestVDF();
	CMapStringToPtr& Map = pLatest->GetMap();	

	DWORD dwSourcePathLen = 0;
	DWORD dwTargetPathLen = 0;

	CString strKey;
	LPVDFNode pNode;
	TCHAR szTargetPath[_MAX_PATH]={0,};

	// 일단 file op를 위한 버퍼 크기를 구하자!
	POSITION pos = Map.GetStartPosition();
	while(pos)
	{
		Map.GetNextAssoc(pos, strKey, (LPVOID&)pNode);

		dwSourcePathLen += (lstrlen(pNode->szFilePath+1)+1);
		dwTargetPathLen += (lstrlen(pNode->szFileDest+1)+1);
	}

	dwSourcePathLen += (nSourceFolderLen * Map.GetCount());
	dwTargetPathLen += (nTargetFolderLen * Map.GetCount());

	LPTSTR pszSources = new TCHAR [dwSourcePathLen+1];
	LPTSTR pszTargets = new TCHAR [dwTargetPathLen+1];
	memset(pszSources, 0, sizeof(TCHAR)* (dwSourcePathLen+1));
	memset(pszTargets, 0, sizeof(TCHAR)* (dwTargetPathLen+1));
	LPTSTR pszSourceNext = pszSources;
	LPTSTR pszTargetNext = pszTargets;

	CMapStringToString tempMap;
	CString strTemp;

	// 실제로 버퍼에 쓰자~!
	pos = Map.GetStartPosition();
	while(pos)
	{
		Map.GetNextAssoc(pos, strKey, (LPVOID&)pNode);

		lstrcpy(pszTargetNext, szTempPath);
		lstrcat(pszTargetNext, pNode->szFileDest + 1);
		pszTargetNext += (nTargetFolderLen + lstrlen(pNode->szFileDest + 1) + 1);

		lstrcpy(pszSourceNext, CPatchMakerConfig::GetInstance()->GetSourceFolderFullPath());
		lstrcat(pszSourceNext, pNode->szFilePath + 1);
		pszSourceNext += (nSourceFolderLen + lstrlen(pNode->szFilePath + 1) + 1);
	}

	// 파일 복사~!
	if(!CopyFileEx(pszSources, pszTargets))
	{
		DWORD dwErr = GetLastError();
		MessageBox(m_hUIWnd, _T("Failed to copy full version file.."), _T("Building Last Full Version"), MB_OK|MB_ICONERROR);
		bRet = FALSE;
	}

	SetFileAttributeEx(szTempPath, FILE_ATTRIBUTE_NORMAL);
	
	delete pLatest;
	delete [] pszSources;
	delete [] pszTargets;

	// 자 이제.. 기존 VDF 제거 하고...
	// 기존 소스를 모두 제거하고..
	for(;;)
	{
		if(!LockLastVersion(FALSE)) break;
		if(!RemoveLastVersion()) break;
	}	

	// Folder를 소스로 옮기고 
	TCHAR szFolder[_MAX_PATH]={0,};
	wsprintf(szFolder, _T("%s\\%s"), CPatchMakerConfig::GetInstance()->GetSourceFolderFullPath(), VersionData.szVersion);

	MoveFileEx(szTempPath, szFolder, MOVEFILE_WRITE_THROUGH|MOVEFILE_REPLACE_EXISTING);
	DeleteFolderEx(szTempPath);

	// file들을 체크!!!!!!!
	CheckAndModifySource(VersionData.szVersion, IsThereNoPatch());

	// Browse and make VDF File.
	if(!BrowseAndMakeVDF(&VersionData))
	{
		MessageBox(m_hUIWnd, _T("Failed to browse and make vdf"), 
			_T("Adding New Version"), MB_OK|MB_ICONERROR);

		return FALSE;
	}

	// 저장후, 다시 로드? 흠..
	TCHAR szVDF[_MAX_PATH]={0,};
	wsprintf(szVDF, _T("%s\\%s.vdf"), CPatchMakerConfig::GetInstance()->GetVDFFolderFullPath(), VersionData.szVersion);

	return LoadVDF(szVDF);
}

BOOL CPatchEngine::BuildLastFullVersion()							// 최신의 full버전을 제작
{
	BOOL bRet = TRUE;

	TCHAR szFullVersionPath[_MAX_PATH]={0,};

	// ./Target/04043101/Full
	wsprintf(szFullVersionPath, _T("%s\\%s\\Full"), 
		CPatchMakerConfig::GetInstance()->GetTargetFolderFullPath(),
		GetLastVersionVDF()->GetPatchVersionData()->szVersion);

	// 소스폴더의 절대경로 길이.
	int nSourceFolderLen = lstrlen(CPatchMakerConfig::GetInstance()->GetSourceFolderFullPath());
	int nTargetFolderLen = lstrlen(szFullVersionPath);

	// 기존 존재한 폴더는 제거.
	if(CheckFolderExistance(szFullVersionPath))
	{
		DeleteFolderEx(szFullVersionPath);
	}
	
	CreateDirectory(szFullVersionPath, NULL);

	// 가상의 VDBrowser 생성... 최신 버전으로 세팅.
	CVDBrowser* pLatest = MakeVirtualLatestVDF();
	CMapStringToPtr& Map = pLatest->GetMap();	

	DWORD dwSourcePathLen = 0;
	DWORD dwTargetPathLen = 0;

	CString strKey;
	LPVDFNode pNode;
	TCHAR szTargetPath[_MAX_PATH]={0,};

	// 일단 file op를 위한 버퍼 크기를 구하자!
	POSITION pos = Map.GetStartPosition();
	while(pos)
	{
		Map.GetNextAssoc(pos, strKey, (LPVOID&)pNode);

		dwSourcePathLen += (lstrlen(pNode->szFilePath+1)+1);
		dwTargetPathLen += (lstrlen(pNode->szFileDest+1)+1);
	}

	dwSourcePathLen += (nSourceFolderLen * Map.GetCount());
	dwTargetPathLen += (nTargetFolderLen * Map.GetCount());

	LPTSTR pszSources = new TCHAR [dwSourcePathLen+1];
	LPTSTR pszTargets = new TCHAR [dwTargetPathLen+1];
	memset(pszSources, 0, sizeof(TCHAR)* (dwSourcePathLen+1));
	memset(pszTargets, 0, sizeof(TCHAR)* (dwTargetPathLen+1));
	LPTSTR pszSourceNext = pszSources;
	LPTSTR pszTargetNext = pszTargets;

	CMapStringToString tempMap;
	CString strTemp;

	// 실제로 버퍼에 쓰자~!
	pos = Map.GetStartPosition();
	while(pos)
	{
		Map.GetNextAssoc(pos, strKey, (LPVOID&)pNode);

		lstrcpy(pszTargetNext, szFullVersionPath);
		lstrcat(pszTargetNext, pNode->szFileDest + 1);
		pszTargetNext += (nTargetFolderLen + lstrlen(pNode->szFileDest + 1) + 1);

		lstrcpy(pszSourceNext, CPatchMakerConfig::GetInstance()->GetSourceFolderFullPath());
		lstrcat(pszSourceNext, pNode->szFilePath + 1);
		pszSourceNext += (nSourceFolderLen + lstrlen(pNode->szFilePath + 1) + 1);
	}

	// 파일 복사~!
	if(!CopyFileEx(pszSources, pszTargets))
	{
		DWORD dwErr = GetLastError();
		MessageBox(m_hUIWnd, _T("Failed to copy full version file.."), _T("Building Last Full Version"), MB_OK|MB_ICONERROR);
		bRet = FALSE;
	}

	// version.ver file 추가.
	TCHAR szVersionFile[_MAX_PATH]={0,};
	__AddVersionInfoFile(GetLastVersionVDF()->GetPatchVersionData()->szVersion, 
		szFullVersionPath, export_full, szVersionFile);

	SetFileAttributeEx(szFullVersionPath, FILE_ATTRIBUTE_NORMAL);
	
	delete pLatest;
	delete [] pszSources;
	delete [] pszTargets;

	return bRet;
}


BOOL CPatchEngine::BuildLastPatchFiles(BOOL bBreak)						// 최신의 패치 파일들을 제작
{
	if(m_PatchList.GetCount() < 2)
	{
		MessageBox(m_hUIWnd, _T("There must be more than 2 version."), _T("Building Patch Files"), MB_OK|MB_ICONWARNING);
		return FALSE;
	}

	TCHAR szFolder[_MAX_PATH]={0,};
	wsprintf(szFolder, _T("%s\\%s"), 
		CPatchMakerConfig::GetInstance()->GetTargetFolderFullPath(),
		GetLastVersionVDF()->GetPatchVersionData()->szVersion);

	// 기존 존재한 폴더는 제거.
	if(CheckFolderExistance(szFolder))
	{
		SearchAndDeleteFiles(szFolder, _T("*.zip"));
	}
	else
	{
		CreateDirectory(szFolder, NULL);
	}	

	// 자동 패치용 ZIP 파일 생성.
	if(!__CreateAutoPatchZipFiles(bBreak))
	{
		return FALSE;	
	}

	if(!bBreak)
	{
		// 수동 누적 패치 제작한다고 세팅되어 있으면...
		if(CPatchMakerConfig::GetInstance()->ManualPatchNeeded())
		{
			// 수동 패치용 누적 패치 ZIP 파일 생성.
			if(!__CreateManualPatchZipFile())
			{
				return FALSE;	
			}
		}
	}

	return TRUE;
}

BOOL CPatchEngine::BuildLastPatchFilesWithSlicing(BOOL bBreak)	// 최신의 패치 파일들을 제작(분할압축형태)
{
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CPatchEngine::CheckPassword(LPCSTR szPwd)
{
	// CHECK ADMIN PASSWORD!!	
#ifdef NO_ENCRYPT
	return (strcmp("deepdark", szPwd) == 0);
#else
	DFXform xform;
	CHAR szTemp[0xff]={0,};
	CHAR szTemp2[0xff]={0,};
	UINT nSize = 0;

	xform.UUEncode((LPBYTE)szPwd, strlen(szPwd)+1, (LPBYTE)szTemp, &nSize);
	xform.XXEncode((LPBYTE)szTemp, nSize, (LPBYTE)szTemp2, &nSize);

	// check password!!
	return (strcmp("NZFWQKNHVbRdV37vNbBGUaNhHIs-", szTemp2) == 0);
#endif
}

BOOL CPatchEngine::LoadConfiguration()
{
	return CPatchMakerConfig::GetInstance()->Initialize();
}

void CPatchEngine::CreateSystemFolders()
{
	CreateDirectory(CPatchMakerConfig::GetInstance()->GetHelperFolderFullPath(), NULL);
	CreateDirectory(CPatchMakerConfig::GetInstance()->GetSourceFolderFullPath(), NULL);
	CreateDirectory(CPatchMakerConfig::GetInstance()->GetTargetFolderFullPath(), NULL);
	CreateDirectory(CPatchMakerConfig::GetInstance()->GetTempFolderFullPath(), NULL);
	CreateDirectory(CPatchMakerConfig::GetInstance()->GetVDFFolderFullPath(), NULL);
}

BOOL CPatchEngine::CheckSystemFolderExistance()
{
	if(!CheckFolderExistance(CPatchMakerConfig::GetInstance()->GetHelperFolderFullPath())) return FALSE;
	if(!CheckFolderExistance(CPatchMakerConfig::GetInstance()->GetSourceFolderFullPath())) return FALSE;
	if(!CheckFolderExistance(CPatchMakerConfig::GetInstance()->GetTargetFolderFullPath())) return FALSE;
	if(!CheckFolderExistance(CPatchMakerConfig::GetInstance()->GetTempFolderFullPath())) return FALSE;
	if(!CheckFolderExistance(CPatchMakerConfig::GetInstance()->GetVDFFolderFullPath())) return FALSE;

	return TRUE;
}

BOOL CPatchEngine::CreateVersionSourceFolder(LPCTSTR szVersion)	// ./Source에 Version Folder 생성.
{
	TCHAR szFolder[_MAX_PATH]={0,};
	wsprintf(szFolder, _T("%s\\%s"), CPatchMakerConfig::GetInstance()->GetSourceFolderFullPath(), szVersion);

	TCHAR szSource[_MAX_PATH]={0,};
	GetFolderPath(_T(""), szSource);

	if(!CopyFolderEx(szSource, szFolder))
	{
		DeleteFolderEx(szFolder);
		return FALSE;
	}

	return TRUE;
}

void CPatchEngine::DeleteVersionSourceFolder(LPCTSTR szVersion)	// ./Source에서 Version Folder 제거(파일 포함).
{
	TCHAR szFolder[_MAX_PATH]={0,};
	wsprintf(szFolder, _T("%s\\%s"), CPatchMakerConfig::GetInstance()->GetSourceFolderFullPath(), szVersion);

	DeleteFolderEx(szFolder);
}

void CPatchEngine::MaintainAutopatcher(LPCTSTR szSourcePath)
{
	TCHAR szSourceFile[_MAX_PATH]={0,};
	TCHAR szTargetFile[_MAX_PATH]={0,};

	// exe
	wsprintf(szSourceFile, _T("%s\\%s"), szSourcePath, CPatchMakerConfig::GetInstance()->GetAutoPatcherFileName());

	if(IsThereFile(szSourceFile))
	{
		wsprintf(szTargetFile, _T("%s\\%s"), 
			CPatchMakerConfig::GetInstance()->GetHelperFolderFullPath(),
			CPatchMakerConfig::GetInstance()->GetAutoPatcherFileName());

		CopyFile(szSourceFile, szTargetFile, FALSE);
	}

	// dll
	wsprintf(szSourceFile, _T("%s\\%s"), szSourcePath, CPatchMakerConfig::GetInstance()->GetAutoPatcherDllFileName());

	if(IsThereFile(szSourceFile))
	{
		wsprintf(szTargetFile, _T("%s\\%s"), 
			CPatchMakerConfig::GetInstance()->GetHelperFolderFullPath(),
			CPatchMakerConfig::GetInstance()->GetAutoPatcherDllFileName());

		CopyFile(szSourceFile, szTargetFile, FALSE);
	}

	// Storage dll
	wsprintf(szSourceFile, _T("%s\\%s"), szSourcePath, CPatchMakerConfig::GetInstance()->GetStorageDllFileName());

	if(IsThereFile(szSourceFile))
	{
		wsprintf(szTargetFile, _T("%s\\%s"), 
			CPatchMakerConfig::GetInstance()->GetHelperFolderFullPath(),
			CPatchMakerConfig::GetInstance()->GetStorageDllFileName());

		CopyFile(szSourceFile, szTargetFile, FALSE);
	}
}

// patch에 문제가 되는 파일이 존재하는 지 체크 후, 삭제 혹은 이름 변경.
void CPatchEngine::CheckAndModifySource(LPCTSTR szVersion, BOOL bOriginal)
{
	TCHAR szSourcePath[_MAX_PATH]={0,};
	wsprintf(szSourcePath, _T("%s\\%s"),CPatchMakerConfig::GetInstance()->GetSourceFolderFullPath(), szVersion);
	
	/*	
	  1. 첫 버전이 아닌 경우 File_Downloader.dll는 삭제한다.
	  2. Notice*.txt는 삭제한다.
	  3. CorumVersionInfo.ver는 삭제한다.
	  4. SelfUpdate.exe도 제거한다.
	  5. 모든 file을 readonly로 수정한다.
	*/

	DeleteExclusiveFiles(szSourcePath, bOriginal);
	SetFileAttributeEx(szSourcePath, FILE_ATTRIBUTE_READONLY);

	MaintainAutopatcher(szSourcePath);
}

void CPatchEngine::DeleteExclusiveFiles(LPCTSTR szPath, BOOL bOriginal)
{
	TCHAR szFile[_MAX_PATH]={0,};

	// 1. 첫 버전이 아닌 경우 FTP_Downloader.dll는 삭제한다.
//	if(!bOriginal)
//	{
//		wsprintf(szFile, _T("%s\\%s"), szPath, CPatchMakerConfig::GetInstance()->GetAutoPatcherDllFileName());
//		DeleteFile(szFile);
//	}	

	// 2. Notice*.txt는 삭제한다.
	SearchAndDeleteFiles(szPath, _T("Notice*.txt"));

	// 3. CorumVersionInfo.ver는 삭제한다.
	wsprintf(szFile, _T("%s\\%s"), szPath, CPatchMakerConfig::GetInstance()->GetVesionFileName());
	DeleteFile(szFile);

	// 4. CorumVersionInfo_New.ver는 삭제한다.
	TCHAR szNewFile[_MAX_PATH]={0,};
	__MakeNewFileName(szFile, szNewFile);
	DeleteFile(szNewFile);

	// 5. SelfUpdate.exe도 제거한다.
	wsprintf(szFile, _T("%s\\%s"), szPath, CPatchMakerConfig::GetInstance()->GetSelfUpdateFileName());
	DeleteFile(szFile);

	// 6. Autopatch_New.exe이 존재하면... 오리지널인 경우, 기존 것에 덮을 것인지 물어보고, 아니면,...
	// _New를 제거하여... 패치만든다..?
	wsprintf(szFile, _T("%s\\%s"), szPath, CPatchMakerConfig::GetInstance()->GetAutoPatcherFileName());
	__MakeNewFileName(szFile, szNewFile);

	if(IsThereFile(szNewFile))
	{
		if(IsThereFile(szFile))
		{
			if(IDYES == MessageBox(m_hUIWnd, _T("Update NEW file to original?"), _T("New Autopatch found"), MB_ICONWARNING|MB_YESNO))
			{
				DeleteFile(szFile);
				MoveFile(szNewFile, szFile);
			}
			else
			{
				DeleteFile(szNewFile);
			}
		}
		else
		{
			MoveFile(szNewFile, szFile);
		}
	}

	// 7. RainFtp_new.dll 등의 autopatch dll을 처리
	wsprintf(szFile, _T("%s\\%s"), szPath, CPatchMakerConfig::GetInstance()->GetAutoPatcherDllFileName());
	__MakeNewFileName(szFile, szNewFile);

	if(IsThereFile(szNewFile))
	{
		if(IsThereFile(szFile))
		{
			if(IDYES == MessageBox(m_hUIWnd, _T("Update NEW file to original?"), _T("New Autopatch DLL found"), MB_ICONWARNING|MB_YESNO))
			{
				DeleteFile(szFile);
				MoveFile(szNewFile, szFile);
			}
			else
			{
				DeleteFile(szNewFile);
			}
		}
		else
		{
			MoveFile(szNewFile, szFile);
		}
	}

	// 8. Storage_new.dll 처리
	wsprintf(szFile, _T("%s\\%s"), szPath, CPatchMakerConfig::GetInstance()->GetStorageDllFileName());
	__MakeNewFileName(szFile, szNewFile);

	if(IsThereFile(szNewFile))
	{
		if(IsThereFile(szFile))
		{
			if(IDYES == MessageBox(m_hUIWnd, _T("Update NEW file to original?"), _T("New File Storage DLL found"), MB_ICONWARNING|MB_YESNO))
			{
				DeleteFile(szFile);
				MoveFile(szNewFile, szFile);
			}
			else
			{
				DeleteFile(szNewFile);
			}
		}
		else
		{
			MoveFile(szNewFile, szFile);
		}
	}
}

BOOL CPatchEngine::IsThereSameVersion(LPCTSTR szVersion)
{
	POSITION pos = m_PatchList.GetHeadPosition();
	while(pos)
	{
		if(lstrcmpi(m_PatchList.GetNext(pos)->GetPatchVersionData()->szVersion, szVersion) == 0)
			return TRUE;
	}

	return FALSE;
}

BOOL CPatchEngine::LoadExistingVDF()
{
	CString strFile;
	CFileFind finder;
	TCHAR szSearch[_MAX_PATH];
	wsprintf(szSearch, _T("%s\\*.vdf"), CPatchMakerConfig::GetInstance()->GetVDFFolderFullPath());

	BOOL bWorking = finder.FindFile(szSearch);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		
		if(!LoadVDF(finder.GetFilePath()))
		{
			MessageBox(m_hUIWnd, _T("Failed to load VDF file"), _T("LoadVDF"), MB_OK|MB_ICONERROR);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CPatchEngine::LoadVDF(LPCTSTR szFile)
{
	CVDBrowser* pBrowser = new CVDBrowser;

	if(!pBrowser->InitBrowserFromVDF(szFile))
	{
		return FALSE;
	}

	m_PatchList.AddTail(pBrowser);

	// for ui
	COPYDATASTRUCT cpydata;
	cpydata.lpData = pBrowser->GetPatchVersionData();
	cpydata.cbData = sizeof(VERSION_DATA);	

	SendMessage(m_hUIWnd, WM_COPYDATA, 0, (LPARAM)&cpydata);

	return TRUE;
}

void CPatchEngine::UnloadVDFs()
{
	POSITION pos = m_PatchList.GetHeadPosition();
	while(pos)
	{
		delete m_PatchList.GetNext(pos);
	}

	m_PatchList.RemoveAll();
}

CVDBrowser* CPatchEngine::GetLastVersionVDF()
{
	if(m_PatchList.GetCount() == 0) return NULL;

	return m_PatchList.GetTail();
}

BOOL CPatchEngine::BrowseAndMakeVDF(LPVERSION_DATA pData)
{
	TCHAR szFolder[_MAX_PATH]={0,};
	wsprintf(szFolder, _T("%s\\%s"), CPatchMakerConfig::GetInstance()->GetSourceFolderFullPath(), pData->szVersion);

	// 업데이트 목록으로 생짜 VDF를 만든다.
	CVDBrowser* pNewBrowser = new CVDBrowser;
	pNewBrowser->InitBrowser(szFolder);

	// 이제 최신의 VDF가 만들어졌으므로 FILE로 저장한다.
	TCHAR szVDF[_MAX_PATH]={0,};
	wsprintf(szVDF, _T("%s\\%s.vdf"), CPatchMakerConfig::GetInstance()->GetVDFFolderFullPath(), pData->szVersion);

	if(!pNewBrowser->SaveVDFFile(szVDF, pNewBrowser->GetMap(), pData, FALSE))
	{
		MessageBox(m_hUIWnd, _T("Failed to Save VDF"), _T("Save VDF"), MB_OK|MB_ICONERROR);
		delete pNewBrowser;

		return FALSE;
	}

	delete pNewBrowser;

	return TRUE;
}

CVDBrowser* CPatchEngine::MakeVirtualLatestVDF(LPCTSTR szVersion)
{
	CVDBrowser* pNewBrowser = new CVDBrowser;
	CVDBrowser* pBrowser = NULL;

	POSITION pos = m_PatchList.GetHeadPosition();
	while(pos)
	{
		pBrowser = m_PatchList.GetNext(pos);

		// 지칭한 버전이 더 작은 경우!!
		if(lstrcmpi(szVersion, pBrowser->GetPatchVersionData()->szVersion) < 0)
		{
			pNewBrowser->InitBrowser(pBrowser);
		}
	}

	return pNewBrowser;
}

CVDBrowser* CPatchEngine::MakeVirtualLatestVDF(BOOL bExcludeOrg)
{
	CVDBrowser* pBrowser = NULL;
	CVDBrowser* pNewBrowser = new CVDBrowser;

	POSITION pos = m_PatchList.GetHeadPosition();
	while(pos)
	{
		pBrowser = m_PatchList.GetNext(pos);

		if(bExcludeOrg && (m_PatchList.GetHead() == pBrowser))
			continue;

		pNewBrowser->InitBrowser(pBrowser);		
	}

	return pNewBrowser;
}

BOOL CPatchEngine::IsThereNoPatch()
{
	return (m_PatchList.GetCount() == 0);
}

BOOL CPatchEngine::__CreateZipFile(LPCTSTR szFileName, CVDBrowser* pBrowser, LPEXPORT_DESC lpExportDesc)
{
	CMyZipArchive* lpZipArchive = new CMyZipArchive;
	if(lpZipArchive == NULL)
	{
		MessageBox(m_hUIWnd, _T("Failed to create ZipArchive"), _T("Creating ZipArchive"), MB_OK|MB_ICONERROR);
		return FALSE;
	}

	lpZipArchive->Open(szFileName, CZipArchive::create, NULL);
	
	CMapStringToPtr& Map = pBrowser->GetMap();
	
	CString strKey;
	LPVDFNode pNode = NULL;

	TCHAR szRealPath[0xff]={0,};

	int iTotal	= Map.GetCount();
	int iCur	= 0;

	m_pProgressCtrlXSub->SetShowPercent(TRUE);

	POSITION pos = Map.GetStartPosition();
	while(pos)
	{
		Map.GetNextAssoc(pos, strKey, (LPVOID&)pNode);

		ZeroMemory(szRealPath, sizeof(szRealPath));
		lstrcpy(szRealPath, CPatchMakerConfig::GetInstance()->GetSourceFolderFullPath());
		lstrcat(szRealPath, pNode->szFilePath + 1);

		// 아래의 형태를 따른다!!!! "./"를 제거하면 된다.
		// lpZipArchive->AddNewFile("data2\\file1.txt", "c:\\ziptemp\\file4.txt", time, FILE_ATTRIBUTE_NORMAL);
		lpZipArchive->AddNewFile(pNode->szFileDest+2, szRealPath, pNode->ModTime, FILE_ATTRIBUTE_NORMAL);

		// progress SetPos
		int iPercentage = int(((++iCur * 100.) / iTotal));
		m_pProgressCtrlXSub->SetPos(iPercentage);
	}

	m_pProgressCtrlXSub->SetPos(0);
	m_pProgressCtrlXSub->HideText();

	/*
	  1. 현재 버전에 맞게 버전 정보 파일을 추가한다.
	    - FULL   : CorumVersionInfo_New.ver를 추가한다.
		- Manual : CorumVersionInfo.ver를 추가한다.
		- Auto   : CorumVersionInfo.ver를 추가한다.

	  2. AutoPatch의 경우, 
	    - CorumAutoPatch.exe는 CorumAutoPatch_New.exe로 변경.
	    - CorumSelfUpdate.exe 추가. 	
	*/

	// versioninfo.ver 추가, 파일 이름은 callee에서 만들면서 결정해 줌.
	FILETIME time;
	GetSystemTimeAsFileTime(&time);

	TCHAR szDest[0xff]={0,};
	GetFileTitle(lpExportDesc->szVersionInfo, szDest);
	
	lpZipArchive->AddNewFile(szDest, lpExportDesc->szVersionInfo, time, FILE_ATTRIBUTE_NORMAL);
	
	// autopatch 정리!!
	BOOL bAddSelfUpdate = FALSE;
	if(lpExportDesc->e_type == export_auto)
	{
		// autopatch.exe 의 업데이트
		int nIndex = lpZipArchive->FindFile(CPatchMakerConfig::GetInstance()->GetAutoPatcherFileName());
		if(nIndex != -1)
		{
			// 기존 넘 제거.
			lpZipArchive->DeleteFile(nIndex);

			// 이름 바꾼 넘 추가.
			TCHAR szNewVersionFile[0xff]={0,};
			__MakeNewFileName(CPatchMakerConfig::GetInstance()->GetAutoPatcherFileName(), szNewVersionFile);

			TCHAR szDest[0xff]={0,};
			wsprintf(szDest, _T(".\\%s"), CPatchMakerConfig::GetInstance()->GetAutoPatcherFileName());
			Map.Lookup(CharLower(szDest), (LPVOID&) pNode);
			
			ZeroMemory(szRealPath, sizeof(szRealPath));
			lstrcpy(szRealPath, CPatchMakerConfig::GetInstance()->GetSourceFolderFullPath());
			lstrcat(szRealPath, pNode->szFilePath + 1);

			lpZipArchive->AddNewFile(szNewVersionFile, szRealPath, pNode->ModTime, FILE_ATTRIBUTE_NORMAL);

			// SelpUpdate 추가.
			bAddSelfUpdate = TRUE;
		}

		// rainftp.dll의 업데이트
		nIndex = lpZipArchive->FindFile(CPatchMakerConfig::GetInstance()->GetAutoPatcherDllFileName());
		if(nIndex != -1)
		{
			// 기존 넘 제거.
			lpZipArchive->DeleteFile(nIndex);

			// 이름 바꾼 넘 추가.
			TCHAR szNewVersionFile[0xff]={0,};
			__MakeNewFileName(CPatchMakerConfig::GetInstance()->GetAutoPatcherDllFileName(), szNewVersionFile);

			TCHAR szDest[0xff]={0,};
			wsprintf(szDest, _T(".\\%s"), CPatchMakerConfig::GetInstance()->GetAutoPatcherDllFileName());
			Map.Lookup(CharLower(szDest), (LPVOID&) pNode);			

			ZeroMemory(szRealPath, sizeof(szRealPath));
			lstrcpy(szRealPath, CPatchMakerConfig::GetInstance()->GetSourceFolderFullPath());
			lstrcat(szRealPath, pNode->szFilePath + 1);

			lpZipArchive->AddNewFile(szNewVersionFile, szRealPath, pNode->ModTime, FILE_ATTRIBUTE_NORMAL);

			// SelpUpdate 추가.
			bAddSelfUpdate = TRUE;
		}

		// storage dll의 업데이트.
		nIndex = lpZipArchive->FindFile(CPatchMakerConfig::GetInstance()->GetStorageDllFileName());
		if(nIndex != -1)
		{
			// 기존 넘 제거.
			lpZipArchive->DeleteFile(nIndex);

			// 이름 바꾼 넘 추가.
			TCHAR szNewVersionFile[0xff]={0,};
			__MakeNewFileName(CPatchMakerConfig::GetInstance()->GetStorageDllFileName(), szNewVersionFile);

			TCHAR szDest[0xff]={0,};
			wsprintf(szDest, _T(".\\%s"), CPatchMakerConfig::GetInstance()->GetStorageDllFileName());
			Map.Lookup(CharLower(szDest), (LPVOID&) pNode);
			
			ZeroMemory(szRealPath, sizeof(szRealPath));
			lstrcpy(szRealPath, CPatchMakerConfig::GetInstance()->GetSourceFolderFullPath());
			lstrcat(szRealPath, pNode->szFilePath + 1);

			lpZipArchive->AddNewFile(szNewVersionFile, szRealPath, pNode->ModTime, FILE_ATTRIBUTE_NORMAL);

			// SelpUpdate 추가.
			bAddSelfUpdate = TRUE;
		}

		// autopatch 관련 업데이트가 없다면.. 그리고 끊는 경우라면...
		if(!bAddSelfUpdate && lpExportDesc->bBreak)
		{
			TCHAR szNewVersionFile[0xff]={0,};
			__MakeNewFileName(CPatchMakerConfig::GetInstance()->GetAutoPatcherFileName(), szNewVersionFile);
			
			TCHAR szFilePath[0xff]={0,};
			wsprintf(szFilePath, _T("%s\\%s"), 
				CPatchMakerConfig::GetInstance()->GetHelperFolderFullPath(),
				CPatchMakerConfig::GetInstance()->GetAutoPatcherFileName());

			while(!IsThereFile(szFilePath))
			{
				MessageBox(m_hUIWnd, _T("Copy Autopatcher to Helper Folder Now!"),
					_T("Autopatcher Not Found"), MB_OK|MB_ICONWARNING);
			}

			lpZipArchive->AddNewFile(szNewVersionFile, szFilePath, GetFileModTime(szFilePath), FILE_ATTRIBUTE_NORMAL);
			
			bAddSelfUpdate = TRUE;
		}

		if(bAddSelfUpdate)
		{
			// SelpUpdate 추가.
			__AddSelpUpdateToZip(lpZipArchive);
		}
	}

	lpZipArchive->Close();

	delete lpZipArchive;

	return TRUE;
}

void CPatchEngine::__AddSelpUpdateToZip(CMyZipArchive* lpZipArchive)
{
	// SelpUpdate 추가.
	TCHAR szSelfUpdatePath[_MAX_PATH]={0,};
	wsprintf(szSelfUpdatePath, _T("%s\\%s"), 
		CPatchMakerConfig::GetInstance()->GetHelperFolderFullPath(),
		CPatchMakerConfig::GetInstance()->GetSelfUpdateFileName());

	// 존재하지 않으면 존재할 때까지 넣으라고 지랄한다.. 지랄지랄...
	while(!IsThereFile(szSelfUpdatePath))
	{
		MessageBox(m_hUIWnd, _T("Copy SelfUpdate to Helper Folder Now!"),
			_T("SelfUpdate Not Found"), MB_OK|MB_ICONWARNING);
	}

	FILETIME time;
	GetSystemTimeAsFileTime(&time);

	lpZipArchive->AddNewFile(CPatchMakerConfig::GetInstance()->GetSelfUpdateFileName(), 
		szSelfUpdatePath, time, FILE_ATTRIBUTE_NORMAL);
}


BOOL CPatchEngine::__CreateAutoPatchZipFiles(BOOL bBreak)
{
	EXPORT_DESC ExportDesc;
	TCHAR szFileName[_MAX_PATH]={0,};
	CVDBrowser* pBrowser = NULL;
	CVDBrowser* pLatest = NULL;
	BOOL bRet = FALSE;

	// TEMP 폴더에 VERSIONINFO.VER 파일 생성.
	TCHAR szVersionFile[_MAX_PATH]={0,};
	__AddVersionInfoFile(GetLastVersionVDF()->GetPatchVersionData()->szVersion, 
		CPatchMakerConfig::GetInstance()->GetTempFolderFullPath(), export_auto, szVersionFile);

	int iTotal = m_PatchList.GetCount() - 1;	// 전체 버전 갯수 - 1 이 패치 파일의 숫자다!!
	int iCur   = 0; 
	POSITION pos = m_PatchList.GetHeadPosition();

	m_pProgressCtrlX->SetPos(0);
	m_pProgressCtrlX->SetShowPercent(TRUE);	
	
	while(pos)
	{
		pBrowser = m_PatchList.GetNext(pos);
		if(m_PatchList.GetTail() == pBrowser) continue;

		// ./Target/04043101/OPEN04043101.zip
		wsprintf(szFileName, _T("%s\\%s\\%s%s.zip"), 
			CPatchMakerConfig::GetInstance()->GetTargetFolderFullPath(),
			GetLastVersionVDF()->GetPatchVersionData()->szVersion,
			CPatchMakerConfig::GetInstance()->GetGameStatus(),
			pBrowser->GetPatchVersionData()->szVersion);

		pLatest = MakeVirtualLatestVDF(pBrowser->GetPatchVersionData()->szVersion);

		ExportDesc.e_type = export_auto;
		ExportDesc.bBreak = bBreak;
		lstrcpy(ExportDesc.szVersionInfo, szVersionFile);

		bRet = __CreateZipFile(szFileName, pLatest, &ExportDesc);

		delete pLatest;
		
		// progress SetPos
		int iPercentage = int(((++iCur * 100.) / iTotal));
		m_pProgressCtrlX->SetPos(iPercentage);

		if(bRet == FALSE) break;
	}

	m_pProgressCtrlXSub->SetPos(0);
	m_pProgressCtrlXSub->SetShowPercent(FALSE);
	m_pProgressCtrlXSub->HideText();

	DeleteFile(szVersionFile);

	return bRet;
}

BOOL CPatchEngine::__CreateManualPatchZipFile()
{
	EXPORT_DESC ExportDesc;
	TCHAR szFileName[_MAX_PATH]={0,};

	// ./Target/04043101/OPEN04043101_Manual.zip
	wsprintf(szFileName, _T("%s\\%s\\%s%s_Manual.zip"), 
		CPatchMakerConfig::GetInstance()->GetTargetFolderFullPath(),
		GetLastVersionVDF()->GetPatchVersionData()->szVersion,
		CPatchMakerConfig::GetInstance()->GetGameStatus(),
		GetLastVersionVDF()->GetPatchVersionData()->szVersion);

	// TEMP 폴더에 VERSIONINFO.VER 파일 생성.
	TCHAR szVersionFile[_MAX_PATH]={0,};
	__AddVersionInfoFile(GetLastVersionVDF()->GetPatchVersionData()->szVersion, 
		CPatchMakerConfig::GetInstance()->GetTempFolderFullPath(), export_full,	szVersionFile);

	//// 0: Desination. 1: SourcePath
	ExportDesc.e_type = export_full;
	ExportDesc.bBreak = FALSE;
	lstrcpy(ExportDesc.szVersionInfo, szVersionFile);
	
	CVDBrowser* pLatest = MakeVirtualLatestVDF(TRUE);

	BOOL bRet = __CreateZipFile(szFileName, pLatest, &ExportDesc);

	delete pLatest;

	DeleteFile(szVersionFile);

	m_pProgressCtrlX->SetPos((int)100);
	m_pProgressCtrlX->SetPos(0);
	m_pProgressCtrlX->HideText();

	return bRet;
}

void CPatchEngine::__MakeNewFileName(LPCTSTR szFile, LPTSTR szNewFile)
{
	int iLen = lstrlen(szFile);

	lstrcpyn(szNewFile, szFile, iLen - 3);
	_tcscat(szNewFile, _T("_New"));
	_tcscat(szNewFile, szFile + iLen - 4);
}

void CPatchEngine::__AddVersionInfoFile(LPCTSTR szVersion, LPCTSTR szPath, export_type e_type, OUT LPTSTR szVersionFile)
{
	TCHAR szNewFile[_MAX_PATH]={0,};

	if(e_type == export_auto)
		__MakeNewFileName(CPatchMakerConfig::GetInstance()->GetVesionFileName(), szNewFile);
	else
		lstrcpy(szNewFile, CPatchMakerConfig::GetInstance()->GetVesionFileName());

	wsprintf(szVersionFile, _T("%s\\%s"), szPath, szNewFile);

	HANDLE	hFile;
	DWORD	dwBytesWritten;
	DWORD	dwReturn;

	hFile = CreateFile(	szVersionFile, GENERIC_WRITE, 0, NULL,
						CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL );

	if( hFile == INVALID_HANDLE_VALUE )
	{
		MessageBox(m_hUIWnd, _T("Failed to create Version Info File"),
			_T("VesionInfo Writing"), MB_OK);

		return;
	}

	TCHAR szVersionInfo[_MAX_PATH]={0,};
	wsprintf(szVersionInfo, _T("%s%s\r\n"), CPatchMakerConfig::GetInstance()->GetGameStatus(), szVersion);

	char szASCIIVersionInfo[_MAX_PATH]={0,};

#ifdef _UNICODE
	strcpy(szASCIIVersionInfo, Convert2Multi(szVersionInfo));
#else
	lstrcpy(szASCIIVersionInfo, szVersionInfo);
#endif

	dwReturn = WriteFile(	hFile, szASCIIVersionInfo, strlen(szASCIIVersionInfo),
							&dwBytesWritten, NULL );

	if(!dwReturn)
	{
		MessageBox(m_hUIWnd, _T("Failed to create Version Info File"),
			_T("VesionInfo Writing"), MB_OK);

		CloseHandle( hFile );

		return;
	}

	CloseHandle( hFile );
}
