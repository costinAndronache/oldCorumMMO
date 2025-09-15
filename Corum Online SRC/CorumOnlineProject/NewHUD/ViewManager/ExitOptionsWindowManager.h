#pragma once
#include "../ExitOptionsWindow.h"
#include "../../NewHelpers/GameExitManager.h"

namespace NewInterface {
	class ExitOptionsWindowManager {
	public:
		enum Mode { dungeon, worldMap};
		ExitOptionsWindowManager(
			std::shared_ptr<ExitOptionsWindow> managedView,
			std::shared_ptr<GameExitManager> exitManager,
			Mode mode
		);

	private:
		std::shared_ptr<GameExitManager> _exitManager;
		std::shared_ptr<ExitOptionsWindow> _managedView;
	};
}

