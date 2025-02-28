#pragma once

//#ifndef GLOBAL_H
//#define GLOBAL_H

#define GLOBAL_FUNC_DLL 

#include "../4DyuchiGRX_common/typedef.h"
#include "type.h"

//#include "../4DyuchiGRX_common/math.inl"

#define IN
#define OUT

#ifndef _MAX_EXP
	GLOBAL_FUNC_DLL float	  Sin(float x);
	GLOBAL_FUNC_DLL float	  Cos(float x);
	GLOBAL_FUNC_DLL float	  ACos(float x);
	GLOBAL_FUNC_DLL float	  ASin(float x);
#endif
GLOBAL_FUNC_DLL BOOL	  ClipRenderObjectWithViewVolume(VIEW_VOLUME* pVolume,COLLISION_MESH_OBJECT_DESC* pColMeshObjDesc,DWORD dwFlag);
GLOBAL_FUNC_DLL DWORD	  ClipLightWithRS(LIGHT_INDEX_DESC* pbLightIndexArrayResult,COLLISION_MESH_OBJECT_DESC* pColMeshDesc,LIGHT_DESC* pLightDescIn,DWORD dwLightNum,BYTE bStartIndex);
GLOBAL_FUNC_DLL BOOL	  CalcIntersectPointLineAndPlane(VECTOR3* pv3Result,PLANE* pPlane,VECTOR3* pv3From,VECTOR3* pv3To, float* pft);
GLOBAL_FUNC_DLL void	  SET_VECTOR3(VECTOR3* pv3,float fVal);
GLOBAL_FUNC_DLL void	  VECTOR3_ADD_VECTOR3(VECTOR3* pv3Result,VECTOR3* pv3Arg1,VECTOR3* pv3Arg2);
GLOBAL_FUNC_DLL void	  VECTOR3_SUB_VECTOR3(VECTOR3* pv3Result,VECTOR3* pv3Arg1,VECTOR3* pv3Arg2);
GLOBAL_FUNC_DLL void	  VECTOR3_MUL_VECTOR3(VECTOR3* pv3Result,VECTOR3* pv3Arg1,VECTOR3* pv3Arg2);
GLOBAL_FUNC_DLL void	  VECTOR3_DIV_VECTOR3(VECTOR3* pv3Result,VECTOR3* pv3Arg1,VECTOR3* pv3Arg2);

GLOBAL_FUNC_DLL void	  VECTOR3_ADDEQU_VECTOR3(VECTOR3* pv3Result,VECTOR3* pv3Arg1);
GLOBAL_FUNC_DLL void	  VECTOR3_SUBEQU_VECTOR3(VECTOR3* pv3Result,VECTOR3* pv3Arg1);
GLOBAL_FUNC_DLL void	  VECTOR3_MULEQU_VECTOR3(VECTOR3* pv3Result,VECTOR3* pv3Arg1);
GLOBAL_FUNC_DLL void	  VECTOR3_DIVEQU_VECTOR3(VECTOR3* pv3Result,VECTOR3* pv3Arg1);

GLOBAL_FUNC_DLL void	  VECTOR3_ADDEQU_FLOAT(VECTOR3* pv3Result,float fVal);
GLOBAL_FUNC_DLL void	  VECTOR3_SUBEQU_FLOAT(VECTOR3* pv3Result,float fVal);
GLOBAL_FUNC_DLL void	  VECTOR3_MULEQU_FLOAT(VECTOR3* pv3Result,float fVal);
GLOBAL_FUNC_DLL void	  VECTOR3_DIVEQU_FLOAT(VECTOR3* pv3Result,float fVal);

GLOBAL_FUNC_DLL void	  VECTOR3_ADD_FLOAT(VECTOR3* pv3Result,VECTOR3* pv3Arg1,float fVal);
GLOBAL_FUNC_DLL void	  VECTOR3_SUB_FLOAT(VECTOR3* pv3Result,VECTOR3* pv3Arg1,float fVal);
GLOBAL_FUNC_DLL void	  VECTOR3_MUL_FLOAT(VECTOR3* pv3Result,VECTOR3* pv3Arg1,float fVal);
GLOBAL_FUNC_DLL void	  VECTOR3_DIV_FLOAT(VECTOR3* pv3Result,VECTOR3* pv3Arg1,float fVal);

