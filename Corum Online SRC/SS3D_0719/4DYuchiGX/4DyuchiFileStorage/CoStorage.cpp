#include "StdAfx.h"
#include "CoStorage.h"
#include "PackFile.h"
#include <crtdbg.h>




//DWORD g_dwPackCount = 0;
//CPackFile*	g_pPack[100];
extern LONG			g_lComponent;


char*	g_pMemLast = NULL;

BOOL IsSpace(int c)
{
	//0x09 ? 0x0D or 0x20
	BOOL	bResult;
	__asm 
	{
		xor		eax,eax

		

		mov		edx,dword ptr[c]

		cmp		dl,0x00
		jz		lb_true

		cmp		dl,0x20
		jz		lb_true

		cmp		dl,0x09
		jb		lb_exit

		cmp		dl,0x0d
		ja		lb_exit

lb_true:
		inc		eax
		
lb_exit:
		mov		dword ptr[bResult],eax
	}
	return bResult;

	
}

STDMETHODIMP CoStorage::QueryInterface(REFIID refiid, PPVOID ppv)
{
	if (IID_IUnknown == refiid)
		*ppv = this;
	else if (IID_4DyuchiFileStorage == refiid)
		*ppv = (I4DyuchiFileStorage*)this;
	else 
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	((IUnknown*)(*ppv))->AddRef();
	return NOERROR;
	
}
STDMETHODIMP_(ULONG) CoStorage::AddRef(void)
{
	m_dwRefCount++;
	return 0;
}
STDMETHODIMP_(ULONG) CoStorage::Release(void)
{
	m_dwRefCount--;
	if (!m_dwRefCount)
		delete this;

	return 0;
}

CoStorage::CoStorage()
{
	InterlockedIncrement( &g_lComponent);
	
	m_dwRefCount = 0;
	
	m_dwClusterSize = 8192;

	m_dwMaxFileNum = 0;

	m_dwMaxFileNameLen = 0;
	m_dwMaxInsertedFileNameLen = 0;
	
	m_fpLog = NULL;

#ifdef _DEBUG
	int	flag = _CRTDBG_ALLOC_MEM_DF |_CRTDBG_LEAK_CHECK_DF;

	_CrtSetDbgFlag(flag);
#endif
}
BOOL  CoStorage::Initialize(DWORD dwMaxFileNum,DWORD dwMaxFileHandleNumAtSameTime,DWORD dwMaxFileNameLen,FILE_ACCESS_METHOD accessMethod)
{
	m_dwMaxFileNameLen = dwMaxFileNameLen;
	m_dwMaxFileNum = dwMaxFileNum;
	m_fileAccessMethod = accessMethod;
	return TRUE;
}
BOOL  CoStorage::BeginLogging(char* szFileName,DWORD /*dwFlag*/)
{

	BOOL	bResult = FALSE;
	m_fpLog = fopen(szFileName,"wt");
	if (!m_fpLog)
		goto lb_return;

	bResult = TRUE;
lb_return:
	return bResult;

}
BOOL  CoStorage::EndLogging()
{
	BOOL		bResult = FALSE;

	if (!m_fpLog)
		goto lb_return;
	
	fclose(m_fpLog);
	bResult = TRUE;

lb_return:
	return bResult;
}
DWORD  CoStorage::GetFileNum(void* pPackFileHandle)
{
	DWORD dwNum = 0;

	// �ڵ�üũ �ѹ� ������..
	//
	//////////////////////////
	CPackFile*	pPackFile = (CPackFile*)pPackFileHandle;
	dwNum = pPackFile->GetFileNum();
	return dwNum;
}
DWORD  CoStorage::CreateFileInfoList(void* pPackFileHandle,FSFILE_ATOM_INFO** ppInfoList,DWORD dwMaxNum)
{
	DWORD	dwFileNum = 0;
	CPackFile*	pPackFile = (CPackFile*)pPackFileHandle;
	dwFileNum = pPackFile->CreateFileInfoList(ppInfoList,dwMaxNum);
	return dwFileNum;
}

