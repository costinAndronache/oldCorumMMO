#ifndef __VDFNODEPOOL_H__
#define __VDFNODEPOOL_H__

#pragma once

#include "VDFFile.h"
#include <set>
#include <list>
#pragma warning(disable: 4018 4702)

using namespace std;


class DFHeap
{
public:
	DFHeap(DWORD dwHeapReservedSize, BOOL bNoSerialize = FALSE);
	~DFHeap();

	LPVOID			Alloc(DWORD dwAllocSize);
	void			Free(LPVOID pMemBlock);

protected:
	BOOL			Create(DWORD dwHeapReservedSize);

	HANDLE			m_hHeap;
	BOOL			m_bNoSerialize;
};

class CVDFNodePool : public DFHeap
{
	typedef set<VDFNode*> DFMemBlockSet;
	typedef list<VDFNode*> DFMemBlockList;

	#define DEFAULT_POOL_SIZE	10240

public:
	~CVDFNodePool();
	static CVDFNodePool* GetInstance(DWORD dwPoolSize = 0);
	static void DeleteInstance();

	VDFNode*	Acquire();
	void		Release(VDFNode* pMsg);

protected:
	CVDFNodePool(DWORD dwPoolSize);

private:
	void		AllocVDFNode(DWORD dwCount);
	void		ReleaseAllVDFNode();

	DWORD				m_dwPoolSize;
	DFMemBlockSet		m_UsedVDFNodePoolSet;
	DFMemBlockList		m_FreeVDFNodePoolList;

	static CVDFNodePool* s_pVDFNodePoolInstance;
};

VDFNode*	AcquireVDFNode();
void		ReleaseVDFNode(VDFNode* pOld);


#endif // __VDFNODEPOOL_H__