GLOBAL_FUNC_DLL void	  FindNearestVertexOnLine(VECTOR3* OUT pv3Result ,float* OUT pT, VECTOR3* IN pv3From, VECTOR3* IN pv3To, VECTOR3* IN pv3Point);
GLOBAL_FUNC_DLL float	  VECTOR3Length(VECTOR3* pv3);
GLOBAL_FUNC_DLL void	  MakeViewVolume(VIEW_VOLUME* pViewVolume,VECTOR3* pv3From,VECTOR3* pv3To,VECTOR3* pv3Up, float fov, float fHVRatio, float fFar);
GLOBAL_FUNC_DLL void	  MakeViewVolumeWithOrtho(VIEW_VOLUME* pViewVolume,VECTOR3* pv3From,VECTOR3* pv3To,VECTOR3* pv3Up, float fWidth, float fHVRatio, float fFar);


GLOBAL_FUNC_DLL void	  CrossProduct(VECTOR3* r,VECTOR3* u,VECTOR3* v);
GLOBAL_FUNC_DLL float	  DotProduct(VECTOR3* pv3_0,VECTOR3* pv3_1);
GLOBAL_FUNC_DLL void	  Normalize(VECTOR3* OUT vn,VECTOR3* IN v);
GLOBAL_FUNC_DLL float	  CalcAngle(VECTOR3* vec1, VECTOR3* vec2);
GLOBAL_FUNC_DLL float	  CalcCosAngle(VECTOR3* vec1, VECTOR3* vec2);
GLOBAL_FUNC_DLL float	  CalcDistance(VECTOR3* pv3Start,VECTOR3* pv3End);
GLOBAL_FUNC_DLL BOOL	  IsCollisionSpherAndSphere(VECTOR3* pv3PointTarget,float fRsTarget,VECTOR3* pv3PointSrc,float fRsSrc);
GLOBAL_FUNC_DLL BOOL	  SetIdentityMatrix(MATRIX4* pMat);
GLOBAL_FUNC_DLL BOOL	  SetInverseMatrix(MATRIX4* q, MATRIX4* a );

GLOBAL_FUNC_DLL void	  TransformVector3_VPTR1(VECTOR3* pv3SrcDest, MATRIX4* pMat,DWORD dwNum);
GLOBAL_FUNC_DLL void	  TransformVector3_VPTR2(VECTOR3* pv3Dest,VECTOR3* pv3Src, MATRIX4* pMat,DWORD dwNum);
GLOBAL_FUNC_DLL void	  TransformVector4_VPTR2(VECTOR4* pv3Dest,VECTOR4* pv3Src, MATRIX4* pMat,DWORD dwNum);
GLOBAL_FUNC_DLL void	  TransformV3TOV4(VECTOR4* pv3Dest,VECTOR3* pv3Src, MATRIX4* pMat,DWORD dwNum);
GLOBAL_FUNC_DLL void	  TransformNSizeVector3_VPTR1(char* pv3SrcDest,DWORD dwSize,MATRIX4* pMat,DWORD dwNum);

