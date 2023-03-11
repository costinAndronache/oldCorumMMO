#include "stdafx.h"
#include "DBMsgQueue.h"




///////////////////////////////////////////////////////////////////////////////////////////

CMsgQueue::CMsgQueue(DWORD dwMaxNodeNum)
{
	m_pMsgQueue[0] = new CMsgQueueUnit(dwMaxNodeNum);
	m_pMsgQueue[1] = new CMsgQueueUnit(dwMaxNodeNum);

	assert(m_pMsgQueue[0] != 0);
	assert(m_pMsgQueue[1] != 0);

	m_lForegroundQueue = 0;
}

CMsgQueue::~CMsgQueue()
{
	delete m_pMsgQueue[0];
	delete m_pMsgQueue[1];
}

inline CMsgQueueUnit* CMsgQueue::GetForegroundQueue()	// insert 하는 큐.
{
	return m_pMsgQueue[m_lForegroundQueue];
}

inline CMsgQueueUnit* CMsgQueue::GetBackgroundQueue()	// remove 하는 큐.
{
	return m_pMsgQueue[!m_lForegroundQueue];
}

void CMsgQueue::SwitchQueues()
{
	AcquireSpinLock(m_Lock);
	m_lForegroundQueue = !m_lForegroundQueue;
	ReleaseSpinLock(m_Lock);
}

POSITION_ CMsgQueue::AddTail(void* newElement)
{
	AcquireSpinLock(m_Lock);
	POSITION_ pos = GetForegroundQueue()->AddTail(newElement);
	ReleaseSpinLock(m_Lock);

	return pos;
}

int CMsgQueue::GetCount(BOOL bForeground)
{
	AcquireSpinLock(m_Lock);
	int iCount = bForeground ? GetForegroundQueue()->GetCount() : GetBackgroundQueue()->GetCount();
	ReleaseSpinLock(m_Lock);
	
	return iCount;
}

BOOL CMsgQueue::IsEmpty(BOOL bForeground)
{
	return (0 == GetCount(bForeground));
}

void CMsgQueue::RemoveAll()
{
	GetBackgroundQueue()->RemoveAll();
}

POSITION_ CMsgQueue::GetHeadPosition()
{
	return GetBackgroundQueue()->GetHeadPosition();
}

void* CMsgQueue::GetNext(POSITION_& pos)
{
	return GetBackgroundQueue()->GetNext(pos);
}

