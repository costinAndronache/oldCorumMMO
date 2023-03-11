// DUNGEON_DATA.h: interface for the DUNGEON_DATA class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DUNGEON_DATA_H__937E4CB7_8A67_4DA5_BB5C_E12286084706__INCLUDED_)
#define AFX_DUNGEON_DATA_H__937E4CB7_8A67_4DA5_BB5C_E12286084706__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Define.h"
#include "../CommonServer/CommonHeader.h"


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
	WORD	m_wAreaLevel_h;	//0 = ���ʺ���, 1 = �ʺ���, 2 = �߼���, 3 = �����, 4 = �����������
	WORD	m_wAreaLevel_l;
	WORD	m_wInitLevel;		// ���� �⺻ ����
	WORD	m_wUpgradeLevel;	// ���� ���׷��̵�� ����
	DWORD	m_wBattleTimeLimit;	// ������ ���� �ð�.
	WORD	m_wProduction;	// ����ǰ ���̵�.
	BYTE	m_byProductionCount;	// ����ǰ ����.	
	BYTE	m_bSiege;		//������ : 1
	BYTE	m_bOperationType;	// ���� � ����..0 = ��ָ��, 1 = �����, 2 = ������, 3 = ���������
	WORD	m_wLayerCount;		// ������ ���̾�  ����.
	DWORD	m_dwOwnerIndex;		// ���� �ε���
	

	char	m_szOwner[ MAX_CHARACTER_NAME_LENGTH ];		//������ �Ǵ� �������� 	
	char    m_szManager[ MAX_CHARACTER_NAME_LENGTH];	//������ ĳ���͸�..  �������ΰ�� ������� �渶
	
	char	m_szMemo[ MAX_DUNGEON_MEMO_LENGTH ];		//���� �������� �޸� 
	DWORD	m_dwOwnerGuildNum;
	CItem	m_cMagicFieldArrayItem;		// ������ ������
	DWORD	m_dwEntrance;				// �����
	DWORD	m_dwAccEntrance;			// ������ �����
	DWORD	m_dwAccExp;			// ������ ����ġ
	CItem	m_cGuardianItem;			// ����� ������	
	BYTE	m_byDefenseCount;			// ���潺 Ƚ��
	BYTE	m_byProperty;					// �Ӽ�
	BYTE	m_byEntererMinLevel;			// ���ó��� �ּ� ����
	BYTE	m_byEntererMaxLevel;			// ���ó��� �ִ� ����

	
};
#pragma pack(pop)
#endif // !defined(AFX_DUNGEON_DATA_H__937E4CB7_8A67_4DA5_BB5C_E12286084706__INCLUDED_)
