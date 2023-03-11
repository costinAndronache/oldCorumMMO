#include "stdafx.h"
#include "VDFFile.h"


CVDFFile::CVDFFile()
{
	m_hFile = INVALID_HANDLE_VALUE;
	m_hMapFile = 0;
	m_lpMapAddress = 0;
}

CVDFFile::~CVDFFile()
{
	CloseVDFFile();
}

void CVDFFile::CloseVDFFile()
{
	if(m_lpMapAddress != NULL)			{ FlushViewOfFile(m_lpMapAddress, 0); UnmapViewOfFile(m_lpMapAddress); m_lpMapAddress = NULL; }
	if(m_hMapFile != NULL)				{ CloseHandle(m_hMapFile);  m_hMapFile = NULL; }
	if(m_hFile != INVALID_HANDLE_VALUE) { CloseHandle(m_hFile);  m_hFile = INVALID_HANDLE_VALUE; }
}

BOOL CVDFFile::OpenVDFFile(LPCTSTR szFile)
{
	BOOL bRet = FALSE;
	CloseVDFFile();

	__try
	{
		m_hFile = CreateFile(	szFile, 
								GENERIC_READ|GENERIC_WRITE, 
								FILE_SHARE_READ, 
								NULL, 
								OPEN_EXISTING, 
								FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,
								NULL);

		if(INVALID_HANDLE_VALUE == m_hFile) __leave;

		m_hMapFile = CreateFileMapping(	m_hFile,		 // Current file handle. 
										NULL,            // Default security. 
										PAGE_READWRITE,  // Read/write permission. 
										0,               // Max. object size. 
										0,               // Size of hFile. 
										NULL);           // Name of mapping object. 

		if(NULL == m_hMapFile) __leave;

		m_lpMapAddress = (LPBYTE)MapViewOfFile(	m_hMapFile,	// Handle to mapping object. 
										FILE_MAP_ALL_ACCESS,// Read/write permission 
										0,                  // Max. object size. 
										0,                  // Size of hFile. 
										0);                 // Map entire file. 
 
		if(NULL == m_lpMapAddress) __leave;
		
		bRet = TRUE;
	}

	__finally
	{
		if(!bRet) CloseVDFFile();
		
		return bRet;
	}
}

DWORD CVDFFile::GetVDFNodeCount()
{
	ASSERT(m_lpMapAddress != NULL);

	LPVDFHeader pHeader = (LPVDFHeader)m_lpMapAddress;
	return pHeader->dwVDFNodeCount;
}

LPVERSION_DATA CVDFFile::GetPatchVersionData()
{
	ASSERT(m_lpMapAddress != NULL);

	LPVDFHeader pHeader = (LPVDFHeader)m_lpMapAddress;
	return &pHeader->VersionData;
}

LPVDFNode CVDFFile::GetVDFNode(DWORD dwIndex)
{
	ASSERT(dwIndex < GetVDFNodeCount());

	LPVDFNode pNodeArray = LPVDFNode(m_lpMapAddress + sizeof(VDFHeader));
	return (pNodeArray + dwIndex);
}

void CVDFFile::GetVDFNodeMap(CMapStringToPtr& Map)
{
	ASSERT(m_lpMapAddress != NULL);

	TCHAR szLower[_MAX_PATH]={0,};
	DWORD dwCount = GetVDFNodeCount();
	LPVDFNode pNodeArray = LPVDFNode(m_lpMapAddress + sizeof(VDFHeader));

	Map.RemoveAll();
	for(DWORD i = 0; i < dwCount; i++)
	{
		lstrcpy(szLower, pNodeArray[i].szFileDest);
		Map.SetAt(CharLower(szLower), &pNodeArray[i]);
	}
}

BOOL CVDFFile::IsLocked()
{
	ASSERT(m_lpMapAddress != NULL);

	LPVDFHeader pHeader = (LPVDFHeader)m_lpMapAddress;
	return pHeader->VersionData.bLocked;
}

void CVDFFile::SetLock(BOOL bLock)
{
	ASSERT(m_lpMapAddress != NULL);
	
	LPVDFHeader pHeader = (LPVDFHeader)m_lpMapAddress;
	pHeader->VersionData.bLocked = bLock;
}

BOOL CVDFFile::SaveVDFFile(LPCTSTR szFile, CMapStringToPtr& Map, LPVERSION_DATA lpVerData, BOOL bLocked)
{
	BOOL bRet = FALSE;
	CloseVDFFile();
	CString strKey;
	LPVDFNode pNode =0;
	int i = 0;
	LPBYTE pPoint =0;
	VDFHeader header;
	POSITION pos = 0;

	m_hFile = CreateFile(	szFile, 
							GENERIC_READ|GENERIC_WRITE, 
							FILE_SHARE_READ, 
							NULL, 
							CREATE_ALWAYS, 
							FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,
							NULL);

	if(INVALID_HANDLE_VALUE == m_hFile) goto lb_fin;

	m_hMapFile = CreateFileMapping(	m_hFile,		 // Current file handle. 
									NULL,            // Default security. 
									PAGE_READWRITE,  // Read/write permission. 
									0,               // Max. object size. 
									Map.GetCount() * sizeof(VDFNode) + sizeof(VDFHeader),               // Size of hFile. 
									NULL);           // Name of mapping object. 

	if(NULL == m_hMapFile) goto lb_fin;

	m_lpMapAddress = (LPBYTE)MapViewOfFile(	m_hMapFile,			// Handle to mapping object. 
									FILE_MAP_ALL_ACCESS,// Read/write permission 
									0,                  // Max. object size. 
									0,                  // Size of hFile. 
									0);                 // Map entire file. 

	if(NULL == m_lpMapAddress) goto lb_fin;

	// ¾²±â!!
	header.dwVDFNodeCount		= Map.GetCount();
	header.VersionData			= *lpVerData;
	header.VersionData.bLocked	= bLocked;

	pPoint = m_lpMapAddress;
	memcpy(pPoint, &header, sizeof(header));
	pPoint += sizeof(header);

	pos = Map.GetStartPosition();
	while(pos)
	{
		Map.GetNextAssoc(pos, strKey, (LPVOID&)pNode);
		memcpy(pPoint, pNode, sizeof(VDFNode));
		pPoint += sizeof(VDFNode);
	}
	
	bRet = TRUE;

lb_fin:
	if(!bRet) CloseVDFFile();
	
	return bRet;
}


#ifdef _DEBUG


void MapDataPrint(CMapStringToPtr& Map, LPCSTR szFile)
{
	POSITION pos;
	CString strKey;
	VDFNode* pNode;

	FILE* pf = fopen(szFile, "w");

	pos = Map.GetStartPosition();
	while(pos)
	{
		Map.GetNextAssoc(pos, strKey, (LPVOID&)pNode);
		
		fwrite(Convert2Multi(pNode->szFileDest), strlen(Convert2Multi(pNode->szFileDest))+1, 1, pf);
		fwrite("\n", 1, 1, pf);
		fwrite(Convert2Multi(pNode->szFilePath), strlen(Convert2Multi(pNode->szFilePath))+1, 1, pf);
		fwrite("\n\n", 2, 1, pf);
	}

	fclose(pf);
}
#endif // #ifdef _DEBUG