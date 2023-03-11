// MyZipArchive.h: interface for the CMyZipArchive class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYZIPARCHIVE_H__A9508A06_816D_4CCC_B26E_46F22CA39149__INCLUDED_)
#define AFX_MYZIPARCHIVE_H__A9508A06_816D_4CCC_B26E_46F22CA39149__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ZipArchive.h"

class CMyZipArchive : public CZipArchive  
{
public:
	bool	AddNewFile( LPCTSTR lpszFilePath, LPCTSTR lpszCloneFilePath, FILETIME ftFileTime, DWORD dwFileAttr, int iLevel = -1, ZIPCALLBACKFUN pCallback = NULL, void* pUserData = NULL, unsigned long nBufSize = 65535);
	bool	OpenNewFile( FILETIME ftFileTime, DWORD dwFileAttr, CZipFileHeader& header, int iLevel = Z_DEFAULT_COMPRESSION);

public:
	CMyZipArchive();
	virtual ~CMyZipArchive();

};

#endif // !defined(AFX_MYZIPARCHIVE_H__A9508A06_816D_4CCC_B26E_46F22CA39149__INCLUDED_)
