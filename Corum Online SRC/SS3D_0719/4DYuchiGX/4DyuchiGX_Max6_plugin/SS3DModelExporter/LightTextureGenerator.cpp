#include "LightTextureGenerator.h"
#include "../../4DyuchiGXGFunc/global.h"
#include "maxexp_global.h"


CLightTextureGenerator::CLightTextureGenerator()
{
	memset(this,0,sizeof(CLightTextureGenerator));
	m_dwDetailConst = 1;
}
CLightTexture* CLightTextureGenerator::CreateLightTexture(TVERTEX** OUT pptv,FACE* pFace,VECTOR3* pv3,DWORD dwFacesNum,CLightMapQuadTree* pTree)
{
	m_pTree = pTree;


	VECTOR3* pv3Point = new VECTOR3[dwFacesNum*3];
	TVERTEX* ptvTriList = *pptv = new TVERTEX[dwFacesNum*3];

	for (DWORD i=0; i<dwFacesNum; i++)
	{
		pv3Point[i*3+0] = pv3[pFace[i].a];
		pv3Point[i*3+1] = pv3[pFace[i].b];
		pv3Point[i*3+2] = pv3[pFace[i].c];
	}

	IsValidVector3(pv3Point,dwFacesNum*3);

	m_pUVPlane = new CUVPlane[dwFacesNum];

	CUVPlane*	puvplane = NULL;
	PLANE		plane;
	
	// 같은 평면에 속한 페이스들끼리 정렬.
	for (i=0; i<dwFacesNum; i++)
	{
		CalcPlaneEquation(&plane,pv3Point+i*3);

		if (puvplane = SearchPlane(&plane))
			goto lb_add_vertex;
	
		puvplane = AddPlane(&plane);

	lb_add_vertex:
		puvplane->AddVertex(pv3Point+i*3,ptvTriList+i*3);
	}

	// 정렬된 페이스들을 2D에 트랜스폼해서 2차원 충돌박스를 만든다.
	// 쿼드 트리에 넣어서 라이트텍스쳐에서의 위치를 얻어내고 텍셀 사이즈도 결정한다.
	DWORD		sx,sy,size;

	m_pTree->Clear();
	for (i=0; i<m_dwPlaneNum; i++)
	{
		if (!IsValidVector3(&m_pUVPlane[i].GetPlane()->v3Up,1))
			__asm int 3
		TransformToPlane(m_pUVPlane+i,i);
		if (!m_pTree->AllocBlock(&size,&sx,&sy,m_pUVPlane[i].GetTexturePlane()->dwTexelSize))
			MessageBox(NULL,"Fail to Alloc LightTexture","Error",MB_OK);
		
		memcpy(&m_pUVPlane[i].GetTexturePlane()->plane,m_pUVPlane[i].GetPlane(),sizeof(PLANE));
		m_pUVPlane[i].GetTexturePlane()->dwEntryPosX = sx;
		m_pUVPlane[i].GetTexturePlane()->dwEntryPosY = sy;
	}
	RECT	rect;
	m_pTree->GetMaxRect(&rect);
	
	m_dwSurfaceWidth = rect.right - rect.left;
	m_dwSurfaceHeight = rect.bottom - rect.top;


	// 2의 n승으로 리사이즈.
	size = 2;

lb_pow_width:
	size *= 2;
	if (size < m_dwSurfaceWidth)
		goto lb_pow_width;

	m_dwSurfaceWidth = size;


	size = 2;

lb_pow_height:
	size *= 2;
	if (size < m_dwSurfaceHeight)
		goto lb_pow_height;

	m_dwSurfaceHeight = size;

	VERTEX_LINK*		pCurVL;
	for (i=0; i<m_dwPlaneNum; i++)
	{
//		m_pUVPlane[i].GetTexturePlane()->dwEntryPosX += 1;
//		m_pUVPlane[i].GetTexturePlane()->dwEntryPosY += 1;
//		m_pUVPlane[i].GetTexturePlane()->dwTexelSize -= 2;
		

		pCurVL = m_pUVPlane[i].GetVLHead();
		while (pCurVL)
		{
			for (DWORD j=0; j<3; j++)
			{
				ResetTexCoord(
					&pCurVL->ptv[j].u,
					&pCurVL->ptv[j].v,
					m_pUVPlane[i].GetTexturePlane()->dwEntryPosX+1,
					m_pUVPlane[i].GetTexturePlane()->dwEntryPosY+1,
					m_pUVPlane[i].GetTexturePlane()->dwTexelSize-2,
					m_pUVPlane[i].GetTexturePlane()->dwTexelSize-2);
			}
			pCurVL = pCurVL->pNext;
		}
	}
	CLightTexture*	pTex = new CLightTexture;
	pTex->Initialize(m_dwPlaneNum,m_dwSurfaceWidth,m_dwSurfaceHeight,2);
	
	for (i=0; i<m_dwPlaneNum; i++)
	{
		pTex->InsertTexturePlane(m_pUVPlane[i].GetTexturePlane());
		
	}

	if (pv3Point)
	{
		delete [] pv3Point;
		pv3Point = NULL;

	}
	if (m_pUVPlane)
	{
		delete [] m_pUVPlane;
		m_pUVPlane = NULL;
	}

	return pTex;
}
CUVPlane* CLightTextureGenerator::SearchPlane(PLANE* pPlane)
{
	CUVPlane*	puvPlane = NULL;

	for (DWORD i=0; i<m_dwPlaneNum; i++)
	{
		if (ComparePlane(m_pUVPlane[i].GetPlane(),pPlane))
		{
			puvPlane = m_pUVPlane+i;
			goto lb_return;
		}
	}

lb_return:
	return puvPlane;
}
CUVPlane* CLightTextureGenerator::AddPlane(PLANE* pPlane)
{
	CUVPlane*	pUVPlane = m_pUVPlane+m_dwPlaneNum;
	
	pUVPlane->SetPlane(pPlane);
	m_dwPlaneNum++;

	return pUVPlane;

}


