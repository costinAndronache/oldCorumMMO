#pragma once

#include "stdafx.h"
#include "IExecutive_GUID.h"
#include "IRenderer.h"
#include "IGeometry.h"

#include "typedef.h"

interface I4DyuchiGXExecutive : public IUnknown
{
	virtual I4DyuchiGXRenderer*		GetRenderer() = 0;		// 인터페이스에 대해 Release()필요없음.
	virtual I4DyuchiGXGeometry*		GetGeometry() = 0;		// 인터페이스에 대해 Release()필요없음.

	virtual HRESULT					GetRenderer(I4DyuchiGXRenderer** ppRenderer) = 0;		// Release필요함
	virtual HRESULT					GetGeometry(I4DyuchiGXGeometry** ppGeometry) = 0;		// Release필요함
	virtual	HRESULT					GetFileStorage(I4DyuchiFileStorage** ppFileStorage) = 0;	// Release필요함

	virtual BOOL					InitializeWithoutRegistry(HWND hWnd,DISPLAY_INFO* pInfo,DWORD dwMaxObjectNum,DWORD dwMaxLightNum,DWORD dwMaxTriggerNum,DWORD dwViewportNum, DWORD dwMaxDecalNum,ErrorHandleProc pErrorHandleFunc) = 0;
	virtual BOOL					Initialize(HWND hWnd,DISPLAY_INFO* pInfo,DWORD dwMaxObjectNum,DWORD dwMaxLightNum,DWORD dwMaxTriggerNum,DWORD dwViewportNum, DWORD dwMaxDecalNum,ErrorHandleProc pErrorHandleFunc) = 0;
	virtual BOOL					InitializeFileStorageWithoutRegistry(DWORD dwMaxFileNum,DWORD dwMaxFileHandleNumAtSameTime,DWORD dwMaxFileNameLen,FILE_ACCESS_METHOD accessMethod,PACKFILE_NAME_TABLE* pPackFileList,DWORD dwNum) = 0;
	virtual BOOL					InitializeFileStorage(DWORD dwMaxFileNum,DWORD dwMaxFileHandleNumAtSameTime,DWORD dwMaxFileNameLen,FILE_ACCESS_METHOD accessMethod,PACKFILE_NAME_TABLE* pPackFileList,DWORD dwNum) = 0;
	
	virtual void					SetViewport(DWORD dwViewportIndex) = 0;

	virtual DWORD					IsValidHandle(GXMAP_OBJECT_HANDLE gxh) = 0;	// 디버그용 함수.무진장 느리다.

	virtual DWORD					GXOProcess(GXOBJECT_HANDLE gxo,DWORD dwFrameInc) = 0;
	virtual BOOL					GXORender(GXOBJECT_HANDLE gxo) = 0;

	
	virtual void					GXOSetCurrentFrame(GXOBJECT_HANDLE gxh,DWORD dwFrame) = 0;
	virtual void					GXOSetCurrentMotionIndex(GXOBJECT_HANDLE gxh,DWORD dwMotionIndex) = 0;
	virtual BOOL					GXOGetMotionDesc(GXOBJECT_HANDLE gxh,MOTION_DESC* pMotionDesc,DWORD dwMotionIndex,DWORD dwModelIndex) = 0;
	virtual DWORD					GXOGetMotionNum(GXOBJECT_HANDLE gxh,DWORD dwModelIndex) = 0;			// 모션들의 갯수를 얻는다.
	virtual DWORD					GXOGetCurrentFrame(GXOBJECT_HANDLE gxh) = 0;
	virtual DWORD					GXOGetCurrentMotionIndex(GXOBJECT_HANDLE gxh) = 0;
	
	virtual void					GXOSetCurrentMaterialIndex(GXOBJECT_HANDLE gxh,DWORD dwMaterialIndex) = 0;
	virtual	DWORD					GXOGetMaterialNum(GXOBJECT_HANDLE gxh,DWORD dwModelIndex) = 0;
	virtual DWORD					GXOGetCurrentMaterialIndex(GXOBJECT_HANDLE gxh) = 0;

