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
	{//세션 체크
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

	CDataConnection m_DataConnection;//CDataSource ,CSession 을 포함 하고 있는 랩핑 클래스.;
//	CEnumerator m_Enumerator;
};

template <typename TResultTableData>
class CBasicQuery
	:	public CCommand< CAccessor< TResultTableData > >//CDynamicAccessor
	,	protected CDBConnection
{
//	example > --> 테이블별로 정의를 해줘야 하기때문에 그다지 쓸모있지는 않다.
//	CBasicQuery<TalbleUserAccount> UserTable(g_AccountDBConn);// TalbleUserAccount OLE에서 정의하는 테이블 클래스
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
	bool Execute(const string &strQuery, const bool bIsDBChange = false)//결과물이 나오는 쿼리.(EX>Select 쿼리.)
	{
		if(!CheckSession())
		{
			return false;
		}

		Close();//받아 두었던 결과물은 삭제 시키는 것이 좋다.

		HRESULT hr = E_FAIL;
		if(!bIsDBChange)
		{
			hr = Open(DataConnection(), strQuery.c_str());//SQL Command를 만드는것
		}
		else
		{// open the dataset
			CDBPropSet propset(DBPROPSET_ROWSET);//The CDBPropSet controls the way the database gets opened        
			propset.AddProperty(DBPROP_CANFETCHBACKWARDS, true);
			propset.AddProperty(DBPROP_IRowsetScroll, true);
			propset.AddProperty(DBPROP_IRowsetChange, true);
			propset.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE );

			hr = Open(DataConnection(), strQuery.c_str(), &propset);//SQL Command를 만드는것 //propset을 셋팅 하지 않으면 ReadOnly가 된다.
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

class CArgQuery//파라메터가 있는 쿼리.
	: public CCommand<CDynamicParameterAccessor>
	, protected CDBConnection
{//Set을 할때 파라메터의 사이즈를 조심하라. 특히 String
//	Example >
//	CArgQuery DataTable(g_ElementDBConn);
//	DataTable.SetQuery("insert into [TBL_Character_Item] (CharacterName, Equip, Bank) VALUES (?,?,?)");
//
//	const DWORD dwCount = DataTable.GetParameterCount();
//	char acBLOB[255] = {0,};
//	DataTable.SetString(1, "임상우님");
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

		return GetParamCount();//쿼리문에서 파라메터 갯수를 받는다.
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
		hr = BindParameters( &m_hParameterAccessor, m_spCommand, &pDummy);// , true, true);//뒤에 두 부분을 true로 해 놓으면 길이와 상태를 받아 올 수 있다.
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
		
		HRESULT hr = Open( NULL, NULL, FALSE );//커맨드 Open.
		
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
			Bind();//바인딩.
		}
//		LPOLESTR ps = TestTable.GetParamName(1);//파라메터 이름 가져오기.
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

	template <>//명시적으로 string 에관해서 부분적 특화.
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

/*	template <>//명시적으로 BYTE(바이너리)에관해서 부분적 특화.
	bool GetResult(IN const ULONG lIndex, OUT BYTE &rOutData)
	{//BYTE 포인터가 제대로 와야 한다.
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

	template <>//명시적으로 BLOB 데이터 받는곳 특화 
	bool GetResult(IN const ULONG lIndex, OUT vector< BYTE > &rvecOutData)
	{//vector값의 길이를 제대로 넣어야함.
		bool bIsSuccess = false;// GetValue(lIndex, *Data);

		void *p = GetValue(lIndex);
		if(p)
		{//바이너리값이 
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
				//나머지 경우는 에러
			}
		}

		if(!bIsSuccess)
		{
			assert(bIsSuccess && "Can't GetData");
		}
		return bIsSuccess;
	}

	template <>//명시적으로 boolean 에관해서 부분적 특화.
	bool GetResult(IN const ULONG lIndex, OUT bool &rOutData)
	{//BYTE 포인터가 제대로 와야 한다.
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
	char* GetParameter(const DWORD dwIndex)//GetParam() 과 이름이 겹치지 않게 주의.
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