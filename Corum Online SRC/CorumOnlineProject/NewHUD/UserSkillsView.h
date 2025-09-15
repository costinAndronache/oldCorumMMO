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
		std::shared_ptr<CustomUI::SpriteRenderable> _spriteRenderable;
		std::shared_ptr<CustomUI::Button> _increaseButton;
		std::shared_ptr<CustomUI::SingleLineLabel> _levelInfoLabel;
		std::shared_ptr<CustomUI::Hoverable> _hoverableOverSprite;
	};

	class SkillsContainerView: public CustomUI::Renderable {
	public:
		using Model = GenericSkillView::Model;
		static CustomUI::Size appropriateSizeFor(int skillElementsCount);

		SkillsContainerView(CustomUI::Rect frame, const std::string& title);
		void refreshWithModels(const std::vector<Model>& models); 

	private:
		std::shared_ptr<CustomUI::SingleLineLabel> _titleLabel;
		std::shared_ptr<CustomUI::MatrixContainer> _container;
		std::shared_ptr<CustomUI::BorderRenderable> _border;
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
		std::shared_ptr<GenericSkillView> _masterySkillView;
		std::shared_ptr<CustomUI::MatrixContainer> _skillContainersHost;
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

		std::shared_ptr<DisplacementHandleRenderable> displacementHandle() { return _displacementHandle;}
	private:
		std::shared_ptr<DisplacementHandleRenderable> _displacementHandle;
		std::shared_ptr<CustomUI::SingleLineLabel> _titleLabel;
		std::shared_ptr<CustomUI::Button> _closeButton;
		std::shared_ptr<CustomUI::RadioButtonGroup> _radioGroup;
		std::vector<std::shared_ptr<SkillSheetView>> _skillSheetViews;
		ClassSheetSwitchHandler _classSwitchHandler;
		void setVisibleSkillSheetAtIndex(int index);
	};
}


