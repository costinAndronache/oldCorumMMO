#if !defined(AFX_MYVIEW_H__FE14DE53_1CDA_4E2A_8717_EE14B414DC2A__INCLUDED_)
#define AFX_MYVIEW_H__FE14DE53_1CDA_4E2A_8717_EE14B414DC2A__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


#ifndef __AFXEXT_H__
#include <afxext.h>
#endif


#pragma pack(push,1)
struct HAVEITEM
{
	__int64 i64Serial;
	char	szCharName[20];
	BYTE	bwhere;
	BYTE	bindex;
};
#pragma pack(pop)


#include "ProgressCtrlX.h"


class CMyView : public CFormView
{

protected:

	CMyView(); 
	virtual ~CMyView();
	DECLARE_DYNCREATE(CMyView)

public:

	//{{AFX_DATA(CMyView)
	enum { IDD = IDD_MYFORM };
	CButton			m_NewSerial;
	CComboBox		m_ComBoCtrl;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMyView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	//{{AFX_MSG(CMyView)
	afx_msg void OnDestroy();
	afx_msg void OnGo();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:

	CApp*						m_pApp;
	int							m_nViewID;
	LOGFONT						m_logFont;
	CFont						m_Font;
	CEdit*						m_pEdit;
	CProgressCtrlX*				m_pProgress;

public:

	WORD						m_wRestartCount;
	DWORD						m_dwSerialNum;
	ITEM_SERIAL					m_Serial;

public:

	CWinThread*					m_pWinThread;
	BOOL						m_bRun;

public:

	HANDLE						m_hThread;

public:

	IOleDBThread*				m_pDb;

public:

	char						m_CORUM_DB_NAME		[CURRENT_DB_NUM][32];		
	char						m_CORUM_DB_ACCOUNT	[CURRENT_DB_NUM][32];		
	char						m_CORUM_DB_PASSWORD	[CURRENT_DB_NUM][32];	
	char						m_CORUM_DB_ADDRESS	[CURRENT_DB_NUM][32];
	char						m_CORUM_TDB_NAME	[32];		
	char						m_CORUM_TDB_ACCOUNT	[32];		
	char						m_CORUM_TDB_PASSWORD[32];	
	char						m_CORUM_TDB_ADDRESS	[32];
	BYTE						m_CORUM_GAME_DB		[CURRENT_DB_NUM];
	BYTE						m_CORUM_TOTAL_DB;

public:

	void						InSertNewSerial			();
	void						StepProgress			();
	BOOL						ResetItemSerialNumber	();
	ITEM_SERIAL					GetNewSerial			();
	void						InsertIntoHaveItemForDB	(int sel,HAVEITEM haveitem);

	BOOL						InitDatabase			();
	BOOL						InitCorumDB				();
	void						InitDBAccount			();
};

#endif 


class CTimeTick
{	

private:
	
    LARGE_INTEGER iPerfFreq;	
    LARGE_INTEGER iPerfStart;	
    LARGE_INTEGER iPerfEnd;	
	
public:	
	
    CTimeTick()
	{
		QueryPerformanceFrequency(&iPerfFreq);    
	}
	
    ~CTimeTick()		
    {		
    }
	
    void SetStart()
	{
		QueryPerformanceCounter(&iPerfStart);  // start point
	}
		
    void SetEnd()		
	{
        QueryPerformanceCounter(&iPerfEnd);    // end point		
	}
    	
    DWORD GetTick()		
	{
        return  (DWORD)(((iPerfEnd.QuadPart - iPerfStart.QuadPart)*1000000)/iPerfFreq.QuadPart);		
    }
	
};


