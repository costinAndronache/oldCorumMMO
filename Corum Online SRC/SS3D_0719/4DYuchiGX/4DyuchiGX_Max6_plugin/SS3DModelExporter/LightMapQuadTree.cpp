#include "LightMapQuadTree.h"

BOOL CLightMapQuadTree::Initialize(DWORD dwMaxSize,DWORD dwMinSize)
{
	m_dwNodeNum = 0;
	m_dwMaxSize = dwMaxSize;
	m_dwMinSize = dwMinSize;
	m_pCurrentNode = NULL;

	m_pRootNode = AllocNode();
	m_pRootNode->dwSize = dwMaxSize;
	m_pRootNode->dwSizePow = dwMaxSize*dwMaxSize;
	m_pRootNode->dwSizeBackup = m_pRootNode->dwSize;
	m_dwMaxNodeNum = 1 + BuildRecursive(m_pRootNode,dwMaxSize);

	return TRUE;
}
LM_NODE* CLightMapQuadTree::AllocNode()
{
	LM_NODE*	pNode = new LM_NODE;
	memset(pNode,0,sizeof(LM_NODE));
	
	if (m_pCurrentNode)
		m_pCurrentNode->pNext = pNode;

	m_pCurrentNode = pNode;

	m_dwNodeNum++;
	
	

	return pNode;
}
void CLightMapQuadTree::FreeNode(LM_NODE* pNode)
{
	delete pNode;
}

void CLightMapQuadTree::GetMaxRect(RECT* pRect)
{
	DWORD min_x,min_y,max_x,max_y;
	
	max_x = 0;
	max_y = 0;

	min_x = 0xffffffff;
	min_y = 0xffffffff;
	
	DWORD	width = 0;
	DWORD	height = 0;
	DWORD	temp;

	LM_NODE*	pNode = m_pRootNode;
	while(pNode)
	{
		if (pNode->dwSizePow)
			goto lb_next;

		if (pNode->sx <= min_x)
			min_x = pNode->sx;

		if (pNode->sy <= min_y)
			min_y = pNode->sy;

		temp =  pNode->sx + pNode->dwSize;
		if (temp > max_x)
			max_x = temp;
		
		temp =  pNode->sy + pNode->dwSize;
		if (temp > max_y)
			max_y = temp;

lb_next:
		pNode = pNode->pNext;
	}
	pRect->left = min_x;
	pRect->top = min_y;
	pRect->right = max_x;
	pRect->bottom = max_y;
}
DWORD CLightMapQuadTree::BuildRecursive(LM_NODE* pNode,DWORD dwSize)
{
	DWORD		dwNodeNum = 0;
	DWORD		i;

	if (dwSize <= m_dwMinSize)
		goto lb_return;

	dwSize /= 2;
	for (i=0; i<4; i++)
	{
		pNode->pChild[i] = AllocNode();
		pNode->pChild[i]->pParent = pNode;
		pNode->pChild[i]->dwSize = dwSize;
		pNode->pChild[i]->dwSizeBackup = pNode->pChild[i]->dwSize;
		pNode->pChild[i]->dwSizePow = dwSize*dwSize;
	}	
	pNode->pChild[0]->sx = pNode->sx;
	pNode->pChild[0]->sy = pNode->sy;
	
	pNode->pChild[1]->sx = pNode->sx;
	pNode->pChild[1]->sy = pNode->sy + dwSize;

	pNode->pChild[2]->sx = pNode->sx + dwSize;
	pNode->pChild[2]->sy = pNode->sy + dwSize;

	pNode->pChild[3]->sx = pNode->sx + dwSize;
	pNode->pChild[3]->sy = pNode->sy;

	dwNodeNum = 4;

	for (i=0; i<4; i++)
	{
		dwNodeNum += BuildRecursive(pNode->pChild[i],dwSize);
	}

lb_return:
	return dwNodeNum;

}
BOOL CLightMapQuadTree::AllocBlock(DWORD* pdwSize,DWORD* psx,DWORD* psy,DWORD dwSize)
{
	
	LM_NODE*	pNode = m_pRootNode;
	LM_NODE*	pParent = NULL;
	DWORD		i;
	BOOL		bResult = FALSE;
	DWORD		dwSizePow = dwSize*dwSize;

	if (pNode->dwSizePow < dwSizePow)
		goto lb_return;

lb_search_child:

	for (i=0; i<4; i++)
	{
		if (!pNode->pChild[i]->dwSizePow)
			continue;

		if (pNode->pChild[i]->dwSizePow >= dwSizePow)
		{
			pParent = pNode;
			pNode = pNode->pChild[i];
			if (pNode->pChild[0])
				goto lb_search_child;
			else 
				goto lb_alloc;
		}
	}
	goto lb_alloc;

	

lb_alloc:
	pParent = pNode->pParent;
	while (pParent)
	{
		pParent->dwSizePow -= pNode->dwSizePow;
		pParent = pParent->pParent;
	}

	pNode->dwSizePow = 0;
	
	*psx = pNode->sx;
	*psy = pNode->sy;
	*pdwSize = pNode->dwSize;

	bResult = TRUE;

lb_return:
	return bResult;
}
void CLightMapQuadTree::DeleteNode(LM_NODE* pNode)
{
	if (!pNode)
		return;

	for (DWORD i=0; i<4; i++)
	{
		if (pNode->pChild[i])
		{
			DeleteNode(pNode->pChild[i]);
		}
	}
	FreeNode(pNode);
}


void CLightMapQuadTree::Release()
{
	DeleteNode(m_pRootNode);
}
void CLightMapQuadTree::Clear()
{
	LM_NODE*	pNode = m_pRootNode;

	while(pNode)
	{
		pNode->dwSize = pNode->dwSizeBackup;
		pNode->dwSizePow = pNode->dwSize*pNode->dwSize;
		pNode = pNode->pNext;
	}

}
