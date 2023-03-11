#include <process.h>
#include "CorumAutopatch.h"


//////////////////////////////////////////////////
//	Global
//////////////////////////////////////////////////
HMODULE					g_hDllModule		= NULL;
IRainFTP*				g_pFtp				= NULL;
CZipArchive*			g_pZip				= NULL;
HWND					g_hwMain			= NULL;
HANDLE					g_hSession			= NULL;
APP_PROGRESS			AP					= AP_INITIALIZED;

char					g_pCmdLine[ 1024*40]={0,};	//	넷마블에서 보낸 커맨드라인.

char					CorumFile[ MAX_PATH]={0,};
char					szVersion[ MAX_PATH]={0,};

WIN32_FIND_DATA*		pwfd				= NULL;

HANDLE					hThread_worker		= NULL;


//////////////////////////////////////////////////
//	Extract 
//////////////////////////////////////////////////
int						g_iExTotalFile		= 0;


//////////////////////////////////////////////////
//	File Storage
//////////////////////////////////////////////////
HMODULE					g_hFileStorage	= NULL;
I4DyuchiFileStorage*	g_pFileStorage	= NULL;
void*					g_hPackFile		= NULL;
char					g_szCorumDir[MAX_PATH];

int						g_iPaTotalFile	= 0;
int						g_iPaCurrentNum	= 0;
int						g_iPaCurPack	= 0;
//////////////////////////////////////////////////
//	notice
char					szNotice[MAX_PATH]={0,};
BOOL					bInit_Notice = FALSE;


//////////////////////////////////////////////////
//	WNDPROC OLD
WNDPROC					WndProc_Button_Old;


//////////////////////////////////////////////////

#ifdef CHINA_LOCALIZING
BOOL					g_bRestartRegionSelect = FALSE;
#endif

//////////////////////////////////////////////////
//
Connect_data		cd;
Directory_data		dd_get;
Directory_data		dd_set;
List_data			ld_notice;
List_data			ld_zip;
File_data			fd_notice;
File_data			fd_zip;

DWORD				dwCurrentSize=0;
DWORD				dwFileMaxSize=0;
DWORD				dwFileSizeMax=0;


