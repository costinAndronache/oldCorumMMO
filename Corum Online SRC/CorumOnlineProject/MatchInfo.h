#ifndef __MATCHINFO_H__
#define __MATCHINFO_H__

#include "stdafx.h"
#include "InitGame.h"
#include "SpriteManager.h"

/*
 *	Purpose : ���� ���� ǥ�� / ���� 
 */
class CMatchInfo
{
public:
	enum MATCH_INFO_STRING
	{
		MATCH_INFO_STRING_TIME = 0,			// [���� ���� �ð�]
		MATCH_INFO_STRING_RAMAINED_TIME,	// %ld Sec
		MATCH_INFO_STRING_MAX_COUNT			// ���� ��Ʈ�� �ְ� ī��Ʈ
	};
	
private:
	BOOL		m_bDisplayMatchInfoFlag;
	DWORD		m_dwFirstTick;
	DWORD		m_dwCurTick;
	DWORD		m_dwMatchTimeTick;
	DWORD		m_dwRemainedTick;

	V2_SPRITE	m_MatchInfoBackBoardSpr;

	char		m_szInfo[MATCH_INFO_STRING_MAX_COUNT][0Xff];

public:	
	CMatchInfo();
	~CMatchInfo();

	void	Init();
	void	Release();
	
	void	InitMatchInfoString();

	// Get Funcs
	BOOL	IsDisplayMatch()		const;
	BOOL	IsMatching()			const;
	DWORD	GetElapseTimeTick()		const;
	DWORD	GetRemainSec()			const;
	char *	GetInfoString(MATCH_INFO_STRING eMatchInfo) const;
	IDISpriteObject* GetBackBoardSprite() const;
	
	// Set Funcs
	void	SetDisplayMatchFlag(BOOL bDisplayFlag);
	void	SetMatchTimeTick(DWORD dwMatchTimeTick);
	void	SetCurTick(DWORD dwCurTick);
	void	SetInfoString(MATCH_INFO_STRING eMatchInfo, const char * szMatchInfo);
	void	SetBackBoardSprite(IDISpriteObject* pSpr);
	
	// Render info
	void	RenderInfo();

};


#endif 
// #define __MATCHINFO_H__
