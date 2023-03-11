#if !defined(AFX_AUTOPATCH_H__82B56A03_EC13_4B72_8544_25634E90AAA7__INCLUDED_)
#define AFX_AUTOPATCH_H__82B56A03_EC13_4B72_8544_25634E90AAA7__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


#include <afxinet.h>
#include "ProgressCtrlX.h"


#define DEFAULT_FTP_ADDRESS						"222.106.120.45"	// AutoPatch FTP
#define DEFAULT_FTP_ACCOUNT						"jjw"	
#define DEFAULT_FTP_PASSWORD					"jjw"			
#define DEFAULT_FTP_PORT						21


class CAutoPatch : public CDialog
{
public:

	CAutoPatch(CWnd* pParent = NULL);  

	//{{AFX_DATA(CAutoPatch)
	enum { IDD = IDD_AUTOPATCH };
	CProgressCtrlX	m_Progress;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAutoPatch)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CAutoPatch)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

	CFtpConnection*		m_pFtp;
	CInternetSession	m_Session;

	void				CloseConnection		();
};

#endif 
