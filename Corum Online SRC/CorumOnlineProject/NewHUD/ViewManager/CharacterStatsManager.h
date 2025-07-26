#pragma once

#include "../CharacterStatsView.h"
#include "../../InitGame.h"

namespace NewInterface {


	class CharacterStatsManager {
	public:
		enum class IncreasedAttribute {ego, str, intt, dex, vit};
		using AttributeIncreaseHandler = std::function<void(IncreasedAttribute)>;

		CharacterStatsManager(CharacterStatsView* managedView, CMainUser* mainUser);

		void refreshCharacterStats(AttributeIncreaseHandler handler);

		static int maxEntryCount();
	private:

		CharacterStatsView* _managedView;
		CMainUser* _mainUser;
	};
}

