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
//	Parameter Description //��Ÿ �ɼ� ����.
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

//nvarchar �ΰ� �װ� ���� white ��Ʈ���� ����.
void ClearWhiteString(IN string &strOrg)//���鹮�ڸ� ó������ ���� �Ǹ� ������ �ִ�.
{// nvarchar �ΰ�..�װ� ���� ���� ���� ���� ����.
	const char szSpace = ' ';
	
	size_t idx = strOrg.find(szSpace);//���鹮�ڸ� ã��
	if(string::npos != idx)//��ã���� �ƴ϶��
	{//������ �κ��� ����.
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
