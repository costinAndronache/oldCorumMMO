#include "graphic_card_info.h"
#include "getdllversion.h"
#include "resource.h"

// ===================================
// Version 정보 뒷자리를 알아온다.
//
//
// ===================================
int	GetGraphicDrvVerFromString( char * pszString )
{
	int		i;
	char *	pStrPos;
	int		iVersion;

	pStrPos = strstr(pszString, ".");
	for (i = 0; i < 2; i++)
	{
		++pStrPos;
		pStrPos = strstr(pStrPos, ".");
	}
	
	++pStrPos;
	iVersion = atoi((LPCTSTR)pStrPos);

	return iVersion;
}



// ==============================================
// SpecVidMem
// 그래픽카드의 비디오 메모리량을 알아 KB 단위로 리턴 
// ==============================================
DWORD SpecVidMem(char * pszVidMem, char * pszAGPMem)
{
	LPDIRECTDRAW	lpDD; 
	LPDIRECTDRAW2	lpDD2; 
	DDSCAPS2		ddsCaps; 
	DWORD			dwTotal; 
	DWORD			dwFree; 

	char			szBuf[255];

	HRESULT			hr;

    hr = DirectDrawCreate( NULL, &lpDD, NULL );
    if( FAILED(hr) )
    {
		strcpy(pszVidMem, "Can't create directdraw object!");
		return -1;
    }

    // Let's see if IID_IDirectDraw2 exists.
    hr = lpDD->QueryInterface( IID_IDirectDraw2, (VOID**)&lpDD2 );
    if( FAILED(hr) )
    {
        // No IDirectDraw2 exists... must be DX1
        lpDD->Release();

		strcpy(pszVidMem, "Can't Query IID_IDirectDraw2");
        return -1;
    }

//	DXSAMPLEFORMATENUM dsPixelFormatEnum;
	//lpDD->get

	// GET VIDEO MEMORY	
	CString	strBuf;
	DWORD dwVidMemory;
	ddsCaps.dwCaps = // 현재 구할 캡스 스테이트 
		DDSCAPS_OFFSCREENPLAIN|DDSCAPS_VIDEOMEMORY|DDSCAPS_LOCALVIDMEM; 
	hr = lpDD2->GetAvailableVidMem((DDSCAPS*)&ddsCaps, &dwTotal, &dwFree); 
	if (FAILED(hr)) 
	{
		
		lpDD2->Release();
		lpDD->Release();

		strBuf.LoadString(IDS_MSG1);
		MessageBox(NULL, (LPCSTR)strBuf, "ERROR", MB_OK);
		return -1;
	}
	
	// 전체 비디오 메모리 / 사용가능 비디오 메모리 
	if (pszVidMem)
	{
		dwTotal = dwTotal / KILO;
		ultoa(dwTotal, szBuf, 10);
		strcpy(pszVidMem, szBuf);
		dwFree = dwFree / KILO;
		ultoa(dwFree, szBuf, 10);
		strcat(pszVidMem, " / ");
		strcat(pszVidMem, szBuf);	
	}

	dwVidMemory = dwTotal;

	// GET AGP MEMORY
	ddsCaps.dwCaps = // 현재 구할 캡스 스테이트 
				DDSCAPS_OFFSCREENPLAIN|DDSCAPS_VIDEOMEMORY|DDSCAPS_NONLOCALVIDMEM; 

	hr = lpDD2->GetAvailableVidMem((DDSCAPS*)&ddsCaps, &dwTotal, &dwFree); 
	if (FAILED(hr)) 
	{
		lpDD2->Release();
		lpDD->Release();

		strBuf.LoadString(IDS_MSG1);
		MessageBox(NULL, (LPCSTR)strBuf, "ERROR", MB_OK);
		return -1;
	}

	dwTotal = dwTotal / KILO;
	if (pszAGPMem)
	{
		ultoa(dwTotal, szBuf, 10);
		strcpy(pszAGPMem, szBuf);
	}

    // IDirectDraw2 exists. We must be at least DX2
    lpDD2->Release();
	lpDD->Release();

	return dwVidMemory;
}


