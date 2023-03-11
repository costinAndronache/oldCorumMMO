#ifndef _LIGHTMAPQUADTREE_H
#define _LIGHTMAPQUADTREE_H

#include "../max_common/stdafx.h"

struct LM_NODE
{
	DWORD		sx;
	DWORD		sy;
	DWORD		dwSize;
	DWORD		dwSizePow;
	DWORD		dwSizeBackup;
	
	LM_NODE*	pParent;
	LM_NODE*	pNext;
	LM_NODE*	pChild[4];
};

class CLightMapQuadTree
{
	DWORD		m_dwMaxSize;
	DWORD		m_dwMinSize;
	DWORD		m_dwMaxNodeNum;
	DWORD		m_dwNodeNum;
	LM_NODE*	m_pRootNode;
	LM_NODE*	m_pCurrentNode;
	void		FreeNode(LM_NODE* pNode);
	LM_NODE*	AllocNode();
	DWORD		BuildRecursive(LM_NODE* pNode,DWORD dwSize);
	void		DeleteNode(LM_NODE* pNode);
public:
	void		GetMaxRect(RECT* pRect);
	BOOL		Initialize(DWORD dwMaxSize,DWORD dwMinSize);
	void		Clear();
	
	
	BOOL		AllocBlock(DWORD* pdwSize,DWORD* psx,DWORD* psy,DWORD dwSize);
	void		Release();
};

#endif
