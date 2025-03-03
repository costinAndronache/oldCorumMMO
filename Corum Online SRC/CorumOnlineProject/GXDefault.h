#pragma once

#ifdef SS3D_TEST
#include "./../SS3D_0719/4dyuchigx/4dyuchiGRX_common/IRenderer.h"
#include "./../SS3D_0719/4dyuchigx/4dyuchiGRX_common/IGeometry.h"
#include "./../SS3D_0719/4dyuchigx/4dyuchiGRX_common/IExecutive.h"
#include "./../SS3D_0719/4dyuchigx/4dyuchiGRX_common/IAudio.h"
#else
#include "./../SS3D_0719/4dyuchigx/4DYUCHIGRX_COMMON/IRenderer.h"
#include "./../SS3D_0719/4dyuchigx/4DYUCHIGRX_COMMON/IGeometry.h"
#include "./../SS3D_0719/4dyuchigx/4DYUCHIGRX_COMMON/IExecutive.h"
#include "./../SS3D_0719/4dyuchigx/4DYUCHIGRX_COMMON/IAudio.h"
#endif
DWORD __stdcall GXPlayerPROC(I4DyuchiGXExecutive* pExecutive,GXOBJECT_HANDLE gxh,DWORD msg,int arg1,int arg2,void* pData);
