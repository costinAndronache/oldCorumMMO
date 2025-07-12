#include "PagedMatrixContainer.h"

using namespace CustomUI;

Size PagedMatrixContainer::appropriateSizeFor(MatrixContainer::Sizes sizes, int elementsCountPerPage) {
	auto matrixContainerSize = MatrixContainer::appropriateSizeFor(sizes, elementsCountPerPage);
	return Size{
		matrixContainerSize.width,
		(float)PagedContainer::appropriateHeightFor(matrixContainerSize.height)
	};
}