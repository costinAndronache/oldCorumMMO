#pragma once
#include "Voidlist.h"
#ifdef SS3D_TEST
#include "./../SS3D_0719/4dyuchigx/4DyuchiGXGFunc/global.h"
#else
#include "./../4DyuchiGXGFunc/global.h"
#endif



//CVoidList�� ��ӹ��� Ŭ������ ���� Data�� ���� ���� ����..  
//���Ḹ ���ش�...     �Ǵٸ� �������� Node�� Ǯ�� ���� 

class COnlyList : public CVoidList
{
	STMPOOL_HANDLE			m_pNodePool;
public:
	
	CNode*	NewNode(CNode* pPrev, CNode* pNext);
	void	FreeNode(CNode* pNode);

	void	RemoveAllData();

	void	(*DeleteData)();
	
	COnlyList(DWORD dwMaxNodePoolNum, void (*DeleteFun)() = NULL);
	~COnlyList();
};

