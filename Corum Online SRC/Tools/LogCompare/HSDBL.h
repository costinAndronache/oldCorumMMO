#pragma once
#include <atldbcli.h>
#include "STLSupport.H"

#pragma pack(push, 1)
extern void ClearWhiteString(IN string &strOrg);

class CDBConnection
{
public:
	CDBConnection(void);
	~CDBConnection(void);

public:
	bool Init(const wstring &wstrDBSource, const wstring &wstrDBCatalog, const wstring &wstrUserID, const wstring &wstrUserPW);
	
	void CloseDB();
	
	const wstring DBSource()const;
	const wstring DBCatalog()const;
	const wstring UserID()const;
	const wstring UserPW()const;

	void DBSource(const wstring &wstrDBSource);
	void DBCatalog(const wstring &wstrDBCatalog);
	void UserID(const wstring &wstrUserID);
	void UserPW(const wstring &wstrUserPW);

	void SetDataConnection(const CDataConnection &DataConnection)
	{
		m_DataConnection = DataConnection;
	}
	
	CDataConnection& DataConnection()
	{
		return m_DataConnection;
	}

protected:
	bool CheckSession()
	{//���� üũ
		if(DataConnection())
		{
			return true;
		}
		assert(NULL && "Incorrect Session");
		return false;
	}

private:
	bool OpenDB();
	wstring	m_wstrDBSource;
	wstring	m_wstrDBCatalog;
	wstring m_wstrUserID;
	wstring m_wstrUserPW;

	CDataConnection m_DataConnection;//CDataSource ,CSession �� ���� �ϰ� �ִ� ���� Ŭ����.;
//	CEnumerator m_Enumerator;
};

template <typename TResultTableData>
class CBasicQuery
	:	public CCommand< CAccessor< TResultTableData > >//CDynamicAccessor
	,	protected CDBConnection
{
//	example > --> ���̺��� ���Ǹ� ����� �ϱ⶧���� �״��� ���������� �ʴ�.
//	CBasicQuery<TalbleUserAccount> UserTable(g_AccountDBConn);// TalbleUserAccount OLE���� �����ϴ� ���̺� Ŭ����
//	UserTable.Execute("select * from [UserAccount]");
//	if(S_OK == UserTable.MoveFirst())
//	{
//		int xxxxx = 0;
//		UserTable.UserID;
//		UserTable.UserPW;
//	}

public:
	CBasicQuery(const CDataConnection &InDataConnection)
	{
		Close();	
		DataConnection(InDataConnection);
	}

	CBasicQuery(void){Close();}
	~CBasicQuery(void){Close();}

public:
	bool Execute(const string &strQuery, const bool bIsDBChange = false)//������� ������ ����.(EX>Select ����.)
	{
		if(!CheckSession())
		{
			return false;
		}

		Close();//�޾� �ξ��� ������� ���� ��Ű�� ���� ����.

		HRESULT hr = E_FAIL;
		if(!bIsDBChange)
		{
			hr = Open(DataConnection(), strQuery.c_str());//SQL Command�� ����°�
		}
		else
		{// open the dataset
			CDBPropSet propset(DBPROPSET_ROWSET);//The CDBPropSet controls the way the database gets opened        
			propset.AddProperty(DBPROP_CANFETCHBACKWARDS, true);
			propset.AddProperty(DBPROP_IRowsetScroll, true);
			propset.AddProperty(DBPROP_IRowsetChange, true);
			propset.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE );

			hr = Open(DataConnection(), strQuery.c_str(), &propset);//SQL Command�� ����°� //propset�� ���� ���� ������ ReadOnly�� �ȴ�.
		}

		if(FAILED(hr))
        {
			cout << "Can't Open DataBase Table" << endl;
			Close();
			ReleaseCommand();
			return false;
        }

		return true;
/****************************************
	EX> read all the data
	int line = 0;
	while(MoveNext() == S_OK)
	{	++line;
		m_CompanyName,
		m_ContactName ,
		m_Phone);
	}
	EX> Can Find?
	hr = MoveFirst();
	if(FAILED(hr))
	{
		return false;
	}
	return true;
*****************************************/
	}
};