BOOL CALLBACK Autopatch(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//
	DC_SET		DCset;
	char		Buf_sv[MAX_PATH];
	DWORD		dwThreadId;

	int			iR;

	HANDLE		hSession;
	PVOID		pvdata;
	//

	switch (message)
	{
	case WM_INITDIALOG:
		{
			iR = g_pFtp->Initialize( hWnd, WM_RAIN_FTP_NOTIFY, 1);
			g_hSession = g_pFtp->CreateSession();
			if( g_hSession == NULL )
			{
				EndDialog( hWnd, -1);
				return TRUE;
			}

			WndProc_Button_Old = (WNDPROC) SetWindowLong( GetDlgItem( hWnd, IDC_BTN_1), GWL_WNDPROC, (LONG)WndProc_Button);
			SetWindowLong( GetDlgItem( hWnd, IDC_BTN_2), GWL_WNDPROC, (LONG)WndProc_Button);

			g_pcp->OnInitialize( hWnd);

			/////////////////////////////////////////////////////	SC_CONNECT
			cd.pvExcuteContext			= (PVOID) SC_CONNECT;
			cd.bPassiveMode				= TRUE;			
			cd.wServerPort				= FTP_PORT;
			lstrcpy( cd.szUserName,	FTP_USER);
			lstrcpy( cd.szUserPassword, FTP_PASSWORD);

#ifdef CHINA_LOCALIZING
			lstrcpy(cd.szServerName, GetFtpServerNameChina());
#else
			lstrcpy( cd.szServerName, FTP_IP);
#endif
			/////////////////////////////////////////////////////	SC_GET_CURRENT_DIRECTORY
			dd_get.pvExcuteContext		= (PVOID) SC_GET_CURRENT_DIRECTORY;

			/////////////////////////////////////////////////////	SC_SET_CURRENT_DIRECTORY
#ifdef USE_SUBFOLDER
			// 패치의 기본 폴더를 바꾼다.
			dd_set.pvExcuteContext		= (PVOID) SC_SET_CURRENT_DIRECTORY;
			lstrcpy(dd_set.szCurrentDirectory, FTP_FOLDER);
#endif
			/////////////////////////////////////////////////////	SC_GETLIST_NOTICE
			ld_notice.pvExcuteContext	= (PVOID) SC_GETLIST_NOTICE;
			ld_notice.dwFileMax			= FIND_DATA_MAX;
			ld_notice.pFile				= pwfd;
			ld_notice.pszSearchFile		= g_szSearchNotice;

			/////////////////////////////////////////////////////	SC_DOWNLOAD_NOTICE
			fd_notice.pvExcuteContext	= (PVOID) SC_DOWNLOAD_NOTICE;
			fd_notice.bResume			= FALSE;
	
			/////////////////////////////////////////////////////	SC_GETLIST_ZIP
			ld_zip.pvExcuteContext		= (PVOID) SC_GETLIST_ZIP;
			ld_zip.dwFileMax			= FIND_DATA_MAX;
			ld_zip.pFile				= pwfd;
			ld_zip.pszSearchFile		= CorumFile;

			/////////////////////////////////////////////////////	SC_DOWNLOAD_ZIP
			fd_zip.pvExcuteContext		= (PVOID) SC_DOWNLOAD_ZIP;
			fd_zip.bResume				= TRUE;
			lstrcpy( fd_zip.szRemoteFile, CorumFile);
			lstrcpy( fd_zip.szLocalFile, g_szCorumpatchFile);


			g_pcp->OpenDC_SET( &DCset);		//	open
			AP = AP_CONNECT;
			UpdateAP( &DCset, AP);
			g_pcp->CloseDC_SET( &DCset);	//	close

			//	Connect to FTP
			iR = g_pFtp->Connect( g_hSession, &cd);
			_ASSERTE( iR == 0);

			g_hwMain = hWnd;
			return TRUE;
		}
	case WM_RAIN_FTP_NOTIFY:
		{
			hSession = g_pFtp->GetCompletionHandle();
			if( hSession == NULL )
				return TRUE;

			iR = g_pFtp->GetCompletionResult( hSession, &pvdata);
			_ASSERTE( iR == 0 );

			if( *((DWORD*)pvdata + 1) > CE_ERROR )
			{
				if( AP == AP_CONNECT )
				{
					char szMsg[] = 
#ifdef KOREAN_VERSION
						"서버에 접속할수 없습니다.\n점검중이거나 패치중인지 자세한 내용은 홈페이지에서 확인해주세요.";
#elif defined CHINA_LOCALIZING
						"轟랬젯쌈륩蛟포。\n헝瞳寮女쟁횅훰角뤠瞳쏵契郭빱샀껸땀櫓";
#else	
						"Can't connect to server.\nVisit our official site to check for inspection and patch.";
#endif

					MessageBox_Info( szMsg );					
				}

				g_pcp->OpenDC_SET( &DCset);
				//	Clear - Status Board
				ClearStatusBoard( &DCset);

				//	Status		프로그램 스테이터스
				AP = AP_INITIALIZED;
				UpdateAP( &DCset, AP);

//				UpdateEI( &DCset, EI_SHUTDOWN);

				g_pcp->btn_2.Update( DCset.hDC, BS_NONE, BSI_EXIT);

				//	Connect 버튼 활성화.
				EnableWindow( g_pcp->btn_1.hWnd, TRUE);

				g_pcp->CloseDC_SET(&DCset);
				return TRUE;
			}

			g_pcp->OpenDC_SET( &DCset);

			switch( *((DWORD*)pvdata) )
			{
			////////////////////////////////////////////////////////////////////////////////
			case SC_CONNECT:
				{
					Connect_data* pcd = (Connect_data*)pvdata;
					
					//	서버접속완료.
					//	현제 디렉토리를 얻는다.

					AP = AP_GET_DIRECTORY;
					UpdateAP( &DCset, AP);

					iR = g_pFtp->GetCurrentDirectory( g_hSession, &dd_get);
					_ASSERTE( iR == 0);

					break;
				}
			////////////////////////////////////////////////////////////////////////////////
			case SC_GET_CURRENT_DIRECTORY:
				{
					Connect_data* pcd = (Connect_data*)pvdata;
					
					//	Get Current Directory 완료.
					//	Notice.txt 를 서버에서 검색한다.

					AP = AP_GET_NOTICE_INFO;
					UpdateAP( &DCset, AP);

#ifdef USE_SUBFOLDER
					iR = g_pFtp->SetCurrentDirectory( g_hSession, &dd_set );
#else
					//	Notice.txt 를 서버에서 검색한다.
					iR = g_pFtp->GetList( g_hSession, &ld_notice);
#endif
					_ASSERTE( iR == 0 );
					break;
				}

			case SC_SET_CURRENT_DIRECTORY:
				{
					//	Notice.txt 를 서버에서 검색한다.
					iR = g_pFtp->GetList( g_hSession, &ld_notice);
					_ASSERTE( iR == 0);

					break;
				}

			////////////////////////////////////////////////////////////////////////////////
			case SC_GETLIST_NOTICE:	//	getlist "Notice_*.txt"
				{
					List_data* pld = (List_data*)pvdata;
					
					if( pld->dwFileNum == 0 )
					{
						//	getlist "Notice_*.txt" - Fail

						iR = g_pFtp->Shutdown( g_hSession);
						_ASSERTE( iR == 0);

						MessageBox_Info( "Fail to Get List - NoticeFile in Server.");
						break;
					}

					//	받아야 할지 말아야할지 체크 한다.
					if( CheckNewNotice( pld->pFile->cFileName) == TRUE )
					{
						lstrcpy( fd_notice.szRemoteFile, pld->pFile->cFileName);
						lstrcpy( fd_notice.szLocalFile, fd_notice.szRemoteFile);

						AP = AP_DOWNLOAD_NOTICE;
						UpdateAP( &DCset, AP);

						//	Notice 파일을 다운로드 한다.
						iR = g_pFtp->DownloadFile( g_hSession, &fd_notice);
						_ASSERTE( iR == 0);
						break;
					}

					if( bInit_Notice == FALSE )
					{
						bInit_Notice = Init_Notice( &DCset);
					}

					AP = AP_GET_VER_INFO;
					UpdateAP( &DCset, AP);


					//	CorumFile을 서버에서 검색한다.
					iR = g_pFtp->GetList( g_hSession, &ld_zip);
					_ASSERTE( iR == 0);

					break;
				}
			////////////////////////////////////////////////////////////////////////////////
			case SC_DOWNLOAD_NOTICE:	//	download "Notice_*.txt"
				{
					File_data* pfd = (File_data*)pvdata;

					//	Notice 파일 전송 진행상황에는 관심없다.

					if( pfd->CompletionError == CE_SUCCEED  )
					{
						lstrcpy( szNotice, fd_notice.szLocalFile);

						bInit_Notice = Init_Notice( &DCset);

						AP = AP_GET_VER_INFO;
						UpdateAP( &DCset, AP);

						//	CorumFile을 서버에서 검색한다.
						iR = g_pFtp->GetList( g_hSession, &ld_zip);
						_ASSERTE( iR == 0);
					}

					break;
				}
			////////////////////////////////////////////////////////////////////////////////
			case SC_GETLIST_ZIP:	//	getlist "CorumFile"
				{
					List_data* pld = (List_data*)pvdata;

					if( pld->dwFileNum == 0 )
					{
						//	받을게 엄따. 최신버젼이다.

						ClearStatusBoard( &DCset);

						AP = AP_CHECK_PACK;
						UpdateAP( &DCset, AP);

						//	Connect 버튼 비활성화.
						EnableWindow( g_pcp->btn_1.hWnd, FALSE);
						EnableWindow( g_pcp->btn_2.hWnd, FALSE);

						//	팩 파일을 검사한다..
						hThread_worker = (HANDLE) _beginthreadex( NULL, 0, &Thread_worker, (PVOID) 1, 0, (UINT*) &dwThreadId);

						break;
					}

					//	파일 다운로드 .
					//
					AP = AP_DOWNLOAD_ZIP;
					UpdateAP( &DCset, AP);

					//	다운로드 다이얼로그 콘트롤 보이기.
					//	...
					dwCurrentSize					= 0;
					dwFileSizeMax					= pld->pFile->nFileSizeLow;

					//	File
					g_pcp->sv_FileC.Update( DCset.hMDC_Src, DCset.hMDC_Des, "FileSize");
					wsprintf( Buf_sv, "0 Kbytes  /  %d Kbytes.", dwFileSizeMax / 1024);
					g_pcp->sv_FileC2.Update( DCset.hMDC_Src, DCset.hMDC_Des, Buf_sv);


					//	프로그래스바 최대값 세트.
					g_pcp->pv.Init_Range( pld->pFile->nFileSizeLow); // Ff.dwFileSize[0] );

					
					//	CorumFile을 다운로드 한다.
					iR = g_pFtp->DownloadFile( g_hSession, &fd_zip);
					_ASSERTE( iR == 0);
					break;
				}
			////////////////////////////////////////////////////////////////////////////////
			case SC_DOWNLOAD_ZIP:	//	download "CorumFile"
				{
					File_data* pfd = (File_data*)pvdata;


					if( pfd->CompletionError == CE_DOWNLOAD_PROGRESS )		//	ZIP 파일 전송 진행상황은 관심 있다.
					{				
						dwCurrentSize = pfd->dwTransferBytes;

						if( g_pcp->pv.Update( dwCurrentSize) )
							g_pcp->pv.UpdateBar( DCset.hDC, DCset.hMDC_Des);


						wsprintf( Buf_sv, "%d Kbytes  /  %d Kbytes.", dwCurrentSize / 1024, dwFileSizeMax / 1024);
						g_pcp->sv_FileC2.Update( DCset.hMDC_Src, DCset.hMDC_Des, Buf_sv);
					}

					if( pfd->CompletionError == CE_SUCCEED  )
					{
						//	다운로드가 완료 되엇다.
						ClearStatusBoard( &DCset);

						AP = AP_EXTRACT;
						UpdateAP( &DCset, AP);

						//	Connect 버튼 비활성화.
						EnableWindow( g_pcp->btn_1.hWnd, FALSE);
						EnableWindow( g_pcp->btn_2.hWnd, FALSE);
						
						//	압축을 푼다.
						hThread_worker = (HANDLE) _beginthreadex( NULL, 0, &Thread_worker, (PVOID) 0, 0, (UINT*) &dwThreadId);
					}
				
					break;
				}

			default:
				break;
			}

			g_pcp->CloseDC_SET( &DCset);
			return TRUE;
		}

	case WM_EXTRACT:
		{
			DWORD	dwMsg;
			int		iCount;

			dwMsg	= (DWORD) wParam;
			iCount	= (int) lParam;


			if( dwMsg == EX_FAIL )
			{
				MessageBox_Info("Fail to Extract - UpdateFile.");
				EndDialog( hWnd, -2);
				return TRUE;
			}
			
			g_pcp->OpenDC_SET( &DCset);

			if( dwMsg == EX_INIT )
			{
				g_pcp->sv_ExFileC.Update( DCset.hMDC_Src, DCset.hMDC_Des, "Extract File" );
				iR = wsprintf( Buf_sv, "%d / %d Total Files", 0, g_iExTotalFile);
				g_pcp->sv_ExFileC2.Update( DCset.hMDC_Src, DCset.hMDC_Des, Buf_sv );
				g_pcp->pv.Init_Range( g_iExTotalFile );
			}

			if( dwMsg == EX_PROGRESS )
			{
				if( g_pcp->pv.Update( iCount ) )
					g_pcp->pv.UpdateBar( DCset.hDC, DCset.hMDC_Des);

				iR = wsprintf( Buf_sv, "%d / %d Total Files", iCount, g_iExTotalFile);
				g_pcp->sv_ExFileC2.Update( DCset.hMDC_Src, DCset.hMDC_Des, Buf_sv );
			}

			if( dwMsg == EX_COMPLETE )
			{
				ClearStatusBoard( &DCset);

				AP = AP_CHECK_PACK;
				UpdateAP( &DCset, AP);
			}

//			UpdateWindow( hWnd);
			g_pcp->CloseDC_SET( &DCset);
			return TRUE;
		}

	case WM_PACK:
		{
			DWORD	dwMsg;
			int		iCount;

			dwMsg	= (DWORD) wParam;
			iCount	= (int) lParam;


			if( dwMsg == PA_FAIL )
			{
				MessageBox_Info("Fail to Pack - DataFile.");
				EndDialog( hWnd, -2);
				return TRUE;
			}
			
			g_pcp->OpenDC_SET( &DCset);

			if( dwMsg == PA_INIT )
			{
				AP = AP_PACK;
				UpdateAP( &DCset, AP);

				g_pcp->sv_ExFileC.Update( DCset.hMDC_Src, DCset.hMDC_Des, "Package File" );
				iR = wsprintf( Buf_sv, "%d / %d Total Files", 0, g_iPaTotalFile);
				g_pcp->sv_ExFileC2.Update( DCset.hMDC_Src, DCset.hMDC_Des, Buf_sv );
				g_pcp->pv.Init_Range( g_iPaTotalFile );
			}

			if( dwMsg == PA_PROGRESS )
			{
				if( g_pcp->pv.Update( iCount ) )
					g_pcp->pv.UpdateBar( DCset.hDC, DCset.hMDC_Des);

				iR = wsprintf( Buf_sv, "%d / %d Total Files", iCount, g_iPaTotalFile);
				g_pcp->sv_ExFileC2.Update( DCset.hMDC_Src, DCset.hMDC_Des, Buf_sv );
			}

			if( dwMsg == PA_COMPLETE )
			{
				ClearStatusBoard( &DCset);

				AP = AP_COMPLETED;
				UpdateAP( &DCset, AP);

				//
				//	버전을 업데이트 한다.

				if( g_iExTotalFile > 0 )
				{
					if( UpdateVersionInfoFile( CORUM_VERSION_INFO_FILE, CORUM_VERSION_INFO_FINE_NEW) == FALSE )
					{
						g_pcp->CloseDC_SET( &DCset);
						//	버전 업데이트 실패.
						MessageBox_Info ("Fail to Update - VersionInfoFile.");
						EndDialog( hWnd, -1 );
						return TRUE;
					}

					//	New Version 출력.
					GetVersionInfoFile( CORUM_VERSION_INFO_FILE, szVersion);
					szVersion[VERSION_TOTAL_LENGTH] = NULL;
					wsprintf( Buf_sv,"Client Version [ %s ]", szVersion);
					g_pcp->sv_ClientVer.Update( DCset.hMDC_Src, DCset.hMDC_Des, Buf_sv);

					if( SelfUpdate() == TRUE )
					{
						g_pcp->CloseDC_SET( &DCset);
						EndDialog( hWnd, 0);
						return TRUE;
					}
#ifdef CHINA_LOCALIZING
					else if(g_bRestartRegionSelect == TRUE)
					{
					//	MessageBox_Info("proc2: Call RegionSelectAgain!");

						RegionSelectAgain();

						g_pcp->CloseDC_SET( &DCset);
						EndDialog( hWnd, 0);
						return TRUE;
					}
#endif
				}

				//	btn_( 1 )		CONNECT -> START
				g_pcp->btn_1.Update( DCset.hDC, BS_NONE, BSI_START);
				//	btn_ ( 2 )		CANCEL -> EXIT
				g_pcp->btn_2.Update( DCset.hDC, BS_NONE, BSI_EXIT);

				//	start, exit 버튼 활성화.
				EnableWindow( g_pcp->btn_1.hWnd, TRUE);
				EnableWindow( g_pcp->btn_2.hWnd, TRUE);
			}

//			UpdateWindow( hWnd);
			g_pcp->CloseDC_SET( &DCset);
			return TRUE;
		}


	case WM_COMMAND:
		{
			//	Connect, Start 버튼.
			if ( LOWORD(wParam) == IDC_BTN_1 && HIWORD(wParam) == 0 )
			{
				iR = g_pcp->OnBtn_1();
				if( iR < 0 )
					EndDialog( hWnd, iR);
				else
					iR = g_pFtp->Connect( g_hSession, &cd);
			}
			//	Cancel, Exit 버튼.
			if ( LOWORD(wParam) == IDC_BTN_2 && HIWORD(wParam) == 0 )
			{
				iR = g_pcp->OnBtn_2();
				if( iR < 0 )
					EndDialog( hWnd, iR);
			}

			return TRUE;
		}

	case WM_ERASEBKGND:
		{
			return TRUE;
		}
	case WM_MOUSEWHEEL:
		{
			if( bInit_Notice )
			{
				if( ((short) HIWORD(wParam)) > 0 )
					wParam = SB_LINEUP;
				else
					wParam = SB_LINEDOWN;

				g_pcp->OnVscroll( wParam, NULL);
			}
			return BOOL( 0);
		}
	case WM_VSCROLL:
		{
			if( bInit_Notice )
				g_pcp->OnVscroll( wParam, lParam);

			return BOOL( 0);
		}
	case WM_MOUSEMOVE:
		{
			g_pcp->OnMouseMove( wParam, lParam);
			return TRUE;
		}
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
		{
			g_pcp->OnLButtonDown( wParam, lParam);
			return BOOL( 0);
		}
	case WM_LBUTTONUP:
		{
			g_pcp->OnLButtonUp( wParam, lParam);
			return BOOL( 0);
		}
	case WM_CTLCOLORSCROLLBAR:
		{
			return ( g_pcp->OnCtlColorScrollBar( wParam, lParam));
		}
	case WM_DRAWITEM:
		{
			g_pcp->OnDrawItem( wParam, lParam);
			return TRUE;
		}
	case WM_PAINT:
		{
			g_pcp->OnPaint();
			return TRUE;
		}
	case WM_KEYDOWN:
		return TRUE;

	}	//	SWITCH END

    return FALSE;
}

