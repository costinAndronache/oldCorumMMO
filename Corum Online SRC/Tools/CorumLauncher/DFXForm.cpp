#include "stdafx.h"
#include "DFXForm.h"

#define MAGIC_NUMBER	77

// definition of Macros and static array.
#define UUE(b) ((b+MAGIC_NUMBER))
#define UUD(b) ((b-MAGIC_NUMBER))

#define XXE(b) ((set[b]))
static char set[] =
        "+-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

inline BYTE XXD(BYTE b)
{
    if(b >= 97) return (b - 59);    // a ~ z
    if(b >= 65) return (b - 53);    // A ~ Z
    if(b >= 48) return (b - 46);    // 0 ~ 9
    if(b == 45) return 1;            // -
    return 0;                        // +
}

DFXform::DFXform(){}
DFXform::~DFXform(){}

//////////////////////////////////////////////////////////////////////
/////////////
void DFXform::UUEncode(BYTE* pSrc, UINT nSrcSize, BYTE* pDes, UINT* pDesSize)
{
    register UINT i = 0;
    
    // 패딩이 필요한 사이즈를 구한다.
    UINT nPaddingSize = (nSrcSize % 3) ? 3 - (nSrcSize % 3) : 0;

    // 몇 회의 부분 인코딩이 필요한 지 구한다.
    UINT nCount = (UINT)nSrcSize / 3;

    // 인코딩한다.
    for(i = 0; i <  nCount; i++)
    {
        UUPartialEncode(pSrc + 3*i, pDes + 4*i);
    }

    // 뒷 부분을 패딩하고 인코딩한다(0/1을 이용한다).
    UUPadAndEncode(pSrc + 3*i, pDes + 4*i, nPaddingSize);
    i++;

    // 변환된 전체 사이즈를 구한다.
    *pDesSize = 4*i;
}

void DFXform::UUPadAndEncode(BYTE* pSrc, BYTE* pDes, UINT nPaddingSize)
{
    BYTE Zero   = 0;
    BYTE One    = 1;
    register BYTE c1, c2, c3, c4;

    if(nPaddingSize == 0)    
    {
        c1 = (BYTE)(One >> 2);
        c2 = (BYTE)((One << 4) & 0x30 | (Zero >> 4) & 0x0F);
        c3 = (BYTE)((Zero << 2) & 0x3C | (Zero >> 6) & 0x03);
        c4 = (BYTE)(Zero & 0x3F);
    }
    else if(nPaddingSize == 1)
    {
        c1 = (BYTE)(pSrc[0] >> 2);
        c2 = (BYTE)((pSrc[0] << 4) & 0x30 | (pSrc[1] >> 4) & 0x0F);
        c3 = (BYTE)((pSrc[1] << 2) & 0x3C | (One >> 6) & 0x03);
        c4 = (BYTE)(One & 0x3F);
    }
    else if(nPaddingSize == 2)
    {
        c1 = (BYTE)(pSrc[0] >> 2);
        c2 = (BYTE)((pSrc[0] << 4) & 0x30 | (One >> 4) & 0x0F);
        c3 = (BYTE)((One << 2) & 0x3C | (Zero >> 6) & 0x03);
        c4 = (BYTE)(Zero & 0x3F);
    }

    pDes[0] = (BYTE)UUE(c1);
    pDes[1] = (BYTE)UUE(c2);
    pDes[2] = (BYTE)UUE(c3);
    pDes[3] = (BYTE)UUE(c4);
}

void DFXform::UUPartialEncode(BYTE* pSrc, BYTE* pDes)
{
    register BYTE c1, c2, c3, c4;

    c1 = (BYTE)(pSrc[0] >> 2);
    c2 = (BYTE)((pSrc[0] << 4) & 0x30 | (pSrc[1] >> 4) & 0x0F);
    c3 = (BYTE)((pSrc[1] << 2) & 0x3C | (pSrc[2] >> 6) & 0x03);
    c4 = (BYTE)(pSrc[2] & 0x3F);

    pDes[0] = (BYTE)UUE(c1);
    pDes[1] = (BYTE)UUE(c2);
    pDes[2] = (BYTE)UUE(c3);
    pDes[3] = (BYTE)UUE(c4);
}

void DFXform::UUDecode(BYTE* pSrc, UINT nSrcSize, BYTE* pDes, UINT* pDesSize)
{
    register UINT    i = 0;
    register UINT    nCount = 0;

    // 디코딩할 횟수를 구한다.
    nCount = (UINT)nSrcSize / 4;    

    // 디코딩한다.
    for(i = 0; i < nCount; i++)
    {
        UUPartialDecode(pSrc + 4*i, pDes + 3*i);
    }

    // 디코딩된 사이즈를 구한다.
    *pDesSize = 3*i;

    // 패딩을 제거한다.
    if(pDes[*pDesSize - 1] == 0)
        *pDesSize -= 1;
    if(pDes[*pDesSize - 1] == 0)
        *pDesSize -= 1;
    if(pDes[*pDesSize - 1] == 1)
        *pDesSize -= 1;
}

