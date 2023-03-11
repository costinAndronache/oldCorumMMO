#include "stdafx.h"
#include "Section.h"
#include "Monster.h"
#include "MonsterManager.h"


CSection::CSection()
{
	m_pPcList = new CPcList;
	m_pMonsterList = new CMonsterList;	
}


CSection::~CSection()
{
	if (m_pPcList)
	{
		delete m_pPcList;
		m_pPcList = NULL;
	}

	if (m_pMonsterList)
	{
		delete m_pMonsterList;
		m_pMonsterList = NULL;
	}
}


// �ش� ���ǿ� BaseMonsterID ������ ���Ͱ� �ִ��� ã�´�. ������ NULL
// ���Ǿȿ� Ư�� BaseMonsterID�� ���Ͱ� �ִ��� ã�´�. 
CMonster* CSection::FindMonster(DWORD dwBaseMonsterID)
{
	POSITION_ pos		= m_pMonsterList->GetHeadPosition();
	CMonster* pMonster	= NULL;

	while(pos)
	{
		pMonster = (CMonster*)m_pMonsterList->GetNext(pos);

		if(pMonster->IsGuardian())
			continue;
		if(pMonster->GetBaseMonsterInfo()->dwID == dwBaseMonsterID)
			return pMonster;
	}

	return NULL;
}
