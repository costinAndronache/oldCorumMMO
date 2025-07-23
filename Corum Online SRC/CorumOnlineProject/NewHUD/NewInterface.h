#pragma once
#include "LeftHUD.h"
#include "RightHUD.h"
#include "GroupedItemInventoryView.h"
#include "../NewUI/MouseTrackingSpriteRenderable.h"
#include "../NewUI/DragNDropSystem.h"
#include "ViewManager/DragNDropManager.h"
#include "UserInventoryView.h"
#include "ViewManager/EquipItemsManager.h"

namespace NewInterface {
	class Interface: 
		public CustomUI::Renderable, 
		public CMainUserUpdateInterested,
		public CustomUI::DragNDropSystemRenderer {
	public:
		Interface(CustomUI::Size screenSize, 
				  CMainUser* mainUser,
				  const LP_SKILL_LIST_MANAGER skillListManager,
				  CItemResourceHash* resourceHash);
		void renderWithRenderer(I4DyuchiGXRenderer* renderer, int zIndex) override;
		bool swallowsMouse(CustomUI::Point mouse) override;
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
		void updatedItemInventory(CMainUser*) override;

	public:
		virtual void renderOnMouseCursorAvatar(Renderable* avatar, 
			std::function<void(CustomUI::Rect avatarCurrentGlobalFrame)> onLeftMouseButtonUP
		);
		virtual void clearCurrentMouseCursorAvatar();
	private:
		LeftHUD* _leftHUD;
		RightHUD* _rightHUD;

		NewSkillSelectionView* _skillSelectionView;

		std::shared_ptr<Interface> _thisAsShared;

		LP_SKILL_LIST_MANAGER _skillListManager;
		CMainUser* _mainUser;

		CustomUI::DragNDropSystem* _dragNDropSystem;
		CustomUI::MouseTrackingSpriteRenderable* _mouseTracking;

		
		NewItemsWindow* _newItemsWindow;
		UserInventoryManager* _userInventoryManager;
		DragNDropManager* _dragNDropManager;
		EquipItemsManager* _equipItemsManager;

		void updateLeftHUDWithSelectedLeftRightSkills();
	};
}

