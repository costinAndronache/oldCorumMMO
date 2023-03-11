#include "scene.h"
#include "geom_obj.h"
#include "camera_obj.h"
#include "light_obj.h"
#include "LightTextureGenerator.h"
#include "bone_obj.h"
#include "phymod.h"
#include <bipexp.h>
#include "dialog.h"
#include "../../4DyuchiGXGFunc/global.h"
#include <crtdbg.h>

CScene*		g_pScene = NULL;


/*
DWORD CMeshObject::SetVertexNormal(VECTOR3* pv3NormalResult,VECTOR3* pv3VertexList,DWORD dwVertexNum)
{
	DWORD	i;
	memset(pv3NormalResult,0,sizeof(VECTOR3)*dwVertexNum);
	for (i=0; i<m_dwFaceGroupNum; i++)
	{
		SetFaceNormalToVertex(pv3NormalResult,pv3VertexList,dwVertexNum,m_pFaceGroup[i].GetFacesPtr(),m_pFaceGroup[i].GetFacesNum());
	}
	DWORD	dwCount = 0;

	for (i=0; i<dwVertexNum; i++)
	{
		if (!(pv3NormalResult[i].x == 0.0f && 
			pv3NormalResult[i].y == 0.0f && 
			pv3NormalResult[i].z == 0.0f)) 
		{

			Normalize(&pv3NormalResult[i],&pv3NormalResult[i]);
			dwCount++;
		}
	}
	return dwCount;
}*/

CScene::CScene(CObjectPropertyList* pMeshPropertyList,Interface* pInterface)
{


	memset(this,0,sizeof(CScene));
	m_pMeshPropertyList = pMeshPropertyList;
	m_pInterface = pInterface;

	m_treeLightMap.Initialize(1024,4);
	m_bSuccess = TRUE;
}


