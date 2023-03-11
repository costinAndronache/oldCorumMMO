#include "../../StdAfx.h"
#include "ForbidWord.h"

LPCFORBIDWORD	CFORBIDWORD::m_gpForbidWord = NULL;

CFORBIDWORD::CFORBIDWORD()
{
	m_bCheck = false;
}

CFORBIDWORD::~CFORBIDWORD()
{
}

void CFORBIDWORD::LoadForbidWord( char* szFilename )
{
	m_lSlander.clear();
	char szLine[128];
	FILE* pfile;

	if( szFilename == NULL )	pfile = fopen( g_szDefaultFilename, "rt" );
	else						pfile = fopen( szFilename, "rt" );

	if( pfile == NULL )		{	m_bCheck = false;		return;		}

	while( fscanf( pfile, "%s", szLine ) != EOF )
	{		m_lSlander.push_back( szLine );		}

	fclose( pfile );
	pfile = NULL;
	m_bCheck = true;
}
void CFORBIDWORD::UnloadForbidWord()
{
	m_lSlander.clear();
}

bool CFORBIDWORD::IsForbidWord( char* szString )
{
	if( !m_bCheck )	return false;
	// ��ҹ��� ���� ���� üũ �����ϰ�
	//char *temp = _strupr( _strdup( szString ) );
	char temp[512] = { 0,};
	strcpy( temp, szString );
	std::string CheckString(temp);

	// ���� ����
	int a_pos;
	while( (a_pos = CheckString.find (" ")) > -1 )
	{
		CheckString = CheckString.replace (a_pos, 1, "");
	}

	// ���� �ܾ� ���ۿ��� ã�� ���Ѵ�.
	a_pos = 0;
	for( std::list< std::string >::iterator it = m_lSlander.begin(); it != m_lSlander.end(); it++ )
	{
		a_pos = CheckString.find( it->c_str() );
		if( a_pos > -1 )
		{
			// �����ܾ��� ��� �ܾ����� Ȯ�� ��ƾ




			return true;
		}
	}

	// ���ڸ��� �̻� ���� ����
	int nNumCnt = 0, i = 0;
	while( i < (int)CheckString.length() )
	{
		if( CheckString[i] >= '0' && CheckString[i] <= '9' ) ++nNumCnt;
		++i;
	}
	if( nNumCnt > 2 ) return true;

	return false;
}