#ifndef __OLEDBTHREADCLS_H__
#define __OLEDBTHREADCLS_H__

#pragma once


#include "IOleDBThread.h"
#include "OleDBThread.h"
#include "DBMsgQueue.h"



class COleDBThreadCls : public IOleDBThread
{
public:
	// IUnknown 메서드
	HRESULT __stdcall			QueryInterface(REFIID riid, LPVOID* ppv);
	ULONG	__stdcall			AddRef(void);
	ULONG	__stdcall			Release(void);

public:
	//IOleDBThread 메서드 
	BOOL	__stdcall			InitDBModule(DB_INITIALIZE_DESC* desc);
	int		__stdcall			CreateDBConnection();
	int		__stdcall			Connect(const char* szDataSource, const char* szDefaultDb, const char* szUserId, const char* szPassword, int nConnectTimeout = 20,									
										int nQuerytimeout = 10, BOOL bCommandTimeOut = FALSE, BYTE bConnectionIndex = 0);
	int		__stdcall			Connect(const char* szDataSource, const char* szDefaultDb, const char* szUserId, const char* szPassword, int nConnectTimeout = 20,									
										BYTE bConnectionIndex = 0);
	BOOL	__stdcall			ReleaseDBConnectionForSingleThread(BYTE bConnectionIndex);	
	int		__stdcall			ExecuteSQL(char* szQuerySQL, BYTE bConnectionIndex = 0);
	int		__stdcall			ExecuteSQLByParam(char* szQuerySQL, DBBINDING* pBinding, void* pParamValue,
													BYTE bParamNum, BYTE bConnectionIndex = 0);
	DBBINDING* __stdcall		CreateParamInfo(WORD wParamNum, BYTE bConnectionIndex = 0);
	BOOL	__stdcall			ReleaseParamInfo(DBBINDING* pBinding, BYTE bConnectionIndex = 0);
	int		__stdcall			OpenRecord(char* szQuerySQL, void* pRecordSet, 
											DWORD dwMaxNumRows = DEFAULT_RETURNED_MAX_ROWS, BYTE bConnectionIndex = 0);
	DBRECEIVEDATA*  __stdcall	OpenRecordEx(char* szQuerySQL, DWORD dwMaxNumRows = DEFAULT_RETURNED_MAX_ROWS, 										   
												DWORD dwRowPerRead = DEFAULT_ROWS_PER_READ, BYTE bConnectionIndex = 0);									   
	BOOL	 __stdcall			GetData(void* pReceiveData, DBRECEIVEDATA* pResultData, 
										DWORD dwRowNum, WORD wColumnNum, BYTE bConnectionIndex = 0);	//dwRowNum와 wColumnNum 은 zero based Index이다..
	BOOL	__stdcall			ReleaseRecordset(DBRECEIVEDATA* pResultData, BYTE bConnectionIndex = 0);

	
	int		__stdcall			GetBLOBData(char* szTableName, char* szColumnName, char* szFilter, 
											void* pReadBuffer, DWORD dwReadSize, BYTE bConnectionIndex = 0);
	int		__stdcall			SetBLOBData(char* szTableName, char* szColumnName, char* szFilter, 
											void* pWriteData,  DWORD dwWriteSize, BYTE bConnectionIndex = 0);
	int		__stdcall			GetBLOBData(char* szTableName, char* szColumnName, char* szFilter, 
											char* szFilePath, DWORD dwReadSize, BYTE bConnectionIndex = 0);
	int		__stdcall			SetBLOBData(char* szTableName, char* szColumnName, char* szFilter, 
											char* szFilePath, BYTE bConnectionIndex = 0);

	BOOL	__stdcall			ChangeDB(char* szDbName, BYTE bConnectionIndex = 0);
	
	BOOL	__stdcall			SetDefaultDBIndex(BYTE bConnectionIndex);
	BYTE	__stdcall			GetDefaultDBIndex();

	//완성형 한글 -> 유니코드 한글 형으로 변환
	BOOL	__stdcall			KSCToUnicode(const char *pKsc, WCHAR *pUni);
	BOOL	__stdcall			UnicodeToKSC(WCHAR *pUni, char *pKsc, int nKscSize);

	int		__stdcall			QueryDBCatalog(DBSCHEMA* pSchemaBuf, DWORD dwMaxNumRows, BYTE bConnectionIndex = 0);
	int		__stdcall			QueryDBTable(DBSCHEMA* pSchemaBuf, DWORD dwMaxNumRows, BYTE bConnectionIndex = 0);
	
	DBCOLUMNINFO* __stdcall		GetColumnInfo(char* szQuerySQL,   DWORD* pColnum, BYTE bConnectionIndex = 0);
		
