#include "stdafx.h"
#include "OwnServer.h"
#include "recvmsg.h"
#include "User.h"
#include "WorldServerTable.h"
#include "UserTable.h"
#include "CPSSOLib.h"
#include "ServerFunction.h"
#include "Version.h"
#include "ChinaBillingProc.h"
#include "LicensingProc.h"
#include "Filter.h"
#include "../BaseLibrary/mdump.h"

CNTNetWork			*g_pNet				= NULL;
IOleDBThread		*g_pDb				= NULL;
IOleDBThread		*g_pBillDb			= NULL;		
IOleDBThread		*g_pLogDb			= NULL;
LPBASEITEM_HASH		g_pBaseItemHash		= NULL;
BOOL				g_bIsServerRunning	= TRUE;
DWORD				g_dwVersion			= 0;
DWORD				GAME_DB		= 0;
DWORD				TOTAL_DB	= 0;
DWORD				MEMBER_DB	= 0;
DWORD				BILL_DB		= 0;
DWORD				LOG_DB      = 0;
CTime				g_RecentCheckTime;
COwnServer			*g_pThis	= NULL;
MACHINENAME			g_MachineName;


DWORD GetEncryptedVersion(const char* szSource)
{
	DFXform Xform;
	
	char szTemp[0xff]={0,};
	char szTemp2[0xff]={0,};
	UINT nSize = 0;

	Xform.XXDecode((const BYTE*)szSource, lstrlen(szSource)+1, (LPBYTE)szTemp, &nSize);
	Xform.UUDecode((const BYTE*)szTemp, nSize, (LPBYTE)szTemp2, &nSize);

	return (DWORD)atol(szTemp2);
}

BOOL OverOneMonth(DWORD dwVersion)
{
	SYSTEMTIME Time;
	GetLocalTime(&Time);

	DWORD dwTemp = dwVersion;

	int Year	= dwVersion/1000000;	dwTemp -= Year * 1000000;
	int Month	= dwTemp   /  10000;	dwTemp -= Month * 10000;
	int Day		= dwTemp   /    100;

	CTime TimeVer(Year+ 2000, Month, Day, 0, 0, 0);
	CTime TimeNow(Time);

	CTimeSpan Gap = TimeNow - TimeVer;

	return (Gap.GetDays() > 31);
}

void MainLoop()
{
	DWORD dwResult = 0;
	INPUT_RECORD irBuffer;
	memset(&irBuffer, 0, sizeof(INPUT_RECORD));

	while ( g_bIsServerRunning )
	{
		GetConsoleKeyInput(&irBuffer, &dwResult);

		if (irBuffer.EventType == KEY_EVENT)
		{
			if (irBuffer.Event.KeyEvent.bKeyDown)
			{
				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
				{
					if( MessageBox( NULL, "Are you sure to DESTROY LoginAgent?", "LoginAgent Server Terminate Question", MB_YESNO ) == IDYES )
					{
						Log(LOG_IMPORTANT,"MainLoop() : SetEvent(g_pThis->m_hKeyEvent[7])");
						SetEvent(g_pThis->m_hKeyEvent[7]);
					}					
				}

				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_F1)
				{
					SetEvent(g_pThis->m_hKeyEvent[1]);
				}

				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_F3)
				{
					SetEvent(g_pThis->m_hKeyEvent[0]);
				}

				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_F5)
				{
					SetEvent(g_pThis->m_hKeyEvent[2]);
				}

				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_F7)
				{
					if( MessageBox( NULL, "Are you sure to logout All User?", "Logout Question", MB_YESNO ) == IDYES )
					{
						SetEvent(g_pThis->m_hKeyEvent[4]);
					}
				}
				
				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_F11)
				{
					SetEvent(g_pThis->m_hKeyEvent[5]);
				}

				if (irBuffer.Event.KeyEvent.wVirtualKeyCode == VK_F12)
				{
					SetEvent(g_pThis->m_hKeyEvent[6]);
				}				
			}
		}		
	}
}

void SetVersionString()
{
	switch( MY_NATION_CODE )
	{
	case NC_JAPAN:
	case NC_CHINA:
	case NC_TAIWAN:
	case NC_THAI:
	case NC_KOREA:
	default:
		{
			CURRENT_VERSION_STRING = "KJ-VTJa+KLtNI33BHJpBHE2+"; // 05033101
			//05020401 /*KJ-VTJZkIMFNI33BHJpBHE2+*/
		}break;
	}
}

