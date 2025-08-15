#include "ExitOptionsWindow.h"
#include "HUDSpriteCollection.h"

using namespace CustomUI;
using namespace NewInterface;

static auto optionSize = Size {200, 40};

ExitOptionsWindow::ExitOptionsWindow(CustomUI::Point originInParent) {
	auto mcSizes = MatrixContainer::Sizes { optionSize, 1, 0, 3 };
	auto matrixContainerSize = MatrixContainer::appropriateSizeFor(mcSizes, 3);

	_frameInParent = Rect{ 
		originInParent, 
		{ matrixContainerSize.width, matrixContainerSize.height + optionSize.height }
	};

	auto displacementHandleFrame = Rect{{0, 0}, {optionSize.height, optionSize.height}};
	_displacementHandle = registerChildRenderable<DisplacementHandleRenderable>([=](){
		return new DisplacementHandleRenderable(displacementHandleFrame);
	});

	auto closeButtonFrame = bounds()
		.fromMaxXOrigin(-optionSize.height)
		.withSize({optionSize.height, optionSize.height});
	_closeButton = registerChildRenderable<Button>([=](){
		return new Button(NewHUDResources::xClose, closeButtonFrame);
	});

	auto matrixContainerFrame = bounds()
		.withY(optionSize.height)
		.withHeight(matrixContainerSize.height);

	_mc = registerChildRenderable<MatrixContainer>([=](){
		return new MatrixContainer(
			matrixContainerFrame, 
			{ 
				MatrixContainer::VerticalGrowthDirection::downwards,
				mcSizes
			}
		);
	});

	updateBackground(NewHUDResources::genericBackgroundSprite);
}

void ExitOptionsWindow::rebuildInDungeonMode(DungeonModeHandlers handlers) {
	std::vector<Model> models {
		{ "SERVER SELECTION", handlers.onServerSelectionOption }, 
		{ "MOVE TO WORLD MAP", handlers.onWorldModeOption},
        { "EXIT GAME", handlers.onGameExitOption}
	};

	rebuildWithModels(models);
}

void ExitOptionsWindow::rebuildInWorldMode(WorldModeHandlers handlers) {
	std::vector<Model> models {
		{ "SERVER SELECTION", handlers.onServerSelectionOption },
        { "EXIT GAME", handlers.onGameExitOption}
	};
	rebuildWithModels(models);
}

void ExitOptionsWindow::rebuildWithModels(const std::vector<Model> &models) {
  _mc->rebuild<Model>(models, [=](auto model, auto index, auto frame) {
    Button *button = new Button(
		NewHUDResources::genericButtonBackgroundSprites, 
		{ model.text },
		frame
    );
	button->onClickEndLEFT(model.handler);
	return button;
  });
}

void ExitOptionsWindow::onClose(EventHandler handler) {
	_closeButton->onClickEndLEFT(handler);
}