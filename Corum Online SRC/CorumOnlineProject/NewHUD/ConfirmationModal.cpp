#include "ConfirmationModal.h"
#include "HUDSpriteCollection.h"
using namespace CustomUI;
using namespace NewInterface;

static Size confirmButtonSize = {100, 40};

ConfirmationModal::ConfirmationModal(CustomUI::Rect frameInParent) {
	_frameInParent = frameInParent;

	auto confirmButtonFrame = bounds()
		.fromMaxYOrigin(-confirmButtonSize.height)
		.fromMaxXOrigin(-confirmButtonSize.width)
		.withSize(confirmButtonSize);

	_confirmButton = registerChildRenderable<Button>([=](){
		return std::make_shared<Button>(
			NewHUDResources::genericButtonBackgroundSprites,
			Button::LabelModel("Confirm"),
			confirmButtonFrame
		);
	});

	auto messageLabelFrame = bounds()
		.withHeightOffset(-confirmButtonSize.height);

	_messageLabel = registerChildRenderable<SingleLineLabel>([=](){
		return std::make_shared<SingleLineLabel>(
			messageLabelFrame,
			SingleLineLabel::Appearance::defaultAppearance(),
			""
		);
	});

	updateBackground(NewHUDResources::genericBackgroundSprite);
} 

void ConfirmationModal::updateMessageTo(std::string message) {
	_messageLabel->updateTextTo(message);
}

void ConfirmationModal::onConfirmation(std::function<void()> handler) {
	_confirmButton->onClickEndLEFT(handler);
}