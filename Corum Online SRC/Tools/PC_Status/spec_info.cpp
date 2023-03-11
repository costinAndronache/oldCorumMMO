#include "spec_info.h"
#include "getdxver.h"
#include "dxerr8.h"
#include "tchar.h"


// global variable
CPUInfo			g_CPUInfo;

// ===============================
// Spec Cpu Type
// ===============================
void SpecCPU(char * pszCpu)
{
	strcpy(pszCpu, g_CPUInfo.GetExtendedProcessorName());
//	itoa(g_CPUInfo.GetProcessorClockFrequency(), szBuf, 10);
//	strcat(pszCpu, szBuf);
//	strcat(pszCpu, " Mhz");

	if ( g_CPUInfo.DoesCPUSupportFeature(SSE_FEATURE) )
	{ // SSE 를 지원한다.
		strcat(pszCpu, "\nSupport SSE");
	}
	else
	{
		strcat(pszCpu, "\nNot Support SSE");
	}
}


HRESULT SpecSoundCard(char * pszSound)
{
//	HWND				hSoundEdit;
	HRESULT				hr;

	char				szBuf[255] = {'\0', };

    // Free any previous DirectSound objects
    //FreeDirectSound();

	LPDIRECTSOUND	lpDS;
/*
    // Get the HWNDs the combo boxes
    //HWND hSoundDeviceCombo   = (HWND) GetDlgItem(IDC_SOUND_CARD_EDIT );

    // Get the index of the currently selected devices
    INT nSoundIndex   = (INT) ::SendMessage( hSoundDeviceCombo, CB_GETCURSEL, 0, 0 ); 

    // Get the GUID attached to the combo box item
    GUID* pSoundGUID = (GUID*) ::SendMessage( hSoundDeviceCombo, CB_GETITEMDATA, 
                                            nSoundIndex, 0 );
*/
    // Create IDirectSound using the select sound device
    if( FAILED( hr = DirectSoundCreate( /*pSoundGUID*/0, &lpDS, NULL ) ) )
	{
		strcpy(pszSound, "No Driver");
        return DXTRACE_ERR( TEXT("DirectSoundCreate"), hr );
	}

    // Release the IDirectSound object immediately since we don't want
    // to limit this sample to only computers that support full duplex audio
//    SAFE_RELEASE( m_lpDS ); 

//	hSoundEdit = GetDlgItem( IDC_SOUND_CARD_EDIT )->GetSafeHwnd();

    if( FAILED( hr = DirectSoundEnumerate( (LPDSENUMCALLBACK)DSoundEnumCallback,
                                                  (VOID*)pszSound ) ) )
	{
		pszSound = "No Driver";
		MessageBox(NULL,"Error : DirectSoundCaptureEnumerate!!", "Error", MB_OK);
        return S_FALSE;
	}

	// Set the current index
	

    // Release the IDirectSound object immediately since we don't want
    // to limit this sample to only computers that support full duplex audio
    SAFE_RELEASE( lpDS ); 

	return S_OK;
}


