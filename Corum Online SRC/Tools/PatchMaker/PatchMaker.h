// PatchMaker.h : main header file for the PATCHMAKER application
//

#if !defined(AFX_PATCHMAKER_H__46181F1E_E383_4C50_BA05_C89C8E3C396D__INCLUDED_)
#define AFX_PATCHMAKER_H__46181F1E_E383_4C50_BA05_C89C8E3C396D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPatchMakerApp:
// See PatchMaker.cpp for the implementation of this class
//

class CPatchMakerApp : public CWinApp
{
public:
	CPatchMakerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPatchMakerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPatchMakerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATCHMAKER_H__46181F1E_E383_4C50_BA05_C89C8E3C396D__INCLUDED_)
