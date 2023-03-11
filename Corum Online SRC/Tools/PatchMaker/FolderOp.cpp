#include "stdafx.h"
#include "FolderOp.h"


static int CALLBACK MyBrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	lParam;
	
    switch(uMsg) 
	{
    case BFFM_INITIALIZED:
        ::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
        break;
	}

    return 0;
}

void GetFolderPath(LPCTSTR szDefault, OUT LPTSTR szFolder)
{
    TCHAR szPath[MAX_PATH] = {0,};
    TCHAR szDefaultFolder[MAX_PATH] = {0,};
	
	lstrcpy(szDefaultFolder, (LPCTSTR) szDefault);

    BROWSEINFO browse;
	ZeroMemory(&browse, sizeof(BROWSEINFO));

    browse.hwndOwner		= NULL;
    browse.pidlRoot			= NULL;
    browse.pszDisplayName	= (LPTSTR)szDefaultFolder;
    browse.lpszTitle		= _T("Choose Source Folder..");
    browse.ulFlags			= BIF_RETURNONLYFSDIRS;
    browse.lpfn				= MyBrowseCallbackProc;
    browse.lParam			= (LPARAM)szDefaultFolder;

    LPITEMIDLIST pidl = SHBrowseForFolder(&browse);

	if(pidl && SHGetPathFromIDList(pidl, szPath))
		lstrcpy(szFolder, szPath);
	else
		lstrcpy(szFolder, _T(""));
}

BOOL CopyFolderEx(LPCTSTR szSource, LPCTSTR szTarget)
{
	SHFILEOPSTRUCT FileOp;
	ZeroMemory(&FileOp, sizeof(SHFILEOPSTRUCT));

	TCHAR szSourceFolder[_MAX_PATH] = {0,};
	TCHAR szTargetFolder[_MAX_PATH] = {0,};

	lstrcpy(szSourceFolder, szSource);
	lstrcpy(szTargetFolder, szTarget);

	TCHAR szCaption[] = _T("Copying Source Folder.. ");

	FileOp.wFunc				= FO_COPY;
	FileOp.hwnd					= NULL;
	FileOp.pFrom				= szSourceFolder;
	FileOp.pTo					= szTargetFolder;
	FileOp.fFlags				= FOF_NOCONFIRMMKDIR|FOF_NOCONFIRMATION; 
	FileOp.fAnyOperationsAborted= TRUE;
	FileOp.lpszProgressTitle	= szCaption;
	
	return (0 == SHFileOperation(&FileOp));
}

BOOL CopyFileEx(LPCTSTR szSource, LPCTSTR szTarget)
{
	SHFILEOPSTRUCT FileOp;
	ZeroMemory(&FileOp, sizeof(SHFILEOPSTRUCT));

	TCHAR szCaption[] = _T("Copying Files..");

	FileOp.wFunc				= FO_COPY;
	FileOp.hwnd					= NULL;
	FileOp.pFrom				= szSource;
	FileOp.pTo					= szTarget;
	FileOp.fFlags				= FOF_MULTIDESTFILES|FOF_NOCONFIRMMKDIR; 
	FileOp.fAnyOperationsAborted= TRUE;
	FileOp.lpszProgressTitle	= szCaption;
	
	return (0 == SHFileOperation(&FileOp));
}

void DeleteFolderEx(LPCTSTR szFolder)
{
	SHFILEOPSTRUCT FileOp;
	ZeroMemory(&FileOp, sizeof(SHFILEOPSTRUCT));

	TCHAR szCaption[] = _T("Deleting Folder..");

	FileOp.wFunc				= FO_DELETE ;
	FileOp.hwnd					= NULL;
	FileOp.pFrom				= szFolder;
	FileOp.pTo					= NULL;
	FileOp.fFlags				= FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR; 
	FileOp.fAnyOperationsAborted= FALSE;
	FileOp.lpszProgressTitle	= szCaption;

	SHFileOperation(&FileOp);
}

