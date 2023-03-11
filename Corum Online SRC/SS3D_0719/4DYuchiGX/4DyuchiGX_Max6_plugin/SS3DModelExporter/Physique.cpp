#include "physique.h"
#include "Scene.h"

CPhysique::CPhysique()
{
	memset(this,0,sizeof(CPhysique));
}

BOOL CPhysique::CreatePhysiqueVertex(DWORD dwVertexNum,DWORD dwTotalBonesNum,DWORD dwExtVertexNum,DWORD* pdwExtVertexIndex)
{
	m_dwOrigVertexNum = dwVertexNum;
	m_dwVertexNum = dwVertexNum + dwExtVertexNum;
	m_dwExtVertexNum = dwExtVertexNum;
	m_pdwExtVertexIndexList = pdwExtVertexIndex;

	m_pPhyVertexList = new PHYSIQUE_VERTEX[m_dwVertexNum];
	memset(m_pPhyVertexList,0,sizeof(PHYSIQUE_VERTEX)*m_dwVertexNum);

	m_dwTotalBonesNum = dwTotalBonesNum;
	m_pBoneList = new BONE[dwTotalBonesNum];
	memset(m_pBoneList,0,sizeof(BONE)*dwTotalBonesNum);

	

	return TRUE;
}
BOOL CPhysique::Build(VECTOR3* pv3World,VECTOR3* pv3NormalWorld,VECTOR3* pv3TangentU)
{
	BOOL	bResult = FALSE;

	DWORD	i,j;
	if (!m_dwVertexNum)
		goto lb_return;

	MATRIX4		matInverse;

	for (i=0; i<m_dwVertexNum; i++)
	{
		for (j=0; j<m_pPhyVertexList[i].bBonesNum; j++)
		{
			
			char*	szObjName;
			szObjName = ((INode*)m_pPhyVertexList[i].pBoneList[j].pBone)->GetName();
			CBaseObject* pObj = g_pScene->SelectObject(szObjName);
			if (!pObj)
			{
				char	txt[512];
				wsprintf(txt,"������Ʈ�� ã�� �� �����ϴ�.�ش� ������Ʈ�� ���̿�����Ʈ���� Ȯ�����ּ���.: %s",szObjName);
				MessageBox(NULL,txt,"Error",MB_OK);
				__asm int 3
			}
			m_pPhyVertexList[i].pBoneList[j].pBone = pObj;

			matInverse = pObj->GetTM()->mat4Inverse;
			matInverse._41 = 0.0f;
			matInverse._42 = 0.0f;
			matInverse._43 = 0.0f;

		
			TransformVector3_VPTR2(
				&m_pPhyVertexList[i].pBoneList[j].v3NormalOffset,
				&pv3NormalWorld[i],
				&matInverse,1);


//			TransformVector3_VPTR2(
//				&m_pPhyVertexList[i].pBoneList[j].v3TangentOffset,
//				&pv3TangentU[i],
//				&matInverse,1);
		}
	}
	bResult = TRUE;

lb_return:
	return bResult;

}
BOOL CPhysique::CreateBoneList(DWORD dwVertexIndex,BYTE bBonesNum)
{
	if (m_dwAllocBonesNum + (DWORD)bBonesNum > m_dwTotalBonesNum)
		__asm int 3

	m_pPhyVertexList[dwVertexIndex].bBonesNum = bBonesNum;
	m_pPhyVertexList[dwVertexIndex].pBoneList = m_pBoneList + m_dwAllocBonesNum;;

	m_dwAllocBonesNum += (DWORD)bBonesNum;

	memset(m_pPhyVertexList[dwVertexIndex].pBoneList,0,sizeof(BONE)*bBonesNum);
	
	return TRUE;
}
void CPhysique::SetBone(DWORD dwVertexIndex,DWORD dwBoneIndex,void* pBone,float fWeight,VECTOR3* pv3Offset)
{
	// �ƽ� �ͽ����Ϳ����� pBone�� INode


	m_pPhyVertexList[dwVertexIndex].pBoneList[dwBoneIndex].pBone = (void*)pBone;
	m_pPhyVertexList[dwVertexIndex].pBoneList[dwBoneIndex].fWeight = fWeight;
	m_pPhyVertexList[dwVertexIndex].pBoneList[dwBoneIndex].v3Offset = *pv3Offset;
	

}

DWORD CPhysique::WriteFile(FILE* fp)
{
		

	// �ͽ��ٵ�� ���ؽ��� ó���Ѵ�.
	// ������Ʈ�� �״�� ���Ͽ� ���
	// ���� ���ο� ������� ���ؽ� ������ ���� �迭�Ǿ��ִ�.
	// ����ũ���ؽ��� �� �����Ϳ��� �� ������Ʈ �ε����� �ִ´�.
	DWORD	i;	

	PHYSIQUE_VERTEX*	pExtEntry = m_pPhyVertexList + m_dwOrigVertexNum;
	for (i=0; i<m_dwExtVertexNum; i++)
	{
		*pExtEntry = m_pPhyVertexList[m_pdwExtVertexIndexList[i]];
		pExtEntry++;
	}
	DWORD	oldPos = ftell(fp);


	FILE_PHYSIQUE_HEADER* pHeader = (FILE_PHYSIQUE_HEADER*)this;
	fwrite(pHeader,sizeof(FILE_PHYSIQUE_HEADER),1,fp);

	PHYSIQUE_VERTEX		phyVer;
	for (i=0; i<m_dwVertexNum; i++)
	{
		phyVer.bBonesNum = m_pPhyVertexList[i].bBonesNum;
		phyVer.pBoneList = (BONE*)( m_pPhyVertexList[i].pBoneList - m_pBoneList );

		fwrite(&phyVer,sizeof(PHYSIQUE_VERTEX),1,fp);
	}

	

	CBaseObject*	pObj;
	
	//�� �ͽ����ʹ� 2.0������ �����Ѵ�. �������� �������� üũ�Ͽ� ���� �о���δ�.
	BONE	bone;
	for (i=0; i<m_dwTotalBonesNum; i++)
	{
		pObj = (CBaseObject*)m_pBoneList[i].pBone;
		bone.fWeight = m_pBoneList[i].fWeight;
		bone.pBone = (void*)pObj->GetIndex();
		bone.v3Offset = m_pBoneList[i].v3Offset;
//		bone.v3TangentOffset = m_pBoneList[i].v3TangentOffset;
		bone.v3NormalOffset = m_pBoneList[i].v3NormalOffset;

		fwrite(&bone,sizeof(BONE),1,fp);
	}
	return (ftell(fp)-oldPos);
	
}
CPhysique::~CPhysique()
{
	if (m_pBoneList)
	{
		delete [] m_pBoneList;
		m_pBoneList = NULL;
	}
	if (m_pPhyVertexList)
	{
		delete [] m_pPhyVertexList;
		m_pPhyVertexList = NULL;
	}
}