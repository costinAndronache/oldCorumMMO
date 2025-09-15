#pragma once
#include "../NewUI/Button.h"
#include "HUDSpriteCollection.h"

namespace NewInterface {
	class ConfirmationModal: public CustomUI::Renderable {
	public:
		ConfirmationModal(CustomUI::Rect frameInParent);

		void updateMessageTo(std::string message);
		void onConfirmation(std::function<void()> handler);

	private:
		std::shared_ptr<CustomUI::SingleLineLabel> _messageLabel;
		std::shared_ptr<CustomUI::Button> _confirmButton;
	};
}


