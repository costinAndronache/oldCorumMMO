
#pragma once
#include "../../stdafx.h"
#include <time.h>

#define BYTES_PER_LINE		16

/**
 * �ش� ������ �޸� ���� hex �ڵ�� ������ ���. 
 *
 * @param fileName		���� ������ ������ ���ϸ�
 * @param dw64Offset	������ ���� ������
 * @param dwSize		������ ���� ������ ũ��
 * @param bAlign		    ���ٷ� ���� ����(�⺻�� : FALSE)
 */
static void HexaDump( LPSTR fileName, DWORD64 dw64Offset, DWORD dwSize, BOOL bAlign = FALSE, LPSTR AddInfo = NULL )
{
	DWORD dwLoc, dwILoc, dwX;
	LPBYTE pOut = (LPBYTE) dw64Offset;
	TCHAR szOutput[BYTES_PER_LINE * 6];

	HFILE handle = _lopen( fileName, OF_WRITE);	
	if( handle == HFILE_ERROR) {
		handle = _lcreat( fileName, 0);
	}
	else _llseek( handle, 0, FILE_END);

	//////////////////////////////////////////////////////////////////////////////////////////////
	TCHAR sztemp[11] = {0,};
	time_t tval;
	time( &tval );
	struct tm *currTM = localtime( &tval );
	sprintf( sztemp, "%02d:%02d:%02d  ", currTM->tm_hour, currTM->tm_min, currTM->tm_sec );
	_lwrite( handle, sztemp, (UINT)10 );

	if( AddInfo )
		_lwrite( handle, AddInfo, (UINT)(_tcslen( AddInfo )) );
	_lwrite( handle, "\r\n", (UINT)2 );
	/////////////////////////////////////////////////////////////////////////////////////////////

	if( bAlign == TRUE )
		pOut = (LPBYTE) ((dw64Offset >> 4) << 4);

	for( dwLoc = 0; dwLoc < dwSize; dwLoc += 16, pOut += BYTES_PER_LINE ) {
		LPBYTE pLine = pOut;

		_stprintf( szOutput, _T( "%08lX: " ), (DWORD64) pOut );
		for( dwX = 0, dwILoc = dwLoc; dwX < BYTES_PER_LINE; dwX++ ) {
			if( dwX == (BYTES_PER_LINE / 2) )
				_stprintf( szOutput + _tcslen( szOutput ), _T( " " ) );
			if( dwILoc++ >= dwSize ) {
				_stprintf( szOutput + _tcslen( szOutput ), _T( "?? " ) );
			} else {
				_stprintf( szOutput + _tcslen( szOutput ), _T( "%02X " ), *(pLine++) );
			}
		}
		pLine = pOut;
		_stprintf( szOutput + _tcslen( szOutput ), _T( " " ) );
		for( dwX = 0, dwILoc = dwLoc; dwX < BYTES_PER_LINE; dwX++ ) {
			if( dwILoc++ >= dwSize ) {
				_stprintf( szOutput + _tcslen( szOutput ), _T( " " ) );
			} else {
				_stprintf( szOutput + _tcslen( szOutput ), _T( "%c" ), isprint( *pLine ) ? *pLine : '.');
				pLine++;
			}
		}
		//fprintf(stdout, "%d", _tcslen( szOutput ));//76

		_stprintf( szOutput + _tcslen( szOutput ), _T( "\r\n" ) );//���� �ڿ� �ٹٲ� ���� ����.(0x0d0a00).<-�ڵ����� 00 �پ ���ڿ� �� ǥ��. _tcslen(szOutput) �� ���� ���ۿ� ����ִ� ���ڿ�ũ��!

		//fprintf( stdout, "%s", szOutput );
		_lwrite( handle, szOutput, (UINT)(_tcslen( szOutput )) );
	}
	_stprintf( szOutput, _T( "\r\n" ) );//�������� ���۸� �ٹٲ����� ������ ��.

	//fprintf(stdout, "%d", _tcslen( szOutput ));//2 <-- ���ڿ� �� ǥ�� 00 ������ �� ����.

	_lwrite( handle, szOutput, (UINT)(_tcslen( szOutput )) );//�ٹٲ� ���� �ѷ���.
	_lclose( handle);
}


/**
 * �ش� �������� ��Ʈ���� ���Ϸ� ����Ѵ�.
 *
 * @param szFilename ��Ʈ�� ������ ������ ���ϸ�
 * @param szMsg	    ��Ʈ�� ���� ������
 * @param  ...				va_list
  */
static void StringDump( char* szFilename, char* szMsg, ... )
{
	char szOutBuff[256] = {0,};
	va_list	v1;
	va_start( v1, szMsg );
	vsprintf( szOutBuff, szMsg, v1 );
	va_end(v1);

	//FILE* fp;
	//fp = fopen( szFilename, "a" );
	HFILE handle = _lopen( szFilename, OF_WRITE);	
	if( handle == HFILE_ERROR) {
		handle = _lcreat( szFilename, 0);
	}
	else _llseek( handle, 0, FILE_END);

	////////////////////////////////////////////////////////////////////////	
	TCHAR sztemp[23] = {0,};

	time_t tval;
	time( &tval );
	struct tm *currTM = localtime( &tval );

	sprintf( sztemp, "[%04d/%02d/%02d %02d:%02d:%02d] ",
			currTM->tm_year+1900, currTM->tm_mon+1, currTM->tm_mday,
			currTM->tm_hour, currTM->tm_min, currTM->tm_sec );
	_lwrite( handle, sztemp, (UINT)22 );

	_lwrite( handle, szOutBuff, (UINT)(strlen( szOutBuff )) );

	////////////////////////////////////////////////////////////////////////

	sprintf( sztemp, "\r\n" );
	_lwrite( handle, sztemp, (UINT)2 );

	//fprintf( fp, "%s\n", szOutBuff );
	//fflush( fp );
	//fclose( fp );

	_lclose( handle);
}
