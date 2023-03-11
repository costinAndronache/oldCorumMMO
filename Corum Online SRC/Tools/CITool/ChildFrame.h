#if !defined(AFX_CHILDFRAME_H__5043BC4C_88EE_478B_8CF6_913AD6BE552F__INCLUDED_)
#define AFX_CHILDFRAME_H__5043BC4C_88EE_478B_8CF6_913AD6BE552F__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)

public:

	CChildFrame();
	virtual ~CChildFrame();

	//{{AFX_VIRTUAL(CChildFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	//{{AFX_MSG(CChildFrame)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:

	int			m_minWidth;
    int			m_minHeight; 
	int			m_nFrameID;
};

#endif 
