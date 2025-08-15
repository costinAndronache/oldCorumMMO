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
		DisplacementHandleRenderable* displacementHandle() const { return _displacementHandle; }

	private:
		CustomUI::MatrixContainer* _mc;
		CustomUI::Button* _closeButton;
		DisplacementHandleRenderable* _displacementHandle;

        struct Model {
          std::string text;
          ExitOptionsWindow::EventHandler handler;
        };
		void rebuildWithModels(const std::vector<Model>& models);
	};
}

