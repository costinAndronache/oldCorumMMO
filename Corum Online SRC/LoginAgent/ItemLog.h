#ifndef __ITEMLOG_H__
#define __ITEMLOG_H__

class	CItem;
/*
#define		ITEM_LOG_PICKUP					1		// �������� �ݴ�.
#define		ITEM_LOG_DROP					2		// �������� ������.
#define		ITEM_LOG_USED					3		// ������ ��� 
#define		ITEM_LOG_EXCHANGE				4		// �������� ��ȯ���� �ݴ�.
#define		ITEM_LOG_SUM					5		// �������� ���ƴ�.
#define		ITEM_LOG_SUB					6		// �������� ��������.
#define		ITEM_LOG_DELETE_BY_SUM			7		// ������ ��ġ�⿡ ���� ���� �ƴ�.
#define		ITEM_LOG_CREATE_BY_SUB			8		// ������ �����⿡ ���� �����ƴ�.
#define		ITEM_LOG_CREATE_BY_SHOP			9		// NPC���� �����ߴ�.
#define		ITEM_LOG_DELETE_BY_SHOP			10		// NPC���� �ȴ�.
#define		ITEM_LOG_UPGRADE				11		// ������ ���׷��̵�
#define		ITEM_LOG_PLAYERSHOP				12		
#define		ITEM_LOG_TRADE					13
#define		ITEM_LOG_UPGRADE_FAIL			14		// ������ ���׷��̵� ���� 
#define		ITEM_LOG_UPGRADE_FAIL_INSURANCE 15		// ������ ���׷���Ʈ ���� ������ ���� ó��
#define		ITEM_LOG_KARZ_DROP				16	 
#define		ITEM_LOG_KARZ_PICKUP			17
#define		ITEM_LOG_USER_INSURANCE			18
*/
#define		ITEM_LOG_ITEM_MALL				19		// �����۸����� ����

BOOL	SetItemLog( CItem* pItem, BYTE bLogCode, WORD wDungeonID, char* szOwner, char* szSeller );

#endif // __ITEMLOG_H__