#include "stdafx.h"
#include "ServerFunction.h"
#include "Version.h"
#include "OwnServer.h"
#include "ThrowQuery.h"
#include "recvmsg.h"
#include "WorldServerTable.h"
#include "User.h"
#include "UserTable.h"
#include "ChinaBillingProc.h"
#include "LicensingProc.h"
#include "PacketEncrypt.h"
#include <OleDBThreadCls.h>

extern LPBASEITEM_HASH		g_pBaseItemHash;
extern BOOL					g_bIsServerRunning;
extern CTime				g_RecentCheckTime;
extern IOleDBThread*		g_pDb;
extern IOleDBThread*		g_pBillDb;
extern IOleDBThread*		g_pLogDb;
extern DWORD				BILL_DB;
extern COwnServer*			g_pThis;
extern MACHINENAME			g_MachineName;
extern DWORD				LOG_DB;
DWORD						g_dwExp[256]={0,};
BYTE						g_byStatus[256]={0,};


void __stdcall GetDBResult(DWORD dwEventIndex)
{
	dwEventIndex;

	g_pDb->GetDBResult();
}

void __stdcall GetBillingDBResult(DWORD dwEventIndex)
{
	dwEventIndex;

	g_pBillDb->GetDBResult();
}

void	__stdcall	GetLogDBResult( DWORD dwEventIndex )
{
	g_pLogDb->GetDBResult();
}

void DisplayDBMessage(char* szMsg)
{
	Log(LOG_IMPORTANT, "[DB]::");
	Log(LOG_IMPORTANT, szMsg);
}

void DisplayBillDBMessage(char* szMsg)
{
	Log(LOG_IMPORTANT, "[BillDB]::");
	Log(LOG_IMPORTANT, szMsg);
}

void DisplayDBReport(char* szMsg)
{
	Log(LOG_IMPORTANT, "[DBReport]::");
	Log(LOG_IMPORTANT, szMsg);
}

void DisplayDBBillReport(char* szMsg)
{
	Log(LOG_IMPORTANT, "[DBBillReport]::");
	Log(LOG_IMPORTANT, szMsg);
}