	virtual void					GXOSetPosition(GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos, BOOL bDoInterpolation) = 0;
	virtual void					GXOMovePosition(GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos) = 0;
	virtual void					GXOGetPosition(GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos) = 0;
	virtual	void					GXOMovePositionWithCollide(GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos)=0;		// 2002/05/22
	virtual void					GXOGetInterpolatedPosition( GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos)=0; // 보간되어서 랜더링 되는 포지션을 출력. 실제 GXO의 위치는 아니다. 카메라위치 갱신할 때 쓰면 좋을듯.
	
	virtual void					GXOSetDirection(GXOBJECT_HANDLE gxh,VECTOR3* pv3Axis,float fRad) = 0;
	virtual void					GXOSetDirectionFPSStyle(GXOBJECT_HANDLE gxh, VECTOR3* pv3Angles /* in */) = 0;
	virtual	void					GXOChangeDirection(GXOBJECT_HANDLE gxh,float fRad) = 0;
	virtual void					GXOGetDirection(GXOBJECT_HANDLE gxh,VECTOR3* pv3Axis,float* pfRad) = 0;
	virtual void					GXOGetDirectionFPSStyle(GXOBJECT_HANDLE gxh, VECTOR3* pv3Angle /* out */) = 0;
	virtual void					GXOSetScale(GXOBJECT_HANDLE gxh,VECTOR3* pv3Scale) = 0;
	virtual void					GXOGetScale(GXOBJECT_HANDLE gxh,VECTOR3* pv3Scale) = 0;
	virtual void					GXOMoveForward(GXOBJECT_HANDLE gxh,float fDistance) = 0;
	
	virtual void					GXOEnableHFieldApply(GXOBJECT_HANDLE gxo) = 0;
	virtual void					GXODisableHFieldApply(GXOBJECT_HANDLE gxo) = 0;
	virtual BOOL					GXOIsHFieldApply(GXOBJECT_HANDLE gxo) = 0;

	virtual void					GXOSetZOrder(GXOBJECT_HANDLE gxo,int iZOrder) = 0;

	virtual void					GXOEnableAsEffect(GXOBJECT_HANDLE gxo) = 0;
	virtual void					GXODisableAsEffect(GXOBJECT_HANDLE gxo) = 0;
	virtual BOOL					GXOIsAsEffect(GXOBJECT_HANDLE gxo) = 0;

	virtual void					GXOEnableSelfIllumin(GXOBJECT_HANDLE gxo) = 0;
	virtual void					GXODisableSelfIllumin(GXOBJECT_HANDLE gxo) = 0;
	virtual BOOL					GXOIsEnableSelfIllumin(GXOBJECT_HANDLE gxo) = 0;

	virtual void					EstimatedMoveForward(GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos,float fDistance) = 0;

	//////////////////////////
	virtual	void					GXOSetBoundingVolume( GXOBJECT_HANDLE gxh, BOUNDING_VOLUME* pBV)=0;
	virtual	BOOL					GXOIsLanding( GXOBJECT_HANDLE gxh)=0;

