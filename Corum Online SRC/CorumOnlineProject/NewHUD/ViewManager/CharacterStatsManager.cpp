#include "CharacterStatsManager.h"

using namespace NewInterface;

using IncreaseFn = CharacterStatsManager::AttributeIncreaseHandler;
using Model = CharacterStatsView::Model;
using ModelCreate = std::function<Model(CMainUser*, IncreaseFn)>;

using Attr = CharacterStatsManager::IncreasedAttribute;

static std::vector<ModelCreate> _entriesGenerators;
const std::vector<ModelCreate>& entriesGenerators();

int CharacterStatsManager::entryCount() {
	return entriesGenerators().size();
}

CharacterStatsManager::CharacterStatsManager(CharacterStatsView* managedView, CMainUser* mainUser) {
	_managedView = managedView;
	_mainUser = mainUser;
}

void CharacterStatsManager::refreshCharacterStats(AttributeIncreaseHandler handler) {
	std::vector<Model> models;
	auto generators = entriesGenerators();

	std::transform(
		std::begin(generators),
		std::end(generators),
		std::back_inserter(models),
		[=](auto genFn) -> Model {
			return genFn(_mainUser, handler);
	});

	_managedView->rebuildWithModels(models);
}

const std::vector<ModelCreate>& entriesGenerators() {
	if (!_entriesGenerators.empty()) { return _entriesGenerators; }

	auto putIncreaseFnIfStatPoints = [=](CMainUser* user, std::function<void()> fn) -> std::function<void()> {
		return user->currentStatPoints() ? fn : nullptr;
	};

	auto makePropertyModelCreate = [=](
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

	_entriesGenerators = {
		// name 
		[=](CMainUser* user, IncreaseFn inc) {
			return Model {
				std::string("Name:"),
				std::string(user->m_szName),
				nullptr
			};
		},

		// class
		[=](CMainUser* user, IncreaseFn inc) {
			return Model {
				std::string("Class / Job:"),
				std::string(user->m_szClassName),
				nullptr
			};
		},

		// group , guild?
		[=](CMainUser* user, IncreaseFn inc) {
			return Model {
				std::string("Group:"),
				std::string(user->m_szGroupName),
				nullptr
			};
		},

		// group , guild?
		[=](CMainUser* user, IncreaseFn inc) {
			return Model {
				std::string("Guild:"),
				std::string(user->m_szGuildName),
				nullptr
			};
		},

		// hp
		[=](CMainUser* user, IncreaseFn inc) {
			char hp[50] = { 0 };
			wsprintf(hp, "%d / %d", user->currentHP(), user->maxHP());
			return Model {
				std::string("HP:"),
				std::string(hp),
				nullptr
			};
		},

		// sp
		[=](CMainUser* user, IncreaseFn inc) {
			char sp[50] = { 0 };
			wsprintf(sp, "%d / %d", user->currentSP(), user->maxSP());
			return Model {
				std::string("SP:"),
				std::string(sp),
				nullptr
			};
		},

		// hp
		[=](CMainUser* user, IncreaseFn inc) {
			char lv[50] = { 0 };
			wsprintf(lv, "%d", user->currentLevel());
			return Model {
				std::string("Level:"),
				std::string(lv),
				nullptr
			};
		},

		// exp
		[=](CMainUser* user, IncreaseFn inc) {
			char exp[50] = { 0 };
			wsprintf(exp, "%d / %d",
				user->currentEXP(),
				GetExpTableOfLevel(OBJECT_TYPE_PLAYER, user->currentLevel() + 1)
			);
			return Model {
				std::string("EXP:"),
				std::string(exp),
				nullptr
			};
		},

		// EGO
		makePropertyModelCreate(
			[=](CMainUser* user) { return user->currentEGO(); },
			"EGO:",
			Attr::ego
		),

		// DEX
		makePropertyModelCreate(
			[=](CMainUser* user) { return user->currentDEX(); },
			"DEX:",
			Attr::dex
		),

		// STR
		makePropertyModelCreate(
			[=](CMainUser* user) { return user->currentSTR(); },
			"STR:",
			Attr::str
		),

		// INT
		makePropertyModelCreate(
			[=](CMainUser* user) { return user->currentINT(); },
			"INT:",
			Attr::intt
		),

		// VIT
		makePropertyModelCreate(
			[=](CMainUser* user) { return user->currentVIT(); },
			"VIT:",
			Attr::vit
		)
	};

	return _entriesGenerators;
}