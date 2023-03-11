// CorumLauncher.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <STDIO.H>
#include <time.h>
#include "DFXForm.h"

#ifndef TRUE

#define TRUE	1
#define FALSE	0

#endif

// ========================
// Name		: ThereIsFile
// Usage	: 파일 존재 유무 판단
// return	: true / false
// By, Date	: seven, 03/08/01
// ========================
BOOL ThereIsFile(LPCSTR pszFileName)
{
	HANDLE			hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA wfd;
	BOOL			bReturn = FALSE;

	hFind = FindFirstFile(pszFileName, &wfd);
	if( hFind == INVALID_HANDLE_VALUE )
		return FALSE;

	FindClose(hFind);
	return TRUE;
}

VOID GetGlobalEventName(char* szEventName)
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	char szTemp[0xff]={0,};
	wsprintf(szTemp, "Fuck%02dYou%02dHacker%02d", time.wYear, time.wMonth, time.wDay);

	DFXform xform;
	UINT nSize = 0;
	char szBuf[0xff]={0,};
	xform.XXEncode((LPBYTE)szTemp, lstrlen(szTemp), (LPBYTE)szBuf, &nSize);

	wsprintf(szEventName, "Global\\%s", szBuf);
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	char				szCmdLine[1024*40] = {0, };
	char				szError[0xff] = {0,	};
	STARTUPINFO			suInfo;
	PROCESS_INFORMATION procInfo;
	BOOL				bRet = 0;
	int					nRet = 0;
	HANDLE				hEvent = NULL;

	memset(&procInfo, 0, sizeof(PROCESS_INFORMATION));
	memset(&suInfo, 0, sizeof(STARTUPINFO));
	suInfo.cb = sizeof(STARTUPINFO);

	__try
	{	
		// GFunction dll의 존재 유무를 확인한다.
		if(	!ThereIsFile(".\\SS3DGFuncN.dll") ||
			!ThereIsFile(".\\SS3DGFuncSSE.dll") )
		{
			strcpy(szError, "File Not Founds (SS3dGFuncN.dll, SS3DGFuncSSE.dll)!!");
			nRet = 1;
			__leave;
		}

		// SelectGFunc 을 실행한다. 
		bRet = CreateProcess(".\\SelectGFunc.exe", NULL, NULL, NULL, TRUE, 
			CREATE_NO_WINDOW, NULL, NULL, &suInfo, &procInfo);

		if(!bRet)
		{
			strcpy(szError, "Execution of SelectGFunc.exe Failed!");
			nRet = 2;
			__leave;
		}		

		// 최대 10초간 SelectGFunc.exe가 종료되길 대기한다.
		DWORD dwRet = WaitForSingleObject(procInfo.hProcess, 10000);
		if(WAIT_TIMEOUT == dwRet)
		{
			DWORD dwExitCode = 0;
			GetExitCodeProcess(procInfo.hProcess, &dwExitCode);

			if(STILL_ACTIVE == dwExitCode)
			{
				strcpy(szError, "Execution of SelectGFunc.exe Failed(timeout)!");
				nRet = 3;
				__leave;
			}
		}
		else if(WAIT_FAILED == dwRet)
		{
			sprintf(szError, "Waiting SelectGFunc.exe Failed(error: %d)!", GetLastError());
			nRet = 4;
			__leave;
		}

		CloseHandle(procInfo.hThread);
		CloseHandle(procInfo.hProcess);

		////////////////////////////////////////////////////////////////////////////
		// 해킹툴 블럭을 위한 코드.
		char szEventName[0xff] = {0,};
		GetGlobalEventName(szEventName);
		
	//	MessageBox(NULL, szEventName, "EventName", MB_OK);
		
		hEvent = CreateEvent(NULL, FALSE, FALSE, szEventName);
		if(hEvent == NULL)
		{
			nRet = 5;
			__leave;
		}
		//////////////////////////////////////////////////////////////////////////////

		// CorumOnline 을 실행한다.		
		sprintf(szCmdLine, " %s", lpCmdLine);
		bRet = CreateProcess(".\\CorumOnline.exe", szCmdLine, NULL, NULL, TRUE, 
			CREATE_NEW_CONSOLE, NULL, NULL, &suInfo, &procInfo);

		if(!bRet)
		{
			strcpy(szError, "Execution of CorumOnline.exe Failed!");
			nRet = 6;
			__leave;
		}

		//////////////////////////////////////////////////////////////////////////////
		// 역시, 해킹툴 블럭을 위한 코드.
		WaitForSingleObject(hEvent, 10000);
		//////////////////////////////////////////////////////////////////////////////

	}

	__finally
	{
		if(nRet)
		{
			MessageBox(NULL, szError, "Error", MB_OK|MB_ICONERROR);
		}

		if(procInfo.hThread)	CloseHandle(procInfo.hThread);
		if(procInfo.hProcess)	CloseHandle(procInfo.hProcess);
		if(hEvent)				CloseHandle(hEvent);

		return nRet;
	}
}







