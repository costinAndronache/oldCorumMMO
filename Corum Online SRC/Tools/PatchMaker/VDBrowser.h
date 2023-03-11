#ifndef __VDBROWSER_H__
#define __VDBROWSER_H__

#pragma once

#include "VDFFile.h"

enum Memory_type {from_pool, from_vdf};

class CVDBrowser : public CVDFFile
{
public:
	CVDBrowser();
	~CVDBrowser();
	
	void	Clear();

	void	InitBrowser(CVDBrowser* pBrowser);
	void	InitBrowser(LPCTSTR szFolder);
	BOOL	InitBrowserFromVDF(LPCTSTR szFile);

	CMapStringToPtr& GetMap();

private:
	void	__BrowseFolder(LPCTSTR szStartFolder, LPCTSTR szFolder);
	BOOL	GetExcutiveFileVersion(LPCTSTR szFile, LPEXECUTIVE_VERSION lpVersion);
	void	GetDestinationFromPath(LPCTSTR szRoot, LPCTSTR szPath, LPTSTR szDesination);

	CMapStringToPtr		m_Map; // 브라우징한 결과를 저장.
	Memory_type			m_MemType; // vdf에서 memmap인지. pool에서 할당한것인지.
};

#endif // __VDBROWSER_H__