#pragma once
#include "linkedlist.h"
#include "../SS3D_0719/4DYUCHIGX/4DyuchiGXGFunc/global.h"



//CVoidList�� ��ӹ��� Ŭ������ ���� Data�� ���� ���� ����..  
//���Ḹ ���ش�...     �Ǵٸ� �������� Node�� Ǯ�� ���� 

class COnlyList : public CVoidList
{
	STMPOOL_HANDLE			m_pNodePool;
public:
	
	CNode*	NewNode(CNode* pPrev, CNode* pNext);
	void	FreeNode(CNode* pNode);

	COnlyList(DWORD dwMaxNodePoolNum);
	~COnlyList();
};

