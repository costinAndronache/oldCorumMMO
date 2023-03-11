#ifndef __TEXT_RESOURCE_MESSAGE_POOL_H__
#define __TEXT_RESOURCE_MESSAGE_POOL_H__

#pragma once

//---------------------------------------------------------------------------------
//
//		Coded by deepdark. 2004.01.16
//			the Text Resource Pool for Effective Memory Management
//
//---------------------------------------------------------------------------------

#define MAX_TEXT_LEN	255

#ifndef DECODE_KEY
#	define DECODE_KEY			"��ܵ����,����ؿؿ�"
#endif

#ifndef DECODE_SUBKEY
#	define DECODE_SUBKEY		2
#endif

////////////////////////////////////////////////////////////////////////////////////////

#pragma  pack(push, 1)

// DB�κ��� �������� ���� ������ �� ����ü.
// DB�� ����� ���� �״��.
typedef struct 
{
	DWORD	dwTextID;					// TEXT ���ҽ� ID
	char	szText[MAX_TEXT_LEN];		// TEXT ���ҽ� ������.
} TEXT_RESOURCE, *LPTEXT_RESOURCE;
#pragma  pack(pop)


// �ؽ�Ʈ ���ҽ� ������ ���.
typedef struct 
{
	char	szSign[4];					// TEXT ���ҽ� Identifier
	DWORD	dwResourceCount;			// TEXT ���ҽ� ����.
	DWORD	dwTextArraySize;			// TEXT DATA ARRAY SIZE
} TEXT_RESOURCE_HEADER, *LPTEXT_RESOURCE_HEADER;

// �ؽ�Ʈ ���ҽ� ������ �� ���ҽ� ����.
typedef struct 
{
	DWORD	dwTextID;					// �ؽ�Ʈ ���ҽ� ID
	DWORD	dwTextPos;					// �ؽ�Ʈ ���ҽ� ��ġ(������ ��꿡 ����)
} TEXT_RESOURCE_INFO, *LPTEXT_RESOURCE_INFO;



////////////////////////////////////////////////////////////////////////////////////////
/*
	TEXT ���ҽ� ������ ����(n�� �ؽ�Ʈ ���ҽ�).
	+--------+-------------------------+---------------------------------------------+
	| Header | Info[n] Array ...       | TextData Array  ...                         |
	+--------+-------------------------+---------------------------------------------+

	; �ؽ�Ʈ�� text data array�� ���̾� ����ȴ�.
*/

typedef struct __MESSAGEx
{
	DWORD	dwId;		// MessageID
	LPSTR	szMessage;	// Message!
} MESSAGEx, *LPMESSAGEx;


class CMessagePool
{
public:
	CMessagePool();
	~CMessagePool();

	BOOL	LoadTextResource(char* szSourceFile, char* szDecodeKey = DECODE_KEY, int nDecodeSubKey = DECODE_SUBKEY);
	MESSAGEx operator [] (DWORD dwIndex);
	LPSTR	GetMessage(DWORD dwIndex);
	DWORD	GetCount() { return m_dwTextResourceCount;	}

	DWORD	GetFirstMessageIdInString(LPCSTR szString);
	DWORD	GetLastMessageIdInString(LPCSTR szString);
	BOOL	IsThereAnyMessageInString(LPCSTR szString);
	LPCSTR	GetFirstMessageInString(LPCSTR szString);

	// for emoticon
	int		CompareRoutineEmoticon(LPCSTR szString);
	int		FilterEmoticonString(LPCSTR szString);

private:
	BOOL	VerifySourceFile(LPTEXT_RESOURCE_HEADER pHeader, DWORD dwFileSize);
	BOOL	DecodeData4Text(char* szLoadFile,  void* pReturnValue, char* szDecodeKey, int nDecodeSubKey);
	BOOL	AllocResourceInfo(DWORD dwCount, LPVOID pData = NULL);
	BOOL	AllocResourceStringBuffer(DWORD dwSize, LPVOID pData = NULL);
	void	ClearPool();

	BOOL	CompareRoutine(LPCSTR szString, LPCSTR szConv);

	DWORD					m_dwTextResourceCount;
	LPTEXT_RESOURCE_INFO	m_pTextResourceInfo;
	LPBYTE					m_pTextResourceString;
};

#endif // __TEXT_RESOURCE_MESSAGE_POOL_H__