#include "StdAfx.h"
#include "light_obj.h"
#include "global_variable.h"

CLightObject::CLightObject()
{
	m_objType = OBJECT_TYPE_LIGHT;
}

DWORD CLightObject::ReadFile(void* pFP)
{
	DWORD	dwLen = 0;

	dwLen += CBaseObject::ReadFile(pFP);

	FILE_LIGHT_HEADER*	pHeader = (FILE_LIGHT_HEADER*)&this->m_dwColor;
	dwLen += g_pFileStorage->FSRead(pFP,pHeader,sizeof(FILE_LIGHT_HEADER));
	
	return dwLen;
}


CLightObject::~CLightObject()
{
	CBaseObject::~CBaseObject();
}