void InitOleDBThread()
{
	g_pDb = new COleDBThreadCls;

	DB_INITIALIZE_DESC desc;

	desc.bUsingThread = FALSE;					//Thread 모델의 사용 여부, FALSE일 경우 (접두어 TH~ 계열의 메소드는 사용불가!)
	desc.bUsingEventObject = TRUE;				//TRUE  : Event Object 방식으로 쿼리결과를 받음
												//FALSE : Message 방식으로 쿼리 결과를 Callback 함수  
	
	desc.ReceiveFunc = ReceivedFromDB;			//Event Object 방식일경우 결과값을 받을 Callback 함수 포인터
	
	desc.hWndToPostMessage = NULL;				//Message방식일 경우 Message를 받을 윈도우의 핸들값 
	desc.uMessage = NULL;						//Message방식일 경우의 Message	

	if( IS_ABLE_SERVICE_TYPE(ST_DEVELOP) )//DEV_MODE : 050104 Hwoarang
	{
		desc.wMaxNumOfProcessMessage_Input	= 500;	//DLL에서 동시에 수용할수 있는 최대 Input 메세지 갯수..  (Input 메세지큐 메모리풀의 최대용량)
		desc.wMaxNumOfProcessMessage_Output = 500;	//DLL에서 동시에 수용할수 있는 최대 Output 메세지 갯수..  (Output 메세지큐 메모리풀의 최대용량)
	}
	else
	{
		desc.wMaxNumOfProcessMessage_Input	= 2048;	//DLL에서 동시에 수용할수 있는 최대 Input 메세지 갯수..  (Input 메세지큐 메모리풀의 최대용량)
		desc.wMaxNumOfProcessMessage_Output = 2048;	//DLL에서 동시에 수용할수 있는 최대 Output 메세지 갯수..  (Output 메세지큐 메모리풀의 최대용량)
	}
	desc.wMaxRowPerRead = 50;					//DB에서 Data를 읽어드릴때 한번에 읽어드릴수 있는 최대 행의갯수 
												//예) 만약 쿼리로인해 Return되는 결과행수가 200행이고 
												//	  OpenRecordEx의 dwRowPerRead 인자값을 20으로 주었을경우
												//    한번에 20행씩 10번을 읽어서 200개를 받아온다.
												//	  이때 dwRowPerRead인자값이 가질수있는 최대범위가 50이라는 의미!!
	desc.wMaxReturnedRowNum = 50;				//쿼리문에서 반환될 수 있는 행의 최대 갯수
	desc.wMaxReturnedColNum = 128;				//쿼리문에서 반환될 수 있는 칼럼의 최대 갯수
	desc.dwMaxRowSize = 2048;					//쿼리문에서 반환될 수 있는 한 행의 최대 사이즈	
	desc.OutputMessageFunc = DisplayDBMessage;	//DLL내부의 메세지나 오류메세지를 출력받을 외부 함수 포인터
	desc.ReportFunc = DisplayDBReport;	
	desc.bMaxParamNum		= 100;
	
	
	// DB모듈 초기화 
	if(g_pDb->InitDBModule(&desc) == FALSE)
	{
		Log(LOG_NORMAL, "Fail to initialize DB Module!");
		return;
	}

	//Event Object 방식일경우 Event 핸들 셋팅
	g_pDb->SetDBResultEvent( g_pNet->GetCustomEventHandle(0) );

	g_pDb->SetPerformanceReport(FALSE);

	// Billing Check DB Instance 생성 
	g_pBillDb = new COleDBThreadCls;


	desc.bUsingThread		= FALSE;						//Thread 모델의 사용 여부, FALSE일 경우 (접두어 TH~ 계열의 메소드는 사용불가!)
	desc.bUsingEventObject	= TRUE;						//TRUE  : Event Object 방식으로 쿼리결과를 받음
														//FALSE : Message 방식으로 쿼리 결과를 Callback 함수  
	
	desc.ReceiveFunc		= ReceivedFromDBBill;		//Event Object 방식일경우 결과값을 받을 Callback 함수 포인터
	
	desc.hWndToPostMessage	= NULL;						//Message방식일 경우 Message를 받을 윈도우의 핸들값 
	desc.uMessage			= NULL;						//Message방식일 경우의 Message	

	if( IS_ABLE_SERVICE_TYPE(ST_DEVELOP) )//DEV_MODE : 050104 Hwoarang
	{
		desc.wMaxNumOfProcessMessage_Input	= 50;			//DLL에서 동시에 수용할수 있는 최대 Input 메세지 갯수..  (Input 메세지큐 메모리풀의 최대용량)
		desc.wMaxNumOfProcessMessage_Output = 50;			//DLL에서 동시에 수용할수 있는 최대 Output 메세지 갯수..  (Output 메세지큐 메모리풀의 최대용량)
	} // if( IS_ABLE_SERVICE_TYPE(ST_DEVELOP) )//DEV_MODE : 050104 Hwoarang
	else
	{
		desc.wMaxNumOfProcessMessage_Input	= 2048;			//DLL에서 동시에 수용할수 있는 최대 Input 메세지 갯수..  (Input 메세지큐 메모리풀의 최대용량)
		desc.wMaxNumOfProcessMessage_Output = 2048;			//DLL에서 동시에 수용할수 있는 최대 Output 메세지 갯수..  (Output 메세지큐 메모리풀의 최대용량)
	}
	desc.wMaxRowPerRead = 50;							//DB에서 Data를 읽어드릴때 한번에 읽어드릴수 있는 최대 행의갯수 
														//예) 만약 쿼리로인해 Return되는 결과행수가 200행이고 
														//	  OpenRecordEx의 dwRowPerRead 인자값을 20으로 주었을경우
														//    한번에 20행씩 10번을 읽어서 200개를 받아온다.
														//	  이때 dwRowPerRead인자값이 가질수있는 최대범위가 50이라는 의미!!
	desc.wMaxReturnedRowNum		= 50;					//쿼리문에서 반환될 수 있는 행의 최대 갯수
	desc.wMaxReturnedColNum		= 128;					//쿼리문에서 반환될 수 있는 칼럼의 최대 갯수
	desc.dwMaxRowSize			= 128;					//쿼리문에서 반환될 수 있는 한 행의 최대 사이즈	
	desc.OutputMessageFunc		= DisplayBillDBMessage;	//DLL내부의 메세지나 오류메세지를 출력받을 외부 함수 포인터
	desc.ReportFunc				= DisplayDBBillReport;
	desc.bMaxParamNum			= 100;	
	
	//DB모듈 초기화 
	if(g_pBillDb->InitDBModule(&desc) == FALSE)
	{
		Log(LOG_NORMAL, "Fail to initialize DB Module!(BillingDB)");
		return;
	}

	//Event Object 방식일경우 Event 핸들 셋팅
	g_pBillDb->SetDBResultEvent( g_pNet->GetCustomEventHandle(9) );

	g_pBillDb->SetPerformanceReport(FALSE);	

	////////////////////////////////////////////////////////////////////////////
	// LogDB 컴포넌트 생성
	//////////////////////////////////////////////////////////////////////////
	// LogDB..	

	g_pLogDb = new COleDBThreadCls;

	//	DB_INITIALIZE_DESC desc;

	desc.bUsingThread		= FALSE;					//Thread 모델의 사용 여부, FALSE일 경우 (접두어 TH~ 계열의 메소드는 사용불가!)
	desc.bUsingEventObject	= TRUE;					//TRUE  : Event Object 방식으로 쿼리결과를 받음
	//FALSE : Message 방식으로 쿼리 결과를 Callback 함수  

	desc.ReceiveFunc		= ReceivedFromDBLog;	//Event Object 방식일경우 결과값을 받을 Callback 함수 포인터

	desc.hWndToPostMessage	= NULL;					//Message방식일 경우 Message를 받을 윈도우의 핸들값 
	desc.uMessage			= NULL;					//Message방식일 경우의 Message

	if( IS_ABLE_SERVICE_TYPE(ST_DEVELOP) )
	{
		desc.wMaxNumOfProcessMessage_Input = 50;	//DLL에서 동시에 수용할수 있는 최대 Input 메세지 갯수..  (Input 메세지큐 메모리풀의 최대용량)
		desc.wMaxNumOfProcessMessage_Output = 50;	//DLL에서 동시에 수용할수 있는 최대 Output 메세지 갯수..  (Output 메세지큐 메모리풀의 최대용량)
	}
	else
	{
		desc.wMaxNumOfProcessMessage_Input = 2048;	//DLL에서 동시에 수용할수 있는 최대 Input 메세지 갯수..  (Input 메세지큐 메모리풀의 최대용량)
		desc.wMaxNumOfProcessMessage_Output = 2048;	//DLL에서 동시에 수용할수 있는 최대 Output 메세지 갯수..  (Output 메세지큐 메모리풀의 최대용량)
	}

	desc.wMaxRowPerRead = 50;						//DB에서 Data를 읽어드릴때 한번에 읽어드릴수 있는 최대 행의갯수 
	//예) 만약 쿼리로인해 Return되는 결과행수가 200행이고 
	//	  OpenRecordEx의 dwRowPerRead 인자값을 20으로 주었을경우
	//    한번에 20행씩 10번을 읽어서 200개를 받아온다.
	//	  이때 dwRowPerRead인자값이 가질수있는 최대범위가 50이라는 의미!!

	desc.wMaxReturnedRowNum		= 50;				//쿼리문에서 반환될 수 있는 행의 최대 갯수
	desc.wMaxReturnedColNum		= 128;				//쿼리문에서 반환될 수 있는 칼럼의 최대 갯수
	desc.dwMaxRowSize			= 1024;				//쿼리문에서 반환될 수 있는 한 행의 최대 사이즈	
	desc.bMaxParamNum			= 100;

	desc.OutputMessageFunc		= DisplayDBMessage;	//DLL내부의 메세지나 오류메세지를 출력받을 외부 함수 포인터

	//DB모듈 초기화 
	if(g_pLogDb->InitDBModule(&desc) == FALSE)
	{
		Log(LOG_NORMAL, "Fail to initialize DB Module!(LogDB)");
		return;
	}

	//	Log(LOG_NORMAL, "Log DB Size : %u Byte", g_pLogDb->GetOLEDBSize());

	//Event Object 방식일경우 Event 핸들 셋팅
	g_pLogDb->SetDBResultEvent( g_pNet->GetCustomEventHandle(16) );

	g_pLogDb->SetPerformanceReport(FALSE);
}


void TerminateServer()
{
	Log(LOG_NORMAL, "Start! - Logout All User!");
	
	char szTitle[ 0xff ]={0,};
	wsprintf(szTitle,"[%d] CorumOnline Login Agent Server (%s)",g_pThis->GetPortForUser(),g_pThis->GetIPForUser());
	HWND hWnd = NULL;
	hWnd = FindWindow(NULL,szTitle);

	if(ThrowLogoutAllQuery(FALSE) < -1)
	{
		MessageBox(NULL, "Logout All Failed!", "Logout Error", MB_OK);
	}

	Log(LOG_NORMAL, "End! - Logout All User!");

		
	Log(LOG_NORMAL, "Start! - Delete OwnServerInfo!");
	if(g_pThis)
		delete g_pThis;
	Log(LOG_NORMAL, "End! - Delete OwnServerInfo!");

	if(g_pServerTable)
	{
		delete g_pServerTable;
		g_pServerTable = NULL;
	}

	if(g_pUserTable)
	{
		delete g_pUserTable;
		g_pUserTable = NULL;
	}

	if( g_pBaseItemHash )
	{
		g_pBaseItemHash->Destroy( TRUE );
		delete g_pBaseItemHash;
		g_pBaseItemHash	= NULL;
	}

	for(int i=0;i<9;i++)
		g_pNet->PauseTimer(i);
	
	g_bIsServerRunning = FALSE;	

	if(hWnd)
	{
		ShowWindow(hWnd,SW_MINIMIZE);
		ShowWindow(hWnd,SW_NORMAL);
	}	
}

