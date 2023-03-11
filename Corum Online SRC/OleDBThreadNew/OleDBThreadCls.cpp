#include "stdafx.h"
#include "OleDBThreadCls.h"

extern LONG g_cObjects;
extern LONG g_cLocks;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI OLEDBThread(LPVOID lpVoid)
{
	COleDBThreadCls* pDb = (COleDBThreadCls*)lpVoid;
	return pDb->OleDBThreadFunc();
}

VOID CALLBACK InformInputQueryThreadAPC(ULONG_PTR dwParam)
{
	dwParam;
}

////////////////////////////////////////////////////////////////////////////////////////
HRESULT __stdcall COleDBThreadCls::QueryInterface(REFIID riid, LPVOID* ppv)
{
	HRESULT hr = E_NOINTERFACE;
	*ppv = NULL;

	if(riid == IID_IUnknown || riid == IID_OLEDBTHREAD)
		*ppv = (IOleDBThread*)this;
	
	if(*ppv != NULL) 
	{
		AddRef();
		hr = S_OK;
	}

	return hr;
}

ULONG __stdcall COleDBThreadCls::AddRef()
{
	return ++m_cRef;
}

ULONG __stdcall COleDBThreadCls::Release()
{
	if(--m_cRef == 0) 
	{
		delete this;
	}

	return m_cRef;
}

/////////////////////////////////////////////////////////////////////////////////////////
COleDBThreadCls::COleDBThreadCls()
{
	CoInitialize(NULL);

	m_cRef					= 0;
	m_bDefault				= 0;
	m_bCloseQueryForThread	= 0;

	m_pInputQueue			= NULL;
	m_pOutputMsgPool		= NULL;

	m_pInputMsgPool			= NULL;
	m_pOutputMsgPool		= NULL;
	m_pParamPool			= NULL;
	m_pResultPool			= NULL;			//결과값 Buffer 생성용 
	m_pBindingPool			= NULL;			//DBBINDING 생성용 

	memset(m_pDBObjectList, 0, sizeof(m_pDBObjectList));

	m_hDBKillEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
}

COleDBThreadCls::~COleDBThreadCls()
{
	if(m_InitDesc.bUsingThread)		//쓰레드 사용하면 
	{
		SetEvent( m_hDBKillEvent );
		WaitForSingleObject( m_hDBThread, INFINITE );

		CloseHandle(m_hDBThread);
		CloseHandle(m_hDBKillEvent);
	}
	
	for(int i = 0; i < MAX_DB_CONNECTION; i++)
	{
		if(m_pDBObjectList[i])
			delete m_pDBObjectList[i];
	}

	if(m_pInputQueue)		
		delete m_pInputQueue;

	if(m_InitDesc.bUsingEventObject && m_pOutputMsgPool)	
		delete m_pOutputMsgPool;

	if(m_pInputMsgPool)		delete m_pInputMsgPool;
	if(m_pParamPool)		delete m_pParamPool;
	if(m_pResultPool)		delete m_pResultPool;		
	if(m_pBindingPool)		delete m_pBindingPool;	

	CoUninitialize();
}

BOOL __stdcall COleDBThreadCls::KSCToUnicode(const char *pKsc, WCHAR *pUni)
{
	int nUniSize = 0;
	int nKscSize = strlen(pKsc);

	if(nKscSize <= 0)	return FALSE;
	
	//1. 먼저 유니코드로 변환하고자 하는 문자열의 크기를 알아낸다.
	nUniSize = MultiByteToWideChar(CP_ACP, 0, pKsc, nKscSize+1,NULL,0);

	//2. 유니코드의 크기(uni_size)와 변환하고자하는 소스문자열(pViewString)과 목적문자열(string)을 이용한다.
	MultiByteToWideChar(CP_ACP, 0, pKsc, nKscSize, pUni, nUniSize*2+100);

	pUni[nUniSize] = L'\0';
	
	return TRUE;
}

BOOL __stdcall COleDBThreadCls::UnicodeToKSC(WCHAR *pUni, char *pKsc, int nKscSize)
{
	int nMultibyteSize = 0;
	nMultibyteSize = WideCharToMultiByte(CP_ACP, 0, pUni, wcslen(pUni)+1, pKsc, nKscSize, NULL, 0);

	pKsc[ nMultibyteSize ] = '\0';
	return TRUE;
}

inline void COleDBThreadCls::ProtectInputMessage(BOOL bProtect)
{
	m_bCloseQueryForThread = bProtect;
}

void COleDBThreadCls::THIgnoreQueryInput()
{
	ProtectInputMessage(TRUE);
}

void COleDBThreadCls::THAcceptQueryInput()
{
	ProtectInputMessage(FALSE);
}

BOOL COleDBThreadCls::CheckSafeIndex(BYTE bConnectionIndex)
{
	return !(bConnectionIndex > MAX_DB_CONNECTION || !m_pDBObjectList[ bConnectionIndex ]);
}

BOOL __stdcall COleDBThreadCls::SetDefaultDBIndex(BYTE bConnectionIndex)
{
	if(!CheckSafeIndex(BYTE(bConnectionIndex - 1)))	
		return FALSE;
	
	m_bDefault = BYTE(bConnectionIndex - 1);

	return TRUE;
}

BYTE __stdcall COleDBThreadCls::GetDefaultDBIndex()
{
	return BYTE(m_bDefault + 1);
}