GLOBAL_FUNC_DLL void	  SetRotationMatrix(MATRIX4* mat, VECTOR3* vDir, FLOAT fRads );
GLOBAL_FUNC_DLL void	  SetRotationXMatrix(MATRIX4* pMat, float fRad);
GLOBAL_FUNC_DLL void	  SetRotationYMatrix(MATRIX4* pMat, float fRad);
GLOBAL_FUNC_DLL void	  SetRotationZMatrix(MATRIX4* pMat, float fRad);
GLOBAL_FUNC_DLL void	  MatrixMultiply3(MATRIX4* pResult,MATRIX4* mat1, MATRIX4* mat2, MATRIX4* mat3);
GLOBAL_FUNC_DLL void	  MatrixMultiply2(MATRIX4* pResult, MATRIX4* mat1, MATRIX4* mat2);
GLOBAL_FUNC_DLL void	  TranslateMatrix(MATRIX4* m, VECTOR3* vec);
GLOBAL_FUNC_DLL void	  SetScaleMatrix(MATRIX4* m, VECTOR3* v );
GLOBAL_FUNC_DLL void	  TransposeMatrix(MATRIX4* pMat);
GLOBAL_FUNC_DLL void	  SetLightTexMatrix(MATRIX4* pMat,VECTOR3* pv3Pos,float fRs);
GLOBAL_FUNC_DLL VOID	  MatrixFromQuaternion(MATRIX4* mat, QUARTERNION* q);
GLOBAL_FUNC_DLL VOID	  QuaternionSlerp(QUARTERNION* qQ,QUARTERNION* aQ,QUARTERNION* bQ,float fAlpha);
GLOBAL_FUNC_DLL BOOL	  CalcPlaneEquation(PLANE* pPlane,VECTOR3* pv3Tri);
GLOBAL_FUNC_DLL BOOL	  ComparePlane(PLANE* pTarget,PLANE* pSrc);
GLOBAL_FUNC_DLL BOOL	  CalcBoundingMesh(VECTOR3* pv3,DWORD dwVertexNum,COLLISION_MESH_OBJECT_DESC* pDesc);
GLOBAL_FUNC_DLL void	  CreateVertexListWithBox(char* pv3Array36,DWORD dwVertexSize,VECTOR3* pv3Oct);
GLOBAL_FUNC_DLL BOOL	  IsCollisionSphereAndPlane(VECTOR3* pv3Point,float fRs,PLANE* pPlane);
GLOBAL_FUNC_DLL void	  CalcXZ(float* px,float* pz,DWORD dwPosX,DWORD dwPosY,float top,float left,float bottom,float right,float width,float height,DWORD dwFacesNumX,DWORD dwFacesNumY);

GLOBAL_FUNC_DLL void	  Set2PowValueLess(DWORD* pdwOut,DWORD dwIn);
GLOBAL_FUNC_DLL BOOL	  TransformBoundingSphere(BOUNDING_SPHERE* pDest,BOUNDING_SPHERE* pSrc,DWORD dwFlag);
GLOBAL_FUNC_DLL DWORD	  COLORtoDWORD(float r,float g,float b,float a);
GLOBAL_FUNC_DLL void	  QuaternionFromRotation(QUARTERNION* pQ,VECTOR3* v, float fTheta );
GLOBAL_FUNC_DLL void	  QuaternionMultiply(QUARTERNION* pQResult,QUARTERNION* pQA,QUARTERNION* pQB);

GLOBAL_FUNC_DLL BOOL	  AddBoundingMesh(COLLISION_MESH_OBJECT_DESC* pDesc,COLLISION_MESH_OBJECT_DESC** ppDesc,DWORD dwNum);
GLOBAL_FUNC_DLL void	  DWORDtoCOLOR(DWORD color,float* r,float* g,float* b,float* a);
GLOBAL_FUNC_DLL	void	  PhysiqueTransform(BYTE* pVertexResult,PHYSIQUE_VERTEX* pPhyVertex,DWORD dwVertexNum,DWORD dwSize,MATRIX4* pMatrixEntry);
GLOBAL_FUNC_DLL	void	  PhysiqueTransformPosAndWriteUV(BYTE* pVertexResult,PHYSIQUE_VERTEX* pPhyVertex,TVERTEX* ptv,DWORD dwVertexNum,DWORD dwSize,MATRIX4* pMatrixEntry);
GLOBAL_FUNC_DLL void	  PhysiqueTransformPosAndNormalAndUV(BYTE* pVertexResult,PHYSIQUE_VERTEX* pPhyVertex,TVERTEX* ptv,DWORD dwVertexNum,DWORD dwSize,MATRIX4* pMatrixEntry);
GLOBAL_FUNC_DLL	void	  PhysiqueTransformPosAndNormal(BYTE* pVertexResult,PHYSIQUE_VERTEX* pPhyVertex,DWORD dwVertexNum,DWORD dwSize,MATRIX4* pMatrixEntry);


