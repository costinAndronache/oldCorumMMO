#ifndef __VDFFILE_H__
#define __VDFFILE_H__

#pragma once

#define MAX_VERSION_STRING		8
#define MAX_DESCRIPTION_STRING	255

// 패치 버전의 정보를 담을 구조체.
typedef struct __VER_DATA
{
	TCHAR		szVersion[MAX_VERSION_STRING+1];
	SYSTEMTIME	timeBuilt;
	TCHAR		szDescription[MAX_DESCRIPTION_STRING+1];
	BOOL		bLocked;

	__VER_DATA()
	{
		memset(this, 0, sizeof(__VER_DATA));
	}
} VERSION_DATA, *LPVERSION_DATA;

// EXE, DLL의 버전정보 담을 구조체.
typedef struct __VERSION
{
	WORD wdMS1;
	WORD wdMS2;
	WORD wdLS1;
	WORD wdLS2;

#ifndef __NO_COMPARISION
	int Compare(__VERSION Version) // 가진 값이 크면 1.
	{
		if		(wdMS1 > Version.wdMS1) return 1;
		else if	(wdMS1 < Version.wdMS1) return -1;
		else 
		{
			if		(wdMS2 > Version.wdMS2) return 1;
			else if	(wdMS2 < Version.wdMS2) return -1;
			else
			{
				if(wdLS1 > Version.wdLS1) return 1;
				else if(wdLS1 < Version.wdLS1) return -1;
				else
				{
					if(wdLS2 > Version.wdLS2) return 1;
					else if(wdLS2 < Version.wdLS2) return -1;
					else return 0;
				}
			}
		}
	}
#endif // __NO_COMPARISION

} EXECUTIVE_VERSION, *LPEXECUTIVE_VERSION;


// VDF에 기록될 각 파일 버전의 정보.
typedef struct __VDFNode
{
	TCHAR				szFileDest[_MAX_PATH];	// 이 파일이 목적하는 진짜 위치.	상대주소를 저장!!
	TCHAR				szFilePath[_MAX_PATH];	// 실제 파일이 존재하는 path.					

	FILETIME			ModTime;
	EXECUTIVE_VERSION	Version;

	__VDFNode()
	{
		ZeroMemory(this, sizeof(__VDFNode));
	}

#ifndef __NO_COMPARISION
	int Compare(__VDFNode* node)
	{
		int iRet = Version.Compare(node->Version);
		if(iRet != 0) return iRet;
		return CompareFileTime(&ModTime,&node->ModTime);
	}
#endif // __NO_COMPARISION

} VDFNode, *LPVDFNode;

typedef struct __VDFHeader
{
	VERSION_DATA		VersionData;
	DWORD				dwVDFNodeCount;
	
} VDFHeader, *LPVDFHeader;


class CVDFFile
{
public:
	CVDFFile();
	~CVDFFile();

	// 읽고 쓰는 데 필요한 넘들.
	void			CloseVDFFile();
	BOOL			OpenVDFFile(LPCTSTR szFile);
	LPVERSION_DATA	GetPatchVersionData();
	DWORD			GetVDFNodeCount();
	LPVDFNode		GetVDFNode(DWORD dwIndex);
	void			GetVDFNodeMap(CMapStringToPtr& Map); 
	BOOL			IsLocked();
	void			SetLock(BOOL bLock = FALSE);
	BOOL			SaveVDFFile(LPCTSTR szFile, CMapStringToPtr& Map, LPVERSION_DATA lpVerData, BOOL bLocked = FALSE);

private:
	HANDLE	m_hFile;
	HANDLE	m_hMapFile;
	LPBYTE	m_lpMapAddress;
};


#ifdef _DEBUG
LPCSTR  Convert2Multi(LPCWSTR wszSource);
void MapDataPrint(CMapStringToPtr& Map, LPCSTR szFile);
#endif

#endif // __VDFFILE_H__