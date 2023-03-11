#include <windows.h>
#include <tlhelp32.h>
#include <STDIO.H>


//	넷마블 공용.
#define NETMARBLE_CMDLINE_SIZE	1024 * 40
#define AUTOPATCHER				"CorumAutopatch.exe"
#define FTP_COMPONENT			"RainFTP.dll"
#define STORAGE_COMPONENT		"SS3DFileStorage.dll"


BOOL RunProcess(LPCSTR lpCmdLine)
{
	HANDLE			hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA wfd;
	BOOL			bReturn = FALSE;

	hFind = FindFirstFile( AUTOPATCHER, &wfd);
	if( hFind == INVALID_HANDLE_VALUE )
		return FALSE;

	FindClose(hFind);

	PROCESS_INFORMATION pi;
	STARTUPINFO	sp;

	memset( &sp, 0, sizeof(STARTUPINFO));

	sp.cb = sizeof(STARTUPINFO);
	sp.dwFlags = STARTF_USEPOSITION | STARTF_USESIZE | STARTF_USESTDHANDLES; //STARTF_USESHOWWINDOW 
	sp.wShowWindow = SW_SHOW; 		

	bReturn = CreateProcess(	AUTOPATCHER,			// name of executable module
								(LPSTR)lpCmdLine,       // command line string
								NULL,
								NULL,
								FALSE,                  // handle inheritance option
								CREATE_DEFAULT_ERROR_MODE | NORMAL_PRIORITY_CLASS,      // creation flags
								NULL,                   // new environment block
								NULL,					// current directory name
								&sp,					// startup information
								&pi);					// process information
							
	if( !bReturn )
	{
		MessageBox( NULL, "Can't Complete patch process < >", "Patch Update Info", MB_OK|MB_ICONERROR);
		return FALSE;
	}
	
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );

	return TRUE;
}


void MakeNewFileName(LPCSTR szFile, LPSTR szNewFile)
{
	int iLen = lstrlen(szFile);

	lstrcpyn(szNewFile, szFile, iLen - 3);
	strcat(szNewFile, "_New");
	strcat(szNewFile, szFile + iLen - 4);
}


BOOL IsThereFile(LPCSTR szFile)
{
	WIN32_FIND_DATA Find;
	HANDLE hFind = FindFirstFile(szFile, &Find);

	if(hFind == INVALID_HANDLE_VALUE) return FALSE;

	FindClose(hFind);

	return TRUE;
}


BOOL ExchangeFile(LPSTR szSource)
{
	char szUpdateFile[0xff]={0,};
	MakeNewFileName(szSource, szUpdateFile);

	if(IsThereFile(szUpdateFile))
	{	
		SetFileAttributes(szSource, FILE_ATTRIBUTE_NORMAL);

		for(;;)
		{
			if(!DeleteFile(szSource))
			{
				if( GetLastError() == 2 ) break;
				Sleep(500);
				continue;
			}

			break;
		}

		if(!MoveFile(szUpdateFile, szSource)) return FALSE;
	}	

	return TRUE;
}


BOOL IsDeadProcess(char* szProcessName)
{
	HANDLE			hProcessSnap;
	PROCESSENTRY32	pe32;
	int				ret = 0;
		
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if( hProcessSnap == INVALID_HANDLE_VALUE )
	{
		return FALSE;
	}
	
	pe32.dwSize = sizeof( PROCESSENTRY32 );
	
	if(!Process32First(hProcessSnap,&pe32))
	{
		CloseHandle( hProcessSnap );				 
		return FALSE;
	}
	
	do
	{
		ret = strcmp(szProcessName,pe32.szExeFile);
		if(!ret)
		{
			CloseHandle(hProcessSnap);
			return FALSE;
		}
	}while(Process32Next(hProcessSnap,&pe32));
	
	CloseHandle(hProcessSnap);
	return TRUE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// add by Ultra
	// CorumAutopatch.exe가 확실히 죽어있는지 체크한다.
	for(;;)
	{
		if(IsDeadProcess(AUTOPATCHER))
		{
			break;
		}

		Sleep(500);
	}

	// autopatcher update
	if(!ExchangeFile(AUTOPATCHER))
	{
		MessageBox(NULL, "Updating Autopatcher failed", "Error", MB_OK|MB_ICONERROR);
		return 0;
	}

	// ftp component update
	if(!ExchangeFile(FTP_COMPONENT))
	{
		MessageBox(NULL, "Updating RainFTP failed", "Error", MB_OK|MB_ICONERROR);
		return 0;
	}

	// storage component update
	if(!ExchangeFile(STORAGE_COMPONENT))
	{
		MessageBox(NULL, "Updating Storage Component failed", "Error", MB_OK|MB_ICONERROR);
		return 0;
	}

	// do anything you want....
	


	// Execute New Patcher.
	char	szCmdLine[ NETMARBLE_CMDLINE_SIZE] = {0,};
	sprintf(szCmdLine, " %s", lpCmdLine);
	RunProcess(szCmdLine);

	return 0;
}



