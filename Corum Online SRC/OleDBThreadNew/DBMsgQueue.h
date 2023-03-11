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


// 항상.... INSERT 하는 넘이 FOREGROUD로 정의한다.
// 대신 있는 넘을 처리하고 지우는 넘이 BACKGROUND로 한다. 
// 좀 이상하더라도 참아라.. 이름짓기 귀찮다.
class CMsgQueue
{
public:
	CMsgQueue(DWORD dwMaxNodeNum = 100);
	~CMsgQueue();

	// 큐 스위칭.
	void			SwitchQueues();

	// 공용 큐 메쏘드	
	int				GetCount(BOOL bForeground);
	BOOL			IsEmpty(BOOL bForeground);

	// forground용 메쏘드
	POSITION_		AddTail(void* newElement);

	// background용 메쏘드
	void			RemoveAll();
	POSITION_		GetHeadPosition();
	void*			GetNext(POSITION_& pos);

private:
	CMsgQueueUnit*	GetForegroundQueue();	// insert 하는 큐.
	CMsgQueueUnit*	GetBackgroundQueue();	// remove 하는 큐.


	LONG volatile	m_lForegroundQueue;	// 현재 큐를 표시.
	CSpinLock		m_Lock;
	CMsgQueueUnit*	m_pMsgQueue[2];

};


#endif // __DBMSGQUEUE_H__