void  CoStorage::DeleteFileInfoList(void* pPackFileHandle,FSFILE_ATOM_INFO* pInfoList)
{
	CPackFile*	pPackFile = (CPackFile*)pPackFileHandle;
	pPackFile->DeleteFileInfoList(pInfoList);
}
void*  CoStorage::MapPackFile(char* szPackFileName)
{
	CPackFile*	pPackFile = NULL;

	BOOL	bResult = FALSE;
	
	
	char	szSearchPackFileName[256];
	DWORD	dwSearchPackFileLen;
	memset(szSearchPackFileName,0,sizeof(szSearchPackFileName));

	dwSearchPackFileLen = GetSearchFileName(szSearchPackFileName,szPackFileName);

	std::string key(szSearchPackFileName);
	if (_packFilePerFilename.find(key) != _packFilePerFilename.end()) {
		return NULL;
	}

	/*if (VBHSelect(m_pPackFileNameHash, (DWORD*)&pPackFile, 1, szSearchPackFileName, dwSearchPackFileLen))
	{
		pPackFile = NULL;
		goto lb_return;
	}
	*/
	pPackFile = new CPackFile;
	bResult = pPackFile->Initialize(this,szPackFileName);
	pPackFile->containerKey = key;

	/*void* pHashHandle;
	pHashHandle = VBHInsert(m_pPackFileNameHash,(DWORD)pPackFile,szSearchPackFileName,dwSearchPackFileLen);
	if (!pHashHandle)
	{
		delete pPackFile;
		pPackFile = NULL;
		goto lb_return;
	}
	pPackFile->m_pSearchHandle = pHashHandle;
	*/

	_packFilePerFilename.insert({ key, pPackFile });
lb_return:
	return pPackFile;
}
void  CoStorage::UnmapPackFile(void* pPackFileHandle)
{
	CPackFile*	pPackFile = (CPackFile*)pPackFileHandle;
	
	_packFilePerFilename.erase(pPackFile->containerKey);
	//VBHDelete(m_pPackFileNameHash,pPackFile->m_pSearchHandle);
	
	delete pPackFile;
}

BOOL  CoStorage::LockPackFile(void* pPackFileHandle,DWORD dwFlag)
{
	BOOL	bResult = FALSE;

	CPackFile*	pPackFile = (CPackFile*)pPackFileHandle;
	bResult = pPackFile->BeginCreatePackFile(dwFlag);
	
lb_return:
	return bResult;
}

BOOL  CoStorage::UnlockPackFile(void* pPackFileHandle,LOAD_CALLBACK_FUNC pCallBackFunc)
{
	BOOL	bResult = FALSE;

	CPackFile*	pPackFile = (CPackFile*)pPackFileHandle;
	bResult = pPackFile->EndCreatePackFile(pCallBackFunc);
	
	
lb_return:
	return bResult;
}

BOOL  CoStorage::GetPackFileInfo(void* pPackFileHandle,FSPACK_FILE_INFO* pFileInfo)
{
	CPackFile* pPackFile = (CPackFile*)pPackFileHandle;
	pPackFile->GetInfo(pFileInfo);
	return TRUE;
}

BOOL  CoStorage::IsExistInFileStorage(char* szFileName)
{
	BOOL	bResult = FALSE;

	char	szSearchFileItemName[256];
	DWORD	dwSearchFileItemLen;
	memset(szSearchFileItemName,0,sizeof(szSearchFileItemName));


	dwSearchFileItemLen = GetSearchFileName(szSearchFileItemName,szFileName);
	std::string key(szSearchFileItemName);
	if (_filedescPerFilename.find(key) != _filedescPerFilename.end()) {
		return TRUE;
	}
	return FALSE;

	/*FSFILE_DESC* pOldDesc;
	if (VBHSelect(m_pFileItemNameHash,(DWORD*)&pOldDesc,1,szSearchFileItemName,dwSearchFileItemLen))
	{
		bResult = TRUE;
	}
lb_return:
	return bResult;
	*/
}
BOOL  CoStorage::InsertFileToPackFile(void* pPackFileHandle,char* szFileName)
{
	

	BOOL	bResult = FALSE;

	CPackFile*	pPackFile = (CPackFile*)pPackFileHandle;
	bResult = AddFileToPackFile(pPackFile,szFileName,NULL);
lb_return:
	return bResult;
}



