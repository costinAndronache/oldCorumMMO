// LogCompareDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "afxcmn.h"
#include "ItemEx.h"


struct SItemAndIndex
{
	CItemEx	m_Item;
	DWORD	m_dwIndex;

	SItemAndIndex()
	{
	}

	SItemAndIndex( const CItemEx& item )
		: m_Item(item), m_dwIndex(0)
	{
	}

	SItemAndIndex( const CItemEx& item, DWORD dwIndex )
		: m_Item(item), m_dwIndex(dwIndex)
	{
	}

	bool	operator==( const SItemAndIndex& rhs ) const
	{
		if( m_dwIndex == rhs.m_dwIndex )
		{
			return m_Item == rhs.m_Item;
		}

		return false;
	}

	bool	operator<( const SItemAndIndex& rhs ) const
	{
		if( m_dwIndex < rhs.m_dwIndex )
		{
			return m_Item < rhs.m_Item;
		}

		return false;
	}
};

struct SItemAndID
{
	CItemEx	m_Item;
	char	m_szName[20];

	SItemAndID()
	{
	}

	SItemAndID( const CItemEx& item, LPCSTR szID )
		: m_Item(item)
	{
		::strcpy( m_szName,szID );
	}

	SItemAndID( const CItemEx& item )
		: m_Item(item)
	{
		m_szName[0] = 0;
	}

	bool	operator==( const SItemAndID& rhs ) const
	{
		if( strcmp( m_szName, rhs.m_szName ) == 0 )
		{
			return m_Item == rhs.m_Item;
		}

		return false;
	}

	bool	operator<( const SItemAndID& rhs ) const
	{
		if( strcmp( m_szName, rhs.m_szName ) <  0 )
		{
			return m_Item < rhs.m_Item;
		}

		return false;
	}
};


// CLogCompareDlg 대화 상자
class CLogCompareDlg : public CDialog
{
// 생성
public:
	CLogCompareDlg(CWnd* pParent = NULL);	// 표준 생성자

// 대화 상자 데이터
	enum { IDD = IDD_LOGCOMPARE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원


// 구현
protected:
	HICON m_hIcon;

	// 메시지 맵 함수를 생성했습니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	__inline CDBConnection&	GetDBConObject( int iIndex )
	{
		ASSERT(iIndex >= 0 && iIndex < static_cast<signed>(m_vecDBCon.size()));

		return m_vecDBCon[iIndex];
	}

	__inline CDBConnection&	GetMainDBConObject( int iIndex )
	{
		ASSERT(iIndex >= 0 && iIndex < static_cast<signed>(m_vecMainDBCon.size()));

		return m_vecMainDBCon[iIndex];
	}

	CComboBox m_cbServer;
	CMonthCalCtrl m_calSrc;
	CMonthCalCtrl m_calDst;
	CListCtrl m_listLog;
	afx_msg void OnCbnSelchangeComboServer();
	afx_msg void OnBnClickedButtonCompare();
private:
	vector< CDBConnection >		m_vecDBCon;
	vector< CDBConnection >		m_vecMainDBCon;
	vector< DWORD >				m_vecMallItem;
	vector< CItemEx >			m_vecItem;

	vector< SItemAndIndex >		m_vecItemAndIndex;
	vector< SItemAndID >		m_vecItemAndID;

	SYSTEMTIME					srcTime, dstTime;
	void UpdateServerComboBox(void);
	void UpdateListCtrl();

	BOOL	BuildItemList( vector< CItemEx >& rVectorItem, CDBConnection& rConnection );
	BOOL	BuildItemListAndCharIndex( vector< SItemAndIndex >& rVectorItem, CDBConnection& rConnection );
	BOOL	BuildItemListAndID( vector< SItemAndID >& rVectorItem, CDBConnection& rConnection );
	void	AddList( 
		UINT uMaxIndex, 
		vector< BYTE >& rVector, 
		vector< CItemEx >& rVectorItem 
		);
	void	AddList( 
		UINT uMaxIndex, 
		vector< BYTE >& rVector, 
		vector< SItemAndID >& rVectorItem,
		LPCSTR szID
		);
	void	AddList( 
			UINT uMaxIndex, 
			vector< BYTE >& rVector, 
			vector< SItemAndIndex >& rVectorItem,
			DWORD dwIndex
			);
public:
	afx_msg void OnAppExit();
	afx_msg void On57603();
	afx_msg void OnFileOpen();
	CProgressCtrl m_prCompare;
	afx_msg void OnLvnKeydownListLogResult(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_cbMainServer;
	afx_msg void OnBnClickedButtonCompareMain();
};
