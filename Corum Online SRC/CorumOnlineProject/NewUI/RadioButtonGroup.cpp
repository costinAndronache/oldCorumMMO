#include "RadioButtonGroup.h"
#include <functional>

using namespace CustomUI;

RadioButtonGroup::LabeledButtonModel RadioButtonGroup::LabeledButtonModel::defaultWith(std::string text) {
	RadioButtonGroup::LabeledButtonModel specimen;
	specimen.labelModel.appearance.color.a = 255;
	specimen.labelModel.appearance.color.r = 201;
	specimen.labelModel.appearance.color.g = 210;
	specimen.labelModel.appearance.color.b = 250;

	specimen.sprites = Button::Sprites{
		{
			SpriteCollection::genericButtonBackground,
			SpriteCollection::genericButtonBackgroundSize
		},
		SpriteModel::zero,
		{
			SpriteCollection::genericButtonPressedBackground,
			SpriteCollection::genericButtonBackgroundSize
		},
	};

	specimen.labelModel.text = text;

	return specimen;
}

static void fillButtons(
	std::vector<ToggleButton*>& buttonList, int count,
	Point origin, Size buttonSize, int xStep, int yStep,
	std::function<ToggleButton*(int, Rect)> createFn) {
	for (int i = 0; i < count; i++) {
		Rect btnFrame = { { origin.x + i * xStep, origin.y + i * yStep }, buttonSize };

		ToggleButton* tgb = createFn(i, btnFrame);
		buttonList.push_back(tgb);
	}
}

RadioButtonGroup::RadioButtonGroup(std::vector<ButtonModel> models, Rect frameInParent, unsigned int activeButtonIndex):
	_activeButtonIndex(activeButtonIndex) {
	_buttons.reserve(models.size());
	_frameInParent = frameInParent;

	std::function<ToggleButton* (int, Rect)> createFn = [&](int index, Rect btnFrame) {
		return buildFromModelList(&models, index, btnFrame);
	};

	if (frameInParent.size.width > frameInParent.size.height) {
		Size size = { frameInParent.size.width / models.size(), frameInParent.size.height };
		fillButtons(_buttons, models.size(), {0, 0}, size, size.width, 0, createFn);
	}
	else {
		Size size = { frameInParent.size.width, frameInParent.size.height / models.size() };
		fillButtons(_buttons, models.size(), {0, 0}, size, 0, size.height, createFn);
	}
	adjustButtons(_activeButtonIndex);
}

RadioButtonGroup::RadioButtonGroup(std::vector<LabeledButtonModel> models, Rect frameInParent, unsigned int activeButtonIndex):
	_activeButtonIndex(activeButtonIndex) {
	_frameInParent = frameInParent;

	_buttons.reserve(models.size());
	std::function<ToggleButton*(int, Rect)> createFn = [&](int index, Rect btnFrame) {
		return buildFromLabeledModelList(&models, index, btnFrame);
	};

	if (frameInParent.size.width > frameInParent.size.height) {
		Size size = { frameInParent.size.width / models.size(), frameInParent.size.height };
		fillButtons(_buttons, models.size(), {0, 0}, size, size.width, 0, createFn);
	}
	else {
		Size size = { frameInParent.size.width, frameInParent.size.height / models.size() };
		fillButtons(_buttons, models.size(), {0, 0}, size, 0, size.height, createFn);
	}
	adjustButtons(_activeButtonIndex);
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
		if (_handler) { _handler(_activeButtonIndex); }
	}
}

void RadioButtonGroup::setActiveButtonIndex(unsigned int activeButtonIndex) {
	_activeButtonIndex = activeButtonIndex;
	adjustButtons(_activeButtonIndex);
}

void RadioButtonGroup::adjustButtons(const unsigned int activeButtonIndex) {
	for (int i = 0; i < _buttons.size(); i++) {
		_buttons[i]->setState(activeButtonIndex == i);
	}
}

ToggleButton* RadioButtonGroup::buildFromModelList(const std::vector<ButtonModel>* models, int index, Rect frame) {
	ButtonModel model = (*models)[index];
	auto toggleButton = registerChildRenderable<ToggleButton>([=]() {
		return new ToggleButton(model.sprites, frame);
	});

	toggleButton->onStateSwitch([this, toggleButton](bool isOn){
		toggleButtonDidSwitchState(toggleButton, isOn);
	});

	return toggleButton;
}

ToggleButton *RadioButtonGroup::buildFromLabeledModelList(
    const std::vector<LabeledButtonModel>* models, int index, Rect frame) {
	LabeledButtonModel model = (*models)[index];

	auto toggleButton = registerChildRenderable<ToggleButton>([=]() {
		return new ToggleButton(model.sprites, model.labelModel, frame);
	});

	toggleButton->onStateSwitch([this, toggleButton](bool isOn) {
		toggleButtonDidSwitchState(toggleButton, isOn);
	});

	return toggleButton;
}