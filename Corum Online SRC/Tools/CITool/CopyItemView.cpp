#include "stdafx.h"
#include "NewSerial.h"
#include "CopyItemView.h"
#include "CopyItemFrame.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CCopyItemView, CFormView)


BEGIN_MESSAGE_MAP(CCopyItemView, CFormView)
	//{{AFX_MSG_MAP(CCopyItemView)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO, OnSelchangeCombo)
	ON_BN_CLICKED(IDC_BUTTON, OnGo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
CCopyItemView::CCopyItemView() : CFormView(CCopyItemView::IDD)
{
	//{{AFX_DATA_INIT(CCopyItemView)
	//}}AFX_DATA_INIT
	m_pApp			= NULL;
	m_nViewID		= 0;
	m_pEdit			= NULL;
	m_pProgress		= NULL;	
	m_bRun			= FALSE;
	m_pWinThread	= NULL;	
	m_hThread		= NULL;	
	m_pDb			= NULL;
	
	memset(m_CORUM_DB_NAME,0,sizeof(m_CORUM_DB_NAME));		
	memset(m_CORUM_DB_ACCOUNT,0,sizeof(m_CORUM_DB_ACCOUNT));		
	memset(m_CORUM_DB_PASSWORD,0,sizeof(m_CORUM_DB_PASSWORD));	
	memset(m_CORUM_DB_ADDRESS,0,sizeof(m_CORUM_DB_ADDRESS));
	m_CORUM_BACKUP_DB	= 0;
	m_nDbCount			= 0;

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


CCopyItemView::~CCopyItemView()
{

}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CCopyItemView::OnDestroy() 
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

	m_haveList.clear();
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CCopyItemView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCopyItemView)
	DDX_Control(pDX, IDC_COMBO, m_ComBoCtrl);
	//}}AFX_DATA_MAP
}


