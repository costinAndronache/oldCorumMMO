#pragma once
#include <windows.h>
#include <windowsx.h>
#include <memory>

class CMainUser;

class CMainUserUpdateInterested {
public:
	virtual void updatedCurrentHP(CMainUser*, DWORD oldValue, DWORD newValue) {}
	virtual void updatedCurrentSP(CMainUser*, DWORD oldValue, DWORD newValue) {}
	virtual void updatedMAXHP(CMainUser*, DWORD oldValue, DWORD newValue) {}
	virtual void updatedMAXSP(CMainUser*, DWORD oldValue, DWORD newValue) {}
	virtual void updatedEXP(CMainUser*, DWORD oldValue, DWORD newValue) {}
	virtual void updatedStatPoints(CMainUser*, DWORD oldValue, DWORD newValue) {}
	virtual void updatedSkillPoints(CMainUser*, DWORD oldValue, DWORD newValue) {}
	virtual void updatedCoolPoints(CMainUser*, float oldValue, float newValue) {}
	virtual void updatedLevel(CMainUser*, DWORD oldValue, DWORD newValue) {}
	virtual void updatedSkills(CMainUser*) {}
};

typedef std::weak_ptr<CMainUserUpdateInterested> CMainUserUpdateInterestedWeakRef;
typedef std::shared_ptr<CMainUserUpdateInterested> CMainUserUpdateInterestedSharedRef;