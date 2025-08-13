#pragma once

#include "../NewUI/MatrixContainer.h"
#include "../NewUI/Button.h"
#include "../NewUI/SpriteRenderable.h"
#include "../NewUI/RadioButtonGroup.h"
#include "../NewUI/BorderRenderable.h"
#include "../NewUI/Hoverable.h"
#include "DisplacementHandleRenderable.h"

namespace NewInterface {
	class GenericSkillView : public CustomUI::Renderable {
	public:
		struct Model {
			CustomUI::SpriteModel spriteModel;
			std::string levelInfo;
			std::function<void()> increaseHandler;

			CustomUI::Hoverable::OnHovering onHovering;
			CustomUI::Hoverable::OnHoveringEnd onHoveringEnd;

			static Model zero();
		};

		static CustomUI::Size preferredSize();
		GenericSkillView(CustomUI::Rect frameInParent);

		void updateModel(Model model);
	private:
		CustomUI::SpriteRenderable* _spriteRenderable;
		CustomUI::Button* _increaseButton;
		CustomUI::SingleLineLabel* _levelInfoLabel;
		CustomUI::Hoverable* _hoverableOverSprite;
	};

	class SkillsContainerView: public CustomUI::Renderable {
	public:
		using Model = GenericSkillView::Model;
		static CustomUI::Size appropriateSizeFor(int skillElementsCount);

		SkillsContainerView(CustomUI::Rect frame, const std::string& title);
		void refreshWithModels(const std::vector<Model>& models); 

	private:
		CustomUI::SingleLineLabel* _titleLabel;
		CustomUI::MatrixContainer* _container;
		CustomUI::BorderRenderable* _border;
	};

	class SkillSheetView: public CustomUI::Renderable {
	public:
		struct Model {
			std::vector<SkillsContainerView::Model> activeSkills;
			std::vector<SkillsContainerView::Model> passiveSkills;
			std::vector<SkillsContainerView::Model> overdriveSkills;
			SkillsContainerView::Model masterySkill;
		};
		static CustomUI::Size appropriateSizeForMaxNumberOfSkillsPerList(int maxNumOfSkills);
		static CustomUI::Size masterySkillAreaSize();

		SkillSheetView(CustomUI::Rect frameInParent);

		void refreshWith(Model model);

	private:
		GenericSkillView* _masterySkillView;
		CustomUI::MatrixContainer* _skillContainersHost;
	};


	class UserSkillsView : public CustomUI::Renderable {
	public:
		struct Model {
			SkillSheetView::Model summonerSkills;
			SkillSheetView::Model fighterSkills;
			SkillSheetView::Model priestSkills;
			SkillSheetView::Model rangerSkills;
			SkillSheetView::Model sorceressSkills;
			int availableSkillPoints;
		};
		using ClassSheetSwitchHandler = std::function<void(CLASS_TYPE to)>;

		static CustomUI::Size appropriateSizeForMaxNumberOfSkillsPerList(int maxNumOfSkills);
		
		UserSkillsView(CustomUI::Rect frameInParent);
		
		void onClose(CustomUI::Button::EventHandler handler);
		void refreshWith(Model model);
		void onClassSheetSwitch(ClassSheetSwitchHandler handler) { _classSwitchHandler = handler;}

		DisplacementHandleRenderable* displacementHandle() { return _displacementHandle;}
	private:
		DisplacementHandleRenderable* _displacementHandle;
		CustomUI::SingleLineLabel* _titleLabel;
		CustomUI::Button *_closeButton;
		CustomUI::RadioButtonGroup* _radioGroup;
		std::vector<SkillSheetView*> _skillSheetViews;
		ClassSheetSwitchHandler _classSwitchHandler;
		void setVisibleSkillSheetAtIndex(int index);
	};
}


