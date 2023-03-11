#if !defined(AFX_GUILDFRAME_H__194B4069_CE45_4D2B_B3F7_97AA5A4BDCC5__INCLUDED_)
#define AFX_GUILDFRAME_H__194B4069_CE45_4D2B_B3F7_97AA5A4BDCC5__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


class CGuildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CGuildFrame)
protected:
	CGuildFrame();           

public:

	//{{AFX_VIRTUAL(CGuildFrame)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:

	virtual ~CGuildFrame();

	//{{AFX_MSG(CGuildFrame)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	
	int			m_minWidth;
    int			m_minHeight; 
	int			m_nFrameID;
};


#endif
