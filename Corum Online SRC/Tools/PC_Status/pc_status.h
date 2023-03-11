// pc_status.h : main header file for the PC_STATUS application
//

#if !defined(AFX_PC_STATUS_H__FD11FB14_9E8C_4D5C_B227_25D6B790DA3C__INCLUDED_)
#define AFX_PC_STATUS_H__FD11FB14_9E8C_4D5C_B227_25D6B790DA3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPc_statusApp:
// See pc_status.cpp for the implementation of this class
//

class CPc_statusApp : public CWinApp
{
public:
	CPc_statusApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPc_statusApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPc_statusApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PC_STATUS_H__FD11FB14_9E8C_4D5C_B227_25D6B790DA3C__INCLUDED_)
