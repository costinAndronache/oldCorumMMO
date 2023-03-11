#include "stdafx.h"
#include ".\hsdbl.h"
#include "assert.h"

CDBConnection::CDBConnection(void)
{
	::CoInitialize(NULL);
}

CDBConnection::~CDBConnection(void)
{
	::CoUninitialize();
}

bool CDBConnection::Init(const wstring &wstrDBSource, const wstring &wstrDBCatalog, const wstring &wstrUserID, const wstring &wstrUserPW)
{
	DBSource(wstrDBSource);
	DBCatalog(wstrDBCatalog);
	UserID(wstrUserID);
	UserPW(wstrUserPW);
	return OpenDB();
}

bool CDBConnection::OpenDB()
{// connect to the database
	wstring strCnn= L"Provider = SQLOLEDB.1"
					L"; Data Source="		+ DBSource()+
					L"; Initial Catalog="	+ DBCatalog()+
					L"; User Id="			+ UserID()+
					L"; Password="			+ UserPW();
//	Parameter Description //기타 옵션 있음.
//	Trusted_Connection
//	Current Language 
//	Network Address 
//	Network Library 
//	Use Procedure for Prepare Determines 
//	Auto Translate 
//	Packet Size 
//	Application Name 
//	Workstation ID 
	HRESULT hr = m_DataConnection.Open(strCnn.c_str());

	if(FAILED(hr))
	{
		cout << "Can't OpenDB" << endl;
		return false;
	}
	return true;
}

void CDBConnection::CloseDB()
{
	m_DataConnection.CloseDataSource();
}

const wstring CDBConnection::DBSource()const 
{
	return m_wstrDBSource;
}

const wstring CDBConnection::DBCatalog()const 
{
	return m_wstrDBCatalog;
}

const wstring CDBConnection::UserID()const 
{
	return m_wstrUserID;
}

const wstring CDBConnection::UserPW()const 
{
	return m_wstrUserPW;
}

void CDBConnection::DBSource(const wstring &wstrDBSource)
{
	m_wstrDBSource = wstrDBSource;
}

void CDBConnection::DBCatalog(const wstring &wstrDBCatalog)
{
	m_wstrDBCatalog = wstrDBCatalog;
}

void CDBConnection::UserID(const wstring &wstrUserID)
{
	m_wstrUserID = wstrUserID;
}

void CDBConnection::UserPW(const wstring &wstrUserPW)
{
	m_wstrUserPW = wstrUserPW;
}

//nvarchar 인가 그걸 쓰면 white 스트링이 없다.
void ClearWhiteString(IN string &strOrg)//공백문자를 처음부터 쓰게 되면 문제가 있다.
{// nvarchar 인가..그거 쓰면 끝에 공백 문자 없다.
	const char szSpace = ' ';
	
	size_t idx = strOrg.find(szSpace);//공백문자를 찾고
	if(string::npos != idx)//못찾은게 아니라면
	{//나머지 부분을 삭제.
		strOrg.at(idx) = '\0';
		string strTemp(strOrg.c_str());
		strOrg.swap(strTemp);// 
	}
//	char *pDest = ::strchr(szStr, szSpace);
//	(*pDest) = '\0';
}

size_t HS_MBTOWS(const string &strSrc, wstring &wstrTgt )
{
	const size_t src_size = strSrc.size();
	if(0 < src_size)
	{
		wstrTgt.resize( src_size );
		return mbstowcs(  &wstrTgt.at(0) ,   strSrc.c_str(),   src_size );
	} // if()
	return 0;
}
