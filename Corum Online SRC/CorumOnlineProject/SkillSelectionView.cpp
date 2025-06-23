#include "SkillSelectionView.h"
#include "GameControl.h"
#include "Interface.h"
#include "UserInterface.h"
#include "ChatListWnd.h"
#include "DefResource.h"
#include "message.h"
#include "CResNameResolver.h"
#include "InitGame.h"
#include "NetworkClient.h"
#include "effect.h"
#include "CodeFun.h"
#include "DungeonInterfaceLayout.h"

///

VECTOR2 positionForIndex(unsigned int index) {
	return {
		skillSelectionBottomRowStart.x + SKILL_ICON_SIZE * (index % SKILL_SELECTION_ROW_COUNT),
		skillSelectionBottomRowStart.y - SKILL_ICON_SIZE * (index / SKILL_SELECTION_ROW_COUNT)
	};
}

int indexForPosition(VECTOR2 pos) {
	const auto size = SKILL_SELECTION_ROW_COUNT * SKILL_ICON_SIZE;
	const auto inXRange = (skillSelectionBottomRowStart.x <= pos.x && pos.x <= skillSelectionBottomRowStart.x + size);
	const auto inYRange = ( (skillSelectionBottomRowStart.y + SKILL_ICON_SIZE) - size <= pos.y && pos.y <= skillSelectionBottomRowStart.y + SKILL_ICON_SIZE);

	if (!inXRange || !inYRange) { return -1; }

	
	const int xWalk = (pos.x - skillSelectionBottomRowStart.x) / SKILL_ICON_SIZE;
	const int yWalk = (int)(skillSelectionBottomRowStart.y + SKILL_ICON_SIZE - pos.y) / SKILL_ICON_SIZE;

	return yWalk * SKILL_SELECTION_ROW_COUNT + xWalk;

}

///
std::shared_ptr<SkillSelectionView> SkillSelectionView::sharedInstance() {
	static std::shared_ptr<SkillSelectionView> _shared = nullptr;
	
	if (!_shared) { 
		_shared = std::make_shared<SkillSelectionView>(g_pMainPlayer->skillsAvailableOnLeft(), g_pMainPlayer->skillsAvailableOnRight()); 
	}
	return _shared;
}

void SkillSelectionView::handleMouseButtonDownEvent(VECTOR2 mousePoint) {
	if (pointInsideSquare(mousePoint, leftSkillPos)) { 
		setActiveSkillSelectionWindowType(SkillSelectionWindow::leftSkills);
		return;
	}

	if (pointInsideSquare(mousePoint, rightSkillPos)) {
		setActiveSkillSelectionWindowType(SkillSelectionWindow::rightSkills);
		return;
	}

	if (pointInsideSquare(mousePoint, guardianSkillPos)) {
		setActiveSkillSelectionWindowType(SkillSelectionWindow::guardianSkills);
		return;
	}

	const auto skillSource = currentSkillSource();
	const auto index = indexForPosition(mousePoint);
	if (index >= 0) { 
		skillSource.selectSkillAtIndex(index);
		setActiveSkillSelectionWindowType(SkillSelectionWindow::none);
	}
}

SkillSelectionWindow SkillSelectionView::activeSkillSelectionWindowType() const {
	return _activeSkillSelectionWindowType;
}

void SkillSelectionView::setActiveSkillSelectionWindowType(SkillSelectionWindow type) {
	_activeSkillSelectionWindowType = type;
}

SkillSelectionView::SkillSource SkillSelectionView::SkillSource::none{ 
	std::vector<BYTE>(), 
	[](int) {}
};

SkillSelectionView::SkillSelectionView(std::vector<BYTE> skillsOnLeft, std::vector<BYTE> skillsOnRight) {
	m_pLineSpr = g_pRenderer->CreateSpriteObject(GetFile("menu_4.tif", DATA_TYPE_UI), 71, 33, 71, 38, 0);
	m_pAttackSpr = g_pRenderer->CreateSpriteObject(GetFile("skill_icon1.tga", DATA_TYPE_UI), 0, 0, 32, 32, 0);
	_activeSkillSelectionWindowType = SkillSelectionWindow::none;

	_currentLeftSkills = skillsOnLeft;
	_currentRightSkills = skillsOnRight;

	_currentLeftSkills.insert(_currentLeftSkills.begin(), __SKILL_ATTACK__);

}

