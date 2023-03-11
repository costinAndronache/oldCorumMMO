//
// File: CLowFile.cpp
// Desc: File class
//
// Auth: Standil (cpascal@nownuri.net)
//

#ifndef _CLOWFILE_H_
#define _CLOWFILE_H_

#include "OpenMode.h"

typedef class CLowFile*		LPCLowFile;
class CLowFile {
public:
	CLowFile();
	~CLowFile();

	enum SeekFlags {
		seekBegin	= 0,
		seekCurrent = 1,
		seekEnd		= 2
	};

	void	Close( void );											// ���� �ݱ�
	BOOL	Open( const LPCSTR szFilename, UINT nOpenFlags = modeRead );	// ���� ����
	BOOL	OpenReadf( LPCTSTR szFilename, ... );
	BOOL	OpenWritef( LPCTSTR szFilename, ... );
	BOOL	OpenCreatef( LPCTSTR szFilename, ... );

	int		Size( void );											// ���� ���� ���
	int		GetFilePointer( void );									// ���� ��ġ ���
	int		Read( LPVOID lpBuffer, DWORD dwBytesToRead );			// ���� �б�
	int		Write( LPVOID lpBuffer, DWORD dwBytesToRead );			// ���� ����
	int		Seek( LONG dwBytesToMove , SeekFlags dwMoveMethod );	// ��ġ �̵�
	BOOL	IsEOF( void );											// ������ �˻�

	// ���ڷ� �����̸��� �����ϴ� �������̽� �Լ���
	BOOL	Access( LPCSTR szFilename );		// ���� ���翩�� �˻�
	int		Size( LPCSTR szFilename );			// ���� ���� ��ȯ(-1 = ����)
	int		Read( LPCSTR szFilename, LPVOID lpBuffer, DWORD dwBytesToRead );
	int		Write( LPCSTR szFilename, LPVOID lpBuffer, DWORD dwBytesToRead );

private:
	HANDLE	m_hFile;
	TCHAR	m_szFilename[MAX_PATH];
};

#endif
