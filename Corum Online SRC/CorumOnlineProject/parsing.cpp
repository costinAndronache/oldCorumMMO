#include "stdafx.h"
#include "parsing.h"
#include "SafeMemory.h"
#include "StringFunc.h"


char g_szToken[0xff];

// ---------------------------------------------
// Name : GetLengthOfString
// Desc : ���� ��Ʈ���� ������ �Լ�.
//		  �������� ���� ���̳ʸ� ������ Ʋ����.
//		  �׸��Ͽ� �� �Լ��� �������.
// parameters : 
//			iCount	- ���� ���ڼ� 
//	[OUT]	pszDest - ���� ���ڿ��� �޾ƿ� ����
//  [IN]	pszSrc	- ���� ���ڿ��� ����   
//								[6/22/2004] / by seven 
// ---------------------------------------------
bool GetLengthOfString(const char * IN pszSrc, char * OUT pszDest, int iCount)
{
	//////////////////////////////////////////////////////////////////////////
	// check valid of variable
	assert(pszDest && pszSrc);

	if (0 == iCount || iCount > (int)lstrlen(pszSrc))
	{
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
		
	SAFEMEMSET(pszDest, 0, sizeof(pszDest));	
	
	// iCount ���� ��ŭ ���ڸ� ��´�.
	char			szOneChar[5] = {0, };		// byte per character
	char *			pszNextChar;
	char *			pszPrevChar;
	int				iByteLen = 0;
	pszNextChar = (char *)pszPrevChar = (char *)pszSrc;

	for (int i = 0; i < iCount; i++)
	{	
		// ���� ���ڷ� �̵� 
		pszNextChar = CharNextExA(0, pszNextChar, 0);

		// �� ���� �����Ѵ�.(Copy the one of character)
		iByteLen = (int)(pszNextChar - pszPrevChar);
		lstrcpyn(szOneChar, pszPrevChar, iByteLen+1);	// auto terminal null character, because do + 1
		lstrcat(pszDest, szOneChar);
		
		// �������� ���� 
		pszPrevChar = pszNextChar;									
	}

	return TRUE;
}


// ---------------------------------------------
// Name		: FindBeforeTokenStrings
// Desc		: ��Ʈ�� ������ ��ū�� �����ϸ� ������ ���ڿ����� 
//			  �����ִ� �Լ����̴�.
//			  ���ϵǴ� ���ڿ��� �۷ι��� ������ ��Ƴ��� ���ڿ� ���� �����̴�.
// Aim		: �ϴ� �ӵ����� ��Ȯ���� ���뼺�� ���ο� �д�.
// Progress : ����� �������� ���� ���������� ��ū�� ������ �����ְ� �Ѵٸ�
//			  ��ū ���� ���� �����ڸ� �ξ� ������带 ���ϼ� �ִ�.
// Date / Writer :  [7/5/2004] / Seven
// ---------------------------------------------
char * 
FindBeforeTokenStrings(char * IN pszSrc, 
				 char ** IN ppszSeps, 
				 int iTokenStringCount)
{
	assert(iTokenStringCount > 0);
	assert(pszSrc && ppszSeps && *ppszSeps);

	int i, j;
	int iRet = 0;
	int iTokenLen = 0;
	int iSrcLen	= lstrlen(pszSrc);
//	int iTokenPos = 0;

	if (iSrcLen == 0)	return NULL;
	
	for (i = 0; i < iSrcLen; i++)
	{
		for (j = 0; j < iTokenStringCount; j++)
		{
			iTokenLen = lstrlen(ppszSeps[j]);

			if ( int(lstrlen(pszSrc+i) - iTokenLen) < 0 )
				continue;

			iRet = CompareString(LOCALE_USER_DEFAULT, NORM_IGNORECASE, pszSrc+i, iTokenLen, ppszSeps[j], iTokenLen );
			if (CSTR_EQUAL  == iRet)
			{
				__lstrcpyn(g_szToken, pszSrc, i);
				g_szToken[i] = 0;
				return (char *)(g_szToken);				
			}
		}
	}
	
	
	// ������ �����ϴ� ���ڿ��� �ѱ��.
	// �ϴ� ������� �Դٴ°���
	// ���̻� ���� �ִ� ��ū�� ���ٴ� ���̴�.
	__lstrcpyn(g_szToken, pszSrc, iSrcLen);
	g_szToken[i] = 0;
	return (char *)(g_szToken);
}




// ---------------------------------------------
// Name : OneCharPerStrLen
// Desc : �ؿ� / ���� ���� 
//		  ������ ���������� ���ڰ����� ���´�.
//		  ���ڿ�(����)�� ����
// ---------------------------------------------
int OneCharPerStrLen(const char * IN pszSrc)
{
	assert(pszSrc);

	char * pszPos = (char *)pszSrc;

	int i = 0;
	for(;;)
	{	
		pszPos= CharNextExA(0, pszPos, 0);
		i++;
		if ( NULL == *pszPos ) break;
	}

	return i;
}

// ---------------------------------------------
// Name : IsThereString
// Desc : �ؿ� / ���� ���� 
//		  pszSrc ���ڿ��� � pszDest �� �����ϴ°�?
// 
// parameters : 
//	[OUT]	pszSrc - ���� ���ڿ��� �޾ƿ� ����
//  [IN]	pszDest	- ���� ���ڿ��� ����   
// date / writer :  [10/18/2004] / seven
// ---------------------------------------------
BOOL IsThereString(const char * IN pszSrc, const char * IN pszDest) 
{
	if (NULL == pszSrc && NULL == pszDest)
	{
		return FALSE;
	}

	int iSrcStringLenth = OneCharPerStrLen(pszSrc); 
	int iDestStringLenth = OneCharPerStrLen(pszDest); 

	if (iSrcStringLenth < iDestStringLenth)
	{
		return FALSE;
	}

	char *		pszSrcTempPos = (char *)&pszSrc[0];
//	int			iTokenLen = 0;
	int			iRet = 0;
	int			iTokenDestLen = lstrlen(pszDest);
	for (int i = 0; i < int(iSrcStringLenth - iDestStringLenth); i++)
	{
		iRet = CompareString(LOCALE_USER_DEFAULT, NULL, pszSrcTempPos, iTokenDestLen, pszDest, iTokenDestLen );
		if (CSTR_EQUAL  == iRet)
		{
			return TRUE;
		}

		pszSrcTempPos = CharNextExA(0, pszSrcTempPos, 0);
		if ( NULL == pszSrcTempPos ) break;
	}

	return FALSE;
}


// ---------------------------------------------
// Name : GetToken
// Desc : 1. ��ū ���ڸ� ��� 
//		  2. ��ū ���� ���� �� ���� ��ġ�� �������ش�. 
//
// parameters :
//   [IN]	pszSrc		: ���� ��Ʈ��
//	 [OUT]	pszGetToken	: ��ū�� �޾ƿ� ������ ���� 
//			cToken		: ��ū ���� 
//
// return :
//		��ū ���� ��� ��ū ���� �����͸� �����Ѵ�.
//
// date / writer: [10/29/2004] / seven 
// ---------------------------------------------
char * GetToken(char * IN pszSrc, char * OUT pszGetToken, char cToken /* = ' ' */)
{
	assert(pszGetToken);

	if (!pszSrc)	return NULL;

	int iLen = OneCharPerStrLen(pszSrc);
	if (0 == iLen)
	{
		// ���� ���� �̻� ���̻� ��ū ���� �ʿ䰡 ����.
		return NULL;
	}
	else 
	if (1 == iLen)
	{
		// ���ڿ��� �ϳ��̸� ���̻� �� �ʿ� ����. 
		__lstrcpyn(pszGetToken, pszSrc, iLen);
		return NULL;
	}

	// �ּ� ���ڿ��� ���̰� 2 �̻��̴�.

	int		iTokenCount = 0;
	char* pszTempPos = pszSrc;
	for (pszTempPos = pszSrc; NULL != *pszTempPos; 
		 pszTempPos = CharNextExA(0, pszTempPos, 0))
	{
		if (*pszTempPos == cToken)
		{
			iTokenCount++;
			pszTempPos = CharNextExA(0, pszTempPos, 0);		// ������ ��ū�� ��ŵ 
			break;
		}
	}

	int iTokenLen = (pszTempPos-pszSrc) - iTokenCount;

	// 1. ��ū������ ���ڸ� ��´�. 

	__lstrcpyn(pszGetToken, pszSrc, iTokenLen);

	// 2. ��ū�� ������ �����ͷ� ���� �����͸� ���� 
	
	return (NULL == *pszTempPos)? NULL : pszTempPos;		
}
