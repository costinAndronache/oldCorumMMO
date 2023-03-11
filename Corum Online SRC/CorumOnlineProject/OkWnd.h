//======================================================//
// Code by Jang.							2003.04.03	//
// Object : Ok Wnd.										//
//======================================================//
#ifndef		__OKWND_INC__
#define		__OKWND_INC__
//==============================================
//	0 bit ==>	��Ż Ż���÷���
//	1 bit ==>	���� ��ũ��
//	2 bit ==>	���� ��Ż
//	3 bit ==>	��Ż ��ǥ ���̺�
//==============================================

#define	__OKWND_TYPE_POTAL_VILLAGE_USE	1		// ���� ��ũ�� ����ϱ� ���ؼ�.
#define	__OKWND_TYPE_POTAL_DUNGEON_USE	2		// ���� ��Ż ����ϱ� ���ؼ�.
#define	__OKWND_TYPE_POTAL_SAVE			4		// ��Ż ���̺긦 ���ؼ�.
#define	__OKWND_TYPE_POTAL_ENTRANCE		8		// ����� ����� ���ؼ�.
#define __OKWND_TYPE_UNPACK_PRESENT_BOX	16		// ��������(�±ر�) ������ Ǫ�°��� ����� ���ؼ� 
#define	__OKWND_TYPE_DUNGEON_ENTER		32		// ���� �����ҷ�?

#define SPR_OBJ_OK_CREATE_WND		0
#define SPR_OBJ_OK_CLOSE1			1
#define SPR_OBJ_OK_CLOSE2			2
#define SPR_OBJ_OK_OK1				3
#define SPR_OBJ_OK_OK2				4
#define SPR_OBJ_OK_OK3				5
#define SPR_OBJ_OK_CANCEL1			6
#define SPR_OBJ_OK_CANCEL2			7
#define SPR_OBJ_OK_CANCEL3			8

#include	"Menu.h"
class DUNGEON_DATA_EX;
class COkWnd : public CMenu
{
public:	
	
	BOOL	m_bBtn[2];

	char	m_szInfo[0xff];
	RECT	m_rtPos;
	
	BYTE	m_bZipCode;	
	BYTE	m_bType;	// BIT ü�踦 ����.... ���� �ִ°� ���� ��.��
	BYTE	m_byRenderType;
	DWORD	m_dwDungeonID;

public:
	
	// Singleton Patten //
private:
	static COkWnd* c_pThis;

public:
	static COkWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new COkWnd; return c_pThis; }
	static void		DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// �������� ���Ǵ� �͵��� ������ �Ѵ� //
	BOOL	Init();


	void	RenderText();	
	void	Remove();
	void	SetOrder();	
	void	SetActive(BOOL bActive = TRUE);
	
	int		CheckInterface();

	// üũ ���� �Լ� //

	COkWnd();
	virtual ~COkWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//