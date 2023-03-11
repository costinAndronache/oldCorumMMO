#include "stdafx.h"
#include "NewSerial.h"


#include "MainFrame.h"
#include "ChildFrame.h"
#include "GuardianFrame.h"
#include "CopyItemFrame.h"
#include "CopyItemDeleteFrame.h"
#include "GuildFrame.h"
#include "Doc.h"
#include "MyView.h"
#include "GuardianView.h"
#include "CopyItemView.h"
#include "CopyItemDelete.h"
#include "GuildInitView.h"
#include "LoginDlg.h"
#include "AutoPatch.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CApp, CWinApp)
	//{{AFX_MSG_MAP(CApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


CApp				theApp;
IOleDBThread*		g_pDb									= NULL;
char				g_CORUM_DB_NAME		[CURRENT_DB_NUM][32];		
char				g_CORUM_DB_ACCOUNT	[CURRENT_DB_NUM][32];		
char				g_CORUM_DB_PASSWORD	[CURRENT_DB_NUM][32];	
char				g_CORUM_DB_ADDRESS	[CURRENT_DB_NUM][32];
char				g_CORUM_TDB_NAME	[32];		
char				g_CORUM_TDB_ACCOUNT	[32];		
char				g_CORUM_TDB_PASSWORD[32];	
char				g_CORUM_TDB_ADDRESS	[32];
BYTE				g_CORUM_GAME_DB		[CURRENT_DB_NUM]	= {0,};
BYTE				g_CORUM_TOTAL_DB						= 0;
char				g_szExePath			[MAX_PATH]			= {0,};	
STMPOOL_HANDLE		g_pGuildOfflinePool;	
STMPOOL_HANDLE		g_pGuildTablePool;
STMPOOL_HANDLE		g_pGuildWarPool;


