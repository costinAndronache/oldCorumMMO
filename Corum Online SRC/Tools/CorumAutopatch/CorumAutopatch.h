#pragma once


#include "./ZipArchive/ZipArchive.h"

#include "IFileStorage.h"
#include "Interface_RainFTP.h"

#include "CorumPatch.h"
#include "ServerList.h"


BOOL CALLBACK	Autopatch		(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK	WndProc_Button	(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
DWORD CALLBACK	Pack_progress	(DWORD dwCurCount, DWORD dwTotalCount, void* pArg);


//////////////////////////////////////////////////
//	Define Window Message
//////////////////////////////////////////////////
#define SAFETY_RANGE			WM_USER + 5000

#define WM_RAIN_FTP_NOTIFY		SAFETY_RANGE
#define WM_EXTRACT				(SAFETY_RANGE + 1)
#define WM_PACK					(SAFETY_RANGE + 2)

enum EXTRACT_MSG
{
	EX_INIT			= 0,
	EX_PROGRESS		,
	EX_COMPLETE		,
	EX_FAIL
};

enum PACK_MSG
{
	PA_INIT			= 0,
	PA_PROGRESS		,
	PA_COMPLETE		,
	PA_FAIL
};

//////////////////////////////////////////////////
//	Define Session Context
//////////////////////////////////////////////////
#define SC_CONNECT					1
#define SC_GET_CURRENT_DIRECTORY	2
#define SC_GETLIST_NOTICE			3
#define SC_DOWNLOAD_NOTICE			4
#define SC_GETLIST_ZIP				5
#define SC_DOWNLOAD_ZIP				6
#define SC_SET_CURRENT_DIRECTORY	7

//	Define Size
#define FIND_DATA_MAX					10
#define PACK_FILE_NAME_BUFFER_SIZE		(1024 * 1024)
//

#define CORUM_VERSION_INFO_FILE			"CorumVerInfo.ver"
#define CORUM_VERSION_INFO_FINE_NEW		"CorumVerInfo_New.ver"
#define VERSION_HEADER_TEXT_LENGTH		4
#define VERSION_INFO_LENGTH				8
#define VERSION_TOTAL_LENGTH			VERSION_HEADER_TEXT_LENGTH + VERSION_INFO_LENGTH

//
#define	CORUM_EXE_FILE					"CorumOnlineProject.exe"
#define SELF_UPDATE_FILE				"CorumSelfupdate.exe"

#ifdef CHINA_LOCALIZING
#define REGION_SELECT_FILE				"RegionSelect.exe"
#define REGIONINFO_FILE					"Region.ini"
#endif

char* const	g_szCorumpatchFile			= "Corumpatch.zip";
char* const	g_szSearchNotice			= "Notice_*.txt";


//////////////////////////////////////////////////
//	Global
//////////////////////////////////////////////////
extern HMODULE			g_hDllModule;
extern IRainFTP*		g_pFtp;
extern CZipArchive*		g_pZip;
extern HWND				g_hwMain;
extern HANDLE			g_hSession;
extern APP_PROGRESS		AP;

extern char				g_pCmdLine[ 1024*40];			//	넷마블에서 보낸 커맨드라인.


extern char				CorumFile[MAX_PATH];
extern char				szVersion[MAX_PATH];

extern WIN32_FIND_DATA*	pwfd;

extern HANDLE			hThread_worker;


//////////////////////////////////////////////////
//	Extract 
//////////////////////////////////////////////////
extern int				g_iExTotalFile;

//////////////////////////////////////////////////
//	File Storage
//////////////////////////////////////////////////
#define PACK_DIR_MAX			13

extern HMODULE					g_hFileStorage;
extern I4DyuchiFileStorage*		g_pFileStorage;
extern void*					g_hPackFile;
extern char						g_szCorumDir[MAX_PATH];

extern int						g_iPaTotalFile;
extern int						g_iPaCurrentNum;
extern int						g_iPaCurPack;

//////////////////////////////////////////////////
#ifdef CHINA_LOCALIZING
extern BOOL						g_bRestartRegionSelect;
#endif

//////////////////////////////////////////////////
//	Info MessageBox
void MessageBox_Info (char* szInfoText);

//	Load, UnLoad RainFTP
BOOL Initialize_Ftp();
void UnInitialize_Ftp();

//	
BOOL Initialize_PACK();
void UnInitialize_PACK();


//	Read/Write VerInfo File
BOOL GetVersionInfoFile(char* lpszVerInfoFile, char* lpszVerInfo);
BOOL UpdateVersionInfoFile( char* lpszOldVerInfoFile, char* lpszNewVerInfoFile);

//	Create Process
BOOL RunCorum();
BOOL SelfUpdate();

#ifdef CHINA_LOCALIZING
BOOL RegionSelectAgain();
#endif

//	Check Notice Version.
BOOL CheckNewNotice(char* pszNewNotice);

//	for GDI
void ClearStatusBoard( DC_SET* pDCSet);
void UpdateAP( DC_SET* pDCSet, APP_PROGRESS ap);
void UpdateEI( DC_SET* pDCSet, ERROR_INFO ei);
BOOL Init_Notice( DC_SET* pDCSet);


//	worker thread
UINT __stdcall Thread_worker( PVOID pvParam);

//	extract
void	Extract();

//	pack
void	PackFile();
DWORD	GetPackInfo( char** ppCurBuffer, char* szPackDir, char* szPackFile);
BOOL	AddtoPack( char** ppCurBuffer, char* szPackDir, char* szPackFile, DWORD dwPackNum);