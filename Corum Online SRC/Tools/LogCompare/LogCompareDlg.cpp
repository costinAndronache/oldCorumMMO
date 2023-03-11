// LogCompareDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "LogCompare.h"
#include "LogCompareDlg.h"
#include ".\logcomparedlg.h"
#include "ItemEx.h"
#include "DebugLog.h"

const size_t SIZE_OF_EQUIP = sizeof(CItemEx) * MAX_EQUIP;
const size_t SIZE_OF_LARGE = sizeof(CItemEx) * MAX_INV_LARGE;
const size_t SIZE_OF_SMALL = sizeof(CItemEx) * MAX_INV_SMALL;
const size_t SIZE_OF_BELT = sizeof(CItemEx) * MAX_BELT;
const size_t SIZE_OF_GUARDIAN = sizeof(CItemEx) * MAX_INV_GUARDIAN;

const size_t SIZE_OF_BANK_LARGE = sizeof(CItemEx)*MAX_BANK_LARGE;
const size_t SIZE_OF_BANK_SMALL = sizeof(CItemEx)*MAX_BANK_SMALL;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 대화 상자 데이터
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원

	// 구현
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CLogCompareDlg 대화 상자



CLogCompareDlg::CLogCompareDlg(CWnd* pParent /*=NULL*/)
: CDialog(CLogCompareDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLogCompareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SERVER, m_cbServer);
	DDX_Control(pDX, IDC_MONTHCALENDAR_SRC, m_calSrc);
	DDX_Control(pDX, IDC_MONTHCALENDAR_DST, m_calDst);
	DDX_Control(pDX, IDC_LIST_LOG_RESULT, m_listLog);
	DDX_Control(pDX, IDC_PROGRESS_, m_prCompare);
	DDX_Control(pDX, IDC_COMBO_MAIN, m_cbMainServer);
}

BEGIN_MESSAGE_MAP(CLogCompareDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_SERVER, OnCbnSelchangeComboServer)
	ON_BN_CLICKED(IDC_BUTTON_COMPARE, OnBnClickedButtonCompare)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_COMMAND(ID_FILE_SAVE, On57603)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_LOG_RESULT, OnLvnKeydownListLogResult)
	ON_BN_CLICKED(IDC_BUTTON_COMPARE_MAIN, OnBnClickedButtonCompareMain)
END_MESSAGE_MAP()


// CLogCompareDlg 메시지 처리기

BOOL CLogCompareDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	// 프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	// 콤보박스 업데이트.
	UpdateServerComboBox();

	m_listLog.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	LPTSTR	lpColumn[]		= 
	{
		"SerialNum",	
		"ItemID",		
		"SerialID",		
		"ServerSet",
		"RestartCount",
		"GuardianItemID",
		"GuardianItemEndLife",
		"CharName or ID"
	};

	int			ColumnSize[]	= {100,100,100,100,100,100,100,100};	
	LV_COLUMN	col;
	
	for( int ic = 0 ; ic < 8 ; ic++ )
	{
		//ListView 컨트롤 초기화..  칼럼설정 	
		col.mask		= LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT;
		col.fmt			= LVCFMT_LEFT;
		col.cx			= ColumnSize[ic];
		col.pszText		= lpColumn[ic];
		col.cchTextMax	= 100;
		col.iSubItem	= ic;

		m_listLog.InsertColumn(ic, &col);
	}

	m_calSrc.SetMaxSelCount( 20 );
	m_calDst.SetMaxSelCount( 20 );

	fstream	file( "MallItem.txt" );

	DWORD	dwMallItem;
	while( file.eof() == false )
	{
		file>>dwMallItem;

		m_vecMallItem.push_back( dwMallItem );
	}

	::sort( m_vecMallItem.begin(), m_vecMallItem.end() );

	return TRUE;  // 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
}

void CLogCompareDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면 
// 아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.

void CLogCompareDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다. 
HCURSOR CLogCompareDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLogCompareDlg::OnCbnSelchangeComboServer()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_listLog.DeleteAllItems();
}

struct CItemCompare : public unary_function< CItemEx, bool >
{
	CItemEx	m_Item;

	CItemCompare( const CItemEx& val )
		: m_Item( val )
	{
	}

    bool operator () ( const CItemEx& rhs )
	{
		if( m_Item.m_wItemID != rhs.m_wItemID )
			return false;

		if( m_Item.m_Serial.i64Serial != rhs.m_Serial.i64Serial )
			return false;

		return true;
	}
};