GLOBAL_FUNC_DLL BOOL	  RemoveCRLF(char* pStr,DWORD dwLen);
GLOBAL_FUNC_DLL BOOL	  IntersectTriangle(VECTOR3* pv3IntersectPoint,VECTOR3* orig,VECTOR3* dir,VECTOR3* v0,VECTOR3* v1, VECTOR3* v2,float* t, float* u, float* v,BOOL bCullBackface);
GLOBAL_FUNC_DLL void	  CalcRay(VECTOR3* pv3Pos,VECTOR3* pv3Dir,DWORD dwPosX,DWORD dwPosY,DWORD dwWidth,DWORD dwHeight,MATRIX4* pProj,MATRIX4* pView);
GLOBAL_FUNC_DLL BOOL	  IsCollisionMeshAndRay(VECTOR3* pv3IntersectPoint,float* pfDist,COLLISION_MESH_OBJECT_DESC* pColMeshDesc,VECTOR3* pv3Pos,VECTOR3* pv3Dir);
GLOBAL_FUNC_DLL BOOL	  IsCollisionMeshWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,COLLISION_MESH_OBJECT_DESC* pColMeshDesc,MATRIX4* pMatView,MATRIX4* pMatProj,POINT* ptCursor,SHORT_RECT* pClipRect);
GLOBAL_FUNC_DLL void	  SetTransformBoundingMesh(COLLISION_MESH_OBJECT_DESC* pDest,COLLISION_MESH_OBJECT_DESC* pSrc,MATRIX4* pMat);
GLOBAL_FUNC_DLL void	  ResetTM(NODE_TM* pTM);
GLOBAL_FUNC_DLL BOOL	  SetViewMatrix(MATRIX4* pMat,VECTOR3* pv3From,VECTOR3* pv3To,VECTOR3* pv3Up);
GLOBAL_FUNC_DLL BOOL	  SetProjectionMatrix(MATRIX4* pMat,float fFOV,float fAspect,float fNearPlane,float fFarPlane);
GLOBAL_FUNC_DLL void	  SetBitmapFromTexturePlane(char* pBits,DWORD dwPitch,TEXTURE_PLANE* pTexPlane,DWORD dwTexPlaneNum);
GLOBAL_FUNC_DLL BOOL	  WriteTGA(char* szFileName,char* pSrc,DWORD dwWidth,DWORD dwHeight,DWORD dwPitch,DWORD bpp);

GLOBAL_FUNC_DLL BOOL	  ResizeImage(char* pDest,DWORD dwDestWidth,DWORD dwDestHeight,char* pSrc,DWORD dwSrcWidth,DWORD dwSrcHeight,DWORD dwBytesPerPixel);
GLOBAL_FUNC_DLL DWORD	  CullBackFace(WORD* pFaceIndexList,TRI_FACE* pFaceEntry,VECTOR3* pv3Eye,DWORD dwFacesNum);
GLOBAL_FUNC_DLL DWORD	  BuildIndexBufferWithFaceIndexList(WORD* pIndexBufferEntry,TRI_FACE* pFaceEntry,WORD* pwFaceIndexEntry,VECTOR3* pv3Eye,DWORD dwFacesNum);
GLOBAL_FUNC_DLL DWORD	  BuildIndexBufferWithFaceIndexListForShadow(WORD* pIndexBufferEntry,TRI_FACE* pFaceEntry,WORD* pwFaceIndexEntry,VECTOR3* pv3Eye,BOUNDING_SPHERE* pSphere,DWORD dwFacesNum);
GLOBAL_FUNC_DLL void	  BuildFaceList(TRI_FACE* pFacesEntry,VECTOR3* pv3,WORD* pIndex,DWORD dwFacesNum);
GLOBAL_FUNC_DLL BOOL	  MakeViewVolumeIncludingSphere(VIEW_VOLUME* pViewVolume,VECTOR3* pv3From, BOUNDING_SPHERE* pBS, float fFar);
GLOBAL_FUNC_DLL void	  MakeViewVolumeWithSelectRange(VIEW_VOLUME* pViewVolume,VECTOR3* pv3From, VECTOR3* pv3LeftUp, VECTOR3* pv3RightUp, VECTOR3* pv3RightDown, VECTOR3* pv3LeftDown);
GLOBAL_FUNC_DLL void	  ConvertFromTo2YPR(VECTOR3* pVEC, VECTOR3* pOutYPR);
GLOBAL_FUNC_DLL	void	  RotatePositionWithPivot( VECTOR3* pOutPos, VECTOR3* pPivot, VECTOR3* pPosition, VECTOR3* pCameraAnglePYR, float fRotateYaw, float fRotatePitch);

