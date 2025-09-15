#include "SelectionView.h"
using namespace CustomUI;


SpriteModel SelectionViewResources::checkmarkSpriteModel = { NULL, {14, 16} };
void SelectionViewResources::initialize() {
	if (checkmarkSpriteModel.sprite == NULL) {
		checkmarkSpriteModel.sprite = g_pRenderer->CreateSpriteObject(
			GetFile("checkv1.tif", DATA_TYPE_UI),
			0, 0,
			checkmarkSpriteModel.naturalSize.width, checkmarkSpriteModel.naturalSize.height,
			0);
	}
}

SelectionView::SelectionView(Rect frameInParent, RenderableCreateFn renderableCreate): _isSelected(false) {
	SelectionViewResources::initialize();
	_frameInParent = frameInParent;
	const auto _bounds = bounds();

	_renderable = registerChildRenderable<Renderable>([=]() {
		return renderableCreate(_bounds);
	});

	Button::Sprites sprites{ SpriteModel::zero, SpriteModel::zero, SpriteModel::zero };

	_button = registerChildRenderable<Button>([=]() { 
		return std::make_shared<Button>(sprites, bounds());
	});

	_button->onClickEndLEFT([this]() {
		onButtonPressRelease(_button.get());
	});

}

void SelectionView::renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) {
	_renderable->renderWithRenderer(renderer, order);
	const auto frame = globalFrame();

	auto _checkmarkFrame = bounds().scaled(0.5, 0.5).centeredWith(frame);

	if (_isSelected) {
		SelectionViewResources::checkmarkSpriteModel.renderWith(renderer, _checkmarkFrame, order + 1);
	}
}

void SelectionView::setSelectionState(bool isSelected) {
	_isSelected = isSelected;
}

bool SelectionView::isSelected() {
	return _isSelected;
}

void SelectionView::setEnableSelection(bool selectionEnabled) {
	_selectionEnabled = selectionEnabled;
}

std::shared_ptr<Renderable> SelectionView::renderable() {
	return _renderable;
}

void SelectionView::onButtonPress(Button* button) {

}

void SelectionView::onButtonPressRelease(Button* button) {
	if (!_selectionEnabled) {
		return;
	}

	_isSelected = !_isSelected;
	if (_handler) { _handler(_isSelected); }

	_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_BTNCLICK, g_v3InterfaceSoundPos, FALSE);

}