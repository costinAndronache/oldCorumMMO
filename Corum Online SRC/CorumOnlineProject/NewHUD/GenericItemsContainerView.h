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
		using HandlerItemLongClickLEFT = std::function<void(CItem, CUISpriteModel, int index, CustomUI::Rect globalFrame)>;

		using HandlerItemClickRIGHT = std::function<void(CItem, int index)>;

		using Appearance = CustomUI::MatrixContainer::Appearance; 
	
		GenericItemsContainerView(CustomUI::Rect frameInParent, 
					  CItemResourceHash* resourceHash,
					  Appearance appearance);

		void updateWithItems(
			const std::vector<CItem>& items,
			CustomUI::SpriteModel itemUnderlaySprite,
			HandlerItemLongClickLEFT onLongPressItemLMB = nullptr,
			HandlerItemClickRIGHT onRightClick = nullptr
		);

		struct ItemWithUnderlay {
			CItem item;
			CustomUI::SpriteModel itemUnderlaySprite;
		};

		void updateWithItems(
			const std::vector<ItemWithUnderlay>& items,
			HandlerItemLongClickLEFT onLongPressItemLMB = nullptr,
			HandlerItemClickRIGHT onRightClick = nullptr
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


