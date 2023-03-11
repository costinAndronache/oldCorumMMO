#ifndef _____AAAAAA_____H
#define _____AAAAAA_____H


class DFXform
{
public:
	
	DFXform();
	~DFXform();

	void DFXform::UUEncode(BYTE* pSrc, UINT nSrcSize, BYTE* pDes, UINT* pDesSize);
	void DFXform::UUDecode(BYTE* pSrc, UINT nSrcSize, BYTE* pDes, UINT* pDesSize);

	void DFXform::XXEncode(BYTE* pSrc, UINT nSrcSize, BYTE* pDes, UINT* pDesSize);
	void DFXform::XXDecode(BYTE* pSrc, UINT nSrcSize, BYTE* pDes, UINT* pDesSize);

private:
	void DFXform::UUPadAndEncode(BYTE* pSrc, BYTE* pDes, UINT nPaddingSize);
	void DFXform::UUPartialEncode(BYTE* pSrc, BYTE* pDes);
	void DFXform::UUPartialDecode(BYTE* pSrc, BYTE* pDes);
	
	void DFXform::XXPaddAndEncode(BYTE* pSrc, BYTE* pDes, UINT nPaddingSize);
	void DFXform::XXPartialEncode(BYTE* pSrc, BYTE* pDes);
	void DFXform::XXPartialDecode(BYTE* pSrc, BYTE* pDes);
};

#endif // _____AAAAAA_____H