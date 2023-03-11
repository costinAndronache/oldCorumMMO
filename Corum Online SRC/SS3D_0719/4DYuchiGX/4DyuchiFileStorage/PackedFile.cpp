// PackedFile.cpp: implementation of the CPackedFile class.
//
//////////////////////////////////////////////////////////////////////

#include "PackedFile.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DWORD GetMulConst(DWORD dwSize,DWORD dwConst)
{
	DWORD dwNewSize = dwSize - (dwSize % dwConst) * dwConst + dwConst;
	return dwNewSize;
}
CPackedFile::CPackedFile()
{

}

BOOL CPackedFile::Initialize(DWORD dwMaxFileNum,DWORD dwMaxFileNameLen)
{
	m_dwMaxFileNameLen = dwMaxFileNameLen;
	m_dwMaxFileNum = dwMaxFileNum;
	m_pFileDescPool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(m_pFileDescPool,sizeof(FSFILE_DESC),m_dwMaxFileNum/16+1,m_dwMaxFileNum);

	m_pFileNameHash = VBHCreate();
	VBHInitialize(m_pFileNameHash,m_dwMaxFileNum / 4 + 4,m_dwMaxFileNameLen,m_dwMaxFileNum);
	return TRUE;
}

BOOL CPackedFile::BeginCreatePackedFile(char* szFileName,DWORD dwFlag)
{
	// 플래그에 따라 새로 만들지, 추가할지를 결정한다.

	// 일단 임시 파일을 만들어 저장하고 추가모드인 경우는 디스크립터들을 순회하면서 새로 파일을 만들어 카피하고
	// 새로 만들었을 경우는 이름만 바꾼다
	BOOL		bResult = FALSE;

	m_bCreateType;

	m_dwFileNameLen = lstrlen(szFileName);
	memset(m_szFileName,0,sizeof(m_szFileName));
	memcpy(m_szFileName,szFileName,m_dwFileNameLen);


	char		szTempFileName[_MAX_PATH];
	memset(szTempFileName,0,sizeof(szTempFileName));
	memcpy(szTempFileName,szFileName,m_dwFileNameLen);
	lstrcat(szTempFileName,"_temp_");


	m_hWorkFile = CreateFile(szTempFileName,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (INVALID_HANDLE_VALUE == m_hWorkFile)
	{

		goto lb_return;
	}
	// 파일의 끝으로 이동..
	SetEndOfFile(m_hWorkFile);


	
	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL CPackedFile::InsertFileToPackFile(char* szFileName)
{
	// 파일을 추가한다.추가할때 같은 파일이름이 존재하면 기존의 것을 지우고 덮어쓴다.
	// 추가할때 기존의 파일의 디스크에서의 사이즈보다 나중 파일의 디스크 사이즈가 크면 
	// 팩파일 전체를 재구성해야한다. 이 경우는 갱신될 파일리스트를 작성하고 나중에 한번에
	// 업데이트 한다.

	// 해쉬에 이미 같은 이름의 파일이 존재하면..일단 보류..
	//
	//
	//
	/////////////////////////////////////////////////////////////////////////////////////////////
	BOOL	bResult = FALSE;

	char	szSearchFileName[256];
	memset(szSearchFileName,0,sizeof(szSearchFileName));
	GetNameRemovePath(szSearchFileName,szFileName);
	CharToSmallASCIILen(szSearchFileName,szSearchFileName);
	DWORD	dwSearchFileNameLen;
	dwSearchFileNameLen = lstrlen(szSearchFileName);

	HANDLE hFile;
	hFile = CreateFile(szFileName,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		goto lb_fail_create_file;
	}

	
	DWORD	dwSize,dwNewSize,dwReadBytes,dwWriteBytes;
	DWORD	dwHighDWord;
	dwSize = GetFileSize(hFile,&dwHighDWord);
	dwNewSize = GetMulConst(dwSize,m_dwClusterSize);

	char*	pBuffer;
	pBuffer = new char[dwNewSize];
	if (!pBuffer)
	{
		goto lb_return;
	}
	memset(pBuffer,0,dwNewSize);
	if (!ReadFile(hFile,pBuffer,dwSize,&dwReadBytes,NULL))
	{

		goto lb_return;
	}

	DWORD dwCurrentFilePosition; 
	dwCurrentFilePosition = SetFilePointer(hFile,0,NULL,FILE_CURRENT);

	if (!WriteFile(m_hWorkFile,pBuffer,dwNewSize,&dwWriteBytes,NULL))
	{
		goto lb_return;
	}
	delete [] pBuffer;
	pBuffer = NULL;


	




	FSFILE_DESC*	pNewDesc;
	pNewDesc = NULL;

	
	pNewDesc = (FSFILE_DESC*)LALAlloc(m_pFileDescPool);
	if (!pNewDesc)
		goto lb_return;


	void*	pHashHandle;
	pHashHandle = VBHInsert(m_pFileNameHash,(DWORD)pNewDesc,szSearchFileName,dwSearchFileNameLen);
	if (!pHashHandle)
	{
		LALFree(m_pFileDescPool,pNewDesc);
		pNewDesc = NULL;
		goto lb_return;
	}

	// 해쉬에서 뒤져보고 존재하면....
	FSFILE_DESC*	pOldDesc;

	if (VBHSelect(m_pFileNameHash,(DWORD*)&pOldDesc,1,szSearchFileName,dwSearchFileNameLen))
	{
		// 기존파일 아이템은 지우기 위해 디스크립터에 표시를 해둔다.
		pOldDesc->bMustDelete = TRUE;
	}

	pNewDesc->dwFileSize = dwSize;
	pNewDesc->dwFileSizeInDisk = dwNewSize;
	pNewDesc->pHashHandle = pHashHandle;
	pNewDesc->dwPositionInPackedFile = dwCurrentFilePosition;

	
	



	DWORD	dwFileNameLen;

	dwFileNameLen = lstrlen(szFileName);
	if (m_dwMaxInsertedFileNameLen < dwFileNameLen)
		m_dwMaxInsertedFileNameLen = dwFileNameLen;

	bResult = TRUE;

lb_return:


	CloseHandle(hFile);
lb_fail_create_file:
	return FALSE;
}
/*
GLOBAL_FUNC_DLL STMPOOL_HANDLE	__stdcall	();
GLOBAL_FUNC_DLL void			__stdcall	(STMPOOL_HANDLE pool);


GLOBAL_FUNC_DLL void*			__stdcall	LALAlloc(STMPOOL_HANDLE pool);
GLOBAL_FUNC_DLL void			__stdcall	LALFree(STMPOOL_HANDLE pool,void* pMemory);
*/
BOOL CPackedFile::EndCreatePackedFile()
{
	// 갱신될 파일 리스트를 가지고 있다가 이때 한번에 갱신해야된다.


	return FALSE;
}

FSFILE_POINTER* CPackedFile::PFOpenFile(char* szFileName,DWORD dwAccessMode,DWORD dwAttribute)
{
	// 메모리에 맵핑되어있지 않으면 메모리에 맵핑한다.

	return NULL;
}
DWORD CPackedFile::PFReadFile(void* pDestBuffer,FSFILE_POINTER* pFP,DWORD dwLen)
{
	// 맵핑된 메모리로부터 읽어들인다.힙 경계체크를 한다.
	return 0;
}
DWORD CPackedFile::PFWriteFile(void* pSrcBuffer,FSFILE_POINTER* pFP,DWORD dwLen)
{
	// 맵핑된 메모리에 WRITE한다.원래 사이즈를 초과할 때는 기존 메모리를 지우고 새로 할당받아 카피한후 
	// 연장해서 write한다.
	return 0;
}
BOOL CPackedFile::PFCloseFile(FSFILE_POINTER* pFP)
{
	// 메모리의 내용을 디스크에 써넣는다. 만약 WRITE하고자 하는 파일이 원래 사이즈보다 큰 경우 전체 
	// 팩 파일을 모두 갱신해야한다.
	return FALSE;
}
CPackedFile::~CPackedFile()
{
	if (m_pFileNameHash)
	{
		VBHRelease(m_pFileNameHash);
		m_pFileNameHash = NULL;

	}
	if (m_pFileDescPool)
	{
		ReleaseStaticMemoryPool(m_pFileDescPool);
		m_pFileDescPool = NULL;
	}

}
