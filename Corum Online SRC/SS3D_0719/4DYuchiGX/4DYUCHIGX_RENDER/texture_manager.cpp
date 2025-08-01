#include "StdAfx.h"
#include "texture_manager.h"
#include "d3d_helper.h"
#include "image.h"
#include "../4DyuchiGXGFunc/global.h"
#include "D3DResourceManager.h"
#include "CoD3DDevice.h"
#include "global_variable.h"


CTextureManager::CTextureManager()
{
	m_pRenderer = NULL;
	m_pResourceManager = NULL;
	m_pFileNameHash = NULL;
	m_pDefaulTexContainer = NULL;
//	m_pTexContainerSortTable = NULL;
//	m_dwCommitedTextureNum = 0;
//	m_dwRetainTextureNum = 0;
	m_dwMaxTextureNum = NULL;

	m_dwTextureNum = 0;


	m_dwBytesPerPixelTGA = 0;
	m_dwBytesPerPixelTIF = 0;
}
BOOL CTextureManager::Initialize(CoD3DDevice* pRenderer,CD3DResourceManager* pResourceManager,DWORD dwMaxTexNum,DWORD dwMaxBucketNum)
{
	m_pRenderer = pRenderer;
	m_dwMaxTextureNum = dwMaxTexNum;
	m_pFileNameHash = VBHCreate();
	VBHInitialize(m_pFileNameHash,dwMaxBucketNum,MAX_NAME_LEN,dwMaxTexNum);
	m_pResourceManager = pResourceManager;


	
	AllocDefaultTexture(&m_DefaultTexture);
//	m_pTexContainerSortTable = new SORT_ITEM_INT[m_dwMaxTextureNum];
//	memset(m_pTexContainerSortTable,0,sizeof(SORT_ITEM_INT)*m_dwMaxTextureNum);
//	m_dwRetainTextureNum = 320;

	return TRUE;
}
void CTextureManager::AllocDefaultTexture(CTexture* pTexHandle)
{

	BOOL	bResult = FALSE;
	IDirect3DTexture8*	pTex;
	HRESULT		hr;

	if (!m_pDefaulTexContainer)
	{

		hr = m_pResourceManager->CreateTexture(
			1,
			1,
			1,
			0,
			D3DFMT_R5G6B5,
			D3DPOOL_MANAGED,
			&pTex);

		if (hr != D3D_OK)
		{
			__asm int 3
		}
		WORD	wBits = 0xffff;
		
		m_pResourceManager->SetSurfaceWithBitmap(0,pTex,(char*)&wBits,1,1,D3DFMT_R5G6B5);
		m_pDefaulTexContainer = new CBaseTextureContainer;
		memset(m_pDefaulTexContainer,0,sizeof(CBaseTextureContainer));

		m_pDefaulTexContainer->m_pTextureManager = this;
		m_pDefaulTexContainer->m_pD3DTexture = pTex;


	}

	pTexHandle->SetTextureInfo(m_pDefaulTexContainer);
	
}
void CTextureManager::SetBytesPerPixel(DWORD dwBPS)
{
	m_dwBytesPerPixelTGA = 2;
	m_dwBytesPerPixelTIF = dwBPS;

	if (dwBPS == 4)
	{
		m_d3dfmtTIF = D3DFMT_A8R8G8B8;
		m_d3dfmtTGA = D3DFMT_R5G6B5;

	}
	else if (dwBPS == 2)
	{
		m_d3dfmtTGA = D3DFMT_R5G6B5;
		m_d3dfmtTIF = D3DFMT_A4R4G4B4;
	}
}

