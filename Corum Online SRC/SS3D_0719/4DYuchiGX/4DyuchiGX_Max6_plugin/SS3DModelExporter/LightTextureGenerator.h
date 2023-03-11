#pragma once

#include "../max_common/stdafx.h"

#include "uv_plane.h"
#include "LightTexture.h"
#include "LightMapQuadTree.h"

class CLightTextureGenerator
{
	DWORD				m_dwPlaneNum;
	DWORD				m_dwUTexelNum;
	DWORD				m_dwVTexelNum;
	DWORD				m_dwSurfaceWidth;
	DWORD				m_dwSurfaceHeight;
	DWORD				m_dwFacesNum;
	
	CUVPlane*			m_pUVPlane;
	CLightMapQuadTree*	m_pTree;
	DWORD				m_dwDetailConst;

	BOOL				TransformToPlane(CUVPlane* pUVPlane,DWORD dwPlaneIndex);
	void				ResetTexCoord(float* tu,float* tv,DWORD xPos,DWORD yPos,DWORD dwXSize,DWORD dwYSize);
	CUVPlane*			AddPlane(PLANE* pPlane);
	CUVPlane*			SearchPlane(PLANE* pPlane);
public:
	CLightTexture*		CreateLightTexture(TVERTEX** OUT pptv,FACE* pFace,VECTOR3* pv3,DWORD dwFacesNum,CLightMapQuadTree* pTree);
	void				SetLightMapDetail(DWORD dwDetailConst) { m_dwDetailConst = dwDetailConst;}

	
	

	CLightTextureGenerator();
	~CLightTextureGenerator();

};
