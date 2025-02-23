#pragma once

#include "../4DyuchiGRX_common/IGeometry.h"
#include "../4DyuchiGRX_common/IRenderer.h"
#include "../4DyuchiGRX_common/IExecutive.h"
#include "../4DyuchiGRX_common/IFileStorage.h"
#include "../4DyuchiGXGFunc/global.h"


#include "GxLight.h"
#include "map.h"
#include "GXDecal.h"
#include "4DyuchiGRX_myself97/MClipper.h"


struct MODEL_FILE_DESC
{
	DWORD			dwRefCount;
	void*			pSearchHandle;
	CoGXObject*		pGXObject;
	char			szFileName[MAX_NAME_LEN];


};

class CoGXObject;
class CoExecutive : public I4DyuchiGXExecutive
{
	DWORD						m_dwRefCount;

	I4DyuchiFileStorage*		m_pFileStorage;
	I4DyuchiGXRenderer*			m_pRenderer;
	I4DyuchiGXGeometry*			m_pGeometry;
	
	DWORD						m_dwViewportIndex;

	DWORD						m_dwInitialTick;
	DWORD						m_dwPrvFrameCount;
	DWORD						m_dwFrameCount;
	DWORD						m_dwTicksPerFrame;
	DWORD						m_dwGameFPS;

	DWORD						m_dwTickIncrease;			// 최근 게임프레임에서 다음게임프레임 전까지 경과한 틱, 0에서 m_dwTicksPerFrame 사이값이 들어있어야 한다. Process에서 갱신.

	BOOL						m_bCanDeleteObject;

	DWORD						m_dwMaxObjectNum;
	DWORD						m_dwMaxTriggerNum;
	DWORD						m_dwMaxDecalNum;

	DWORD						m_dwMaxMapObjectsNum;


	DWORD*						m_pdwIndexList;
	
	DWORD						m_dwRenderMode;


	DPCQ_HANDLE					m_pDPCQ;
	STMPOOL_HANDLE				m_pStaticPoolGXObject;
	STMPOOL_HANDLE				m_pStaticPoolGXLight;
	STMPOOL_HANDLE				m_pStaticPoolGXDecal;

	ITEMTABLE_HANDLE			m_pIndexItemTableGXObject;
	ITEMTABLE_HANDLE			m_pIndexItemTableGXLight;
	ITEMTABLE_HANDLE			m_pIndexItemTableGXTrigger;
	ITEMTABLE_HANDLE			m_pIndexItemTableGXDecal;
	
		
	QBHASH_HANDLE				m_pIDHash;
	VBHASH_HANDLE				m_pFileNameHash;

	I3DModel*					m_pModelList[SYMBOL_TYPE_NUM];

//	CoGXObject*					m_pTracedGXObject;
	

	
	
	
	DWORD						m_dwCurrentGXLightNum;
	CoGXLight**					m_ppGXLightList;
	CoGXObject*					m_pCameraAttatchedGXObject;
	float						m_fDistCameraFront;

	FLOAT_RECT					m_frcMapBoundingRect;
	CGXMap*						m_pgxMap;
	CoGXObject*					m_pDummyGXObject;
	CoGXLight*					m_pDummyGXLight;
	CGXDecal*					m_pDummyGXDecal;
	
	I3DModel*					m_pPreLoadedModelList;
	DWORD						m_dwPreLoadedModelNum;

	void*						m_pPackFileHandleList[MAX_PACK_FILE_NUM];
	DWORD						m_dwPackFilesNum;



	int							LoadModelData(MODEL_HANDLE* pModelHandle,void* pFP);
	int							PreLoadModelData(void* pFP);

	DWORD						LoadGXObject(MODEL_HANDLE* pModelHandle,DWORD dwMaxModelNum,char* szFileName);
	CoGXObject*					AddGXObject(MODEL_HANDLE* pModelHandle,DWORD dwModelNum,GXSchedulePROC pProc,DWORD dwFlag);
	
	// gxobject에서 충돌처리할때 쓰일 헬퍼.		2002/05/29
	DBG_COLLISION_INFO			m_CollisionInfo;