void __stdcall ShutDownServer(DWORD dwVal)
{
	dwVal;

	Log(LOG_IMPORTANT,"ShutDownServer()");

#ifdef USE_LICENSING_SERVER
	DisconnectLicensingServer();
#endif

	if( IS_ABLE_SERVICE_TYPE(ST_CHINA_BILLING) )//중국빌링 : 050104 hwoarang
	{
		DisconnectBillingServer(); // 빌링 서버와의 연결을 끊고, 서버를 종료시킨다.
	}
	else
	{
		TerminateServer();
	}
}

void ListenForUserside()
{
	if(g_pThis->m_bUserListening)
	{
		Log(LOG_IMPORTANT, "@ Userside already listening!");
	}
	else
	{
		if(!g_pNet->StartServerWithUserSide(g_pThis->GetIPForUser(), g_pThis->GetPortForUser()))
		{
			Log(LOG_IMPORTANT, "@ Can Not Listen For Userside! IP=\"%s\", Port=%d"
				,g_pThis->GetIPForUser(), g_pThis->GetPortForUser());
			return;
		}

		g_pThis->m_bUserListening = TRUE;
		Log(LOG_IMPORTANT, "@ Now Listening... (For Userside(%d))", g_pThis->GetPortForUser());

		ReportToCMListener(2, &g_MachineName);
	}
}

void ListenForServerside()
{
	if(g_pThis->m_bServerListening)
	{
		Log(LOG_JUST_DISPLAY, "@ Serverside already listening!");
	}
	else
	{
		if(!g_pNet->StartServerWithServerSide(g_pThis->GetIPForServer(), g_pThis->GetPortForServer()))
		{
			Log(LOG_NORMAL, "@ Can Not Listen For Serverside! IP=\"%s\", Port=%d"
				,g_pThis->GetIPForServer(), g_pThis->GetPortForServer());
			return;
		}

		g_pThis->m_bServerListening = TRUE;
		Log(LOG_IMPORTANT, "@ Now Listening... (For Serverside(%d))", g_pThis->GetPortForServer());

		ReportToCMListener(1, &g_MachineName);
	}		
}

bool ConnectToDBServer()
{
	char szDbName[0xff]={0,};

	GAME_DB = g_pDb->CreateDBConnection();

	wsprintf( szDbName, "%s%s", g_DBInfo.Get(DT_GAME_DB,DBIT_CATALOG), g_pThis->GetServerSetCode() );	
	
	Log(LOG_IMPORTANT, "@ Connecting SQL Server(Game DB)...");
	
	{
		const char* ip = g_pThis->GetIPForGameDB();
		const char* username = g_DBInfo.Get(DT_GAME_DB, DBIT_ID);
		const char* pw = g_DBInfo.Get(DT_GAME_DB, DBIT_PW);
		if (g_pDb->Connect(ip, szDbName,
			username,
			pw, 20, /*10, FALSE,*/ (BYTE)GAME_DB) < 0)
		{
			Log(LOG_IMPORTANT, "@ Can not connect SQL Server(Game DB)! IP=\"%s\"", g_pThis->GetIPForGameDB());
			return false;
		}
	}
	
	Log(LOG_IMPORTANT, "@ SQL Server(Game DB) Connected!(%s)", g_pThis->GetIPForGameDB());
	
	g_pDb->SetDefaultDBIndex((BYTE)GAME_DB);

	//Syste DB
	TOTAL_DB = g_pDb->CreateDBConnection();
	{
		Log(LOG_JUST_DISPLAY, "@ Connecting SQL Server(Total DB)...");
		const char* ip = g_pThis->GetIPForTotalDB();
		const char* id = g_DBInfo.Get(DT_TOTAL_DB, DBIT_ID);
		const char* db = g_DBInfo.Get(DT_TOTAL_DB, DBIT_CATALOG);
		const char* pw = g_DBInfo.Get(DT_TOTAL_DB, DBIT_PW);
		if (g_pDb->Connect(ip,
			db,
			id,
			pw,
			20, /*10, FALSE,*/ (BYTE)TOTAL_DB) < 0)
		{
			Log(LOG_IMPORTANT, "@ Can not connect SQL Server(TOTAL_DB)! IP=\"%s\"", g_pThis->GetIPForTotalDB());
			return false;
		}
	}

	Log(LOG_IMPORTANT, "@ SQL Server(Total DB) Connected!(%s)", g_pThis->GetIPForTotalDB());
		
	//Member DB
	MEMBER_DB = g_pDb->CreateDBConnection();
	{
		Log(LOG_JUST_DISPLAY, "@ Connecting SQL Server(Member DB)...");
		const char* ip = g_pThis->GetIPForMemberDB();
		const char* db = g_DBInfo.Get(DT_MEMBER_DB, DBIT_CATALOG);
		const char* id = g_DBInfo.Get(DT_MEMBER_DB, DBIT_ID);
		const char* pw = g_DBInfo.Get(DT_MEMBER_DB, DBIT_PW);
		if (g_pDb->Connect(ip,
			db,
			id,
			pw,
			20, /*10, FALSE,*/ (BYTE)MEMBER_DB) < 0)
		{
			Log(LOG_IMPORTANT, "@ Can not connect SQL Server(MEMBER_DB)! IP=\"%s\"", g_pThis->GetIPForMemberDB());
			return false;
		}
	}


	Log(LOG_IMPORTANT, "@ SQL Server(Member DB) Connected!(%s)", g_pThis->GetIPForMemberDB());

	// Bill DB Connection
	BILL_DB = g_pBillDb->CreateDBConnection();
	
	{
		Log(LOG_IMPORTANT, "@ Connecting SQL Server(Billing DB)...");
		const char* ip = g_pThis->GetIPForMemberDB();
		const char* db = g_DBInfo.Get(DT_MEMBER_DB, DBIT_CATALOG);
		const char* id = g_DBInfo.Get(DT_MEMBER_DB, DBIT_ID);
		const char* pw = g_DBInfo.Get(DT_MEMBER_DB, DBIT_PW);
		if (g_pBillDb->Connect(ip,
			db,
			id,
			pw, 20, /*10, FALSE,*/ (BYTE)BILL_DB) < 0)
		{
			Log(LOG_IMPORTANT, "@ Can not connect SQL Server(BILL_DB)! IP=\"%s\"", g_pThis->GetIPForMemberDB());
			return false;
		}
	}

	Log(LOG_IMPORTANT, "@ SQL Server(Member DB) Connected!(%s)", g_pThis->GetIPForMemberDB());

	
	// Log DB Connection
	LOG_DB = g_pLogDb->CreateDBConnection();
	Log(LOG_IMPORTANT, "@ Connecting SQL Server(Log DB)...");
	{
		char szLogDBName[0xff] = { 0, };

		if (!IS_ABLE_SERVICE_TYPE(ST_NO_LOG))// : hwoarang 050201 
		{
			::wsprintf(szLogDBName, "%s%d", g_DBInfo.Get(DT_LOG_DB, DBIT_CATALOG), g_pThis->GetServerSetNum());

			const char* ip = g_pThis->GetIPForLogDB();
			const char* id = g_DBInfo.Get(DT_LOG_DB, DBIT_ID);
			const char* pw = g_DBInfo.Get(DT_LOG_DB, DBIT_PW);
			if (g_pLogDb->Connect(g_pThis->GetIPForLogDB(),
				szLogDBName,
				id,
				pw,
				20, /*10, FALSE,*/ (BYTE)LOG_DB) < 0)
			{
				Log(LOG_NORMAL, "@ Can not connect SQL Server(Log DB)! IP=\"%s\"", g_pThis->GetIPForLogDB());
				return false;
			}
			Log(LOG_IMPORTANT, "@ SQL Server(Log DB) Connected!(%s)", g_pThis->GetIPForLogDB());
		}
	}
	
	
	//Logintable 삭제 
	ThrowLogoutAllQuery(FALSE);
	return true;
}

