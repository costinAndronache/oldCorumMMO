#pragma once
#include <map>
#include <string>

namespace CorumPreferences {
	enum SkillHotkey {
		SkillHotkey_Q = 0,
		SkillHotkey_W,
		SkillHotkey_E,
		SkillHotkey_R,
		SkillHotkey_Z,
		SkillHotkey_X,
		SkillHotkey_C,
		SkillHotkey_V,
		SkillHotkey_B,
		SkillHotkey_F1,
		SkillHotkey_F2,
		SkillHotkey_F3,
		SkillHotkey_F4,
		SkillHotkey_F5,
		SkillHotkey_unknown
	};

	enum SkillClass {
		SkillClass_summoner = 0,
		SkillClass_priest,
		SkillClass_fighter,
		SkillClass_ranger,
		SkillClass_sorceress,
		SkillClass_unknown
	};

	std::string stringFor(SkillHotkey hotkey);

}