	virtual BOOL					GXOAttach(GXOBJECT_HANDLE gxh,GXOBJECT_HANDLE to_gxh,char* szObjName) = 0;
	virtual BOOL					GXODetach(GXOBJECT_HANDLE gxh,GXOBJECT_HANDLE child_gxh) = 0;
	virtual BOOL					GXOAttachLight(GXOBJECT_HANDLE gxoh,char* szObjName,GXLIGHT_HANDLE gxlh,VECTOR3* pv3Offset,DWORD dwAttachType) = 0;
	virtual BOOL					GXODetachLight(GXOBJECT_HANDLE gxoh,GXLIGHT_HANDLE child_gxh) = 0;
	virtual BOOL					GXOAttachCameraFront(GXOBJECT_HANDLE gxo,float fDist) =0;
	virtual BOOL					GXODetachCameraFront(GXOBJECT_HANDLE gxo) = 0;
	virtual ULONG					GXOGetModel(GXOBJECT_HANDLE gxh,I3DModel** ppModel,DWORD* pdwRefIndex,DWORD dwModelIndex) = 0;// Release()필요
	virtual	GXOBJECT_HANDLE			GXOGetObjectWithScreenCoord(VECTOR3* pv3IntersectPoint,DWORD* pdwModelIndex,DWORD* pdwObjIndex,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag) = 0;
	virtual DWORD					GXOGetMultipleObjectWithScreenCoordRect(PICK_GXOBJECT_DESC* pPickDesc,DWORD dwMaxDescNum,RECT* pRect,DWORD dwViewportIndex,DWORD dwFlag) = 0;
	virtual DWORD					GXOGetMultipleObjectWithScreenCoord(PICK_GXOBJECT_DESC* pPickDesc,DWORD dwMaxDescNum,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag) = 0;
	virtual BOOL					GXOReplaceModel(GXOBJECT_HANDLE gxh,DWORD dwModelIndex,char* szFileName) = 0;
	virtual void					GXOGetCollisionMesh(GXOBJECT_HANDLE gxh,COLLISION_MESH_OBJECT_DESC* pColMeshDesc) = 0;
	virtual DWORD					GXOGetObjectIndex(GXOBJECT_HANDLE gxh,char* szObjName,DWORD dwModelIndex) = 0;
	virtual BOOL					GXOGetWorldMatrixPerObject(GXOBJECT_HANDLE gxh,MATRIX4* pMatWorld,DWORD dwModelIndex,DWORD dwObjIndex) = 0;
	virtual DWORD					GXOGetAllObjectsWitLoadMapScript(GXOBJECT_HANDLE* pObjArray,DWORD dwMaxNum) = 0;
	
	virtual void				 	DisableRender(GXMAP_OBJECT_HANDLE gxh) = 0;
	virtual void				 	EnableRender(GXMAP_OBJECT_HANDLE gxh) = 0;
	virtual void					DisableSchedule(GXMAP_OBJECT_HANDLE gxh) = 0;
	virtual	BOOL					IsRenderable(GXMAP_OBJECT_HANDLE gxh) = 0;
	virtual void					EnableSchedule(GXMAP_OBJECT_HANDLE gxh) = 0;

	virtual void					SetPickType(GXMAP_OBJECT_HANDLE	gxh,PICK_TYPE type) = 0;
	virtual PICK_TYPE				GetPickType(GXMAP_OBJECT_HANDLE	gxh) = 0;

	virtual void					SetRenderFlag(GXMAP_OBJECT_HANDLE gxh,DWORD dwFlag) = 0;
	virtual DWORD					GetRenderFlag(GXMAP_OBJECT_HANDLE gxh) = 0;
	virtual void					SetAlphaFlag(GXMAP_OBJECT_HANDLE gxh,DWORD dwFlag) = 0;
	virtual DWORD					GetAlphaFlag(GXMAP_OBJECT_HANDLE gxh) = 0;
	virtual void*					GetData(GXMAP_OBJECT_HANDLE gxh) = 0;
	virtual void					SetData(GXMAP_OBJECT_HANDLE gxh,void* pData) = 0;
	virtual DWORD					GetID(GXMAP_OBJECT_HANDLE gxh) = 0;
	virtual BOOL					SetID(GXMAP_OBJECT_HANDLE gxh,DWORD dwID) = 0;
	virtual void 					ReleaseID(GXMAP_OBJECT_HANDLE gxh) = 0;
	virtual BOOL					IsInViewVolume(GXMAP_OBJECT_HANDLE gxh) = 0;
	virtual BOOL					IsCollisionWithScreenCoord(GXOBJECT_HANDLE gxh,VECTOR3* pv3IntersectPoint,DWORD* pdwModelIndex,DWORD* pdwObjIndex,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag) = 0;