void CLogCompareDlg::OnBnClickedButtonCompare()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 날짜정보를 얻자.
	
	TCHAR		strBuffer[0xff];

	string		strSrc, strDst;

	if( FALSE != m_calSrc.GetCurSel( &srcTime ) )
	{
		if( FALSE != m_calDst.GetCurSel( &dstTime ) )
		{            			
			INT	iCurSel = m_cbServer.GetCurSel();

			if( iCurSel < 0 )
				return;

			CLogCompareApp*	pApp = reinterpret_cast< CLogCompareApp* >(AfxGetApp());
			CDBConnectionInfo&	rInfo = pApp->GetDBConnectionInfo( iCurSel );

			sprintf( 
				strBuffer, 
				"%s_%04d%02d%02d", 
				rInfo.GetDBName().c_str(),
				srcTime.wYear,
				srcTime.wMonth,
				srcTime.wDay
				);

			strSrc = strBuffer;

			sprintf( 
				strBuffer, 
//				"%s_%04d%02d%02d", 
				"%s",
				pApp->GetMainDBConnectionInfo( 0 ).GetDBName().c_str()
				//rInfo.GetDBName().c_str()/*,
				/*
				dstTime.wYear,
				dstTime.wMonth,
				dstTime.wDay*/
				);
			
			strDst = strBuffer;
			
			m_vecDBCon.reserve( 2 );
			m_vecDBCon.assign( 2, CDBConnection() );

			wstring	wstrArray[CDBConnectionInfo::CDBDM_MAX*2];

			rInfo.HS_MBTOWS( strSrc, wstrArray[CDBConnectionInfo::CDBDM_NAME] );
			rInfo.HS_MBTOWS( CDBConnectionInfo::CDBDM_ADDRESS, wstrArray[CDBConnectionInfo::CDBDM_ADDRESS] );
			rInfo.HS_MBTOWS( CDBConnectionInfo::CDBDM_ACCOUNT, wstrArray[CDBConnectionInfo::CDBDM_ACCOUNT] );
			rInfo.HS_MBTOWS( CDBConnectionInfo::CDBDM_PASSWORD, wstrArray[CDBConnectionInfo::CDBDM_PASSWORD] );

			if( false == m_vecDBCon[0].Init( 
				wstrArray[CDBConnectionInfo::CDBDM_ADDRESS],
				wstrArray[CDBConnectionInfo::CDBDM_NAME],
				wstrArray[CDBConnectionInfo::CDBDM_ACCOUNT],
				wstrArray[CDBConnectionInfo::CDBDM_PASSWORD]
				) )
			{
				ASSERT( !"Error" );
			}

			pApp->GetMainDBConnectionInfo( 0 ).HS_MBTOWS( strDst, wstrArray[CDBConnectionInfo::CDBDM_MAX+CDBConnectionInfo::CDBDM_NAME] );
			pApp->GetMainDBConnectionInfo( 0 ).HS_MBTOWS( CDBConnectionInfo::CDBDM_ADDRESS, wstrArray[CDBConnectionInfo::CDBDM_MAX+CDBConnectionInfo::CDBDM_ADDRESS] );
			pApp->GetMainDBConnectionInfo( 0 ).HS_MBTOWS( CDBConnectionInfo::CDBDM_ACCOUNT, wstrArray[CDBConnectionInfo::CDBDM_MAX+CDBConnectionInfo::CDBDM_ACCOUNT] );
			pApp->GetMainDBConnectionInfo( 0 ).HS_MBTOWS( CDBConnectionInfo::CDBDM_PASSWORD, wstrArray[CDBConnectionInfo::CDBDM_MAX+CDBConnectionInfo::CDBDM_PASSWORD] );

			if( false == m_vecDBCon[1].Init( 
				wstrArray[CDBConnectionInfo::CDBDM_MAX+CDBConnectionInfo::CDBDM_ADDRESS],
				wstrArray[CDBConnectionInfo::CDBDM_MAX+CDBConnectionInfo::CDBDM_NAME],
				wstrArray[CDBConnectionInfo::CDBDM_MAX+CDBConnectionInfo::CDBDM_ACCOUNT],
				wstrArray[CDBConnectionInfo::CDBDM_MAX+CDBConnectionInfo::CDBDM_PASSWORD]
				) )
				{
					ASSERT( !"Error" );
				}
		}
	}
	else
	{
		ASSERT( !"Error" );
	}

	// 연결되었다. 이제 아이템 정보를 뽑아온다.
	vector< CItemEx >	vecSrc;

	BuildItemList( vecSrc, m_vecDBCon[0] );
	sort( vecSrc.begin(), vecSrc.end() );

	vector< CItemEx >	vecDst;

	BuildItemList( vecDst, m_vecDBCon[1] );
	sort( vecDst.begin(), vecDst.end() );

	vector< CItemEx >::const_iterator iter_begin = vecDst.begin();
	vector< CItemEx >::const_iterator iter_end = vecDst.end();

	m_vecItem.clear();

	while( iter_begin != iter_end )
	{
		// 비교하여. 다른것의 리스트를 뽑는다......
		if( vecSrc.end() == find_if( vecSrc.begin(), vecSrc.end(), CItemCompare(*iter_begin) ) )
		{
			// 리스트 컨트롤에 넣는다.
			m_vecItem.push_back( *iter_begin );
		}

		++iter_begin;
	}

	sort( m_vecItem.begin(), m_vecItem.end() );

	UpdateListCtrl();
}