BOOL CLightTextureGenerator::TransformToPlane(CUVPlane* pUVPlane,DWORD dwPlaneIndex)
{
	DWORD		i;
	VECTOR3		zaxis;
	MATRIX4		rotMat;
	VECTOR3		axis;
	VERTEX_LINK*		pCurVL = pUVPlane->GetVLHead();
			

	zaxis.x = 0;
	zaxis.y = 0;
	zaxis.z = 1.0f;

	
	float max_y = -1000000.0f;
	float max_x = -1000000.0f;
	float min_x = 1000000.0f;
	float min_y = 1000000.0f;
	float	z;

	// 회전각
	float rad = CalcAngle(&zaxis,&pUVPlane->GetPlane()->v3Up);
	
	// 회전축
	CrossProduct(&axis,&zaxis,&pUVPlane->GetPlane()->v3Up);		
	if (!(axis.x == 0 && axis.y == 0 && axis.z == 0))
		Normalize(&axis,&axis);

	SetRotationMatrix(&rotMat,&axis,rad);

	// z축에 맞춰서 변환.이걸로 x,y만 유효 
	while (pCurVL)
	{
		if (pUVPlane->GetPlane()->v3Up.z != 1.0f)
		{
			TransformVector3_VPTR2(pCurVL->v3Out,pCurVL->pv3In,&rotMat,3);
		}
		else
		{
			memcpy(pCurVL->v3Out,pCurVL->pv3In,sizeof(VECTOR3)*3);
		}

		z = pCurVL->v3Out[0].z;
		
		for (i=0; i<3; i++)
		{
			if (pCurVL->v3Out[i].x < min_x)
				min_x = pCurVL->v3Out[i].x;

			if (pCurVL->v3Out[i].x > max_x)
				max_x = pCurVL->v3Out[i].x;

			if (pCurVL->v3Out[i].y < min_y)
				min_y = pCurVL->v3Out[i].y;

			if (pCurVL->v3Out[i].y > max_y)
				max_y = pCurVL->v3Out[i].y;
		}
		pCurVL = pCurVL->pNext;
	}	

	VECTOR3	v3[4];

	v3[0].x = min_x;
	v3[0].y = min_y;
	v3[0].z = z;

	v3[1].x = min_x;
	v3[1].y = max_y;
	v3[1].z = z;

	v3[2].x = max_x;
	v3[2].y = max_y;
	v3[2].z = z;

	v3[3].x = max_x;
	v3[3].y = min_y;
	v3[3].z = z;

	pCurVL = pUVPlane->GetVLHead();
	while (pCurVL)
	{
		for (i=0; i<3; i++)
		{
			pCurVL->ptv[i].u = (pCurVL->v3Out[i].x-min_x) / (max_x - min_x);
			pCurVL->ptv[i].v = (pCurVL->v3Out[i].y-min_y) / (max_y - min_y);
		}
		pCurVL = pCurVL->pNext;
	}
	// 텍스쳐플레인의  면적을 구하고 면적에 따라 텍셀 사이즈를 결정한다.
	
	

	float				fAreaSize;
	fAreaSize	= CalcTriAreaSize(v3)*2;
	if (!IsValidFloat(fAreaSize))
		__asm int 3

	if (fAreaSize < 160000.0f)		// 4m*4m
		pUVPlane->GetTexturePlane()->dwTexelSize = 8*m_dwDetailConst;
	else if (fAreaSize < 2560000.0f)	// 16m*16m
		pUVPlane->GetTexturePlane()->dwTexelSize = 16*m_dwDetailConst;
	else if (fAreaSize >= 2560000.0f)
		pUVPlane->GetTexturePlane()->dwTexelSize = 32*m_dwDetailConst;
	else 
	{
	
		__asm int 3
		MessageBox(NULL,"라이트텍스쳐생성플레인면적의 오류","Error",MB_OK);
		


	}


	// 3차원상의 텍스쳐 플레인 점 4개 세팅.
	if (pUVPlane->GetPlane()->v3Up.z != 1.0f)
	{
		SetRotationMatrix(&rotMat,&axis,(-1)*rad);
		TransformVector3_VPTR2(pUVPlane->GetTexturePlane()->v3Rect,v3,&rotMat,4);
	}
	else
	{
		memcpy(pUVPlane->GetTexturePlane()->v3Rect,v3,sizeof(VECTOR3)*4);

	}
	pUVPlane->GetTexturePlane()->v3Normal = pUVPlane->GetPlane()->v3Up;
	return TRUE;
}

