#include "CorumPatch.h"
#include "CorumAutopatch.h"


HANDLE	hOnlyInstance = NULL;

#ifdef CHINA_LOCALIZING
BOOL CheckRegionSelectNeeded()
{
	int		iRegionID = -1;
	LPCSTR* ppArgv = (LPCSTR*) __argv;
	int		nArgc = __argc;	

	for(int i = 1; i < nArgc; i++)
	{
		if(	(_strnicmp(ppArgv[i], "-region_select", lstrlen("-region_select")) == 0) ||
			(_strnicmp(ppArgv[i], "/region_select", lstrlen("/region_select")) == 0) )
		{
			return TRUE;
		}
	}

	return FALSE;
}
#endif

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	DWORD	dwR;

#ifdef CHINA_LOCALIZING
	// cmd line ���� region select �ٽ� ������ �ϸ�, �ٽ� ������,
	if(CheckRegionSelectNeeded()) { RegionSelectAgain(); return 1; }
#endif

	// ������ �ٿ�ε� �Ͽ��� �������� ����.
	DeleteFile( g_szCorumpatchFile );

/////////////////////////////////////////////////////////////////////
//	�ݸ����� �䱸�� Directory Setting Code.	-	030924
//
	char	pszCurrentDir[MAX_PATH];
	DWORD	dwNum;
	char*	p;


	//	��������.
	if( *lpCmdLine != NULL )
	{
		wsprintf(g_pCmdLine, " %s",lpCmdLine);				//	�ݸ���.

		dwNum = GetModuleFileName( NULL, pszCurrentDir, MAX_PATH);
		if( dwNum == 0 )
		{
			MessageBox_Info( "Fail to Find - ModuleFullPath.");
			return 0;
		}

		//	---------------
		p = pszCurrentDir + dwNum - 1;

		while( *p != '\\' )	//(*p) != ('\') )
		{
			p--;
		}

		*(p+1) = NULL;
		// ---------------

		if( FALSE == SetCurrentDirectory( pszCurrentDir) )
		{
			MessageBox_Info("Fail to Set - ModuleDirectory.");
			return 0;
		}
	}
//
//
/////////////////////////////////////////////////////////////////////



	hOnlyInstance = CreateEvent( NULL, FALSE, FALSE, "Global\\CorumPatch" );
	if( GetLastError() == ERROR_ALREADY_EXISTS )
		return 1;

	DeleteFile( SELF_UPDATE_FILE);
	DeleteFile( g_szCorumpatchFile);

	dwR = GetCurrentDirectory( MAX_PATH, g_szCorumDir);
	if( dwR == 0 )
		return 2;

	//	GDI Ŭ���� ����.
	g_pcp = new CorumPatch;
	if( g_pcp == NULL )
		return 3;

	//	������ ���ϸ��� ���´�.
	if( GetVersionInfoFile( CORUM_VERSION_INFO_FILE, CorumFile ) == FALSE )
		goto lb_exit;

	//	Get Version
	memcpy( szVersion, CorumFile, VERSION_TOTAL_LENGTH);
	szVersion[VERSION_TOTAL_LENGTH] = NULL;

	//	Ftp ������Ʈ ����.
	if( Initialize_Ftp() == FALSE )
		goto lb_exit;

	//	��ũ ���� ������Ʈ ����.
	g_pZip = new CZipArchive;
	if( g_pZip == NULL )
		goto lb_exit;

	pwfd = new WIN32_FIND_DATA[ FIND_DATA_MAX];
	if( pwfd == NULL )
		goto lb_exit;

	//	���� ���̾�α� ����.
	dwR = DialogBox( hInstance, (LPCTSTR)IDD_AUTOPATCH, NULL, Autopatch);
	g_hwMain = NULL;

	if( hThread_worker )
	{
		WaitForSingleObject( hThread_worker, INFINITE);
		CloseHandle( hThread_worker);
	}

lb_exit:

	if( pwfd )
		delete pwfd;

	//	���� ������Ʈ ����.
	if( g_pZip )
		delete g_pZip;

	//	�ٿ�ε� �Ͽ��� �������� ����.
	DeleteFile( g_szCorumpatchFile );

	//	Pack ����.
	UnInitialize_PACK();

	//	Ftp ����.
	UnInitialize_Ftp();

	//	GDI Ŭ���� ����.
	if( g_pcp )
		delete g_pcp;

	if( hOnlyInstance )
		CloseHandle( hOnlyInstance);

	return 0;
}

