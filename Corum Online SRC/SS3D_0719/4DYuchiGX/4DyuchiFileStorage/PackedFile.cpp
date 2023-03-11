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
	// �÷��׿� ���� ���� ������, �߰������� �����Ѵ�.

	// �ϴ� �ӽ� ������ ����� �����ϰ� �߰������ ���� ��ũ���͵��� ��ȸ�ϸ鼭 ���� ������ ����� ī���ϰ�
	// ���� ������� ���� �̸��� �ٲ۴�
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
	// ������ ������ �̵�..
	SetEndOfFile(m_hWorkFile);


	
	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL CPackedFile::InsertFileToPackFile(char* szFileName)
{
	// ������ �߰��Ѵ�.�߰��Ҷ� ���� �����̸��� �����ϸ� ������ ���� ����� �����.
	// �߰��Ҷ� ������ ������ ��ũ������ ������� ���� ������ ��ũ ����� ũ�� 
	// ������ ��ü�� �籸���ؾ��Ѵ�. �� ���� ���ŵ� ���ϸ���Ʈ�� �ۼ��ϰ� ���߿� �ѹ���
	// ������Ʈ �Ѵ�.

	// �ؽ��� �̹� ���� �̸��� ������ �����ϸ�..�ϴ� ����..
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

	// �ؽ����� �������� �����ϸ�....
	FSFILE_DESC*	pOldDesc;

	if (VBHSelect(m_pFileNameHash,(DWORD*)&pOldDesc,1,szSearchFileName,dwSearchFileNameLen))
	{
		// �������� �������� ����� ���� ��ũ���Ϳ� ǥ�ø� �صд�.
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
	// ���ŵ� ���� ����Ʈ�� ������ �ִٰ� �̶� �ѹ��� �����ؾߵȴ�.


	return FALSE;
}

FSFILE_POINTER* CPackedFile::PFOpenFile(char* szFileName,DWORD dwAccessMode,DWORD dwAttribute)
{
	// �޸𸮿� ���εǾ����� ������ �޸𸮿� �����Ѵ�.

	return NULL;
}
DWORD CPackedFile::PFReadFile(void* pDestBuffer,FSFILE_POINTER* pFP,DWORD dwLen)
{
	// ���ε� �޸𸮷κ��� �о���δ�.�� ���üũ�� �Ѵ�.
	return 0;
}
DWORD CPackedFile::PFWriteFile(void* pSrcBuffer,FSFILE_POINTER* pFP,DWORD dwLen)
{
	// ���ε� �޸𸮿� WRITE�Ѵ�.���� ����� �ʰ��� ���� ���� �޸𸮸� ����� ���� �Ҵ�޾� ī������ 
	// �����ؼ� write�Ѵ�.
	return 0;
}
BOOL CPackedFile::PFCloseFile(FSFILE_POINTER* pFP)
{
	// �޸��� ������ ��ũ�� ��ִ´�. ���� WRITE�ϰ��� �ϴ� ������ ���� ������� ū ��� ��ü 
	// �� ������ ��� �����ؾ��Ѵ�.
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
