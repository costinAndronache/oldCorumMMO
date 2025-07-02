#include "NewSkillSelectionView.h"

using namespace CustomUI;
using namespace NewInterface;

static const Size iconsSize{ 40, 40 };

NewSkillSelectionView::NewSkillSelectionView(CustomUI::Point growthOrigin,
	CustomUI::MatrixContainer::VerticalGrowthDirection direction, SSKILL_LIST_MANAGER* skillListManager):
	_skillListManager(skillListManager) {
	
	_leftSkillsContainer = registerChildRenderable<MatrixContainer>([=]() {
		return new MatrixContainer(growthOrigin, direction, iconsSize);
	});

	_righttSkillsContainer = registerChildRenderable<MatrixContainer>([=]() {
		return new MatrixContainer(growthOrigin, direction, iconsSize);
	});

	_guardianSkillsContainer = registerChildRenderable<MatrixContainer>([=]() {
		return new MatrixContainer(growthOrigin, direction, iconsSize);
	});

	switchToActiveSelection(ActiveSkillSelection::none);
}

void NewSkillSelectionView::updateCurrentSkills(CurrentSkills cs) {
	cs.currentLeftSkills.insert(cs.currentLeftSkills.begin(), __SKILL_ATTACK__);

	std::function<Button::Sprites (BYTE)> spritesForSkill = [this](BYTE skillKind) {
		Button::Sprites specimen = Button::Sprites::allZero;
		if (!(0 <= skillKind && skillKind < MAX_SKILL)) { return specimen; }

		auto sprite =  _skillListManager->spriteForSkillKind[skillKind];
		specimen.normal.sprite = sprite;
		specimen.normal.size = iconsSize;
		specimen.normal.rotation = 0;

		return specimen;
	};

	_leftSkillsContainer->rebuild<BYTE>(cs.currentLeftSkills, 4, [&](BYTE skillKind, Rect frameInContainer) {
		const auto sprites = spritesForSkill(skillKind);
		auto button = new Button(sprites, frameInContainer);
		button->onClickEnd([&]() {
			this->_activeSkillSelection = ActiveSkillSelection::none;
			this->_handlers.onLeftSkillSelection(skillKind);
		});

		return button;
	});

	_righttSkillsContainer->rebuild<BYTE>(cs.currentRightSkills, 4, [&](BYTE skillKind, Rect frameInContainer) {
		const auto sprites = spritesForSkill(skillKind);
		auto button = new Button(sprites, frameInContainer);
		button->onClickEnd([&]() {
			this->_activeSkillSelection = ActiveSkillSelection::none;
			this->_handlers.onRightSkillSelection(skillKind);
		});
		return button;
	});

	_guardianSkillsContainer->rebuild<BYTE>(cs.currentGuardianSkills, 4, [&](BYTE skillKind, Rect frameInContainer) {
		const auto sprites = spritesForSkill(skillKind);
		auto button = new Button(sprites, frameInContainer);
		button->onClickEnd([&]() {
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