	virtual GXMAP_OBJECT_HANDLE 	GetGXMapObjectWithID(DWORD dwID) = 0;
	
	virtual void					SetPropertyFlag(GXMAP_OBJECT_HANDLE gxh,DWORD dwFlag) = 0;
	virtual DWORD					GetPropertyFlag(GXMAP_OBJECT_HANDLE gxh) = 0;

	virtual void					EnablePick(GXMAP_OBJECT_HANDLE gxh) = 0;
	virtual void					DisablePick(GXMAP_OBJECT_HANDLE gxh) = 0;
	virtual void					EnableUpdateShading(GXMAP_OBJECT_HANDLE gxh) = 0;
	virtual void					DisableUpdateShading(GXMAP_OBJECT_HANDLE gxh) = 0;
	virtual	void					EnableSendShadow(GXMAP_OBJECT_HANDLE gxh) = 0;
	virtual void					DisableSendShadow(GXMAP_OBJECT_HANDLE gxh) = 0;

	
	
	virtual	BOOL					PreCreateLight(char* szFileName,DWORD dwFlag) = 0;
	virtual DWORD					PreLoadGXObject(char* szFileName) = 0;
	virtual GXOBJECT_HANDLE 		CreateGXObject(char* szFileName,GXSchedulePROC pProc,void* pData,DWORD dwFlag) = 0;
	virtual GXLIGHT_HANDLE			CreateGXLight(LIGHT_DESC* pDesc,GXLightSchedulePROC pProc,void* pData,DWORD dwProjTexIndex,MATERIAL* pMtl,DWORD dwFlag) = 0;
	virtual GXTRIGGER_HANDLE		CreateGXEventTrigger(GXSchedulePROC pProc,void* pData,DWORD dwFlag) = 0;
	
	virtual GXMAP_HANDLE			CreateGXMap(GXMapSchedulePROC pProc,void* pData,DWORD dwFlag) = 0;
	virtual void					UnloadAllPreLoadedGXObject(DWORD dwFlag) = 0;
	virtual void					UnloadPreLoadedGXObject(GXOBJECT_HANDLE gxo) = 0;


	virtual BOOL					BeginBuildMap(DWORD dwFlag) = 0;
	virtual BOOL					InsertHFieldToGXMap(IHeightField* pHField) = 0;
	virtual BOOL					InsertStaticModelTOGXMap(I3DStaticModel* pModel) = 0;
	virtual BOOL					EndBuildMap(float fTop,float fBottom) = 0;
	virtual	BOOL					RebuildMap(float fTop,float fBottom)=0;
	virtual BOOL					GetWorldBoundingBox( MAABB* pWorldBox) = 0;

	virtual BOOL					DeleteGXMap(GXMAP_HANDLE gxm) = 0;
	virtual BOOL					DeleteAllGXObjects() = 0;
	virtual BOOL					DeleteAllGXLights() = 0;
	virtual BOOL					DeleteAllGXEventTriggers() = 0;
	virtual BOOL					DeleteGXObject(GXOBJECT_HANDLE gxh) = 0;
	virtual BOOL					DeleteGXLight(GXLIGHT_HANDLE gxh) = 0;
	virtual BOOL					DeleteGXEventTrigger(GXTRIGGER_HANDLE gxh) = 0;

	virtual DWORD					GetFramePerSec() = 0;
	virtual	void					SetFramePerSec(DWORD dwFrame) = 0;
	
