#pragma once

#include "FileStorage_typedef.h"
#include "IFileStorage_GUID.h"
#include "typedef.h"

interface I4DyuchiFileStorage : public IUnknown
{	
	virtual 	BOOL		Initialize(DWORD dwMaxFileNum,DWORD dwMaxFileHandleNumAtSameTime,DWORD dwMaxFileNameLen,FILE_ACCESS_METHOD accessMethod) = 0;
	virtual		void*		MapPackFile(char* szPackFileName) = 0;
	virtual		void		UnmapPackFile(void* pPackFileHandle) = 0;
	
	virtual		DWORD		GetFileNum(void* pPackFileHandle) = 0;
	virtual 	DWORD		CreateFileInfoList(void* pPackFileHandle,FSFILE_ATOM_INFO** ppInfoList,DWORD dwMaxNum) = 0;
	virtual		void		DeleteFileInfoList(void* pPackFileHandle,FSFILE_ATOM_INFO* pInfoList) = 0;
	virtual 	BOOL		IsExistInFileStorage(char* szFileName) = 0;

	virtual		BOOL		LockPackFile(void* pPackFileHandle,DWORD dwFlag) = 0;
	virtual		BOOL		InsertFileToPackFile(void* pPackFileHandle,char* szFileName) = 0;
	virtual		BOOL		DeleteFileFromPackFile(char* szFileName) = 0;
	virtual		BOOL		UnlockPackFile(void* pPackFileHandle,LOAD_CALLBACK_FUNC pCallBackFunc) = 0;

	virtual 	BOOL		ExtractFile(char* szFileName) = 0;
	virtual		BOOL		ExtractAllFiles() = 0;
	virtual		DWORD		ExtractAllFilesFromPackFile(void* pPackFileHandle) = 0;

	virtual		void*		FSOpenFile(char* szFileName,DWORD dwAccessMode) = 0;
	virtual		int			FSScanf(void* pFP,char* szFormat, ...) = 0;
	virtual		DWORD		FSRead(void* pFP,void* pDest,DWORD dwLen) = 0;
	virtual		DWORD		FSSeek(void* pFP,DWORD dwOffset,FSFILE_SEEK seekBase) = 0;
	virtual		BOOL		FSCloseFile(void* pFP) = 0;

	virtual		BOOL		GetPackFileInfo(void* pPackFileHandle,FSPACK_FILE_INFO* pFileInfo) = 0;

	virtual		BOOL		BeginLogging(char* szFileName,DWORD dwFlag) = 0;
	virtual		BOOL		EndLogging() = 0;
	
};