GLOBAL_FUNC_DLL	float	  DistanceFromSphereAndPlane( BOUNDING_SPHERE* pSphere, PLANE* pPlane);
GLOBAL_FUNC_DLL	BOOL	  IsCollisionSphereAndBox( BOUNDING_SPHERE* pSphere, BOUNDING_BOX* pBox);

// ItemQueue


GLOBAL_FUNC_DLL ITEMQ_HANDLE	 	CreateItemQueue();
GLOBAL_FUNC_DLL void			 	ReleaseItemQueue(ITEMQ_HANDLE hQ);

GLOBAL_FUNC_DLL BOOL			 	BeginInitialize(ITEMQ_HANDLE pQ,DWORD dwMaxTypeNum);
GLOBAL_FUNC_DLL BOOL			 	SetType(ITEMQ_HANDLE pQ,DWORD dwTypeID,DWORD dwItemSize,void* pFunc,DWORD dwMaxNum);
GLOBAL_FUNC_DLL void			 	EndInitialize(ITEMQ_HANDLE pQ);

GLOBAL_FUNC_DLL BOOL			 	PushItem(ITEMQ_HANDLE pQ,DWORD dwTypeID,char* pItem);
GLOBAL_FUNC_DLL char*			 	GetItemSeq(ITEMQ_HANDLE pQ,DWORD* pdwTypeID,void** ppFunc);
GLOBAL_FUNC_DLL char*			 	GetItem(ITEMQ_HANDLE pQ,void** ppFunc,DWORD dwTypeID,DWORD dwSeqIndex);

GLOBAL_FUNC_DLL void			 	Clear(ITEMQ_HANDLE pQ);


GLOBAL_FUNC_DLL DWORD			  	GetItemNum(ITEMQ_HANDLE pQ);
GLOBAL_FUNC_DLL DWORD			 	GetItemNumWithType(ITEMQ_HANDLE pQ,DWORD dwTypeID);
GLOBAL_FUNC_DLL void			 	ResetReadPosition(ITEMQ_HANDLE pQ);
	


// LookAsideList

GLOBAL_FUNC_DLL STMPOOL_HANDLE	 	CreateStaticMemoryPool();
GLOBAL_FUNC_DLL void			 	ReleaseStaticMemoryPool(STMPOOL_HANDLE pool);
GLOBAL_FUNC_DLL BOOL			 	InitializeStaticMemoryPool(STMPOOL_HANDLE pool,DWORD dwUnitSize,DWORD dwDefaultCommitNum,DWORD dwMaxNum);
// Various Bytes HashTable



GLOBAL_FUNC_DLL void*			 	VBHGetSearchHandle(VBHASH_HANDLE pHash,void* pKeyData,DWORD dwSize);
GLOBAL_FUNC_DLL void*			 	VBHGetKeyPtr(void* pSearchHandle);

GLOBAL_FUNC_DLL	VBHASH_HANDLE	 	VBHCreate();
GLOBAL_FUNC_DLL	BOOL			 	VBHInitialize(VBHASH_HANDLE pHash,DWORD dwMaxBucketNum,DWORD dwMaxSize,DWORD dwMaxItemNum);
GLOBAL_FUNC_DLL	void			 	VBHRelease(VBHASH_HANDLE pHash);
GLOBAL_FUNC_DLL	DWORD			 	VBHGetMaxBucketNum(VBHASH_HANDLE pHash);
GLOBAL_FUNC_DLL void			 	VBHDeleteAll(VBHASH_HANDLE pHash);

GLOBAL_FUNC_DLL DWORD			 	QBHSelect(QBHASH_HANDLE pHash,void** ppHashHandle,DWORD OUT* pItems,DWORD dwMaxItemNum,DWORD dwKeyData);
GLOBAL_FUNC_DLL void*			 	QBHInsert(QBHASH_HANDLE pHash,DWORD dwItem,DWORD dwKeyData);
GLOBAL_FUNC_DLL BOOL			  	QBHDelete(QBHASH_HANDLE pHash,void* pBucket);