	DWORD						m_dwBuildTreeFlag;		// octree, bsp tree 등의 존재 여부. BUILD_TREE_FLAG타입.

	HMODULE						m_hGeometry;
	HMODULE						m_hRenderer;
	HMODULE						m_hStorage;


	void						SetCanDelete()				{m_bCanDeleteObject = TRUE;}
	void						SetCannotDelete()			{m_bCanDeleteObject = FALSE;}
	BOOL						IsCanDeleteObject()			{return m_bCanDeleteObject;}

	// Decal;
//	CGXDecal*					m_pDecalBuff;
//	int*						m_pDecalClipperIndexBuff;
	DWORD						m_dwCurrentDecalCount;
	
	DWORD						m_dwRenderFrameCount;

	AfterInterpolationCallBack	m_pfAfterInterpolationCallBack;

//	void						RenderDecal();
	
	BOOL							Initialize(I4DyuchiGXGeometry* pGeometry,I4DyuchiGXRenderer* pRenderer,HWND hWnd,DISPLAY_INFO* pInfo,DWORD dwMaxObjectNum,DWORD dwMaxLightNum,DWORD dwMaxTriggerNum,DWORD dwViewportNum, DWORD dwMaxDecalNum,ErrorHandleProc pErrorHandleFunc);
	BOOL							InitializeFileStorageCommon(DWORD dwMaxFileNum,DWORD dwMaxFileHandleNumAtSameTime,DWORD dwMaxFileNameLen,FILE_ACCESS_METHOD accessMethod,PACKFILE_NAME_TABLE* pPackFileList,DWORD dwNum);
public:
	void						ResourceCheck();
#ifdef _DEBUG
	void						CheckHandle(GXMAP_OBJECT_HANDLE gxh);
	void						CheckGXLightsList(CoGXLight* pInLight);
	void						CheckGXObjectsList(CoGXObject* pInObject);
	void						CheckGXTriggersList(CoGXEventTrigger* pInTrigger);
	void						CheckGXDecalsList(CGXDecal* pInDecal);
#endif

//	MCollisionHelper*			GetCollisionHelper(){return &m_Collision;}
	I4DyuchiGXRenderer*			INL_GetRenderer()	{return m_pRenderer;}
	I4DyuchiGXGeometry*			INL_GetGeometry()	{return m_pGeometry;}
	CGXMap*						GetGXMap()	{return m_pgxMap;}
	void						GetCameraDesc(CAMERA_DESC*	pCameraDesc) {m_pGeometry->GetCameraDesc(pCameraDesc,m_dwViewportIndex);}
	void						GetCameraEyePlane(PLANE* pPlane) {m_pGeometry->GetCameraEyePlane(pPlane,m_dwViewportIndex);}

	I3DModel*					GetSymbolModel(SYMBOL_TYPE type) {return m_pModelList[type];}

	QBHASH_HANDLE				GetIDHash()	{return m_pIDHash;}
	void						DeleteModelFileDesc(MODEL_FILE_DESC* pModelFileDesc);
//	CoGXObject*					GetTracedGXObject() {return m_pTracedGXObject;}

	// external interface
    STDMETHODIMP			QueryInterface(REFIID, PPVOID);
	STDMETHODIMP_(ULONG)	AddRef(void);
    STDMETHODIMP_(ULONG)	Release(void);

	I4DyuchiGXRenderer*		GetRenderer();
	I4DyuchiGXGeometry*		GetGeometry();

	HRESULT					GetRenderer(I4DyuchiGXRenderer** ppRenderer);		// Release필요함
	HRESULT					GetGeometry(I4DyuchiGXGeometry** ppGeometry);		// Release필요함
	HRESULT					GetFileStorage(I4DyuchiFileStorage** ppFileStorage);
		
