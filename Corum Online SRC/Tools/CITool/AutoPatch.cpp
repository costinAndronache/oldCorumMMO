#include "stdafx.h"
#include "NewSerial.h"
#include "AutoPatch.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CAutoPatch, CDialog)
	//{{AFX_MSG_MAP(CAutoPatch)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
CAutoPatch::CAutoPatch(CWnd* pParent /*=NULL*/)	: CDialog(CAutoPatch::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoPatch)
	//}}AFX_DATA_INIT
	m_pFtp = NULL;
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CAutoPatch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoPatch)
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	//}}AFX_DATA_MAP
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CAutoPatch::CloseConnection()
{
	if(m_pFtp)
	{
		m_pFtp->Close();
		delete m_pFtp;
		m_pFtp = NULL;
	}
	
	m_Session.Close();
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
BOOL CAutoPatch::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_Progress.SetRange(0,100);
	m_Progress.SetPos(0);
	m_Progress.SetBkColor(RGB(228,228,228));
	m_Progress.SetGradientColors(RGB(0,109,255),RGB(0,0,62));
	m_Progress.SetShowPercent(TRUE);
	m_Progress.ShowWindow(SW_HIDE);
	CRect rect(1,1,1,1);
	m_Progress.SetBorders(&rect);
	
	SetTimer(1, 200, NULL);
	
	return TRUE;  
}


//------------------------------------------------------------------------------
//	DESCRIPTION	:	-
//------------------------------------------------------------------------------
void CAutoPatch::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
		//----------------------------------------------------------------------
		case 1:
		{
			KillTimer(1);
			::SetCurrentDirectory(g_szExePath);

			SetDlgItemText(IDC_STATIC,"Now Connecting FTP Server...");

			try
			{
				m_pFtp = m_Session.GetFtpConnection	(
														DEFAULT_FTP_ADDRESS,
														DEFAULT_FTP_ACCOUNT,
														DEFAULT_FTP_PASSWORD,
														DEFAULT_FTP_PORT
													);
			}
			catch(CInternetException *pEx)
			{
				// 접속실패 
				CString str;
								
				int ret = pEx->m_dwError;
				pEx->Delete();
				
				str.Format("Connect Fail... (%d)",ret);
				SetDlgItemText(IDC_STATIC,str);

				AfxMessageBox("AutoPatch Server Connect Fail...",MB_OK|MB_ICONSTOP);
				
				// EndDialog(IDCANCEL) 비정상적 종료 
				SetTimer(10,3000,NULL);
				return;
			}

			SetDlgItemText(IDC_STATIC,"Now Request Version...");

			int ret = m_pFtp->GetFile	(
											"CIToolPatch\\CIToolVerInfo_New.ini",
											"CIToolVerInfo_New.ini",
											FALSE
										);
			if(!ret)
			{
				// 파일 다운로드 실패...
				SetDlgItemText(IDC_STATIC,"Fail to download ini File...");
				AfxMessageBox("AutoPatch Fail To DownLoad ini File...",MB_OK|MB_ICONSTOP);				
				CloseConnection();
				
				// EndDialog(IDCANCEL) 비정상적 종료 
				SetTimer(10,3000,NULL);
				return;
			}

			char inipath[0xff];
			::GetCurrentDirectory(sizeof(inipath),inipath);
			strcat(inipath,"\\CIToolVerInfo_New.ini");
			DWORD dwVer				= ::GetPrivateProfileInt("Version","ver",0,inipath);
			DWORD dwNewFileCount	= ::GetPrivateProfileInt("Version","NewFileCount",0,inipath); 

			CApp* pApp = (CApp*)AfxGetApp();

			// 패치를 해야 된다.
			if( pApp->GetVersion() < dwVer )
			{
				char	filename[0xff]			= {0,};
				char	KeyName[0xff]			= {0,};
				char	FTPFilePath[MAX_PATH]	= {0,};
				char    message[0xff]			= {0,};
				int		ret						= 0;
				m_Progress.ShowWindow(SW_SHOW);
				
				for( int i = 0; i < dwNewFileCount; i++ )
				{
					sprintf(message,"File Update... [ %d/%d ]",i+1,dwNewFileCount);
					SetDlgItemText(IDC_STATIC,message);

					float percentage = ((i+1)*1.0 / dwNewFileCount*1.0) * 100.;
					m_Progress.SetPos((int)percentage);

					sprintf(KeyName,"FILE%d",i);
					ret = ::GetPrivateProfileString("File",KeyName,"",filename,sizeof(filename),inipath);

					// ini에서 파일명 못읽어온경우
					if( !ret )
					{
						SetDlgItemText(IDC_STATIC,"Fail To Read ini File...");
						AfxMessageBox("AutoPatch Fail To Read ini File...",MB_OK|MB_ICONSTOP);						
						CloseConnection();
						
						// EndDialog(IDCANCEL) 비정상적 종료 
						SetTimer(10,3000,NULL);
					}
					
					// FTP에서 파일 받기 
					sprintf(FTPFilePath,"CIToolPatch/%s",filename);
					ret = m_pFtp->GetFile(FTPFilePath,filename,FALSE);
					
					// 파일 다운로드에 실패한 경우 
					if( !ret )
					{
						SetDlgItemText(IDC_STATIC,"Fail to download file...");
						AfxMessageBox("AutoPatch Fail To Download File...",MB_OK|MB_ICONSTOP);						
						CloseConnection();
						
						// EndDialog(IDCANCEL) 비정상적 종료 
						SetTimer(10,3000,NULL);
						return;
					}
				}

				// 파일 다운로드 끝 
				SetTimer(11,1000,NULL);
				::ShellExecute(m_hWnd,NULL,"CIToolSelfUpdate.exe",NULL,NULL,SW_SHOWNORMAL);				
			}
			// 패치 필요 없슴. 					
			else
			{
				BOOL	bReturn = FALSE;
				int		i		= 0;
				
				while(!bReturn)
				{
					bReturn = DeleteFile( "CIToolVerInfo_New.ini" );
					
					if( GetLastError() == 2 )
					{
						break;
					}
					
					Sleep(200);
					i++;
				}

				SetDlgItemText(IDC_STATIC,"This Is CITool New Version!!!");
				CloseConnection();
				EndDialog(IDOK);
			}
		}
		break;
		//----------------------------------------------------------------------

		//----------------------------------------------------------------------
		case 10:	// 잠시후 Dialog 종료 루틴..
		{
			CloseConnection();
			KillTimer(10);
			
			EndDialog(IDCANCEL);	
		}
		break;
		//----------------------------------------------------------------------
			
		//----------------------------------------------------------------------
		case 11:
		{
			CloseConnection();
			KillTimer(11);
			
			// 프로그램 종료
			EndDialog(-1);
		}
		break;
		//----------------------------------------------------------------------
	}
	CDialog::OnTimer(nIDEvent);
}
