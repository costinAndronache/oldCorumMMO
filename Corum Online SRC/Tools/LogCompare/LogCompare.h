// LogCompare.h : PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH에서 이 파일을 포함하기 전에 'stdafx.h'를 포함하십시오.
#endif

#include "resource.h"		// 주 기호
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
// 이 클래스의 구현에 대해서는 LogCompare.cpp을 참조하십시오.
//

class CLogCompareApp : public CWinApp
{
public:
	CLogCompareApp();

// 재정의
	public:
	virtual BOOL InitInstance();

// 구현

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