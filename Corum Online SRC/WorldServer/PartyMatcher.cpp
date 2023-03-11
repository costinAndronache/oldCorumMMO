#include "stdafx.h"
#include "PartyMatcher.h"

// ��Ƽ ��Ī�� ���� ���� ���� ������
bool operator<(const MATCH_CONDITION_PARTY& f, const MATCH_CONDITION_PARTY& b)
{
	int f_sum, b_sum;

	f_sum = f.bLevel * 100 + f.bExp * 10 + f.bItem;
	b_sum = b.bLevel * 100 + b.bExp * 10 + b.bItem;
	
	return (f_sum < b_sum);
}

// ���� ��Ƽ ��Ī �ý���
CMatcher<MATCH_CONDITION_PARTY, DWORD, CMatcherActionParty> g_PartyMatcher;