BOOL CALLBACK WndProc_Button(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message )
	{
	case WM_CAPTURECHANGED:
	case WM_SETFOCUS:
	case WM_KILLFOCUS:
	case WM_ERASEBKGND:
	case WM_KEYDOWN:
			return TRUE;
	case WM_LBUTTONDOWN:
			ReleaseCapture();
			break;
	case WM_LBUTTONDBLCLK:
			message = WM_LBUTTONDOWN;
			break;
	case WM_LBUTTONUP:
		{
			BOOL bR;
			bR = CallWindowProc( WndProc_Button_Old, hWnd, message, wParam, lParam);
			g_pcp->On_Btn_MouseMove( hWnd, wParam , lParam);
			return bR;
		}
	case WM_MOUSEMOVE:
			g_pcp->On_Btn_MouseMove( hWnd, wParam, lParam);
			break;
	default:
		break;
	}

	return ( CallWindowProc( WndProc_Button_Old, hWnd, message, wParam, lParam) );
}

DWORD CALLBACK Pack_progress(DWORD dwCurCount, DWORD dwTotalCount, void* pArg)
{
	int		iCount;
	float	fPer;
	static	iCount_old = 0;

	if( dwCurCount == 0 )
		return 0;

	fPer	= (float)(dwCurCount) / (float)(dwTotalCount);
	iCount	= (g_iPaCurPack * fPer);
	if( iCount == iCount_old )
		return 0;

 	PostMessage( g_pcp->mw.hWnd, WM_PACK, (WPARAM) PA_PROGRESS, (LPARAM) (g_iPaCurrentNum + iCount));
	iCount_old = iCount;

	return 0;
}