//------------------------------------------------------------------------------
//	DESCRIPTION	:	- OleDbThread.dll 내부의 메세지나 오류메세지를 출력받을 외부 함수 포인터
//------------------------------------------------------------------------------
inline void DisplayDBMessage(char* szMsg)
{
	char szDBMsg[0xff] = {0,};
	sprintf(szDBMsg,"[Query Error Msg For DB] %s",szMsg);
	MessageBox(NULL,szDBMsg,NULL,NULL);
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
CApp::CApp()
{
	m_dwDocCount	= 0;
	m_dwDocCount2	= 0;
	m_dwDocCount3	= 0;
	m_dwDocCount4	= 0;
	memset(m_szLocalAddress,0,sizeof(m_szLocalAddress));
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
DWORD CApp::GetVersion()
{
	TCHAR execFilename[_MAX_PATH];
	VERIFY(::GetModuleFileName(NULL, execFilename, _MAX_PATH));
	
	BYTE		*block;
	DWORD FAR	*translation;
	DWORD FAR	*buffer;
	DWORD		handle;
	UINT		bytes;
	TCHAR		*lpszFileName = execFilename;
	TCHAR		name[512];
	CString		szString;
	DWORD		dwCurVersion = 0;
	DWORD		dwNewVersion = 0; 
	
	// 버전 정보의 실제 크기를 가져온다.
	bytes = (UINT)::GetFileVersionInfoSize( lpszFileName, &handle);
	
	if(bytes)
	{
		block = new BYTE[bytes];
		
		// 버전 정보에 대한 실제 블록을 가져온다.
		if( ::GetFileVersionInfo( lpszFileName, handle, bytes, block) ) 
		{
			if( ::VerQueryValue( block, "\\VarFileInfo\\Translation", (LPVOID *)&translation,(UINT FAR *)&bytes) )
			{
				// 버전 정보
				wsprintf(name,"\\StringFileInfo\\%04x%04x\\FileVersion",LOWORD(*translation), HIWORD(*translation) );
				
				if( ::VerQueryValue(block, name, (LPVOID *)&buffer, (UINT FAR *)&bytes) ) 
				{
					szString.Format("%s", (LPCTSTR)buffer );
				}
				else
				{
					MessageBox(NULL,"Version Info not summon","Version Error",MB_OK);
					PostQuitMessage(0);
				}
			}
			else 
			{
				MessageBox(NULL,"Translation Type not summon","Version Error",MB_OK);
				PostQuitMessage(0);
			}
		}
		else 
		{
			MessageBox(NULL,"FileVersion not summon","Version Error",MB_OK);
			PostQuitMessage(0);
		}
		
		delete [] block;
	}
	else 
	{
		MessageBox(NULL,"FileVersionInfo size not summon","Version Error",MB_OK);
		PostQuitMessage(0);
	}
	
	char* pToken1 = strtok( szString.GetBuffer(szString.GetLength()), "," );
	char* pToken2 = strtok( NULL,  "," );
	char* pToken3 = strtok( NULL,  "," );
	char* pToken4 = strtok( NULL,  "," );
	
	szString.Format("%2d%02d%02d%02d", atoi(pToken1), atoi(pToken2), atoi(pToken3), atoi(pToken4) );
	dwCurVersion = (DWORD)atol((LPCTSTR)szString);
		
	return dwCurVersion;
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
BOOL CApp::InitInstance()
{
	::GetCurrentDirectory(MAX_PATH,g_szExePath);

	char path_buffer[_MAX_PATH] = {0.};	
	char drive[_MAX_DRIVE]		= {0.};		
	char dir[_MAX_DIR]			= {0.};		
	char fname[_MAX_FNAME]		= {0.};		
	char ext[_MAX_EXT]			= {0.};		
	
	_makepath(path_buffer,"c","\\sample\\crt\\","makepath","c");	
	_splitpath(path_buffer,drive,dir,fname,ext);		
	
#ifdef DEVELOPMODE
	CAutoPatch autodlg;
	int ret = autodlg.DoModal();
	
	if( ret == -1 )
		return FALSE;	
#endif

	AfxEnableControlContainer();

#ifdef _AFXDLL
	Enable3dControls();		
#else
	Enable3dControlsStatic();	
#endif
	
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings();
	
	// OleDbThread.dll COM 초기화
//	if(!InitDatabase())
//		return FALSE;

	InitDBAccount(); 

	Beep(0xdf, 100);

	if(!InitCorumDB())
		return FALSE;

//	LoadBaseItem(); hwoarang

#ifdef JJWMODE
	sprintf(m_LogInfo.ID,"%s","jjw6263");
	sprintf(m_LogInfo.IP,"%s","203.238.38.159");
	m_LogInfo.LoginType = 10000;
	sprintf(m_LogInfo.Name,"%s","정진욱");
	sprintf(m_LogInfo.Password,"%s","6263");
#else 
	// Login 
	CLoginDlg logindlg;
//	m_LogInfo.LoginType = 10000;
	if(logindlg.DoModal() == IDCANCEL)
	{
		PostQuitMessage(0);
		return FALSE;
	}
#endif

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_NEWSERTYPE,RUNTIME_CLASS(CDoc),RUNTIME_CLASS(CGuardianFrame),RUNTIME_CLASS(CGuardianView));
	AddDocTemplate(pDocTemplate);
	pDocTemplate = new CMultiDocTemplate(IDR_NEWSERTYPE,RUNTIME_CLASS(CDoc),RUNTIME_CLASS(CChildFrame),RUNTIME_CLASS(CMyView));
	AddDocTemplate(pDocTemplate);
	pDocTemplate = new CMultiDocTemplate(IDR_NEWSERTYPE,RUNTIME_CLASS(CDoc),RUNTIME_CLASS(CCopyItemFrame),RUNTIME_CLASS(CCopyItemView));
	AddDocTemplate(pDocTemplate);
	pDocTemplate = new CMultiDocTemplate(IDR_NEWSERTYPE,RUNTIME_CLASS(CDoc),RUNTIME_CLASS(CCopyItemDeleteFrame),RUNTIME_CLASS(CCopyItemDelete));
	AddDocTemplate(pDocTemplate);
	pDocTemplate = new CMultiDocTemplate(IDR_NEWSERTYPE,RUNTIME_CLASS(CDoc),RUNTIME_CLASS(CGuildFrame),RUNTIME_CLASS(CGuildInitView));
	AddDocTemplate(pDocTemplate);
	
	CMainFrame* pMainFrame = new CMainFrame;
	if(!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	pMainFrame->SetMenu(NULL); 
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
	pDocTemplate->m_hMenuShared = NULL;	

	return TRUE;
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	- 
//------------------------------------------------------------------------------
int CApp::ExitInstance() 
{
	m_ItemList.RemoveAll();
	m_ItemNoList.RemoveAll();

	if(g_pDb)	
	{
		// 인자가 0이면 모든 Connection Release...
		g_pDb->ReleaseDBConnectionForSingleThread(0);	
		g_pDb->Release();
	}

	return CWinApp::ExitInstance();
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	- OleDbThread.dll COM 초기화 
//------------------------------------------------------------------------------
BOOL CApp::InitDatabase()
{
	//	해당 프로세스가 COM 서브시스템을 사용할수 있도록 준비
	//	내부적으로 COM 서브시스템을 사용하는데 중요한 정보를 담을 여러가지 자료구조를 할당 
	//	클라이언트 프로그램의 프로세스 번호를 알아내고 이 번호를 사용하여 그 프로그램이 수행하는 COM관련 동작의 진행 상황을 추적한다.
	//	하나의 포인터 매개변수를 받으면 여기에는 항상 NULL을 넘겨준다.
	CoInitialize(NULL);				
	
	HRESULT hr = ::CoCreateInstance(	CLSID_OLEDBTHREAD,			/* 클래스 팩토리의 클래스 ID */
										NULL,						/* 외부객체의 IUnknown 인터페이스의 포인터 */ 
										CLSCTX_INPROC_SERVER,		/* 객체 생성이 이루어지길 원하는 COM 서버타입을 나타내는 CLSCTX 열거 상수 */
										IID_OLEDBTHREAD,			/* 새 객체로부터 얻기를 원하는 인터페이스 ID */
										(void**)&g_pDb			);	/* 반환되는 인터페이스 포인터를 받아낼 출력 매개변수로 사용되는 포인터의 주소 */
	
	if(FAILED(hr))
	{
		MessageBox(NULL, "Cannot create IID_OLEDBTHREAD", NULL, NULL);
		return FALSE;
	}
	
	DB_INITIALIZE_DESC desc;
	
	desc.bUsingThread					= FALSE;			// Thread 모델의 사용 여부, FALSE일 경우 (접두어 TH~ 계열의 메소드는 사용불가!)
	desc.bUsingEventObject				= FALSE;			// TRUE  : Event Object 방식으로 쿼리결과를 받음	FALSE : Message 방식으로 쿼리 결과를 Callback 함수  
	desc.ReceiveFunc					= NULL;				// Event Object 방식일경우 결과값을 받을 Callback 함수 포인터
	desc.hWndToPostMessage				= NULL;				// Message방식일 경우 Message를 받을 윈도우의 핸들값 
	desc.uMessage						= NULL;				// Message방식일 경우의 Message
	desc.wMaxNumOfProcessMessage_Input	= 100;				// DLL에서 동시에 수용할수 있는 최대 Input 메세지 갯수..  (Input 메세지큐 메모리풀의 최대용량)
	desc.wMaxNumOfProcessMessage_Output = 100;				// DLL에서 동시에 수용할수 있는 최대 Output 메세지 갯수..  (Output 메세지큐 메모리풀의 최대용량)
	desc.wMaxRowPerRead = 50;								// DB에서 Data를 읽어드릴때 한번에 읽어드릴수 있는 최대 행의갯수 
	
	//예)	만약 쿼리로인해 Return되는 결과행수가 200행이고 
	//		OpenRecordEx의 dwRowPerRead 인자값을 20으로 주었을경우
	//		한번에 20행씩 10번을 읽어서 200개를 받아온다.
	//		이때 dwRowPerRead인자값이 가질수있는 최대범위가 50이라는 의미!!
	
	desc.wMaxReturnedRowNum				= 50;				// 쿼리문에서 반환될 수 있는 행의 최대 갯수
	desc.wMaxReturnedColNum				= 128;				// 쿼리문에서 반환될 수 있는 칼럼의 최대 갯수
	desc.dwMaxRowSize					= 10240;			// 쿼리문에서 반환될 수 있는 한 행의 최대 사이즈	
	desc.OutputMessageFunc				= DisplayDBMessage;	// DLL내부의 메세지나 오류메세지를 출력받을 외부 함수 포인터
	
	// DB모듈 초기화 
	if(g_pDb->InitDBModule(&desc) == FALSE)
	{
		MessageBox(NULL,"Fail to initialize DB Module!",NULL,NULL);
		return FALSE;
	}
	
	return TRUE;
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CApp::InitDBAccount()
{
	char path[MAX_PATH] = {0,};
	sprintf(path,"%s\\DB.ini",".");
	
	char	keyname[ 0xff ]	= {0,};							
	int		TotalDBNum		= 0;
		
	TotalDBNum	= ::GetPrivateProfileInt("DB","TotalDBNum",0,path);
		
	for( int i = 0; i < TotalDBNum; i++ )
	{
		sprintf(keyname,"DB_INFO_0%d",i);
		::GetPrivateProfileString(keyname,"CORUM_DB_NAME",		"",g_CORUM_DB_NAME[i]		,sizeof(g_CORUM_DB_NAME[i])		,path);	
		::GetPrivateProfileString(keyname,"CORUM_DB_ACCOUNT",	"",g_CORUM_DB_ACCOUNT[i]	,sizeof(g_CORUM_DB_ACCOUNT[i])	,path);	
		::GetPrivateProfileString(keyname,"CORUM_DB_PASSWORD",	"",g_CORUM_DB_PASSWORD[i]	,sizeof(g_CORUM_DB_PASSWORD[i])	,path);
		::GetPrivateProfileString(keyname,"CORUM_DB_ADDRESS",	"",g_CORUM_DB_ADDRESS[i]	,sizeof(g_CORUM_DB_ADDRESS[i])	,path);
	}

	sprintf(keyname,"TOTALDB_INFO");
	::GetPrivateProfileString(keyname,"CORUM_DB_NAME",		"",g_CORUM_TDB_NAME			,sizeof(g_CORUM_TDB_NAME)		,path);	
	::GetPrivateProfileString(keyname,"CORUM_DB_ACCOUNT",	"",g_CORUM_TDB_ACCOUNT		,sizeof(g_CORUM_TDB_ACCOUNT)	,path);	
	::GetPrivateProfileString(keyname,"CORUM_DB_PASSWORD",	"",g_CORUM_TDB_PASSWORD		,sizeof(g_CORUM_TDB_PASSWORD)	,path);
	::GetPrivateProfileString(keyname,"CORUM_DB_ADDRESS",	"",g_CORUM_TDB_ADDRESS		,sizeof(g_CORUM_TDB_ADDRESS)	,path);
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
BOOL CApp::InitCorumDB()
{
	char path[MAX_PATH] = {0,};
	sprintf(path,"%s\\DB.ini",".");

	int TotalDBNum = ::GetPrivateProfileInt("DB","TotalDBNum",0,path);
/*
	for( int i = 0; i < TotalDBNum; i++ )
	{
		g_CORUM_GAME_DB[i] = g_pDb->CreateDBConnection();
		
		if(g_pDb->Connect(g_CORUM_DB_ADDRESS[i],g_CORUM_DB_NAME[i],g_CORUM_DB_ACCOUNT[i],g_CORUM_DB_PASSWORD[i],20,10,FALSE,g_CORUM_GAME_DB[i]) < 0)
		{
			g_pDb->ReleaseDBConnectionForSingleThread(g_CORUM_GAME_DB[i]);
			g_CORUM_GAME_DB[i] = 0;
			CString str;
			str.Format("Fail to Connect GAME_DB! (IP:%s, DBName:%s)", g_CORUM_DB_ADDRESS[i], g_CORUM_DB_NAME[i]);
			AfxMessageBox(str,MB_OK|MB_ICONSTOP);
			return FALSE;
		}		
	}

	g_CORUM_TOTAL_DB = g_pDb->CreateDBConnection();
	
	if(g_pDb->Connect(g_CORUM_TDB_ADDRESS,g_CORUM_TDB_NAME,g_CORUM_TDB_ACCOUNT,g_CORUM_TDB_PASSWORD,20,10,FALSE,g_CORUM_TOTAL_DB) < 0)
	{
		g_pDb->ReleaseDBConnectionForSingleThread(g_CORUM_TOTAL_DB);
		g_CORUM_TOTAL_DB = 0;
		CString str;
		str.Format("Fail to Connect TOTAL_DB! (IP:%s, DBName:%s)",g_CORUM_TDB_ADDRESS,g_CORUM_TDB_NAME);
		AfxMessageBox(str,MB_OK|MB_ICONSTOP);
		return FALSE;
	} // if(g_pDb->Connect(g_CORUM_TDB_ADDRESS,g_CORUM_TDB_NAME,g_CORUM_TDB_ACCOUNT,g_CORUM_TDB_PASSWORD,20,10,FALSE,g_CORUM_TOTAL_DB) < 0)
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for( int i = 0; i < TotalDBNum; i++ )
	{
		wstring wstrAddr, wstrDBName, wstrDBID, wstrDBPW;

		HS_MBTOWS( g_CORUM_DB_ADDRESS[i], wstrAddr);
		HS_MBTOWS( g_CORUM_DB_NAME[i], wstrDBName);
		HS_MBTOWS( g_CORUM_DB_ACCOUNT[i], wstrDBID);
		HS_MBTOWS( g_CORUM_DB_PASSWORD[i], wstrDBPW);
		
		if( !g_DBConn[i].Init( wstrAddr, wstrDBName, wstrDBID, wstrDBPW))
		{
			return FALSE;
		}
	} // for( int i = 0; i < TotalDBNum; i++ )


	return TRUE;
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	- 
//------------------------------------------------------------------------------
void CApp::LoadBaseItem()
{
	DBRECEIVEDATA*		pData			= NULL;
	CBaseItem*			pBaseItem		= NULL;
	
	// Item Weapon
	pData = g_pDb->OpenRecordEx("Select * from Item_Weapon",500,DEFAULT_ROWS_PER_READ,g_CORUM_GAME_DB[0]);
	
	DWORD i;

	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset(pBaseItem,0,sizeof(CBaseItem));
		memcpy(pBaseItem,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize);
		m_ItemList.AddTail(*pBaseItem);		
		delete pBaseItem;
	}

	g_pDb->ReleaseRecordset(pData);

	// Item Armor
	pData = g_pDb->OpenRecordEx("Select * from Item_Armor", 600, DEFAULT_ROWS_PER_READ,g_CORUM_GAME_DB[0]);

	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset(pBaseItem,0,sizeof(CBaseItem));
		memcpy(pBaseItem,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize);
		if( pBaseItem->wID == 3200 )
		{
			int a = 0;
		}
		m_ItemList.AddTail(*pBaseItem);
		delete pBaseItem;
	}

	g_pDb->ReleaseRecordset(pData);

	// Item Guardian(1900 ~)
	pData = g_pDb->OpenRecordEx("Select * from Item_Guardian",500,DEFAULT_ROWS_PER_READ,g_CORUM_GAME_DB[0]);
	
	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset(pBaseItem,0,sizeof(CBaseItem));
		memcpy(pBaseItem,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize );
		m_ItemList.AddTail(*pBaseItem);
		delete pBaseItem;
	}
	
	g_pDb->ReleaseRecordset(pData);

	// Item RideObject(2000 ~)
	pData = g_pDb->OpenRecordEx("Select * from Item_Ride",500,DEFAULT_ROWS_PER_READ,g_CORUM_GAME_DB[0]);
	
	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset(pBaseItem,0,sizeof(CBaseItem));
		memcpy(pBaseItem,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize );
		m_ItemList.AddTail(*pBaseItem);	
		delete pBaseItem;
	}
	
	g_pDb->ReleaseRecordset(pData);

	// Item Mix Upgrade(3000 ~)
	pData = g_pDb->OpenRecordEx("Select * from Item_Mix_Upgrade",500,DEFAULT_ROWS_PER_READ,g_CORUM_GAME_DB[0]);

	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset(pBaseItem,0,sizeof(CBaseItem));
		memcpy(pBaseItem,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize );
		m_ItemList.AddTail(*pBaseItem);	
		delete pBaseItem;
	}
	
	g_pDb->ReleaseRecordset(pData);

	// Item Zodiac(4000 ~)
	pData = g_pDb->OpenRecordEx("Select * from Item_Zodiac",500,DEFAULT_ROWS_PER_READ,g_CORUM_GAME_DB[0]);
	
	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset(pBaseItem,0,sizeof(CBaseItem));
		memcpy(pBaseItem,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize );
		m_ItemList.AddTail(*pBaseItem);	
		delete pBaseItem;
	}
	
	g_pDb->ReleaseRecordset(pData);
	
	// Item Supplies(5000 ~)
	pData = g_pDb->OpenRecordEx("Select * from Item_Supplies",500,DEFAULT_ROWS_PER_READ,g_CORUM_GAME_DB[0]);

	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset(pBaseItem,0,sizeof(CBaseItem));
		memcpy(pBaseItem,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize );
		m_ItemList.AddTail(*pBaseItem);	
		delete pBaseItem;
	}

	g_pDb->ReleaseRecordset(pData);

	// Item Special(6000 ~)
	pData = g_pDb->OpenRecordEx("Select * from Item_Special",500,DEFAULT_ROWS_PER_READ,g_CORUM_GAME_DB[0]);

	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset(pBaseItem,0,sizeof(CBaseItem));
		memcpy(pBaseItem,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize );
		m_ItemList.AddTail(*pBaseItem);	
		delete pBaseItem;
	}

	g_pDb->ReleaseRecordset(pData);

	// Item Material(7000 ~)
	pData = g_pDb->OpenRecordEx("Select * from Item_Materials",500,DEFAULT_ROWS_PER_READ,g_CORUM_GAME_DB[0]);
	
	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset(pBaseItem,0,sizeof(CBaseItem));
		memcpy(pBaseItem,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize );
		m_ItemList.AddTail(*pBaseItem);
		delete pBaseItem;
	}
	
	g_pDb->ReleaseRecordset(pData);

	// Item MagicArray(8000 ~) 
	pData = g_pDb->OpenRecordEx("Select * from Item_Magic_Array",500,DEFAULT_ROWS_PER_READ,g_CORUM_GAME_DB[0]);

	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset(pBaseItem,0,sizeof(CBaseItem));
		memcpy(pBaseItem,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize );
		m_ItemList.AddTail(*pBaseItem);	
		delete pBaseItem;
	}

	g_pDb->ReleaseRecordset(pData);

	// Item Magicfield Array(9000 ~)
	pData = g_pDb->OpenRecordEx("Select * from Item_Magicfield_Array",500,DEFAULT_ROWS_PER_READ,g_CORUM_GAME_DB[0]);

	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset(pBaseItem,0,sizeof(CBaseItem));
		memcpy(pBaseItem,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize );
		m_ItemList.AddTail(*pBaseItem);	
		delete pBaseItem;
	}

	g_pDb->ReleaseRecordset(pData);

	// Item Consumable(10100 ~)
	pData = g_pDb->OpenRecordEx("Select * from Item_Consumable", 500, DEFAULT_ROWS_PER_READ,g_CORUM_GAME_DB[0]);
	
	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset(pBaseItem,0,sizeof(CBaseItem));
		memcpy(pBaseItem,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize );
		m_ItemList.AddTail(*pBaseItem);	
		delete pBaseItem;
	}
	
	g_pDb->ReleaseRecordset(pData);
}


class CAboutDlg : public CDialog
{

public:

	CAboutDlg();

	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}


void CApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}