BOOL QueryAllServer()
{
	Log(LOG_JUST_DISPLAY, "@ Loading All WorldServer Informations...");

	//World서버에 접속할 서버 정보를 DB에서 쿼리 
	char szQuery[ QUERY_STRING_SIZE ]={0,};
	wsprintf(szQuery, "Select WorldMapID, IPForUser, PortForUser, IPForServer, PortForServer from WorldServerTable Where ServerSet = %s", g_pThis->GetServerSetCode());
		
	ServerRecord RecordSet[ MAX_WORLD_SERVER_NUM ];
	memset(RecordSet, 0, sizeof(RecordSet));
	int nRet = g_pDb->OpenRecord(szQuery, RecordSet, MAX_WORLD_SERVER_NUM, (BYTE)GAME_DB);

	if(nRet < 0)
	{
		Log(LOG_IMPORTANT, "Fail To Query WorldServer Informations!");
		return FALSE;
	}

	{
		WORLD_SERVER_DATA* pServer = g_pServerTable->AllocNewServer(1);

		if (!pServer)
		{
			Log(LOG_IMPORTANT, "WorldServer Alloc Failed! (dwWorldMapID:%d)", 1);
			return FALSE;
		}

		pServer->wPortForUser = 13201;
		pServer->dwIPForUser = inet_addr("127.0.0.1");
		pServer->wPortForServer = 17201;
		pServer->dwIPForServer = inet_addr("127.0.0.1");
		memcpy(pServer->szIPForServer, "127.0.0.1", MAX_IP_LENGTH);
	}

	/*
	for(int i=0; i<nRet; i++)
	{
		WORLD_SERVER_DATA* pServer = g_pServerTable->AllocNewServer(RecordSet[i].dwWorldMapID);

		if(!pServer)
		{
			Log(LOG_IMPORTANT, "WorldServer Alloc Failed! (dwWorldMapID:%d)", RecordSet[i].dwWorldMapID);
			return FALSE;
		}

		pServer->wPortForUser	= RecordSet[i].wPortForUser;
		pServer->dwIPForUser	= inet_addr(RecordSet[i].szIpForUser);
		pServer->wPortForServer = RecordSet[i].wPortForServer;
		pServer->dwIPForServer = inet_addr(RecordSet[i].szIpForServer);
		memcpy(pServer->szIPForServer, RecordSet[i].szIpForServer, MAX_IP_LENGTH);
	}
	*/

	return TRUE;
}

BOOL QueryBaseClassInfo()
{
	// Get BaseClassInfo
	if(g_pDb->OpenRecord( "CZP_QUERY_BASECLASSINFO", g_pUserTable->GetBaseClassInfo(1), MAX_CLASS, (BYTE)GAME_DB ) < 0)
	{
		Log(LOG_IMPORTANT, "Fail to query BaseClassInfo!");
		return FALSE;
	}

	return TRUE;
}

BOOL InitItemData()
{
	DBRECEIVEDATA*		pData		= 0;
	CBaseItem*			pBaseItem	= 0;
	
	// BaseItem Hash 테이블 초기화 
	g_pBaseItemHash = new BASEITEM_HASH;
	g_pBaseItemHash->InitializeHashTable( 3000, 2000 );
	SetCommonServerBaseItemHash(g_pBaseItemHash);
	Log(LOG_JUST_DISPLAY, "@ Loading Item Informations...");
	
	g_pBaseItemHash->Destroy( TRUE );

	// Item Weapon..
	pData = g_pDb->OpenRecordEx("Select * from Item_Weapon", 1000, DEFAULT_ROWS_PER_READ, (BYTE)GAME_DB );
	for(DWORD i=0; i<pData->Query.select.dwRowCount; i++)
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize ); 
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}
	g_pDb->ReleaseRecordset(pData);

	// Item Consumable..
	pData = g_pDb->OpenRecordEx("Select * from Item_Consumable", 1000, DEFAULT_ROWS_PER_READ, (BYTE)GAME_DB );
	for(int i=0; i<pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize );
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}
	g_pDb->ReleaseRecordset(pData);

	// Item Armor..
	pData = g_pDb->OpenRecordEx("Select * from Item_Armor", 1000, DEFAULT_ROWS_PER_READ, (BYTE)GAME_DB );
	for(int i=0; i<pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize );
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}
	g_pDb->ReleaseRecordset(pData);
	
	// Item Supplies..
	pData = g_pDb->OpenRecordEx("Select * from Item_Supplies", 500, DEFAULT_ROWS_PER_READ, (BYTE)GAME_DB );
	for(int i=0; i<pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize );
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}
	g_pDb->ReleaseRecordset(pData);
	
	// Item Zodiac..
	pData = g_pDb->OpenRecordEx("Select * from Item_Zodiac", 500, DEFAULT_ROWS_PER_READ, (BYTE)GAME_DB );
	for(int i=0; i<pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize );
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}
	g_pDb->ReleaseRecordset(pData);
	
	// Item Ride //
	pData = g_pDb->OpenRecordEx("Select * from Item_Ride", 500, DEFAULT_ROWS_PER_READ, (BYTE)GAME_DB );
	for(int i=0; i<pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize );
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}
	g_pDb->ReleaseRecordset(pData);
	
	// Item Special //
	pData = g_pDb->OpenRecordEx("Select * from Item_Special", 500, DEFAULT_ROWS_PER_READ, (BYTE)GAME_DB );
	for(int i=0; i<pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize );
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}
	g_pDb->ReleaseRecordset(pData);
	
	// Item Guardian //
	pData = g_pDb->OpenRecordEx("Select * from Item_Guardian", 500, DEFAULT_ROWS_PER_READ, (BYTE)GAME_DB );
	for(int i=0; i<pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize );
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}
	g_pDb->ReleaseRecordset(pData);

	// Item MagicArray..
	pData = g_pDb->OpenRecordEx("Select * from Item_Magic_Array", 500, DEFAULT_ROWS_PER_READ, (BYTE)GAME_DB );
	for(int i=0; i<pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize );
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}
	g_pDb->ReleaseRecordset(pData);

	// Item Materials //
	pData = g_pDb->OpenRecordEx("Select * from Item_Materials", 500, DEFAULT_ROWS_PER_READ, (BYTE)GAME_DB );
	for(int i=0; i<pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize );
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}
	g_pDb->ReleaseRecordset(pData);

	// Item Mix Upgrade //
	pData = g_pDb->OpenRecordEx("Select * from Item_Mix_Upgrade", 500, DEFAULT_ROWS_PER_READ, (BYTE)GAME_DB );
	for(int i=0; i<pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize );
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}
	g_pDb->ReleaseRecordset(pData);

	// Item MagicFieldArray //
	pData = g_pDb->OpenRecordEx("Select * from Item_MagicField_Array", 500, DEFAULT_ROWS_PER_READ, (BYTE)GAME_DB );
	for(int i=0; i<pData->Query.select.dwRowCount; i++ )
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize );
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}
	g_pDb->ReleaseRecordset(pData);
	
	// Item Upgrade //
	pData = g_pDb->OpenRecordEx("Select * from Item_Upgrade",500,DEFAULT_ROWS_PER_READ,(BYTE)GAME_DB);
	for(int i=0; i<pData->Query.select.dwRowCount; i++)
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize );
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}
	g_pDb->ReleaseRecordset(pData);

	// Item liquid //
	pData = g_pDb->OpenRecordEx("Select * from Item_liquid",500,DEFAULT_ROWS_PER_READ,(BYTE)GAME_DB);
	for(int i=0; i<pData->Query.select.dwRowCount; i++)
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize );
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}
	g_pDb->ReleaseRecordset(pData);

	// Item Edition //
	pData = g_pDb->OpenRecordEx("Select * from Item_Edition",500,DEFAULT_ROWS_PER_READ,(BYTE)GAME_DB);
	for(int i=0; i<pData->Query.select.dwRowCount; i++)
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize );
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}
	g_pDb->ReleaseRecordset(pData);

	// Item Bag // 
	pData = g_pDb->OpenRecordEx("Select * from Item_Bag", 500, DEFAULT_ROWS_PER_READ, (BYTE)GAME_DB );
	for(int i=0; i < pData->Query.select.dwRowCount; i++)
	{
		pBaseItem = new CBaseItem;
		memset( pBaseItem, 0, sizeof(CBaseItem) );
		memcpy( pBaseItem, pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize ); 
		g_pBaseItemHash->Insert( pBaseItem, pBaseItem->wID );
	}	
	g_pDb->ReleaseRecordset(pData);	
	
	Log(LOG_JUST_DISPLAY, "@ Loading Item Informations successfully!");
	return TRUE;
}


