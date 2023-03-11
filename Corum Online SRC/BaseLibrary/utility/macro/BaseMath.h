#pragma once


///////////////// ���� �Ǽ� ��� ���� /////////////////
#define PI					3.141592653589			//180��
#define PI2					6.283185307178			//360��
#define PIx2				PI2						//360��
#define PItime2				PI2						//360��
#define PIdiv2				1.570796326795			//90��
#define PIover2				PIdiv2					//90��
#define DEG90				PIdiv2					//90��
#define DEG45				0.7853981633974			//45��
#define DEG5				0.087266462599			//5��
#define DEG10				0.174532925199			//10��
#define DEG20				0.349065850398			//20��
#define DEG30				0.523598775597			//30��
#define DEG40				0.698131700796			//40��
#define DEG50				0.872664625995			//50��
#define DEG60				1.047197551194			//60��
#define DEG120				2.094395102388			//120��
#define DEG80				1.396263401592			//80��
#define DEG160				2.792526803191			//160��
#define DEG140				2.443460952792			//140��
#define SQRT2				1.4142135623731			//45*����
#define	SQRT_2				0.7071067811865			//1/SQRT2
#define logNE				2.71828182845904523536	//���� log��
#define logN2				0.69314718055994530942	//2�� ����
#define logN10				2.30258509299404568402	//10�� ����
#define RADtoDEG			57.2957795132
#define DEGtoRAD			0.0174532925199

//////////////// ���� �����Լ� ���� ///////////////////
#define	ODD(a)				((a)&1)									//Ȧ���� ��� ��(1)�� ����
#define EVEN(a)				(!(a)&1)								//¦���� ��� ��(1)�� ����
#define SGN(a)				(((a)<(0)) ? -1 : ((a)>(0)) ? 1 : 0)	//��ȣ�� ����
#define SQR(a)				((a)*(a))								//����
#define	CUBE(a)				((a)*(a)*(a))							//������	
#define ABS(a)				(((a)<(0)) ? -(a) : (a))				//���밪
#define ROUND(a)			((a)>0 ? (int)(a+.5) : -(int)(.5-a))	//�Ҽ������ϰ��� �ݿø�
#define TRUNC(a)			(int)(a)								//�Ҽ������ϰ��� ����
#define MIN(a,b)			(((a)<(b)) ? (a) : (b))					//�μ��� �������� ����
#define MAX(a,b)			(((a)>(b)) ? (a) : (b))					//�μ��� ū���� ����
#define MIN3(a,b,c)			(MIN(MIN(a,b),c))						//������ �������� ����
#define MAX3(a,b,c)			(MAX(MAX(a,b),c))						//������ ū���� ����
#define SWAP(a,b)			((a)^=(b)^=(a)^=(b))					//a xor b and b xor a and a xor b
#define SWAPPT(a,b)			(SWAP(a.y,b.y), SWAP(a.x,b.x))			//��(POINT)�� ��ǥ�� �ٲ�
#define RECIP(a)			((1)/(a))								//a^-1 ����
#define NOT(a)				(~(a))									//not
#define XOR(a,b)			((a)^(b))								//��Ÿ������
#define	ASL(a)				((a)<<1)								//�·� 1ĭ ����Ʈ
#define ASL2(a,b)			((a)<<(b))								//�·� bĭ ��ŭ ����Ʈ
#define ASR(a)				((a)>>1)								//��� 1ĭ ����Ʈ
#define ASR2(a,b)			((a)>>(b))								//��� bĭ ��ŭ ����Ʈ
#define LIMIT(val,l,h)		((val)<(l) ? (l) : (val)>(h) ? (h) : (val))		//���� ������ ����

#define TORAD(a)			((a)*(DEGtoRAD))						//�������� ���Ȱ����� ��ȯ
#define TODEG(a)			((a)*(RADtoDEG))						//���Ȱ��� ���������� ��ȯ

#define CSC(a)				(((a)==0) ? 0 : (1/sin(a)))				//�ڽ���Ʈ
#define SEC(a)				(((a)==0) ? 0 : (1/cos(a)))				//����Ʈ
#define COT(a)				(((a)==0) ? 0 : (1/tan(a)))				//��ź��Ʈ
#define ARCSIN(a)			(atan((x)/sqrt(-(x)*(x)+1)))			//��ũ����
#define ARCCOS(a)			(-atan((x)/sqrt(-(x)*(x)+1))+PIdiv2)	//��ũ�ڻ���
#define AVG(a,b)			(((a)+(b))/2)							//���
#define perppos(a)			((a)+PIdiv2)							//���簢�� �ð�ݴ������ ������
#define perpneg(a)			((a)-PIdiv2)							//���簢�� �ð������ ������
#define SUM(a,b)			((a)+(b))								//��

#define LO_BYTE(a)			((a)&=255)							//����byte����
#define	HI_BYTE(a)			((a)&=65280)						//����byte����

#define HI_LOSWAPshort(a)	{a=((((a)&0xff)<8) | (((a)&0xff00)>>8));}			//����byte�� ����byte�� �ٲ�
#define HI_LOSWAPlong(a)	{a=((((a)&0xffff)<<16) | (((a)&0xffff0000)<<16));}	//����word�� ����word�� �ٲ�
// BDH: Use Standard Math
#define DIST(a,b)			(int) (_hypot( (double) (a), (double) (b) ))
// #define DIST(a,b)			(float)(sqrt((float)((a)*(a))+((b)*(b))))					//������ �Ÿ� ���
#define ANGLE(x,y)			(float)((x!=0) ? (float)cos(x) : (float)sin(y))		//x�� y���� ������ ���� ���
#define TOCELSIUS(a)		(((a)-32/5*9)
#define TOFAREN(a)			(((a)*5/9)+32)
#define TOCENT(a)			((a)*2.54)
#define TOINCH(a)			((a)*.3937)
#define derive(a,x,b,c)		(((a)*(b))*pow((x),(b)-1))
#define VOLCYLINDER(r,h)	((h)*(PI*((r)*(r))))								//�Ǹ����� ����
#define VOLCUBE(s)			((s)*(s)*(s))										//������
#define VOLSPHERE(r)		(PI*((r)*(r)*(r)))									//���ǿ���
#define SURCYLINDER(r,h)	((h)*2*(r)*PI)										//ǥ����
#define	SURCUBE(s)			((s)*(s)*6)
#define SURSPHERE(s)		(3*PI*(r)*(r))
#define SURCONE(r,h)		(((PI*(r))*sqrt((r)*(r)+(h)*(h)))
#define AREARECT(x,y)		((x)*(y))
#define AREACONE(r,h)		(((PI*(r)*(r))*(h)/3)
#define AREACIRCLE(r)		((PI*(r)*(r))
#define AREATRIANGLE(b,h)	((b)*(h)*.5)
#define GALTOLITRE(a)		((a)*3.78541)
#define LITRETOGAL(a)		((a)*.264172)
#define LB_toKG(a)			((a)*.453592)
#define KG_toLB(a)			((a)*2.20462)
#define GRAVITY(m1,m2,dist)	((((m1)+(m2))*.0000000000667)/((dist)*dist))		//�ΰ��� �η�
#define ACCELERATION(f,m)	((f)/(m))
#define VALOCITY(acc,time)	((acc)/(time))
#define WORK(m,v)			((m)*(v)*(v))
#define NEWTON(kg,meter)	((kg)*(meter))


namespace BaseMath
{
	extern const unsigned short	m_FastHeToBi[0x100];
	extern const char*			PrefixOfHexadecimal;

	//// String�� ������ �ٲ�(String�� 0x�� ���� Hex string�̴�.)
	//inline char			StrToHex08(char *szSrc);
	//inline unsigned short	StrToHex16(char *szSrc);
	//inline unsigned int	StrToHex32(char *szSrc);
	//inline __int64		StrToHex64(char *szSrc);

	// String�� ������ �ٲ�(0x�� �ٴ���, ���� �ʴ������� ���� �ٲ� �� �ִ�)
	inline char				Atoc(char *szSrc);
	inline unsigned short	Atos(char *szSrc);
	inline unsigned int		Atoi(char *szSrc);
	inline __int64			Atol64(char *szSrc);

	//// ���� Hex String���� �ٲ�
	//inline void			Hex08ToStr(char *szDest, BYTE hex);
	//inline void			Hex16ToStr(char *szDest, WORD hex);
	//inline void			Hex32ToStr(char *szDest, DWORD hex);
	//inline void			Hex64ToStr(char *szDest, DWORD64 hex);

	inline void Hex08ToStr( char *szDest, BYTE hex )
	{
		*((WORD *) szDest) = m_FastHeToBi[ hex ];
		szDest += 2;
		*(szDest) = '\0';
	}

