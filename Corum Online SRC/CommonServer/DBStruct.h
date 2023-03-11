#pragma once

#include "stdafx.h"
#include "oledb.h"

#pragma pack(push,1)

#define MAX_DB_CONNECTION				10
#define DEFAULT_RETURNED_MAX_ROWS		20
#define DEFAULT_ROWS_PER_READ			10
#define MAX_SCHEMA_BUFFER_SIZE			128
#define DEFAULT_STRING_LENGTH			128

#define MAX_SQL_STRING_LENGTH			1023

#define QUERY_TYPE_SELECT				1	//select
#define QUERY_TYPE_EXECUTE				2	//update, delete, sp....
#define QUERY_TYPE_EXECUTE_BY_PARAM		3
#define QUERY_TYPE_CHANGE_DB			4
#define QUERY_TYPE_USER_LOGOUT			5
#define QUERY_TYPE_COMPULSIVE_LOGOUT	6


struct ISequentialStream;


struct RECEIVEDATA_SELECT
{
	DWORD			dwRowCount;	//����� �����ȯ�Ǵ��� 
	DWORD			dwRowSize;	//�� ���� Data Size
	DBBINDING*		pBindings;	//Į�� ����
	BYTE			bColCount;	//Į�� ���� 
	char*			pResult;	//���� �޾ƿ��� ��� ������ 
};

struct RECEIVEDATA_EXECUTE
{
	DWORD	dwEffected;
};


struct DBRECEIVEDATA
{
	BYTE			bQueryType;	//���� Ÿ�� 
	DWORD			dwQueryUID;	//������ȣ 
	int				nError;		//�����̸� Error
	
	union
	{
		RECEIVEDATA_SELECT select;
		RECEIVEDATA_EXECUTE execute;
		RECEIVEDATA_EXECUTE execute_by_param;
	} Query;
	
	void*			pData;		//�ΰ����� 
};


struct PARAMVALUE
{
	int		Age;
	double	Eye;
	BYTE	Myimage[16];
};

struct BLOBDATA
{
    DBSTATUS            dwStatus;   
    DWORD               dwLength; 
    ISequentialStream*  pISeqStream;
};

struct DBSCHEMA
{
	char szSchemaBuffer[ MAX_SCHEMA_BUFFER_SIZE ];

	DBSCHEMA()
	{
		memset(szSchemaBuffer, 0, sizeof(szSchemaBuffer));
	}
};


struct DB_INITIALIZE_DESC
{
	BYTE		bUsingThread;
	BYTE		bUsingEventObject;
	BYTE		bMaxParamNum;						//ExecuteSqlByParam���� �����Ҽ� �ִ� �ִ� �Ķ���� ����
	WORD		wMaxNumOfProcessMessage_Input;		//DLL���� �ѹ��� �����Ҽ� �ִ� �޼��� ����..  (�޼����� �޸�Ǯ ����)
	WORD		wMaxNumOfProcessMessage_Output;		//DLL���� �ѹ��� �����Ҽ� �ִ� �޼��� ����..  (�޼����� �޸�Ǯ ����)
	WORD		wMaxRowPerRead;						//DB���� �о�帱�� �ѹ��� ���پ� �о�帱���ΰ�.. ( �������� ������ ������ ���� )
	WORD		wMaxReturnedRowNum;					//Select���� ���ü� �ִ� ���� �ִ� ����
	WORD		wMaxReturnedColNum;					//Select���� ���ü� �ִ� Į���� �ִ� ����
	DWORD		dwMaxRowSize;						//����� �� ���� �ִ� ������ 
	UINT		uMessage;
	HWND		hWndToPostMessage;
	void		(*OutputMessageFunc)(char*);		//�޼��� ����� �Լ� ������ 
	void		(*ReceiveFunc)(DBRECEIVEDATA*);		//ObjectFunction �Լ� ������ 
	void		(*ReportFunc)(char*);				//ReportFunc 

};


#pragma pack(pop)