void CLogCompareDlg::UpdateServerComboBox(void)
{
	CLogCompareApp*	pApp = reinterpret_cast< CLogCompareApp* >(AfxGetApp());
	UINT	maxSize = pApp->GetDBConnectionInfoSize();

	for( UINT	uIter = 0 ; uIter < maxSize ; ++uIter )
	{
		m_cbServer.AddString( pApp->GetDBConnectionInfo( uIter ).GetDBAddress().c_str() );
	}

	m_cbServer.SetCurSel( 0 );

	maxSize = pApp->GetMainDBConnectionInfoSize();

	for( uIter = 0 ; uIter < maxSize ; ++uIter )
	{
		m_cbMainServer.AddString( pApp->GetMainDBConnectionInfo( uIter ).GetDBAddress().c_str() );
	}

	m_cbMainServer.SetCurSel( 0 );
}

BOOL	CLogCompareDlg::BuildItemList( vector< CItemEx >& rVectorItem, CDBConnection& rConnection )
{
	DWORD	dwCharCount = 0;
	TCHAR	szQuery[0xff];
/*
	sprintf( szQuery, "Select Count(*) From CharacterInfo" );

	{
		CArgQuery Query( rConnection.DataConnection() );
		Query.SetQuery( szQuery );
		if( Query.Execute( true ) && S_OK == Query.MoveFirst() && Query.GetResult( 1, dwCharCount ) )
		{	
		}
		else
			goto END;
	}

	DWORD dwBankinfoCount = 0;

	sprintf(szQuery,"Select Count(*) From BankInfo");

	{
		CArgQuery Query( rConnection.DataConnection() );
		Query.SetQuery( szQuery );

		if( Query.Execute( true ) && S_OK == Query.MoveFirst() && Query.GetResult( 1, dwBankinfoCount) )
		{	
		} // if( Query.Execute( szQuery ) )
		else 
			goto END;
	}
*/
//	m_prCompare.SetRange( 0, dwCharCount + dwBankinfoCount );
//	m_prCompare.SetPos( 0 );

	sprintf( szQuery,"Select [Char_Index],[Equip],[Inv_large],[Inv_small],[Belt],[Guardian] From CharacterInfo" );

	{
		CArgQuery Query( rConnection.DataConnection() );

		Query.SetQuery(szQuery);
		ULONG TotalCount = 0;

		if( Query.Execute( true ) )
		{
#ifdef TEST
			int	iCount = 0;
#endif
			if( S_OK == Query.MoveFirst() )
			{
				vector< BYTE > vecEquip( SIZE_OF_EQUIP, 0 );
				vector< BYTE > vecInvLarge( SIZE_OF_LARGE, 0 );
				vector< BYTE > vecInvSmall( SIZE_OF_SMALL, 0 );
				vector< BYTE > vecBelt( SIZE_OF_BELT, 0 );
				vector< BYTE > vecGuardian( SIZE_OF_GUARDIAN, 0 );

				do
				{
					if(0 ==  (rand()%4))
					{
						Sleep(1);
					}

					++TotalCount;

					ULONG index = 0;
					DWORD dwCharIndex = 0;				

					ZeroMemory( &vecEquip[0], SIZE_OF_EQUIP );
					ZeroMemory( &vecInvLarge[0], SIZE_OF_LARGE );
					ZeroMemory( &vecInvSmall[0], SIZE_OF_SMALL );
					ZeroMemory( &vecBelt[0], SIZE_OF_BELT );
					ZeroMemory( &vecGuardian[0], SIZE_OF_GUARDIAN );

					Query.GetResult( ++index, dwCharIndex );

					Query.GetResult( ++index, vecEquip );
					Query.GetResult( ++index, vecInvLarge );
					Query.GetResult( ++index, vecInvSmall );
					Query.GetResult( ++index, vecBelt );
					Query.GetResult( ++index, vecGuardian );

					AddList( MAX_EQUIP, vecEquip, rVectorItem );
					AddList( MAX_INV_LARGE, vecInvLarge, rVectorItem );
					AddList( MAX_INV_SMALL, vecInvSmall, rVectorItem );
					AddList( MAX_BELT, vecBelt, rVectorItem );
					AddList( MAX_INV_GUARDIAN, vecGuardian, rVectorItem );

//					m_prCompare.SetPos( m_prCompare.GetPos() + 1 );
				} while( S_OK == Query.MoveNext() 
#ifdef TEST
					&& ++iCount < 10000
#endif					
					);
			}

		}	 // if( Query.Execute( szQuery ) )
#ifndef TEST
/*		if(dwCharCount != TotalCount)
		{
			assert( !"Char Info Count fault" );
		}*/
#endif
	}	

	sprintf(szQuery,"Select [ID],[Bank_large],[Bank_small] From BankInfo" );

	{
		CArgQuery Query( rConnection.DataConnection() );
		Query.SetQuery( szQuery );
		
		ULONG TotalCount = 0;

		if( Query.Execute( true ) )
		{	
#ifdef TEST
			int	iCount = 0;
#endif

			if(S_OK == Query.MoveFirst())
			{
				vector< BYTE > vecBankLarge( SIZE_OF_BANK_LARGE, 0 );
				vector< BYTE > vecBankSmall( SIZE_OF_BANK_SMALL, 0 );

				do
				{
					if(0 ==  (rand()%4))
					{
						Sleep(1);
					}

					++TotalCount;

					ULONG index = 0;
					string strID = "";
					ZeroMemory( &vecBankLarge[0], SIZE_OF_BANK_LARGE );
					ZeroMemory( &vecBankSmall[0], SIZE_OF_BANK_SMALL );

					Query.GetResult( ++index, strID );

					Query.GetResult( ++index, vecBankLarge );
					Query.GetResult( ++index, vecBankSmall );

					AddList( MAX_BANK_LARGE, vecBankLarge, rVectorItem );
					AddList( MAX_BANK_SMALL, vecBankSmall, rVectorItem );
					
					//m_prCompare.SetPos( m_prCompare.GetPos() + 1 );
				}while ( S_OK == Query.MoveNext() 
#ifdef TEST
					&& ++iCount < 10000
#endif
					);
			}
		}
#ifndef TEST
/*		if( TotalCount != dwBankinfoCount)
		{
			assert(NULL && "Bank Info Count fault");
		}*/
#endif

		return TRUE;
	}


END:
	return FALSE;
}

