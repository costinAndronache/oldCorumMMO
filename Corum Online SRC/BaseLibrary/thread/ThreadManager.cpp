#ifdef _MT

#include "../StdAfx.h"
#include "ThreadManager.h"

#include <algorithm>
#include <functional>

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructor and destructor
CThreadManager::CThreadManager(void) : m_nThreadNum(0), m_bJoinStarted(FALSE)
{
	for (int i=0; i<MAX_THREAD_NUM; ++i) {
		m_pThreads[i] = NULL;
		m_hThreads[i] = INVALID_HANDLE_VALUE;
	}
}

CThreadManager::~CThreadManager(void)
{
//	JoinThread();
}

bool CThreadManager::RegisterAndRun(CThread* pThread)
{		
	unsigned int nThreadID		= 0;
	unsigned int nThreadIndex	= 0;

	if (NULL == pThread) {		
		return false;
	}

	// Lock
	{
		CBCSLock::Syncronize sync(m_ThreadLock);

		if (MAX_THREAD_NUM <= m_nThreadNum || TRUE == m_bJoinStarted) {
			return false;	
		}

		nThreadIndex = m_nThreadNum++;
	}

	m_pThreads[nThreadIndex] = pThread;
	m_hThreads[nThreadIndex] = reinterpret_cast<HANDLE>(::_beginthreadex(NULL, 0, CThread::ThreadFunc, pThread, 0, &nThreadID));

	return (0 != m_hThreads[nThreadIndex]);
}

bool CThreadManager::JoinThread( bool bDelete )
{
	{			
		CBCSLock::Syncronize sync(m_ThreadLock);
		if (0 == m_nThreadNum) {
			return true;
		}

		// ������ ���۵Ǹ�, ���̻��� ������ ���� �� ����� ������.
		m_bJoinStarted = TRUE;
	}

	// ������ ���� ���� & ���.
	std::for_each(&m_pThreads[0], &m_pThreads[m_nThreadNum], std::mem_fun(&CThread::End));	
	WaitForMultipleObjects(m_nThreadNum, m_hThreads, TRUE, INFINITE);

	CThread**	ppPastEndThread = m_pThreads + MAX_THREAD_NUM;
	HANDLE*		ppPastEndHandle = m_hThreads + MAX_THREAD_NUM;

	// close thread and thread handle
	for (int i=0; i<m_nThreadNum; ++i) {
		if( bDelete )
			delete m_pThreads[i];
		::CloseHandle(m_hThreads[i]);

		m_pThreads[i] = NULL;
		m_hThreads[i] = INVALID_HANDLE_VALUE;
	}
    
	{
		// ������ ���� �� ���� ���� ����.
		CBCSLock::Syncronize sync(m_ThreadLock);
		m_nThreadNum = 0;
		m_bJoinStarted = FALSE;
	}
	
	return true;
}

HANDLE CThreadManager::Run(CThread* pThread) 
{
	unsigned int nThreadID = 0;
	HANDLE hThread = reinterpret_cast<HANDLE>(_beginthreadex(NULL,  0, CThread::ThreadFunc, pThread, 0, &nThreadID));

	pThread->SetHandle(hThread);
	return hThread;
}

/*//
bool CThreadManager::Stop(CThread* pThread, DWORD dwTimeout)
{
	if (0 == pThread) {
		return false;
	}

	HANDLE hThread = pThread->GetHandle();
	if (INVALID_HANDLE_VALUE == hThread) {
		return false;
	}

	pThread->SetHandle(INVALID_HANDLE_VALUE);
	pThread->End();
	WaitForSingleObject(hThread, dwTimeout);

	return (TRUE == CloseHandle(hThread));
}
//*/

#endif
