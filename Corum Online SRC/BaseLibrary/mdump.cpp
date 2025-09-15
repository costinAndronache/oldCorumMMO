
#include "stdafx.h"
#include <ShlObj_core.h>
#include "mdump.h"

PWSTR MiniDumper::m_szAppName;

MiniDumper::MiniDumper( const PWSTR szAppName )
{
	// if this assert fires then you have two instances of MiniDumper
	// which is not allowed
	assert( m_szAppName==NULL );

	m_szAppName = szAppName ? wcsdup(szAppName) : L"Application";

	::SetUnhandledExceptionFilter( TopLevelFilter );
}

LONG MiniDumper::TopLevelFilter( struct _EXCEPTION_POINTERS *pExceptionInfo )
{
	LONG retval = EXCEPTION_CONTINUE_SEARCH;
	HWND hParent = NULL;						// find a better value for your app

	// firstly see if dbghelp.dll is around and has the function we need
	// look next to the EXE first, as the one in System32 might be old 
	// (e.g. Windows 2000)
	HMODULE hDll = NULL;
	char szDbgHelpPath[_MAX_PATH];

	if (GetModuleFileName( NULL, szDbgHelpPath, _MAX_PATH ))
	{
		char *pSlash = _tcsrchr( szDbgHelpPath, '\\' );
		if (pSlash)
		{
			_tcscpy( pSlash+1, "DBGHELP.DLL" );
			hDll = ::LoadLibrary( szDbgHelpPath );
		}
	}

	if (hDll==NULL)
	{
		// load any version we can
		hDll = ::LoadLibrary( "DBGHELP.DLL" );
	}

	PWSTR szResult = NULL;

	if (hDll)
	{
		MINIDUMPWRITEDUMP pDump = (MINIDUMPWRITEDUMP)::GetProcAddress( hDll, "MiniDumpWriteDump" );
		if (pDump)
		{
			wchar_t szDumpPath[_MAX_PATH];
			wchar_t szScratch [_MAX_PATH];

			// work out a good place for the dump file

			wchar_t* path = 0;
			HRESULT result = SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &path);

			if (result == S_OK && lstrlenW(path)) {
				wcscpy( szDumpPath, path);
				wcsncat(szDumpPath, L"\\", _MAX_PATH);
				wcsncat( szDumpPath, m_szAppName, _MAX_PATH);
				wcsncat( szDumpPath, L".dmp", _MAX_PATH);
				wprintf(L"szDumpPath from desktop: %ls :: %ls\n", szDumpPath, path);
			} else {
				GetModuleFileNameW(NULL, szDumpPath, _MAX_PATH);
				wprintf(L"szDumpPath from module name: %ls\n", szDumpPath);
				if(lstrlenW(szDumpPath) == 0) {
					wcscpy(szDumpPath, L"C:\\");
				}

				wcsncat(szDumpPath, m_szAppName, _MAX_PATH);
				wcsncat(szDumpPath, L".dmp", _MAX_PATH);
			}

			wprintf(L"szDumpPath: %ls\n", szDumpPath);
			// ask the user if they want to save a dump file
			::MessageBoxW(NULL, L"Something bad happened in your program, a diagnostic file will be created", m_szAppName, MB_OK);
			{
				// create the file
				HANDLE hFile = ::CreateFileW( szDumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
											FILE_ATTRIBUTE_NORMAL, NULL );

				if (hFile!=INVALID_HANDLE_VALUE)
				{
					_MINIDUMP_EXCEPTION_INFORMATION ExInfo;

					ExInfo.ThreadId = ::GetCurrentThreadId();
					ExInfo.ExceptionPointers = pExceptionInfo;
					ExInfo.ClientPointers = NULL;

					// write the dump
					BOOL bOK = pDump( GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL );
					if (bOK)
					{
						wsprintfW( szScratch, L"Saved dump file to '%ls', please send it to the developer", szDumpPath );
						szResult = szScratch;
						retval = EXCEPTION_EXECUTE_HANDLER;
					}
					else
					{
						wsprintfW( szScratch, L"Failed to save dump file to '%ls' (error %d)", szDumpPath, GetLastError() );
						szResult = szScratch;
					}
					::CloseHandle(hFile);
				}
				else
				{
					wsprintfW( szScratch, L"Failed to create dump file '%s' (error %d)", szDumpPath, GetLastError() );
					szResult = szScratch;
				}
			}
		}
		else
		{
			szResult = L"DBGHELP.DLL too old";
		}
	}
	else
	{
		szResult = L"DBGHELP.DLL not found";
	}

	if (szResult)
		::MessageBoxW( NULL, szResult, m_szAppName, MB_OK );

	return retval;
}