class CArgQuery//�Ķ���Ͱ� �ִ� ����.
	: public CCommand<CDynamicParameterAccessor>
	, protected CDBConnection
{//Set�� �Ҷ� �Ķ������ ����� �����϶�. Ư�� String
//	Example >
//	CArgQuery DataTable(g_ElementDBConn);
//	DataTable.SetQuery("insert into [TBL_Character_Item] (CharacterName, Equip, Bank) VALUES (?,?,?)");
//
//	const DWORD dwCount = DataTable.GetParameterCount();
//	char acBLOB[255] = {0,};
//	DataTable.SetString(1, "�ӻ���");
//	DataTable.SetBinary(2, acBLOB, 50);
//	DataTable.SetBinary(3, acBLOB, 50);
//	
//	DataTable.Execute();
public:
	CArgQuery(const CDataConnection &InDataConnection)
	{
		SetDataConnection(InDataConnection);
		m_bIsResultQuery = false;
	}

//	CArgQuery(void){}
	~CArgQuery(void)
	{
		this->ReleaseCommand();
		if(m_bIsResultQuery)
		{
			this->ReleaseRows();
		}
	}
	
public:
	ULONG GetParameterCount()
	{
		if(!CheckSession())
		{
			return false;
		}

		return GetParamCount();//���������� �Ķ���� ������ �޴´�.
	}

	bool SetString(const ULONG dwIndex, const string &strString)
	{
		char* pParamOrg = GetParameter(dwIndex);
		if(pParamOrg)
		{
			::strcpy(pParamOrg,strString.c_str());
			return true;
		}
		return false;	
	}

	bool SetInt(const ULONG dwIndex, const __int32 iInt)
	{
		__int32* piParamOrg = (__int32*)GetParameter(dwIndex);

		if(piParamOrg)
		{
			*piParamOrg = iInt;
			return true;
		}
		return false;	
	}

	bool SetBinary(const ULONG dwIndex, const BYTE *pParamData, const DWORD dwDataSize)
	{
		if(!pParamData)
		{
			return false;
		}

		char* pParamOrg = GetParameter(dwIndex);
		
		DBLENGTH db_len = 0;
		
		if( this->GetParamSize(dwIndex, &db_len))
		{
			if(dwDataSize == db_len)
			{
				::memcpy(pParamOrg, pParamData, dwDataSize);
				return true;
			}
		} // if( this->GetParamSize(dwIndex, &len))

		return false;
	}

	bool SetBinary(const ULONG dwIndex, const vector< BYTE > &rvecOutData)
	{
		return SetBinary(dwIndex, &rvecOutData.at(0), static_cast<DWORD>( rvecOutData.size() ));
	}

public:
	bool SetQuery(const string &strQuery) throw()
	{
		if(!CheckSession())
		{
			return false;
		}

		m_strQuery = strQuery;

		HRESULT hr = S_OK;
		hr = Create(DataConnection(), strQuery.c_str());
		if(FAILED(hr))
		{
			assert( NULL && "SetQuery() -> Create() Failed");
			return false;
		}

		hr = Prepare();
		if(FAILED(hr))
		{
			assert( NULL && "SetQuery() -> Prepare() Failed");
			return false;
		}

		void* pDummy = NULL;
		hr = BindParameters( &m_hParameterAccessor, m_spCommand, &pDummy);// , true, true);//�ڿ� �� �κ��� true�� �� ������ ���̿� ���¸� �޾� �� �� �ִ�.
		if(FAILED(hr))
		{
			assert( NULL && "SetQuery() -> BindParameters() Failed");
			return false;
		}

		return true;
	}

	bool Execute(const bool bIsHaveResult)
	{
		if(!CheckSession())
		{
			return false;
		}
		
		HRESULT hr = Open( NULL, NULL, FALSE );//Ŀ�ǵ� Open.
		
		if (FAILED(hr))
		{
			IErrorInfo* pErrorInfo = NULL;
			BSTR bstrDesc = NULL;

			::GetErrorInfo(0,&pErrorInfo);
			pErrorInfo->GetDescription(&bstrDesc);
			
			//Not using smart pointers, release the memory
//			ACE_DEBUG( (LM_DEBUG, "Warning! Query Failed [%s]-\n", m_strQuery.c_str()));
			assert( NULL && "Query Failed");
			
			SysFreeString(bstrDesc);
			pErrorInfo->Release();
//			ReleaseCommand();
			return false;
		}
		m_bIsResultQuery = bIsHaveResult;
		if(bIsHaveResult)
		{
			Bind();//���ε�.
		}
//		LPOLESTR ps = TestTable.GetParamName(1);//�Ķ���� �̸� ��������.
		return true;
	}

	template <typename T1>
	bool GetResult(IN const ULONG lIndex, OUT T1 &rOutData)
	{
		bool bIsSuccess = false;// GetValue(lIndex, *Data);
		bIsSuccess = GetValue(lIndex, &rOutData);

		if(!bIsSuccess)
		{
			assert(bIsSuccess && "Can't GetData");
		}
		return bIsSuccess;
	}

	template <>//��������� string �����ؼ� �κ��� Ưȭ.
	bool GetResult(IN const ULONG lIndex, OUT string &rOutData)
	{
		bool bIsSuccess = false;// GetValue(lIndex, *Data);

		void *p = GetValue(lIndex);
		if(p)
		{
			rOutData.swap( string( (const char*)p) ); 
			bIsSuccess = true;
		}

		if(!bIsSuccess)
		{
			assert(bIsSuccess && "Can't GetData");
		}
		return bIsSuccess;
	}

/*	template <>//��������� BYTE(���̳ʸ�)�����ؼ� �κ��� Ưȭ.
	bool GetResult(IN const ULONG lIndex, OUT BYTE &rOutData)
	{//BYTE �����Ͱ� ����� �;� �Ѵ�.
		bool bIsSuccess = false;// GetValue(lIndex, *Data);

		void *p = GetValue(lIndex);
		if(p)
		{
			DBLENGTH uiLen = 0;
			if(GetLength(lIndex,&uiLen))
			{
				::memcpy(&rOutData, p, sizeof(uiLen));
				bIsSuccess = true;
			}
		}

		if(!bIsSuccess)
		{
			assert(bIsSuccess && "Can't GetData");
		}
		return bIsSuccess;
	}*/

	template <>//��������� BLOB ������ �޴°� Ưȭ 
	bool GetResult(IN const ULONG lIndex, OUT vector< BYTE > &rvecOutData)
	{//vector���� ���̸� ����� �־����.
		bool bIsSuccess = false;// GetValue(lIndex, *Data);

		void *p = GetValue(lIndex);
		if(p)
		{//���̳ʸ����� 
			DBLENGTH uiLen = 0;
			if( GetLength(lIndex, &uiLen) )
			{
				if(0 < uiLen )
				{
//					rvecOutData.reserve(uiLen);
//					rvecOutData.resize(uiLen);
					if(rvecOutData.size() == uiLen)
					{
						::memcpy(&rvecOutData.at(0), p, uiLen);
						bIsSuccess = true;
					}					
				} // if(0 < uiLen )
				else if( 0 == uiLen)
				{
//					rvecOutData.resize(0);
					bIsSuccess = true;
				}
				//������ ���� ����
			}
		}

		if(!bIsSuccess)
		{
			assert(bIsSuccess && "Can't GetData");
		}
		return bIsSuccess;
	}

	template <>//��������� boolean �����ؼ� �κ��� Ưȭ.
	bool GetResult(IN const ULONG lIndex, OUT bool &rOutData)
	{//BYTE �����Ͱ� ����� �;� �Ѵ�.
		bool bIsSuccess = false;// GetValue(lIndex, *Data);

		void *p = GetValue(lIndex);
		if(p)
		{
			if(! *(char*)p)
			{
				rOutData = false;
			}
			else
			{
				rOutData = true;
			}
			bIsSuccess = true;
		}

		if(!bIsSuccess)
		{
			assert(bIsSuccess && "Can't GetData");
		}
		return bIsSuccess;
	}

private:
	char* GetParameter(const DWORD dwIndex)//GetParam() �� �̸��� ��ġ�� �ʰ� ����.
	{
		if(!CheckSession())
		{
			return false;
		}

		char* pParamOrg = (char*)GetParam( dwIndex );
		return pParamOrg;
	}
	
	bool m_bIsResultQuery;
	string m_strQuery;
};

extern size_t HS_MBTOWS(const string &strSrc, wstring &wstrTgt );

#pragma pack(pop, 1)