// �̰��� ������ Ŭ�����ʿ��� ������ ȣ���ϴ� �Լ��̴�. 
// �� �Լ��� ȣ��� �������� add������ �̹� �����Ǿ��ִ�.
// ���� �̸��� ������ �����ϸ� ���������.
BOOL CoStorage::AddFileToPackFile(CPackFile* pPackFile,char* szFileName,FSFILE_HEADER* pfsHeader)
{
	BOOL	bResult = FALSE;

	char	szSearchFileItemName[256];
	DWORD	dwSearchFileItemLen;
	memset(szSearchFileItemName,0,sizeof(szSearchFileItemName));


	dwSearchFileItemLen = GetSearchFileName(szSearchFileItemName,szFileName);
	std::string key(szSearchFileItemName);

	FSFILE_DESC* pNewDesc = new FSFILE_DESC;

	if (!pNewDesc) {
		return FALSE;
	}

	memset(pNewDesc,0,sizeof(FSFILE_DESC));

	auto found = _filedescPerFilename.find(key);
	if (found != _filedescPerFilename.end()) {
		auto old = found->second;
		old->pOwnerPackFile->DeleteFileItem(old);
		_filedescPerFilename.erase(key);
		delete old;
	}

	/*FSFILE_DESC* pOldDesc;
	if (VBHSelect(m_pFileItemNameHash, (DWORD*)&pOldDesc, 1, szSearchFileItemName, dwSearchFileItemLen))
	{

		// �� ���Ͽ��� �����Ѵ�.
		pOldDesc->pOwnerPackFile->DeleteFileItem(pOldDesc);
		
		// �������� ��ũ���͸� �����Ѵ�.
		VBHDelete(m_pFileItemNameHash,pOldDesc->pHashHandle);
		delete pOldDesc;
	}
	*/

	_filedescPerFilename.insert({ key, pNewDesc });
	pNewDesc->key = key;

	// �ؽ����� �ִ´�.
	/*void* pHashHandle;
	pHashHandle = VBHInsert(m_pFileItemNameHash,(DWORD)pNewDesc,szSearchFileItemName,dwSearchFileItemLen);
	if (!pHashHandle)
	{
		memset(pNewDesc,0,sizeof(FSFILE_DESC));
		delete pNewDesc;
		pNewDesc = NULL;
		goto lb_return;
	}
	
	// �����Ͽ��� �ִ´�.
	//pNewDesc->pHashHandle = pHashHandle;
		char*	pszFileName;
	pszFileName = (char*)VBHGetKeyPtr(pNewDesc->pHashHandle);
	*/

	if (pfsHeader)
	{
		pNewDesc->bSourceFromPackFile = TRUE;
		pNewDesc->dwTotalSize = pfsHeader->dwTotalSize;
		pNewDesc->dwRealFileSize = pfsHeader->dwRealFileSize;
		pNewDesc->dwPositionInPackFile = pfsHeader->dwFileDataOffset;
	}
		
	pPackFile->AddFileItem(szFileName,szSearchFileItemName,dwSearchFileItemLen,pNewDesc);

	DWORD	dwFileNameLen;

	dwFileNameLen = lstrlen(szSearchFileItemName);
	if (m_dwMaxInsertedFileNameLen < dwFileNameLen)
		m_dwMaxInsertedFileNameLen = dwFileNameLen;

	bResult = TRUE;

lb_return:
	return bResult;
}

BOOL  CoStorage::DeleteFileFromPackFile(char* szFileName)
{
	char	szSearchFileItemName[256];
	DWORD	dwSearchFileItemLen;
	memset(szSearchFileItemName,0,sizeof(szSearchFileItemName));
	
	dwSearchFileItemLen = GetSearchFileName(szSearchFileItemName,szFileName);
	std::string key(szSearchFileItemName);

	/*FSFILE_DESC*	pDelDesc;
	if (!VBHSelect(m_pFileItemNameHash,(DWORD*)&pDelDesc,1,szSearchFileItemName,dwSearchFileItemLen))
		goto lb_return;
	
	bResult = RemoveFileFromPackFile(pDelDesc);
	
lb_return:
	return bResult;
	*/
	auto found = _filedescPerFilename.find(key);
	if (found != _filedescPerFilename.end()) {
		return RemoveFileFromPackFile(found->second);
	}
	return FALSE;
}

BOOL  CoStorage::ExtractAllFiles()
{
	/*FSFILE_DESC** ppDescList = new FSFILE_DESC * [m_dwMaxFileNum];
	
	DWORD dwNum = VBHGetAllItem(m_pFileItemNameHash,(DWORD*)ppDescList,m_dwMaxFileNum);

	for (DWORD i=0; i<dwNum; i++)
	{
		ppDescList[i]->pOwnerPackFile->ExtractFile(ppDescList[i]);
	}
	delete [] ppDescList;
	*/

	for (auto const& item : _filedescPerFilename) {
		item.second->pOwnerPackFile->ExtractFile(item.second);
	}
	return TRUE;
}

