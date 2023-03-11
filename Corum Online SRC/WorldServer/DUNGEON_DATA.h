#if !defined(AFX_DUNGEON_DATA_H__4320E585_0233_49AB_A72D_5FE1E50B5D53__INCLUDED_)
#define AFX_DUNGEON_DATA_H__4320E585_0233_49AB_A72D_5FE1E50B5D53__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


#include "../CommonServer/CommonHeader.h"


#pragma pack(push,1)
struct DUNGEON_DATA
{
	DWORD	m_dwID;										// ���� ���̵�.
	WORD	m_wWorldServerNo;							// �� ������ � ���弭���� �����ִ� �������� ������ �ִ� ���弭�� ��ȣ 
	WORD	m_wWorldMapID;								// ����� �����ȣ, (����)�����ȣ�� �ٸ��ٰ� �ؼ� ���弭���� �ٸ����� �ƴϴ�. 
	WORD	m_wGroupID;									// �׷� ���̵�.
	char	m_szDungeonName[MAX_DUNGEON_NAME_LENGTH];
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
	WORD	m_wLayerCount;								// ������.
	DWORD	m_dwOwnerIndex;
	char	m_szOwnerID[MAX_CHARACTER_NAME_LENGTH ];	// �������� �α� ���̵�
	char	m_szOwner[MAX_CHARACTER_NAME_LENGTH];		// ������ �Ǵ� �������� 	
	char    m_szManager[MAX_CHARACTER_NAME_LENGTH];		// ������ ĳ���͸�..  �������ΰ�� ������� �渶
	char	m_szMemo[MAX_DUNGEON_MEMO_LENGTH];			// ���� �������� �޸� 
	DWORD	m_dwOwnerGuildNum;
	CItem	m_cMagicFieldArrayItem;						// ������ ������
	DWORD	m_dwPort;
	DWORD	m_dwEntrance;								// �����
	DWORD	m_dwAccEntrance;							// ������ �����
	DWORD	m_dwAccExp;									// ������ ����ġ
	CItem	m_cGuardianItem;							// ����� ������
	DWORD	m_dwPieceStartTime;							// ��ȭ�Ӱ� ���°� �ٲ�� ������ �ð�.
	DWORD	m_dwPieceCoolTime;							// ���� �ֱ�
	DWORD	m_wBattleStartTime;							// ������ ������ �ð�
	BYTE	m_byDefenseCount;							// ��� ����Ƚ��
	BYTE	m_byProperty;								// �Ӽ�
	BYTE	m_byEntererMinLevel;						// ���ó��� �ּ� ����
	BYTE	m_byEntererMaxLevel;						// ���ó��� �ִ� ����
	double	m_dWorldPos_X;		
	double	m_dWorldPos_Z;
};
#pragma pack(pop)


#endif
