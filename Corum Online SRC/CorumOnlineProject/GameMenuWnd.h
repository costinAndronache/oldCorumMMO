//======================================================//
// Code by Jang.							2002.12.23	//
// Obejct : Char Window.								//
//======================================================//
#ifndef		__GAMEMENU_WINDOW_INC__
#define		__GAMEMENU_WINDOW_INC__

#include	"Menu.h"

#define SPR_OBJ_OPTIONWND				0
#define	SPR_OBJ_OPTION_SHADOW			1
#define SPR_OBJ_OPTION_BGM				2
#define SPR_OBJ_OPTION_TEXT				3
#define SPR_OBJ_OPTION_EFFECT			4
#define SPR_OBJ_OPTION_EFCHK			5
#define SPR_OBJ_SYSTEM_CLOSE1			6
#define SPR_OBJ_SYSTEM_CLOSE2			7
#define BUTTON_OBJ_EXIT1				8
#define BUTTON_OBJ_EXIT2				9
#define BUTTON_OBJ_EXIT3				10
#define SPR_OBJ_OPTION_BGMBOL			11
#define SPR_OBJ_OPTION_EFFECTBOL		12
#define SPR_OBJ_OPTION_SYSTEMMSG		13
#define SPR_OBJ_OPTION_TEXTSPEED		14
#define SPR_OBJ_OPTION_TEXTLINE			15
#define SPR_OBJ_OPTION_NOTICE			16
#define SPR_OBJ_OPTION_EMOTICON			17
#define SPR_OBJ_OPTION_WHISPER			18
#define SPR_OBJ_OPTION_GUILD			19
#define SPR_OBJ_OPTION_PARTY			20
#define SPR_OBJ_OPTION_FRIEND			21

class CGameMenuWnd : public CMenu
{
public:

	BOOL	m_bMoveChk;	

	BYTE	m_byEffectIndex;			// Effect Index.

	BOOL	m_bBtn;		
	BOOL	m_bExitDown;		

	BOOL	m_bTextFlag;				// �ɼ� �÷��� ����.
	BOOL	m_bBGMFlag;					// �ɼ� �÷��� ����.	
	BOOL	m_bShadowFlag;				// �ɼ� �÷��� ����.
	BOOL	m_bEffectFlag;				// �ɼ� �÷��� ����.
	BOOL	m_bBGMRepeat;				// �ɼ� �÷��� ����.
	BOOL	m_bSystemMsgFlag;			// �ɼ� �÷��� ����.
	// sung-han 2005-03-30------------------------------------------------------------------------------------
	BYTE	m_byChatRemainTime;			// �ɼ� �÷���, ���ä�� ǥ�� ������� �ð�(1:5��, 2:10��, 3:15��) 
	BOOL	m_bChatKindParty;			// �ɼ� �÷���, ���ä��â�� ��Ƽä�� ǥ�ÿ���
	BOOL	m_bChatKindGuild;			// �ɼ� �÷���, ���ä��â�� ���ä�� ǥ�ÿ���
	BOOL	m_bChatKindWhisper;			// �ɼ� �÷���, ���ä��â�� �Ӹ�ä�� ǥ�ÿ���
	BOOL	m_bChatKindFriend;			// �ɼ� �÷���, ���ä��â�� ģ��ä�� ǥ�ÿ���
	BOOL	m_bChatEmoticon;			// �ɼ� �÷���, ä�ýÿ� �ڵ� �̸�Ƽ���� ���� ����
	BYTE	m_byKindNotich;				// �ɼ� �÷���, ���� �޼��� ǥ�ù��
	// --------------------------------------------------------------------------------------------------------
	DWORD	m_dwBGMId;					// BG ���̵�.
	char	m_szLast_BGM_Name[255];		// �ֱ�(���� ������) ���� BackGround Name // add seven 040408

public:

	// Singleton Patten //
private:
	static CGameMenuWnd* c_pThis;

public:
	static CGameMenuWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CGameMenuWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// �������� ���Ǵ� �͵��� ������ �Ѵ� //
	BOOL	Init();

	void	RenderText();	
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);
	void	OpenWnd();
	
	int		CheckInterface();

	// üũ ���� �Լ� //		
	void	SetEffect();

	CGameMenuWnd();
	~CGameMenuWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//