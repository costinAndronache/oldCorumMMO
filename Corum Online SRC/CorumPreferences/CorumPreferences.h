#pragma once

#include <functional>
#include <vector>
#include "SkillHotkeys.h"

namespace CorumPreferences {
	using Data = std::vector<uint8_t>;
	struct Resolution { int width; int height; };

	typedef std::map<SkillHotkey, int> SkillHotkeyMap;

	class Preferences {
	public:
		static Preferences* buildFromData(const Data& data);
		static Preferences* buildFromFile(const wchar_t* filePath);
		static Preferences defaults();

		static const wchar_t* defaultFileName() { return L"CorumPrefs.prefs"; } 

	public:
		Preferences();
        bool windowMode() const { return _windowMode; };
		Resolution resolution() const { return _resolution; }

		void storeWindowMode(bool wm) { _windowMode = wm; }
		void storeResolution(Resolution r) { _resolution = r; }

		SkillHotkey hotkeyForSkill(int skillID, SkillClass skillClass) const;
		int skillIDForHotkey(SkillHotkey hk, SkillClass skillClass) const;

		std::vector<std::pair<SkillHotkey, int>> currentSkillHotkeyEntries(SkillClass skillClass) const;
		void storeHotkeyForSkill(SkillHotkey hk, int skillID, SkillClass skillClass);

		Data serialize() const;
		void serializeToFile(const wchar_t* filePath) const;

	private:
		typedef std::map<SkillClass, SkillHotkeyMap> SkillHotkeyMapPerClass;

		bool _windowMode;
		Resolution _resolution;
		SkillHotkeyMapPerClass _skillHotkeyMapPerClass;
	};

}

void fixDungeonStaticDataEntry(int id, const char* name);
void fixLayerFormationForAllEntries();
void serializeCurrentDungeonStaticDataFixes();