// ===================================
// OS Version Info
// ===================================
void SpecOS(char * pszOS)
{
	char szBuf[255] = {'\0',};
	char szCatBuf[64] = {'\0',};

	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;

	// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
	// If that fails, try using the OSVERSIONINFO structure.

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		// If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
		 return;
	}

	switch (osvi.dwPlatformId)
	{
		// Tests for Windows NT product family.
		case VER_PLATFORM_WIN32_NT:

		// Test for the product.
		if ( osvi.dwMajorVersion <= 4 )
			sprintf(szBuf, "%s", "Microsoft Windows NT ");

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
			sprintf(szBuf, "%s", "Microsoft Windows 2000 ");

		if( bOsVersionInfoEx )  // Use information from GetVersionEx.
		{ 
			// Test for the workstation type.
			if ( osvi.wProductType == VER_NT_WORKSTATION )
			{
			   if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
				  strcat(szBuf, "Microsoft Windows XP ");

			   if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
				  strcat(szBuf, "Home Edition " );
			   else
				  strcat(szBuf, "Professional " );
			}

			// Test for the server type.
			else if ( osvi.wProductType == VER_NT_SERVER )
			{
			   if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
				  strcat(szBuf,"Microsoft Windows .NET ");

			   if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
				  strcat(szBuf, "DataCenter Server " );
			   else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
				  if( osvi.dwMajorVersion == 4 )
					 strcat(szBuf, "Advanced Server " );
				  else
					 strcat(szBuf, "Enterprise Server " );
			   else if ( osvi.wSuiteMask == VER_SUITE_BLADE )
				  strcat(szBuf, "Web Server " );
			   else
				  strcat(szBuf, "Server " );
			}
		}
		else   // Use the registry on early versions of Windows NT.
		{
			HKEY hKey;
			char szProductType[BUFSIZE];
			DWORD dwBufLen=BUFSIZE;
			LONG lRet;

			lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
			   "SYSTEM\\CurrentControlSet\\Control\\ProductOptions",
			   0, KEY_QUERY_VALUE, &hKey );
			if( lRet != ERROR_SUCCESS )
				return;

			lRet = RegQueryValueEx( hKey, "ProductType", NULL, NULL,
			   (LPBYTE) szProductType, &dwBufLen);
			if( (lRet != ERROR_SUCCESS) || (dwBufLen > BUFSIZE) )
				return;

			RegCloseKey( hKey );

			if ( lstrcmpi( "WINNT", szProductType) == 0 )
			   strcat(szBuf, "Professional " );
			if ( lstrcmpi( "LANMANNT", szProductType) == 0 )
			   strcat(szBuf, "Server " );
			if ( lstrcmpi( "SERVERNT", szProductType) == 0 )
			   strcat(szBuf, "Advanced Server " );
		}

		// Display version, service pack (if any), and build number.

		if ( osvi.dwMajorVersion <= 4 )
		{
			sprintf (szCatBuf, "\r\nversion %d.%d %s (Build %d)",
			   osvi.dwMajorVersion,
			   osvi.dwMinorVersion,
			   osvi.szCSDVersion,
			   osvi.dwBuildNumber & 0xFFFF);
		}
		else
		{ 
			sprintf (szCatBuf, "\r\n%s (Build %d)",
			   osvi.szCSDVersion,
			   osvi.dwBuildNumber & 0xFFFF);
		}
		break;

		// Test for the Windows 95 product family.
		case VER_PLATFORM_WIN32_WINDOWS:

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
		{
			 strcpy(szBuf, "Microsoft Windows 95 ");
			 if ( osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B' )
				strcat(szBuf, "OSR2 " );
		} 

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
		{
			 strcpy(szBuf, "Microsoft Windows 98 ");
			 if ( osvi.szCSDVersion[1] == 'A' )
				strcat(szBuf, "SE " );
		} 

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
		{
			 strcpy (szBuf, "Microsoft Windows Millennium Edition ");
		} 
		break;
	}

	if ( strcmp(szCatBuf, "\0") != 0 )
	{ // 추가사항이 있다.
		strcat (szBuf, szCatBuf);
	}
	
	strcpy(pszOS, szBuf);
}


// ===================================
// DX Version Info
// ===================================
DWORD SpecDxVer(char * pszDxVer)
{
	HRESULT hr;
    DWORD	dwDXVersion;
	char	strResult[128];
	
	hr = GetDXVersion(&dwDXVersion, pszDxVer, 10);
    if( SUCCEEDED(hr) )
    {
        if( dwDXVersion > 0 )
            _sntprintf( strResult, 128, TEXT("DirectX %s installed"), pszDxVer );
        else
            _tcsncpy( strResult, TEXT("DirectX not installed"), 128 );
        strResult[127] = 0;
    }
    else
    {
        _sntprintf( strResult, 128, TEXT("Unknown version of DirectX installed"), hr );
        strResult[127] = 0;
    }

	strcpy(pszDxVer, strResult);

	return dwDXVersion;
}


// ===================================
// Ram Spec
// ===================================
DWORD SpecRAM(char * pszRAM )
{
	MEMORYSTATUS	state;
	char			szBuf[255];

	memset(&state, 0, sizeof(state));
	state.dwLength = sizeof (state);
	GlobalMemoryStatus(&state);

	DWORD	dwTotalMemory;
	dwTotalMemory = DWORD(state.dwTotalPhys / MEGA)+1;
	
	itoa((DWORD)dwTotalMemory, szBuf, 10);

	strcpy(pszRAM, szBuf);

	return dwTotalMemory;
}


// ===================================
// Free Hard space
// ===================================
ULONG SpecHardDisk(const char * const  pszHard)
{
	char  szBuf[255];

	GetCurrentDirectory(255, szBuf); 



    ULARGE_INTEGER FreeBytesAvailableToCaller;	// receives the number of bytes on // disk available to the caller
    ULARGE_INTEGER TotalNumberOfBytes;			// receives the number of bytes on disk
    ULARGE_INTEGER TotalNumberOfFreeBytes;		// receives the free bytes on disk

	int iRet;
	iRet = GetDiskFreeSpaceEx(pszHard,					// current directory information	
		&FreeBytesAvailableToCaller, 
		&TotalNumberOfBytes,
		&TotalNumberOfFreeBytes);
	if (0 == iRet)
		return 0;

	ULONG	ulFreeMegaBytes = ULONG(TotalNumberOfFreeBytes.QuadPart / MEGA);	// 1048576 == 1024^2
	_ultoa(ulFreeMegaBytes, szBuf, 10);

//	strcpy(pszHard, szBuf);

	return ulFreeMegaBytes;
}

