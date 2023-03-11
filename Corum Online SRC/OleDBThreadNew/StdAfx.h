// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__FDD2B1F8_E0E0_43D8_B432_12AA13B5BE7B__INCLUDED_)
#define AFX_STDAFX_H__FDD2B1F8_E0E0_43D8_B432_12AA13B5BE7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define _WIN32_WINNT 0x0500

#include <windows.h>
#include <process.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define _CHECK_MEMORY()	if(_CrtCheckMemory()==NULL) { __asm int 3 }
#else
#define _CHECK_MEMORY()	
#endif

#ifdef _DEBUG
#define assert(x)	if(!x) { _asm int 3  }
#else
#define assert(x)
#endif

typedef unsigned (__stdcall *PTHREAD_START)(void*);

#define BEGINTHREADEX(psa, cbStack, pfnStartAddr, pvParam, fdwCreate, pdwThreadId) \
	((HANDLE)_beginthreadex((void*)(psa), \
	(unsigned)(cbStack), (PTHREAD_START)(pfnStartAddr),\
	(void*)(pvParam), (unsigned)(fdwCreate), (unsigned*)(pdwThreadId)))




#define DBINITCONSTANTS
#define INITGUID

#include <initguid.h>
#include <oledb.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <stddef.h>
#include "Global.h"


#pragma comment(lib, ".\\SS3DGFunc.lib")

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__FDD2B1F8_E0E0_43D8_B432_12AA13B5BE7B__INCLUDED_)