	virtual	void					GXLSetLightDesc(GXLIGHT_HANDLE gxh,LIGHT_DESC* pLightDesc) = 0;
	virtual	void					GXLGetLightDesc(GXLIGHT_HANDLE gxh,LIGHT_DESC* pLightDesc) = 0;
	virtual void					GXLSetPosition(GXLIGHT_HANDLE gxh,VECTOR3* pv3Pos) = 0;
	virtual void					GXLGetPosition(GXLIGHT_HANDLE gxh,VECTOR3* pv3Pos) = 0;
	virtual	void					GXLMovePosition(GXLIGHT_HANDLE gxh,VECTOR3* pv3Pos) = 0;
	virtual GXLIGHT_HANDLE			GXLGetLightWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag) = 0;
	virtual	void					GXLDisableStaticShadow(GXLIGHT_HANDLE gxl) = 0;
	virtual void					GXLEnableStaticShadow(GXLIGHT_HANDLE gxl) = 0;
	virtual BOOL					GXLIsDisableStaticShadow(GXLIGHT_HANDLE gxl) = 0;
		
	virtual void					GXLEnableDynamicLight(GXLIGHT_HANDLE gxl) = 0;
	virtual void					GXLDisableDynamicLight(GXLIGHT_HANDLE gxl) = 0;
	virtual	BOOL					GXLIsEnableDynamicLight(GXLIGHT_HANDLE gxl) = 0;

	virtual void					GXTSetPosition(GXTRIGGER_HANDLE gxh,VECTOR3* pv3Pos) = 0;
	virtual void					GXTSetScale(GXTRIGGER_HANDLE gxh,VECTOR3* pv3PScale) = 0;
	virtual void					GXTSetRotation(GXTRIGGER_HANDLE gxh,VECTOR3* pv3Rot) = 0;
	virtual void					GXTGetEventTriggerDesc(GXTRIGGER_HANDLE gxh,EVENT_TRIGGER_DESC* pEVDesc) = 0;
	virtual GXTRIGGER_HANDLE		GXTGetObjectWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag) = 0;
	
	virtual BOOL					GXMGetCollisionPointWithScreenCoord(DWORD* pdwObjIndex,VECTOR3* pv3IntersectPoint,float* pfDist,POINT* ptCursor,DWORD dwFlag) = 0;
	virtual BOOL					GXMGetHFieldCollisionPointWithRay(VECTOR3* pv3IntersectPoint,float* pfDist,VECTOR3* pv3Orig,VECTOR3* pv3Dir) = 0;
	virtual	BOOL					GXMGetHFieldCollisionPointWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,POINT* ptCursor) = 0;
	virtual BOOL					GXMGetHFieldHeight(float* py,float x,float z) = 0;
	virtual BOOL					GXMStaticShadeMap(DWORD dwAmbientColor,LIGHT_DESC* pLight,DWORD dwLightNum,DWORD dwFlag,SHADE_FUNC pFunc) = 0;
	virtual void					GXMSetHFieldAlphaFlag(DWORD dwAlpha) = 0;
	virtual void					GXMSetSTMAlphaFlag(DWORD dwAlpha) = 0;;
	
	
	
	virtual	BOOL					SetSymbol(SYMBOL_TYPE type,char* szFileName) = 0;
	virtual	void					SetRenderMode(DWORD dwRenderMode) = 0;
	virtual	DWORD					GetGXObjectsNum() = 0;
	virtual	GXOBJECT_HANDLE			GetGXObjectWithSeqIndex(DWORD dwIndex) = 0;

	virtual DWORD					GetGXLightsNum() = 0;
	virtual GXLIGHT_HANDLE			GetGXLightWithSeqIndex(DWORD dwIndex) = 0;

	virtual DWORD					GetGXEventTriggersNum() = 0;
	virtual GXTRIGGER_HANDLE		GetGXEventTriggerWithSeqIndex(DWORD dwIndex) = 0;
	virtual BOOL					LoadMapScript(char* szFileName,LOAD_CALLBACK_FUNC pFunc,DWORD dwFlag) = 0;
	virtual void					DeleteAllGXMapObjectsWitLoadMapScript() = 0;
	
	virtual void					Render() = 0;
	virtual BOOL					RenderCameraFrontObject(float fDist) = 0;
	// 랜더링을 건다.
	// 이 함수는 Process()와 짝을 이루어서 쓰는것이 좋다. Run()과는 배타적.
	virtual DWORD					Process() = 0;
	// 게임 프레임을 테스트 해서 증가시키고, 조건이 만족되면 GXObject의 프로시저를 순회한다.
	// 리턴값은 프레임 증가량이다.
	
	virtual DWORD					Run(DWORD dwBackColor,GX_FUNC pfBeforeRenderFunc,GX_FUNC pfAfterRenderFunc,DWORD dwFlag) = 0;
	// 게임 프로시저를 돌리고 랜더링을 건다.
	// 내부적으로 Process()와 Render()를 돌릴 뿐이다.
	// 게임에서 이 함수를 쓸 경우, Render()와 Process()를 직접 호출 해서는 안된다.
	// 리턴값은 프레임 증가량이다.
	virtual BOOL					SetCameraFitGXObject(GXOBJECT_HANDLE gxo,float fNear,float fFar,float fFov,DWORD dwViewportIndex) = 0;
	virtual void					SetHFieldDetail(DWORD dwDetail) = 0;

	// 데칼 2003.01.15
	virtual	GXDECAL_HANDLE			CreateGXDecal( DECAL_DESC* pDesc, GXDecalSchedulePROC pProc, void* pData, DWORD dwFlag) = 0;
	virtual	BOOL					DeleteGXDecal( GXDECAL_HANDLE gxd) = 0;
