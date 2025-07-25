#pragma once
#include "../NewUI/Label.h"
#include "../NewUI/Button.h"
#include "../NewUI/MatrixContainer.h"
namespace NewInterface {

	class CharacterAttributeView : public CustomUI::Renderable {
	public:
		struct Model {
			std::string name, value;
			std::function<void()> increaseAction;
		};

		CharacterAttributeView(CustomUI::Rect frameInParent, Model model);

	private:
		CustomUI::SingleLineLabel* _nameLabel;
		CustomUI::SingleLineLabel* _valueLabel;
		CustomUI::Button* _increaseButton;
	};

	class CharacterStatsView: public CustomUI::Renderable {
	public:
		using Model = CharacterAttributeView::Model;
		static float appropriateSizeForElementsCount(int count);
		CharacterStatsView(CustomUI::Rect frameInParent);

		void rebuildWithModels(const std::vector<Model>& models);
		void onClose(std::function<void()> handler);

	private:
		CustomUI::SingleLineLabel* _titleLabel;
		CustomUI::Button* _closeButton;
		CustomUI::MatrixContainer* _mc;
	};
}