void SetDBInfo()
{
	
	if( IS_ABLE_SERVICE_TYPE(ST_DEVELOP) )
	{
		g_DBInfo.Set(DT_GAME_DB, DBIT_CATALOG, "Corum");
		g_DBInfo.Set(DT_GAME_DB, DBIT_ID, "potato");
		g_DBInfo.Set(DT_GAME_DB, DBIT_PW, "ahffk");//5378abc

		g_DBInfo.Set(DT_TOTAL_DB, DBIT_CATALOG, "Corum_Total_DB");
		g_DBInfo.Set(DT_TOTAL_DB, DBIT_ID, "potato");
		g_DBInfo.Set(DT_TOTAL_DB, DBIT_PW, "ahffk");

		g_DBInfo.Set(DT_MEMBER_DB, DBIT_CATALOG, "Member");
		g_DBInfo.Set(DT_MEMBER_DB, DBIT_ID, "potato");
		g_DBInfo.Set(DT_MEMBER_DB, DBIT_PW, "ahffk");

		g_DBInfo.Set(DT_LOG_DB, DBIT_CATALOG, "Log_Corum_");
		g_DBInfo.Set(DT_LOG_DB, DBIT_ID, "potato");
		g_DBInfo.Set(DT_LOG_DB, DBIT_PW, "ahffk");
		return;
	} // if( IS_ABLE_SERVICE_TYPE(ST_DEVELOP) )

	switch( MY_NATION_CODE )
	{
	case NC_JAPAN:
		{
			g_DBInfo.Set(DT_GAME_DB, DBIT_CATALOG, "Corum");
			g_DBInfo.Set(DT_GAME_DB, DBIT_ID, "potato");
			g_DBInfo.Set(DT_GAME_DB, DBIT_PW, "ajtwlssja");

			g_DBInfo.Set(DT_TOTAL_DB, DBIT_CATALOG, "Total_DB");
			g_DBInfo.Set(DT_TOTAL_DB, DBIT_ID, "potato");
			g_DBInfo.Set(DT_TOTAL_DB, DBIT_PW, "ajtwlssja");

			g_DBInfo.Set(DT_MEMBER_DB, DBIT_CATALOG, "CORUM");
			g_DBInfo.Set(DT_MEMBER_DB, DBIT_ID, "potato");
			g_DBInfo.Set(DT_MEMBER_DB, DBIT_PW, "ajtwlssja");

			g_DBInfo.Set(DT_LOG_DB, DBIT_CATALOG, "Log_Corum_");
			g_DBInfo.Set(DT_LOG_DB, DBIT_ID, "potato");
			g_DBInfo.Set(DT_LOG_DB, DBIT_PW, "ajtwlssja");
		}break;
	case NC_CHINA:
		{
			g_DBInfo.Set(DT_GAME_DB, DBIT_CATALOG, "Corum");
			g_DBInfo.Set(DT_GAME_DB, DBIT_ID, "potato");
			g_DBInfo.Set(DT_GAME_DB, DBIT_PW, "ajtwlssja");

			g_DBInfo.Set(DT_TOTAL_DB, DBIT_CATALOG, "Total_DB");
			g_DBInfo.Set(DT_TOTAL_DB, DBIT_ID, "potato");
			g_DBInfo.Set(DT_TOTAL_DB, DBIT_PW, "ajtwlssja");

			g_DBInfo.Set(DT_MEMBER_DB, DBIT_CATALOG, "CORUM");
			g_DBInfo.Set(DT_MEMBER_DB, DBIT_ID, "potato");
			g_DBInfo.Set(DT_MEMBER_DB, DBIT_PW, "ajtwlssja");

			g_DBInfo.Set(DT_LOG_DB, DBIT_CATALOG, "Log_Corum_");
			g_DBInfo.Set(DT_LOG_DB, DBIT_ID, "potato");
			g_DBInfo.Set(DT_LOG_DB, DBIT_PW, "ajtwlssja");		
		}break;
	case NC_TAIWAN:
		{
			g_DBInfo.Set(DT_GAME_DB, DBIT_CATALOG, "Corum");
			g_DBInfo.Set(DT_GAME_DB, DBIT_ID, "potato");
			g_DBInfo.Set(DT_GAME_DB, DBIT_PW, "8x0ozs1xos");

			g_DBInfo.Set(DT_TOTAL_DB, DBIT_CATALOG, "Total_DB");
			g_DBInfo.Set(DT_TOTAL_DB, DBIT_ID, "potato");
			g_DBInfo.Set(DT_TOTAL_DB, DBIT_PW, "8x0ozs1xos");

			g_DBInfo.Set(DT_MEMBER_DB, DBIT_CATALOG, "Member");
			g_DBInfo.Set(DT_MEMBER_DB, DBIT_ID, "potato");
			g_DBInfo.Set(DT_MEMBER_DB, DBIT_PW, "8x0ozs1xos");

			g_DBInfo.Set(DT_LOG_DB, DBIT_CATALOG, "Log_Corum_");
			g_DBInfo.Set(DT_LOG_DB, DBIT_ID, "potato");
			g_DBInfo.Set(DT_LOG_DB, DBIT_PW, "8x0ozs1xos");

		}break;
	case NC_KOREA:
		{
			g_DBInfo.Set(DT_GAME_DB, DBIT_CATALOG, "Corum");
			g_DBInfo.Set(DT_GAME_DB, DBIT_ID, "GameSrv");
			g_DBInfo.Set(DT_GAME_DB, DBIT_PW, "vhrwnQkdqhdxn!");

			g_DBInfo.Set(DT_TOTAL_DB, DBIT_CATALOG, "Total_DB");
			g_DBInfo.Set(DT_TOTAL_DB, DBIT_ID, "GameSrv");
			g_DBInfo.Set(DT_TOTAL_DB, DBIT_PW, "vhrwnQkdqhdxn!");

			g_DBInfo.Set(DT_MEMBER_DB, DBIT_CATALOG, "Member");
			g_DBInfo.Set(DT_MEMBER_DB, DBIT_ID, "GameSrv");
			g_DBInfo.Set(DT_MEMBER_DB, DBIT_PW, "vhrwnQkdqhdxn!");

			g_DBInfo.Set(DT_LOG_DB, DBIT_CATALOG, "Log_Corum_");
			g_DBInfo.Set(DT_LOG_DB, DBIT_ID, "GameSrv");
			g_DBInfo.Set(DT_LOG_DB, DBIT_PW, "vhrwnQkdqhdxn!");
		}break;
	default:
		{
		}break;
	} // switch( MY_NATION_CODE )
}

