///////////////////////////////////////////////////////////////////////////
// Sound
#include "InitGame.h"
#include "DungeonProcess.h"
#include "GameMenuWnd.h"
#include "GameControl.h"
#include "DungeonTable.h"
#include "CodeFun.h"

int GetDungeonLayerProperty(DWORD dwLayerID)
{
	int iProperty=0;

	if (dwLayerID < 100)
	{ // ���� 
		iProperty = Dungeon_Layer_Property_Village;
	}
	else if (dwLayerID >= 100 && dwLayerID < 200)
	{ // ��� ���� 
		iProperty = Dungeon_Layer_Property_Ancient;
	}
	else if (dwLayerID >= 200 && dwLayerID < 300)
	{ // ���ϼ��� ���� 
		iProperty = Dungeon_Layer_Property_Underground_Waterway;
	}
	else if (dwLayerID >= 300 && dwLayerID < 401)
	{ // �� ����
		iProperty = Dungeon_Layer_Property_Castle;
	}
	else if (dwLayerID >= 401 && dwLayerID < 500)
	{ // ���� ����
		iProperty = Dungeon_Layer_Property_Underground;
	}
	else if (dwLayerID >= 500 && dwLayerID < 600)
	{ // ���̽� ���� 
		iProperty = Dungeon_Layer_Property_Ice;
	}
	else if (dwLayerID >= 600 && dwLayerID < 700)
	{ // �� ����
		iProperty = Dungeon_Layer_Property_Forest;
	}
	else if (dwLayerID == 750)
	{ // �巡���� ����
		iProperty = Dungeon_Layer_Property_Ameritart;
	}
	else if (dwLayerID == 801)
	{ // ����� �ʵ�
		iProperty = Dungeon_Layer_Property_GuildWar_Field;
	}
	else if (dwLayerID >= 900 && dwLayerID < 1000)
	{ // ��� ���� 
		iProperty = Dungeon_Layer_Property_Lava;
	}
	else if (dwLayerID >= 1100 && dwLayerID < 1200)
	{ // ž
		iProperty = Dungeon_Layer_Property_Towner;
	}
	else if (dwLayerID >= 1200 && dwLayerID < 1220)
	{ // ���ߴ���
		iProperty = Dungeon_Layer_Property_Aqua;
	}
	else if (dwLayerID >= 1221 && dwLayerID < 1230)
	{ // ������
		iProperty = Dungeon_Layer_Property_Light_House;
	}

	return iProperty;
}

