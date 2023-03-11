#pragma once

#include <winsock2.h>
#include <windows.h>
#include <process.h>

#include "Lock.hpp"
#include "Thread.h"


typedef class CThreadManager*		LPCThreadManager;
class CThreadManager
{
private:
	enum { MAX_THREAD_NUM = 15 };

	CThread*		m_pThreads[MAX_THREAD_NUM];
	HANDLE			m_hThreads[MAX_THREAD_NUM];
	
	CBCSLock		m_ThreadLock;

	int				m_nThreadNum;
	int				m_bJoinStarted;


public:
	// constructor and destructor
	CThreadManager(void);
	virtual ~CThreadManager(void);


	bool			RegisterAndRun(CThread* pThread);		// �����带 ����Ѵ�. �ִ� 63������ ����� �� �ִ�.
	bool			JoinThread(bool bDelete = true);						// ��� �����带 �����Ų��.

	inline int		GetThreadNum(void);
	inline CThread*	GetThreadPtr(int nIndex);

	// �Ŵ����� ������� �ʰ�, �׳� ���� ��Ų��.
	static HANDLE	Run(CThread* pThread);
	//static bool		Stop(CThread* pThread, DWORD dwTimeout = INFINITE);
};

int CThreadManager::GetThreadNum(void)
{
	return m_nThreadNum;
}

CThread* CThreadManager::GetThreadPtr(int nIndex)
{
	return m_pThreads[nIndex];
}
