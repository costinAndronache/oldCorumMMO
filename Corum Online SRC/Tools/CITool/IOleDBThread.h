#pragma once
#include <initguid.h>
#include <oledb.h>


// {896940DB-7E35-4e4f-8D90-AF5E78A10B3B}
DEFINE_GUID(CLSID_OLEDBTHREAD, 0x896940db, 0x7e35, 0x4e4f, 0x8d, 0x90, 0xaf, 0x5e, 0x78, 0xa1, 0xb, 0x3b);

// {E8BDB6F8-679D-40bf-83A3-B647E797FB1B}
DEFINE_GUID(IID_OLEDBTHREAD, 0xe8bdb6f8, 0x679d, 0x40bf, 0x83, 0xa3, 0xb6, 0x47, 0xe7, 0x97, 0xfb, 0x1b);


#define MAX_DB_CONNECTION			30
#define DEFAULT_RETURNED_MAX_ROWS	20
#define DEFAULT_ROWS_PER_READ		10
#define MAX_SCHEMA_BUFFER_SIZE		128
#define DEFAULT_STRING_LENGTH		128
#define MAX_SQL_STRING_LENGTH		1023
#define QUERY_TYPE_SELECT			1	// Select
#define QUERY_TYPE_EXECUTE			2	// Update, Delete, Sp....
#define QUERY_TYPE_EXECUTE_BY_PARAM 3
#define QUERY_TYPE_CHANGE_DB		4
#define MAX_DB_COUNT				200	// DBSchema ����


#pragma pack(push,1)


struct RECEIVEDATA_SELECT
{
	DWORD			dwRowCount;	// ����� �����ȯ�Ǵ��� 
	DWORD			dwRowSize;	// �� ���� Data Size
	DBBINDING*		pBindings;	// Į�� ����
	BYTE			bColCount;	// Į�� ���� 
	char*			pResult;	// ���� �޾ƿ��� ��� ������ 
};


struct RECEIVEDATA_EXECUTE
{
	DWORD			dwEffected;
};


struct DBRECEIVEDATA
{
	BYTE			bQueryType;	// ���� Ÿ�� 
	DWORD			dwQueryUID;	// ������ȣ 
	int				nError;		// �����̸� Error
	
	union
	{
		RECEIVEDATA_SELECT	select;
		RECEIVEDATA_EXECUTE execute;
		RECEIVEDATA_EXECUTE execute_by_param;
	} Query;
	
	void*			pData;		// �ΰ����� 
};


struct PARAMVALUE
{
	int				Age;
	double			Eye;
	BYTE			Myimage[16];
};


struct BLOBDATA
{
    DBSTATUS            dwStatus;   
    DWORD               dwLength; 
    ISequentialStream*  pISeqStream;
};


struct DBSCHEMA
{
	char				szSchemaBuffer[MAX_SCHEMA_BUFFER_SIZE];
	
	DBSCHEMA()
	{
		memset(szSchemaBuffer, 0, sizeof(szSchemaBuffer));
	}
};


struct DB_INITIALIZE_DESC
{
	BYTE		bUsingThread;
	BYTE		bUsingEventObject;
	BYTE		bMaxParamNum;						// ExecuteSqlByParam���� �����Ҽ� �ִ� �ִ� �Ķ���� ����
	WORD		wMaxNumOfProcessMessage_Input;		// DLL���� �ѹ��� �����Ҽ� �ִ� �޼��� ����..  (�޼����� �޸�Ǯ ����)
	WORD		wMaxNumOfProcessMessage_Output;		// DLL���� �ѹ��� �����Ҽ� �ִ� �޼��� ����..  (�޼����� �޸�Ǯ ����)
	WORD		wMaxRowPerRead;						// DB���� �о�帱�� �ѹ��� ���پ� �о�帱���ΰ�.. ( �������� ������ ������ ���� )
	WORD		wMaxReturnedRowNum;					// Select���� ���ü� �ִ� ���� �ִ� ����
	WORD		wMaxReturnedColNum;					// Select���� ���ü� �ִ� Į���� �ִ� ����
	DWORD		dwMaxRowSize;						// ����� �� ���� �ִ� ������ 
	UINT		uMessage;
	HWND		hWndToPostMessage;
	void		(*OutputMessageFunc)(char*);		// �޼��� ����� �Լ� ������ 
	void		(*ReceiveFunc)(DBRECEIVEDATA*);		// ObjectFunction �Լ� ������
};


#pragma pack(pop)

interface IOleDBThread : IUnknown
{			
	virtual BOOL	__stdcall InitDBModule(DB_INITIALIZE_DESC* desc) = 0;
	
