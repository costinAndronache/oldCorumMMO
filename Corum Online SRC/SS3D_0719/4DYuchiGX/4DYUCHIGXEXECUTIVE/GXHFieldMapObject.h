#ifndef GXHFIELD_MAP_OBJECT_H
#define GXHFIELD_MAP_OBJECT_H

#include "GXMapObject.h"

class CGXMap;

class CGXHFieldMapObject : public CGXMapObject
{
	IHeightField*		m_pHField;
	DWORD				m_dwObjectIndex;

public:
	void				SetRenderFlag(DWORD	dwFlag)	{m_dwRenderFlag = dwFlag;}
	BOOL				Initialize(CoExecutive* pExecutive,CGXMap* pMap,DWORD dwIndex,DWORD dwObjIndex);

	COLLISION_MESH_OBJECT_DESC* 	GetCollisionMeshObjectDesc();
	DWORD							OnFrame(I4DyuchiGXExecutive* pExecutive,DWORD msg,int arg1,int arg2);
	DWORD							GetObjectIndexInModel();
	BOOL							Render();
	BOOL							IsCollisionWithScreenCoord(VECTOR3* pv3IntersectPoint,DWORD* pdwModelIndex,DWORD* pdwObjIndex,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag);
		
	DWORD							CreateIVertexList(IVERTEX** ppVertex);
	void							ReleaseIVertexList(IVERTEX*	pVertex);
	BOOL							ShadeLightMapObject(VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pLight,SHADE_FUNC pFunc);
};

#endif
