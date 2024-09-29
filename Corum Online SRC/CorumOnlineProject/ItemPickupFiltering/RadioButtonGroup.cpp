#include "RadioButtonGroup.h"

using namespace CustomUI;

typedef ToggleButton* (RadioButtonGroup::*ToggleButtonProducer)(void*, int, Rect);

static void fillButtons(
	std::vector<ToggleButton*>& buttonList, int count,
	Point origin, Size buttonSize, int xStep, int yStep,
	void* context, ToggleButtonProducer toggleButtonProducer, RadioButtonGroup* object) {
	for (int i = 0; i < count; i++) {
		Rect btnFrame = { { origin.x + i * xStep, origin.y + i * yStep }, buttonSize };

		ToggleButton* tgb = (object->*toggleButtonProducer)(context, i, btnFrame);
		buttonList.push_back(tgb);
	}
}

RadioButtonGroup::RadioButtonGroup(std::vector<ButtonModel> models, Rect frame, unsigned int activeButtonIndex, RadioButtonGroupClient* client):
_frame(frame), _client(client), _activeButtonIndex(activeButtonIndex) {
	_buttons.reserve(models.size());
	if (frame.size.width > frame.size.height) {
		Size size = { frame.size.width / models.size(), frame.size.height };
		fillButtons(_buttons, models.size(), frame.origin, size, size.width, 0, (void*)&models, &RadioButtonGroup::buildFromModelList, this);
	}
	else {
		Size size = { frame.size.width, frame.size.height / models.size() };
		fillButtons(_buttons, models.size(), frame.origin, size, 0, size.height, (void*)&models, &RadioButtonGroup::buildFromModelList, this);
	}
	adjustButtons(_activeButtonIndex);
}

RadioButtonGroup::RadioButtonGroup(std::vector<LabeledButtonModel> models, Rect frame, unsigned int activeButtonIndex, RadioButtonGroupClient* client):
	_frame(frame), _client(client), _activeButtonIndex(activeButtonIndex) {
	_buttons.reserve(models.size());
	if (frame.size.width > frame.size.height) {
		Size size = { frame.size.width / models.size(), frame.size.height };
		fillButtons(_buttons, models.size(), frame.origin, size, size.width, 0, (void*)&models, &RadioButtonGroup::buildFromLabeledModelList, this);
	}
	else {
		Size size = { frame.size.width, frame.size.height / models.size() };
		fillButtons(_buttons, models.size(), frame.origin, size, 0, size.height, (void*)&models, &RadioButtonGroup::buildFromLabeledModelList, this);
	}
	adjustButtons(_activeButtonIndex);
}

void RadioButtonGroup::renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) {
	for (int i = 0; i < _buttons.size(); i++) {
		_buttons[i]->renderWithRenderer(renderer, order);
	}
}

void RadioButtonGroup::toggleButtonDidSwitchState(ToggleButton* button, bool isOn) {
	auto it = std::find(_buttons.begin(), _buttons.end(), button);
	if (it != _buttons.end()) {
		const int index = it - _buttons.begin();

		if (index == _activeButtonIndex) {
			button->setState(true);
			return;
		}

		_activeButtonIndex = index;
		adjustButtons(_activeButtonIndex);
		if (_client) {
			_client->radioButtonGroupToggledActiveButtonAt(this, _activeButtonIndex);
		}
	}
}

void RadioButtonGroup::adjustButtons(const unsigned int activeButtonIndex) {
	for (int i = 0; i < _buttons.size(); i++) {
		_buttons[i]->setState(activeButtonIndex == i);
	}
}

ToggleButton* RadioButtonGroup::buildFromModelList(void* context, int index, Rect frame) {
	std::vector<ButtonModel>* models = static_cast<std::vector<ButtonModel>*>(context);
	ButtonModel model = (*models)[index];
	return new ToggleButton(model.spriteModel, model.pressedStateSpriteModel, frame, this);
}

ToggleButton* RadioButtonGroup::buildFromLabeledModelList(void* context, int index, Rect frame) {
	std::vector<LabeledButtonModel>* models = static_cast<std::vector<LabeledButtonModel>*>(context);
	LabeledButtonModel model = (*models)[index];
	return new ToggleButton(model.spriteModel, model.pressedStateSpriteModel, model.labelModel, frame, this);
}