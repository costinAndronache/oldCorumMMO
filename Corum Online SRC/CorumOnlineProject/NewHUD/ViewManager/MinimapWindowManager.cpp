#include "MinimapWindowManager.h"
#include "../HUDSpriteCollection.h"
using namespace CustomUI;
using namespace NewInterface;

static const Size minimapTIFSize = {144, 144};

MinimapWindowManager::MinimapWindowManager(
	std::shared_ptr<MinimapWindow> managedWindow, 
	I4DyuchiGXRenderer* renderer
) {
	_minimapWindow = managedWindow;
	_renderer = renderer;
	_currentSizeZ = _currentSizeX = 1.0;

	_mainPlayerDot = managedWindow->addDot(NewHUDResources::blueDot);
}

void MinimapWindowManager::updateForLayer(Layer layer) {

	auto sprite = spriteForLayer(layer.layerID);
	_currentSizeX = max(layer.xSize, 1.0);
	_currentSizeZ = max(layer.zSize, 1.0);

	_minimapWindow->updateMapSprite(sprite, layer.title);
}

void MinimapWindowManager::updateMainPlayerDotFrom3DPosition(VECTOR3 pos) {
	auto flipZ = max(_currentSizeZ - pos.z, 0.f);

	auto xRatio = pos.x / _currentSizeX;
	auto zRatio = flipZ / _currentSizeZ;

	_minimapWindow->updateDot(_mainPlayerDot, {xRatio, zRatio});
}

SpriteModel MinimapWindowManager::spriteForLayer(int layerID) {
	char szMapModelFile[0xff] = {0,};
	char szFileNameTIF[0xff] = {0,};

	wsprintf(szFileNameTIF, "%d.tif", layerID);
	lstrcpy(szMapModelFile, GetFile(szFileNameTIF, DATA_TYPE_MAP));

	
	return SpriteModel::from(_renderer, szMapModelFile, {{0, 0}, minimapTIFSize});
}