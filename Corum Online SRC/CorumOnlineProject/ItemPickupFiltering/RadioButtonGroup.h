#pragma once
#include "CustomUIBase.h"
#include "ToggleButton.h"
#include <vector>
namespace CustomUI {
	class RadioButtonGroup;
	class RadioButtonGroupClient {
	public:
		virtual void radioButtonGroupToggledActiveButtonAt(RadioButtonGroup*, unsigned int index) = 0;
	};

	class RadioButtonGroup: public ToggleButtonClient, public Renderable {
	public:
		struct ButtonModel {
			SpriteModel spriteModel;
			SpriteModel pressedStateSpriteModel;
		};

		struct LabeledButtonModel: public ButtonModel {
			Button::LabelModel labelModel;
		};

		RadioButtonGroup(std::vector<ButtonModel>, Rect frame, unsigned int activeButtonIndex, RadioButtonGroupClient *client);
		RadioButtonGroup(std::vector<LabeledButtonModel>, Rect frame, unsigned int activeButtonIndex, RadioButtonGroupClient* client);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int order) override;

	private:
		void toggleButtonDidSwitchState(ToggleButton* button, bool isOn) override;

		ToggleButton* buildFromModelList(void* context, int index, Rect frame);
		ToggleButton* buildFromLabeledModelList(void* context, int index, Rect frame);

		void adjustButtons(const unsigned int activeButtonIndex);

		Rect _frame;
		RadioButtonGroupClient* _client;
		unsigned int _activeButtonIndex;
		std::vector<ToggleButton*> _buttons;
	};
}



