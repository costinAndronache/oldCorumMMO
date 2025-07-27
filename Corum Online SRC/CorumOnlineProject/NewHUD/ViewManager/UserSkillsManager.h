#pragma once

#include "../../InitGame.h"
#include "../UserSkillsView.h"
#include "../../NewHelpers/SharedNetwork.h"

namespace NewInterface {
	class UserSkillsManager {
	public:
		static int maxNumOfSkillsInList();

		UserSkillsManager(
			UserSkillsView* managedView, 
			CMainUser* mainUser,
			SharedNetwork* network,
			SSKILL_LIST_MANAGER* skillListManager,
			EffectLayer* effectLayer
		);

		void refreshUserSkillsView();

	private:
		UserSkillsView* _managedView;
		CMainUser* _mainUser;
		SSKILL_LIST_MANAGER* _skillListManager;
		SharedNetwork* _network;
		EffectLayer* _effectLayer;

		GenericSkillView::Model buildModelFor(LP_SKILL_RESOURCE_EX skillResource);
		bool canIncreaseLevelForThis(LP_SKILL_RESOURCE_EX skillResource);
	};
}