BOOL  CoStorage::ExtractFile(char* szFileName)
{
	BOOL	bResult = FALSE;

	char	szSearchFileItemName[256];
	DWORD	dwSearchFileItemLen;
	memset(szSearchFileItemName,0,sizeof(szSearchFileItemName));
	
	dwSearchFileItemLen = GetSearchFileName(szSearchFileItemName,szFileName);
	std::string key(szSearchFileItemName);

	/**FSFILE_DESC* pDesc;
	if (!VBHSelect(m_pFileItemNameHash,(DWORD*)&pDesc,1,szSearchFileItemName,dwSearchFileItemLen))
		goto lb_return;

	bResult = pDesc->pOwnerPackFile->ExtractFile(pDesc);

lb_return:
	return bResult;
	*/
	auto found = _filedescPerFilename.find(key);
	if (found != _filedescPerFilename.end()) {
		return found->second->pOwnerPackFile->ExtractFile(found->second);
	}
	return FALSE;
}

DWORD  CoStorage::ExtractAllFilesFromPackFile(void* pPackFileHandle)
{
	BOOL	bResult = FALSE;

	CPackFile*		pPackFile = (CPackFile*)pPackFileHandle;
	bResult = pPackFile->ExtractAllFiles();
	return bResult;

}

BOOL CoStorage::RemoveFileFromPackFile(FSFILE_DESC* pDelDesc)
{

	BOOL	bResult = FALSE;
	
	CPackFile*	pPackFile;

	pPackFile = pDelDesc->pOwnerPackFile;
	if (!pPackFile)
		__asm int 3

	pPackFile->DeleteFileItem(pDelDesc);
	

	// �������� ��ũ���͸� �����Ѵ�.
	//VBHDelete(m_pFileItemNameHash,pDelDesc->pHashHandle);
	_filedescPerFilename.erase(pDelDesc->key);
	delete pDelDesc;

	bResult = TRUE;


lb_return:
	return bResult;
}

void* 	CoStorage::FSOpenFile(char* szFileName,DWORD dwAccessMode)
{
	FSFILE_POINTER*	pFP = new FSFILE_POINTER;
	BOOL	bResult = FALSE;
	FILE*	fp = NULL;
	FSFILE_DESC*	pDesc = NULL;


	memset(pFP,0,sizeof(FSFILE_POINTER));

	char	szSearchFileItemName[256];
	DWORD	dwSearchFileItemLen;
	memset(szSearchFileItemName,0,sizeof(szSearchFileItemName));


	dwSearchFileItemLen = GetSearchFileName(szSearchFileItemName,szFileName);
	std::string key(szSearchFileItemName);
	
	//if (VBHSelect(m_pFileItemNameHash,(DWORD*)&pDesc,1,szSearchFileItemName,dwSearchFileItemLen))
	auto found = _filedescPerFilename.find(key);

	if (m_fileAccessMethod == FILE_ACCESS_METHOD_ONLY_FILE)
		goto lb_create_file;

	if (found != _filedescPerFilename.end())
	{
		pDesc = found->second;
		pFP->bFromPack = TRUE;
		pFP->pFSDesc = pDesc;
		if (dwAccessMode == FSFILE_ACCESSMODE_TEXT)
		{
			// �� ���Ͽ� ����ִµ� �ؽ�Ʈ ���� ������ ���..
			if (!pDesc->pEntryInMemory)
			{
#ifdef _DEBUG
				if (pDesc->dwOpenCount)
					__asm int 3
#endif

				pDesc->pEntryInMemory = new char[pDesc->dwRealFileSize + 1];
				pDesc->pOwnerPackFile->PFReadFile(pDesc->pEntryInMemory,pDesc,0,pDesc->dwRealFileSize);			
				pDesc->pEntryInMemory[pDesc->dwRealFileSize] = 0;
			}
		}
		pDesc->dwOpenCount++;
		goto lb_return;
	}

	if (m_fileAccessMethod == FILE_ACCESS_METHOD_FILE_OR_PACK)
	{
lb_create_file:
		char* pAccessText = "rt";
		char* pAccessBin = "rb";
		char*	pAccessStr;
		if (dwAccessMode == FSFILE_ACCESSMODE_TEXT)
			pAccessStr = pAccessText;
		else
			pAccessStr = pAccessBin;
		pFP->fp = fopen(szFileName,pAccessStr);
		if (!pFP->fp)
			goto lb_fail;

		pFP->bFromPack = FALSE;
		goto lb_return;
		
	}

lb_fail:
	// ������� ������ ������ ���µ� �����ߴ�.
	delete pFP;
	pFP = NULL;
	if (m_fpLog)
	{
		WriteFileNotFoundLog(szFileName);
	}

lb_return:

	return pFP;
}
void CoStorage::WriteFileNotFoundLog(char* szFileName)
{
	fprintf(m_fpLog,"File Not Found : %s \n",szFileName);
}
DWORD  CoStorage::FSRead(void* pFP,void* pDest,DWORD dwLen)
{

	FSFILE_POINTER*	pScanFP = (FSFILE_POINTER*)pFP;
	DWORD	dwReadBytes = 0;
	if (pScanFP->bFromPack)
	{
#ifdef _DEBUG
		if (!pScanFP->pFSDesc)
			__asm int 3

		if (!pScanFP->pFSDesc->pOwnerPackFile)
			__asm int 3

#endif
		dwReadBytes = pScanFP->pFSDesc->pOwnerPackFile->PFReadFile((char*)pDest,pScanFP->pFSDesc,pScanFP->dwOffset,dwLen);
		pScanFP->dwOffset += dwReadBytes;
	}
	else
	{
#ifdef _DEBUG
		if (!pScanFP->fp)
			__asm int 3
#endif
		dwReadBytes = fread(pDest,sizeof(char),dwLen,pScanFP->fp);
	}
	return dwReadBytes;
}