// ===================================
// Free Hard space
// pszHard에다가 문자열로도 얻어온다.
// ===================================
ULONG CurSpecHardDisk(char * pszHard)
{
	char  szBuf[255];

	GetCurrentDirectory(255, szBuf); 

//	CString strBuf = szBuf;

	// UNC name style 로 만든다.
//	int pos = 0;
//	for (int i = pos; i < strBuf.GetLength(); i++)
//	{
//		pos = strBuf.Find("\\", i);
//		if (pos != -1)
//		{ // 비어있지 않다.
//			i = pos +2;
//			strBuf.Insert(pos, "\\");			
//		}
//	}

    ULARGE_INTEGER FreeBytesAvailableToCaller;	// receives the number of bytes on // disk available to the caller
    ULARGE_INTEGER TotalNumberOfBytes;			// receives the number of bytes on disk
    ULARGE_INTEGER TotalNumberOfFreeBytes;		// receives the free bytes on disk

	int iRet;
	iRet = GetDiskFreeSpaceEx(szBuf,					// current directory information	
		&FreeBytesAvailableToCaller, 
		&TotalNumberOfBytes,
		&TotalNumberOfFreeBytes);
	if (0 == iRet)
		return 0;

	ULONG	ulFreeMegaBytes = ULONG(TotalNumberOfFreeBytes.QuadPart / MEGA);	// 1048576 == 1024^2
	_ultoa(ulFreeMegaBytes, szBuf, 10);

	strcpy(pszHard, szBuf);

	return ulFreeMegaBytes;
}

//-----------------------------------------------------------------------------
// Name: DSoundEnumCallback()
// Desc: Enumeration callback called by DirectSoundEnumerate
//-----------------------------------------------------------------------------
INT_PTR CALLBACK DSoundEnumCallback( GUID* pGUID, LPSTR strDesc, LPSTR strDrvName,
                                  VOID* pContext )
{
    // Set aside static storage space for 20 audio drivers
    static GUID  AudioDriverGUIDs[20];
    static DWORD dwAudioDriverIndex = 0;

	char* pBuf;

    GUID* pTemp  = NULL;

	pBuf = (char*)pContext;


	memcpy( pBuf, strDesc, strlen(strDesc) );
/*
    if( pGUID )
    {
        if( dwAudioDriverIndex >= 20 )
            return TRUE;

        pTemp = &AudioDriverGUIDs[dwAudioDriverIndex++];
        memcpy( pTemp, pGUID, sizeof(GUID) );
    }

    HWND hSoundDeviceCombo = (HWND)pContext;

    // Add the string to the combo box
    SendMessage( hSoundDeviceCombo, CB_ADDSTRING, 
                 0, (LPARAM) (LPCTSTR) strDesc );

    // Get the index of the string in the combo box
    INT nIndex = (INT)SendMessage( hSoundDeviceCombo, CB_FINDSTRING, 
                                   0, (LPARAM) (LPCTSTR) strDesc );

    // Set the item data to a pointer to the static guid stored in AudioDriverGUIDs
    SendMessage( hSoundDeviceCombo, CB_SETITEMDATA, 
                 nIndex, (LPARAM) pTemp );
*/
    return TRUE;
}


HRESULT InitDirectSound(HWND hWnd)
{
   HRESULT hr;

    // Free any previous DirectSound objects
    //FreeDirectSound();

	LPDIRECTSOUND	lpDS;
/*
    // Get the HWNDs the combo boxes
    //HWND hSoundDeviceCombo   = (HWND) GetDlgItem(IDC_SOUND_CARD_EDIT );

    // Get the index of the currently selected devices
    INT nSoundIndex   = (INT) ::SendMessage( hSoundDeviceCombo, CB_GETCURSEL, 0, 0 ); 

    // Get the GUID attached to the combo box item
    GUID* pSoundGUID = (GUID*) ::SendMessage( hSoundDeviceCombo, CB_GETITEMDATA, 
                                            nSoundIndex, 0 );
*/
    // Create IDirectSound using the select sound device
    if( FAILED( hr = DirectSoundCreate( /* pSoundGUID */ 0, &lpDS, NULL ) ) )
	{
        return DXTRACE_ERR( TEXT("DirectSoundCreate"), hr );
	}

    // Release the IDirectSound object immediately since we don't want
    // to limit this sample to only computers that support full duplex audio
//    SAFE_RELEASE( m_lpDS ); 

    return S_OK;
}