	BOOL					InitializeWithoutRegistry(char* szGeometryFileName,char* szRendererFileName,HWND hWnd,DISPLAY_INFO* pInfo,DWORD dwMaxObjectNum,DWORD dwMaxLightNum,DWORD dwMaxTriggerNum,DWORD dwViewportNum, DWORD dwMaxDecalNum,ErrorHandleProc pErrorHandleFunc);
	BOOL					Initialize(HWND hWnd,DISPLAY_INFO* pInfo,DWORD dwMaxObjectNum,DWORD dwMaxLightNum,DWORD dwMaxTriggerNum,DWORD dwViewportNum, DWORD dwMaxDecalNum,ErrorHandleProc pErrorHandleFunc);
	BOOL					InitializeFileStorageWithoutRegistry(char* szFileName,DWORD dwMaxFileNum,DWORD dwMaxFileHandleNumAtSameTime,DWORD dwMaxFileNameLen,FILE_ACCESS_METHOD accessMethod,PACKFILE_NAME_TABLE* pPackFileList,DWORD dwNum);
	BOOL					InitializeFileStorage(DWORD dwMaxFileNum,DWORD dwMaxFileHandleNumAtSameTime,DWORD dwMaxFileNameLen,FILE_ACCESS_METHOD accessMethod,PACKFILE_NAME_TABLE* pPackFileList,DWORD dwNum);
	void					SetViewport(DWORD dwViewportIndex);

	DWORD					IsValidHandle(GXMAP_OBJECT_HANDLE gxh);
	DWORD					GXOProcess(GXOBJECT_HANDLE gxo,DWORD dwFrameInc);
	BOOL					GXORender(GXOBJECT_HANDLE gxo);
	
	void					GXOSetCurrentFrame(GXOBJECT_HANDLE gxh,DWORD dwFrame);
	void					GXOSetCurrentMotionIndex(GXOBJECT_HANDLE gxh,DWORD dwMotionIndex);
	BOOL					GXOGetMotionDesc(GXOBJECT_HANDLE gxh,MOTION_DESC* pMotionDesc,DWORD dwMotionIndex,DWORD dwModelIndex);
	DWORD					GXOGetMotionNum(GXOBJECT_HANDLE gxh,DWORD dwModelIndex);
	DWORD					GXOGetCurrentFrame(GXOBJECT_HANDLE gxh);
	DWORD					GXOGetCurrentMotionIndex(GXOBJECT_HANDLE gxh);
	
	void					GXOSetCurrentMaterialIndex(GXOBJECT_HANDLE gxh,DWORD dwMaterialIndex);
	DWORD					GXOGetMaterialNum(GXOBJECT_HANDLE gxh,DWORD dwModelIndex);
	DWORD					GXOGetCurrentMaterialIndex(GXOBJECT_HANDLE gxh);
	

	void					GXOSetPosition(GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos, BOOL bDoInterpolation);
	void					GXOMovePosition(GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos);
	void					GXOGetPosition(GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos);
	void					GXOSetDirection(GXOBJECT_HANDLE gxh,VECTOR3* pv3Axis,float fRad);
	void					GXOSetDirectionFPSStyle(GXOBJECT_HANDLE gxh, VECTOR3* pv3Angles /* in */);

	void					GXOGetInterpolatedPosition( GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos); // 보간되어서 랜더링 되는 포지션을 출력. 실제 GXO의 위치는 아니다. 카메라위치 갱신할 때 쓰면 좋을듯.
	void					GXOChangeDirection(GXOBJECT_HANDLE gxh,float fRad);
	void					GXOGetDirection(GXOBJECT_HANDLE gxh,VECTOR3* pv3Axis,float* pfRad);
	void					GXOGetDirectionFPSStyle(GXOBJECT_HANDLE gxh, VECTOR3* pv3Angle /* out */);
	void					GXOSetScale(GXOBJECT_HANDLE gxh,VECTOR3* pv3Scale);

	void					GXOGetScale(GXOBJECT_HANDLE gxh,VECTOR3* pv3Scale);
	
//	void					GXOSetAcceleratedVelocity( GXOBJECT_HANDLE gxh, VECTOR3* pv3AcceleratedVelocity);
	BOOL					GXOIsLanding( GXOBJECT_HANDLE gxh);
//	void					GXOSetApplyVelocityFlag( GXOBJECT_HANDLE gxh, BOOL bApply);		// 매 프레임마다 속도벡터를 적용할 것인가?