BOOL __stdcall COleDBThreadCls::InitDBModule(DB_INITIALIZE_DESC* desc)
{	
	m_dwOLEDBSize = 0;

	memcpy(&m_InitDesc, desc, sizeof(m_InitDesc));

	if(m_InitDesc.bUsingThread)
	{
		_CHECK_MEMORY();

		m_pInputMsgPool = new CThreadSafeStaticMemPool(sizeof(DBCommandMsg), m_InitDesc.wMaxNumOfProcessMessage_Input/2, m_InitDesc.wMaxNumOfProcessMessage_Input);
		if(!m_pInputMsgPool)
			__asm int 3

		_CHECK_MEMORY();
		
		m_dwOLEDBSize += (sizeof(DBCommandMsg) * m_InitDesc.wMaxNumOfProcessMessage_Input);

		m_pResultPool = new CThreadSafeStaticMemPool(desc->dwMaxRowSize * desc->wMaxReturnedRowNum, desc->wMaxNumOfProcessMessage_Output/2, desc->wMaxNumOfProcessMessage_Output);
		if(!m_pResultPool)
			__asm int 3

		_CHECK_MEMORY();

		m_dwOLEDBSize += (desc->dwMaxRowSize * desc->wMaxReturnedRowNum * desc->wMaxNumOfProcessMessage_Output);
		
		m_pBindingPool = new CThreadSafeStaticMemPool(sizeof(DBBINDING)*desc->wMaxReturnedColNum, desc->wMaxNumOfProcessMessage_Output/2, desc->wMaxNumOfProcessMessage_Output);
		if(!m_pBindingPool)
			__asm int 3		

		_CHECK_MEMORY();

		m_dwOLEDBSize += (sizeof(DBBINDING)*desc->wMaxReturnedColNum * desc->wMaxNumOfProcessMessage_Output);

		m_pOutputMsgPool = new CThreadSafeStaticMemPool(sizeof(DBRECEIVEDATA), desc->wMaxNumOfProcessMessage_Output/2, desc->wMaxNumOfProcessMessage_Output);
		if(!m_pOutputMsgPool)
			__asm int 3

		_CHECK_MEMORY();

		m_dwOLEDBSize += (sizeof(DBRECEIVEDATA) * desc->wMaxNumOfProcessMessage_Output);

		m_pParamPool = new CThreadSafeStaticMemPool(sizeof(DBBINDING) * desc->bMaxParamNum, desc->wMaxNumOfProcessMessage_Input/2, desc->wMaxNumOfProcessMessage_Input);
		if(!m_pParamPool)	__asm int 3

		_CHECK_MEMORY();

		m_dwOLEDBSize += (sizeof(DBRECEIVEDATA) * desc->bMaxParamNum * desc->wMaxNumOfProcessMessage_Input);

		//명령을 받을 2개의 메세지 큐 생성 
		m_pInputQueue = new CMsgQueue(m_InitDesc.wMaxNumOfProcessMessage_Input);
		if(!m_pInputQueue)
			__asm int 3

		_CHECK_MEMORY();

		if(desc->bUsingEventObject)
		{
			//출력용 2개의 메세지 큐 생성 
			m_pOutputQueue = new CMsgQueue(m_InitDesc.wMaxNumOfProcessMessage_Output); 				
			if(!m_pOutputQueue)
				__asm int 3
		}
		
		_CHECK_MEMORY();

		DWORD dwThreadID;
		m_hDBThread	= BEGINTHREADEX(NULL, NULL, OLEDBThread, this, NULL, &dwThreadID);				
	}

	return TRUE;
}

int	__stdcall COleDBThreadCls::CreateDBConnection()
{		
	for(int i = 0; i < MAX_DB_CONNECTION; i++)
	{
		if(!m_pDBObjectList[i])
		{			
			m_pDBObjectList[i] = new COleDBThread(&m_InitDesc, this);
			return (i+1);
		}
	}

	return -1;
}

BOOL __stdcall COleDBThreadCls::ReleaseDBConnectionForSingleThread(BYTE bConnectionIndex)
{
	if(bConnectionIndex)	//특정 DB만 Release
	{
		int index = bConnectionIndex - 1;

		if(!CheckSafeIndex( BYTE(index) ))	
			return FALSE;

		delete m_pDBObjectList[ index ];
		
		m_pDBObjectList[ index ] = NULL;

		return TRUE;
	}
	else	//모든 DB Release
	{
		for(int i=0; i < MAX_DB_CONNECTION ; i++)
		{
			if(m_pDBObjectList[i])		
				delete m_pDBObjectList[i];
		}	
		
		memset(m_pDBObjectList, 0, sizeof(m_pDBObjectList));
		
		return TRUE;
	}
}

int	__stdcall COleDBThreadCls::Connect(const char* szDataSource, const char* szDefaultDb, const char* szUserId, 
										const char* szPassword, int nConnectTimeout, int nQuerytimeout, BOOL bCommandTimeOut, BYTE bConnectionIndex)
{
	WCHAR	wszDataSource[256] = {0,};
	WCHAR	wszDefaultDb[256] = {0,};
	WCHAR	wszUserId[256] = {0,};
	WCHAR	wszPassword[256] = {0,};
	
	KSCToUnicode(szDataSource, wszDataSource);			
	KSCToUnicode(szDefaultDb, wszDefaultDb);			
	KSCToUnicode(szUserId, wszUserId);			
	KSCToUnicode(szPassword, wszPassword);			

	WORD wRet = GetResultIndex(bConnectionIndex);

	if(wRet == 0xffff)
		return -1;
	
	return m_pDBObjectList[wRet]->Initialize(wszDataSource, wszDefaultDb, wszUserId, wszPassword, nConnectTimeout, nQuerytimeout, bCommandTimeOut);	
}

int	__stdcall COleDBThreadCls::Connect(const char* szDataSource, const char* szDefaultDb, const char* szUserId, 
										const char* szPassword, int nConnectTimeout, BYTE bConnectionIndex)
{
	WCHAR	wszDataSource[256] = {0,};
	WCHAR	wszDefaultDb[256] = {0,};
	WCHAR	wszUserId[256] = {0,};
	WCHAR	wszPassword[256] = {0,};	
	
	KSCToUnicode(szDataSource, wszDataSource);			
	KSCToUnicode(szDefaultDb, wszDefaultDb);			
	KSCToUnicode(szUserId, wszUserId);			
	KSCToUnicode(szPassword, wszPassword);			

	WORD wRet = GetResultIndex(bConnectionIndex);

	if(wRet == 0xffff)
		return -1;
	
	return m_pDBObjectList[wRet]->Initialize(wszDataSource, wszDefaultDb, wszUserId, wszPassword, nConnectTimeout);	
}

int	__stdcall COleDBThreadCls::ExecuteSQL(char* szQuerySQL, BYTE bConnectionIndex)
{
	WCHAR wszQuerySQL[ MAX_SQL_STRING_LENGTH + 1 ]  = {0,};
	KSCToUnicode(szQuerySQL, wszQuerySQL);

	WORD wRet = GetResultIndex(bConnectionIndex);

	if(wRet == 0xffff)
		return -1;

	return m_pDBObjectList[wRet]->ExecuteSQL(wszQuerySQL);
}

int	__stdcall COleDBThreadCls::ExecuteSQLByParam(char* szQuerySQL, DBBINDING* pBinding, void* pParamValue,
													BYTE bParamNum, BYTE bConnectionIndex)
{
	WCHAR wszQuerySQL[ MAX_SQL_STRING_LENGTH + 1 ] = {0,};
	KSCToUnicode(szQuerySQL, wszQuerySQL);

	WORD wRet = GetResultIndex(bConnectionIndex);

	if(wRet == 0xffff)
		return -1;

	return m_pDBObjectList[wRet]->ExecuteSQLByParam(wszQuerySQL, pBinding, pParamValue, bParamNum);
}

