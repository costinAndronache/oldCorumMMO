#include "UserSkillsView.h"

#include "../InitGame.h"
#include "../NewHUD/HUDSpriteCollection.h"

using namespace CustomUI;
using namespace NewInterface;

static Size skillIconSize = { 40, 40 };

GenericSkillView::Model GenericSkillView::Model::zero() {
	return { SpriteModel::zero, "", nullptr };
}

CustomUI::Size GenericSkillView::preferredSize() {
	return { 2 * skillIconSize.width, skillIconSize.height };
};


GenericSkillView::GenericSkillView(CustomUI::Rect frameInParent) {
	_frameInParent = frameInParent;
	auto _bounds = bounds();

	auto skillSpriteFrame = _bounds.scaled(0.5, 1.0);
	_spriteRenderable = registerChildRenderable<SpriteRenderable>([=]() {
		return new SpriteRenderable(skillSpriteFrame, SpriteModel::zero);
	});

	auto labelFrame =
		skillSpriteFrame
		.fromMaxXOrigin(0)
		.scaled(1.0, 0.5);

	_levelInfoLabel = registerChildRenderable<SingleLineLabel>([=]() {
		return new SingleLineLabel(
			labelFrame,
			SingleLineLabel::Appearance::defaultAppearance(),
			""
		);
	});

	auto btnFrame = labelFrame.fromMaxYOrigin(0);
	_increaseButton = registerChildRenderable<Button>([=]() {
		return new Button(NewHUDResources::up, btnFrame);
	});
}

void GenericSkillView::updateModel(Model model) {
	if (model.increaseHandler) {
		_increaseButton->setHidden(false);
		_increaseButton->onClickEndLEFT(model.increaseHandler);
	} else {
		_increaseButton->setHidden(true);
	}

	_levelInfoLabel->updateTextTo(model.levelInfo);
	_spriteRenderable->updateSprite(model.spriteModel);
}

static int titleHeight = 30;
static Insets insets = { 5, 5, 5, 5 };

static MatrixContainer::Appearance skillsContainerAppearance{
	MatrixContainer::VerticalGrowthDirection::downwards,
	{
		GenericSkillView::preferredSize(),
		2,
		12,
		12
	}
};

CustomUI::Size SkillsContainerView::appropriateSizeFor(int skillElementsCount) {
	auto matrixContainerSize = MatrixContainer::appropriateSizeFor(skillsContainerAppearance.sizes, skillElementsCount).toCoverInsets(insets);
	return { matrixContainerSize.width, matrixContainerSize.height + titleHeight };
}

SkillsContainerView::SkillsContainerView(CustomUI::Rect frame, const std::string& title) {
	_frameInParent = frame;
	auto _bounds = bounds();

	auto titleFrame = _bounds.withHeight(titleHeight);
	_titleLabel = registerChildRenderable<SingleLineLabel>([=]() {
		return new SingleLineLabel(
			titleFrame, SingleLineLabel::Appearance::defaultAppearance(), title
		);
	});
	_titleLabel->updateBackground(NewHUDResources::borderedBlackBackgroundSolid);
	_titleLabel->updateRenderingModeToCentered();

	auto containerFrame = _bounds
		.withOriginOffsetBy({0, (int)titleFrame.size.height })
		.withHeightOffset(-titleFrame.size.height)
		.withInsets(insets);

	_container = registerChildRenderable<MatrixContainer>([=]() {
		return new MatrixContainer(containerFrame, skillsContainerAppearance);
	});
}

void SkillsContainerView::refreshWithModels(const std::vector<Model>& models) {
	_container->rebuild<Model>(
		models,
		[=](Model m, int, Rect itemFrame) {
			auto view = new GenericSkillView(itemFrame);
			view->updateModel(m);
			return view;
		}
	);
}

static int horizontalSpacingBetweenContainers = 50;
Size SkillSheetView::masterySkillAreaSize() {
	return GenericSkillView::preferredSize().toCoverInsets({ 10, 10, 0, 0 });
}

static MatrixContainer::Sizes skillsContainerHostSizes(Size oneContainerSize, int horizontalSpacingBetweenContainers) {
	return {
		oneContainerSize,
		3,
		horizontalSpacingBetweenContainers,
		0
	};
}
Size SkillSheetView::appropriateSizeForMaxNumberOfSkillsPerList(int maxNumOfSkills) {
	auto oneContainerSize = SkillsContainerView::appropriateSizeFor(maxNumOfSkills);
	auto hostContainerSize = MatrixContainer::appropriateSizeFor(
		skillsContainerHostSizes(oneContainerSize, horizontalSpacingBetweenContainers),
		3
	);
	auto masterySize = masterySkillAreaSize();
	return { hostContainerSize.width, hostContainerSize.height + masterySize.height };
}

SkillSheetView::SkillSheetView(CustomUI::Rect frameInParent) {
	_frameInParent = frameInParent;

	auto _masterySkillAreaSize = masterySkillAreaSize();
	auto masterySkillAreaFrame = bounds()
		.withHeight(_masterySkillAreaSize.height);

	auto masterySkillViewFrame = Rect{ {0, 0}, GenericSkillView::preferredSize()};
	masterySkillViewFrame = masterySkillViewFrame.centeredWith(masterySkillAreaFrame);

	_masterySkillView = registerChildRenderable<GenericSkillView>([=]() {
		return new GenericSkillView(masterySkillViewFrame);
	});

	auto skillContainersHostFrame = bounds()
		.withOriginOffsetBy({ 0, (int)masterySkillAreaFrame.size.height })
		.withHeightOffset(-masterySkillAreaFrame.size.height);

	auto oneContainerSize = Size{
		(bounds().size.width - 2 * horizontalSpacingBetweenContainers) / 3,
		bounds().size.height
	};
	_skillContainersHost = registerChildRenderable<MatrixContainer>([=]() {
		return new MatrixContainer(
			skillContainersHostFrame,
			{
				MatrixContainer::VerticalGrowthDirection::downwards,
				skillsContainerHostSizes(oneContainerSize, horizontalSpacingBetweenContainers)
			}
		);
	});
}