	void					GXOMoveForward(GXOBJECT_HANDLE gxh,float fDistance);
	void					GXOEnableHFieldApply(GXOBJECT_HANDLE gxo);
	void					GXODisableHFieldApply(GXOBJECT_HANDLE gxo);
	BOOL					GXOIsHFieldApply(GXOBJECT_HANDLE gxo);
	
	void					GXOEnableAsEffect(GXOBJECT_HANDLE gxo);
	void					GXODisableAsEffect(GXOBJECT_HANDLE gxo);
	BOOL					GXOIsAsEffect(GXOBJECT_HANDLE gxo);

	void					GXOSetZOrder(GXOBJECT_HANDLE gxo,int iZOrder);

	void					GXOEnableSelfIllumin(GXOBJECT_HANDLE gxo);
	void					GXODisableSelfIllumin(GXOBJECT_HANDLE gxo);
	BOOL					GXOIsEnableSelfIllumin(GXOBJECT_HANDLE gxo);

	void					EstimatedMoveForward(GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos,float fDistance);

	BOOL					GXOAttach(GXOBJECT_HANDLE gxh,GXOBJECT_HANDLE to_gxh,char* szObjName);
	BOOL					GXODetach(GXOBJECT_HANDLE gxh,GXOBJECT_HANDLE child_gxh);
	BOOL					GXOAttachLight(GXOBJECT_HANDLE gxoh,char* szObjName,GXLIGHT_HANDLE gxlh,VECTOR3* pv3Offset,DWORD dwAttachType);
	BOOL					GXODetachLight(GXOBJECT_HANDLE gxoh,GXLIGHT_HANDLE child_gxlh);		
	BOOL					GXOAttachCameraFront(GXOBJECT_HANDLE gxo,float fDist);
	BOOL					GXODetachCameraFront(GXOBJECT_HANDLE gxo);
	ULONG					GXOGetModel(GXOBJECT_HANDLE gxh,I3DModel** ppModel,DWORD* pdwRefIndex,DWORD dwModelIndex);

	GXOBJECT_HANDLE			GXOGetObjectWithScreenCoord(VECTOR3* pv3IntersectPoint,DWORD* pdwModelIndex,DWORD* pdwObjIndex,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag);
	
	DWORD					GXOGetMultipleObjectWithScreenCoordRect(PICK_GXOBJECT_DESC* pPickDesc,DWORD dwMaxDescNum,RECT* pRect,DWORD dwViewportIndex,DWORD dwFlag);
	DWORD					GXOGetMultipleObjectWithScreenCoord(PICK_GXOBJECT_DESC* pPickDesc,DWORD dwMaxDescNum,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag);
	BOOL					GXOReplaceModel(GXOBJECT_HANDLE gxh,DWORD dwModelIndex,char* szFileName);

	
	void					GXOGetCollisionMesh(GXOBJECT_HANDLE gxh,COLLISION_MESH_OBJECT_DESC* pColMeshDesc);

	DWORD					GXOGetObjectIndex(GXOBJECT_HANDLE gxh,char* szObjName,DWORD dwModelIndex);
	BOOL					GXOGetWorldMatrixPerObject(GXOBJECT_HANDLE gxh,MATRIX4* pMatWorld,DWORD dwModelIndex,DWORD dwObjIndex);

	DWORD					GXOGetAllObjectsWitLoadMapScript(GXOBJECT_HANDLE* pObjArray,DWORD dwMaxNum);
	


	void				 	DisableRender(GXMAP_OBJECT_HANDLE gxh);
	void				 	EnableRender(GXMAP_OBJECT_HANDLE gxh);
	void					DisableSchedule(GXMAP_OBJECT_HANDLE gxh);
	void					EnableSchedule(GXMAP_OBJECT_HANDLE gxh);
	BOOL					IsRenderable(GXMAP_OBJECT_HANDLE gxh);

