#pragma once

#include "../max_common/stdafx.h"

struct FILE_LIGHT_TEXTURE_HEADER
{
	DWORD			dwTexturePlaneNum;
	DWORD			dwSurfaceWidth;
	DWORD			dwSurfaceHeight;
};

class CLightTexture
{
	DWORD			m_dwTexturePlaneNum;
	DWORD			m_dwSurfaceWidth;
	DWORD			m_dwSurfaceHeight;

	
	TVERTEX*		m_ptvTriList;
	TEXTURE_PLANE*	m_pTexturePlane;
	PATCH*			m_pPatch;
	DWORD			m_dwPatchNum;

public:

	TVERTEX*		GetTexVertexTriList()				{return m_ptvTriList;}
	void			SetTexVertexTriList(TVERTEX* ptv)	{m_ptvTriList = ptv;}
	TEXTURE_PLANE*	GetTexturePlane() {return m_pTexturePlane;}
	DWORD			GetTexturePlaneNum() {return m_dwTexturePlaneNum;}
	DWORD			GetSurfaceWidth()		{return m_dwSurfaceWidth;}
	DWORD			GetSurfaceHeight()		{return m_dwSurfaceHeight;}

	void			Initialize(DWORD dwTexPlaneNum,DWORD dwSurfaceWidth,DWORD dwSurfaceHeight,DWORD dwPixelFormat);
	BOOL			InsertTexturePlane(TEXTURE_PLANE* pTexPlane);
	
	DWORD			WriteFile(FILE* fp);

	CLightTexture();
	~CLightTexture();

};
