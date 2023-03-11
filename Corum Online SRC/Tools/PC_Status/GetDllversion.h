#ifndef GETDLLVERSION_H
#define GETDLLVERSION_H

#include "global_define.h"

void	GetDllVersion(char * pszInOutName)
{
//	TCHAR execFilename[_MAX_PATH];
//	VERIFY(::GetModuleFileName(NULL, execFilename, _MAX_PATH));

	HWND hWindow = NULL;

	BYTE *block;
	DWORD FAR *translation;
	DWORD FAR *buffer;
	DWORD handle;
	UINT bytes;
	TCHAR *lpszFileName = pszInOutName;
	TCHAR name[512];
	CString szString;

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
				wsprintf(name,"\\StringFileInfo\\%04x%04x\\FileVersion", 
				LOWORD(*translation), HIWORD(*translation) );

				if( ::VerQueryValue(block, name, (LPVOID *)&buffer, (UINT FAR *)&bytes) ) 
				{
					szString.Format("%s", (LPCTSTR)buffer );
				}
				else
				{
					MessageBox(hWindow,"Can't call Version Info","Version Error",MB_OK);
					PostQuitMessage(0);
				}
			
			}
			else 
			{
				MessageBox(hWindow,"Can't call Translation Type","Version Error",MB_OK);
				PostQuitMessage(0);
			}
				
		}
		else 
		{
			MessageBox(hWindow,"Can't call Version Info","Version Error",MB_OK);
			PostQuitMessage(0);
		}

		delete [] block;
	}
	else 
	{
		MessageBox(hWindow,"Can't call FileVersionInfo Size","Version Error",MB_OK);
		PostQuitMessage(0);
	}

//	szString.Remove(',');
//	szString.Remove(' ');

	strcpy(pszInOutName, (LPCTSTR)szString );
}

#endif 
//GETDLLVERSION_H