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
			std::shared_ptr<UserSkillsView> managedView, 
			CMainUser* mainUser,
			std::shared_ptr<SharedNetwork> network,
			SSKILL_LIST_MANAGER* skillListManager,
			EffectLayer* effectLayer,
			SoundLibrary* soundLibrary,
			std::shared_ptr<TooltipHelper> tooltipHelper,
			std::shared_ptr<TooltipLayer> tooltipLayer
		);

		void refreshUserSkillsView();

	private:
		std::shared_ptr<UserSkillsView> _managedView;
		CMainUser* _mainUser;
		SSKILL_LIST_MANAGER* _skillListManager;
		std::shared_ptr<SharedNetwork> _network;
		EffectLayer* _effectLayer;
		SoundLibrary* _soundLibrary;
		std::shared_ptr<TooltipHelper> _tooltipHelper;
		std::shared_ptr<TooltipLayer> _tooltipLayer;
		std::shared_ptr<TooltipManager> _tooltipManager;


		bool canIncreaseLevelForThis(BYTE skillKind);
		GenericSkillView::Model buildModelFor(LP_SKILL_RESOURCE_EX skillResource, TooltipManager* tooltipManager);
	};
}


