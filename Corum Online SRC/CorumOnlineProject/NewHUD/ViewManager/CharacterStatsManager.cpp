#include "CharacterStatsManager.h"

using namespace NewInterface;

using IncreaseFn = CharacterStatsManager::AttributeIncreaseHandler;
using Model = CharacterStatsView::Model;
using ModelCreate = std::function<Model(CMainUser*, IncreaseFn)>;

using Attr = CharacterStatsManager::IncreasedAttribute;

static std::vector<std::vector<ModelCreate>> _entriesGenerators;
const std::vector<std::vector<ModelCreate>>& entriesGenerators();

int CharacterStatsManager::maxEntryCount() {
	auto generators = entriesGenerators();
	std::vector<std::size_t> sizes;

	std::transform(
		std::begin(generators),
		std::end(generators),
		std::back_inserter(sizes),
		[=](const auto& vec) { return vec.size();}
	);

	return *std::max_element(std::begin(sizes), std::end(sizes));
}

CharacterStatsManager::CharacterStatsManager(CharacterStatsView* managedView, CMainUser* mainUser) {
	_managedView = managedView;
	_mainUser = mainUser;
}

void CharacterStatsManager::refreshCharacterStats(AttributeIncreaseHandler handler) {
	std::vector<std::vector<Model>> models;
	auto generators = entriesGenerators();

	std::transform(
		std::begin(generators),
		std::end(generators),
		std::back_inserter(models),
		[=](std::vector<ModelCreate> genFnArray) -> std::vector<Model> {
			std::vector<Model> currentModels;
			std::transform(
				std::begin(genFnArray),
				std::end(genFnArray),
				std::back_inserter(currentModels),
				[=](ModelCreate genFn) {
					return genFn(_mainUser, handler);
				}
			);
			return currentModels;
		}
	);

	_managedView->rebuildWithModels(models);
}

