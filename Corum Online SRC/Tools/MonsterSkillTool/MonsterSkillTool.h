// MonsterSkillTool.h : main header file for the MONSTERSKILLTOOL application
//

#if !defined(AFX_MONSTERSKILLTOOL_H__4600615E_FF51_4631_8401_01CA6D45C02A__INCLUDED_)
#define AFX_MONSTERSKILLTOOL_H__4600615E_FF51_4631_8401_01CA6D45C02A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMonsterSkillToolApp:
// See MonsterSkillTool.cpp for the implementation of this class
//

class CMonsterSkillToolApp : public CWinApp
{
public:
	CMonsterSkillToolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonsterSkillToolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMonsterSkillToolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONSTERSKILLTOOL_H__4600615E_FF51_4631_8401_01CA6D45C02A__INCLUDED_)