int main(int argc, char* argv[])
{
	static auto minidumper = new MiniDumper("LOGIN_CRASH");

	if(!g_Local.LoadFromINI("./Local.ini"))// Local Info 셋팅 : 050102 hwoarang
	{
		assert(NULL && "local.ini not exist");
		return 0;
	} // if(!g_Local.LoadFromINI("./Local.ini"))// Local Info 셋팅 : 050102 hwoarang

	SetVersionString();//버전값 셋팅 : 050103 hwoarang
	SetDBInfo();// DB 계정 정보 셋팅 : 050103 hwoarang

	if( IS_ABLE_SERVICE_TYPE(ST_DEVELOP) )//DEV_MODE : 050104 Hwoarang
	{
		ShowAllPacketSize();
	}

	g_dwVersion = GetEncryptedVersion( CURRENT_VERSION_STRING.c_str() );//로컬매니저로 해당 데이터는 string 형으로 변경 :050104 hwoarang



	switch( MY_NATION_CODE )
	{	//중국은 한달 넘으면 작동 안함
	case NC_CHINA:
		{		
			if(OverOneMonth(g_dwVersion)) 
			{
				MessageBox(NULL, "Not latest version! Update!", "VersionCheck", MB_OK|MB_ICONWARNING);
				return 0;
			} // if(OverOneMonth(g_dwVersion))
		}break;
	} // switch( MY_NATION_CODE )

	AllocConsole();
	
	InitLog();

	g_pThis = new COwnServer;
	g_pThis->Initialize();

	// 동일 서버의 중복실행 방지!
	if(GKO_EXIST == CreateGlobalKernelObject(g_pThis->GetPortForUser()))
	{
		MessageBox(NULL,	"Same Server Already Running.\n"
							"Check out another terminal session!\n"
							"Now Server Down!", "Not Unique Process", MB_OK|MB_ICONINFORMATION);

		delete g_pThis;
		FreeLog();
		FreeConsole();
		return 0;
	}
	
	char szTitle[ 0xff ]={0,};
	wsprintf(szTitle,"[%d] CorumOnline Login Agent Server (%s)",g_pThis->GetPortForUser(),g_pThis->GetIPForUser());	
	SetConsoleTitle(szTitle);

	Log(LOG_NORMAL, "########### CorumOnline - LoginAgent Server Started! ###########");

	char szVersion[0xff]={0,};
	wsprintf(szVersion,"@ [CURRENT VERSION] %d",g_dwVersion);
	Log(LOG_JUST_DISPLAY, szVersion);

	g_pServerTable = new CWorldServerTable;

	if( IS_ABLE_SERVICE_TYPE(ST_DEVELOP) )//DEV_MODE : 050104 Hwoarang
	{
		g_pUserTable = new CUserTable(100);
	}
	else
	{
		g_pUserTable = new CUserTable(5000);	
	}

	CoInitialize(NULL);		
	
	InitNetwork();			
	InitOleDBThread();	
	if( ! ConnectToDBServer() )
	{
		assert( NULL && "ConnectToDBServer()" );
		return 0;
	}
	QueryAllServer();
	QueryBaseClassInfo();	
	InitItemData();		

	// 중문 버전의 겨우, 캐릭터 생성 시, 필터 적용을 서버에서도 한다.
	switch( MY_NATION_CODE )
	{	//중국은 한달 넘으면 작동 안함
	case NC_CHINA:
		{		
			InitLimitedWords();
		}break;
	}
	
	if(!g_pThis->ResetItemSerialNumber())
	{
		Log(LOG_IMPORTANT,"Reset Item Serial Number() : SetEvent(g_pThis->m_hKeyEvent[7])");
		SetEvent(g_pThis->m_hKeyEvent[7]);
	}	

	GetComputerName(g_MachineName.szBuffer,&g_MachineName.dwSize);
	g_MachineName.wPort			= g_pThis->GetPortForUser();
	g_MachineName.bServerType	= g_pThis->GetServerType();
	wsprintf(g_MachineName.szIpForUser,g_pThis->GetIPForUser());
	
#ifdef USE_LICENSING_SERVER
	int nCount = 0;

	// 연결에 실패하면 종료해야지~!
	InitializeProcFunctionForLicensing();
	if(!ConnectToLicensingServer()) goto lb_Exit;
	
	// 인증절차를 거친다.

	// 10 초간 연결, 인증을 거쳐야 한다.
	for(;;)
	{
		if(g_pThis->IsLicensingServerConnected() && g_pThis->IsAuthenticated()) break;
		else if(nCount > 100) goto lb_Exit;
		else 
		{
			nCount++;
			Sleep(100);
		}
	}
#endif	

	g_pThis->ToggleUserAccept();
	ListenForServerside();
	ListenForUserside();

	if( IS_ABLE_SERVICE_TYPE(ST_CHINA_BILLING) )//중국빌링 Test : 050104 hwoarang
	{
		InitializeProcFunction();
		ConnectToBillingServer();
	}
	
	ReportToCMListener(0, &g_MachineName);

	MainLoop();	

#ifdef USE_LICENSING_SERVER	
lb_Exit:
#endif	

	Log(LOG_NORMAL, "Start! - Release BaseNetwork!");
	if (g_pNet)
	{
		g_pNet->Release();
		g_pNet = NULL;
	}
	Log(LOG_NORMAL, "End! - Release BaseNetwork!");

	Log(LOG_NORMAL, "Start! - Release DB Component!");
	if(g_pDb)
	{
		//g_pDb->ReleaseDBConnection(0);
		g_pDb->Release();
		g_pDb = NULL;
	}

	if(g_pBillDb)
	{
		g_pBillDb->Release();
		g_pBillDb = NULL;
	}

	// Log Db IO 해제.
	if( g_pLogDb )
	{
        g_pLogDb->Release();
		g_pLogDb = NULL;
	}

	Log(LOG_NORMAL, "End! - Release DB Component!");
	Log(LOG_NORMAL, "Start! - CoUninitialize!");
	FreeLog();
	CoFreeUnusedLibraries();
	CoUninitialize();
	FreeConsole();
	DeleteGlobalKernelObject();
	ReportToCMListener(4, &g_MachineName);
	return 0;
}