	void					SetPickType(GXMAP_OBJECT_HANDLE	gxh,PICK_TYPE type);
	PICK_TYPE				GetPickType(GXMAP_OBJECT_HANDLE	gxh);

	void					SetRenderFlag(GXMAP_OBJECT_HANDLE gxh,DWORD dwFlag);
	DWORD					GetRenderFlag(GXMAP_OBJECT_HANDLE gxh);
	void					SetAlphaFlag(GXMAP_OBJECT_HANDLE gxh,DWORD dwFlag);
	DWORD					GetAlphaFlag(GXMAP_OBJECT_HANDLE gxh);
	void*					GetData(GXMAP_OBJECT_HANDLE gxh);
	void					SetData(GXMAP_OBJECT_HANDLE gxh,void* pData);
	DWORD					GetID(GXMAP_OBJECT_HANDLE gxh);
	BOOL					SetID(GXMAP_OBJECT_HANDLE gxh,DWORD dwID);
	void 					ReleaseID(GXMAP_OBJECT_HANDLE gxh);
	

	GXMAP_OBJECT_HANDLE 	GetGXMapObjectWithID(DWORD dwID);
	
	void					SetPropertyFlag(GXMAP_OBJECT_HANDLE gxh,DWORD dwFlag);
	DWORD					GetPropertyFlag(GXMAP_OBJECT_HANDLE gxh);

	void					EnablePick(GXMAP_OBJECT_HANDLE gxh);
	void					DisablePick(GXMAP_OBJECT_HANDLE gxh);
	
	void					EnableUpdateShading(GXMAP_OBJECT_HANDLE gxh);
	void					DisableUpdateShading(GXMAP_OBJECT_HANDLE gxh);

	void					EnableSendShadow(GXMAP_OBJECT_HANDLE gxh);
	void					DisableSendShadow(GXMAP_OBJECT_HANDLE gxh);
	BOOL					IsInViewVolume(GXMAP_OBJECT_HANDLE gxh);
	BOOL					IsCollisionWithScreenCoord(GXOBJECT_HANDLE gxh,VECTOR3* pv3IntersectPoint,DWORD* pdwModelIndex,DWORD* pdwObjIndex,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag);
	

	

	DWORD					GetFrameCount();


	
	
		
	BOOL					PreCreateLight(char* szFileName,DWORD dwFlag);
	DWORD					PreLoadGXObject(char* szFileName);



	GXOBJECT_HANDLE			CreateGXObject(char* szFileName,GXSchedulePROC pProc,void* pData,DWORD dwFlag);
	GXLIGHT_HANDLE			CreateGXLight(LIGHT_DESC* pDesc,GXLightSchedulePROC pProc,void* pData,DWORD dwProjTexIndex,MATERIAL* pMtl,DWORD dwFlag);
	GXTRIGGER_HANDLE		CreateGXEventTrigger(GXSchedulePROC pProc,void* pData,DWORD dwFlag);
	GXMAP_HANDLE			CreateGXMap(GXMapSchedulePROC pProc,void* pData,DWORD dwFlag);
	
	void					UnloadPreLoadedGXObject(GXOBJECT_HANDLE gxo);
	void					UnloadAllPreLoadedGXObject(DWORD dwFlag);

	BOOL					BeginBuildMap(DWORD dwFlag);
	BOOL					InsertHFieldToGXMap(IHeightField* pHField);
	BOOL					InsertStaticModelTOGXMap(I3DStaticModel* pModel);
	BOOL					EndBuildMap(float fTop,float fBottom);
	BOOL					RebuildMap(float fTop,float fBottom);
	BOOL					GetWorldBoundingBox( MAABB* pWorldBox);
	
	
	
	DWORD					GetFramePerSec();
	void					SetFramePerSec(DWORD dwFrame);
	