DBBINDING*  __stdcall COleDBThreadCls::CreateParamInfo(WORD wParamNum, BYTE bConnectionIndex)
{
	WORD wRet = GetResultIndex(bConnectionIndex);

	if(wRet == 0xffff)
		return NULL;

	return m_pDBObjectList[wRet]->CreateParamInfo(wParamNum);	
}

BOOL __stdcall COleDBThreadCls::ReleaseParamInfo(DBBINDING* pBinding, BYTE bConnectionIndex)
{
	WORD wRet = GetResultIndex(bConnectionIndex);

	if(wRet == 0xffff)
		return NULL;

	return m_pDBObjectList[wRet]->ReleaseParamInfo(pBinding);	
}

DBBINDING*  __stdcall COleDBThreadCls::THCreateParamInfo(WORD wParamNum)
{
	DBBINDING* pDbBinding = (DBBINDING*)GetParamPool()->Alloc();
	if(!pDbBinding)	
	{
		OutputMessage("OLEDBThread Error! - Parameter Memory Pool is Full!");
		return NULL;
	}
	
//	memset(pDbBinding, 0, sizeof(DBBINDING));

	for(int i = 0; i < (int)wParamNum; i++)
    {
	    pDbBinding[i].obLength	= 0;
        pDbBinding[i].obStatus	= 0;
        pDbBinding[i].pTypeInfo = NULL;
        pDbBinding[i].pObject	= NULL;
        pDbBinding[i].pBindExt	= NULL;
        pDbBinding[i].dwPart	= DBPART_VALUE;
        pDbBinding[i].dwMemOwner= DBMEMOWNER_CLIENTOWNED;
        pDbBinding[i].dwFlags	= 0;
        pDbBinding[i].bScale	= 0;
		pDbBinding[i].iOrdinal	= i+1;
	    pDbBinding[i].eParamIO	= DBPARAMIO_INPUT;
	    pDbBinding[i].bPrecision= 11;
	} //endfor	

	return pDbBinding;
}

void COleDBThreadCls::THReleaseParamInfo(DBBINDING* pBinding)
{
	if(!pBinding)
	{
		OutputMessage("[OLEDB Fatal Error] pBinding is NULL Entered at THReleaseParamInfo Method!");
		return;
	}

	GetParamPool()->Free(pBinding);
}

int	__stdcall COleDBThreadCls::OpenRecord(char* szQuerySQL, void* pRecordSet, DWORD dwMaxNumRows, BYTE bConnectionIndex)
{
	WCHAR wszQuerySQL[ MAX_SQL_STRING_LENGTH + 1 ] = {0,};
	KSCToUnicode(szQuerySQL, wszQuerySQL);

	WORD wRet = GetResultIndex(bConnectionIndex);

	if(wRet == 0xffff)
		return -1;

	return m_pDBObjectList[wRet]->OpenRecord(wszQuerySQL, pRecordSet, dwMaxNumRows);	
}

DBRECEIVEDATA*  __stdcall COleDBThreadCls::OpenRecordEx(char* szQuerySQL, DWORD dwMaxNumRows, DWORD dwRowPerRead, BYTE bConnectionIndex)
{
	WCHAR wszQuerySQL[ MAX_SQL_STRING_LENGTH + 1 ] = {0,};
	KSCToUnicode(szQuerySQL, wszQuerySQL);

	WORD wRet = GetResultIndex(bConnectionIndex);

	if(wRet == 0xffff)
		return NULL;

	return m_pDBObjectList[wRet]->OpenRecordEx(wszQuerySQL, dwMaxNumRows, dwRowPerRead);	
}

DBCOLUMNINFO*	__stdcall COleDBThreadCls::GetColumnInfo(char* szQuerySQL,  DWORD* pColnum, BYTE bConnectionIndex)
{
	WCHAR wszQuerySQL[ MAX_SQL_STRING_LENGTH + 1 ] = {0,};
	KSCToUnicode(szQuerySQL, wszQuerySQL);	

	WORD wRet = GetResultIndex(bConnectionIndex);

	if(wRet == 0xffff)
		return NULL;

	return m_pDBObjectList[wRet]->GetColumnInfo(wszQuerySQL, pColnum);	
}

BOOL __stdcall COleDBThreadCls::GetData(void* pReceiveData, DBRECEIVEDATA* pResultData, 
										DWORD dwRowNum, WORD wColumnNum, BYTE bConnectionIndex)
{
	WORD wRet = GetResultIndex(bConnectionIndex);

	if(wRet == 0xffff)
		return FALSE;

	return m_pDBObjectList[wRet]->GetData(pReceiveData, pResultData, dwRowNum, wColumnNum);	
}

BOOL __stdcall COleDBThreadCls::ReleaseRecordset(DBRECEIVEDATA* pResultData, BYTE bConnectionIndex)
{
	WORD wRet = GetResultIndex(bConnectionIndex);

	if(wRet == 0xffff)
		return FALSE;

	return m_pDBObjectList[wRet]->ReleaseRecordset(pResultData);
}

int	__stdcall COleDBThreadCls::GetBLOBData(char* szTableName, char* szColumnName, char* szFilter, 
												void* pReadBuffer, DWORD dwReadSize, BYTE bConnectionIndex)
{
	WCHAR wszTableName[ 256 ] = {0,};
	WCHAR wszColumnName[ 256 ] = {0,};
	WCHAR wszFilter[ 256 ] = {0,};

	KSCToUnicode(szTableName, wszTableName);
	KSCToUnicode(szColumnName, wszColumnName);
	KSCToUnicode(szFilter, wszFilter);

	WORD wRet = GetResultIndex(bConnectionIndex);

	if(wRet == 0xffff)
		return -1;

	return m_pDBObjectList[wRet]->GetBLOBData(wszTableName, wszColumnName, wszFilter, pReadBuffer, dwReadSize);	
}

int	__stdcall COleDBThreadCls::GetBLOBData(char* szTableName, char* szColumnName, char* szFilter, 
												char* szFilePath, DWORD dwReadSize, BYTE bConnectionIndex)
{
	WCHAR wszTableName[ 256 ] = {0,};
	WCHAR wszColumnName[ 256 ] = {0,};
	WCHAR wszFilter[ 256 ] = {0,};
	WCHAR wszFilePath[ MAX_PATH + 1 ] = {0,};	

	KSCToUnicode(szTableName, wszTableName);
	KSCToUnicode(szColumnName, wszColumnName);
	KSCToUnicode(szFilter, wszFilter);
	KSCToUnicode(szFilePath, wszFilePath);

	WORD wRet = GetResultIndex(bConnectionIndex);

	if(wRet == 0xffff)
		return -1;

	return m_pDBObjectList[wRet]->GetBLOBData(wszTableName, wszColumnName, wszFilter, wszFilePath, dwReadSize);	
}