BOOL CTextureManager::AllocRawTexture(CTexture* pTextureHandle,UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool)
{
	BOOL	bResult = FALSE;
	IDirect3DTexture8*	pTex;
	HRESULT		hr;

	

	hr = m_pResourceManager->CreateTexture(
		Width,
		Height,
		Levels,
		Usage,
		Format,
		Pool,
		&pTex);
	if (hr != D3D_OK)
		goto lb_return;

	

	CBaseTextureContainer* pTexContainer;
	pTexContainer = new CBaseTextureContainer;
	
	IMAGE_HEADER	imgHeader;
	imgHeader.dwWidth = Width;
	imgHeader.dwHeight = Height;
	

	pTexContainer->SetTextureImage(pTex,&imgHeader);
	pTextureHandle->SetTextureInfo(pTexContainer);
	pTexContainer->m_pTextureManager = this;


	m_dwTextureNum++;




	bResult = TRUE;
lb_return:
	return bResult;


}
BOOL CTextureManager::AllocTexture(CTexture* pTextureHandle,char* szFileName,DWORD dwWidthHeight,BOOL bUseMipmap,BOOL bCompress)
{

	char	txt[4] = {0,0,0,0};
//	D3DPOOL		pool = D3DPOOL_DEFAULT;
	D3DPOOL		pool = D3DPOOL_MANAGED;
	
	

	CTextureContainer* pResult = NULL;

	DWORD			dwIndexOut;
	BOOL			bResult = FALSE;

//	if (!szFileName)
//		szFileName = "psteam.tga";
		
	if (0==lstrlen(szFileName))
		goto lb_return;
	

	char		szSearchFileName[256];
	GetNameRemovePath(szSearchFileName,szFileName);
	
	DWORD			dwSearchNameLen;
	dwSearchNameLen = lstrlen(szSearchFileName);
	CharToSmallASCII(szSearchFileName,szSearchFileName,dwSearchNameLen);

	if (VBHSelect(m_pFileNameHash,&dwIndexOut,1,szSearchFileName,dwSearchNameLen))
	{
		pResult = (CTextureContainer*)dwIndexOut;
		goto lb_set_result;
	}

	pResult = AddTexture(szFileName,0,pool,dwWidthHeight,bUseMipmap,bCompress);
	
lb_set_result:
	if (!pResult)
	{


		bResult = FALSE;
		goto lb_return;
	}
	pTextureHandle->SetTextureInfo(pResult);

	bResult = TRUE;

lb_return:
	return bResult;

}
void CTextureManager::FreeTexture(CBaseTextureContainer* pTexContainer)
{
	if (pTexContainer->IsFromFile())
	{
		VBHDelete(m_pFileNameHash,pTexContainer->m_pSearchHandle);
	}
/*
	m_dwCommitedTextureNum--;

	m_pTexContainerSortTable[pTexContainer->m_dwIndexInSortTable] = m_pTexContainerSortTable[m_dwTextureNum];
	((CBaseTextureContainer*)m_pTexContainerSortTable[m_dwTextureNum].pItem)->m_dwIndexInSortTable = pTexContainer->m_dwIndexInSortTable;
	

	m_pTexContainerSortTable[m_dwTextureNum].pItem = NULL;
*/	
	m_dwTextureNum--;

	
}