SkillSelectionView::SkillSource SkillSelectionView::currentSkillSource() {
	CMonster* pGuardian = g_pMainPlayer->m_pGuardian[0];

	switch (_activeSkillSelectionWindowType) {
	case SkillSelectionWindow::none:
		return SkillSource::none;
		break;
	case SkillSelectionWindow::leftSkills:
		return {
			_currentLeftSkills,
			[this](int index) {
				if (!(0 <= index && index < _currentLeftSkills.size())) { return; }
				g_pMainPlayer->SetSkillChangeLR(_currentLeftSkills[index], 0);
			}
		};
		break;
	case SkillSelectionWindow::rightSkills:
		return {
			_currentRightSkills,
			[this](int index) {
				if (!(0 <= index && index < _currentRightSkills.size())) { return; }
				g_pMainPlayer->SetSkillChangeLR(_currentRightSkills[index], 1);
			}
		};
		break;
	case SkillSelectionWindow::guardianSkills:
		if (!pGuardian) { return SkillSource::none; }
		_currentGuardianSkills = pGuardian->currentSkills();
		return {
			_currentGuardianSkills,
			[pGuardian, this](int index) {
				if (!(0 <= index && index < _currentGuardianSkills.size())) { return; }
				pGuardian->SetSelectedSkill(_currentGuardianSkills[index]);
			}
		};
		break;
	}
}

bool SkillSelectionView::isMouseHovering() {
	const auto skillSource = currentSkillSource();
	for (int i = 0; i < skillSource.skills.size(); i++) {
		const auto pos = positionForIndex(i);
		if (pointInsideSquare({ (float)g_Mouse.MousePos.x, (float)g_Mouse.MousePos.y }, pos)) {
			return true;
		}
	}

	return false;
}

void SkillSelectionView::renderCurrentSelectedLeftRightSkills() {
	int nOrder = __ORDER_USERINTERFACE_START_ + 4;

	BYTE skillKind_left = g_pMainPlayer->GetSkillKind(SELECT_ATTACK_TYPE_LBUTTON);
	BYTE skillKind_right = g_pMainPlayer->GetSkillKind(SELECT_ATTACK_TYPE_RBUTTON);

	if (skillKind_left != __SKILL_NONE_SELECT__)
	{

		if (skillKind_left == __SKILL_ATTACK__)
		{
			g_pRenderer->RenderSprite(m_pAttackSpr, NULL, 0.0f, &leftSkillPos, NULL, 0xffffffff, nOrder, RENDER_TYPE_DISABLE_TEX_FILTERING);
		}
		else
		{
			if (g_sSkillListManager.spriteForSkillKind[skillKind_left])
				g_pRenderer->RenderSprite(g_sSkillListManager.spriteForSkillKind[skillKind_left], NULL, 0.0f, &leftSkillPos, NULL, 0xffffffff, nOrder, RENDER_TYPE_DISABLE_TEX_FILTERING);
		}
	}

	if (skillKind_right != -1)
	{

		if (skillKind_right == __SKILL_ATTACK__)
		{
			g_pRenderer->RenderSprite(m_pAttackSpr, NULL, 0.0f, &rightSkillPos, NULL, 0xffffffff, nOrder, RENDER_TYPE_DISABLE_TEX_FILTERING);
		}
		else
		{
			if (g_sSkillListManager.spriteForSkillKind[skillKind_right])
				g_pRenderer->RenderSprite(g_sSkillListManager.spriteForSkillKind[skillKind_right], NULL, 0.0f, &rightSkillPos, NULL, 0xffffffff, nOrder, RENDER_TYPE_DISABLE_TEX_FILTERING);
		}
	}

	// 가디언이 소환중이면
	CMonster* pGuardian = g_pMainPlayer->m_pGuardian[0];
	if (pGuardian) {
		const auto currentGuardianSkill = pGuardian->GetSelectedSkill();
		// 선택된 스킬이 없다면
		if (currentGuardianSkill == 0) {
			// 물리공격 혹은 렌덤스킬 스프라이트 출력
			g_pRenderer->RenderSprite(m_pAttackSpr, NULL, 0.0f, &guardianSkillPos, NULL, 0xffaaffaa, nOrder, RENDER_TYPE_DISABLE_TEX_FILTERING);
		}
		else {
			// 선택된 스킬 스프라이트 출력
			if (g_sSkillListManager.spriteForSkillKind[currentGuardianSkill])
				g_pRenderer->RenderSprite(g_sSkillListManager.spriteForSkillKind[currentGuardianSkill], NULL, 0.0f, &guardianSkillPos, NULL, 0xffaaffaa, nOrder, RENDER_TYPE_DISABLE_TEX_FILTERING);
		}
	}
}


