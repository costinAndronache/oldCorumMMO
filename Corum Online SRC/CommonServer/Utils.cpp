#include "stdafx.h"
#include "Utils.h"

COMMONSERVERDECLSPECIFIER const char* codeToString(USER_PROPERTY_CODE code) {
	switch (code)
	{
	case USER_MAXHP: return "MAX_HP";
		break;
	case USER_MAXMP: return "MAX_MP";
		break;
	case USER_HONOR: return "USER_HONOR";
		break;
	case USER_EGO:	return "USER_EGO";
		break;
	case USER_STR:	return "USER_STR";
		break;
	case USER_INT:	return "USER_INT";
		break;
	case USER_DEX:	return "USER_DEX";
		break;
	case USER_VIT:	return "USER_VIT";
		break;
	case USER_LUCK:	return "USER_LUCK";
		break;
	case USER_ATTACK_R: return "USER_ATTACK_R";
		break;
	case USER_ATTACK_L: return "USER_ATTACK_L";
		break;
	case USER_AA: return "USER_AA";
		break;
	case USER_AVOID: return "USER_AVOID";
		break;
	case USER_BLOCKRATE: return "USER_BLOCKRATE";
		break;
	case USER_FIRE_RESIST: return "USER_FIRE_RESIST";
		break;
	case USER_ICE_RESIST: return "USER_ICE_RESIST";
		break;
	case USER_LIGHT_RESIST: return "USER_LIGHT_RESIST";
		break;
	case USER_POI_RESIST: return "USER_POI_RESIST";
		break;
	case USER_PHY_RESIST: return "USER_PHY_RESIST";
		break;
	case USER_MOVESPEED: return "USER_MOVESPEED";
		break;
	case USER_EXP: return "USER_EXP";
		break;
	case USER_HP: return "USER_HP";
		break;
	case USER_MP: return "USER_MP";
		break;
	case USER_CASTINGTIME: return "USER_CASTINGTIME";
		break;
	case USER_COOLTIME: return "USER_COOLTIME";
		break;
	case USER_ATTACKRATE_PERCENT: return "USER_ATTACKRATE_PERCENT";
		break;
	case USER_ATTACKACURACY_PERCENT: return "USER_ATTACKACURACY_PERCENT";
		break;
	case USER_OVERDRIVE_DELAY: return "USER_OVERDRIVE_DELAY";
		break;
	case USER_HEALHPSPEED: return "USER_HEALHPSPEED";
		break;
	case USER_HEALMPSPEED: return "USER_HEALMPSPEED";
		break;
	case USER_POISONING: return "USER_POISONING";
		break;
	case USER_DEC_SKILLLEVEL: return "USER_DEC_SKILLLEVEL";
		break;
	case USER_SKILLATTACK_R_PERCENT: return "USER_SKILLATTACK_R_PERCENT";
		break;
	case USER_SKILLATTACK_L_PERCENT: return "USER_SKILLATTACK_L_PERCENT";
		break;
	case USER_CTTIME: return "USER_CTTIME";
		break;
	case USER_ALL_RESIST: return "USER_ALL_RESIST";
		break;
	case USER_MAX_RESIST: return "USER_MAX_RESIST";
		break;
	case USER_MOVESPEED_PERCENT: return "USER_MOVESPEED_PERCENT";
		break;
	case USER_HEALHPSPEED_PERCENT: return "USER_HEALHPSPEED_PERCENT";
		break;
	case USER_PHY_RESIST_PERCENT: return "USER_PHY_RESIST_PERCENT";
		break;
	case USER_MP_PERCENT: return "USER_MP_PERCENT";
		break;
	case USER_MAXHP_PERCENT: return "USER_MAXHP_PERCENT";
		break;
	case USER_ATTACK_R_PERCENT: return "USER_ATTACK_R_PERCENT";
		break;
	case USER_BLOCKRATE_PERCENT: return "USER_BLOCKRATE_PERCENT";
		break;
	case USER_AVOID_PERCENT: return "USER_AVOID_PERCENT";
		break;
	case USER_HP_PERCENT: return "USER_HP_PERCENT";
		break;
	case USER_POI_RESIST_PERCENT: return "USER_POI_RESIST_PERCENT";
		break;
	case USER_ITEM_ATTACK: return "USER_ITEM_ATTACK";
		break;
	case USER_ITEM_DEFENSE: return "USER_ITEM_DEFENSE";
		break;
	case USER_BLIND: return "USER_BLIND";
		break;
	case USER_MAXMP_PERCENT: return "USER_MAXMP_PERCENT";
		break;
	case USER_ATTACK_L_PERCENT: return "USER_ATTACK_L_PERCENT";
		break;
	case USER_DETECTION: return "USER_DETECTION";
		break;
	case USER_MAX_STATUS: return "USER_MAX_STATUS";
		break;
	default:
		return "unknown-code";
		break;
	}
}

COMMONSERVERDECLSPECIFIER void printUserStatusList(const USER_STATUS* statusList, int count) {
	if (count <= 0 || !statusList) {
		return;
	}

	for (int i = 0; i < count; i++) {
		const auto current = statusList[i];
		printf("%s: %u - %u\n", codeToString((USER_PROPERTY_CODE)current.dwCode), current.dwMin, current.dwMax);
	}
}