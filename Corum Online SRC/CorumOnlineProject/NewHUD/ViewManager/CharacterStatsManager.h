#pragma once

#include "../CharacterStatsView.h"
#include "../../InitGame.h"
#include "../../NewHelpers/StatusPointManager.h"
#include "../../NewHelpers/SoundLibrary.h"
namespace NewInterface {


	class CharacterStatsManager {
	public:
		using IncreasedAttribute = STATUS_POINT_KIND;
		using AttributeIncreaseHandler = std::function<void(IncreasedAttribute)>;

		CharacterStatsManager(
			CharacterStatsView* managedView,
			StatusPointManager* _statusPointManager,
			CMainUser* mainUser,
			SoundLibrary* soundLibrary
		);

		void refreshCharacterStats();
		static int maxEntryCount();
	private:
		StatusPointManager* _statusPointManager;
		CharacterStatsView* _managedView;
		CMainUser* _mainUser;
		SoundLibrary* _soundLibrary;
		void refreshCharacterStats(AttributeIncreaseHandler handler);
	};
}

