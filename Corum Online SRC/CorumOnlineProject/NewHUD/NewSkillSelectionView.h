#pragma once
#include "../NewUI/MatrixContainer.h"
#include "../NewUI/Button.h"

namespace NewInterface {
	class NewSkillSelectionView: public CustomUI::Renderable {
	public:
		typedef std::function<void(BYTE skillKind)> SkillSelectionHandler;

		enum class ActiveSkillSelection { none, leftSkills, rightSkills, guardianSkills };

		struct CurrentSkills {
			std::vector<BYTE> currentLeftSkills;
			std::vector<BYTE> currentRightSkills;
			std::vector<BYTE> currentGuardianSkills;
		};

		struct Handlers {
			SkillSelectionHandler onLeftSkillSelection;
			SkillSelectionHandler onRightSkillSelection;
			SkillSelectionHandler onGuardianSkillSelection;
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
		CustomUI::MatrixContainer* _leftSkillsContainer;
		CustomUI::MatrixContainer* _righttSkillsContainer;
		CustomUI::MatrixContainer* _guardianSkillsContainer;
	};
}