////////////////////////////////////////////////////
//
//	Load / UnLoad - RainFTP
//
////////////////////////////////////////////////////
BOOL Initialize_Ftp()
{
	DllGetClassObject_RainFTP pfLoader;
	HRESULT	hr;

	g_hDllModule = LoadLibrary( "RainFTP.dll");
	if( g_hDllModule == NULL )
	{
		MessageBox_Info( "Fail to Load - RainFTP.dll");
		return FALSE;
	}
	
	pfLoader = (DllGetClassObject_RainFTP) GetProcAddress( g_hDllModule, "DllGetClassObject");
	hr = (*pfLoader)( CLSID_RainFTP, IID_RainFTP, (void**)&g_pFtp);
	if ( FAILED(hr) )
	{
		MessageBox_Info( "No Matching CLSID or IID.");
		return FALSE;
	}

	return TRUE;
}

void UnInitialize_Ftp()
{
	if( g_pFtp )
	{
		if( g_hSession)
			g_pFtp->CloseSession( g_hSession);

		g_pFtp->Release();
		g_pFtp = NULL;
	}

	FreeLibrary( g_hDllModule);
}

////////////////////////////////////////////////////
//
//	Read / Write  - VerInfo File
//
////////////////////////////////////////////////////
BOOL GetVersionInfoFile( char* lpszVerInfoFile, char* lpszVerInfo)
{
	HANDLE	hFile;
	DWORD	dwBytesRead;
	BOOL	bR;

	hFile = CreateFile( lpszVerInfoFile,
							GENERIC_READ,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL );

	if( hFile == INVALID_HANDLE_VALUE )
	{
		MessageBox_Info( "Fail to Open - CorumVerInfo.ver");
		return FALSE;
	}

	bR = ReadFile( hFile,
					lpszVerInfo,
					VERSION_TOTAL_LENGTH,
					&dwBytesRead,
					NULL );

	if( bR == FALSE  ||  dwBytesRead != VERSION_TOTAL_LENGTH )
	{
		MessageBox_Info( "Fail to Read - CorumVerInfo.ver");
		return FALSE;
	}

	CloseHandle( hFile );

	memcpy( lpszVerInfo + VERSION_TOTAL_LENGTH, ".zip", 4);

	return TRUE;
}

