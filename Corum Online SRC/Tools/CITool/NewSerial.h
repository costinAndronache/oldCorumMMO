#if !defined(AFX_NEWSERIAL_H__779A0D73_6A23_463E_914C_6F8653C82FB6__INCLUDED_)
#define AFX_NEWSERIAL_H__779A0D73_6A23_463E_914C_6F8653C82FB6__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif


#include "resource.h"    


class CApp : public CWinApp
{

public:

	CApp();

	//{{AFX_VIRTUAL(CApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:

	DWORD						m_dwDocCount;
	DWORD						m_dwDocCount2;
	DWORD						m_dwDocCount3;
	DWORD						m_dwDocCount4;
	CList<CBaseItem,CBaseItem&>	m_ItemList;
	CList<WORD,WORD&>			m_ItemNoList;
	LoginInfo					m_LogInfo;				// 로그인하는 툴 관리자의정보
	char						m_szLocalAddress[16];	// 사용자 컴퓨터의 IP받아올 변수

public:
	
	BOOL		InitDatabase	();
	void		InitDBAccount	();
	BOOL		InitCorumDB		();
	void		LoadBaseItem	();
	DWORD		GetVersion		();
	
};


#endif 
