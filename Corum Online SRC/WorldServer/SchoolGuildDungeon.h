#ifndef	__SCHOOL_GUILD_DUNGEON_H__
#define	__SCHOOL_GUILD_DUNGEON_H__


#include "../CommonServer/AbyssHash.h"


typedef struct __SchoolGuildDungeon 
{
	DWORD	dwDungeonID;
	char	szSchoolName[MAX_CHARACTER_NAME_LENGTH];
} SCHOOL_GUILD_DUNGEON, *LP_SCHOOL_GUILD_DUNGEON;

class CSchoolGuildDungeonHash : public CAbyssHash<SCHOOL_GUILD_DUNGEON>
{
public:

};

void InitSchoolGuildDungeon(); // ���� ��� ���� �̸��� �ε��Ѵ�.

extern CSchoolGuildDungeonHash*	g_pSchoolGuildDungeonTableHash;
extern SCHOOL_GUILD_DUNGEON g_pGuildDungeon[50];

#endif 

