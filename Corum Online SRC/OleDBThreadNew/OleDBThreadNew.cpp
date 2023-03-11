// OleDBThreadNew.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "IOleDBThread.h"
#include "Factory.h"
#include "ExceptionFilter.h"

#pragma warning (disable : 4268)
EXTERN_C const CLSID CLSID_OLEDBTHREAD;
#pragma warning (default : 4268)


LONG			g_cObjects	= 0;		// COM 개체 카운터
LONG			g_cLocks	= 0;		// 로크 카운터
static HMODULE	g_hModule	= 0;


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	lpReserved;

	switch(ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_hModule = (HMODULE)hModule;

		// added by deepdark.
		InitializeCriticalSection(&g_ExceptionCS);
		break;
		
	case DLL_PROCESS_DETACH: 
		// added by deepdark.
		DeleteCriticalSection(&g_ExceptionCS);
		break;

	default:
		break;
	}

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////

extern "C"
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
#ifdef OLEDB_DLL

	HRESULT			hr = CLASS_E_CLASSNOTAVAILABLE;
	IUnknown*		pUnk = NULL;
	IClassFactory*	pCF;

	*ppv = NULL;
	
	if(rclsid == CLSID_OLEDBTHREAD)
	{
		hr = E_OUTOFMEMORY;
		pUnk = new CFOleDBThread;

		if(pUnk)
		{
			hr = pUnk->QueryInterface(IID_IUnknown, (void**)&pCF);
			
			if(FAILED(hr))
			{
				delete pUnk;
				pUnk = NULL;
				return hr;
			}

			hr = pCF->CreateInstance( NULL, riid, ppv);
			pCF->Release();
		}
	}	
	return hr;

#else

	HRESULT hr = CLASS_E_CLASSNOTAVAILABLE;
	IUnknown* pUnk = NULL;

	if(rclsid == CLSID_OLEDBTHREAD) {
		hr = E_OUTOFMEMORY;
		pUnk = new CFOleDBThread;
	}

	if(pUnk != NULL) {
		hr = pUnk->QueryInterface(riid, ppv);
		if(FAILED(hr))
			delete pUnk;
	}

	return hr;
	
#endif	
}

extern "C"
STDAPI DllCanUnloadNow(void)
{
	if(g_cObjects == 0 && g_cLocks == 0)
		return S_OK;
	return S_FALSE;
}

BOOL SetRegKeyValue(LPTSTR pszKey, 
					LPTSTR pszSubkey, 
					LPTSTR pszValue);

extern "C"
STDAPI DllRegisterServer (void)
{
	HRESULT hr = NOERROR;
	TCHAR szID[129];
	TCHAR szCLSID[129];
	TCHAR szModulePath[MAX_PATH];
	wchar_t wszCLSID[129];

	GetModuleFileName(g_hModule, szModulePath, 
		sizeof(szModulePath)/sizeof(TCHAR));
	StringFromGUID2(CLSID_OLEDBTHREAD, wszCLSID, 128);
	wcstombs(szID, wszCLSID, 128) ;

	lstrcpy(szCLSID, TEXT("CLSID\\"));
	lstrcat(szCLSID, szID);
	SetRegKeyValue(
		TEXT("OleDBThread.OleDBThread.1"),
		NULL,
		TEXT("OLEDB COM Model by Byung-Soo Koo"));
	SetRegKeyValue(
		TEXT("OleDBThread.OleDBThread.1"),
		TEXT("CLSID"),
		szID);
	
	SetRegKeyValue(
		szCLSID,
		NULL,
		TEXT("OleDBThread Component"));
	SetRegKeyValue(
		szCLSID,
		TEXT("ProgID"),
		TEXT("OleDBThread.OleDBThread.1"));
	SetRegKeyValue(
		szCLSID,
		TEXT("InprocServer32"),
		szModulePath);

	return (hr);
}

extern "C"
STDAPI DllUnregisterServer (void)
{
	HRESULT hr = NOERROR;
	TCHAR szID[129];
	TCHAR szCLSID[129];
	TCHAR szTemp[129];
	wchar_t wszCLSID[129];

	StringFromGUID2(CLSID_OLEDBTHREAD, wszCLSID, 128);
	wcstombs(szID, wszCLSID, 128) ;

	lstrcpy(szCLSID, TEXT("CLSID\\"));
	lstrcat(szCLSID, szID);

	RegDeleteKey(HKEY_CLASSES_ROOT, 
		TEXT("COleDBThread.COleDBThread.1\\CLSID"));
	RegDeleteKey(HKEY_CLASSES_ROOT, 
		TEXT("COleDBThread.COleDBThread.1"));

	wsprintf(szTemp, TEXT("%s\\%s"), szCLSID, 
		TEXT("InprocServer32"));
	RegDeleteKey(HKEY_CLASSES_ROOT, szTemp);
	wsprintf(szTemp, TEXT("%s\\%s"), szCLSID, 
		TEXT("ProgID"));
	RegDeleteKey(HKEY_CLASSES_ROOT, szTemp);
	RegDeleteKey(HKEY_CLASSES_ROOT, szCLSID);

	return (hr);
}

BOOL SetRegKeyValue(LPTSTR pszKey, 
					LPTSTR pszSubkey, 
					LPTSTR pszValue)
{
	BOOL bOk = FALSE;
	LONG ec;
	HKEY hKey;
	TCHAR szKey[256];

	lstrcpy(szKey, pszKey);

	if(NULL != pszSubkey) {
		lstrcat(szKey, TEXT("\\"));
		lstrcat(szKey, pszSubkey);
	}

	ec = RegCreateKeyEx(
			HKEY_CLASSES_ROOT,
			szKey,
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS,
			NULL,
			&hKey,
			NULL);

	if(ERROR_SUCCESS == ec) {
		if(NULL != pszValue) {
			ec = RegSetValueEx(
						hKey,
						NULL,
						0,
						REG_SZ,
						(BYTE *)pszValue,
						(lstrlen(pszValue)+1)*sizeof(TCHAR));
		}
		if(ERROR_SUCCESS == ec)
			bOk = TRUE;
		RegCloseKey(hKey);
	}
	return bOk;
}

