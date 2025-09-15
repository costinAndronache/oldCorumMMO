#include "NewSkillSelectionView.h"
#include "HUDSpriteCollection.h"
#include "../InitGame.h"
#include "../../BaseLibrary/WorkQueue.h"

using namespace CustomUI;
using namespace NewInterface;

static const Size iconsSize{ 40, 40 };
static SingleLineLabel::Appearance skillHotkeyAppearance() {
	return {Color::red, verdanaBIG};
}

NewSkillSelectionView::NewSkillSelectionView(CustomUI::Rect frameInParent,
	CustomUI::MatrixContainer::VerticalGrowthDirection direction, SSKILL_LIST_MANAGER* skillListManager):
	_skillListManager(skillListManager) {
	_frameInParent = frameInParent;

	auto appearance = MatrixContainer::Appearance{
		direction,
		{
			iconsSize,
			4,
			5,
			5
		}
	};

	_leftSkillsContainer = registerChildRenderable<MatrixContainer>([=]() {
		return std::make_shared<MatrixContainer>(bounds(), appearance);
	});

	_righttSkillsContainer = registerChildRenderable<MatrixContainer>([=]() {
		return std::make_shared<MatrixContainer>(bounds(), appearance);
	});

	_guardianSkillsContainer = registerChildRenderable<MatrixContainer>([=]() {
		return std::make_shared<MatrixContainer>(bounds(), appearance);
	});

	switchToActiveSelection(ActiveSkillSelection::none);
}

void NewSkillSelectionView::updateCurrentSkills(CurrentSkills cs) {
	cs.currentLeftSkills.insert(cs.currentLeftSkills.begin(), __SKILL_ATTACK__);

	std::function<Button::Sprites (BYTE)> spritesForSkill = [this](BYTE skillKind) {
		Button::Sprites specimen = Button::Sprites::allZero;
		specimen.normal = NewHUDResources::spriteForSkill(skillKind, _skillListManager);
		return specimen;
	};

	auto hotkeyForSkill = [=](BYTE skillKind) -> std::string {
		auto found = cs.skillHotkeyMap.find(skillKind);
		if(skillKind == __SKILL_ICEWAVE__) {
			printf("");
		}
		if(found != cs.skillHotkeyMap.end()) {
			return found->second;
		} else {
			return "";
		}
	};

	_leftSkillsContainer->rebuild<BYTE>(cs.currentLeftSkills, [&](BYTE skillKind, int, Rect frameInContainer) {
		const auto sprites = spritesForSkill(skillKind);
		auto lm = Button::LabelModel(hotkeyForSkill(skillKind), skillHotkeyAppearance());

		auto button = std::make_shared<Button>(sprites, lm, frameInContainer);

		button->onClickEndLEFT([=]() {
			this->_activeSkillSelection = ActiveSkillSelection::none;
			this->_handlers.onLeftSkillSelection(skillKind);
		});

		button->onKeyDown([=](WPARAM wparam, LPARAM lparam) {
			if(_handlers.onKeyDownWhenHoveringSkill) {
				_handlers.onKeyDownWhenHoveringSkill(wparam, lparam, skillKind);

				 // dirty dirty hack 
				 // this will trigger the container to delete & rebuild the buttons
				 // while in the middle of the `handleKeyDown` of the current button
				WorkQueue::mainThreadQueue()->enqueueWorkItem([=](){
				});
			}
		});

		return button;
	});

	_righttSkillsContainer->rebuild<BYTE>(cs.currentRightSkills, [&](BYTE skillKind, int, Rect frameInContainer) {
		const auto sprites = spritesForSkill(skillKind);
		auto lm = Button::LabelModel(hotkeyForSkill(skillKind), skillHotkeyAppearance());
		auto button = std::make_shared<Button>(sprites, lm, frameInContainer);
		
		button->onClickEndLEFT([=]() {
			this->_activeSkillSelection = ActiveSkillSelection::none;
			this->_handlers.onRightSkillSelection(skillKind);
		});

		button->onKeyDown([=](WPARAM wparam, LPARAM lparam) {
			if(_handlers.onKeyDownWhenHoveringSkill) {
				_handlers.onKeyDownWhenHoveringSkill(wparam, lparam, skillKind);
			}
		});

		return button;
	});

	_guardianSkillsContainer->rebuild<BYTE>(cs.currentGuardianSkills, [&](BYTE skillKind, int, Rect frameInContainer) {
		const auto sprites = spritesForSkill(skillKind);
		auto button = std::make_shared<Button>(sprites, frameInContainer);
		button->onClickEndLEFT([=]() {
			this->_activeSkillSelection = ActiveSkillSelection::none;
			this->_handlers.onGuardianSkillSelection(skillKind);
		});
		return button;
	});
}

void NewSkillSelectionView::switchToActiveSelection(ActiveSkillSelection selection) {
	_activeSkillSelection = selection;
	_leftSkillsContainer->setHidden(selection != ActiveSkillSelection::leftSkills);
	_righttSkillsContainer->setHidden(selection != ActiveSkillSelection::rightSkills);
	_guardianSkillsContainer->setHidden(selection != ActiveSkillSelection::guardianSkills);

}