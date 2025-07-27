#include "UserSkillsManager.h"
#include "../../DungeonInterfaceLayout.h"

using namespace NewInterface;
using namespace CustomUI;


int UserSkillsManager::maxNumOfSkillsInList() {
	return 16;
}

struct SkillKinds {
	std::vector<LP_SKILL_RESOURCE_EX> active;
	std::vector<LP_SKILL_RESOURCE_EX> passive;
	std::vector<LP_SKILL_RESOURCE_EX> overdrive;
	std::vector<LP_SKILL_RESOURCE_EX> mastery;
};

static SkillKinds getSkillKindsForClass(SSKILL_LIST skillList ) {
	auto transformFn = [=](void* item, unsigned int) -> LP_SKILL_RESOURCE_EX {
		return (LP_SKILL_RESOURCE_EX)item;
	};

	return {
		skillList.pActiveList->transform<LP_SKILL_RESOURCE_EX>(transformFn),
		skillList.pPassiveList->transform<LP_SKILL_RESOURCE_EX>(transformFn),
		skillList.pOverDriveList->transform<LP_SKILL_RESOURCE_EX>(transformFn),
		skillList.pMasteryList->transform<LP_SKILL_RESOURCE_EX>(transformFn),
	};
}

static SkillSheetView::Model convert(
	const SkillKinds& skillKinds,
	std::function<GenericSkillView::Model(LP_SKILL_RESOURCE_EX)> transformFn
) {
	std::function<
		std::vector<GenericSkillView::Model> (const std::vector<LP_SKILL_RESOURCE_EX>& from)
	> transformArrays = [=](auto from) {
		std::vector<GenericSkillView::Model> resultArray;
		std::transform(
			std::begin(from),
			std::end(from),
			std::back_inserter(resultArray),
			transformFn
		);
		return resultArray;
	};

	auto masteryModel = [=]() -> GenericSkillView::Model {
		if (skillKinds.mastery.size() > 0) {
			return transformFn(skillKinds.mastery[0]);
		}
		else {
			return GenericSkillView::Model::zero();
		}
	}();
	masteryModel.increaseHandler = nullptr;

	return {
		transformArrays(skillKinds.active),
		transformArrays(skillKinds.passive),
		transformArrays(skillKinds.overdrive),
		masteryModel
	};
}

UserSkillsManager::UserSkillsManager(
	UserSkillsView* managedView,
	CMainUser* mainUser,
	SharedNetwork* network,
	SSKILL_LIST_MANAGER* skillListManager,
	EffectLayer* effectLayer,
	SoundLibrary* soundLibrary
) {
	_managedView = managedView;
	_mainUser = mainUser;
	_skillListManager = skillListManager;
	_network = network;
	_effectLayer = effectLayer;
	_soundLibrary = soundLibrary;
}

void UserSkillsManager::refreshUserSkillsView() {
	auto transformFn = [=](LP_SKILL_RESOURCE_EX skill) {
		return buildModelFor(skill);
	};

	auto summoner = convert(getSkillKindsForClass(_skillListManager->pSkillList[CLASS_TYPE_SUMMONER]), transformFn);
	auto fighter = convert(getSkillKindsForClass(_skillListManager->pSkillList[CLASS_TYPE_WARRIOR]), transformFn);
	auto priest = convert(getSkillKindsForClass(_skillListManager->pSkillList[CLASS_TYPE_PRIEST]), transformFn);
	auto ranger = convert(getSkillKindsForClass(_skillListManager->pSkillList[CLASS_TYPE_HUNTER]), transformFn);
	auto sorceress = convert(getSkillKindsForClass(_skillListManager->pSkillList[CLASS_TYPE_WIZARD]), transformFn);

	_managedView->refreshWith({
		summoner, 
		fighter, 
		priest, 
		ranger, 
		sorceress, 
		(int)_mainUser->currentSkillPoints()
	});
}

bool UserSkillsManager::canIncreaseLevelForThis(LP_SKILL_RESOURCE_EX lpSkillResourceEx) {
	if (lpSkillResourceEx)
	{
		Effect* pEffect = _effectLayer->GetEffectInfo(lpSkillResourceEx->wId);
		auto m_byBitClassType = (BYTE)pow((float)2, (float)(_mainUser->m_wClass - 1));
		if (pEffect->bAbleClass & m_byBitClassType)
		{
			if (_mainUser->GetSkillLevel(lpSkillResourceEx->wId) < MAX_SKILL_LEVEL)
			{
				int		nMaxLevel = 0;
				BYTE	byMastery = _effectLayer->GetSkillMasteryKind(lpSkillResourceEx->wId);
				BYTE	byLevel = _mainUser->GetSkillLevel(byMastery);
				Effect* pEffect = _effectLayer->GetEffectInfo(lpSkillResourceEx->wId);
				Effect* pMasteryEffect = _effectLayer->GetEffectInfo(byMastery);

				if (byLevel != 0)
					nMaxLevel = pMasteryEffect->GetMaxMastery(byLevel - 1);

				if (nMaxLevel >= (int)pEffect->dwMinMastery) {
					return true;
				}
			}
		}
	}
	return false;
}

GenericSkillView::Model UserSkillsManager::buildModelFor(LP_SKILL_RESOURCE_EX skillResource)  {
	auto level = _mainUser->GetSkillLevel(skillResource->wId);
	auto sprite = [=]() -> SpriteModel {
		if ( level > 0) {
			return { skillResource->pSpr, { SKILL_ICON_SIZE, SKILL_ICON_SIZE } };
		} else {
			return { skillResource->pSprAct, { SKILL_ICON_SIZE, SKILL_ICON_SIZE } };
		}
	}();

	auto text = [=]() -> std::string {
		if (level == 0) { return ""; }
		return std::string("Lv: ") + std::to_string(level);
	}();

	auto increaseHandler = [=]() -> std::function<void()> {
		if (_mainUser->currentSkillPoints() > 0 && 
			canIncreaseLevelForThis(skillResource)) {
			return [=]() { 
				CTDS_SKILL_LEVELUP levelUpSkill;
				levelUpSkill.nSkillIndex = skillResource->wId;
				_network->send(levelUpSkill);
				_soundLibrary->playSkillPointUp();
			};
		} else {
			return nullptr;
		}
	}();

	return {
		sprite, text, increaseHandler
	};
}