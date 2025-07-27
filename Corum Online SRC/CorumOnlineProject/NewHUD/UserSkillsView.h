#pragma once

#include "../NewUI/MatrixContainer.h"
#include "../NewUI/Button.h"
#include "../NewUI/SpriteRenderable.h"
#include "../NewUI/RadioButtonGroup.h"

namespace NewInterface {
	class GenericSkillView : public CustomUI::Renderable {
	public:
		struct Model {
			CustomUI::SpriteModel spriteModel;
			std::string levelInfo;
			std::function<void()> increaseHandler;

			static Model zero();
		};

		static CustomUI::Size preferredSize();
		GenericSkillView(CustomUI::Rect frameInParent);

		void updateModel(Model model);
	private:
		CustomUI::SpriteRenderable* _spriteRenderable;
		CustomUI::Button* _increaseButton;
		CustomUI::SingleLineLabel* _levelInfoLabel;
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

		static CustomUI::Size appropriateSizeForMaxNumberOfSkillsPerList(int maxNumOfSkills);
		
		UserSkillsView(CustomUI::Rect frameInParent);
		
		void onClose(CustomUI::Button::EventHandler handler);
		void refreshWith(Model model);

	private:
		CustomUI::SingleLineLabel* _titleLabel;
		CustomUI::Button *_closeButton;
		CustomUI::RadioButtonGroup* _radioGroup;
		std::vector<SkillSheetView*> _skillSheetViews;

		void setVisibleSkillSheetAtIndex(int index);
	};
}


