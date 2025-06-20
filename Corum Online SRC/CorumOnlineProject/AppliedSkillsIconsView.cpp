#include "AppliedSkillsIconsView.h"
#include "UserInterface.h"
#include <set>

const auto skillIconSize = 35;

void renderAppliedSkill(const AppliedSkill* appliedSkill, unsigned int index) {
	static char szInfo[0xff] = { 0, };
	if (!g_sSkillListManager.pSpr[appliedSkill->pEffect->skillKind]) { return; }

	int nPosX = (int)10 + (index * 35);
	int nPosY = (int)10;

	VECTOR2 vPos = { (float)nPosX,  (float)nPosY };

	g_pRenderer->RenderSprite(g_sSkillListManager.pSpr[appliedSkill->pEffect->skillKind], NULL, 0.0f, &vPos, NULL, 0xffffffff, __ORDER_USERINTERFACE_START_, RENDER_TYPE_DISABLE_TEX_FILTERING);

	wsprintf(szInfo, "%u", appliedSkill->GetRemainTime(g_dwCurTick));

	const auto nSize = lstrlen(szInfo);
	RECT rt;

	SetRect(
		&rt,
		nPosX + 30 - (nSize * 7),
		nPosY + 17,
		nPosX + 30,
		nPosY + 31
	);

	g_pGeometry->RenderFont(GetFont(), szInfo, nSize, &rt, 0xffffffff, CHAR_CODE_TYPE_ASCII, 1, 0);
}

void AppliedSkillsIconsView::renderAppliedSkillsIcons() {
	
	unsigned int processedSkills = 0;
	int		nSize = 0;

	g_pMainPlayer->skillsAppliedOnThisUnit->iterateWith([&processedSkills](void* current, unsigned int index) {
		AppliedSkill* pEffectDesc = (AppliedSkill*)current;
		renderAppliedSkill(pEffectDesc, processedSkills);
		processedSkills += 1;
	});

	std::set<BYTE> renderedAlready;
	CVoidList::FilterConditionFn onlyElementalsAndOD = [](void* current) {
		const auto appliedSkill = (AppliedSkill*)current;
		return
			(appliedSkill->skillKind == __SKILL_GREENELEMENTAL__) ||
			(appliedSkill->skillKind == __SKILL_REDELEMENTAL__) ||
			(appliedSkill->skillKind == __SKILL_BLUEELEMENTAL__) ||
			(appliedSkill->skillKind == __SKILL_SUMMONACCELERATOR__);
	};

	for (int i = 0; i < MAX_USER_GUARDIAN; i++) {
		const auto servant = g_pMainPlayer->servantMonsters[i];
		if (!servant) { continue;}

		const auto servantAppliedSkillsRelevant = servant->skillsAppliedOnThisUnit->filterWith(onlyElementalsAndOD);
		for (const auto item : servantAppliedSkillsRelevant) {
			const AppliedSkill* appliedSkill = (const AppliedSkill*)item;
			if (renderedAlready.find(appliedSkill->skillKind) == renderedAlready.end()) {
				renderedAlready.insert(appliedSkill->skillKind);
				renderAppliedSkill(appliedSkill, processedSkills);
				processedSkills += 1;
			}
		}
		
	}

	if (CUserInterface::GetInstance()->m_dwMagicArray != 0)
	{
		LP_ITEM_RESOURCE_EX lpItemResource = g_pItemResourceHash->GetData(CUserInterface::GetInstance()->m_dwMagicArray);

		if (lpItemResource)
		{
			int nPosX = (int)10 + (processedSkills * 35);
			int nPosY = (int)10;

			VECTOR2 vPos = { (float)nPosX, (float)nPosY };
			g_pRenderer->RenderSprite(lpItemResource->pSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, __ORDER_USERINTERFACE_START_, RENDER_TYPE_DISABLE_TEX_FILTERING);
		}
	}
}