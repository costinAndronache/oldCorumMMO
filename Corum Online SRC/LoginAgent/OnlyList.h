#pragma once
#include "linkedlist.h"
#include <global2.h>



//CVoidList를 상속받은 클래스로 실제 Data의 삭제 과정 없이..  
//연결만 해준다...     또다른 차이점은 Node를 풀로 생성 

class COnlyList : public CVoidList
{
	STMPOOL_HANDLE			m_pNodePool;
public:
	
	CNode*	NewNode(CNode* pPrev, CNode* pNext);
	void	FreeNode(CNode* pNode);

	COnlyList(DWORD dwMaxNodePoolNum);
	~COnlyList();
};