BOOL UpdateVersionInfoFile( char* lpszOldVerInfoFile, char* lpszNewVerInfoFile)
{
	HANDLE				hFind;
	WIN32_FIND_DATA		wfd;
	BOOL				bR;

	hFind = FindFirstFile( lpszNewVerInfoFile, &wfd);
	if( hFind == INVALID_HANDLE_VALUE )
		return FALSE;
	
	bR = DeleteFile( lpszOldVerInfoFile);
	if( !bR )
		return FALSE;

	bR = MoveFile( lpszNewVerInfoFile, lpszOldVerInfoFile); // for win2000 later ==> , MOVEFILE_REPLACE_EXISTING);
	if( !bR )
	{
		return FALSE;
	}

	return TRUE;
}

////////////////////////////////////////////////////
//
//	Create Process
//
////////////////////////////////////////////////////
BOOL RunCorum()
{
	STARTUPINFO				sp;
	PROCESS_INFORMATION		pi;
	BOOL					bR;

	memset( &sp, 0, sizeof(STARTUPINFO));
	sp.cb			= sizeof(STARTUPINFO);
	sp.dwFlags		= STARTF_USEPOSITION | STARTF_USESIZE | STARTF_USESTDHANDLES; //STARTF_USESHOWWINDOW 
	sp.wShowWindow	= SW_SHOW;

	bR = CreateProcess( CORUM_EXE_FILE,	// name of executable module
							g_pCmdLine,		// command line string 넷마블.
							NULL,			// SD
							NULL,			// SD
							FALSE,			// handle inheritance option
							CREATE_DEFAULT_ERROR_MODE | NORMAL_PRIORITY_CLASS,	// creation flags
							NULL,			// new environment block
							NULL,			// current directory name
							&sp,			// startup information
							&pi				// process information
						);

	if( bR == FALSE )
		return FALSE;

	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );

	return TRUE;
}

