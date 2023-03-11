#include "stdafx.h"
#include <CGuid.h>
#include "oledbthread.h"
#include "SeqStream.h"
#include "OleDBThreadCls.h"
#include "ExceptionFilter.h"



#define FAIL_CHECK(ret, x1, x2)		if(FAILED((x1)))	{ (ret) = (x2); __leave; }
#define FAIL_RETURN(x1, ret)		if(FAILED(x1))		{ ErrorDisplay((x1)); return (ret); }


#define START	__asm	rdtsc			\
				__asm	push	eax		\
				__asm	push	edx		\


#define END	__asm	rdtsc			\
			__asm	pop		ebx		\
			__asm	pop		ecx		\
			__asm	sub		edx,ebx	\
			__asm	sub		eax,ecx	\
			__asm	int		3		\


const GUID CLSID_SQLOLEDB = {0xc7ff16cL,0x38e3,0x11d0,{0x97,0xab,0x0,0xc0,0x4f,0xc2,0xad,0x98}};


DWORD WINAPI OLEDBExcuteThread(LPVOID lpVoid)
{
	COleDBThread* pDBThread = (COleDBThread*)lpVoid;
	return pDBThread->OleDBExecuteThreadFunc();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
COleDBThread::COleDBThread(DB_INITIALIZE_DESC* desc, COleDBThreadCls* pParent)
{
	m_pIDBInitialize		= NULL;
	m_pIDBProperties		= NULL;
	m_pIDBCreateSession		= NULL;
	m_pIDBCreateCommand		= NULL;
	m_pICommandText			= NULL;
	m_pIAccessor			= NULL;
	m_pHrowPool				= NULL;
	m_pOutputMessageFunc	= desc->OutputMessageFunc;
	m_pParent				= pParent;

	m_hDBExcuteThread		= NULL;//변수 초기화 : 050103 hwoarang
		 
	if(desc->bUsingThread)
	{		
		m_pHrowPool = new CThreadSafeStaticMemPool(sizeof(HROW) * desc->wMaxRowPerRead, 1, 2);
		if(!m_pHrowPool)
		{
			OutputDebugStringW(L"InitializeStaticMemoryPool failed for HROW pool");
			__asm int 3
		}
	}
}

int COleDBThread::Initialize(wchar_t* szDataSource, wchar_t* szDefaultDb, wchar_t* szUserId, wchar_t* szPassword, int nConnectTimeout)
{
	nConnectTimeout;

    //COM Library 초기화...
	int			nRet = 1;
	HRESULT		hr = NULL;	
	DBPROP		InitProperties[6];
	DBPROPSET	rgInitPropSet[1];

    //SQLOLEDB Provider 
    hr = ::CoCreateInstance(CLSID_SQLOLEDB, NULL, CLSCTX_INPROC_SERVER, IID_IDBInitialize, (void **) &m_pIDBInitialize);
 	if(FAILED(hr))    
        return -1000;

	__try
	{
		for(int i = 0; i < 6; i++) 
			VariantInit(&InitProperties[i].vValue);

		//Server name.
		InitProperties[0].dwPropertyID  = DBPROP_INIT_DATASOURCE;
		InitProperties[0].vValue.vt     = VT_BSTR;
		InitProperties[0].vValue.bstrVal= SysAllocString(szDataSource);
		InitProperties[0].dwOptions     = DBPROPOPTIONS_REQUIRED;
		InitProperties[0].colid         = DB_NULLID;

		//Database.
		InitProperties[1].dwPropertyID  = DBPROP_INIT_CATALOG;
		InitProperties[1].vValue.vt     = VT_BSTR;
		InitProperties[1].vValue.bstrVal= SysAllocString(szDefaultDb);
		InitProperties[1].dwOptions     = DBPROPOPTIONS_REQUIRED;
		InitProperties[1].colid         = DB_NULLID;

		//Username (login).
		InitProperties[2].dwPropertyID  = DBPROP_AUTH_USERID; 
		InitProperties[2].vValue.vt     = VT_BSTR;
		InitProperties[2].vValue.bstrVal= SysAllocString(szUserId);
		InitProperties[2].dwOptions     = DBPROPOPTIONS_REQUIRED;
		InitProperties[2].colid         = DB_NULLID;
		
		//Password.
		InitProperties[3].dwPropertyID  = DBPROP_AUTH_PASSWORD;
		InitProperties[3].vValue.vt     = VT_BSTR;
		InitProperties[3].vValue.bstrVal= SysAllocString(szPassword);
		InitProperties[3].dwOptions     = DBPROPOPTIONS_REQUIRED;
		InitProperties[3].colid         = DB_NULLID;

		rgInitPropSet[0].guidPropertySet	= DBPROPSET_DBINIT;
		rgInitPropSet[0].cProperties		= 4;
		rgInitPropSet[0].rgProperties		= InitProperties;
  								
		hr = m_pIDBInitialize->QueryInterface(IID_IDBProperties, (void **)&m_pIDBProperties);
		FAIL_CHECK(nRet, hr, -1100);

		hr = m_pIDBProperties->SetProperties(1, rgInitPropSet); 
		FAIL_CHECK(nRet, hr, -1200);

		//초기화..
		hr = m_pIDBInitialize->Initialize();
		FAIL_CHECK(nRet, hr, -1300);
		
		//Create a session object.
		hr = m_pIDBInitialize->QueryInterface( IID_IDBCreateSession, (void**) &m_pIDBCreateSession);
		FAIL_CHECK(nRet, hr, -1400);

		//세션을 만들고...  요청한 인터페이스 포인터를 받는다.
		//Creates a new session from the data source object and returns the requested interface on the newly created session.
		hr = m_pIDBCreateSession->CreateSession(NULL, IID_IDBCreateCommand, (IUnknown**) &m_pIDBCreateCommand);
		FAIL_CHECK(nRet, hr, -1500);

		//Access the ICommandText interface.
		//Creates a new command.
		hr = m_pIDBCreateCommand->CreateCommand(NULL, IID_ICommandText, (IUnknown**) &m_pICommandText);
		FAIL_CHECK(nRet, hr, -1600);
	}

	__finally
	{
		if(nRet != 1)
			ErrorDisplay(hr);

		SysFreeString(InitProperties[0].vValue.bstrVal);
		SysFreeString(InitProperties[1].vValue.bstrVal);
		SysFreeString(InitProperties[2].vValue.bstrVal);
		SysFreeString(InitProperties[3].vValue.bstrVal);

		return nRet;
	}
}

int COleDBThread::Initialize(wchar_t* szDataSource, wchar_t* szDefaultDb, wchar_t* szUserId, wchar_t* szPassword, 
							 int nConnectTimeout, int nCommandTimeOut, BOOL bCommandTimeOut)
{
    //COM Library 초기화...
	int			nRet = 1;
	DWORD		dwThreadID = 0;
	HRESULT		hr = NULL;	
	DBPROP		InitProperties[6];
	DBPROPSET	rgInitPropSet[1];
	int			nInitProp	= 6;

    //SQLOLEDB Provider 
    hr = ::CoCreateInstance(CLSID_SQLOLEDB, NULL, CLSCTX_INPROC_SERVER, IID_IDBInitialize, (void **) &m_pIDBInitialize);
 	if(FAILED(hr))    
        return -1000;

	__try
	{
		for(int i = 0; i < 6; i++) 
			VariantInit(&InitProperties[i].vValue);
	
		//Server name.
		InitProperties[0].dwPropertyID  = DBPROP_INIT_DATASOURCE;
		InitProperties[0].vValue.vt     = VT_BSTR;
		InitProperties[0].vValue.bstrVal= SysAllocString(szDataSource);
		InitProperties[0].dwOptions     = DBPROPOPTIONS_REQUIRED;
		InitProperties[0].colid         = DB_NULLID;

		//Database.
		InitProperties[1].dwPropertyID  = DBPROP_INIT_CATALOG;
		InitProperties[1].vValue.vt     = VT_BSTR;
		InitProperties[1].vValue.bstrVal= SysAllocString(szDefaultDb);
		InitProperties[1].dwOptions     = DBPROPOPTIONS_REQUIRED;
		InitProperties[1].colid         = DB_NULLID;

		//Username (login).
		InitProperties[2].dwPropertyID  = DBPROP_AUTH_USERID; 
		InitProperties[2].vValue.vt     = VT_BSTR;
		InitProperties[2].vValue.bstrVal= SysAllocString(szUserId);
		InitProperties[2].dwOptions     = DBPROPOPTIONS_REQUIRED;
		InitProperties[2].colid         = DB_NULLID;
		
		//Password.
		InitProperties[3].dwPropertyID  = DBPROP_AUTH_PASSWORD;
		InitProperties[3].vValue.vt     = VT_BSTR;
		InitProperties[3].vValue.bstrVal= SysAllocString(szPassword);
		InitProperties[3].dwOptions     = DBPROPOPTIONS_REQUIRED;
		InitProperties[3].colid         = DB_NULLID;

		//Connection Timeout.
		InitProperties[4].dwPropertyID  = DBPROP_INIT_TIMEOUT;
		InitProperties[4].vValue.vt     = VT_I4;
		InitProperties[4].vValue.intVal = nConnectTimeout;
		InitProperties[4].dwOptions     = DBPROPOPTIONS_REQUIRED;
		InitProperties[4].colid         = DB_NULLID;

		//Command Timeout.
		if(bCommandTimeOut)
		{
			InitProperties[5].dwPropertyID  = DBPROP_COMMANDTIMEOUT;
			InitProperties[5].vValue.vt     = VT_I4;
			InitProperties[5].vValue.intVal = nCommandTimeOut;
			InitProperties[5].dwOptions     = DBPROPOPTIONS_REQUIRED;
			InitProperties[5].colid         = DB_NULLID;
		}
		else
		{
			nInitProp = 5;
		}
  
		rgInitPropSet[0].guidPropertySet	= DBPROPSET_DBINIT;
		rgInitPropSet[0].cProperties		= nInitProp;
		rgInitPropSet[0].rgProperties		= InitProperties;
  
		m_dwCommandTimeOut	= nCommandTimeOut*1000;
		m_bCommandTimeOut	= bCommandTimeOut;	
		m_byTimeOut			= DEF_EXCUTE_START;
		hExcuteEvent		= CreateEvent(NULL, FALSE, FALSE, NULL);

		if(m_bCommandTimeOut)
			m_hDBExcuteThread = BEGINTHREADEX(NULL, NULL, OLEDBExcuteThread, this, NULL, &dwThreadID);		
							
		hr = m_pIDBInitialize->QueryInterface(IID_IDBProperties, (void **)&m_pIDBProperties);
		FAIL_CHECK(nRet, hr, -1100);

		hr = m_pIDBProperties->SetProperties(1, rgInitPropSet); 
		FAIL_CHECK(nRet, hr, -1200);

		//초기화..
		hr = m_pIDBInitialize->Initialize();
		FAIL_CHECK(nRet, hr, -1300);
		
		//Create a session object.
		hr = m_pIDBInitialize->QueryInterface( IID_IDBCreateSession, (void**) &m_pIDBCreateSession);
		FAIL_CHECK(nRet, hr, -1400);

		//세션을 만들고...  요청한 인터페이스 포인터를 받는다.
		//Creates a new session from the data source object and returns the requested interface on the newly created session.
		hr = m_pIDBCreateSession->CreateSession(NULL, IID_IDBCreateCommand, (IUnknown**) &m_pIDBCreateCommand);
		FAIL_CHECK(nRet, hr, -1500);		

		//Access the ICommandText interface.
		//Creates a new command.
		hr = m_pIDBCreateCommand->CreateCommand(NULL, IID_ICommandText, (IUnknown**) &m_pICommandText);
		FAIL_CHECK(nRet, hr, -1600);
	}

	__finally
	{
		if(nRet != 1)
			ErrorDisplay(hr);

		SysFreeString(InitProperties[0].vValue.bstrVal);
		SysFreeString(InitProperties[1].vValue.bstrVal);
		SysFreeString(InitProperties[2].vValue.bstrVal);
		SysFreeString(InitProperties[3].vValue.bstrVal);

		return nRet;
	}
}

BOOL COleDBThread::ChangeDB(WCHAR* szDbName)
{
	HRESULT		hr = NULL;
	DBPROP		InitProperties[1];
	DBPROPSET   rgInitPropSet[1];

    InitProperties[0].dwPropertyID  = DBPROP_CURRENTCATALOG;
    InitProperties[0].vValue.vt     = VT_BSTR;
    InitProperties[0].vValue.bstrVal= SysAllocString(szDbName);
	InitProperties[0].dwOptions     = DBPROPOPTIONS_REQUIRED;
    InitProperties[0].colid         = DB_NULLID;

	rgInitPropSet[0].guidPropertySet = DBPROPSET_DATASOURCE;
    rgInitPropSet[0].cProperties     = 1;
    rgInitPropSet[0].rgProperties    = InitProperties;

	hr = m_pIDBProperties->SetProperties(1, rgInitPropSet); 

	SysFreeString(InitProperties[0].vValue.bstrVal);

	return SUCCEEDED(hr);
}

DBRECEIVEDATA* COleDBThread::OpenRecordEx(wchar_t* szQuerySQL, DWORD dwMaxNumRows, DWORD dwRowPerRead)
{
	HRESULT				 hr						= NULL;
	IRowset*			 pIRowset				= NULL;
	IColumnsInfo*        pIColumnsInfo			= NULL;
	DBCOLUMNINFO*        pDBColumnInfo			= NULL;
	ULONG                ConsumerBufColOffset	= 0;
	ULONG                lNumCols				= 0;		// 몇개 칼럼이 있는지 받아올 변수 
	WCHAR*               pStringsBuffer			= NULL;
	HACCESSOR            hAccessor				= 0;
	ULONG                lNumRowsRetrieved		= 0;		// 몇개의 결과가 나왔는지 저장할 변수 
	HROW*                pRows					= NULL;		// = &hRows[0];
	DBRECEIVEDATA*		 pRData					= NULL;	

	//Creates a new command.
	if(!m_pICommandText)// m_pICommandText객체가 NULL이라면 : 050103 hwoarang 
	{
		return NULL;
	}

    m_pICommandText->Release();

	hr = m_pIDBCreateCommand->CreateCommand(NULL, IID_ICommandText, (IUnknown**) &m_pICommandText);
	FAIL_RETURN(hr, NULL);

	hr = m_pICommandText->SetCommandText(DBGUID_DBSQL, szQuerySQL);
	FAIL_RETURN(hr, NULL);

	if(!SetCommandExecute(szQuerySQL, DEF_IID_IROWSET, NULL, &pIRowset))
		return NULL;

	//Obtain access to the IColumnInfo interface, from the Rowset  object.
    hr = pIRowset->QueryInterface(IID_IColumnsInfo, (void **)&pIColumnsInfo);
	FAIL_RETURN(hr, NULL);
    
    //Retrieve the column information.
    pIColumnsInfo->GetColumnInfo(&lNumCols, &pDBColumnInfo, &pStringsBuffer);

    //Free the column information interface.
    pIColumnsInfo->Release();

    pRData = new DBRECEIVEDATA;
	if(!pRData)
	{
		OutputDebugStringW(L"Faile to allocate DBRECEIVEDATA by new\n");
		return NULL;
	}
	
	pRData->Query.select.bColCount = (BYTE)lNumCols;
	
	//Create a DBBINDING array.
    pRData->Query.select.pBindings = new DBBINDING[lNumCols];
	
    //Using the ColumnInfo structure, fill out the pBindings array.
    for(ULONG j=0; j < lNumCols; j++)
	{
        pRData->Query.select.pBindings[j].iOrdinal		= j+1;
        pRData->Query.select.pBindings[j].obValue		= ConsumerBufColOffset;				//Buffer의 옵셋 
        pRData->Query.select.pBindings[j].pTypeInfo		= NULL;		 				
        pRData->Query.select.pBindings[j].pObject		= NULL;
        pRData->Query.select.pBindings[j].pBindExt		= NULL;
        pRData->Query.select.pBindings[j].dwPart		= DBPART_VALUE;
        pRData->Query.select.pBindings[j].dwMemOwner	= DBMEMOWNER_CLIENTOWNED;
        pRData->Query.select.pBindings[j].eParamIO		= DBPARAMIO_NOTPARAM;
        pRData->Query.select.pBindings[j].cbMaxLen		= pDBColumnInfo[j].ulColumnSize;	// 할당받은 메모리에 ColumnSize 저장.. 
        pRData->Query.select.pBindings[j].dwFlags		= 0;
        pRData->Query.select.pBindings[j].wType			= pDBColumnInfo[j].wType;			// 할당받은 메모리에 ColumnType 저장.. 
        pRData->Query.select.pBindings[j].bPrecision	= pDBColumnInfo[j].bPrecision;		// 정확도..
        pRData->Query.select.pBindings[j].bScale		= pDBColumnInfo[j].bScale;
        
		if(pRData->Query.select.pBindings[j].wType == DBTYPE_WSTR)	//Unicode String이면 
		{
			pRData->Query.select.pBindings[j].cbMaxLen = pDBColumnInfo[j].ulColumnSize * 2;	
			ConsumerBufColOffset = ConsumerBufColOffset + (pDBColumnInfo[j].ulColumnSize * 2);
		}
		else
		{
			pRData->Query.select.pBindings[j].cbMaxLen = pDBColumnInfo[j].ulColumnSize;	//할당받은 메모리에 ColumnSize 저장.. 
			//Compute the next buffer offset.
			ConsumerBufColOffset = ConsumerBufColOffset + pDBColumnInfo[j].ulColumnSize;
		}
    }

	pRData->Query.select.dwRowSize = ConsumerBufColOffset;

    //Get the IAccessor interface.
    hr = pIRowset->QueryInterface(IID_IAccessor, (void **) &m_pIAccessor);
    FAIL_RETURN(hr, pRData);
	
	//Create an accessor from the set of bindings (pBindings).
    m_pIAccessor->CreateAccessor(DBACCESSOR_ROWDATA, lNumCols, pRData->Query.select.pBindings, 0, &hAccessor, NULL);
                                               
  	pRows = new HROW[ dwRowPerRead ];
	assert(pRows != NULL);

	// Get a set of m_NumPerReceive만큼의 rows.
	// lNumRowsRetrieved : 몇개의 결과가 나왔는지..
	// pRows : 받은 값의 포인터.
    pIRowset->GetNextRows(NULL, 0, dwRowPerRead, &lNumRowsRetrieved, &pRows);

    //Allocate space for the row buffer.
	DWORD dwOffset				= 0;
	DWORD dwTotalReceivedRow	= 0;
			
	char* pBuffer = new char[ ConsumerBufColOffset * dwMaxNumRows ];
	assert(pBuffer != NULL);

	memset(pBuffer, 0, ConsumerBufColOffset * dwMaxNumRows);
	
    //Display the rows.
    while(lNumRowsRetrieved > 0) 
	{
        //For each row, print the column data.
        for(j=0; j<lNumRowsRetrieved; j++) 
		{
			if(dwTotalReceivedRow >= dwMaxNumRows)	//파라미터로 지정해준 MAX 행까지 받았으면 Release 한후 끝낸다.
			{
				pIRowset->ReleaseRows(lNumRowsRetrieved, pRows, NULL, NULL, NULL);
				goto QUERYEND;
			}
    
            //Get the row data values.
            pIRowset->GetData(pRows[j], hAccessor, pBuffer+dwOffset);
			dwOffset += ConsumerBufColOffset;
			dwTotalReceivedRow++;	        
        } //for.

        //Release the rows retrieved.
        pIRowset->ReleaseRows(lNumRowsRetrieved, pRows, NULL, NULL, NULL);
        
		//Get the next set of m_NumPerReceive rows.
        pIRowset->GetNextRows(NULL, 0, dwRowPerRead, &lNumRowsRetrieved, &pRows);
    }

QUERYEND:

	char* pResult = new char[ ConsumerBufColOffset * dwTotalReceivedRow ];
	assert(pResult != NULL);
	memcpy(pResult,pBuffer,ConsumerBufColOffset * dwTotalReceivedRow);

	pRData->Query.select.dwRowCount = dwTotalReceivedRow;
	pRData->Query.select.pResult = pResult;
		
    //Free up all allocated memory.
    delete [] pRows;
	delete [] pBuffer;
	
    m_pIAccessor->ReleaseAccessor(hAccessor, NULL);
	m_pIAccessor->Release();
	m_pIAccessor = NULL;

	pIRowset->Release();

	return pRData;
}

DBCOLUMNINFO* COleDBThread::GetColumnInfo(WCHAR* szQuerySQL, DWORD* pColnum)
{
	HRESULT				 hr					= NULL;
	IRowset*			 pIRowset			= NULL;
	IColumnsInfo*        pIColumnsInfo		= NULL;
	DBCOLUMNINFO*        pDBColumnInfo      = NULL;
	WCHAR*               pStringsBuffer		= NULL;

	hr = m_pICommandText->SetCommandText(DBGUID_DBSQL, szQuerySQL);
	FAIL_RETURN(hr, NULL);

	if(!SetCommandExecute(szQuerySQL, DEF_IID_IROWSET, NULL, &pIRowset))
		return NULL;

	//Obtain access to the IColumnInfo interface, from the Rowset  object.
    hr = pIRowset->QueryInterface(IID_IColumnsInfo, (void **)&pIColumnsInfo);
    FAIL_RETURN(hr, NULL);

    //Retrieve the column information.
    pIColumnsInfo->GetColumnInfo(pColnum, &pDBColumnInfo, &pStringsBuffer);

    //Free the column information interface.
    pIColumnsInfo->Release();
	pIRowset->Release();

	return pDBColumnInfo;
}

DBRECEIVEDATA* COleDBThread::OpenRecordExForThread(wchar_t* szQuerySQL, DWORD dwMaxNumRows, DWORD dwRowPerRead)
{
	HRESULT				 hr						= NULL;
	IRowset*			 pIRowset				= NULL;
	IColumnsInfo*        pIColumnsInfo			= NULL;
	DBCOLUMNINFO*        pDBColumnInfo			= NULL;
	ULONG                ConsumerBufColOffset	= 0;
	ULONG                lNumCols				= 0;		//몇개 칼럼이 있는지 받아올 변수 
	WCHAR*               pStringsBuffer			= NULL;
	HACCESSOR            hAccessor				= 0;
	ULONG                lNumRowsRetrieved		= 0;		//몇개의 결과가 나왔는지 저장할 변수 
	HROW*                pRows					= NULL;		// = &hRows[0];
	DBRECEIVEDATA*		 pRData					= NULL;
	
	hr = m_pICommandText->SetCommandText(DBGUID_DBSQL, szQuerySQL);
	FAIL_RETURN(hr, NULL);

	if(!SetCommandExecute(szQuerySQL, DEF_IID_IROWSET, NULL, &pIRowset))
		return NULL;
		
	// ADDED BY DEEPDARK
	if(pIRowset == NULL)
	{
		OutputDebugStringW(L"failed to acquire rowset!");
		return NULL;
	}

	//Obtain access to the IColumnInfo interface, from the Rowset  object.
    hr = pIRowset->QueryInterface(IID_IColumnsInfo, (void **)&pIColumnsInfo);
	FAIL_RETURN(hr, NULL);

    //Retrieve the column information.
    pIColumnsInfo->GetColumnInfo(&lNumCols, &pDBColumnInfo, &pStringsBuffer);
    pIColumnsInfo->Release();

	pRData = (DBRECEIVEDATA*)m_pParent->GetOutputMsgPool()->Alloc();
	if(!pRData)	
	{
		OutputDebugStringW(L"Failed to allocate OutputMessageData");
		return NULL;
	}

	//kkk
//	memset(pRData, 0, sizeof(DBRECEIVEDATA));
	pRData->Query.select.bColCount = (BYTE)lNumCols;
	
	assert(m_pParent != NULL);

	pRData->Query.select.pBindings = (DBBINDING*)m_pParent->GetBindingPool()->Alloc();
	char* pBuffer				   = (char*)m_pParent->GetResultPool()->Alloc();
	
	if(!pRData->Query.select.pBindings)	
	{
		OutputDebugStringW(L"Binding Memory Pool is Full!");
		_asm int 3
	}

	if(!pBuffer)	
	{
		OutputDebugStringW(L"Result Memory Pool is Full!");
		_asm int 3
	}
	
//	memset(pBuffer, 0, m_pParent->GetInitDesc()->dwMaxRowSize * m_pParent->GetInitDesc()->wMaxReturnedRowNum);
//	memset(pRData->Query.select.pBindings, 0, sizeof(DBBINDING));
	
    //Using the ColumnInfo structure, fill out the pBindings array.
    for(ULONG j=0; j < lNumCols; j++)
	{
        pRData->Query.select.pBindings[j].iOrdinal		= j+1;
        pRData->Query.select.pBindings[j].obValue		= ConsumerBufColOffset;				// Buffer의 옵셋.
        pRData->Query.select.pBindings[j].pTypeInfo		= NULL;		 				
        pRData->Query.select.pBindings[j].pObject		= NULL;
        pRData->Query.select.pBindings[j].pBindExt		= NULL;
        pRData->Query.select.pBindings[j].dwPart		= DBPART_VALUE;
        pRData->Query.select.pBindings[j].dwMemOwner	= DBMEMOWNER_CLIENTOWNED;
        pRData->Query.select.pBindings[j].eParamIO		= DBPARAMIO_NOTPARAM;
        pRData->Query.select.pBindings[j].cbMaxLen		= pDBColumnInfo[j].ulColumnSize;	// 할당받은 메모리에 ColumnSize 저장.. 
        pRData->Query.select.pBindings[j].dwFlags		= 0;
        pRData->Query.select.pBindings[j].wType			= pDBColumnInfo[j].wType;			// 할당받은 메모리에 ColumnType 저장.. 
        pRData->Query.select.pBindings[j].bPrecision	= pDBColumnInfo[j].bPrecision;		// 정확도..
        pRData->Query.select.pBindings[j].bScale		= pDBColumnInfo[j].bScale;
        
        //Compute the next buffer offset.
        
		if(pRData->Query.select.pBindings[j].wType == DBTYPE_WSTR)	//Unicode String이면 
		{
			pRData->Query.select.pBindings[j].cbMaxLen = pDBColumnInfo[j].ulColumnSize * 2;	
			ConsumerBufColOffset = ConsumerBufColOffset + (pDBColumnInfo[j].ulColumnSize * 2);
		}
		else
		{
			pRData->Query.select.pBindings[j].cbMaxLen = pDBColumnInfo[j].ulColumnSize;	//할당받은 메모리에 ColumnSize 저장.. 
			//Compute the next buffer offset.
			ConsumerBufColOffset = ConsumerBufColOffset + pDBColumnInfo[j].ulColumnSize;
		}
    }

	pRData->Query.select.dwRowSize = ConsumerBufColOffset;

    //Get the IAccessor interface.
    hr = pIRowset->QueryInterface(IID_IAccessor, (void **) &m_pIAccessor);
    if(FAILED(hr))
    {
		ErrorDisplay(hr);

		if(pRData->Query.select.pBindings)
			m_pParent->GetBindingPool()->Free(pRData->Query.select.pBindings);

		if(pBuffer)
			m_pParent->GetResultPool()->Free(pBuffer);

		if(pRData)
			m_pParent->GetOutputMsgPool()->Free(pRData);	
		
        return NULL;
    }
	
	//Create an accessor from the set of bindings (pBindings).
    m_pIAccessor->CreateAccessor(DBACCESSOR_ROWDATA, lNumCols, pRData->Query.select.pBindings, 0, &hAccessor, NULL);
                          
	pRows = (HROW*)m_pHrowPool->Alloc();
	if(!pRows)
	{
		OutputDebugStringW(L"Failed to allocate Row from pool");
		_asm int 3
	}
	
//	memset(pRows, 0, sizeof(HROW) * m_pParent->GetInitDesc()->wMaxRowPerRead);

	// Get a set of m_NumPerReceive만큼의 rows.
	// lNumRowsRetrieved : 몇개의 결과가 나왔는지..
	// pRows : 받은 값의 포인터.
    //Allocate space for the row buffer.
	DWORD dwOffset = 0;
	DWORD dwTotalReceivedRow = 0;

	pIRowset->GetNextRows(NULL, 0, dwRowPerRead, &lNumRowsRetrieved, &pRows);				

    //Display the rows.
    while(lNumRowsRetrieved > 0) 
	{
        //For each row, print the column data.
        for(j = 0; j < lNumRowsRetrieved; j++) 
		{
			if(dwTotalReceivedRow >= dwMaxNumRows)	//파라미터로 지정해준 MAX 행까지 받았으면 Release 한후 끝낸다.
			{
				pIRowset->ReleaseRows(lNumRowsRetrieved, pRows, NULL, NULL, NULL);
				goto QUERYEND;
			}
    
            //Get the row data values.
            pIRowset->GetData(pRows[j], hAccessor, pBuffer+dwOffset);
			dwOffset += ConsumerBufColOffset;
			dwTotalReceivedRow++;
	        
        } //for.

        //Release the rows retrieved.
        pIRowset->ReleaseRows(lNumRowsRetrieved, pRows, NULL, NULL, NULL);
        
		//Get the next set of m_NumPerReceive rows.
        pIRowset->GetNextRows(NULL, 0, dwRowPerRead, &lNumRowsRetrieved, &pRows);
    }

QUERYEND:
	
	pRData->Query.select.dwRowCount = dwTotalReceivedRow;
	pRData->Query.select.pResult	= pBuffer;
	
    //Free up all allocated memory.	
	if(pRows)
		m_pHrowPool->Free(pRows);

    m_pIAccessor->ReleaseAccessor(hAccessor, NULL);
	m_pIAccessor->Release();
	m_pIAccessor = NULL;

	pIRowset->Release();

	return pRData;
}

BOOL COleDBThread::ReleaseRecordset(DBRECEIVEDATA* pResultData)
{
	if(!pResultData)
		return FALSE;
	
	delete [] pResultData->Query.select.pBindings; 
	delete [] pResultData->Query.select.pResult;

	delete pResultData;

	return TRUE;
}


int	COleDBThread::OpenRecord(WCHAR* szQuerySQL, void* pRecordSet, DWORD dwMaxNumRows)
{
	DWORD				 dwRowPerRead = DEFAULT_ROWS_PER_READ;	
	HRESULT				 hr					= NULL;
	IRowset*			 pIRowset			= NULL;
	IColumnsInfo*        pIColumnsInfo		= NULL;
	DBCOLUMNINFO*        pDBColumnInfo      = NULL;
	ULONG                ConsumerBufColOffset = 0;
	ULONG                lNumCols			= 0;			//몇개 칼럼이 있는지 받아올 변수 
	WCHAR*               pStringsBuffer		= NULL;
	HACCESSOR            hAccessor			= 0;
	ULONG                lNumRowsRetrieved	= 0; //몇개의 결과가 나왔는지 저장할 변수 
	HROW                 hRows[10]			= {NULL,};
	HROW*                pRows				= &hRows[0];
	DBBINDING*           pBindings			= NULL;	

	if(!m_pICommandText)// m_pICommandText객체가 NULL이라면 : 050103 hwoarang 
	{
		return -2900;
	}

	hr = m_pICommandText->SetCommandText(DBGUID_DBSQL, szQuerySQL);
	FAIL_RETURN(hr, -3000);
 
	if(!SetCommandExecute(szQuerySQL, DEF_IID_IROWSET, NULL, &pIRowset))
		return -3100;	

	//Obtain access to the IColumnInfo interface, from the Rowset  object.
    hr = pIRowset->QueryInterface(IID_IColumnsInfo, (void **)&pIColumnsInfo);
	FAIL_RETURN(hr, -3200);

    //Retrieve the column information.
    pIColumnsInfo->GetColumnInfo(&lNumCols, &pDBColumnInfo, &pStringsBuffer);	

    //Free the column information interface.
    pIColumnsInfo->Release();

	//Create a DBBINDING array.
    pBindings = new DBBINDING[lNumCols];		
		
    //Using the ColumnInfo structure, fill out the pBindings array.
    for(ULONG j=0; j<lNumCols; j++)
	{
        pBindings[j].iOrdinal	= j+1;
        pBindings[j].obValue	= ConsumerBufColOffset;		//Buffer의 옵셋 
        pBindings[j].pTypeInfo	= NULL;		 				
        pBindings[j].pObject	= NULL;
        pBindings[j].pBindExt	= NULL;
        pBindings[j].dwPart		= DBPART_VALUE;
        pBindings[j].dwMemOwner = DBMEMOWNER_CLIENTOWNED;
        pBindings[j].eParamIO	= DBPARAMIO_NOTPARAM;
        pBindings[j].wType		= pDBColumnInfo[j].wType;				//할당받은 메모리에 ColumnType 저장.. 
		pBindings[j].dwFlags	= 0;
        pBindings[j].bPrecision = pDBColumnInfo[j].bPrecision;		//정확도..
        pBindings[j].bScale		= pDBColumnInfo[j].bScale;
        
		if(pBindings[j].wType == DBTYPE_WSTR)	//Unicode String이면 
		{
			pBindings[j].cbMaxLen = pDBColumnInfo[j].ulColumnSize * 2;	
			ConsumerBufColOffset = ConsumerBufColOffset + (pDBColumnInfo[j].ulColumnSize * 2);
		}
		else
		{
			pBindings[j].cbMaxLen = pDBColumnInfo[j].ulColumnSize;	//할당받은 메모리에 ColumnSize 저장.. 
			//Compute the next buffer offset.
			ConsumerBufColOffset = ConsumerBufColOffset + pDBColumnInfo[j].ulColumnSize;
		}
    }

    //Get the IAccessor interface.
    hr = pIRowset->QueryInterface(IID_IAccessor, (void **) &m_pIAccessor);
    if(FAILED(hr))
    {
		m_pIAccessor = NULL;
   		delete [] pBindings;
  		pIRowset->Release();
		ErrorDisplay(hr);
        return -3300;
    }
	
	//Create an accessor from the set of bindings (pBindings).
    m_pIAccessor->CreateAccessor(DBACCESSOR_ROWDATA, lNumCols, pBindings, 0, &hAccessor, NULL);                                               
  	
    //Allocate space for the row buffer.
	DWORD dwOffset = 0;
	DWORD dwTotalReceivedRow = 0;

	//Get a set of m_NumPerReceive만큼의 rows.
    pIRowset->GetNextRows(NULL, 0, dwRowPerRead, &lNumRowsRetrieved, &pRows);

    //Display the rows.
    while(lNumRowsRetrieved > 0) 
	{
        //For each row, print the column data.
        for(j=0; j<lNumRowsRetrieved; j++) 
		{
            //Clear the buffer.
            //memset(pBuffer, 0, ConsumerBufColOffset);
            //Get the row data values.
			if(dwTotalReceivedRow >= dwMaxNumRows)	//파라미터로 지정해준 MAX 행까지 받았으면 Release 한후 끝낸다.
			{
				pIRowset->ReleaseRows(lNumRowsRetrieved, hRows, NULL, NULL, NULL);
				goto QUERYEND;
			}
			
			pIRowset->GetData(hRows[j], hAccessor, (BYTE*)pRecordSet + dwOffset);
			dwOffset += ConsumerBufColOffset;
			dwTotalReceivedRow++;
	        
        } //for.

        //Release the rows retrieved.
        pIRowset->ReleaseRows(lNumRowsRetrieved, hRows, NULL, NULL, NULL);
        
		//Get the next set of m_NumPerReceive rows.
        pIRowset->GetNextRows(NULL, 0, dwRowPerRead, &lNumRowsRetrieved, &pRows);
    } 

QUERYEND:
    m_pIAccessor->ReleaseAccessor(hAccessor, NULL);
	m_pIAccessor->Release();
	m_pIAccessor = NULL;
    
	delete [] pBindings;	
	
  	pIRowset->Release();	

	return (int)dwTotalReceivedRow;
}

int	COleDBThread::QueryDBCatalog(DBSCHEMA* pSchemaBuf, DWORD dwMaxNumRows)
{	
	HRESULT						hr					= NULL;
	IDBSchemaRowset				*pIDBSchemaRowset	= NULL;
	IRowset*					pIRowset			= NULL;
	IAccessor*					pIAccessor			= NULL;
	HACCESSOR					hAccessor			= 0;
	ULONG						lret				= 0;
	DWORD						dwOffset			= 0;
	DWORD						dwTotalReceivedRow	= 0;
	WCHAR						wBuffer[ 0xff ]		= {0, };
	char						szBuffer[ 1024 ]	= {0, };
	HROW		                hRows[10]			= {NULL,};
	HROW*				        pRows				= &hRows[0];
	const static DBCOUNTITEM	cBindings			= 1;
	DBBINDING rgBindings[cBindings] = 
	{
			1,		//iOrinal
			0,		//pData = phChapter + 0 offset = phChapter
			0,
			0,	
			NULL,			
			NULL, 		
			NULL,		
			DBPART_VALUE,
			DBMEMOWNER_CLIENTOWNED,		
			DBPARAMIO_NOTPARAM, 
			MAX_SCHEMA_BUFFER_SIZE,
			0, 				
			DBTYPE_WSTR,
			0,	
			0, 				
	};


	//IDBSchemaRowset의 포인터 얻기 		
	hr = m_pIDBCreateCommand->QueryInterface(IID_IDBSchemaRowset, (void**)&pIDBSchemaRowset);
	FAIL_RETURN(hr, -7000);

	hr = pIDBSchemaRowset->GetRowset(NULL, DBSCHEMA_CATALOGS, 0, 
										NULL, IID_IRowset, 0, NULL, (IUnknown**)&pIRowset);
	FAIL_RETURN(hr, -7100);

    hr = pIRowset->QueryInterface(IID_IAccessor, (void **) &pIAccessor);
    FAIL_RETURN(hr, -7200);

	hr = pIAccessor->CreateAccessor(DBACCESSOR_ROWDATA, cBindings, rgBindings, 0, &hAccessor, NULL);
	FAIL_RETURN(hr, -7300);

	hr = pIRowset->GetNextRows(DB_NULL_HCHAPTER, 0, 10, &lret,	&pRows);
	FAIL_RETURN(hr, -7400);	

	while(lret > 0) 
	{
        //For each row, print the column data.
		for(ULONG j=0; j<lret; j++)
		{			
			if(dwTotalReceivedRow >= dwMaxNumRows)	//파라미터로 지정해준 MAX 행까지 받았으면 Release 한후 끝낸다.
			{
				pIRowset->ReleaseRows(lret, hRows, NULL, NULL, NULL);
				goto QUERYEND;
			}
			
			wBuffer[0] = L'\0';
			pIRowset->GetData(hRows[j], hAccessor, wBuffer);
			
			sprintf((LPTSTR)szBuffer, (LPCTSTR)"%S", wBuffer);
			memcpy(pSchemaBuf->szSchemaBuffer + dwOffset, szBuffer, strlen(szBuffer));
			dwOffset += MAX_SCHEMA_BUFFER_SIZE;
			dwTotalReceivedRow++;
		}
		 
        pIRowset->ReleaseRows(lret, hRows, NULL, NULL, NULL);
        pIRowset->GetNextRows(NULL, 0, 10, &lret, &pRows);
	}

QUERYEND:

	pIDBSchemaRowset->Release();
	pIAccessor->ReleaseAccessor(hAccessor, NULL);
	pIAccessor->Release();
	pIRowset->Release();
	
	return (int)dwTotalReceivedRow;
}

int COleDBThread::QueryDBTable(DBSCHEMA *pSchemaBuf, DWORD dwMaxNumRows)
{
	
	HRESULT						hr					= NULL;
	IDBSchemaRowset				*pIDBSchemaRowset	= NULL;
	IRowset*					pIRowset			= NULL;
	IAccessor*					pIAccessor			= NULL;
	HACCESSOR					hAccessor			= 0;
	ULONG						lret				= 0;
	DWORD						dwOffset			= 0;
	DWORD						dwTotalReceivedRow	= 0;
	WCHAR						wBuffer[ 0xff ]		= {0,};
	char						szBuffer[ 1024 ]	= {0,};
	HROW		                hRows[10]			= {NULL,};
	HROW*				        pRows				= &hRows[0];
	const static DBCOUNTITEM	cBindings			= 1;
	DBBINDING rgBindings[cBindings] = 
	{
			3,		//테이블 이름만 받아오자..   테이블 이름이 3번째 칼럼...
			0,		//pData = phChapter + 0 offset = phChapter
			0,
			0,	
			NULL,			
			NULL, 		
			NULL,		
			DBPART_VALUE,
			DBMEMOWNER_CLIENTOWNED,		
			DBPARAMIO_NOTPARAM, 
			MAX_SCHEMA_BUFFER_SIZE,
			0, 				
			DBTYPE_WSTR,
			0,	
			0, 				
	};

	//IDBSchemaRowset의 포인터 얻기 		
	hr = m_pIDBCreateCommand->QueryInterface(IID_IDBSchemaRowset, (void**)&pIDBSchemaRowset);
	FAIL_RETURN(hr, -8000);

	//Restriction 값 주기...   UserTable만 뽑아내기 위해 	
	VARIANT		rgRestrictions[ 4 ];
	ULONG		cRestrictions = 4;
	
	for(ULONG j = 0; j < 4; j++)
			VariantInit( &rgRestrictions[j] );
	
//	rgRestrictions[0].bstrVal = SysAllocString(L"dragon");
//	rgRestrictions[0].vt = VT_BSTR;
	
	rgRestrictions[3].bstrVal = SysAllocString(L"TABLE");
	rgRestrictions[3].vt = VT_BSTR;	

	hr = pIDBSchemaRowset->GetRowset(NULL, DBSCHEMA_TABLES, cRestrictions, 
										rgRestrictions, IID_IRowset, 0, NULL, (IUnknown**)&pIRowset);
	FAIL_RETURN(hr, -8100);

    hr = pIRowset->QueryInterface(IID_IAccessor, (void **) &pIAccessor);
    FAIL_RETURN(hr, -8200);

	hr = pIAccessor->CreateAccessor(DBACCESSOR_ROWDATA, cBindings, rgBindings, 0, &hAccessor, NULL);
	FAIL_RETURN(hr, -8300);

	hr = pIRowset->GetNextRows(DB_NULL_HCHAPTER, 0, 10, &lret,	&pRows);
	FAIL_RETURN(hr, -8400);	

	while(lret > 0) 
	{
        //For each row, print the column data.
		for(ULONG j=0; j<lret; j++)
		{			
			if(dwTotalReceivedRow >= dwMaxNumRows)	//파라미터로 지정해준 MAX 행까지 받았으면 Release 한후 끝낸다.
			{
				pIRowset->ReleaseRows(lret, hRows, NULL, NULL, NULL);
				goto QUERYEND;
			}
			
			wBuffer[0] = L'\0';
			pIRowset->GetData(hRows[j], hAccessor, wBuffer);
			
			sprintf((LPTSTR)szBuffer, (LPCTSTR)"%S", wBuffer);
			memcpy(pSchemaBuf->szSchemaBuffer + dwOffset, szBuffer, strlen(szBuffer));		//Unicode -> Ansi   짱나..ㅡㅡ
			dwOffset += MAX_SCHEMA_BUFFER_SIZE;
			dwTotalReceivedRow++;
		}
		 
        pIRowset->ReleaseRows(lret, hRows, NULL, NULL, NULL);
        pIRowset->GetNextRows(NULL, 0, 10, &lret, &pRows);
	}

QUERYEND:

	pIDBSchemaRowset->Release();
	pIAccessor->ReleaseAccessor(hAccessor, NULL);
	pIAccessor->Release();
	pIRowset->Release();
	
	return (int)dwTotalReceivedRow;
}

BOOL COleDBThread::GetData(void* pReceiveData, DBRECEIVEDATA* pResultData, DWORD dwRowNum, WORD wColumnNum)
{
	if(!pReceiveData) 
		return FALSE;
	
	memcpy(pReceiveData 
			, pResultData->Query.select.pResult + (pResultData->Query.select.dwRowSize * dwRowNum) + pResultData->Query.select.pBindings[ wColumnNum ].obValue 
			, pResultData->Query.select.pBindings[ wColumnNum ].cbMaxLen);

	return TRUE;
}

int	COleDBThread::ExecuteSQL(wchar_t* szQuerySQL)
{
	HRESULT	 hr = NULL;
	LONG     cNumRows = 0;
	
	hr = m_pICommandText->SetCommandText(DBGUID_DBSQL, szQuerySQL);
	FAIL_RETURN(hr, -2000);

	if(!SetCommandExecute(szQuerySQL, DEF_IID_NULL, NULL, NULL, NULL, FALSE, &cNumRows))
		return 0;
	
	return cNumRows;
}

DBBINDING* COleDBThread::CreateParamInfo(WORD wParamNum)
{
	DBBINDING* pDbBinding = new DBBINDING[ wParamNum ];
	if(!pDbBinding)	return NULL;

	for(int i = 0; i < (int)wParamNum; i++)
    {
	    pDbBinding[i].obLength		= 0;
        pDbBinding[i].obStatus		= 0;
        pDbBinding[i].pTypeInfo		= NULL;
        pDbBinding[i].pObject		= NULL;
        pDbBinding[i].pBindExt		= NULL;
        pDbBinding[i].dwPart		= DBPART_VALUE;
        pDbBinding[i].dwMemOwner	= DBMEMOWNER_CLIENTOWNED;
        pDbBinding[i].dwFlags		= 0;
        pDbBinding[i].bScale		= 0;
		pDbBinding[i].iOrdinal		= i+1;
	    pDbBinding[i].eParamIO		= DBPARAMIO_INPUT;
	    pDbBinding[i].bPrecision	= 11;
	} //endfor

	return pDbBinding;
}

BOOL COleDBThread::ReleaseParamInfo(DBBINDING* pBinding)
{
	if(pBinding) delete [] pBinding;

	return TRUE;
}

int COleDBThread::ExecuteSQLByParam(WCHAR* szQuerySQL, DBBINDING* pBinding, void* pParamValue, BYTE bParamNum)
{
	int			i			= 0;
	HRESULT		hr			= NULL;
	DBPARAMS    Params;
	HACCESSOR   hAccessor	= 0;
	DWORD		dwParamSize = 0;
	LONG		cNumRows	= 0;

	//Set the command text.
	hr = m_pICommandText->SetCommandText(DBGUID_DBSQL, szQuerySQL);
    if(FAILED(hr))
    {
       	m_pIAccessor = NULL;
		ErrorDisplay(hr);
		return -4000;
    }
/*	
	[bPrecision(정확도) 설명]
	The maximum precision to use when getting data and 
	wType is DBTYPE_NUMERIC or DBTYPE_VARNUMERIC. 
	This is ignored in three cases: when setting data; 
	when wType is not DBTYPE_NUMERIC or DBTYPE_VARNUMERIC; 
	and when the DBPART_VALUE bit is not set in dwPart. 
	For more information, see "Conversions Involving DBTYPE_NUMERIC 
	or DBTYPE_DECIMAL" in Appendix A: Data Types.
*/
    
    //Create an accessor from the above set of bindings.
    hr = m_pICommandText->QueryInterface(IID_IAccessor, (void**)&m_pIAccessor);
    if (FAILED(hr))
    {
     	m_pIAccessor = NULL;
		ErrorDisplay(hr);
		return -4100;
    }
	
	//총 파라미터의 사이즈를 구하고..
	for(i=0; i<bParamNum ; i++)
		dwParamSize += pBinding[i].cbMaxLen; 

	hr = m_pIAccessor->CreateAccessor(DBACCESSOR_PARAMETERDATA, bParamNum, pBinding, dwParamSize, &hAccessor, NULL);
    if (FAILED(hr))
    {
		ErrorDisplay(hr);
		m_pIAccessor = NULL;
		return -4000;
    }
    
    //Fill in DBPARAMS structure for the command execution. This structure
    //specifies the parameter values in the command and is then passed 
    //to Execute.
    Params.pData = pParamValue;
    Params.cParamSets = 1;
    Params.hAccessor = hAccessor;
	 
	if(!SetCommandExecute(szQuerySQL, DEF_IID_NULL, NULL, NULL, &Params, TRUE, &cNumRows))
	{
		m_pIAccessor->ReleaseAccessor(hAccessor, NULL);
		m_pIAccessor->Release();
		m_pIAccessor = NULL;
		return -4000;
	}  

//	ReleaseParamInfo(pBinding);		//Added 020703	//Deleted 020807
	
    //Free up memory.
    m_pIAccessor->ReleaseAccessor(hAccessor, NULL);
    m_pIAccessor->Release();
	m_pIAccessor = NULL;
    
	return cNumRows;
}

#define SAFE_RELEASE(pIUnknown) if(pIUnknown) (pIUnknown)->Release();

int COleDBThread::GetBLOBData(WCHAR* szTableName, WCHAR* szColumnName, WCHAR* szFilter,
								void* pReadBuffer, DWORD dwReadSize)
{
	WCHAR szwQuery[1024] = {0,};
	swprintf(szwQuery,L"select %s from %s where %s",szColumnName,szTableName,szFilter);
	
	DBOBJECT ObjectStruct;
    ObjectStruct.dwFlags = STGM_READ;
    ObjectStruct.iid     = IID_ISequentialStream;

	BLOBDATA BLOBGetData;

    const ULONG cBindings = 1; 
    DBBINDING rgBindings[cBindings]; 
    HRESULT hr = S_OK;
    
    ICommandProperties* pICommandProperties = NULL;
    IRowsetChange*      pIRowsetChange      = NULL;
    IRowset*            pIRowset            = NULL;
    
	ULONG cRowsObtained = 0;
    HACCESSOR hAccessor = DB_NULL_HACCESSOR;
    DBBINDSTATUS rgBindStatus[cBindings];
    HROW*  rghRows=NULL;	
    
	const ULONG cPropSets = 1;
    DBPROPSET   rgPropSets[cPropSets];
    const ULONG cProperties = 1;
    DBPROP      rgProperties[cProperties];
    
	ULONG       cBytesRead = 0;
   
    /*
    Set the DBPROPSET structure.  It is used to pass an array 
    of DBPROP structures to SetProperties().
    */
    rgPropSets[0].guidPropertySet	= DBPROPSET_ROWSET;
    rgPropSets[0].cProperties		= cProperties;	/*1*/
    rgPropSets[0].rgProperties		= rgProperties;

    //Now set properties in the property group (DBPROPSET_ROWSET)
    rgPropSets[0].rgProperties[0].dwPropertyID	= DBPROP_UPDATABILITY;
    rgPropSets[0].rgProperties[0].dwOptions		= DBPROPOPTIONS_REQUIRED;
    rgPropSets[0].rgProperties[0].dwStatus		= DBPROPSTATUS_OK;
    rgPropSets[0].rgProperties[0].colid			= DB_NULLID;
    rgPropSets[0].rgProperties[0].vValue.vt		= VT_I4;
    V_I4(&rgPropSets[0].rgProperties[0].vValue) = DBPROPVAL_UP_CHANGE;

    //Set the rowset properties
    hr = m_pICommandText->QueryInterface(IID_ICommandProperties, (void**)&pICommandProperties);
	FAIL_RETURN(hr, -5000);
    
	hr = pICommandProperties->SetProperties(cPropSets, rgPropSets);
    FAIL_RETURN(hr, -5100);
    
    hr = m_pICommandText->SetCommandText(DBGUID_DBSQL,szwQuery);     
    if (FAILED(hr))
    {
        pICommandProperties->Release();
		ErrorDisplay(hr);
		return -5200;
    }

	if(!SetCommandExecute(szwQuery, DEF_IID_IROWSETCHANGE, &pIRowsetChange, NULL))
		return -5300;	
   	
	//Fill the DBBINDINGS array.
    rgBindings[0].iOrdinal	= 1; //ordinal position
    rgBindings[0].obValue	= offsetof(BLOBDATA, pISeqStream);
    rgBindings[0].obLength	= offsetof(BLOBDATA, dwLength);
    rgBindings[0].obStatus	= offsetof(BLOBDATA, dwStatus);
    rgBindings[0].pTypeInfo = NULL;
    rgBindings[0].pObject	= &ObjectStruct;
    rgBindings[0].pBindExt	= NULL;
    rgBindings[0].dwPart	= DBPART_VALUE | DBPART_STATUS | DBPART_LENGTH;
    rgBindings[0].dwMemOwner= DBMEMOWNER_CLIENTOWNED;
    rgBindings[0].eParamIO	= DBPARAMIO_NOTPARAM;
    rgBindings[0].cbMaxLen	= 0; 
    rgBindings[0].dwFlags	= 0;
    rgBindings[0].wType		= DBTYPE_IUNKNOWN;
    rgBindings[0].bPrecision= 0;
    rgBindings[0].bScale	= 0;

    //Create an accessor using the binding information.
    hr = pIRowsetChange->QueryInterface(IID_IAccessor, (void**)&m_pIAccessor);
    if (FAILED(hr))
    {
        pICommandProperties->Release();
		ErrorDisplay(hr);
		return -5400;
    }

    hr = m_pIAccessor->CreateAccessor(DBACCESSOR_ROWDATA, cBindings, rgBindings, sizeof(BLOBDATA), &hAccessor, rgBindStatus);
    if (FAILED(hr))
    {
        pICommandProperties->Release();
		pIRowsetChange->Release();
		ErrorDisplay(hr);
		return -5500;
    }

    hr = pIRowsetChange->QueryInterface(IID_IRowset, (void **)&pIRowset);
    if (FAILED(hr))
    {
		pICommandProperties->Release();
		m_pIAccessor->ReleaseAccessor(hAccessor, NULL);
		m_pIAccessor->Release();
		m_pIAccessor = NULL;
		
        pIRowset->ReleaseRows(cRowsObtained, rghRows, NULL, NULL, NULL);
    	pIRowset->Release();
		pIRowsetChange->Release();
		ErrorDisplay(hr);
        return -5600;
    } 

    hr = pIRowset->GetNextRows(NULL, 0, 1, &cRowsObtained, &rghRows);

    hr = pIRowset->GetData(rghRows[0], hAccessor, &BLOBGetData);

    //Verify the retrieved data, only if data is not null.
   	if (BLOBGetData.dwStatus == DBSTATUS_S_ISNULL)
	{
		;
	} 
	else if(BLOBGetData.dwStatus == DBSTATUS_S_OK) 
	  //Provider returned a nonNULL value
	{
		BLOBGetData.pISeqStream->Read(pReadBuffer, dwReadSize, &cBytesRead);
		SAFE_RELEASE(BLOBGetData.pISeqStream);
	}

	hr = pICommandProperties->Release();
	FAIL_RETURN(hr, -5601);

    hr = m_pIAccessor->ReleaseAccessor(hAccessor, NULL);
    FAIL_RETURN(hr, -5700);

	m_pIAccessor->Release();
	m_pIAccessor = NULL;

    hr = pIRowset->ReleaseRows(cRowsObtained, rghRows, NULL, NULL, NULL);
    FAIL_RETURN(hr, -5800);

	pIRowset->Release();
	pIRowsetChange->Release();

	return cBytesRead;
}

int COleDBThread::GetBLOBData(WCHAR* szTableName, WCHAR* szColumnName, WCHAR* szFilter, WCHAR* szFilePath, DWORD dwReadSize)
{	
	char* pBuffer = new char[ dwReadSize ];

	DWORD dwReadByte = GetBLOBData(szTableName, szColumnName, szFilter, pBuffer, dwReadSize);

	if(dwReadByte < 0)
	{
		delete [] pBuffer;
		return -5910;
	}

	HANDLE wHandle = CreateFileW(szFilePath,GENERIC_WRITE, 0, NULL, 
									CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

	if(wHandle == INVALID_HANDLE_VALUE)
	{
		delete [] pBuffer;
		return -5920;
	}

	DWORD nWritten;
	if(!WriteFile(wHandle, pBuffer, dwReadByte, &nWritten, NULL))
	{
		CloseHandle(wHandle);
		delete [] pBuffer;
		return -5930;
	}

	CloseHandle(wHandle);
	delete [] pBuffer;
	return nWritten;
}


int COleDBThread::SetBLOBData(WCHAR* szTableName, WCHAR* szColumnName, WCHAR* szFilter, WCHAR* szFilePath)
{
	//	char buf[20480];	memset(buf,0,20480);
	HANDLE rHandle = CreateFileW(szFilePath,GENERIC_READ, FILE_SHARE_READ, NULL, 
									OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(rHandle == INVALID_HANDLE_VALUE)
	{
		return -6920;
	}

	DWORD dwSize = 0;
	dwSize = GetFileSize(rHandle,NULL);

	if(dwSize == -1)
	{
		CloseHandle(rHandle);
		return -6930;
	}

	char *pBuffer = new char[ dwSize ];

	DWORD dwRead = 0;
    if(!ReadFile(rHandle, pBuffer, dwSize, &dwRead, NULL))
	{
		CloseHandle(rHandle);
		delete [] pBuffer;
	}

	CloseHandle(rHandle);

	int cRowsObtained = SetBLOBData(szTableName, szColumnName, szFilter, pBuffer, dwRead);
	if(cRowsObtained < 0)
	{
		delete [] pBuffer;
		return -6940;
	}

	delete [] pBuffer;
	return cRowsObtained;
}

int COleDBThread::SetBLOBData(WCHAR* szTableName, WCHAR* szColumnName, WCHAR* szFilter,
								void* pWriteData, DWORD	dwWriteSize)
{
	WCHAR szwQuery[1024] = {0,};
	swprintf(szwQuery,L"select %s from %s where %s",szColumnName,szTableName,szFilter);
	
	DBOBJECT ObjectStruct;
    ObjectStruct.dwFlags = STGM_READ;
    ObjectStruct.iid     = IID_ISequentialStream;
    
    BLOBDATA	BLOBSetData;
    const ULONG cBindings = 1; /**/
    DBBINDING	rgBindings[cBindings]; 
    HRESULT		hr = S_OK;
    
    ICommandProperties* pICommandProperties = NULL;
    IRowsetChange*      pIRowsetChange      = NULL;
    IRowset*            pIRowset            = NULL;
    CSeqStream*         pMySeqStream        = NULL;
    
	HROW*			rghRows			= NULL;
	ULONG			cRowsObtained	= 0;
    HACCESSOR		hAccessor		= DB_NULL_HACCESSOR;
    DBBINDSTATUS	rgBindStatus[cBindings];
    
	ULONG		cPropSets	= 1;
    ULONG		cProperties = 1;
	DBPROPSET   rgPropSets[1/*cPropSets*/];    
    DBPROP      rgProperties[1/*cProperties*/];
    
    /*
    Set the DBPROPSET structure.  It is used to pass an array 
    of DBPROP structures to SetProperties().
    */
    rgPropSets[0].guidPropertySet	= DBPROPSET_ROWSET;
    rgPropSets[0].cProperties		= cProperties;	/*1*/
    rgPropSets[0].rgProperties		= rgProperties;

    //Now set properties in the property group (DBPROPSET_ROWSET)
    rgPropSets[0].rgProperties[0].dwPropertyID	= DBPROP_UPDATABILITY;
    rgPropSets[0].rgProperties[0].dwOptions		= DBPROPOPTIONS_REQUIRED;
    rgPropSets[0].rgProperties[0].dwStatus		= DBPROPSTATUS_OK;
    rgPropSets[0].rgProperties[0].colid			= DB_NULLID;
    rgPropSets[0].rgProperties[0].vValue.vt		= VT_I4;
    V_I4(&rgPropSets[0].rgProperties[0].vValue) = DBPROPVAL_UP_CHANGE;

    //Set the rowset properties
    hr = m_pICommandText->QueryInterface(IID_ICommandProperties, (void **)&pICommandProperties);
	FAIL_RETURN(hr, -6000);
    
	hr = pICommandProperties->SetProperties(cPropSets, rgPropSets);
	FAIL_RETURN(hr, -6100);
    
    hr = m_pICommandText->SetCommandText(DBGUID_DBSQL,szwQuery);
                                       //L"SELECT * FROM testoledb where Name='구병수'");
    if (FAILED(hr))
    {
		pICommandProperties->Release();
		ErrorDisplay(hr);
		return -6200;
    } 

	if(!SetCommandExecute(szwQuery, DEF_IID_IROWSETCHANGE, &pIRowsetChange, NULL))
	{
		pICommandProperties->Release();
		return -6300;
	}	
	
	//Fill the DBBINDINGS array.
    rgBindings[0].iOrdinal	= 1; //ordinal position
    rgBindings[0].obValue	= offsetof(BLOBDATA, pISeqStream);
    rgBindings[0].obLength	= offsetof(BLOBDATA, dwLength);
    rgBindings[0].obStatus	= offsetof(BLOBDATA, dwStatus);
    rgBindings[0].pTypeInfo = NULL;
    rgBindings[0].pObject	= &ObjectStruct;
    rgBindings[0].pBindExt	= NULL;
    rgBindings[0].dwPart	= DBPART_VALUE | DBPART_STATUS | DBPART_LENGTH;
    rgBindings[0].dwMemOwner= DBMEMOWNER_CLIENTOWNED;
    rgBindings[0].eParamIO	= DBPARAMIO_NOTPARAM;
    rgBindings[0].cbMaxLen	= 0; 
    rgBindings[0].dwFlags	= 0;
    rgBindings[0].wType		= DBTYPE_IUNKNOWN;
    rgBindings[0].bPrecision= 0;
    rgBindings[0].bScale	= 0;

    //Create an accessor using the binding information.
    hr = pIRowsetChange->QueryInterface(IID_IAccessor, (void**)&m_pIAccessor);
    if (FAILED(hr))
    {
        pICommandProperties->Release();
		ErrorDisplay(hr);
		return -6400;
    } //end if

    hr = m_pIAccessor->CreateAccessor(DBACCESSOR_ROWDATA, cBindings, rgBindings, sizeof(BLOBDATA), &hAccessor, rgBindStatus);
    if (FAILED(hr))
    {
        pICommandProperties->Release();
		pIRowsetChange->Release();
		ErrorDisplay(hr);
		return -6500;
    } 

    //Now get the first row. 
	hr = pIRowsetChange->QueryInterface(IID_IRowset, (void **)&pIRowset);
    if (FAILED(hr))
    {
        pICommandProperties->Release();
		m_pIAccessor->ReleaseAccessor(hAccessor, NULL);
		m_pIAccessor->Release();
		m_pIAccessor = NULL;
	
        pIRowset->ReleaseRows(cRowsObtained, rghRows, NULL, NULL, NULL);
    	pIRowset->Release();
		pIRowsetChange->Release();
		ErrorDisplay(hr);
        return -6600;
    } //end if

    hr = pIRowset->GetNextRows(NULL, 0, 1, &cRowsObtained, &rghRows);

    pMySeqStream = new CSeqStream();

    /*
    Put data in to the ISequentialStream object 
    for the provider to write.
    */
    pMySeqStream->Write(pWriteData, dwWriteSize, NULL);

    BLOBSetData.pISeqStream = (ISequentialStream*)pMySeqStream;
    BLOBSetData.dwStatus    = DBSTATUS_S_OK;
    BLOBSetData.dwLength    = pMySeqStream->Length();
      
    //Set the data.
    hr = pIRowsetChange->SetData(rghRows[0], hAccessor, &BLOBSetData);
    if (FAILED(hr))
    {
        pICommandProperties->Release();
		m_pIAccessor->ReleaseAccessor(hAccessor, NULL);
		m_pIAccessor->Release();
		m_pIAccessor = NULL;
		
        pIRowset->ReleaseRows(cRowsObtained, rghRows, NULL, NULL, NULL);
    	pIRowset->Release();
		pIRowsetChange->Release();
		ErrorDisplay(hr);

		delete pMySeqStream;
		
        return -6600;
    } 
	
	pICommandProperties->Release();

    hr = m_pIAccessor->ReleaseAccessor(hAccessor, NULL);
	FAIL_RETURN(hr, -6700);

	m_pIAccessor->Release();
	m_pIAccessor = NULL;

    hr = pIRowset->ReleaseRows(cRowsObtained, rghRows, NULL, NULL, NULL);
    FAIL_RETURN(hr, -6800);

	pIRowset->Release();
	pIRowsetChange->Release();

	delete pMySeqStream;

	return cRowsObtained;
}

void COleDBThread::ErrorDisplay(HRESULT hrErr)
{
	DWORD MYLOCALEID = GetUserDefaultLCID();  //A locale is a collection of language-related

   IErrorInfo*          pErrorInfo			= NULL;
   IErrorInfo*          pErrorInfoRec		= NULL;
   IErrorRecords*       pErrorRecords		= NULL;
   ISupportErrorInfo*   pSupportErrorInfo	= NULL;
   HRESULT              hr					= NULL;
   ULONG                i = 0, ulNumErrorRecs = 0;
   ERRORINFO            ErrorInfo;

   if(!FAILED(hrErr))
	   return;

   // Check that the current interface supports error objects.
	if(m_pICommandText)//객체가 없을경우 액세스를 방지. 05103 : hwoarang 
	{
		hr = m_pICommandText->QueryInterface(IID_ISupportErrorInfo, (void**) &pSupportErrorInfo);
		if(SUCCEEDED(hr)) 
		{
				GetErrorInfo(0, &pErrorInfo);
				if(!pErrorInfo) 
				{
					if(pSupportErrorInfo)
						pSupportErrorInfo->Release();

					return;
				}
		        
				pErrorInfo->QueryInterface(IID_IErrorRecords, (void**)&pErrorRecords);
				pErrorRecords->GetRecordCount(&ulNumErrorRecs);

				// Read through error records and display them.
				for (i = 0; i < ulNumErrorRecs; i++) 
				{
					// Get basic error information.
					pErrorRecords->GetBasicErrorInfo(i, &ErrorInfo);

					// Get error description and source through the
					// IErrorInfo interface pointer on a particular record.
					hr = pErrorRecords->GetErrorInfo(i, MYLOCALEID, &pErrorInfoRec);
				//	if(FAILED(hr)) // 사용되지 않는 코드라 주석처리했음 - deepdark.
				//		int k=0;

					BSTR bstrDescriptionOfError = NULL;
					BSTR bstrSourceOfError = NULL;

					pErrorInfoRec->GetDescription(&bstrDescriptionOfError);
					pErrorInfoRec->GetSource(&bstrSourceOfError);

					// At this point, you could call GetCustomErrorObject
					// and query for additional interfaces to determine
					// what else happened.

       				WCHAR wstr[2048] = {0, };
					swprintf(wstr,L"HRESULT: %lx, Minor Code: %lu, Source: %s, Description: %s",
					ErrorInfo.hrError,
					ErrorInfo.dwMinor,	//프로바이더 제공 에러코드 
					bstrSourceOfError,	//
					bstrDescriptionOfError);

					char szStr[2048] = {0, };
					int nMultibyteSize = 0;
					nMultibyteSize = WideCharToMultiByte(CP_ACP, 0, wstr, wcslen(wstr), szStr, 2048, NULL, 0);
					szStr[ nMultibyteSize ] = '\0';

					m_pOutputMessageFunc(szStr);

					// Free the resources.
					SysFreeString(bstrDescriptionOfError);
					SysFreeString(bstrSourceOfError);
					pErrorInfoRec->Release();
				}

				// Release the error object.
				pErrorRecords->Release();
				pErrorInfo->Release();

				if(pSupportErrorInfo)
					pSupportErrorInfo->Release();	
		} // if(SUCCEEDED(hr))
	}
}

void COleDBThread::ErrorDisplayEx(HRESULT hrErr, wchar_t* szQuery)
{
	DWORD				MYLOCALEID	= GetUserDefaultLCID();  //A locale is a collection of language-related

   IErrorInfo *          pErrorInfo			= NULL;
   IErrorInfo *          pErrorInfoRec		= NULL;
   IErrorRecords *       pErrorRecords		= NULL;
   ISupportErrorInfo *   pSupportErrorInfo	= NULL;
   HRESULT               hr					= NULL;
   ULONG                 i = 0, ulNumErrorRecs = 0;
   ERRORINFO             ErrorInfo;
   
   if (!FAILED(hrErr))      
	   return;

   // Check that the current interface supports error objects.
   hr = m_pICommandText->QueryInterface(IID_ISupportErrorInfo, (void**) &pSupportErrorInfo);
   if(SUCCEEDED(hr)) 
   {
         GetErrorInfo(0,&pErrorInfo);         
		 if(!pErrorInfo) 
		 {
			 if(pSupportErrorInfo)
				pSupportErrorInfo->Release();

			 return;
		 }
        
         pErrorInfo->QueryInterface(IID_IErrorRecords, (void**) &pErrorRecords);
         pErrorRecords->GetRecordCount(&ulNumErrorRecs);

         // Read through error records and display them.
         for (i = 0; i < ulNumErrorRecs; i++)
		 {
            // Get basic error information.
            pErrorRecords->GetBasicErrorInfo(i, &ErrorInfo);

            // Get error description and source through the
            // IErrorInfo interface pointer on a particular record.
            hr = pErrorRecords->GetErrorInfo(i, MYLOCALEID, &pErrorInfoRec);
			
            BSTR bstrDescriptionOfError = NULL;
            BSTR bstrSourceOfError		= NULL;

            pErrorInfoRec->GetDescription(&bstrDescriptionOfError);
            pErrorInfoRec->GetSource(&bstrSourceOfError);
            
       		WCHAR wstr[2048] = {0, };
			// ErrorInfo.dwMinor : 프로바이더 제공 에러코드 
			swprintf(wstr,L"HRESULT: %lx, Minor Code: %lu, Source: %s, Description: %s, Query : %s",
			   ErrorInfo.hrError, ErrorInfo.dwMinor, bstrSourceOfError, bstrDescriptionOfError, szQuery);

			char szStr[2048] = {0, };
			int nMultibyteSize = 0;
			nMultibyteSize = WideCharToMultiByte(CP_ACP, 0, wstr, wcslen(wstr), szStr, 2048, NULL, 0);
			szStr[ nMultibyteSize ] = '\0';

			m_pOutputMessageFunc(szStr);
			
            // Free the resources.
            SysFreeString(bstrDescriptionOfError);
            SysFreeString(bstrSourceOfError);
            pErrorInfoRec->Release();
         }

         // Release the error object.         
		 pErrorRecords->Release();
		 pErrorInfo->Release();

		 if(pSupportErrorInfo)
			pSupportErrorInfo->Release();
   }
}

COleDBThread::~COleDBThread()
{
	if(m_pICommandText)		m_pICommandText->Release();
	if(m_pIAccessor)		m_pIAccessor->Release();
	if(m_pIDBCreateCommand) m_pIDBCreateCommand->Release();
	if(m_pIDBProperties)	m_pIDBProperties->Release();
    if(m_pIDBCreateSession)	m_pIDBCreateSession->Release();

	if(m_pIDBInitialize)
	{
		HRESULT hr = m_pIDBInitialize->Uninitialize();
		if(FAILED(hr))
		{
			ErrorDisplay(hr);
			MessageBoxW(GetActiveWindow(),L"Uninitialize failed.",L"Warning!",MB_OK);
		}
		m_pIDBInitialize->Release();
	}

	if(m_pHrowPool)			
		delete m_pHrowPool;

	SetEvent(GetExcuteEndEvent());

	if(m_bCommandTimeOut
	&& GetExcuteThreadHandle() )//핸들이 있으면 : 050103 hwoarang
	{
		WaitForSingleObject(GetExcuteThreadHandle(), INFINITE);
		CloseHandle(GetExcuteThreadHandle());
	}
}

LONG COleDBThread::SetCommandExecute(wchar_t* szQuerySQL, BYTE byType, IRowsetChange** pIRowsetChange,
									 IRowset** pIRowset, DBPARAMS* dbParams, BOOL bParam, LONG* cNumRows)
{
	// cNumRows : Update, Insert, Delete 시에 몇개의 행이 영향을 받았는지.
	
	HRESULT	hr = NULL;
	LONG lNumRows = 0;

	__try
	{
		SetEvent(GetExcuteEvent());	

		// Execute the command.	
		switch(byType)
		{
		case DEF_IID_NULL:		
			{
				hr = (bParam) ?
					m_pICommandText->Execute(NULL, IID_NULL, dbParams, &lNumRows, NULL) :
					m_pICommandText->Execute(NULL, IID_NULL, NULL, &lNumRows, NULL);
			}
			break;		

		case DEF_IID_IROWSETCHANGE:
			hr = m_pICommandText->Execute(NULL, IID_IRowsetChange, NULL, &lNumRows, (IUnknown**)pIRowsetChange);
			break;

		case DEF_IID_IROWSET:
			hr = m_pICommandText->Execute(NULL, IID_IRowset, NULL, &lNumRows, (IUnknown**)pIRowset);
			break;
		}

		if(cNumRows)
			*cNumRows = lNumRows;

		SetExcuteInit();

		if(FAILED(hr))
		{
			ErrorDisplayEx(hr, szQuerySQL);
			return 0;
		}

		return 1;
	}

	__except(EXCEPTION_FILTER())
	{
		return 0;
	}
}

DWORD COleDBThread::OleDBExecuteThreadFunc()
{
	for(;;)
	{
		if(WAIT_OBJECT_0 == WaitForSingleObject(GetExcuteEvent(), INFINITE))
		{
			if(GetExcuteType()==DEF_EXCUTE_END)
				return 0;
			
			DWORD dwPrevTime = timeGetTime();
			DWORD dwCurrTime = timeGetTime();

			while(dwPrevTime+GetCommandTimeOutTime() > dwCurrTime)
			{
				dwCurrTime = timeGetTime();

				if(GetExcuteType()==DEF_EXCUTE_START)
					break;
			}

			// 타임아웃 되었을 경우... 로그 남기고 작업 취소한다.
			if(GetExcuteType()==DEF_EXCUTE_TIMEOUT)
			{
				/////////////////////////////////////////////////////////////////////////////////////////////
				WCHAR wszQuery[2048]={0,};
				struct _GUID DBGUID = DBGUID_DBSQL;
				GetCommandText()->GetCommandText(&DBGUID, (WCHAR**)&wszQuery);

				WCHAR wstr[2048] = {0, };
				swprintf(wstr,L"*** Query canceled by time out: %s", wszQuery);

				char szStr[2048] = {0, };
				int nMultibyteSize = 0;
				nMultibyteSize = WideCharToMultiByte(CP_ACP, 0, wstr, wcslen(wstr), szStr, 2048, NULL, 0);
				szStr[ nMultibyteSize ] = '\0';

				m_pOutputMessageFunc(szStr);
				/////////////////////////////////////////////////////////////////////////////////////////////

				GetCommandText()->Cancel();
			}
		}
	}

	return 0;
}

