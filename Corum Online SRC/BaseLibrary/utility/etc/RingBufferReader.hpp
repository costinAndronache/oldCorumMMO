#pragma once

#include <iostream>

/**
 * �����ۿ��� memcpy ���� ������ ������ ���� ���� �뵵�� �̿�.
 * ����μ� �� ���������� ���� ����� ������ �ִ�.
 * short,WORD - 2bytes, int,DWORD - 4bytes, 
 */

typedef class CRingBufferReader*		LPCRingBufferReader;
class CRingBufferReader
{
public:
	inline CRingBufferReader( LPBYTE pBufferLinearStart, LPBYTE pBufferStart, LPBYTE pBufferLinearEnd, LPBYTE pBufferEnd );
	inline ~CRingBufferReader( void );

public:
	inline LPBYTE	Skip( size_t nSize = 1); // ���� ���� �����͸� nSize ��ŭ �̵��ϸ� ��ȯ�� ���� �����͸� ��ȯ��.
	inline size_t	GetLeftSizeToLinearEnd( void );

public:
	inline CHAR		GetCHAR( void );
	inline BYTE		GetBYTE( void );
	inline short	GetSHORT( void );
	inline WORD		GetWORD( void );
	inline INT		GetINT( void );
	inline DWORD	GetDWORD( void );
	inline INT64	GetINT64( void );
	inline DWORD64	GetDWORD64( void );
	inline void		GetSTRING( LPBYTE szBuffer );//String ���� StrLenByte + StrData �� �����Ѵ�!
	inline void		GetBINARY( LPBYTE szBuffer, size_t nLen );// ���ڿ��� 1����Ʈ�� ���ӵ� ������ ������ ����� Ȱ��.

	inline LPBYTE	GetCurrentPtr() { return m_pCurrentPtr; }

private:
	LPBYTE	m_pBufferLinearStart;	// ��ü �������� ������ ��������(Including)
	LPBYTE	m_pBufferStart;		// �б� ������ ����(Including)
	LPBYTE	m_pBufferLinearEnd;		// ��ü �������� ������ ��������(Excluding)
	LPBYTE	m_pBufferEnd;			// �б� ������ ����(Excluding)
	LPBYTE	m_pCurrentPtr;		// �����ۿ��� ���� ���� ���� ������
};

CRingBufferReader::CRingBufferReader( LPBYTE pBufferLinearStart, LPBYTE pBufferStart, LPBYTE pBufferLinearEnd, LPBYTE pBufferEnd ) : m_pBufferLinearStart(pBufferLinearStart), m_pBufferStart(pBufferStart), m_pBufferLinearEnd(pBufferLinearEnd), m_pBufferEnd(pBufferEnd), m_pCurrentPtr(pBufferStart)
{
}

CRingBufferReader::~CRingBufferReader( void )
{
}

LPBYTE CRingBufferReader::Skip( size_t nSize )
{
	LPBYTE lpRet = m_pCurrentPtr;

	if( m_pCurrentPtr + nSize < m_pBufferLinearEnd )
		m_pCurrentPtr += nSize;
	else //m_pCurrentPtr + nSize == m_pBufferLinearEnd �� ��� ��������
		m_pCurrentPtr = m_pBufferLinearStart + nSize - GetLeftSizeToLinearEnd();
	
	return lpRet;
}

size_t CRingBufferReader::GetLeftSizeToLinearEnd( void )
{
	return m_pBufferLinearEnd - m_pCurrentPtr;
}


CHAR CRingBufferReader::GetCHAR(void)
{
	register CHAR chValue;

	chValue = *((CHAR *)m_pCurrentPtr);

	Skip(sizeof(CHAR));

	return chValue;
}

BYTE CRingBufferReader::GetBYTE(void)
{
	register BYTE byValue;

	byValue = *((BYTE *) m_pCurrentPtr);

	Skip(sizeof(BYTE));

	return byValue;
}

short CRingBufferReader::GetSHORT(void)
{
	short sValue;

	if( sizeof(short) > GetLeftSizeToLinearEnd() )
	{
		LPBYTE pOne		= Skip();
		LPBYTE pTwo		= Skip();

		sValue = *pTwo;
		sValue <<= 8;
		sValue += *pOne;
	}
	else
	{
		sValue = *((short *) m_pCurrentPtr);
		Skip( sizeof(short) );
	}

	return sValue;
}

WORD CRingBufferReader::GetWORD(void)
{
	WORD wValue;

	if( sizeof(WORD) > GetLeftSizeToLinearEnd() )
	{
		LPBYTE pOne		= Skip();
		LPBYTE pTwo		= Skip();

		wValue = *pTwo;
		wValue <<= 8;
		wValue += *pOne;
	}
	else
	{
		wValue = *((WORD *) m_pCurrentPtr);
		Skip( sizeof(WORD) );
	}
	return wValue;
}

