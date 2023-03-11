//==================================================//
// Code by Jang.						2002.10.15	//
// Modified by deepdark for readonly ini file...    //
//==================================================//
#include "Ini.h"
//==================================================//
// Extern Function.									//
//==================================================//
BOOL WritePPSection(LPCTSTR lpAppName, LPCTSTR lpString, LPCTSTR lpFileName, BOOL bForceReadOnly)
{
	// iniȭ�Ͽ� �ش� Ű�� ���� ������ ����մϴ�.
	// ���ǰ� Ű�� ������ �ڵ������� �߰��մϴ�. 
	// "Ű=��"�� �������� �Ǿ��ִ� ���ڿ� ����Ÿ �ƴѶ� ���.
	// �Ķ���� : ���Ǹ�, ���ڿ�, ini ȭ�ϸ�(full-path�� �����ؾ���) 

	if(!WritePrivateProfileSection(lpAppName, lpString, lpFileName))
	{
		if(bForceReadOnly && (ERROR_ACCESS_DENIED == GetLastError()))
		{
			SetFileAttributes(lpFileName, FILE_ATTRIBUTE_NORMAL);
			return WritePrivateProfileSection(lpAppName, lpString, lpFileName);
		}

		return FALSE;
	}

	return TRUE;
}

BOOL WritePPSting(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString, LPCTSTR lpFileName, BOOL bForceReadOnly)
{
	// iniȭ�Ͽ� �ش� Ű�� ���� ������ ����մϴ�.
	// ���ǰ� Ű�� ������ �ڵ������� �߰��մϴ�. 
	// "Ű=��"�� �������� �Ǿ��ִ� ���ڿ� ����Ÿ.
	// �Ķ���� : ���Ǹ�, Ű�̸�, Ű��, ini ȭ�ϸ�(full-path�� �����ؾ���) 
	if(!WritePrivateProfileString(lpAppName, lpKeyName, lpString, lpFileName))
	{
		if(bForceReadOnly && (ERROR_ACCESS_DENIED == GetLastError()))
		{
			SetFileAttributes(lpFileName, FILE_ATTRIBUTE_NORMAL);
			return WritePrivateProfileString(lpAppName, lpKeyName, lpString, lpFileName);
		}

		return FALSE;
	}

	return TRUE;
}

DWORD GetPPS(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, 
				LPTSTR lpReturnedString, DWORD nSize, LPCTSTR lpFileName)
{
	// iniȭ�Ϸκ��� ���ڿ� Ÿ���� Ű���� �о�ɴϴ�.
	// �Ķ���� : ���Ǹ�, Ű �̸�, Ű�� �߰����� ������ ��� ��ȯ�Ǵ� ��,
	// ���� ���� ����, ���� ���� ������ �ִ����, ini ȭ�ϸ�(full-path�� �����ؾ���)
	return GetPrivateProfileString(lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize, lpFileName);
}

UINT GetPPI(LPCTSTR lpAppName, LPCTSTR lpKeyName, INT nDefault, LPCTSTR lpFileName)
{
	// iniȭ�Ϸκ��� ���� Ÿ���� Ű���� �о�ɴϴ�. 
	// ���Ǹ�, Ű �̸�, ���� Ű�� �߰ߵ��� ������ ��ȯ�Ǵ� ��,
	// ini ȭ�ϸ�(full-path�� �����ؾ���) 
	return GetPrivateProfileInt(lpAppName, lpKeyName, nDefault, lpFileName);
}
//==================================================//
// End.												//
//==================================================//
