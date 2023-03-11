#pragma once

#include "stdafx.h"

class CHashTable 
{
public:
	virtual DWORD			Select(DWORD OUT* pItems,DWORD dwMaxItemNum,void* pKeyData,DWORD dwSize) = 0;
	virtual	void*			Insert(DWORD dwItem,void* pKeyData,DWORD dwSize) = 0;
	virtual BOOL			Delete(void* pBucket) = 0;
	virtual void			Release() = 0;
	virtual DWORD			GetMaxBucketNum() = 0;
	virtual BOOL			Initialize(DWORD dwMaxBucketNum,DWORD dwMaxSize,DWORD dwMaxItemNum) = 0;


};

