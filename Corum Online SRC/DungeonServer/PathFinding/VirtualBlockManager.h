#pragma once

#include "../stdafx.h"
#include "MemoryBlock.h"
#include "ControlStruct.h"
#include "DirectionManager.h"


struct VirtualBlock
{
	//	S	Data
	Direction_32_Info*	pDirection_32_Info;		//	���� �����ؾ��ϴ� ������. DirectionManager �̿��� ����� ���Ѵ�.

	//	R	Data
	DWORD				dwOffset_Map;			//	�� ������� �����ϴ� ���� �ʺ���ġ�� �ɼ�.
	DWORD				dwX_This;				//	�� ������� X ��ǥ��. 
	DWORD				dwY_This;				//	�� ������� Y ��ǥ��.

	//	R/W	Data
	DWORD				dwTarget;				//	��ǥ Ȯ�ο�.
	VirtualBlock*		pVirtualBlock_Origin;	//	������ �����Ǵ� ���� ������ �� ������.
	VirtualBlock*		pVirtualBlock_Parent;	//	�Ѵܰ� ���� �� ������.
	float				fCostFromStart;			//	��ŸƮ�� ��ġ�κ��� ������ ��ġ���� ���Ŀ� �� �Ÿ���.
	DWORD				dwCostTotal;			//	dwCostFromStart�� �޸���ƽ Cost�� �߰��� ��.
};

class VirtualBlockManager : public MemoryBlock
{
private:
	VirtualBlock*		m_pVirtualBlock;
	BlockManager		m_BlockManager_Map;
	DWORD				m_dwCenter_X;
	DWORD				m_dwCenter_Y;
	
	bool				m_bSetVirtualBlockSize;
	bool				m_bSetOffset_Map;

private:
	bool				CreateVirtualBlock			();

public:
	bool				CreateVirtualBlock			( DWORD dwMaxBlock_X, DWORD dwMaxBlock_Y, BlockManager* pBlockManager_Map );	//	X , Y ���� �ִ���Ǹ� ���ڷ� �޾�, ������� ������ �޸𸮸� �Ҵ��Ѵ�.
	bool				SetVirtualBlockSize			( DWORD dwMaxBlock_X, DWORD dwMaxBlcok_Y );
	bool				SetOffset_Map				( BlockManager* pBlockManager_Map);


	DWORD				GetCenter_X					()		{ return m_dwCenter_X; }			//	������� ���� X�� ����.
	DWORD				GetCenter_Y					()		{ return m_dwCenter_Y; }			//	������� ���� Y�� ����.

	VirtualBlockManager();
	~VirtualBlockManager();
};

