//==========================================================//
// Code by Jang.								2003.05.09	//
//==========================================================//
#ifndef		__GUILDDELETE__WND__
#define		__GUILDDELETE__WND__

#include	"Menu.h"

#define	SPR_OBJ_GUILDDELETE_WND			0
#define	SPR_OBJ_GUILDDELETE_CLOSE1		1
#define	SPR_OBJ_GUILDDELETE_CLOSE2		2
#define	SPR_OBJ_GUILDDELETE_OK1			3
#define	SPR_OBJ_GUILDDELETE_OK2			4
#define	SPR_OBJ_GUILDDELETE_OK3			5
#define	SPR_OBJ_GUILDDELETE_CANCEL1		6
#define	SPR_OBJ_GUILDDELETE_CANCEL2		7
#define	SPR_OBJ_GUILDDELETE_CANCEL3		8

class CGuildDeleteWnd : public CMenu
{
public:
			
	char	m_szInfo[0xff];
	RECT	m_rtPos;

	DWORD	m_dwUserIndex;	

	char	m_szName[MAX_CHARACTER_NAME_LENGTH];

	BOOL	m_bChk;
	BYTE	m_byType;

	BOOL	m_bBtn[2];
	
public:
	
	// Singleton Patten //
private:
	static CGuildDeleteWnd* c_pThis;

public:
	static CGuildDeleteWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CGuildDeleteWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// �������� ���Ǵ� �͵��� ������ �Ѵ� //	
	BOOL	Init();
			
	void	RenderText();	
	void	Remove();
	void	SetOrder();	
	void	SetActive(BOOL bActive = TRUE);	

	int		CheckInterface();

	// üũ ���� �Լ� //		

	CGuildDeleteWnd();
	~CGuildDeleteWnd();
};

#endif
//==========================================================//
// End.														//
//==========================================================//
