#ifndef __CONNECTION_DISTRIBUTION_TABLE_H__
#define __CONNECTION_DISTRIBUTION_TABLE_H__


#pragma once


#include "stdafx.h"
#include "OnlyList.h"
#include "../4DyuchiGXGFunc/global.h"
#include "WorldUser.h"


class CConnectionDistributionTable
{	
	DWORD					m_dwConnectionNum;				//���� �����ϰ� �ִ� Connection ���� 
	DWORD					m_dwMaxConnectionPerBucket;		//�� ������ ������ �ִ� �ִ� Connection ���� 
	DWORD					m_dwAllocListNumPerOverflow;	//�� ������ ����Ʈ�� �� �������� Alloc�� List�� ����  	
	DWORD					m_dwCurBucketListCount;			//���� ������ BucketList Count
	DWORD					m_dwCurBucketID;				//���� ����Ű�� �ִ� Bucket �ѹ� 

public:								  

	DWORD					m_dwNextCheckBucketListID;		//������ KeepAlive Check�ؾ��� List ID
	DWORD					m_dwMaxBucketNum;	
	COnlyList**				m_ppBucketList;

	DWORD AllocAddedList();
	DWORD SetCurBucketList(DWORD dwCurID);
	BOOL  Add(CWorldUser* pUser);
	BOOL  Remove(CWorldUser* pUser);
	DWORD SendKeepAlivePacket();
	void  CheckKeepAlive();
	void  SetNextList();
	
	CConnectionDistributionTable(DWORD dwMaxBucketNum, DWORD dwMaxConnectionPerBucket, DWORD dwAllocListNumPerOverFlow);
	~CConnectionDistributionTable();
};


extern CConnectionDistributionTable* g_pCDTable;


#endif 