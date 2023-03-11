// LogCompare.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "LogCompare.h"
#include "LogCompareDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLogCompareApp

BEGIN_MESSAGE_MAP(CLogCompareApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CLogCompareApp ����

CLogCompareApp::CLogCompareApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CLogCompareApp ��ü�Դϴ�.

CLogCompareApp theApp;


// CLogCompareApp �ʱ�ȭ

BOOL CLogCompareApp::InitInstance()
{
	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControls()�� �ʿ��մϴ�. 
	// InitCommonControls()�� ������� ������ â�� ���� �� �����ϴ�.
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));

	// DB �ʱ�ȭ. 
	InitializeDBConnection( g_lpDBConfigFileName );

	CLogCompareDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ���⿡ ��ȭ ���ڰ� Ȯ���� ���� �������� ��� ó����
		// �ڵ带 ��ġ�մϴ�.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ���⿡ ��ȭ ���ڰ� ��Ҹ� ���� �������� ��� ó����
		// �ڵ带 ��ġ�մϴ�.
	}

	// ��ȭ ���ڰ� �������Ƿ� ���� ���α׷��� �޽��� ������ �������� �ʰ�
	// ���� ���α׷��� ���� �� �ֵ��� FALSE�� ��ȯ�մϴ�.
	return FALSE;
}

BOOL	CLogCompareApp::InitializeDBConnection( LPCTSTR lpConfigName )
{
	TCHAR	strRecv[0xFF];
	::GetCurrentDirectory( 0xff, strRecv );

	UINT uDBCount = ::GetPrivateProfileInt( 
		g_lpDBConfig_Info, 
		g_lpDBConfig_TotalDBCount, 
		0, 
		lpConfigName 
		);

	ASSERT(uDBCount);

	m_uDBCount = uDBCount;

	ZeroMemory( m_strBuffer, sizeof(m_strBuffer) );

	m_vecDBConInfo.reserve( uDBCount );
	m_vecDBConInfo.resize( uDBCount );
	
	LPCTSTR	lpDBInfoElement[] = 
	{
		g_lpDBConfig_DBName,
		g_lpDBConfig_DBAddress,
		g_lpDBConfig_Account,
		g_lpDBConfig_Password
	};

	UINT uIter2;

	for( UINT uIter = 0 ; uIter < uDBCount ; ++uIter )
	{
		::wsprintf( m_strBuffer, g_lpDBConfig_DBInfo, uIter );

		for( uIter2 = 0 ; uIter2 < CDBConnectionInfo::CDBDM_MAX ; ++uIter2 )
		{
			::GetPrivateProfileString( 
				m_strBuffer, 
				lpDBInfoElement[uIter2], 
				"Error", 
				strRecv, 
				sizeof(strRecv),  
				lpConfigName
				);

			m_vecDBConInfo[uIter].GetStr( uIter2 ) = strRecv;
		}        
	}

	/////////////////////////
	uDBCount = ::GetPrivateProfileInt( 
		g_lpDBConfig_Info, 
		g_lpDBConfig_MainDBCount, 
		0, 
		lpConfigName 
		);

	ASSERT(uDBCount);

	m_uMainDBCount = uDBCount;

	ZeroMemory( m_strBuffer, sizeof(m_strBuffer) );

	m_vecMainDBConInfo.reserve( uDBCount );
	m_vecMainDBConInfo.resize( uDBCount );

	for( uIter = 0 ; uIter < uDBCount ; ++uIter )
	{
		::wsprintf( m_strBuffer, g_lpDBConfig_MainDBInfo, uIter );

		for( uIter2 = 0 ; uIter2 < CDBConnectionInfo::CDBDM_MAX ; ++uIter2 )
		{
			::GetPrivateProfileString( 
				m_strBuffer, 
				lpDBInfoElement[uIter2], 
				"Error", 
				strRecv, 
				sizeof(strRecv),  
				lpConfigName
				);

			m_vecMainDBConInfo[uIter].GetStr( uIter2 ) = strRecv;
		}        
	}

	return TRUE;
}