	void					GXLSetLightDesc(GXLIGHT_HANDLE gxh,LIGHT_DESC* pLightDesc);
	void					GXLGetLightDesc(GXLIGHT_HANDLE gxh,LIGHT_DESC* pLightDesc);
	void					GXLSetPosition(GXLIGHT_HANDLE gxh,VECTOR3* pv3Pos);
	void					GXLGetPosition(GXLIGHT_HANDLE gxh,VECTOR3* pv3Pos);
	void					GXLMovePosition(GXLIGHT_HANDLE gxh,VECTOR3* pv3Pos);
	GXLIGHT_HANDLE			GXLGetLightWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag);
	void					GXLDisableStaticShadow(GXLIGHT_HANDLE gxl);
	void					GXLEnableStaticShadow(GXLIGHT_HANDLE gxl);
	BOOL					GXLIsDisableStaticShadow(GXLIGHT_HANDLE gxl);
	
	void					GXLEnableDynamicLight(GXLIGHT_HANDLE gxl);
	void					GXLDisableDynamicLight(GXLIGHT_HANDLE gxl);
	BOOL					GXLIsEnableDynamicLight(GXLIGHT_HANDLE gxl);

	void					GXTSetPosition(GXTRIGGER_HANDLE gxh,VECTOR3* pv3Pos);
	void					GXTSetScale(GXTRIGGER_HANDLE gxh,VECTOR3* pv3PScale);
	void					GXTSetRotation(GXTRIGGER_HANDLE gxh,VECTOR3* pv3Rot);
	void					GXTGetEventTriggerDesc(GXTRIGGER_HANDLE gxh,EVENT_TRIGGER_DESC* pEVDesc);
	GXTRIGGER_HANDLE		GXTGetObjectWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag);

	BOOL					GXMGetCollisionPointWithScreenCoord(DWORD* pdwObjIndex,VECTOR3* pv3IntersectPoint,float* pfDist,POINT* ptCursor,DWORD dwFlag);
	BOOL					GXMGetHFieldCollisionPointWithRay(VECTOR3* pv3IntersectPoint,float* pfDist,VECTOR3* pv3Orig,VECTOR3* pv3Dir);
	BOOL					GXMGetHFieldCollisionPointWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,POINT* ptCursor);
	BOOL					GXMGetHFieldHeight(float* py,float x,float z);
	BOOL					GXMStaticShadeMap(DWORD dwAmbientColor,LIGHT_DESC* pLight,DWORD dwLightNum,DWORD dwFlag,SHADE_FUNC pFunc);
	
	void					GXMSetHFieldAlphaFlag(DWORD dwAlpha);
	void					GXMSetSTMAlphaFlag(DWORD dwAlpha);

	
	BOOL					SetSymbol(SYMBOL_TYPE type,char* szFileName);
	void					SetRenderMode(DWORD dwRenderMode);


	// 충돌처리 함수군. 2002/03/06
	void							GXOSetBoundingVolume( GXOBJECT_HANDLE gxh, BOUNDING_VOLUME* pBV);


	void					GXOMovePositionWithCollide(GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos);		// 2002/05/22
	
	DWORD					GetGXObjectsNum();
	GXOBJECT_HANDLE			GetGXObjectWithSeqIndex(DWORD dwIndex);

	DWORD					GetGXLightsNum();
	GXLIGHT_HANDLE			GetGXLightWithSeqIndex(DWORD dwIndex);

	DWORD					GetGXEventTriggersNum();
	GXTRIGGER_HANDLE		GetGXEventTriggerWithSeqIndex(DWORD dwIndex);
	BOOL					LoadMapScript(char* szFileName,LOAD_CALLBACK_FUNC pFunc,DWORD dwFlag);
	void					DeleteAllGXMapObjectsWitLoadMapScript();


	void					Render();
	BOOL					RenderCameraFrontObject(float fDist);
	DWORD					Process();
	

	DWORD					Run(DWORD dwBackColor,GX_FUNC pfBeforeRenderFunc,GX_FUNC pfAfterRenderFunc,DWORD dwFlag);
	BOOL					SetCameraFitGXObject(GXOBJECT_HANDLE gxo,float fNear,float fFar,float fFov,DWORD dwViewportIndex);
	void					SetHFieldDetail(DWORD dwDetail);


	BOOL					DeleteGXObject(GXOBJECT_HANDLE gxh);
	BOOL					DeleteGXLight(GXLIGHT_HANDLE gxh);
	BOOL					DeleteGXEventTrigger(GXTRIGGER_HANDLE gxh);
	
	BOOL					DeleteGXMap(GXMAP_HANDLE gxm);
	BOOL					DeleteAllGXObjects();
	BOOL					DeleteAllGXLights();
	BOOL					DeleteAllGXEventTriggers();

		// 즉시 지우는 함수들..
	BOOL					ImmDeleteGXObject(GXOBJECT_HANDLE gxh);
	BOOL					ImmDeleteGXLight(GXLIGHT_HANDLE gxh);
	BOOL					ImmDeleteGXEventTrigger(GXTRIGGER_HANDLE gxh);
	BOOL					ImmDeleteGXDecal( GXDECAL_HANDLE gxh);

	// 나중에 지우는 함수들..
	BOOL					DefDeleteGXObject(GXOBJECT_HANDLE gxh);
	BOOL					DefDeleteGXLight(GXLIGHT_HANDLE gxh);
	BOOL					DefDeleteGXEventTrigger(GXTRIGGER_HANDLE gxh);
	BOOL					DefDeleteGXDecal(GXTRIGGER_HANDLE gxh);

	// 데칼 2003.01.15
	GXDECAL_HANDLE			CreateGXDecal( DECAL_DESC* pDesc, GXDecalSchedulePROC pProc, void* pData, DWORD dwFlag);
	BOOL					DeleteGXDecal( GXDECAL_HANDLE gxh);