	inline void Hex16ToStr( char *szDest, WORD hex )
	{
		LPBYTE pSrc = (LPBYTE) &hex;

	#ifdef BIG_ENDIAN
		*((WORD *) (szDest +  0)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  2)) = m_FastHeToBi[ *(pSrc++) ];
	#else
		*((WORD *) (szDest +  2)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  0)) = m_FastHeToBi[ *(pSrc++) ];
	#endif
		*(szDest + 4) = '\0';
	}

	inline void Hex32ToStr( char *szDest, DWORD hex )
	{
		LPBYTE pSrc = (LPBYTE) &hex;

	#ifdef BIG_ENDIAN
		*((WORD *) (szDest +  0)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  2)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  4)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  6)) = m_FastHeToBi[ *(pSrc++) ];
	#else
		*((WORD *) (szDest +  6)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  4)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  2)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  0)) = m_FastHeToBi[ *(pSrc++) ];
	#endif
		*(szDest + 8) = '\0';
	}

	inline void Hex64ToStr( char *szDest, DWORD64 hex )
	{
		LPBYTE pSrc = (LPBYTE) &hex;

	#ifdef BIG_ENDIAN
		*((WORD *) (szDest +  0)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  2)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  4)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  6)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  8)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest + 10)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest + 12)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest + 14)) = m_FastHeToBi[ *(pSrc++) ];
	#else
		*((WORD *) (szDest + 14)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest + 12)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest + 10)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  8)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  6)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  4)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  2)) = m_FastHeToBi[ *(pSrc++) ];
		*((WORD *) (szDest +  0)) = m_FastHeToBi[ *(pSrc++) ];
	#endif
		*(szDest + 16) = '\0';
	}

	// this function returns the equivalent binary value for an individual character specified in the ascii format
	inline UCHAR BiToHe( char cBin )
	{
		if ( (cBin >= '0') && (cBin <= '9') )
			return ( cBin - '0' );
		else if ( (cBin >= 'A') && (cBin <= 'F') )
			return ( cBin - 'A' + 0xA );
		if ( (cBin >= 'a') && (cBin <= 'f') )
			return ( cBin -'a' + 0xA );

		return cBin;
	}

	inline char StrToHex08( char *szSrc )
	{
		char	*pStart		= szSrc + 2;
		char	cHex		= 0;

		for ( int i=0; i<1; i++ ) {
			char	c1		= BiToHe(*pStart++);
			c1 <<= (8*(7-i)+4);
			char	c2		= BiToHe(*pStart++);
			c2 <<= (8*(7-i));
			char	cRet	=  c1  + c2;
			cHex	+= cRet;

		}
		return cHex;
	}

	inline unsigned short StrToHex16( char *szSrc )
	{
		char	*pStart		= szSrc + 2;
		unsigned short	sHex		= 0;

		for ( int i=0; i<2; i++ ) {
			
			unsigned short	s1	= BiToHe(*pStart++);
			s1 <<= (8*(1-i)+4);
			unsigned short	s2	= BiToHe(*pStart++);
			s2 <<= (8*(1-i));
			unsigned short	sRet =  s1  + s2;

			sHex	+= (unsigned short) sRet;
		}

		return sHex;
	}

	// convert string to hexadecimal value
	inline unsigned int StrToHex32( char *szSrc )
	{
		char			*pStart		= szSrc + 2;
		unsigned int	nHex		= 0;

		for ( int i=0; i<4; i++ ) {
			unsigned int n1		= BiToHe(*pStart++);
			n1 <<= (8*(3-i)+4);
			unsigned int n2		= BiToHe(*pStart++);
			n2 <<= (8*(3-i));

			unsigned int nRet	=  n1  + n2;

			nHex += nRet;
		}

		return nHex;
	}

	// convert string to hexadecimal value
	inline __int64 StrToHex64( char *szSrc )
	{
		char	*pStart		= szSrc + 2;
		__int64	dlHex		= 0;

		for ( int i=0; i<8; i++ ) {
			__int64	dl1		= BiToHe(*pStart++);
			dl1 <<= (8*(7-i)+4);
			__int64	dl2		= BiToHe(*pStart++);
			dl2 <<= (8*(7-i));

			__int64	dlRet	=  dl1  + dl2;

			dlHex	+= dlRet;

		}
		return dlHex;
	}


	// convert string to value
	inline DWORD CommaStrToDWORD( char *szSrc )
	{
		char	*pStart			= szSrc;
		DWORD	dwValue			= 0;
		DWORD	dwFactor		= 1;

		int nLen	= static_cast<int>(strlen(pStart));
		for ( int i=(nLen-1); i>=0; --i) {
			if ( ( ' ' == pStart[i] ) || ( '\"' == pStart[i] ) || ( ',' == pStart[i] ) )
				continue;

			dwValue	+= ( BiToHe(pStart[i]) * dwFactor );
			dwFactor	*= 10;
		}

		return dwValue;
	}
};