const std::vector<std::vector<ModelCreate>>& entriesGenerators() {
	if (!_entriesGenerators.empty()) { return _entriesGenerators; }

	auto putIncreaseFnIfStatPoints = [=](CMainUser* user, std::function<void()> fn) -> std::function<void()> {
		return user->currentStatPoints() ? fn : nullptr;
	};

	auto makeIncreasablePropertyModelCreate = [=](
		std::function<int(CMainUser*)> propertyValueExtract,
		const char* propertyName,
		Attr attribute
	) -> ModelCreate {
		return [=](CMainUser* user, IncreaseFn inc) {
			char value[50] = { 0 };
			wsprintf(value, "%d", propertyValueExtract(user));

			return Model{
				std::string(propertyName),
				std::string(value),
				putIncreaseFnIfStatPoints(
					user, [=]() { inc(attribute); }
				)
			};
		};
	};

	auto makeIntegerPropertyModelCreate = [=](
		std::function<int(CMainUser*)> propertyValueExtract,
		const char* propertyName
	) -> ModelCreate {
		return [=](CMainUser* user, IncreaseFn inc) {
			char value[50] = { 0 };
			wsprintf(value, "%d", propertyValueExtract(user));

			return Model{
				std::string(propertyName),
				std::string(value),
				nullptr
			};
		};
	};

	auto makeStringPropertyModelCreate = [=](
		std::function<char*(CMainUser*)> propertyValueExtract,
		const char* propertyName
		) -> ModelCreate {
		return [=](CMainUser* user, IncreaseFn inc) {
			char value[50] = { 0 };
			wsprintf(value, "%s", propertyValueExtract(user));

			return Model{
				std::string(propertyName),
				std::string(value),
				nullptr
			};
		};
	};
	
	auto makeIntegerCurrentMaxPropertyModelCreate = [=](
		std::function<int(CMainUser*)> propertyValueExtractCurrent,
		std::function<int(CMainUser*)> propertyValueExtractMax,
		const char* propertyName
		) -> ModelCreate {
		return [=](CMainUser* user, IncreaseFn inc) {
			char value[50] = { 0 };
			wsprintf(value, "%d / %d", 
				propertyValueExtractCurrent(user),
				propertyValueExtractMax(user)
			);

			return Model{
				std::string(propertyName),
				std::string(value),
				nullptr
			};
		};
	};

	std::vector<ModelCreate> entriesPage1 = {
		makeStringPropertyModelCreate(
			[=](CMainUser* user) { return user->m_szName; },
			"Name:"
		),

		makeStringPropertyModelCreate(
			[=](CMainUser* user) { return user->m_szClassName; },
			"Class / Job:"
		),

		makeStringPropertyModelCreate(
			[=](CMainUser* user) { return user->m_szClassName; },
			"Class / Job:"
		),

		makeStringPropertyModelCreate(
			[=](CMainUser* user) { return user->m_szGroupName; },
			"Group:"
		),

		makeStringPropertyModelCreate(
			[=](CMainUser* user) { return user->m_szGuildName; },
			"Guild:"
		),

		makeIntegerPropertyModelCreate(
			[=](CMainUser* user) { return user->currentLevel();},
			"Level:"
		),

		makeIntegerPropertyModelCreate(
			[=](CMainUser* user) { return user->m_wCriminalTime;},
			"Criminal time:"
		),

		makeIntegerCurrentMaxPropertyModelCreate(
			[=](CMainUser* user) { return user->currentEXP();},
			[=](CMainUser* user) {
				return GetExpTableOfLevel(OBJECT_TYPE_PLAYER, user->currentLevel() + 1);
			},
			"EXP:"
		)
	};

	std::vector<ModelCreate> entriesPage2 = {
		makeIncreasablePropertyModelCreate(
			[=](CMainUser* user) { return user->currentEGO(); },
			"EGO:",
			Attr::ego
		),

		makeIncreasablePropertyModelCreate(
			[=](CMainUser* user) { return user->currentDEX(); },
			"DEX:",
			Attr::dex
		),

		makeIncreasablePropertyModelCreate(
			[=](CMainUser* user) { return user->currentSTR(); },
			"STR:",
			Attr::str
		),

		makeIncreasablePropertyModelCreate(
			[=](CMainUser* user) { return user->currentINT(); },
			"INT:",
			Attr::intt
		),

		makeIncreasablePropertyModelCreate(
			[=](CMainUser* user) { return user->currentVIT(); },
			"VIT:",
			Attr::vit
		),

		makeIntegerCurrentMaxPropertyModelCreate(
			[=](CMainUser* user) { return user->currentHP();},
			[=](CMainUser* user) { return user->maxHP();},
			"HP:"
		),

		makeIntegerCurrentMaxPropertyModelCreate(
			[=](CMainUser* user) { return user->currentSP();},
			[=](CMainUser* user) { return user->maxSP();},
			"SP:"
		),

		[=](CMainUser* user, IncreaseFn) -> Model {
			WORD dmgMin = 0, dmgMax = 0;
			user->GetAttackDamage_L(dmgMin, dmgMax);
			char value[50] = { 0 };
			wsprintf(value, "%d ~~~> %d", dmgMin, dmgMax);
			return Model{
				"L Damage:",
				value,
				nullptr
			};
		},

		[=](CMainUser* user, IncreaseFn) -> Model {
			WORD dmgMin = 0, dmgMax = 0;
			user->GetAttackDamage_R(dmgMin, dmgMax);
			char value[50] = { 0 };
			wsprintf(value, "%d ~~~> %d", dmgMin, dmgMax);
			return Model{
				"R Damage:",
				value,
				nullptr
			};
		},

		makeIntegerPropertyModelCreate(
			[=](CMainUser* user) { return user->m_wAttackAcuracy;},
			"ACCU :"
		),

		makeIntegerPropertyModelCreate(
			[=](CMainUser* user) { return user->m_wAvoid;},
			"AVOID :"
		),

		makeIntegerPropertyModelCreate(
			[=](CMainUser* user) { return user->GetPhyResist();},
			"PHYS resist% :"
		),

		makeIntegerPropertyModelCreate(
			[=](CMainUser* user) { return user->m_wFireResist;},
			"FIRE resist% :"
		),

		makeIntegerPropertyModelCreate(
			[=](CMainUser* user) { return user->m_wIceResist;},
			"ICE resist% :"
		),

		makeIntegerPropertyModelCreate(
			[=](CMainUser* user) { return user->m_wLightResist;},
			"LIGHT resist% :"
		),

		makeIntegerPropertyModelCreate(
			[=](CMainUser* user) { return user->m_wPoiResist;},
			"POISON resist% :"
		)
	};

	_entriesGenerators = { entriesPage2, entriesPage1 };
	return _entriesGenerators;
}