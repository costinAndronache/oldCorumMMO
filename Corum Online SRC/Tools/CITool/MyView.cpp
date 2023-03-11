#include "stdafx.h"
#include "NewSerial.h"
#include "MyView.h"
#include "ChildFrame.h"
#include "Doc.h"
#include "UIThread.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CMyView, CFormView)


BEGIN_MESSAGE_MAP(CMyView, CFormView)
	//{{AFX_MSG_MAP(CMyView)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON, OnGo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


LPCTSTR g_szWhere[] = {"EQUIP","INV_LARGE","INV_SMALL","BELT","GUARDIAN","BANK_LARGE","BANK_SMALL"};



//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
CMyView::CMyView() : CFormView(CMyView::IDD)
{
	//{{AFX_DATA_INIT(CMyView)
	//}}AFX_DATA_INIT
	m_pApp			= NULL;
	m_nViewID		= 0;
	m_wRestartCount	= 0;
	m_dwSerialNum	= 0;

	m_bRun			= FALSE;
	m_pWinThread	= NULL;

	m_hThread		= NULL;

	m_pDb			= NULL;

	memset(m_CORUM_DB_NAME,0,sizeof(m_CORUM_DB_NAME));		
	memset(m_CORUM_DB_ACCOUNT,0,sizeof(m_CORUM_DB_ACCOUNT));		
	memset(m_CORUM_DB_PASSWORD,0,sizeof(m_CORUM_DB_PASSWORD));	
	memset(m_CORUM_DB_ADDRESS,0,sizeof(m_CORUM_DB_ADDRESS));
	memset(m_CORUM_TDB_NAME,0,sizeof(m_CORUM_TDB_NAME));		
	memset(m_CORUM_TDB_ACCOUNT,0,sizeof(m_CORUM_TDB_ACCOUNT));		
	memset(m_CORUM_TDB_PASSWORD,0,sizeof(m_CORUM_TDB_PASSWORD));	
	memset(m_CORUM_TDB_ADDRESS,0,sizeof(m_CORUM_TDB_ADDRESS));
	memset(m_CORUM_GAME_DB,0,sizeof(m_CORUM_GAME_DB));
	m_CORUM_TOTAL_DB = 0;

	m_logFont.lfHeight			= -11;
	m_logFont.lfWidth			= 0;
	m_logFont.lfEscapement		= 0;
	m_logFont.lfOrientation		= 0;
	m_logFont.lfWeight			= FW_BOLD;
	m_logFont.lfItalic			= 0;
	m_logFont.lfUnderline		= 0;
	m_logFont.lfStrikeOut		= 0;
	m_logFont.lfCharSet			= DEFAULT_CHARSET;
	m_logFont.lfOutPrecision	= 0;
	m_logFont.lfClipPrecision	= 0;
	m_logFont.lfQuality			= PROOF_QUALITY;	//NONANTIALIASED_QUALITY;//PROOF_QUALITY;
	m_logFont.lfPitchAndFamily	= 0;
	lstrcpy(m_logFont.lfFaceName,"MS Sans Serif");
	m_Font.CreateFontIndirect(&m_logFont);
}


