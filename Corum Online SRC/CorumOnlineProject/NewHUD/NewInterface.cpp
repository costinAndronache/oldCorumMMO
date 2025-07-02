#include "NewInterface.h"

using namespace CustomUI;
using namespace NewInterface;

Interface::Interface(CustomUI::Size screenSize,
					 CMainUser* mainUser,
					 LP_SKILL_LIST_MANAGER skillListManager) {
	_frameInParent = Rect{ {0, 0}, screenSize };
	const auto leftHUDOriginY = (long)(screenSize.height - LeftHUD::preferredSize().height);
	_leftHUD = registerChildRenderable<LeftHUD>([&]() {
		return new LeftHUD(Point{ 0,  leftHUDOriginY });
	});

	_skillSelectionView = registerChildRenderable<NewSkillSelectionView>([&]() {
		return new NewSkillSelectionView(
			Point{ 0, leftHUDOriginY - 5 },
			MatrixContainer::VerticalGrowthDirection::upwards,
			skillListManager
		);
	});

	_skillSelectionView->updateCurrentSkills({
		mainUser->skillsAvailableOnLeft(),
		mainUser->skillsAvailableOnRight(),
		std::vector<BYTE>()
	});

	_thisAsShared = std::shared_ptr<Interface>(this, [](Interface*) {
		// no destruction
	});

	mainUser->addUpdateListener(std::weak_ptr<Interface>(_thisAsShared));

	_leftHUD->updateEXPScale(mainUser->percentageEXP());
	_leftHUD->updateHPAttributes(mainUser->currentHP(), mainUser->maxHP());
}

void Interface::updatedSkills(CMainUser* mainUser) {
	_skillSelectionView->updateCurrentSkills({
		mainUser->skillsAvailableOnLeft(),
		mainUser->skillsAvailableOnRight(),
		std::vector<BYTE>()
	});
}

//CMainUserUpdateInterested

void Interface::updatedCurrentHP(CMainUser*, DWORD oldValue, DWORD newValue) {

}

void Interface::updatedCurrentSP(CMainUser*, DWORD oldValue, DWORD newValue) {

}

void Interface::updatedMAXHP(CMainUser*, DWORD oldValue, DWORD newValue) {

}

void Interface::updatedMAXSP(CMainUser*, DWORD oldValue, DWORD newValue) {

}

void Interface::updatedEXP(CMainUser* mainUser, DWORD oldValue, DWORD newValue) {
	_leftHUD->updateEXPScale(mainUser->percentageEXP());
}

void Interface::updatedStatPoints(CMainUser*, DWORD oldValue, DWORD newValue) {

}

void Interface::updatedSkillPoints(CMainUser*, DWORD oldValue, DWORD newValue) {

}

void Interface::updatedCoolPoints(CMainUser*, float oldValue, float newValue) {

}

void Interface::updatedLevel(CMainUser*, DWORD oldValue, DWORD newValue) {

}