GLOBAL_FUNC_DLL	QBHASH_HANDLE	 	QBHCreate();
GLOBAL_FUNC_DLL BOOL			 	QBHInitialize(QBHASH_HANDLE pHash,DWORD dwMaxBucketNum,DWORD dwMaxItemNum);
GLOBAL_FUNC_DLL	void			 	QBHRelease(QBHASH_HANDLE pHash);
GLOBAL_FUNC_DLL	DWORD			 	QBHGetMaxBucketNum(QBHASH_HANDLE pHash);
GLOBAL_FUNC_DLL	void			 	QBDeleteAll(QBHASH_HANDLE pHash);
GLOBAL_FUNC_DLL DWORD			 	QBHGetAllItem(VBHASH_HANDLE pHash,DWORD* pdwItemList,DWORD dwMaxItemNum);



// IndexCreattor
GLOBAL_FUNC_DLL		INDEXCR_HANDLE		 	ICCreate();
GLOBAL_FUNC_DLL		void				 	ICRelease(INDEXCR_HANDLE pIC);
GLOBAL_FUNC_DLL		DWORD				 	ICAllocIndex(INDEXCR_HANDLE pIC);
GLOBAL_FUNC_DLL		void				   	ICFreeIndex(INDEXCR_HANDLE pIC,DWORD dwIndex);
GLOBAL_FUNC_DLL		BOOL				  	ICInitialize(INDEXCR_HANDLE pIC,DWORD dwMaxNum);
GLOBAL_FUNC_DLL		DWORD				 	ICGetIndexNum(INDEXCR_HANDLE pIC);

// ItemIndexTable
GLOBAL_FUNC_DLL		ITEMTABLE_HANDLE	 	ITCreate();
GLOBAL_FUNC_DLL		void				 	ITRelease(ITEMTABLE_HANDLE pIT);

GLOBAL_FUNC_DLL		BOOL				 	ITInitialize(ITEMTABLE_HANDLE pIT,DWORD dwMaxItemNum);
GLOBAL_FUNC_DLL		DWORD				 	ITAddItem(ITEMTABLE_HANDLE pIT,void* pVoid);
GLOBAL_FUNC_DLL		BOOL				 	ITDeleteItem(ITEMTABLE_HANDLE pIT,DWORD dwItemIndex);
GLOBAL_FUNC_DLL		DWORD				 	ITGetItemNum(ITEMTABLE_HANDLE pIT);
GLOBAL_FUNC_DLL		void*				 	ITGetItem(ITEMTABLE_HANDLE pIT,DWORD dwItemIndex);
GLOBAL_FUNC_DLL		void*				 	ITGetItemSequential(ITEMTABLE_HANDLE pIT,DWORD dwSeqIndex);

GLOBAL_FUNC_DLL		DWORD				 	RemoveExt(char* szResultName,DWORD dwBufferLen, char* szFileName);
GLOBAL_FUNC_DLL		DWORD				 	GetNameRemovePath(char* dest,char* src);
GLOBAL_FUNC_DLL		DWORD				 	GetNamePath(char* szResultPath,char* szFileName);
GLOBAL_FUNC_DLL		DWORD				 	GetExt(char* szResultName,char* szFileName);


// BinSearchAE
GLOBAL_FUNC_DLL		BSAE_HANDLE			 	BSAECreate();
GLOBAL_FUNC_DLL		DWORD				 	BSAEGetItemNum(BSAE_HANDLE pBSAE);
GLOBAL_FUNC_DLL		BOOL				 	BSAEInitialize(BSAE_HANDLE pBSAE,DWORD dwMaxItemNum);
GLOBAL_FUNC_DLL		ITEM_CONTAINER_AE*	 	BSAEInsertItem(BSAE_HANDLE pBSAE,DWORD dwKey,void* pItem);
GLOBAL_FUNC_DLL		BOOL				 	BSAEDeleteItem(BSAE_HANDLE pBSAE,ITEM_CONTAINER_AE* pItemContainer);
GLOBAL_FUNC_DLL		void*				 	BSAESearchItem(BSAE_HANDLE pBSAE,DWORD dwKey);
GLOBAL_FUNC_DLL		ITEM_CONTAINER_AE*	 	BSAESearchItemContainer(BSAE_HANDLE pBSAE,DWORD dwKey);
GLOBAL_FUNC_DLL		void				 	BSAERelease(BSAE_HANDLE pBSAE);

