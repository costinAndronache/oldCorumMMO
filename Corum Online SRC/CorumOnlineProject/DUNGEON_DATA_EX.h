// DUNGEON_DATA_EX.h: interface for the DUNGEON_DATA_EX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DUNGEON_DATA_EX_H__36AA0CD6_6522_4119_9CC7_866802C63C95__INCLUDED_)
#define AFX_DUNGEON_DATA_EX_H__36AA0CD6_6522_4119_9CC7_866802C63C95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dungeon_data.h"
#include "VoidList.h"
struct V2_SPRITE;
class CUser;


/// Constants ////////////////////////////////////////////////////////////////

#define EVENT_DUNGEON_EFFECT_INCRESEMENT	0xFFF00000
#define EVENT_DUNGEON_RELEASE_INCRESEMENT	0x7FF00000

//----------------------------------------------------------------------------
class DUNGEON_DATA_EX  : public DUNGEON_DATA
{

public:

	BYTE				m_byDungeonType;					// ���� Ÿ��.
	
	DWORD				m_dwRemainSiegeStartTick;		// ������ �ҷ��� ��ð� ���Ҵ°�?
	DWORD				m_dwRemainSiegeEndTick;			// ������ ����ϱ� ��ð� ���Ҵ°�?
	DWORD				m_dwRemainLevelStartTick;		// ���� ���� (�ð��� ������ ���� ������ �����Ѵ�.)
	DWORD				m_dwStopLevelTick;				// ���߾����� �������۵Ǳ������ �����ð�.
	DWORD				m_dwStopStartTick;

	WORD				m_wWorldMapID;					// ��ġ�� ���� ��ȣ
#if IS_JAPAN_LOCALIZING()
	char				m_szSchoolName[MAX_CHARACTER_NAME_LENGTH]; // �Ϻ����� ���, ���� �б� ��� �̸�
#endif

	BYTE				bReceivedInfo;		//World ������ ���� Owner ���� ���� �����ΰ� �ƴѰ� (0�̸� ���� �ȹ޾ҽ�, 1�̸� ���� �޾ҽ�, 2�̸� ���� �����) 
	VECTOR3				vPos;				//����ʻ��� ���� ��ǥ 	
	V2_SPRITE*			pSprMark;			//������� ��ũ 
	BYTE				bDivision;			//���� ���� 

	POSITION_			pListPos;
	POSITION_			pReceivedDungeonPos;	
	DUNGEON_DATA_EX*	pNext;
	DUNGEON_DATA_EX*	pPrev;
	GXOBJECT_HANDLE		m_hOwnDungeon;		// �ڱ� ���� ǥ�� ����Ʈ
	GXOBJECT_HANDLE		m_hSiegingDungeon;	// �������� ���� ǥ�� ����Ʈ
	GXOBJECT_HANDLE		m_hGroupDungeon;	// ���� �׷쳢���� ���� ǥ�� ����Ʈ 

	GXOBJECT_HANDLE		m_hEventDungeonModel;	// �̺�Ʈ �������� Ư���� ���, ����ʿ� ������ �Ա��� �����͸� ���� ���� �־�� �Ѵ�.
	GXOBJECT_HANDLE		m_hEventDungeonEffect;	// �̺�Ʈ �������� Ư���� ���, ����ʿ� ������ ����Ʈ �����͸� ���� ���� �־�� �Ѵ�.		
	
	
	void				SetDungeonType(DWORD dwID);
	INT					GetDungeonType();
	WORD				GetLevel();								// ���� ����
	WORD				GetUpgradeTime();						// ���� ���׷��̵� �ð�
	int					GetIdleTime();							// ������ �Ҷ������� �޽ıⰣ.
	void				SetSiegeStartDestTime(DWORD dwTick);	// ������ �� �ð� �����Ѵ�.
	void				SetSiegeEndDestTime(DWORD dwTick);
	BOOL				StartSiegeWarTime();					// ������ �����ص� �Ǵ°�?
	BOOL				LevelUpgrade();							// ���� ���� ���׷��̵��ϱ�.
	int					GetRemainTime(DWORD dwDestTime, DWORD dwCurTick, DWORD dwFlag);	// TIME_SEC, TIME_MIN, TIME_HOUR
	
	BOOL				IsConquer();
	BOOL				IsVillage()const{ return (DUNGEON_TYPE_VILLAGE == m_byDungeonType);}//���� ���̳�?: hwoarang 050202
	BOOL				IsPathWay()const{ return (DUNGEON_TYPE_TUNNEL == m_byDungeonType);}//��� ���̳�?: hwoarang 050202
	bool				IsStadium()const{ return (DUNGEON_TYPE_MATCH_STATIDUM == m_byDungeonType);}//����� ���̳�?: hwoarang 050202

	BOOL				IsStartWarTime();
	BOOL				IsEndWarTime();
	BOOL				IsLevelUpgradeTime();
	DWORD				GetCPTime();							// cp�ð�.
	void				SetOwner(DWORD dwOwnerIndex, DWORD dwGuildID, char* pOwnerName, char* pszGuildName);	
	DWORD				GetEntrancePay();
	DWORD				GetMaxEntrancePay();
	void				Initialize();
	void				SetOperationMode(BYTE bOperationMode);
	void				StopOperationTime();					// �������� ���۵Ǹ� ������, ��������� �ð��� ���߰� �ȴ�.
	void				ResumeOperationTime();					// �������� ���߰� �Ǹ� ������, ��������� �ð��� �ٽ� �帣�� �ȴ�.
	BOOL				IsDungeonOwner(CUser* pUser);

	BOOL				IsKaien();

	// �� �Ʒ��� Event Dungeon ���� �κ�.	
	BYTE						m_byEventAlphaStep;		// ���ĺ��� ����.
	BYTE						m_byEventDisplayState;	// ȭ�� ǥ�ø� ���� ����.
	BOOL						m_bEventUsed;	
	BYTE						m_byEventType;			// �̺�Ʈ ���� Ÿ��.
	DWORD						m_dwEventEntered;		// �̺�Ʈ ������ �� ���� ��.
	DWORD						m_dwEventStartTick;		// �̺�Ʈ ���� ���۵� �ð�.
	DWORD						m_dwEventEndTick;
	DWORD						m_dwEventOpenReadyTime;	// �̺�Ʈ ���� �Ա� ���� �ð�.
	DWORD						m_dwEndOpenReadyTime;
	BYTE						m_bEventState;			// �̺�Ʈ ���� ����.
	BYTE						m_byEventEnterMinLevel;	// �̺�Ʈ ���� ���� ���� �ּ� ����.
	BYTE						m_byEventEnterMaxLevel;	// �̺�Ʈ ���� ���� ���� �ִ� ����.	
	//BOOL						IsEvent(VOID);			// �̺�Ʈ �����ΰ�?
	
	DUNGEON_DATA_EX();
	virtual ~DUNGEON_DATA_EX();
};


//----------------------------------------------------------------------------

#endif // !defined(AFX_DUNGEON_DATA_EX_H__36AA0CD6_6522_4119_9CC7_866802C63C95__INCLUDED_)

