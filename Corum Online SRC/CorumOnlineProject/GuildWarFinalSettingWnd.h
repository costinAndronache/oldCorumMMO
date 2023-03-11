#pragma once

#include "crProxyMenuToDialog.h"
#include "crStaticText.h"
#include "crButton.h"
#include "crListBox.h"
#include "./../CommonServer/WorldPacket.h"
#include "crImage.h"

/**
 * \ingroup CorumOnlineProject
 *
 *
 * \par requirements
 * win98 or later\n
 * win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 2005-02-02
 *
 * \author ������
 *
 * \par license
 * This code is absolutely free to use and modify. The code is provided "as is" with
 * no expressed or implied warranty. The author accepts no liability if it causes
 * any damage to your computer, causes your pet to fall ill, increases baldness
 * or makes your car start emitting strange noises when you start it up.
 * This code has no bugs, just undocumented features!
 * 
 * \todo 
 *
 * \bug 
 *
 */
class CGuildWarFinalSettingWnd : public crProxyMenuToDialog
{
public:
	/**
	 *
	 */
	virtual void	RenderText();

	//////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *
	 * \return 
	 */
	virtual BOOL	Init();

	CGuildWarFinalSettingWnd();
	virtual ~CGuildWarFinalSettingWnd();
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *
	 * \param pPacket 
	 */
	void	ProcessPacket( GUILD_MATCHBEGIN_SETTING * pPacket );
	//////////////////////////////////////////////////////////////////////////////////////////

	void	SetActive(BOOL bActive );

	//////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *
	 * \return 
	 */
	static CGuildWarFinalSettingWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CGuildWarFinalSettingWnd; return c_pThis; }
	/**
	 *
	 */
	static void					DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }
	static CGuildWarFinalSettingWnd* c_pThis;
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *
	 */
//	virtual void	Remove();
	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////
	enum	ExplainText_Type
	{
		ETT_DEFAULT = 0,
        ETT_LEVELSORT,
		ETT_REFRESH,
		ETT_CALL,
		ETT_KICK,
		ETT_SELECT_KICKEE,
		ETT_READY,
		ETT_CANCEL,
		ETT_INVITATION,
		ETT_MAX
	};
	/**
	 *
	 */
	void	SetExplainText( ExplainText_Type eType );
	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////
	typedef	GUILD_MATCHBEGIN_MEMBER				MEMBERLIST_VALUE_TYPE;
	typedef list< MEMBERLIST_VALUE_TYPE >		MEMBERLIST;
	typedef	MEMBERLIST::const_iterator			MEMBERLIST_CONST_ITER;
	typedef	MEMBERLIST::iterator				MEMBERLIST_ITER;
	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////
	__inline	crListBox&	GetListBox()
	{
		return m_listGuildMember;
	}
	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////
    /**
     *
     */
    void	ListSortByLevel();
	/**
	 *
	 */
	void	Refresh();
	void	Call();
	void	Kick();
	void	ReadyToggle();
	void	ListClick();
	void	Cancel();
	void	CancelYes();
	void	CancelNo();
	void	Invitation();

	void	UpdateListCount();
	
	void	UpdateList( const crListBoxItemInterface& rhs );
	void	UpdateList_SelectedCheck( crListBoxItemInterface& rhs );
	////////////////////////////////////////////////////////////////////////////////////

	__inline	crStaticText&	GetPageCount()
	{
		return m_textListGuildMemberCurPage;
	}

	void	SetTime( DWORD dwBegin, DWORD dwEnd );
private:
	crDialog		m_DlgOkCancel;
	crStaticText	m_textOkCancel;
	crButton		m_btnOkCancel_Yes;
	crButton		m_btnOkCancel_No;

	// ����â, ���Ͻÿ� ���ؼ� ����ȴ�.
	crDialog		m_DlgMain;
		
	crStaticText	m_textGuildName[2];		//	����̸� 
	crStaticText	m_textLadderPoint[2];	//	��������
	crStaticText	m_textAverageLevel[2];	//	��շ���
	crStaticText	m_textMemberCount[2];	//	�ο���

	crStaticText	m_textLadder;
	crStaticText	m_textAverage;
	crStaticText	m_textMember;
	crStaticText	m_textMark[4];			//	: ǥ��

	crStaticText	m_textExplain[2];		// �����ൿ ���� �ؽ�Ʈ
	crStaticText	m_textNumber[20];		// �̸���, ��ȣ ǥ��, Align Right
	crStaticText	m_textClock;			// ���� �ð� ǥ��.

	crButton		m_btnLevelSort;			// �������� ��ư
	crButton		m_btnRefresh;			// ���� ��ư
	crButton		m_btnCall;				// �����ȯ ��ư
	crButton		m_btnInvitation;		// �ʴ� ��ư
	crButton		m_btnKick;				// ���� ��ư
    crButton		m_btnCancel;			// ��� ��� ��ư
//	crButton		m_btnReady;				// �غ� ��ư

	crButton		m_btnPrev;				// ����Ʈ�ڽ� ������ư
	crButton		m_btnNext;				// ����Ʈ�ڽ� ������ư

	crListBox		m_listGuildMember;	
	crImage			m_imgListGuildMemberPage;
	crStaticText	m_textListGuildMemberCurPage;

	////////////////////////////////////////////////////////////////////////////////////////////////
	DWORD			m_dwLadderGrade;							// ���� ���
	DWORD			m_dwLadderPoint;							// ���� ����
	DWORD			m_dwSelectedAndCalledMemberAverageLevel;	// �����ο��� ȣ���ο��� ���� ��հ�.	
	DWORD			m_dwMemberAverageLevel;						// ����ο��� ���� ��հ�
	DWORD			m_dwSelectedAndCalledMemberCount;			// ���� ���õ� �ο��� ȣ��� �ο��� ��	

	DWORD			m_dwEnemyLadderGrade;						// ���� ���
	DWORD			m_dwEnemyLadderPoint;						// ���� ����
	DWORD			m_dwEnemyWaitMemberAverageLevel;			// ����� ��ü����� ��շ���
	DWORD			m_dwEnemyWaitMemberCount;					// ����� ��ü����� ��
	
	DWORD			m_dwMaxMemberCount;							// ����� ���� �ִ� �ο�	
	////////////////////////////////////////////////////////////////////////////////////////////////
	
	MEMBERLIST		m_MemberList;
	MEMBERLIST		m_MemberList_TempRep;
	MEMBERLIST		m_MemberList_TempRep_Selected;

	BOOL			m_bReadyFlag;								// ���� ����� �������� ��Ÿ��.
	BOOL			m_bKickFlag;								// ���� Kick�� �������� ��Ÿ��.

	DWORD			m_dwStartingSec;
	DWORD			m_dwMin;
	DWORD			m_dwSec;
	DWORD			m_dwRemainBeginTime;
	DWORD			m_dwRemainEndTime;
	DWORD			m_dwRemainBeginTimeRecvTime;
	DWORD			m_dwRemainEndTimeRecvTime;
};