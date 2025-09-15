#pragma once

#include "CommonHeader.h"

#pragma pack(push,1)
// actually the order of the fields is messed up, 
// but for now I keep it that way nd process what's wrong
// after db retrieval ; 
struct CZP_QUERY_DUNGEON_INFO_WORLD_ResultRow 
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
	DWORD	m_dwPieceCoolTime;	
	DWORD	m_wBattleStartTime;
	BYTE	m_byDefenseCount;							// ��� ����Ƚ��
	BYTE	m_byProperty;								// �Ӽ�
	BYTE	m_byEntererMinLevel;						// ���ó��� �ּ� ����
	BYTE	m_byEntererMaxLevel;						// ���ó��� �ִ� ����
	double	m_dWorldPos_X;		
	double	m_dWorldPos_Z;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct CZP_REQUEST_DUNGEON_INFO_DG_ResultRow
{	
	DWORD	m_dwID;
	WORD	m_wGroupID; // unused
	char	m_szDungeonName[ MAX_DUNGEON_NAME_LENGTH ];
	WORD	m_wAreaNum; // unused
	WORD	m_wAreaLevel_h; // unused								// 0 = ���ʺ���, 1 = �ʺ���, 2 = �߼���, 3 = �����, 4 = �����������
	WORD	m_wAreaLevel_l; // unused
	WORD	m_wInitLevel;								// ���� �⺻ ����
	WORD	m_wUpgradeLevel;	// + ^ :: connected to siege wars, control point time; 
	DWORD	m_wBattleTimeLimit;	// unused???						// ������ ���� �ð�.
	WORD	siegeDungeonPrize_itemID;			// 					// ����ǰ ���̵�.
	BYTE	siegeDungeonPrize_currentMakingStep; // a.k.a siege dungeon item evolution step (on 3 steps it is ready)		
	BYTE	inSiegeWarNow;									// ������ : 1
	BYTE	current_DUNGEON_OPERATIONTYPE;							// ���� � ����..0 = ��ָ��, 1 = �����, 2 = ������, 3 = ���������
	WORD	m_wLayerCount;								// ������ ���̾�  ����.
	DWORD	ownerUserID;								// ���� ���� �ε���
	char	m_szOwner[ MAX_CHARACTER_NAME_LENGTH ];		// ������ �Ǵ� �������� 	
	char    m_szManager[ MAX_CHARACTER_NAME_LENGTH ];	// unused
	DWORD	ownerGuildID;
	DWORD	entranceFee;								// �����
	DWORD	accumulatedEntranceFees;							// ������ �����
	DWORD	accumulatedEXPForOwner;									// ���� ����ġ
	CItem	m_cMagicFieldArrayItem;						// ������ ������
	DWORD	m_dwLayerFormation[MAX_LAYER_PER_DUNGEON];
	CItem	m_cGuardianItem;							// ����� ������	
	BYTE	m_byDefenseCount;							// ��� ���� ��
	BYTE	siegeDungeonPrize_productionSchemeIndex;								// �Ӽ�
	BYTE	m_byEntererMinLevel;						// ���ó��� �ּ� ����
	BYTE	m_byEntererMaxLevel;						// ���ó��� �ִ� ����
	BYTE	invokeDungeonGuardianOnSiegeWar;							// ��ȣ ����� �θ��� �ֳ�?
};
#pragma pack(pop)