void CreateItem(CItem* pItem, BYTE byCount)
{
	int iRand = 0, iSlot = 0, i = 0, k = 0, iValue = 0;
	int nRandSlot = 0;
	
	CBaseItem *pBaseItem = g_pBaseItemHash->GetData(pItem->m_wItemID);
	if(!pBaseItem)	return;

	switch(GetItemKind(pItem->GetID()))
	{	
		case ITEM_KIND_WEAPON:
		{
			pItem->m_Item_Weapon.wMin_Damage	= pBaseItem->BaseItem_Weapon.wMin_Damage;
			pItem->m_Item_Weapon.wMax_Damage	= pBaseItem->BaseItem_Weapon.wMax_Damage;
			pItem->m_Item_Weapon.wMana_Damage	= pBaseItem->BaseItem_Weapon.wMana_Damage;

			iSlot = pBaseItem->BaseItem_Weapon.wMax_Slot - pBaseItem->BaseItem_Weapon.wMin_Slot;
			pItem->m_bSlot.uMaxSlot = pBaseItem->BaseItem_Weapon.wMin_Slot;
			nRandSlot = rand()%( iSlot + 1 );
			pItem->m_bSlot.uMaxSlot = pItem->m_bSlot.uMaxSlot + (BYTE)nRandSlot;

			if( pBaseItem->BaseItem_Weapon.wSetID != 0 )	
			{
				k = 0;

				for(int i=0; i < 6; i++ )
				{
					if( pBaseItem->BaseItem_Weapon.pBaseSetItemOption[i].wKind == 0 )
					{
						continue;
					}
					
					iValue =	pBaseItem->BaseItem_Weapon.pBaseSetItemOption[i].wMax -
								pBaseItem->BaseItem_Weapon.pBaseSetItemOption[i].wMin;

					if( iValue > 0 )
					{
						pItem->wAttr[k].uValue = pBaseItem->BaseItem_Weapon.pBaseSetItemOption[i].wMin;
						iRand = rand()%(iValue+1);
						pItem->wAttr[i].uValue = pItem->wAttr[i].uValue + (WORD)iRand;
					}
					else
					{
						pItem->wAttr[k].uValue = pBaseItem->BaseItem_Weapon.pBaseSetItemOption[i].wMin;
					}
					
					if( pItem->wAttr[i].uValue != 0 )
					{
						pItem->wAttr[k].uCode = pBaseItem->BaseItem_Weapon.pBaseSetItemOption[i].wKind;
						k++;
					}
				}				
			}
		}
		break;

		case ITEM_KIND_AROMR:
		{
			pItem->m_Item_Armor.wDefense	= pBaseItem->BaseItem_Armor.wMin_Damage;
			
			iSlot = pBaseItem->BaseItem_Armor.wMax_Slot - pBaseItem->BaseItem_Armor.wMin_Slot;
			pItem->m_bSlot.uMaxSlot = pBaseItem->BaseItem_Armor.wMin_Slot;
			nRandSlot = rand()%( iSlot + 1 );
			pItem->m_bSlot.uMaxSlot = pItem->m_bSlot.uMaxSlot + (BYTE)nRandSlot;

			if( pBaseItem->BaseItem_Armor.wSetID != 0 )	
			{
				k = 0;
				for(int i=0; i < 6; i++ )
				{
					if( pBaseItem->BaseItem_Armor.pBaseSetItemOption[i].wKind == 0 )
					{
						continue;
					}
					
					iValue =	pBaseItem->BaseItem_Armor.pBaseSetItemOption[i].wMax -
								pBaseItem->BaseItem_Armor.pBaseSetItemOption[i].wMin;

					if( iValue > 0 )
					{
						pItem->wAttr[k].uValue = pBaseItem->BaseItem_Armor.pBaseSetItemOption[i].wMin;
						iRand = rand()%(iValue+1);
						pItem->wAttr[i].uValue = pItem->wAttr[i].uValue + (WORD)iRand;
					}
					else
					{
						pItem->wAttr[k].uValue = pBaseItem->BaseItem_Armor.pBaseSetItemOption[i].wMin;
					}
					
					if( pItem->wAttr[k].uValue != 0 )
					{
						pItem->wAttr[k].uCode = pBaseItem->BaseItem_Armor.pBaseSetItemOption[i].wKind;
						k++;
					}
				}				
			}
		}
		break;	

		case ITEM_KIND_BAG :
		{// 마일리지 몰에서 가방아이템 추가 김영대 2005.03.04
			pItem->m_Item_Bag.bType	= pBaseItem->BaseItem_Bag.bType;
			WORD wProbability = pBaseItem->BaseItem_Bag.WValue_Max - pBaseItem->BaseItem_Bag.wValue_Min;
			pItem->m_Item_Bag.wProbability = pBaseItem->BaseItem_Bag.wValue_Min;
			pItem->m_Item_Bag.wProbability += rand() % ( wProbability + 1 );	
		}
		break;
			
		case ITEM_KIND_GUARDIAN:	break;
		case ITEM_KIND_RIDE:		break;	
		
		case ITEM_KIND_CONSUMABLE:
		{
			if(ITEM_LAND_TYPE_SERVERTIME == pBaseItem->GetLandItemType())
			{
				pItem->m_Item_Consumable.dwEndDate	= GetDueDay(pBaseItem->GetLastTime());
				pItem->m_Item_Consumable.bInvIndex	= 1;
			}		
			
			pItem->SetQuantity(byCount);
		}
		break;

		case ITEM_KIND_ZODIAC:
		{
			if(	pItem->m_wItemID==__ITEM_PORTAL_VILL1__ ||
				pItem->m_wItemID==__ITEM_PORTAL_VILL2__ ||
				pItem->m_wItemID==__ITEM_PORTAL_VILL3__ ||
				pItem->m_wItemID==__ITEM_PORTAL_VILL4__ ||
				pItem->m_wItemID==__ITEM_PORTAL_VILL5__)
			{
				if(pItem->m_wItemID==__ITEM_PORTAL_VILL1__)
				{
					pItem->m_Item_Zodiac.wMapId	= 1;
					pItem->m_Item_Zodiac.wPosX	= 0;
					pItem->m_Item_Zodiac.wPosZ	= 0;
				}
				else if(pItem->m_wItemID==__ITEM_PORTAL_VILL2__)
				{
					pItem->m_Item_Zodiac.wMapId	= 3;
					pItem->m_Item_Zodiac.wPosX	= 0;
					pItem->m_Item_Zodiac.wPosZ	= 0;
				}
				else if(pItem->m_wItemID==__ITEM_PORTAL_VILL3__)
				{
					pItem->m_Item_Zodiac.wMapId	= 4;
					pItem->m_Item_Zodiac.wPosX	= 0;
					pItem->m_Item_Zodiac.wPosZ	= 0;
				}
				else if(pItem->m_wItemID==__ITEM_PORTAL_VILL4__)
				{
					pItem->m_Item_Zodiac.wMapId	= 5;
					pItem->m_Item_Zodiac.wPosX	= 0;
					pItem->m_Item_Zodiac.wPosZ	= 0;
				}
				else if(pItem->m_wItemID==__ITEM_PORTAL_VILL5__)
				{
					pItem->m_Item_Zodiac.wMapId	= 6;
					pItem->m_Item_Zodiac.wPosX	= 0;
					pItem->m_Item_Zodiac.wPosZ	= 0;
				}

				pItem->m_Item_Zodiac.bLayer	= 0;
				pItem->m_Item_Zodiac.bType	= 1;
			}

			pItem->SetQuantity(byCount);
		}
		break;

		case ITEM_KIND_LAND:
		{
			
		}
		break;
		
		case ITEM_KIND_MAGICARRAY:	
		case ITEM_KIND_MIXUPGRADE:
		case ITEM_KIND_MAGICFIELDARRAY:
		case ITEM_KIND_SPECIAL:
		case ITEM_KIND_MATERIALS:
		case ITEM_KIND_SUPPLIES:
		case ITEM_KIND_UPGRADE:
		case ITEM_KIND_LIQUID:
		case ITEM_KIND_EDITION:
			pItem->SetQuantity(byCount);
		break;		
	}	
}


