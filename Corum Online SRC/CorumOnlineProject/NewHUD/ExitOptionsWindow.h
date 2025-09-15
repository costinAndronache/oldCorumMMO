#pragma once

#include "../NewUI/Button.h"
#include "../NewUI/MatrixContainer.h"
#include "DisplacementHandleRenderable.h"

namespace NewInterface {
	class ExitOptionsWindow: public CustomUI::Renderable {
	public:
		using EventHandler = std::function<void()>;
		struct WorldModeHandlers {
			EventHandler onGameExitOption;
			EventHandler onServerSelectionOption;
		};

		struct DungeonModeHandlers: public WorldModeHandlers {
			EventHandler onWorldModeOption;
		};

	public:
		ExitOptionsWindow(CustomUI::Point originInParent);

		void rebuildInDungeonMode(DungeonModeHandlers handlers);
		void rebuildInWorldMode(WorldModeHandlers handlers);
		void onClose(EventHandler handler);
		std::shared_ptr<DisplacementHandleRenderable> displacementHandle() const { return _displacementHandle; }

	private:
		std::shared_ptr<CustomUI::MatrixContainer> _mc;
		std::shared_ptr<CustomUI::Button> _closeButton;
		std::shared_ptr<DisplacementHandleRenderable> _displacementHandle;

        struct Model {
          std::string text;
          ExitOptionsWindow::EventHandler handler;
        };
		void rebuildWithModels(const std::vector<Model>& models);
	};
}

