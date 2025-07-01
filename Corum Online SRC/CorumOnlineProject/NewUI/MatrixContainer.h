#pragma once
#include "Button.h"

namespace CustomUI {
	class MatrixContainer: public Renderable {
	public:
		enum class VerticalGrowthDirection {downwards, upwards};

		MatrixContainer(Point growthOrigin, VerticalGrowthDirection direction, Size itemSize);

		template<typename Model>
		void rebuild(
			const std::vector<Model>& models,
			int maxColumnsPerRow,
			std::function<Renderable* (Model model, Rect frameInContainer)> createFn
		) {
			deconstructAllChildren();
			for (int i = 0; i < models.size(); i++) {
				const auto currentFrame = Rect{ originForIndex(i, maxColumnsPerRow), _itemSize };
				registerChildRenderable<Renderable>([=]() {
					createFn(model, currentFrame);
				});
			}

			_frameInParent = boundingBoxInParent();
		}

	private:
		VerticalGrowthDirection _direction;
		Point _growthOrigin;
		Size _itemSize;
		
		Point originForIndex(int index, int maxColumnsPerRow);
	};
}

