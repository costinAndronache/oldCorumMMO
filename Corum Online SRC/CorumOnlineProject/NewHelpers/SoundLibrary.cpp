#include "SoundLibrary.h"
#include "../DungeonProcess_Sound.h"

SoundLibrary* SoundLibrary::sharedInstance() {
	static SoundLibrary* shared = nullptr;
	if (!shared) { shared = new SoundLibrary(); }
	return shared;
}

SoundLibrary::SoundLibrary() {}


void SoundLibrary::playItemMousePickUp() {
	_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ITEMCLICK, g_v3InterfaceSoundPos, FALSE);
}

void SoundLibrary::playItemMouseDropInInterface() {
	_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_ITEM_THROW_MAGICITEM, g_v3InterfaceSoundPos, FALSE);

}

void SoundLibrary::playItemMouseDropInTile() {
	_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_ITEM_THROW_MAGICITEM, g_v3InterfaceSoundPos, FALSE);

}

void SoundLibrary::playStatPointUp() {
	_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ATTPOINTUP, g_v3InterfaceSoundPos, FALSE);

}

void SoundLibrary::playSkillPointUp() {
	_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ATTPOINTUP, g_v3InterfaceSoundPos, FALSE);
}

void SoundLibrary::playWindowOpen() {
	_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
}

void SoundLibrary::playWindowClose() {
	_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
}

void SoundLibrary::playButtonClick() {
	_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_BTNCLICK, g_v3InterfaceSoundPos, FALSE);
}