// various heap
GLOBAL_FUNC_DLL		VHEAP_HANDLE		 	VHPCreateHeap();
GLOBAL_FUNC_DLL		void				 	VHPHeapCheck(VHEAP_HANDLE pVHeap);
GLOBAL_FUNC_DLL		void				 	VHPLeakCheck(VHEAP_HANDLE pVHeap);
GLOBAL_FUNC_DLL		BOOL				 	VHPInitialize(VHEAP_HANDLE pVHeap,void* pMassMemory,DWORD dwMassMemorySize,DWORD dwMaxBlockNum,DWORD dwUnitSize,DWORD dwAlignUnitNum,BOOL bEnableDebug);

GLOBAL_FUNC_DLL		void*				 	VHPAlloc(VHEAP_HANDLE pVHeap,DWORD* pdwErrorCode,DWORD dwSize,BOOL bAlign);
GLOBAL_FUNC_DLL		void* 				 	VHPAllocUnit(VHEAP_HANDLE pVHeap,DWORD* pdwErrorCode,DWORD dwUnitNum,DWORD* pdwSize,BOOL bAlign);
GLOBAL_FUNC_DLL		void				 	VHPFree(VHEAP_HANDLE pVHeap,void* pMem);
GLOBAL_FUNC_DLL		void*				 	VHPDBGAlloc(VHEAP_HANDLE pVHeap,DWORD* pdwErrorCode,DWORD dwSize,BOOL bAlign);
GLOBAL_FUNC_DLL		void* 				 	VHPDBGAllocUnit(VHEAP_HANDLE pVHeap,DWORD* pdwErrorCode,DWORD dwUnitNum,DWORD* pdwSize,BOOL bAlign);
GLOBAL_FUNC_DLL		void				  	VHPDBGFree(VHEAP_HANDLE pVHeap,void* pMem);
GLOBAL_FUNC_DLL		void				 	VHPReleaseHeap(VHEAP_HANDLE pVHeap);



// DPCQ
GLOBAL_FUNC_DLL		DPCQ_HANDLE			 	DPCQCreate();
GLOBAL_FUNC_DLL		BOOL				 	DPCQInitialize(DPCQ_HANDLE pDPCQHandle,DWORD dwMaxBufferSize);
GLOBAL_FUNC_DLL		BOOL				 	DPCQPushDPC(DPCQ_HANDLE pDPCQHandle,void* pStdcallFunc,WORD wArgNum,DWORD* pArgList,int* pRetAddr,int iOrder);
GLOBAL_FUNC_DLL		void				 	DPCQProcess(DPCQ_HANDLE pDPCQHandle);
GLOBAL_FUNC_DLL		void				 	DPCQProcessWithAscSort(DPCQ_HANDLE pDPCQHandle);

GLOBAL_FUNC_DLL		void				 	DPCQClear(DPCQ_HANDLE pDPCQHandle);
GLOBAL_FUNC_DLL		void				 	DPCQRelease(DPCQ_HANDLE pDPCQHandle);

// check performance
GLOBAL_FUNC_DLL void BeginAnalyze(PERFORMANCE_CONTEXT* pCtx);
GLOBAL_FUNC_DLL BOOL EndAnalyze(PERFORMANCE_CONTEXT* pCtx);

GLOBAL_FUNC_DLL void   GetEIP(DWORD* pdwEIP);

#define RDTSC_BEGIN		__asm	rdtsc			\
						__asm	push	eax		\
						__asm	push	edx		\


#define RDTSC_END		__asm	rdtsc			\
						__asm	pop		ebx		\
						__asm	pop		ecx		\
						__asm	sub		eax,ecx	\
						__asm	sub		edx,ebx	\
						__asm	int		3		\