void SkillSheetView::refreshWith(Model model) {
	_masterySkillView->updateModel(model.masterySkill);

	struct ModelsWithTitle { std::vector<SkillsContainerView::Model> models; std::string title; };

	std::vector<ModelsWithTitle> containerModels = {
		{ model.activeSkills, "ACTIVE"},
		{ model.passiveSkills, "PASSIVE"},
		{ model.overdriveSkills, "OVERDRIVE"}
	};

	_skillContainersHost->rebuild<ModelsWithTitle>(
		containerModels,
		[=](ModelsWithTitle m, int, Rect frame) {
			auto skillContainer = new SkillsContainerView(frame, m.title);
			skillContainer->refreshWithModels(m.models);
			return skillContainer;
		}
	);

}


static Size radioButtonSize{ 100, 50 };
static int spacingRadioButtonsSkillSheetArea = 20;
static Size closeBtnSize{ 40, 40 };

Size UserSkillsView::appropriateSizeForMaxNumberOfSkillsPerList(int maxNumOfSkills) {
	auto size = SkillSheetView::appropriateSizeForMaxNumberOfSkillsPerList(maxNumOfSkills);

	return {
		size.width + radioButtonSize.width + spacingRadioButtonsSkillSheetArea,
		size.height + closeBtnSize.height
	};
}

UserSkillsView::UserSkillsView(CustomUI::Rect frameInParent) {
	_frameInParent = frameInParent;
	updateBackground(NewHUDResources::genericBackgroundSprite);

	auto labelFrame = bounds()
		.withHeight(closeBtnSize.height);

	_titleLabel = registerChildRenderable<SingleLineLabel>([=]() {
		return new SingleLineLabel(
			labelFrame, SingleLineLabel::Appearance::defaultAppearance(), ""
		);
	});

	_titleLabel->updateRenderingModeToCentered();

	auto closeBtnFrame = bounds()
		.fromMaxXOrigin(-closeBtnSize.width)
		.withSize(closeBtnSize);

	_closeButton = registerChildRenderable<Button>([=]() {
		return new Button(NewHUDResources::xClose, closeBtnFrame);
	});



	using LabeledButtonModel = RadioButtonGroup::LabeledButtonModel;
	std::vector<LabeledButtonModel> buttonModels = {
		LabeledButtonModel::defaultWith("SUMMONER"),
		LabeledButtonModel::defaultWith("FIGHTER"),
		LabeledButtonModel::defaultWith("PRIEST"),
		LabeledButtonModel::defaultWith("RANGER"),
		LabeledButtonModel::defaultWith("SORCERESS"),
	};

	auto skillSheetMasteryAreaSize = SkillSheetView::masterySkillAreaSize();
	auto radioGroupFrame = bounds()
		.withWidth(radioButtonSize.width)
		.withOriginOffsetBy({ 0, (int)closeBtnSize.height + (int)skillSheetMasteryAreaSize.height })
		.withHeight(buttonModels.size() * radioButtonSize.height);

	_radioGroup = registerChildRenderable<RadioButtonGroup>([=]() {
		return new RadioButtonGroup(buttonModels, radioGroupFrame, 0);
	});

	auto skillSheetFrame = bounds()
		.withX(radioGroupFrame.maxX() + spacingRadioButtonsSkillSheetArea)
		.withWidthOffset(-(radioGroupFrame.size.width + spacingRadioButtonsSkillSheetArea))
		.withY(closeBtnFrame.maxY())
		.withHeightOffset(-closeBtnFrame.size.height);


	std::transform(
		std::begin(buttonModels),
		std::end(buttonModels),
		std::back_inserter(_skillSheetViews),
		[=](LabeledButtonModel) -> SkillSheetView* {
			return registerChildRenderable<SkillSheetView>([=]() {
				return new SkillSheetView(skillSheetFrame);
			});
		}
	);

	_radioGroup->setActiveButtonIndex(0);
	setVisibleSkillSheetAtIndex(0);

	_radioGroup->onActiveIndexUpdate([=](unsigned int index) {
		setVisibleSkillSheetAtIndex(index);
	});
}

void UserSkillsView::setVisibleSkillSheetAtIndex(int index) {
	if (!(0 <= index && index < _skillSheetViews.size())) { return; }
	for (auto skillSheet : _skillSheetViews) { skillSheet->setHidden(true); }
	_skillSheetViews[index]->setHidden(false);
}

void UserSkillsView::onClose(CustomUI::Button::EventHandler handler) {
	_closeButton->onClickEndLEFT(handler);
}

void UserSkillsView::refreshWith(Model model) {
	std::vector< SkillSheetView::Model> models = {
		model.summonerSkills,
		model.fighterSkills,
		model.priestSkills,
		model.rangerSkills,
		model.sorceressSkills
	};

	for (int i = 0; i < 5; i++) {
		_skillSheetViews[i]->refreshWith(models[i]);
	}

	std::string title = [=]() {
		if (model.availableSkillPoints > 0) {
			return "Skills: (" + std::to_string(model.availableSkillPoints) + ")";
		} else {
			return std::string("Skills");
		}
	}();

	_titleLabel->updateTextTo(title);
}