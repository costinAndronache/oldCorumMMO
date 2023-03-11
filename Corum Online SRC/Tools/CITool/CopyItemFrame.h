#if !defined(AFX_COPYITEMFRAME_H__75DE61A4_5662_4F51_9563_C9BF610675DC__INCLUDED_)
#define AFX_COPYITEMFRAME_H__75DE61A4_5662_4F51_9563_C9BF610675DC__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


class CCopyItemFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CCopyItemFrame)
protected:
	CCopyItemFrame();           

public:

	//{{AFX_VIRTUAL(CCopyItemFrame)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:

	virtual ~CCopyItemFrame();

	//{{AFX_MSG(CCopyItemFrame)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

	int			m_minWidth;
    int			m_minHeight; 
	int			m_nFrameID;
};

#endif 