void CLightTextureGenerator::ResetTexCoord(float* tu,float* tv,DWORD xPos,DWORD yPos,DWORD dwXSize,DWORD dwYSize)
{
	float	u,v;

	u = (((float)xPos + *tu * (float)dwXSize)) / (float)m_dwSurfaceWidth;
	v = (((float)yPos + *tv * (float)dwYSize)) / (float)m_dwSurfaceHeight;


	if (u > 1.0f || u < 0.0f || v > 1.0f || v < 0.0f)
	{
		__asm int 3
	}
	*tu = u;
	*tv = v;

	if (u*m_dwSurfaceWidth-0.5f <= 0 || u*m_dwSurfaceWidth-0.5f>= (m_dwSurfaceWidth-1)) 
	{
		__asm int 3
	}
	if (v*m_dwSurfaceHeight-0.5f <= 0 || v*m_dwSurfaceHeight-0.5f >= (m_dwSurfaceHeight-1))
	{
		__asm int 3
	}


//	*tu = (((float)(xPos+1) + *tu * (float)(dwXSize-2))) / (float)m_dwSurfaceWidth;
//	*tv = (((float)(yPos+1) + *tv * (float)(dwYSize-2))) / (float)m_dwSurfaceHeight;

	
}

CLightTextureGenerator::~CLightTextureGenerator()
{

}

