#include "NewSkillSelectionView.h"
#include "HUDSpriteCollection.h"

using namespace CustomUI;
using namespace NewInterface;

static const Size iconsSize{ 40, 40 };

NewSkillSelectionView::NewSkillSelectionView(CustomUI::Rect frameInParent,
	CustomUI::MatrixContainer::VerticalGrowthDirection direction, SSKILL_LIST_MANAGER* skillListManager):
	_skillListManager(skillListManager) {
	_frameInParent = frameInParent;

	_leftSkillsContainer = registerChildRenderable<MatrixContainer>([=]() {
		return new MatrixContainer(bounds(), direction, iconsSize, 25);
	});

	_righttSkillsContainer = registerChildRenderable<MatrixContainer>([=]() {
		return new MatrixContainer(bounds(), direction, iconsSize, 25);
	});

	_guardianSkillsContainer = registerChildRenderable<MatrixContainer>([=]() {
		return new MatrixContainer(bounds(), direction, iconsSize, 25);
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

	_leftSkillsContainer->rebuild<BYTE>(cs.currentLeftSkills, 4, [&](BYTE skillKind, Rect frameInContainer) {
		const auto sprites = spritesForSkill(skillKind);
		auto button = new Button(sprites, frameInContainer);
		button->onClickEndLEFT([=]() {
			this->_activeSkillSelection = ActiveSkillSelection::none;
			this->_handlers.onLeftSkillSelection(skillKind);
		});

		return button;
	});

	_righttSkillsContainer->rebuild<BYTE>(cs.currentRightSkills, 4, [&](BYTE skillKind, Rect frameInContainer) {
		const auto sprites = spritesForSkill(skillKind);
		auto button = new Button(sprites, frameInContainer);
		button->onClickEndLEFT([=]() {
			this->_activeSkillSelection = ActiveSkillSelection::none;
			this->_handlers.onRightSkillSelection(skillKind);
		});
		return button;
	});

	_guardianSkillsContainer->rebuild<BYTE>(cs.currentGuardianSkills, 4, [&](BYTE skillKind, Rect frameInContainer) {
		const auto sprites = spritesForSkill(skillKind);
		auto button = new Button(sprites, frameInContainer);
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