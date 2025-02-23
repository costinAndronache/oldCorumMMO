#pragma once

#include "../4DyuchiGRX_Common/IRenderer.h"
#include "../4DyuchiGRX_Common/IGeometry.h"
#include "../4DyuchiGRX_Common/IExecutive.h"


DWORD  GXDefaultSchedulePROC(I4DyuchiGXExecutive* pExecutive,GXOBJECT_HANDLE gxh,DWORD msg,int arg1,int arg2,void* pData);
DWORD  GXDefaultLightSchedulePROC(I4DyuchiGXExecutive* pExecutive,GXLIGHT_HANDLE gxh,DWORD msg,int arg1,int arg2,void* pData);
DWORD  GXDefaultDecalSchedulePROC(I4DyuchiGXExecutive* pExecutive,GXDECAL_HANDLE gxd,DWORD msg,int arg1,int arg2,void* pData);



