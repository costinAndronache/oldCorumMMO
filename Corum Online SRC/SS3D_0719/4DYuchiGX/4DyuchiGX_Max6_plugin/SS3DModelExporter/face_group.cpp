#include "face_group.h"
#include "LightTextureGenerator.h"

CFaceGroup::CFaceGroup()
{
	memset(this,0,sizeof(CFaceGroup));

}
WORD* CFaceGroup::CreateFaceList(DWORD dwFacesNum)
{
	m_pFace = new WORD[dwFacesNum*3];
	m_ptLightUV1 = new TVERTEX[dwFacesNum*3];
	m_ptLightUV2 = new TVERTEX[dwFacesNum*3];
	m_dwMaxFacesNum = dwFacesNum;
	
	
	return m_pFace;
}

BOOL CFaceGroup::SetFace(DWORD dwIndex,WORD a,WORD b,WORD c)
{
	BOOL	bResult = FALSE;
	
	if (dwIndex >= m_dwFacesNum)
		goto lb_return;

	m_pFace[dwIndex*3+0] = a;
	m_pFace[dwIndex*3+1] = b;
	m_pFace[dwIndex*3+2] = c;

lb_return:
	return bResult;

}

BOOL CFaceGroup::InsertFace(WORD a,WORD b,WORD c)
{
	BOOL	bResult = FALSE;
	
	if (m_dwFacesNum >= m_dwMaxFacesNum)
		goto lb_return;

	m_pFace[m_dwFacesNum*3+0] = a;
	m_pFace[m_dwFacesNum*3+1] = b;
	m_pFace[m_dwFacesNum*3+2] = c;
	m_dwFacesNum++;

lb_return:
	return bResult;
}

BOOL CFaceGroup::InsertFace(WORD a,WORD b,WORD c,TVERTEX* ptv)
{
	BOOL	bResult = FALSE;
	
	if (m_dwFacesNum >= m_dwMaxFacesNum)
		goto lb_return;

	m_pFace[m_dwFacesNum*3+0] = a;
	m_pFace[m_dwFacesNum*3+1] = b;
	m_pFace[m_dwFacesNum*3+2] = c;
	
	// 라이트맵 매쉬인 경우
	memcpy(m_ptLightUV1 + m_dwFacesNum*3,ptv,sizeof(TVERTEX)*3);
	m_dwLightUVNum1 += 3;
	
	m_dwFacesNum++;


lb_return:
	return bResult;
}

BOOL CFaceGroup::InsertFace(WORD a,WORD b,WORD c,TVERTEX* ptv1,TVERTEX* ptv2)
{
	BOOL	bResult = FALSE;
	
	if (m_dwFacesNum >= m_dwMaxFacesNum)
		goto lb_return;

	m_pFace[m_dwFacesNum*3+0] = a;
	m_pFace[m_dwFacesNum*3+1] = b;
	m_pFace[m_dwFacesNum*3+2] = c;
	
	// 라이트맵 매쉬인 경우
	memcpy(m_ptLightUV1 + m_dwFacesNum*3,ptv1,sizeof(TVERTEX)*3);
	m_dwLightUVNum1 += 3;

	memcpy(m_ptLightUV2 + m_dwFacesNum*3,ptv2,sizeof(TVERTEX)*3);
	m_dwLightUVNum2 += 3;
	
	m_dwFacesNum++;


lb_return:
	return bResult;
}


DWORD CFaceGroup::WriteFile(FILE* fp)
{
	FILE_FACE_GROUP_HEADER* pHeader = (FILE_FACE_GROUP_HEADER*)&(this->m_dwMtlIndex);

	DWORD	oldPos = ftell(fp);
	
	fwrite(pHeader,sizeof(FILE_FACE_GROUP_HEADER),1,fp);
	
	if (m_dwFacesNum)
		fwrite(m_pFace,sizeof(WORD),m_dwFacesNum*3,fp);
		
	if (m_dwLightUVNum1)
		fwrite(m_ptLightUV1,sizeof(TVERTEX),m_dwLightUVNum1,fp);

	if (m_dwLightUVNum2)
		fwrite(m_ptLightUV2,sizeof(TVERTEX),m_dwLightUVNum2,fp);

	return (ftell(fp)-oldPos);
}
CFaceGroup::~CFaceGroup()
{
	if (m_pFace)
	{
		delete [] m_pFace;
		m_pFace = NULL;
	}
	if (m_ptLightUV1)
	{
		delete [] m_ptLightUV1;
		m_ptLightUV1 = NULL;
	}

	if (m_ptLightUV2)
	{
		delete [] m_ptLightUV2;
		m_ptLightUV2 = NULL;
	}
}