int __stdcall COleDBThreadCls::SetBLOBData(char* szTableName, char* szColumnName, char* szFilter, 
											void* pWriteData,  DWORD dwWriteSize, BYTE bConnectionIndex)
{
	WCHAR wszTableName[ 256 ] = {0,};
	WCHAR wszColumnName[ 256 ] = {0,};
	WCHAR wszFilter[ 256 ] = {0,};

	KSCToUnicode(szTableName, wszTableName);
	KSCToUnicode(szColumnName, wszColumnName);
	KSCToUnicode(szFilter, wszFilter);

	WORD wRet = GetResultIndex(bConnectionIndex);

	if(wRet == 0xffff)
		return -1;

	return m_pDBObjectList[wRet]->SetBLOBData(wszTableName, wszColumnName, wszFilter, pWriteData, dwWriteSize);	
}

int __stdcall COleDBThreadCls::SetBLOBData(char* szTableName, char* szColumnName, char* szFilter, 
											char* szFilePath, BYTE bConnectionIndex)
{
	WCHAR wszTableName[ 256 ] = {0,};
	WCHAR wszColumnName[ 256 ] = {0,};
	WCHAR wszFilter[ 256 ] = {0,};
	WCHAR wszFilePath[ MAX_PATH + 1 ] = {0,};	

	KSCToUnicode(szTableName, wszTableName);
	KSCToUnicode(szColumnName, wszColumnName);
	KSCToUnicode(szFilter, wszFilter);
	KSCToUnicode(szFilePath, wszFilePath);

	WORD wRet = GetResultIndex(bConnectionIndex);

	if(wRet == 0xffff)
		return -1;

	return m_pDBObjectList[wRet]->SetBLOBData(wszTableName, wszColumnName, wszFilter, wszFilePath);	
}

BOOL __stdcall COleDBThreadCls::ChangeDB(char* szDbName, BYTE bConnectionIndex)
{
	WCHAR wszDbName[ 256 ] = {0,};
	KSCToUnicode(szDbName, wszDbName);
	
	WORD wRet = GetResultIndex(bConnectionIndex);

	if(wRet == 0xffff)
		return -1;

	return m_pDBObjectList[wRet]->ChangeDB(wszDbName);	
}

int __stdcall COleDBThreadCls::QueryDBCatalog(DBSCHEMA* pSchemaBuf, DWORD dwMaxNumRows, BYTE bConnectionIndex)
{
	WORD wRet = GetResultIndex(bConnectionIndex);

	if(wRet == 0xffff)
		return -1;

	return m_pDBObjectList[wRet]->QueryDBCatalog(pSchemaBuf, dwMaxNumRows);	
}

int __stdcall COleDBThreadCls::QueryDBTable(DBSCHEMA* pSchemaBuf, DWORD dwMaxNumRows, BYTE bConnectionIndex)
{
	WORD wRet = GetResultIndex(bConnectionIndex);

	if(wRet == 0xffff)
		return -1;

	return m_pDBObjectList[wRet]->QueryDBTable(pSchemaBuf, dwMaxNumRows);	
}

void __stdcall COleDBThreadCls::THOpenRecord(char* szQuerySQL, DWORD dwQueryUID,  void* pData, DWORD dwMaxNumRows, BYTE bConnectionIndex)
{	
	LPDBCMDMSG pMsg = (LPDBCMDMSG)GetInputMsgPool()->Alloc();
	if(!pMsg)
	{
		char szMsg[1024] = {0,};
		sprintf(szMsg, "[OLEDB Fatal Error] InputMessagePool Overflow!(COleDBThreadCls::THOpenRecord, Query:%s)", szQuerySQL);
		OutputMessage(szMsg);
		return;
	}
	
//	memset(pMsg, 0, sizeof(DBCommandMsg));

	pMsg->bQueryType = QUERY_TYPE_SELECT;						//OpenRecord
	if(!bConnectionIndex)
	{
		pMsg->bConnectionIndex = m_bDefault;					//ConnectionIndex
	}
	else
	{
		if(!CheckSafeIndexChk(BYTE(bConnectionIndex - 1)))
			return;		
			
		pMsg->bConnectionIndex = BYTE(bConnectionIndex-1);
	}

	KSCToUnicode(szQuerySQL, pMsg->wszQuery);					//Query String	
	pMsg->AddInfo.select.dwMaxNumRows = dwMaxNumRows;			
	pMsg->AddInfo.select.dwRowPerRead = DEFAULT_ROWS_PER_READ;
	pMsg->dwQueryUID = dwQueryUID;
	pMsg->pData = pData;
	
	PostDBMessage(pMsg);	
}

void __stdcall COleDBThreadCls::THExecuteSQL(char* szQuerySQL,  BYTE bReturnResult, DWORD dwQueryUID,  void* pData, BYTE bConnectionIndex)
{
	LPDBCMDMSG pMsg = (LPDBCMDMSG)GetInputMsgPool()->Alloc();
	if(!pMsg)	
	{
		char szMsg[1024] = {0,};
		sprintf(szMsg, "[OLEDB Fatal Error] InputMessagePool Overflow!(COleDBThreadCls::THExecuteSQL, Query:%s)", szQuerySQL);
		OutputMessage(szMsg);
		return;
	}

//	memset(pMsg, 0, sizeof(DBCommandMsg));

	pMsg->bQueryType = QUERY_TYPE_EXECUTE;

	if(!bConnectionIndex)
	{
		pMsg->bConnectionIndex = m_bDefault;					//ConnectionIndex
	}
	else
	{
		if(!CheckSafeIndexChk(BYTE(bConnectionIndex - 1)))		
			return;

		pMsg->bConnectionIndex = BYTE(bConnectionIndex-1);
	}

	KSCToUnicode(szQuerySQL, pMsg->wszQuery);					//Query String	
	pMsg->AddInfo.execute.bReturnResult = bReturnResult;		//몇행영향을 받았나 결과값 받을것인지 안받을것인지 
	pMsg->dwQueryUID = dwQueryUID;
	pMsg->pData = pData;

	PostDBMessage(pMsg);
}

