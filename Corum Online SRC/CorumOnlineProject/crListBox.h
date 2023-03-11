#pragma once

#include "Menu.h"
#include "crBaseMenu.h"
#include "crListBoxItemInterface.h"

class crButton;

struct	crListBox_Status
{
	DWORD	m_bShowColumnInfo : 1;
	DWORD	m_bEnableCheck : 1;		// ����Ʈ �������� üũ�� �����ϴ�.
	DWORD	m_bLimitCheck : 1;
	DWORD	m_bReserved : 29;

	crListBox_Status()
		: m_bShowColumnInfo(FALSE),
        m_bEnableCheck(TRUE),
		m_bLimitCheck(FALSE)
	{
	}
};

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
class crListBox : public crBaseMenu
{
public:
	///////////////////////////////////////////////////////////
	crListBox();
	virtual ~crListBox();
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	/**
	*
	* \param pParentMenu 
	* \param iX 
	* \param iY 
	* \return 
	*/
	virtual	BOOL	Render( crBaseMenu*	pParentMenu, INT	iX, INT iY, DWORD dwZOrder );

	/**
	*
	* \return 
	*/
	virtual	INT		ProcessMessgae();

	/**
	*
	* \return 
	*/
	virtual	INT		Destroy();

	/**
	 *
	 * \param iX 
	 * \param iY 
	 * \return 
	 */
	virtual	BOOL	Event( INT iX, INT iY );

	/**
	 *
	 * \param iX 
	 * \param iY 
	 * \return 
	 */
	virtual	BOOL	EventOver( INT iX, INT iY );

	/**
	*
	*/
	virtual	void	ValidateState();
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	/**
	 *
	 * \param dwPage 
	 * \return 
	 */
	virtual	BOOL	SetPage( DWORD dwPage );

	/**
	 *
	 * \return 
	 */
	virtual	DWORD	GetPage()
	{
		return m_dwVisiblePage;
	}

	virtual	DWORD	GetPageCount()
	{
		return (m_vecListItem.size() / m_dwOnceVisbleLineCount) + (m_vecListItem.size()%m_dwOnceVisbleLineCount > 0 ? 1 : 0);
	}

	/**
	 *
	 * \return 
	 */
	virtual	BOOL	NextPage();

	/**
	 *
	 * \return 
	 */
	virtual	BOOL	PrevPage();

	/**
	 *
	 * \param dwLineStep 
	 */
	__inline	void	SetLineStep( DWORD dwLineStep )
	{
		m_dwLineStep = dwLineStep;
	}

	/**
	 *
	 * \param dwVisibleLine 
	 */
	__inline	void	SetMaxVisbleLine( DWORD dwVisibleLine )
	{
		m_dwOnceVisbleLineCount = dwVisibleLine;
	}

	__inline	void	SetTextColor( DWORD dwIndex, DWORD dwColor )
	{
		assert( dwIndex < 4 );

		m_dwColor[dwIndex] = dwColor;
	}
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	virtual	BOOL	DeleteAllItems();
	virtual	BOOL	AddItem( LPCTSTR lpText, void* pData = NULL );
	virtual	BOOL	InsertItem( INT iIndex, LPCTSTR lpText, void* pData = NULL );
	virtual	BOOL	AddItemFromFormat( void* pData, LPCTSTR lpText, ... );
	virtual	BOOL	InsertItemFromFormat( INT iIndex, void* pData, LPCTSTR lpText, ... );
	///////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////
	typedef crListBoxItemInterface				ListItem;
	typedef vector< ListItem >					ListItemList;
	typedef vector< ListItem >::const_iterator	ListItemList_Const_Iter;
	typedef vector< ListItem >::iterator		ListItemList_Iter;
	///////////////////////////////////////////////////////////

	__inline	ListItem*	GetItem( DWORD dwIndex )
	{
		if( dwIndex >= GetItemSize() )
			return NULL;

		return &m_vecListItem[dwIndex];
	}
	__inline	DWORD	GetItemSize()
	{
		return m_vecListItem.size();
	}
	__inline	void	DeleteLastItem()
	{
		m_vecListItem.erase( m_iterLastSelected );
	}

	///////////////////////////////////////////////////////////
	ListItemList&	GetItemList() 
	{
		return m_vecListItem;
	}
		

	template < typename Type >
		void	ForEach()
	{
		for_each( m_vecListItem.begin(), m_vecListItem.end(), Type() );
	}
	template < typename Type >
		void	SortBy()
	{
		sort( m_vecListItem.begin(), m_vecListItem.end(), Type() );
	}
	template < typename Type >
		DWORD	CountBy()
	{
		return count_if( m_vecListItem.begin(), m_vecListItem.end(), Type() );
	}
	///////////////////////////////////////////////////////////

	__inline	ListItem&	GetLastSelected()
	{
		return (*m_iterLastSelected);
	}
private:
	///////////////////////////////////////////////////////////
	/**
	 *
	 */
	void	UpdateList();
	///////////////////////////////////////////////////////////

    ListItemList			m_vecListItem;				// �׳� ��Ʈ������ ó�Ѵ�. ���⿡ �پ����� �ٷ���, ���ø� �Ǵ� �������̽��� �ĳ����� �ȴ�.
	crListBox_Status		m_SListBoxStatus;			// ����Ʈ�ڽ��� ���¸� ��Ÿ���� ����ü ���ʵ�
	DWORD					m_dwOnceVisbleLineCount;	// �ѹ��� ������ ���� �ǰڴ�.
	DWORD					m_dwLineStep;				//	����Ʈ ������ �ϳ��� ����
	DWORD					m_dwMaxCheckedItem;			// üũ������ ���������� �Ѱ�� ���Ѱ�.

	DWORD					m_dwVisiblePage;			// ���� �������� ������

	DWORD					m_dwColor[4];				// ���� ���, ���콺��, ����, ���� ���̴�.

	ListItemList_Iter		m_iterLastSelected;			// ��������, ���õ� ����Ʈ �������̴�.
};