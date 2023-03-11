#if !defined(AFX_UITHREAD_H__CB80968A_DCC8_4889_BD9B_FC9A86E52A2D__INCLUDED_)
#define AFX_UITHREAD_H__CB80968A_DCC8_4889_BD9B_FC9A86E52A2D__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


class CUIThread : public CWinThread
{
	DECLARE_DYNCREATE(CUIThread)

protected:

	CUIThread();      
	virtual ~CUIThread();

public:

	//{{AFX_VIRTUAL(CUIThread)
	public:
	virtual int ExitInstance();
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CUIThread)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};

#endif 
