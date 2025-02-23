#pragma once

#include "GXMapObject.h"
#include "./4DyuchiGRX_myself97/MStaticObjectTree.h"

class CGXMap;

class CGXStaticMapObject : public CGXMapObject
{
	I3DStaticModel*		m_pModel;
	DWORD				m_dwObjectIndex;

	MStaticObjectTree*	m_pTree;

public:
	void				SetRenderFlag(DWORD	dwFlag)	{m_dwRenderFlag = dwFlag;}
	BOOL				Initialize(CoExecutive* pExecutive,CGXMap* pMap,DWORD dwIndex,DWORD dwObjIndex);

	void				BuildBSPTree();
	void				DeleteBSPTree();

	// 외부에 그냥 트리를 내놓는다.
	MStaticObjectTree*				GetTree(void){return m_pTree;}

	~CGXStaticMapObject();

	COLLISION_MESH_OBJECT_DESC* 	GetCollisionMeshObjectDesc();
	DWORD							OnFrame(I4DyuchiGXExecutive* pExecutive,DWORD msg,int arg1,int arg2);
	DWORD							GetObjectIndexInModel();
	BOOL							Render();
	BOOL							IsCollisionWithScreenCoord(VECTOR3* pv3IntersectPoint,DWORD* pdwModelIndex,DWORD* pdwObjIndex,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag);

	DWORD							CreateIVertexList(IVERTEX** ppVertex);
	void							ReleaseIVertexList(IVERTEX*	pVertex);
	BOOL							ShadeLightMapObject(VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pLight,SHADE_FUNC pFunc);
	
};