void __stdcall COleDBThreadCls::THExecuteSQLByParam(char* szQuerySQL, DBBINDING* pBinding, void* pParamValue, DWORD dwParamValueSize, BYTE bParamNum,
										BYTE bReturnResult, DWORD dwQueryUID,  void* pData, BYTE bConnectionIndex)
{
	if(dwParamValueSize > MAX_PARAM_VALUE_SIZE)
	{
		OutputMessage("[OLEDB Fatal Error] Param value size overflow!(COleDBThreadCls::THExecuteSQLByParam)");
		return;
	}

	LPDBCMDMSG pMsg = (LPDBCMDMSG)GetInputMsgPool()->Alloc();
	if(!pMsg)	
	{
		char szMsg[1024] = {0,};
		sprintf(szMsg, "[OLEDB Fatal Error] InputMessagePool Overflow!(COleDBThreadCls::THExecuteSQLByParam, Query:%s)", szQuerySQL);
		OutputMessage(szMsg);
		return;
	}

//	memset(pMsg, 0, sizeof(DBCommandMsg));

	pMsg->bQueryType = QUERY_TYPE_EXECUTE_BY_PARAM;

	if(!bConnectionIndex)
	{
		pMsg->bConnectionIndex = m_bDefault;					//ConnectionIndex
	}
	else
	{
		if(!CheckSafeIndexChk(BYTE(bConnectionIndex - 1)))			
			return;
		
		pMsg->bConnectionIndex = BYTE(bConnectionIndex-1);
	}

	KSCToUnicode(szQuerySQL, pMsg->wszQuery);	
	pMsg->AddInfo.execute_by_param.bReturnResult = bReturnResult;
	pMsg->AddInfo.execute_by_param.pBinding = pBinding;
	pMsg->AddInfo.execute_by_param.bParamNum = bParamNum;
	memcpy(pMsg->AddInfo.execute_by_param.szParamValueBuf , pParamValue, dwParamValueSize);
	pMsg->dwQueryUID = dwQueryUID;
	pMsg->pData = pData;

	PostDBMessage(pMsg);

#ifdef _DEBUG
	if( !_CrtCheckMemory( ) ) 
	{
		OutputMessage("[OLEDB Fatal Error] _CrtCheckMemory returned zero!(COleDBThreadCls::THExecuteSQLByParam)");
		return;
	}
#endif
}

BOOL __stdcall COleDBThreadCls::THChangeDB(char* szDbName, BYTE bReturnResult, DWORD dwQueryUID, BYTE bConnectionIndex)
{
	LPDBCMDMSG pMsg = (LPDBCMDMSG)GetInputMsgPool()->Alloc();
	if(!pMsg)	
	{
		OutputMessage("[OLEDB Fatal Error] InputMessagePool Overflow!(COleDBThreadCls::THChangeDB)");
		return FALSE;
	}

//	memset(pMsg, 0, sizeof(DBCommandMsg));

	pMsg->bQueryType = QUERY_TYPE_CHANGE_DB;

	if(!bConnectionIndex)
	{
		pMsg->bConnectionIndex = m_bDefault;					//ConnectionIndex
	}
	else
	{
		if(!CheckSafeIndexChk(BYTE(bConnectionIndex - 1)))
			return FALSE;
		
		pMsg->bConnectionIndex = BYTE(bConnectionIndex-1);
	}

	KSCToUnicode(szDbName, pMsg->wszQuery);	

	pMsg->AddInfo.change_db.bReturnResult = bReturnResult;
	pMsg->dwQueryUID = dwQueryUID;

	PostDBMessage(pMsg);

	return TRUE;
}

void __stdcall COleDBThreadCls::GetDBResult()
{
	SwitchOutputQueue();

	POSITION_ pos = NULL;
	DBRECEIVEDATA* pResult = NULL;

	pos = GetOutputMsgQueue()->GetHeadPosition();
	while(pos)
	{	
		pResult = (DBRECEIVEDATA*)GetOutputMsgQueue()->GetNext(pos);

		if(pResult)
		{
			m_InitDesc.ReceiveFunc(pResult);			//결과물을 외부함수로 반환 
			
			switch(pResult->bQueryType)
			{
			case QUERY_TYPE_SELECT:		
				THReleaseRecordset(	pResult );		
				break;	//메모리 풀에서 결과값 Release
			}

			GetOutputMsgPool()->Free(pResult);
		}			
	}
	
	GetOutputMsgQueue()->RemoveAll();

	SwitchOutputQueue();

	if (GetOutputMsgQueue()->IsEmpty(FALSE) == FALSE)
	{
		SwitchOutputQueue();
		SetEvent(GetDBResultEvent());
	}
}

void __stdcall COleDBThreadCls::FreeResultMessage(DBRECEIVEDATA* pMsg)
{
	if(pMsg) GetOutputMsgPool()->Free(pMsg);
}

BOOL __stdcall COleDBThreadCls::THReleaseRecordset(DBRECEIVEDATA* pResultData)
{
	if(!pResultData)	return FALSE;

	if(pResultData->Query.select.pBindings)
		GetBindingPool()->Free(pResultData->Query.select.pBindings);
	
	if(pResultData->Query.select.pResult)
		GetResultPool()->Free(pResultData->Query.select.pResult);

	return TRUE;
}

void __stdcall COleDBThreadCls::SetPerformanceReport(BOOL bEnable)
{
	m_bEnableReport = bEnable;
}

BOOL COleDBThreadCls::PostDBMessage(LPDBCMDMSG pMsg)
{
	if(!pMsg)	
	{
		OutputMessage("[OLEDB Fatal Error] LPDBCMDMSG is NULL!(COleDBThreadCls::PostDBMessage)");
		return FALSE;
	}

	if(m_bCloseQueryForThread)		//Input 막음 
		return FALSE;

	DWORD dwQCount = GetInputMsgQueue()->GetCount(TRUE);
	if(m_InitDesc.wMaxNumOfProcessMessage_Input <= dwQCount)
	{
		OutputMessage("[OLEDB Fatal Error] DBInput Message Queue Overflow!(COleDBThreadCls::PostDBMessage)");
		return FALSE;
	}

	GetInputMsgQueue()->AddTail(pMsg);

	InformDBInput();

	return TRUE;
}

void COleDBThreadCls::SwitchInputQueue()
{
	GetInputMsgQueue()->SwitchQueues();
}

void COleDBThreadCls::SwitchOutputQueue()
{
	GetOutputMsgQueue()->SwitchQueues();
}

void COleDBThreadCls::OutputMessage(char* szMsg)
{
	m_InitDesc.OutputMessageFunc(szMsg);
}

