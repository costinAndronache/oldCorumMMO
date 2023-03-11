#include "stdafx.h"
#include "NewSerial.h"
#include "GuardianView.h"
#include "GuardianFrame.h"
#include "CopyItemDelete.h"
#include "Doc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CGuardianView, CFormView)


BEGIN_MESSAGE_MAP(CGuardianView, CFormView)
	//{{AFX_MSG_MAP(CGuardianView)
	ON_BN_CLICKED(IDC_BUTTON_GUARDIAN, OnGo)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
CGuardianView::CGuardianView() : CFormView(CGuardianView::IDD)
{
	//{{AFX_DATA_INIT(CGuardianView)
	//}}AFX_DATA_INIT
	m_pApp			= NULL;
	m_nViewID		= 0;
	m_pEdit			= NULL;
	m_pProgress		= NULL;
	m_dwGuardian	= 0;	

	m_pWinThread	= NULL;
	m_bRun			= FALSE;

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


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
CGuardianView::~CGuardianView()
{

}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CGuardianView::OnDestroy() 
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
void CGuardianView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGuardianView)
	DDX_Control(pDX, IDC_COMBO_GUARDIAN, m_ComBoCtrl);
	//}}AFX_DATA_MAP
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CGuardianView::OnInitialUpdate() 
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
	
	CGuardianFrame* pFrame = (CGuardianFrame*)GetParent();
	m_pApp->m_dwDocCount2++;	
	pFrame->m_nFrameID = m_pApp->m_dwDocCount2;	//�����ӿ� �ڱ� Frame ID ����

	CDoc* pDoc = (CDoc*)GetDocument();
	pDoc->m_nDocID2 = m_pApp->m_dwDocCount2;

	m_nViewID = m_pApp->m_dwDocCount2;

	CString str;
	str.Format("[%02d] CorumOnline Guardian Tool",pFrame->m_nFrameID); 
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
UINT NewThreadFunc(LPVOID pParam)
{

	CCopyItemDelete* pView = (CCopyItemDelete*)pParam;	
	pView->CharacterMove();

	//CGuardianView* pView	= (CGuardianView*)pParam;
	//pView->NewGuardian();

	pView->m_bRun			= FALSE;
	pView->m_ComBoCtrl.EnableWindow(TRUE);
	pView->m_pWinThread		= NULL;
	return 0;
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CGuardianView::OnGo() 
{
	if(!m_bRun)
	{
		m_pWinThread	= ::AfxBeginThread(NewThreadFunc,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
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
void CGuardianView::StepProgress()
{
	int	nLower, nUpper;
	int	nCurPos = m_pProgress->GetPos();		
	m_pProgress->GetRange(nLower, nUpper);
	
	if( nCurPos >= nUpper - nLower )
		m_pProgress->SetPos(0);
	else
		m_pProgress->StepIt();
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CGuardianView::NewGuardian()
{
	char			szQuery[0xff]	= {0,};
	char			Name[20]		= {0,};
	char			ID[20]			= {0,};
	CString			ShowQuery;
	DWORD			nameCount		= 0;
	DWORD			bankCount		= 0;
	DBRECEIVEDATA*	pData			= NULL;
	BYTE			bServerSet		= 0;
	CharacterItem	CharItem;
	BANKINFO		BankInfo;

	int sel = m_ComBoCtrl.GetCurSel();

	if(m_pApp->m_ItemNoList.GetCount() <= 0)
	{
		AfxMessageBox("Can't Insert New Serial!!! Select Item Please!!!",MB_OK|MB_ICONSTOP);
		return;
	}
	
	CTime start = CTime::GetCurrentTime();

	/*ĳ���� �̸�*/
	ShowQuery.Format("All CharName Query");
	m_pEdit->SetWindowText(ShowQuery);

	sprintf(szQuery,"SELECT Count(*) FROM CharacterInfo");	
	int ret = g_pDb->OpenRecord(szQuery,&nameCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);	

	sprintf(szQuery,"SELECT [Name] FROM CharacterInfo");	
	pData = g_pDb->OpenRecordEx(szQuery,nameCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);		

	if( pData != NULL )
	{
		for( DWORD i = 0; i < pData->Query.select.dwRowCount; i++ )	
		{
			if(!m_bRun)
			{				
				g_pDb->ReleaseRecordset(pData);				
				return;				
			}
			::Sleep(10);

			memset(Name,0,sizeof(Name));					
			memcpy(Name,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize);
			
			ShowQuery.Format("Q(CharacterInfo):<%d>[%d].[%s]",pData->Query.select.dwRowCount,i+1,Name);
			m_pEdit->SetWindowText(ShowQuery);	
			
			sprintf(szQuery,"Select Equip,Inv_large,Inv_small,Belt,Guardian From CharacterInfo Where [Name]='%s'",Name);
			memset(&CharItem,0,sizeof(CharacterItem));			
			ret = g_pDb->OpenRecord(szQuery,&CharItem,1,m_CORUM_GAME_DB[sel]);
			
			if( ret <= 0 )
				continue;
			
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
						// 2�� 3������ ����� ������ 
						if(CharItem.Inv_Large[inv_large].m_Serial.bServerSet == 2 || CharItem.Inv_Large[inv_large].m_Serial.bServerSet == 3)
						{						
							InsertGuardianInfo(sel,Name,WHERE_INV_LARGE,inv_large,CharItem.Inv_Large[inv_large]);						
						}
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
						// 2�� 3������ ����� ������ 
						if(CharItem.Guardian[guardian].m_Serial.bServerSet == 2 || CharItem.Guardian[guardian].m_Serial.bServerSet == 3)
						{						
							InsertGuardianInfo(sel,Name,WHERE_GUARDIAN,guardian,CharItem.Guardian[guardian]);						
						}
						break;
					}
				}
			}			
			StepProgress();
		}			
		g_pDb->ReleaseRecordset(pData);
		pData = NULL;
	}
	
	/*â����̵� �̸�*/
	ShowQuery.Format("All BankID Query");
	m_pEdit->SetWindowText(ShowQuery);

	sprintf(szQuery,"SELECT Count(*) FROM BankInfo");	
	ret = g_pDb->OpenRecord(szQuery,&bankCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);
	
	sprintf(szQuery,"SELECT [ID] FROM BankInfo");
	pData = g_pDb->OpenRecordEx(szQuery,bankCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);	

	if( pData != NULL )
	{
		for( DWORD i = 0; i < pData->Query.select.dwRowCount; i++ )	
		{
			if(!m_bRun)
			{				
				g_pDb->ReleaseRecordset(pData);				
				return;				
			}
			::Sleep(10);

			memset(	ID,0,sizeof(ID)	);				
			memcpy(	ID,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize);

			ShowQuery.Format("Q(Bank):<%d>[%d].[%s]",pData->Query.select.dwRowCount,i+1,ID);
			m_pEdit->SetWindowText(ShowQuery);
			
			sprintf(szQuery,"SELECT Bank_Large,Bank_Small,[Money] FROM BankInfo WHERE [ID]='%s'",ID);	
			memset(&BankInfo,0,sizeof(BANKINFO));			
			int ret = g_pDb->OpenRecord(szQuery,&BankInfo,1,m_CORUM_GAME_DB[sel]);		
			
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
						// 2�� 3������ ����� ������ 
						if(BankInfo.Bank_Large[banklarge].m_Serial.bServerSet == 2 || BankInfo.Bank_Large[banklarge].m_Serial.bServerSet == 3)
						{						
							InsertGuardianInfo(sel,ID,WHERE_BANK_LARGE,banklarge,BankInfo.Bank_Large[banklarge]);						
						}
						break;
					}
				}
			}
			StepProgress();
		}		
		g_pDb->ReleaseRecordset(pData);		
	}

	m_pProgress->SetPos(0);		
	
	// �� ������.
	CTimeSpan result; 
	CTime end	= CTime::GetCurrentTime();
	result		= end - start;
	LONG hour	= result.GetTotalHours(); 
	LONG min	= result.GetTotalMinutes();
	LONG sec	= result.GetTotalSeconds();
	ShowQuery.Format("[%d]Hours [%d]Min [%d]Guardian Passed",hour,min,m_dwGuardian);
	m_pEdit->SetWindowText(ShowQuery); 
	m_dwGuardian = 0;
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CGuardianView::InsertGuardianInfo(BYTE bFindServerSet,CString szName,BYTE bWhere,BYTE bIndex,CItem GuardianItem)
{
	char	szQuery[0xff]	= {0,};
	BYTE	bServer			= 0;
	DWORD	dwOldID			= 0;
	DWORD	dwNewID			= 0;
	int		ret				= 0;

	GUARDIAN_INFO	guardianinfo;
	CharacterItem	CharItem;
	BANKINFO		BankInfo;
	
	memset(&guardianinfo,0,sizeof(GUARDIAN_INFO));
	memset(&CharItem,0,sizeof(CharacterItem));
	memset(&BankInfo,0,sizeof(BANKINFO));
	bServer = GuardianItem.m_Serial.bServerSet;

	if(bServer == 2)
	{
		sprintf(szQuery,"Select * From GuardianInfo Where ID = %d",GuardianItem.m_Item_Guardian.dwID);
		ret = g_pDb->OpenRecord(szQuery,&guardianinfo,1,m_CORUM_GAME_DB[bServer - 1]);
		if(ret <= 0)
			return;
	}
	else if(bServer == 3)
	{
		sprintf(szQuery,"Select * From GuardianInfo Where ID = %d",GuardianItem.m_Item_Guardian.dwID);
		ret = g_pDb->OpenRecord(szQuery,&guardianinfo,1,m_CORUM_GAME_DB[bServer - 1]);
		if(ret <= 0)
			return;
	}

	sprintf(szQuery,"Select MAX(ID) FROM GuardianInfo");
	ret = g_pDb->OpenRecord(szQuery,&dwOldID,1,m_CORUM_GAME_DB[bFindServerSet]);
	if(ret <= 0)
		return;

	sprintf(szQuery,"Insert Into GuardianInfo ([Name],[BaseIndex],[Level],[Exp],[HP],[MP]) values ('%s',%d,%d,%d,%d,%d)",
					guardianinfo.szName,guardianinfo.dwKind,guardianinfo.dwLevel,guardianinfo.dwExp,guardianinfo.dwHP,guardianinfo.dwMP);
	ret = g_pDb->ExecuteSQL(szQuery,m_CORUM_GAME_DB[bFindServerSet]);
	if(ret <= 0)
		return;

	sprintf(szQuery,"Select MAX(ID) FROM GuardianInfo");
	ret = g_pDb->OpenRecord(szQuery,&dwNewID,1,m_CORUM_GAME_DB[bFindServerSet]);
	if(ret <= 0)
		return;

	if(dwNewID > dwOldID)
	{
		if(bWhere == WHERE_INV_LARGE || bWhere == WHERE_GUARDIAN)
		{
			sprintf(szQuery,"Select Equip,Inv_large,Inv_small,Belt,Guardian From CharacterInfo Where [Name]='%s'",szName);
			ret = g_pDb->OpenRecord(szQuery,&CharItem,1,m_CORUM_GAME_DB[bFindServerSet]);
			if( ret <= 0 )
				return;

			if(bWhere == WHERE_INV_LARGE)
			{
				CharItem.Inv_Large[bIndex].m_Item_Guardian.dwID = dwNewID;
				CharItem.Inv_Large[bIndex].m_Serial.bServerSet	= bFindServerSet + 1;
			}
			else if(bWhere == WHERE_GUARDIAN)
			{
				CharItem.Guardian[bIndex].m_Item_Guardian.dwID	= dwNewID;
				CharItem.Guardian[bIndex].m_Serial.bServerSet	= bFindServerSet + 1;
			}

			// ������Ʈ 
			int		nColumn			= 5;	
			DWORD	dwParamOffset	= 0;	
			int		nAffected		= 0;
			
			sprintf(szQuery,"Update CharacterInfo Set [Equip]=?,[Inv_large]=?,[Inv_small]=?,[Belt]=?,"
							"[Guardian]=? Where [Name]='%s'",szName);
			
			DBBINDING* pBinding = g_pDb->CreateParamInfo(nColumn);
			if(!pBinding)
				return;				
			
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
			
			int affected = g_pDb->ExecuteSQLByParam(szQuery,pBinding,&CharItem,nColumn,m_CORUM_GAME_DB[bFindServerSet]);	
			if(affected <= 0)
			{
				g_pDb->ReleaseParamInfo(pBinding);
				return;	
			}				
			g_pDb->ReleaseParamInfo(pBinding);

			m_dwGuardian += 1;
		}
		else if(bWhere == WHERE_BANK_LARGE)
		{
			sprintf(szQuery,"Select Bank_Large,Bank_Small,[Money] FROM BankInfo Where [ID]='%s'",szName);
			int ret = g_pDb->OpenRecord(szQuery,&BankInfo,1,m_CORUM_GAME_DB[bFindServerSet]);			
			if( ret <= 0 )
				return;
			
			BankInfo.Bank_Large[bIndex].m_Item_Guardian.dwID	= dwNewID;
			BankInfo.Bank_Large[bIndex].m_Serial.bServerSet		= bFindServerSet + 1;

			// ������Ʈ 
			int		nColumn			= 3;	
			DWORD	dwParamOffset	= 0;	
			
			sprintf(szQuery,"Update Bankinfo Set Bank_Large=?,Bank_Small=?,[Money]=? Where [ID]='%s'",szName);				
			DBBINDING* pBinding = g_pDb->CreateParamInfo(nColumn);
			
			if(!pBinding)
				return;
			
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
			
			int affected = g_pDb->ExecuteSQLByParam(szQuery,pBinding,&BankInfo,nColumn,m_CORUM_GAME_DB[bFindServerSet]);	
			if( affected < 0 )
			{
				g_pDb->ReleaseParamInfo(pBinding);
				return;
			}				
			g_pDb->ReleaseParamInfo(pBinding);

			m_dwGuardian += 1;
		}
	}
}