CTextureContainer* CTextureManager::AddTexture(char* szFileName,DWORD dwUsage,D3DPOOL pool,DWORD dwWidthHeight,BOOL bUseMipmap,BOOL bCompress)
{
	DWORD	dwTexFlag = 0;
	DWORD	dwWidth;
	DWORD	dwHeight;


	LPDIRECT3DTEXTURE8		pTex = NULL;
	IMAGE_HEADER			imgHeader;;

	char	szTempFileName[_MAX_PATH];
	memset(szTempFileName,0,_MAX_PATH);
	memcpy(szTempFileName,szFileName,lstrlen(szFileName));
	szFileName = szTempFileName;

	memset(&imgHeader,0,sizeof(IMAGE_HEADER));


	CTextureContainer*	pTexContainer = NULL;
	CImage	image;
	

	DWORD	len = lstrlen(szFileName);
	
	
	if (!lstrcmp(".tga",szFileName+lstrlen(szFileName)-4) || !lstrcmp(".TGA",szFileName+lstrlen(szFileName)-4))
	{
lb_load_tga:
		if (m_pResourceManager->CreateTextureWithDDS(&pTex,szFileName))
		{
			D3DSURFACE_DESC		ddsc;
			memset(&ddsc,0,sizeof(ddsc));
			
			pTex->GetLevelDesc(0,&ddsc);
							
			imgHeader.dwWidth = ddsc.Width;
			imgHeader.dwHeight = ddsc.Height;
			imgHeader.dwPitch = 0xffffffff;
			imgHeader.dwBPS = 0xffffffff;

			
			
			goto lb_success;

		}
		if (image.LoadTga(szFileName,m_dwBytesPerPixelTGA))
		{
			if (dwWidthHeight)
			{
				dwWidth = dwWidthHeight;
				dwHeight = dwWidthHeight;
			}
			else 
			{
				Set2PowValueLess(&dwWidth,image.GetWidth());
				Set2PowValueLess(&dwHeight,image.GetHeight());
			}
			image.Resize(dwWidth,dwHeight,m_dwBytesPerPixelTGA);
			
			if (!m_pResourceManager->CreateTextureWithBitmap(
				&pTex,
				dwUsage,
				m_d3dfmtTGA,
				pool,
				(char*)image.GetRawImage(),
				dwWidth,
				dwHeight,
				bUseMipmap,
				bCompress))
				goto lb_return;

			imgHeader.dwWidth = dwWidth;
			imgHeader.dwHeight = dwHeight;
			imgHeader.dwBPS = m_dwBytesPerPixelTGA;
		}
		else 
			goto lb_file_not_found;
	}
	else if (!lstrcmp(".tif",szFileName+lstrlen(szFileName)-4) || !lstrcmp(".TIF",szFileName+lstrlen(szFileName)-4))
	{
		if (image.LoadTIF(szFileName,m_dwBytesPerPixelTIF))
		{	
			if (dwWidthHeight)
			{
				dwWidth = dwWidthHeight;
				dwHeight = dwWidthHeight;
			}
			else {
				/*
				* Pre-2006 or so graphic cards required texture sizes to be powers of 2 for optimization purposes.
				*/
				
				//Set2PowValueLess(&dwWidth,image.GetWidth());
				//Set2PowValueLess(&dwHeight,image.GetHeight());

				// But for nowadays I'm just gonna skip it
				dwWidth = image.GetWidth();
				dwHeight = image.GetHeight();
			}
			if (!m_pResourceManager->CreateTextureWithBitmap(
				&pTex,
				dwUsage,
				m_d3dfmtTIF,
				pool,
				(char*)image.GetRawImage(),
				dwWidth,
				dwHeight,
				bUseMipmap,
				bCompress))
				goto lb_return;
			
			imgHeader.dwWidth = dwWidth;
			imgHeader.dwHeight = dwHeight;
			imgHeader.dwBPS = m_dwBytesPerPixelTIF;
			

			dwTexFlag |= TEX_FLAG_ALPHA_OPASITY;
		}
		else
			goto lb_file_not_found;
			
	}
	else 
	{
		lstrcpy(szFileName+len-4,".tga");
		goto lb_load_tga;
	}
lb_success:
	
	char		szSearchFileName[256];
	GetNameRemovePath(szSearchFileName,szFileName);
	
	DWORD			dwSearchNameLen;
	dwSearchNameLen = lstrlen(szSearchFileName);
	CharToSmallASCII(szSearchFileName,szSearchFileName,dwSearchNameLen);

	pTexContainer = new CTextureContainer;
	pTexContainer->SetFileName(szSearchFileName);
	
	pTexContainer->SetTextureImage(pTex,&imgHeader);

	

	
	pTexContainer->m_dwTexFlag |= dwTexFlag;
	pTexContainer->m_pSearchHandle = VBHInsert(m_pFileNameHash,(DWORD)pTexContainer,szSearchFileName,dwSearchNameLen);
	pTexContainer->m_pTextureManager = this;

//	m_pTexContainerSortTable[m_dwTextureNum].pItem = (void*)pTexContainer;
//	pTexContainer->m_dwIndexInSortTable = m_dwTextureNum;
//	m_dwCommitedTextureNum++;
//	if (m_dwCommitedTextureNum > m_dwRetainTextureNum)
//		ArrangeTexture();

	m_dwTextureNum++;
	goto lb_return;

lb_file_not_found:
#ifdef _DEBUG

	char	txt[512];
	memset(txt,0,512);
	wsprintf(txt,"Texture File Not Found : %s \n",szFileName);
	OutputDebugString(txt);
#endif
	// FILE_NOT_FOUND /////////////////////////////////////////////////////////
	DWORD	dwAddr;
	GetEIP(&dwAddr);
	g_pErrorHandleFunc(ERROR_TYPE_FILE_NOT_FOUND,1,(void*)dwAddr,szFileName);
	///////////////////////////////////////////////////////////////////////////
	
lb_return:

	return pTexContainer;
	
}
/*

BOOL CTextureManager::UpdateTexture(CBaseTextureContainer* pTexContainer)
{
	if (m_dwCommitedTextureNum > m_dwRetainTextureNum)
		ArrangeTexture();

	pTexContainer->SwapToDriverMemory();
	m_dwCommitedTextureNum++;

	return TRUE;
}
void CTextureManager::CheckUse(DWORD dwSortItemIndex)
{
	g_dwTexRefCount++;
	m_pTexContainerSortTable[dwSortItemIndex].iCmp = g_dwTexRefCount;
	
}
void CTextureManager::ArrangeTexture()
{
	QSortIntASC(m_pTexContainerSortTable,m_dwTextureNum);
	
	CBaseTextureContainer*	pTexContainer;
	for (DWORD i=0; i<m_dwTextureNum; i++)
	{
		if (!m_dwCommitedTextureNum)
			break;

		if (m_dwCommitedTextureNum < m_dwRetainTextureNum)
			break;

		pTexContainer = (CBaseTextureContainer*)m_pTexContainerSortTable[i].pItem;
		if (!pTexContainer->m_dwPageFlag)
		{
			
			pTexContainer->SwapToSystemMemory();
			m_dwCommitedTextureNum--;
		}

	}
}*/
void CTextureManager::ResourceCheck()
{
#ifdef _DEBUG
	CTextureContainer*	pTexList[2048];
	memset(pTexList,0,sizeof(pTexList));

	DWORD	dwNum = VBHGetAllItem(m_pFileNameHash,(DWORD*)pTexList,2048);
	if (dwNum)
	{
		char	txt[512];
		wsprintf(txt,"\n\n\n%s\n","Texture Resource Leak!! Look the VC++'s Output Window");
		OutputDebugString(txt);
		for (DWORD i=0; i<dwNum; i++)
		{
			memset(txt,0,sizeof(txt));
			wsprintf(txt,"Resource Leak!!!!! --- Texture %s is not Freed \n",pTexList[i]->m_szFileName);
			OutputDebugString(txt);
		}
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_RESOURCE_LEAK,0,(void*)dwAddr,"CTextureManager::ResourceCheck() ResourceLeak!!!");
	}
#endif 
}
CTextureManager::~CTextureManager()
{
//	if (m_pTexContainerSortTable)
//	{
//		delete [] m_pTexContainerSortTable;
//		m_pTexContainerSortTable = NULL;
//	}
	m_DefaultTexture.Release();
	ResourceCheck();
	if (m_pFileNameHash)
	{
		VBHRelease(m_pFileNameHash);
		m_pFileNameHash = NULL;
	}
}