void	CLogCompareDlg::AddList( 
								UINT uMaxIndex, 
								vector< BYTE >& rVector, 
								vector< CItemEx >& rVectorItem
								)
{
	BYTE*		pByte = &rVector[0];
	CItemEx*	pItem;
	WORD		wItemID;

	for( UINT uIter = 0 ; uIter < uMaxIndex ; ++uIter )
	{
		pItem = reinterpret_cast< CItemEx* >(pByte);
		wItemID = pItem->m_wItemID;

		if( wItemID > 0 
			&& true == binary_search( m_vecMallItem.begin(), m_vecMallItem.end(), wItemID ) 
			)
		{
			rVectorItem.push_back( CItemEx( *pItem ) );
		}
		pByte += sizeof(CItemEx);	
	}
}

void	CLogCompareDlg::AddList( 
								UINT uMaxIndex, 
								vector< BYTE >& rVector, 
								vector< SItemAndID >& rVectorItem,
								LPCSTR szID
								)
{
	BYTE*		pByte = &rVector[0];
	CItemEx*	pItem;
	WORD		wItemID;

	for( UINT uIter = 0 ; uIter < uMaxIndex ; ++uIter )
	{
		pItem = reinterpret_cast< CItemEx* >(pByte);
		wItemID = pItem->m_wItemID;

		if( wItemID > 0 
			&& true == binary_search( m_vecMallItem.begin(), m_vecMallItem.end(), wItemID ) 
			)
		{
			rVectorItem.push_back( SItemAndID( *pItem, szID ) );
		}
		pByte += sizeof(CItemEx);	
	}
}

void	CLogCompareDlg::AddList( 
								UINT uMaxIndex, 
								vector< BYTE >& rVector, 
								vector< SItemAndIndex >& rVectorItem,
								DWORD dwIndex
								)
{
	BYTE*		pByte = &rVector[0];
	CItemEx*	pItem;
	WORD		wItemID;

	for( UINT uIter = 0 ; uIter < uMaxIndex ; ++uIter )
	{
		pItem = reinterpret_cast< CItemEx* >(pByte);
		wItemID = pItem->m_wItemID;

		if( wItemID > 0 
			&& true == binary_search( m_vecMallItem.begin(), m_vecMallItem.end(), wItemID ) 
			)
		{
			rVectorItem.push_back( SItemAndIndex( *pItem, dwIndex ) );
		}
		pByte += sizeof(CItemEx);	
	}
}

