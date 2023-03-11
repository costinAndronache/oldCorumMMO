// ComboBox.h: interface for the CComboBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMBOBOX_H__5B6692F1_D30C_444E_ABCE_226AC4106BD4__INCLUDED_)
#define AFX_COMBOBOX_H__5B6692F1_D30C_444E_ABCE_226AC4106BD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "voidlist.h"
struct COMBOBOX
{
	char szComboName[30];	// �޺� ����Ʈ�� ��Ÿ�� �̸�.
	void* pDesc;			// ����Ÿ ������.
	DWORD dwIndex;			// �޺��ڽ��� ��ϵ� ����Ʈ �ε���. 
	
};
class CComboList : public CVoidList
{
	void FreeData(void* pData)
	{
		if (pData)
			delete (COMBOBOX*)pData;
		pData = NULL;
	}
};
struct V2_SPRITE;
interface IDIFontObject;
class CComboBox  
{
	CComboList*			m_pComboList;				// �޺��ڽ��� ����� �Ǿ��մ� ����Ʈ
	POSITION_			m_pComboDisplayFirst;		// ���÷��̵� ù��° ����Ʈ.
	DWORD				m_dwComboListMaxDisplay;	// ���÷��̵� �ִ� ��ϰ���.
	POSITION_			m_pComboCurSel;				// ���� ���õ� ���
	DWORD				m_dwScrollRecoverY;				// ��ũ�� ��ư y��ǥ. â �����϶� �ʿ��ϴ�.
	V2_SPRITE*			m_pSprParent;
	V2_SPRITE*			m_pSprSelectBar;			
	V2_SPRITE*			m_pSprBoard;
	V2_SPRITE*			m_pSprScrollButton;
	V2_SPRITE*			m_pSprButton[2];

	BOOL				m_bScrollClick;
	BOOL				m_bUpChk;
public:
	void RemoveSprite();							// �޺��ڽ��� �ʿ��ߴ� ��������Ʈ�� ������.
	void SetActive(BOOL bActive);					// Ȱ��ȭ ó��.
	void OnMove();									// �θ� �����찡 ���������� ó��.
	int MouseEventCheck();							// ������ ���� ���콺���¸� üũ�ϰ�, �޺�������� �浹�ϸ� ���ϰ��� 1
	void RenderList(IDIFontObject* pFont, int nOrder); // �޺��ڽ� ����� ���÷����϶�.
	char* GetSelectedString();						// ���� ���õǾ��� �޺��ڽ������ ��Ʈ�� ���� ���Ͷ�.
	void SetOrder();								// ��������Ʈ �ѷ������� ������ �����Ѵ�.
	
	V2_SPRITE* CreateSpriteComboButtonDown(DWORD dwSprID, float fRelativeX, float fRelativeY, int nStartX, int nStartY, int nWidth, int nHeight, bool bShow, int nOrder, BYTE bAlpha = 0xff); // �޺��ڽ� ��ư
	V2_SPRITE* CreateSpriteComboSelectBar(DWORD dwSprID, float fRelativeX, float fRelativeY, int nStartX, int nStartY, int nWidth, int nHeight, bool bShow, int nOrder, BYTE bAlpha = 0xff);	// �޺��ڽ� ��ϼ���Ʈ��
	V2_SPRITE* CreateSpriteComboBoard(DWORD dwSprID, float fRelativeX, float fRelativeY, int nStartX, int nStartY, int nWidth, int nHeight, bool bShow, int nOrder, BYTE bAlpha = 0xff);	// �޺��ڽ� ��Ϲ�
	V2_SPRITE* CreateSpriteComboScrollButton(DWORD dwSprID, float fRelativeX, float fRelativeY, int nStartX, int nStartY, int nWidth, int nHeight, bool bShow, int nOrder, BYTE bAlpha = 0xff);	// �޺��ڽ� ��ũ�� ��ư
	V2_SPRITE* CreateSpriteComboButtonNormal(DWORD dwSprID, float fRelativeX, float fRelativeY, int nStartX, int nStartY, int nWidth, int nHeight, bool bShow, int nOrder, BYTE bAlpha = 0xff); // �޺��ڽ� ��ư
	void Initialize(V2_SPRITE* pSprParent, DWORD dwMaxDisplay);	// pSprParent �θ� �� â������ ������, dwMaxDisplay �޺��ڽ��� �������� ȭ�鿡 ��Ÿ���� �ִ� �ִ�ũ��.
	int GetCount();		// ����Ʈ�� ��ϵǾ� �ִ� ����.
	void AddTail(char* szName, void* pDesc = NULL);	// �޺��ڽ��� �߰�
	void RemoveHead();
	CComboBox();
	~CComboBox();
private:
	void OnScroll();								// ��ũ�� ó��.
};

#endif // !defined(AFX_COMBOBOX_H__5B6692F1_D30C_444E_ABCE_226AC4106BD4__INCLUDED_)