/*
BOOL CLightTextureGenerator::BuildLightTexture()
{
	// 1. z축 (z=1,x=0,y=0)에 평행하도록 트랜스폼한다.
	// 2. 2차원에 투영되었다고 보고 min,max값을 구한다.
	// 3. 각 점들에 대한 uv좌표를 구한다.
	// 4. 트랜스폼했던 축과 (-각)만큼 min,max값을 트랜스폼시킨다.

	m_dwSurfaceWidth = m_dwPlaneNum * m_dwPixelNumPerTexel;
	if (m_dwSurfaceWidth > MAX_TEX_WIDTH)
		m_dwSurfaceWidth = MAX_TEX_WIDTH;
	
	m_dwSurfaceHeight = (m_dwPlaneNum * m_dwPixelNumPerTexel) / (MAX_TEX_WIDTH)*m_dwPixelNumPerTexel + m_dwPixelNumPerTexel;
	
	

	DWORD	size = 2;

lb_pow_width:
	size *= 2;
	if (size < m_dwSurfaceWidth)
		goto lb_pow_width;

	m_dwSurfaceWidth = size;


	size = 2;

lb_pow_height:
	size *= 2;
	if (size < m_dwSurfaceHeight)
		goto lb_pow_height;

	m_dwSurfaceHeight = size;




	for (DWORD i=0; i<m_dwPlaneNum; i++)
	{
		Calc3DRect(m_pUVPlane+i,i);
	}
	return TRUE;
}

BOOL CLightTextureGenerator::Calc3DRect(CUVPlane* pUVPlane,DWORD dwPlaneIndex)
{
	DWORD		i;
	VECTOR3		zaxis;
	MATRIX4		rotMat;
	VECTOR3		axis;
	DWORD		xpos,ypos;
	VERTEX_LINK*		pCurVL = pUVPlane->GetVLHead();
			

	memcpy(&pUVPlane->GetTexturePlane()->plane,pUVPlane->GetPlane(),sizeof(PLANE));
	pUVPlane->GetTexturePlane()->dwEntryPosX = xpos = (dwPlaneIndex*m_dwPixelNumPerTexel) % MAX_TEX_WIDTH;
	pUVPlane->GetTexturePlane()->dwEntryPosY = ypos = ((dwPlaneIndex*m_dwPixelNumPerTexel) / MAX_TEX_WIDTH)*m_dwPixelNumPerTexel;

	zaxis.x = 0;
	zaxis.y = 0;
	zaxis.z = 1.0f;

	
	float max_y = -1000000.0f;
	float max_x = -1000000.0f;
	float min_x = 1000000.0f;
	float min_y = 1000000.0f;
	float	z;

	// 회전각
	float rad = CalcAngle(&zaxis,&pUVPlane->GetPlane()->v3Up);
	
	// 회전축
	CrossProduct(&axis,&zaxis,&pUVPlane->GetPlane()->v3Up);		
	if (!(axis.x == 0 && axis.y == 0 && axis.z == 0))
		Normalize(&axis,&axis);

	SetRotationMatrix(&rotMat,&axis,rad);

	// z축에 맞춰서 변환.이걸로 x,y만 유효 
	while (pCurVL)
	{
		TransformVector3(pCurVL->v3Out,pCurVL->pv3In,&rotMat,3);
		z = pCurVL->v3Out[0].z;
		
		for (i=0; i<3; i++)
		{
			if (pCurVL->v3Out[i].x < min_x)
				min_x = pCurVL->v3Out[i].x;

			if (pCurVL->v3Out[i].x > max_x)
				max_x = pCurVL->v3Out[i].x;

			if (pCurVL->v3Out[i].y < min_y)
				min_y = pCurVL->v3Out[i].y;

			if (pCurVL->v3Out[i].y > max_y)
				max_y = pCurVL->v3Out[i].y;
		}
		pCurVL = pCurVL->pNext;
	}	

	VECTOR3		v3[4];
	memset(v3,0,sizeof(VECTOR3)*4);

	v3[0].x = min_x;
	v3[0].y = min_y;
	v3[0].z = z;

	v3[1].x = min_x;
	v3[1].y = max_y;
	v3[1].z = z;

	v3[2].x = max_x;
	v3[2].y = max_y;
	v3[2].z = z;

	v3[3].x = max_x;
	v3[3].y = min_y;
	v3[3].z = z;

	pCurVL = pUVPlane->GetVLHead();
	while (pCurVL)
	{
		for (i=0; i<3; i++)
		{
			pCurVL->ptv[i].u = (pCurVL->v3Out[i].x-min_x) / (max_x - min_x);
			pCurVL->ptv[i].v = (pCurVL->v3Out[i].y-min_y) / (max_y - min_y);
			ResetTexCoord(xpos,ypos,&pCurVL->ptv[i].u,&pCurVL->ptv[i].v);

		}
		pCurVL = pCurVL->pNext;
	}
	// 패치 생성
	// 생성된 패치별로 라이트를 계산


	SetRotationMatrix(&rotMat,&axis,(-1)*rad);
	TransformVector3(pUVPlane->GetTexturePlane()->v3Rect,v3,&rotMat,4);
	pUVPlane->GetTexturePlane()->v3Normal = pUVPlane->GetPlane()->v3Up;
	return TRUE;
}*/

/*
void CLightTextureGenerator::ResetTexCoord(DWORD xPos,DWORD yPos,float* tu,float* tv)
{
	*tu = (((float)xPos + *tu * (float)m_dwPixelNumPerTexel)) / (float)m_dwSurfaceWidth;
	*tv = (((float)yPos + *tv * (float)m_dwPixelNumPerTexel)) / (float)m_dwSurfaceHeight;
}*/