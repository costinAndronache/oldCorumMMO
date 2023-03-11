//-----------------------------------------------------------------------------
// File: GetDXVer.cpp
//
// Desc: Demonstrates how applications can detect what version of DirectX
//       is installed.
//
// (C) Copyright Microsoft Corp.  All rights reserved.
//-----------------------------------------------------------------------------
#define INITGUID
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <dxdiag.h>


HRESULT GetDirectXVersionViaDxDiag( DWORD* pdwDirectXVersionMajor, DWORD* pdwDirectXVersionMinor, TCHAR* pcDirectXVersionLetter );
HRESULT GetDirectXVerionViaFileVersions( DWORD* pdwDirectXVersionMajor, DWORD* pdwDirectXVersionMinor, TCHAR* pcDirectXVersionLetter );
HRESULT GetFileVersion( TCHAR* szPath, ULARGE_INTEGER* pllFileVersion );
ULARGE_INTEGER MakeInt64( WORD a, WORD b, WORD c, WORD d );
int CompareLargeInts( ULARGE_INTEGER ullParam1, ULARGE_INTEGER ullParam2 );


HRESULT GetDXVersion( DWORD* pdwDirectXVersion, TCHAR* strDirectXVersion, int cchDirectXVersion );

#define	DX0		0x00000000	/* No DirectX installed */
#define	DX10	0x00010000	/* DirectX 1.0 installed */
#define	DX20	0x00020000	/* DirectX 2.0 installed */
#define	DX30	0x00030000	/* DirectX 3.0 installed */
#define	DX30A	0x00030001	/* DirectX 3.0a installed */
#define	DX50	0x00050000	/* DirectX 5.0 installed */
#define DX60	0x00060000	/* DirectX 6.0 installed */
#define	DX61    0x00060100	/* DirectX 6.1 installed */
#define DX61A   0x00060101	/* DirectX 6.1a installed */
#define	DX70	0x00070000	/* DirectX 7.0 installed */
#define	DX70A	0x00070001	/* DirectX 7.0a installed */
#define	DX80	0x00080000	/* DirectX 8.0 installed */
#define	DX81	0x00080100	/* DirectX 8.1 installed */
#define	DX81A   0x00080101	/* DirectX 8.1a installed */
#define	DX81B   0x00080102	/* DirectX 8.1b installed */
#define	DX82	0x00080200	/* DirectX 8.2 installed */
#define	DX90    0x00090000	/* DirectX 9.0 installed */
#define	DX90A	0x00090001	/* DirectX 9.0a installed */

