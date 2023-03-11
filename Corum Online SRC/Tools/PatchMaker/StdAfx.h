// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__C0C76591_DF79_4023_81DF_F4DAD2542EE5__INCLUDED_)
#define AFX_STDAFX_H__C0C76591_DF79_4023_81DF_F4DAD2542EE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma comment(linker, "/subsystem:Windows,5")
#pragma comment(linker, "/version:5")

#define _WIN32_WINNT	0x0500
#ifdef WINVER
#undef WINVER
#define WINVER			0x0500
#endif

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#define __NO_COMPARISION
//#define __SLICED_ZIP_FORMAT


#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__C0C76591_DF79_4023_81DF_F4DAD2542EE5__INCLUDED_)


