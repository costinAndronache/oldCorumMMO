// LogCompare.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"		// �� ��ȣ
#include "HSDBL.h"
#include "DBConnectionInfo.h"

#define g_lpDBConfigFileName		_T(".\\DBConfig.ini")
#define g_lpDBConfig_Info			_T("INFO")
#define g_lpDBConfig_TotalDBCount	_T("TOTAL_DB_COUNT")
#define g_lpDBConfig_MainDBCount	_T("MAIN_DB_COUNT")
#define g_lpDBConfig_DBInfo			_T("DB_INFO_%02d")
#define g_lpDBConfig_MainDBInfo		_T("MAIN_DB_INFO_%02d")
#define g_lpDBConfig_DBName			_T("DB_NAME")
#define g_lpDBConfig_DBAddress		_T("DB_ADDRESS")
#define g_lpDBConfig_Account		_T("ACCOUNT")
#define g_lpDBConfig_Password		_T("PASSWORD")

// CLogCompareApp:
// �� Ŭ������ ������ ���ؼ��� LogCompare.cpp�� �����Ͻʽÿ�.
//

class CLogCompareApp : public CWinApp
{
public:
	CLogCompareApp();

// ������
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()

public:
	__inline CDBConnectionInfo&	GetDBConnectionInfo( UINT uIndex )
	{
		ASSERT( uIndex < m_uDBCount && m_vecDBConInfo.empty() == false );

		return m_vecDBConInfo[uIndex];
	}
	__inline CDBConnectionInfo&	GetMainDBConnectionInfo( UINT uIndex )
	{
		ASSERT( uIndex < m_uMainDBCount && m_vecMainDBConInfo.empty() == false );

		return m_vecMainDBConInfo[uIndex];
	}

	__inline	UINT	GetDBConnectionInfoSize()
	{
		return m_uDBCount;
	}
	__inline	UINT	GetMainDBConnectionInfoSize()
	{
		return m_uMainDBCount;
	}

private:	
	vector< CDBConnectionInfo >	m_vecDBConInfo;
	vector< CDBConnectionInfo >	m_vecMainDBConInfo;
	UINT						m_uDBCount;
	UINT						m_uMainDBCount;
    TCHAR						m_strBuffer[0xFF];	

	BOOL	InitializeDBConnection( LPCTSTR lpConfigName );
};

extern CLogCompareApp theApp;