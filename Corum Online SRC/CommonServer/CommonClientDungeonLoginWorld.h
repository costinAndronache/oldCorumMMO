#pragma once

#pragma pack(push,1)
typedef struct BASE_CLASS_INFO
{
	int		iType;
	int		iEGO;
	int		iSTR;
	int		iINT;
	int		iDEX;
	int		iVIT;
	int		iBHP;
	int		iBMP;
	int		iLHP;
	int		iLMP;
	int		iHP;
	int		iMP;
	int		iHPR;
	int		iSPR;
	int		iAA;
	int		iAD;
	int		iMD;
	int		iDP;
	int		iBR;
	int		iDR;
	int		iHR;
	int		iMS;
	int		iAS;
	int		iCS;
	int		iRFire;
	int		iRIce;
	int		iRLighting;
	int		iRPhisics;
	int		iRPoison;
	int		iMax_Aura;
	int		iMax_Magic;
	int		iMax_Divine;
	int		iMax_Chakra;
	int		iMax_Summon;
	WORD	wPotion_Recovery;
	WORD	wPotion_Delay;
	BYTE	byItem_Efficiency;
}*LPBASE_CLASS_INFO;

#pragma pack(pop)