#pragma once

#include	"Menu.h"

#define	MAX_ITEM_UPGRADE	4
#define	ITEM_UPGRADE_SUCCESS_TYPE_AD	1	// ���ݷ� ����
#define	ITEM_UPGRADE_SUCCESS_TYPE_DP	2	// ���� ����
#define	ITEM_UPGRADE_SUCCESS_TYPE_OP	4	// �ɼ� ����
#define	ITEM_UPGRADE_SUCCESS_TYPE_SL	8	// �������� ����
#define	ITEM_UPGRADE_WAITING			127	// ���׷��̵� ��� ��ٸ�����
#define	ITEM_UPGRADE_FAIL				128	// ����
#define ITEM_UPGRADE_FAIL_INSURANCE		129	// ���� ������ ������ ���� ó�� 

#define SPR_OBJ_ITEMUPGRADEWND			0
#define SPR_OBJ_ITEMUPGRADE_HELPWND		1
#define BUTTON_OBJ_ITEMUPGRADE_CLOSE1	2
#define BUTTON_OBJ_ITEMUPGRADE_CLOSE2	3
#define BUTTON_OBJ_ITEMUPGRADE1			4
#define BUTTON_OBJ_ITEMUPGRADE2			5
#define BUTTON_OBJ_ITEMUPGRADE3			6


class CItemUpgradeWnd : public CMenu
{	
public:
		
	CItem	m_pUpgradeItem[MAX_ITEM_UPGRADE];

	char	m_szInfo[TEXT_RENDER_BUFFER];
	RECT	m_rtPos;
			
	BOOL	m_bWaitingUpgrade;	// ������ ��ٸ������ΰ�?
	char	m_szUpgradeDesc[16][TEXT_RENDER_BUFFER];
	
	BOOL	m_bBtn;
	
public:
	
private:
	static CItemUpgradeWnd* c_pThis;

public:
	static CItemUpgradeWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CItemUpgradeWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// �������� ���Ǵ� �͵��� ������ �Ѵ� //
	BOOL	Init();
		
	void	RenderText();	
	void	Remove();
	void	SetOrder();	
	void	SetActive(BOOL bActive = TRUE);
	
	int		CheckInterface();

	// üũ ���� �Լ� //	
	void	RenderItem();

	void	ItemInfoRender();	
	
public:

	BOOL	IsAbleUpgradeAriesZodiac(CItem* pItem);

	CItemUpgradeWnd();
	~CItemUpgradeWnd();

private:
	

	void	OnMove();
	void	SetButtonStatus(DWORD dwSprID, DWORD dwStatus);// dwStatus = 0(nomal) : 1(mouseover) : 2(mousedown)	
};