//------------------------------------------------------------------------------
//	FUNCTION	:	-
//	DESCRIPTION	:	- ���ڿ����� ���� �̿��� ��� ���ڸ� �����ϴ� �Լ�
//------------------------------------------------------------------------------
int CCopyItemView::RemoveNotNumber(CString &strNumber)
{
	char*	pChar	= NULL;
	
    do
    {
		pChar = (char *)_mbsspnp((unsigned char *)(LPCTSTR)strNumber,(unsigned char *)_T(".0123456789"));
		
        if( pChar == NULL )
		{
            break;
		}
		
        *pChar = '.';
		
    }while(1);
	
    strNumber.Remove('.');
	
    return strNumber.GetLength();
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CCopyItemView::OnInitialUpdate() 
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

	CCopyItemFrame* pFrame = (CCopyItemFrame*)GetParent();
	m_pApp->m_dwDocCount3++;	
	pFrame->m_nFrameID = m_pApp->m_dwDocCount3;	//�����ӿ� �ڱ� Frame ID ����

	m_nViewID = m_pApp->m_dwDocCount3;

	CString str;
	str.Format("[%02d] CorumOnline Copy Item Search Tool",pFrame->m_nFrameID); 
	pFrame->SetWindowText(str);

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

	CString szDb,szTemp;
	int j = 0;
	for(int i = 0; i < m_nDbCount; i++)
	{
		szDb	= m_DbSchema[i].szSchemaBuffer;
		szTemp	= szDb.Left(5);
		
		if( RemoveNotNumber(szDb) )
		{
			// Corum ���� DB�� 
			if( !szTemp.CompareNoCase("Corum") )
			{
				m_ComBoCtrl.InsertString(j,m_DbSchema[i].szSchemaBuffer);
				j++;
			}
		}
	}
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
UINT ThreadFuncCopyItem(LPVOID pParam)
{
	CCopyItemView* pView	= (CCopyItemView*)pParam;
//	pView->CopyItemSearch();
	pView->ItemSearch();
	pView->m_bRun			= FALSE;
	pView->m_ComBoCtrl.EnableWindow(TRUE);
	pView->m_pWinThread		= NULL;
	return 0;
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
int compare(const HAVEITEMEX& p1, const HAVEITEMEX& p2)
{	
    return p1.i64Serial < p2.i64Serial;
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CCopyItemView::OnGo() 
{
	if(!m_bRun)
	{
		int	nIndex		= m_ComBoCtrl.GetCurSel();
		if(nIndex < 0)
		{
			AfxMessageBox("Can't Find DB Name!!!",MB_OK|MB_ICONINFORMATION);
			return;
		}

		if(m_pApp->m_ItemNoList.GetCount() <= 0)
		{
			AfxMessageBox("Can't Insert New Serial!!! Select Item Please!!!",MB_OK|MB_ICONSTOP);
			return;
		}

		m_pWinThread	= ::AfxBeginThread(ThreadFuncCopyItem,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
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
void CCopyItemView::ItemSearch()
{
	char			szQuery[0xff]	= {0,};
	char			Name[20]		= {0,};
	char			ID[20]			= {0,};
	CString			ShowQuery;
	DWORD			nameCount		= 0;
	DWORD			bankCount		= 0;
	DBRECEIVEDATA*	pData			= NULL;
	CharacterItem2	CharItem;
	BankItem2		BankInfo;
	int				sel				= m_ComBoCtrl.GetCurSel();
	CTime			start			= CTime::GetCurrentTime();

	// ���� ������ �ִ� �������� ����Ʈ�� ���Ϸ� �̴´�.
	CString		folder;
	folder.Format("%s\\SerachItem",g_szExePath);
	
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
	
	CString szServerName;
	m_ComBoCtrl.GetLBText(sel,szServerName);
	SYSTEMTIME tm;
	GetSystemTime(&tm);
	CString szFileName;
	szFileName.Format("%s\\[have][%s][%d.%d.%d %d.%d.%d].csv",folder,szServerName,tm.wYear,tm.wMonth,tm.wDay,tm.wHour,tm.wMinute,tm.wSecond);
	
	ShowQuery.Format("Make .csv File.............");
	m_pEdit->SetWindowText(ShowQuery);

	HANDLE		hFile		= NULL;
	BOOL		result		= FALSE;
	DWORD		Written		= 0;
	CString		szWrite;
	
	hFile = ::CreateFile(szFileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	szWrite.Format("CharName,ID,where,Index\r\n");
	result = WriteFile(hFile,szWrite.GetBuffer(szWrite.GetLength()),szWrite.GetLength(),&Written,NULL);

	/*ĳ���� �̸�*/
	ShowQuery.Format("All CharName Query");
	m_pEdit->SetWindowText(ShowQuery);
	sprintf(szQuery,"SELECT Count(*) FROM CharacterInfo");
	int ret = m_pDb->OpenRecord(szQuery,&nameCount,DEFAULT_ROWS_PER_READ,m_CORUM_BACKUP_DB);		
	sprintf(szQuery,"SELECT [Name] FROM CharacterInfo");		
	pData = m_pDb->OpenRecordEx(szQuery,nameCount,DEFAULT_ROWS_PER_READ,m_CORUM_BACKUP_DB);	

	if( pData != NULL )
	{
		for( DWORD i = 0; i < pData->Query.select.dwRowCount; i++ )	
		{
			if(!m_bRun)
			{				
				m_pDb->ReleaseRecordset(pData);	
				::CloseHandle(hFile);
				return;				
			}
			::Sleep(10);
			
			memset(Name,0,sizeof(Name));					
			memcpy(Name,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize);

			ShowQuery.Format("Q(CharacterInfo):<%d>[%d].[%s]",pData->Query.select.dwRowCount,i+1,Name);
			m_pEdit->SetWindowText(ShowQuery);
			
			sprintf(szQuery,"Select Equip,Inv_large,Inv_small,Belt,Guardian From CharacterInfo Where [Name]='%s'",Name);
			memset(&CharItem,0,sizeof(CharacterItem2));
			ret = m_pDb->OpenRecord(szQuery,&CharItem,1,m_CORUM_BACKUP_DB);

			if( ret <= 0 )
			{
				continue;
			}
			
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
						szWrite.Format("%s,%d,%d,%d\r\n",Name,CharItem.Equip[equip].m_wItemID,WHERE_EQUIP,equip);
						result = WriteFile(hFile,szWrite.GetBuffer(szWrite.GetLength()),szWrite.GetLength(),&Written,NULL);
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
						szWrite.Format("%s,%d,%d,%d\r\n",Name,CharItem.Inv_Large[inv_large].m_wItemID,WHERE_INV_LARGE,inv_large);
						result = WriteFile(hFile,szWrite.GetBuffer(szWrite.GetLength()),szWrite.GetLength(),&Written,NULL);
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
						szWrite.Format("%s,%d,%d,%d\r\n",Name,CharItem.Inv_Small[inv_small].m_wItemID,WHERE_INV_SMALL,inv_small);
						result = WriteFile(hFile,szWrite.GetBuffer(szWrite.GetLength()),szWrite.GetLength(),&Written,NULL);
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
						szWrite.Format("%s,%d,%d,%d\r\n",Name,CharItem.Belt[belt].m_wItemID,WHERE_BELT,belt);
						result = WriteFile(hFile,szWrite.GetBuffer(szWrite.GetLength()),szWrite.GetLength(),&Written,NULL);
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
						szWrite.Format("%s,%d,%d,%d\r\n",Name,CharItem.Guardian[guardian].m_wItemID,WHERE_GUARDIAN,guardian);
						result = WriteFile(hFile,szWrite.GetBuffer(szWrite.GetLength()),szWrite.GetLength(),&Written,NULL);
						break;
					}
				}
			}			
		}		
		m_pDb->ReleaseRecordset(pData);
		pData = NULL;
	}
	
	/*â����̵� �̸�*/
	ShowQuery.Format("All BankID Query");
	m_pEdit->SetWindowText(ShowQuery);
	
	sprintf(szQuery,"SELECT Count(*) FROM BankInfo");	
	ret = m_pDb->OpenRecord(szQuery,&bankCount,DEFAULT_ROWS_PER_READ,m_CORUM_BACKUP_DB);
	sprintf(szQuery,"SELECT [ID] FROM BankInfo");
	pData = m_pDb->OpenRecordEx(szQuery,bankCount,DEFAULT_ROWS_PER_READ,m_CORUM_BACKUP_DB);	

	if( pData != NULL )
	{
		for( DWORD i = 0; i < pData->Query.select.dwRowCount; i++ )	
		{
			if(!m_bRun)
			{				
				m_pDb->ReleaseRecordset(pData);	
				::CloseHandle(hFile);
				return;				
			}
			::Sleep(10);
			
			memset(	ID,0,sizeof(ID)	);				
			memcpy(	ID,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize);
			
			ShowQuery.Format("Q(Bank):<%d>[%d].[%s]",pData->Query.select.dwRowCount,i+1,ID);
			m_pEdit->SetWindowText(ShowQuery);
			
			sprintf(szQuery,"SELECT Bank_Large,Bank_Small FROM BankInfo WHERE [ID]='%s'",ID);
			memset(&BankInfo,0,sizeof(BankItem2));
			int ret = m_pDb->OpenRecord(szQuery,&BankInfo,1,m_CORUM_BACKUP_DB);
			
			if( ret <= 0 )
			{
				StepProgress();
				continue;
			}
			
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
						szWrite.Format("%s,%d,%d,%d\r\n",ID,BankInfo.Bank_Large[banklarge].m_wItemID,WHERE_BANK_LARGE,banklarge);
						result = WriteFile(hFile,szWrite.GetBuffer(szWrite.GetLength()),szWrite.GetLength(),&Written,NULL);
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
						szWrite.Format("%s,%d,%d,%d\r\n",ID,BankInfo.Bank_Small[banksmall].m_wItemID,WHERE_BANK_SMALL,banksmall);
						result = WriteFile(hFile,szWrite.GetBuffer(szWrite.GetLength()),szWrite.GetLength(),&Written,NULL);
						break;
					}
				}
			}			
		}		
		m_pDb->ReleaseRecordset(pData);
		pData = NULL;
	}
	
	::CloseHandle(hFile);

	// �� ������.
	CTimeSpan sult; 
	CTime end	= CTime::GetCurrentTime();
	sult		= end - start;
	LONG hour	= sult.GetTotalHours(); 
	LONG min	= sult.GetTotalMinutes();
	LONG sec	= sult.GetTotalSeconds();
	ShowQuery.Format("[%d]Hours [%d]Min [%d]Sec Passed.....",hour,min,sec);
	m_pEdit->SetWindowText(ShowQuery);
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CCopyItemView::CopyItemSearch()
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
	BANKINFO		BankInfo;
	int				sel				= m_ComBoCtrl.GetCurSel();
	CTime			start			= CTime::GetCurrentTime();
	BOOL			charNoOk		= FALSE;
	BOOL			bankNoOk        = FALSE;

	m_haveList.clear();

	// ���� ������ �ִ� �������� ����Ʈ�� ���Ϸ� �̴´�.
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

	CString szServerName;
	m_ComBoCtrl.GetLBText(sel,szServerName);
	SYSTEMTIME tm;
	GetSystemTime(&tm);
	CString szFileName;
	szFileName.Format("%s\\[have][%s][%d.%d.%d %d.%d.%d].csv",folder,szServerName,tm.wYear,tm.wMonth,tm.wDay,tm.wHour,tm.wMinute,tm.wSecond);
	
	ShowQuery.Format("Make .csv File.............");
	m_pEdit->SetWindowText(ShowQuery);

	HANDLE		hFile		= NULL;
	BOOL		result		= FALSE;
	DWORD		Written		= 0;
	CString		szWrite;

	hFile = ::CreateFile(szFileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	szWrite.Format("Serial,CharName,where,Index,No\r\n");
	result = WriteFile(hFile,szWrite.GetBuffer(szWrite.GetLength()),szWrite.GetLength(),&Written,NULL);

	/*ĳ���� �̸�*/
	ShowQuery.Format("All CharName Query");
	m_pEdit->SetWindowText(ShowQuery);
	sprintf(szQuery,"SELECT Count(*) FROM CharacterInfo");
	int ret = m_pDb->OpenRecord(szQuery,&nameCount,DEFAULT_ROWS_PER_READ,m_CORUM_BACKUP_DB);		
	sprintf(szQuery,"SELECT [Name] FROM CharacterInfo");		
	pData = m_pDb->OpenRecordEx(szQuery,nameCount,DEFAULT_ROWS_PER_READ,m_CORUM_BACKUP_DB);	

	if( pData != NULL )
	{
		for( DWORD i = 0; i < pData->Query.select.dwRowCount; i++ )	
		{
			if(!m_bRun)
			{				
				m_pDb->ReleaseRecordset(pData);	
				::CloseHandle(hFile);
				return;				
			}
			::Sleep(10);
			
			memset(Name,0,sizeof(Name));					
			memcpy(Name,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize);

			ShowQuery.Format("Q(CharacterInfo):<%d>[%d].[%s]",pData->Query.select.dwRowCount,i+1,Name);
			m_pEdit->SetWindowText(ShowQuery);
			
			sprintf(szQuery,"Select Equip,Inv_large,Inv_small,Belt,Guardian From CharacterInfo Where [Name]='%s'",Name);
			memset(&CharItem,0,sizeof(CharacterItem));
			ret = m_pDb->OpenRecord(szQuery,&CharItem,1,m_CORUM_BACKUP_DB);

			if( ret <= 0 )
			{
				StepProgress();
				continue;
			}
			
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
						HAVEITEMEX haveitem;
						memset(&haveitem,0,sizeof(HAVEITEMEX));
						sprintf(haveitem.szCharName,Name);
						haveitem.i64Serial	= CharItem.Equip[equip].m_Serial.i64Serial;
						haveitem.wNo		= id;
						haveitem.bwhere		= WHERE_EQUIP;
						haveitem.bindex		= equip;
						m_haveList.push_back(haveitem);

						szWrite.Format("%I64d,%s,%s,%d,%d\r\n",haveitem.i64Serial,haveitem.szCharName,g_szWhere[haveitem.bwhere],haveitem.bindex,haveitem.wNo);
						result = WriteFile(hFile,szWrite.GetBuffer(szWrite.GetLength()),szWrite.GetLength(),&Written,NULL);
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
						HAVEITEMEX haveitem;
						memset(&haveitem,0,sizeof(HAVEITEMEX));
						sprintf(haveitem.szCharName,Name);
						haveitem.i64Serial	= CharItem.Inv_Large[inv_large].m_Serial.i64Serial;
						haveitem.wNo		= id;
						haveitem.bwhere		= WHERE_INV_LARGE;
						haveitem.bindex		= inv_large;
						m_haveList.push_back(haveitem);

						szWrite.Format("%I64d,%s,%s,%d,%d\r\n",haveitem.i64Serial,haveitem.szCharName,g_szWhere[haveitem.bwhere],haveitem.bindex,haveitem.wNo);
						result = WriteFile(hFile,szWrite.GetBuffer(szWrite.GetLength()),szWrite.GetLength(),&Written,NULL);
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
						HAVEITEMEX haveitem;
						memset(&haveitem,0,sizeof(HAVEITEMEX));
						sprintf(haveitem.szCharName,Name);
						haveitem.i64Serial	= CharItem.Inv_Small[inv_small].m_Serial.i64Serial;
						haveitem.wNo		= id;
						haveitem.bwhere		= WHERE_INV_SMALL;
						haveitem.bindex		= inv_small;
						m_haveList.push_back(haveitem);

						szWrite.Format("%I64d,%s,%s,%d,%d\r\n",haveitem.i64Serial,haveitem.szCharName,g_szWhere[haveitem.bwhere],haveitem.bindex,haveitem.wNo);
						result = WriteFile(hFile,szWrite.GetBuffer(szWrite.GetLength()),szWrite.GetLength(),&Written,NULL);
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
						HAVEITEMEX haveitem;
						memset(&haveitem,0,sizeof(HAVEITEMEX));
						sprintf(haveitem.szCharName,Name);
						haveitem.i64Serial	= CharItem.Belt[belt].m_Serial.i64Serial;
						haveitem.wNo		= id;
						haveitem.bwhere		= WHERE_BELT;
						haveitem.bindex		= belt;
						m_haveList.push_back(haveitem);

						szWrite.Format("%I64d,%s,%s,%d,%d\r\n",haveitem.i64Serial,haveitem.szCharName,g_szWhere[haveitem.bwhere],haveitem.bindex,haveitem.wNo);
						result = WriteFile(hFile,szWrite.GetBuffer(szWrite.GetLength()),szWrite.GetLength(),&Written,NULL);
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
						HAVEITEMEX haveitem;
						memset(&haveitem,0,sizeof(HAVEITEMEX));
						sprintf(haveitem.szCharName,Name);
						haveitem.i64Serial	= CharItem.Guardian[guardian].m_Serial.i64Serial;
						haveitem.wNo		= id;
						haveitem.bwhere		= WHERE_GUARDIAN;
						haveitem.bindex		= guardian;
						m_haveList.push_back(haveitem);

						szWrite.Format("%I64d,%s,%s,%d,%d\r\n",haveitem.i64Serial,haveitem.szCharName,g_szWhere[haveitem.bwhere],haveitem.bindex,haveitem.wNo);
						result = WriteFile(hFile,szWrite.GetBuffer(szWrite.GetLength()),szWrite.GetLength(),&Written,NULL);
						break;
					}
				}
			}			
			StepProgress();
		}		
		m_pDb->ReleaseRecordset(pData);
		pData = NULL;
	}
	else
	{
		charNoOk = TRUE;
	}

	m_pProgress->SetPos(0);
	
	/*â����̵� �̸�*/
	ShowQuery.Format("All BankID Query");
	m_pEdit->SetWindowText(ShowQuery);

	sprintf(szQuery,"SELECT Count(*) FROM BankInfo");	
	ret = m_pDb->OpenRecord(szQuery,&bankCount,DEFAULT_ROWS_PER_READ,m_CORUM_BACKUP_DB);
	sprintf(szQuery,"SELECT [ID] FROM BankInfo");
	pData = m_pDb->OpenRecordEx(szQuery,bankCount,DEFAULT_ROWS_PER_READ,m_CORUM_BACKUP_DB);		

	if( pData != NULL )
	{
		for( DWORD i = 0; i < pData->Query.select.dwRowCount; i++ )	
		{
			if(!m_bRun)
			{				
				m_pDb->ReleaseRecordset(pData);	
				::CloseHandle(hFile);
				return;				
			}
			::Sleep(10);
			
			memset(	ID,0,sizeof(ID)	);				
			memcpy(	ID,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize);

			ShowQuery.Format("Q(Bank):<%d>[%d].[%s]",pData->Query.select.dwRowCount,i+1,ID);
			m_pEdit->SetWindowText(ShowQuery);
			
			sprintf(szQuery,"SELECT Bank_Large,Bank_Small,[Money] FROM BankInfo WHERE [ID]='%s'",ID);
			memset(&BankInfo,0,sizeof(BANKINFO));
			int ret = m_pDb->OpenRecord(szQuery,&BankInfo,1,m_CORUM_BACKUP_DB);
			
			if( ret <= 0 )
			{
				StepProgress();
				continue;
			}
			
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
						HAVEITEMEX haveitem;
						memset(&haveitem,0,sizeof(HAVEITEMEX));
						sprintf(haveitem.szCharName,ID);
						haveitem.i64Serial	= BankInfo.Bank_Large[banklarge].m_Serial.i64Serial;
						haveitem.wNo		= id;
						haveitem.bwhere		= WHERE_BANK_LARGE;
						haveitem.bindex		= banklarge;
						m_haveList.push_back(haveitem);

						szWrite.Format("%I64d,%s,%s,%d,%d\r\n",haveitem.i64Serial,haveitem.szCharName,g_szWhere[haveitem.bwhere],haveitem.bindex,haveitem.wNo);
						result = WriteFile(hFile,szWrite.GetBuffer(szWrite.GetLength()),szWrite.GetLength(),&Written,NULL);
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
						HAVEITEMEX haveitem;
						memset(&haveitem,0,sizeof(HAVEITEMEX));
						sprintf(haveitem.szCharName,ID);
						haveitem.i64Serial	= BankInfo.Bank_Small[banksmall].m_Serial.i64Serial;
						haveitem.wNo		= id;
						haveitem.bwhere		= WHERE_BANK_SMALL;
						haveitem.bindex		= banksmall;
						m_haveList.push_back(haveitem);

						szWrite.Format("%I64d,%s,%s,%d,%d\r\n",haveitem.i64Serial,haveitem.szCharName,g_szWhere[haveitem.bwhere],haveitem.bindex,haveitem.wNo);
						result = WriteFile(hFile,szWrite.GetBuffer(szWrite.GetLength()),szWrite.GetLength(),&Written,NULL);
						break;
					}
				}
			}
			StepProgress();
		}		
		m_pDb->ReleaseRecordset(pData);		
	}
	else
	{
		bankNoOk = TRUE;
	}

	m_pProgress->SetPos(0);
	::CloseHandle(hFile);

	ShowQuery.Format("Wait vector Sort.........");
	m_pEdit->SetWindowText(ShowQuery);

	//************************************************
	sort(m_haveList.begin(),m_haveList.end(),compare);
	//************************************************

	szFileName.Format("%s\\[copy][%s][%d.%d.%d %d.%d.%d].csv",folder,szServerName,tm.wYear,tm.wMonth,tm.wDay,tm.wHour,tm.wMinute,tm.wSecond);
	hFile = ::CreateFile(szFileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	if(charNoOk)
	{
		szWrite.Format("Query Fail To CharacterInfo Name\r\n");
		result = WriteFile(hFile,szWrite.GetBuffer(szWrite.GetLength()),szWrite.GetLength(),&Written,NULL);
	}

	if(bankNoOk)
	{
		szWrite.Format("Query Fail To Bank ID\r\n");
		result = WriteFile(hFile,szWrite.GetBuffer(szWrite.GetLength()),szWrite.GetLength(),&Written,NULL);
	}

	szWrite.Format("Serial,CharName,where,Index,No\r\n");
	result = WriteFile(hFile,szWrite.GetBuffer(szWrite.GetLength()),szWrite.GetLength(),&Written,NULL);

	HAVEITEMEX	h;
	HAVEITEMEX	next;
	HAVEITEMEX	prev;
	int			size	= m_haveList.size();
	int			count	= 0;
	int			n		= 0;
	int			p		= size - 1;
	
	for(int i = 0; i < size; i++)
	{
		if(!m_bRun)
		{				
			::CloseHandle(hFile);
			return;				
		}
		::Sleep(10);

		memset(&h,0,sizeof(HAVEITEMEX));
		memset(&next,0,sizeof(HAVEITEMEX));
		memset(&prev,0,sizeof(HAVEITEMEX));

		h = m_haveList.at(count);

		ShowQuery.Format("Find Copy Item:<%d>[%d].[%s]",size,i+1,h.szCharName);
		m_pEdit->SetWindowText(ShowQuery);
		
		if(count+1 <= p)
			next = m_haveList.at(count+1);
		if(count-1 >= n)
			prev = m_haveList.at(count-1);
		
		if(h.i64Serial == next.i64Serial || h.i64Serial == prev.i64Serial)	
		{
			szWrite.Format("%I64d,%s,%s,%d,%d\r\n",h.i64Serial,h.szCharName,g_szWhere[h.bwhere],h.bindex,h.wNo);
			result = WriteFile(hFile,szWrite.GetBuffer(szWrite.GetLength()),szWrite.GetLength(),&Written,NULL);
		}		
		
		count++;
		StepProgress();
	}

	m_pProgress->SetPos(0);
	::CloseHandle(hFile);

	// �� ������.
	CTimeSpan sult; 
	CTime end	= CTime::GetCurrentTime();
	sult		= end - start;
	LONG hour	= sult.GetTotalHours(); 
	LONG min	= sult.GetTotalMinutes();
	LONG sec	= sult.GetTotalSeconds();
	ShowQuery.Format("[%d]Hours [%d]Min [%d]Sec Passed.....",hour,min,sec);
	m_pEdit->SetWindowText(ShowQuery);
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CCopyItemView::StepProgress()
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
void CCopyItemView::AssertValid() const
{
	CFormView::AssertValid();
}
void CCopyItemView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif 


//------------------------------------------------------------------------------
//	DESCRIPTION	:	- OleDbThread.dll COM �ʱ�ȭ 
//------------------------------------------------------------------------------
BOOL CCopyItemView::InitDatabase()
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
void CCopyItemView::InitDBAccount()
{
	char path[MAX_PATH] = {0,};
	sprintf(path,"%s\\DB.ini",".");
	
	::GetPrivateProfileString("BACKUPDB_INFO","CORUM_DB_NAME",		"",m_CORUM_DB_NAME		,sizeof(m_CORUM_DB_NAME)	,path);	
	::GetPrivateProfileString("BACKUPDB_INFO","CORUM_DB_ACCOUNT",	"",m_CORUM_DB_ACCOUNT	,sizeof(m_CORUM_DB_ACCOUNT)	,path);	
	::GetPrivateProfileString("BACKUPDB_INFO","CORUM_DB_PASSWORD",	"",m_CORUM_DB_PASSWORD	,sizeof(m_CORUM_DB_PASSWORD),path);
	::GetPrivateProfileString("BACKUPDB_INFO","CORUM_DB_ADDRESS",	"",m_CORUM_DB_ADDRESS	,sizeof(m_CORUM_DB_ADDRESS)	,path);
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
BOOL CCopyItemView::InitCorumDB()
{
	char path[MAX_PATH] = {0,};
	sprintf(path,"%s\\DB.ini",".");

	m_CORUM_BACKUP_DB = m_pDb->CreateDBConnection();
		
	if(m_pDb->Connect(m_CORUM_DB_ADDRESS,m_CORUM_DB_NAME,m_CORUM_DB_ACCOUNT,m_CORUM_DB_PASSWORD,20,10,FALSE,m_CORUM_BACKUP_DB) < 0)
	{
		m_pDb->ReleaseDBConnectionForSingleThread(m_CORUM_BACKUP_DB);
		m_CORUM_BACKUP_DB = 0;
		CString str;
		str.Format("Fail to Connect GAME_DB! (IP:%s, DBName:%s)", m_CORUM_DB_ADDRESS, m_CORUM_DB_NAME);
		AfxMessageBox(str,MB_OK|MB_ICONSTOP);
		return FALSE;
	}	
	
	int ret = m_pDb->QueryDBCatalog(m_DbSchema,200,m_CORUM_BACKUP_DB);
	
	if(ret < 0)
	{
		AfxMessageBox("Fail to load DBSCHEMA");
		return FALSE;
	}
	else
	{
		m_nDbCount = ret;
	}
	
	return TRUE;
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CCopyItemView::OnSelchangeCombo() 
{
	int		nIndex		= m_ComBoCtrl.GetCurSel();
	char	szDb[0xff]	= {0,};
	
	m_ComBoCtrl.GetLBText(nIndex,szDb);

	if(!m_pDb->ChangeDB(szDb,m_CORUM_BACKUP_DB))
		AfxMessageBox("ChangeDB Failed!!!");				
}







