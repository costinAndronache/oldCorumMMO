#pragma once
#include "LeftHUD.h"
#include "../NewUI/MatrixContainer.h"
#include <vector>

namespace NewInterface {
	class GenericItemView : public CustomUI::Renderable {
	public:
		GenericItemView(CustomUI::Rect frameInParent, CustomUI::SpriteModel underlay);

	public:
		CustomUI::Button* _button;
		CustomUI::SingleLineLabel* _instanceCountLabel;
	};

	class GenericItemsContainerView: public CustomUI::Renderable {
	public:
		struct Appearance {
			CustomUI::Size itemSize;
			int spacing;
			int itemsPerRow;
			CustomUI::SpriteModel itemUnderlay;
		};
		GenericItemsContainerView(CustomUI::Rect frameInParent, 
					  CItemResourceHash* resourceHash,
					  Appearance appearance);

		void updateWithItems(const std::vector<CItem>& items);
	private:
		CItemResourceHash* _resourceHash;
		std::vector<GenericItemView*> _itemViews;
		CustomUI::MatrixContainer* _matrixContainer;
		Appearance _appearance;
	};
}


