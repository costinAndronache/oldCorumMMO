#ifndef __THMEMPOOL_H__
#define __THMEMPOOL_H__

#pragma once

#include "CSpinLock.h"

class CThreadSafeStaticMemPool
{
public:
	CThreadSafeStaticMemPool(int dwObjSize, DWORD dwBaseSize, DWORD dwMaxCount);
	~CThreadSafeStaticMemPool();

	void*	Alloc();
	void	Free(void* pBlock);

private:
	DWORD			m_dwObjSize;
	STMPOOL_HANDLE	m_pPool;
	CSpinLock		m_Lock;	
};

#endif // __THMEMPOOL_H__