time_t GetDueDay(int nEndMin)
{
	struct tm when;
	time_t now;

	time(&now);
	when = *localtime(&now);
	when.tm_min += nEndMin;
	now = mktime(&when);
	
	return now;	
}


int GetFreeTime(DBTIMESTAMP* pRecentLogoutTime, int nRemainedDBFreeTime)
{
	int nFreeTimePerDay;

	// DBTimeStamp형의 최근로그아웃 시간을 CTime형으로 변환 
	CTime RecentLogoutTime(pRecentLogoutTime->year, pRecentLogoutTime->month, pRecentLogoutTime->day,
							pRecentLogoutTime->hour, pRecentLogoutTime->minute, pRecentLogoutTime->second);

	// 현재 시간 구함 
	SYSTEMTIME CurrentTime;
	GetSystemTime(&CurrentTime);
	CTime RefreshTime;

	if(CurrentTime.wHour >= 6)		//현재 시간 새벽 6이후 (갱신시간이후)
	{
		//현재일을 기준으로 한 무료 1시간 갱신 Time
		CTime RefreshTime( CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay, 6, 0, 0 );	
				
		//갱신시간 이전에 로그아웃 한유저라면 새롭게 1시간 무료시간을 준다.
		if(RecentLogoutTime < RefreshTime)	
			nFreeTimePerDay = 3600;
		else//DB에 있는값이 그대로 남아있는 시간으로 적용된다. 
			nFreeTimePerDay = nRemainedDBFreeTime;				
	}
	else
	{
		//현재일을 기준으로 한 무료 1시간 갱신 Time
		CTime TempTime( CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay, 6, 0, 0 );	
		CTime RefreshTime = TempTime - CTimeSpan(1,0,0,0);	//1 day before
				
		//갱신시간 이전에 로그아웃 한유저라면 새롭게 1시간 무료시간을 준다.
		if(RecentLogoutTime < RefreshTime)	
			nFreeTimePerDay = 3600;
		else//DB에 있는값이 그대로 남아있는 시간으로 적용된다. 
			nFreeTimePerDay = nRemainedDBFreeTime;		
	}
	
	return nFreeTimePerDay;
}


