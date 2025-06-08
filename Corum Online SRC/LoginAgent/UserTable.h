#ifndef __USER_TABLE_H__
#define __USER_TABLE_H__

#pragma once

#include "../SS3D_0719/4DYUCHIGX/4DyuchiGXGFunc/global.h"
#include "../CommonServer/CommonAllServers.h"
#include "../CommonServer/CommonClientDungeonLoginWorld.h"
#include "OnlyList.h"

class CUser;
class CNameSearchTable;


#define MAX_MONITORING_COUNT	16

class CUserTable
{
	
	DWORD					m_dwNum;			//Global User Count
	STMPOOL_HANDLE			m_pUserPool;		//유저정보 Alloc할 메모리풀 포인터 
	BASE_CLASS_INFO			m_BaseClassInfo[ MAX_CLASS + 1 ];

	void					RemoveUserTable();

public:								 

	COnlyList*				m_pUserList;		
	DWORD					m_dwMaxBucketNum;	
	CUser**					m_ppInfoTable;
	DWORD					a_dwMonitoringList[MAX_MONITORING_COUNT];

	CUser*					AllocNewUser();
	void					FreeUser(CUser* pUser);
	CUser*					GetUserInfo(DWORD dwPropID);
	BOOL					Add(CUser* pUser);
	void					Remove(DWORD dwConnectionIndex);
	void					Remove(CUser* pInfo);
	void					BroadCast(char* pPacket, DWORD dwLength);
	DWORD					GetCount()	{	return m_dwNum;		}
	LPBASE_CLASS_INFO		GetBaseClassInfo( BYTE bClassType )	{	return &m_BaseClassInfo[ bClassType ];	}
	COnlyList*				GetUserList()	{ return m_pUserList; }	
	void					BroadCastToMonitoringTool(char* pPacket, DWORD dwLength);
	bool					AddMonitoringTool(DWORD dwConnectionIndex);
	bool					RemoveMonitoringTool(DWORD dwConnectionIndex);
	POSITION_				GetHeadPosition();
	CUser*					GetNext(POSITION_& Position);

	CUserTable(DWORD dwNum);
	~CUserTable();		
};


extern CUserTable	*g_pUserTable;	


#endif 