BOOL CheckNewNotice(char* pszNewNotice)
{
	HANDLE					hFind;
	WIN32_FIND_DATA			wfd;
	int						iR;

	wsprintf( szNotice, "%s", pszNewNotice);

	hFind = FindFirstFile( g_szSearchNotice, &wfd);
	if( hFind == INVALID_HANDLE_VALUE )
		return TRUE;

	FindClose(hFind);

	iR = lstrcmpi( wfd.cFileName, pszNewNotice);
	if( iR == 0 )
		return FALSE;

	DeleteFile( wfd.cFileName );

	return TRUE;
}


BOOL SelfUpdate()
{
	HANDLE					hFind;
	WIN32_FIND_DATA			wfd;
	STARTUPINFO				sp;
	PROCESS_INFORMATION		pi;
	BOOL					bR;


	hFind = FindFirstFile( SELF_UPDATE_FILE, &wfd);
	if( hFind == INVALID_HANDLE_VALUE )
		return FALSE;

	FindClose(hFind);

	memset( &sp, 0, sizeof(STARTUPINFO));
	sp.cb			= sizeof(STARTUPINFO);
	sp.dwFlags		= STARTF_USEPOSITION | STARTF_USESIZE | STARTF_USESTDHANDLES; //STARTF_USESHOWWINDOW 
	sp.wShowWindow	= SW_SHOW; 

#ifdef CHINA_LOCALIZING
	if(g_bRestartRegionSelect)
	{
		strcat(g_pCmdLine, " /region_select");
	}
#endif

	bR = CreateProcess( SELF_UPDATE_FILE,	// name of executable module
							g_pCmdLine,		// command line string 넷마블.
							NULL,			// SD
							NULL,			// SD
							FALSE,			// handle inheritance option
							CREATE_DEFAULT_ERROR_MODE | NORMAL_PRIORITY_CLASS,	// creation flags
							NULL,			// new environment block
							NULL,			// current directory name
							&sp,			// startup information
							&pi				// process information
						);

	if( bR == FALSE )
		return FALSE;

	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );

	return TRUE;
}

#ifdef CHINA_LOCALIZING
BOOL RegionSelectAgain()
{
	STARTUPINFO				sp;
	PROCESS_INFORMATION		pi;
	BOOL					bR;	
	
	memset( &sp, 0, sizeof(STARTUPINFO));
	sp.cb			= sizeof(STARTUPINFO);
	sp.dwFlags		= STARTF_USEPOSITION | STARTF_USESIZE | STARTF_USESTDHANDLES; //STARTF_USESHOWWINDOW 
	sp.wShowWindow	= SW_SHOW;

//	MessageBox_Info("proc3: Run RegionSelect.exe!");

	bR = CreateProcess( REGION_SELECT_FILE,	// name of executable module
							NULL,			// command line string 
							NULL,			// SD
							NULL,			// SD
							FALSE,			// handle inheritance option
							CREATE_DEFAULT_ERROR_MODE | NORMAL_PRIORITY_CLASS,	// creation flags
							NULL,			// new environment block
							NULL,			// current directory name
							&sp,			// startup information
							&pi				// process information
						);

	if( bR == FALSE )
	{
		MessageBox_Info("Fail to run RegionSelect.exe!");
		return FALSE;
	}

	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );

	return TRUE;
}
#endif

//	Info MessageBox
void MessageBox_Info (char* szInfoText)
{
	MessageBox( g_hwMain , szInfoText, "CorumPatch Info", MB_OK);
}

void ClearStatusBoard( DC_SET* pDCSet)
{
	BitBlt( pDCSet->hMDC_Des, 6, 412, 500, 96,
			pDCSet->hMDC_Src, 6, 412, SRCCOPY);
	BitBlt( pDCSet->hDC, 6, 412, 500, 96,
			pDCSet->hMDC_Src, 6, 412, SRCCOPY);
}