void COleDBThreadCls::OutputReport(char* szMsg)
{
	m_InitDesc.ReportFunc(szMsg);
}

BOOL COleDBThreadCls::CheckSafeIndexChk(BYTE bConnectionIndex)
{
	if(!CheckSafeIndex(bConnectionIndex))	
	{
		OutputMessage("[OLEDB Fatal Error] Invalid Connection Index!(COleDBThreadCls::THChangeDB)");
		return FALSE;
	}
	return TRUE;
}

WORD COleDBThreadCls::GetResultIndex(BYTE bConnectionIndex)
{
	if(!bConnectionIndex)
		return m_bDefault;	
	
	if(!CheckSafeIndex(BYTE(bConnectionIndex - 1) ))	
		return 0xffff;

	return WORD(bConnectionIndex - 1);
}

void COleDBThreadCls::InformDBInput()
{
	QueueUserAPC(InformInputQueryThreadAPC, m_hDBThread, 0);
}

void COleDBThreadCls::InformDBOutput(DBRECEIVEDATA* pResult)
{
	if(m_InitDesc.bUsingEventObject)
	{		
		GetOutputMsgQueue()->AddTail( pResult );			//OutputQ에 Push							

		SetEvent( GetDBResultEvent() ); // 결과가 만들어졌음을 통보!!
	}
	else
	{
		PostMessage( m_InitDesc.hWndToPostMessage, m_InitDesc.uMessage, (WPARAM)pResult, 0 );
	}
}



void COleDBThreadCls::ProcessSelectQuery(LPDBCMDMSG pMsg)
{
	DBRECEIVEDATA* pResult = GetConnection(pMsg->bConnectionIndex)->OpenRecordExForThread(pMsg->wszQuery,
								pMsg->AddInfo.select.dwMaxNumRows, pMsg->AddInfo.select.dwRowPerRead );								
	
	if(!pResult)		//Error
	{
		pResult = (DBRECEIVEDATA*)GetOutputMsgPool()->Alloc();
		if(pResult == NULL)
		{
			OutputMessage("OLEDBThread Error! -  Output MessagePool is Full!(OLEDBThread() case: QUERY_TYPE_SELECT ))");
			_asm int 3
		}
		
		pResult->nError = -1;
		
		//Log Error Message
		WCHAR wszStr[ 2048 ] = {0,};
		char  szStr[ 2048 ] = {0,};
		swprintf(wszStr, L"[OLEDB Query Error] '%s'( OLEDBThread(VOID) )", pMsg->wszQuery);
		UnicodeToKSC(wszStr, szStr, 2048);
		OutputMessage(szStr);
	}

	pResult->dwQueryUID = pMsg->dwQueryUID;
	pResult->pData		= pMsg->pData;
	pResult->bQueryType = pMsg->bQueryType;		
	
	InformDBOutput(pResult);
}

void COleDBThreadCls::ProcessExecuteQuery(LPDBCMDMSG pMsg)
{
	int nRet = GetConnection(pMsg->bConnectionIndex)->ExecuteSQL(pMsg->wszQuery);
	if(nRet < -1000 || pMsg->AddInfo.execute.bReturnResult)	//Error 발생시 또는 몇행이 영향을 받았는지 받기를 원할때 
	{
		DBRECEIVEDATA* pResult = (DBRECEIVEDATA*)GetOutputMsgPool()->Alloc();	
		if(!pResult)
		{									
			OutputMessage("OLEDBThread Error! -  Output MessagePool is Full!(OLEDBThread() case: QUERY_TYPE_EXECUTE ))");
			_asm int 3
		}

		if(nRet < -1000)
		{
			pResult->nError = nRet;
			
			//Log Error Message
			WCHAR wszStr[ 2048 ] = {0,};
			char  szStr[ 2048 ] = {0,};
			swprintf(wszStr, L"[OLEDB Query Error] '%s'(code:%d)( OLEDBThread(VOID) )", pMsg->wszQuery, nRet);
			UnicodeToKSC(wszStr, szStr, 2048);
			OutputMessage(szStr);
		}
		else
		{
			pResult->Query.execute.dwEffected = nRet;
		}

		pResult->dwQueryUID = pMsg->dwQueryUID;
		pResult->pData		= pMsg->pData;
		pResult->bQueryType = pMsg->bQueryType;	

		InformDBOutput(pResult);
	}
}

void COleDBThreadCls::ProcessExecuteWithParamQuery(LPDBCMDMSG pMsg)
{
	DBMSG_EXECUTESQL_BY_PARAM* pAdd = &pMsg->AddInfo.execute_by_param;
	int nRet = GetConnection(pMsg->bConnectionIndex)->ExecuteSQLByParam(pMsg->wszQuery, pAdd->pBinding,
																 pAdd->szParamValueBuf, pAdd->bParamNum);

	if(nRet < -1000 || pAdd->bReturnResult)	//Error 발생시 또는 몇행이 영향을 받았는지 받기를 원할때 
	{
		DBRECEIVEDATA* pResult = (DBRECEIVEDATA*)GetOutputMsgPool()->Alloc();
		if(!pResult)
		{									
			OutputMessage("OLEDB Fatal Error! - Output MessagePool is Full!(OLEDBThread() case: QUERY_TYPE_EXECUTE_BY_PARAM )");
			_asm int 3
		}

		if(nRet <-1000)
		{
			pResult->nError = nRet;
			
			//Log Error Message
			WCHAR wszStr[ 2048 ] = {0,};
			char  szStr[ 2048 ] = {0,};
			swprintf(wszStr, L"[OLEDB Query Error] QUERY_TYPE_EXECUTE_BY_PARAM (QueryUID:%d, Query:%s)"
										L"( OLEDBThread(VOID) )", pMsg->dwQueryUID, pMsg->wszQuery);
			UnicodeToKSC(wszStr, szStr, 2048);
			OutputMessage(szStr);
		}
		else
		{
			pResult->Query.execute_by_param.dwEffected = nRet;
		}

		pResult->dwQueryUID = pMsg->dwQueryUID;
		pResult->pData		= pMsg->pData;
		pResult->bQueryType = pMsg->bQueryType;	

		InformDBOutput(pResult);
	}

	THReleaseParamInfo( pAdd->pBinding );
}

