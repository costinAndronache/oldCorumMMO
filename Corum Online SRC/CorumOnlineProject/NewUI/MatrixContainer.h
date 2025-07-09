#pragma once
#include "Button.h"

namespace CustomUI {
	class MatrixContainer: public Renderable {
	public:
		enum class VerticalGrowthDirection {downwards, upwards};

		MatrixContainer(Rect frameInParent, VerticalGrowthDirection direction, Size itemSize, int spacing = 0);

		template<typename Model>
		void rebuild(
			const std::vector<Model>& models,
			int maxColumnsPerRow,
			std::function<Renderable* (Model model, Rect frameInContainer)> createFn
		) {
			deconstructAllChildren();
			for (int i = 0; i < models.size(); i++) {
				const auto currentFrame = Rect{ originForIndex(i, maxColumnsPerRow), _itemSize };
				registerChildRenderable<Renderable>([=]() -> Renderable* {
					return createFn(models[i], currentFrame);
				});
			}
		}

		virtual void renderWithRenderer(I4DyuchiGXRenderer* renderer, int zIndex) override {
			Renderable::renderWithRenderer(renderer, zIndex);
			const auto gFrame = globalFrame();
		}

	private:
		VerticalGrowthDirection _direction;
		Size _itemSize;
		int _spacing;
		Point originForIndex(int index, int maxColumnsPerRow);
	};
}

