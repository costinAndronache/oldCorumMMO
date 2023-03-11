#ifndef __VDFFILE_H__
#define __VDFFILE_H__

#pragma once

#define MAX_VERSION_STRING		8
#define MAX_DESCRIPTION_STRING	255

// ��ġ ������ ������ ���� ����ü.
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

// EXE, DLL�� �������� ���� ����ü.
typedef struct __VERSION
{
	WORD wdMS1;
	WORD wdMS2;
	WORD wdLS1;
	WORD wdLS2;

#ifndef __NO_COMPARISION
	int Compare(__VERSION Version) // ���� ���� ũ�� 1.
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


// VDF�� ��ϵ� �� ���� ������ ����.
typedef struct __VDFNode
{
	TCHAR				szFileDest[_MAX_PATH];	// �� ������ �����ϴ� ��¥ ��ġ.	����ּҸ� ����!!
	TCHAR				szFilePath[_MAX_PATH];	// ���� ������ �����ϴ� path.					

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

	// �а� ���� �� �ʿ��� �ѵ�.
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