void COleDBThreadCls::ProcessChangeDBQuery(LPDBCMDMSG pMsg)
{
	BOOL bRet = GetConnection( pMsg->bConnectionIndex )->ChangeDB( pMsg->wszQuery );
	if(bRet == FALSE || pMsg->AddInfo.change_db.bReturnResult)
	{
		DBRECEIVEDATA* pResult = (DBRECEIVEDATA*)GetOutputMsgPool()->Alloc();
		if(!pResult)
		{									
			OutputMessage("OLEDB Fatal Error! - Output MessagePool is Full!(OLEDBThread() case: QUERY_TYPE_CHANGE_DB )");
			_asm int 3
		}
		
		if(bRet == FALSE)
		{
			pResult->nError = -1;
			
			//Log Error Message
			WCHAR wszStr[ 2048 ] = {0,};
			char  szStr[ 2048 ] = {0,};
			swprintf(wszStr, L"[OLEDB Query Error] '%s'( OLEDBThread(VOID) )", pMsg->wszQuery);
			UnicodeToKSC(wszStr, szStr, 2048);
			OutputMessage(szStr);
		}
		else
		{
			pResult->nError = 0;
		}

		pResult->dwQueryUID = pMsg->dwQueryUID;
		pResult->bQueryType = pMsg->bQueryType;	

		InformDBOutput(pResult);
	}
}

//-----------------------------------------------------------------------------------
DWORD COleDBThreadCls::OleDBThreadFunc()
{
	HANDLE hEvent = GetDBKillEvent();

	DWORD		dwRet = 0;
	POSITION_	pos = NULL;
	LPDBCMDMSG	pMsg = NULL;
	DWORD		dwInputQCount = 0;
	DWORD		dwCurCount = 0;
	DWORD		dwTime = 0;

	for(;;)
	{	
		dwRet = WaitForSingleObjectEx(hEvent, INFINITE, TRUE);
		if(dwRet == WAIT_OBJECT_0) return 0;	// 종료하라~!!!
		else if(dwRet == WAIT_IO_COMPLETION)	// 들어온 쿼리가 있다~ 처리하자.
		{
			// adding 큐와 process 큐를 교체한다.
			SwitchInputQueue();
			
			dwCurCount = 0;
			dwInputQCount = GetInputMsgQueue()->GetCount(FALSE);				

			pos = GetInputMsgQueue()->GetHeadPosition();
			while(pos)
			{				
				pMsg = (LPDBCMDMSG)GetInputMsgQueue()->GetNext(pos);
				if(!pMsg)
				{
					OutputMessage("### OLEDB Warning! : InputMsg Pointer in list is NULL");
					continue;
				}

				dwCurCount++;

				if(m_bEnableReport)
				{
					dwTime = GetTickCount();

					switch(pMsg->bQueryType)
					{
					case QUERY_TYPE_SELECT:				ProcessSelectQuery(pMsg);			break;
					case QUERY_TYPE_EXECUTE:			ProcessExecuteQuery(pMsg);			break;
					case QUERY_TYPE_EXECUTE_BY_PARAM:	ProcessExecuteWithParamQuery(pMsg); break;
					case QUERY_TYPE_CHANGE_DB:			ProcessChangeDBQuery(pMsg);			break;
					}

					dwTime = GetTickCount() - dwTime;

					char szBuf[1024] = {0,};
					sprintf(szBuf, "[DB Report - Connection:%u, QueryID:%u] "
							       "Inner Input Queue: %u/%u, Outer Input Queue: %u, "
								   "Execute Time: %u\n", pMsg->bConnectionIndex, pMsg->dwQueryUID, dwCurCount, 
								   dwInputQCount, GetInputMsgQueue()->GetCount(TRUE), dwTime);
					
					OutputReport(szBuf);
				}
				else
				{
					switch(pMsg->bQueryType)
					{
					case QUERY_TYPE_SELECT:				ProcessSelectQuery(pMsg);			break;
					case QUERY_TYPE_EXECUTE:			ProcessExecuteQuery(pMsg);			break;
					case QUERY_TYPE_EXECUTE_BY_PARAM:	ProcessExecuteWithParamQuery(pMsg); break;
					case QUERY_TYPE_CHANGE_DB:			ProcessChangeDBQuery(pMsg);			break;
					}
				}

				GetInputMsgPool()->Free(pMsg);
			}				
		}

		GetInputMsgQueue()->RemoveAll();
		
		// 만약 현재 adding 용 큐가 비어있지 않다면, 
		// 다시 처리하도록 알려준다. (자기 자신에 apc한다.... 이상하지만서도..)
		if(GetInputMsgQueue()->GetCount(TRUE))
		{
			InformDBInput();
		}
	}

	return 0;
}