DWORD  CoStorage::FSSeek(void* pFP,DWORD dwOffset,FSFILE_SEEK seekBase)
{


	DWORD	dwCurOffset;
	FSFILE_POINTER*	pScanFP = (FSFILE_POINTER*)pFP;

	if (pScanFP->bFromPack)
	{
		if (dwOffset > pScanFP->pFSDesc->dwRealFileSize)
			dwOffset = pScanFP->pFSDesc->dwRealFileSize;

		if (seekBase == FSFILE_SEEK_SET)
		{
			pScanFP->dwOffset = dwOffset;
		} 
		else if (seekBase == FSFILE_SEEK_CUR)
		{
			pScanFP->dwOffset += dwOffset;
		}
		else if (seekBase == FSFILE_SEEK_END)
		{
			pScanFP->dwOffset = pScanFP->pFSDesc->dwRealFileSize - dwOffset;
		}

		if (pScanFP->dwOffset > pScanFP->pFSDesc->dwRealFileSize)
			pScanFP->dwOffset = pScanFP->pFSDesc->dwRealFileSize;


		dwCurOffset = pScanFP->dwOffset;
	}
	else
	{
#ifdef _DEBUG
		if (!pScanFP->fp)
			__asm int 3
#endif
		dwCurOffset = fseek(pScanFP->fp,dwOffset,seekBase);

	}
	return dwCurOffset;
}


int ConvertHexToBin(char* pResult4BinArray,char cHexCode)
{
	int		iCount,iResult,i;
	char	decimal;
	char	div_value;
	char	mod_value;


	iResult = -1;
	
	
	if (cHexCode >= 'a' && cHexCode <= 'z')
	{
		decimal = cHexCode - 'a' + 10;
		goto lb_convert;
	}

	if (cHexCode >= 'A' && cHexCode <= 'Z')
	{
		decimal = cHexCode - 'A' + 10;
		goto lb_convert;
	}
	if (cHexCode >= '0' && cHexCode <= '9')
	{
		decimal = cHexCode - '0';
		goto lb_convert;
	}

	goto lb_return;

lb_convert:
	pResult4BinArray[0] = 0;
	pResult4BinArray[1] = 0;
	pResult4BinArray[2] = 0;
	pResult4BinArray[3] = 0;

	iCount = 0;
	for (i=0; i<4; i++)
	{
//	while (decimal)
//	{
		div_value = decimal >> 1;
		mod_value = decimal - (div_value<<1);
		decimal = div_value;
		pResult4BinArray[iCount] = mod_value;
		iCount++;
	}
lb_return:
	return iResult;
}
int ConvertHexToInt(char* pStr)
{
	char		pBin[4];
	int	iChrNum;
	
	iChrNum = lstrlen(pStr);
	int	pow = 1;

	int			iResult = 0;
	for (int i=0; i<iChrNum; i++)
	{
		ConvertHexToBin(pBin,pStr[iChrNum-i-1]);
		for (int j=0; j<4; j++)
		{
			int	temp = (int)pBin[j];
			iResult += ( (~(temp - 1)) & pow);
			pow = pow << 1;

		}
	}
	
	return iResult;

}

////////////////////////////////////////////////////////////////////////////////////////////