CMyView::~CMyView()
{

}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CMyView::OnDestroy() 
{
	CFormView::OnDestroy();	

	delete m_pEdit;
	delete m_pProgress;
	
	if(m_pDb)	
	{
		// ���ڰ� 0�̸� ��� Connection Release...
		m_pDb->ReleaseDBConnectionForSingleThread(0);	
		m_pDb->Release();
	}
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CMyView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyView)
	DDX_Control(pDX, IDC_CHECK_NEW_SERIAL, m_NewSerial);
	DDX_Control(pDX, IDC_COMBO, m_ComBoCtrl);
	//}}AFX_DATA_MAP
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CMyView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	m_pApp = (CApp*)AfxGetApp();

	// OleDbThread.dll COM �ʱ�ȭ
	if(!InitDatabase())
		return;
	
	InitDBAccount();
	
	if(!InitCorumDB())
		return;

	ResizeParentToFit(FALSE);
	ResizeParentToFit(TRUE);	
	GetParentFrame()->RecalcLayout();	
	ModifyStyleEx(WS_EX_CLIENTEDGE,0,SWP_FRAMECHANGED);
	ModifyStyle(WS_BORDER,0,SWP_FRAMECHANGED);

	CChildFrame* pFrame = (CChildFrame*)GetParent();
	m_pApp->m_dwDocCount++;	
	pFrame->m_nFrameID = m_pApp->m_dwDocCount;	//�����ӿ� �ڱ� Frame ID ����

	CDoc* pDoc = (CDoc*)GetDocument();
	pDoc->m_nDocID = m_pApp->m_dwDocCount;

	m_nViewID = m_pApp->m_dwDocCount;

	CString str;
	str.Format("[%02d] CorumOnline New Serial Tool",pFrame->m_nFrameID); 
	pFrame->SetWindowText(str);

	char path[MAX_PATH] = {0,};
	sprintf(path,"%s\\DB.ini",".");
	int TotalDBNum	= ::GetPrivateProfileInt("DB","TotalDBNum",0,path);

	// �޺�
	for( int i = 0 ; i < TotalDBNum; i++ ) 
		m_ComBoCtrl.InsertString(i,g_CORUM_DB_NAME[i]);
	m_ComBoCtrl.SetCurSel(0);

	m_pEdit = new CEdit;
	m_pEdit->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER,CRect(25,48,322,68),this,1);
	m_pEdit->SetReadOnly();
	m_pEdit->SetFocus();
	m_pEdit->SetFont(&m_Font);
	
	m_pProgress = new CProgressCtrlX;
	m_pProgress->Create(WS_CHILD|WS_VISIBLE|PBS_SMOOTH,CRect(25,76,322,96),this,2);
	m_pProgress->SetRange(0,100);
	m_pProgress->SetPos(0);
	m_pProgress->SetStep(1);
	m_pProgress->SetBkColor(RGB(228,228,228));
	m_pProgress->SetGradientColors(RGB(0,109,255),RGB(0,0,62));
	CRect rect(1,1,1,1);
	m_pProgress->SetBorders(&rect);
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
UINT ThreadFunc(LPVOID pParam)
{
	CMyView* pView		= (CMyView*)pParam;
	pView->InSertNewSerial();
	pView->m_bRun		= FALSE;
	pView->m_ComBoCtrl.EnableWindow(TRUE);
	pView->m_pWinThread	= NULL;
	return 0;
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CMyView::OnGo()
{
	if(!m_bRun)
	{
		m_pWinThread	= ::AfxBeginThread(ThreadFunc,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
		::DuplicateHandle(GetCurrentProcess(),m_pWinThread->m_hThread,GetCurrentProcess(),&m_hThread,0,FALSE,DUPLICATE_SAME_ACCESS);
		m_pWinThread->ResumeThread();

		m_bRun			= TRUE;
		m_ComBoCtrl.EnableWindow(FALSE);
	}
	else
	{
		AfxMessageBox("Thread Running....",MB_OK|MB_ICONINFORMATION);
	}
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
ITEM_SERIAL	CMyView::GetNewSerial()
{
	m_Serial.dwSerialNum++;
	return m_Serial;
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
BOOL CMyView::ResetItemSerialNumber()
{
	char	szQuery[0xff] = {0,};
	int		nRestartCount = -1;
	
	sprintf( szQuery,"Select * From CM_RESET_ITEM_SERIAL" );
	int ret = m_pDb->OpenRecord(szQuery,&nRestartCount,1,m_CORUM_TOTAL_DB);
	
	m_wRestartCount = (WORD)nRestartCount;
	nRestartCount	+= 1;
	
	int		nColumn			= 1;	// �Ķ���� ���� ���� 
	DWORD	dwParamOffset	= 0;	// �Ķ���� Binding ���� ����ü���� �� �Ķ���� Value���� Offset	
	int		nAffected		= 0;
	
	sprintf(szQuery,"Update CM_RESET_ITEM_SERIAL Set RestartCount=?");
	DBBINDING* pBinding = m_pDb->CreateParamInfo(nColumn,m_CORUM_TOTAL_DB);
	
	pBinding[0].obValue		= dwParamOffset;	
	pBinding[0].cbMaxLen	= sizeof(nRestartCount);
	pBinding[0].wType		= DBTYPE_I4;
	dwParamOffset			+= sizeof(nRestartCount);
	
	nAffected = m_pDb->ExecuteSQLByParam(szQuery,pBinding,&nRestartCount,nColumn,m_CORUM_TOTAL_DB);
	
	if(nAffected < 0)
	{
		return FALSE;
	}
	
	m_pDb->ReleaseParamInfo(pBinding,m_CORUM_TOTAL_DB);

	m_Serial.bServerSet			= (BYTE)253;
	m_Serial.bServerSerialID	= 0;
	m_Serial.wRestartCount		= (WORD)nRestartCount;
	m_Serial.dwSerialNum		= 0;
	
	return TRUE;
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CMyView::InSertNewSerial()
{
	char			szQuery[0xff]	= {0,};
	char			Name[20]		= {0,};
	char			ID[20]			= {0,};
	CString			ShowQuery;
	DWORD			nameCount		= 0;
	DWORD			bankCount		= 0;
	DWORD			haveCount		= 0;
	DWORD			copyCount		= 0;
	DBRECEIVEDATA*	pData			= NULL;
	DBRECEIVEDATA*	pData2			= NULL;
	__int64			Serial			= 0;

	CharacterItem	CharItem;
//	BANKINFO		BankInfo;

	int sel = m_ComBoCtrl.GetCurSel();

	if(m_pApp->m_ItemNoList.GetCount() <= 0)
	{
		AfxMessageBox("Can't Insert New Serial!!! Select Item Please!!!",MB_OK|MB_ICONSTOP);
		return;
	}

	if(!ResetItemSerialNumber())
	{
		AfxMessageBox("Can't Insert New Serial!!!",MB_OK|MB_ICONSTOP);
		return;
	}

	CTime start = CTime::GetCurrentTime();
	
	//ĳ���� �̸�
	ShowQuery.Format("All CharName Query");
	m_pEdit->SetWindowText(ShowQuery);
	sprintf(szQuery,"SELECT Count(*) FROM CharacterInfo");
	int ret = m_pDb->OpenRecord(szQuery,&nameCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);		
	sprintf(szQuery,"SELECT [Name] FROM CharacterInfo");		
	pData = m_pDb->OpenRecordEx(szQuery,nameCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);	

	/*char			szQuery[0xff]	= {0,};
	char			Name[20]		= {0,};
	char			ID[20]			= {0,};
	CString			ShowQuery;
	DWORD			nameCount		= 0;
	DWORD			bankCount		= 0;
	DBRECEIVEDATA*	pData			= NULL;
	BYTE			bServerSet		= 0;
	DWORD			dwParamOffset	= 0;
	SInvLarge		CharItem;
	BANKINFO		BankInfo;
	
	int sel = m_ComBoCtrl.GetCurSel();
	
	if(m_pApp->m_ItemNoList.GetCount() <= 0)
	{
		AfxMessageBox("Can't Insert New Serial!!! Select Item Please!!!",MB_OK|MB_ICONSTOP);
		return;
	}
	
	CTime start = CTime::GetCurrentTime();
	
	//ĳ���� �̸�
	ShowQuery.Format("All CharName Query");
	m_pEdit->SetWindowText(ShowQuery);
	
	sprintf(szQuery,"SELECT Count(*) FROM CharacterInfo");	
	int ret = g_pDb->OpenRecord(szQuery,&nameCount,DEFAULT_ROWS_PER_READ, m_CORUM_GAME_DB[sel]);	
	
	sprintf(szQuery,"SELECT [Name] FROM CharacterInfo");	
	pData = g_pDb->OpenRecordEx(szQuery,nameCount,DEFAULT_ROWS_PER_READ, m_CORUM_GAME_DB[sel]);*/	
	
	if( pData != NULL )
	{	
		for( DWORD i = 0; i < pData->Query.select.dwRowCount; i++ )	
		{
			if(!m_bRun)
			{				
				m_pDb->ReleaseRecordset(pData);				
				return;				
			}
			::Sleep(10);
			
			memset(Name,0,sizeof(Name));					
			memcpy(Name,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize);

			ShowQuery.Format("Q(CharacterInfo):<%d>[%d].[%s]",pData->Query.select.dwRowCount,i+1,Name);
			m_pEdit->SetWindowText(ShowQuery);
			
			sprintf(szQuery,"Select Equip,Inv_large,Inv_small,Belt,Guardian From CharacterInfo Where [Name]='%s'",Name);
			memset(&CharItem,0,sizeof(CharacterItem));
			ret = m_pDb->OpenRecord(szQuery,&CharItem,1,m_CORUM_GAME_DB[sel]);

			if( ret <= 0 )
				continue;
			
			// EQuip
			for( int equip = 0; equip < MAX_EQUIP; equip++ )
			{
				WORD id	= CharItem.Equip[equip].GetID();
				if(id == 0)
					continue;

				POSITION	pos	= m_pApp->m_ItemNoList.GetHeadPosition();
				WORD		wNo	= 0;
				for(int item = 0; item < m_pApp->m_ItemNoList.GetCount(); item++)
				{
					wNo = m_pApp->m_ItemNoList.GetNext(pos);
					if(wNo == id)
					{
						HAVEITEM haveitem;
						memset(&haveitem,0,sizeof(HAVEITEM));
						sprintf(haveitem.szCharName,Name);
						haveitem.i64Serial	= CharItem.Equip[equip].m_Serial.i64Serial;
						haveitem.bwhere		= WHERE_EQUIP;
						haveitem.bindex		= equip;
						InsertIntoHaveItemForDB(sel,haveitem);
						break;
					}
				}
			}			
			// Inv_Large
			for( int inv_large = 0; inv_large < MAX_INV_LARGE; inv_large++ )
			{
				WORD id	= CharItem.Inv_Large[inv_large].GetID();
				if(id == 0)
					continue;

				POSITION	pos	= m_pApp->m_ItemNoList.GetHeadPosition();
				WORD		wNo	= 0;
				for(int item = 0; item < m_pApp->m_ItemNoList.GetCount(); item++)
				{
					wNo = m_pApp->m_ItemNoList.GetNext(pos);
					if(wNo == id)
					{
						HAVEITEM haveitem;
						memset(&haveitem,0,sizeof(HAVEITEM));
						sprintf(haveitem.szCharName,Name);
						haveitem.i64Serial	= CharItem.Inv_Large[inv_large].m_Serial.i64Serial;
						haveitem.bwhere		= WHERE_INV_LARGE;
						haveitem.bindex		= inv_large;
						InsertIntoHaveItemForDB(sel,haveitem);
						break;
					}
				}
			}			
			// Inv_Small
			for( int inv_small = 0; inv_small < MAX_INV_SMALL; inv_small++ )
			{
				WORD id	= CharItem.Inv_Small[inv_small].GetID();
				if(id == 0)
					continue;

				POSITION	pos	= m_pApp->m_ItemNoList.GetHeadPosition();
				WORD		wNo	= 0;
				for(int item = 0; item < m_pApp->m_ItemNoList.GetCount(); item++)
				{
					wNo = m_pApp->m_ItemNoList.GetNext(pos);
					if(wNo == id)
					{
						HAVEITEM haveitem;
						memset(&haveitem,0,sizeof(HAVEITEM));
						sprintf(haveitem.szCharName,Name);
						haveitem.i64Serial	= CharItem.Inv_Small[inv_small].m_Serial.i64Serial;
						haveitem.bwhere		= WHERE_INV_SMALL;
						haveitem.bindex		= inv_small;
						InsertIntoHaveItemForDB(sel,haveitem);
						break;
					}
				}
			}			
			// Belt
			for( int belt = 0; belt < MAX_BELT; belt++ )
			{
				WORD id	= CharItem.Belt[belt].GetID();
				if(id == 0)
					continue;

				POSITION	pos	= m_pApp->m_ItemNoList.GetHeadPosition();
				WORD		wNo	= 0;
				for(int item = 0; item < m_pApp->m_ItemNoList.GetCount(); item++)
				{
					wNo = m_pApp->m_ItemNoList.GetNext(pos);
					if(wNo == id)
					{
						HAVEITEM haveitem;
						memset(&haveitem,0,sizeof(HAVEITEM));
						sprintf(haveitem.szCharName,Name);
						haveitem.i64Serial	= CharItem.Belt[belt].m_Serial.i64Serial;
						haveitem.bwhere		= WHERE_BELT;
						haveitem.bindex		= belt;
						InsertIntoHaveItemForDB(sel,haveitem);
						break;
					}
				}
			}			
			// Guardian
			for( int guardian = 0; guardian < MAX_GUARDIAN; guardian++ )
			{
				WORD id	= CharItem.Guardian[guardian].GetID();
				if(id == 0)
					continue;
				
				POSITION	pos	= m_pApp->m_ItemNoList.GetHeadPosition();
				WORD		wNo	= 0;
				for(int item = 0; item < m_pApp->m_ItemNoList.GetCount(); item++)
				{
					wNo = m_pApp->m_ItemNoList.GetNext(pos);
					if(wNo == id)
					{
						HAVEITEM haveitem;
						memset(&haveitem,0,sizeof(HAVEITEM));
						sprintf(haveitem.szCharName,Name);
						haveitem.i64Serial	= CharItem.Guardian[guardian].m_Serial.i64Serial;
						haveitem.bwhere		= WHERE_GUARDIAN;
						haveitem.bindex		= guardian;
						InsertIntoHaveItemForDB(sel,haveitem);
						break;
					}
				}
			}			
			StepProgress();
		}		
		m_pDb->ReleaseRecordset(pData);
		pData = NULL;
	}
	
	//â����̵� �̸�
	/*ShowQuery.Format("All BankID Query");
	m_pEdit->SetWindowText(ShowQuery);
	sprintf(szQuery,"SELECT Count(*) FROM BankInfo");	
	ret = m_pDb->OpenRecord(szQuery,&bankCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);
	sprintf(szQuery,"SELECT [ID] FROM BankInfo");	
	pData = m_pDb->OpenRecordEx(szQuery,bankCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);		

	if( pData != NULL )
	{
		for( DWORD i = 0; i < pData->Query.select.dwRowCount; i++ )	
		{
			if(!m_bRun)
			{				
				m_pDb->ReleaseRecordset(pData);				
				return;				
			}
			::Sleep(10);
			
			memset(	ID,0,sizeof(ID)	);				
			memcpy(	ID,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize);

			ShowQuery.Format("Q(Bank):<%d>[%d].[%s]",pData->Query.select.dwRowCount,i+1,ID);
			m_pEdit->SetWindowText(ShowQuery);
			
			sprintf(szQuery,"SELECT Bank_Large,Bank_Small,[Money] FROM BankInfo WHERE [ID]='%s'",ID);
			memset(&BankInfo,0,sizeof(BANKINFO));
			int ret = m_pDb->OpenRecord(szQuery,&BankInfo,1,m_CORUM_GAME_DB[sel]);
			
			if( ret <= 0 )
				continue;
			
			// Bank_Large
			for( int banklarge = 0; banklarge < MAX_BANK_LARGE; banklarge++ )
			{
				WORD id	= BankInfo.Bank_Large[banklarge].GetID();
				if(id == 0)
					continue;
				
				POSITION	pos	= m_pApp->m_ItemNoList.GetHeadPosition();
				WORD		wNo	= 0;
				for(int item = 0; item < m_pApp->m_ItemNoList.GetCount(); item++)
				{
					wNo = m_pApp->m_ItemNoList.GetNext(pos);
					if(wNo == id)
					{
						HAVEITEM haveitem;
						memset(&haveitem,0,sizeof(HAVEITEM));
						sprintf(haveitem.szCharName,ID);
						haveitem.i64Serial	= BankInfo.Bank_Large[banklarge].m_Serial.i64Serial;
						haveitem.bwhere		= WHERE_BANK_LARGE;
						haveitem.bindex		= banklarge;
						InsertIntoHaveItemForDB(sel,haveitem);
						break;
					}
				}
			}
			// Bank_Small
			for( int banksmall = 0; banksmall < MAX_BANK_SMALL; banksmall++ )
			{
				WORD id	= BankInfo.Bank_Small[banksmall].GetID();
				if(id == 0)
					continue;
				
				POSITION	pos	= m_pApp->m_ItemNoList.GetHeadPosition();
				WORD		wNo	= 0;
				for(int item = 0; item < m_pApp->m_ItemNoList.GetCount(); item++)
				{
					wNo = m_pApp->m_ItemNoList.GetNext(pos);
					if(wNo == id)
					{
						HAVEITEM haveitem;
						memset(&haveitem,0,sizeof(HAVEITEM));
						sprintf(haveitem.szCharName,ID);
						haveitem.i64Serial	= BankInfo.Bank_Small[banksmall].m_Serial.i64Serial;
						haveitem.bwhere		= WHERE_BANK_SMALL;
						haveitem.bindex		= banksmall;
						InsertIntoHaveItemForDB(sel,haveitem);
						break;
					}
				}
			}
			StepProgress();
		}		
		m_pDb->ReleaseRecordset(pData);		
	}*/

	//���� ���� �˻�
	/*ShowQuery.Format("Find Copy Item...........");
	m_pEdit->SetWindowText(ShowQuery);
	m_pProgress->SetPos(0);

	haveCount	= 0;
	copyCount	= 0;
	pData		= NULL;
	pData2		= NULL;
	HAVEITEM copyitem;
	
	sprintf(szQuery,"Select Count(*) From CM_COPYITEM_%d",sel+1);	
	ret = m_pDb->OpenRecord(szQuery,&haveCount,DEFAULT_ROWS_PER_READ,m_CORUM_TOTAL_DB);	
	sprintf(szQuery,"Select Serial From CM_COPYITEM_%d GROUP BY Serial",sel+1);	
	pData = m_pDb->OpenRecordEx(szQuery,haveCount,DEFAULT_ROWS_PER_READ,m_CORUM_TOTAL_DB);	

	if(pData != NULL)
	{
		for( DWORD have = 0; have < pData->Query.select.dwRowCount; have++ )
		{
			if(!m_bRun) return;
			::Sleep(10);
			
			ShowQuery.Format("New Serial:<%d>[%d]",pData->Query.select.dwRowCount,have+1);
			m_pEdit->SetWindowText(ShowQuery);

			memcpy(&Serial,pData->Query.select.pResult + (have * pData->Query.select.dwRowSize),pData->Query.select.dwRowSize);

			sprintf(szQuery,"Select Count(*) From CM_COPYITEM_%d Where Serial = %I64d",sel+1,Serial);			
			ret = m_pDb->OpenRecord(szQuery,&copyCount,DEFAULT_ROWS_PER_READ,m_CORUM_TOTAL_DB);			
			
			// �� �ø��� �ֱ� 
			if(copyCount > 1)
			{
				if(m_NewSerial.GetCheck())
				{
					pData2 = NULL;
					sprintf(szQuery,"Select * From CM_COPYITEM_%d Where Serial = %I64d",sel+1,Serial);					
					pData2 = m_pDb->OpenRecordEx(szQuery,copyCount,DEFAULT_ROWS_PER_READ,m_CORUM_TOTAL_DB);					

					if(pData2 != NULL)
					{
						for( DWORD copy = 0; copy < pData2->Query.select.dwRowCount; copy++ )
						{
							if(!m_bRun) return;
							::Sleep(10);							
							
							memset(&copyitem,0,sizeof(HAVEITEM));
							memcpy(&copyitem,pData2->Query.select.pResult + (copy * pData2->Query.select.dwRowSize),pData2->Query.select.dwRowSize);

							if( (copyitem.bwhere == WHERE_EQUIP) || (copyitem.bwhere == WHERE_INV_LARGE) || (copyitem.bwhere == WHERE_INV_SMALL) || (copyitem.bwhere == WHERE_BELT) || (copyitem.bwhere == WHERE_GUARDIAN) )
							{
								memset(&CharItem,0,sizeof(CharacterItem));
								sprintf(szQuery,"Select Equip,Inv_large,Inv_small,Belt,Guardian From CharacterInfo Where [Name]='%s'",copyitem.szCharName);
								ret = m_pDb->OpenRecord(szQuery,&CharItem,1,m_CORUM_GAME_DB[sel]);
								if( ret <= 0 )
									continue;

								if(copyitem.bwhere == WHERE_EQUIP)
									CharItem.Equip[copyitem.bindex].m_Serial		= GetNewSerial();
								else if(copyitem.bwhere == WHERE_INV_LARGE)
									CharItem.Inv_Large[copyitem.bindex].m_Serial	= GetNewSerial();
								else if(copyitem.bwhere == WHERE_INV_SMALL)
									CharItem.Inv_Small[copyitem.bindex].m_Serial	= GetNewSerial();
								else if(copyitem.bwhere == WHERE_BELT)
									CharItem.Belt[copyitem.bindex].m_Serial			= GetNewSerial();
								else if(copyitem.bwhere == WHERE_GUARDIAN)
									CharItem.Guardian[copyitem.bindex].m_Serial		= GetNewSerial();				
								
								// ������Ʈ 
								int		nColumn			= 5;	
								DWORD	dwParamOffset	= 0;	
								int		nAffected		= 0;
								
								sprintf(szQuery,"Update CharacterInfo Set [Equip]=?,[Inv_large]=?,[Inv_small]=?,[Belt]=?,"
												"[Guardian]=? Where [Name]='%s'",copyitem.szCharName);
								
								DBBINDING* pBinding = m_pDb->CreateParamInfo(nColumn);
								if(!pBinding)
									continue;				
								
								pBinding[0].obValue		= dwParamOffset;	
								pBinding[0].cbMaxLen	= sizeof(CharItem.Equip);
								pBinding[0].wType		= DBTYPE_BYTES;
								dwParamOffset			+= sizeof(CharItem.Equip);				
								
								pBinding[1].obValue		= dwParamOffset;	
								pBinding[1].cbMaxLen	= sizeof(CharItem.Inv_Large);
								pBinding[1].wType		= DBTYPE_BYTES;
								dwParamOffset			+= sizeof(CharItem.Inv_Large);		
								
								pBinding[2].obValue		= dwParamOffset;	
								pBinding[2].cbMaxLen	= sizeof(CharItem.Inv_Small);
								pBinding[2].wType		= DBTYPE_BYTES;
								dwParamOffset			+= sizeof(CharItem.Inv_Small);			
								
								pBinding[3].obValue		= dwParamOffset;	
								pBinding[3].cbMaxLen	= sizeof(CharItem.Belt);
								pBinding[3].wType		= DBTYPE_BYTES;
								dwParamOffset			+= sizeof(CharItem.Belt);
								
								pBinding[4].obValue		= dwParamOffset;	
								pBinding[4].cbMaxLen	= sizeof(CharItem.Guardian);
								pBinding[4].wType		= DBTYPE_BYTES;
								dwParamOffset			+= sizeof(CharItem.Guardian);
								
								int affected = m_pDb->ExecuteSQLByParam(szQuery,pBinding,&CharItem,nColumn,m_CORUM_GAME_DB[sel]);	
								if(affected <= 0)
								{
									m_pDb->ReleaseParamInfo(pBinding);
									continue;	
								}				
								m_pDb->ReleaseParamInfo(pBinding);
							}
							else if( (copyitem.bwhere == WHERE_BANK_LARGE) || (copyitem.bwhere == WHERE_BANK_SMALL) )
							{
								memset(&BankInfo,0,sizeof(BANKINFO));
								sprintf(szQuery,"SELECT Bank_Large,Bank_Small,[Money] FROM BankInfo WHERE [ID]='%s'",copyitem.szCharName);
								int ret = m_pDb->OpenRecord(szQuery,&BankInfo,1,m_CORUM_GAME_DB[sel]);			
								if( ret <= 0 )
									continue;

								if(copyitem.bwhere == WHERE_BANK_LARGE)
									BankInfo.Bank_Large[copyitem.bindex].m_Serial = GetNewSerial();
								else if(copyitem.bwhere == WHERE_BANK_SMALL)				
									BankInfo.Bank_Small[copyitem.bindex].m_Serial = GetNewSerial();

								// ������Ʈ 
								int		nColumn			= 3;	
								DWORD	dwParamOffset	= 0;	
								
								sprintf(szQuery,"Update Bankinfo Set Bank_Large=?,Bank_Small=?,[Money]=? Where [ID]='%s'",copyitem.szCharName);				
								DBBINDING* pBinding = m_pDb->CreateParamInfo(nColumn);
								
								if(!pBinding)
									continue;
												
								pBinding[0].obValue		= dwParamOffset;	
								pBinding[0].cbMaxLen	= sizeof(BankInfo.Bank_Large);
								pBinding[0].wType		= DBTYPE_BYTES;
								dwParamOffset			+= sizeof(BankInfo.Bank_Large);		
								
								pBinding[1].obValue		= dwParamOffset;	
								pBinding[1].cbMaxLen	= sizeof(BankInfo.Bank_Small);
								pBinding[1].wType		= DBTYPE_BYTES;
								dwParamOffset			+= sizeof(BankInfo.Bank_Small);		
								
								pBinding[2].obValue		= dwParamOffset;	
								pBinding[2].cbMaxLen	= sizeof(BankInfo.dwMoney);	
								pBinding[2].wType		= DBTYPE_I4;
								dwParamOffset			+= sizeof(BankInfo.dwMoney);			
								
								int affected = m_pDb->ExecuteSQLByParam(szQuery,pBinding,&BankInfo,nColumn,m_CORUM_GAME_DB[sel]);	
								if( affected < 0 )
								{
									m_pDb->ReleaseParamInfo(pBinding);
									continue;
								}				
								m_pDb->ReleaseParamInfo(pBinding);
							}							
							StepProgress();
						}
					}
				}
			}
			// ����� �������� �ƴ϶�� 
			else
			{				
				sprintf(szQuery,"Delete From CM_COPYITEM_%d Where Serial = %I64d",sel+1,Serial);
				ret = m_pDb->ExecuteSQL(szQuery, m_CORUM_TOTAL_DB);				
			}
		}
	}
	m_pProgress->SetPos(0);	
	
	// ���� �������� ����Ʈ�� �̴´�.
	CString		folder;
	folder.Format("%s\\CopyItemList",g_szExePath);
	
	CFileFind	finder;
	BOOL		bOK		= FALSE;
	bOK					= finder.FindFile(folder);
		
	// ������ ������ 
	if(bOK)
	{
		while(bOK)
		{
			bOK = finder.FindNextFile();
			if(!finder.IsDirectory())	
			{
				// ������� �Ȱ��� ������ �ִµ�(Ȯ���� ����) ������ �ƴҰ��
				::CreateDirectory((LPCTSTR)folder,NULL);
				break;
			}
		}
		
	}
	// ������ ������ 
	else
	{
		::CreateDirectory((LPCTSTR)folder,NULL);
	}
	
	{
		CString szServerName;
		m_ComBoCtrl.GetLBText(sel,szServerName);
		SYSTEMTIME tm;
		GetSystemTime(&tm);
		CString szFileName;
		szFileName.Format("%s\\[%s][%d.%d.%d %d.%d.%d].csv",folder,szServerName,tm.wYear,tm.wMonth,tm.wDay,tm.wHour,tm.wMinute,tm.wSecond);

		ShowQuery.Format("Make .csv File.............");
		m_pEdit->SetWindowText(ShowQuery);

		{
			HANDLE		hFile		= NULL;
			BOOL		result		= FALSE;
			DWORD		Written		= 0;
			CString		szWrite;
			int			FuckCount	= 0;
			HAVEITEM	FuckItem;
			pData					= NULL;

			hFile = ::CreateFile(szFileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

			sprintf(szQuery,"Select Count(*) From CM_COPYITEM_%d",sel+1);			
			ret = m_pDb->OpenRecord(szQuery,&FuckCount,DEFAULT_ROWS_PER_READ,m_CORUM_TOTAL_DB);
			sprintf(szQuery,"Select * From CM_COPYITEM_%d Order By Serial",sel+1);			
			pData = m_pDb->OpenRecordEx(szQuery,FuckCount,DEFAULT_ROWS_PER_READ,m_CORUM_TOTAL_DB);			

			if(pData != NULL)
			{
				for( DWORD fuck = 0; fuck < pData->Query.select.dwRowCount; fuck++ )
				{
					if(!m_bRun) return;
					::Sleep(10);
					ShowQuery.Format("Make .csv File:<%d>[%d]",pData->Query.select.dwRowCount,fuck+1);
					m_pEdit->SetWindowText(ShowQuery);
					memset(&FuckItem,0,sizeof(HAVEITEM));
					memcpy(&FuckItem,pData->Query.select.pResult + (fuck * pData->Query.select.dwRowSize),pData->Query.select.dwRowSize);
					szWrite.Format("%I64d,%s,%s,%d\r\n",FuckItem.i64Serial,FuckItem.szCharName,g_szWhere[FuckItem.bwhere],FuckItem.bindex);
					result = WriteFile(hFile,szWrite.GetBuffer(szWrite.GetLength()),szWrite.GetLength(),&Written,NULL);
				}
			}

			::CloseHandle(hFile);
			sprintf(szQuery,"Delete From CM_COPYITEM_%d",sel+1,Serial);			
			ret = m_pDb->ExecuteSQL(szQuery, m_CORUM_TOTAL_DB);			
		}
	}*/
		
	// �� ������.
	CTimeSpan result; 
	CTime end	= CTime::GetCurrentTime();
	result		= end - start;
	LONG hour	= result.GetTotalHours(); 
	LONG min	= result.GetTotalMinutes();
	LONG sec	= result.GetTotalSeconds();
	ShowQuery.Format("[%d]Hours [%d]Min [%d]Sec Passed.....",hour,min,sec);
	m_pEdit->SetWindowText(ShowQuery);
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CMyView::InsertIntoHaveItemForDB(int sel,HAVEITEM haveitem)
{
	int					nColumn			= 4;	
	int					col				= 0;
	DWORD				dwParamOffset	= 0;
	char				szQuery[0xff]   = {0,};
	DBBINDING*			pBinding		= m_pDb->CreateParamInfo(nColumn,m_CORUM_TOTAL_DB);

	// Serial
	pBinding[col].obValue		= dwParamOffset;
	pBinding[col].cbMaxLen		= sizeof(haveitem.i64Serial);
	pBinding[col++].wType		= DBTYPE_I8;		
	dwParamOffset				+= sizeof(haveitem.i64Serial);
	
	// CharName
	pBinding[col].obValue		= dwParamOffset;
	pBinding[col].cbMaxLen		= sizeof(haveitem.szCharName);
	pBinding[col++].wType		= DBTYPE_STR;		
	dwParamOffset				+= sizeof(haveitem.szCharName);
	
	// bWhere
	pBinding[col].obValue		= dwParamOffset;
	pBinding[col].cbMaxLen		= sizeof(haveitem.bwhere);
	pBinding[col++].wType		= DBTYPE_UI1;		
	dwParamOffset				+= sizeof(haveitem.bwhere);
	
	// bIndex
	pBinding[col].obValue		= dwParamOffset;
	pBinding[col].cbMaxLen		= sizeof(haveitem.bindex);
	pBinding[col++].wType		= DBTYPE_UI1;		
	dwParamOffset				+= sizeof(haveitem.bindex);

	sprintf(szQuery,"Insert into CM_COPYITEM_%d (Serial,CharName,bWhere,bIndex) values (?,?,?,?)",sel+1);
	int affected = m_pDb->ExecuteSQLByParam(szQuery,pBinding,&haveitem.i64Serial,nColumn,m_CORUM_TOTAL_DB);

	m_pDb->ReleaseParamInfo(pBinding,m_CORUM_TOTAL_DB);		
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CMyView::StepProgress()
{
	int	nLower, nUpper;
	int	nCurPos = m_pProgress->GetPos();		
	m_pProgress->GetRange(nLower, nUpper);
	
	if( nCurPos >= nUpper - nLower )
		m_pProgress->SetPos(0);
	else
		m_pProgress->StepIt();
}


#ifdef _DEBUG
void CMyView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMyView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif 


//------------------------------------------------------------------------------
//	DESCRIPTION	:	- OleDbThread.dll COM �ʱ�ȭ 
//------------------------------------------------------------------------------
BOOL CMyView::InitDatabase()
{
	//	�ش� ���μ����� COM ����ý����� ����Ҽ� �ֵ��� �غ�
	//	���������� COM ����ý����� ����ϴµ� �߿��� ������ ���� �������� �ڷᱸ���� �Ҵ� 
	//	Ŭ���̾�Ʈ ���α׷��� ���μ��� ��ȣ�� �˾Ƴ��� �� ��ȣ�� ����Ͽ� �� ���α׷��� �����ϴ� COM���� ������ ���� ��Ȳ�� �����Ѵ�.
	//	�ϳ��� ������ �Ű������� ������ ���⿡�� �׻� NULL�� �Ѱ��ش�.
	CoInitialize(NULL);				
	
	HRESULT hr = ::CoCreateInstance(	CLSID_OLEDBTHREAD,			/* Ŭ���� ���丮�� Ŭ���� ID */
										NULL,						/* �ܺΰ�ü�� IUnknown �������̽��� ������ */ 
										CLSCTX_INPROC_SERVER,		/* ��ü ������ �̷������ ���ϴ� COM ����Ÿ���� ��Ÿ���� CLSCTX ���� ��� */
										IID_OLEDBTHREAD,			/* �� ��ü�κ��� ��⸦ ���ϴ� �������̽� ID */
										(void**)&m_pDb			);	/* ��ȯ�Ǵ� �������̽� �����͸� �޾Ƴ� ��� �Ű������� ���Ǵ� �������� �ּ� */
	
	if(FAILED(hr))
	{
		AfxMessageBox("Cannot create IID_OLEDBTHREAD",MB_OK|MB_ICONSTOP);
		return FALSE;
	}
	
	DB_INITIALIZE_DESC desc;
	
	desc.bUsingThread					= FALSE;			// Thread ���� ��� ����, FALSE�� ��� (���ξ� TH~ �迭�� �޼ҵ�� ���Ұ�!)
	desc.bUsingEventObject				= FALSE;			// TRUE  : Event Object ������� ��������� ����	FALSE : Message ������� ���� ����� Callback �Լ�  
	desc.ReceiveFunc					= NULL;				// Event Object ����ϰ�� ������� ���� Callback �Լ� ������
	desc.hWndToPostMessage				= NULL;				// Message����� ��� Message�� ���� �������� �ڵ鰪 
	desc.uMessage						= NULL;				// Message����� ����� Message
	desc.wMaxNumOfProcessMessage_Input	= 100;				// DLL���� ���ÿ� �����Ҽ� �ִ� �ִ� Input �޼��� ����..  (Input �޼���ť �޸�Ǯ�� �ִ�뷮)
	desc.wMaxNumOfProcessMessage_Output = 100;				// DLL���� ���ÿ� �����Ҽ� �ִ� �ִ� Output �޼��� ����..  (Output �޼���ť �޸�Ǯ�� �ִ�뷮)
	desc.wMaxRowPerRead = 50;								// DB���� Data�� �о�帱�� �ѹ��� �о�帱�� �ִ� �ִ� ���ǰ��� 
	
	//��)	���� ���������� Return�Ǵ� �������� 200���̰� 
	//		OpenRecordEx�� dwRowPerRead ���ڰ��� 20���� �־������
	//		�ѹ��� 20�྿ 10���� �о 200���� �޾ƿ´�.
	//		�̶� dwRowPerRead���ڰ��� �������ִ� �ִ������ 50�̶�� �ǹ�!!
	
	desc.wMaxReturnedRowNum				= 50;				// ���������� ��ȯ�� �� �ִ� ���� �ִ� ����
	desc.wMaxReturnedColNum				= 128;				// ���������� ��ȯ�� �� �ִ� Į���� �ִ� ����
	desc.dwMaxRowSize					= 10240;			// ���������� ��ȯ�� �� �ִ� �� ���� �ִ� ������	
	desc.OutputMessageFunc				= DisplayDBMessage;	// DLL������ �޼����� �����޼����� ��¹��� �ܺ� �Լ� ������
	
	// DB��� �ʱ�ȭ 
	if(m_pDb->InitDBModule(&desc) == FALSE)
	{
		AfxMessageBox("Fail to initialize DB Module!",MB_OK|MB_ICONSTOP);
		return FALSE;
	}
	
	return TRUE;
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CMyView::InitDBAccount()
{
	char path[MAX_PATH] = {0,};
	sprintf(path,"%s\\DB.ini",".");
	
	char	keyname[ 0xff ]	= {0,};							
	int		TotalDBNum		= 0;
	
	TotalDBNum	= ::GetPrivateProfileInt("DB","TotalDBNum",0,path);
	
	for( int i = 0; i < TotalDBNum; i++ )
	{
		sprintf(keyname,"DB_INFO_0%d",i);
		::GetPrivateProfileString(keyname,"CORUM_DB_NAME",		"",m_CORUM_DB_NAME[i]		,sizeof(m_CORUM_DB_NAME[i])		,path);	
		::GetPrivateProfileString(keyname,"CORUM_DB_ACCOUNT",	"",m_CORUM_DB_ACCOUNT[i]	,sizeof(m_CORUM_DB_ACCOUNT[i])	,path);	
		::GetPrivateProfileString(keyname,"CORUM_DB_PASSWORD",	"",m_CORUM_DB_PASSWORD[i]	,sizeof(m_CORUM_DB_PASSWORD[i])	,path);
		::GetPrivateProfileString(keyname,"CORUM_DB_ADDRESS",	"",m_CORUM_DB_ADDRESS[i]	,sizeof(m_CORUM_DB_ADDRESS[i])	,path);
	}
	
	sprintf(keyname,"TOTALDB_INFO");
	::GetPrivateProfileString(keyname,"CORUM_DB_NAME",		"",m_CORUM_TDB_NAME			,sizeof(m_CORUM_TDB_NAME)		,path);	
	::GetPrivateProfileString(keyname,"CORUM_DB_ACCOUNT",	"",m_CORUM_TDB_ACCOUNT		,sizeof(m_CORUM_TDB_ACCOUNT)	,path);	
	::GetPrivateProfileString(keyname,"CORUM_DB_PASSWORD",	"",m_CORUM_TDB_PASSWORD		,sizeof(m_CORUM_TDB_PASSWORD)	,path);
	::GetPrivateProfileString(keyname,"CORUM_DB_ADDRESS",	"",m_CORUM_TDB_ADDRESS		,sizeof(m_CORUM_TDB_ADDRESS)	,path);
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
BOOL CMyView::InitCorumDB()
{
	char path[MAX_PATH] = {0,};
	sprintf(path,"%s\\DB.ini",".");

	int TotalDBNum = ::GetPrivateProfileInt("DB","TotalDBNum",0,path);

	for( int i = 0; i < TotalDBNum; i++ )
	{
		m_CORUM_GAME_DB[i] = m_pDb->CreateDBConnection();
		
		if(m_pDb->Connect(m_CORUM_DB_ADDRESS[i],m_CORUM_DB_NAME[i],m_CORUM_DB_ACCOUNT[i],m_CORUM_DB_PASSWORD[i],20,10,FALSE,m_CORUM_GAME_DB[i]) < 0)
		{
			m_pDb->ReleaseDBConnectionForSingleThread(m_CORUM_GAME_DB[i]);
			m_CORUM_GAME_DB[i] = 0;
			CString str;
			str.Format("Fail to Connect GAME_DB! (IP:%s, DBName:%s)", m_CORUM_DB_ADDRESS[i], m_CORUM_DB_NAME[i]);
			AfxMessageBox(str,MB_OK|MB_ICONSTOP);
			return FALSE;
		}		
	}

	m_CORUM_TOTAL_DB = m_pDb->CreateDBConnection();
	
	if(m_pDb->Connect(m_CORUM_TDB_ADDRESS,m_CORUM_TDB_NAME,m_CORUM_TDB_ACCOUNT,m_CORUM_TDB_PASSWORD,20,10,FALSE,m_CORUM_TOTAL_DB) < 0)
	{
		m_pDb->ReleaseDBConnectionForSingleThread(m_CORUM_TOTAL_DB);
		m_CORUM_TOTAL_DB = 0;
		CString str;
		str.Format("Fail to Connect TOTAL_DB! (IP:%s, DBName:%s)",m_CORUM_TDB_ADDRESS,m_CORUM_TDB_NAME);
		AfxMessageBox(str,MB_OK|MB_ICONSTOP);
		return FALSE;
	}

	return TRUE;
}





























