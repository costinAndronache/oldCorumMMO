// GuildWarRequest.h: interface for the CGuildWarRequest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDWARREQUEST_H__E0B2951B_0AAC_4575_B1B0_554594F41AF0__INCLUDED_)
#define AFX_GUILDWARREQUEST_H__E0B2951B_0AAC_4575_B1B0_554594F41AF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Menu.h"

#define SPR_OBJ_GUILDWAR_REQUEST_WINDOWS1			0
#define SPR_OBJ_GUILDWAR_REQUEST_WINDOWS2			1
#define SPR_OBJ_GUILDWAR_REQUEST_WINDOWS3			2
#define SPR_OBJ_GUILDWAR_REQUEST_CLOSE1				3
#define SPR_OBJ_GUILDWAR_REQUEST_CLOSE2				4
#define BUTTON_OBJ_GUILDWAR_REQUEST_REQUEST1		5
#define BUTTON_OBJ_GUILDWAR_REQUEST_REQUEST2		6
#define BUTTON_OBJ_GUILDWAR_REQUEST_REQUEST3		7
#define BUTTON_OBJ_GUILDWAR_REQUEST_REREQUEST1		8
#define BUTTON_OBJ_GUILDWAR_REQUEST_REREQUEST2		9
#define BUTTON_OBJ_GUILDWAR_REQUEST_REREQUEST3		10
#define BUTTON_OBJ_GUILDWAR_REQUEST_CANCEL1			11
#define BUTTON_OBJ_GUILDWAR_REQUEST_CANCEL2			12
#define BUTTON_OBJ_GUILDWAR_REQUEST_CANCEL3			13
#define DROPDOWN_OBJ_FORM_LEFT						14
#define DROPDOWN_OBJ_FORM_MIDDLE					15
#define DROPDOWN_OBJ_FORM_RIGHT1					16
#define DROPDOWN_OBJ_FORM_RIGHT2					17
#define DROPDOWN_OBJ_FORM_RIGHT3					18
#define DROPDOWN_OBJ_FORM_BOARD						19
#define DROPDOWN_OBJ_TIME_LEFT						20
#define DROPDOWN_OBJ_TIME_MIDDLE					21
#define DROPDOWN_OBJ_TIME_RIGHT1					22
#define DROPDOWN_OBJ_TIME_RIGHT2					23
#define DROPDOWN_OBJ_TIME_RIGHT3					24
#define DROPDOWN_OBJ_TIME_BOARD						25
#define DROPDOWN_OBJ_STAFF_LEFT						26
#define DROPDOWN_OBJ_STAFF_MIDDLE					27
#define DROPDOWN_OBJ_STAFF_RIGHT1					28
#define DROPDOWN_OBJ_STAFF_RIGHT2					29
#define DROPDOWN_OBJ_STAFF_RIGHT3					30
#define DROPDOWN_OBJ_STAFF_BOARD					31
#define BUTTON_OBJ_GUILDWAR_REQUEST_APPLY1			32
#define BUTTON_OBJ_GUILDWAR_REQUEST_APPLY2			33
#define BUTTON_OBJ_GUILDWAR_REQUEST_APPLY3			34
#define BUTTON_OBJ_GUILDWAR_REQUEST_REJECT1			35
#define BUTTON_OBJ_GUILDWAR_REQUEST_REJECT2			36
#define BUTTON_OBJ_GUILDWAR_REQUEST_REJECT3			37

// CMenu::m_rt ���� �����ؼ� ���߿� �߰��ɶ� �ٽ� �з��� �����ؾ� �ȴ�.(DB : InterfaceComponent table, ID:69��)
#define GWR_DROPDOWN_SELECT_FORM_START				10
#define GWR_DROPDOWN_SELECT_TIME_START				20
#define GWR_DROPDOWN_SELECT_STAFF_START				30

enum GUILDWAR_REQUEST_DROP_DOWN_STATUS//��� ��û �������̽����� ��Ӵٿ� �ڽ��� ���¸� ��Ÿ��(m_byDropDownStatus)
{
	GRDDS_NONE	= 0,
	GRDDS_FORM	= 1,
	GRDDS_TIME	= 2,
	GRDDS_STAFF	= 3,
};

//interface.h����			GUILDWAR_REQUEST	=		68
class CGuildWarRequest : public CMenu
{
public:
	BYTE				m_byDropDownStatus;//��� ��û �������̽����� ��Ӵٿ� �ڽ��� ���¸� ��Ÿ��(�ʱⰪ:0)
	char				m_szRequestName[MAX_CHARACTER_NAME_LENGTH];//��û�� �̸�
	int					m_nGameForm;//����� ������ ��� ��(�ʱⰪ 0)
	int					m_nGameTime;//���ð� ������ ��� ��(�ʱⰪ 0)
	int					m_nGameStaff;//����ο� ������ ��� ��(�ʱⰪ 0)
	BOOL				m_bReRequestEnable;//������û ��ư�� Ȱ��ȭ �� �ֳ�?
	GUILD_LADDER_DATA	m_RankingData;//��û ����� ��ŷ ����Ÿ.

	char m_szForm[2][TEXT_RENDER_BUFFER];
	char m_szTime[4][TEXT_RENDER_BUFFER];
	char m_szStaff[5][TEXT_RENDER_BUFFER];

	BYTE m_byBtnRequest;//��ư ��û ����(�ʱⰪ:0)
	BYTE m_byBtnReRequest;//��ư ������û ����(�ʱⰪ:0)
	BYTE m_byBtnCancel;//��ư ��� ����(�ʱⰪ:0)

	DWORD		m_dwTargetGuildId;//����� ��� ���̵�.

private:
	static CGuildWarRequest* c_pThis;

public:
	static CGuildWarRequest*	GetInstance()		{ if(!c_pThis) c_pThis = new CGuildWarRequest; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }

	BOOL	Init();
			
	void	RenderText();	
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);
	int		CheckInterface();
	void	SetRenderStatus();
	void	SetRankData(GUILD_LADDER_DATA LadderData);

public:
	CGuildWarRequest();
	virtual ~CGuildWarRequest();
};

#endif // !defined(AFX_GUILDWARREQUEST_H__E0B2951B_0AAC_4575_B1B0_554594F41AF0__INCLUDED_)
