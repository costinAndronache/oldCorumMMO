#ifndef __ACCEPT_TABLE_H__
#define __ACCEPT_TABLE_H__


#pragma once


#include "../4DyuchiGXGFunc/global.h"
#include "OnlyList.h"
#include "WorldUser.h"


struct ACCEPT_USER
{
	DWORD			dwSerialCode;		//Agent�� �ø��� �ڵ� 
	DWORD			dwChar_Index;		//������ Character�� DB���� Char_Index
	DWORD			dwAcceptedTick;		//Accept �� ������ TickCount
	POSITION_		AcceptedListPos;	//LinkList Position

	BILLING_INFO	BillingInfo;		//���� ���� 

	BYTE			bBillingType;		//���� Ÿ�� 
	DWORD			dwRemainTime;		//���� �ð� 
	DBTIMESTAMP		BillingEday;		//���� �����½ð� 
	DWORD			dwConnection;
	DWORD			dwPropID;
	BYTE			bEventFlag;
	ACCEPT_USER*	pPrevData;
	ACCEPT_USER*	pNextData;
};


class CAcceptTable
{
	DWORD					m_dwMaxBucketNum;	
	DWORD					m_dwUserNum;			
	ACCEPT_USER**			m_ppInfoTable;
	STMPOOL_HANDLE			m_pAcceptPool;	

	void					RemoveAcceptTable();

public:	

	COnlyList				*m_pAcceptedList;
	
	ACCEPT_USER*			AllocNewAccept();
	ACCEPT_USER*			GetAcceptInfo(DWORD dwChar_Index);
	BOOL					Add(ACCEPT_USER* pUser);
	void					Remove(ACCEPT_USER* pUser);
	DWORD					GetCount()	{	return m_dwUserNum;	}

	CAcceptTable(DWORD dwBucketNum, DWORD dwPoolSize);
	~CAcceptTable();
};


extern CAcceptTable*	g_pAcceptTable;

#endif 