CBaseObject* CScene::AllocObject(OBJECT_TYPE type,char* szObjName)
{

	I3DModel*	pModel = GetRootModel();

	CBaseObject*	pObj;
	if (OBJECT_TYPE_MESH == type)
		pObj = new CMeshObject;
	else if (OBJECT_TYPE_LIGHT == type)
		pObj = new CLightObject;
	else if (OBJECT_TYPE_CAMERA == type || OBJECT_TYPE_CAMERA_TARGET == type)
		pObj = new CCameraObject;
	else if (OBJECT_TYPE_BONE == type)
		pObj = new CBoneObject;

	
	pObj->SetObjectType(type);
	pObj->m_pNextObject = m_pBaseObject;
	m_pBaseObject = pObj;
	
	if (pModel)
	{
		DWORD dwObjIndex = pModel->GetObjectIndex(szObjName);
		if (0xffffffff == dwObjIndex)
		{
			MessageBox(NULL,"��Ʈ �𵨰� ���� ���� ������Ʈ �̸��� ��ġ���� �ʽ��ϴ�.","Error",MB_OK);
			m_bSuccess = FALSE;
		}
		pObj->SetIndex(dwObjIndex);
	}
	else
	{
		pObj->SetIndex(m_dwObjectNum);
	}
	m_dwObjectNum++;

	return pObj;
}
void CScene::FreeObject(CBaseObject* pObj)
{
	CBaseObject*	pCur = m_pBaseObject;
	CBaseObject*	pPrv = NULL;

	while (pCur != pObj)
	{
		pPrv = pCur;
		pCur = pCur->m_pNextObject;
	}
	if (!pPrv)
		m_pBaseObject = pCur->m_pNextObject;
	else
		pPrv->m_pNextObject = pCur->m_pNextObject;
	
	if (OBJECT_TYPE_MESH == pCur->GetObjectType())
		delete (CMeshObject*)pCur;
	else if (OBJECT_TYPE_LIGHT == pCur->GetObjectType())
		delete (CLightObject*)pCur;
	else if (OBJECT_TYPE_CAMERA == pCur->GetObjectType() || OBJECT_TYPE_CAMERA_TARGET == pCur->GetObjectType())
		delete (CCameraObject*)pCur;
	else if (OBJECT_TYPE_BONE == pCur->GetObjectType())
		delete (CBoneObject*)pCur;
	

	m_dwObjectNum--;
}
CBaseObject* CScene::InsertObject(INode* pNode)
{
	CBaseObject*	pObject = NULL;

	ObjectState os = pNode->EvalWorldState(0);	// ����������, 0���� �����Ѵ�.
	
	TCHAR*	name = pNode->GetName();
	if (!os.obj)
		goto lb_return;
	
	// The obj member of ObjectState is the actual object we will export.
	if (os.obj) 
	{
		switch(os.obj->SuperClassID()) 
		{
			case GEOMOBJECT_CLASS_ID: 
				pObject = ConvertGeometry(pNode);
				break;
			case CAMERA_CLASS_ID:
				pObject = ConvertCamera(pNode);
				break;
			case LIGHT_CLASS_ID:
				pObject = ConvertLight(pNode);
				break;
			case SHAPE_CLASS_ID:
				
				break;
			case HELPER_CLASS_ID:
				
				break;
			
		}
	}	

lb_return:
	return pObject;
}
CBaseObject* CScene::ConvertGeometry(INode* pNode)
{
	BOOL	bResult = FALSE;
	BOOL	needDel;

	CBaseObject*	pObject = NULL;
	Control *c = pNode->GetTMController();

	char*	szName	=	pNode->GetName();
	DWORD	dwLen	=	lstrlen("Bone");

	// mesh�̸�
	TriObject* tri = GetTriObjectFromNode(pNode, 0, needDel);
	if (!tri)
		goto lb_return;

	
	if (!memcmp(szName,"Bone",dwLen))
		goto lb_bone;

	if ((c->ClassID() == BIPSLAVE_CONTROL_CLASS_ID) ||
		(c->ClassID() == BIPBODY_CONTROL_CLASS_ID) ||
		(c->ClassID() == FOOTPRINT_CLASS_ID))
	{
lb_bone:
		pObject = AllocObject(OBJECT_TYPE_BONE,pNode->GetName());
		pObject->SetObjectName(pNode->GetName());

		// Get the Biped Export Interface from the controller 
        
		IBipedExport *BipIface = (IBipedExport *) c->GetInterface(I_BIPINTERFACE);
     /*
			
		// Remove the non uniform scale
        
        BipIface->RemoveNonUniformScale(1);
     
        // Redraw if you want to see the result
        
        m_pInterface->RedrawViews(m_pInterface->GetTime());

        
        // Release the interface when you are done with it
        c->ReleaseInterface(I_BIPINTERFACE, BipIface);
*/
		

		ConvertNodeHeader(pObject,pNode);
		ConvertNodeTM(pObject->GetTM(),pNode);
		CreateBoundingMesh(pObject,pNode,tri);
		m_dwBoneObjectNum++;
	}
	else
	{
		pObject = AllocObject(OBJECT_TYPE_MESH,pNode->GetName());
		pObject->SetObjectName(pNode->GetName());
		ConvertNodeHeader(pObject,pNode);
		ConvertNodeTM(pObject->GetTM(),pNode);
	
		if (!ConvertMesh(pObject,pNode,tri))
		{
			FreeObject(pObject);
			goto lb_skip;
		}
		m_dwMeshObjectNum++;
	}	
lb_skip:
	if (needDel) 
	{
		delete tri;
	}

lb_return:
	return pObject;

}
BOOL CScene::CreateBoundingMesh(CBaseObject* pObj,INode* pNode,TriObject* tri)
{	
	CBoneObject* pBone = (CBoneObject*)pObj;

	Matrix3		tm = pNode->GetObjTMAfterWSM(0);
	BOOL		negScale = TMNegParity(tm);
	ObjectState os = pNode->EvalWorldState(0);

	if (!os.obj || os.obj->SuperClassID()!=GEOMOBJECT_CLASS_ID)
		return FALSE;

	int vx1, vx2, vx3;

	// Order of the vertices. Get 'em counter clockwise if the objects is
	// negatively scaled.
	if (negScale) 
	{
		vx1 = 2;
		vx2 = 1;
		vx3 = 0;
	}
	else {
		vx1 = 0;
		vx2 = 1;
		vx3 = 2;
	}

	Mesh*		mesh = NULL;

	mesh = &tri->GetMesh();
	
	DWORD dwFacesNum = mesh->getNumFaces();

	COLLISION_MESH_OBJECT_DESC	colMeshDesc;
	memset(&colMeshDesc,0,sizeof(colMeshDesc));
	
	
	DWORD		i,j;
	if (dwFacesNum)
	{
		VECTOR3*		pv3 = new VECTOR3[dwFacesNum*3];
		DWORD			dwVertexNum = 0;
		
		for (i=0; i<dwFacesNum; i++)
		{
			for (j=0; j<3; j++)
			{
				Point3		vp;

				vp = tm * mesh->verts[mesh->faces[i].v[j]];
				pv3[i*3+j].x = vp.x;
				pv3[i*3+j].y = vp.z;
				pv3[i*3+j].z = vp.y;
			}
		}
		CalcBoundingMesh(pv3,dwFacesNum*3,&colMeshDesc);
		
/*		fRs = colMeshDesc.boundingSphere.fRs;

		for (i=0; i<dwFacesNum; i++)
		{
			for (j=0; j<3; j++)
			{
				pv3[i*3+j].x = mesh->verts[mesh->faces[i].v[j]].x;
				pv3[i*3+j].y = mesh->verts[mesh->faces[i].v[j]].z;
				pv3[i*3+j].z = mesh->verts[mesh->faces[i].v[j]].y;
			}
		}
		
		CalcBoundingMesh(pv3,dwFacesNum*3,&colMeshDesc,flag);
		colMeshDesc.boundingSphere.fRs = fRs;*/
		COLLISION_MESH_OBJECT_DESC_SAVELOAD	colMeshSaveLoad;
		memcpy(&colMeshSaveLoad.boundingBox,&colMeshDesc.boundingBox,sizeof(colMeshDesc.boundingBox));
		memset(&colMeshSaveLoad.boundingCylinder,0,sizeof(colMeshSaveLoad.boundingCylinder));
		memcpy(&colMeshSaveLoad.boundingSphere,&colMeshDesc.boundingSphere,sizeof(colMeshDesc.boundingSphere));
		colMeshSaveLoad.dwObjIndex = pObj->GetIndex();
		strcpy(colMeshSaveLoad.szObjName,pObj->GetObjectName());
		
		pBone->SetColMeshDesc(&colMeshSaveLoad);
		delete [] pv3;

	}

	return TRUE;
}