void CLogCompareDlg::UpdateListCtrl()
{
	m_listLog.DeleteAllItems();

	LVITEM item;
	char	temp[0xff];

	if( m_vecItemAndIndex.empty() == false || m_vecItemAndID.empty() == false )
	{
		for( UINT i = 0; i < m_vecItemAndIndex.size() ; i++ )
		{		
			CItemEx&	rItem = m_vecItemAndIndex[i].m_Item;
			// id 
			item.mask		= LVIF_TEXT | LVIF_PARAM;		
			item.iItem		= i;			//행번
			item.iSubItem	= 0;			//열번호 
			sprintf( temp, "%u", rItem.m_Serial.dwSerialNum );
			m_listLog.InsertItem(&item);

			item.mask		= LVIF_TEXT;
			item.iSubItem	= 1;
			sprintf( temp, "%u", rItem.m_wItemID );		
			item.pszText	= temp;
			m_listLog.SetItem(&item);

			item.mask		= LVIF_TEXT;
			item.iSubItem	= 2;
			sprintf( temp, "%u", rItem.m_Serial.bServerSerialID );
			item.pszText	= temp;
			m_listLog.SetItem(&item);

			item.mask		= LVIF_TEXT;
			item.iSubItem	= 3;
			sprintf( temp, "%u", rItem.m_Serial.bServerSet );
			item.pszText	= temp;
			m_listLog.SetItem(&item);

			item.mask		= LVIF_TEXT;
			item.iSubItem	= 4;
			sprintf( temp, "%u", rItem.m_Serial.wRestartCount );
			item.pszText	= temp;
			m_listLog.SetItem(&item);

			if( rItem.GetKind() == __ITEM_GUARDIAN__ )
			{
				item.mask		= LVIF_TEXT;
				item.iSubItem	= 5;
				sprintf( temp, "%u", rItem.m_Item_Guardian.wID );
				item.pszText	= temp;
				m_listLog.SetItem(&item);

				item.mask		= LVIF_TEXT;
				item.iSubItem	= 6;
				sprintf( temp, "%u", rItem.m_Item_Guardian.dwEndLife );
				item.pszText	= temp;
				m_listLog.SetItem(&item);
			}	

			item.mask		= LVIF_TEXT;
			item.iSubItem	= 7;
			sprintf( temp, "%u", m_vecItemAndIndex[i].m_dwIndex );
			item.pszText	= temp;
			m_listLog.SetItem(&item);
		}

		for( UINT i = 0; i < m_vecItemAndID.size() ; i++ )
		{		
			CItemEx&	rItem = m_vecItemAndID[i].m_Item;
			// id 
			item.mask		= LVIF_TEXT | LVIF_PARAM;		
			item.iItem		= i+m_vecItemAndIndex.size();			//행번
			item.iSubItem	= 0;			//열번호 
			sprintf( temp, "%u", rItem.m_Serial.dwSerialNum );
			m_listLog.InsertItem(&item);

			item.mask		= LVIF_TEXT;
			item.iSubItem	= 1;
			sprintf( temp, "%u", rItem.m_wItemID );		
			item.pszText	= temp;
			m_listLog.SetItem(&item);

			item.mask		= LVIF_TEXT;
			item.iSubItem	= 2;
			sprintf( temp, "%u", rItem.m_Serial.bServerSerialID );
			item.pszText	= temp;
			m_listLog.SetItem(&item);

			item.mask		= LVIF_TEXT;
			item.iSubItem	= 3;
			sprintf( temp, "%u", rItem.m_Serial.bServerSet );
			item.pszText	= temp;
			m_listLog.SetItem(&item);

			item.mask		= LVIF_TEXT;
			item.iSubItem	= 4;
			sprintf( temp, "%u", rItem.m_Serial.wRestartCount );
			item.pszText	= temp;
			m_listLog.SetItem(&item);

			if( rItem.GetKind() == __ITEM_GUARDIAN__ )
			{
				item.mask		= LVIF_TEXT;
				item.iSubItem	= 5;
				sprintf( temp, "%u", rItem.m_Item_Guardian.wID );
				item.pszText	= temp;
				m_listLog.SetItem(&item);

				item.mask		= LVIF_TEXT;
				item.iSubItem	= 6;
				sprintf( temp, "%u", rItem.m_Item_Guardian.dwEndLife );
				item.pszText	= temp;
				m_listLog.SetItem(&item);
			}		

			item.mask		= LVIF_TEXT;
			item.iSubItem	= 7;
			item.pszText	= m_vecItemAndID[i].m_szName;
			m_listLog.SetItem(&item);
		}
	}
	else
	{
		for( UINT i = 0; i < m_vecItem.size() ; i++ )
		{		
			CItemEx&	rItem = m_vecItem[i];
			// id 
			item.mask		= LVIF_TEXT | LVIF_PARAM;		
			item.iItem		= i;			//행번
			item.iSubItem	= 0;			//열번호 
			sprintf( temp, "%u", rItem.m_Serial.dwSerialNum );
			item.pszText	= temp;
			m_listLog.InsertItem(&item);

			item.mask		= LVIF_TEXT;
			item.iSubItem	= 1;
			sprintf( temp, "%u", rItem.m_wItemID );		
			item.pszText	= temp;
			m_listLog.SetItem(&item);

			item.mask		= LVIF_TEXT;
			item.iSubItem	= 2;
			sprintf( temp, "%u", rItem.m_Serial.bServerSerialID );
			item.pszText	= temp;
			m_listLog.SetItem(&item);

			item.mask		= LVIF_TEXT;
			item.iSubItem	= 3;
			sprintf( temp, "%u", rItem.m_Serial.bServerSet );
			item.pszText	= temp;
			m_listLog.SetItem(&item);

			item.mask		= LVIF_TEXT;
			item.iSubItem	= 4;
			sprintf( temp, "%u", rItem.m_Serial.wRestartCount );
			item.pszText	= temp;
			m_listLog.SetItem(&item);

			if( rItem.GetKind() == __ITEM_GUARDIAN__ )
			{
				item.mask		= LVIF_TEXT;
				item.iSubItem	= 5;
				sprintf( temp, "%u", rItem.m_Item_Guardian.wID );
				item.pszText	= temp;
				m_listLog.SetItem(&item);

				item.mask		= LVIF_TEXT;
				item.iSubItem	= 6;
				sprintf( temp, "%u", rItem.m_Item_Guardian.dwEndLife );
				item.pszText	= temp;
				m_listLog.SetItem(&item);
			}		
		}
	}	
}
void CLogCompareDlg::OnAppExit()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	DestroyWindow();	
}

