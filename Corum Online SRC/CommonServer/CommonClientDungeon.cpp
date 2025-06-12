#include "stdafx.h"
#include "CommonClientDungeon.h"
#include <map>

struct CacheCumulatedExp {
private:
	DWORD cumulatedAtLevel[MAX_LEVEL + 1];
public:
	CacheCumulatedExp(const DWORD (&expToGatherForLevel)[MAX_LEVEL + 1]) {
		DWORD sum = 0;
		for (int i = 1; i <= MAX_LEVEL; i++) {
			sum += expToGatherForLevel[i];
			cumulatedAtLevel[i] = sum;
		}
	}

	DWORD cumulatedAt(DWORD level) const {
		if (!(1 <= level && level <= MAX_LEVEL)) { return 0; }

		return cumulatedAtLevel[level];
	}
};

struct CacheCumulatedExpStore {
	std::map<const DWORD*, CacheCumulatedExp*> stored;
	const CacheCumulatedExp& getCacheFor(const DWORD (&expToGatherForLevel)[MAX_LEVEL + 1]) {
		auto found = stored.find(expToGatherForLevel);
		if (found == stored.end()) {
			CacheCumulatedExp* newInstance = new CacheCumulatedExp(expToGatherForLevel);
			stored.insert({ expToGatherForLevel, newInstance });
			return *newInstance;
		}

		return *found->second;
	}
};

static CacheCumulatedExpStore _cumulatedExpStore;

DWORD DECLSPECIFIER cumulatedExperienceAtLevel(DWORD level, const DWORD (&expToGatherForLevel)[MAX_LEVEL + 1]) {
	return _cumulatedExpStore.getCacheFor(expToGatherForLevel).cumulatedAt(level);
}

DWORD DECLSPECIFIER levelForCumulatedExperience(DWORD exp, const DWORD (&expToGatherForLevel)[MAX_LEVEL + 1]) {

	const auto cache = _cumulatedExpStore.getCacheFor(expToGatherForLevel);
	for (int level = 1; level <= MAX_LEVEL; level += 1) {
		if (exp <= cache.cumulatedAt(level)) { return level - 1; }
	}

	return MAX_LEVEL;
}

void AttackResult::applyFor(std::function<void(DWORD)> hpApply, std::function<void(DWORD)> spApply) {
	if (bStatusKind == USER_HP) {
		hpApply(dwCurUserHP);
	} else if (bStatusKind == USER_MP) {
		hpApply(dwCurUserHP >> 16);
		spApply(dwCurUserHP & 0xffff);
	}
}

AttackResult::~AttackResult() { }