int  SearchFloat(BYTE* pMem,float* pFloat)
{
#ifdef _DEBUG
	if ((int)pMem >= (int)g_pMemLast)
		__asm int 3
#endif

	BYTE*	p = pMem;

	BYTE	temp[64];
	temp[63] = 0;
	
	int		iOffset;
	DWORD	dwCount = 0;

	// ���鹮�ڰ� �ƴ� ���ڰ� ���ö����� �ɼ� �̵�
	while (IsSpace(*p))
	{
		if ((int)p >= (int)g_pMemLast)
			goto lb_return;
		p++;
	};

	if (NULL == *p)
		goto lb_return;

	while (!IsSpace(*p))
	{
		
#ifdef _DEBUG
		if (dwCount >= 63)
			__asm int 3
#endif	

		temp[dwCount] = *p;
		p++;
		dwCount++;
	}
	temp[dwCount] = 0;
	*pFloat = atof((char*)temp);

lb_return:
	iOffset = (DWORD)p - (DWORD)pMem;

	return iOffset;
}

int  SearchInt(BYTE* pMem,int* pInt)
{
#ifdef _DEBUG
	if ((int)pMem >= (int)g_pMemLast)
		__asm int 3
#endif


	BYTE*	p = pMem;

	BYTE	temp[16];
	temp[15] = 0;
	
	int		iOffset;
	DWORD	dwCount = 0;
	
	// ���鹮�ڰ� �ƴ� ���ڰ� ���ö����� �ɼ� �̵�
	while (IsSpace(*p))
	{		
		if ((int)p >= (int)g_pMemLast)
			goto lb_return;

		p++;
	};


	if (NULL == *p)
		goto lb_return;

	while (!IsSpace(*p))
	{
		
#ifdef _DEBUG
		if (dwCount >= 15)
			__asm int 3
#endif

		temp[dwCount] = *p;
		p++;
		dwCount++;
	}
	temp[dwCount] = 0;
	*pInt = atoi((char*)temp);

lb_return:
	iOffset = (DWORD)p - (DWORD)pMem;

	return iOffset;
}

int  SearchHex(BYTE* pMem,int* pHex)
{
#ifdef _DEBUG
	if ((int)pMem >= (int)g_pMemLast)
		__asm int 3
#endif

	BYTE*	p = pMem;

	BYTE	temp[9];
	temp[8] = 0;
	
	int		iOffset;
	DWORD	dwCount = 0;
	
	// ���鹮�ڰ� �ƴ� ���ڰ� ���ö����� �ɼ� �̵�
	while (IsSpace(*p))
	{
		if ((int)p >= (int)g_pMemLast)
			goto lb_return;
		p++;
	};

	if (NULL == *p)
		goto lb_return;

	while (!IsSpace(*p))
	{
		
#ifdef _DEBUG
		if (dwCount >= 8)
			__asm int 3
#endif
		temp[dwCount] = *p;
		p++;
		dwCount++;
	}
	temp[dwCount] = 0;
	*pHex = ConvertHexToInt((char*)temp);

lb_return:
	iOffset = (DWORD)p - (DWORD)pMem;

	
	return iOffset;
}
int  SearchString(BYTE* pMem,BYTE* pStr)
{

//	if ((DWORD)pMem == 0x018be009)
//		__asm int 3
#ifdef _DEBUG
	if ((int)pMem >= (int)g_pMemLast)
		__asm int 3
#endif

	int		iOffset;
	DWORD	dwCount = 0;

	BYTE*	p = pMem;
	*pStr = NULL;


	// ���鹮�ڰ� �ƴ� ���ڰ� ���ö����� �ɼ� �̵�
	while (IsSpace(*p))
	{
		if ((int)p >= (int)g_pMemLast)
			goto lb_return;
		p++;
	};

	if (NULL == *p)
		goto lb_return;

	while (!IsSpace(*p))
	{
		
#ifdef _DEBUG
		if (dwCount >= 4096)
			__asm int 3
#endif
		pStr[dwCount] = *p;
		p++;
		dwCount++;
	}
	

lb_return:
	pStr[dwCount] = 0;
	iOffset = (DWORD)p - (DWORD)pMem;
	return iOffset;
}

/////////////////////////////////////////////////////////////////////////////////////////////