void SendToUserLoginFailPacket( CUser* pUser, BYTE bResult, DWORD	dwExData )
{
	//	0: 비번틀림
	//	1: 로그인 성공(사용안함)
	//	2: 없는아이디
	//	3: 아이디사용중
	//	4: 서버 최대인원 초과
	//	5: 테스트 기간에 접속할 수 없는 아이디, 
	//	6: 버전틀림
	//	7: 현재 서버에서 로그인 시도중
	//	8: 캐릭터 자리 겹침 

	WSTC_LOGIN_FAIL	FailPacket;
	FailPacket.bResult	= bResult;
	FailPacket.dwExData	= dwExData;
	SendToUser( pUser, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}


//--------------------------------------------------------------------------
// 이미 UserHash에 남아있는데 또 로그인 시도가 되었을 경우 처리 
//--------------------------------------------------------------------------
void PrevUserProcess( DWORD dwPropID )
{
	CUser* pPrevUser = (CUser*)g_pUserTable->GetUserInfo( dwPropID );

	while(pPrevUser)
	{
		Log(LOG_NORMAL, "User Already Exists in UserHash Table! (CharIndex:%d)", pPrevUser->m_dwChar_Index);
		
		if(pPrevUser->m_dwChar_Index)
		{
			ASTWS_KICK_USER	KickPacket;
			//다른 월드나 던전 서버에 유저가 있다면 모두 종료시켜 버려라 
			KickPacket.dwCharIndex = pPrevUser->m_dwChar_Index;
			g_pServerTable->BroadCastWorldServer((char*)&KickPacket, KickPacket.GetPacketSize());
		}

		// Agent에 접속되어 있는 유저 
		if(pPrevUser->m_dwCurWorldServerPos == SERVER_POS_AGENT)	
		{
			DWORD dwConnectionIndex = pPrevUser->GetConnectionIndex();
			OnDisconnectUser( dwConnectionIndex );
			g_pNet->SetUserInfo( dwConnectionIndex , NULL);
			g_pNet->CompulsiveDisconnectUser( dwConnectionIndex );
			
			pPrevUser = (CUser*)g_pUserTable->GetUserInfo( dwPropID );
			g_pUserTable->Remove(pPrevUser);					
		}
		else
		{
			//유저 테이블에서 이 유저 삭제 
			g_pUserTable->Remove(pPrevUser);
		}
		
		pPrevUser = (CUser*)g_pUserTable->GetUserInfo( dwPropID );
	}
}


BOOL SetBillingOrder(BILLING_INFO* pBillInfo)
{
	if(!pBillInfo)	return FALSE;

	int cnt = 0;
	
	if(pBillInfo->bUserBillType	== USER_BILLING_ABSOLUTELY_FREE)
	{
		pBillInfo->BillingOrder[0] = USER_BILLING_ABSOLUTELY_FREE;
	}
	else
	{
		//하루 한시간 무료 갱신 
		pBillInfo->dwTimeFor1HourFree = GetFreeTime(&pBillInfo->RecentLogout, pBillInfo->dwTimeFor1HourFree);
		
		//1순위: PC방 정액제 
		if(pBillInfo->bPCRoom && pBillInfo->bPCRoomBillType == PCROOM_BILLING_FLAT_RATE)
			pBillInfo->BillingOrder[cnt++] = PCROOM_BILLING_FLAT_RATE;
			
		//2순위: 개인 정액제 
		if(pBillInfo->bUserBillType == USER_BILLING_FLAT_RATE)	
			pBillInfo->BillingOrder[cnt++] = USER_BILLING_FLAT_RATE;

		//3순위: 개인 하루 1시간 무료 
		if(pBillInfo->dwTimeFor1HourFree > 0)
			pBillInfo->BillingOrder[cnt++] = USER_BILLING_1HOUR_FREE;

		//4순위: PC방 종량제 
		if(pBillInfo->bPCRoom && pBillInfo->bPCRoomBillType == PCROOM_BILLING_METER_RATE)
			pBillInfo->BillingOrder[cnt++] = PCROOM_BILLING_METER_RATE;
		
		//5순위: 개인 종량제 
		if(pBillInfo->bUserBillType == USER_BILLING_METER_RATE)
			pBillInfo->BillingOrder[cnt++] = USER_BILLING_METER_RATE;

		//6순위: 쿠폰 
		//if(pBillInfo->bUserBillType == USER_BILLING_COUPON)
		//	pBillInfo->BillingOrder[cnt++] = USER_BILLING_COUPON;
	}
	
	
	return TRUE;
}

void __stdcall ShowAllServerStatus(DWORD dwEventIndex)
{
	dwEventIndex;

	g_pServerTable->ShowAllServerStatus();
}

void __stdcall ShowCurUserCount(DWORD dwEventIndex)
{
	dwEventIndex;

	Log(LOG_JUST_DISPLAY, "Current User(Logined/Connection): %d / %d", 
		g_pUserTable->GetCount(), g_pNet->GetConnectedUserNum() );
}

void __stdcall StartServerWithUserSide(DWORD dwEventIndex)
{
	dwEventIndex;

	ListenForUserside();	
}

void __stdcall BillingCheck(DWORD dwEventIndex)
{
	// 대만은 빌링체크함.
	switch( MY_NATION_CODE )
	{
	case NC_JAPAN:
	case NC_CHINA:	
		{
			return;
		}break;
	}
	
	if( !IS_ABLE_SERVICE_TYPE(ST_BILLING) )//일반빌링아니면 :050104 hwoarang
	{
		return;
	}

	if(!g_pUserTable)	return;
	if(!g_pUserTable->GetUserList())	return;
	
	POSITION_ pos = g_pUserTable->GetUserList()->GetHeadPosition();

	DWORD dwTick = timeGetTime();
	DWORD dwSec=0;
	DWORD dwCount = 0;
	char szQuery[QUERY_STRING_SIZE]={0,};
	CUser* pUser=0;
	
	while(pos)
	{
		pUser = (CUser*)g_pUserTable->GetUserList()->GetNext(pos);

		if(!pUser->m_dwLastBillingCheckTick) continue;		//아직 월드맵에 안들어간 유저 

		//TickCount가 4Byte 값을 넘겼을경우 
		if(dwTick < pUser->m_dwLastBillingCheckTick)		//4Byte 값 돌아간 경우 	
		{
			dwSec = 0xffffffff - pUser->m_dwLastBillingCheckTick;
			dwSec += dwTick;
		}
		else
			dwSec = dwTick - pUser->m_dwLastBillingCheckTick;	//정상적인 경우 

		if(dwSec >= BILLING_CHECK_USER)	//마지막으로 빌링 체크한지 5분이상 되었으면 
		{
			//TickCount가 4Byte 값을 넘겼을경우 
			if(dwTick < pUser->m_dwBillingStartTick)		//4Byte 값 돌아간 경우 	
			{
				dwSec = 0xffffffff - pUser->m_dwBillingStartTick;
				dwSec += dwTick;
			}
			else
				dwSec = dwTick - pUser->m_dwBillingStartTick;	//정상적인 경우 

			dwSec = dwSec / 1000;
			wsprintf(szQuery, "EXEC up_UserCheck %d, %d, %d, '%s'", pUser->m_bBillingType, pUser->GetPropID(), dwSec, pUser->GetIPString() );
			g_pBillDb->THOpenRecord(szQuery, QUERY_TYPE_BILLING_CHECK, (void*)pUser->m_dwConnectionIndex, 1, (BYTE)BILL_DB);
			pUser->m_dwLastBillingCheckTick = dwTick;
			dwCount++;

			if( IS_ABLE_SERVICE_TYPE(ST_DEVELOP) )//DEV_MODE : 050104 Hwoarang
			{
				OutputDebugString(szQuery);
				OutputDebugString("\n");
			}
		}
	}

	Log(LOG_NORMAL, "[Billing Check] Total %d user checked!", dwCount);

	//최근 체크 타임 
	g_RecentCheckTime = CTime::GetCurrentTime();
}


void __stdcall StartServerWithServerSide(DWORD dwEventIndex)
{
	dwEventIndex;

	ListenForServerside();	
}

void __stdcall LogOutAllUser(DWORD dwEventIndex)
{
	dwEventIndex;
	
	ThrowLogoutAllQuery();
}

void __stdcall IncreaseUserLimit(DWORD dwEventIndex)
{
	dwEventIndex;

	g_pThis->SetMaxUserAllowedNum( g_pThis->GetMaxUserAllowedNum() + 100 );
	Log(LOG_JUST_DISPLAY, "Current User Limit : %d", g_pThis->GetMaxUserAllowedNum());
}

void __stdcall DecreaseUserLimit(DWORD dwEventIndex)
{
	dwEventIndex;

	g_pThis->SetMaxUserAllowedNum( g_pThis->GetMaxUserAllowedNum() - 100 );
	Log(LOG_JUST_DISPLAY, "Current User Limit : %d", g_pThis->GetMaxUserAllowedNum());
}


void InitNetwork()
{
	/*
* 				F1 -- key[1]--event2, (idx+1)
		   F3 -- key[0] --event1
		   F5 -- [2]
		   F7 -- [4] -- logout all user
		   F11 -- [5]
		   F12 -- [6]
*/
	DWORD dwEventCount = 0;
	CUSTOM_EVENT *ev = new CUSTOM_EVENT[32];

	memset(ev, 0, sizeof(CUSTOM_EVENT)*32);

	ev[0].dwPeriodicTime = 0;
	ev[0].pEventFunc = GetDBResult;
	dwEventCount++;

	ev[1].dwPeriodicTime = 0;
	ev[1].pEventFunc = ShowAllServerStatus;
	dwEventCount++;
	
	ev[2].dwPeriodicTime = 0;
	ev[2].pEventFunc = ShowCurUserCount;
	dwEventCount++;

	ev[3].dwPeriodicTime = 0;
	ev[3].pEventFunc = StartServerWithUserSide;
	dwEventCount++;

	ev[4].dwPeriodicTime = 0;
	ev[4].pEventFunc = StartServerWithServerSide;
	dwEventCount++;

	ev[5].dwPeriodicTime = 0;
	ev[5].pEventFunc = LogOutAllUser;
	dwEventCount++;

	ev[6].dwPeriodicTime = 0;
	ev[6].pEventFunc = DecreaseUserLimit;
	dwEventCount++;

	ev[7].dwPeriodicTime = 0;
	ev[7].pEventFunc = IncreaseUserLimit;
	dwEventCount++;

	ev[8].dwPeriodicTime = 0;
	//ev[8].pEventFunc = GetLogDBResult;
	ev[8].pEventFunc = ShutDownServer;
	dwEventCount++;

	ev[9].dwPeriodicTime = 0;
	ev[9].pEventFunc = GetBillingDBResult;
	dwEventCount++;

	ev[10].dwPeriodicTime = BILLING_PROCESS_CHECK_TICK;		//2분 
	ev[10].pEventFunc = BillingCheck;
	dwEventCount++;

	// CHINA_BILLING
	ev[CHINA_BILLING_KEEP_ALIVE_TIMER_INDEX].dwPeriodicTime	= CHINA_BILLING_KEEP_ALIVE_SEND_PERIOD; // 30초
	ev[CHINA_BILLING_KEEP_ALIVE_TIMER_INDEX].pEventFunc		= OnChinaBillingKeepAlive;
	dwEventCount++;

	ev[CHINA_BILLING_CONNECT_TIMER_INDEX].dwPeriodicTime	= CHINA_BILLING_CONNECT_PERIOD; // 10초
	ev[CHINA_BILLING_CONNECT_TIMER_INDEX].pEventFunc		= OnChinaBillingTryConnect;
	dwEventCount++;

	// LICENSING
	ev[LICENSING_HEARTBEAT_TIMER_INDEX].dwPeriodicTime	= LICENSING_HEARTBEAT_SEND_PERIOD; // 10초
	ev[LICENSING_HEARTBEAT_TIMER_INDEX].pEventFunc		= OnLicensingHeartbeat;
	dwEventCount++;

	ev[LICENSING_CONNECT_TIMER_INDEX].dwPeriodicTime	= LICENSING_CONNECT_PERIOD; // 10초
	ev[LICENSING_CONNECT_TIMER_INDEX].pEventFunc		= OnLicensingTryConnect;
	dwEventCount++;

	ev[LICENSING_SEND_INFO_INDEX].dwPeriodicTime		= LICENSING_SEND_INFO_PERIOD; // 10초
	ev[LICENSING_SEND_INFO_INDEX].pEventFunc			= OnLicensingSendInfo;
	dwEventCount++;	

	// 전병렬
	ev[dwEventCount].dwPeriodicTime		= 0; // 10초
	ev[dwEventCount].pEventFunc			= GetLogDBResult;
	dwEventCount++;	
	

	DESC_NETWORK desc;
	desc.OnAcceptUser						= OnAcceptUser;
	desc.OnAcceptServer						= OnAcceptServer;
	desc.OnDisconnectServer					= OnDisconnectServer;
	desc.OnDisconnectUser					= OnDisconnectUser;

	if( IS_ABLE_SERVICE_TYPE(ST_DEVELOP))//DEV_MODE : 050104 Hwoarang
	{
		desc.dwMainMsgQueMaxBufferSize			= 51200;
		desc.dwMaxServerNum						= 10;
		desc.dwMaxUserNum						= 100;
		desc.dwServerBufferSizePerConnection	= 25600;
		desc.dwServerMaxTransferSize			= 6500;
		desc.dwUserBufferSizePerConnection		= 6500;
		desc.dwUserMaxTransferSize				= 8192;
	}
	else
	{
		desc.dwMainMsgQueMaxBufferSize			= 5120000;

		desc.dwMaxServerNum						= 10;
		desc.dwServerBufferSizePerConnection	= 10240; // 최대 패킷 크기는 1028 바이트
		desc.dwServerMaxTransferSize			= 8192;

		desc.dwMaxUserNum						= 4000;
		desc.dwUserBufferSizePerConnection		= 10240; // 최대 패킷 크기는 1028 바이트
		desc.dwUserMaxTransferSize				= 8192;
	}

	desc.OnRecvFromServerTCP				= OnRecvFromServerTCP;
	desc.OnRecvFromUserTCP					= OnRecvFromUserTCP;
	desc.dwCustomDefineEventNum				= dwEventCount;
	desc.dwConnectNumAtSameTime				= 5;
	desc.pEvent								= ev;
	desc.dwFlag								= 0;	

	HRESULT hr;
	//HRESULT hr = CoCreateInstance( CLSID_4DyuchiNET, NULL, CLSCTX_INPROC_SERVER, IID_4DyuchiNET, (void**)&g_pNet);
	// 패킷 암호화  g_pNet 구성 변경 
	// 2005.01.05 김영대
	//HRESULT hr;
	g_pNet = new CNTNetWork();
	if (!g_pNet->CreateNetwork(&desc, 50 , 0))
	{
		Log(LOG_NORMAL, "Fail to CreateNetwork!");
	}


	g_pThis->m_hKeyEvent[0] = g_pNet->GetCustomEventHandle(1);
	g_pThis->m_hKeyEvent[1] = g_pNet->GetCustomEventHandle(2);
	g_pThis->m_hKeyEvent[2] = g_pNet->GetCustomEventHandle(3);
	g_pThis->m_hKeyEvent[3] = g_pNet->GetCustomEventHandle(4);
	g_pThis->m_hKeyEvent[4] = g_pNet->GetCustomEventHandle(5);
	g_pThis->m_hKeyEvent[5] = g_pNet->GetCustomEventHandle(6);
	g_pThis->m_hKeyEvent[6] = g_pNet->GetCustomEventHandle(7);
	g_pThis->m_hKeyEvent[7] = g_pNet->GetCustomEventHandle(8);

	// CHINA_BILLING
	g_pNet->PauseTimer(CHINA_BILLING_KEEP_ALIVE_TIMER_INDEX);
	g_pNet->PauseTimer(CHINA_BILLING_CONNECT_TIMER_INDEX);

	// LICENSING
	g_pNet->PauseTimer(LICENSING_HEARTBEAT_TIMER_INDEX);
	g_pNet->PauseTimer(LICENSING_CONNECT_TIMER_INDEX);
	g_pNet->PauseTimer(LICENSING_SEND_INFO_INDEX);

	InitializePacketProc();	//Initialize Packet Callback Function Pointer	
}