INT CRingBufferReader::GetINT(void)
{
	INT nValue;

	if( sizeof(INT) > GetLeftSizeToLinearEnd() )
	{
		LPBYTE pOne		= Skip();
		LPBYTE pTwo		= Skip();
		LPBYTE pThree	= Skip();
		LPBYTE pFour	= Skip();

		nValue = *pFour;
		nValue <<= 8;
		nValue += *pThree;
		nValue <<= 8;
		nValue += *pTwo;
		nValue <<= 8;
		nValue += *pOne;
	}
	else
	{
		nValue = *((INT *) m_pCurrentPtr);
		Skip( sizeof(INT) );
	}

	return nValue;
}

DWORD CRingBufferReader::GetDWORD(void)
{
	DWORD dwValue;

	if( sizeof(DWORD) > GetLeftSizeToLinearEnd() )
	{
		LPBYTE pOne		= Skip();
		LPBYTE pTwo		= Skip();
		LPBYTE pThree	= Skip();
		LPBYTE pFour	= Skip();

		dwValue = *pFour;
		dwValue <<= 8;
		dwValue += *pThree;
		dwValue <<= 8;
		dwValue += *pTwo;
		dwValue <<= 8;
		dwValue += *pOne;
	}
	else
	{
		dwValue = *((DWORD *) m_pCurrentPtr);
		Skip( sizeof(DWORD) );
	}

	return dwValue;
}

INT64 CRingBufferReader::GetINT64(void)
{
	INT64 i64Value;

	if( sizeof(INT64) > GetLeftSizeToLinearEnd() )
	{
		LPBYTE pOne		= Skip();
		LPBYTE pTwo		= Skip();
		LPBYTE pThree	= Skip();
		LPBYTE pFour	= Skip();
		LPBYTE pFive	= Skip();
		LPBYTE pSix		= Skip();
		LPBYTE pSeven	= Skip();
		LPBYTE pEight	= Skip();

		i64Value = *pEight;
		i64Value <<= 8;
		i64Value += *pSeven;
		i64Value <<= 8;
		i64Value += *pSix;
		i64Value <<= 8;
		i64Value += *pFive;
		i64Value <<= 8;
		i64Value += *pFour;
		i64Value <<= 8;
		i64Value += *pThree;
		i64Value <<= 8;
		i64Value += *pTwo;
		i64Value <<= 8;
		i64Value += *pOne;
	}
	else
	{
		i64Value = *((INT64 *) m_pCurrentPtr);
		Skip( sizeof(INT64) );
	}

	return i64Value;
}

DWORD64 CRingBufferReader::GetDWORD64(void)
{
	DWORD64 dw64Value;

	if( sizeof(DWORD64) > GetLeftSizeToLinearEnd() )
	{
		LPBYTE pOne		= Skip();
		LPBYTE pTwo		= Skip();
		LPBYTE pThree	= Skip();
		LPBYTE pFour	= Skip();
		LPBYTE pFive	= Skip();
		LPBYTE pSix		= Skip();
		LPBYTE pSeven	= Skip();
		LPBYTE pEight	= Skip();

		dw64Value = *pEight;
		dw64Value <<= 8;
		dw64Value += *pSeven;
		dw64Value <<= 8;
		dw64Value += *pSix;
		dw64Value <<= 8;
		dw64Value += *pFive;
		dw64Value <<= 8;
		dw64Value += *pFour;
		dw64Value <<= 8;
		dw64Value += *pThree;
		dw64Value <<= 8;
		dw64Value += *pTwo;
		dw64Value <<= 8;
		dw64Value += *pOne;
	}
	else
	{
		dw64Value = *((DWORD64 *) m_pCurrentPtr);
		Skip( sizeof(DWORD64) );
	}

	return dw64Value;
}

void CRingBufferReader::GetSTRING( LPBYTE szBuffer )
{
	BYTE byStrLen = GetBYTE();
	GetBINARY(szBuffer, byStrLen);
}

void CRingBufferReader::GetBINARY( LPBYTE szBuffer, size_t len )
{
	size_t leftSize = GetLeftSizeToLinearEnd();
	if( len > leftSize )
	{
		memcpy( szBuffer, m_pCurrentPtr, leftSize );
		memcpy( szBuffer + leftSize, m_pBufferLinearStart, len - leftSize );
	}
	else
	{
		memcpy( szBuffer, m_pCurrentPtr, len );
	}

	Skip( len );
}

