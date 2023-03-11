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

	BOOL		CheckPassword(LPCSTR szPwd);							// 패스워드 확인
	BOOL		Initialize(HWND hUIWnd,CProgressCtrlX* pProgerssCtrl,CProgressCtrlX* pProgressCtrlSub);	// 엔진 초기화!

	BOOL		AddNewVersion(LPVERSION_DATA pData);					// 새로운 버전을 추가
	BOOL		RemoveLastVersion();									// 마지막 버전을 제거(Unlocked여야 함.)
	BOOL		LockLastVersion(BOOL bLock);							// 마지막 버전을 Lock/Unlock!

	BOOL		BuildLastFullVersion();									// 최신의 full버전을 제작
	BOOL		BuildLastPatchFiles(BOOL bBreak = FALSE);				// 최신의 패치 파일들을 제작
	BOOL		BuildLastPatchFilesWithSlicing(BOOL bBreak = FALSE);	// 최신의 패치 파일들을 제작(분할압축형태) 
	BOOL		ResetPatchAfterBreaking();
	
	BOOL		IsThereNoPatch();										// 기존의 버전이 존재하지 않는가?
	BOOL		LoadExistingVDF();										// 기존에 존재하는 모든 vdf 로드.
	BOOL		LoadVDF(LPCTSTR szVersion);								// 해당 버전의 vdf 로드
	void		UnloadVDFs();											// 모든 vdf 언로드.

	void		MigrateVDF();

private:
	// 초기화 관련.
	BOOL		LoadConfiguration();									// PatchMaker.ini 로드
	BOOL		CheckSystemFolderExistance();							// 엔진 시스템 폴더가 존재하는 지 확인.
	void		CreateSystemFolders();									// 패치 관련 시스템 폴더 생성
	
	// 소스 복사/제거 관련	
	BOOL		CreateVersionSourceFolder(LPCTSTR szVersion);			// ./Source에 Version Folder 생성.파일복사.
	void		DeleteVersionSourceFolder(LPCTSTR szVersion);			// ./Source에서 Version Folder 제거(파일 포함).

	// vdf 작성 관련.
	LPCTSTR		GetLatestVersionSting();								// 현재 최신 버전의 버전명.
	BOOL		IsThereSameVersion(LPCTSTR szVersion);					// 해당 버전이 존재하는가?
	void		CheckAndModifySource(LPCTSTR szPath, BOOL bOriginal);	// 특수 파일 체크, 삭제,변경,추가 담당.
	void		DeleteExclusiveFiles(LPCTSTR szPath, BOOL bOriginal);	// 소스에서 제거되어야 할 file들 제거.
	void		MaintainAutopatcher(LPCTSTR szSourcePath);				// 최신 autopatcher를 따로 관리!
	BOOL		BrowseAndMakeVDF(LPVERSION_DATA pData);					// 새로운 VDF 생성.
	CVDBrowser*	GetLastVersionVDF();									// 로드된 가장 최신의 vdf 리턴.

	// 패치용 가상 vdf작성 관련.
	CVDBrowser*	MakeVirtualLatestVDF(BOOL bExcludeOrg = FALSE);			// 패치 export를 위한 최신 버전의 가상 vdf 생성
	CVDBrowser*	MakeVirtualLatestVDF(LPCTSTR szVersion);				// 패치 export를 위한 최신 버전의 가상 vdf 생성(특정버전 기준)

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