	void	__stdcall			THOpenRecord(char* szQuerySQL, DWORD dwQueryUID, void* pData = NULL, DWORD dwMaxNumRows = DEFAULT_RETURNED_MAX_ROWS, BYTE bConnectionIndex = 0);
	void	__stdcall			THExecuteSQL(char* szQuerySQL,  BYTE bReturnResult = FALSE, DWORD dwQueryUID = 0,  void* pData = NULL, BYTE bConnectionIndex = 0);
	void	__stdcall			THExecuteSQLByParam(char* szQuerySQL, DBBINDING* pBinding, void* pParamValue,  DWORD dwParamValueSize, BYTE bParamNum,
														BYTE bReturnResult = FALSE, DWORD dwQueryUID = 0,  void* pData= NULL, BYTE bConnectionIndex = 0);

	DBBINDING* __stdcall		THCreateParamInfo(WORD wParamNum);
	BOOL	__stdcall			THChangeDB(char* szDbName, BYTE bReturnResult = FALSE, DWORD dwQueryUID = 0, BYTE bConnectionIndex = 0);
	BOOL	__stdcall			THReleaseRecordset(DBRECEIVEDATA* pResultData);
	void	__stdcall			FreeResultMessage(DBRECEIVEDATA* pMsg);
	void	__stdcall			SetDBResultEvent(HANDLE val)	{	m_hResultEvent = val;	}
	void	__stdcall			GetDBResult();

	void	__stdcall			THIgnoreQueryInput();
	void	__stdcall			THAcceptQueryInput();

	void	 __stdcall			SetPerformanceReport(BOOL bEnable);

	DWORD	__stdcall			GetOLEDBSize() {	return m_dwOLEDBSize;	}


public:
	COleDBThreadCls();
	~COleDBThreadCls();


	WORD						GetResultIndex(BYTE bConnectionIndex);
	BOOL						CheckSafeIndexChk(BYTE bConnectionIndex);
	BOOL						CheckSafeIndex(BYTE bConnectionIndex);
	BOOL						PostDBMessage(LPDBCMDMSG pMsg);
	void						SwitchInputQueue();
	void						SwitchOutputQueue();
	void						OutputMessage(char* szMsg);
	void						OutputReport(char* szMsg);
	void						THReleaseParamInfo(DBBINDING* pBinding);
	void						ProtectInputMessage(BOOL bProtect = TRUE);	
	COleDBThread*				GetConnection( BYTE bConnectionIndex )	{ return m_pDBObjectList[ bConnectionIndex ];	}
	HANDLE						GetDBKillEvent()						{ return m_hDBKillEvent;	}
	HANDLE						GetDBResultEvent()						{ return m_hResultEvent;	}
	const DB_INITIALIZE_DESC*	GetInitDesc()							{ return &m_InitDesc; }							

	CThreadSafeStaticMemPool*	GetOutputMsgPool()						{ return m_pOutputMsgPool;}
	CThreadSafeStaticMemPool*	GetInputMsgPool()						{ return m_pInputMsgPool;}
	CThreadSafeStaticMemPool*	GetBindingPool()						{ return m_pBindingPool;}
	CThreadSafeStaticMemPool*	GetParamPool()							{ return m_pParamPool;}
	CThreadSafeStaticMemPool*	GetResultPool()							{ return m_pResultPool;}
	
	CMsgQueue*					GetInputMsgQueue() const				{ return m_pInputQueue; }
	CMsgQueue*					GetOutputMsgQueue() const				{ return m_pOutputQueue; }
	void						InformDBInput();
	void						InformDBOutput(DBRECEIVEDATA* pResult);

	DWORD						OleDBThreadFunc(); // 쓰레드가 호출하는 함수~!

private:

	void						ProcessSelectQuery(LPDBCMDMSG pMsg);
	void						ProcessExecuteQuery(LPDBCMDMSG pMsg);
	void						ProcessExecuteWithParamQuery(LPDBCMDMSG pMsg);
	void						ProcessChangeDBQuery(LPDBCMDMSG pMsg);


	DWORD						m_cRef;	
	DB_INITIALIZE_DESC			m_InitDesc;
	BOOL						m_bEnableReport;		//Report Flag
	DWORD						m_dwOLEDBSize;	
	BYTE						m_bDefault;				//디폴트 컨넥션 DB 설정할 변수 
	BOOL						m_bCloseQueryForThread;	//Thread에 쿼리 메세지 입력을 막을때 

	HANDLE						m_hDBThread;			//DBThread 핸들
	HANDLE						m_hDBKillEvent;			// DBThead 종료 이벤트.
	HANDLE						m_hResultEvent;			// DB 작업 결과가 있음을 통보할 이벤트.

	COleDBThread*				m_pDBObjectList[ MAX_DB_CONNECTION ];
	
	CMsgQueue*					m_pInputQueue;
	CMsgQueue*					m_pOutputQueue;

	CThreadSafeStaticMemPool*	m_pInputMsgPool;
	CThreadSafeStaticMemPool*	m_pOutputMsgPool;
	CThreadSafeStaticMemPool*	m_pResultPool;
	CThreadSafeStaticMemPool*	m_pBindingPool;
	CThreadSafeStaticMemPool*	m_pParamPool;

};



#endif // __OLEDBTHREADCLS_H__