#pragma once
#include "../MinimapWindow.h"

namespace NewInterface {
	class MinimapWindowManager {
	public:
		struct Layer {
			int layerID;
			float xSize, zSize;
			std::string title;
		};
	public:
		MinimapWindowManager(
			std::shared_ptr<MinimapWindow> managedWindow, 
			I4DyuchiGXRenderer* renderer
		);

		void updateForLayer(Layer layer);
		void updateMainPlayerDotFrom3DPosition(VECTOR3 pos);

	private:
		I4DyuchiGXRenderer* _renderer;
		std::shared_ptr<MinimapWindow> _minimapWindow;
		MinimapWindow::DotHandle _mainPlayerDot;
		float _currentSizeX, _currentSizeZ;

		CustomUI::SpriteModel spriteForLayer(int layerID);
	};
}


