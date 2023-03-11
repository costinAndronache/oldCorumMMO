#if !defined(AFX_ITEMUPGRADEHELPWND_H__AA4C0719_1631_40D4_9DA3_52D7DE6AE9B6__INCLUDED_)
#define AFX_ITEMUPGRADEHELPWND_H__AA4C0719_1631_40D4_9DA3_52D7DE6AE9B6__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


#include "Menu.h"


#define SPR_OBJ_NOTICE_WND1			0
#define SPR_OBJ_NOTICE_WND2			1
#define SPR_OBJ_NOTICE_WND3			2
#define SPR_OBJ_NOTICE_CLOSE1		3
#define SPR_OBJ_NOTICE_CLOSE2		4
#define SPR_OBJ_HELP_NEXT1			5
#define SPR_OBJ_HELP_NEXT2			6
#define SPR_OBJ_HELP_NEXT3			7


class CItemUpgradeHelpWnd : public CMenu  
{

public:

	CItemUpgradeHelpWnd();
	~CItemUpgradeHelpWnd();

private:
	static CItemUpgradeHelpWnd* c_pThis;

public:
	static CItemUpgradeHelpWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CItemUpgradeHelpWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	char	m_szHelpMessage[1024];
	BOOL	m_bBtn;
	BYTE	m_bypage;
	
	void	RenderTextEx	();
	void	SetButtonStatus	(DWORD dwSprID, DWORD dwStatus);
	
	// �������� ���Ǵ� �͵��� ������ �Ѵ� //
	BOOL	Init			();	
	void	RenderText		();	
	void	Remove			();
	void	SetOrder		();	
	void	SetActive		(BOOL bActive = TRUE);	
	int		CheckInterface	();	
};

#endif 