	virtual	int		__stdcall CreateDBConnection() = 0;
	virtual	int		__stdcall Connect(char* szDataSource, char* szDefaultDb, char* szUserId, char* szPassword, int nConnectTimeout = 20, int nQuerytimeout = 10, BOOL bCommandTimeOut = FALSE, BYTE bConnectionIndex = 0) = 0;
	virtual	int		__stdcall Connect(char* szDataSource, char* szDefaultDb, char* szUserId, char* szPassword, int nConnectTimeout = 20, BYTE bConnectionIndex = 0) = 0;
	virtual	BOOL	__stdcall ReleaseDBConnectionForSingleThread(BYTE bConnectionIndex) = 0;	
	
	
	virtual	int			__stdcall ExecuteSQL(char* szQuerySQL, BYTE bConnectionIndex = 0) = 0;
	virtual	int			__stdcall ExecuteSQLByParam(char* szQuerySQL, DBBINDING* pBinding, void* pParamValue, BYTE bParamNum, BYTE bConnectionIndex = 0) = 0;
	virtual	DBBINDING*  __stdcall CreateParamInfo(WORD wParamNum, BYTE bConnectionIndex = 0) = 0;
	virtual	BOOL		__stdcall ReleaseParamInfo(DBBINDING* pBinding, BYTE bConnectionIndex = 0) = 0;
	
	
	virtual	int				__stdcall OpenRecord(char* szQuerySQL, void* pRecordSet, DWORD dwMaxNumRows = DEFAULT_RETURNED_MAX_ROWS, BYTE bConnectionIndex = 0) = 0;
	virtual	DBRECEIVEDATA*  __stdcall OpenRecordEx(char* szQuerySQL, DWORD dwMaxNumRows = DEFAULT_RETURNED_MAX_ROWS, DWORD dwRowPerRead = DEFAULT_ROWS_PER_READ, BYTE bConnectionIndex = 0) = 0;
	virtual	BOOL			__stdcall GetData(void* pReceiveData, DBRECEIVEDATA* pResultData, DWORD dwRowNum, WORD wColumnNum, BYTE bConnectionIndex = 0) = 0;	//dwRowNum�� wColumnNum �� zero based Index�̴�..
	virtual	BOOL			__stdcall ReleaseRecordset(DBRECEIVEDATA* pResultData, BYTE bConnectionIndex = 0) = 0;
	
	
	virtual	int		__stdcall GetBLOBData(char* szTableName, char* szColumnName, char* szFilter, void* pReadBuffer, DWORD dwReadSize, BYTE bConnectionIndex = 0) = 0;
	virtual	int		__stdcall SetBLOBData(char* szTableName, char* szColumnName, char* szFilter, void* pWriteData,  DWORD dwWriteSize, BYTE bConnectionIndex = 0) = 0;
	//by File
	virtual int __stdcall GetBLOBData(char* szTableName, char* szColumnName, char* szFilter, 
								char* szFilePath, DWORD dwReadSize, BYTE bConnectionIndex = 0) = 0;
	virtual int __stdcall SetBLOBData(char* szTableName, char* szColumnName, char* szFilter, 
								char* szFilePath, BYTE bConnectionIndex = 0) = 0;
	
	
	virtual	BOOL	__stdcall ChangeDB(char* szDbName, BYTE bConnectionIndex) = 0;
	virtual	BOOL	__stdcall SetDefaultDBIndex(BYTE bConnectionIndex) = 0;
	virtual	BYTE	__stdcall GetDefaultDBIndex() = 0;
	
	//Multibyte �ѱ� -> �����ڵ� �ѱ� ������ ��ȯ
	virtual BOOL	__stdcall KSCToUnicode(char *pKsc, WCHAR *pUni) = 0;
	//�����ڵ��� �ѱ� -> Multibyte�ѱ۷� ��ȯ 
	virtual BOOL	__stdcall UnicodeToKSC(WCHAR *pUni, char *pKsc, int nKscSize) = 0;
	
	//Added at 020109
	//SQLServer�� ��� Database ����ȯ 
	virtual int		__stdcall QueryDBCatalog(DBSCHEMA* pSchemaBuf, DWORD dwMaxNumRows, BYTE bConnectionIndex = 0) = 0;
	//Added at 020110
	//���� Database�� ��� UserTable �� ��ȯ 
	virtual int		__stdcall QueryDBTable(DBSCHEMA* pSchemaBuf, DWORD dwMaxNumRows, BYTE bConnectionIndex = 0) = 0;
	
	virtual DBCOLUMNINFO*	__stdcall GetColumnInfo(char* szQuerySQL, DWORD* pColnum/*out*/, BYTE bConnectionIndex = 0) = 0;
	
	
	//Methods For Thread 
	virtual void	__stdcall THOpenRecord(char* szQuerySQL,  DWORD dwQueryUID,  void* pData = NULL, DWORD dwMaxNumRows = DEFAULT_RETURNED_MAX_ROWS, BYTE bConnectionIndex = 0) = 0;
	virtual void    __stdcall THExecuteSQL(char* szQuerySQL,  BYTE bReturnResult = FALSE, DWORD dwQueryUID = 0,  void* pData = NULL, BYTE bConnectionIndex = 0) = 0;
	virtual void	__stdcall THExecuteSQLByParam(char* szQuerySQL, DBBINDING* pBinding, void* pParamValue, DWORD dwParamValueSize, BYTE bParamNum,
		BYTE bReturnResult = FALSE, DWORD dwQueryUID = 0,  void* pData= NULL, BYTE bConnectionIndex = 0) = 0;
	virtual DBBINDING* __stdcall THCreateParamInfo(WORD wParamNum) = 0;
	virtual BOOL	__stdcall THChangeDB(char* szDbName, BYTE bReturnResult = FALSE, DWORD dwQueryUID = 0, BYTE bConnectionIndex = 0) = 0;
	virtual BOOL	__stdcall THReleaseRecordset(DBRECEIVEDATA* pResultData) = 0;
	virtual void    __stdcall FreeResultMessage(DBRECEIVEDATA* pMsg) = 0;
	virtual void	__stdcall SetDBResultEvent(HANDLE val) = 0;
	virtual void    __stdcall GetDBResult() = 0;
	
	virtual	void	__stdcall THIgnoreQueryInput() = 0;
	virtual void	__stdcall THAcceptQueryInput() = 0;
	
	virtual void	__stdcall SetPerformanceReport(BOOL bEnable) = 0;
	
	virtual DWORD	__stdcall GetOLEDBSize() = 0;
};