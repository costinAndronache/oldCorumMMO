#include "stdafx.h"
#include "VDBrowser.h"
#include "VDFNodePool.h"
#include "Config.h"


CVDBrowser::CVDBrowser()
{

}

CVDBrowser::~CVDBrowser()
{
	Clear();
}

void CVDBrowser::Clear()
{
	CString strKey;
	LPVDFNode pNode = NULL;

	if(m_MemType == from_pool)
	{
		POSITION pos = m_Map.GetStartPosition();
		while(pos)
		{
			m_Map.GetNextAssoc(pos, strKey, (LPVOID&)pNode);

			ReleaseVDFNode(pNode);
		}

		m_Map.RemoveAll();
	}
}

void CVDBrowser::InitBrowser(CVDBrowser* pBrowser)
{
	TCHAR szKey[_MAX_PATH];
	CString strKey;
	LPVDFNode pNode = NULL, pNewNode = NULL;

	CMapStringToPtr& Map = pBrowser->GetMap();
	
	POSITION pos = Map.GetStartPosition();
	while(pos)
	{
		Map.GetNextAssoc(pos, strKey, (LPVOID&)pNode);

		ZeroMemory(szKey, sizeof(szKey));
		lstrcpy(szKey, (LPCTSTR)strKey);
		CharLower(szKey);

		// 기존에 존재하는 경우 일단 제거하고 새로 할당-_-
		if(m_Map.Lookup(szKey, (LPVOID&)pNewNode))
		{
		//	m_Map.RemoveKey(strKey);
			ReleaseVDFNode(pNode);
		}

		pNewNode = AcquireVDFNode();					
		memcpy(pNewNode, pNode, sizeof(VDFNode));
		
		strKey.MakeLower();
		m_Map.SetAt(szKey, pNewNode);
	}

	m_MemType = from_pool;
}

void CVDBrowser::InitBrowser(LPCTSTR szFolder)
{
	__BrowseFolder(szFolder, szFolder);
	m_MemType = from_pool;
}

void CVDBrowser::__BrowseFolder(LPCTSTR szStartFolder, LPCTSTR szFolder)
{
	CString strFile;
	CFileFind finder;
	TCHAR szSearch[_MAX_PATH]={0,};
	TCHAR szLowerName[_MAX_PATH]={0,};
	TCHAR szRelativePath[_MAX_PATH]={0,};
	TCHAR szTemp[_MAX_PATH]={0,};

	wsprintf(szSearch, _T("%s\\*.*"), szFolder);

	int nSourceFolderLen = lstrlen(CPatchMakerConfig::GetInstance()->GetSourceFolderFullPath());

	BOOL bWorking = finder.FindFile(szSearch);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		
		if(finder.IsDirectory())	
		{
			if(!finder.IsDots())
				__BrowseFolder(szStartFolder, finder.GetFilePath());
		}
		else
		{
			strFile = finder.GetFilePath();
			lstrcpy(szTemp, strFile);
			wsprintf(szRelativePath, _T(".%s"), szTemp + nSourceFolderLen);

			VDFNode* pNode = AcquireVDFNode();
			ZeroMemory(pNode, sizeof(VDFNode));
			
			// --
			// 비교의 크기를 줄이기 위해.. 상대 경로 사용을 고려....
			lstrcpy(pNode->szFilePath, szRelativePath);

			// 완전한 목적지의 상대 경로...
			GetDestinationFromPath(szStartFolder, strFile, pNode->szFileDest);
			
			finder.GetLastWriteTime(&pNode->ModTime);
			
			if(	(strFile.Right(3).CompareNoCase(_T("exe")) == 0) ||
				(strFile.Right(3).CompareNoCase(_T("dll")) == 0) )
				GetExcutiveFileVersion(strFile, &pNode->Version);

			lstrcpy(szLowerName, pNode->szFileDest);
			m_Map.SetAt(CharLower(szLowerName), pNode);
		}
	}
}

BOOL CVDBrowser::InitBrowserFromVDF(LPCTSTR szFile)
{
	if(!OpenVDFFile(szFile))
	{
		MessageBox(NULL, _T("Failed to open VDF File"), _T("Open VDF"), MB_OK|MB_ICONERROR);
		return FALSE;
	}

	GetVDFNodeMap(m_Map);

	m_MemType = from_vdf;

	return TRUE;
}

CMapStringToPtr& CVDBrowser::GetMap()
{
	return m_Map;
}

BOOL CVDBrowser::GetExcutiveFileVersion(LPCTSTR szFile, LPEXECUTIVE_VERSION lpVersion)
{
	DWORD dwSize = 0;
	LPBYTE pBlock = NULL;
	BOOL bRet = FALSE;
	VS_FIXEDFILEINFO* pvsInfo = NULL;
	UINT uLen = 0;

	__try
	{	
		memset(lpVersion, 0, sizeof(EXECUTIVE_VERSION));

		dwSize = GetFileVersionInfoSize(szFile, 0);
		if(dwSize == 0) __leave;

		pBlock = new BYTE [dwSize+1];
		if(!pBlock) __leave;

		if(!GetFileVersionInfo(szFile, 0, dwSize, pBlock)) __leave;

		if(!VerQueryValue(pBlock, _T("\\"), (void**)&pvsInfo, &uLen)) __leave;

		lpVersion->wdMS1 = WORD(pvsInfo->dwFileVersionMS / 65536);
		lpVersion->wdMS2 = WORD(pvsInfo->dwFileVersionMS % 65536);
		lpVersion->wdLS1 = WORD(pvsInfo->dwFileVersionLS / 65536);
		lpVersion->wdLS2 = WORD(pvsInfo->dwFileVersionLS % 65536);

		bRet = TRUE;
		__leave;
	}

	__finally
	{
		if(pBlock) delete [] pBlock;

		return bRet;
	}
}

inline void CVDBrowser::GetDestinationFromPath(LPCTSTR szRoot, LPCTSTR szPath, LPTSTR szDesination)
{
	swprintf(szDesination, _T(".%s"), szPath + lstrlen(szRoot));
}
