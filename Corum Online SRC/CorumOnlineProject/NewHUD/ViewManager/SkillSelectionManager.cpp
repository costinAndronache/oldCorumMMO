#include "SkillSelectionManager.h"

using namespace NewInterface;
using namespace CorumPreferences;

static SkillHotkey hotkeyFrom(WPARAM wparam);

SkillSelectionManager::SkillSelectionManager(
	std::shared_ptr<NewSkillSelectionView> managedView, 
	CMainUser* user,
	std::shared_ptr<UserPreferencesManager> userPreferencesManager,
	SoundLibrary* soundLibrary
) {
	_managedView = managedView;
	_user = user;
	_userPreferencesManager = userPreferencesManager;
	_soundLibrary = soundLibrary;

	std::function<void(BYTE skillID, int LR)> onSkillSelection = [=](BYTE skillID, int LR) {
		_managedView->switchToActiveSelection(NewSkillSelectionView::ActiveSkillSelection::none);
		_user->SetSkillChangeLR(skillID, LR);
		_soundLibrary->playButtonClick();
		if(_skillSelectionHandler) { _skillSelectionHandler(); }
	};
	_managedView->setHandlers({
		[=](BYTE leftSkillKind) {
		onSkillSelection(leftSkillKind, 0);
	},
		[=](BYTE rightSkillKind) {
		onSkillSelection(rightSkillKind, 1);
	},
		[=](BYTE guardianSkillKind) {
		// not yet
		_managedView->switchToActiveSelection(NewSkillSelectionView::ActiveSkillSelection::none);
		if(_skillSelectionHandler) { _skillSelectionHandler(); };
	},
		[=](WPARAM wparam, LPARAM lparam, BYTE skillKind) {
		processKeyDownWhenHoveringSkill(wparam, lparam, skillKind);
	}
		});
}

void SkillSelectionManager::refreshSkillSelectionView() {
	_managedView->updateCurrentSkills({
		_user->skillsAvailableOnLeft(),
		_user->skillsAvailableOnRight(),
		std::vector<BYTE>(),
		currentSkillHotkeys()
	});
}

bool SkillSelectionManager::trySkillSelectionViaHotkey(WPARAM wparam, LPARAM lparam) {
	const auto hotkey = hotkeyFrom(wparam);
	if(hotkey == SkillHotkey::SkillHotkey_unknown) { return false; }

	auto skillID = _userPreferencesManager->currentPreferences()->skillIDForHotkey(hotkey, currentUserSkillClass());
	if(0 > skillID) { return false; }

	_user->SetSkillChangeLR((BYTE)skillID, 1);
	_soundLibrary->playButtonClick();

	return true; 
}

void SkillSelectionManager::processKeyDownWhenHoveringSkill(WPARAM wparam, LPARAM lparam, BYTE skillKind) {
	auto hotkey = hotkeyFrom(wparam);
	if(hotkey == SkillHotkey_unknown) { return; }

	_userPreferencesManager->persistChanges([=](auto prefs) {
		prefs->storeHotkeyForSkill(hotkey, skillKind, currentUserSkillClass());
	});
	
	refreshSkillSelectionView();
}

CorumPreferences::SkillClass SkillSelectionManager::currentUserSkillClass() {
	SkillClass skillClass = [=](){
		switch(_user->m_wClass) {
		case CLASS_TYPE_WARRIOR: return SkillClass::SkillClass_fighter;
		case CLASS_TYPE_SUMMONER: return SkillClass::SkillClass_summoner;
		case CLASS_TYPE_PRIEST: return SkillClass::SkillClass_priest;
		case CLASS_TYPE_HUNTER: return SkillClass::SkillClass_ranger;
		case CLASS_TYPE_WIZARD: return SkillClass::SkillClass_sorceress;
		default: return SkillClass::SkillClass_unknown;
		}
	}();
	return skillClass;
}

NewSkillSelectionView::CurrentSkills::SkillHotkeyMap SkillSelectionManager::currentSkillHotkeys() {
	auto fromPreferences = _userPreferencesManager->currentPreferences()->currentSkillHotkeyEntries(currentUserSkillClass());
	NewSkillSelectionView::CurrentSkills::SkillHotkeyMap result{};

	for(const auto& preferencesEntry: fromPreferences) {
		result.insert({
			(BYTE)preferencesEntry.second,
			stringFor(preferencesEntry.first)
		});
	}

	return result;
}

static SkillHotkey hotkeyFrom(WPARAM wparam) {
	switch(wparam) {
	case 'Q': return SkillHotkey_Q;
	case 'W': return SkillHotkey_W;
	case 'E': return SkillHotkey_E;
	case 'R': return SkillHotkey_R;
	case 'Z': return SkillHotkey_Z;
	case 'X': return SkillHotkey_X;
	case 'C': return SkillHotkey_C;
	case 'V': return SkillHotkey_V;
	case 'B': return SkillHotkey_B;
	case VK_F1: return SkillHotkey_F1;
	case VK_F2: return SkillHotkey_F2;
	case VK_F3: return SkillHotkey_F3;
	case VK_F4: return SkillHotkey_F4;
	case VK_F5: return SkillHotkey_F5;
	default: return SkillHotkey_unknown;
	}
}