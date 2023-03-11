// ====================================================================
// Debug.h
// ====================================================================

#ifndef DEBUG_H
#define DEBUG_H

// ====================================================================
// Les prototypes de fonctions
// ====================================================================

void     DisplayRect(LPCTSTR lpszText, RECT &stRect);
char    *GetBinaryString(short sValue, char *szBuf = NULL);
BOOL     GetBit(int nBit, LPARAM lParam);
LPCTSTR  GetNotificationString(UINT uiCode);
LPCTSTR  GetMessageString(DWORD dwMessage);
LPCTSTR  GetScrollCodeString(int nScrollCode);
LPCTSTR  GetSizeType(WPARAM fwSizeType);

#ifndef _AFXDLL
//void TRACE(char *szText, ...);
#endif

// ====================================================================
// ====================================================================

#endif