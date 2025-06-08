#ifndef __WORLD_USER_TABLE__
#define __WORLD_USER_TABLE__

#pragma once

#include "../SS3D_0719/4DYUCHIGX/4DyuchiGXGFunc/global.h"
#include "../CommonServer/CommonAllServers.h"
#include "../CommonServer/CommonClientDungeonLoginWorld.h"

#include "OnlyList.h"
#include "WorldUser.h"
#include "NameSearchTable.h"


class CWorldUserTable
{
	
	DWORD					m_dwUserNum;			//Global User Count	

	STMPOOL_HANDLE			m_pUserPool;		//�������� Alloc�� �޸�Ǯ ������ 
	CNameSearchTable*		m_pNameSearch;		//���� �̸����� ã�� ���� String Hash Table

	void					RemoveWorldUserTable();
	
	BASE_CLASS_INFO			m_BaseClassInfo[ MAX_CLASS + 1 ];	

public:								  

	COnlyList*				m_pTotalUserList;		//��ü ���� ����Ʈ 
	COnlyList*				m_pUserListInWorld;		//����ʿ� �ִ� ���� ����Ʈ 
	DWORD					m_dwMaxBucketNum;	
	CWorldUser**			m_ppInfoTable;
	LPBASE_CLASS_INFO		GetBaseClassInfo( BYTE bClassType )	{	return &m_BaseClassInfo[ bClassType ];	}

	CWorldUser*				AllocNewUser();
	CWorldUser*				GetUserInfo(DWORD dwUserIndex);
	CWorldUser*				GetUserInfoByName(const char* szCharacterName);		//Name���� ��ġ 
	CWorldUser*				GetUserInfoByNameWithoutCase(const char* szCharacterName);	//Name���� ��ġ, case ����.
	BOOL					Add(CWorldUser* pUser);
	void					Remove(DWORD dwConnectionIndex);
	void					Remove(CWorldUser* pInfo);
	void					BroadCast(char* pPacket, DWORD dwLength);
	void					SendUserInWorld(char* pPacket, DWORD dwLength);
	DWORD					GetCount()	{	return m_dwUserNum;		}
	void					RemoveUserResource(CWorldUser* pUser);
	BOOL					AddCharacterToSearchTable(CWorldUser* pUser);

//	void					DeleteUserFromDBLoginTable(char* szID);	//SYSTEM DB�� ConnectingTable���� ���� ���� 
	
	CWorldUserTable( DWORD dwBucketNum, DWORD dwPoolSize );
	~CWorldUserTable();
};


extern CWorldUserTable* g_pUserTable;


#endif // __WORLD_USER_TABLE__