void SkillSelectionView::renderOpenedSkillSelection() {
	int nOrder = __ORDER_USERINTERFACE_START_ + 4;
	const auto currentSkills = currentSkillSource();
	for (int i = 0; i < currentSkills.skills.size(); i++) {
		VECTOR2 pos = positionForIndex(i);
		const auto skill = currentSkills.skills[i];

		if (skill == __SKILL_ATTACK__) {
			g_pRenderer->RenderSprite(m_pAttackSpr, NULL, 0.0f, &pos, NULL, 0xffaaffaa, nOrder, RENDER_TYPE_DISABLE_TEX_FILTERING);
			continue;
		}

		if (skill > 0 && g_pMainPlayer->GetSkillLevel(skill) > 0) {
			g_pRenderer->RenderSprite(g_sSkillListManager.spriteForSkillKind[skill], NULL, 0.0f, &pos, NULL, 0xffffffff, nOrder, RENDER_TYPE_DISABLE_TEX_FILTERING);
		}
	}
}

void SkillSelectionView::handleKeyEvent(char ascii) {
	//if (pSkillWnd->activeSkillSelectionWindowType == SkillSelectionWindow::leftSkills)
	//{
	//	for (int i = 0; i < 16; i++)
	//	{
	//		if (g_pMainPlayer->m_nSkillKey[i] != -1)
	//		{
	//			if (g_pMainPlayer->m_nSkillKey[i] == __SKILL_ATTACK__)
	//			{
	//				if (g_pMainPlayer->m_nPosSK[i] == 0)
	//				{
	//					int		nPosX = 116;
	//					int		nPosY = 646;
	//					char* szKey = KeySearch(g_sKeyConfig.snKey[18 + i]);

	//					if (!IsEmptyString(szKey))
	//						RenderFont(szKey, nPosX + 3, nPosX + 32, nPosY + 12, nPosY + 26, nOrder, 0xff00ff00);
	//				}
	//			}
	//			else
	//			{
	//				int nSkill_Idx = 1;

	//				for (int j = 0; j < g_sSkillListManager.byLeftSkillCnt; j++)
	//				{
	//					if (g_pMainPlayer->GetSkillLevel(g_sSkillListManager.byLeftSkill[j]) > 0)
	//					{
	//						//	int nPosX = 116+(nSkill_Idx%5)*33;
	//						//	int nPosY = 646-(nSkill_Idx/5)*33;

	//						if (g_pMainPlayer->m_nSkillKey[i] == g_sSkillListManager.byLeftSkill[j])
	//						{
	//							if (g_pMainPlayer->m_nPosSK[i] == 0)
	//							{
	//								int		nPosX = 116 + (nSkill_Idx % 5) * 32;
	//								int		nPosY = 646 - (nSkill_Idx / 5) * 32;
	//								char* szKey = KeySearch(g_sKeyConfig.snKey[18 + i]);

	//								if (szKey && !IsEmptyString(szKey))
	//									RenderFont(szKey, nPosX + 3, nPosX + 32, nPosY + 12, nPosY + 26, nOrder, 0xff00ff00);
	//							}
	//						}
	//						nSkill_Idx++;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
	//else if (pSkillWnd->activeSkillSelectionWindowType == SkillSelectionWindow::rightSkills)
	//{
	//	//		Effect* pEffect;

	//	for (int i = 0; i < 16; i++)
	//	{
	//		if (g_pMainPlayer->m_nSkillKey[i] != -1)
	//		{
	//			int nSkill_Idx = 0;

	//			for (int j = 0; j < g_sSkillListManager.byRightSkillCnt; j++)
	//			{
	//				if (g_pMainPlayer->GetSkillLevel(g_sSkillListManager.byRightSkill[j]) > 0)
	//				{
	//					if (g_pMainPlayer->m_nSkillKey[i] == g_sSkillListManager.byRightSkill[j])
	//					{
	//						if (g_pMainPlayer->m_nPosSK[i] == 1)
	//						{
	//							int		nPosX = 116 + (nSkill_Idx % 5) * 32;
	//							int		nPosY = 646 - (nSkill_Idx / 5) * 32;
	//							char* szKey = KeySearch(g_sKeyConfig.snKey[18 + i]);

	//							if (!IsEmptyString(szKey))
	//								RenderFont(szKey, nPosX + 3, nPosX + 32, nPosY + 12, nPosY + 26, nOrder, 0xff00ff00);
	//						}
	//					}
	//					nSkill_Idx++;
	//				}
	//			}
	//		}
	//	}
	//}
}

