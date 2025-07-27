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

CharacterStatsManager::CharacterStatsManager(
	CharacterStatsView* managedView,
	StatusPointManager* statusPointManager,
	CMainUser* mainUser,
	SoundLibrary* soundLibrary
) {
	_statusPointManager = statusPointManager;
	_managedView = managedView;
	_mainUser = mainUser;
	_soundLibrary = soundLibrary;
}

void CharacterStatsManager::refreshCharacterStats() {
	refreshCharacterStats([=](IncreasedAttribute attr) {
		_statusPointManager->increasePlayerStatusPoint(attr);
		_soundLibrary->playStatPointUp();
	});
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

	_managedView->rebuildWithModels(models, _mainUser->currentStatPoints());
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
			sprintf(value, "%d", propertyValueExtract(user));

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
			sprintf(value, "%d", propertyValueExtract(user));

			return Model{
				std::string(propertyName),
				std::string(value),
				nullptr
			};
		};
	};

	auto makeFloatCurrentMaxPropertyModelCreate = [=](
		std::function<float(CMainUser*)> propertyValueExtractCurrent,
		std::function<float(CMainUser*)> propertyValueExtractMax,
		const char* propertyName
		) -> ModelCreate {
		return [=](CMainUser* user, IncreaseFn inc) {
			char value[50] = { 0 };
			sprintf(value, "%.2f / %.2f",
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

	auto makeStringPropertyModelCreate = [=](
		std::function<char*(CMainUser*)> propertyValueExtract,
		const char* propertyName
		) -> ModelCreate {
		return [=](CMainUser* user, IncreaseFn inc) {
			char value[50] = { 0 };
			sprintf(value, "%s", propertyValueExtract(user));

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
			sprintf(value, "%d / %d", 
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
				return GetCumulatedExpByLevel(OBJECT_TYPE_PLAYER, user->currentLevel() + 1);
			},
			"EXP:"
		)
	};

	std::vector<ModelCreate> entriesPage2 = {
		makeIncreasablePropertyModelCreate(
			[=](CMainUser* user) { return user->currentEGO(); },
			"EGO:",
			Attr::STATUS_POINT_KIND_EGO
		),

		makeIncreasablePropertyModelCreate(
			[=](CMainUser* user) { return user->currentDEX(); },
			"DEX:",
			Attr::STATUS_POINT_KIND_DEX
		),

		makeIncreasablePropertyModelCreate(
			[=](CMainUser* user) { return user->currentSTR(); },
			"STR:",
			Attr::STATUS_POINT_KIND_STR
		),

		makeIncreasablePropertyModelCreate(
			[=](CMainUser* user) { return user->currentINT(); },
			"INT:",
			Attr::STATUS_POINT_KIND_INT
		),

		makeIncreasablePropertyModelCreate(
			[=](CMainUser* user) { return user->currentVIT(); },
			"VIT:",
			Attr::STATUS_POINT_KIND_VIT
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

		makeFloatCurrentMaxPropertyModelCreate(
			[=](CMainUser* user) { return user->currentCoolPoints();},
			[=](CMainUser* user) { return user->maxCoolPoints();},
			"Cool points:"
		),

		[=](CMainUser* user, IncreaseFn) -> Model {
			WORD dmgMin = 0, dmgMax = 0;
			user->GetAttackDamage_L(dmgMin, dmgMax);
			char value[50] = { 0 };
			sprintf(value, "%d <--> %d", dmgMin, dmgMax);
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
			sprintf(value, "%d <--> %d", dmgMin, dmgMax);
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