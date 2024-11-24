#include "SelectionView.h"
using namespace CustomUI;


SpriteModel SelectionViewResources::checkmarkSpriteModel = { NULL, {14, 16}, 0 };
void SelectionViewResources::initialize() {
	if (checkmarkSpriteModel.sprite == NULL) {
		checkmarkSpriteModel.sprite = g_pRenderer->CreateSpriteObject(
			GetFile("checkv1.tif", DATA_TYPE_UI),
			0, 0,
			checkmarkSpriteModel.size.width, checkmarkSpriteModel.size.height,
			0);
	}
}

SelectionView::SelectionView(Rect frame, Renderable* subView, SelectionViewClient* client):
	_frame(frame), _renderable(subView), _client(client), _isSelected(false) {
	SelectionViewResources::initialize();
	_button = new Button(SpriteModel::zero, SpriteModel::zero, frame, this);
	_checkmarkFrame = frame.scaled(0.5, 0.5).centeredWith(frame);
}

void SelectionView::renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) {
	_renderable->renderWithRenderer(renderer, order);

	_button->renderWithRenderer(renderer, order);

	if (_isSelected) {
		VECTOR2 scale = _checkmarkFrame.size.divideBy(SelectionViewResources::checkmarkSpriteModel.size);
		VECTOR2 pos = { _checkmarkFrame.origin.x, _checkmarkFrame.origin.y };
		renderer->RenderSprite(SelectionViewResources::checkmarkSpriteModel.sprite
			, &scale, 0.0f, &pos, NULL, 0xffffffff, order, RENDER_TYPE_DISABLE_TEX_FILTERING);
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

Renderable* SelectionView::renderable() {
	return _renderable;
}

void SelectionView::onButtonPress(Button* button) {

}

void SelectionView::onButtonPressRelease(Button* button) {
	if (!_selectionEnabled) {
		return;
	}

	_isSelected = !_isSelected;
	if (_client) { _client->selectionViewDidChangeSelectionState(this, _isSelected); }

	_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_BTNCLICK, g_v3InterfaceSoundPos, FALSE);

}