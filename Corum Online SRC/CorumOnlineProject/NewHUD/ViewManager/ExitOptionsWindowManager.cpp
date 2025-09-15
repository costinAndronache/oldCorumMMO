#include "ExitOptionsWindowManager.h"

using namespace NewInterface;

ExitOptionsWindowManager::ExitOptionsWindowManager(
	std::shared_ptr<ExitOptionsWindow> managedView,
	std::shared_ptr<GameExitManager> exitManager,
	Mode mode
):	_managedView(managedView), 
	_exitManager(exitManager) {

	auto onCharSelectionOption = [=](){ exitManager->exitOnCharSelect(); };
	auto onWorldModeOption = [=](){ exitManager->tryExitOnWorldMap(); };
	auto onGameExit = [=](){ exitManager->exitGame(); };

	switch (mode) {
	case Mode::dungeon: {
		ExitOptionsWindow::DungeonModeHandlers dungeonHandlers;
		dungeonHandlers.onServerSelectionOption = onCharSelectionOption;
		dungeonHandlers.onGameExitOption = onGameExit;
		dungeonHandlers.onWorldModeOption = onWorldModeOption;

		managedView->rebuildInDungeonMode(dungeonHandlers);
	} break;
	case Mode::worldMap:
		managedView->rebuildInWorldMode({
			onGameExit, onCharSelectionOption
		});
		break;
	}
}