/*
//-----------------------------------------------------------------------------------
DWORD COleDBThreadCls::OleDBThreadFunc()
{
	HANDLE hEvent = GetDBKillEvent();
	DWORD dwRet = 0;

	POSITION_	pos = NULL;
	LPDBCMDMSG	pMsg = NULL;
	DWORD		dwInputQCount = 0;
	DWORD		dwCurCount = 0;
	DWORD		dwTime = 0;

	for(;;)
	{	
		dwRet = WaitForSingleObjectEx(hEvent, INFINITE, TRUE);
		if(dwRet == WAIT_OBJECT_0) return 0;	// 종료하라~!!!
		else if(dwRet == WAIT_IO_COMPLETION)	// 들어온 쿼리가 있다~ 처리하자.
		{
			SwitchInputQueue();
			
			dwCurCount = 0;
			dwInputQCount = GetInputMsgQueue()->GetCount(FALSE);				

			pos = GetInputMsgQueue()->GetHeadPosition();
			while(pos)
			{				
				pMsg = (LPDBCMDMSG)GetInputMsgQueue()->GetNext(pos);
				if(!pMsg)
				{
					OutputMessage("OLEDB Warning! : InputMsg Pointer in list is NULL");
					continue;
				}

				dwCurCount++;
			
				switch(pMsg->bQueryType)
				{
				case QUERY_TYPE_SELECT:	
					{
						dwTime = GetTickCount();
						DBRECEIVEDATA* pResult = GetConnection(pMsg->bConnectionIndex)->OpenRecordExForThread(pMsg->wszQuery ,
													pMsg->AddInfo.select.dwMaxNumRows , pMsg->AddInfo.select.dwRowPerRead );								
						dwTime = GetTickCount() - dwTime;
						
						if(!pResult)		//Error
						{
							pResult = (DBRECEIVEDATA*)GetOutputMsgPool()->Alloc();
							if(pResult == NULL)
							{
								OutputMessage("OLEDBThread Error! -  Output MessagePool is Full!(OLEDBThread() case: QUERY_TYPE_SELECT ))");
								_asm int 3
							}
							
							pResult->nError = -1;
							
							//Log Error Message
							WCHAR wszStr[ 2048 ] = {0,};
							char  szStr[ 2048 ] = {0,};
							swprintf(wszStr, L"[OLEDB Query Error] '%s'( OLEDBThread(VOID) )", pMsg->wszQuery);
							UnicodeToKSC(wszStr, szStr, 2048);
							OutputMessage(szStr);
						}

						pResult->dwQueryUID = pMsg->dwQueryUID;
						pResult->pData = pMsg->pData;
						pResult->bQueryType = pMsg->bQueryType;		
						
						InformDBOutput(pResult);
					}
					break;
				case QUERY_TYPE_EXECUTE:
					{
						dwTime = GetTickCount();
						int nRet = GetConnection(pMsg->bConnectionIndex)->ExecuteSQL(pMsg->wszQuery);
						dwTime = GetTickCount() - dwTime;

						if(nRet < -1000 || pMsg->AddInfo.execute.bReturnResult)	//Error 발생시 또는 몇행이 영향을 받았는지 받기를 원할때 
						{
							DBRECEIVEDATA* pResult = (DBRECEIVEDATA*)GetOutputMsgPool()->Alloc();	
							if(!pResult)
							{									
								OutputMessage("OLEDBThread Error! -  Output MessagePool is Full!(OLEDBThread() case: QUERY_TYPE_EXECUTE ))");
								_asm int 3
							}

							if(nRet < -1000)
							{
								pResult->nError = nRet;
								
								//Log Error Message
								WCHAR wszStr[ 2048 ] = {0,};
								char  szStr[ 2048 ] = {0,};
								swprintf(wszStr, L"[OLEDB Query Error] '%s'(code:%d)( OLEDBThread(VOID) )", pMsg->wszQuery, nRet);
								UnicodeToKSC(wszStr, szStr, 2048);
								OutputMessage(szStr);
							}
							else
							{
								pResult->Query.execute.dwEffected = nRet;
							}

							pResult->dwQueryUID = pMsg->dwQueryUID;
							pResult->pData = pMsg->pData;
							pResult->bQueryType = pMsg->bQueryType;	

							InformDBOutput(pResult);
						}
					}
					break;
				case QUERY_TYPE_EXECUTE_BY_PARAM:
					{
						DBMSG_EXECUTESQL_BY_PARAM* pAdd = &pMsg->AddInfo.execute_by_param;
						dwTime = GetTickCount();
						int nRet = GetConnection(pMsg->bConnectionIndex)->ExecuteSQLByParam(pMsg->wszQuery, pAdd->pBinding,
																					 pAdd->szParamValueBuf, pAdd->bParamNum);
						dwTime = GetTickCount() - dwTime;

						if(nRet < -1000 || pAdd->bReturnResult)	//Error 발생시 또는 몇행이 영향을 받았는지 받기를 원할때 
						{
							DBRECEIVEDATA* pResult = (DBRECEIVEDATA*)GetOutputMsgPool()->Alloc();
							if(!pResult)
							{									
								OutputMessage("OLEDB Fatal Error! - Output MessagePool is Full!(OLEDBThread() case: QUERY_TYPE_EXECUTE_BY_PARAM )");
								_asm int 3
							}

							if(nRet <-1000)
							{
								pResult->nError = nRet;
								
								//Log Error Message
								WCHAR wszStr[ 2048 ] = {0,};
								char  szStr[ 2048 ] = {0,};
								swprintf(wszStr, L"[OLEDB Query Error] QUERY_TYPE_EXECUTE_BY_PARAM (QueryUID:%d, Query:%s)"
															L"( OLEDBThread(VOID) )", pMsg->dwQueryUID, pMsg->wszQuery);
								UnicodeToKSC(wszStr, szStr, 2048);
								OutputMessage(szStr);
							}
							else
							{
								pResult->Query.execute_by_param.dwEffected = nRet;
							}

							pResult->dwQueryUID = pMsg->dwQueryUID;
							pResult->pData = pMsg->pData;
							pResult->bQueryType = pMsg->bQueryType;	

							InformDBOutput(pResult);
						}

						THReleaseParamInfo( pAdd->pBinding );
					}
					break;

				case QUERY_TYPE_CHANGE_DB:
					{
						dwTime = GetTickCount();
						BOOL bRet = GetConnection( pMsg->bConnectionIndex )->ChangeDB( pMsg->wszQuery );
						dwTime = GetTickCount() - dwTime;

						if(bRet == FALSE || pMsg->AddInfo.change_db.bReturnResult)
						{
							DBRECEIVEDATA* pResult = (DBRECEIVEDATA*)GetOutputMsgPool()->Alloc();
							if(!pResult)
							{									
								OutputMessage("OLEDB Fatal Error! - Output MessagePool is Full!(OLEDBThread() case: QUERY_TYPE_CHANGE_DB )");
								_asm int 3
							}
							
							if(bRet == FALSE)
							{
								pResult->nError = -1;
								
								//Log Error Message
								WCHAR wszStr[ 2048 ] = {0,};
								char  szStr[ 2048 ] = {0,};
								swprintf(wszStr, L"[OLEDB Query Error] '%s'( OLEDBThread(VOID) )", pMsg->wszQuery);
								UnicodeToKSC(wszStr, szStr, 2048);
								OutputMessage(szStr);
							}
							else
							{
								pResult->nError = 0;
							}

							pResult->dwQueryUID = pMsg->dwQueryUID;
							pResult->bQueryType = pMsg->bQueryType;	

							InformDBOutput(pResult);
						}
					}
					break;
				}
				

				if(m_bEnableReport)
				{
					char szBuf[1024] = {0,};
					sprintf(szBuf, "[DB Report - Connection:%d, QueryID:%d] "
							       "Inner Input Queue: %d/%d, Outer Input Queue: %d, "
								   "Execute Time:%d\n", pMsg->bConnectionIndex, pMsg->dwQueryUID, dwCurCount, 
								   dwInputQCount, GetInputMsgQueue()->GetCount(FALSE), dwTime);
					
					OutputReport(szBuf);
				}
				

				GetInputMsgPool()->Free(pMsg);
			}				
		}

		GetInputMsgQueue()->RemoveAll();	//Modified 030304
		
		// 만약 현재 adding 용 큐가 비어있지 않다면, 
		// 다시 처리하도록 알려준다. (자기 자신에 apc한다.... 이상하지만서도..)
		if(GetInputMsgQueue()->IsEmpty(TRUE) == FALSE)
		{
			InformDBInput();
		}
	}

	return 0;
}
*/

