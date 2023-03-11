#include "LightTexture.h"
#include "../../4DyuchiGXGFunc/global.h"

CLightTexture::CLightTexture()
{
	memset(this,0,sizeof(CLightTexture));
	
}

void CLightTexture::Initialize(DWORD dwTexPlaneNum,DWORD dwSurfaceWidth,DWORD dwSurfaceHeight,DWORD dwPixelFormat)
{
	m_dwSurfaceWidth = dwSurfaceWidth;
	m_dwSurfaceHeight = dwSurfaceHeight;
	m_pTexturePlane = new TEXTURE_PLANE[dwTexPlaneNum];
}
BOOL CLightTexture::InsertTexturePlane(TEXTURE_PLANE* pTexPlane)
{
	memcpy(m_pTexturePlane+m_dwTexturePlaneNum,pTexPlane,sizeof(TEXTURE_PLANE));
	m_dwTexturePlaneNum++;

	return TRUE;
}



DWORD CLightTexture::WriteFile(FILE* fp)
{
	FILE_LIGHT_TEXTURE_HEADER* pHeader = (FILE_LIGHT_TEXTURE_HEADER*)&this->m_dwTexturePlaneNum;

	DWORD	oldPos = ftell(fp);
	
	fwrite(pHeader,sizeof(FILE_LIGHT_TEXTURE_HEADER),1,fp);
	fwrite(m_pTexturePlane,sizeof(TEXTURE_PLANE),m_dwTexturePlaneNum,fp);
	

	return (ftell(fp)-oldPos);
}

CLightTexture::~CLightTexture()
{
	if (m_pTexturePlane)
	{
		delete [] m_pTexturePlane;
		m_pTexturePlane = NULL;
	}
}
