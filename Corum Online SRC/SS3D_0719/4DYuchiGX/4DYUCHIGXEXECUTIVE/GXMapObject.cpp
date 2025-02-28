#include "StdAfx.h"
#include "GXMapObject.h"
#include "../4DyuchiGXGFunc/global.h"
#include "executive.h"


//	GLOBAL_FUNC_DLL DWORD				QBHSelect(QBHASH_HANDLE pHash,DWORD OUT* pItems,DWORD dwMaxItemNum,DWORD dwKeyData);
//GLOBAL_FUNC_DLL void*				QBHInsert(QBHASH_HANDLE pHash,DWORD dwItem,DWORD dwKeyData);

#define INVALID_ID 0xffffffff

CGXMapObject::CGXMapObject() {
	m_dwID = INVALID_ID;
}

void CGXMapObject::Cleanup()
{
	m_dwIndex = 0xffffffff;
	m_dwClipperIndex = 0xffffffff;
	m_dwRenderFrameCount = 0xffffffff;
	m_dwPickTypeFlag = PICK_TYPE_DEFAULT;
	m_dwIndexInInitialTable = 0xffffffff;
}


BOOL CGXMapObject::SetID(DWORD dwID)	
{
	auto pHash = m_pExecutive->gxMapObjectPerID();
	BOOL	bResult = FALSE;

	if (dwID == m_dwID) {
		return TRUE;
	}
	
	if (dwID >= 0xfffffff0) {
		return FALSE;
	}

	const auto found = pHash->find(dwID);
	if (found != pHash->end())
	{
		MessageBox(NULL,"conflict of GXObject's UID","Error",MB_OK);
		goto lb_return;
	}

	pHash->insert({ dwID, this });

	m_dwID = dwID;

lb_true:
	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL CGXMapObject::IsRenderable()
{ 
	BOOL	bResult;
	
	if (m_dwScheduleFlag & SCHEDULE_FLAG_NOT_RENDER)
		bResult = FALSE;
	else
		bResult = TRUE;

lb_return:
	return bResult;

}
void CGXMapObject::ReleaseID()
{
	auto pHash = m_pExecutive->gxMapObjectPerID();
	pHash->erase(m_dwID);
	m_dwID = INVALID_ID;
}



void CGXMapObject::Duplcate(CGXMapObject* pDestGXMObj)
{
	pDestGXMObj->m_dwScheduleFlag = m_dwScheduleFlag;
	pDestGXMObj->m_dwRenderFlag = m_dwRenderFlag;
	pDestGXMObj->m_dwAlphaFlag = m_dwAlphaFlag;
	pDestGXMObj->m_dwPropertyFlag = m_dwPropertyFlag;
	pDestGXMObj->m_pExecutive = m_pExecutive;
	
}