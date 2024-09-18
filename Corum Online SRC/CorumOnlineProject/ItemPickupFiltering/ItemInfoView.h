#pragma once
#include "../InitGame.h"
#include "CustomUIBase.h"
#include <vector>
#include <string>

namespace CustomUI {
	class ItemInfoView: public Renderable {
	public:
		struct Model {
			CItem* item;
			Size itemResourceSize;
		};
		ItemInfoView(Model model, Rect frame, SpriteModel backgroundSpriteModel);
		void updateModel(Model newModel);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) override;
		bool renderInfoIfMouseHover();
	private:
		const Rect _rect;
		Model _model;
		SpriteModel _backgroundSpriteModel;
	};

	struct ItemInfoViewResources {
		static void initialize();
		static SpriteModel bgSpriteModel;
		static SpriteModel unknownSpriteModel;
	};
}