//	virtual	BOOL					DeleteAllGXDecal() = 0;
	
	virtual BOOL					GXOInitializeIllusionEffect(GXOBJECT_HANDLE gxh,DWORD dwMaxIllusionFrameNum,char* szObjName,void* pMtlHandle,DWORD dwFlag) = 0;
		
	virtual void					GXOBeginIllusionEffect(GXOBJECT_HANDLE gxh) = 0;
	virtual void					GXOEndIllusionEffect(GXOBJECT_HANDLE gxh) = 0;
	virtual DWORD					GXOGetAttachedGXObjects(GXOBJECT_HANDLE gxh,GXOBJECT_HANDLE* pGXOList,DWORD dwMaxNum) = 0;

	virtual void					GXOSetScheduleProc(GXOBJECT_HANDLE gxo,GXSchedulePROC pProc) = 0;
	virtual GXSchedulePROC			GXOGetScheduleProc(GXOBJECT_HANDLE gxo) = 0;

	virtual	void					GXOSetEffectIndex(GXOBJECT_HANDLE gxo,DWORD dwEffectIndex) = 0;
	virtual	DWORD					GXOGetEffectIndex(GXOBJECT_HANDLE gxo) = 0;

	virtual	void					SetAfterInterpolation( AfterInterpolationCallBack pfCallBack)=0;
	// 익스큐티브::Render()에서 위치보간과 랜더링 사이에 호출되는 콜백.
	// 카메라를 캐릭터에 붙이거나할 때 쓰면 유용하다. 해제 하려면 NULL을 설정한다.

	// 현재의 게임 프레임을 리턴한다.
	virtual	DWORD					GetCurrentGameFrame(void)=0;

	// 지금 올라와있는 stm을 리턴한다. 없을경우 NULL
	virtual ULONG					GXMGetStaticModel( I3DStaticModel** ppStaticModel) = 0; //얻어온 COM개체에 대해 Release필요.성공하면 리턴값0,실패하면 0xffffffff
	virtual ULONG					GXMGetHField(IHeightField** ppHField) = 0;			// 얻어온 COM개체에 대해 Release필요.성공하면 리턴값0,실패하면 0xffffffff

	virtual void					GXODisableUnloadPreLoaded(GXOBJECT_HANDLE gxh) = 0;
	virtual void					GXOEnableUnloadPreLoaded(GXOBJECT_HANDLE gxh) = 0;

	virtual void					GXOEnableScaleOfAttachedModel(GXOBJECT_HANDLE gxo) = 0;
	virtual void					GXODisableScaleOfAttachedModel(GXOBJECT_HANDLE gxo) = 0;
};
