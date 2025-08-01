#pragma once
#include "CustomUIBase.h"
#include "ToggleButton.h"
#include <vector>
namespace CustomUI {
	class RadioButtonGroup: public Renderable {
	public:
		typedef std::function<void(unsigned int)> ActiveIndexUpdateHandler;

		struct ButtonModel {
			Button::Sprites sprites;
		};

		struct LabeledButtonModel: public ButtonModel {
			Button::LabelModel labelModel;

			static LabeledButtonModel defaultWith(std::string text);
		};

		RadioButtonGroup(std::vector<ButtonModel>, Rect frameInParent, unsigned int activeButtonIndex);
		RadioButtonGroup(std::vector<LabeledButtonModel>, Rect frameInParent, unsigned int activeButtonIndex);

		void setActiveButtonIndex(unsigned int activeButtonIndex);

		void onActiveIndexUpdate(ActiveIndexUpdateHandler handler) { _handler = handler; };
	private:
		void toggleButtonDidSwitchState(ToggleButton* button, bool isOn);

		ToggleButton* buildFromModelList(const std::vector<ButtonModel>*, int index, Rect frame);
		ToggleButton* buildFromLabeledModelList(const std::vector<LabeledButtonModel>*, int index, Rect frame);

		void adjustButtons(const unsigned int activeButtonIndex);

		ActiveIndexUpdateHandler _handler;
		unsigned int _activeButtonIndex;
		std::vector<ToggleButton*> _buttons;
	};
}



