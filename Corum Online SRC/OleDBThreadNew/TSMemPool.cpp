#include "stdafx.h"
#include "TSMemPool.h"



CThreadSafeStaticMemPool::CThreadSafeStaticMemPool(int dwObjSize, DWORD dwBaseSize, DWORD dwMaxCount)
{
	m_dwObjSize = dwObjSize;
	m_pPool = CreateStaticMemoryPool();
	assert(m_pPool != NULL);

	InitializeStaticMemoryPool(m_pPool, dwObjSize, dwBaseSize, dwMaxCount);
}

CThreadSafeStaticMemPool::~CThreadSafeStaticMemPool()
{
	if(m_pPool)
	{
		ReleaseStaticMemoryPool(m_pPool); 
	}
}

void* CThreadSafeStaticMemPool::Alloc()
{
	assert(m_pPool != NULL);

	void* pMemBlock = 0;

	AcquireSpinLock(m_Lock);
	pMemBlock = LALAlloc(m_pPool);
	ReleaseSpinLock(m_Lock);

	assert(pMemBlock != NULL);

	ZeroMemory(pMemBlock, m_dwObjSize);

	return pMemBlock;
}

void CThreadSafeStaticMemPool::Free(void* pBlock)
{
	assert(pBlock);

	AcquireSpinLock(m_Lock);
	LALFree(m_pPool, pBlock);
	ReleaseSpinLock(m_Lock);
}
