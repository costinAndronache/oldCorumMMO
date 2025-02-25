// PackedFile.h: interface for the CPackFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKEDFILE_H__2B2CB799_1342_4849_A639_F783FF629097__INCLUDED_)
#define AFX_PACKEDFILE_H__2B2CB799_1342_4849_A639_F783FF629097__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "typedef.h"
#include "../4DyuchiGXGFunc/global.h"
#include <string>

class	CoStorage;

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
class CPackFile  
{
	friend class CoStorage;
	CoStorage*			m_pStorage;
	BOOL				m_bCreateType;
	DWORD				m_dwMaxFileNum;
	char				m_szFileName[_MAX_PATH];
	DWORD				m_dwFileNameLen;
	FSFILE_DESC*		m_pfsHead;
	DWORD				m_dwFileItemNum;

	HANDLE				m_hOriginalFile;
	BOOL				m_bExistOriginalFile;

	void*				m_pSearchHandle;
	std::string			containerKey;
	BOOL				m_bMustRebuild;
	DWORD				m_dwCurrentVersion;


	/**
	 *
	 */
	void				Cleanup();
	/**
	 *
	 */
	void				InitializeMembers();

public:
	/**
	 *
	 * \param pPackFileInfo 
	 */
	void				GetInfo(FSPACK_FILE_INFO* pPackFileInfo);
	/**
	 *
	 * \param pStorage 
	 * \param pFileNameHash 
	 * \param szPackFileName 
	 * \return 
	 */
	BOOL				Initialize(CoStorage* pStorage,char* szPackFileName);
	/**
	 *
	 * \return 
	 */
	DWORD				GetFileNum() {return m_dwFileItemNum;}
	/**
	 *
	 * \param ppInfoList 
	 * \param dwMaxNum 
	 * \return 
	 */
	DWORD				CreateFileInfoList(FSFILE_ATOM_INFO** ppInfoList,DWORD dwMaxNum);
	/**
	 *
	 * \param pInfoList 
	 */
	void				DeleteFileInfoList(FSFILE_ATOM_INFO* pInfoList);
	/**
	 *
	 * \param dwFlag 
	 * \return 
	 */
	BOOL				BeginCreatePackFile(DWORD dwFlag);
	/**
	 *
	 * \param szFileName 
	 * \param szSearchFileItemName 
	 * \param dwSearchFileItemLen 
	 * \param pNewDesc 
	 * \return 
	 */
	BOOL				AddFileItem(char* szFileName,char* szSearchFileItemName,DWORD dwSearchFileItemLen,FSFILE_DESC* pNewDesc);
	/**
	 *
	 * \param pfsDesc 
	 * \return 
	 */
	BOOL				ExtractFile(FSFILE_DESC* pfsDesc);
	/**
	 *
	 * \return 
	 */
	DWORD				ExtractAllFiles();
	/**
	 *
	 * \param pfsDesc 
	 * \return 
	 */
	BOOL				DeleteFileItem(FSFILE_DESC* pfsDesc);
	/**
	 *
	 * \param pCallBackFunc 
	 * \return 
	 */
	BOOL				EndCreatePackFile(LOAD_CALLBACK_FUNC pCallBackFunc);
	
	/**
	 *
	 * \param pDestBuffer 
	 * \param pDesc 
	 * \param dwOffset 
	 * \param dwLen 
	 * \return 
	 */
	DWORD				PFReadFile(char* pDestBuffer,FSFILE_DESC* pDesc,DWORD dwOffset,DWORD dwLen);

	/**
	 *
	 * \return 
	 */
	CPackFile();
	/**
	 *
	 * \return 
	 */
	~CPackFile();
};

/**
 *
 * \param szDest 
 * \param szSrc 
 * \return 
 */
DWORD GetSearchFileName(char* szDest,char* szSrc);

#endif // !defined(AFX_PACKEDFILE_H__2B2CB799_1342_4849_A639_F783FF629097__INCLUDED_)
