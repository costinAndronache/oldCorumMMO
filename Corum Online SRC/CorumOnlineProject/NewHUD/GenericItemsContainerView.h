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
		using ItemLongPressHandlerLMB = std::function<void(CItem, CUISpriteModel, int index, CustomUI::Rect globalFrame)>;

		struct Appearance {
			CustomUI::MatrixContainer::Appearance containerAppearance;
			CustomUI::SpriteModel itemUnderlay;
		};
		GenericItemsContainerView(CustomUI::Rect frameInParent, 
					  CItemResourceHash* resourceHash,
					  Appearance appearance);

		void updateWithItems(
			const std::vector<CItem>& items, 
			ItemLongPressHandlerLMB onLongPressItemLMB = nullptr
		);

		int itemIndexForGlobalPoint(CustomUI::Point p); // -1 on not found
		void setHiddenStateForItemAtIndex(int index, bool isHidden);


	private:
		CItemResourceHash* _resourceHash;
		std::vector<GenericItemView*> _itemViews;
		CustomUI::MatrixContainer* _matrixContainer;
		Appearance _appearance;

	};
}


