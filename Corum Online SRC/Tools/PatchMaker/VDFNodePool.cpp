#include "stdafx.h"
#include "VDFNodePool.h"

DFHeap::DFHeap(DWORD dwHeapReservedSize, BOOL bNoSerialize)
{
	m_bNoSerialize = bNoSerialize;
	VERIFY(Create(dwHeapReservedSize));
}

DFHeap::~DFHeap()
{
	VERIFY(HeapDestroy(m_hHeap));
}

BOOL DFHeap::Create(DWORD dwHeapReservedSize)
{
	m_hHeap = HeapCreate(	HEAP_GENERATE_EXCEPTIONS | ((m_bNoSerialize) ? HEAP_NO_SERIALIZE : 0),
							dwHeapReservedSize,
							0);

	return (m_hHeap != NULL);
}

LPVOID DFHeap::Alloc(DWORD dwAllocSize)
{
	LPVOID pMemBlock = 
		HeapAlloc(	m_hHeap, 
					HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY | ((m_bNoSerialize) ? HEAP_NO_SERIALIZE : 0), 
					dwAllocSize);

	ASSERT(pMemBlock != NULL);

	return pMemBlock;
}

void DFHeap::Free(LPVOID pMemBlock)
{
	ASSERT(pMemBlock != NULL);

	if(pMemBlock == NULL) return;

	VERIFY(HeapFree(m_hHeap, ((m_bNoSerialize) ? HEAP_NO_SERIALIZE : 0), pMemBlock));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
CVDFNodePool* CVDFNodePool::s_pVDFNodePoolInstance = NULL;

CVDFNodePool::CVDFNodePool(DWORD dwPoolSize) : DFHeap(dwPoolSize*sizeof(VDFNode), FALSE)
{
	ASSERT(dwPoolSize);
	m_dwPoolSize = dwPoolSize;
}

CVDFNodePool::~CVDFNodePool()
{
	ReleaseAllVDFNode();
}

CVDFNodePool* CVDFNodePool::GetInstance(DWORD dwPoolSize)
{
	if(s_pVDFNodePoolInstance == NULL)
	{
		s_pVDFNodePoolInstance = new CVDFNodePool(dwPoolSize ? dwPoolSize : DEFAULT_POOL_SIZE);
	}
	
	return s_pVDFNodePoolInstance;
}

void CVDFNodePool::DeleteInstance()
{
	if(s_pVDFNodePoolInstance) 
	{
		delete s_pVDFNodePoolInstance;
		s_pVDFNodePoolInstance = NULL;
	}
}

VDFNode* CVDFNodePool::Acquire()
{
	VDFNode* pMsg = NULL;

	if(m_FreeVDFNodePoolList.size() == 0)
	{
		if(m_UsedVDFNodePoolSet.size())
		{
			int iAllocSize = m_UsedVDFNodePoolSet.size() / 2; // 50 % ¾¿ Áõ°¡
			AllocVDFNode(iAllocSize);
			m_dwPoolSize += iAllocSize;
		}
		else
		{
			AllocVDFNode(m_dwPoolSize);
		}
	}

	if(m_FreeVDFNodePoolList.size() == 0) return NULL;

	pMsg = (VDFNode*)m_FreeVDFNodePoolList.front();
	
	m_FreeVDFNodePoolList.pop_front();
	m_UsedVDFNodePoolSet.insert(pMsg);

	return pMsg;
}

void CVDFNodePool::Release(VDFNode* pMsg)
{

#ifdef _DEBUG
	DFMemBlockSet::iterator it = m_UsedVDFNodePoolSet.find(pMsg);
	ASSERT(it != m_UsedVDFNodePoolSet.end());
#endif 

	m_UsedVDFNodePoolSet.erase(pMsg);
	m_FreeVDFNodePoolList.push_back(pMsg);
}

void CVDFNodePool::AllocVDFNode(DWORD dwCount)
{
	while(dwCount--)
	{
		VDFNode* pMsg = new (Alloc(sizeof(VDFNode))) VDFNode;
		m_FreeVDFNodePoolList.push_back(pMsg);
	}
}

void CVDFNodePool::ReleaseAllVDFNode()
{
	VDFNode* pMsg = NULL;
	DFMemBlockSet::iterator it;

	while(!m_FreeVDFNodePoolList.empty())
	{
		pMsg = (VDFNode*)m_FreeVDFNodePoolList.front();
		m_FreeVDFNodePoolList.pop_front();

		Free(pMsg);
	}
	
	m_FreeVDFNodePoolList.clear();
	
	for(it = m_UsedVDFNodePoolSet.begin(); it != m_UsedVDFNodePoolSet.end(); ++it)
	{
		pMsg = (VDFNode*)(*it);

		Free(pMsg);
	}

	m_UsedVDFNodePoolSet.clear();
}

VDFNode* AcquireVDFNode()
{
	return CVDFNodePool::GetInstance()->Acquire();
}

void ReleaseVDFNode(VDFNode* pOld)
{
	ASSERT(pOld != NULL);
	CVDFNodePool::GetInstance()->Release(pOld);
}
