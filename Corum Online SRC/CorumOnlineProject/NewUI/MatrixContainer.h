#pragma once
#include "Button.h"

namespace CustomUI {
	class MatrixContainer: public Renderable {
	public:
		enum class VerticalGrowthDirection {downwards, upwards};
		struct Sizes {
			Size itemSize;
			int columnsPerRow;
			int spacingX;
			int spacingY;
		};

		struct Appearance {
			VerticalGrowthDirection verticalGrowthDirection;
			Sizes sizes;
		};

		static Size appropriateSizeFor(Sizes sizes, int elementsCount);
		MatrixContainer(Rect frameInParent, Appearance appearance);

		template<typename Model>
		void rebuild(
			const std::vector<Model>& models,
			std::function<std::shared_ptr<Renderable> (Model model, int index, Rect frameInContainer)> createFn
		) {
			deconstructAllChildren();
			for (int i = 0; i < models.size(); i++) {
				const auto currentFrame = Rect{ originForIndex(i, _appearance.sizes.columnsPerRow), _appearance.sizes.itemSize };
				registerChildRenderable<Renderable>([=]() -> std::shared_ptr<Renderable> {
					return createFn(models[i], i, currentFrame);
				});
			}
		}
	private:
		Appearance _appearance;
		Point originForIndex(int index, int maxColumnsPerRow);
	};
}