//#endif
GLOBAL_FUNC_DLL BOOL   IsValidFloat(float f);
GLOBAL_FUNC_DLL BOOL   IsValidVector3(VECTOR3* pv3,DWORD dwNum);
GLOBAL_FUNC_DLL BOOL   IsValidTri(VECTOR3* pv3Point_0,VECTOR3* pv3Point_1,VECTOR3* pv3Point_2);

GLOBAL_FUNC_DLL void   QSortIntASC(SORT_ITEM_INT* pArray,DWORD dwNum);
GLOBAL_FUNC_DLL void   QSortIntDSC(SORT_ITEM_INT* pArray,DWORD dwNum);
GLOBAL_FUNC_DLL void   QSortFloatASC(SORT_ITEM_FLOAT* pArray,DWORD dwNum);
GLOBAL_FUNC_DLL void   QSortFloatDSC(SORT_ITEM_FLOAT* pArray,DWORD dwNum);


GLOBAL_FUNC_DLL void   CharToSmallASCII(char* szDest,char* szStr,DWORD dwLen);
GLOBAL_FUNC_DLL void   CharToSmallASCIILen(char* szDest,char* szStr);


GLOBAL_FUNC_DLL void   LinktToSortLink(SORT_LINK** ppHead,SORT_LINK** ppTail,SORT_LINK* pNew);
GLOBAL_FUNC_DLL void   UnLinkFromSortLink(SORT_LINK** ppHead,SORT_LINK** ppTail,SORT_LINK* pDel);


GLOBAL_FUNC_DLL STRCPOOL_HANDLE   CreateResourcePool();
GLOBAL_FUNC_DLL BOOL			  RCPInitialize(STRCPOOL_HANDLE pHandle,DWORD dwMaxItemNum,RESOURCR_ALLOC_FUNC pAllocFunc,RESOURCR_FREE_FUNC pFreeFunc);
GLOBAL_FUNC_DLL void*			  RCPAlloc(STRCPOOL_HANDLE pHandle,BOOL* pbEnable,DWORD* pdwResourceHandle,void* pArg1,void* pArg2,void* pArg3,DWORD dwSize);
GLOBAL_FUNC_DLL void			  RCPFree(STRCPOOL_HANDLE pHandle,DWORD dwResourceHandle);
GLOBAL_FUNC_DLL void			  RCPFreeAll(STRCPOOL_HANDLE pHandle);
GLOBAL_FUNC_DLL void			  RCPRelease(STRCPOOL_HANDLE pHandle);


GLOBAL_FUNC_DLL void   Convert8BitTo2BitImage(char* p2Bits,char* p8Bits,DWORD dwWidth,DWORD dwHeight);
GLOBAL_FUNC_DLL void   Convert2BitTo8BitImage(char* p8Bits,char* p2Bits,DWORD dwWidth,DWORD dwHeight);
GLOBAL_FUNC_DLL void   Convert2BitTo16BitImageWithPitch(char* p16Bits,char* p2Bits,DWORD dwWidth,DWORD dwHeight,DWORD dwPitch);
GLOBAL_FUNC_DLL void   SetPixel2BitImage(char* p2Bits,DWORD dwPx,DWORD dwPy,DWORD dwWidth,DWORD dwHeight,DWORD dwColor);
GLOBAL_FUNC_DLL void   SubPixel2BitImage(char* p2Bits,DWORD dwPx,DWORD dwPy,DWORD dwWidth,DWORD dwHeight,DWORD dwColor);
GLOBAL_FUNC_DLL void   AddPixel2BitImage(char* p2Bits,DWORD dwPx,DWORD dwPy,DWORD dwWidth,DWORD dwHeight,DWORD dwColor);
GLOBAL_FUNC_DLL void   Create8BitsAttenuationMap(char* p8Bits,DWORD dwWidth,DWORD dwHeight,DWORD dwRS);

GLOBAL_FUNC_DLL void   SetOutLineBorder4Bytes(char* pBits,DWORD dwWidth,DWORD dwHeight,DWORD dwPitch,DWORD dwBorderColor);
GLOBAL_FUNC_DLL void   SetOutLineBorder2Bytes(char* pBits,DWORD dwWidth,DWORD dwHeight,DWORD dwPitch,DWORD wBorderColor);






