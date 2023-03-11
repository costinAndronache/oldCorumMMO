#include "stdafx.h"
#include "SimpleVoidList.h"

CMsgQueueUnit::CMsgQueueUnit(DWORD dwMaxNodeNum)
{
	m_pNodeHead	= NULL;
    m_pNodeTail	= NULL;
    m_nCount	= 0;

	m_pNodePool = new CThreadSafeStaticMemPool(sizeof(CNode), dwMaxNodeNum/2, dwMaxNodeNum);
	assert(m_pNodePool != NULL);
}

CMsgQueueUnit::~CMsgQueueUnit()
{
	RemoveAll();
	delete m_pNodePool;
}

CNode* CMsgQueueUnit::NewNode(CNode* pPrev, CNode* pNext)
{
	CNode* pNode = (CNode*)m_pNodePool->Alloc();
	if(pNode == NULL) return NULL;

    pNode->pPrev = pPrev;
    pNode->pNext = pNext;
    pNode->data  = NULL;
    
	m_nCount++;

	return pNode;
}

void CMsgQueueUnit::FreeNode(CNode* pNode)
{
    if(pNode == NULL) return;

	m_pNodePool->Free(pNode);

    m_nCount--;
}

POSITION_ CMsgQueueUnit::AddTail(void* newElement)
{
	CNode* pNewNode = NewNode(m_pNodeTail, NULL);

	pNewNode->data = newElement;

	if (m_pNodeTail != NULL)
		m_pNodeTail->pNext = pNewNode;
	else
		m_pNodeHead = pNewNode;

	m_pNodeTail = pNewNode;

	return (POSITION_) pNewNode;
}

void CMsgQueueUnit::RemoveAll()
{
    CNode* pNode = NULL;
    CNode* pNext = NULL;

    pNode = m_pNodeHead;
    while(pNode != NULL)
    {
        pNext = pNode->pNext;
        FreeNode(pNode);
        pNode = pNext;
    }

    m_nCount = 0;
    m_pNodeHead = m_pNodeTail = NULL;
}

void* CMsgQueueUnit::GetNext(POSITION_& rPosition) const // return *Position++
{ 
	CNode* pNode = (CNode*) rPosition;

	if(pNode == NULL) return NULL;

	rPosition = (POSITION_) pNode->pNext;

	return pNode->data; 
}
