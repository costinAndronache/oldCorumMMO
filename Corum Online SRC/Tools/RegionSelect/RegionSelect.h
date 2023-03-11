// RegionSelect.h : main header file for the REGIONSELECT application
//

#if !defined(AFX_REGIONSELECT_H__C32AE4AC_391E_426A_9AD5_8D3CB64EF844__INCLUDED_)
#define AFX_REGIONSELECT_H__C32AE4AC_391E_426A_9AD5_8D3CB64EF844__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRegionSelectApp:
// See RegionSelect.cpp for the implementation of this class
//

class CRegionSelectApp : public CWinApp
{
public:
	CRegionSelectApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegionSelectApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRegionSelectApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGIONSELECT_H__C32AE4AC_391E_426A_9AD5_8D3CB64EF844__INCLUDED_)
