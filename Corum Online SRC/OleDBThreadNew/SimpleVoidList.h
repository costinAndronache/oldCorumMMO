#ifndef __SIMPLEVOIDLIST_H__
#define __SIMPLEVOIDLIST_H__

#pragma once

#include "CSpinLock.h"
#include "TSMemPool.h"


typedef void* POSITION_;

typedef struct __CNode
{
    __CNode*	pNext;
    __CNode*	pPrev;
    void*		data;
} CNode;

class CMsgQueueUnit
{
public:
	CMsgQueueUnit(DWORD dwMaxNodeNum = 100);
	~CMsgQueueUnit();

	int			GetCount() const		{ return m_nCount ;}
	BOOL		IsEmpty() const			{ return (0 == m_nCount);}
	POSITION_	AddTail(void* newElement);
    POSITION_	GetHeadPosition() const { return (POSITION_) m_pNodeHead; }
	void		RemoveAll();
	void*		GetNext(POSITION_& rPosition) const; // return *Position++

private:

	CNode*		NewNode(CNode* pPrev, CNode* pNext);
    void		FreeNode(CNode* pNode);

    CNode*						m_pNodeHead;
    CNode*						m_pNodeTail;
    int							m_nCount;
	CThreadSafeStaticMemPool*	m_pNodePool;
};

#endif // __SIMPLEVOIDLIST_H__