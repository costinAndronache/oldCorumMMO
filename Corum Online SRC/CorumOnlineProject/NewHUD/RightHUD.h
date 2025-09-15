#pragma once
#include "LeftHUD.h"
#include "HUDSpriteCollection.h"
#include "GenericItemsContainerView.h"
#include "ViewManager/BeltManager.h"

namespace NewInterface {
	class RightHUD: public CustomUI::Renderable {
	public:
		struct EventHandlers {
			EventHandler group, lair, chat, rest, match, options;
		};
	public:
		RightHUD(
			CustomUI::Point originInParent,
			CItemResourceHash* resourceHash,
			std::shared_ptr<ItemUsageManager> itemUsageManager
		);


		void setEventHandlers(EventHandlers handlers);
		void updateSPScale(float scale);
		void updateCooldownScale(float scale);

		std::shared_ptr<BeltManager> beltDragNDropParticipant() { return _beltDragNDropParticipant; }

	private:
		EventHandlers _handlers;
		std::shared_ptr<CustomUI::HorizontalScalingBar> _spBar, _cooldownBar;
		std::shared_ptr<CustomUI::Button> _groupBtn, _lairBtn, _chatBtn,
		_restBtn, _matchBtn, _optionsBtn;

		std::shared_ptr<CustomUI::SpriteRenderable> _rightHUDSprite;

		std::shared_ptr<GenericItemsContainerView> _beltItemsView;
		std::shared_ptr<BeltManager> _beltDragNDropParticipant;
	};
}