// ==================================
// Name		: _PlaySound
// Desc		: 3d ���带 �÷����ϴ� �Լ� 
// return	: ���� �����ϰ�쿡�� ����� �����ϰ� �������� NULL ���� �����Ѵ�. 
// Caution	: ������ ��쿣 ��Ȯ�� Stop �ؾ��� ������ ��������Ѵ�!!!
//			  �ܹ߼��� �˾Ƽ� ���߱� ������ �������.
//														by seven
// ==================================
ISoundEffect * _PlaySound( DWORD dwSoundIndex, DWORD dwType, DWORD dwKind, VECTOR3 & v3Pos, bool bLoop)
{		
	if (CGameMenuWnd::GetInstance()->m_bEffectFlag == FALSE)
		return NULL;

	if (!g_pSoundLib)
		return NULL;

	char				szBuf[255] = {0,};
//	int					iMoveSound = 0;
//	SOUND_ERROR_CODE	Result = SOUND_ERROR_NOERROR;
	ISoundEffect *		pSoundEffect = NULL;
	SOUND_FILE_HANDLE *	phFile = NULL;

	switch( dwType )
	{	
	case SOUND_TYPE_ITEM:
		{
			wsprintf(szBuf, "%d.wav", dwKind+5000);
			GetSoundEffectFile(&g_SoundLibHandles.hSoundItemFile[dwKind], 
				GetFile(szBuf, DATA_TYPE_SOUND));
			phFile = &g_SoundLibHandles.hSoundItemFile[dwKind];
		}										
		break;

	case SOUND_TYPE_SYSTEM:
		{
			//if ( dwKind == SOUND_SYSTEM_WNDCLOSE && GetGameStatus() == UPDATE_GAME_LOGIN)  return NULL; // �α��ν� ���ϴ� ���� ����Ʈ �ȳ����� ���Ѵ�. �迵�� 2003.02.22
			wsprintf(szBuf, "%d.wav", dwKind+4000);																
			GetSoundEffectFile(&g_SoundLibHandles.hSoundSystemFile[dwKind], 
													GetFile(szBuf, DATA_TYPE_SOUND));
			phFile = &g_SoundLibHandles.hSoundSystemFile[dwKind];
		}
		break;

	case SOUND_TYPE_GAMEPLAY:	
		if( dwSoundIndex == GAMEPLAY_HEABYSTONE_WALK ) 
		{
			dwKind = GAMEPLAY_HEABYSTONE_WALK + GetRandom(4);
		}
		else if ( dwSoundIndex == GAMEPLAY_HEABYSTONE_RUN )
		{
			dwKind = GAMEPLAY_HEABYSTONE_RUN + GetRandom(4);
		}
			
		wsprintf( szBuf, "%d.wav", dwKind+2000 );
		GetSoundEffectFile(&g_SoundLibHandles.hSoundGamePlayFile[dwKind], 
			GetFile(szBuf, DATA_TYPE_SOUND));

		phFile = &g_SoundLibHandles.hSoundGamePlayFile[dwKind];
		break;

	case SOUND_TYPE_CHARACTER:
		{
			switch( dwSoundIndex )
			{
			case CHARACTER_SOUND_ATTACK:
				dwKind += GetRandom(3);
				break;
			
			case CHARACTER_SOUND_DAMAGE:
				dwKind += GetRandom(3);
				break;

			case CHARACTER_SOUND_DEAD:
				dwKind += GetRandom(2);
				break;

			default: 
				return pSoundEffect;
			}

			wsprintf( szBuf, "%d.wav", dwKind+SOUND_NUMBER_CHARACTER );
			GetSoundEffectFile(&g_SoundLibHandles.hSoundPlayerFile[dwKind], 
				GetFile(szBuf, DATA_TYPE_SOUND));

			phFile = &g_SoundLibHandles.hSoundPlayerFile[dwKind];
		}
		break;


	case SOUND_TYPE_WEAPONATTACK:
		{
			dwKind += GetRandom(3);

			wsprintf( szBuf, "%d.wav", dwKind+SOUND_NUMBER_WEAPON );
			GetSoundEffectFile(&g_SoundLibHandles.hSoundWeaponFile[dwKind], 
				GetFile(szBuf, DATA_TYPE_SOUND));

			phFile = &g_SoundLibHandles.hSoundWeaponFile[dwKind];
		}
		break;

	case SOUND_TYPE_WEAPONDAMAGE:
		{
			switch( dwSoundIndex )
			{
			case WEAPONDAMAGE_SOUND_DAMAGE:
				dwKind += GetRandom(2);
				break;

			case WEAPONDAMAGE_SOUND_CRI:
				dwKind += GetRandom(2) + WEAPONDAMAGE_SOUND_CRI;
				break;

			case WEAPONDAMAGE_SOUND_BLOCK:
				dwKind += GetRandom(2) + WEAPONDAMAGE_SOUND_BLOCK;
				break;
			}

			wsprintf( szBuf, "%d.wav", dwKind+SOUND_NUMBER_WEAPONDAMAGE );

			GetSoundEffectFile(&g_SoundLibHandles.hSoundWeaponDamageFile[dwKind], 
				GetFile(szBuf, DATA_TYPE_SOUND));

			phFile = &g_SoundLibHandles.hSoundWeaponDamageFile[dwKind];
		}
		break;
	}

	if (phFile)
	{
		if ( v3Pos == g_v3InterfaceSoundPos )
		{ // �������� �������̽��� �׳� ó��.
			goto default_sound_process;
		}

		if (g_pMainPlayer)
		{
			float fDis = CalcDistance(&g_pMainPlayer->m_v3CurPos, &v3Pos);
			if ( fDis >= SOUND_LISTEN_SCOPE_DISTANCE )
			{
				return NULL;
			}
		}

default_sound_process:
		GetSoundEffect(&pSoundEffect, *phFile, v3Pos);
		PlaySoundEffect(pSoundEffect, &v3Pos, bLoop);

		g_SoundEffectBasketCase.Add(pSoundEffect);
	}

	// �� �����ʹ� ������ ���
	// Stop �� �߽�Ű�� �ȴ�. 
	// ���� �ܹ߼��϶� �ƹ��͵� ���ص��ȴ�.
	return pSoundEffect;		
}