void UpdateAP( DC_SET* pDCSet, APP_PROGRESS ap)
{
	//	Status		프로그램 스테이터스
	g_pcp->sv_Status.Clear( pDCSet->hMDC_Src, pDCSet->hMDC_Des);
	g_pcp->sv_Status.Update( pDCSet->hMDC_Src, pDCSet->hMDC_Des, APS[ ap] );
}

void UpdateEI( DC_SET* pDCSet, ERROR_INFO ei)
{
	g_pcp->sv_ErrInfo.Clear( pDCSet->hMDC_Src, pDCSet->hMDC_Des);
	g_pcp->sv_ErrInfo.Update( pDCSet->hMDC_Src, pDCSet->hMDC_Des, EIS[ ei] );
}

BOOL Init_Notice( DC_SET* pDCSet)
{
	char	Buf_sv[MAX_PATH];
	BOOL	bR;

	g_pcp->Init_TextView( pDCSet->hDC, szNotice);
	bR = g_pcp->Init_TextScroll();

	if( bR )
	{
		wsprintf( Buf_sv,"%d  / %d", g_pcp->ts.iLine + 1, g_pcp->ts.iLineMax + 1);
		g_pcp->sv_page.Update_r( pDCSet->hMDC_Src, pDCSet->hMDC_Des, Buf_sv);
		g_pcp->sv_page.Invalidate();
	}

	g_pcp->tv.Update( pDCSet->hMDC_Src, pDCSet->hMDC_Des, g_pcp->ts.iLine);
	g_pcp->tv.Invalidate();

	SetFocus( g_pcp->ts.hWnd);

	return bR;
}

BOOL Initialize_PACK()
{
	HRESULT	hr;

//	CoInitialize(NULL);

	g_hFileStorage = LoadLibrary("SS3DFileStorage.dll");
	if ( g_hFileStorage == NULL )
	{
		MessageBox_Info( "Fail to Load - SS3DFileStorage.dll");
		return FALSE;
	}

	CREATE_INSTANCE_FUNC pFunc = (CREATE_INSTANCE_FUNC)GetProcAddress(g_hFileStorage,"DllCreateInstance");
	hr = pFunc((void**)&g_pFileStorage);
	if ( FAILED(hr) )
	{
		MessageBox_Info( "Fail to CreateInstance - SS3DFileStorage.dll");
		return FALSE;
	}

	g_pFileStorage->Initialize(8192,
							   1024,
							   64,
							   FILE_ACCESS_METHOD_ONLY_FILE);


	return TRUE;
}

void UnInitialize_PACK()
{
	if (NULL != g_pFileStorage)
	{
		g_pFileStorage->Release();
		g_pFileStorage = NULL;
	}

	if( g_hFileStorage )
	{
		FreeLibrary(g_hFileStorage);
		g_hFileStorage = NULL;
	}

//	CoUninitialize();
}


////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////
UINT __stdcall Thread_worker( PVOID pvParam)
{
	//		Extract
	//		Pack

	if( (DWORD)pvParam == 0 )
		Extract();

	PackFile();

	return 1;
}

////////////////////////////////////////////////////
//
//	Extract()
//
////////////////////////////////////////////////////
void Extract()
{
	int	iCount;

	g_pZip->Open( g_szCorumpatchFile, CZipArchive::zipOpen);
	g_iExTotalFile = g_pZip->GetCount();

#ifdef CHINA_LOCALIZING
	if(-1 != g_pZip->FindFile(REGIONINFO_FILE))
	{
	//	MessageBox_Info("proc1: Found Region.ini!");

		g_bRestartRegionSelect = TRUE;
	}
#endif

	PostMessage( g_pcp->mw.hWnd, WM_EXTRACT, (WPARAM) EX_INIT, (LPARAM) 0);

	for ( iCount = 0; iCount < g_iExTotalFile; iCount++)
	{
		if( g_pZip->ExtractFile( iCount, NULL ) == false )
		{
			PostMessage( g_pcp->mw.hWnd, WM_EXTRACT, (WPARAM) EX_FAIL, (LPARAM) 0);
			return;
		}

		PostMessage( g_pcp->mw.hWnd, WM_EXTRACT, (WPARAM) EX_PROGRESS, (LPARAM) iCount);
	}

	PostMessage( g_pcp->mw.hWnd, WM_EXTRACT, (WPARAM) EX_COMPLETE, (LPARAM) 0);
}

