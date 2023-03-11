//==========================================================//
// Code by Jang.								2003.05.09	//
//==========================================================//
#ifndef		__PARTYJOIN__WND__
#define		__PARTYJOIN__WND__

#include	"Menu.h"

#define SPR_OBJ_PARTYJOIN_WND			0
#define SPR_OBJ_PARTYJOIN_CLOSE1		1
#define SPR_OBJ_PARTYJOIN_CLOSE2		2
#define SPR_OBJ_PARTYJOIN_OK1			3
#define SPR_OBJ_PARTYJOIN_OK2			4
#define SPR_OBJ_PARTYJOIN_OK3			5
#define SPR_OBJ_PARTYJOIN_CANCEL1		6
#define SPR_OBJ_PARTYJOIN_CANCEL2		7
#define SPR_OBJ_PARTYJOIN_CANCEL3		8

class CPartyJoinWnd : public CMenu
{
public:
		
	char	m_szName[MAX_CHARACTER_NAME_LENGTH];
		
	char	m_szInfo[0xff];
	RECT	m_rtPos;

	DWORD	m_dwUserIndex;

	BOOL	m_bBtn[2];
	
public:

	
	// Singleton Patten //
private:
	static CPartyJoinWnd* c_pThis;

public:
	static CPartyJoinWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CPartyJoinWnd; return c_pThis; }
	static void		DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// �������� ���Ǵ� �͵��� ������ �Ѵ� //	
	BOOL	Init();
			
	void	RenderText();	
	void	Remove();
	void	SetOrder();	
	void	SetActive(BOOL bActive = TRUE);	

	int		CheckInterface();

	// üũ ���� �Լ� //	

	CPartyJoinWnd();
	~CPartyJoinWnd();
};

#endif
//==========================================================//
// End.														//
//==========================================================//