void DFXform::UUPartialDecode(BYTE* pSrc, BYTE* pDes)
{
    register BYTE c1, c2, c3;

    c1 = (BYTE)((UUD(pSrc[0]) << 2) & 0xFC | (UUD(pSrc[1]) >> 4) & 0x03);
    c2 = (BYTE)((UUD(pSrc[1]) << 4) | (UUD(pSrc[2]) >> 2) & 0x0F);
    c3 = (BYTE)((UUD(pSrc[2]) << 6) | UUD(pSrc[3]) & 0x3F);

    pDes[0] = (BYTE)c1;
    pDes[1] = (BYTE)c2;
    pDes[2] = (BYTE)c3;
}

//////////////////////////////////////////////////////////////////////
/////////////
void DFXform::XXEncode(BYTE* pSrc, UINT nSrcSize, BYTE* pDes, UINT* pDesSize)
{
    register UINT    i = 0;
    
    // 패딩이 필요한 사이즈를 구한다.
    UINT nPaddingSize = (nSrcSize % 3) ? 3 - (nSrcSize % 3) : 0;

    // 몇 회의 부분 인코딩이 필요한 지 구한다.
    UINT nCount = (UINT)nSrcSize / 3;

    // 인코딩한다.
    for(i = 0; i < nCount; i++)
    {
        XXPartialEncode(pSrc + 3*i, pDes + 4*i);
    }

    // 뒷 부분을 패딩하고 인코딩한다(0/1을 이용한다).
    XXPaddAndEncode(pSrc + 3*i, pDes + 4*i, nPaddingSize);
    i++;

    // 변환된 전체 사이즈를 구한다.
    *pDesSize = 4*i;
}

void DFXform::XXPaddAndEncode(BYTE* pSrc, BYTE* pDes, UINT nPaddingSize)
{
    BYTE Zero    = 0;
    BYTE One    = 1;
    register BYTE c1, c2, c3, c4;

    if(nPaddingSize == 0)    
    {
        c1 = (BYTE)(One >> 2);
        c2 = (BYTE)((One <<  4) & 0x30 | (Zero >> 4) & 0x0F);
        c3 = (BYTE)((Zero <<  2) & 0x3C | (Zero >> 6) & 0x03);
        c4 = (BYTE)(Zero & 0x3F);
    }
    else if(nPaddingSize == 1)
    {
        c1 = (BYTE)(pSrc[0] >> 2);
        c2 = (BYTE)((pSrc[0] <<  4) & 0x30 | (pSrc[1] >> 4) & 0x0F);
        c3 = (BYTE)((pSrc[1] <<  2) & 0x3C | (One >> 6) & 0x03);
        c4 = (BYTE)(One & 0x3F);
    }
    else if(nPaddingSize == 2)
    {
        c1 = (BYTE)(pSrc[0] >> 2);
        c2 = (BYTE)((pSrc[0] << 4) & 0x30 | (One >> 4) & 0x0F);
        c3 = (BYTE)((One << 2) & 0x3C | (Zero >> 6) & 0x03);
        c4 = (BYTE)(Zero & 0x3F);
    }

    pDes[0] = (BYTE)XXE(c1);
    pDes[1] = (BYTE)XXE(c2);
    pDes[2] = (BYTE)XXE(c3);
    pDes[3] = (BYTE)XXE(c4);
}

void DFXform::XXPartialEncode(BYTE* pSrc, BYTE* pDes)
{
    register BYTE c1, c2, c3, c4;

    c1 = (BYTE)(pSrc[0] >> 2);
    c2 = (BYTE)((pSrc[0] <<  4) & 0x30 | (pSrc[1] >> 4) & 0x0F);
    c3 = (BYTE)((pSrc[1] <<  2) & 0x3C | (pSrc[2] >> 6) & 0x03);
    c4 = (BYTE)(pSrc[2] & 0x3F);

    pDes[0] = (BYTE)XXE(c1);
    pDes[1] = (BYTE)XXE(c2);
    pDes[2] = (BYTE)XXE(c3);
    pDes[3] = (BYTE)XXE(c4);
}

void DFXform::XXDecode(BYTE* pSrc, UINT nSrcSize, BYTE* pDes, UINT* pDesSize)
{
    register UINT    i = 0;
    register UINT    nCount = 0;

    // 디코딩할 횟수를 구한다.
    nCount = (UINT)nSrcSize / 4;    

    // 디코딩한다.
    for(i = 0; i < nCount; i++)
    {
        XXPartialDecode(pSrc + 4*i, pDes + 3*i);
    }

    // 디코딩된 사이즈를 구한다.
    *pDesSize = 3*i;

    // 패딩을 제거한다(원 데이터의 마지막 바이트가 0이라면 제거될 수도 있다).
    if(pDes[*pDesSize - 1] == 0)
        *pDesSize -= 1;
    if(pDes[*pDesSize - 1] == 0)
        *pDesSize -= 1;
    if(pDes[*pDesSize - 1] == 1)
        *pDesSize -= 1;
}

void DFXform::XXPartialDecode(BYTE* pSrc, BYTE* pDes)
{
    register BYTE c1, c2, c3;

    c1 = (XXD(pSrc[0]) <<  2) & 0xFC | (XXD(pSrc[1]) >> 4) & 0x03;
    c2 = (XXD(pSrc[1]) <<  4) | (XXD(pSrc[2]) >> 2) & 0x0F;
    c3 = (XXD(pSrc[2]) <<  6) | (XXD(pSrc[3])) & 0x3F;

    pDes[0] = c1;
    pDes[1] = c2;
    pDes[2] = c3;
}