////////////////////////////////////////////////////
//
//	PackFile()
//
////////////////////////////////////////////////////
void PackFile()
{
	char	szWorkDir[ MAX_PATH];
	BOOL	bError;
//
//

	DWORD	adwPackNum[ PACK_DIR_MAX];
	DWORD	dwCurDir;

	DWORD	dwTotalPackNum;
	DWORD	dwPackNum;

	char*	pFileNameBuffer;
	char*	pCurBuffer;


	bError = true;
	
	pFileNameBuffer = new CHAR[ PACK_FILE_NAME_BUFFER_SIZE];
	if( pFileNameBuffer == NULL )
		goto lb_exit;


	pCurBuffer		= pFileNameBuffer;
	dwCurDir		= 0;
	dwTotalPackNum	= 0;

	//	Find files to pack.

	while( dwCurDir < PACK_DIR_MAX )
	{
		memset( szWorkDir, 0, MAX_PATH);
		lstrcpy( szWorkDir, g_szCorumDir);
		lstrcat( szWorkDir, g_aszPackDir[ dwCurDir]);

		dwPackNum = GetPackInfo( &pCurBuffer, szWorkDir, g_aszPackFile[ dwCurDir]);
		adwPackNum[ dwCurDir] = dwPackNum;
		dwTotalPackNum += dwPackNum;
		dwCurDir++;
	}

	if( dwTotalPackNum == 0 )
	{
		bError = false;
		goto lb_exit;
	}

	//	팩 작업을 시작한다.

	if( Initialize_PACK() == FALSE )
		goto lb_exit;

	g_iPaTotalFile = dwTotalPackNum;
	PostMessage( g_pcp->mw.hWnd, WM_PACK, (WPARAM) PA_INIT, (LPARAM) 0);

	pCurBuffer	= pFileNameBuffer;
	for( dwCurDir = 0; dwCurDir < PACK_DIR_MAX; dwCurDir++ )
	{
		dwPackNum = adwPackNum[ dwCurDir];
		if( dwPackNum == 0 )
			continue;

		memset( szWorkDir, 0, MAX_PATH);
		lstrcpy( szWorkDir, g_szCorumDir);
		lstrcat( szWorkDir, g_aszPackDir[ dwCurDir]);

		if( AddtoPack( &pCurBuffer, szWorkDir, g_aszPackFile[ dwCurDir], dwPackNum) == FALSE )
			goto lb_exit;
	}

	//	팩 작업 완료.
	PostMessage( g_pcp->mw.hWnd, WM_PACK, (WPARAM) PA_PROGRESS, (LPARAM) g_iPaTotalFile);
	bError = false;
		
lb_exit:

	SetCurrentDirectory( g_szCorumDir);
	if( pFileNameBuffer )
		delete [] pFileNameBuffer;

	if( bError )
		PostMessage( g_pcp->mw.hWnd, WM_PACK, (WPARAM) PA_FAIL, (LPARAM) 0);
	else
		PostMessage( g_pcp->mw.hWnd, WM_PACK, (WPARAM) PA_COMPLETE, (LPARAM) 0);
}

DWORD GetPackInfo( char** ppCurBuffer, char* szPackDir, char* szPackFile)
{
	HANDLE				hFind;
	WIN32_FIND_DATA		wfd;
	char*				pCur;
	int					iLength; 
	DWORD				dwPackNum;
	BOOL				bExit;

	dwPackNum = 0;

	if( SetCurrentDirectory( szPackDir) == FALSE )
		return dwPackNum;

	hFind = FindFirstFile( "*.*", &wfd);
	if( hFind == INVALID_HANDLE_VALUE )
		return dwPackNum;

	pCur = *ppCurBuffer;

	for( bExit = TRUE; bExit != FALSE; bExit = FindNextFile( hFind, &wfd) )
	{
		if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY  ||
			lstrcmpi( szPackFile, wfd.cFileName) == 0 )
			continue;

		iLength = lstrlen( wfd.cFileName);
		iLength++;
		memcpy( pCur, wfd.cFileName, iLength);
		pCur += iLength;
		dwPackNum++;
	}

	FindClose( hFind);

	*ppCurBuffer = pCur;
	
	return dwPackNum;
}

BOOL AddtoPack( char** ppCurBuffer, char* szPackDir, char* szPackFile, DWORD dwPackNum)
{
	char	szPackFile_Full[ MAX_PATH];
	char	szInsertFile_Full[ MAX_PATH];
	char*	pCur;
	DWORD	dwCur;
	int		iLength;


	pCur = *ppCurBuffer;

	memset( szPackFile_Full, 0, MAX_PATH);
	lstrcpy( szPackFile_Full, szPackDir);
	lstrcat( szPackFile_Full, "\\");
	lstrcat( szPackFile_Full, szPackFile);

	g_hPackFile = g_pFileStorage->MapPackFile( szPackFile_Full);
	if( g_hPackFile == NULL )
		return FALSE;

	if( g_pFileStorage->LockPackFile( g_hPackFile, NULL) == FALSE )
	{
		g_pFileStorage->UnmapPackFile( g_hPackFile);
		return FALSE;
	}

	for( dwCur = 0; dwCur < dwPackNum; dwCur++)
	{
		memset( szInsertFile_Full, 0, MAX_PATH);
		lstrcpy( szInsertFile_Full, szPackDir);
		lstrcat( szInsertFile_Full, "\\");
		lstrcat( szInsertFile_Full, pCur);

		iLength = lstrlen( pCur);
		pCur += (iLength + 1);

		g_pFileStorage->InsertFileToPackFile( g_hPackFile, szInsertFile_Full);
	}

	g_iPaCurPack = dwPackNum;

	g_pFileStorage->UnlockPackFile( g_hPackFile, Pack_progress);
	g_pFileStorage->UnmapPackFile( g_hPackFile);
	g_hPackFile = NULL;

	g_iPaCurrentNum	+= g_iPaCurPack;

	SetCurrentDirectory( szPackDir);

	pCur = *ppCurBuffer;
	for( dwCur = 0; dwCur < dwPackNum; dwCur++)
	{
		DeleteFile( pCur);
		iLength = lstrlen( pCur);
		pCur += (iLength + 1);
	}

	*ppCurBuffer = pCur;

	return TRUE;
}