void CLogCompareDlg::On57603()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if( m_vecItem.empty() == false )
	{
		TCHAR	strBuffer[0xff];
		CLogCompareApp*	pApp = reinterpret_cast< CLogCompareApp* >(AfxGetApp());

		LPCTSTR lpDBName = pApp->GetDBConnectionInfo( m_cbServer.GetCurSel() ).GetDBName().c_str();

		sprintf( strBuffer, "%s_%04d%02d%02d_%04d%02d%02d.txt", 
			lpDBName,
			srcTime.wYear, srcTime.wMonth, srcTime.wDay,
			dstTime.wYear, dstTime.wMonth, dstTime.wDay
			);

		CDebugLog Dbg;
		Dbg.Init(OUTPUT_FILE_AND_TRACE ,"log", strBuffer);

		TCHAR	strBuffer2[0xff];

		for( UINT i = 0 ; i < m_vecItemAndIndex.size() ; ++i )
		{
			_i64toa( m_vecItemAndIndex[i].m_Item.m_Serial.i64Serial, strBuffer2, 10 );

			Dbg.Log( LOG_LV3, "CharIndex: %u ItemID: %u ItemSerial: %s", 
				m_vecItemAndIndex[i].m_dwIndex, m_vecItemAndIndex[i].m_Item.m_wItemID, strBuffer2 );
		}

		for( i = 0 ; i < m_vecItemAndID.size() ; ++i )
		{
			_i64toa( m_vecItemAndID[i].m_Item.m_Serial.i64Serial, strBuffer2, 10 );

			Dbg.Log( LOG_LV3, "ID: %s ItemID: %u ItemSerial: %s", 
				m_vecItemAndID[i].m_szName, m_vecItemAndID[i].m_Item.m_wItemID, strBuffer2 );
		}

		sprintf( strBuffer, "%s_%04d%02d%02d_%04d%02d%02d.lcb", 
			lpDBName,
			srcTime.wYear, srcTime.wMonth, srcTime.wDay,
			dstTime.wYear, dstTime.wMonth, dstTime.wDay
			);

		DWORD dwWritten;
		HANDLE	hFile = ::CreateFile(
			strBuffer, 
			GENERIC_WRITE, 
			FILE_SHARE_WRITE,
			NULL,
			CREATE_NEW,
			FILE_ATTRIBUTE_NORMAL,
			NULL
			);

		if( INVALID_HANDLE_VALUE == hFile )
		{
			ASSERT( !"Error" );
		}

		DWORD dwSize = m_vecItem.size();

		if( FALSE == ::WriteFile( hFile, static_cast< void* >(&dwSize), 4, &dwWritten, NULL ) )
		{
			ASSERT( !"Error" );
		}
		if( FALSE == ::WriteFile( hFile, static_cast< void* >(&m_vecItem[0]), dwSize * sizeof(CItemEx), &dwWritten, NULL ) )
		{
			ASSERT( !"Error" );
		}
		
		::CloseHandle( hFile );

		sprintf( strBuffer, "%s_%04d%02d%02d_%04d%02d%02d.txt 파일에 로그를 저장했습니다.", 
			lpDBName,
			srcTime.wYear, srcTime.wMonth, srcTime.wDay,
			dstTime.wYear, dstTime.wMonth, dstTime.wDay
			);

		AfxMessageBox( strBuffer, MB_OK );
	}
	else
	{
		AfxMessageBox( "저장할 로그가 없습니다.", MB_OK );
	}
}

void CLogCompareDlg::OnLvnKeydownListLogResult(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	switch( pLVKeyDow->wVKey )
	{
	case VK_DELETE:
		// 선택된것 삭제
//		m_listLog.DeleteColumn(  );
		break;
	}
}

