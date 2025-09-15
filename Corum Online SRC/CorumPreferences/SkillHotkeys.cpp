#include "SkillHotkeys.h"

using namespace CorumPreferences;

std::string CorumPreferences::stringFor(SkillHotkey hotkey) {
	switch(hotkey) {
	case SkillHotkey_Q: return "Q";
	case SkillHotkey_W: return "W";
	case SkillHotkey_E: return "E";
	case SkillHotkey_R: return "R";
	case SkillHotkey_Z: return "Z";
	case SkillHotkey_X: return "X";
	case SkillHotkey_C: return "C";
	case SkillHotkey_V: return "V";
	case SkillHotkey_B: return "B";
	case SkillHotkey_F1: return "F1";
	case SkillHotkey_F2: return "F2";
	case SkillHotkey_F3: return "F3";
	case SkillHotkey_F4: return "F4";
	case SkillHotkey_F5: return "F5";
	case SkillHotkey_unknown: return "??";
	default: return "?x?";
	}
}