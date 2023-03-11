#pragma once

#include "../4DyuchiGRX_Common/IFileStorage.h"
#include "typedef.h"
#include "../4DyuchiGXGFunc/global.h"

#include "PackFile.h"



/**
 * \ingroup 4DyuchiFileStorage
 *
 *
 * \par requirements
 * win98 or later\n
 * win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 2005-03-16
 *
 * \author Àüº´·Ä
 *
 * \par license
 * This code is absolutely free to use and modify. The code is provided "as is" with
 * no expressed or implied warranty. The author accepts no liability if it causes
 * any damage to your computer, causes your pet to fall ill, increases baldness
 * or makes your car start emitting strange noises when you start it up.
 * This code has no bugs, just undocumented features!
 * 
 * \todo 
 *
 * \bug 
 *
 */
class CoStorage : public I4DyuchiFileStorage
{
	DWORD				m_dwRefCount;
	BOOL				m_bEnableUsePackFile;
	VBHASH_HANDLE		m_pFileItemNameHash;
	VBHASH_HANDLE		m_pPackFileNameHash;
	STMPOOL_HANDLE		m_pFileDescPool;
	STMPOOL_HANDLE		m_pFileHandlePool;
	DWORD				m_dwClusterSize;

	DWORD				m_dwMaxFileNum;

	DWORD				m_dwMaxFileNameLen;
	DWORD				m_dwMaxInsertedFileNameLen;
	FILE_ACCESS_METHOD	m_fileAccessMethod;

	char				m_szLogFileName[_MAX_PATH];
	FILE*				m_fpLog;

	/**
	 *
	 */
	void				Cleanup();
	/**
	 *
	 * \param szFileName 
	 */
	void				WriteFileNotFoundLog(char* szFileName);
public:
    /**
     *
     * \param REFIID 
     * \param PPVOID 
     * \return 
     */
    STDMETHODIMP			QueryInterface(REFIID, PPVOID);
	/**
	 *
	 * \param ULONG 
	 * \return 
	 */
	STDMETHODIMP_(ULONG)	AddRef(void);
    /**
     *
     * \param ULONG 
     * \return 
     */
    STDMETHODIMP_(ULONG)	Release(void);

	/**
	 *
	 * \param pPackFile 
	 * \param szFileName 
	 * \param pfsHeader 
	 * \return 
	 */
	BOOL				AddFileToPackFile(CPackFile* pPackFile,char* szFileName,FSFILE_HEADER* pfsHeader);
	/**
	 *
	 * \param pDesc 
	 * \return 
	 */
	BOOL				RemoveFileFromPackFile(FSFILE_DESC* pDesc);


	/**
	 *
	 * \param szPackFileName 
	 */
	void*	__stdcall	MapPackFile(char* szPackFileName);
	/**
	 *
	 * \param pPackFileHandle 
	 */
	void	__stdcall	UnmapPackFile(void* pPackFileHandle);
	/**
	 *
	 * \param dwMaxFileNum 
	 * \param dwMaxFileHandleNumAtSameTime 
	 * \param dwMaxFileNameLen 
	 * \param accessMethod 
	 * \return 
	 */
	BOOL	__stdcall	Initialize(DWORD dwMaxFileNum,DWORD dwMaxFileHandleNumAtSameTime,DWORD dwMaxFileNameLen,FILE_ACCESS_METHOD accessMethod);
	/**
	 *
	 * \param pPackFileHandle 
	 * \return 
	 */
	DWORD	__stdcall	GetFileNum(void* pPackFileHandle);
	/**
	 *
	 * \param pPackFileHandle 
	 * \param ppInfoList 
	 * \param dwMaxNum 
	 * \return 
	 */
	DWORD	__stdcall	CreateFileInfoList(void* pPackFileHandle,FSFILE_ATOM_INFO** ppInfoList,DWORD dwMaxNum);
	/**
	 *
	 * \param pPackFileHandle 
	 * \param pInfoList 
	 */
	void	__stdcall	DeleteFileInfoList(void* pPackFileHandle,FSFILE_ATOM_INFO* pInfoList);

	/**
	 *
	 * \param szFileName 
	 * \return 
	 */
	BOOL	__stdcall	IsExistInFileStorage(char* szFileName);

	/**
	 *
	 * \param pPackFileHandle 
	 * \param dwFlag 
	 * \return 
	 */
	BOOL	__stdcall	LockPackFile(void* pPackFileHandle,DWORD dwFlag);
	/**
	 *
	 * \param pPackFileHandle 
	 * \param szFileName 
	 * \return 
	 */
	BOOL	__stdcall	InsertFileToPackFile(void* pPackFileHandle,char* szFileName);
	/**
	 *
	 * \param szFileName 
	 * \return 
	 */
	BOOL	__stdcall	DeleteFileFromPackFile(char* szFileName);
	/**
	 *
	 * \param pPackFileHandle 
	 * \param pCallBackFunc 
	 * \return 
	 */
	BOOL	__stdcall	UnlockPackFile(void* pPackFileHandle,LOAD_CALLBACK_FUNC pCallBackFunc);

	/**
	 *
	 * \param szFileName 
	 * \return 
	 */
	BOOL	__stdcall	ExtractFile(char* szFileName);
	/**
	 *
	 * \return 
	 */
	BOOL	__stdcall	ExtractAllFiles();
	/**
	 *
	 * \param pPackFileHandle 
	 * \return 
	 */
	DWORD	__stdcall	ExtractAllFilesFromPackFile(void* pPackFileHandle);

	/**
	 *
	 * \param szFileName 
	 * \param dwAccessMode 
	 */
	void*	__stdcall	FSOpenFile(char* szFileName,DWORD dwAccessMode);
	/**
	 *
	 * \param pFP 
	 * \param szFormat 
	 * \param ... 
	 * \return 
	 */
	int		__stdcall	FSScanf(void* pFP,char* szFormat, ...);
	/**
	 *
	 * \param pFP 
	 * \param pDest 
	 * \param dwLen 
	 * \return 
	 */
	DWORD	__stdcall	FSRead(void* pFP,void* pDest,DWORD dwLen);
	/**
	 *
	 * \param pFP 
	 * \param dwOffset 
	 * \param seekBase 
	 * \return 
	 */
	DWORD	__stdcall	FSSeek(void* pFP,DWORD dwOffset,FSFILE_SEEK seekBase);
	/**
	 *
	 * \param pFP 
	 * \return 
	 */
	BOOL	__stdcall	FSCloseFile(void* pFP);

	/**
	 *
	 * \param pPackFileHandle 
	 * \param pFileInfo 
	 * \return 
	 */
	BOOL	__stdcall	GetPackFileInfo(void* pPackFileHandle,FSPACK_FILE_INFO* pFileInfo);
	/**
	 *
	 * \param szFileName 
	 * \param dwFlag 
	 * \return 
	 */
	BOOL	__stdcall	BeginLogging(char* szFileName,DWORD dwFlag);
	/**
	 *
	 * \return 
	 */
	BOOL	__stdcall	EndLogging();
	


	/**
	 *
	 * \return 
	 */
	CoStorage();
	/**
	 *
	 * \return 
	 */
	~CoStorage();
	
};
//extern DWORD g_dwPackCount;
//class CPackFile;
//extern CPackFile*	g_pPack[100];