void PlayItemSound(WORD	wItemId)
{
	if ( !g_pSoundLib )
		return; 
	
	DWORD	dwKind	= 0;
	int		nVal	= wItemId/ITEM_DISTRIBUTE;

	if(GetType((WORD)nVal) == ITEM_LARGE)
	{
		switch(nVal)
		{
			case 0:		dwKind = SOUND_ITEM_SWORD;	break;
			case 1:		dwKind = SOUND_ITEM_MACE;	break;
			case 2:		dwKind = SOUND_ITEM_KATAR;	break;
			case 3:		dwKind = SOUND_ITEM_SWORD2;	break;
			case 4:		dwKind = SOUND_ITEM_MACE2;	break;
			case 5:		dwKind = SOUND_ITEM_SPEAR;	break;
			case 6:		dwKind = SOUND_ITEM_WAND;	break;
			case 7:		dwKind = SOUND_ITEM_STAFF;	break;
			case 10:	dwKind = SOUND_ITEM_HELM;	break;
			case 11:	dwKind = SOUND_ITEM_ARMOR;	break;
			case 12:	dwKind = SOUND_ITEM_SHIELD;	break;
			case 13:	dwKind = SOUND_ITEM_BELT;	break;
			case 14:	dwKind = SOUND_ITEM_GLOVE;	break;
			case 15:	dwKind = SOUND_ITEM_BOOTS;	break;
			case 16:	dwKind = SOUND_ITEM_RING;	break;
			case 17:	dwKind = SOUND_ITEM_AMULET;	break;
			default:								break;
		}
	}
	else if(GetType((WORD)nVal) == ITEM_SMALL)
	{
		if(nVal>=ITEM_SPECIALST_INDEX && nVal<=ITEM_SPECIALEND_INDEX)
		{
			dwKind = SOUND_ITEM_ZODIAC;
		}
		else if(nVal==ITEM_SUPPLIES_INDEX)
		{
			dwKind = SOUND_ITEM_POSION;
		}
		else if(nVal>=ITEM_MIXUPGRADEST_INDEX && nVal<=ITEM_MIXUPGRADEEND_INDEX)
		{
			if(nVal==31)
				dwKind = SOUND_ITEM_GEM;
			else if(nVal==32)
				dwKind = SOUND_ITEM_ELIXIR;
			else
				dwKind = SOUND_ITEM_JEWEL;
		}
		else if(nVal==ITEM_ZODIAC_INDEX ||
				nVal==ITEM_MAGICFIELDARRAY_INDEX ||
				(nVal>=ITEM_CONSUMABLEST_INDEX && nVal<=ITEM_CONSUMABLEEND_INDEX) ||
				nVal==ITEM_MAGICARRAY_INDEX ||
				nVal==ITEM_PRESENT_BOX_INDEX)
		{
			dwKind = SOUND_ITEM_ZODIAC;
		}
		else if(nVal>=ITEM_MATERIALST_INDEX && nVal<=ITEM_MATERIALEND_INDEX)
		{
			if(nVal==70)
				dwKind = SOUND_ITEM_LEATHER;
			else if(nVal==71)
				dwKind = SOUND_ITEM_WOOD;
			else
				dwKind = SOUND_ITEM_METAL;
		}
	}

	if(dwKind!=0)
	{
		_PlaySound(0, SOUND_TYPE_ITEM, dwKind, g_v3InterfaceSoundPos, FALSE);
	}
}


