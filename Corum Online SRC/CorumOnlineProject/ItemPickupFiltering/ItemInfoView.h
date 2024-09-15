#pragma once
#include "../InitGame.h"
#include "CustomUIBase.h"
#include <vector>
#include <string>

namespace CustomUI {
	class ItemInfoView {
	public:
		struct Model {
			CItem* item;
			Size itemResourceSize;
		};
		ItemInfoView(Model model, Rect frame);
		void updateModel(Model newModel);
		void renderImageWithRenderer(I4DyuchiGXRenderer* renderer, int order);
		bool renderInfoIfMouseHover();
		static void setupBackgroundSprite(IDISpriteObject* bgSprite, Size bgSpriteSize);
	private:
		const Rect _rect;
		Model _model;
		static IDISpriteObject* backgroundSprite;
		static Size bgSpriteSize;
	};
}