BOOL CScene::ConvertIllusionMesh(CBaseObject* pObj,INode* pNode,TriObject* tri)
{

	BOOL		bResult = FALSE;

	ObjectState os = pNode->EvalWorldState(0);
	Matrix3		tm = pNode->GetObjTMAfterWSM(0);

	CMeshObject* pMesh = (CMeshObject*)pObj;

	if (!os.obj || os.obj->SuperClassID()!=GEOMOBJECT_CLASS_ID)
		goto lb_return;
	
	Mesh*		mesh;
	mesh = &tri->GetMesh();
	
	Mtl*		nodeMtl;
	nodeMtl = pNode->GetMtl();

	DWORD		dwVertexNum;
	dwVertexNum = mesh->getNumVerts();
	if (!dwVertexNum)
		goto lb_return;

//	mesh->buildNormals();


	if (dwVertexNum)
	{
		pMesh->CreateVertexList(dwVertexNum);
		pMesh->CreateNormalVertexList(NULL);	// ������ ������� ������ �ͽ���Ʈ�� �ϰ����� ���� ���� ���´�.
		pMesh->CreateTangentULIst();

		VECTOR3		v3Point;
		
		DWORD	i;
		for (i=0; i<dwVertexNum; i++)
		{
			Point3		vp;
			vp = tm * mesh->verts[i];

			v3Point.x = vp.x; 
			v3Point.y = vp.z;
			v3Point.z = vp.y;

			pMesh->InsertVertex(&v3Point);
		}
	}

	DWORD dwMtlIndex;
	dwMtlIndex = AddMaterial(nodeMtl,0);
	pMesh->SetMtlIndex(dwMtlIndex);

	bResult = TRUE;
lb_return:
	return bResult;
}
BOOL CScene::ConvertMesh(CBaseObject* pObj,INode* pNode,TriObject* tri)
{	
	CMeshObject* pMesh = (CMeshObject*)pObj;

	// �Ž� �÷��׸� ���� ������.
	CMeshFlag*	pMeshFlag;
	
	pMeshFlag = (CMeshFlag*)m_pMeshPropertyList->GetPropertyPtr(pNode->GetName());
	pMesh->SetMeshFlag(*pMeshFlag);


	if (!memcmp(pObj->GetObjectName(),"_ILLUSION_MESH",lstrlen("_ILLUSION_MESH")))
	{
		pMeshFlag->SetTransformType(TRANSFORM_TYPE_ILLUSION);
		pMesh->SetMeshFlag(*pMeshFlag);
		return ConvertIllusionMesh(pObj,pNode,tri);
	}
	// The first two functions access the entire buffer
	BOOL	bHasPhysique = FALSE;
	
	if (IsPhysiqueObject(pNode))
	{
		bHasPhysique = TRUE;
		pMesh->GetMeshFlagPtr()->SetTransformType(TRANSFORM_TYPE_NOT_SOLID);

		if (pMesh->GetMeshFlagPtr()->GetShadeType() == SHADE_TYPE_LIGHT_MAP)
			pMesh->GetMeshFlagPtr()->SetShadeType(SHADE_TYPE_VERTEX_LIGHT_IM);
	}

	Mtl*		nodeMtl = pNode->GetMtl();
	Matrix3		tm = pNode->GetObjTMAfterWSM(0);
	BOOL		negScale = TMNegParity(tm);
	ObjectState os = pNode->EvalWorldState(0);
	int vx1, vx2, vx3;

	VECTOR3		v3Plane[3];
	VECTOR3		v3Dir;
	PLANE		plane;
	BOOL		bResult = FALSE;
	Mesh*		mesh = NULL;
	DWORD		i;
	DWORD		dwFacesNum = 0;
	DWORD		dwTFacesNum = 0;
	DWORD		dwFaceGroupNum = 0;
		
	CFaceGroup*	pFaceGroup = NULL;
	DWORD		dwMtlIndex[MAXEXP_MAX_MATERIAL_NUM];

	FACE*		pFace = NULL;
	TFACE*		pTFace = NULL;

	VECTOR3*	pv3 = NULL;
	TVERTEX*	puv = NULL;
	TVERTEX*	puvtemp = NULL;
	Point3		vn;
	Point3		vp;
	
	DWORD		dwVertexNum;
	DWORD		dwTexVertexNum;
	TVERTEX*	ptLightUV1 = NULL;
	TVERTEX*	ptLightUV2 = NULL;
		
	CLightTextureGenerator generator1;
	CLightTextureGenerator generator2;

	generator1.SetLightMapDetail(GetLightMapDetail());
	generator2.SetLightMapDetail(GetLightMapDetail());

	CLightTexture*		pLightTexture1 = NULL;
	CLightTexture*		pLightTexture2 = NULL;

	VECTOR3 zero;
	zero.x = 0.0f;
	zero.y = 0.0f;
	zero.z = 0.0f;

	if (!os.obj || os.obj->SuperClassID()!=GEOMOBJECT_CLASS_ID)
		return FALSE;
	
	// Order of the vertices. Get 'em counter clockwise if the objects is
	// negatively scaled.
	if (negScale) {
		vx1 = 2;
		vx2 = 1;
		vx3 = 0;
	}
	else {
		vx1 = 0;
		vx2 = 1;
		vx3 = 2;
	}

	mesh = &tri->GetMesh();
	mesh->buildNormals();
	
	
	memset(dwMtlIndex,0,sizeof(DWORD)*MAXEXP_MAX_MATERIAL_NUM);

	dwFacesNum = mesh->getNumFaces();
	if (!dwFacesNum)
	{
		return FALSE;
	}
	pMesh->SetTotalFacesNum(dwFacesNum);
	DWORD	dwFaceCount = 0;

	int  vert;
	Face* f;

	if (dwFacesNum)
	{



		

		pFace = new FACE[dwFacesNum];
		
//		pv3Map = new VECTOR3[dwFaceNum*3];

		for (i=0; i<dwFacesNum; i++)
		{
			if (mesh->faces[i].getMatID() >= MAXEXP_MAX_MATERIAL_NUM)
			{
				MessageBox(NULL,"Max Materials over","ConvertMesh..",MB_OK);
				return FALSE;
			}

			VECTOR3		vp[3];
			memcpy(&vp[0],tm * mesh->verts[mesh->faces[i].v[vx1]],sizeof(VECTOR3));
			memcpy(&vp[1],tm * mesh->verts[mesh->faces[i].v[vx3]],sizeof(VECTOR3));
			memcpy(&vp[2],tm * mesh->verts[mesh->faces[i].v[vx2]],sizeof(VECTOR3));

			
			
		
			if (!IsValidTri(vp+0,vp+1,vp+2))
			{
				__asm nop
			}
			else
			{
				pFace[dwFaceCount].a = (WORD)mesh->faces[i].v[vx1];
				pFace[dwFaceCount].b = (WORD)mesh->faces[i].v[vx3];
				pFace[dwFaceCount].c = (WORD)mesh->faces[i].v[vx2];
				pFace[dwFaceCount].mtlid = mesh->faces[i].getMatID(); 

				// ��� ���� ..
				f = &mesh->faces[i];
				
				vert = f->getVert(vx1);
				vn = GetVertexNormal(mesh, i, mesh->getRVertPtr(vert));

				pFace[dwFaceCount].v3Normal[vx1].x = vn.x;
				pFace[dwFaceCount].v3Normal[vx1].y = vn.z;
				pFace[dwFaceCount].v3Normal[vx1].z = vn.y;

				vert = f->getVert(vx2);
				vn = GetVertexNormal(mesh, i, mesh->getRVertPtr(vert));

				pFace[dwFaceCount].v3Normal[vx2].x = vn.x;
				pFace[dwFaceCount].v3Normal[vx2].y = vn.z;
				pFace[dwFaceCount].v3Normal[vx2].z = vn.y;

				vert = f->getVert(vx3);
				vn = GetVertexNormal(mesh, i, mesh->getRVertPtr(vert));

				pFace[dwFaceCount].v3Normal[vx3].x = vn.x;
				pFace[dwFaceCount].v3Normal[vx3].y = vn.z;
				pFace[dwFaceCount].v3Normal[vx3].z = vn.y;


				dwMtlIndex[pFace[dwFaceCount].mtlid]++;
				dwFaceCount++;
			}
		}
		dwVertexNum = mesh->getNumVerts();

		if (dwVertexNum)
		{
			pv3 = new VECTOR3[dwVertexNum];
			
			for (i=0; i<dwVertexNum; i++)
			{
				vp = tm * mesh->verts[i];
				
				pv3[i].x = vp.x;
				pv3[i].y = vp.z;
				pv3[i].z = vp.y;

			}
		}
	}
	dwFacesNum = dwFaceCount;

	dwTexVertexNum = mesh->getNumTVerts();
	dwTFacesNum = mesh->getNumFaces();
	

	//////////////////////////////////////////////////////////////////////////////////////////////
	// uv�� ��ġ�� ���ؽ��� �ɰ��Ƿ� ����� �̸� ��������..

	VECTOR3*	pv3Normal;
	pv3Normal = NULL;

	if (dwVertexNum)
	{
		
		WORD*		pIndex;
		pv3Normal = new VECTOR3[dwVertexNum];
		memset(pv3Normal,0,sizeof(VECTOR3)*dwVertexNum);

		pIndex = new WORD[dwFacesNum*3];
  /*      
		for (i=0; i<dwFacesNum; i++)
		{


			pv3Normal[pFace[i].a].x = 	pFace[i].v3Normal[vx1].x;
			pv3Normal[pFace[i].a].y = 	pFace[i].v3Normal[vx1].y;
			pv3Normal[pFace[i].a].z = 	pFace[i].v3Normal[vx1].z;

			pv3Normal[pFace[i].b].x = 	pFace[i].v3Normal[vx2].x;
			pv3Normal[pFace[i].b].y = 	pFace[i].v3Normal[vx2].y;
			pv3Normal[pFace[i].b].z = 	pFace[i].v3Normal[vx2].z;

			pv3Normal[pFace[i].c].x = 	pFace[i].v3Normal[vx3].x;
			pv3Normal[pFace[i].c].y = 	pFace[i].v3Normal[vx3].y;
			pv3Normal[pFace[i].c].z = 	pFace[i].v3Normal[vx3].z;
		}
*/

		for (i=0; i<dwFacesNum; i++)
		{
			pIndex[i*3+0] = pFace[i].a;
			pIndex[i*3+1] = pFace[i].b;
			pIndex[i*3+2] = pFace[i].c;
		}
		// ���� �ʿ�
//		SetFaceNormalToVertex(pv3Normal,pv3,dwVertexNum,pIndex,dwFacesNum);


		for (i=0; i<dwVertexNum; i++)
		{

            if (!(pv3Normal[i].x == 0.0f && 
                pv3Normal[i].y == 0.0f && 
                pv3Normal[i].z == 0.0f)) 
			{
                Normalize(&pv3Normal[i],&pv3Normal[i]);
			}
		}

	
		delete [] pIndex;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////
    

	//////////////////////////////////////////////////////////////////////////////////////////////






	dwFaceCount = 0;


	if (dwTexVertexNum)
	{
		pTFace = new TFACE[dwTFacesNum];
		for (i=0; i<dwTFacesNum; i++)
		{
			VECTOR3		vp[3];
			memcpy(&vp[0],tm * mesh->verts[mesh->faces[i].v[vx1]],sizeof(VECTOR3));
			memcpy(&vp[1],tm * mesh->verts[mesh->faces[i].v[vx3]],sizeof(VECTOR3));
			memcpy(&vp[2],tm * mesh->verts[mesh->faces[i].v[vx2]],sizeof(VECTOR3));

				
			if (!IsValidTri(vp+0,vp+1,vp+2))
			{
				__asm nop
			}
			else
			{
				pTFace[dwFaceCount].a = mesh->tvFace[i].t[vx1];
				pTFace[dwFaceCount].b = mesh->tvFace[i].t[vx3];
				pTFace[dwFaceCount].c = mesh->tvFace[i].t[vx2];
				dwFaceCount++;
			}
		}

		puv = new TVERTEX[dwTexVertexNum];

		for (i=0; i<dwTexVertexNum; i++)
		{
			puv[i].u = mesh->tVerts[i].x;
			puv[i].v = mesh->tVerts[i].y;
		}
		dwTFacesNum = dwFaceCount;
	

		//////////////////////////////////////////////////////////////////////////////////////////////
		//
		// �ؽ��İ� ���� ��� tvertex�� ���缭 ���ؽ� �ٽ� ����
		//
		///////////////////////////////////////////////////////////////////////////////////////////////


		pMesh->SetOriginalVertexNum(dwVertexNum);
		dwTexVertexNum = dwVertexNum = ReCreateVertexList(pTFace,pFace,&pv3,&puv,dwTFacesNum,dwVertexNum,pMesh->GetExtVertexPtrPtr(),pMesh->GetExtVertexNumPtr());
		delete [] pTFace;
		pTFace = NULL;
	}

	BOOL	bHasNormal;
	
	if (pMesh->GetMeshFlagPtr()->GetShadeType() == SHADE_TYPE_LIGHT_MAP)
	{
		bHasNormal = FALSE;
		// ����Ʈ �ؽ���,��ǥ ���� 
		pMesh->SetLightTexture1(generator1.CreateLightTexture(&ptLightUV1,pFace,pv3,dwFacesNum,&m_treeLightMap));

	}
	else
	{
		bHasNormal = TRUE;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////

	// �����带 ���� ���� ���� ����
	if (pFace)
	{
		v3Plane[0] = pv3[pFace[0].a];
		v3Plane[1] = pv3[pFace[0].b];
		v3Plane[2] = pv3[pFace[0].c];

		CalcPlaneEquation(&plane,v3Plane);
		Normalize(&v3Dir,&plane.v3Up);
		pMesh->SetDirectionVector(&v3Dir);


	}

	//////////////////////////////////////////////////////////////////////////////////////////////

	// material ���� facegroup����,�и�
	DWORD	dwFacegroupMtlIndex;
	char	txt[512];
	for (i=0; i<MAXEXP_MAX_MATERIAL_NUM; i++)
	{
		if (dwMtlIndex[i])
		{
			pFaceGroup = pMesh->AllocFaceGroup();
			pFaceGroup->CreateFaceList(dwMtlIndex[i]);
			dwFacegroupMtlIndex = AddMaterial(nodeMtl,i);
			if (dwFacegroupMtlIndex == 0xffffffff)
			{
				wsprintf(txt,"���͸����� �����Ǿ����� ���� ���� �ֽ��ϴ�.:%s.�����Ǽ������ֽ��ϴ�.",pObj->GetObjectName());
				MessageBox(NULL,txt,"Error",MB_OK);
			}
			pFaceGroup->SetMtlIndex(dwFacegroupMtlIndex);
			pFaceGroup->SetUniqueIndex(i);
			dwFaceGroupNum++;
		}
	}
	for (i=0; i<dwFacesNum; i++)
	{
		pFaceGroup = pMesh->GetFaceGroup(pFace[i].mtlid);
		if (!pFaceGroup)
			continue;

		if (ptLightUV1 && ptLightUV2)
			pFaceGroup->InsertFace(pFace[i].a,pFace[i].b,pFace[i].c,ptLightUV1+i*3,ptLightUV2+i*3);
		else if (ptLightUV1)
			pFaceGroup->InsertFace(pFace[i].a,pFace[i].b,pFace[i].c,ptLightUV1+i*3);
		else 
			pFaceGroup->InsertFace(pFace[i].a,pFace[i].b,pFace[i].c);

	}
	if(pFace)
	{
		delete [] pFace;
		pFace = NULL;
	}

	if (ptLightUV1)
	{
		delete [] ptLightUV1;
		ptLightUV1 = NULL;
	}
	if (ptLightUV2)
	{
		delete [] ptLightUV2;
		ptLightUV2 = NULL;
	}
	
	// ���ؽ� ī�� 
	// �� ���ؽ��� ���� �������� tvertex�� ����, ������� ���̾���Ѵ�.
	if (pv3)
	{
		pMesh->CreateVertexList(dwVertexNum);
		for (i=0; i<dwVertexNum; i++)
			pMesh->InsertVertex(pv3+i);

		delete [] pv3;
		pv3 = NULL;
	}
	
	if (bHasNormal)
	{
		pMesh->CreateNormalVertexList(pv3Normal);
	
	}
	if (puv)
	{
		// �ؽ��� ���ؽ� ī��
		pMesh->CreateTexVertexList(dwTexVertexNum);
		for (i=0; i<dwTexVertexNum; i++)
			pMesh->InsertTexVertex(puv+i);
	
		delete [] puv;
		puv = NULL;

		pMesh->CreateTangentULIst();
	}
	
	
	if (bHasPhysique)
	{
		pMesh->CreatePhysique(pNode);
	
	}
	if (pv3Normal)
	{
        delete [] pv3Normal;
		pv3Normal = NULL;
	}
	bResult = TRUE;


lb_return:
	return bResult;
}
DWORD CScene::ReCreateVertexList(TFACE* ptFace,FACE* pFace,VECTOR3** ppv3,TVERTEX** pptv,DWORD dwTFacesNum,DWORD dwVertexNum,DWORD** ppdwExtVertexIndex,DWORD* pdwExtVertexNum)
{
	DWORD		dwTotalVertexNum = 0;

	VECTOR3*	pv3 = *ppv3;
	TVERTEX*	ptv = *pptv;

	DWORD		i;
	DWORD		dwExtVertexCount = 0;

	

	POINT3D_REF_TVERTEX*	pvRef = new POINT3D_REF_TVERTEX[dwTFacesNum*3];
	POINT3D_REF_TVERTEX*	pvRefExt = pvRef+dwVertexNum;
	DWORD*		pOriginalIndex = new DWORD[dwTFacesNum*3];

	
	
	memset(pvRef,0,sizeof(POINT3D_REF_TVERTEX)*dwTFacesNum*3);
	for (i=0; i<dwTFacesNum*3; i++)
		pvRef[i].wIndex = (WORD)i;
	
	for (i=0; i<dwTFacesNum; i++)
	{
		ReIndexFace(&pFace[i].a,ptFace[i].a,pvRef,pvRefExt,pv3,ptv,&dwExtVertexCount,pOriginalIndex);
		ReIndexFace(&pFace[i].b,ptFace[i].b,pvRef,pvRefExt,pv3,ptv,&dwExtVertexCount,pOriginalIndex);
		ReIndexFace(&pFace[i].c,ptFace[i].c,pvRef,pvRefExt,pv3,ptv,&dwExtVertexCount,pOriginalIndex);
	}

	dwTotalVertexNum = dwVertexNum+dwExtVertexCount;
	*pdwExtVertexNum = dwExtVertexCount;

	VECTOR3*	pvPoint = new VECTOR3[dwTotalVertexNum];
	TVERTEX*	ptUVW = new TVERTEX[dwTotalVertexNum];

	DWORD		noncount1 = 0;
	DWORD		noncount2 = 0;
	for (i=0; i<dwTotalVertexNum; i++)
	{
		if (!pvRef[i].pv3Point)
			noncount1++;
		else 
			pvPoint[i] = *pvRef[i].pv3Point;
		
		if (!pvRef[i].ptv)
			noncount1++;
		else 
			ptUVW[i] = *pvRef[i].ptv;
	}

	delete [] pv3;
	delete [] ptv;
	delete [] pvRef;

	if (dwExtVertexCount)
	{
		*ppdwExtVertexIndex = new DWORD[dwExtVertexCount];
		memcpy(*ppdwExtVertexIndex,pOriginalIndex,sizeof(DWORD)*dwExtVertexCount);
	}
	delete [] pOriginalIndex;

	*ppv3 = pvPoint;
	*pptv = ptUVW;

	return dwTotalVertexNum;
}
void CScene::ReIndexFace(WORD* pFaceIndex,WORD dwTFaceIndex,POINT3D_REF_TVERTEX* pvRef,POINT3D_REF_TVERTEX* pvRefExt,VECTOR3* pv3,TVERTEX* ptv,DWORD* pCount,DWORD* pOriginalIndex)
{
	POINT3D_REF_TVERTEX* pvRefCur;
	POINT3D_REF_TVERTEX* pvRefPrv;

//	if (*pFaceIndex == 47)
//		__asm int 3

	if (!pvRef[*pFaceIndex].ptv)
	{
		pvRef[*pFaceIndex].ptv = ptv + dwTFaceIndex;
		pvRef[*pFaceIndex].pv3Point = pv3+*pFaceIndex;
		if (!pvRef[*pFaceIndex].pv3Point)
			__asm int 3

		goto lb_return;
	}
	pvRefCur = &pvRef[*pFaceIndex];
	
	while(pvRefCur)
	{
		pvRefPrv = pvRefCur;
		if ((pvRefCur->ptv == ptv + dwTFaceIndex) && (pvRefCur->pv3Point == pv3 + *pFaceIndex))
		{
			*pFaceIndex = pvRefCur->wIndex;
			goto lb_return;
		}
		pvRefCur = pvRefCur->pNext;
	}
	pvRefPrv->pNext = pvRefExt + *pCount;
	
	pvRefExt[*pCount].ptv = ptv + dwTFaceIndex;
	pvRefExt[*pCount].pv3Point = pv3 + *pFaceIndex;
	if (!pvRef[*pFaceIndex].pv3Point)
			__asm int 3
	
	pOriginalIndex[*pCount] = (DWORD)(*pFaceIndex);
	
	*pFaceIndex = pvRefExt[*pCount].wIndex; 
	(*pCount)++;

lb_return:
	return;
}


DWORD CScene::AddMaterial(Mtl* pMtl,DWORD dwSubMtlIndex)
{
	DWORD dwResult = 0xffffffff;
	CMaterial	Material;
	CMaterial*	pCur = m_pBaseMaterial;

	StdMat*		pStdMtl;
	Texmap*		pTex;
	DWORD		i;

	if (!pMtl)
		goto lb_return;

	if (!pMtl->NumSubMtls())
		goto lb_mtl_set;

	pMtl = pMtl->GetSubMtl(dwSubMtlIndex);

lb_mtl_set:
	if (!pMtl)
		goto lb_return;

	if (pMtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
	{
		pStdMtl = (StdMat*)pMtl;
		Material.SetAmbient(pStdMtl->GetAmbient(0).r,pStdMtl->GetAmbient(0).g,pStdMtl->GetAmbient(0).b,0.0f);
		Material.SetDiffuse(pStdMtl->GetDiffuse(0).r,pStdMtl->GetDiffuse(0).g,pStdMtl->GetDiffuse(0).b,0.0f);
		Material.SetSpecular(pStdMtl->GetSpecular(0).r,pStdMtl->GetSpecular(0).g,pStdMtl->GetSpecular(0).b,0.0f);
		Material.SetShine(pStdMtl->GetShininess(0));
		Material.SetShineStrength(pStdMtl->GetShinStr(0));
		Material.SetTransparency(pStdMtl->GetXParency(0));

		switch (pStdMtl->GetTransparencyType()) 
		{
		case TRANSP_FILTER:
			Material.SetTranspFilter();
			break;
		case TRANSP_SUBTRACTIVE:
			Material.SetTranspSubtractive();
			break;
		case TRANSP_ADDITIVE:		
			Material.SetTranspAdditive();
			break;
		}

		if (pStdMtl->GetTwoSided()) 
			Material.Set2Side();

		if (pStdMtl->GetSelfIllumColorOn())
			Material.SetSelfIllunum();
		
	}
	else 
	{	
		Material.SetAmbient(pMtl->GetAmbient(0).r,pMtl->GetAmbient(0).g,pMtl->GetAmbient(0).b,0.0f);
		Material.SetDiffuse(pMtl->GetDiffuse(0).r,pMtl->GetDiffuse(0).g,pMtl->GetDiffuse(0).b,0.0f);
		Material.SetSpecular(pMtl->GetSpecular(0).r,pMtl->GetSpecular(0).g,pMtl->GetSpecular(0).b,0.0f);
		Material.SetShine(pMtl->GetShininess(0));
		Material.SetShineStrength(pMtl->GetShinStr(0));
		Material.SetTransparency(pMtl->GetXParency(0));
		if (pMtl->GetSelfIllumColorOn())
			Material.SetSelfIllunum();
	}
	for (i=0; i<pMtl->NumSubTexmaps(); i++)
	{
		pTex = pMtl->GetSubTexmap(i);
		if (!pTex)
			continue;

		if (pTex->ClassID() == Class_ID(BMTEX_CLASS_ID, 0x00) )
			Material.SetTexture((TEXTURE_MAP_TYPE)i,((BitmapTex*)pTex)->GetMapName());
		
	}
lb_insert:
	// ���͸����� �ߺ����� �ʵ��� �߰��ϰ� �� �ε����� ����
	if (!m_pBaseMaterial)
	{
		pCur = AllocMaterial();
		goto lb_set_mtl;
	}
	
	while (pCur)
	{
		if (Material.IsEqual(pCur))
		//if (!memcmp(pCur,&Material,sizeof(CMaterial)-8))
		{
			dwResult = pCur->GetIndex();
			goto lb_return;
		}
		pCur = pCur->m_pNextMaterial;

	}
	pCur = AllocMaterial();

lb_set_mtl:
	
	Material.SetIndex(m_dwMaterialNum);
	Material.SetName(pMtl->GetName());
	memcpy(pCur,&Material,sizeof(CMaterial)-4);
	dwResult = Material.GetIndex();

lb_return:
	return dwResult;
}

void CScene::ConvertLightSetting(CLightObject* pObj,INode* node)
{	
	ObjectState os = node->EvalWorldState(0);

	if (!os.obj)
		return;
	
	GenLight* light = (GenLight*)os.obj;
	struct LightState ls;
	Interval valid = FOREVER;

	light->EvalLightState(0, valid, &ls);

	int shadowMethod = light->GetShadowMethod();

	pObj->SetColor(ls.color.r,ls.color.g,ls.color.b,1.0f);
//	fprintf(pStream,"%s\t\t%s %s\n", indent.data(), ID_LIGHT_INTENS, Format(ls->intens));
//	fprintf(pStream,"%s\t\t%s %s\n", indent.data(), ID_LIGHT_ASPECT, Format(ls->aspect));

	if (ls.type != DIR_LIGHT && ls.useAtten) 
		pObj->SetAttent(ls.attenStart,ls.attenEnd);
}

CMaterial* CScene::AllocMaterial()
{
	CMaterial* pMtl = new CMaterial;
	pMtl->m_pNextMaterial = m_pBaseMaterial;
	m_pBaseMaterial = pMtl;
	m_dwMaterialNum++;
	
	return pMtl;

}
BOOL CScene::ConvertNodeTM(NODE_TM* pTM,INode* pNode)
{
	BOOL		bResult = FALSE;

	Matrix3 pivot = pNode->GetNodeTM(0);
	Matrix3 parenttm = pNode->GetParentTM(0);

	DumpMatrix3(pTM,&pivot,&parenttm);
	SetInverseMatrix(&pTM->mat4Inverse,&pTM->mat4);
	bResult = TRUE;

lb_return:
	return bResult;

}


//yuchi
BOOL CScene::DumpMatrix3(NODE_TM* pTM,Matrix3* m, Matrix3* pParentTM)
{
	Point3 row;

	row = m->GetRow(0);

	pTM->mat4._11 = row.x;
	pTM->mat4._12 = row.z;
	pTM->mat4._13 = row.y;
	pTM->mat4._14 = 0.0f;
	
	row = m->GetRow(2);

	pTM->mat4._21 = row.x;
	pTM->mat4._22 = row.z;
	pTM->mat4._23 = row.y;
	pTM->mat4._24 = 0.0f;

	row = m->GetRow(1);
	pTM->mat4._31 = row.x;
	pTM->mat4._32 = row.z;
	pTM->mat4._33 = row.y;
	pTM->mat4._34 = 0.0f;

	row = m->GetRow(3);
	pTM->mat4._41 = row.x;
	pTM->mat4._42 = row.z;
	pTM->mat4._43 = row.y;
	pTM->mat4._44 = 1.0f;

	// Decompose the matrix and dump the contents
	AffineParts ap;
	float rotAngle;
	Point3 rotAxis;
	float scaleAxAngle;
	Point3 scaleAxis;

	Matrix3 tm;
	if (pParentTM)
	{
		tm = (*m) * Inverse(*pParentTM);
		decomp_affine(tm, &ap);
	}
	else 
	{
		decomp_affine(*m,&ap);
	}
	
	// Quaternions are dumped as angle axis.
	AngAxisFromQ(ap.q, &rotAngle, rotAxis);
	AngAxisFromQ(ap.u, &scaleAxAngle, scaleAxis);
	
	pTM->fPosX = ap.t.x;
	pTM->fPosY = ap.t.z;
	pTM->fPosZ = ap.t.y;

	pTM->fRotAxisX = rotAxis.x;
	pTM->fRotAxisY = rotAxis.z;
	pTM->fRotAxisZ = rotAxis.y;

	pTM->fRotAng = rotAngle;
	
	pTM->fScaleAxisX = scaleAxis.x;
	pTM->fScaleAxisY = scaleAxis.z;
	pTM->fScaleAxisZ = scaleAxis.y;

	pTM->fScaleX = ap.k.x;
	pTM->fScaleY = ap.k.z;
	pTM->fScaleZ = ap.k.y;

	pTM->fScaleAngle = scaleAxAngle;

	return TRUE;
}
BOOL CScene::ConvertNodeHeader(CBaseObject* pObj,INode* pNode)
{
	BOOL		bResult = FALSE;
	INode*		parent = pNode->GetParentNode();

	if (parent->IsRootNode())
		goto lb_return;

	
	
	pObj->SetParentObjectName(parent->GetName());
	bResult = TRUE;

lb_return:
	return bResult;

}
CBaseObject* CScene::ConvertCamera(INode* pNode)
{
		
	INode* target = pNode->GetTarget();

	CCameraObject*	pObject = (CCameraObject*)AllocObject(OBJECT_TYPE_CAMERA,pNode->GetName());
	CCameraObject*	pTarget = (CCameraObject*)AllocObject(OBJECT_TYPE_CAMERA_TARGET,target->GetName());
	m_dwCameraObjectNum++;

	pObject->SetTargetIndex(pTarget->GetIndex());
	
	ConvertNodeTM(pObject->GetTM(),pNode);
	ConvertNodeTM(pTarget->GetTM(),target);

	pObject->SetObjectName(pNode->GetName());
	pTarget->SetObjectName(target->GetName());
	
	ConvertNodeHeader(pObject,pNode);
	ConvertNodeHeader(pTarget,target);


	return (CBaseObject*)pObject;
}

CBaseObject* CScene::ConvertLight(INode* pNode)
{
	CLightObject*	pObject = (CLightObject*)AllocObject(OBJECT_TYPE_LIGHT,pNode->GetName());
	m_dwLightObjectNum++;
	
	ConvertNodeTM(pObject->GetTM(),pNode);
	pObject->SetObjectName(pNode->GetName());
	ConvertNodeHeader(pObject,pNode);
	ConvertLightSetting(pObject,pNode);
	
	return (CBaseObject*)pObject;
}


BOOL CScene::TMNegParity(Matrix3 &m)
{
	return (DotProd(CrossProd(m.GetRow(0),m.GetRow(1)),m.GetRow(2))<0.0)?1:0;
}

TriObject* CScene::GetTriObjectFromNode(INode *node, TimeValue t, int &deleteIt)
{
	deleteIt = FALSE;
	Object *obj = node->EvalWorldState(t).obj;
	if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) { 
		TriObject *tri = (TriObject *) obj->ConvertToType(t, 
			Class_ID(TRIOBJ_CLASS_ID, 0));
		// Note that the TriObject should only be deleted
		// if the pointer to it is not equal to the object
		// pointer that called ConvertToType()
		if (obj != tri) deleteIt = TRUE;
		return tri;
	}
	else {
		return NULL;
	}
}
Point3 CScene::GetVertexNormal(Mesh* mesh, int faceNo, RVertex* rv)
{
	Face* f = &mesh->faces[faceNo];
	DWORD smGroup = f->smGroup;
	int numNormals;
	Point3 vertexNormal;
	
	// Is normal specified
	// SPCIFIED is not currently used, but may be used in future versions.
	if (rv->rFlags & SPECIFIED_NORMAL) {
		vertexNormal = rv->rn.getNormal();
	}
	// If normal is not specified it's only available if the face belongs
	// to a smoothing group
	else if ((numNormals = rv->rFlags & NORCT_MASK) && smGroup) {
		// If there is only one vertex is found in the rn member.
		if (numNormals == 1) {
			vertexNormal = rv->rn.getNormal();
		}
		else {
			// If two or more vertices are there you need to step through them
			// and find the vertex with the same smoothing group as the current face.
			// You will find multiple normals in the ern member.
			for (int i = 0; i < numNormals; i++) {
				if (rv->ern[i].getSmGroup() & smGroup) {
					vertexNormal = rv->ern[i].getNormal();
				}
			}
		}
	}
	else {
		// Get the normal from the Face if no smoothing groups are there
		vertexNormal = mesh->getFaceNormal(faceNo);
	}
	
	return vertexNormal;
}

BOOL CScene::WriteFile(TCHAR* uszFileName)
{
	FILE* fp = _tfopen(uszFileName,"wb");
	CBaseObject*	pCurObj = m_pBaseObject;
	CMaterial*		pCurMtl = m_pBaseMaterial;

	OBJECT_TYPE type;
	DWORD		size;
	long		entry;
	
	FILE_SCENE_HEADER header;
	header.dwVersion = 0x00000002;
	header.dwObjectNum = m_dwObjectNum;
	header.dwMaterialNum = m_dwMaterialNum;
	header.dwLightObjectNum = m_dwLightObjectNum;
	header.dwMeshObjectNum = m_dwMeshObjectNum;
	header.dwCameraObjectNum = m_dwCameraObjectNum;
	header.dwBoneObjectNum = m_dwBoneObjectNum;

	fwrite(&header,sizeof(FILE_SCENE_HEADER),1,fp);

	// ���͸��� write
	type = OBJECT_TYPE_MATERIAL;
	while (pCurMtl)
	{
		fwrite(&type,sizeof(DWORD),1,fp);
		entry = ftell(fp);
		fseek(fp,4,SEEK_CUR);

		size = pCurMtl->WriteFile(fp);

		fseek(fp,entry,SEEK_SET);
		fwrite(&size,sizeof(DWORD),1,fp);
		fseek(fp,size,SEEK_CUR);

		pCurMtl = pCurMtl->m_pNextMaterial;
	}

	while (pCurObj)
	{
		type = pCurObj->GetObjectType();

		fwrite(&type,sizeof(DWORD),1,fp);
		entry = ftell(fp);					// ���̸� ����ϱ� ���� ���� �ɼ� 
		fseek(fp,4,SEEK_CUR);				// 4����Ʈ �ǳʶٰ�
	
		size = pCurObj->WriteFile(fp);		// WRITE �� ������
		
		fseek(fp,entry,SEEK_SET);			// ���̸� ���
		fwrite(&size,sizeof(DWORD),1,fp);
		fseek(fp,size,SEEK_CUR);			// ���� ûũ�� ����� �ɼ�����..
			
		pCurObj = pCurObj->m_pNextObject;
	}
	fclose(fp);


	return TRUE;
}
BOOL CScene::SetHirerarchy()
{
	I3DModel*	pModel = GetRootModel();
	if (pModel)
	{
		if (pModel->GetObjectNum() != m_dwObjectNum)
		{
			MessageBox(NULL,"��Ʈ�𵨰� ������� ������Ʈ ������ ��ġ���� �ʽ��ϴ�.","Error",MB_OK);
			m_bSuccess = FALSE;
			return FALSE;
		}
	}

	CBaseObject* pCur = m_pBaseObject;
	CBaseObject* pParent;

	while(pCur)
	{
		pParent = SelectObject(pCur->GetParentObjectName());
		pCur->SetParentObject(pParent);
		
		
		if (pParent)
		{
			pParent->AddChildObjectCount();
			pCur->SetParentObjectIndex(pParent->GetIndex());
		} 
		else 
		{
			pCur->SetParentObjectIndex(0xffffffff);
				
		}

		pCur = pCur->m_pNextObject;
	}
	
	pCur = m_pBaseObject;
	while (pCur)
	{
		pCur->CreateChildObjectList();
		pCur = pCur->m_pNextObject;
	}
	
	pCur = m_pBaseObject;
	while (pCur)
	{
		pParent = pCur->GetParentObject();
		if (pParent)
			pParent->InsertChildObjectIndex(pCur->GetIndex());

		pCur = pCur->m_pNextObject;
	}

	return TRUE;
}


CBaseObject* CScene::SelectObject(TCHAR* uszObjName)
{
	CBaseObject* pCur = m_pBaseObject;
	while(pCur)
	{
		if (!_tcscmp(uszObjName,pCur->GetObjectName()))
			return pCur;

		pCur = pCur->m_pNextObject;
	}
	return NULL;
}

void CScene::ReleaseMaterials()
{
	CMaterial*	pCur = m_pBaseMaterial;
	CMaterial*	pNext;

	while (pCur)
	{
		pNext = pCur->m_pNextMaterial;
		delete pCur;
		pCur = pNext;
	}
	m_pBaseMaterial = NULL;
	m_dwMaterialNum = 0;

}
void CScene::ReleaseObjects()
{
	CBaseObject* pCur = m_pBaseObject;
	CBaseObject* pNext;

	while(pCur)
	{
		pNext = pCur->m_pNextObject;
		delete pCur;
		pCur = pNext;
	}
	m_pBaseObject = NULL;
	m_dwObjectNum = 0;
}
CScene::~CScene()
{
	
	ReleaseObjects();
	ReleaseMaterials();
	m_treeLightMap.Release();
	_ASSERT(_CrtCheckMemory());


}