void CLogCompareDlg::OnBnClickedButtonCompareMain()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	AfxMessageBox( "CharName을 추가합니다.", MB_OK );

	TCHAR		strBuffer[0xff];

	INT	iCurSel = m_cbMainServer.GetCurSel();

	if( iCurSel < 0 )
		return;

	CLogCompareApp*	pApp = reinterpret_cast< CLogCompareApp* >(AfxGetApp());
	CDBConnectionInfo&	rInfo = pApp->GetMainDBConnectionInfo( iCurSel );

	m_vecMainDBCon.reserve( 1 );
	m_vecMainDBCon.assign( 1, CDBConnection() );
	
	wstring	wstrArray[CDBConnectionInfo::CDBDM_MAX];

	rInfo.HS_MBTOWS( CDBConnectionInfo::CDBDM_NAME, wstrArray[CDBConnectionInfo::CDBDM_NAME] );
	rInfo.HS_MBTOWS( CDBConnectionInfo::CDBDM_ADDRESS, wstrArray[CDBConnectionInfo::CDBDM_ADDRESS] );
	rInfo.HS_MBTOWS( CDBConnectionInfo::CDBDM_ACCOUNT, wstrArray[CDBConnectionInfo::CDBDM_ACCOUNT] );
	rInfo.HS_MBTOWS( CDBConnectionInfo::CDBDM_PASSWORD, wstrArray[CDBConnectionInfo::CDBDM_PASSWORD] );

	if( false == m_vecMainDBCon[0].Init( 
		wstrArray[CDBConnectionInfo::CDBDM_ADDRESS],
		wstrArray[CDBConnectionInfo::CDBDM_NAME],
		wstrArray[CDBConnectionInfo::CDBDM_ACCOUNT],
		wstrArray[CDBConnectionInfo::CDBDM_PASSWORD]
		) )
	{
		ASSERT( !"Error" );
	}

	m_vecItemAndIndex.clear();
	m_vecItemAndID.clear();

	vector< SItemAndIndex >		vecCharIndex;
	vecCharIndex.reserve(20000);
	
	BuildItemListAndCharIndex( vecCharIndex, m_vecMainDBCon[0] );
	sort( vecCharIndex.begin(), vecCharIndex.end() );

	vector< SItemAndIndex >::const_iterator	iter_begin = vecCharIndex.begin();
	vector< SItemAndIndex >::const_iterator	iter_end = vecCharIndex.end();

	while( iter_begin != iter_end )
	{
		if( m_vecItem.end() != find_if( m_vecItem.begin(), m_vecItem.end(), CItemCompare((*iter_begin).m_Item) ) )
		{
			m_vecItemAndIndex.push_back( (*iter_begin) );
		}

		++iter_begin;
	}

	vector< SItemAndID >	vecID;

	BuildItemListAndID( vecID, m_vecMainDBCon[0] );
	sort( vecID.begin(), vecID.end() );
	
	vector< SItemAndID >::const_iterator	iter_begin2 = vecID.begin();
	vector< SItemAndID >::const_iterator	iter_end2 = vecID.end();

	while( iter_begin2 != iter_end2 )
	{
		if( m_vecItem.end() != find_if( m_vecItem.begin(), m_vecItem.end(), CItemCompare((*iter_begin2).m_Item) ) )
		{
			m_vecItemAndID.push_back( (*iter_begin2) );
		}

		++iter_begin2;
	}

	UpdateListCtrl();
}

BOOL	CLogCompareDlg::BuildItemListAndCharIndex( vector< SItemAndIndex >& rVectorItem, CDBConnection& rConnection )
{
	DWORD	dwCharCount = 0;
	TCHAR	szQuery[0xff];
/*
	sprintf( szQuery, "Select Count(*) From CharacterInfo" );

	{
		CArgQuery Query( rConnection.DataConnection() );
		Query.SetQuery( szQuery );
		if( Query.Execute( true ) && S_OK == Query.MoveFirst() && Query.GetResult( 1, dwCharCount ) )
		{	
		}
		else
			goto END;
	}
*/
//	m_prCompare.SetRange( 0, dwCharCount );
//	m_prCompare.SetPos( 0 );

	sprintf( szQuery,"Select [Char_Index],[Equip],[Inv_large],[Inv_small],[Belt],[Guardian] From CharacterInfo" );

	{
		CArgQuery Query( rConnection.DataConnection() );

		Query.SetQuery(szQuery);
		ULONG TotalCount = 0;

		if( Query.Execute( true ) )
		{
#ifdef TEST
			int	iCount = 0;
#endif
			if( S_OK == Query.MoveFirst() )
			{
				vector< BYTE > vecEquip( SIZE_OF_EQUIP, 0 );
				vector< BYTE > vecInvLarge( SIZE_OF_LARGE, 0 );
				vector< BYTE > vecInvSmall( SIZE_OF_SMALL, 0 );
				vector< BYTE > vecBelt( SIZE_OF_BELT, 0 );
				vector< BYTE > vecGuardian( SIZE_OF_GUARDIAN, 0 );

				do
				{
					if(0 ==  (rand()%4))
					{
						Sleep(1);
					}

					++TotalCount;

					ULONG index = 0;
					DWORD dwCharIndex = 0;				

					ZeroMemory( &vecEquip[0], SIZE_OF_EQUIP );
					ZeroMemory( &vecInvLarge[0], SIZE_OF_LARGE );
					ZeroMemory( &vecInvSmall[0], SIZE_OF_SMALL );
					ZeroMemory( &vecBelt[0], SIZE_OF_BELT );
					ZeroMemory( &vecGuardian[0], SIZE_OF_GUARDIAN );

					Query.GetResult( ++index, dwCharIndex );

					Query.GetResult( ++index, vecEquip );
					Query.GetResult( ++index, vecInvLarge );
					Query.GetResult( ++index, vecInvSmall );
					Query.GetResult( ++index, vecBelt );
					Query.GetResult( ++index, vecGuardian );

					AddList( MAX_EQUIP, vecEquip, rVectorItem, dwCharIndex );
					AddList( MAX_INV_LARGE, vecInvLarge, rVectorItem, dwCharIndex );
					AddList( MAX_INV_SMALL, vecInvSmall, rVectorItem, dwCharIndex );
					AddList( MAX_BELT, vecBelt, rVectorItem, dwCharIndex );
					AddList( MAX_INV_GUARDIAN, vecGuardian, rVectorItem, dwCharIndex );

					m_prCompare.SetPos( m_prCompare.GetPos() + 1 );
				} while( S_OK == Query.MoveNext() 
#ifdef TEST
					&& ++iCount < 10000
#endif
				);
			}

		}	 // if( Query.Execute( szQuery ) )
#ifndef TEST
/*		if(dwCharCount != TotalCount)
		{
			assert( !"Char Info Count fault" );
		}*/
#endif
	}

END:
	return FALSE;
}

