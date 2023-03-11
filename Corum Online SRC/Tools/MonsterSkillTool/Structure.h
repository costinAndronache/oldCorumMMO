#ifndef __STRUCTURE_H__
#define __STRUCTURE_H__

#pragma once

#pragma pack(push, 1)
struct SKILL_INFO
{
	BYTE	btSkillID;	// 
	BYTE	btLev;		// 1 ~ 50

	SKILL_INFO()
	{
		ZeroMemory(this, sizeof(SKILL_INFO));
	}

	BOOL	SetLevel(BYTE btLevel)	
	{ 
		if(btLev > 50) return FALSE;
		
		btLev = btLevel;
		return TRUE;
	}

	BYTE	GetLevel()
	{
		return btLev;
	}

	void	SetID(BYTE btID)
	{
		btSkillID = btID;
	}

	BYTE	GetID()
	{
		return btSkillID;
	}
};

struct MONSTER_SKILL_INFO
{
	int			Serial_Num;
	char		szName[50];
	WORD		wdNon_Skill_Rate;
	WORD		wdUse_Skill_Rate;
	SKILL_INFO	SkillInfo[10];

	MONSTER_SKILL_INFO()
	{
		ZeroMemory(this, sizeof(MONSTER_SKILL_INFO));
	}

	int			GetMonsterID()
	{
		return Serial_Num;
	}

	LPCSTR		GetMonsterName()
	{
		return szName;
	}

	BOOL		SetNoneSkillRate(WORD wdRate)
	{
		if(wdRate > 32767) return FALSE;

		wdNon_Skill_Rate = wdRate;
		return TRUE;
	}

	BOOL		SetUseSkillRate(WORD wdRate)
	{
		if(wdRate > 32767) return FALSE;

		wdUse_Skill_Rate = wdRate;
		return TRUE;
	}

	BOOL		SetSkillInfo(BYTE btIndex, WORD wdSkillID, BYTE btLevel)
	{
		if(btIndex > 14) return FALSE;

		SkillInfo[btIndex].SetID(wdSkillID);
		return SkillInfo[btIndex].SetLevel(btLevel);
	}

	WORD		GetNoneSkillRate()
	{
		return wdNon_Skill_Rate;
	}

	WORD		GetUseSkillRate()
	{
		return wdUse_Skill_Rate;
	}

	SKILL_INFO*	GetSkillInfo(BYTE btIndex)
	{
		return &SkillInfo[btIndex];
	}
};

struct SKILL_DESC
{
	int		iSkillID;
	char	szName[50];

	SKILL_DESC()
	{
		ZeroMemory(this, sizeof(SKILL_DESC));
	}

	int		GetSkillID()	{ return iSkillID;		}
	LPCSTR	GetSkillName()	{ return (LPCSTR)szName;}
};

struct SKILL_DESC_STRUCT
{
	SKILL_DESC* pSkillDesc;
	int			nCount;

	LPCSTR GetSkillName(int iSkillID)
	{
		for(int i = 0; i < nCount; i++)
		{
			if(pSkillDesc[i].GetSkillID() == iSkillID)
				return pSkillDesc[i].GetSkillName();
		}
		
		return NULL;
	}
};

#pragma pack(pop)

#endif // __STRUCTURE_H__