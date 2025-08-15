#pragma once

#include "../../InitGame.h"
#include "../UserSkillsView.h"
#include "../../NewHelpers/SharedNetwork.h"
#include "../../NewHelpers/SoundLibrary.h"
#include "TooltipManager.h"

namespace NewInterface {
	class UserSkillsManager {
	public:
		static int maxNumOfSkillsInList();

		UserSkillsManager(
			UserSkillsView* managedView, 
			CMainUser* mainUser,
			std::shared_ptr<SharedNetwork> network,
			SSKILL_LIST_MANAGER* skillListManager,
			EffectLayer* effectLayer,
			SoundLibrary* soundLibrary,
			TooltipHelper* tooltipHelper,
			TooltipLayer* tooltipLayer
		);

		void refreshUserSkillsView();

	private:
		UserSkillsView* _managedView;
		CMainUser* _mainUser;
		SSKILL_LIST_MANAGER* _skillListManager;
		std::shared_ptr<SharedNetwork> _network;
		EffectLayer* _effectLayer;
		SoundLibrary* _soundLibrary;
		TooltipHelper* _tooltipHelper;
		TooltipLayer* _tooltipLayer;
		TooltipManager* _tooltipManager;


		bool canIncreaseLevelForThis(BYTE skillKind);
		GenericSkillView::Model buildModelFor(LP_SKILL_RESOURCE_EX skillResource, TooltipManager* tooltipManager);
	};
}


