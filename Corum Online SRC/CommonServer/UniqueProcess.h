#ifndef __UNIQUE_PROCESS__H__
#define __UNIQUE_PROCESS__H__


#pragma  once


#include "CommonServer.h"


#define GKO_SUCCESS		1
#define GKO_ERROR		-1
#define GKO_EXIST		0


int		DECLSPECIFIER CreateGlobalKernelObject(DWORD dwServerID); // FALSE: �̹� �������� ������!
void	DECLSPECIFIER DeleteGlobalKernelObject();


#endif 