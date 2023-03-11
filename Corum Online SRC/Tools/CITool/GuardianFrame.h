#if !defined(AFX_GUARDIANFRAME_H__DBD65B84_19CB_4F99_972C_5E148C78F62B__INCLUDED_)
#define AFX_GUARDIANFRAME_H__DBD65B84_19CB_4F99_972C_5E148C78F62B__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


class CGuardianFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CGuardianFrame)

protected:
	CGuardianFrame();

public:

	//{{AFX_VIRTUAL(CGuardianFrame)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	virtual ~CGuardianFrame();

	//{{AFX_MSG(CGuardianFrame)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

	int			m_minWidth;
    int			m_minHeight; 
	int			m_nFrameID;
};

#endif 