BOOL	CLogCompareDlg::BuildItemListAndID( vector< SItemAndID >& rVectorItem, CDBConnection& rConnection )
{
	TCHAR	szQuery[0xff];

	DWORD dwBankinfoCount = 0;
/*
	sprintf(szQuery,"Select Count(*) From BankInfo");

	{
		CArgQuery Query( rConnection.DataConnection() );
		Query.SetQuery( szQuery );

		if( Query.Execute( true ) && S_OK == Query.MoveFirst() && Query.GetResult( 1, dwBankinfoCount) )
		{	
		} // if( Query.Execute( szQuery ) )
		else 
			goto END;
	}
*/
	
	sprintf(szQuery,"Select [ID],[Bank_large],[Bank_small] From BankInfo" );

	{
		CArgQuery Query( rConnection.DataConnection() );
		Query.SetQuery( szQuery );

		ULONG TotalCount = 0;

		if( Query.Execute( true ) )
		{			
	
#ifdef TEST
					int	iCount = 0;
#endif
			if(S_OK == Query.MoveFirst())
			{
				vector< BYTE > vecBankLarge( SIZE_OF_BANK_LARGE, 0 );
				vector< BYTE > vecBankSmall( SIZE_OF_BANK_SMALL, 0 );

				do
				{
					if(0 ==  (rand()%4))
					{
						Sleep(1);
					}

					++TotalCount;

					ULONG index = 0;
					string strID = "";
					ZeroMemory( &vecBankLarge[0], SIZE_OF_BANK_LARGE );
					ZeroMemory( &vecBankSmall[0], SIZE_OF_BANK_SMALL );

					Query.GetResult( ++index, strID );

					Query.GetResult( ++index, vecBankLarge );
					Query.GetResult( ++index, vecBankSmall );

					AddList( MAX_BANK_LARGE, vecBankLarge, rVectorItem, strID.c_str() );
					AddList( MAX_BANK_SMALL, vecBankSmall, rVectorItem, strID.c_str() );

		//			m_prCompare.SetPos( m_prCompare.GetPos() + 1 );
				}while ( S_OK == Query.MoveNext() 
#ifdef TEST
					&& ++iCount < 10000
#endif
					);
			}
		}
#ifndef TEST
/*		if( TotalCount != dwBankinfoCount)
		{
			assert(NULL && "Bank Info Count fault");
		}*/
#endif

		return TRUE;
	}


END:
	return FALSE;
}

void CLogCompareDlg::OnFileOpen()
{
	CFileDialog	fd( TRUE );

	if( fd.DoModal() )
	{
		string str = fd.m_ofn.lpstrFileTitle;
		
			HANDLE hFile = ::CreateFile(
				fd.m_ofn.lpstrFile, 
				GENERIC_READ, 
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL
				);

			if( INVALID_HANDLE_VALUE == hFile )
			{
				ASSERT( !"Error" );
			}

			DWORD dwSize;
			m_vecItem.clear();
			DWORD	dwRead;

			if( FALSE == ::ReadFile( hFile, static_cast< void* >(&dwSize), 4, &dwRead, NULL ) )
			{
				ASSERT( !"Error" );
			}

			m_vecItem.reserve( dwSize );
			m_vecItem.resize( dwSize );

			if( FALSE == ::ReadFile( hFile, static_cast< void* >(&m_vecItem[0]), dwSize * sizeof(CItemEx), &dwRead, NULL ) )
			{
				ASSERT( !"Error" );
			}
			::CloseHandle( hFile );
		
			UpdateListCtrl();
	}
}