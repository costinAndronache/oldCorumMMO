#ifndef __DBMSGQUEUE_H__
#define __DBMSGQUEUE_H__

#pragma once

#include "SimpleVoidList.h"
#include "dbstruct.h"
#include "define.h"
#include "CSpinLock.h"

#pragma pack(push,1)

struct DBMSG_OPENRECORD
{
	DWORD dwMaxNumRows;
	DWORD dwRowPerRead;
};

struct DBMSG_EXECUTESQL
{
	BYTE	bReturnResult;
};

struct DBMSG_EXECUTESQL_BY_PARAM
{
	BYTE		bReturnResult;
	BYTE		bParamNum;
	DBBINDING*	pBinding;
	char		szParamValueBuf[ MAX_PARAM_VALUE_SIZE ];
};

struct DBMSG_CHANGE_DB
{
	BYTE		bReturnResult;
};

typedef struct DBCommandMsg
{
	BYTE	bQueryType;
	BYTE	bConnectionIndex;
	DWORD	dwQueryUID;
	void*	pData;
	WCHAR	wszQuery[ MAX_SQL_STRING_LENGTH + 1 ];

	union
	{
		DBMSG_OPENRECORD			select;
		DBMSG_EXECUTESQL			execute;
		DBMSG_EXECUTESQL_BY_PARAM	execute_by_param;
		DBMSG_CHANGE_DB				change_db;

	} AddInfo;

} *LPDBCMDMSG;

#pragma pack(pop)


// �׻�.... INSERT �ϴ� ���� FOREGROUD�� �����Ѵ�.
// ��� �ִ� ���� ó���ϰ� ����� ���� BACKGROUND�� �Ѵ�. 
// �� �̻��ϴ��� ���ƶ�.. �̸����� ������.
class CMsgQueue
{
public:
	CMsgQueue(DWORD dwMaxNodeNum = 100);
	~CMsgQueue();

	// ť ����Ī.
	void			SwitchQueues();

	// ���� ť �޽��	
	int				GetCount(BOOL bForeground);
	BOOL			IsEmpty(BOOL bForeground);

	// forground�� �޽��
	POSITION_		AddTail(void* newElement);

	// background�� �޽��
	void			RemoveAll();
	POSITION_		GetHeadPosition();
	void*			GetNext(POSITION_& pos);

private:
	CMsgQueueUnit*	GetForegroundQueue();	// insert �ϴ� ť.
	CMsgQueueUnit*	GetBackgroundQueue();	// remove �ϴ� ť.


	LONG volatile	m_lForegroundQueue;	// ���� ť�� ǥ��.
	CSpinLock		m_Lock;
	CMsgQueueUnit*	m_pMsgQueue[2];

};


#endif // __DBMSGQUEUE_H__