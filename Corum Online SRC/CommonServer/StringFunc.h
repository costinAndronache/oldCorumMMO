#ifndef __STRING_FUNC_H__
#define __STRING_FUNC_H__


#pragma once


#include "CommonServer.h"


// �ٱ��� ������ ���� �ζ��� ���ڿ� ���Լ��� �����Ѵ�


inline int COMMONSERVERDECLSPECIFIER __strcmp(const char* szString1, const char* szString2)
{
	return (CompareString(LOCALE_USER_DEFAULT, 0, szString1, lstrlen(szString1), szString2, lstrlen(szString2)) - 2);
}

inline int COMMONSERVERDECLSPECIFIER __stricmp(const char* szString1, const char* szString2)
{
	return (CompareString(LOCALE_USER_DEFAULT, NORM_IGNORECASE|NORM_IGNOREKANATYPE|NORM_IGNOREWIDTH, szString1, lstrlen(szString1), szString2, lstrlen(szString2)) - 2);
}

inline int COMMONSERVERDECLSPECIFIER __strncmp(const char* szString1, const char* szString2, int nLength)
{
	return (CompareString(LOCALE_USER_DEFAULT, 0, szString1, nLength, szString2, nLength) - 2);
}

inline int COMMONSERVERDECLSPECIFIER __strnicmp(const char* szString1, const char* szString2, int nLength)
{
	return (CompareString(LOCALE_USER_DEFAULT, NORM_IGNORECASE|NORM_IGNOREKANATYPE|NORM_IGNOREWIDTH, szString1, nLength, szString2, nLength) - 2);
}

inline LPSTR COMMONSERVERDECLSPECIFIER __lstrcpyn(const LPSTR lpString1, const LPSTR lpString2, int iMaxLength)
{
	return lstrcpyn(lpString1, lpString2, iMaxLength+1);
}

inline bool COMMONSERVERDECLSPECIFIER IsEmptyString(const char* szString)
{
	return (lstrlen(szString) == 0);
}


#endif 