/*_CRTIMP */int __cdecl SScanf(char* pMem,char* szFormat, ...)
{
	// �� �Լ����� ó�� ������ ������ 
	// %s %d %u %x %f �̴�.�̿��� ������ �������� �ʴ´�.
	int	iResult;
	va_list argptr;
	DWORD	dwArgCount = 0;

#define SearchString_INDEX		0
#define SearchFloat_INDEX		1
#define SearchInt_INDEX			2
#define SearchHex_INDEX			3

	char*		pError = "Fail to SScanf(),ó���Ұ��� �����Դϴ�.";
	void*		pSearchFunc[8];
	pSearchFunc[SearchString_INDEX] = SearchString;
	pSearchFunc[SearchFloat_INDEX] = SearchFloat;
	pSearchFunc[SearchInt_INDEX] = SearchInt;
	pSearchFunc[SearchHex_INDEX] = SearchHex;
	pSearchFunc[4] = 0;
	pSearchFunc[5] = 0;
	pSearchFunc[6] = 0;
	pSearchFunc[7] = 0;

		
	// ����° ������ �ּҸ� ���´�
	argptr = (va_list)&szFormat + _INTSIZEOF(szFormat);
	__asm
	{
		xor			ecx,ecx		
		mov			esi,dword ptr[szFormat]
		mov			edi,dword ptr[argptr]
		mov			ebx,dword ptr[pMem]
		
lb_begin_count_arg:

		mov			al,byte ptr[esi]
		or			al,al
		jz			lb_exit_count;

		cmp			al,'%'
		jnz			lb_loop_count_arg



		inc			esi
		mov			al,byte ptr[esi]	; %f �Ǵ� %d���...
		inc			ecx					; �Ķ���� ���� ����



lb_check_string:


		cmp			al,'s'
		jnz			lb_check_float

		; string �̸�..
		mov			edx,SearchString_INDEX
		jmp			lb_call_searchfunc
		

lb_check_float:
		cmp			al,'f'
		jnz			lb_check_int

		; float�� 
		mov			edx,SearchFloat_INDEX
		jmp			lb_call_searchfunc

		
lb_check_int:
		cmp			al,'d'
		jnz			lb_check_dword
		jmp			lb_detect_int

lb_check_dword:

		cmp			al,'u'
		jnz			lb_check_hex

lb_detect_int:
		; int �̸�..
		mov			edx,SearchInt_INDEX
		jmp			lb_call_searchfunc

lb_check_hex:
		cmp			al,'x'
		jnz			lb_check_invalid

		; hex �̸�..
		mov			edx,SearchHex_INDEX
		jmp			lb_call_searchfunc

lb_check_invalid:
		; ������� ������ ó���Ұ�..
		mov			eax,dword ptr[pError]
		push		eax
		call		dword ptr[OutputDebugString]
		int			3
		int			3
		int			3
		jmp			lb_return;
		
		
lb_call_searchfunc:
		mov			eax,dword ptr[edi]

		; �������� ���
		push		esi
		push		edi
		push		ebx
		push		ecx

		; �Լ� ȣ�� 


		// ����üũ
		mov			ecx,dword ptr[g_pMemLast]
		cmp			ecx,ebx
		jg			lb_valid
#ifdef _DEBUG
		// ����׸��� ���ͷ�Ʈ �ɰ�..
		int			3
#else
		jmp			lb_skip_call
#endif
		// ����üũ
lb_valid:
		push		eax
		push		ebx

		; �Լ������� �ε� 
		lea			ebx,pSearchFunc
		shl			edx,2
		add			edx,ebx
		mov			ebx,dword ptr[edx]

		call		ebx

lb_skip_call:
		; �������� ���� 
		pop			ecx
		pop			ebx
		pop			edi
		pop			esi

		add			ebx,eax				; ���ϵ� �����¸�ŭ �޸� �����͸� �̵���Ų��.
		add			edi,4

lb_loop_count_arg:
		inc			esi
		jmp			lb_begin_count_arg


lb_exit_count:
		mov			dword ptr[dwArgCount],ecx	
		sub			ebx,dword ptr[pMem]

		; �̵��� �޸� ������,�� ��� ����
		mov			dword ptr[iResult],ebx
	}
lb_return:
	return iResult;

}

