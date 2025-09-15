#pragma once
#include "../NewSkillSelectionView.h"
#include "../../NewHelpers/UserPreferencesManager.h"
#include "../../NewHelpers/SoundLibrary.h"

namespace NewInterface {
	class SkillSelectionManager {
	public:
		typedef std::function<void()> SkillSelectionEvent;

	public:
		SkillSelectionManager(
			std::shared_ptr<NewSkillSelectionView> managedView, 
			CMainUser* user,
			std::shared_ptr<UserPreferencesManager> userPreferencesManager,
			SoundLibrary* soundLibrary
		);

		void refreshSkillSelectionView();
		void onSkillSelection(SkillSelectionEvent handler) { _skillSelectionHandler = handler; }
		bool trySkillSelectionViaHotkey(WPARAM, LPARAM);
	private:
		SkillSelectionEvent _skillSelectionHandler;
		std::shared_ptr<NewSkillSelectionView> _managedView;
		CMainUser* _user;
		std::shared_ptr<UserPreferencesManager> _userPreferencesManager;
		SoundLibrary* _soundLibrary;

		NewSkillSelectionView::CurrentSkills::SkillHotkeyMap currentSkillHotkeys();
		CorumPreferences::SkillClass currentUserSkillClass();
		void processKeyDownWhenHoveringSkill(WPARAM wparam, LPARAM lparam, BYTE skillKind);
	};
}


