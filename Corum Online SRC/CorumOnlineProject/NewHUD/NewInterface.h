#pragma once
#include "LeftHUD.h"

namespace NewInterface {
	class Interface: public CustomUI::Renderable, public CMainUserUpdateInterested {
	public:
		Interface(CustomUI::Size screenSize, 
				  CMainUser* mainUser,
				  LP_SKILL_LIST_MANAGER skillListManager);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int zIndex) override;
	public:
		void updatedCurrentHP(CMainUser*, DWORD oldValue, DWORD newValue) override;
		void updatedCurrentSP(CMainUser*, DWORD oldValue, DWORD newValue) override;
		void updatedMAXHP(CMainUser*, DWORD oldValue, DWORD newValue) override;
		void updatedMAXSP(CMainUser*, DWORD oldValue, DWORD newValue) override;
		void updatedEXP(CMainUser*, DWORD oldValue, DWORD newValue) override;
		void updatedStatPoints(CMainUser*, DWORD oldValue, DWORD newValue) override;
		void updatedSkillPoints(CMainUser*, DWORD oldValue, DWORD newValue) override;
		void updatedCoolPoints(CMainUser*, float oldValue, float newValue) override;
		void updatedLevel(CMainUser*, DWORD oldValue, DWORD newValue) override;
		void updatedSkills(CMainUser*) override;
		void updatedLeftRightSkillSelection(CMainUser*) override;
	private:
		LeftHUD* _leftHUD;
		NewSkillSelectionView* _skillSelectionView;
		std::shared_ptr<Interface> _thisAsShared;

		LP_SKILL_LIST_MANAGER _skillListManager;
		CMainUser* _mainUser;
		void updateLeftHUDWithSelectedLeftRightSkills();
	};
}