int  CoStorage::FSScanf(void* pFP,char* szFormat, ...)
{



	int iResult = EOF;
	FSFILE_POINTER*	pScanFP = (FSFILE_POINTER*)pFP;

	va_list argptr;
		
	// ����° ������ �ּҸ� ���´�
	argptr = (va_list)&szFormat + _INTSIZEOF(szFormat);

	BOOL		bFromPack = pScanFP->bFromPack;
	
	void*		pScanFuncPtr;
	void*		pFilePtr;

	char*	pMemLastPos = NULL;
	char*	pMem = NULL;
	

	DWORD	dwOffset = 0;		// �������϶� �̵��ؾ��� �ɼ��� ������Ų��.
	if (bFromPack)
	{
		pMem = (char*)pScanFP->pFSDesc->pEntryInMemory + pScanFP->dwOffset;
		pMemLastPos = pScanFP->pFSDesc->pEntryInMemory + pScanFP->pFSDesc->dwRealFileSize;

		if (pMem >= pMemLastPos)
			goto lb_return;


		pScanFuncPtr = (void*)SScanf;
		pFilePtr = pMem;
		g_pMemLast = pMemLastPos;
	}
	else
	{
		pScanFuncPtr = (void*)fscanf;
		pFilePtr = pScanFP->fp;

	}
	// ���ÿ� ���� �ֱ�..
	DWORD	dwArgCount;
	__asm
	{
		xor			ecx,ecx		
		mov			esi,dword ptr[szFormat]
		
lb_begin_count_arg:
		mov			al,byte ptr[esi]
		or			al,al
		jz			lb_exit_count;

		cmp			al,'%'
		jnz			lb_loop_count_arg

		inc			ecx

lb_loop_count_arg:
		inc			esi
		jmp			lb_begin_count_arg


lb_exit_count:
		mov			dword ptr[dwArgCount],ecx	

lb_call:		
		or			ecx,ecx
		jz			lb_return;
		
		mov			esi,dword ptr[argptr]
		mov			ebx,ecx
		dec			ebx
		shl			ebx,2
		add			esi,ebx
		
		add			ebx,4+(4+4)					; �Լ�ȣ�� �� esp + ���� ����*4�ϱ� ����.

lb_loop_push_arg:
		mov			eax,dword ptr[esi]
		push		eax
		sub			esi,4

		loop		lb_loop_push_arg
	}
	// ���������� �Լ� ȣ���ϱ�
	__asm
	{
		
		mov			edx,dword ptr[szFormat]
		mov			eax,dword ptr[pFilePtr]
		
		push		edx
		push		eax
		
		//call		sscanf
		call		dword ptr[pScanFuncPtr]
		mov			dword ptr[iResult],eax	; return value
		mov			dword ptr[dwOffset],eax	; pack������ ��� eax���� ���������� �ɼ��� ����ִ�
		add			esp,ebx
	}

	// �������� ��쿡 ���������� �̵���Ű��
	if (bFromPack)
	{
		pScanFP->dwOffset += dwOffset;
		if (pScanFP->dwOffset > pScanFP->pFSDesc->dwRealFileSize)
		{
			pScanFP->dwOffset = pScanFP->pFSDesc->dwRealFileSize;
	//		iResult = EOF;
		}
	}

lb_return:


	return iResult;

}
BOOL CoStorage::FSCloseFile(void* pFP)
{


	FSFILE_POINTER*	pDelFP = (FSFILE_POINTER*)pFP;

	if (pDelFP->bFromPack)
	{
		pDelFP->pFSDesc->dwOpenCount--;
		if (!pDelFP->pFSDesc->dwOpenCount)
		{
			if (pDelFP->pFSDesc->pEntryInMemory)
			{
				delete [] pDelFP->pFSDesc->pEntryInMemory;
				pDelFP->pFSDesc->pEntryInMemory = NULL;
			}
		}
	}
	else
	{
#ifdef _DEBUG
		if (!pDelFP->fp)
			__asm int 3
#endif	
		fclose(pDelFP->fp);
	}

	delete pFP;

	return TRUE;
}

void CoStorage::Cleanup()
{
	if (m_fpLog)
	{
		fclose(m_fpLog);
		m_fpLog = NULL;
	}

	for (auto const& item : _packFilePerFilename) {
		delete item.second;
	}

	_packFilePerFilename.clear();
	_filedescPerFilename.clear();

	/*CPackFile* pPackFile[MAX_PACK_FILE_NUM];
	DWORD dwNum = VBHGetAllItem(m_pPackFileNameHash,(DWORD*)pPackFile,MAX_PACK_FILE_NUM);

	for (DWORD i=0; i<dwNum; i++)
	{
		delete pPackFile[i];
	}

	if (m_pFileItemNameHash)
	{
		VBHRelease(m_pFileItemNameHash);
		m_pFileItemNameHash = NULL;

	}
	if (m_pPackFileNameHash)
	{
		ReleaseStaticMemoryPool(m_pPackFileNameHash);
		m_pPackFileNameHash = NULL;
	}
	*/
}

CoStorage::~CoStorage()
{
#ifdef _DEBUG
	char	szDebugString[]	=	"CoStorage::~CoStorage()\n";
	OutputDebugString( szDebugString);
#endif

	Cleanup();
#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif
	InterlockedDecrement( &g_lComponent);

}