//	BOOL					DeleteAllGXDecal();

	BOOL					GetMapReadCount(char* szFileName);
	
	BOOL					GXOInitializeIllusionEffect(GXOBJECT_HANDLE gxh,DWORD dwMaxIllusionFrameNum,char* szObjName,void* pMtlHandle,DWORD dwFlag);
	void					GXOBeginIllusionEffect(GXOBJECT_HANDLE gxh);
	void					GXOEndIllusionEffect(GXOBJECT_HANDLE gxh);
	DWORD					GXOGetAttachedGXObjects(GXOBJECT_HANDLE gxh,GXOBJECT_HANDLE* pGXOList,DWORD dwMaxNum);
	
	void					GXOSetScheduleProc(GXOBJECT_HANDLE gxo,GXSchedulePROC pProc);
	GXSchedulePROC			GXOGetScheduleProc(GXOBJECT_HANDLE gxo);
	
	void					GXOSetEffectIndex(GXOBJECT_HANDLE gxo,DWORD dwEffectIndex);
	DWORD					GXOGetEffectIndex(GXOBJECT_HANDLE gxo);
	
	void					SetAfterInterpolation( AfterInterpolationCallBack pfCallBack);
	// 익스큐티브::Run()에서 위치보간과 랜더링 사이에 호출되는 콜백.
	// 카메라를 캐릭터에 붙이거나할 때 쓰면 유용하다. 해제 하려면 NULL을 설정한다.

	// 현재의 게임 프레임을 리턴한다.
	DWORD					GetCurrentGameFrame(void);

//	void					GXOGetVelocity( GXOBJECT_HANDLE gxo, VECTOR3* pOutVelocity);
	ULONG					GXMGetStaticModel( I3DStaticModel** ppStaticModel);		
	ULONG					GXMGetHField(IHeightField** ppHField);
	
	void					GXODisableUnloadPreLoaded(GXOBJECT_HANDLE gxh);
	void					GXOEnableUnloadPreLoaded(GXOBJECT_HANDLE gxh);

	void					GXOGetLastVelocityAfterCollisionTest( GXOBJECT_HANDLE gxh, VECTOR3* pLastVelocity);

	void					GXOEnableScaleOfAttachedModel(GXOBJECT_HANDLE gxo);
	void					GXODisableScaleOfAttachedModel(GXOBJECT_HANDLE gxo);

	CoExecutive();
	~CoExecutive();
};