void SkillSelectionView::render() {
	renderCurrentSelectedLeftRightSkills();
	renderOpenedSkillSelection();

	// + render the shortcut key over the current selected left right skills 
	//if (pSkillWnd->activeSkillSelectionWindowType == SkillSelectionWindow::leftSkills)
	//{
	//	for (int i = 0; i < 16; i++)
	//	{
	//		if (g_pMainPlayer->m_nSkillKey[i] != -1)
	//		{
	//			if (g_pMainPlayer->m_nSkillKey[i] == __SKILL_ATTACK__)
	//			{
	//				if (g_pMainPlayer->m_nPosSK[i] == 0)
	//				{
	//					int		nPosX = 116;
	//					int		nPosY = 646;
	//					char* szKey = KeySearch(g_sKeyConfig.snKey[18 + i]);

	//					if (!IsEmptyString(szKey))
	//						RenderFont(szKey, nPosX + 3, nPosX + 32, nPosY + 12, nPosY + 26, nOrder, 0xff00ff00);
	//				}
	//			}
	//			else
	//			{
	//				int nSkill_Idx = 1;

	//				for (int j = 0; j < g_sSkillListManager.byLeftSkillCnt; j++)
	//				{
	//					if (g_pMainPlayer->GetSkillLevel(g_sSkillListManager.byLeftSkill[j]) > 0)
	//					{
	//						//	int nPosX = 116+(nSkill_Idx%5)*33;
	//						//	int nPosY = 646-(nSkill_Idx/5)*33;

	//						if (g_pMainPlayer->m_nSkillKey[i] == g_sSkillListManager.byLeftSkill[j])
	//						{
	//							if (g_pMainPlayer->m_nPosSK[i] == 0)
	//							{
	//								int		nPosX = 116 + (nSkill_Idx % 5) * 32;
	//								int		nPosY = 646 - (nSkill_Idx / 5) * 32;
	//								char* szKey = KeySearch(g_sKeyConfig.snKey[18 + i]);

	//								if (szKey && !IsEmptyString(szKey))
	//									RenderFont(szKey, nPosX + 3, nPosX + 32, nPosY + 12, nPosY + 26, nOrder, 0xff00ff00);
	//							}
	//						}
	//						nSkill_Idx++;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
	//else if (pSkillWnd->activeSkillSelectionWindowType == SkillSelectionWindow::rightSkills)
	//{
	//	//		Effect* pEffect;

	//	for (int i = 0; i < 16; i++)
	//	{
	//		if (g_pMainPlayer->m_nSkillKey[i] != -1)
	//		{
	//			int nSkill_Idx = 0;

	//			for (int j = 0; j < g_sSkillListManager.byRightSkillCnt; j++)
	//			{
	//				if (g_pMainPlayer->GetSkillLevel(g_sSkillListManager.byRightSkill[j]) > 0)
	//				{
	//					if (g_pMainPlayer->m_nSkillKey[i] == g_sSkillListManager.byRightSkill[j])
	//					{
	//						if (g_pMainPlayer->m_nPosSK[i] == 1)
	//						{
	//							int		nPosX = 116 + (nSkill_Idx % 5) * 32;
	//							int		nPosY = 646 - (nSkill_Idx / 5) * 32;
	//							char* szKey = KeySearch(g_sKeyConfig.snKey[18 + i]);

	//							if (!IsEmptyString(szKey))
	//								RenderFont(szKey, nPosX + 3, nPosX + 32, nPosY + 12, nPosY + 26, nOrder, 0xff00ff00);
	//						}
	//					}
	//					nSkill_Idx++;
	//				}
	//			}
	//		}
	//	}
	//}
	
	// + render an info box overed the hovered skill with its name
	/*RenderFont(g_pEffectLayer->m_Effect[bySkillValue].szName, g_Mouse.MousePos.x-nSize/2*6, g_Mouse.MousePos.x+100, g_Mouse.MousePos.y-10, g_Mouse.MousePos.y+4, __ORDER_ITEM_DESC__+2);
															
	CInterface::GetInstance()->RenderInfoBox(float(g_Mouse.MousePos.x - g_pEffectLayer->m_nEffectSize[bySkillValue] / 2 * 7 - 5), float(g_Mouse.MousePos.y - 16), float(g_pEffectLayer->m_nEffectSize[bySkillValue] / 2 * 8 * 2 + 10), 24, __ORDER_ITEM_DESC__);*/
}


void SkillSelectionView::updatedSkills(CMainUser* mainUser) {
	_currentLeftSkills = mainUser->skillsAvailableOnLeft();
	_currentRightSkills = mainUser->skillsAvailableOnRight();

	_currentLeftSkills.insert(_currentLeftSkills.begin(), __SKILL_ATTACK__);
}