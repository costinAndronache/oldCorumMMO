#include "stdafx.h"
#include "NewSerial.h"
#include "CopyItemDelete.h"
#include "CopyItemDeleteFrame.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CCopyItemDelete, CFormView)


BEGIN_MESSAGE_MAP(CCopyItemDelete, CFormView)
	//{{AFX_MSG_MAP(CCopyItemDelete)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO, OnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO_BACKUP, OnSelchangeComboBackup)
	ON_BN_CLICKED(IDC_BUTTON, OnGo)
	ON_BN_CLICKED(IDC_BUTTON_SKILLINIT, OnSkillinit)
	ON_BN_CLICKED(IDC_BUTTON_STATINIT, OnStatinit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
CCopyItemDelete::CCopyItemDelete()	: CFormView(CCopyItemDelete::IDD)
{
	//{{AFX_DATA_INIT(CCopyItemDelete)
	m_szCharName = _T("");
	//}}AFX_DATA_INIT
	m_pApp			= NULL;
	m_nViewID		= 0;
	m_pEdit			= NULL;
	m_pProgress		= NULL;	
	m_bRun			= FALSE;
	m_pWinThread	= NULL;	
	m_hThread		= NULL;	
	m_pDb			= NULL;
	m_nDbCount		= 0;

	memset(m_CORUM_DB_NAME,0,sizeof(m_CORUM_DB_NAME));		
	memset(m_CORUM_DB_ACCOUNT,0,sizeof(m_CORUM_DB_ACCOUNT));		
	memset(m_CORUM_DB_PASSWORD,0,sizeof(m_CORUM_DB_PASSWORD));	
	memset(m_CORUM_DB_ADDRESS,0,sizeof(m_CORUM_DB_ADDRESS));
	memset(m_CORUM_BDB_NAME,0,sizeof(m_CORUM_BDB_NAME));		
	memset(m_CORUM_BDB_ACCOUNT,0,sizeof(m_CORUM_BDB_ACCOUNT));		
	memset(m_CORUM_BDB_PASSWORD,0,sizeof(m_CORUM_BDB_PASSWORD));	
	memset(m_CORUM_BDB_ADDRESS,0,sizeof(m_CORUM_BDB_ADDRESS));
	memset(m_CORUM_GAME_DB,0,sizeof(m_CORUM_GAME_DB));	

	memset(m_CORUM_BDBB_NAME,0,sizeof(m_CORUM_BDBB_NAME));
	memset(m_CORUM_BDBB_ACCOUNT,0,sizeof(m_CORUM_BDBB_ACCOUNT));		
	memset(m_CORUM_BDBB_PASSWORD,0,sizeof(m_CORUM_BDBB_PASSWORD));	
	memset(m_CORUM_BDBB_ADDRESS,0,sizeof(m_CORUM_BDBB_ADDRESS));

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


CCopyItemDelete::~CCopyItemDelete()
{
	
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CCopyItemDelete::OnDestroy() 
{
	CFormView::OnDestroy();	

	delete m_pEdit;
	delete m_pProgress;
	
	if(m_pDb)	
	{
		// 인자가 0이면 모든 Connection Release...
		m_pDb->ReleaseDBConnection(0);	
		m_pDb->Release();
	}
	
	m_haveList.clear();
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CCopyItemDelete::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCopyItemDelete)
	DDX_Control(pDX, IDC_CHECK, m_Check);
	DDX_Control(pDX, IDC_COMBO_BACKUP, m_ComBoCtrl_Backup);
	DDX_Control(pDX, IDC_COMBO, m_ComBoCtrl);
	DDX_Text(pDX, IDC_EDIT_CHARNAME, m_szCharName);
	//}}AFX_DATA_MAP
}


//------------------------------------------------------------------------------
//	FUNCTION	:	-
//	DESCRIPTION	:	- 문자열에서 숫자 이외의 모든 문자를 제거하는 함수
//------------------------------------------------------------------------------
int CCopyItemDelete::RemoveNotNumber(CString &strNumber)
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
void CCopyItemDelete::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	m_pApp = (CApp*)AfxGetApp();

	// OleDbThread.dll COM 초기화
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

	CCopyItemDeleteFrame* pFrame = (CCopyItemDeleteFrame*)GetParent();
	m_pApp->m_dwDocCount4++;	
	pFrame->m_nFrameID = m_pApp->m_dwDocCount4;	//프레임에 자기 Frame ID 셋팅	
	m_nViewID = m_pApp->m_dwDocCount4;

	CString str;
//	str.Format("[%02d] CorumOnline Copy Item Delete Tool",pFrame->m_nFrameID); 
	str.Format("[%02d] CorumOnline Character Move Tool",pFrame->m_nFrameID); 
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
	m_pProgress->ShowWindow(SW_HIDE);

	char path[MAX_PATH] = {0,};
	sprintf(path,"%s\\DB.ini",".");
	int TotalDBNum	= ::GetPrivateProfileInt("DB","TotalDBNum",0,path);

	// 콤보
	for( int i = 0 ; i < TotalDBNum; i++ ) 
		m_ComBoCtrl.InsertString(i,m_CORUM_DB_NAME[i]);
	m_ComBoCtrl.InsertString(TotalDBNum,"Backup");
	m_ComBoCtrl.SetCurSel(0);
	m_ComBoCtrl_Backup.EnableWindow(FALSE);
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	- 
//------------------------------------------------------------------------------
void CCopyItemDelete::OnSelchangeCombo() 
{
	int sel = m_ComBoCtrl.GetCurSel();
	CString str;
	CString szBackup("Backup");
	m_ComBoCtrl.GetLBText(sel,str);

	if(str == szBackup)
	{
		m_ComBoCtrl_Backup.EnableWindow(TRUE);
		m_ComBoCtrl_Backup.ResetContent();

		CString szDb,szTemp;
		int j = 0;
		for(int i = 0; i < m_nDbCount; i++)
		{
			szDb	= m_DbSchema[i].szSchemaBuffer;
			szTemp	= szDb.Left(5);
			
			if( RemoveNotNumber(szDb) )
			{
				// Corum 관련 DB만 
				if( !szTemp.CompareNoCase("Corum") )
				{
					m_ComBoCtrl_Backup.InsertString(j,m_DbSchema[i].szSchemaBuffer);
					j++;
				}
			}
		}
	}
	else
	{
		m_ComBoCtrl_Backup.EnableWindow(FALSE);
	}
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	- 
//------------------------------------------------------------------------------
void CCopyItemDelete::OnSelchangeComboBackup() 
{
	int		nIndex		= m_ComBoCtrl_Backup.GetCurSel();
	char	szDb[0xff]	= {0,};
	
	m_ComBoCtrl_Backup.GetLBText(nIndex,szDb);

	char path[MAX_PATH] = {0,};
	sprintf(path,"%s\\DB.ini",".");
	int TotalDBNum	= ::GetPrivateProfileInt("DB","TotalDBNum",0,path);
	
	if(!m_pDb->ChangeDB(szDb,m_CORUM_GAME_DB[TotalDBNum]))
		AfxMessageBox("ChangeDB Failed!!!");	
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
UINT ThreadFuncDeleteItem(LPVOID pParam)
{
	CCopyItemDelete* pView	= (CCopyItemDelete*)pParam;

//	if(pView->m_Check.GetCheck())
//		pView->SetLevelExp();
//	else
//		pView->SetAttrArmor();
//		pView->DeleteItemSearch();

//	pView->ConvertCItem();
//	pView->StatInitialize();
//	pView->SkillInitialize();

//	pView->MapInfoInitialize();

//	pView->DeleteItemmall();

//	pView->ConvertGuardianInfo();

//	pView->FindItem();

	pView->CharacterMoveSin();
	
	pView->m_bRun			= FALSE;
	pView->m_ComBoCtrl.EnableWindow(TRUE);
	pView->m_ComBoCtrl_Backup.EnableWindow(TRUE);
	pView->m_pWinThread		= NULL;
	return 0;
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CCopyItemDelete::OnGo() 
{
	if(!m_bRun)
	{
		char path[MAX_PATH] = {0,};
		sprintf(path,"%s\\DB.ini",".");
		int TotalDBNum	= ::GetPrivateProfileInt("DB","TotalDBNum",0,path);
		int sel = m_ComBoCtrl.GetCurSel();

		if(sel == TotalDBNum)
		{
			int	nIndex		= m_ComBoCtrl_Backup.GetCurSel();
			if(nIndex < 0)
			{
				AfxMessageBox("Can't Find BackupDB Name!!!",MB_OK|MB_ICONINFORMATION);
				return;
			}
		}
/*
		if(m_pApp->m_ItemNoList.GetCount() <= 0)
		{
			AfxMessageBox("Can't Insert New Serial!!! Select Item Please!!!",MB_OK|MB_ICONSTOP);
			return;
		}
*/		
		m_pWinThread	= ::AfxBeginThread(ThreadFuncDeleteItem,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
		::DuplicateHandle(GetCurrentProcess(),m_pWinThread->m_hThread,GetCurrentProcess(),&m_hThread,0,FALSE,DUPLICATE_SAME_ACCESS);
		m_pWinThread->ResumeThread();
		
		m_bRun			= TRUE;
		m_ComBoCtrl.EnableWindow(FALSE);
		m_ComBoCtrl_Backup.EnableWindow(FALSE);
	}
	else
	{
		AfxMessageBox("Thread Running....",MB_OK|MB_ICONINFORMATION);
	}	
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
int D_compare(const HAVEITEMEX& p1, const HAVEITEMEX& p2)
{	
    return p1.i64Serial < p2.i64Serial;
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CCopyItemDelete::SkillInitialize()
{
	char				szQuery[0xff]	= {0,};
	char				Name[20]		= {0,};
	DWORD				nameCount		= 0;
	DBRECEIVEDATA*		pData			= NULL;
	int					sel				= m_ComBoCtrl.GetCurSel();
	CString				ShowQuery;
	SkillInit			skillinit;	

	/*캐릭터 이름*/
	ShowQuery.Format("All CharName Query");
	m_pEdit->SetWindowText(ShowQuery);
	sprintf(szQuery,"SELECT Count(*) FROM CharacterInfo");
	int ret = m_pDb->OpenRecord(szQuery,&nameCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);		
	sprintf(szQuery,"SELECT [Name] FROM CharacterInfo");		
	pData = m_pDb->OpenRecordEx(szQuery,nameCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);
	
	if( pData == NULL )
	{
		return;		
	}
	
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

		sprintf(szQuery,"Select Point_Skill,[Level],SelectedSkill_L,SelectedSkill_R,SkillLevel,LastUsedSkillItem From CharacterInfo Where [Name]='%s'",Name);
		memset(&skillinit,0,sizeof(SkillInit));
		ret = m_pDb->OpenRecord(szQuery,&skillinit,1,m_CORUM_GAME_DB[sel]);

		if( ret <= 0 )
		{
			continue;
		}

		if(skillinit.m_byLastUsedSkillItem == 0)
		{
			skillinit.m_wPointSkill = skillinit.m_dwLevel;
		}
		else if(skillinit.m_byLastUsedSkillItem != 0)
		{
			skillinit.m_wPointSkill = skillinit.m_dwLevel + skillinit.m_byLastUsedSkillItem - 50;
		}

		skillinit.m_shSelectedSkill[0] = 151;
		skillinit.m_shSelectedSkill[1] = -1;

		memset(skillinit.m_pwSkillLevel,0,sizeof(skillinit.m_pwSkillLevel));

		int			nColumn			= 6;	// 파라미터 갯수 지정 
		DWORD		dwParamOffset	= 0;	// 파라미터 Binding 정보 구조체에서 각 파라미터 Value들의 Offset
		DBBINDING*	pBinding		= g_pDb->CreateParamInfo(nColumn);		
		if(!pBinding)	continue;
		
		sprintf(szQuery," Update CharacterInfo Set [Point_Skill]=?, [Level]=?, [SelectedSkill_L]=?, [SelectedSkill_R]=?, "
						" [SkillLevel]=?, [LastUsedSkillItem]=? Where [Name]='%s' ",Name);
		
		pBinding[0].obValue		= dwParamOffset;	
		pBinding[0].cbMaxLen	= sizeof(skillinit.m_wPointSkill);
		pBinding[0].wType		= DBTYPE_I2;
		dwParamOffset			+= sizeof(skillinit.m_wPointSkill);				
		
		pBinding[1].obValue		= dwParamOffset;	
		pBinding[1].cbMaxLen	= sizeof(skillinit.m_dwLevel);
		pBinding[1].wType		= DBTYPE_I4;
		dwParamOffset			+= sizeof(skillinit.m_dwLevel);		
		
		pBinding[2].obValue		= dwParamOffset;	
		pBinding[2].cbMaxLen	= sizeof(skillinit.m_shSelectedSkill[0]);
		pBinding[2].wType		= DBTYPE_I2;
		dwParamOffset			+= sizeof(skillinit.m_shSelectedSkill[0]);			
		
		pBinding[3].obValue		= dwParamOffset;	
		pBinding[3].cbMaxLen	= sizeof(skillinit.m_shSelectedSkill[1]);
		pBinding[3].wType		= DBTYPE_I2;
		dwParamOffset			+= sizeof(skillinit.m_shSelectedSkill[1]);
		
		pBinding[4].obValue		= dwParamOffset;	
		pBinding[4].cbMaxLen	= sizeof(skillinit.m_pwSkillLevel);
		pBinding[4].wType		= DBTYPE_BYTES;
		dwParamOffset			+= sizeof(skillinit.m_pwSkillLevel);

		pBinding[5].obValue		= dwParamOffset;	
		pBinding[5].cbMaxLen	= sizeof(skillinit.m_byLastUsedSkillItem);
		pBinding[5].wType		= DBTYPE_UI1;
		dwParamOffset			+= sizeof(skillinit.m_byLastUsedSkillItem);
		
		g_pDb->ExecuteSQLByParam(szQuery,pBinding,&skillinit,nColumn,m_CORUM_GAME_DB[sel]);
		g_pDb->ReleaseParamInfo(pBinding);
	}

	m_pDb->ReleaseRecordset(pData);
	
	// 다 끝났다.
	AfxMessageBox("Skill Initialize End!!!",MB_OK|MB_ICONINFORMATION);
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CCopyItemDelete::OnSkillinit() 
{
	UpdateData(TRUE);

	char				szQuery[0xff]	= {0,};
	char				Name[20]		= {0,};
	DWORD				nameCount		= 0;
	DBRECEIVEDATA*		pData			= NULL;
	int					sel				= m_ComBoCtrl.GetCurSel();
	CString				ShowQuery;
	SkillInit			skillinit;
	
	sprintf(szQuery,"Select Point_Skill,[Level],SelectedSkill_L,SelectedSkill_R,SkillLevel,LastUsedSkillItem From CharacterInfo Where [Name]='%s'",m_szCharName);
	memset(&skillinit,0,sizeof(SkillInit));
	int ret = m_pDb->OpenRecord(szQuery,&skillinit,1,m_CORUM_GAME_DB[sel]);

	if(ret <= 0)
	{
		AfxMessageBox("Can't Find Character",MB_OK|MB_ICONSTOP);
		return;
	}

	if(skillinit.m_byLastUsedSkillItem == 0)
	{
		skillinit.m_wPointSkill = skillinit.m_dwLevel;
	}
	else if(skillinit.m_byLastUsedSkillItem != 0)
	{
		skillinit.m_wPointSkill = skillinit.m_dwLevel + skillinit.m_byLastUsedSkillItem - 50;
	}
	
	skillinit.m_shSelectedSkill[0] = 151;
	skillinit.m_shSelectedSkill[1] = -1;
	
	memset(skillinit.m_pwSkillLevel,0,sizeof(skillinit.m_pwSkillLevel));
	
	int			nColumn			= 6;	// 파라미터 갯수 지정 
	DWORD		dwParamOffset	= 0;	// 파라미터 Binding 정보 구조체에서 각 파라미터 Value들의 Offset
	DBBINDING*	pBinding		= g_pDb->CreateParamInfo(nColumn);		
	if(!pBinding)
	{
		AfxMessageBox("Skill Initialize Fail!!!",MB_OK|MB_ICONSTOP);
		return;
	}
	
	sprintf(szQuery," Update CharacterInfo Set [Point_Skill]=?, [Level]=?, [SelectedSkill_L]=?, [SelectedSkill_R]=?, "
					" [SkillLevel]=?, [LastUsedSkillItem]=? Where [Name]='%s' ",m_szCharName);
	
	pBinding[0].obValue		= dwParamOffset;	
	pBinding[0].cbMaxLen	= sizeof(skillinit.m_wPointSkill);
	pBinding[0].wType		= DBTYPE_I2;
	dwParamOffset			+= sizeof(skillinit.m_wPointSkill);				
	
	pBinding[1].obValue		= dwParamOffset;	
	pBinding[1].cbMaxLen	= sizeof(skillinit.m_dwLevel);
	pBinding[1].wType		= DBTYPE_I4;
	dwParamOffset			+= sizeof(skillinit.m_dwLevel);		
	
	pBinding[2].obValue		= dwParamOffset;	
	pBinding[2].cbMaxLen	= sizeof(skillinit.m_shSelectedSkill[0]);
	pBinding[2].wType		= DBTYPE_I2;
	dwParamOffset			+= sizeof(skillinit.m_shSelectedSkill[0]);			
	
	pBinding[3].obValue		= dwParamOffset;	
	pBinding[3].cbMaxLen	= sizeof(skillinit.m_shSelectedSkill[1]);
	pBinding[3].wType		= DBTYPE_I2;
	dwParamOffset			+= sizeof(skillinit.m_shSelectedSkill[1]);
	
	pBinding[4].obValue		= dwParamOffset;	
	pBinding[4].cbMaxLen	= sizeof(skillinit.m_pwSkillLevel);
	pBinding[4].wType		= DBTYPE_BYTES;
	dwParamOffset			+= sizeof(skillinit.m_pwSkillLevel);
	
	pBinding[5].obValue		= dwParamOffset;	
	pBinding[5].cbMaxLen	= sizeof(skillinit.m_byLastUsedSkillItem);
	pBinding[5].wType		= DBTYPE_UI1;
	dwParamOffset			+= sizeof(skillinit.m_byLastUsedSkillItem);
	
	int affected = g_pDb->ExecuteSQLByParam(szQuery,pBinding,&skillinit,nColumn,m_CORUM_GAME_DB[sel]);
	g_pDb->ReleaseParamInfo(pBinding);

	if(affected > 0)
	{
		AfxMessageBox("Skill Initialize Success!!!",MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		AfxMessageBox("Skill Initialize Fail!!!",MB_OK|MB_ICONSTOP);
	}
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CCopyItemDelete::StatInitialize()
{
	char				szQuery[0xff]	= {0,};
	char				Name[20]		= {0,};
	DWORD				nameCount		= 0;
	DBRECEIVEDATA*		pData			= NULL;
	int					sel				= m_ComBoCtrl.GetCurSel();
	CString				ShowQuery;
	StatInit			statinit;

	BYTE byStstus[MAX_LEVEL] = {0,};
	int ret = m_pDb->OpenRecord("Select Status From Char_Status",&byStstus,MAX_LEVEL,m_CORUM_GAME_DB[sel]);
	if(ret < 0)	return;

	/*캐릭터 이름*/
	ShowQuery.Format("All CharName Query");
	m_pEdit->SetWindowText(ShowQuery);
	sprintf(szQuery,"SELECT Count(*) FROM CharacterInfo");
	ret = m_pDb->OpenRecord(szQuery,&nameCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);		
	sprintf(szQuery,"SELECT [Name] FROM CharacterInfo");		
	pData = m_pDb->OpenRecordEx(szQuery,nameCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);
	
	if( pData == NULL )
	{
		return;		
	}
	
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
		
		sprintf(szQuery,"Select Class,Point,[Level],Ego,Str,[Int],Dex,Vit,LastUsedStatItem From CharacterInfo Where [Name]='%s'",Name);
		memset(&statinit,0,sizeof(StatInit));
		ret = m_pDb->OpenRecord(szQuery,&statinit,1,m_CORUM_GAME_DB[sel]);
		
		if( ret <= 0 )
		{
			continue;
		}

		if(statinit.m_byLastUsedStatItem == 0)
		{
			for(int i = 0; i < statinit.m_dwLevel-1; i++)	
			{
				statinit.m_wPoint += byStstus[i];
			}
		}
		else if(statinit.m_byLastUsedStatItem != 0)
		{
			for(int i = 0; i < statinit.m_dwLevel-1; i++)	
			{
				statinit.m_wPoint += byStstus[i];
			}

			statinit.m_wPoint = statinit.m_wPoint + statinit.m_byLastUsedStatItem - 50;
		}

		switch(statinit.m_wClass)
		{
		case 1:
			{
				statinit.m_dwEgo	= 10;
				statinit.m_dwStr	= 35;
				statinit.m_dwInt	= 10;
				statinit.m_dwDex	= 15;
				statinit.m_dwVit	= 15;				
			}	
			break;
		case 2:
			{   
				statinit.m_dwEgo	= 10;
				statinit.m_dwStr	= 20;
				statinit.m_dwInt	= 20;
				statinit.m_dwDex	= 15;
				statinit.m_dwVit	= 20;				
			}
			break;
		case 3:
			{
				statinit.m_dwEgo	= 15;
				statinit.m_dwStr	= 10;
				statinit.m_dwInt	= 25;
				statinit.m_dwDex	= 10;
				statinit.m_dwVit	= 25;				
			}
			break;
		case 4:
			{
				statinit.m_dwEgo	= 20;
				statinit.m_dwStr	= 10;
				statinit.m_dwInt	= 10;
				statinit.m_dwDex	= 35;
				statinit.m_dwVit	= 10;				
			}
			break;
		case 5:
			{
				statinit.m_dwEgo	= 25;
				statinit.m_dwStr	= 10;
				statinit.m_dwInt	= 35;
				statinit.m_dwDex	= 5;
				statinit.m_dwVit	= 10;			
			}
			break;
		}
		
		int			nColumn			= 9;	// 파라미터 갯수 지정 
		DWORD		dwParamOffset	= 0;	// 파라미터 Binding 정보 구조체에서 각 파라미터 Value들의 Offset
		DBBINDING*	pBinding		= g_pDb->CreateParamInfo(nColumn);		
		if(!pBinding)	continue;
		
		sprintf(szQuery," Update CharacterInfo Set [Class]=?, [Point]=?, [Level]=?, [Ego]=?, "
						" [Str]=?, [Int]=?, [Dex]=?, [Vit]=?, [LastUsedStatItem]=? Where [Name]='%s' ",Name);
		
		pBinding[0].obValue		= dwParamOffset;	
		pBinding[0].cbMaxLen	= sizeof(statinit.m_wClass);
		pBinding[0].wType		= DBTYPE_I2;
		dwParamOffset			+= sizeof(statinit.m_wClass);				
		
		pBinding[1].obValue		= dwParamOffset;	
		pBinding[1].cbMaxLen	= sizeof(statinit.m_wPoint);
		pBinding[1].wType		= DBTYPE_I2;
		dwParamOffset			+= sizeof(statinit.m_wPoint);		
		
		pBinding[2].obValue		= dwParamOffset;	
		pBinding[2].cbMaxLen	= sizeof(statinit.m_dwLevel);
		pBinding[2].wType		= DBTYPE_I4;
		dwParamOffset			+= sizeof(statinit.m_dwLevel);			
		
		pBinding[3].obValue		= dwParamOffset;	
		pBinding[3].cbMaxLen	= sizeof(statinit.m_dwEgo);
		pBinding[3].wType		= DBTYPE_I4;
		dwParamOffset			+= sizeof(statinit.m_dwEgo);
		
		pBinding[4].obValue		= dwParamOffset;	
		pBinding[4].cbMaxLen	= sizeof(statinit.m_dwStr);
		pBinding[4].wType		= DBTYPE_I4;
		dwParamOffset			+= sizeof(statinit.m_dwStr);
		
		pBinding[5].obValue		= dwParamOffset;	
		pBinding[5].cbMaxLen	= sizeof(statinit.m_dwInt);
		pBinding[5].wType		= DBTYPE_I4;
		dwParamOffset			+= sizeof(statinit.m_dwInt);

		pBinding[6].obValue		= dwParamOffset;	
		pBinding[6].cbMaxLen	= sizeof(statinit.m_dwDex);
		pBinding[6].wType		= DBTYPE_I4;
		dwParamOffset			+= sizeof(statinit.m_dwDex);

		pBinding[7].obValue		= dwParamOffset;	
		pBinding[7].cbMaxLen	= sizeof(statinit.m_dwVit);
		pBinding[7].wType		= DBTYPE_I4;
		dwParamOffset			+= sizeof(statinit.m_dwVit);

		pBinding[8].obValue		= dwParamOffset;	
		pBinding[8].cbMaxLen	= sizeof(statinit.m_byLastUsedStatItem);
		pBinding[8].wType		= DBTYPE_UI1;
		dwParamOffset			+= sizeof(statinit.m_byLastUsedStatItem);
		
		g_pDb->ExecuteSQLByParam(szQuery,pBinding,&statinit,nColumn,m_CORUM_GAME_DB[sel]);
		g_pDb->ReleaseParamInfo(pBinding);
	}

	m_pDb->ReleaseRecordset(pData);
	
	// 다 끝났다.
	AfxMessageBox("Stat Initialize End!!!",MB_OK|MB_ICONINFORMATION);
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CCopyItemDelete::OnStatinit() 
{
	UpdateData(TRUE);

	char				szQuery[0xff]	= {0,};
	char				Name[20]		= {0,};
	DWORD				nameCount		= 0;
	DBRECEIVEDATA*		pData			= NULL;
	int					sel				= m_ComBoCtrl.GetCurSel();
	CString				ShowQuery;
	StatInit			statinit;
	
	BYTE byStstus[MAX_LEVEL] = {0,};
	int ret = m_pDb->OpenRecord("Select Status From Char_Status",&byStstus,MAX_LEVEL,m_CORUM_GAME_DB[sel]);
	if(ret < 0)
	{
		AfxMessageBox("Stat Initialize Fail!!!",MB_OK|MB_ICONSTOP);
		return;
	}

	sprintf(szQuery,"Select Class,Point,[Level],Ego,Str,[Int],Dex,Vit,LastUsedStatItem From CharacterInfo Where [Name]='%s'",m_szCharName);
	memset(&statinit,0,sizeof(StatInit));
	ret = m_pDb->OpenRecord(szQuery,&statinit,1,m_CORUM_GAME_DB[sel]);
	
	if( ret <= 0 )
	{
		AfxMessageBox("Stat Initialize Fail!!!",MB_OK|MB_ICONSTOP);
		return;
	}

	if(statinit.m_byLastUsedStatItem == 0)
	{
		for(int i = 0; i < statinit.m_dwLevel-1; i++)	
		{
			statinit.m_wPoint += byStstus[i];
		}
	}
	else if(statinit.m_byLastUsedStatItem != 0)
	{
		for(int i = 0; i < statinit.m_dwLevel-1; i++)	
		{
			statinit.m_wPoint += byStstus[i];
		}

		statinit.m_wPoint = statinit.m_wPoint + statinit.m_byLastUsedStatItem - 50;
	}

	switch(statinit.m_wClass)
	{
	case 1:
		{
			statinit.m_dwEgo	= 10;
			statinit.m_dwStr	= 35;
			statinit.m_dwInt	= 10;
			statinit.m_dwDex	= 15;
			statinit.m_dwVit	= 15;				
		}	
		break;
	case 2:
		{   
			statinit.m_dwEgo	= 10;
			statinit.m_dwStr	= 20;
			statinit.m_dwInt	= 20;
			statinit.m_dwDex	= 15;
			statinit.m_dwVit	= 20;				
		}
		break;
	case 3:
		{
			statinit.m_dwEgo	= 15;
			statinit.m_dwStr	= 10;
			statinit.m_dwInt	= 25;
			statinit.m_dwDex	= 10;
			statinit.m_dwVit	= 25;				
		}
		break;
	case 4:
		{
			statinit.m_dwEgo	= 20;
			statinit.m_dwStr	= 10;
			statinit.m_dwInt	= 10;
			statinit.m_dwDex	= 35;
			statinit.m_dwVit	= 10;				
		}
		break;
	case 5:
		{
			statinit.m_dwEgo	= 25;
			statinit.m_dwStr	= 10;
			statinit.m_dwInt	= 35;
			statinit.m_dwDex	= 5;
			statinit.m_dwVit	= 10;			
		}
		break;
	}
	
	int			nColumn			= 9;	// 파라미터 갯수 지정 
	DWORD		dwParamOffset	= 0;	// 파라미터 Binding 정보 구조체에서 각 파라미터 Value들의 Offset
	DBBINDING*	pBinding		= g_pDb->CreateParamInfo(nColumn);		
	if(!pBinding)
	{
		AfxMessageBox("Stat Initialize Fail!!!",MB_OK|MB_ICONSTOP);
		return;
	}
	
	sprintf(szQuery," Update CharacterInfo Set [Class]=?, [Point]=?, [Level]=?, [Ego]=?, "
					" [Str]=?, [Int]=?, [Dex]=?, [Vit]=?, [LastUsedStatItem]=? Where [Name]='%s' ",m_szCharName);
	
	pBinding[0].obValue		= dwParamOffset;	
	pBinding[0].cbMaxLen	= sizeof(statinit.m_wClass);
	pBinding[0].wType		= DBTYPE_I2;
	dwParamOffset			+= sizeof(statinit.m_wClass);				
	
	pBinding[1].obValue		= dwParamOffset;	
	pBinding[1].cbMaxLen	= sizeof(statinit.m_wPoint);
	pBinding[1].wType		= DBTYPE_I2;
	dwParamOffset			+= sizeof(statinit.m_wPoint);		
	
	pBinding[2].obValue		= dwParamOffset;	
	pBinding[2].cbMaxLen	= sizeof(statinit.m_dwLevel);
	pBinding[2].wType		= DBTYPE_I4;
	dwParamOffset			+= sizeof(statinit.m_dwLevel);			
	
	pBinding[3].obValue		= dwParamOffset;	
	pBinding[3].cbMaxLen	= sizeof(statinit.m_dwEgo);
	pBinding[3].wType		= DBTYPE_I4;
	dwParamOffset			+= sizeof(statinit.m_dwEgo);
	
	pBinding[4].obValue		= dwParamOffset;	
	pBinding[4].cbMaxLen	= sizeof(statinit.m_dwStr);
	pBinding[4].wType		= DBTYPE_I4;
	dwParamOffset			+= sizeof(statinit.m_dwStr);
	
	pBinding[5].obValue		= dwParamOffset;	
	pBinding[5].cbMaxLen	= sizeof(statinit.m_dwInt);
	pBinding[5].wType		= DBTYPE_I4;
	dwParamOffset			+= sizeof(statinit.m_dwInt);

	pBinding[6].obValue		= dwParamOffset;	
	pBinding[6].cbMaxLen	= sizeof(statinit.m_dwDex);
	pBinding[6].wType		= DBTYPE_I4;
	dwParamOffset			+= sizeof(statinit.m_dwDex);

	pBinding[7].obValue		= dwParamOffset;	
	pBinding[7].cbMaxLen	= sizeof(statinit.m_dwVit);
	pBinding[7].wType		= DBTYPE_I4;
	dwParamOffset			+= sizeof(statinit.m_dwVit);

	pBinding[8].obValue		= dwParamOffset;	
	pBinding[8].cbMaxLen	= sizeof(statinit.m_byLastUsedStatItem);
	pBinding[8].wType		= DBTYPE_UI1;
	dwParamOffset			+= sizeof(statinit.m_byLastUsedStatItem);
	
	int affected = g_pDb->ExecuteSQLByParam(szQuery,pBinding,&statinit,nColumn,m_CORUM_GAME_DB[sel]);
	g_pDb->ReleaseParamInfo(pBinding);

	if(affected > 0)
	{
		AfxMessageBox("Stat Initialize Success!!!",MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		AfxMessageBox("Stat Initialize Fail!!!",MB_OK|MB_ICONSTOP);
	}
}


BOOL CCopyItemDelete::ResetItemSerialNumber()
{
	char	szQuery[ 0xff ]	= {0,};
	int		nRet			= 0;
	int		sel				= m_ComBoCtrl.GetCurSel();
	CString szServerName;	 
	m_ComBoCtrl.GetLBText(sel,szServerName);
	RemoveNotNumber(szServerName);
		
	BYTE bSerialID = 0;
	sprintf(szQuery,"Select ServerSerialID From ServerTable Where Port = %d",13100);
	nRet = m_pDb->OpenRecord(szQuery,&bSerialID,1,(BYTE)m_CORUM_GAME_DB[sel] );
	
	if(nRet < 0)
	{
		return FALSE;
	}
	
	int nRestartCount = -1;
	sprintf(szQuery,"CZP_RESET_ITEM_SERIAL %d",13100);
	nRet = g_pDb->OpenRecord(szQuery,&nRestartCount,1,(BYTE)m_CORUM_GAME_DB[sel] );
	
	if(nRet < 0)
	{
		return FALSE;
	}
	
	m_Serial.bServerSet			= atoi(szServerName);
	m_Serial.bServerSerialID	= bSerialID;
	m_Serial.wRestartCount		= (WORD)nRestartCount;
	m_Serial.dwSerialNum		= 0;
	
	return TRUE;
}


ITEM_SERIAL	CCopyItemDelete::GetNewSerial()
{
	m_Serial.dwSerialNum++;
	return m_Serial;
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	- 서버 이전 시키면 구서버 CharacterInfo 테이블에 있는 이전 
//					  시킨 캐릭터 딜리트 시키자 
//					  맨마지막에는 CharacterMoveSin 이거도 싹 딜리트 해야지 		
//------------------------------------------------------------------------------
void CCopyItemDelete::CharacterMoveSin()
{
	char				szQuery[0xff]	= {0,};
	char				Name[20]		= {0,};
	DBRECEIVEDATA*		pData			= NULL;
	int					sel				= m_ComBoCtrl.GetCurSel();
	int					ret				= 0;
	DWORD				dwCount			= 0;
	CString				ShowQuery;
	CHARACTER_MOVE		CharacterMove;
	ITEM_CHR_MOVE		ItemChrMove;
	CHAR_MOVE_UPDATE_DB CharMoveUpdateDB;
	GUARDIAN_INFO_MOVE	GuardianInfoMove;

	// 로그파일 생성 (예외처리)
	CString	folder;
	folder.Format("%s\\CharacterMoveSin",g_szExePath);
//	folder.Format("%s\\Characterinfo_MoveLog",g_szExePath);
	
	CFileFind	finder;
	BOOL		bOK = FALSE;
	bOK				= finder.FindFile( folder );
	
	// 폴더가 있으면 
	if( bOK )
	{
		while(bOK)
		{
			bOK = finder.FindNextFile();
			if(!finder.IsDirectory())	
			{
				// 폴더명과 똑같은 파일이 있는데(확장자 없는) 폴더가 아닐경우
				::CreateDirectory( (LPCTSTR)folder,NULL );
				break;
			}
		}		
	}
	// 폴더가 없으면 
	else
	{
		::CreateDirectory( (LPCTSTR)folder,NULL );
	}
	
	CString szServerName;
	m_ComBoCtrl.GetLBText(sel,szServerName);
	
	SYSTEMTIME tm;
	::GetLocalTime(&tm);
	
	CString szFile;
	szFile.Format("%s\\[%s][%d.%d.%d %d.%d.%d].txt",folder,szServerName,tm.wYear,tm.wMonth,tm.wDay,tm.wHour,tm.wMinute,tm.wSecond);
	
	FILE* pFile = NULL;
	pFile = fopen(szFile,"at");
	
	char tszTemp[256] = {0,};
	fprintf(pFile, "*============================================================*\n");
	fprintf(pFile, "\tDate / Time\t\t\t: %s   ", _strdate(tszTemp));
	fprintf(pFile, "%s\n", _strtime(tszTemp));
	fprintf(pFile, "*------------------------------------------------------------*\n");

	// 로그파일 생성 끝 
	
	// 시리얼 리셋 
	ResetItemSerialNumber();
	

	// 경험치 스텟 재분배 제2신서버 
	DWORD	dwExp[256]		= {0,};
	BYTE	byStatus[256]	= {0,};
	
	pData = g_pDb->OpenRecordEx("Select [Exp] From LevelExp",256,DEFAULT_ROWS_PER_READ,(BYTE)m_CORUM_GAME_DB[sel]);
	for(DWORD i = 0; i < pData->Query.select.dwRowCount; i++)
	{
		dwExp[i] = 0;
		memcpy(&dwExp[i],pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize);		
	}
	g_pDb->ReleaseRecordset(pData);
	
	pData = g_pDb->OpenRecordEx("Select [status] From Char_Status", 256, DEFAULT_ROWS_PER_READ, (BYTE)m_CORUM_GAME_DB[sel]);
	
	for(i = 0; i < pData->Query.select.dwRowCount; i++)
	{
		byStatus[i] = 0;
		memcpy(&byStatus[i], pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize);		
	}
	g_pDb->ReleaseRecordset(pData);





	ShowQuery.Format("All Character Move Sin");
	m_pEdit->SetWindowText(ShowQuery);

	sprintf(szQuery,"Select Count(*) From CharacterInfo_MoveLogData");
//	sprintf(szQuery,"Select Count(*) From CharacterMoveSin");
	ret = m_pDb->OpenRecord(szQuery,&dwCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);	
	sprintf(szQuery,"Select * From CharacterInfo_MoveLogData");
//	sprintf(szQuery,"Select * From CharacterMoveSin");		
	pData = m_pDb->OpenRecordEx(szQuery,dwCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);

	if(pData == NULL)
	{
		return;
	}

	for( i = 0; i < pData->Query.select.dwRowCount; i++ )	
	{
		if(!m_bRun)
		{
			m_pDb->ReleaseRecordset(pData);
			return;
		}

		::Sleep(10);				
		
		memset(&CharacterMove,0,sizeof(CHARACTER_MOVE));
		memcpy(&CharacterMove,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize);

		ShowQuery.Format("Q(CharacterInfo):<%d>[%d].[%s]",pData->Query.select.dwRowCount,i+1,CharacterMove.szCharName);
		m_pEdit->SetWindowText(ShowQuery);

		// 현재 에스메랄다,펠트로스에 이 계정으로 케릭터가 꽉 차 있는지 검사를 먼저 한다.
		DWORD dwChar_Index[4] = {0,};
		sprintf(szQuery,"Select [Char_Index] From CharacterInfo Where [ID] = '%s'",CharacterMove.szID);
		ret = m_pDb->OpenRecord(szQuery,dwChar_Index,4,m_CORUM_GAME_DB[sel]);
		
		if(ret >= 4)
		{
			// 현재 제2신서버에 캐릭이 꽉 차있다.
			fprintf(pFile,"[Server:%d][ID:%s][Name:%s]현재 제2신서버에 캐릭이 꽉 차있다.\n",CharacterMove.byServerSet,CharacterMove.szID,CharacterMove.szCharName);
			continue;
		}

		// 1. 이전할 서버(에스메랄드,펠트로스)에 같은 캐릭명이 존재하는지
		sprintf(szQuery,"Select [Name] From CharacterInfo Where [Name] = '%s'",CharacterMove.szCharName);
		memset(Name,0,sizeof(Name));
		ret = m_pDb->OpenRecord(szQuery,Name,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);	
		
		if(ret >= 1)
		{
			// 동일명 캐릭터가 있습니다.
			fprintf(pFile,"[Server:%d][ID:%s][Name:%s]동일명 캐릭터가 있습니다.\n",CharacterMove.byServerSet,CharacterMove.szID,CharacterMove.szCharName);
			continue;
		}

		// 2. 구서버에 이전할 캐릭이 실제로 있는지...
		sprintf(szQuery,"Select [Name] From CharacterInfo Where [ID]='%s' And [Name]='%s'",CharacterMove.szID,CharacterMove.szCharName);
		memset(Name,0,sizeof(Name));
		if(CharacterMove.byServerSet == 1)
			ret = m_pDb->OpenRecord(szQuery,Name,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[0]);
		else if(CharacterMove.byServerSet == 4)
			ret = m_pDb->OpenRecord(szQuery,Name,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[1]);
		else if(CharacterMove.byServerSet == 5)
			ret = m_pDb->OpenRecord(szQuery,Name,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[2]);
		else if(CharacterMove.byServerSet == 8)
			ret = m_pDb->OpenRecord(szQuery,Name,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[3]);
		else if(CharacterMove.byServerSet == 9)
			ret = m_pDb->OpenRecord(szQuery,Name,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[4]);	

		if(ret <= 0)
		{
			fprintf(pFile,"[Server:%d][ID:%s][Name:%s]구서버에 이전해야할 캐릭이 없다.\n",
					CharacterMove.byServerSet,CharacterMove.szID,CharacterMove.szCharName);
			continue;
		}

		// 여기까지 왔으면 서버이전 시켜주자.
		sprintf(szQuery,"Select [Name], Head, Class, Grade, Exp, Honor, Luck, Money, Equip, Inv_large, Inv_small, Belt, Guardian From CharacterInfo Where [ID] = '%s' And [Name] = '%s'",CharacterMove.szID,CharacterMove.szCharName);
		memset(&ItemChrMove,0,sizeof(ITEM_CHR_MOVE));
		if(CharacterMove.byServerSet == 1)
			ret = m_pDb->OpenRecord(szQuery,&ItemChrMove,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[0]);
		else if(CharacterMove.byServerSet == 4)
			ret = m_pDb->OpenRecord(szQuery,&ItemChrMove,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[1]);
		else if(CharacterMove.byServerSet == 5)
			ret = m_pDb->OpenRecord(szQuery,&ItemChrMove,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[2]);
		else if(CharacterMove.byServerSet == 8)
			ret = m_pDb->OpenRecord(szQuery,&ItemChrMove,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[3]);
		else if(CharacterMove.byServerSet == 9)
			ret = m_pDb->OpenRecord(szQuery,&ItemChrMove,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[4]);

		if(ret <= 0)
		{
			fprintf(pFile,"[Server:%d][ID:%s][Name:%s]서버이전하려고 구서버에 캐릭터인포 데이터를 셀렉트 하는데 실패했다..\n",CharacterMove.byServerSet,CharacterMove.szID,CharacterMove.szCharName);
			continue;
		}
		else 
		{
			WORD wChrNum[4] = {0,};
			WORD wChrNumIndex[4] = {0,};
			WORD wCharNum	= 0;
			BOOL bNum		= FALSE;

			sprintf(szQuery,"Select ChrNum From CharacterInfo Where [ID] = '%s' Order by ChrNum",CharacterMove.szID);
			ret = m_pDb->OpenRecord(szQuery,wChrNum,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);
			
			for(int i = 0; i < 4; i++)
			{
				if(wChrNum[i]!=0)
					wChrNumIndex[wChrNum[i]-1] = 1;
			}

			for(i = 0; i < 4; i++)
			{
				if(wChrNumIndex[i]==0)
				{
					wCharNum = i + 1;
					bNum = TRUE;
					break;
				}
			}

			if(bNum)
			{
				char szIp[16] = {0,};
				sprintf(szIp,"203.238.38.215");
				sprintf(szQuery,"CZP_CREATE_NEW_CHARACTER '%s','%s',%d,%d,%d,%d,'%s'",CharacterMove.szID,CharacterMove.szCharName,wCharNum,ItemChrMove.wHead,ItemChrMove.wClass,1,szIp);

				CREATE_CHARACTER_RESULT Result;	
				memset(&Result,0,sizeof(CREATE_CHARACTER_RESULT));
				ret = m_pDb->OpenRecord(szQuery,&Result,1,m_CORUM_GAME_DB[sel]);
				
				switch(Result.dwResult)
				{
					// 캐릭터 추가 성공
					case 1:
					{
						memset(&CharMoveUpdateDB,0,sizeof(CHAR_MOVE_UPDATE_DB));

						// Data Convet //												
						for(i = 0; i < MAX_EQUIP; i++)
						{
							if(ItemChrMove.cEquip[i].GetID() != 0)
							{
								WORD wOldID = ItemChrMove.cEquip[i].GetID();
								WORD wNewID = ( wOldID / 100 * 200 ) + wOldID % 100;
								ItemChrMove.cEquip[i].m_wItemID = wNewID;

								CharMoveUpdateDB.cEquip[i].m_wItemID	= ItemChrMove.cEquip[i].m_wItemID;
								CharMoveUpdateDB.cEquip[i].m_Serial		= GetNewSerial();

								memcpy(&CharMoveUpdateDB.cEquip[i].m_ItemWeapon, &ItemChrMove.cEquip[i].m_ItemWeapon, sizeof(ItemChrMove.cEquip[i].m_ItemWeapon));
								memcpy(&CharMoveUpdateDB.cEquip[i].m_bSlot, &ItemChrMove.cEquip[i].m_bSlot, sizeof(ITEM_SLOT) + (sizeof(ITEM_ATTR) * MAX_ITEM_ATTR));	

								if(CharMoveUpdateDB.cEquip[i].GetType() == ITEM_LARGE)
								{
									for(int nAttr = 0; nAttr < MAX_ITEM_ATTR; nAttr++)	
									{
										if(	CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode	== 34 &&
											(CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue >= 376 &&
											CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue	<= 500))
										{
											CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode	= 23;
											CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue	= 1;																
										}
										else if(CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode	== 35	&&
												(CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue >= 1	&&
												CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue	<=	450))
										{
											CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode	= 23;
											CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue	= 1;															
										}
										else if(CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode	== 38 &&
												(CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue	>= 76 &&
												CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue	<= 125))
										{
											CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode	= 0;
											CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue	= 0;																
										}
										else if(CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode	== 39	&&
												(CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue	>= 276	&&
												CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue	<=325))
										{
											CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode	= 0;
											CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue	= 0;																
										}
										else if(CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode==44 &&
												(CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue>=251 &&
												CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue<=300))
										{
											CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode	= 0;
											CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue	= 0;
										}
									}	

									int nValue = CharMoveUpdateDB.cEquip[i].m_wItemID/ITEM_DISTRIBUTE;

									if(nValue>=ITEM_ARMORST_INDEX && nValue<=ITEM_ARMOREND_INDEX)
									{
										WORD wValue = 0;
										
										for(int nAttr = 0; nAttr < MAX_ITEM_ATTR; nAttr++ )	
										{
											if(	CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode == ITEM_ATTR_FIRERESIST		||
												CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode == ITEM_ATTR_LIGHTRESIST		||
												CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode == ITEM_ATTR_ICERESIST		||
												CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode == ITEM_ATTR_POIRESIST		||
												CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode == ITEM_ATTR_ALLRESIST		||
												CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode == ITEM_ATTR_PHYRESIST)
											{
												wValue += CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue;
												CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode	= 0;
												CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue	= 0;																		
											}
										}
										CharMoveUpdateDB.cEquip[i].m_Item_Armor.wDefense += wValue;						
									}
								}					
							}
						}

						for(i = 0; i < MAX_INV_LARGE; i++)
						{
							if(ItemChrMove.cInvLarge[i].GetID() != 0)
							{
								WORD wOldID = ItemChrMove.cInvLarge[i].GetID();
								WORD wNewID = ( wOldID / 100 * 200 ) + wOldID % 100;
								ItemChrMove.cInvLarge[i].m_wItemID = wNewID;

								CharMoveUpdateDB.cInvLarge[i].m_wItemID = ItemChrMove.cInvLarge[i].m_wItemID;
								
								if(ItemChrMove.cInvLarge[i].GetID() / ITEM_DISTRIBUTE != ITEM_GUARDIAN_INDEX)
									CharMoveUpdateDB.cInvLarge[i].m_Serial = GetNewSerial();
								else
									CharMoveUpdateDB.cInvLarge[i].m_Serial = ItemChrMove.cInvLarge[i].m_Serial;							

								memcpy(&CharMoveUpdateDB.cInvLarge[i].m_ItemWeapon, &ItemChrMove.cInvLarge[i].m_ItemWeapon, sizeof(ItemChrMove.cInvLarge[i].m_ItemWeapon));
								memcpy(&CharMoveUpdateDB.cInvLarge[i].m_bSlot, &ItemChrMove.cInvLarge[i].m_bSlot, sizeof(ITEM_SLOT) + (sizeof(ITEM_ATTR) * MAX_ITEM_ATTR));	

								if(CharMoveUpdateDB.cInvLarge[i].GetType()==ITEM_LARGE)
								{
									for(int nAttr = 0; nAttr < MAX_ITEM_ATTR; nAttr++)	
									{
										if(	CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode==34 &&
											(CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue>=376 &&
											CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue<=500))
										{
											CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode	= 23;
											CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue	= 1;																
										}
										else if(CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode==35 &&
												(CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue>=1 &&
												CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue<=450))
										{
											CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode	= 23;
											CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue	= 1;															
										}
										else if(CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode==38 &&
												(CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue>=76 &&
												CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue<=125))
										{
											CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode	= 0;
											CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue	= 0;																
										}
										else if(CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode==39 &&
												(CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue>=276 &&
												CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue<=325))
										{
											CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode	= 0;
											CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue	= 0;																
										}
										else if(CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode==44 &&
												(CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue>=251 &&
												CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue<=300))
										{
											CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode	= 0;
											CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue	= 0;
										}
									}	
									
									int nValue = CharMoveUpdateDB.cInvLarge[i].m_wItemID/ITEM_DISTRIBUTE;

									if(nValue>=ITEM_ARMORST_INDEX && nValue<=ITEM_ARMOREND_INDEX)
									{
										WORD wValue = 0;
										
										for(int nAttr = 0; nAttr < MAX_ITEM_ATTR; nAttr++ )	
										{
											if(	CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode == ITEM_ATTR_FIRERESIST	||
												CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode == ITEM_ATTR_LIGHTRESIST	||
												CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode == ITEM_ATTR_ICERESIST		||
												CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode == ITEM_ATTR_POIRESIST		||
												CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode == ITEM_ATTR_ALLRESIST		||
												CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode == ITEM_ATTR_PHYRESIST)
											{
												wValue += CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue;
												CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode	= 0;
												CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue	= 0;																		
											}
										}
										CharMoveUpdateDB.cInvLarge[i].m_Item_Armor.wDefense += wValue;						
									}						
								}					
							}
						}

						for(i = 0; i < MAX_INV_SMALL; i++)
						{
							if(ItemChrMove.cInvSmall[i].GetID() != 0)
							{
								WORD wOldID = ItemChrMove.cInvSmall[i].GetID();
								WORD wNewID = ( wOldID / 100 * 200 ) + wOldID % 100;
								ItemChrMove.cInvSmall[i].m_wItemID = wNewID;

								if( (wNewID>=10000 && wNewID<=10004)	|| (wNewID>=8000 && wNewID<=8205)   ||
									(wNewID>=13001 && wNewID<=13038)	|| (wNewID>=14000 && wNewID<=14405) || 
									(wNewID>=6200 && wNewID<=6604)		|| (wNewID>=18001 && wNewID<=18026)	||
									(wNewID>=16001 && wNewID<=16065)	|| (wNewID==12200))
								{
									memset(&CharMoveUpdateDB.cInvSmall[i],0,sizeof(CItem));
								}
								else
								{
									CharMoveUpdateDB.cInvSmall[i].m_wItemID	= ItemChrMove.cInvSmall[i].m_wItemID;
									CharMoveUpdateDB.cInvSmall[i].m_Serial	= GetNewSerial();

									memcpy(&CharMoveUpdateDB.cInvSmall[i].m_ItemWeapon, &ItemChrMove.cInvSmall[i].m_ItemWeapon, sizeof(ItemChrMove.cInvSmall[i].m_ItemWeapon));
									memcpy(&CharMoveUpdateDB.cInvSmall[i].m_bSlot, &ItemChrMove.cInvSmall[i].m_bSlot, sizeof(ITEM_SLOT) + (sizeof(ITEM_ATTR) * MAX_ITEM_ATTR));	
								}					
							}
						}

						for(i = 0; i < MAX_BELT; i++)
						{
							if(ItemChrMove.cBelt[i].GetID() != 0)
							{
								WORD wOldID = ItemChrMove.cBelt[i].GetID();
								WORD wNewID = ( wOldID / 100 * 200 ) + wOldID % 100;
								ItemChrMove.cBelt[i].m_wItemID = wNewID;

								if( (wNewID>=10000 && wNewID<=10004)	|| (wNewID>=8000 && wNewID<=8205)   ||
									(wNewID>=13001 && wNewID<=13038)	|| (wNewID>=14000 && wNewID<=14405) || 
									(wNewID>=6200 && wNewID<=6604)		|| (wNewID>=18001 && wNewID<=18026)	||
									(wNewID>=16001 && wNewID<=16065)	|| (wNewID==12200))
								{
									memset(&CharMoveUpdateDB.cBelt[i], 0, sizeof(CItem));
								}
								else
								{
									CharMoveUpdateDB.cBelt[i].m_wItemID	= ItemChrMove.cBelt[i].m_wItemID;
									CharMoveUpdateDB.cBelt[i].m_Serial	= GetNewSerial();

									memcpy(&CharMoveUpdateDB.cBelt[i].m_ItemWeapon, &ItemChrMove.cBelt[i].m_ItemWeapon, sizeof(ItemChrMove.cBelt[i].m_ItemWeapon));
									memcpy(&CharMoveUpdateDB.cBelt[i].m_bSlot, &ItemChrMove.cBelt[i].m_bSlot, sizeof(ITEM_SLOT) + (sizeof(ITEM_ATTR) * MAX_ITEM_ATTR));	
								}					
							}
						}

						for(i = 0; i < MAX_INV_GUARDIAN; i++)
						{
							if(ItemChrMove.cGuardian[i].GetID() != 0)
							{
								WORD wOldID = ItemChrMove.cGuardian[i].GetID();
								WORD wNewID = ( wOldID / 100 * 200 ) + wOldID % 100;
								ItemChrMove.cGuardian[i].m_wItemID = wNewID;

								CharMoveUpdateDB.cGuardian[i].m_wItemID	= ItemChrMove.cGuardian[i].m_wItemID;
							
								if(CharMoveUpdateDB.cGuardian[i].GetID()/ITEM_DISTRIBUTE != ITEM_GUARDIAN_INDEX)
									CharMoveUpdateDB.cGuardian[i].m_Serial	= GetNewSerial();
								else
									CharMoveUpdateDB.cGuardian[i].m_Serial	= ItemChrMove.cGuardian[i].m_Serial;

								memcpy(&CharMoveUpdateDB.cGuardian[i].m_ItemWeapon, &ItemChrMove.cGuardian[i].m_ItemWeapon, sizeof(ItemChrMove.cGuardian[i].m_ItemWeapon));
								memcpy(&CharMoveUpdateDB.cGuardian[i].m_bSlot, &ItemChrMove.cGuardian[i].m_bSlot, sizeof(ITEM_SLOT) + (sizeof(ITEM_ATTR) * MAX_ITEM_ATTR));	
							}
						}

						CharMoveUpdateDB.dwExp		= ItemChrMove.dwExp;			
						CharMoveUpdateDB.dwHonor	= ItemChrMove.dwHonor;
						CharMoveUpdateDB.dwLuck		= ItemChrMove.dwLuck;
						CharMoveUpdateDB.dwMoney	= ItemChrMove.dwMoney / 100;
						CharMoveUpdateDB.wPoint		= 0;

						for(i = 0; i < 256; i++)
						{
							if(dwExp[i] > CharMoveUpdateDB.dwExp)
							{
								CharMoveUpdateDB.wPoint_Skill	= i;
								CharMoveUpdateDB.dwLevel		= i;
																					
								for(DWORD j = 0; j < CharMoveUpdateDB.dwLevel-1; j++)
								{
									CharMoveUpdateDB.wPoint += byStatus[j];													
								}

								break;
							}
						}
													
						// CharacterInfo Update //
						DBBINDING*	pBinding;
						int			nColumn			= 12;
						DWORD		dwParamOffset	= 0;												
						pBinding = g_pDb->CreateParamInfo(nColumn);
						if(!pBinding) continue;			

						sprintf(szQuery, "Update CharacterInfo Set Point=?, Point_Skill=?, Exp=?, [Level]=?, Honor=?, Luck=?, Money=?, Equip=?, Inv_large=?, Inv_small=?, Belt=?, Guardian=? Where Name = '%s'",CharacterMove.szCharName);
										
						pBinding[0].obValue		= dwParamOffset;	
						pBinding[0].cbMaxLen	= sizeof(CharMoveUpdateDB.wPoint);
						pBinding[0].wType		= DBTYPE_I2;					
						dwParamOffset			+= sizeof(CharMoveUpdateDB.wPoint);

						pBinding[1].obValue		= dwParamOffset;	
						pBinding[1].cbMaxLen	= sizeof(CharMoveUpdateDB.wPoint_Skill);
						pBinding[1].wType		= DBTYPE_I2;					
						dwParamOffset			+= sizeof(CharMoveUpdateDB.wPoint_Skill);

						pBinding[2].obValue		= dwParamOffset;	
						pBinding[2].cbMaxLen	= sizeof(CharMoveUpdateDB.dwExp);
						pBinding[2].wType		= DBTYPE_I4;					
						dwParamOffset			+= sizeof(CharMoveUpdateDB.dwExp);

						pBinding[3].obValue		= dwParamOffset;	
						pBinding[3].cbMaxLen	= sizeof(CharMoveUpdateDB.dwLevel);
						pBinding[3].wType		= DBTYPE_I4;					
						dwParamOffset			+= sizeof(CharMoveUpdateDB.dwLevel);

						pBinding[4].obValue		= dwParamOffset;	
						pBinding[4].cbMaxLen	= sizeof(CharMoveUpdateDB.dwHonor);
						pBinding[4].wType		= DBTYPE_I4;					
						dwParamOffset			+= sizeof(CharMoveUpdateDB.dwHonor);

						pBinding[5].obValue		= dwParamOffset;	
						pBinding[5].cbMaxLen	= sizeof(CharMoveUpdateDB.dwLuck);
						pBinding[5].wType		= DBTYPE_I4;					
						dwParamOffset			+= sizeof(CharMoveUpdateDB.dwLuck);

						pBinding[6].obValue		= dwParamOffset;	
						pBinding[6].cbMaxLen	= sizeof(CharMoveUpdateDB.dwMoney);
						pBinding[6].wType		= DBTYPE_I4;					
						dwParamOffset			+= sizeof(CharMoveUpdateDB.dwMoney);

						pBinding[7].obValue		= dwParamOffset;	
						pBinding[7].cbMaxLen	= sizeof(CharMoveUpdateDB.cEquip);
						pBinding[7].wType		= DBTYPE_BYTES;					
						dwParamOffset			+= sizeof(CharMoveUpdateDB.cEquip);
						
						pBinding[8].obValue		= dwParamOffset;	
						pBinding[8].cbMaxLen	= sizeof(CharMoveUpdateDB.cInvLarge);
						pBinding[8].wType		= DBTYPE_BYTES;					
						dwParamOffset			+= sizeof(CharMoveUpdateDB.cInvLarge);
						
						pBinding[9].obValue		= dwParamOffset;	
						pBinding[9].cbMaxLen	= sizeof(CharMoveUpdateDB.cInvSmall);
						pBinding[9].wType		= DBTYPE_BYTES;					
						dwParamOffset			+= sizeof(CharMoveUpdateDB.cInvSmall);
						
						pBinding[10].obValue	= dwParamOffset;	
						pBinding[10].cbMaxLen	= sizeof(CharMoveUpdateDB.cBelt);
						pBinding[10].wType		= DBTYPE_BYTES;					
						dwParamOffset			+= sizeof(CharMoveUpdateDB.cBelt);
						
						pBinding[11].obValue	= dwParamOffset;	
						pBinding[11].cbMaxLen	= sizeof(CharMoveUpdateDB.cGuardian);
						pBinding[11].wType		= DBTYPE_BYTES;					
						dwParamOffset			+= sizeof(CharMoveUpdateDB.cGuardian);
						
						g_pDb->ExecuteSQLByParam(szQuery,pBinding,&CharMoveUpdateDB,nColumn,m_CORUM_GAME_DB[sel]);	
					
						// 가디언 
						for(i = 0; i < MAX_INV_LARGE; i++)
						{
							if(CharMoveUpdateDB.cInvLarge[i].GetID()/ITEM_DISTRIBUTE==ITEM_GUARDIAN_INDEX)
							{
								if(CharMoveUpdateDB.cInvLarge[i].m_Item_Guardian.bCreate == 0)
									continue;
								
								sprintf(szQuery, "Select Name, BaseIndex, [Level], Exp, HP, MP, ServerSerialID, RestartCount, SerialNum From GuardianInfoConvert Where ServerSet = %u And ServerSerialID = %u And RestartCount = %u And SerialNum = %u",
										CharMoveUpdateDB.cInvLarge[i].m_Serial.bServerSet, CharMoveUpdateDB.cInvLarge[i].m_Serial.bServerSerialID, CharMoveUpdateDB.cInvLarge[i].m_Serial.wRestartCount, CharMoveUpdateDB.cInvLarge[i].m_Serial.dwSerialNum);
//								sprintf(szQuery, "Select Name, BaseIndex, [Level], Exp, HP, MP, ServerSerialID, RestartCount, SerialNum From GuardianInfoConvertTest Where ServerSet = %u And ServerSerialID = %u And RestartCount = %u And SerialNum = %u",
//										CharMoveUpdateDB.cInvLarge[i].m_Serial.bServerSet, CharMoveUpdateDB.cInvLarge[i].m_Serial.bServerSerialID, CharMoveUpdateDB.cInvLarge[i].m_Serial.wRestartCount, CharMoveUpdateDB.cInvLarge[i].m_Serial.dwSerialNum);
								memset(&GuardianInfoMove,0,sizeof(GUARDIAN_INFO_MOVE));
								ret = g_pDb->OpenRecord(szQuery,&GuardianInfoMove,1,(BYTE)m_CORUM_GAME_DB[sel]);
								
								if(ret <= 0)
								{
									// 구서버에서 생성한 가디언인포가 없다.
									fprintf(pFile,"[Server:%d][ID:%s][Name:%s]구서버에서 생성한 가디언인포가 없다.\n",
											CharacterMove.byServerSet,CharacterMove.szID,CharacterMove.szCharName);
									continue;
								}
								else
								{
									MY_DB_NUMERIC Result;
									memset(&Result,0,sizeof(MY_DB_NUMERIC));
									sprintf(szQuery,"CZP_MOVE_GUARDIAN_INFO '%s', %u, %u, %u, %u, %u",GuardianInfoMove.szName,GuardianInfoMove.dwBaseIndex,GuardianInfoMove.dwLevel,GuardianInfoMove.dwExp,GuardianInfoMove.dwHP,GuardianInfoMove.dwMP);
									ret = m_pDb->OpenRecord(szQuery,&Result,1,m_CORUM_GAME_DB[sel]);
									
									CharMoveUpdateDB.cInvLarge[i].m_Item_Guardian.bCreate	= 1;
									CharMoveUpdateDB.cInvLarge[i].m_Item_Guardian.dwID		= Result.least;

									sprintf(szQuery,"Delete From GuardianInfoConvert Where ServerSet = %u And ServerSerialID = %u And RestartCount = %u And SerialNum = %u",
											CharMoveUpdateDB.cInvLarge[i].m_Serial.bServerSet, CharMoveUpdateDB.cInvLarge[i].m_Serial.bServerSerialID, CharMoveUpdateDB.cInvLarge[i].m_Serial.wRestartCount, CharMoveUpdateDB.cInvLarge[i].m_Serial.dwSerialNum);
//									sprintf(szQuery,"Delete From GuardianInfoConvertTest Where ServerSet = %u And ServerSerialID = %u And RestartCount = %u And SerialNum = %u",
//										CharMoveUpdateDB.cInvLarge[i].m_Serial.bServerSet, CharMoveUpdateDB.cInvLarge[i].m_Serial.bServerSerialID, CharMoveUpdateDB.cInvLarge[i].m_Serial.wRestartCount, CharMoveUpdateDB.cInvLarge[i].m_Serial.dwSerialNum);
									m_pDb->ExecuteSQL(szQuery,(BYTE)m_CORUM_GAME_DB[sel]);

									CharMoveUpdateDB.cInvLarge[i].m_Serial = GetNewSerial();

									DBBINDING*	pBinding;
									int			nColumn			= 1;
									DWORD		dwParamOffset	= 0;
									pBinding					= m_pDb->CreateParamInfo(nColumn);
									if(!pBinding)	continue;
									
									sprintf(szQuery, "Update CharacterInfo Set Inv_large=? Where Name = '%s'", CharacterMove.szCharName);
									
									INV_UPDATE_DB sInvUpdateDB;
									memset(&sInvUpdateDB,0,sizeof(sInvUpdateDB));
									memcpy(sInvUpdateDB.cInvLarge,CharMoveUpdateDB.cInvLarge,sizeof(CharMoveUpdateDB.cInvLarge));
									
									pBinding[0].obValue		= dwParamOffset;	
									pBinding[0].cbMaxLen	= sizeof(sInvUpdateDB.cInvLarge);
									pBinding[0].wType		= DBTYPE_BYTES;					
									dwParamOffset			+= sizeof(sInvUpdateDB.cInvLarge);
									
									m_pDb->ExecuteSQLByParam(szQuery,pBinding,&sInvUpdateDB,nColumn,(BYTE)m_CORUM_GAME_DB[sel]);
								}
							}
						}
						
						for(i = 0; i < MAX_INV_GUARDIAN; i++)
						{
							if(CharMoveUpdateDB.cGuardian[i].GetID()/ITEM_DISTRIBUTE==ITEM_GUARDIAN_INDEX)
							{					
								if(CharMoveUpdateDB.cGuardian[i].m_Item_Guardian.bCreate == 0)
									continue;

								sprintf(szQuery, "Select Name, BaseIndex, [Level], Exp, HP, MP, ServerSerialID, RestartCount, SerialNum From GuardianInfoConvert Where ServerSet = %u And ServerSerialID = %u And RestartCount = %u And SerialNum = %u",
										CharMoveUpdateDB.cGuardian[i].m_Serial.bServerSet, CharMoveUpdateDB.cGuardian[i].m_Serial.bServerSerialID, CharMoveUpdateDB.cGuardian[i].m_Serial.wRestartCount, CharMoveUpdateDB.cGuardian[i].m_Serial.dwSerialNum);
//								sprintf(szQuery, "Select Name, BaseIndex, [Level], Exp, HP, MP, ServerSerialID, RestartCount, SerialNum From GuardianInfoConvertTest Where ServerSet = %u And ServerSerialID = %u And RestartCount = %u And SerialNum = %u",
//										CharMoveUpdateDB.cGuardian[i].m_Serial.bServerSet, CharMoveUpdateDB.cGuardian[i].m_Serial.bServerSerialID, CharMoveUpdateDB.cGuardian[i].m_Serial.wRestartCount, CharMoveUpdateDB.cGuardian[i].m_Serial.dwSerialNum);
								memset(&GuardianInfoMove,0,sizeof(GUARDIAN_INFO_MOVE));
								ret = g_pDb->OpenRecord(szQuery,&GuardianInfoMove,1,(BYTE)m_CORUM_GAME_DB[sel]);

								if(ret <= 0)
								{
									// 구서버에서 생성한 가디언인포가 없다.
									fprintf(pFile,"[Server:%d][ID:%s][Name:%s]구서버에서 생성한 가디언인포가 없다.\n",
											CharacterMove.byServerSet,CharacterMove.szID,CharacterMove.szCharName);
									continue;
								}
								else
								{
									MY_DB_NUMERIC Result;
									memset(&Result,0,sizeof(MY_DB_NUMERIC));
									sprintf(szQuery,"CZP_MOVE_GUARDIAN_INFO '%s', %u, %u, %u, %u, %u",GuardianInfoMove.szName,GuardianInfoMove.dwBaseIndex,GuardianInfoMove.dwLevel,GuardianInfoMove.dwExp,GuardianInfoMove.dwHP,GuardianInfoMove.dwMP);
									ret = m_pDb->OpenRecord(szQuery,&Result,1,m_CORUM_GAME_DB[sel]);
									
									CharMoveUpdateDB.cGuardian[i].m_Item_Guardian.bCreate	= 1;
									CharMoveUpdateDB.cGuardian[i].m_Item_Guardian.dwID		= Result.least;
									
									sprintf(szQuery,"Delete From GuardianInfoConvert Where ServerSet = %u And ServerSerialID = %u And RestartCount = %u And SerialNum = %u",
											CharMoveUpdateDB.cGuardian[i].m_Serial.bServerSet, CharMoveUpdateDB.cGuardian[i].m_Serial.bServerSerialID, CharMoveUpdateDB.cGuardian[i].m_Serial.wRestartCount, CharMoveUpdateDB.cGuardian[i].m_Serial.dwSerialNum);
//									sprintf(szQuery,"Delete From GuardianInfoConvertTest Where ServerSet = %u And ServerSerialID = %u And RestartCount = %u And SerialNum = %u",
//											CharMoveUpdateDB.cGuardian[i].m_Serial.bServerSet, CharMoveUpdateDB.cGuardian[i].m_Serial.bServerSerialID, CharMoveUpdateDB.cGuardian[i].m_Serial.wRestartCount, CharMoveUpdateDB.cGuardian[i].m_Serial.dwSerialNum);
									m_pDb->ExecuteSQL(szQuery,(BYTE)m_CORUM_GAME_DB[sel]);
									
									CharMoveUpdateDB.cGuardian[i].m_Serial = GetNewSerial();
									
									DBBINDING*	pBinding;
									int			nColumn			= 1;
									DWORD		dwParamOffset	= 0;
									pBinding					= m_pDb->CreateParamInfo(nColumn);
									if(!pBinding)	continue;
									
									sprintf(szQuery, "Update CharacterInfo Set Guardian=? Where Name = '%s'", CharacterMove.szCharName);
									
									GUARDIAN_UPDATE_DB sGuardianUpdateDB;
									memset(&sGuardianUpdateDB,0,sizeof(sGuardianUpdateDB));
									memcpy(sGuardianUpdateDB.cGuardian,CharMoveUpdateDB.cGuardian,sizeof(CharMoveUpdateDB.cGuardian));
									
									pBinding[0].obValue		= dwParamOffset;	
									pBinding[0].cbMaxLen	= sizeof(sGuardianUpdateDB.cGuardian);
									pBinding[0].wType		= DBTYPE_BYTES;					
									dwParamOffset			+= sizeof(sGuardianUpdateDB.cGuardian);
									
									m_pDb->ExecuteSQLByParam(szQuery,pBinding,&sGuardianUpdateDB,nColumn,(BYTE)m_CORUM_GAME_DB[sel]);
								}
							}

							// 자..다 되었다. 구서버에 있는 캐릭터를 지우자.
							char szIp[16] = {0,};
							sprintf(szIp,"203.238.38.215");
							sprintf(szQuery,"CZP_DELETE_CHARACTER '%s','%s','%s'",CharacterMove.szID,CharacterMove.szCharName,szIp);							
							DELETE_CHARACTER_RESULT DeleteResult;	
							memset(&DeleteResult,0,sizeof(CREATE_CHARACTER_RESULT));

							if(CharacterMove.byServerSet == 1)
								ret = m_pDb->OpenRecord(szQuery,&DeleteResult,1,m_CORUM_GAME_DB[0]);							
							else if(CharacterMove.byServerSet == 4)
								ret = m_pDb->OpenRecord(szQuery,&DeleteResult,1,m_CORUM_GAME_DB[1]);							
							else if(CharacterMove.byServerSet == 5)
								ret = m_pDb->OpenRecord(szQuery,&DeleteResult,1,m_CORUM_GAME_DB[2]);							
							else if(CharacterMove.byServerSet == 8)
								ret = m_pDb->OpenRecord(szQuery,&DeleteResult,1,m_CORUM_GAME_DB[3]);							
							else if(CharacterMove.byServerSet == 9)
								ret = m_pDb->OpenRecord(szQuery,&DeleteResult,1,m_CORUM_GAME_DB[4]);
							
							switch(DeleteResult.dwResult)
							{
								// 캐릭터 삭제 성공
								case 1: break;
								// 모두다 지워진 경우
								case 2: break;
								// 잘못된 캐릭터 삭제 시도 
								case 4: break;								
							}
						}
					}
					break;

					// 이미 있는 캐릭터 
					case -1:
					{
						fprintf(pFile,"[Server:%d][ID:%s][Name:%s]뉴캐릭터했더니 이미 있는 캐릭터 이거나 삭제 대기중인 캐릭터 이다.\n",
								CharacterMove.byServerSet,CharacterMove.szID,CharacterMove.szCharName);
						continue;		
					}
					break;

					// 자리 중복. 
					case -2:
					{
						fprintf(pFile,"[Server:%d][ID:%s][Name:%s]뉴캐릭터했는데 자리 중복이다.\n",
								CharacterMove.byServerSet,CharacterMove.szID,CharacterMove.szCharName);
						continue;		
					}
					break;	
				}
			}	
		}
	}	
	
	m_pDb->ReleaseRecordset(pData);
	pData = NULL;

	sprintf(szQuery,"Delete From Characterinfo_MoveLog");
//	sprintf(szQuery,"Delete From CharacterMoveSin");
	m_pDb->ExecuteSQL(szQuery,(BYTE)m_CORUM_GAME_DB[sel]);

	fprintf(pFile,"\n");
	fclose(pFile);

	// 다 끝났다.
	AfxMessageBox("Character Move Sin End!!!",MB_OK|MB_ICONINFORMATION);
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CCopyItemDelete::CharacterMove()
{
	char				szQuery[0xff]	= {0,};
	char				Name[20]		= {0,};
	DBRECEIVEDATA*		pData			= NULL;
	int					sel				= m_ComBoCtrl.GetCurSel();
	int					ret				= 0;
	DWORD				dwCount			= 0;
	CString				ShowQuery;
	CHARACTER_MOVE		CharacterMove;
	ITEM_CHR_MOVE		ItemChrMove;
	CHAR_MOVE_UPDATE_DB CharMoveUpdateDB;
	GUARDIAN_INFO_MOVE	GuardianInfoMove;
	
	CString	folder;
	folder.Format("%s\\CharacterMove",g_szExePath);
	
	CFileFind	finder;
	BOOL		bOK = FALSE;
	bOK				= finder.FindFile( folder );
	
	// 폴더가 있으면 
	if( bOK )
	{
		while(bOK)
		{
			bOK = finder.FindNextFile();
			if(!finder.IsDirectory())	
			{
				// 폴더명과 똑같은 파일이 있는데(확장자 없는) 폴더가 아닐경우
				::CreateDirectory( (LPCTSTR)folder,NULL );
				break;
			}
		}		
	}
	// 폴더가 없으면 
	else
	{
		::CreateDirectory( (LPCTSTR)folder,NULL );
	}
	
	CString szServerName;
	m_ComBoCtrl.GetLBText(sel,szServerName);
	
	SYSTEMTIME tm;
	::GetLocalTime(&tm);
	
	CString szFile;
	szFile.Format("%s\\[%s][%d.%d.%d %d.%d.%d].txt",folder,szServerName,tm.wYear,tm.wMonth,tm.wDay,tm.wHour,tm.wMinute,tm.wSecond);
	
	FILE* pFile = NULL;
	pFile = fopen(szFile,"at");
	
	char tszTemp[256] = {0,};
	fprintf(pFile, "*============================================================*\n");
	fprintf(pFile, "\tDate / Time\t\t\t: %s   ", _strdate(tszTemp));
	fprintf(pFile, "%s\n", _strtime(tszTemp));
	fprintf(pFile, "*------------------------------------------------------------*\n");

	ResetItemSerialNumber();
	
	DWORD	dwExp[256]		= {0,};
	BYTE	byStatus[256]	= {0,};
	
	pData = g_pDb->OpenRecordEx("Select [Exp] From LevelExp",256,DEFAULT_ROWS_PER_READ,(BYTE)m_CORUM_GAME_DB[sel]);
	for(DWORD i = 0; i < pData->Query.select.dwRowCount; i++)
	{
		dwExp[i] = 0;
		memcpy(&dwExp[i],pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize);		
	}
	g_pDb->ReleaseRecordset(pData);
	
	pData = g_pDb->OpenRecordEx("Select [status] From Char_Status", 256, DEFAULT_ROWS_PER_READ, (BYTE)m_CORUM_GAME_DB[sel]);
	
	for(i = 0; i < pData->Query.select.dwRowCount; i++)
	{
		byStatus[i] = 0;
		memcpy(&byStatus[i], pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize);		
	}
	g_pDb->ReleaseRecordset(pData);

	ShowQuery.Format("All Character Move");
	m_pEdit->SetWindowText(ShowQuery);
	sprintf(szQuery,"Select Count(*) From CharacterMove");
	ret = m_pDb->OpenRecord(szQuery,&dwCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);		
	sprintf(szQuery,"Select * From CharacterMove");		
	pData = m_pDb->OpenRecordEx(szQuery,dwCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);

	if(pData == NULL)
	{
		return;
	}		

	for( i = 0; i < pData->Query.select.dwRowCount; i++ )	
	{
		if(!m_bRun)
		{
			m_pDb->ReleaseRecordset(pData);
			return;
		}
		
		::Sleep(10);				
		
		memset(&CharacterMove,0,sizeof(CHARACTER_MOVE));
		memcpy(&CharacterMove,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize);

		ShowQuery.Format("Q(CharacterInfo):<%d>[%d].[%s]",pData->Query.select.dwRowCount,i+1,CharacterMove.szCharName);
		m_pEdit->SetWindowText(ShowQuery);

		// 현재 에스메랄다,펠트로스에 이 계정으로 케릭터가 꽉 차 있는지 검사를 먼저 한다.
		DWORD dwChar_Index[4] = {0,};
		sprintf(szQuery,"Select [Char_Index] From CharacterInfo Where [ID] = '%s'",CharacterMove.szID);
		ret = m_pDb->OpenRecord(szQuery,dwChar_Index,4,m_CORUM_GAME_DB[sel]);
		
		if(ret >= 4)
		{
			// 현재 제2신서버에 캐릭이 꽉 차있다.
			fprintf(pFile,"[Server:%d][ID:%s][Name:%s]현재 제2신서버에 캐릭이 꽉 차있다.\n",CharacterMove.byServerSet,CharacterMove.szID,CharacterMove.szCharName);
			continue;
		}

		// 1. CharacterInfo_Convert 테이블에 있는지 확인 
		sprintf(szQuery,"Select [Name] From CharacterInfo_Convert Where [ID] = '%s' And [Name] = '%s'",CharacterMove.szID,CharacterMove.szCharName);
		memset(Name,0,sizeof(Name));
		ret = m_pDb->OpenRecord(szQuery,Name,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);

		if(ret >= 1)
		{
			// 유저가 게임내에서 캐릭터를 받지를 않았다.
			fprintf(pFile,"[Server:%d][ID:%s][Name:%s]유저가 게임내에서 캐릭터를 받지를 않았다.\n",CharacterMove.byServerSet,CharacterMove.szID,CharacterMove.szCharName);
			continue;
		}

		// 2. CharacterInfo 테이블에 있는지 확인
		sprintf(szQuery,"Select [Name] From CharacterInfo Where [ID] = '%s' And [Name] = '%s'",CharacterMove.szID,CharacterMove.szCharName);
		memset(Name,0,sizeof(Name));
		ret = m_pDb->OpenRecord(szQuery,Name,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);	

		if(ret >= 1)
		{
			// 캐릭이 이전되어 있는데 안됬다고 우기는 거다.
			fprintf(pFile,"[Server:%d][ID:%s][Name:%s]캐릭이 이전되어 있는데 안됬다고 우기는 거다.\n",CharacterMove.byServerSet,CharacterMove.szID,CharacterMove.szCharName);
			continue;
		}

		// 구서버에 아직 캐릭이 있는지 조사한다.만약 있으면 신청 안한놈들이다.
		sprintf(szQuery,"Select [Name] From CharacterInfo Where [ID] = '%s' And [Name] = '%s'",CharacterMove.szID,CharacterMove.szCharName);
		memset(Name,0,sizeof(Name));

		if(CharacterMove.byServerSet == 1)
			ret = m_pDb->OpenRecord(szQuery,Name,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[0]);
		else if(CharacterMove.byServerSet == 4)
			ret = m_pDb->OpenRecord(szQuery,Name,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[1]);
		else if(CharacterMove.byServerSet == 5)
			ret = m_pDb->OpenRecord(szQuery,Name,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[2]);
		else if(CharacterMove.byServerSet == 8)
			ret = m_pDb->OpenRecord(szQuery,Name,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[3]);
		else if(CharacterMove.byServerSet == 9)
			ret = m_pDb->OpenRecord(szQuery,Name,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[4]);

		if(ret >= 1)
		{
			fprintf(pFile,"[Server:%d][ID:%s][Name:%s]구서버에 아직캐릭이 있다.\n",
					CharacterMove.byServerSet,CharacterMove.szID,CharacterMove.szCharName);
			continue;
		}

		// 여기까지 왔으면 캐릭이전하다가 사라진 캐릭이다.복구 해주자.
		sprintf(szQuery,"Select [Name], Head, Class, Grade, Exp, Honor, Luck, Money, Equip, Inv_large, Inv_small, Belt, Guardian From CharacterInfo_%d Where [ID] = '%s' And [Name] = '%s'",
						CharacterMove.byServerSet,
						CharacterMove.szID,
						CharacterMove.szCharName);
		memset(&ItemChrMove,0,sizeof(ITEM_CHR_MOVE));
		ret = m_pDb->OpenRecord(szQuery,&ItemChrMove,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);

		if(ret <= 0)
		{
			fprintf(pFile,"[Server:%d][ID:%s][Name:%s]캐릭이전하다가 사라져서 복구해줄라고 했더니 백업디비에 캐릭이 없다.\n",CharacterMove.byServerSet,CharacterMove.szID,CharacterMove.szCharName);
			continue;
		}
		else 
		{
			WORD wChrNum[4] = {0,};
			WORD wCharNum	= 0;
			BOOL bNum		= FALSE;

			sprintf(szQuery,"Select ChrNum From CharacterInfo Where [ID] = '%s' Order by ChrNum",CharacterMove.szID);
			ret = m_pDb->OpenRecord(szQuery,wChrNum,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);
			
			for(int i = 0; i < 4; i++)
			{
				if(wChrNum[i] == 0)
				{
					wCharNum = i + 1;
					bNum = TRUE;
					break;
				}
			}

			if(bNum)
			{
				char szIp[16] = {0,};
				sprintf(szIp,"203.238.38.215");
				sprintf(szQuery,"CZP_CREATE_NEW_CHARACTER '%s','%s', %d, %d, %d, %d, '%s'",CharacterMove.szID,CharacterMove.szCharName,wCharNum,ItemChrMove.wHead,ItemChrMove.wClass,1,szIp);	

				CREATE_CHARACTER_RESULT Result;
				memset(&Result,0,sizeof(CREATE_CHARACTER_RESULT));
				ret = m_pDb->OpenRecord(szQuery,&Result,1,m_CORUM_GAME_DB[sel]);
				
				switch(Result.dwResult)
				{
					// 캐릭터 추가 성공
					case 1:
					{
						memset(&CharMoveUpdateDB,0,sizeof(CHAR_MOVE_UPDATE_DB));

						// Data Convet //												
						for(i = 0; i < MAX_EQUIP; i++)
						{
							if(ItemChrMove.cEquip[i].GetID() != 0)
							{
								WORD wOldID = ItemChrMove.cEquip[i].GetID();
								WORD wNewID = ( wOldID / 100 * 200 ) + wOldID % 100;
								ItemChrMove.cEquip[i].m_wItemID = wNewID;

								CharMoveUpdateDB.cEquip[i].m_wItemID	= ItemChrMove.cEquip[i].m_wItemID;
								CharMoveUpdateDB.cEquip[i].m_Serial		= GetNewSerial();

								memcpy(&CharMoveUpdateDB.cEquip[i].m_ItemWeapon, &ItemChrMove.cEquip[i].m_ItemWeapon, sizeof(ItemChrMove.cEquip[i].m_ItemWeapon));
								memcpy(&CharMoveUpdateDB.cEquip[i].m_bSlot, &ItemChrMove.cEquip[i].m_bSlot, sizeof(ITEM_SLOT) + (sizeof(ITEM_ATTR) * MAX_ITEM_ATTR));	

								if(CharMoveUpdateDB.cEquip[i].GetType() == ITEM_LARGE)
								{
									for(int nAttr = 0; nAttr < MAX_ITEM_ATTR; nAttr++)	
									{
										if(	CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode	== 34 &&
											(CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue >= 376 &&
											CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue	<= 500))
										{
											CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode	= 23;
											CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue	= 1;																
										}
										else if(CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode	== 35	&&
												(CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue >= 1	&&
												CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue	<=	450))
										{
											CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode	= 23;
											CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue	= 1;															
										}
										else if(CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode	== 38 &&
												(CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue	>= 76 &&
												CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue	<= 125))
										{
											CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode	= 0;
											CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue	= 0;																
										}
										else if(CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode	== 39	&&
												(CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue	>= 276	&&
												CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue	<=325))
										{
											CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode	= 0;
											CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue	= 0;																
										}
										else if(CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode==44 &&
												(CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue>=251 &&
												CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue<=300))
										{
											CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode	= 0;
											CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue	= 0;
										}
									}	

									int nValue = CharMoveUpdateDB.cEquip[i].m_wItemID/ITEM_DISTRIBUTE;

									if(nValue>=ITEM_ARMORST_INDEX && nValue<=ITEM_ARMOREND_INDEX)
									{
										WORD wValue = 0;
										
										for(int nAttr = 0; nAttr < MAX_ITEM_ATTR; nAttr++ )	
										{
											if(	CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode == ITEM_ATTR_FIRERESIST		||
												CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode == ITEM_ATTR_LIGHTRESIST		||
												CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode == ITEM_ATTR_ICERESIST		||
												CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode == ITEM_ATTR_POIRESIST		||
												CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode == ITEM_ATTR_ALLRESIST		||
												CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode == ITEM_ATTR_PHYRESIST)
											{
												wValue += CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue;
												CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uCode	= 0;
												CharMoveUpdateDB.cEquip[i].wAttr[nAttr].uValue	= 0;																		
											}
										}
										CharMoveUpdateDB.cEquip[i].m_Item_Armor.wDefense += wValue;						
									}
								}					
							}
						}

						for(i = 0; i < MAX_INV_LARGE; i++)
						{
							if(ItemChrMove.cInvLarge[i].GetID() != 0)
							{
								WORD wOldID = ItemChrMove.cInvLarge[i].GetID();
								WORD wNewID = ( wOldID / 100 * 200 ) + wOldID % 100;
								ItemChrMove.cInvLarge[i].m_wItemID = wNewID;

								CharMoveUpdateDB.cInvLarge[i].m_wItemID = ItemChrMove.cInvLarge[i].m_wItemID;
								
								if(ItemChrMove.cInvLarge[i].GetID() / ITEM_DISTRIBUTE != ITEM_GUARDIAN_INDEX)
									CharMoveUpdateDB.cInvLarge[i].m_Serial = GetNewSerial();
								else
									CharMoveUpdateDB.cInvLarge[i].m_Serial = ItemChrMove.cInvLarge[i].m_Serial;							

								memcpy(&CharMoveUpdateDB.cInvLarge[i].m_ItemWeapon, &ItemChrMove.cInvLarge[i].m_ItemWeapon, sizeof(ItemChrMove.cInvLarge[i].m_ItemWeapon));
								memcpy(&CharMoveUpdateDB.cInvLarge[i].m_bSlot, &ItemChrMove.cInvLarge[i].m_bSlot, sizeof(ITEM_SLOT) + (sizeof(ITEM_ATTR) * MAX_ITEM_ATTR));	

								if(CharMoveUpdateDB.cInvLarge[i].GetType()==ITEM_LARGE)
								{
									for(int nAttr = 0; nAttr < MAX_ITEM_ATTR; nAttr++)	
									{
										if(	CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode==34 &&
											(CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue>=376 &&
											CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue<=500))
										{
											CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode	= 23;
											CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue	= 1;																
										}
										else if(CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode==35 &&
												(CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue>=1 &&
												CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue<=450))
										{
											CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode	= 23;
											CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue	= 1;															
										}
										else if(CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode==38 &&
												(CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue>=76 &&
												CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue<=125))
										{
											CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode	= 0;
											CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue	= 0;																
										}
										else if(CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode==39 &&
												(CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue>=276 &&
												CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue<=325))
										{
											CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode	= 0;
											CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue	= 0;																
										}
										else if(CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode==44 &&
												(CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue>=251 &&
												CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue<=300))
										{
											CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode	= 0;
											CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue	= 0;
										}
									}	
									
									int nValue = CharMoveUpdateDB.cInvLarge[i].m_wItemID/ITEM_DISTRIBUTE;

									if(nValue>=ITEM_ARMORST_INDEX && nValue<=ITEM_ARMOREND_INDEX)
									{
										WORD wValue = 0;
										
										for(int nAttr = 0; nAttr < MAX_ITEM_ATTR; nAttr++ )	
										{
											if(	CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode == ITEM_ATTR_FIRERESIST	||
												CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode == ITEM_ATTR_LIGHTRESIST	||
												CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode == ITEM_ATTR_ICERESIST		||
												CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode == ITEM_ATTR_POIRESIST		||
												CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode == ITEM_ATTR_ALLRESIST		||
												CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode == ITEM_ATTR_PHYRESIST)
											{
												wValue += CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue;
												CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uCode	= 0;
												CharMoveUpdateDB.cInvLarge[i].wAttr[nAttr].uValue	= 0;																		
											}
										}
										CharMoveUpdateDB.cInvLarge[i].m_Item_Armor.wDefense += wValue;						
									}						
								}					
							}
						}

						for(i = 0; i < MAX_INV_SMALL; i++)
						{
							if(ItemChrMove.cInvSmall[i].GetID() != 0)
							{
								WORD wOldID = ItemChrMove.cInvSmall[i].GetID();
								WORD wNewID = ( wOldID / 100 * 200 ) + wOldID % 100;
								ItemChrMove.cInvSmall[i].m_wItemID = wNewID;

								if( (wNewID>=10000 && wNewID<=10004)	|| (wNewID>=8000 && wNewID<=8205)   ||
									(wNewID>=13001 && wNewID<=13038)	|| (wNewID>=14000 && wNewID<=14405) || 
									(wNewID>=6200 && wNewID<=6604)		|| (wNewID>=18001 && wNewID<=18026)	||
									(wNewID>=16001 && wNewID<=16065)	|| (wNewID==12200))
								{
									memset(&CharMoveUpdateDB.cInvSmall[i],0,sizeof(CItem));
								}
								else
								{
									CharMoveUpdateDB.cInvSmall[i].m_wItemID	= ItemChrMove.cInvSmall[i].m_wItemID;
									CharMoveUpdateDB.cInvSmall[i].m_Serial	= GetNewSerial();

									memcpy(&CharMoveUpdateDB.cInvSmall[i].m_ItemWeapon, &ItemChrMove.cInvSmall[i].m_ItemWeapon, sizeof(ItemChrMove.cInvSmall[i].m_ItemWeapon));
									memcpy(&CharMoveUpdateDB.cInvSmall[i].m_bSlot, &ItemChrMove.cInvSmall[i].m_bSlot, sizeof(ITEM_SLOT) + (sizeof(ITEM_ATTR) * MAX_ITEM_ATTR));	
								}					
							}
						}

						for(i = 0; i < MAX_BELT; i++)
						{
							if(ItemChrMove.cBelt[i].GetID() != 0)
							{
								WORD wOldID = ItemChrMove.cBelt[i].GetID();
								WORD wNewID = ( wOldID / 100 * 200 ) + wOldID % 100;
								ItemChrMove.cBelt[i].m_wItemID = wNewID;

								if( (wNewID>=10000 && wNewID<=10004)	|| (wNewID>=8000 && wNewID<=8205)   ||
									(wNewID>=13001 && wNewID<=13038)	|| (wNewID>=14000 && wNewID<=14405) || 
									(wNewID>=6200 && wNewID<=6604)		|| (wNewID>=18001 && wNewID<=18026)	||
									(wNewID>=16001 && wNewID<=16065)	|| (wNewID==12200))
								{
									memset(&CharMoveUpdateDB.cBelt[i], 0, sizeof(CItem));
								}
								else
								{
									CharMoveUpdateDB.cBelt[i].m_wItemID	= ItemChrMove.cBelt[i].m_wItemID;
									CharMoveUpdateDB.cBelt[i].m_Serial	= GetNewSerial();

									memcpy(&CharMoveUpdateDB.cBelt[i].m_ItemWeapon, &ItemChrMove.cBelt[i].m_ItemWeapon, sizeof(ItemChrMove.cBelt[i].m_ItemWeapon));
									memcpy(&CharMoveUpdateDB.cBelt[i].m_bSlot, &ItemChrMove.cBelt[i].m_bSlot, sizeof(ITEM_SLOT) + (sizeof(ITEM_ATTR) * MAX_ITEM_ATTR));	
								}					
							}
						}

						for(i = 0; i < MAX_INV_GUARDIAN; i++)
						{
							if(ItemChrMove.cGuardian[i].GetID() != 0)
							{
								WORD wOldID = ItemChrMove.cGuardian[i].GetID();
								WORD wNewID = ( wOldID / 100 * 200 ) + wOldID % 100;
								ItemChrMove.cGuardian[i].m_wItemID = wNewID;

								CharMoveUpdateDB.cGuardian[i].m_wItemID	= ItemChrMove.cGuardian[i].m_wItemID;
							
								if(CharMoveUpdateDB.cGuardian[i].GetID()/ITEM_DISTRIBUTE != ITEM_GUARDIAN_INDEX)
									CharMoveUpdateDB.cGuardian[i].m_Serial	= GetNewSerial();
								else
									CharMoveUpdateDB.cGuardian[i].m_Serial	= ItemChrMove.cGuardian[i].m_Serial;

								memcpy(&CharMoveUpdateDB.cGuardian[i].m_ItemWeapon, &ItemChrMove.cGuardian[i].m_ItemWeapon, sizeof(ItemChrMove.cGuardian[i].m_ItemWeapon));
								memcpy(&CharMoveUpdateDB.cGuardian[i].m_bSlot, &ItemChrMove.cGuardian[i].m_bSlot, sizeof(ITEM_SLOT) + (sizeof(ITEM_ATTR) * MAX_ITEM_ATTR));	
							}
						}

						CharMoveUpdateDB.dwExp		= ItemChrMove.dwExp;			
						CharMoveUpdateDB.dwHonor	= ItemChrMove.dwHonor;
						CharMoveUpdateDB.dwLuck		= ItemChrMove.dwLuck;
						CharMoveUpdateDB.dwMoney	= ItemChrMove.dwMoney / 100;
						CharMoveUpdateDB.wPoint		= 0;

						for(i = 0; i < 256; i++)
						{
							if(dwExp[i] > CharMoveUpdateDB.dwExp)
							{
								CharMoveUpdateDB.wPoint_Skill	= i;
								CharMoveUpdateDB.dwLevel		= i;
																					
								for(DWORD j = 0; j < CharMoveUpdateDB.dwLevel-1; j++)
								{
									CharMoveUpdateDB.wPoint += byStatus[j];													
								}

								break;
							}
						}
													
						// CharacterInfo Update //
						DBBINDING*	pBinding;
						int			nColumn			= 12;
						DWORD		dwParamOffset	= 0;												
						pBinding = g_pDb->CreateParamInfo(nColumn);
						if(!pBinding) continue;			

						sprintf(szQuery, "Update CharacterInfo Set Point=?, Point_Skill=?, Exp=?, [Level]=?, Honor=?, Luck=?, Money=?, Equip=?, Inv_large=?, Inv_small=?, Belt=?, Guardian=? Where Name = '%s'",CharacterMove.szCharName);
										
						pBinding[0].obValue		= dwParamOffset;	
						pBinding[0].cbMaxLen	= sizeof(CharMoveUpdateDB.wPoint);
						pBinding[0].wType		= DBTYPE_I2;					
						dwParamOffset			+= sizeof(CharMoveUpdateDB.wPoint);

						pBinding[1].obValue		= dwParamOffset;	
						pBinding[1].cbMaxLen	= sizeof(CharMoveUpdateDB.wPoint_Skill);
						pBinding[1].wType		= DBTYPE_I2;					
						dwParamOffset			+= sizeof(CharMoveUpdateDB.wPoint_Skill);

						pBinding[2].obValue		= dwParamOffset;	
						pBinding[2].cbMaxLen	= sizeof(CharMoveUpdateDB.dwExp);
						pBinding[2].wType		= DBTYPE_I4;					
						dwParamOffset			+= sizeof(CharMoveUpdateDB.dwExp);

						pBinding[3].obValue		= dwParamOffset;	
						pBinding[3].cbMaxLen	= sizeof(CharMoveUpdateDB.dwLevel);
						pBinding[3].wType		= DBTYPE_I4;					
						dwParamOffset			+= sizeof(CharMoveUpdateDB.dwLevel);

						pBinding[4].obValue		= dwParamOffset;	
						pBinding[4].cbMaxLen	= sizeof(CharMoveUpdateDB.dwHonor);
						pBinding[4].wType		= DBTYPE_I4;					
						dwParamOffset			+= sizeof(CharMoveUpdateDB.dwHonor);

						pBinding[5].obValue		= dwParamOffset;	
						pBinding[5].cbMaxLen	= sizeof(CharMoveUpdateDB.dwLuck);
						pBinding[5].wType		= DBTYPE_I4;					
						dwParamOffset			+= sizeof(CharMoveUpdateDB.dwLuck);

						pBinding[6].obValue		= dwParamOffset;	
						pBinding[6].cbMaxLen	= sizeof(CharMoveUpdateDB.dwMoney);
						pBinding[6].wType		= DBTYPE_I4;					
						dwParamOffset			+= sizeof(CharMoveUpdateDB.dwMoney);

						pBinding[7].obValue		= dwParamOffset;	
						pBinding[7].cbMaxLen	= sizeof(CharMoveUpdateDB.cEquip);
						pBinding[7].wType		= DBTYPE_BYTES;					
						dwParamOffset			+= sizeof(CharMoveUpdateDB.cEquip);
						
						pBinding[8].obValue		= dwParamOffset;	
						pBinding[8].cbMaxLen	= sizeof(CharMoveUpdateDB.cInvLarge);
						pBinding[8].wType		= DBTYPE_BYTES;					
						dwParamOffset			+= sizeof(CharMoveUpdateDB.cInvLarge);
						
						pBinding[9].obValue		= dwParamOffset;	
						pBinding[9].cbMaxLen	= sizeof(CharMoveUpdateDB.cInvSmall);
						pBinding[9].wType		= DBTYPE_BYTES;					
						dwParamOffset			+= sizeof(CharMoveUpdateDB.cInvSmall);
						
						pBinding[10].obValue	= dwParamOffset;	
						pBinding[10].cbMaxLen	= sizeof(CharMoveUpdateDB.cBelt);
						pBinding[10].wType		= DBTYPE_BYTES;					
						dwParamOffset			+= sizeof(CharMoveUpdateDB.cBelt);
						
						pBinding[11].obValue	= dwParamOffset;	
						pBinding[11].cbMaxLen	= sizeof(CharMoveUpdateDB.cGuardian);
						pBinding[11].wType		= DBTYPE_BYTES;					
						dwParamOffset			+= sizeof(CharMoveUpdateDB.cGuardian);
						
						g_pDb->ExecuteSQLByParam(szQuery,pBinding,&CharMoveUpdateDB,nColumn,m_CORUM_GAME_DB[sel]);	
						
						for(i = 0; i < MAX_INV_LARGE; i++)
						{
							if(CharMoveUpdateDB.cInvLarge[i].GetID()/ITEM_DISTRIBUTE==ITEM_GUARDIAN_INDEX)
							{
								if(CharMoveUpdateDB.cInvLarge[i].m_Item_Guardian.bCreate == 0)
									continue;
								
								sprintf(szQuery, "Select Name, BaseIndex, [Level], Exp, HP, MP, ServerSerialID, RestartCount, SerialNum From GuardianInfoConvert Where ServerSet = %u And ServerSerialID = %u And RestartCount = %u And SerialNum = %u",
										CharMoveUpdateDB.cInvLarge[i].m_Serial.bServerSet, CharMoveUpdateDB.cInvLarge[i].m_Serial.bServerSerialID, CharMoveUpdateDB.cInvLarge[i].m_Serial.wRestartCount, CharMoveUpdateDB.cInvLarge[i].m_Serial.dwSerialNum);
//								sprintf(szQuery, "Select Name, BaseIndex, [Level], Exp, HP, MP, ServerSerialID, RestartCount, SerialNum From GuardianInfoConvertTest Where ServerSet = %u And ServerSerialID = %u And RestartCount = %u And SerialNum = %u",
//										CharMoveUpdateDB.cInvLarge[i].m_Serial.bServerSet, CharMoveUpdateDB.cInvLarge[i].m_Serial.bServerSerialID, CharMoveUpdateDB.cInvLarge[i].m_Serial.wRestartCount, CharMoveUpdateDB.cInvLarge[i].m_Serial.dwSerialNum);
								memset(&GuardianInfoMove,0,sizeof(GUARDIAN_INFO_MOVE));
								ret = g_pDb->OpenRecord(szQuery,&GuardianInfoMove,1,(BYTE)m_CORUM_GAME_DB[sel]);
								
								if(ret <= 0)
								{
									// 구서버에서 생성한 가디언인포가 없다.
									fprintf(pFile,"[Server:%d][ID:%s][Name:%s]가디언은 생성되었는데 구서버에서 생성한 가디언인포가 없다.\n",
											CharacterMove.byServerSet,CharacterMove.szID,CharacterMove.szCharName);
									continue;
								}
								else
								{
									MY_DB_NUMERIC Result;
									memset(&Result,0,sizeof(MY_DB_NUMERIC));
									sprintf(szQuery,"CZP_MOVE_GUARDIAN_INFO '%s', %u, %u, %u, %u, %u",GuardianInfoMove.szName,GuardianInfoMove.dwBaseIndex,GuardianInfoMove.dwLevel,GuardianInfoMove.dwExp,GuardianInfoMove.dwHP,GuardianInfoMove.dwMP);
									ret = m_pDb->OpenRecord(szQuery,&Result,1,m_CORUM_GAME_DB[sel]);
									
									CharMoveUpdateDB.cInvLarge[i].m_Item_Guardian.bCreate	= 1;
									CharMoveUpdateDB.cInvLarge[i].m_Item_Guardian.dwID		= Result.least;

									sprintf(szQuery,"Delete From GuardianInfoConvert Where ServerSet = %u And ServerSerialID = %u And RestartCount = %u And SerialNum = %u",
											CharMoveUpdateDB.cInvLarge[i].m_Serial.bServerSet, CharMoveUpdateDB.cInvLarge[i].m_Serial.bServerSerialID, CharMoveUpdateDB.cInvLarge[i].m_Serial.wRestartCount, CharMoveUpdateDB.cInvLarge[i].m_Serial.dwSerialNum);
//									sprintf(szQuery,"Delete From GuardianInfoConvertTest Where ServerSet = %u And ServerSerialID = %u And RestartCount = %u And SerialNum = %u",
//										CharMoveUpdateDB.cInvLarge[i].m_Serial.bServerSet, CharMoveUpdateDB.cInvLarge[i].m_Serial.bServerSerialID, CharMoveUpdateDB.cInvLarge[i].m_Serial.wRestartCount, CharMoveUpdateDB.cInvLarge[i].m_Serial.dwSerialNum);
									m_pDb->ExecuteSQL(szQuery,(BYTE)m_CORUM_GAME_DB[sel]);

									CharMoveUpdateDB.cInvLarge[i].m_Serial = GetNewSerial();

									DBBINDING*	pBinding;
									int			nColumn			= 1;
									DWORD		dwParamOffset	= 0;
									pBinding					= m_pDb->CreateParamInfo(nColumn);
									if(!pBinding)	continue;
									
									sprintf(szQuery, "Update CharacterInfo Set Inv_large=? Where Name = '%s'", CharacterMove.szCharName);
									
									INV_UPDATE_DB sInvUpdateDB;
									memset(&sInvUpdateDB,0,sizeof(sInvUpdateDB));
									memcpy(sInvUpdateDB.cInvLarge,CharMoveUpdateDB.cInvLarge,sizeof(CharMoveUpdateDB.cInvLarge));
									
									pBinding[0].obValue		= dwParamOffset;	
									pBinding[0].cbMaxLen	= sizeof(sInvUpdateDB.cInvLarge);
									pBinding[0].wType		= DBTYPE_BYTES;					
									dwParamOffset			+= sizeof(sInvUpdateDB.cInvLarge);
									
									m_pDb->ExecuteSQLByParam(szQuery,pBinding,&sInvUpdateDB,nColumn,(BYTE)m_CORUM_GAME_DB[sel]);
								}
							}
						}
						
						for(i = 0; i < MAX_INV_GUARDIAN; i++)
						{
							if(CharMoveUpdateDB.cGuardian[i].GetID()/ITEM_DISTRIBUTE==ITEM_GUARDIAN_INDEX)
							{					
								if(CharMoveUpdateDB.cGuardian[i].m_Item_Guardian.bCreate == 0)
									continue;

								sprintf(szQuery, "Select Name, BaseIndex, [Level], Exp, HP, MP, ServerSerialID, RestartCount, SerialNum From GuardianInfoConvert Where ServerSet = %u And ServerSerialID = %u And RestartCount = %u And SerialNum = %u",
										CharMoveUpdateDB.cGuardian[i].m_Serial.bServerSet, CharMoveUpdateDB.cGuardian[i].m_Serial.bServerSerialID, CharMoveUpdateDB.cGuardian[i].m_Serial.wRestartCount, CharMoveUpdateDB.cGuardian[i].m_Serial.dwSerialNum);
//								sprintf(szQuery, "Select Name, BaseIndex, [Level], Exp, HP, MP, ServerSerialID, RestartCount, SerialNum From GuardianInfoConvertTest Where ServerSet = %u And ServerSerialID = %u And RestartCount = %u And SerialNum = %u",
//										CharMoveUpdateDB.cGuardian[i].m_Serial.bServerSet, CharMoveUpdateDB.cGuardian[i].m_Serial.bServerSerialID, CharMoveUpdateDB.cGuardian[i].m_Serial.wRestartCount, CharMoveUpdateDB.cGuardian[i].m_Serial.dwSerialNum);
								memset(&GuardianInfoMove,0,sizeof(GUARDIAN_INFO_MOVE));
								ret = g_pDb->OpenRecord(szQuery,&GuardianInfoMove,1,(BYTE)m_CORUM_GAME_DB[sel]);

								if(ret <= 0)
								{
									// 구서버에서 생성한 가디언인포가 없다.
									fprintf(pFile,"[Server:%d][ID:%s][Name:%s]가디언은 생성되었는데 구서버에서 생성한 가디언인포가 없다.\n",
											CharacterMove.byServerSet,CharacterMove.szID,CharacterMove.szCharName);
									continue;
								}
								else
								{
									MY_DB_NUMERIC Result;
									memset(&Result,0,sizeof(MY_DB_NUMERIC));
									sprintf(szQuery,"CZP_MOVE_GUARDIAN_INFO '%s', %u, %u, %u, %u, %u",GuardianInfoMove.szName,GuardianInfoMove.dwBaseIndex,GuardianInfoMove.dwLevel,GuardianInfoMove.dwExp,GuardianInfoMove.dwHP,GuardianInfoMove.dwMP);
									ret = m_pDb->OpenRecord(szQuery,&Result,1,m_CORUM_GAME_DB[sel]);
									
									CharMoveUpdateDB.cGuardian[i].m_Item_Guardian.bCreate	= 1;
									CharMoveUpdateDB.cGuardian[i].m_Item_Guardian.dwID		= Result.least;
									
									sprintf(szQuery,"Delete From GuardianInfoConvert Where ServerSet = %u And ServerSerialID = %u And RestartCount = %u And SerialNum = %u",
											CharMoveUpdateDB.cGuardian[i].m_Serial.bServerSet, CharMoveUpdateDB.cGuardian[i].m_Serial.bServerSerialID, CharMoveUpdateDB.cGuardian[i].m_Serial.wRestartCount, CharMoveUpdateDB.cGuardian[i].m_Serial.dwSerialNum);
//									sprintf(szQuery,"Delete From GuardianInfoConvertTest Where ServerSet = %u And ServerSerialID = %u And RestartCount = %u And SerialNum = %u",
//											CharMoveUpdateDB.cGuardian[i].m_Serial.bServerSet, CharMoveUpdateDB.cGuardian[i].m_Serial.bServerSerialID, CharMoveUpdateDB.cGuardian[i].m_Serial.wRestartCount, CharMoveUpdateDB.cGuardian[i].m_Serial.dwSerialNum);
									m_pDb->ExecuteSQL(szQuery,(BYTE)m_CORUM_GAME_DB[sel]);
									
									CharMoveUpdateDB.cGuardian[i].m_Serial = GetNewSerial();
									
									DBBINDING*	pBinding;
									int			nColumn			= 1;
									DWORD		dwParamOffset	= 0;
									pBinding					= m_pDb->CreateParamInfo(nColumn);
									if(!pBinding)	continue;
									
									sprintf(szQuery, "Update CharacterInfo Set Guardian=? Where Name = '%s'", CharacterMove.szCharName);
									
									GUARDIAN_UPDATE_DB sGuardianUpdateDB;
									memset(&sGuardianUpdateDB,0,sizeof(sGuardianUpdateDB));
									memcpy(sGuardianUpdateDB.cGuardian,CharMoveUpdateDB.cGuardian,sizeof(CharMoveUpdateDB.cGuardian));
									
									pBinding[0].obValue		= dwParamOffset;	
									pBinding[0].cbMaxLen	= sizeof(sGuardianUpdateDB.cGuardian);
									pBinding[0].wType		= DBTYPE_BYTES;					
									dwParamOffset			+= sizeof(sGuardianUpdateDB.cGuardian);
									
									m_pDb->ExecuteSQLByParam(szQuery,pBinding,&sGuardianUpdateDB,nColumn,(BYTE)m_CORUM_GAME_DB[sel]);
								}
							}
						}
					}
					break;

					// 이미 있는 캐릭터 
					case -1:
					{
						fprintf(pFile,"[Server:%d][ID:%s][Name:%s]뉴캐릭터했더니 이미 있는 캐릭터이다.\n",
								CharacterMove.byServerSet,CharacterMove.szID,CharacterMove.szCharName);
						continue;		
					}
					break;

					// 자리 중복. 
					case -2:
					{
						fprintf(pFile,"[Server:%d][ID:%s][Name:%s]뉴캐릭터했는데 자리 중복이다.\n",
								CharacterMove.byServerSet,CharacterMove.szID,CharacterMove.szCharName);
						continue;		
					}
					break;	
				}
			}
		}
	}

	m_pDb->ReleaseRecordset(pData);
	pData = NULL;

	sprintf(szQuery,"Delete From CharacterMove");
	m_pDb->ExecuteSQL(szQuery,(BYTE)m_CORUM_GAME_DB[sel]);

	fprintf(pFile,"\n");
	fclose(pFile);
	
	// 다 끝났다.
	AfxMessageBox("Character Move End!!!",MB_OK|MB_ICONINFORMATION);
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CCopyItemDelete::ConvertCItem()
{
	char				szQuery[0xff]	= {0,};
	char				Name[20]		= {0,};
	char				ID[20]			= {0,};
	DWORD				nameCount		= 0;
	DWORD				bankCount		= 0;
	DBRECEIVEDATA*		pData			= NULL;
	int					sel				= m_ComBoCtrl.GetCurSel();
	CString				ShowQuery;
	CharacterItem		CharItem;
	CharacterItem2		CharItem2;
	BankItem			bankItem;
	BankItem2			bankItem2;

	/*캐릭터 이름*/
	ShowQuery.Format("All CharName Query");
	m_pEdit->SetWindowText(ShowQuery);
	sprintf(szQuery,"SELECT Count(*) FROM CharacterInfo");
	int ret = m_pDb->OpenRecord(szQuery,&nameCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);		
	sprintf(szQuery,"SELECT [Name] FROM CharacterInfo");		
	pData = m_pDb->OpenRecordEx(szQuery,nameCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);
	
	if( pData == NULL )
	{
		return;		
	}

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
		memset(&CharItem2,0,sizeof(CharacterItem2));
		ret = m_pDb->OpenRecord(szQuery,&CharItem,1,m_CORUM_GAME_DB[sel]);

		if( ret <= 0 )
		{
			continue;
		}
	
		// EQuip
		for( int equip = 0; equip < MAX_EQUIP; equip++ )
		{
			if(CharItem.Equip[equip].GetID() != 0)
			{	
				CItem2* pItem2	= &CharItem2.Equip[equip];
				CItem*	pItem	= &CharItem.Equip[equip];

				memcpy(pItem2,pItem,sizeof(WORD) + sizeof(ITEM_SERIAL));
				memcpy(&pItem2->m_ItemWeapon,&pItem->m_ItemWeapon,sizeof(ITEM_WEAPON));
				memcpy(&pItem2->m_bSlot,&pItem->m_bSlot,sizeof(ITEM_SLOT) + (sizeof(ITEM_ATTR) * MAX_ITEM_ATTR));	
			}
		}

		// Inv_Large
		for( int inv_large = 0; inv_large < MAX_INV_LARGE; inv_large++ )
		{
			if(CharItem.Inv_Large[inv_large].GetID() != 0)
			{	
				CItem2* pItem2	= &CharItem2.Inv_Large[inv_large];
				CItem*	pItem	= &CharItem.Inv_Large[inv_large];
				
				memcpy(pItem2,pItem,sizeof(WORD) + sizeof(ITEM_SERIAL));
				memcpy(&pItem2->m_ItemWeapon,&pItem->m_ItemWeapon,sizeof(ITEM_WEAPON));
				memcpy(&pItem2->m_bSlot,&pItem->m_bSlot,sizeof(ITEM_SLOT) + (sizeof(ITEM_ATTR) * MAX_ITEM_ATTR));	
			}
		}

		// Inv_Small
		for( int inv_small = 0; inv_small < MAX_INV_SMALL; inv_small++ )
		{
			if(CharItem.Inv_Small[inv_small].GetID() != 0)
			{	
				CItem2* pItem2	= &CharItem2.Inv_Small[inv_small];
				CItem*	pItem	= &CharItem.Inv_Small[inv_small];
				
				memcpy(pItem2,pItem,sizeof(WORD) + sizeof(ITEM_SERIAL));
				memcpy(&pItem2->m_ItemWeapon,&pItem->m_ItemWeapon,sizeof(ITEM_WEAPON));
				memcpy(&pItem2->m_bSlot,&pItem->m_bSlot,sizeof(ITEM_SLOT) + (sizeof(ITEM_ATTR) * MAX_ITEM_ATTR));	
			}
		}

		// Belt
		for( int belt = 0; belt < MAX_BELT; belt++ )
		{
			if(CharItem.Belt[belt].GetID() != 0)
			{	
				CItem2* pItem2	= &CharItem2.Belt[belt];
				CItem*	pItem	= &CharItem.Belt[belt];
				
				memcpy(pItem2,pItem,sizeof(WORD) + sizeof(ITEM_SERIAL));
				memcpy(&pItem2->m_ItemWeapon,&pItem->m_ItemWeapon,sizeof(ITEM_WEAPON));
				memcpy(&pItem2->m_bSlot,&pItem->m_bSlot,sizeof(ITEM_SLOT) + (sizeof(ITEM_ATTR) * MAX_ITEM_ATTR));	
			}
		}

		// Guardian
		for( int guardian = 0; guardian < MAX_GUARDIAN; guardian++ )
		{
			if(CharItem.Guardian[guardian].GetID() != 0)
			{	
				CItem2* pItem2	= &CharItem2.Guardian[guardian];
				CItem*	pItem	= &CharItem.Guardian[guardian];
				
				memcpy(pItem2,pItem,sizeof(WORD) + sizeof(ITEM_SERIAL));
				memcpy(&pItem2->m_ItemWeapon,&pItem->m_ItemWeapon,sizeof(ITEM_WEAPON));
				memcpy(&pItem2->m_bSlot,&pItem->m_bSlot,sizeof(ITEM_SLOT) + (sizeof(ITEM_ATTR) * MAX_ITEM_ATTR));	
			}
		}
		
		int			nColumn			= 5;	// 파라미터 갯수 지정 
		DWORD		dwParamOffset	= 0;	// 파라미터 Binding 정보 구조체에서 각 파라미터 Value들의 Offset
		DBBINDING*	pBinding		= g_pDb->CreateParamInfo(nColumn);		
		if(!pBinding)	continue;

		sprintf(szQuery," Update CharacterInfo2 Set [Equip]=?, [Inv_large]=?, [Inv_small]=?, [Belt]=?, "
						" [Guardian]=? Where [Name]='%s' ",Name);

		pBinding[0].obValue		= dwParamOffset;	
		pBinding[0].cbMaxLen	= sizeof(CharItem2.Equip);
		pBinding[0].wType		= DBTYPE_BYTES;
		dwParamOffset			+= sizeof(CharItem2.Equip);				
		
		pBinding[1].obValue		= dwParamOffset;	
		pBinding[1].cbMaxLen	= sizeof(CharItem2.Inv_Large);
		pBinding[1].wType		= DBTYPE_BYTES;
		dwParamOffset			+= sizeof(CharItem2.Inv_Large);		
		
		pBinding[2].obValue		= dwParamOffset;	
		pBinding[2].cbMaxLen	= sizeof(CharItem2.Inv_Small);
		pBinding[2].wType		= DBTYPE_BYTES;
		dwParamOffset			+= sizeof(CharItem2.Inv_Small);			
		
		pBinding[3].obValue		= dwParamOffset;	
		pBinding[3].cbMaxLen	= sizeof(CharItem2.Belt);
		pBinding[3].wType		= DBTYPE_BYTES;
		dwParamOffset			+= sizeof(CharItem2.Belt);
		
		pBinding[4].obValue		= dwParamOffset;	
		pBinding[4].cbMaxLen	= sizeof(CharItem2.Guardian);
		pBinding[4].wType		= DBTYPE_BYTES;
		dwParamOffset			+= sizeof(CharItem2.Guardian);
		
		g_pDb->ExecuteSQLByParam(szQuery,pBinding,&CharItem2,nColumn,m_CORUM_GAME_DB[sel]);
		g_pDb->ReleaseParamInfo(pBinding);
	}

	m_pDb->ReleaseRecordset(pData);
	pData = NULL;

	/*지운캐릭터 이름*/
	ShowQuery.Format("All CharName Query");
	m_pEdit->SetWindowText(ShowQuery);
	sprintf(szQuery,"SELECT Count(*) FROM DeletedCharacter");
	ret = m_pDb->OpenRecord(szQuery,&nameCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);		
	sprintf(szQuery,"SELECT [Name] FROM DeletedCharacter");		
	pData = m_pDb->OpenRecordEx(szQuery,nameCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);
	
	if( pData == NULL )
	{
		return;		
	}

	for( i = 0; i < pData->Query.select.dwRowCount; i++ )	
	{
		if(!m_bRun)
		{				
			m_pDb->ReleaseRecordset(pData);					
			return;				
		}
		::Sleep(10);
		
		memset(Name,0,sizeof(Name));					
		memcpy(Name,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize);
		
		ShowQuery.Format("Q(DeletedCharacter):<%d>[%d].[%s]",pData->Query.select.dwRowCount,i+1,Name);
		m_pEdit->SetWindowText(ShowQuery);
		
		sprintf(szQuery,"Select Equip,Inv_large,Inv_small,Belt,Guardian From DeletedCharacter Where [Name]='%s'",Name);
		memset(&CharItem,0,sizeof(CharacterItem));
		memset(&CharItem2,0,sizeof(CharacterItem2));
		ret = m_pDb->OpenRecord(szQuery,&CharItem,1,m_CORUM_GAME_DB[sel]);

		if( ret <= 0 )
		{
			continue;
		}
	
		// EQuip
		for( int equip = 0; equip < MAX_EQUIP; equip++ )
		{
			if(CharItem.Equip[equip].GetID() != 0)
			{	
				CItem2* pItem2	= &CharItem2.Equip[equip];
				CItem*	pItem	= &CharItem.Equip[equip];

				memcpy(pItem2,pItem,sizeof(WORD) + sizeof(ITEM_SERIAL));
				memcpy(&pItem2->m_ItemWeapon,&pItem->m_ItemWeapon,sizeof(ITEM_WEAPON));
				memcpy(&pItem2->m_bSlot,&pItem->m_bSlot,sizeof(ITEM_SLOT) + (sizeof(ITEM_ATTR) * MAX_ITEM_ATTR));	
			}
		}

		// Inv_Large
		for( int inv_large = 0; inv_large < MAX_INV_LARGE; inv_large++ )
		{
			if(CharItem.Inv_Large[inv_large].GetID() != 0)
			{	
				CItem2* pItem2	= &CharItem2.Inv_Large[inv_large];
				CItem*	pItem	= &CharItem.Inv_Large[inv_large];
				
				memcpy(pItem2,pItem,sizeof(WORD) + sizeof(ITEM_SERIAL));
				memcpy(&pItem2->m_ItemWeapon,&pItem->m_ItemWeapon,sizeof(ITEM_WEAPON));
				memcpy(&pItem2->m_bSlot,&pItem->m_bSlot,sizeof(ITEM_SLOT) + (sizeof(ITEM_ATTR) * MAX_ITEM_ATTR));	
			}
		}

		// Inv_Small
		for( int inv_small = 0; inv_small < MAX_INV_SMALL; inv_small++ )
		{
			if(CharItem.Inv_Small[inv_small].GetID() != 0)
			{	
				CItem2* pItem2	= &CharItem2.Inv_Small[inv_small];
				CItem*	pItem	= &CharItem.Inv_Small[inv_small];
				
				memcpy(pItem2,pItem,sizeof(WORD) + sizeof(ITEM_SERIAL));
				memcpy(&pItem2->m_ItemWeapon,&pItem->m_ItemWeapon,sizeof(ITEM_WEAPON));
				memcpy(&pItem2->m_bSlot,&pItem->m_bSlot,sizeof(ITEM_SLOT) + (sizeof(ITEM_ATTR) * MAX_ITEM_ATTR));	
			}
		}

		// Belt
		for( int belt = 0; belt < MAX_BELT; belt++ )
		{
			if(CharItem.Belt[belt].GetID() != 0)
			{	
				CItem2* pItem2	= &CharItem2.Belt[belt];
				CItem*	pItem	= &CharItem.Belt[belt];
				
				memcpy(pItem2,pItem,sizeof(WORD) + sizeof(ITEM_SERIAL));
				memcpy(&pItem2->m_ItemWeapon,&pItem->m_ItemWeapon,sizeof(ITEM_WEAPON));
				memcpy(&pItem2->m_bSlot,&pItem->m_bSlot,sizeof(ITEM_SLOT) + (sizeof(ITEM_ATTR) * MAX_ITEM_ATTR));	
			}
		}

		// Guardian
		for( int guardian = 0; guardian < MAX_GUARDIAN; guardian++ )
		{
			if(CharItem.Guardian[guardian].GetID() != 0)
			{	
				CItem2* pItem2	= &CharItem2.Guardian[guardian];
				CItem*	pItem	= &CharItem.Guardian[guardian];
				
				memcpy(pItem2,pItem,sizeof(WORD) + sizeof(ITEM_SERIAL));
				memcpy(&pItem2->m_ItemWeapon,&pItem->m_ItemWeapon,sizeof(ITEM_WEAPON));
				memcpy(&pItem2->m_bSlot,&pItem->m_bSlot,sizeof(ITEM_SLOT) + (sizeof(ITEM_ATTR) * MAX_ITEM_ATTR));	
			}
		}
		
		int			nColumn			= 5;	// 파라미터 갯수 지정 
		DWORD		dwParamOffset	= 0;	// 파라미터 Binding 정보 구조체에서 각 파라미터 Value들의 Offset
		DBBINDING*	pBinding		= g_pDb->CreateParamInfo(nColumn);		
		if(!pBinding)	continue;

		sprintf(szQuery," Update DeletedCharacter2 Set [Equip]=?, [Inv_large]=?, [Inv_small]=?, [Belt]=?, "
						" [Guardian]=? Where [Name]='%s' ",Name);

		pBinding[0].obValue		= dwParamOffset;	
		pBinding[0].cbMaxLen	= sizeof(CharItem2.Equip);
		pBinding[0].wType		= DBTYPE_BYTES;
		dwParamOffset			+= sizeof(CharItem2.Equip);				
		
		pBinding[1].obValue		= dwParamOffset;	
		pBinding[1].cbMaxLen	= sizeof(CharItem2.Inv_Large);
		pBinding[1].wType		= DBTYPE_BYTES;
		dwParamOffset			+= sizeof(CharItem2.Inv_Large);		
		
		pBinding[2].obValue		= dwParamOffset;	
		pBinding[2].cbMaxLen	= sizeof(CharItem2.Inv_Small);
		pBinding[2].wType		= DBTYPE_BYTES;
		dwParamOffset			+= sizeof(CharItem2.Inv_Small);			
		
		pBinding[3].obValue		= dwParamOffset;	
		pBinding[3].cbMaxLen	= sizeof(CharItem2.Belt);
		pBinding[3].wType		= DBTYPE_BYTES;
		dwParamOffset			+= sizeof(CharItem2.Belt);
		
		pBinding[4].obValue		= dwParamOffset;	
		pBinding[4].cbMaxLen	= sizeof(CharItem2.Guardian);
		pBinding[4].wType		= DBTYPE_BYTES;
		dwParamOffset			+= sizeof(CharItem2.Guardian);
		
		g_pDb->ExecuteSQLByParam(szQuery,pBinding,&CharItem2,nColumn,m_CORUM_GAME_DB[sel]);
		g_pDb->ReleaseParamInfo(pBinding);
	}

	m_pDb->ReleaseRecordset(pData);
	pData = NULL;

	/*창고아이디 이름*/
	ShowQuery.Format("All BankID Query");
	m_pEdit->SetWindowText(ShowQuery);
	
	sprintf(szQuery,"SELECT Count(*) FROM BankInfo");	
	ret = m_pDb->OpenRecord(szQuery,&bankCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);
	sprintf(szQuery,"SELECT [ID] FROM BankInfo");
	pData = m_pDb->OpenRecordEx(szQuery,bankCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);
	
	if( pData == NULL )
	{
		return;		
	}
	
	for( i = 0; i < pData->Query.select.dwRowCount; i++ )	
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
		
		sprintf(szQuery,"Select Bank_Large,Bank_small From BankInfo Where [ID]='%s'",ID);
		memset(&bankItem,0,sizeof(BankItem));
		memset(&bankItem2,0,sizeof(BankItem2));
		int ret = m_pDb->OpenRecord(szQuery,&bankItem,1,m_CORUM_GAME_DB[sel]);
		
		if( ret <= 0 )
		{
			continue;
		}

		// Bank_Large
		for( int banklarge = 0; banklarge < MAX_BANK_LARGE; banklarge++ )
		{
			if(bankItem.Bank_Large[banklarge].GetID() != 0)
			{	
				CItem2* pItem2	= &bankItem2.Bank_Large[banklarge];
				CItem*	pItem	= &bankItem.Bank_Large[banklarge];
				
				memcpy(pItem2,pItem,sizeof(WORD) + sizeof(ITEM_SERIAL));
				memcpy(&pItem2->m_ItemWeapon,&pItem->m_ItemWeapon,sizeof(ITEM_WEAPON));
				memcpy(&pItem2->m_bSlot,&pItem->m_bSlot,sizeof(ITEM_SLOT) + (sizeof(ITEM_ATTR) * MAX_ITEM_ATTR));	
			}	
		}

		// Bank_Small
		for( int banksmall = 0; banksmall < MAX_BANK_SMALL; banksmall++ )
		{
			if(bankItem.Bank_Small[banksmall].GetID() != 0)
			{	
				CItem2* pItem2	= &bankItem2.Bank_Small[banksmall];
				CItem*	pItem	= &bankItem.Bank_Small[banksmall];
				
				memcpy(pItem2,pItem,sizeof(WORD) + sizeof(ITEM_SERIAL));
				memcpy(&pItem2->m_ItemWeapon,&pItem->m_ItemWeapon,sizeof(ITEM_WEAPON));
				memcpy(&pItem2->m_bSlot,&pItem->m_bSlot,sizeof(ITEM_SLOT) + (sizeof(ITEM_ATTR) * MAX_ITEM_ATTR));	
			}
		}

		int			nColumn			= 2;	//파라미터 갯수 지정 
		DWORD		dwParamOffset	= 0;	//파라미터 Binding 정보 구조체에서 각 파라미터 Value들의 Offset
		DBBINDING*	pBinding		= g_pDb->CreateParamInfo(nColumn);
		if(!pBinding)	continue;
				
		sprintf(szQuery,"Update Bankinfo2 Set Bank_Large=?, Bank_Small=? Where [ID]='%s'",ID);		

		pBinding[0].obValue		= dwParamOffset;	
		pBinding[0].cbMaxLen	= sizeof(bankItem2.Bank_Large);
		pBinding[0].wType		= DBTYPE_BYTES;
		dwParamOffset			+= sizeof(bankItem2.Bank_Large);		
		
		pBinding[1].obValue		= dwParamOffset;	
		pBinding[1].cbMaxLen	= sizeof(bankItem2.Bank_Small);
		pBinding[1].wType		= DBTYPE_BYTES;
		dwParamOffset			+= sizeof(bankItem2.Bank_Small);

		g_pDb->ExecuteSQLByParam(szQuery,pBinding,&bankItem2,nColumn,m_CORUM_GAME_DB[sel]);
		g_pDb->ReleaseParamInfo(pBinding);
	}

	m_pDb->ReleaseRecordset(pData);
	pData = NULL;

	int					MapInfoCount = 0; 
	DUNGEON_DATA_EX		Dungeon;
	DUNGEON_DATA_EX2	Dungeon2;

	sprintf(szQuery,"Select Count(*) From MapInfo");
	ret = g_pDb->OpenRecord(szQuery,&MapInfoCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);
	sprintf(szQuery,"Select ID,GuardianItem,MagicArrayItem From MapInfo Order by ID");
	pData = g_pDb->OpenRecordEx(szQuery,MapInfoCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);

	if( pData == NULL )
	{
		return;		
	}

	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		memset(&Dungeon,0,sizeof(DUNGEON_DATA_EX));
		memset(&Dungeon2,0,sizeof(DUNGEON_DATA_EX2));
		memcpy(&Dungeon,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize);

		ShowQuery.Format("Q(MapInfo):<%d>[%d].[%d]",pData->Query.select.dwRowCount,i+1,Dungeon.m_dwID);
		m_pEdit->SetWindowText(ShowQuery);

		if(Dungeon.m_cGuardianItem.GetID() != 0)
		{
			CItem2* pItem2	= &Dungeon2.m_cGuardianItem;
			CItem*	pItem	= &Dungeon.m_cGuardianItem;
			
			memcpy(pItem2,pItem,sizeof(WORD) + sizeof(ITEM_SERIAL));
			memcpy(&pItem2->m_ItemWeapon,&pItem->m_ItemWeapon,sizeof(ITEM_WEAPON));
			memcpy(&pItem2->m_bSlot,&pItem->m_bSlot,sizeof(ITEM_SLOT) + (sizeof(ITEM_ATTR) * MAX_ITEM_ATTR));
		}
		
		if(Dungeon.m_cMagicFieldArrayItem.GetID() != 0)
		{
			CItem2*	pItem2	= &Dungeon2.m_cMagicFieldArrayItem;
			CItem*	pItem	= &Dungeon.m_cMagicFieldArrayItem;
			
			memcpy(pItem2,pItem,sizeof(WORD) + sizeof(ITEM_SERIAL));
			memcpy(&pItem2->m_ItemWeapon,&pItem->m_ItemWeapon,sizeof(ITEM_WEAPON));
			memcpy(&pItem2->m_bSlot,&pItem->m_bSlot,sizeof(ITEM_SLOT) + (sizeof(ITEM_ATTR) * MAX_ITEM_ATTR));
		}

		int			nColumn			= 2;	//파라미터 갯수 지정 
		DWORD		dwParamOffset	= 0;	//파라미터 Binding 정보 구조체에서 각 파라미터 Value들의 Offset
		DBBINDING*	pBinding		= g_pDb->CreateParamInfo(nColumn);
		if(!pBinding)	continue;
		
		sprintf(szQuery,"Update MapInfo2 Set GuardianItem=?, MagicArrayItem=? Where [ID]=%d",Dungeon.m_dwID);		
		
		pBinding[0].obValue		= dwParamOffset;	
		pBinding[0].cbMaxLen	= sizeof(Dungeon2.m_cGuardianItem);
		pBinding[0].wType		= DBTYPE_BYTES;
		dwParamOffset			+= sizeof(Dungeon2.m_cGuardianItem);		
		
		pBinding[1].obValue		= dwParamOffset;	
		pBinding[1].cbMaxLen	= sizeof(Dungeon2.m_cMagicFieldArrayItem);
		pBinding[1].wType		= DBTYPE_BYTES;
		dwParamOffset			+= sizeof(Dungeon2.m_cMagicFieldArrayItem);
		
		g_pDb->ExecuteSQLByParam(szQuery,pBinding,&Dungeon2.m_cGuardianItem,nColumn,m_CORUM_GAME_DB[sel]);
		g_pDb->ReleaseParamInfo(pBinding);
	}

	m_pDb->ReleaseRecordset(pData);
	pData = NULL;

	int			PlayerShopCount = 0;
	DWORD		dwCharIndex		= 0;
	PlayerShop	playershop;
	PlayerShop2	playershop2;

	sprintf(szQuery,"Select Count(*) From PlayerShop");
	ret = g_pDb->OpenRecord(szQuery,&PlayerShopCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);
	sprintf(szQuery,"Select Char_Index From PlayerShop");
	pData = g_pDb->OpenRecordEx(szQuery,PlayerShopCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);
	
	if( pData == NULL )
	{
		return;		
	}

	for( i = 0; i < pData->Query.select.dwRowCount; i++ )	
	{
		if(!m_bRun)
		{				
			m_pDb->ReleaseRecordset(pData);					
			return;				
		}
		::Sleep(10);
		
		dwCharIndex = 0;
		memcpy(&dwCharIndex,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize);
		
		ShowQuery.Format("Q(PlayerShop):<%d>[%d].[%d]",pData->Query.select.dwRowCount,i+1,dwCharIndex);
		m_pEdit->SetWindowText(ShowQuery);
		
		sprintf(szQuery,"Select Item1,Item2,Item3,Item4,Item5,Item6,Item7,Item8,Item9,Item10 From PlayerShop Where [Char_Index]=%d",dwCharIndex);
		memset(&playershop,0,sizeof(PlayerShop));
		memset(&playershop2,0,sizeof(PlayerShop2));
		int ret = m_pDb->OpenRecord(szQuery,&playershop,1,m_CORUM_GAME_DB[sel]);
		
		if( ret <= 0 )
		{
			continue;
		}

		for(int j = 0; j < 10; j++)
		{
			if(playershop.m_Item[j].GetID() != 0)
			{
				CItem2* pItem2	= &playershop2.m_Item[j];
				CItem*	pItem	= &playershop.m_Item[j];
				
				memcpy(pItem2,pItem,sizeof(WORD) + sizeof(ITEM_SERIAL));
				memcpy(&pItem2->m_ItemWeapon,&pItem->m_ItemWeapon,sizeof(ITEM_WEAPON));
				memcpy(&pItem2->m_bSlot,&pItem->m_bSlot,sizeof(ITEM_SLOT) + (sizeof(ITEM_ATTR) * MAX_ITEM_ATTR));
			}
		}

		int			nColumn			= 10;	//파라미터 갯수 지정 
		DWORD		dwParamOffset	= 0;	//파라미터 Binding 정보 구조체에서 각 파라미터 Value들의 Offset
		DBBINDING*	pBinding		= g_pDb->CreateParamInfo(nColumn);
		if(!pBinding)	continue;
		
		sprintf(szQuery,"Update PlayerShop2 Set Item1=?, Item2=?, Item3=?, Item4=?, Item5=?, Item6=?, Item7=?, Item8=?, Item9=?, Item10=? Where [Char_Index]=%d",dwCharIndex);		
		
		pBinding[0].obValue		= dwParamOffset;	
		pBinding[0].cbMaxLen	= sizeof(playershop2.m_Item[0]);
		pBinding[0].wType		= DBTYPE_BYTES;
		dwParamOffset			+= sizeof(playershop2.m_Item[0]);		
		
		pBinding[1].obValue		= dwParamOffset;	
		pBinding[1].cbMaxLen	= sizeof(playershop2.m_Item[1]);
		pBinding[1].wType		= DBTYPE_BYTES;
		dwParamOffset			+= sizeof(playershop2.m_Item[1]);

		pBinding[2].obValue		= dwParamOffset;	
		pBinding[2].cbMaxLen	= sizeof(playershop2.m_Item[2]);
		pBinding[2].wType		= DBTYPE_BYTES;
		dwParamOffset			+= sizeof(playershop2.m_Item[2]);

		pBinding[3].obValue		= dwParamOffset;	
		pBinding[3].cbMaxLen	= sizeof(playershop2.m_Item[3]);
		pBinding[3].wType		= DBTYPE_BYTES;
		dwParamOffset			+= sizeof(playershop2.m_Item[3]);

		pBinding[4].obValue		= dwParamOffset;	
		pBinding[4].cbMaxLen	= sizeof(playershop2.m_Item[4]);
		pBinding[4].wType		= DBTYPE_BYTES;
		dwParamOffset			+= sizeof(playershop2.m_Item[4]);

		pBinding[5].obValue		= dwParamOffset;	
		pBinding[5].cbMaxLen	= sizeof(playershop2.m_Item[5]);
		pBinding[5].wType		= DBTYPE_BYTES;
		dwParamOffset			+= sizeof(playershop2.m_Item[5]);

		pBinding[6].obValue		= dwParamOffset;	
		pBinding[6].cbMaxLen	= sizeof(playershop2.m_Item[6]);
		pBinding[6].wType		= DBTYPE_BYTES;
		dwParamOffset			+= sizeof(playershop2.m_Item[6]);

		pBinding[7].obValue		= dwParamOffset;	
		pBinding[7].cbMaxLen	= sizeof(playershop2.m_Item[7]);
		pBinding[7].wType		= DBTYPE_BYTES;
		dwParamOffset			+= sizeof(playershop2.m_Item[7]);

		pBinding[8].obValue		= dwParamOffset;	
		pBinding[8].cbMaxLen	= sizeof(playershop2.m_Item[8]);
		pBinding[8].wType		= DBTYPE_BYTES;
		dwParamOffset			+= sizeof(playershop2.m_Item[8]);

		pBinding[9].obValue		= dwParamOffset;	
		pBinding[9].cbMaxLen	= sizeof(playershop2.m_Item[9]);
		pBinding[9].wType		= DBTYPE_BYTES;
		dwParamOffset			+= sizeof(playershop2.m_Item[9]);
		
		g_pDb->ExecuteSQLByParam(szQuery,pBinding,&playershop2,nColumn,m_CORUM_GAME_DB[sel]);
		g_pDb->ReleaseParamInfo(pBinding);
	}

	m_pDb->ReleaseRecordset(pData);
	pData = NULL;
	
	// 다 끝났다.
	AfxMessageBox("Convert CItem End!!!",MB_OK|MB_ICONINFORMATION);
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CCopyItemDelete::MapInfoInitialize()
{
	char szQuery[0xff]	= {0,};
	int	 sel			= m_ComBoCtrl.GetCurSel();

	CItem2 cItem[2];
	memset(cItem,NULL,sizeof(cItem));
	
	DBBINDING* pBinding = g_pDb->CreateParamInfo(2);	
	if(!pBinding)	return;
	
	pBinding[0].obValue		= 0;
	pBinding[0].cbMaxLen	= sizeof(CItem2);
	pBinding[0].wType		= DBTYPE_BYTES;							
	pBinding[1].obValue		= sizeof(CItem2);
	pBinding[1].cbMaxLen	= sizeof(CItem2);
	pBinding[1].wType		= DBTYPE_BYTES;					
	
	sprintf(szQuery, "Update MapInfo Set GuardianItem = ?, MagicArrayItem = ?");
	m_pDb->ExecuteSQLByParam(szQuery, pBinding, cItem, 2, m_CORUM_GAME_DB[sel]);	
	m_pDb->ReleaseParamInfo(pBinding);
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CCopyItemDelete::SetLevelExp()
{
	char			szQuery[0xff]		= {0,};
	DWORD			dwExp[MAX_LEVEL+1]	= {0,};
	DWORD			nameCount			= 0;
	DWORD			Count				= 0;
	char			Name[20]			= {0,};
	int				sel					= m_ComBoCtrl.GetCurSel();
	CString			ShowQuery;
	CharacterInfo	CharInfo;
	DBRECEIVEDATA*	pData				= NULL;

	int ret = m_pDb->OpenRecord( "Select [EXP] from LevelExp",dwExp,MAX_LEVEL,m_CORUM_GAME_DB[sel] );			
	
	if(ret < 0)
	{
		AfxMessageBox("Fail to load LevelExp!");
		return;
	}

	/*캐릭터 이름*/
	ShowQuery.Format("All CharName Query");
	m_pEdit->SetWindowText(ShowQuery);
	sprintf(szQuery,"SELECT Count(*) FROM CharacterInfo");
	ret = m_pDb->OpenRecord(szQuery,&nameCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);		
	sprintf(szQuery,"SELECT [Name] FROM CharacterInfo");		
	pData = m_pDb->OpenRecordEx(szQuery,nameCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);

	if( pData == NULL )
	{
		return;		
	}

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
		
		sprintf(szQuery,"Select Exp,[Level] From CharacterInfo Where [Name]='%s'",Name);
		memset(&CharInfo,0,sizeof(CharacterInfo));
		ret = m_pDb->OpenRecord(szQuery,&CharInfo,1,m_CORUM_GAME_DB[sel]);
		
		if( ret <= 0 )
		{
			StepProgress();
			continue;
		}

		for( int j = 0; j < MAX_LEVEL; j++ )
		{
			if(CharInfo.dwExp < dwExp[j])
			{
				CharInfo.dwLevel = j;
				break;
			}
		}
		
		int		nColumn			= 1;	
		DWORD	dwParamOffset	= 0;	
		int		nAffected		= 0;
		
		sprintf(szQuery,"Update CharacterInfo Set [Level]=? Where [Name]='%s'",Name);						 
		
		DBBINDING* pBinding = m_pDb->CreateParamInfo(nColumn);
		if(!pBinding) continue;			
		
		pBinding[0].obValue		= dwParamOffset;	
		pBinding[0].cbMaxLen	= sizeof(CharInfo.dwLevel);
		pBinding[0].wType		= DBTYPE_I4;
		dwParamOffset			+= sizeof(CharInfo.dwLevel);		
		
		int affected = m_pDb->ExecuteSQLByParam(szQuery,pBinding,&CharInfo.dwLevel,nColumn,m_CORUM_GAME_DB[sel]);	
		m_pDb->ReleaseParamInfo(pBinding);
	}

	m_pDb->ReleaseRecordset(pData);
	pData = NULL;
	
	m_pProgress->SetPos(0);

	// 다 끝났다.
	AfxMessageBox("Set Level Exp!!!",MB_OK|MB_ICONINFORMATION);
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CCopyItemDelete::SetAttrWeaponArmor()
{
	char				szQuery[0xff]	= {0,};
	char				Name[20]		= {0,};
	char				ID[20]			= {0,};
	DWORD				nameCount		= 0;
	DWORD				bankCount		= 0;
	DBRECEIVEDATA*		pData			= NULL;
	int					sel				= m_ComBoCtrl.GetCurSel();
	CString				ShowQuery;
	CharacterItemAttr	charitemattr;
	BankItemAttr		bankitemattr;

	/*캐릭터 이름*/
	ShowQuery.Format("All CharName Query");
	m_pEdit->SetWindowText(ShowQuery);
	sprintf(szQuery,"SELECT Count(*) FROM CharacterInfo");
	int ret = m_pDb->OpenRecord(szQuery,&nameCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);		
	sprintf(szQuery,"SELECT [Name] FROM CharacterInfo");		
	pData = m_pDb->OpenRecordEx(szQuery,nameCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);
	
	if( pData == NULL )
	{
		return;		
	}
	
	for( DWORD i = 0; i < pData->Query.select.dwRowCount; i++ )	
	{
		BOOL bCharItemChange = FALSE;
		
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
		
		sprintf(szQuery,"Select Equip,Inv_large From CharacterInfo Where [Name]='%s'",Name);
		memset(&charitemattr,0,sizeof(CharacterItemAttr));
		ret = m_pDb->OpenRecord(szQuery,&charitemattr,1,m_CORUM_GAME_DB[sel]);
		
		if( ret <= 0 )
		{
			continue;
		}

		// EQuip
		for( int equip = 0; equip < MAX_EQUIP; equip++ )
		{
			WORD id	= charitemattr.Equip[equip].GetID();			
			if(id != 0)
			{
				WORD kind = charitemattr.Equip[equip].GetKind();
				if(kind == __ITEM_WEAPON__ || kind == __ITEM_AROMR__)
				{
					for( int attr = 0; attr < MAX_ITEM_ATTR; attr++ )	
					{
						if(	 charitemattr.Equip[equip].wAttr[attr].uCode == 34		&&
							(charitemattr.Equip[equip].wAttr[attr].uValue >= 376	&&
							 charitemattr.Equip[equip].wAttr[attr].uValue <= 500		))
						{
							charitemattr.Equip[equip].wAttr[attr].uCode		= 23;
							charitemattr.Equip[equip].wAttr[attr].uValue	= 1;
							
							bCharItemChange = TRUE;
						}
						else if( charitemattr.Equip[equip].wAttr[attr].uCode == 35		&&
								 (charitemattr.Equip[equip].wAttr[attr].uValue >= 1		&&
							     charitemattr.Equip[equip].wAttr[attr].uValue <= 450		))
						{
							charitemattr.Equip[equip].wAttr[attr].uCode		= 23;
							charitemattr.Equip[equip].wAttr[attr].uValue	= 1;
							
							bCharItemChange = TRUE;
						}
						else if( charitemattr.Equip[equip].wAttr[attr].uCode == 38		&&
								(charitemattr.Equip[equip].wAttr[attr].uValue >= 76		&&
								 charitemattr.Equip[equip].wAttr[attr].uValue <= 125		))
						{
							charitemattr.Equip[equip].wAttr[attr].uCode		= 0;
							charitemattr.Equip[equip].wAttr[attr].uValue	= 0;
							
							bCharItemChange = TRUE;
						}
						else if( charitemattr.Equip[equip].wAttr[attr].uCode == 39		&&
							    (charitemattr.Equip[equip].wAttr[attr].uValue >= 276	&&
							     charitemattr.Equip[equip].wAttr[attr].uValue <= 325		))
						{
							charitemattr.Equip[equip].wAttr[attr].uCode		= 0;
							charitemattr.Equip[equip].wAttr[attr].uValue	= 0;
							
							bCharItemChange = TRUE;
						}
						else if( charitemattr.Equip[equip].wAttr[attr].uCode == 44		&&
							    (charitemattr.Equip[equip].wAttr[attr].uValue >= 251	&&
							     charitemattr.Equip[equip].wAttr[attr].uValue <= 300		))
						{
							charitemattr.Equip[equip].wAttr[attr].uCode		= 0;
							charitemattr.Equip[equip].wAttr[attr].uValue	= 0;
							
							bCharItemChange = TRUE;
						}
					}					
				}
			}
		}

		// Inv_Large
		for( int inv_large = 0; inv_large < MAX_INV_LARGE; inv_large++ )
		{
			WORD id	= charitemattr.Inv_Large[inv_large].GetID();			
			if(id != 0)
			{
				WORD kind = charitemattr.Inv_Large[inv_large].GetKind();
				if(kind == __ITEM_WEAPON__ || kind == __ITEM_AROMR__)
				{
					for( int attr = 0; attr < MAX_ITEM_ATTR; attr++ )	
					{
						if(	 charitemattr.Inv_Large[inv_large].wAttr[attr].uCode == 34		&&
							(charitemattr.Inv_Large[inv_large].wAttr[attr].uValue >= 376	&&
							 charitemattr.Inv_Large[inv_large].wAttr[attr].uValue <= 500		))
						{
							charitemattr.Inv_Large[inv_large].wAttr[attr].uCode		= 23;
							charitemattr.Inv_Large[inv_large].wAttr[attr].uValue	= 1;
							
							bCharItemChange = TRUE;
						}
						else if( charitemattr.Inv_Large[inv_large].wAttr[attr].uCode == 35		&&
								 (charitemattr.Inv_Large[inv_large].wAttr[attr].uValue >= 1		&&
								 charitemattr.Inv_Large[inv_large].wAttr[attr].uValue <= 450		))
						{
							charitemattr.Inv_Large[inv_large].wAttr[attr].uCode		= 23;
							charitemattr.Inv_Large[inv_large].wAttr[attr].uValue	= 1;
							
							bCharItemChange = TRUE;
						}
						else if( charitemattr.Inv_Large[inv_large].wAttr[attr].uCode == 38		&&
							    (charitemattr.Inv_Large[inv_large].wAttr[attr].uValue >= 76		&&
							     charitemattr.Inv_Large[inv_large].wAttr[attr].uValue <= 125		))
						{
							charitemattr.Inv_Large[inv_large].wAttr[attr].uCode		= 0;
							charitemattr.Inv_Large[inv_large].wAttr[attr].uValue	= 0;
							
							bCharItemChange = TRUE;
						}
						else if( charitemattr.Inv_Large[inv_large].wAttr[attr].uCode == 39		&&
							    (charitemattr.Inv_Large[inv_large].wAttr[attr].uValue >= 276	&&
							     charitemattr.Inv_Large[inv_large].wAttr[attr].uValue <= 325		))
						{
							charitemattr.Inv_Large[inv_large].wAttr[attr].uCode		= 0;
							charitemattr.Inv_Large[inv_large].wAttr[attr].uValue	= 0;
							
							bCharItemChange = TRUE;
						}
						else if( charitemattr.Inv_Large[inv_large].wAttr[attr].uCode == 44		&&
							    (charitemattr.Inv_Large[inv_large].wAttr[attr].uValue >= 251	&&
							     charitemattr.Inv_Large[inv_large].wAttr[attr].uValue <= 300		))
						{
							charitemattr.Inv_Large[inv_large].wAttr[attr].uCode		= 0;
							charitemattr.Inv_Large[inv_large].wAttr[attr].uValue	= 0;
							
							bCharItemChange = TRUE;
						}
					}					
				}
			}
		}

		// 업데이트 
		if(bCharItemChange)
		{
			int		nColumn			= 2;	
			DWORD	dwParamOffset	= 0;	
			int		nAffected		= 0;
			
			sprintf(szQuery,"Update CharacterInfo Set [Equip]=?,[Inv_large]=? Where [Name]='%s'",Name);						 
			
			DBBINDING* pBinding = m_pDb->CreateParamInfo(nColumn);
			if(!pBinding) continue;	
			
			pBinding[0].obValue		= dwParamOffset;	
			pBinding[0].cbMaxLen	= sizeof(charitemattr.Equip);
			pBinding[0].wType		= DBTYPE_BYTES;
			dwParamOffset			+= sizeof(charitemattr.Equip);				
			
			pBinding[1].obValue		= dwParamOffset;	
			pBinding[1].cbMaxLen	= sizeof(charitemattr.Inv_Large);
			pBinding[1].wType		= DBTYPE_BYTES;
			dwParamOffset			+= sizeof(charitemattr.Inv_Large);		
			
			int affected = m_pDb->ExecuteSQLByParam(szQuery,pBinding,&charitemattr,nColumn,m_CORUM_GAME_DB[sel]);	
			m_pDb->ReleaseParamInfo(pBinding);
		}
	}

	m_pDb->ReleaseRecordset(pData);
	pData = NULL;

	/*창고아이디 이름*/
	ShowQuery.Format("All BankID Query");
	m_pEdit->SetWindowText(ShowQuery);
	
	sprintf(szQuery,"SELECT Count(*) FROM BankInfo");	
	ret = m_pDb->OpenRecord(szQuery,&bankCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);
	sprintf(szQuery,"SELECT [ID] FROM BankInfo");
	pData = m_pDb->OpenRecordEx(szQuery,bankCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);
	
	if( pData == NULL )
	{
		return;		
	}
	
	for( i = 0; i < pData->Query.select.dwRowCount; i++ )	
	{
		BOOL bBankItemChange = FALSE;
		
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
		
		sprintf(szQuery,"Select Bank_Large From BankInfo WHERE [ID]='%s'",ID);
		memset(&bankitemattr,0,sizeof(BankItemAttr));
		int ret = m_pDb->OpenRecord(szQuery,&bankitemattr,1,m_CORUM_GAME_DB[sel]);
		
		if( ret <= 0 )
		{
			continue;
		}
		
		// Bank_Large
		for( int banklarge = 0; banklarge < MAX_BANK_LARGE; banklarge++ )
		{
			WORD id	= bankitemattr.Bank_Large[banklarge].GetID();			
			if(id != 0)
			{
				WORD kind = bankitemattr.Bank_Large[banklarge].GetKind();
				if(kind == __ITEM_WEAPON__ || kind == __ITEM_AROMR__)
				{
					for( int attr = 0; attr < MAX_ITEM_ATTR; attr++ )	
					{
						if(	 bankitemattr.Bank_Large[banklarge].wAttr[attr].uCode == 34		&&
							(bankitemattr.Bank_Large[banklarge].wAttr[attr].uValue >= 376	&&
							 bankitemattr.Bank_Large[banklarge].wAttr[attr].uValue <= 500		))
						{
							bankitemattr.Bank_Large[banklarge].wAttr[attr].uCode	= 23;
							bankitemattr.Bank_Large[banklarge].wAttr[attr].uValue	= 1;
							
							bBankItemChange = TRUE;
						}
						else if( bankitemattr.Bank_Large[banklarge].wAttr[attr].uCode == 35		&&
							    (bankitemattr.Bank_Large[banklarge].wAttr[attr].uValue >= 1		&&
							     bankitemattr.Bank_Large[banklarge].wAttr[attr].uValue <= 450		))
						{
							bankitemattr.Bank_Large[banklarge].wAttr[attr].uCode	= 23;
							bankitemattr.Bank_Large[banklarge].wAttr[attr].uValue	= 1;
							
							bBankItemChange = TRUE;
						}
						else if( bankitemattr.Bank_Large[banklarge].wAttr[attr].uCode == 38		&&
								(bankitemattr.Bank_Large[banklarge].wAttr[attr].uValue >= 76	&&
								bankitemattr.Bank_Large[banklarge].wAttr[attr].uValue <= 125		))
						{
							bankitemattr.Bank_Large[banklarge].wAttr[attr].uCode	= 0;
							bankitemattr.Bank_Large[banklarge].wAttr[attr].uValue	= 0;
							
							bBankItemChange = TRUE;
						}
						else if( bankitemattr.Bank_Large[banklarge].wAttr[attr].uCode == 39		&&
								(bankitemattr.Bank_Large[banklarge].wAttr[attr].uValue >= 276	&&
								bankitemattr.Bank_Large[banklarge].wAttr[attr].uValue <= 325		))
						{
							bankitemattr.Bank_Large[banklarge].wAttr[attr].uCode	= 0;
							bankitemattr.Bank_Large[banklarge].wAttr[attr].uValue	= 0;
							
							bBankItemChange = TRUE;
						}
						else if( bankitemattr.Bank_Large[banklarge].wAttr[attr].uCode == 44		&&
								(bankitemattr.Bank_Large[banklarge].wAttr[attr].uValue >= 251	&&
								bankitemattr.Bank_Large[banklarge].wAttr[attr].uValue <= 300		))
						{
							bankitemattr.Bank_Large[banklarge].wAttr[attr].uCode	= 0;
							bankitemattr.Bank_Large[banklarge].wAttr[attr].uValue	= 0;
							
							bBankItemChange = TRUE;
						}
					}					
				}
			}
		}
		
		// 업데이트 
		if(bBankItemChange)
		{
			int		nColumn			= 1;	
			DWORD	dwParamOffset	= 0;	
			
			sprintf(szQuery,"Update Bankinfo Set Bank_Large=? Where [ID]='%s'",ID);	
			
			DBBINDING* pBinding = m_pDb->CreateParamInfo(nColumn);		
			if(!pBinding) continue;
			
			pBinding[0].obValue		= dwParamOffset;	
			pBinding[0].cbMaxLen	= sizeof(bankitemattr.Bank_Large);
			pBinding[0].wType		= DBTYPE_BYTES;
			dwParamOffset			+= sizeof(bankitemattr.Bank_Large);		
			
			int affected = m_pDb->ExecuteSQLByParam(szQuery,pBinding,&bankitemattr,nColumn,m_CORUM_GAME_DB[sel]);	
			m_pDb->ReleaseParamInfo(pBinding);
		}		
	}

	m_pDb->ReleaseRecordset(pData);
	pData = NULL;
	
	// 다 끝났다.
	AfxMessageBox("Set Attr Weapon Armor End!!!",MB_OK|MB_ICONINFORMATION);
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CCopyItemDelete::SetAttrArmor()
{
	char				szQuery[0xff]	= {0,};
	char				Name[20]		= {0,};
	char				ID[20]			= {0,};
	DWORD				nameCount		= 0;
	DWORD				bankCount		= 0;
	DBRECEIVEDATA*		pData			= NULL;
	int					sel				= m_ComBoCtrl.GetCurSel();
	CharacterItemArmor	CharItem;
	BankItemArmor		BankItem;	
	CString				ShowQuery;

	/*캐릭터 이름*/
	ShowQuery.Format("All CharName Query");
	m_pEdit->SetWindowText(ShowQuery);
	sprintf(szQuery,"SELECT Count(*) FROM CharacterInfo");
	int ret = m_pDb->OpenRecord(szQuery,&nameCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);		
	sprintf(szQuery,"SELECT [Name] FROM CharacterInfo");		
	pData = m_pDb->OpenRecordEx(szQuery,nameCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);

	if( pData == NULL )
	{
		return;		
	}

	for( DWORD i = 0; i < pData->Query.select.dwRowCount; i++ )	
	{
		BOOL bCharItemChange = FALSE;

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

		sprintf(szQuery,"Select Money,Equip,Inv_large From CharacterInfo Where [Name]='%s'",Name);
		memset(&CharItem,0,sizeof(CharacterItemArmor));
		ret = m_pDb->OpenRecord(szQuery,&CharItem,1,m_CORUM_GAME_DB[sel]);

		if( ret <= 0 )
		{
			StepProgress();
			continue;
		}

		if(CharItem.dwMoney != 0)
		{
			CharItem.dwMoney = CharItem.dwMoney / 100;
		}

		// EQuip
		for( int equip = 0; equip < MAX_EQUIP; equip++ )
		{
			WORD id		= CharItem.Equip[equip].GetID();
			
			if(id != 0)
			{
				WORD kind	= CharItem.Equip[equip].GetKind();

				if(kind == __ITEM_AROMR__)
				{
					WORD nValue = 0;
					
					for( int attr = 0; attr < MAX_ITEM_ATTR; attr++ )	
					{
						if	(	CharItem.Equip[equip].wAttr[attr].uCode == ITEM_ATTR_FIRERESIST		||
								CharItem.Equip[equip].wAttr[attr].uCode == ITEM_ATTR_LIGHTRESIST	||
								CharItem.Equip[equip].wAttr[attr].uCode == ITEM_ATTR_ICERESIST		||
								CharItem.Equip[equip].wAttr[attr].uCode == ITEM_ATTR_POIRESIST		||
								CharItem.Equip[equip].wAttr[attr].uCode == ITEM_ATTR_ALLRESIST		||
								CharItem.Equip[equip].wAttr[attr].uCode == ITEM_ATTR_PHYRESIST								
							)
						{
							nValue += CharItem.Equip[equip].wAttr[attr].uValue;
							CharItem.Equip[equip].wAttr[attr].uCode		= 0;
							CharItem.Equip[equip].wAttr[attr].uValue	= 0;
							
							bCharItemChange = TRUE;
						}
					}

					CharItem.Equip[equip].m_Item_Armor.wDefense += nValue;
				}
			}
		}
		
		// Inv_Large
		for( int inv_large = 0; inv_large < MAX_INV_LARGE; inv_large++ )
		{
			WORD id		= CharItem.Inv_Large[inv_large].GetID();		
			
			if(id != 0)
			{
				WORD kind	= CharItem.Inv_Large[inv_large].GetKind();
				
				if(kind == __ITEM_AROMR__)
				{
					WORD nValue = 0;
					for( int attr = 0; attr < MAX_ITEM_ATTR; attr++ )	
					{
						if	(	CharItem.Inv_Large[inv_large].wAttr[attr].uCode == ITEM_ATTR_FIRERESIST		||
								CharItem.Inv_Large[inv_large].wAttr[attr].uCode == ITEM_ATTR_LIGHTRESIST	||
								CharItem.Inv_Large[inv_large].wAttr[attr].uCode == ITEM_ATTR_ICERESIST		||
								CharItem.Inv_Large[inv_large].wAttr[attr].uCode == ITEM_ATTR_POIRESIST		||
								CharItem.Inv_Large[inv_large].wAttr[attr].uCode == ITEM_ATTR_ALLRESIST		||
								CharItem.Inv_Large[inv_large].wAttr[attr].uCode == ITEM_ATTR_PHYRESIST		
							)
						{
							nValue += CharItem.Inv_Large[inv_large].wAttr[attr].uValue;
							CharItem.Inv_Large[inv_large].wAttr[attr].uCode		= 0;
							CharItem.Inv_Large[inv_large].wAttr[attr].uValue	= 0;
							
							bCharItemChange = TRUE;
						}
					}
					
					CharItem.Inv_Large[inv_large].m_Item_Armor.wDefense += nValue;
				}				
			}
		}

		// 업데이트 
		if(bCharItemChange)
		{
			int		nColumn			= 3;	
			DWORD	dwParamOffset	= 0;	
			int		nAffected		= 0;
			
			sprintf(szQuery,"Update CharacterInfo Set [Money]=?,[Equip]=?,[Inv_large]=? Where [Name]='%s'",Name);						 
			
			DBBINDING* pBinding = m_pDb->CreateParamInfo(nColumn);
			if(!pBinding) continue;	
			
			pBinding[0].obValue		= dwParamOffset;	
			pBinding[0].cbMaxLen	= sizeof(CharItem.dwMoney);
			pBinding[0].wType		= DBTYPE_I4;
			dwParamOffset			+= sizeof(CharItem.dwMoney);
			
			pBinding[1].obValue		= dwParamOffset;	
			pBinding[1].cbMaxLen	= sizeof(CharItem.Equip);
			pBinding[1].wType		= DBTYPE_BYTES;
			dwParamOffset			+= sizeof(CharItem.Equip);				
			
			pBinding[2].obValue		= dwParamOffset;	
			pBinding[2].cbMaxLen	= sizeof(CharItem.Inv_Large);
			pBinding[2].wType		= DBTYPE_BYTES;
			dwParamOffset			+= sizeof(CharItem.Inv_Large);		
			
			int affected = m_pDb->ExecuteSQLByParam(szQuery,pBinding,&CharItem,nColumn,m_CORUM_GAME_DB[sel]);	
			m_pDb->ReleaseParamInfo(pBinding);
		}
				
		StepProgress();		
	}

	m_pDb->ReleaseRecordset(pData);
	pData = NULL;

	m_pProgress->SetPos(0);
	
	/*창고아이디 이름*/
	ShowQuery.Format("All BankID Query");
	m_pEdit->SetWindowText(ShowQuery);
	
	sprintf(szQuery,"SELECT Count(*) FROM BankInfo");	
	ret = m_pDb->OpenRecord(szQuery,&bankCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);
	sprintf(szQuery,"SELECT [ID] FROM BankInfo");
	pData = m_pDb->OpenRecordEx(szQuery,bankCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);

	if( pData == NULL )
	{
		return;		
	}
	
	for( i = 0; i < pData->Query.select.dwRowCount; i++ )	
	{
		BOOL bBankItemChange = FALSE;

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
		
		sprintf(szQuery,"Select Bank_Large,Money From BankInfo WHERE [ID]='%s'",ID);
		memset(&BankItem,0,sizeof(BankItemArmor));
		int ret = m_pDb->OpenRecord(szQuery,&BankItem,1,m_CORUM_GAME_DB[sel]);
		
		if( ret <= 0 )
		{
			StepProgress();
			continue;
		}

		if(BankItem.dwMoney != 0)
		{
			BankItem.dwMoney = BankItem.dwMoney / 100;
		}

		// Bank_Large
		for( int banklarge = 0; banklarge < MAX_BANK_LARGE; banklarge++ )
		{
			WORD id		= BankItem.Bank_Large[banklarge].GetID();		
						
			if(id != 0)
			{
				WORD kind	= BankItem.Bank_Large[banklarge].GetKind();

				if(kind == __ITEM_AROMR__)
				{
					WORD nValue = 0;
					for( int attr = 0; attr < MAX_ITEM_ATTR; attr++ )	
					{
						if	(	BankItem.Bank_Large[banklarge].wAttr[attr].uCode == ITEM_ATTR_FIRERESIST	||
								BankItem.Bank_Large[banklarge].wAttr[attr].uCode == ITEM_ATTR_LIGHTRESIST	||
								BankItem.Bank_Large[banklarge].wAttr[attr].uCode == ITEM_ATTR_ICERESIST		||
								BankItem.Bank_Large[banklarge].wAttr[attr].uCode == ITEM_ATTR_POIRESIST		||
								BankItem.Bank_Large[banklarge].wAttr[attr].uCode == ITEM_ATTR_ALLRESIST		||
								BankItem.Bank_Large[banklarge].wAttr[attr].uCode == ITEM_ATTR_PHYRESIST		
							)
						{
							nValue += BankItem.Bank_Large[banklarge].wAttr[attr].uValue;
							BankItem.Bank_Large[banklarge].wAttr[attr].uCode	= 0;
							BankItem.Bank_Large[banklarge].wAttr[attr].uValue	= 0;
							bBankItemChange = TRUE;
						}
					}
					
					BankItem.Bank_Large[banklarge].m_Item_Armor.wDefense += nValue;
				}
			}
		}

		// 업데이트 
		if(bBankItemChange)
		{
			int		nColumn			= 2;	
			DWORD	dwParamOffset	= 0;	
			
			sprintf(szQuery,"Update Bankinfo Set Bank_Large=?, Money=? Where [ID]='%s'",ID);	
			
			DBBINDING* pBinding = m_pDb->CreateParamInfo(nColumn);		
			if(!pBinding) continue;
			
			pBinding[0].obValue		= dwParamOffset;	
			pBinding[0].cbMaxLen	= sizeof(BankItem.Bank_Large);
			pBinding[0].wType		= DBTYPE_BYTES;
			dwParamOffset			+= sizeof(BankItem.Bank_Large);		

			pBinding[1].obValue		= dwParamOffset;	
			pBinding[1].cbMaxLen	= sizeof(BankItem.dwMoney);
			pBinding[1].wType		= DBTYPE_I4;
			dwParamOffset			+= sizeof(BankItem.dwMoney);		
			
			int affected = m_pDb->ExecuteSQLByParam(szQuery,pBinding,&BankItem,nColumn,m_CORUM_GAME_DB[sel]);	
			m_pDb->ReleaseParamInfo(pBinding);
		}		
		
		StepProgress();	
	}

	m_pDb->ReleaseRecordset(pData);
	pData = NULL;
	
	m_pProgress->SetPos(0);

	// 다 끝났다.
	AfxMessageBox("Set Attr Armor End!!!",MB_OK|MB_ICONINFORMATION);
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CCopyItemDelete::DeleteItemmall()
{
	char			szQuery[0xff]	= {0,};
	char			ID[20]			= {0,};
	CString			ShowQuery;
	DWORD			bankCount		= 0;
	DBRECEIVEDATA*	pData			= NULL;
	int				sel				= m_ComBoCtrl.GetCurSel();
	CHAR_INV_DATA	AllChar[4];
	CHAR_BANK_DATA	BankData;
	CItem2			cItem;
	DWORD			dwSerialNum		= 0;

	memset(&cItem,0,sizeof(CItem2));
	cItem.m_wItemID					= 45;
	cItem.m_Serial.bServerSet		= 255;
	cItem.m_Serial.bServerSerialID	= 0;
	cItem.m_Serial.wRestartCount	= 1;
	cItem.m_Serial.dwSerialNum		= 0;
	cItem.m_ItemWeapon.bClassGrade	= 2;
	cItem.m_ItemWeapon.bMin_Level	= 0;
	cItem.m_ItemWeapon.wMin_Str		= 0;
	cItem.m_ItemWeapon.wMin_Damage	= 15;
	cItem.m_ItemWeapon.wMax_Damage	= 21;
	cItem.m_bSlot.uMaxSlot			= 10;
	cItem.wAttr[0].uCode			= 31;
	cItem.wAttr[0].uValue			= 5;
	cItem.wAttr[1].uCode			= 23;
	cItem.wAttr[1].uValue			= 1;

	/*창고아이디 이름*/
	ShowQuery.Format("All BankID Query");
	m_pEdit->SetWindowText(ShowQuery);
	
	sprintf(szQuery,"Select Count(*) From BankInfo");	
	int ret = m_pDb->OpenRecord(szQuery,&bankCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);
	sprintf(szQuery,"Select [ID] From BankInfo");
	pData = m_pDb->OpenRecordEx(szQuery,bankCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);

	if( pData == NULL )
	{
		return;
	}

	for( DWORD i = 0; i < pData->Query.select.dwRowCount; i++ )	
	{
		if(!m_bRun)
		{				
			m_pDb->ReleaseRecordset(pData);					
			return;				
		}
		::Sleep(10);
		
		memset(ID,0,sizeof(ID));				
		memcpy(ID,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize);

		ShowQuery.Format("Q(ID):<%d>[%d].[%s]",pData->Query.select.dwRowCount,i+1,ID);
		m_pEdit->SetWindowText(ShowQuery);
		
		memset(AllChar, 0, sizeof(AllChar));
		sprintf(szQuery,"Select Name,Char_Index,Equip,Inv_Large From CharacterInfo Where ID = '%s'",ID);		
		ret = g_pDb->OpenRecord(szQuery,AllChar,4,m_CORUM_GAME_DB[sel]);

		if(ret <= 0)
			continue;

		BOOL bChk = FALSE;

		for( DWORD j = 0; j < ret; j++ )
		{
			for( int equip = 0; equip < MAX_EQUIP; equip++ )
			{
				if(AllChar[j].cEquip[equip].GetID() == 45)
				{
					memset(&AllChar[j].cEquip[equip], 0, sizeof(CItem2));
					bChk = TRUE;
				}
			}

			for( int inv_large = 0; inv_large < MAX_INV_LARGE; inv_large++ )
			{
				if(AllChar[j].cLarge[inv_large].GetID() == 45)
				{
					memset(&AllChar[j].cLarge[inv_large], 0, sizeof(CItem2));
					bChk = TRUE;
				}
			}
		}		

		memset(&BankData,0,sizeof(CHAR_BANK_DATA));
		sprintf(szQuery,"Select Bank_large From BankInfo Where ID = '%s'",ID);
		g_pDb->OpenRecord(szQuery,&BankData,1,m_CORUM_GAME_DB[sel]);

		for( int bank_large = 0; bank_large < MAX_BANK_LARGE; bank_large++ )
		{
			if(BankData.cBank[bank_large].GetID() == 45)
			{
				memset(&BankData.cBank[bank_large],0,sizeof(CItem2));
				bChk = TRUE;
			}
		}

		// 업데이트 
		if(bChk)
		{
			for( int update = 0; update < ret; update++ )
			{
				int		nColumn			= 2;	
				DWORD	dwParamOffset	= 0;	
				int		nAffected		= 0;
				
				sprintf(szQuery,"Update CharacterInfo Set [Equip]=?,[Inv_large]=? Where [Name]='%s'",AllChar[update].Name);						 
				
				DBBINDING* pBinding = m_pDb->CreateParamInfo(nColumn);
				if(!pBinding) continue;	
				
				pBinding[0].obValue		= dwParamOffset;	
				pBinding[0].cbMaxLen	= sizeof(AllChar[update].cEquip);
				pBinding[0].wType		= DBTYPE_BYTES;
				dwParamOffset			+= sizeof(AllChar[update].cEquip);
				
				pBinding[1].obValue		= dwParamOffset;	
				pBinding[1].cbMaxLen	= sizeof(AllChar[update].cLarge);
				pBinding[1].wType		= DBTYPE_BYTES;
				dwParamOffset			+= sizeof(AllChar[update].cLarge);				
				
				int affected = m_pDb->ExecuteSQLByParam(szQuery,pBinding,AllChar[update].cEquip,nColumn,m_CORUM_GAME_DB[sel]);	
				m_pDb->ReleaseParamInfo(pBinding);
			}

			for( int bank_large = 0; bank_large < MAX_BANK_LARGE; bank_large++ )
			{
				if(BankData.cBank[bank_large].GetID() == 0)
				{
					cItem.m_Serial.dwSerialNum = dwSerialNum;
					memcpy(&BankData.cBank[bank_large],&cItem,sizeof(CItem2));
					dwSerialNum = dwSerialNum + 1;
					break;
				}
			}
						
			int		nColumn			= 1;	
			DWORD	dwParamOffset	= 0;	
			
			sprintf(szQuery,"Update Bankinfo Set Bank_Large=? Where [ID]='%s'",ID);	
			
			DBBINDING* pBinding = m_pDb->CreateParamInfo(nColumn);		
			if(!pBinding) continue;
			
			pBinding[0].obValue		= dwParamOffset;	
			pBinding[0].cbMaxLen	= sizeof(BankData.cBank);
			pBinding[0].wType		= DBTYPE_BYTES;
			dwParamOffset			+= sizeof(BankData.cBank);		
			
			int affected = m_pDb->ExecuteSQLByParam(szQuery,pBinding,&BankData,nColumn,m_CORUM_GAME_DB[sel]);	
			m_pDb->ReleaseParamInfo(pBinding);
		}			
	}

	m_pDb->ReleaseRecordset(pData);
	pData = NULL;

	AfxMessageBox("Delete Itemmall End!!!",MB_OK|MB_ICONINFORMATION);
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CCopyItemDelete::ConvertGuardianInfo()
{
	char			szQuery[1024]	= {0,};
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
	CharacterGuardian	CharItem;
	GUARDIAN_INFO	GuardianInfo;
	BankGuardian	BankInfo;
	int				sel				= m_ComBoCtrl.GetCurSel();
	CTime			start			= CTime::GetCurrentTime();
	BOOL			charNoOk		= FALSE;
	BOOL			bankNoOk        = FALSE;
	WORD			wKind			= __ITEM_GUARDIAN__;
	BYTE			byServerSet		= NABU;

	/*캐릭터 이름*/
	ShowQuery.Format("All CharName Query");
	m_pEdit->SetWindowText(ShowQuery);
	sprintf(szQuery,"SELECT Count(*) FROM CharacterInfo");
	int ret = m_pDb->OpenRecord(szQuery,&nameCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);		
	sprintf(szQuery,"SELECT [Name] FROM CharacterInfo");		
	pData = m_pDb->OpenRecordEx(szQuery,nameCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);

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
			
			sprintf(szQuery,"Select Inv_large,Guardian From CharacterInfo Where [Name]='%s'",Name);
			memset(&CharItem,0,sizeof(CharacterGuardian));
			ret = m_pDb->OpenRecord(szQuery,&CharItem,1,m_CORUM_GAME_DB[sel]);

			if( ret <= 0 )
			{
				continue;
			}

			// Inv_Large
			for( int inv_large = 0; inv_large < MAX_INV_LARGE; inv_large++ )
			{
				WORD id	= CharItem.Inv_Large[inv_large].GetID();
				if(id != 0)
				{
					WORD kind = CharItem.Inv_Large[inv_large].GetKind();
					if(kind == wKind)
					{
						// 가디언 이다.
						// 디비에 생성 되었나?
						if(CharItem.Inv_Large[inv_large].m_Item_Guardian.bCreate)
						{
							sprintf(szQuery,"Select * From GuardianInfo Where ID = %d",CharItem.Inv_Large[inv_large].m_Item_Guardian.dwID);
							memset(&GuardianInfo,0,sizeof(GUARDIAN_INFO));
							ret = m_pDb->OpenRecord(szQuery,&GuardianInfo,1,m_CORUM_GAME_DB[sel]);
							if(ret <= 0) continue;							
							sprintf(szQuery,"Insert into GuardianInfoConvert(ID, Name, BaseIndex, [Level], Exp, HP, MP, ServerSet, ServerSerialID, RestartCount, SerialNum) values(%u,'%s',%u,%u,%u,%u,%u,%u,%u,%u,%u)",
											GuardianInfo.dwID,
											GuardianInfo.szName,
											GuardianInfo.dwKind,
											GuardianInfo.dwLevel,
											GuardianInfo.dwExp,
											GuardianInfo.dwHP,
											GuardianInfo.dwMP,
											CharItem.Inv_Large[inv_large].m_Serial.bServerSet,
											CharItem.Inv_Large[inv_large].m_Serial.bServerSerialID,
											CharItem.Inv_Large[inv_large].m_Serial.wRestartCount,
											CharItem.Inv_Large[inv_large].m_Serial.dwSerialNum);
							ret = g_pDb->ExecuteSQL(szQuery,m_CORUM_GAME_DB[byServerSet]);
						}
					}
				}
			}

			// Guardian
			for( int guardian = 0; guardian < MAX_GUARDIAN; guardian++ )
			{
				WORD id	= CharItem.Guardian[guardian].GetID();
				if(id != 0)
				{
					WORD kind = CharItem.Guardian[guardian].GetKind();
					if(kind == wKind)
					{
						// 가디언 이다.
						// 디비에 생성 되었나?
						if(CharItem.Guardian[guardian].m_Item_Guardian.bCreate)
						{
							sprintf(szQuery,"Select * From GuardianInfo Where ID = %d",CharItem.Guardian[guardian].m_Item_Guardian.dwID);
							memset(&GuardianInfo,0,sizeof(GUARDIAN_INFO));
							ret = m_pDb->OpenRecord(szQuery,&GuardianInfo,1,m_CORUM_GAME_DB[sel]);
							if(ret <= 0) continue;
							
							sprintf(szQuery,"Insert into GuardianInfoConvert(ID, Name, BaseIndex, [Level], Exp, HP, MP, ServerSet, ServerSerialID, RestartCount, SerialNum) values(%u,'%s',%u,%u,%u,%u,%u,%u,%u,%u,%u)",
											GuardianInfo.dwID,
											GuardianInfo.szName,
											GuardianInfo.dwKind,
											GuardianInfo.dwLevel,
											GuardianInfo.dwExp,
											GuardianInfo.dwHP,
											GuardianInfo.dwMP,
											CharItem.Guardian[guardian].m_Serial.bServerSet,
											CharItem.Guardian[guardian].m_Serial.bServerSerialID,
											CharItem.Guardian[guardian].m_Serial.wRestartCount,
											CharItem.Guardian[guardian].m_Serial.dwSerialNum);
							ret = m_pDb->ExecuteSQL(szQuery,m_CORUM_GAME_DB[byServerSet]);
						}
					}
				}
			}
		}
	}

	m_pDb->ReleaseRecordset(pData);
	pData = NULL;

	/*창고아이디 이름*/
	ShowQuery.Format("All BankID Query");
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
			
			sprintf(szQuery,"SELECT Bank_Large FROM BankInfo WHERE [ID]='%s'",ID);
			memset(&BankInfo,0,sizeof(BankGuardian));
			int ret = m_pDb->OpenRecord(szQuery,&BankInfo,1,m_CORUM_GAME_DB[sel]);
			
			if( ret <= 0 )
			{
				continue;
			}
			
			// Bank_Large
			for( int banklarge = 0; banklarge < MAX_BANK_LARGE; banklarge++ )
			{
				WORD id	= BankInfo.Bank_Large[banklarge].GetID();
				if(id != 0)
				{
					WORD kind = BankInfo.Bank_Large[banklarge].GetKind();
					if(kind == wKind)
					{
						// 가디언 이다.
						// 디비에 생성 되었나?
						if(BankInfo.Bank_Large[banklarge].m_Item_Guardian.bCreate)
						{
							sprintf(szQuery,"Select * From GuardianInfo Where ID = %d",BankInfo.Bank_Large[banklarge].m_Item_Guardian.dwID);
							memset(&GuardianInfo,0,sizeof(GUARDIAN_INFO));
							ret = m_pDb->OpenRecord(szQuery,&GuardianInfo,1,m_CORUM_GAME_DB[sel]);
							if(ret <= 0) continue;							
							sprintf(szQuery,"Insert into GuardianInfoConvert (ID, Name, BaseIndex, [Level], Exp, HP, MP, ServerSet, ServerSerialID, RestartCount, SerialNum) values(%u,'%s',%u,%u,%u,%u,%u,%u,%u,%u,%u)",
											GuardianInfo.dwID,
											GuardianInfo.szName,
											GuardianInfo.dwKind,
											GuardianInfo.dwLevel,
											GuardianInfo.dwExp,
											GuardianInfo.dwHP,
											GuardianInfo.dwMP,
											BankInfo.Bank_Large[banklarge].m_Serial.bServerSet,
											BankInfo.Bank_Large[banklarge].m_Serial.bServerSerialID,
											BankInfo.Bank_Large[banklarge].m_Serial.wRestartCount,
											BankInfo.Bank_Large[banklarge].m_Serial.dwSerialNum);
							ret = g_pDb->ExecuteSQL(szQuery,m_CORUM_GAME_DB[byServerSet]);
						}
					}
				}
			}
		}
	}

	m_pDb->ReleaseRecordset(pData);
	pData = NULL;
	
	AfxMessageBox("Convert GuardianInfo End!!!",MB_OK|MB_ICONINFORMATION);
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CCopyItemDelete::FindItem()
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
	BYTE			byServerSet		= 12;
	DWORD			dwFuckItemCount = 0;
	BOOL			charNoOk		= FALSE;
	BOOL			bankNoOk        = FALSE;
	
	CString	folder;
	folder.Format("%s\\FuckItem",g_szExePath);
	
	CFileFind	finder;
	BOOL		bOK = FALSE;
	bOK				= finder.FindFile( folder );
	
	// 폴더가 있으면 
	if( bOK )
	{
		while(bOK)
		{
			bOK = finder.FindNextFile();
			if(!finder.IsDirectory())	
			{
				// 폴더명과 똑같은 파일이 있는데(확장자 없는) 폴더가 아닐경우
				::CreateDirectory( (LPCTSTR)folder,NULL );
				break;
			}
		}		
	}
	// 폴더가 없으면 
	else
	{
		::CreateDirectory( (LPCTSTR)folder,NULL );
	}

	CString szServerName;
	m_ComBoCtrl.GetLBText(sel,szServerName);
	
	SYSTEMTIME tm;
	::GetLocalTime(&tm);
	
	CString szFile;
	szFile.Format("%s\\[%s][%d.%d.%d %d.%d.%d].txt",folder,szServerName,tm.wYear,tm.wMonth,tm.wDay,tm.wHour,tm.wMinute,tm.wSecond);
	
	FILE* pFile = NULL;
	pFile = fopen(szFile,"at");
	
	char tszTemp[256] = {0,};
	fprintf(pFile, "*============================================================*\n");
	fprintf(pFile, "\tDate / Time\t\t\t: %s   ", _strdate(tszTemp));
	fprintf(pFile, "%s\n", _strtime(tszTemp));
	fprintf(pFile, "*------------------------------------------------------------*\n");

	/*캐릭터 이름*/
	ShowQuery.Format("All CharName Query");
	m_pEdit->SetWindowText(ShowQuery);
	sprintf(szQuery,"SELECT Count(*) FROM CharacterInfo");
	int ret = m_pDb->OpenRecord(szQuery,&nameCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);		
	sprintf(szQuery,"SELECT [Name] FROM CharacterInfo");		
	pData = m_pDb->OpenRecordEx(szQuery,nameCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);	
	
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

			BOOL bFindFuck = FALSE;
			
			memset(Name,0,sizeof(Name));					
			memcpy(Name,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize);
			
			ShowQuery.Format("Q(CharacterInfo):<%d>[%d].[%s]",pData->Query.select.dwRowCount,i+1,Name);
			m_pEdit->SetWindowText(ShowQuery);
			
			sprintf(szQuery,"Select Equip,Inv_large,Inv_small,Belt,Guardian From CharacterInfo Where [Name]='%s'",Name);
			memset(&CharItem,0,sizeof(CharacterItem2));
			ret = m_pDb->OpenRecord(szQuery,&CharItem,1,m_CORUM_GAME_DB[sel]);
			if( ret <= 0 )	continue;

			// EQuip
			for( int equip = 0; equip < MAX_EQUIP; equip++ )
			{
				WORD id	= CharItem.Equip[equip].GetID();
				if(id == 0)	continue;
				
				if(	CharItem.Equip[equip].m_Serial.bServerSet != byServerSet )					
				{
					if(	CharItem.Equip[equip].m_Serial.bServerSet != 255 &&
						CharItem.Equip[equip].m_Serial.bServerSet != 254 &&
						CharItem.Equip[equip].m_Serial.bServerSet != 253 )
					{
						fprintf(pFile,"'%s'[Equip][%d][%d]\n",Name,equip,CharItem.Equip[equip].m_Serial.bServerSet);
						dwFuckItemCount++;	
						bFindFuck = TRUE;
						memset(&CharItem.Equip[equip],0,sizeof(CItem2));
					}
				}
			}			

			// Inv_Large
			for( int inv_large = 0; inv_large < MAX_INV_LARGE; inv_large++ )
			{
				WORD id	= CharItem.Inv_Large[inv_large].GetID();
				if(id == 0)	continue;
				
				if(	CharItem.Inv_Large[inv_large].m_Serial.bServerSet != byServerSet )
				{
					if(	CharItem.Inv_Large[inv_large].m_Serial.bServerSet != 255 &&
						CharItem.Inv_Large[inv_large].m_Serial.bServerSet != 254 &&
						CharItem.Inv_Large[inv_large].m_Serial.bServerSet != 253 )
					{
						fprintf(pFile,"'%s'[Inv Large][%d][%d]\n",Name,inv_large,CharItem.Inv_Large[inv_large].m_Serial.bServerSet);
						dwFuckItemCount++;
						bFindFuck = TRUE;
						memset(&CharItem.Inv_Large[inv_large],0,sizeof(CItem2));
					}
				}
			}			

			// Inv_Small
			for( int inv_small = 0; inv_small < MAX_INV_SMALL; inv_small++ )
			{
				WORD id	= CharItem.Inv_Small[inv_small].GetID();
				if(id == 0)	continue;
				
				if(	CharItem.Inv_Small[inv_small].m_Serial.bServerSet != byServerSet )
				{
					if(	CharItem.Inv_Small[inv_small].m_Serial.bServerSet != 255 &&
						CharItem.Inv_Small[inv_small].m_Serial.bServerSet != 254 &&
						CharItem.Inv_Small[inv_small].m_Serial.bServerSet != 253 )
					{
						fprintf(pFile,"'%s'[Inv Small][%d][%d]\n",Name,inv_small,CharItem.Inv_Small[inv_small].m_Serial.bServerSet);
						dwFuckItemCount++;
						bFindFuck = TRUE;
						memset(&CharItem.Inv_Small[inv_small],0,sizeof(CItem2));
					}
				}
			}			

			// Belt
			for( int belt = 0; belt < MAX_BELT; belt++ )
			{
				WORD id = CharItem.Belt[belt].GetID();
				if(id == 0) continue;

				if(	CharItem.Belt[belt].m_Serial.bServerSet != byServerSet )			
				{
					if(	CharItem.Belt[belt].m_Serial.bServerSet != 255 &&
						CharItem.Belt[belt].m_Serial.bServerSet != 254 &&
						CharItem.Belt[belt].m_Serial.bServerSet != 253 )
					{
						fprintf(pFile,"'%s'[Belt][%d][%d]\n",Name,belt,CharItem.Belt[belt].m_Serial.bServerSet);
						dwFuckItemCount++;
						bFindFuck = TRUE;
						memset(&CharItem.Belt[belt],0,sizeof(CItem2));
					}
				}
			}
				
			// Guardian
			for( int guardian = 0; guardian < MAX_GUARDIAN; guardian++ )
			{
				WORD id	= CharItem.Guardian[guardian].GetID();
				if(id == 0)	continue;	
				
				if(	CharItem.Belt[guardian].m_Serial.bServerSet != byServerSet )					
				{
					if(	CharItem.Guardian[guardian].m_Serial.bServerSet != 255 &&
						CharItem.Guardian[guardian].m_Serial.bServerSet != 254 &&
						CharItem.Guardian[guardian].m_Serial.bServerSet != 253 )
					{
						fprintf(pFile,"'%s'[Guardian][%d][%d]\n",Name,guardian,CharItem.Guardian[guardian].m_Serial.bServerSet);
						dwFuckItemCount++;
						bFindFuck = TRUE;
						memset(&CharItem.Guardian[guardian],0,sizeof(CItem2));
					}
				}
			}

			if( bFindFuck )
			{
				// 업데이트 
				int		nColumn			= 5;	
				DWORD	dwParamOffset	= 0;	
				int		nAffected		= 0;
				
				sprintf(szQuery,"Update CharacterInfo Set [Equip]=?,[Inv_large]=?,[Inv_small]=?,[Belt]=?,"
								"[Guardian]=? Where [Name]='%s'",Name);
				
				DBBINDING* pBinding = m_pDb->CreateParamInfo(nColumn);
				if(!pBinding)	continue;				
				
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
				m_pDb->ReleaseParamInfo(pBinding);
			}
		}
	}

	m_pDb->ReleaseRecordset(pData);
	pData = NULL;

	/*창고아이디 이름*/
	ShowQuery.Format("All BankID Query");
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

			BOOL bFindFuck = FALSE;
			
			memset(	ID,0,sizeof(ID)	);				
			memcpy(	ID,pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ),pData->Query.select.dwRowSize);
			
			ShowQuery.Format("Q(Bank):<%d>[%d].[%s]",pData->Query.select.dwRowCount,i+1,ID);
			m_pEdit->SetWindowText(ShowQuery);
			
			sprintf(szQuery,"SELECT Bank_Large,Bank_Small,[Money] FROM BankInfo WHERE [ID]='%s'",ID);
			memset(&BankInfo,0,sizeof(BankItem2));
			int ret = m_pDb->OpenRecord(szQuery,&BankInfo,1,m_CORUM_GAME_DB[sel]);
			if( ret <= 0 )	continue;
			
			// Bank_Large
			for( int banklarge = 0; banklarge < MAX_BANK_LARGE; banklarge++ )
			{
				WORD id	= BankInfo.Bank_Large[banklarge].GetID();
				if(id == 0)	continue;
				
				if(BankInfo.Bank_Large[banklarge].m_Serial.bServerSet != byServerSet)
				{
					if(	BankInfo.Bank_Large[banklarge].m_Serial.bServerSet != 255 ||
						BankInfo.Bank_Large[banklarge].m_Serial.bServerSet != 254 ||
						BankInfo.Bank_Large[banklarge].m_Serial.bServerSet != 253 )
					{
						fprintf(pFile,"'%s'[Bank Large][%d]\n",ID,banklarge);
						dwFuckItemCount++;
						bFindFuck = TRUE;
						memset(&BankInfo.Bank_Large[banklarge],0,sizeof(CItem2));
					}
				}
			}

			// Bank_Small
			for( int banksmall = 0; banksmall < MAX_BANK_SMALL; banksmall++ )
			{
				WORD id	= BankInfo.Bank_Small[banksmall].GetID();
				if(id == 0)	continue;
				
				if(BankInfo.Bank_Small[banksmall].m_Serial.bServerSet != byServerSet)
				{
					if(	BankInfo.Bank_Small[banksmall].m_Serial.bServerSet != 255 ||
						BankInfo.Bank_Small[banksmall].m_Serial.bServerSet != 254 ||
						BankInfo.Bank_Small[banksmall].m_Serial.bServerSet != 253 )
					{
						fprintf(pFile,"'%s'[Guardian][%d]\n",ID,banksmall);
						dwFuckItemCount++;
						bFindFuck = TRUE;
						memset(&BankInfo.Bank_Small[banksmall],0,sizeof(CItem2));
					}
				}
			}
			
			if( bFindFuck )
			{
				// 업데이트 
				int		nColumn			= 2;	
				DWORD	dwParamOffset	= 0;	
				
				sprintf(szQuery,"Update Bankinfo Set Bank_Large=?,Bank_Small=? Where [ID]='%s'",ID);				
				DBBINDING* pBinding = m_pDb->CreateParamInfo(nColumn);				
				if(!pBinding) continue;
				
				pBinding[0].obValue		= dwParamOffset;	
				pBinding[0].cbMaxLen	= sizeof(BankInfo.Bank_Large);
				pBinding[0].wType		= DBTYPE_BYTES;
				dwParamOffset			+= sizeof(BankInfo.Bank_Large);		
				
				pBinding[1].obValue		= dwParamOffset;	
				pBinding[1].cbMaxLen	= sizeof(BankInfo.Bank_Small);
				pBinding[1].wType		= DBTYPE_BYTES;
				dwParamOffset			+= sizeof(BankInfo.Bank_Small);		
				
				int affected = m_pDb->ExecuteSQLByParam(szQuery,pBinding,&BankInfo,nColumn,m_CORUM_GAME_DB[sel]);	
				m_pDb->ReleaseParamInfo(pBinding);
			}
		}				
	}

	m_pDb->ReleaseRecordset(pData);
	pData = NULL;	

	fprintf(pFile,"\n");
	fprintf(pFile,"Fuck Item Count : [%d]\n",dwFuckItemCount);
	fprintf(pFile,"\n");
	fclose(pFile);

	CItem2 item[10];
	memset(item,0,sizeof(item));
	sprintf(szQuery,"Update PlayerShop Set Item1=?, Item2=?, Item3=?, Item4=?, Item5=?, Item6=?, Item7=?, Item8=?, Item9=?, Item10=?");				
	
	DBBINDING* pBinding = m_pDb->CreateParamInfo(10);				
	if(!pBinding) return;

	DWORD dwParamOffset = 0;
	
	for(int i = 0; i < 10; i++)
	{
		pBinding[i].obValue		= dwParamOffset;	
		pBinding[i].cbMaxLen	= sizeof(item[i]);
		pBinding[i].wType		= DBTYPE_BYTES;
		dwParamOffset			+= sizeof(CItem2);		
	}
	
	int affected = m_pDb->ExecuteSQLByParam(szQuery,pBinding,item,10,m_CORUM_GAME_DB[sel]);	

	AfxMessageBox("Find Fuck Item End!!!",MB_OK|MB_ICONINFORMATION);
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CCopyItemDelete::DeleteItemSearch()
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

	/*캐릭터 이름*/
	ShowQuery.Format("All CharName Query");
	m_pEdit->SetWindowText(ShowQuery);
	sprintf(szQuery,"SELECT Count(*) FROM CharacterInfo");
	int ret = m_pDb->OpenRecord(szQuery,&nameCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);		
	sprintf(szQuery,"SELECT [Name] FROM CharacterInfo");		
	pData = m_pDb->OpenRecordEx(szQuery,nameCount,DEFAULT_ROWS_PER_READ,m_CORUM_GAME_DB[sel]);	

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
	
	/*창고아이디 이름*/
	ShowQuery.Format("All BankID Query");
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

	ShowQuery.Format("Wait vector Sort.........");
	m_pEdit->SetWindowText(ShowQuery);

	//************************************************
	sort(m_haveList.begin(),m_haveList.end(),D_compare);
	//************************************************

	// 복사 아이템의 리스트를 뽑는다.
	CString		folder;
	folder.Format("%s\\CopyItemList",g_szExePath);
	
	CFileFind	finder;
	BOOL		bOK		= FALSE;
	bOK					= finder.FindFile(folder);
	
	// 폴더가 있으면 
	if(bOK)
	{
		while(bOK)
		{
			bOK = finder.FindNextFile();
			if(!finder.IsDirectory())	
			{
				// 폴더명과 똑같은 파일이 있는데(확장자 없는) 폴더가 아닐경우
				::CreateDirectory((LPCTSTR)folder,NULL);
				break;
			}
		}
		
	}
	// 폴더가 없으면 
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
			// 여기서 캐릭터 업데이트 한다.
			UpdateCharacterInfo(h,sel);
		}		
		
		count++;
		StepProgress();
	}
	
	m_pProgress->SetPos(0);
	::CloseHandle(hFile);
	
	// 다 끝났다.
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
void CCopyItemDelete::UpdateCharacterInfo(HAVEITEMEX h,int sel)
{
	char			szQuery[0xff]	= {0,};		
	CharacterItem	CharItem;
	BANKINFO		BankInfo;
	int				ret				= 0;

	if( (h.bwhere == WHERE_EQUIP)		||
		(h.bwhere == WHERE_INV_LARGE)	||
		(h.bwhere == WHERE_INV_SMALL)	||
		(h.bwhere == WHERE_BELT)		||
		(h.bwhere == WHERE_GUARDIAN)		)
	{
		memset(&CharItem,0,sizeof(CharacterItem));
		sprintf(szQuery,"Select Equip,Inv_large,Inv_small,Belt,Guardian From CharacterInfo Where [Name]='%s'",h.szCharName);
		ret = m_pDb->OpenRecord(szQuery,&CharItem,1,m_CORUM_GAME_DB[sel]);

		if( ret <= 0 )
			return;	
		
		if(h.bwhere == WHERE_EQUIP)
			memset(&CharItem.Equip[h.bindex],0,sizeof(CItem));
		else if(h.bwhere == WHERE_INV_LARGE)
			memset(&CharItem.Inv_Large[h.bindex],0,sizeof(CItem));
		else if(h.bwhere == WHERE_INV_SMALL)
			memset(&CharItem.Inv_Small[h.bindex],0,sizeof(CItem));
		else if(h.bwhere == WHERE_BELT)
			memset(&CharItem.Belt[h.bindex],0,sizeof(CItem));
		else if(h.bwhere == WHERE_GUARDIAN)
			memset(&CharItem.Guardian[h.bindex],0,sizeof(CItem));

		// 업데이트 
		int		nColumn			= 5;	
		DWORD	dwParamOffset	= 0;	
		int		nAffected		= 0;
		
		sprintf(szQuery,"Update CharacterInfo Set [Equip]=?,[Inv_large]=?,[Inv_small]=?,[Belt]=?,"
						"[Guardian]=? Where [Name]='%s'",h.szCharName);
		
		DBBINDING* pBinding = m_pDb->CreateParamInfo(nColumn);
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
		
		int affected = m_pDb->ExecuteSQLByParam(szQuery,pBinding,&CharItem,nColumn,m_CORUM_GAME_DB[sel]);	
		if(affected <= 0)
		{
			m_pDb->ReleaseParamInfo(pBinding);
			return;	
		}				
		m_pDb->ReleaseParamInfo(pBinding);
	}
	else if( (h.bwhere == WHERE_BANK_LARGE) || (h.bwhere == WHERE_BANK_SMALL) )
	{
		memset(&BankInfo,0,sizeof(BANKINFO));
		sprintf(szQuery,"Select Bank_Large,Bank_Small,[Money] From BankInfo Where [ID]='%s'",h.szCharName);
		ret = m_pDb->OpenRecord(szQuery,&BankInfo,1,m_CORUM_GAME_DB[sel]);

		if( ret <= 0 )
			return;	

		if(h.bwhere == WHERE_BANK_LARGE)
			memset(&BankInfo.Bank_Large[h.bindex],0,sizeof(CItem));
		else if(h.bwhere == WHERE_BANK_SMALL)				
			memset(&BankInfo.Bank_Small[h.bindex],0,sizeof(CItem));
		
		// 업데이트 
		int		nColumn			= 3;	
		DWORD	dwParamOffset	= 0;	
		
		sprintf(szQuery,"Update Bankinfo Set Bank_Large=?,Bank_Small=?,[Money]=? Where [ID]='%s'",h.szCharName);				
		DBBINDING* pBinding = m_pDb->CreateParamInfo(nColumn);
		
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
		
		int affected = m_pDb->ExecuteSQLByParam(szQuery,pBinding,&BankInfo,nColumn,m_CORUM_GAME_DB[sel]);	
		if( affected < 0 )
		{
			m_pDb->ReleaseParamInfo(pBinding);
			return;
		}				
		m_pDb->ReleaseParamInfo(pBinding);
	}
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CCopyItemDelete::StepProgress()
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
void CCopyItemDelete::AssertValid() const
{
	CFormView::AssertValid();
}
void CCopyItemDelete::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif 


//------------------------------------------------------------------------------
//	DESCRIPTION	:	- OleDbThread.dll COM 초기화 
//------------------------------------------------------------------------------
BOOL CCopyItemDelete::InitDatabase()
{
	//	해당 프로세스가 COM 서브시스템을 사용할수 있도록 준비
	//	내부적으로 COM 서브시스템을 사용하는데 중요한 정보를 담을 여러가지 자료구조를 할당 
	//	클라이언트 프로그램의 프로세스 번호를 알아내고 이 번호를 사용하여 그 프로그램이 수행하는 COM관련 동작의 진행 상황을 추적한다.
	//	하나의 포인터 매개변수를 받으면 여기에는 항상 NULL을 넘겨준다.
	CoInitialize(NULL);				
	
	HRESULT hr = ::CoCreateInstance(	CLSID_OLEDBTHREAD,			/* 클래스 팩토리의 클래스 ID */
										NULL,						/* 외부객체의 IUnknown 인터페이스의 포인터 */ 
										CLSCTX_INPROC_SERVER,		/* 객체 생성이 이루어지길 원하는 COM 서버타입을 나타내는 CLSCTX 열거 상수 */
										IID_OLEDBTHREAD,			/* 새 객체로부터 얻기를 원하는 인터페이스 ID */
										(void**)&m_pDb			);	/* 반환되는 인터페이스 포인터를 받아낼 출력 매개변수로 사용되는 포인터의 주소 */
	
	if(FAILED(hr))
	{
		AfxMessageBox("Cannot create IID_OLEDBTHREAD",MB_OK|MB_ICONSTOP);
		return FALSE;
	}
	
	DB_INITIALIZE_DESC desc;
	
	desc.bUsingThread					= FALSE;			// Thread 모델의 사용 여부, FALSE일 경우 (접두어 TH~ 계열의 메소드는 사용불가!)
	desc.bUsingEventObject				= FALSE;			// TRUE  : Event Object 방식으로 쿼리결과를 받음	FALSE : Message 방식으로 쿼리 결과를 Callback 함수  
	desc.ReceiveFunc					= NULL;				// Event Object 방식일경우 결과값을 받을 Callback 함수 포인터
	desc.hWndToPostMessage				= NULL;				// Message방식일 경우 Message를 받을 윈도우의 핸들값 
	desc.uMessage						= NULL;				// Message방식일 경우의 Message
	desc.wMaxNumOfProcessMessage_Input	= 100;				// DLL에서 동시에 수용할수 있는 최대 Input 메세지 갯수..  (Input 메세지큐 메모리풀의 최대용량)
	desc.wMaxNumOfProcessMessage_Output = 100;				// DLL에서 동시에 수용할수 있는 최대 Output 메세지 갯수..  (Output 메세지큐 메모리풀의 최대용량)
	desc.wMaxRowPerRead = 50;								// DB에서 Data를 읽어드릴때 한번에 읽어드릴수 있는 최대 행의갯수 
	
	//예)	만약 쿼리로인해 Return되는 결과행수가 200행이고 
	//		OpenRecordEx의 dwRowPerRead 인자값을 20으로 주었을경우
	//		한번에 20행씩 10번을 읽어서 200개를 받아온다.
	//		이때 dwRowPerRead인자값이 가질수있는 최대범위가 50이라는 의미!!
	
	desc.wMaxReturnedRowNum				= 50;				// 쿼리문에서 반환될 수 있는 행의 최대 갯수
	desc.wMaxReturnedColNum				= 128;				// 쿼리문에서 반환될 수 있는 칼럼의 최대 갯수
	desc.dwMaxRowSize					= 10240;			// 쿼리문에서 반환될 수 있는 한 행의 최대 사이즈	
	desc.OutputMessageFunc				= DisplayDBMessage;	// DLL내부의 메세지나 오류메세지를 출력받을 외부 함수 포인터
	
	// DB모듈 초기화 
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
void CCopyItemDelete::InitDBAccount()
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
	
	sprintf(keyname,"BACKUPDB_INFO");
	::GetPrivateProfileString(keyname,"CORUM_DB_NAME",		"",m_CORUM_BDB_NAME			,sizeof(m_CORUM_BDB_NAME)		,path);	
	::GetPrivateProfileString(keyname,"CORUM_DB_ACCOUNT",	"",m_CORUM_BDB_ACCOUNT		,sizeof(m_CORUM_BDB_ACCOUNT)	,path);	
	::GetPrivateProfileString(keyname,"CORUM_DB_PASSWORD",	"",m_CORUM_BDB_PASSWORD		,sizeof(m_CORUM_BDB_PASSWORD)	,path);
	::GetPrivateProfileString(keyname,"CORUM_DB_ADDRESS",	"",m_CORUM_BDB_ADDRESS		,sizeof(m_CORUM_BDB_ADDRESS)	,path);

	for( i = 0; i < 3; i++ )
	{
		sprintf(keyname,"BACKUPDB_INFO_TEMP_0%d",i);
		::GetPrivateProfileString(keyname,"CORUM_DB_NAME",		"",m_CORUM_BDBB_NAME[i]		,sizeof(m_CORUM_BDBB_NAME[i])		,path);	
		::GetPrivateProfileString(keyname,"CORUM_DB_ACCOUNT",	"",m_CORUM_BDBB_ACCOUNT[i]	,sizeof(m_CORUM_BDBB_ACCOUNT[i])	,path);	
		::GetPrivateProfileString(keyname,"CORUM_DB_PASSWORD",	"",m_CORUM_BDBB_PASSWORD[i]	,sizeof(m_CORUM_BDBB_PASSWORD[i])	,path);
		::GetPrivateProfileString(keyname,"CORUM_DB_ADDRESS",	"",m_CORUM_BDBB_ADDRESS[i]	,sizeof(m_CORUM_BDBB_ADDRESS[i])	,path);
	}
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
BOOL CCopyItemDelete::InitCorumDB()
{
	char path[MAX_PATH] = {0,};
	sprintf(path,"%s\\DB.ini",".");
	
	int TotalDBNum = ::GetPrivateProfileInt("DB","TotalDBNum",0,path);
	
	for( int i = 0; i < TotalDBNum; i++ )
	{
		m_CORUM_GAME_DB[i] = m_pDb->CreateDBConnection();
		
		if(m_pDb->Connect(m_CORUM_DB_ADDRESS[i],m_CORUM_DB_NAME[i],m_CORUM_DB_ACCOUNT[i],m_CORUM_DB_PASSWORD[i],20,10,FALSE,m_CORUM_GAME_DB[i]) < 0)
		{
			m_pDb->ReleaseDBConnection(m_CORUM_GAME_DB[i]);
			m_CORUM_GAME_DB[i] = 0;
			CString str;
			str.Format("Fail to Connect GAME_DB! (IP:%s, DBName:%s)", m_CORUM_DB_ADDRESS[i], m_CORUM_DB_NAME[i]);
			AfxMessageBox(str,MB_OK|MB_ICONSTOP);
			return FALSE;
		}		
	}
	
	m_CORUM_GAME_DB[TotalDBNum] = m_pDb->CreateDBConnection();
	
	if(m_pDb->Connect(m_CORUM_BDB_ADDRESS,m_CORUM_BDB_NAME,m_CORUM_BDB_ACCOUNT,m_CORUM_BDB_PASSWORD,20,10,FALSE,m_CORUM_GAME_DB[TotalDBNum]) < 0)
	{
		m_pDb->ReleaseDBConnection(m_CORUM_GAME_DB[TotalDBNum]);
		m_CORUM_GAME_DB[TotalDBNum] = 0;
		CString str;
		str.Format("Fail to Connect TOTAL_DB! (IP:%s, DBName:%s)",m_CORUM_BDB_ADDRESS,m_CORUM_BDB_NAME);
		AfxMessageBox(str,MB_OK|MB_ICONSTOP);
		return FALSE;
	}	

	for( i = 0; i < 3; i++ )
	{
		m_CORUM_BACKUP_DB[i] = m_pDb->CreateDBConnection();
		if(m_pDb->Connect(m_CORUM_BDBB_ADDRESS[i],m_CORUM_BDBB_NAME[i],m_CORUM_BDBB_ACCOUNT[i],m_CORUM_BDBB_PASSWORD[i],20,10,FALSE,m_CORUM_BACKUP_DB[i]) < 0)
		{
			m_pDb->ReleaseDBConnection(m_CORUM_BACKUP_DB[i]);
			m_CORUM_BACKUP_DB[i] = 0;
			CString str;
			str.Format("Fail to Connect BACK_DB! (IP:%s, DBName:%s)", m_CORUM_BDBB_ADDRESS[i], m_CORUM_BDBB_NAME[i]);
			AfxMessageBox(str,MB_OK|MB_ICONSTOP);
			return FALSE;
		}
	}
	
	int ret = m_pDb->QueryDBCatalog(m_DbSchema,200,m_CORUM_GAME_DB[TotalDBNum]);
	
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





