/*
sound_handle_index_t PlaySound( DWORD dwSoundIndex, DWORD dwType, DWORD dwKind / * = 0  * /)
{
	sound_handle_index_t hBuf;
	return hBuf;
}

*/


// ==================================
// Name		: SelectBGM(WORD wID, BYTE byteDungeonLayer)
// Desc		: ������ ���� / ���� ID , ������ �°� �÷��� �����ִ��Լ�
// return	: void
// Caution	: 
// Date		: 04/02/17			  
// 													by seven
// ==================================
void SelectBGM(WORD wID, BYTE byteDungeonLayer, DWORD dwLayerID)
{
	if (g_pSoundLib == NULL)
		return;

	DUNGEON_LAYER_PROPERTY	eDungeonProperty;

//	char  szBGMFile[255] = {0,};
	
	CGameMenuWnd * pGameMenuWnd = CGameMenuWnd::GetInstance();

	assert(pGameMenuWnd);

	if (wID >= 10000)
	{ // ����    // ���嵵 �ٲ����Ѵ�..
		lstrcpy(pGameMenuWnd->m_szLast_BGM_Name, GetFile( "Map_World1.mp3", DATA_TYPE_SOUND ));
		goto Play_BGM;
	}
	else
	{ // ���� 
		DUNGEON_DATA_EX * pDungeonInfo = g_pDungeonTable->GetDungeonInfo(wID);
		if (pDungeonInfo == NULL)
		{
			if (wID != 800) return;

			lstrcpy(pGameMenuWnd->m_szLast_BGM_Name, GetFile( "Dungeon_Hold1.mp3", DATA_TYPE_SOUND ));
			goto Play_BGM;
		}

		switch (pDungeonInfo->GetDungeonType()) 
		{
			case DUNGEON_TYPE_VILLAGE:
			{ // ���� 			 
				if (byteDungeonLayer != 0)
					goto default_dungeon_bgm_play;

				if (wID == 6)
				{ // ���ϸ���
					PlayBGM(GetFile( "Town_Npc1.mp3", DATA_TYPE_SOUND ));
					return;
				}

				if(wID == 7)
				{ // �̸���Ʈ ����
					PlayBGM(GetFile( "Town_Irnote.mp3", DATA_TYPE_SOUND ));
					return;
				}

				// ���� ���� 1�� �̴�.
				lstrcpy(pGameMenuWnd->m_szLast_BGM_Name, GetFile( "Town_World1.mp3", DATA_TYPE_SOUND ));
			}
			break;

			case DUNGEON_TYPE_NPC:
				{ // NPC ���� 
				if (wID == 10)
				{ // ī�̿��� �� 
					PlayBGM(GetFile( "Town_Npc1.mp3", DATA_TYPE_SOUND ));
					return;
				}
			}
			break;

			case DUNGEON_TYPE_CONQUER:
			case DUNGEON_TYPE_NOT_CONQUER:
			{
				eDungeonProperty = (DUNGEON_LAYER_PROPERTY)GetDungeonLayerProperty(dwLayerID);
				goto PlayBGM_BY_Dungeon_Property;
			}
			break;

			case DUNGEON_TYPE_TUNNEL:
			{
				PlayBGM(GetFile( "Dungeon_Tunnel1.mp3", DATA_TYPE_SOUND ) );
				return;
			}
			break;

			case DUNGEON_TYPE_EVENT:
			{ // Event Dungeon
				PlayBGM(GetFile( "Dungeon_Event.mp3", DATA_TYPE_SOUND) );
				return;
			}
			break;

			case DUNGEON_TYPE_MATCH_STATIDUM:
			{ // ����� �ʵ�
				PlayBGM( GetFile( "guild_battle.mp3", DATA_TYPE_SOUND ));
				return;
			}
			break;

default_dungeon_bgm_play:
			default:	// ������ ��Ʋ�� ��Ī ���߿� ������ ���� Ư������ ������ Ʋ�������̴�.
			{			// But ����� �������� �����ش�.
//				pGameMenuWnd->m_dwBGMId = BGM_DUNGEON_LEVEL1 + GetRandom(3);
				char szBuf[255];
				wsprintf(szBuf, "Dungeon_Hold%d.mp3", GetRandom(3)+1);
				lstrcpy(pGameMenuWnd->m_szLast_BGM_Name, GetFile( szBuf, DATA_TYPE_SOUND ));
			}
			break;
		}		
	}

Play_BGM:
//	PlayBGM(FILE(CGameMenuWnd::GetInstance()->m_dwBGMId));
	PlayBGM(pGameMenuWnd->m_szLast_BGM_Name);
	return;

PlayBGM_BY_Dungeon_Property:

	switch (eDungeonProperty)
	{
		case Dungeon_Layer_Property_Underground_Waterway:
		{ // ���ϼ���
			PlayBGM(GetFile( "Dungeon_Lair1.mp3", DATA_TYPE_SOUND ));
			return;
		}
		break;

		case Dungeon_Layer_Property_Forest:
		{ // �� 
			PlayBGM(GetFile( "Dungeon_Lair2.mp3", DATA_TYPE_SOUND ));
			return;
		}
		break;

		case Dungeon_Layer_Property_Ancient:
		{ // ��� �� 
			PlayBGM(GetFile( "Dungeon_Lair3.mp3", DATA_TYPE_SOUND ));
			return;
		}
		break;

		case Dungeon_Layer_Property_Ice:
		{ // ���̽� 
			PlayBGM(GetFile( "Dungeon_Lair4.mp3", DATA_TYPE_SOUND ));
			return;
		}
		break;

		case Dungeon_Layer_Property_Castle:
		{ // ��
			PlayBGM(GetFile( "Dungeon_Lair5.mp3", DATA_TYPE_SOUND ));
			return;
		}
		break;

		case Dungeon_Layer_Property_Lava:
		{ // ��� 
			PlayBGM(GetFile( "Dungeon_Lair6.mp3", DATA_TYPE_SOUND ));
			return;
		}
		break;

		case Dungeon_Layer_Property_Towner:
		{ // ž 
			PlayBGM( GetFile( "Dungeon_Tower.mp3", DATA_TYPE_SOUND ));
			return;
		}
		break;

		case Dungeon_Layer_Property_Aqua:
		{ // ���ߴ���
			PlayBGM( GetFile( "Dungeon_Aqua.mp3", DATA_TYPE_SOUND ));
			return;
		}
		break;

		case Dungeon_Layer_Property_Ameritart:
		{ // �巡���� ����
			PlayBGM( GetFile( "Dungeon_Ameritart.mp3", DATA_TYPE_SOUND ));
			return;
		}
		break;

		case Dungeon_Layer_Property_Light_House:
		{ // ������
			PlayBGM( GetFile( "Dungeon_Lighthouse.mp3", DATA_TYPE_SOUND ));
			return;
		}
		break;

		case Dungeon_Layer_Property_GuildWar_Field:
		{ // ����� �ʵ�
			PlayBGM( GetFile( "guild_battle.mp3", DATA_TYPE_SOUND ));
			return;
		}
		break;

		default:
		{
			char szBuf[255];
			wsprintf(szBuf, "Dungeon_Hold%d.mp3", GetRandom(3)+1);
//			PlayBGM(FILE(CGameMenuWnd::GetInstance()->m_dwBGMId));
			PlayBGM( GetFile( szBuf, DATA_TYPE_SOUND ));
		}
		break;	
	}
}