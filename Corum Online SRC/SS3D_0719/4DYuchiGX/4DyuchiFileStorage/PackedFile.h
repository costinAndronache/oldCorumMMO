// PackedFile.h: interface for the CPackedFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKEDFILE_H__2B2CB799_1342_4849_A639_F783FF629097__INCLUDED_)
#define AFX_PACKEDFILE_H__2B2CB799_1342_4849_A639_F783FF629097__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "typedef.h"
#include "../4DyuchiGXGFunc/global.h"

class CPackedFile  
{
	BOOL				m_bCreateType;
	HANDLE				m_hWorkFile;
	DWORD				m_dwMaxFileNum;
	DWORD				m_dwClusterSize;
	char				m_szFileName[_MAX_PATH];
	DWORD				m_dwFileNameLen;
	
	STMPOOL_HANDLE		m_pFileDescPool;
	DWORD				m_dwAddFile;

	DWORD				m_dwMaxFileNameLen;
	DWORD				m_dwMaxInsertedFileNameLen;
	VBHASH_HANDLE		m_pFileNameHash;



public:
	BOOL				Initialize(DWORD dwMaxFileNum,DWORD dwMaxFileNameLen);

	BOOL				BeginCreatePackedFile(char* szFileName,DWORD dwFlag);
	BOOL				InsertFileToPackFile(char* szFileName);
	BOOL				EndCreatePackedFile();
	
	FSFILE_POINTER*		PFOpenFile(char* szFileName,DWORD dwAccessMode,DWORD dwAttribute);
	DWORD				PFReadFile(void* pDestBuffer,FSFILE_POINTER* pFP,DWORD dwLen);
	DWORD				PFWriteFile(void* pSrcBuffer,FSFILE_POINTER* pFP,DWORD dwLen);
	BOOL				PFCloseFile(FSFILE_POINTER* pFP);

	CPackedFile();
	~CPackedFile();

};

#endif // !defined(AFX_PACKEDFILE_H__2B2CB799_1342_4849_A639_F783FF629097__INCLUDED_)