void SetFileAttributeEx(LPCTSTR szPath, DWORD dwAttr)
{	
	WIN32_FIND_DATA Find;
	TCHAR szFiles[_MAX_PATH]={0,};
	wsprintf(szFiles, _T("%s\\*.*"), szPath);
	
	HANDLE hFind = FindFirstFile(szFiles, &Find);

	if(hFind == INVALID_HANDLE_VALUE) return;

	for(;;)
	{
		if(Find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(	lstrcmp(Find.cFileName, _T(".")) != 0 &&
				lstrcmp(Find.cFileName, _T("..")) != 0 )
			{
				wsprintf(szFiles, _T("%s\\%s"), szPath, Find.cFileName);
				SetFileAttributeEx(szFiles, dwAttr);			
			}
		}
		else
		{
			TCHAR szFilePath[_MAX_PATH]={0,};
			wsprintf(szFilePath, _T("%s\\%s"), szPath, Find.cFileName);
			SetFileAttributes(szFilePath, dwAttr);
		}		

		if(!FindNextFile(hFind, &Find)) break;
	}

	FindClose(hFind);
}

void SetCurrentDirectoryToFolderOfExcutable()
{
	TCHAR	tszModulePath[_MAX_PATH] = {0,};
	TCHAR	tszNewPath[_MAX_PATH] = {0,};
	TCHAR	c = _T('\\');
	LPTSTR	pLastOccur = NULL;
	size_t	nLen = 0;

	GetModuleFileName(NULL, tszModulePath, sizeof(tszModulePath)/sizeof(tszModulePath[0]));
	
	pLastOccur = _tcsrchr(tszModulePath, c);
	nLen = size_t(pLastOccur - tszModulePath + 1);
	
	_tcsncpy(tszNewPath, tszModulePath, nLen);

	SetCurrentDirectory(tszNewPath);
}

BOOL CheckFolderExistance(LPCTSTR szFolder)
{
	WIN32_FIND_DATA find;
	HANDLE	hFind = INVALID_HANDLE_VALUE;

	hFind = FindFirstFile(szFolder, &find);
	if(hFind == INVALID_HANDLE_VALUE) return FALSE;
	else FindClose(hFind);

	return TRUE;
}

void GetFileTitle(LPCTSTR szPath, LPTSTR szTitle)
{
	TCHAR	c = TEXT('\\');
	LPTSTR	pLastOccur = NULL;
	size_t	nLen = 0;
			
	pLastOccur = _tcsrchr(szPath, c);
	nLen = size_t(pLastOccur - szPath + 1);
	
	lstrcpy(szTitle, szPath + nLen);
//	_tcsncpy(szTitle, szPath, nLen);
}

void SearchAndDeleteFiles(LPCTSTR szFolder, LPCTSTR szFile)
{
	WIN32_FIND_DATA Find;
	TCHAR szFiles[_MAX_PATH]={0,};
	wsprintf(szFiles, _T("%s\\%s"), szFolder, szFile);
	
	HANDLE hFind = FindFirstFile(szFiles, &Find);

	if(hFind == INVALID_HANDLE_VALUE) return;

	for(;;)
	{
		if(Find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			continue;

		TCHAR szFilePath[_MAX_PATH]={0,};
		wsprintf(szFilePath, _T("%s\\%s"), szFolder, Find.cFileName);

		DeleteFile(szFilePath);

		if(!FindNextFile(hFind, &Find)) break;
	}

	FindClose(hFind);
}

BOOL IsThereFile(LPCTSTR szFile)
{
	WIN32_FIND_DATA Find;
	HANDLE hFind = FindFirstFile(szFile, &Find);

	if(hFind == INVALID_HANDLE_VALUE) return FALSE;

	FindClose(hFind);

	return TRUE;
}

FILETIME GetFileModTime(LPCTSTR szFilePath)
{
	FILETIME time;

	HANDLE hFile = CreateFile(szFilePath, GENERIC_READ, 
		FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile)
	{
		GetFileTime(hFile, NULL, NULL, &time);
		CloseHandle(hFile);
	}
	else
	{
		GetSystemTimeAsFileTime(&time);
	}

	return time;
}

LPCSTR Convert2Multi(LPCWSTR wszSource)
{
	static char szBuf[4096];
	memset(szBuf, 0, sizeof(szBuf));
	
	WideCharToMultiByte(CP_ACP, 
						WC_COMPOSITECHECK, 
						wszSource, 
						wcslen(wszSource), 
						szBuf, 
						4096, 
						NULL,
						NULL);
	return szBuf;
}