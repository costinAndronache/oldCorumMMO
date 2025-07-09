#pragma once
#include "LeftHUD.h"
#include "HUDSpriteCollection.h"
#include "BeltItemsView.h"

namespace NewInterface {
	class RightHUD: public CustomUI::Renderable {
	public:
		struct EventHandlers {
			EventHandler group, lair, chat, rest, match, options;
		};
	public:
		RightHUD(CustomUI::Point originInParent, const CItemResourceHash* resourceHash);


		void setEventHandlers(EventHandlers handlers);
		void updateSPScale(float scale);
		void updateCooldownScale(float scale);
		void updateWithItems(const CItem items[MAX_BELT_POOL]);

	private:
		EventHandlers _handlers;
		CustomUI::HorizontalScalingBar* _spBar;
		CustomUI::HorizontalScalingBar* _cooldownBar;
		CustomUI::Button* _groupBtn, * _lairBtn, * _chatBtn,
			* _restBtn, * _matchBtn, * _optionsBtn;

		CustomUI::SpriteRenderable* _rightHUDSprite;

		BeltItemsView* _beltItemsView;
	};
}

