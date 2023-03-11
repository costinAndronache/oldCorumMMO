#include "stdafx.h"
#include "NewSerial.h"
#include "GuildInitView.h"
#include "GuildFrame.h"
#include "HashTable.h"


#include <afxtempl.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CGuildInitView, CFormView)


BEGIN_MESSAGE_MAP(CGuildInitView, CFormView)
	//{{AFX_MSG_MAP(CGuildInitView)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO, OnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO_BACKUP, OnSelchangeComboBackup)
	ON_BN_CLICKED(IDC_BUTTON, OnGo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CGuildInitView::CGuildInitView() : CFormView(CGuildInitView::IDD)
{
	//{{AFX_DATA_INIT(CGuildInitView)
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


CGuildInitView::~CGuildInitView()
{

}


void CGuildInitView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGuildInitView)
	DDX_Control(pDX, IDC_COMBO_BACKUP, m_ComBoCtrl_Backup);
	DDX_Control(pDX, IDC_COMBO, m_ComBoCtrl);
	//}}AFX_DATA_MAP
}


//------------------------------------------------------------------------------
//	FUNCTION	:	-
//	DESCRIPTION	:	- ���ڿ����� ���� �̿��� ��� ���ڸ� �����ϴ� �Լ�
//------------------------------------------------------------------------------
int CGuildInitView::RemoveNotNumber(CString &strNumber)
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


void CGuildInitView::OnDestroy() 
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


void CGuildInitView::OnInitialUpdate() 
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
	
	CGuildFrame* pFrame = (CGuildFrame*)GetParent();
	m_pApp->m_dwDocCount4++;	
	pFrame->m_nFrameID = m_pApp->m_dwDocCount4;	//�����ӿ� �ڱ� Frame ID ����	
	m_nViewID = m_pApp->m_dwDocCount4;
	
	CString str;
	str.Format("[%02d] CorumOnline Guild Initialize Tool",pFrame->m_nFrameID); 
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
	
	char path[MAX_PATH] = {0,};
	sprintf(path,"%s\\DB.ini",".");
	int TotalDBNum	= ::GetPrivateProfileInt("DB","TotalDBNum",0,path);
	
	// �޺�
	for( int i = 0 ; i < TotalDBNum; i++ ) 
		m_ComBoCtrl.InsertString(i,m_CORUM_DB_NAME[i]);
	m_ComBoCtrl.InsertString(TotalDBNum,"Backup");
	m_ComBoCtrl.SetCurSel(0);
	m_ComBoCtrl_Backup.EnableWindow(FALSE);
}


