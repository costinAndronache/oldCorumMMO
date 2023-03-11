#ifndef __WORLD_USER_TABLE__
#define __WORLD_USER_TABLE__

#pragma once

#include "../4DyuchiGXGFunc/global.h"

#include "OnlyList.h"
#include "WorldUser.h"
#include "NameSearchTable.h"

#define MAX_CLASS		5

typedef struct BASE_CLASS_INFO
{
	int		iEGO;
	int		iSTR;
	int		iINT;
	int		iDEX;
	int		iVIT;
	int		iBHP;
	int		iBMP;
	int		iLHP;
	int		iLMP;
	int		iHP;
	int		iMP;
	int		iAA;
	int		iAD;
	int		iMD;
	int		iDP;
	int		iBR;
	int		iDR;
	int		iHR;
	int		iMS;
	int		iAS;
	int		iCS;
	int		iRFire;
	int		iRIce;
	int		iRLighting;
	int		iRPhisics;
	int		iRPoison;
	int		iMax_Aura;
	int		Max_Magic;
	int		Max_Divine;
	int		Max_Chakra;
	int		Max_Summon;
}* LPBASE_CLASS_INFO;



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