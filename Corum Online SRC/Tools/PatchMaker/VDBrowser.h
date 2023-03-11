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

	CMapStringToPtr		m_Map; // ����¡�� ����� ����.
	Memory_type			m_MemType; // vdf���� memmap����. pool���� �Ҵ��Ѱ�����.
};

#endif // __VDBROWSER_H__