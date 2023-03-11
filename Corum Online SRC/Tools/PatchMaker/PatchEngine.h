#ifndef __PATCHENGINE_H__
#define __PATCHENGINE_H__

#pragma once

#include "Config.h"
#include "VDBrowser.h"
#include "ProgressCtrlX.h"
#include <afxtempl.h>
#include "./ZipArchive/MyZipArchive.h"

enum export_type {export_full, export_manual, export_auto};

typedef struct __EXPORT_DESC
{
	export_type	e_type;
	BOOL		bBreak;
	TCHAR		szVersionInfo[_MAX_PATH];

	__EXPORT_DESC()
	{
		memset(this, 0, sizeof(__EXPORT_DESC));
	}

} EXPORT_DESC, *LPEXPORT_DESC;


class CPatchEngine
{
	typedef CTypedPtrList<CPtrList, CVDBrowser*> PatchBrowserList;
	
public:
	
	CPatchEngine();
	~CPatchEngine();

	BOOL		CheckPassword(LPCSTR szPwd);							// �н����� Ȯ��
	BOOL		Initialize(HWND hUIWnd,CProgressCtrlX* pProgerssCtrl,CProgressCtrlX* pProgressCtrlSub);	// ���� �ʱ�ȭ!

	BOOL		AddNewVersion(LPVERSION_DATA pData);					// ���ο� ������ �߰�
	BOOL		RemoveLastVersion();									// ������ ������ ����(Unlocked���� ��.)
	BOOL		LockLastVersion(BOOL bLock);							// ������ ������ Lock/Unlock!

	BOOL		BuildLastFullVersion();									// �ֽ��� full������ ����
	BOOL		BuildLastPatchFiles(BOOL bBreak = FALSE);				// �ֽ��� ��ġ ���ϵ��� ����
	BOOL		BuildLastPatchFilesWithSlicing(BOOL bBreak = FALSE);	// �ֽ��� ��ġ ���ϵ��� ����(���Ҿ�������) 
	BOOL		ResetPatchAfterBreaking();
	
	BOOL		IsThereNoPatch();										// ������ ������ �������� �ʴ°�?
	BOOL		LoadExistingVDF();										// ������ �����ϴ� ��� vdf �ε�.
	BOOL		LoadVDF(LPCTSTR szVersion);								// �ش� ������ vdf �ε�
	void		UnloadVDFs();											// ��� vdf ��ε�.

	void		MigrateVDF();

private:
	// �ʱ�ȭ ����.
	BOOL		LoadConfiguration();									// PatchMaker.ini �ε�
	BOOL		CheckSystemFolderExistance();							// ���� �ý��� ������ �����ϴ� �� Ȯ��.
	void		CreateSystemFolders();									// ��ġ ���� �ý��� ���� ����
	
	// �ҽ� ����/���� ����	
	BOOL		CreateVersionSourceFolder(LPCTSTR szVersion);			// ./Source�� Version Folder ����.���Ϻ���.
	void		DeleteVersionSourceFolder(LPCTSTR szVersion);			// ./Source���� Version Folder ����(���� ����).

	// vdf �ۼ� ����.
	LPCTSTR		GetLatestVersionSting();								// ���� �ֽ� ������ ������.
	BOOL		IsThereSameVersion(LPCTSTR szVersion);					// �ش� ������ �����ϴ°�?
	void		CheckAndModifySource(LPCTSTR szPath, BOOL bOriginal);	// Ư�� ���� üũ, ����,����,�߰� ���.
	void		DeleteExclusiveFiles(LPCTSTR szPath, BOOL bOriginal);	// �ҽ����� ���ŵǾ�� �� file�� ����.
	void		MaintainAutopatcher(LPCTSTR szSourcePath);				// �ֽ� autopatcher�� ���� ����!
	BOOL		BrowseAndMakeVDF(LPVERSION_DATA pData);					// ���ο� VDF ����.
	CVDBrowser*	GetLastVersionVDF();									// �ε�� ���� �ֽ��� vdf ����.

	// ��ġ�� ���� vdf�ۼ� ����.
	CVDBrowser*	MakeVirtualLatestVDF(BOOL bExcludeOrg = FALSE);			// ��ġ export�� ���� �ֽ� ������ ���� vdf ����
	CVDBrowser*	MakeVirtualLatestVDF(LPCTSTR szVersion);				// ��ġ export�� ���� �ֽ� ������ ���� vdf ����(Ư������ ����)

	// zip making!	
	BOOL		__CreateZipFile(LPCTSTR szFileName, CVDBrowser* pBrowser, LPEXPORT_DESC lpExportDesc);
	BOOL		__CreateAutoPatchZipFiles(BOOL bBreak);
	BOOL		__CreateManualPatchZipFile();
	void		__MakeNewFileName(LPCTSTR szFile, LPTSTR szNewFile);
	void		__AddVersionInfoFile(LPCTSTR szVersion, LPCTSTR szPath, export_type e_type, OUT LPTSTR szNewVersionFile);
	void		__AddSelpUpdateToZip(CMyZipArchive* lpZipArchive);

	void		__MigrateVDF(LPCTSTR szVDFFile);
private:

	PatchBrowserList	m_PatchList;
	HWND				m_hUIWnd;
	
public:

	CProgressCtrlX*		m_pProgressCtrlX;
	CProgressCtrlX*		m_pProgressCtrlXSub;

};

#endif // __PATCHENGINE_H__