/*
// FastMath.h: interface for the CFastMath class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <winsock2.h>
#include <windows.h>
#include <cmath>
#include <tchar.h>
#include <BaseLibrary/Pattern/Singleton.hpp>


#define FP_BITS(fp) (*(DWORD*)&(fp))

class CFastMath : public CSingleton<CFastMath> {
	
private:

	union FastSqrtUnion {
		float f;
		unsigned int i;
	};

	unsigned int m_FastSqrtTable[0x10000];

	static long m_HoldRand;

public:
	static const float	m_fPI;
	static const double	m_dPI;

	// constructor and destructor
	CFastMath();	
	virtual ~CFastMath();

	// -------------------------------------------------------------------------------------------------
	// ���� ������ ó�� �Լ� - ������ ���� ������ --;;;; deprecated by Sparrowhawk : 2002/11/08 5:22

	// void InitSqrt();	// ���� ������ ������ ���� ���̺� �ʱ�ȭ
	// inline float FastSqrt(float n);
	
	// -------------------------------------------------------------------------------------------------
	// ���ڿ� ��ȯ �Լ���
	
	// SPX, IPX �ּ� ����� ���� ����ϴ� �Լ�
	static unsigned char	BiToHe(char cBin);	
	static void				AcToHe(char *szDst, char *szSrc, int iCount);

	// -------------------------------------------------------------------------------------------------
	// String-to-Hash �Լ��� : http://www.cs.yorku.ca/~oz/hash.html ���� ����

	static inline unsigned long djb2Hash(const unsigned char *str);	// first reported by dan bernstein 
	static inline unsigned long sdbmHash(const unsigned char *str);	// this is one of the algorithms used in berkeley db
	static inline unsigned long looseHash(const unsigned char *str);	// ���� ������ �ؽ�. �׳� ��� ����.
};

inline float CFastMath::FastSqrt(float n) 
{
	if (FP_BITS(n) == 0) { return 0.0f; } // check for square root of 0
	FP_BITS(n) = m_FastSqrtTable[(FP_BITS(n) >> 8) & 0xFFFF] | ((((FP_BITS(n) - 0x3F800000) >> 1) + 0x3F800000) & 0x7F800000);
	return n;
};

inline unsigned long CFastMath::djb2Hash(const unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

	while (c = *str++) { hash = ((hash << 5) + hash) + c; } //hash * 33 + c
    return hash;
}


inline unsigned long CFastMath::sdbmHash(const unsigned char *str)
{
    unsigned long hash = 0;
    int c;

	while (c = *str++) { hash = c + (hash << 6) + (hash << 16) - hash; }
    return hash;
}

inline unsigned long CFastMath::looseHash(const unsigned char *str)
{
	unsigned int hash = 0;
	int c;

	while (c = *str++) { hash += c; }
	return hash;
}

// FastMath.cpp: implementation of the CFastMath class.
//
//////////////////////////////////////////////////////////////////////

#include "../StdAfx.h"
#include "FastMath.h"

CFastMath	g_FastMath;


long CFastMath::m_HoldRand = 1L;

CFastMath::CFastMath()
{
//	InitSqrt();
}

CFastMath::~CFastMath()
{

}

void CFastMath::InitSqrt()
{
	unsigned int	i;
	FastSqrtUnion	s;

	for(i = 0; i <= 0x7FFF; ++i) {
		s.i = (i << 8) | (0x7F << 23);
		s.f = (float)sqrt(s.f);
		m_FastSqrtTable[i+0x8000]=(s.i&0x7FFFFF);
		s.i = (i << 8) | (0x80 << 23);
		s.f = (float)sqrt(s.f);
		m_FastSqrtTable[i]=(s.i&0x7FFFFF);
	}
}

//
// this function returns the equivalent binary value for an individual character specified in the ascii format
UCHAR CFastMath::BiToHe( char cBin )
{
	if ( (cBin >= '0') && (cBin <= '9') ) {
		return ( cBin - '0' );
	} else 	if ( (cBin >= 'A') && (cBin <= 'F') ) {
		return ( cBin - 'A' + 0xA );
	} if ( (cBin >= 'a') && (cBin <= 'f') ) {
		return ( cBin -'a' + 0xA );
	}
	return cBin;
}

void CFastMath::AcToHe(char *szDst, char *szSrc, int iCount)
{
	while( iCount-- ) {
		*szDst = BiToHe(*szSrc) << 4;
		*szSrc++;
		*szDst += BiToHe(*szSrc);
		*szSrc++;
		*szDst++;
	}

��� ������ �������� �� ��ƾ���� �������ϸ� ����� �۵����� �ʽ��ϴ�(szDst�� �ƹ��͵� ���簡 �ȵ�). 
�ٸ� �е��� ����������? --������ (2002-08-17)

//	while (iCount--) {
//		*szDst++ = (BiToHe(*szSrc++) << 4) + BiToHe(*szSrc++); 
//	}
	return;
}

char CFastMath::Atoc( char *szSrc )
{
	if ( strncmp( szSrc, BaseMath::PrefixOfHexadecimal, strlen(BaseMath::PrefixOfHexadecimal) ) == 0 ) {
		return StrToHex08( szSrc );
	} else {
		return (char) _ttol( szSrc );
	}
}

//
// TCHAR *�� ��Ĩ�ô�! (By Standil)
//
unsigned short CFastMath::Atos( char *szSrc )
{
	if ( strncmp( szSrc, BaseMath::PrefixOfHexadecimal, strlen(BaseMath::PrefixOfHexadecimal) ) == 0 ) {
		return StrToHex16( szSrc );
	} else {
		return _ttoi( szSrc );
	}
}

//
// TCHAR *�� ��Ĩ�ô�! (By Standil)
//
unsigned int CFastMath::Atoi( char *szSrc )
{
	if ( strncmp( szSrc, , BaseMath::PrefixOfHexadecimal, strlen(BaseMath::PrefixOfHexadecimal) ) == 0 ) {
		return StrToHex32( szSrc );
	} else {
		return _ttol( szSrc );
	}
}

//
// TCHAR *�� ��Ĩ�ô�! (By Standil)
//
__int64 CFastMath::Atol64( char *szSrc )
{
	if ( strncmp( szSrc, , BaseMath::PrefixOfHexadecimal, strlen(BaseMath::PrefixOfHexadecimal) ) == 0 ) {
		return StrToHex32( szSrc );
	} else {
		return _ttoi64( szSrc );
	}
}

const float		CFastMath::m_fPI = 3.14159265358979323846f;
const double	CFastMath::m_dPI = 3.14159265358979323846;

const float CFastMath::m_fSinTable[360] = {
	0.000000f, 0.017452f, 0.034899f, 0.052336f, 0.069756f, 
	0.087156f, 0.104528f, 0.121869f, 0.139173f, 0.156434f, 
	0.173648f, 0.190809f, 0.207912f, 0.224951f, 0.241922f, 
	0.258819f, 0.275637f, 0.292372f, 0.309017f, 0.325568f, 
	0.342020f, 0.358368f, 0.374607f, 0.390731f, 0.406737f, 
	0.422618f, 0.438371f, 0.453990f, 0.469472f, 0.484810f, 
	0.500000f, 0.515038f, 0.529919f, 0.544639f, 0.559193f, 
	0.573576f, 0.587785f, 0.601815f, 0.615661f, 0.629320f, 
	0.642788f, 0.656059f, 0.669131f, 0.681998f, 0.694658f, 
	0.707107f, 0.719340f, 0.731354f, 0.743145f, 0.754710f, 
	0.766044f, 0.777146f, 0.788011f, 0.798636f, 0.809017f, 
	0.819152f, 0.829038f, 0.838671f, 0.848048f, 0.857167f, 
	0.866025f, 0.874620f, 0.882948f, 0.891007f, 0.898794f, 
	0.906308f, 0.913545f, 0.920505f, 0.927184f, 0.933580f, 
	0.939693f, 0.945519f, 0.951057f, 0.956305f, 0.961262f, 
	0.965926f, 0.970296f, 0.974370f, 0.978148f, 0.981627f, 
	0.984808f, 0.987688f, 0.990268f, 0.992546f, 0.994522f, 
	0.996195f, 0.997564f, 0.998630f, 0.999391f, 0.999848f, 
	1.000000f, 0.999848f, 0.999391f, 0.998630f, 0.997564f, 
	0.996195f, 0.994522f, 0.992546f, 0.990268f, 0.987688f, 
	0.984808f, 0.981627f, 0.978148f, 0.974370f, 0.970296f, 
	0.965926f, 0.961262f, 0.956305f, 0.951057f, 0.945519f, 
	0.939693f, 0.933580f, 0.927184f, 0.920505f, 0.913545f, 
	0.906308f, 0.898794f, 0.891007f, 0.882948f, 0.874620f, 
	0.866025f, 0.857167f, 0.848048f, 0.838671f, 0.829038f, 
	0.819152f, 0.809017f, 0.798636f, 0.788011f, 0.777146f, 
	0.766044f, 0.754710f, 0.743145f, 0.731354f, 0.719340f, 
	0.707107f, 0.694658f, 0.681998f, 0.669131f, 0.656059f, 
	0.642788f, 0.629320f, 0.615661f, 0.601815f, 0.587785f, 
	0.573576f, 0.559193f, 0.544639f, 0.529919f, 0.515038f, 
	0.500000f, 0.484810f, 0.469472f, 0.453990f, 0.438371f, 
	0.422618f, 0.406737f, 0.390731f, 0.374607f, 0.358368f, 
	0.342020f, 0.325568f, 0.309017f, 0.292372f, 0.275637f, 
	0.258819f, 0.241922f, 0.224951f, 0.207912f, 0.190809f, 
	0.173648f, 0.156434f, 0.139173f, 0.121869f, 0.104528f, 
	0.087156f, 0.069756f, 0.052336f, 0.034899f, 0.017452f, 
	0.000000f, -0.017452f, -0.034899f, -0.052336f, -0.069756f, 
	-0.087156f, -0.104528f, -0.121869f, -0.139173f, -0.156434f, 
	-0.173648f, -0.190809f, -0.207912f, -0.224951f, -0.241922f, 
	-0.258819f, -0.275637f, -0.292372f, -0.309017f, -0.325568f, 
	-0.342020f, -0.358368f, -0.374607f, -0.390731f, -0.406737f, 
	-0.422618f, -0.438371f, -0.453990f, -0.469472f, -0.484810f, 
	-0.500000f, -0.515038f, -0.529919f, -0.544639f, -0.559193f, 
	-0.573576f, -0.587785f, -0.601815f, -0.615661f, -0.629320f, 
	-0.642788f, -0.656059f, -0.669131f, -0.681998f, -0.694658f, 
	-0.707107f, -0.719340f, -0.731354f, -0.743145f, -0.754710f, 
	-0.766044f, -0.777146f, -0.788011f, -0.798636f, -0.809017f, 
	-0.819152f, -0.829038f, -0.838671f, -0.848048f, -0.857167f, 
	-0.866025f, -0.874620f, -0.882948f, -0.891007f, -0.898794f, 
	-0.906308f, -0.913545f, -0.920505f, -0.927184f, -0.933580f, 
	-0.939693f, -0.945519f, -0.951057f, -0.956305f, -0.961262f, 
	-0.965926f, -0.970296f, -0.974370f, -0.978148f, -0.981627f, 
	-0.984808f, -0.987688f, -0.990268f, -0.992546f, -0.994522f, 
	-0.996195f, -0.997564f, -0.998630f, -0.999391f, -0.999848f, 
	-1.000000f, -0.999848f, -0.999391f, -0.998630f, -0.997564f, 
	-0.996195f, -0.994522f, -0.992546f, -0.990268f, -0.987688f, 
	-0.984808f, -0.981627f, -0.978148f, -0.974370f, -0.970296f, 
	-0.965926f, -0.961262f, -0.956305f, -0.951057f, -0.945519f, 
	-0.939693f, -0.933580f, -0.927184f, -0.920505f, -0.913545f, 
	-0.906308f, -0.898794f, -0.891007f, -0.882948f, -0.874620f, 
	-0.866025f, -0.857167f, -0.848048f, -0.838671f, -0.829038f, 
	-0.819152f, -0.809017f, -0.798636f, -0.788011f, -0.777146f, 
	-0.766044f, -0.754710f, -0.743145f, -0.731354f, -0.719340f, 
	-0.707107f, -0.694658f, -0.681998f, -0.669131f, -0.656059f, 
	-0.642788f, -0.629320f, -0.615661f, -0.601815f, -0.587785f, 
	-0.573576f, -0.559193f, -0.544639f, -0.529919f, -0.515038f, 
	-0.500000f, -0.484810f, -0.469472f, -0.453990f, -0.438371f, 
	-0.422618f, -0.406737f, -0.390731f, -0.374607f, -0.358368f, 
	-0.342020f, -0.325568f, -0.309017f, -0.292372f, -0.275637f, 
	-0.258819f, -0.241922f, -0.224951f, -0.207912f, -0.190809f, 
	-0.173648f, -0.156434f, -0.139173f, -0.121869f, -0.104528f, 
	-0.087156f, -0.069756f, -0.052336f, -0.034899f, -0.017452f
};

const float CFastMath::m_fCosTable[360] = {
	1.000000f, 0.999848f, 0.999391f, 0.998630f, 0.997564f, 
	0.996195f, 0.994522f, 0.992546f, 0.990268f, 0.987688f, 
	0.984808f, 0.981627f, 0.978148f, 0.974370f, 0.970296f, 
	0.965926f, 0.961262f, 0.956305f, 0.951057f, 0.945519f, 
	0.939693f, 0.933580f, 0.927184f, 0.920505f, 0.913545f, 
	0.906308f, 0.898794f, 0.891007f, 0.882948f, 0.874620f, 
	0.866025f, 0.857167f, 0.848048f, 0.838671f, 0.829038f, 
	0.819152f, 0.809017f, 0.798636f, 0.788011f, 0.777146f, 
	0.766044f, 0.754710f, 0.743145f, 0.731354f, 0.719340f, 
	0.707107f, 0.694658f, 0.681998f, 0.669131f, 0.656059f, 
	0.642788f, 0.629320f, 0.615661f, 0.601815f, 0.587785f, 
	0.573576f, 0.559193f, 0.544639f, 0.529919f, 0.515038f, 
	0.500000f, 0.484810f, 0.469472f, 0.453990f, 0.438371f, 
	0.422618f, 0.406737f, 0.390731f, 0.374607f, 0.358368f, 
	0.342020f, 0.325568f, 0.309017f, 0.292372f, 0.275637f, 
	0.258819f, 0.241922f, 0.224951f, 0.207912f, 0.190809f, 
	0.173648f, 0.156434f, 0.139173f, 0.121869f, 0.104528f, 
	0.087156f, 0.069756f, 0.052336f, 0.034899f, 0.017452f, 
	0.000000f, -0.017452f, -0.034899f, -0.052336f, -0.069756f, 
	-0.087156f, -0.104528f, -0.121869f, -0.139173f, -0.156434f, 
	-0.173648f, -0.190809f, -0.207912f, -0.224951f, -0.241922f, 
	-0.258819f, -0.275637f, -0.292372f, -0.309017f, -0.325568f, 
	-0.342020f, -0.358368f, -0.374607f, -0.390731f, -0.406737f, 
	-0.422618f, -0.438371f, -0.453990f, -0.469472f, -0.484810f, 
	-0.500000f, -0.515038f, -0.529919f, -0.544639f, -0.559193f, 
	-0.573576f, -0.587785f, -0.601815f, -0.615661f, -0.629320f, 
	-0.642788f, -0.656059f, -0.669131f, -0.681998f, -0.694658f, 
	-0.707107f, -0.719340f, -0.731354f, -0.743145f, -0.754710f, 
	-0.766044f, -0.777146f, -0.788011f, -0.798636f, -0.809017f, 
	-0.819152f, -0.829038f, -0.838671f, -0.848048f, -0.857167f, 
	-0.866025f, -0.874620f, -0.882948f, -0.891007f, -0.898794f, 
	-0.906308f, -0.913545f, -0.920505f, -0.927184f, -0.933580f, 
	-0.939693f, -0.945519f, -0.951057f, -0.956305f, -0.961262f, 
	-0.965926f, -0.970296f, -0.974370f, -0.978148f, -0.981627f, 
	-0.984808f, -0.987688f, -0.990268f, -0.992546f, -0.994522f, 
	-0.996195f, -0.997564f, -0.998630f, -0.999391f, -0.999848f, 
	-1.000000f, -0.999848f, -0.999391f, -0.998630f, -0.997564f, 
	-0.996195f, -0.994522f, -0.992546f, -0.990268f, -0.987688f, 
	-0.984808f, -0.981627f, -0.978148f, -0.974370f, -0.970296f, 
	-0.965926f, -0.961262f, -0.956305f, -0.951057f, -0.945519f, 
	-0.939693f, -0.933580f, -0.927184f, -0.920505f, -0.913545f, 
	-0.906308f, -0.898794f, -0.891007f, -0.882948f, -0.874620f, 
	-0.866025f, -0.857167f, -0.848048f, -0.838671f, -0.829038f, 
	-0.819152f, -0.809017f, -0.798636f, -0.788011f, -0.777146f, 
	-0.766044f, -0.754710f, -0.743145f, -0.731354f, -0.719340f, 
	-0.707107f, -0.694658f, -0.681998f, -0.669131f, -0.656059f, 
	-0.642788f, -0.629320f, -0.615661f, -0.601815f, -0.587785f, 
	-0.573576f, -0.559193f, -0.544639f, -0.529919f, -0.515038f, 
	-0.500000f, -0.484810f, -0.469472f, -0.453990f, -0.438371f, 
	-0.422618f, -0.406737f, -0.390731f, -0.374607f, -0.358368f, 
	-0.342020f, -0.325568f, -0.309017f, -0.292372f, -0.275637f, 
	-0.258819f, -0.241922f, -0.224951f, -0.207912f, -0.190809f, 
	-0.173648f, -0.156434f, -0.139173f, -0.121869f, -0.104528f, 
	-0.087156f, -0.069756f, -0.052336f, -0.034899f, -0.017452f, 
	-0.000000f, 0.017452f, 0.034899f, 0.052336f, 0.069756f, 
	0.087156f, 0.104528f, 0.121869f, 0.139173f, 0.156434f, 
	0.173648f, 0.190809f, 0.207912f, 0.224951f, 0.241922f, 
	0.258819f, 0.275637f, 0.292372f, 0.309017f, 0.325568f, 
	0.342020f, 0.358368f, 0.374607f, 0.390731f, 0.406737f, 
	0.422618f, 0.438371f, 0.453990f, 0.469472f, 0.484810f, 
	0.500000f, 0.515038f, 0.529919f, 0.544639f, 0.559193f, 
	0.573576f, 0.587785f, 0.601815f, 0.615661f, 0.629320f, 
	0.642788f, 0.656059f, 0.669131f, 0.681998f, 0.694658f, 
	0.707107f, 0.719340f, 0.731354f, 0.743145f, 0.754710f, 
	0.766044f, 0.777146f, 0.788011f, 0.798636f, 0.809017f, 
	0.819152f, 0.829038f, 0.838671f, 0.848048f, 0.857167f, 
	0.866025f, 0.874620f, 0.882948f, 0.891007f, 0.898794f, 
	0.906308f, 0.913545f, 0.920505f, 0.927184f, 0.933580f, 
	0.939693f, 0.945519f, 0.951057f, 0.956305f, 0.961262f, 
	0.965926f, 0.970296f, 0.974370f, 0.978148f, 0.981627f, 
	0.984808f, 0.987688f, 0.990268f, 0.992546f, 0.994522f, 
	0.996195f, 0.997564f, 0.998630f, 0.999391f, 0.999848f
};


// -90 - 90
int CFastMath::as_asin(float as)
{
	double	nearest_diff = 2.0;
	int		nearest_index;

	for(int i = -90; i <=90; ++i) {
		int index = i;
		if (index < 0) {
			index += 360;
		}

		if (fabsf(m_fSinTable[index]-as) < nearest_diff) {
			nearest_diff = fabsf(m_fSinTable[index]-as);
			nearest_index = i;
		}
	}

	return nearest_index;
}


// 0 - 180
int CFastMath::as_acos(float ac)
{
	double	nearest_diff = 2.0;
	int		nearest_index;

	for(int i = 0; i <= 180; ++i) {
		if (fabsf(m_fCosTable[i]-ac) < nearest_diff) {
			nearest_diff = fabsf(m_fCosTable[i]-ac);
			nearest_index = i;
		}
	}

	return nearest_index;
}

inline float CFastMath::as_sin(int degree)
{
	while(degree < 0) {
		degree += 360;
	}

	degree = degree % 360;
	return m_fSinTable[degree];
}

inline float CFastMath::as_cos(int degree)
{
	while(degree < 0) {
		degree += 360;
	}

	degree = degree%360;
	return m_fCosTable[degree];
}

*/