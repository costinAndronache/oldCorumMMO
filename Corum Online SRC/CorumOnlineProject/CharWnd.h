//======================================================//
// Code by Jang.							2002.12.23	//
// Obejct : Char Window.								//
//======================================================//
#ifndef		__CHAR_WINDOW_INC__
#define		__CHAR_WINDOW_INC__

#include	"Menu.h"
#include	"../CommonServer/CommonClientDungeon.h"
#include	"CMainUserUpdateInterested.h"

#define SPR_OBJ_CHAR_WINDOWS1		0
#define SPR_OBJ_CHAR_WINDOWS2		1
#define SPR_OBJ_CHAR_CLOSE1			2
#define SPR_OBJ_CHAR_CLOSE2			3
#define BUTTON_OBJ_EGO_UP1			4
#define BUTTON_OBJ_EGO_UP2			5
#define BUTTON_OBJ_STR_UP1			6
#define BUTTON_OBJ_STR_UP2			7
#define BUTTON_OBJ_INT_UP1			8
#define BUTTON_OBJ_INT_UP2			9
#define BUTTON_OBJ_DEX_UP1			10
#define BUTTON_OBJ_DEX_UP2			11
#define BUTTON_OBJ_VIT_UP1			12
#define BUTTON_OBJ_VIT_UP2			13
#define SPR_OBJ_CHAR_TAB			14
#define SPR_OBJ_GUARDIAN_LIFE		15

// minjin. Now Modifying...... 
/*#######################################################
	Character Information Window Column Precisions
#######################################################*/
#define CHARINFOTEXT_CHAR_WIDTH					7		// ���� ����
#define CHARINFOTEXT_TOP_XPOS_RIGHT				306		// ����, ����, ���, �⺻���� (HP, SP, LEVEL, EXP)
#define CHARINFOTEXT_BASIC_STAT_XPOS_LEFT		94		// EGO, STR, INT, DEX, VIT
#define CHARINFOTEXT_BASIC_STAT_XPOS_RIGHT		318		// EGO, STR, INT, DEX, VIT
#define CHARINFOTEXT_RESIST_STAT_XPOS_LEFT		248		// ���׷� ���� (Physical, Flame, Ice, Electric, Poison)
#define CHARINFOTEXT_RESIST_STAT_XPOS_RIGHT		306		// ���׷� ���� (Physical, Flame, Ice, Electric, Poison)
#define THIS_TEXT_RIGHT
#define THIS_TEXT_LEFT(_LEFT_, _SIZE_)	_LEFT_ - _SIZE_ * CHARINFOTEXT_CHAR_WIDTH
enum ENUM_TAB_TYPE
{
	ENUM_TAB_TYPE_CHARACTERINFO = 1,
	ENUM_TAB_TYPE_GUARDIANINFO = 2,
};

class CMonster;
struct CTDS_CHAR_LEVELUP;

class CCharWnd : public CMenu, public CMainUserUpdateInterested {
public:
	void updatedStatPoints(CMainUser*, DWORD oldValue, DWORD newValue) override;
public:
		
	BYTE	m_byMemKey;
			
	char	m_szInfo[TEXT_RENDER_BUFFER];
	RECT	m_rtPos;		

	BOOL	m_bBtn[5];

public:
	
private:
	static CCharWnd* c_pThis;
	static std::shared_ptr<CCharWnd> _shared;
	ENUM_TAB_TYPE m_enCurrentTabType;

	void	GuardianDisplay();
	void	CharacterDisplay();
	void	Send_CharacterStatPoint(STATUS_POINT_KIND enStatusPointKind);
	void	Send_GuardianStatPoint(STATUS_POINT_KIND enStatusPointKind);
	void	SendStatPoint(GAME_OBJECT_TYPE enObjectType, STATUS_POINT_KIND enStatusPointKind);
	void	ShowAllStatIncreseButton(BOOL bShow);
	void	OnStatButtonEvent(STATUS_POINT_KIND enStatusPointKind, MOUSECHECK enMouseCheck);
	void	SpriteStatButtonEvent(STATUS_POINT_KIND enStatusPointKind, MOUSECHECK enMouseCheck);
	BOOL	IsStatPoint();
public:
	static std::shared_ptr<CCharWnd> getShared();
	static CCharWnd* GetInstance() { return getShared().get(); }
	static void			DestroyInstance() { _shared.reset(); }

	void	(CCharWnd::*m_fnDisplay)();
	void	(CCharWnd::*m_fnSendStatPoint)(STATUS_POINT_KIND enStatusPointKind);
	BOOL	Init();
			
	void	RenderText();	
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);
	void	SetTab(ENUM_TAB_TYPE eTabType);
	int		CheckInterface();	
	void	HideAllStatButton();
	CMonster*	GetMyGuardian();
	
	CCharWnd();
	~CCharWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//