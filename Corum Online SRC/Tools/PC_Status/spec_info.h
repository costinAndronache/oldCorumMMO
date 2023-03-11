#pragma once

#include "global_define.h"
#include "GetDxVer.h"
#include "cpu_info.h"


INT_PTR CALLBACK DSoundEnumCallback( GUID* pGUID, LPSTR strDesc, LPSTR strDrvName,
                                  VOID* pContext );

void	SpecOS( char * pszOS );
DWORD	SpecDxVer( char * pszDxVer );		// 0x801(8.1) 이런 식으로 넘어온다.
DWORD	SpecRAM( char * pszRAM );			// MEGA 바이트 단위로 리턴
ULONG	SpecHardDisk( const char * const pszHard );		// MEGA 바이트 단위로 리턴
ULONG	CurSpecHardDisk( char * pszHard );		// MEGA 바이트 단위로 리턴
DWORD	SpecVidMem( char * pszVidMem, char * pszAGPMem );
HRESULT	SpecSoundCard(char * pszSound);

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

//void map_mname( int family, int model, const char * v_name, char *m_name );



extern CPUInfo			g_CPUInfo;