#ifdef _DEBUG
void CGuardianView::AssertValid() const
{
	CFormView::AssertValid();
}
void CGuardianView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif 


//------------------------------------------------------------------------------
//	DESCRIPTION	:	- OleDbThread.dll COM �ʱ�ȭ 
//------------------------------------------------------------------------------
BOOL CGuardianView::InitDatabase()
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
void CGuardianView::InitDBAccount()
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
BOOL CGuardianView::InitCorumDB()
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

//=============================================================================//
/*{
	char			szQuery[0xff]	= {0,};
	char			Name[20]		= {0,};
	char			ID[20]			= {0,};
	CString			ShowQuery;
	DWORD			nameCount		= 0;
	DWORD			bankCount		= 0;
	DBRECEIVEDATA*	pData			= NULL;
	BYTE			bServerSet		= 0;
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
	pData = g_pDb->OpenRecordEx(szQuery,nameCount,DEFAULT_ROWS_PER_READ, m_CORUM_GAME_DB[sel]);		

	if( pData != NULL )
	{
		for( DWORD i = 0; i < pData->Query.select.dwRowCount; i++ )	
		{
			if(!m_bRun)
			{				
				g_pDb->ReleaseRecordset(pData);				
				return;				
			}
			::Sleep(10);

			memset(Name,0,sizeof(Name));					
			memcpy(Name,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize);
			
			ShowQuery.Format("Q(CharacterInfo):<%d>[%d].[%s]",pData->Query.select.dwRowCount,i+1,Name);
			m_pEdit->SetWindowText(ShowQuery);	
			
			sprintf(szQuery,"Select Inv_large From CharacterInfo Where [Name]='%s'", Name);
			memset(&CharItem,0,sizeof(CharacterItem));			
			ret = g_pDb->OpenRecord(szQuery, &CharItem, 1, m_CORUM_GAME_DB[sel]);
			
			if( ret <= 0 )
				continue;
			
			// Inv_Large
			BOOL bChk = FALSE;

			for(int i = 0; i < MAX_INV_LARGE; i++)
			{
				WORD wId = CharItem.Inv_Large[i].GetID();
				
				if(wId==0)
					continue;								

				if(CharItem.Inv_Large[i].m_Serial.i64Serial==0)
				{
					CharItem.Inv_Large[i].m_Serial = GetNewSerial();
					bChk = TRUE;
				}
			}

			if(bChk)
			{
				sprintf(szQuery,"Update CharacterInfo Set [Inv_large]=? Where [Name]='%s'", Name);
				
				DBBINDING* pBinding = m_pDb->CreateParamInfo(1);
				if(!pBinding)
					continue;				
				
				pBinding[0].obValue		= dwParamOffset;	
				pBinding[0].cbMaxLen	= sizeof(CharItem);
				pBinding[0].wType		= DBTYPE_BYTES;
				dwParamOffset			+= sizeof(CharItem);										
				
				int affected = m_pDb->ExecuteSQLByParam(szQuery,pBinding, &CharItem, 1, m_CORUM_GAME_DB[sel]);	
				
				if(affected <= 0)
				{
					m_pDb->ReleaseParamInfo(pBinding);
					continue;	
				}				
				m_pDb->ReleaseParamInfo(pBinding);
			}
			StepProgress();
		}			
		g_pDb->ReleaseRecordset(pData);
		pData = NULL;
	}
	// �� ������.
	CTimeSpan result; 
	CTime end	= CTime::GetCurrentTime();
	result		= end - start;
	LONG hour	= result.GetTotalHours(); 
	LONG min	= result.GetTotalMinutes();
	LONG sec	= result.GetTotalSeconds();
	ShowQuery.Format("[%d]Hours [%d]Min [%d]Guardian Passed",hour,min,m_dwGuardian);
	m_pEdit->SetWindowText(ShowQuery); 
	m_dwGuardian = 0;
}*/

