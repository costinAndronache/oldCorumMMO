#if !defined(AFX_DUNGEON_DATA_H__184463A9_2436_4FEC_849B_A2604FF03BA6__INCLUDED_)
#define AFX_DUNGEON_DATA_H__184463A9_2436_4FEC_849B_A2604FF03BA6__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


#include "../CommonServer/CommonHeader.h"
#include "GlobalDefine.h"


#define TIME_SEC		1
#define	TIME_MIN		2
#define	TIME_HOUR		3


#pragma pack(push, 1)
struct DUNGEON_DATA
{	
	DWORD	m_dwID;
	WORD	m_wGroupID;
	char	m_szDungeonName[ MAX_DUNGEON_NAME_LENGTH ];
	WORD	m_wAreaNum;
	WORD	m_wAreaLevel_h;								// 0 = ���ʺ���, 1 = �ʺ���, 2 = �߼���, 3 = �����, 4 = �����������
	WORD	m_wAreaLevel_l;
	WORD	m_wInitLevel;								// ���� �⺻ ����
	WORD	m_wUpgradeLevel;							// ���� ���׷��̵�� ����
	DWORD	m_wBattleTimeLimit;							// ������ ���� �ð�.
	WORD	m_wProduction;								// ����ǰ ���̵�.
	BYTE	m_byProductionCount;						// ����ǰ ����.		
	BYTE	m_bSiege;									// ������ : 1
	BYTE	m_bOperationType;							// ���� � ����..0 = ��ָ��, 1 = �����, 2 = ������, 3 = ���������
	WORD	m_wLayerCount;								// ������ ���̾�  ����.
	DWORD	m_dwOwnerIndex;								// ���� ���� �ε���
	char	m_szOwner[ MAX_CHARACTER_NAME_LENGTH ];		// ������ �Ǵ� �������� 	
	char    m_szManager[ MAX_CHARACTER_NAME_LENGTH ];	// ������ ĳ���͸�..  �������ΰ�� ������� �渶
	DWORD	m_dwOwnerGuildNum;
	DWORD	m_dwEntrance;								// �����
	DWORD	m_dwAccEntrance;							// ������ �����
	DWORD	m_dwAccExp;									// ���� ����ġ
	CItem	m_cMagicFieldArrayItem;						// ������ ������
	DWORD	m_dwLayerFormation[MAX_LAYER_PER_DUNGEON];
	CItem	m_cGuardianItem;							// ����� ������	
	BYTE	m_byDefenseCount;							// ��� ���� ��
	BYTE	m_byProperty;								// �Ӽ�
	BYTE	m_byEntererMinLevel;						// ���ó��� �ּ� ����
	BYTE	m_byEntererMaxLevel;						// ���ó��� �ִ� ����
	BYTE	m_byCallGuardian;							// ��ȣ ����� �θ��� �ֳ�?
};
#pragma pack(pop)


#endif 
