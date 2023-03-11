//----------------------------------------------------------------------------------------------------
//							COleDBThread Class by Byung Soo Koo
//----------------------------------------------------------------------------------------------------

#pragma once

#include "dbstruct.h"
#include "Define.h"
#include "TSMemPool.h"

#define DEF_EXCUTE_START	1
#define DEF_EXCUTE_TIMEOUT	2
#define DEF_EXCUTE_END		3

#define DEF_IID_NULL			0
#define DEF_IID_IROWSETCHANGE	1
#define DEF_IID_IROWSET			2

#define DBINITCONSTANTS
#define INITGID

class COleDBThreadCls;

class COleDBThread
{
	IDBInitialize*      m_pIDBInitialize;
	IDBProperties*      m_pIDBProperties;
	IDBCreateSession*   m_pIDBCreateSession;
	IDBCreateCommand*   m_pIDBCreateCommand;
	ICommandText*       m_pICommandText;
	IAccessor*          m_pIAccessor;		

	CThreadSafeStaticMemPool*	m_pHrowPool;
	
	BYTE				m_byTimeOut;
	HANDLE				hExcuteEvent;
	HANDLE				m_hDBExcuteThread;	
	DWORD				m_dwCommandTimeOut;
	BOOL				m_bCommandTimeOut;

protected:

	HANDLE				GetExcuteEvent()			{	m_byTimeOut = DEF_EXCUTE_TIMEOUT; return hExcuteEvent;	}
	void				SetExcuteInit()				{	m_byTimeOut = DEF_EXCUTE_START; }
	HANDLE				GetExcuteEndEvent()			{	m_byTimeOut = DEF_EXCUTE_END; return hExcuteEvent;		}
	BYTE				GetExcuteType()				{	return m_byTimeOut;				}
	DWORD				GetCommandTimeOutTime()		{	return m_dwCommandTimeOut;		}
	HANDLE				GetExcuteThreadHandle()		{	return m_hDBExcuteThread;		}
	ICommandText*       GetCommandText()			{	return m_pICommandText;			}
	
	COleDBThreadCls*	m_pParent;

	void				(*m_pOutputMessageFunc)(char*);		//메세지 출력할 함수 포인터 



public:
	COleDBThread(DB_INITIALIZE_DESC* desc, COleDBThreadCls* pParent);
	~COleDBThread();

	int Initialize(wchar_t* szDataSource, wchar_t* szDefaultDb, wchar_t* szUserId, wchar_t* szPassword, int nConnectTimeout);
	int	Initialize(wchar_t* szDataSource, wchar_t* szDefaultDb, wchar_t* szUserId, wchar_t* szPassword, int nConnectTimeout, int nCommandTimeOut, BOOL bCommandTimeOut);
	
	//Non-Thread Model Functions
	
	//Insert, Update, Delete Function
	int				ExecuteSQL(wchar_t* szQuerySQL);
	int				ExecuteSQLByParam(WCHAR* szQuerySQL, DBBINDING* pBinding, void* pParamValue, BYTE bParamNum);
	DBBINDING*		CreateParamInfo(WORD wParamNum);
	BOOL			ReleaseParamInfo(DBBINDING* pBinding);
		
	//Select Function (반환값이 있는 함수)
	int				OpenRecord(WCHAR* szQuerySQL, void* pRecordSet, DWORD dwMaxNumRows = DEFAULT_RETURNED_MAX_ROWS);
	DBRECEIVEDATA*	OpenRecordEx(wchar_t* szQuerySQL, DWORD dwMaxNumRows = DEFAULT_RETURNED_MAX_ROWS, DWORD dwRowPerRead = DEFAULT_ROWS_PER_READ);
	DBRECEIVEDATA*	OpenRecordExForThread(wchar_t* szQuerySQL, DWORD dwMaxNumRows = DEFAULT_RETURNED_MAX_ROWS, DWORD dwRowPerRead = DEFAULT_ROWS_PER_READ);
	BOOL			GetData(void* pReceiveData, DBRECEIVEDATA* pResultData, DWORD dwRowNum, WORD wColumnNum);	//dwRowNum와 wColumnNum 은 zero based Index이다..
	BOOL			ReleaseRecordset(DBRECEIVEDATA* pResultData);

	//BLOBData 함수 
	int				GetBLOBData(WCHAR* szTableName, WCHAR* szColumnName, WCHAR* szFilter, void* pReadBuffer, DWORD dwReadSize);
	int				SetBLOBData(WCHAR* szTableName, WCHAR* szColumnName, WCHAR* szFilter, void* pWriteData,  DWORD dwWriteSize);
	
	int				GetBLOBData(WCHAR* szTableName, WCHAR* szColumnName, WCHAR* szFilter, WCHAR* szFilePath, DWORD dwReadSize);
	int				SetBLOBData(WCHAR* szTableName, WCHAR* szColumnName, WCHAR* szFilter, WCHAR* szFilePath);	

	//Etc
	BOOL			ChangeDB(WCHAR* szDbName);
		//Added 020109
	int				QueryDBCatalog(DBSCHEMA* pSchemaBuf, DWORD dwMaxNumRows);
		//Added 020110	
	int				QueryDBTable(DBSCHEMA* pSchemaBuf, DWORD dwMaxNumRows);
		
	DBCOLUMNINFO*	GetColumnInfo(WCHAR* szQuerySQL, DWORD* pColnum);

	void			ErrorDisplay(HRESULT hrErr);
	void			ErrorDisplayEx(HRESULT hrErr, wchar_t* szQuery);

	LONG			SetCommandExecute(wchar_t* szQuerySQL, BYTE byType, IRowsetChange** pIRowsetChange, IRowset** pIRowset, DBPARAMS* dbParams = NULL, BOOL bParam = FALSE, LONG* cNumRows = NULL);

	//Thread function.
	DWORD			OleDBExecuteThreadFunc();

};

