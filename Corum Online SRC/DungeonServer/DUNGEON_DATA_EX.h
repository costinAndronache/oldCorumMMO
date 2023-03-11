#if !defined(AFX_DUNGEON_DATA_EX_H__F8906694_C290_4F80_AFC8_5C85DA83FE1C__INCLUDED_)
#define AFX_DUNGEON_DATA_EX_H__F8906694_C290_4F80_AFC8_5C85DA83FE1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


#include "dungeon_data.h"


class CUser;

// ������ Ŭ���� �ǰ� ������ ������ ������.
const DWORD EVENT_DUNGEON_RESET_DELAY = 10000;


class DUNGEON_DATA_EX  : public DUNGEON_DATA
{	
	void				InitializeOperationMode();		// ���������� ������ ������..

public:

	LONG				m_lDungeonType;					// ���� Ÿ��.
	DWORD				m_dwOriginalOwnerIndex;			// ���� ����
	DWORD				m_dwRemainSiegeStartTick;		// ������ �ҷ��� ��ð� ���Ҵ°�?
	DWORD				m_dwRemainSiegeEndTick;			// ������ ����ϱ� ��ð� ���Ҵ°�?
	DWORD				m_dwRemainLevelStartTick;		// ���� ���� (�ð��� ������ ���� ������ �����Ѵ�.)
	DWORD				m_dwRemainGradeCraeteTick;		// ����ǰ ������� ���� �ð�.	
	BYTE				m_byEventDungeonType;

	WORD				GetLevel() const ;				// ���� ����
	WORD				GetUpgradeTime();				// ���� ���׷��̵� �ð�
	int					GetIdleTime();					// ������ �Ҷ������� �޽ıⰣ.
	void				SetSiegeDestTime(DWORD dwTick);	// ������ �� �ð� �����Ѵ�.
	void				DecisionDungeonType(DWORD dwID);
	int					GetDungeonType() const;
	CItem*				GetMagicFieldArrayItem();
	BOOL				LevelUpgrade();					// ���� ���� ���׷��̵��ϱ�.
	WORD				CreateProduction(BOOL bWin);	// ����ǰ �ƾƵ� ����.
	int					GetRemainTime(DWORD dwDestTime, DWORD dwCurTick, DWORD dwFlag);	// TIME_SEC, TIME_MIN, TIME_HOUR
	BOOL				IsStartWarTime();
	BOOL				IsEndWarTime();
	BOOL				IsLevelUpgradeTime();
	BOOL				IsCreateProductionTime();
	WORD				ConvertProduction();
	DWORD				GetCPTime() const;					// cp�ð�.
	void				SetOwner(DWORD dwOwnerIndex, DWORD dwGuildID, char* pOwnerID, char* pOwnerName);
	DWORD				GetEntrancePay();
	DWORD				GetMaxEntrancePay();
	void				SetEntrance(DWORD dwEntrance);
	int					SetAccEntrance(int nPlusMoney);	
	void				SetMagicFieldArray(CItem* pItem);
	void				SetGuardianItem(CItem* pItem);
	const CItem*		GetGuardianItem();
	void				SetOperationMode(BYTE byOperationMode);
	void				SetAccExp(DWORD dwExp);
	BOOL				IsDungeonOwner(const CUser* const pUser) const;
	void				SetDefenseCount(BYTE byDefenseCount, BOOL bDB_Update);

	BOOL				IsPathWay()const;
	BOOL				IsConquer()const;
	BOOL				IsVillage()const;
	BOOL				IsStadium()const;//����� ���̳�?: hwoarang 050202
	BOOL				IsEventDungeon()const;

	DUNGEON_DATA_EX();
	virtual ~DUNGEON_DATA_EX();

};
#pragma pack(pop)

#endif 