void CGuildInitView::OnSelchangeCombo() 
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
				// Corum ���� DB�� 
				if( !szTemp.CompareNoCase("Corum") )
				{
					char *p = szTemp.GetBuffer(szTemp.GetLength());
					if(isupper(*p))
						continue;

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


void CGuildInitView::OnSelchangeComboBackup() 
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


void CGuildInitView::StepProgress()
{
	int	nLower, nUpper;
	int	nCurPos = m_pProgress->GetPos();		
	m_pProgress->GetRange(nLower, nUpper);
	
	if( nCurPos >= nUpper - nLower )
		m_pProgress->SetPos(0);
	else
		m_pProgress->StepIt();
}


UINT ThreadFuncGuildInit(LPVOID pParam)
{
	CGuildInitView* pView	= (CGuildInitView*)pParam;
	pView->GuildInitialize();
//	pView->GuildLuckyBag();
	pView->m_bRun			= FALSE;
	pView->m_ComBoCtrl.EnableWindow(TRUE);
	pView->m_ComBoCtrl_Backup.EnableWindow(TRUE);
	pView->m_pWinThread		= NULL;
	return 0;
}


void CGuildInitView::OnGo() 
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
		
		m_pWinThread	= ::AfxBeginThread(ThreadFuncGuildInit,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
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


void CGuildInitView::GuildLuckyBag()
{
	ListNode<GUILD_OFFLINE_USER>*	lpGuildOfflinePos	= NULL;
	GUILD_OFFLINE_USER*				lpGuildUser			= NULL;

	CharacterSmallItem				CharacterSmall;
	BankInfoSmallItem				BankInfoSmall;

	char	szQuery[0xff]	= {0,};
	CString	ShowQuery;

	ShowQuery.Format("Wait Guild Info Load...");
	m_pEdit->SetWindowText(ShowQuery);

	int		nRt				= GuildInfoLoad();
	int		sel				= m_ComBoCtrl.GetCurSel();

	int		ret				= 0;
	WORD	wFindItemID		= 6539;
	DWORD	dwTotalCount	= 0;

	if(nRt == -1 )
	{
		AfxMessageBox("GuildInfo Or GuildList Table Can't Find Data",MB_OK|MB_ICONSTOP);	
	}

	CString	folder;
	folder.Format("%s\\GuildLuckyBag",g_szExePath);
	
	CFileFind	finder;
	BOOL		bOK = FALSE;
	bOK				= finder.FindFile( folder );
	
	// ������ ������ 
	if( bOK )
	{
		while(bOK)
		{
			bOK = finder.FindNextFile();
			if(!finder.IsDirectory())	
			{
				// ������� �Ȱ��� ������ �ִµ�(Ȯ���� ����) ������ �ƴҰ��
				::CreateDirectory( (LPCTSTR)folder,NULL );
				break;
			}
		}
		
	}
	// ������ ������ 
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

	int Total		= m_pGuildTableHash->GetCount();
	int Progress	= 1;
	m_pProgress->SetRange(0,Total);

	ListNode<GUILD_TABLE>* lpTablePos = m_pGuildTableHash->GetHead();

	while(lpTablePos)
	{
		GUILD_TABLE* lpGuildTable = lpTablePos->pData;

		if(lpGuildTable)
		{
			ShowQuery.Format("[%d][%d][%s]",Total,Progress,lpGuildTable->sGuild.szGuildName);
			m_pEdit->SetWindowText(ShowQuery);

			dwTotalCount = 0;

			lpGuildOfflinePos = lpGuildTable->pGuildOfflineHash->GetHead();

			while(lpGuildOfflinePos)
			{
				lpGuildUser = lpGuildOfflinePos->pData;

				if(lpGuildUser)
				{
					sprintf(szQuery,"Select ID,Inv_small,Belt From CharacterInfo Where [Name]='%s'",lpGuildUser->szName);
					memset(&CharacterSmall,0,sizeof(CharacterSmallItem));
					ret = m_pDb->OpenRecord(szQuery,&CharacterSmall,1,m_CORUM_GAME_DB[sel]);

					if( ret > 0 )
					{
						// Inv_Small
						for( int inv_small = 0; inv_small < MAX_INV_SMALL; inv_small++ )
						{
							if(CharacterSmall.Inv_Small[inv_small].GetID() == wFindItemID)
							{
								dwTotalCount += 1;
							}
						}			
						// Belt
						for( int belt = 0; belt < MAX_BELT; belt++ )
						{
							if(CharacterSmall.Belt[belt].GetID() == wFindItemID)
							{
								dwTotalCount += 1;
							}
						}
					}
					
					sprintf(szQuery,"Select Bank_Small From BankInfo Where [ID]='%s'",CharacterSmall.szID);
					memset(&BankInfoSmall,0,sizeof(BankInfoSmallItem));
					ret = m_pDb->OpenRecord(szQuery,&BankInfoSmall,1,m_CORUM_GAME_DB[sel]);

					if( ret > 0 )
					{
						for( int banksmall = 0; banksmall < MAX_BANK_SMALL; banksmall++ )
						{
							if(BankInfoSmall.Bank_Small[banksmall].GetID() == wFindItemID)
							{
								dwTotalCount += 1;
							}
						}
					}
				}

				lpGuildOfflinePos = lpGuildOfflinePos->pNext;				
			}

			fprintf(pFile,"[%s]:%d\n",lpGuildTable->sGuild.szGuildName,dwTotalCount);
		}
		
		lpTablePos = lpTablePos->pNext;
		Progress++;
			
		StepProgress();
		::Sleep(10);
	}

	fprintf(pFile, "\n");
	fclose(pFile);

	// �� ������.
	ShowQuery.Format("Guild Lucky Bag End");
	m_pEdit->SetWindowText(ShowQuery);
	m_pProgress->SetPos(0);
	
	AfxMessageBox("Guild Lucky Bag End",MB_OK|MB_ICONINFORMATION);	
}


void CGuildInitView::GuildInitialize()
{
	BYTE							byGuildUser[4]	= {0,};
	char							szQuery[0xff]	= {0,};
	DWORD							dwRank			= 0;
	DWORD							dwLevel			= 0;
	DWORD							dwUserIndex		= 0;
	int								nCount			= 0;
	ListNode<GUILD_OFFLINE_USER>*	lpGuildOfflinePos;
	GUILD_OFFLINE_USER*				lpGuildUser;
	GROUP_INFO						GroupInfo[30];
	CString							ShowQuery;							

	int nRt = GuildInfoLoad();
	int	sel	= m_ComBoCtrl.GetCurSel();

	if(nRt == -1 )
	{
		AfxMessageBox("GuildInfo Or GuildList Table Can't Find Data",MB_OK|MB_ICONSTOP);	
	}

	sprintf(szQuery,"Select * From Group_Info Where type = 1");				
	memset(&GroupInfo, 0, sizeof(GroupInfo));	
	m_pDb->OpenRecord(szQuery, GroupInfo, 30, m_CORUM_GAME_DB[sel]);			
		
	int Total		= m_pGuildTableHash->GetCount();
	int Progress	= 1;
	m_pProgress->SetRange(0,Total);
	
	ListNode<GUILD_TABLE>* lpTablePos = m_pGuildTableHash->GetHead();	
		
	while(lpTablePos)
	{
		GUILD_TABLE* lpGuildTable = lpTablePos->pData;
		
		if(lpGuildTable)
		{
			ShowQuery.Format("[%d][%d][%s]",Total,Progress,lpGuildTable->sGuild.szGuildName);
			m_pEdit->SetWindowText(ShowQuery);

			lpGuildOfflinePos = lpGuildTable->pGuildOfflineHash->GetHead();

			byGuildUser[0] = 0;
			byGuildUser[1] = 0;
			byGuildUser[2] = 0;
			byGuildUser[3] = 0;			

			while(lpGuildOfflinePos)
			{
				lpGuildUser = lpGuildOfflinePos->pData;

				if(lpGuildUser)
				{
					if(lpGuildUser->byRank == 1)
					{
						if(dwUserIndex == 0)
						{
							dwUserIndex = lpGuildUser->dwUserIndex;
							dwLevel		= lpGuildUser->dwLevel;
						}
						else
						{
							if(dwLevel < lpGuildUser->dwLevel)
							{
								dwUserIndex = lpGuildUser->dwUserIndex;
								dwLevel		= lpGuildUser->dwLevel;
							}
						}	
					}
					byGuildUser[lpGuildUser->byRank-1] += 1;
				}
				lpGuildOfflinePos = lpGuildOfflinePos->pNext;				
			}

			nCount = byGuildUser[0] + byGuildUser[1] + byGuildUser[2] + byGuildUser[3];
			
			// �����Ͱ� ���� ��� �̰ų� �����Ͱ� �� �̻��� ��� 
			if(byGuildUser[0] != 1)	
			{
				// ������ ���� ����϶�.
				if(lpGuildTable->pGuildOfflineHash->GetCount()==0)
				{
					sprintf(szQuery,"Delete From GuildInfo Where ID = %u", lpGuildTable->sGuild.dwGuildId);
					m_pDb->ExecuteSQL(szQuery,m_CORUM_GAME_DB[sel]);
					sprintf(szQuery,"Delete From GuildList where Guildid = %u",lpGuildTable->sGuild.dwGuildId);
					m_pDb->ExecuteSQL(szQuery,m_CORUM_GAME_DB[sel]);					
				}
				// ������ �ִ� ����϶�.
				else
				{
					// �����Ͱ� �θ� �̻��� ���.
					if(byGuildUser[0] > 1)
					{
						int Rank2 = GroupInfo[lpGuildTable->sGuild.wLevel].wMemeber[2] - lpGuildTable->sGuild.byMember[1];
						int Rank3 = GroupInfo[lpGuildTable->sGuild.wLevel].wMemeber[3] - lpGuildTable->sGuild.byMember[2];
						int Rank4 = GroupInfo[lpGuildTable->sGuild.wLevel].wMemeber[4] - lpGuildTable->sGuild.byMember[3];

						lpGuildOfflinePos = lpGuildTable->pGuildOfflineHash->GetHead();
												
						while(lpGuildOfflinePos)
						{
							lpGuildUser = lpGuildOfflinePos->pData;
							
							if(lpGuildUser)
							{
								if(lpGuildUser->byRank == 1)
								{
									if(lpGuildUser->dwUserIndex != dwUserIndex)
									{
										if(Rank2 > 0)
										{
											sprintf(szQuery,"Update CharacterInfo Set Rank = 2 Where Char_Index = %u", lpGuildUser->dwUserIndex);
											m_pDb->ExecuteSQL(szQuery,m_CORUM_GAME_DB[sel]);
											Rank2			+= 1;											
											byGuildUser[1]	-= 1;																						
										}
										else if(Rank3 > 0)
										{
											sprintf(szQuery,"Update CharacterInfo Set Rank = 3 Where Char_Index = %u", lpGuildUser->dwUserIndex);
											m_pDb->ExecuteSQL(szQuery,m_CORUM_GAME_DB[sel]);
											Rank3			+= 1;
											byGuildUser[2]	-= 1;
										}
										else if(Rank4 > 0)
										{
											sprintf(szQuery,"Update CharacterInfo Set Rank = 4 Where Char_Index = %u", lpGuildUser->dwUserIndex);
											m_pDb->ExecuteSQL(szQuery,m_CORUM_GAME_DB[sel]);
											Rank4			+= 1;
											byGuildUser[3]	-= 1;
										}
										else
										{
										}											
									}
								}
							}
							lpGuildOfflinePos = lpGuildOfflinePos->pNext;
						}
					}
					// �����Ͱ� ���� ���.
					else if(byGuildUser[0] == 0)
					{
						dwUserIndex = 0;
						dwRank		= 0;
						dwLevel		= 0;

						lpGuildOfflinePos = lpGuildTable->pGuildOfflineHash->GetHead();

						while(lpGuildOfflinePos)
						{
							lpGuildUser = lpGuildOfflinePos->pData;
							
							if(lpGuildUser)
							{
								if(dwUserIndex == 0)
								{
									dwUserIndex = lpGuildUser->dwUserIndex;
									dwRank		= lpGuildUser->byRank;
									dwLevel		= lpGuildUser->dwLevel;
								}
								else
								{
									if(dwRank > lpGuildUser->byRank)
									{
										dwUserIndex = lpGuildUser->dwUserIndex;
										dwRank		= lpGuildUser->byRank;
										dwLevel		= lpGuildUser->dwLevel;
									}
									else if(dwRank == lpGuildUser->byRank)
									{
										if(dwLevel<lpGuildUser->dwLevel)
										{
											dwUserIndex = lpGuildUser->dwUserIndex;
											dwRank		= lpGuildUser->byRank;
											dwLevel		= lpGuildUser->dwLevel;
										}
									}
								}
							}
							lpGuildOfflinePos = lpGuildOfflinePos->pNext;				
						}													

						if(dwUserIndex !=0 )
						{
							GUILD_OFFLINE_USER* lpGuildOfflineUser = lpGuildTable->pGuildOfflineHash->GetData(dwUserIndex);

							if(lpGuildOfflineUser != NULL)
							{
								byGuildUser[0] += 1;
								byGuildUser[lpGuildOfflineUser->byRank-1] -= 1;
								
								sprintf(szQuery,"Update CharacterInfo Set Rank = 1 Where Char_Index = %u", dwUserIndex);
								m_pDb->ExecuteSQL(szQuery, m_CORUM_GAME_DB[sel]);
							}
						}						
						else
						{
							
						}
					}

					sprintf(szQuery,"Update GuildInfo Set Count = %u, Member1 = %u, Member2 = %u, Member3 = %u, Member4 = %u Where Id = %u",
							nCount, byGuildUser[0], byGuildUser[1], byGuildUser[2], byGuildUser[3], lpGuildTable->sGuild.dwGuildId);
					m_pDb->ExecuteSQL(szQuery, m_CORUM_GAME_DB[sel]);
				}					
			}
			else if(byGuildUser[1] != lpGuildTable->sGuild.byMember[1] ||
					byGuildUser[2] != lpGuildTable->sGuild.byMember[2] ||
					byGuildUser[3] != lpGuildTable->sGuild.byMember[3])
			{				
				sprintf(szQuery,"Update GuildInfo Set Count = %u, Member1 = %u, Member2 = %u, Member3 = %u, Member4 = %u Where Id = %u",
						nCount, byGuildUser[0], byGuildUser[1], byGuildUser[2], byGuildUser[3], lpGuildTable->sGuild.dwGuildId);
				m_pDb->ExecuteSQL(szQuery, m_CORUM_GAME_DB[sel]);
			}			
		}
		
		lpTablePos = lpTablePos->pNext;
		Progress++;
		
		StepProgress();
		::Sleep(10);
	}
	
	ShowQuery.Format("Wait......");
	m_pEdit->SetWindowText(ShowQuery);

	sprintf(szQuery,"Delete From GuildUserInfo");
	m_pDb->ExecuteSQL(szQuery,m_CORUM_GAME_DB[sel]);
	sprintf(szQuery,"CZP_GUILDUSER_INIT");
	m_pDb->ExecuteSQL(szQuery,m_CORUM_GAME_DB[sel]);
	
	ShowQuery.Format("Guild Initialize End");
	m_pEdit->SetWindowText(ShowQuery);
	m_pProgress->SetPos(0);

	AfxMessageBox("Guild Initialize End",MB_OK|MB_ICONINFORMATION);	

	if(m_pGuildTableHash)
	{
		m_pGuildTableHash->Destroy(TRUE);
		m_pGuildTableHash = NULL;
	}

	if(m_pGuildWarHash)
	{
		m_pGuildWarHash->Destroy(TRUE);
		m_pGuildWarHash = NULL;
	}
	
	if(g_pGuildWarPool)
	{
		ReleaseStaticMemoryPool(g_pGuildWarPool);
		g_pGuildWarPool = NULL;
	} 

	if(g_pGuildTablePool)
	{
		ReleaseStaticMemoryPool(g_pGuildTablePool);
		g_pGuildTablePool = NULL;
	} 

	if(g_pGuildOfflinePool)
	{
		ReleaseStaticMemoryPool(g_pGuildOfflinePool);
		g_pGuildOfflinePool = NULL;
	} 	
}


int CGuildInitView::GuildInfoLoad()
{
	GUILD				sGuild[1000];
	GUILD_LIST			sGuildList[1000];
	GUILD_OFFLINE_USER	sGuildOfflineUser[1000];
	GUILD_TABLE*		lpGuildTable;
	GUILD_OFFLINE_USER* lpGuildOfflineTable;
	GUILD_LIST*			lpGuildListTable;
	char				szSQL[0xff];
	int					nUserCount;
	int					sel	= m_ComBoCtrl.GetCurSel();
	
	m_pGuildTableHash	= new CGuildTableHash;
	m_pGuildWarHash		= new CGuildWarHash;
	g_pGuildWarPool		= CreateStaticMemoryPool();
	g_pGuildTablePool	= CreateStaticMemoryPool();	
	g_pGuildOfflinePool	= CreateStaticMemoryPool();	
	m_pGuildTableHash->InitializeHashTable(1000, 1000);	
	m_pGuildWarHash->InitializeHashTable(1000, 1000);	
	InitializeStaticMemoryPool(g_pGuildTablePool, sizeof(GUILD_TABLE), 1000, 1000);
	InitializeStaticMemoryPool(g_pGuildWarPool, sizeof(GUILD_LIST), 1000, 1000);
	InitializeStaticMemoryPool(g_pGuildOfflinePool, sizeof(GUILD_OFFLINE_USER), 600000, 600000);	

	// "Select * From GuildInfo"
	int nCount = m_pDb->OpenRecord("CZP_GUILDINFO_LOAD", sGuild, 1000, m_CORUM_GAME_DB[sel]);
	
	if(nCount < 0 )
	{
		return -1;		
	}
	
	for(int i = 0; i < nCount; i++)
	{
		// Hash Insert //		
		lpGuildTable = (GUILD_TABLE*)LALAlloc(g_pGuildTablePool);
		memset(lpGuildTable, 0, sizeof(lpGuildTable));
		
		// Off-Line Guild User Hash //
		lpGuildTable->pGuildOfflineHash	= new CGuildOfflineHash;
		lpGuildTable->pGuildOfflineHash->InitializeHashTable(100, 100);
		
		memcpy(&lpGuildTable->sGuild, &sGuild[i], sizeof(GUILD));
		
		m_pGuildTableHash->Insert(lpGuildTable, lpGuildTable->sGuild.dwGuildId);
		
		// Offline User //
		sprintf(szSQL, "CZP_GUILD_USER %u", lpGuildTable->sGuild.dwGuildId);
		
		memset(sGuildOfflineUser, 0, sizeof(sGuildOfflineUser));
		
		nUserCount = g_pDb->OpenRecord(szSQL, sGuildOfflineUser, 100, m_CORUM_GAME_DB[sel]);		
		
		for(int j = 0; j < nUserCount; j++)
		{			
			lpGuildOfflineTable = (GUILD_OFFLINE_USER*)LALAlloc(g_pGuildOfflinePool);
			memset(lpGuildOfflineTable, 0, sizeof(lpGuildOfflineTable));
			memcpy(lpGuildOfflineTable, &sGuildOfflineUser[j], sizeof(GUILD_OFFLINE_USER));						
			
			if(lpGuildTable)
			{
				lpGuildTable->pGuildOfflineHash->Insert(lpGuildOfflineTable, lpGuildOfflineTable->dwUserIndex);
			}
		}				
	}	
	
	memset(sGuildList, 0, sizeof(sGuildList));
	
	// "Select * From GuildList"
	nCount = m_pDb->OpenRecord("CZP_GUILD_LIST", sGuildList, 1000, m_CORUM_GAME_DB[sel]);
	
	if(nCount < 0)
	{
		return -1;
	}
	
	for(i = 0; i < nCount; i++)	
	{
		lpGuildListTable = (GUILD_LIST*)LALAlloc(g_pGuildWarPool);
		memset(lpGuildListTable, 0, sizeof(GUILD_LIST));
		memcpy(lpGuildListTable, &sGuildList[i], sizeof(GUILD_LIST));						
		
		if(lpGuildListTable)
		{
			m_pGuildWarHash->Insert(lpGuildListTable, lpGuildListTable->dwGuildId);				
		}
	}

	return TRUE;
}


#ifdef _DEBUG
void CGuildInitView::AssertValid() const
{
	CFormView::AssertValid();
}
void CGuildInitView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif


//------------------------------------------------------------------------------
//	DESCRIPTION	:	- OleDbThread.dll COM �ʱ�ȭ 
//------------------------------------------------------------------------------
BOOL CGuildInitView::InitDatabase()
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
void CGuildInitView::InitDBAccount()
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

	::GetPrivateProfileString(keyname,"CORUM_DB_NAME",		"",m_CORUM_DB_NAME[i]		,sizeof(m_CORUM_DB_NAME[i])		,path);	
	::GetPrivateProfileString(keyname,"CORUM_DB_ACCOUNT",	"",m_CORUM_DB_ACCOUNT[i]	,sizeof(m_CORUM_DB_ACCOUNT[i])	,path);	
	::GetPrivateProfileString(keyname,"CORUM_DB_PASSWORD",	"",m_CORUM_DB_PASSWORD[i]	,sizeof(m_CORUM_DB_PASSWORD[i])	,path);
	::GetPrivateProfileString(keyname,"CORUM_DB_ADDRESS",	"",m_CORUM_DB_ADDRESS[i]	,sizeof(m_CORUM_DB_ADDRESS[i])	,path);
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
BOOL CGuildInitView::InitCorumDB()
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
	
	m_CORUM_GAME_DB[TotalDBNum] = m_pDb->CreateDBConnection();
	
	if(m_pDb->Connect(m_CORUM_BDB_ADDRESS,m_CORUM_BDB_NAME,m_CORUM_BDB_ACCOUNT,m_CORUM_BDB_PASSWORD,20,10,FALSE,m_CORUM_GAME_DB[TotalDBNum]) < 0)
	{
		m_pDb->ReleaseDBConnectionForSingleThread(m_CORUM_GAME_DB[TotalDBNum]);
		m_CORUM_GAME_DB[TotalDBNum] = 0;
		CString str;
		str.Format("Fail to Connect TOTAL_DB! (IP:%s, DBName:%s)",m_CORUM_BDB_ADDRESS,m_CORUM_BDB_NAME);
		AfxMessageBox(str,MB_OK|MB_ICONSTOP);
		return FALSE;
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











