#if !defined(AFX_COPYITEMDELETEFRAME_H__ACC20EEB_2AB9_4FA5_BD9C_0E92FF8A6186__INCLUDED_)
#define AFX_COPYITEMDELETEFRAME_H__ACC20EEB_2AB9_4FA5_BD9C_0E92FF8A6186__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


class CCopyItemDeleteFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CCopyItemDeleteFrame)
protected:
	CCopyItemDeleteFrame();

public:

	//{{AFX_VIRTUAL(CCopyItemDeleteFrame)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:

	virtual ~CCopyItemDeleteFrame();

	//{{AFX_MSG(CCopyItemDeleteFrame)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	
	int			m_minWidth;
    int			m_minHeight; 
	int			m_nFrameID;
};

#endif 





















