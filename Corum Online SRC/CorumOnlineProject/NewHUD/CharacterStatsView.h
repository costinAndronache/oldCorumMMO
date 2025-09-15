#pragma once
#include "../NewUI/Label.h"
#include "../NewUI/Button.h"
#include "../NewUI/MatrixContainer.h"
#include "../NewUI/PagedContainer.h"
#include "DisplacementHandleRenderable.h"

namespace NewInterface {
	class CharacterAttributeView : public CustomUI::Renderable {
	public:
		struct Model {
			std::string name, value;
			std::function<void()> increaseAction;
		};

		class UpdateProxy {
		public:
			friend class CharacterAttributeView;
			UpdateProxy(): _update(nullptr) { }

			void update(Model model) { if(_update) { _update(model); }}

        private:
			std::function<void(Model model)> _update;
		};

		CharacterAttributeView(CustomUI::Rect frameInParent, UpdateProxy *updateProxy);

	private:
		UpdateProxy* _updateProxy;
		std::shared_ptr<CustomUI::SingleLineLabel> _nameLabel;
		std::shared_ptr<CustomUI::SingleLineLabel> _valueLabel;
		std::shared_ptr<CustomUI::Button> _increaseButton;
	};

	class CharacterStatsView: public CustomUI::Renderable {
	public:
		using UpdateProxy = CharacterAttributeView::UpdateProxy;
		static float appropriateSizeForElementsCountOnPage(int count);
		CharacterStatsView(CustomUI::Rect frameInParent);

		void rebuildWithProxies( const std::vector< std::vector<UpdateProxy*> >& pageModels);
		void updateAvailableStatPointsCount(int availableStatPoints);
		void onClose(std::function<void()> handler);

		std::shared_ptr<DisplacementHandleRenderable> displacementHandle() { return _displacementHandle;}
	private:
		std::shared_ptr<DisplacementHandleRenderable> _displacementHandle;
		std::shared_ptr<CustomUI::SingleLineLabel> _titleLabel;
		std::shared_ptr<CustomUI::Button> _closeButton;
		std::shared_ptr<CustomUI::PagedContainer> _container;
	};
}