// ===================================
// Graphic Card Spec
// CASE :
//  - 그래픽카드 명
//  - 그래픽카드 드라이버 버전
//  - 디스플레이 모드  
//  ...
// return : 버전 뒷자리 4자리를 리턴한다.(숫자)
// ===================================
int SpecGrapicCard(char * pszGraphic, char * pszGraphicDriverVersion, char * pszDisplayMode)
{
	char			szBuf[255];
	D3DDEVTYPE		d3dType = D3DDEVTYPE_HAL;
	D3DCAPS8		d3dCaps;
	LPDIRECT3D8		lpD3D;
	HRESULT			hr;
	int				nRetVer;	// 뒷자리 4자리를 숫자로 내보낸다.
	CString			strBuf;

	// D3D 객체 얻음 
	if( NULL == ( lpD3D = Direct3DCreate8( D3D_SDK_VERSION ) ) )
	{
		AfxMessageBox("Error : Create Direct3D ");
		return 0;
	}

	hr = lpD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, d3dType, &d3dCaps);
	if ( FAILED(hr) )
	{
		AfxMessageBox("GetDeviceCaps Error");
        return 0;		
	}

	// 어뎁터(그래픽 카드)의 정보 얻기 
	D3DADAPTER_IDENTIFIER8  Adapter;
    if( FAILED( lpD3D->GetAdapterIdentifier(0, D3DENUM_NO_WHQL_LEVEL, &Adapter) ) )
	{
		AfxMessageBox("AdapterIdentifer Error");
        return 0;
	}

	strcpy(pszGraphic, Adapter.Description);

	// driver 버전 얻기
	CString StrVersionInfo;
	strcpy(szBuf, Adapter.Driver);	// 버전 파일 명 얻기
	GetDllVersion(szBuf);			// szBuf에 버전정보를 담아 온다.

	nRetVer = GetGraphicDrvVerFromString(szBuf);

	StrVersionInfo.Format("%s (%s)", Adapter.Driver, szBuf);

	strcpy(pszGraphicDriverVersion, (LPCTSTR)StrVersionInfo);
	
	// display mode 정보 얻기 
	D3DDISPLAYMODE d3ddm;
    if( FAILED( lpD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, & d3ddm ) ) )
	{
		strBuf.LoadString(IDS_MSG2);
		AfxMessageBox((LPCTSTR)strBuf);
        return 0;
	}

	sprintf(szBuf, "%dX%d %d Hz", d3ddm.Width, d3ddm.Height, d3ddm.RefreshRate);
	strcpy(pszDisplayMode, szBuf);

	// 객체 릴리즈 
	lpD3D->Release();

	return nRetVer;
}


