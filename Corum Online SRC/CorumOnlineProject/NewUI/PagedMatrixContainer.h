#pragma once

#include "MatrixContainer.h"
#include "PagedContainer.h"
#include "../Utils/Utils.h"

namespace CustomUI {
	class PagedMatrixContainer: public Renderable {
	public:
		struct Appearance {
			MatrixContainer::Appearance matrixContainerAppearance;
			int columnsPerRow;
		};

		static Size appropriateSizeFor(MatrixContainer::Sizes sizes, int elementsCountPerPage);
		PagedMatrixContainer(Rect frameInParent, Appearance appearance) {
			_frameInParent = frameInParent;
			_appearance = appearance;
		}


		template<typename Model>
		void rebuild(
			const std::vector<Model>& models,
			std::function<Renderable* (Model model, int modelIndex, Rect frameInParent)> itemCreateFn) {
			deconstructAllChildren();

			PagedContainer* pagedContainer = registerChildRenderable<PagedContainer>([=]() {
				return new PagedContainer(bounds());
			});

			std::vector<std::vector<Model>> pagedModelArray = split_vector< std::vector<Model> >(models, maxCountPerPage);

			pagedContainer->rebuildPages(pagedModelArray, [=](Rect frame, std::vector<Model> currentPageModelsArray, int currentPageIndex) {
				MatrixContainer* mc = new MatrixContainer(frame, _appearance.matrixContainerAppearance);

				mc->rebuild<Model>(currentPageModelsArray, _appearance.columnsPerRow, [=](Model model, int itemIndexInCurrentPage, Rect itemFrame) {
					auto itemIndexInOriginalModelsArray = currentPageIndex * maxCountPerPage + itemIndexInCurrentPage;

					return itemCreateFn(model, itemIndexInOriginalModelsArray, itemFrame);
				});

				return mc;
			});

			pagedContainer->setActivePage(1);
		}

	private:
		Appearance _appearance;
	};
}

