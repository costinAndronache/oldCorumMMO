#pragma once
#include "../NewUI/MatrixContainer.h"
#include "../NewUI/Button.h"

namespace NewInterface {
	class NewSkillSelectionView: public CustomUI::Renderable {
	public:
		typedef std::function<void(BYTE skillKind)> SkillSelectionHandler;
		typedef std::function<void(WPARAM, LPARAM, BYTE)> KeyDownEventForSkill;

		enum class ActiveSkillSelection { none, leftSkills, rightSkills, guardianSkills };

		struct CurrentSkills {
			typedef std::map<BYTE, std::string> SkillHotkeyMap;
			std::vector<BYTE> currentLeftSkills;
			std::vector<BYTE> currentRightSkills;
			std::vector<BYTE> currentGuardianSkills;
			SkillHotkeyMap skillHotkeyMap;
		};

		struct Handlers {
			SkillSelectionHandler onLeftSkillSelection;
			SkillSelectionHandler onRightSkillSelection;
			SkillSelectionHandler onGuardianSkillSelection;
			KeyDownEventForSkill onKeyDownWhenHoveringSkill;
		};

		NewSkillSelectionView(CustomUI::Rect frameInParent,
			CustomUI::MatrixContainer::VerticalGrowthDirection direction,
			SSKILL_LIST_MANAGER* skillListManager);

		void setHandlers(Handlers handlers) { _handlers = handlers; }
		void updateCurrentSkills(CurrentSkills cs);
		void switchToActiveSelection(ActiveSkillSelection selection);
	private:
		CurrentSkills _currentSkills;
		Handlers _handlers;
		ActiveSkillSelection _activeSkillSelection;
		SSKILL_LIST_MANAGER* _skillListManager;
		std::shared_ptr<CustomUI::MatrixContainer> _leftSkillsContainer;
		std::shared_ptr<CustomUI::MatrixContainer> _righttSkillsContainer;
		std::shared_ptr<CustomUI::MatrixContainer> _guardianSkillsContainer;
	};
}