// ===================================
// IsCompressedTextureFormatOk
//
// CASE : 압축 텍스쳐 지원하는지 안하는지를 알아옴 
// RETURN :
//			TRUE, FALSE ;	지원한다, 안한다,
// ===================================
BOOL IsCompressedTextureFormatOk()
{
	BOOL					bResult = FALSE;
	LPDIRECT3D8				lpD3D;
	HRESULT					hr;

    if( NULL == ( lpD3D = Direct3DCreate8( D3D_SDK_VERSION ) ) )
	{
		MessageBox(NULL, "Can't Create Direct3D object!", "Error", MB_OK);
        return FALSE;
	}

	D3DDISPLAYMODE			d3ddm;
	if( FAILED( lpD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )
	{
		lpD3D->Release();
		return E_FAIL;
	}

	hr = lpD3D->CheckDeviceFormat( D3DADAPTER_DEFAULT,
                                      D3DDEVTYPE_HAL,
                                      d3ddm.Format,
                                      0,
                                      D3DRTYPE_TEXTURE,
                                      D3DFMT_DXT1);

	if (hr == D3D_OK)
	{
		bResult = TRUE;
	}

	lpD3D->Release();

	return bResult;
}


// ====================================================
// SpecGraphicCard()
// - 어떤 그래픽카드인지 알아서 정보를 리턴함.
// ====================================================
enum eGRAPHIC_CARD_OPTION IdentityGraphicCard()
{
	char					szBuf[MAXPATH];
	D3DDEVTYPE				d3dType = D3DDEVTYPE_HAL;
	LPDIRECT3D8				lpD3D;
	eGRAPHIC_CARD_OPTION	eFlag = NORMAL;

	// D3D 객체 얻음 
	if( NULL == ( lpD3D = Direct3DCreate8( D3D_SDK_VERSION ) ) )
	{
		MessageBox(NULL, "Error : Create Direct3D object", NULL, MB_OK);
		return OPTION_NUM;	
	}

	// 어뎁터(그래픽 카드)의 정보 얻기 
	D3DADAPTER_IDENTIFIER8  Adapter;
    if( FAILED( lpD3D->GetAdapterIdentifier(0, D3DENUM_NO_WHQL_LEVEL, &Adapter) ) )
	{	
		MessageBox(NULL, "AdapterIdentifer Error", NULL, MB_OK);
		
		lpD3D->Release();		
        return OPTION_NUM;
	}	

	int		i;
	char *	pString;
	bool	bRadeon8500MoreThan = FALSE;
	DWORD	dwMemKilo;
	
	dwMemKilo = SpecVidMem(NULL, NULL);
	
	switch(Adapter.VendorId)
	{
	case NVIDIA_VENTDER_ID:
		if (dwMemKilo < 30000)
			eFlag = POOR_CARD;
		else
			eFlag = NORMAL;

		goto lb_Exit;

	case ATI_VENTDER_ID:
		for (i = 0; i < 10; i++)
		{ // 숫자가 나오면 무조건 검사한다. 
			itoa(i, szBuf, 10);
			pString = strstr(Adapter.Description, szBuf);

			if (!pString)
				continue;

			if ( strstr(Adapter.Description, "7000") || 
				 strstr(Adapter.Description, "7200") ||
				 strstr(Adapter.Description, "7500") )
			{
				 bRadeon8500MoreThan = FALSE;
				 break;
			}
			else if ( strstr(Adapter.Description, "8500") )
			{
				bRadeon8500MoreThan = TRUE;
				break;
			}
			else if ( strstr(Adapter.Description, "9000") )
			{
				bRadeon8500MoreThan = TRUE;
				break;
			}
			else if ( strstr(Adapter.Description, "9100") )		
			{
				bRadeon8500MoreThan = TRUE;
				break;
			}
			else if ( strstr(Adapter.Description, "9200") )		
			{
				bRadeon8500MoreThan = TRUE;
				break;
			}
			else if ( strstr(Adapter.Description, "9500") )		
			{
				bRadeon8500MoreThan = TRUE;
				break;
			}
			else if ( strstr(Adapter.Description, "9600") )		
			{
				bRadeon8500MoreThan = TRUE;
				break;
			}
			else if ( strstr(Adapter.Description, "9700") )		
			{
				bRadeon8500MoreThan = TRUE;
				break;
			}
			else if ( strstr(Adapter.Description, "9800") )		
			{
				bRadeon8500MoreThan = TRUE;
				break;
			}
			else if ( strstr(Adapter.Description, "VE") )		
			{
				bRadeon8500MoreThan = TRUE;
			}
			else if (dwMemKilo > 120000)
			{ // vid mem 128 이상 
				bRadeon8500MoreThan = TRUE;
			}
			else
			{
				bRadeon8500MoreThan = FALSE;
			}
		}

		if (bRadeon8500MoreThan)
		{
			eFlag = RADEON_7500_ABOVE;
			goto lb_Exit;
		}
		else
		{
			eFlag = RADEON_7500_UNDER;
			goto lb_Exit;
		}

		break;
	case MATROX_VENTDER_ID:
		if (dwMemKilo < 30000)
			eFlag = POOR_CARD;
		else
			eFlag = MATROX;

		goto lb_Exit;

	case VOODOO_VENDER_ID:
		eFlag = VOODOO;
		goto lb_Exit;

	default:
		if (dwMemKilo < 30000)
			eFlag = POOR_CARD;
		else
			eFlag = NORMAL;	

		goto lb_Exit;
	}

lb_Exit:
	// release object
	lpD3D->Release();	
	
	return eFlag;
}

