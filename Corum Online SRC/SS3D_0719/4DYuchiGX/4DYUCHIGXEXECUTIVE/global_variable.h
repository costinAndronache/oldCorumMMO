#pragma once

#include "../4DyuchiGRX_Common/typedef.h"

extern ErrorHandleProc				g_pErrorHandleFunc;

interface I4DyuchiFileStorage;
extern I4DyuchiFileStorage*			g_pFileStorage;


/*
	virtual		void*		FSOpenFile(char* szFileName,DWORD dwAccessMode) = 0;
	virtual		int			FSScanf(void* pFP,char* szFormat, ...) = 0;
	virtual		DWORD		FSRead(void* pFP,char* pDest,DWORD dwLen) = 0;
	virtual		BOOL		FSCloseFile(void* pFP) = 0;

*/
