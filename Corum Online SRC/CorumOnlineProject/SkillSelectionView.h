#pragma once
#include "Menu.h"
#include "User.h"


enum class SkillSelectionWindow { none, leftSkills, rightSkills, guardianSkills };
class SkillSelectionView: public CMainUserUpdateInterested {
public:
	typedef struct SkillSource {
		const std::vector<BYTE>& skills;
		std::function<void(int)> selectSkillAtIndex;
		static SkillSource none;
	};

	static std::shared_ptr<SkillSelectionView> sharedInstance();
	SkillSelectionWindow activeSkillSelectionWindowType() const;
	void setActiveSkillSelectionWindowType(SkillSelectionWindow);
	bool isMouseHovering();
	void handleMouseButtonDownEvent(VECTOR2 mousePoint);
	void handleKeyEvent(char ascii);
	void render();

	SkillSelectionView(std::vector<BYTE> skillsOnLeft, std::vector<BYTE> skillsOnRight);

public:
	void updatedSkills(CMainUser*) override;

private:
	SkillSelectionWindow _activeSkillSelectionWindowType;
	SkillSource currentSkillSource();
	IDISpriteObject* m_pLineSpr;
	IDISpriteObject* m_pAttackSpr;

	std::vector<BYTE> _currentLeftSkills;
	std::vector<BYTE> _currentRightSkills;
	std::vector<BYTE> _currentGuardianSkills;

	void renderCurrentSelectedLeftRightSkills();
	void renderOpenedSkillSelection();

};

