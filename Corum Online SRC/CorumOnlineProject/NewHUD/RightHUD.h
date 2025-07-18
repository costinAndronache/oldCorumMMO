#pragma once
#include "LeftHUD.h"
#include "HUDSpriteCollection.h"
#include "GenericItemsContainerView.h"
#include "DragNDrop/BeltDragNDropParticipant.h"

namespace NewInterface {
	class RightHUD: public CustomUI::Renderable {
	public:
		struct EventHandlers {
			EventHandler group, lair, chat, rest, match, options;
		};
	public:
		RightHUD(CustomUI::Point originInParent, CItemResourceHash* resourceHash);


		void setEventHandlers(EventHandlers handlers);
		void updateSPScale(float scale);
		void updateCooldownScale(float scale);

		BeltDragNDropParticipant* beltDragNDropParticipant() { return _beltDragNDropParticipant; }

	private:
		EventHandlers _handlers;
		CustomUI::HorizontalScalingBar* _spBar;
		CustomUI::HorizontalScalingBar* _cooldownBar;
		CustomUI::Button* _groupBtn, * _lairBtn, * _chatBtn,
			* _restBtn, * _matchBtn, * _optionsBtn;

		CustomUI::SpriteRenderable* _rightHUDSprite;

		GenericItemsContainerView* _beltItemsView;
		BeltDragNDropParticipant* _beltDragNDropParticipant;
	};
}

