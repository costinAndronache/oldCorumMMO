#pragma once

#include "../4DyuchiGRX_common/typedef.h"
#include "d3dtexture.h"
#include "../4DyuchiGXGFunc/global.h"

#include "CTexture.h"


class CD3DResourceManager;
class CoD3DDevice;




class CTextureManager
{
	CoD3DDevice*					m_pRenderer;
	CD3DResourceManager*			m_pResourceManager;
	
	VBHASH_HANDLE					m_pFileNameHash;
	DWORD							m_dwMaxTextureNum;
	DWORD							m_dwTextureNum;
	DWORD							m_dwFileTextureNum;
	

	DWORD							m_dwBytesPerPixelTGA;
	DWORD							m_dwBytesPerPixelTIF;
	D3DFORMAT						m_d3dfmtTGA;
	D3DFORMAT						m_d3dfmtTIF;
	CTexture						m_DefaultTexture;
	CBaseTextureContainer*			m_pDefaulTexContainer;
	
	

	CTextureContainer*				AddTexture(char* szFileName,DWORD dwUsage,D3DPOOL pool,DWORD dwWidthHeight,BOOL bUseMipmap,BOOL bCompress);
	
	void							ReleaseAllTextures();
	void							ResourceCheck();
public:

	CD3DResourceManager*			GetD3DResourceManager()	{return m_pResourceManager;}

	void							SetDefaultTexture(LPDIRECT3DDEVICE8 pDevice,DWORD dwTexStageIndex) 
	{
		m_DefaultTexture.SetTexture(pDevice,dwTexStageIndex);
	}
	void							SetBytesPerPixel(DWORD dwBPS);
	DWORD							GetTextureNum()					{return m_dwTextureNum;}
	DWORD							GetFileTextureNum()				{return m_dwFileTextureNum;}
	
	BOOL							Initialize(CoD3DDevice* pRenderer,CD3DResourceManager* pResourceManager,DWORD dwMaxTexNum,DWORD dwMaxBucketNum);
	BOOL							AllocTexture(CTexture* pTextureHandle,char* szFileName,DWORD dwWidthHeight,BOOL bUseMipmap,BOOL bCompress);
	void							AllocDefaultTexture(CTexture* pTexHandle);
	BOOL							AllocRawTexture(CTexture* pTextureHandle,UINT Width,UINT Height,UINT  Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool);
	void							FreeTexture(CBaseTextureContainer* pTexContainer);

	
	CTextureManager();
	~CTextureManager();
	
	

};
