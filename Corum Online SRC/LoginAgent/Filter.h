#ifndef __FILTER_H__
#define __FILTER_H__


#pragma once

#define		MAX_FILTER_WORDS	1024

#pragma pack(push, 1)
struct FILTER_WORDS  
{
	DWORD	dwIndex;
	char	szFilter[255];
};
#pragma pack(pop)

// ���� �ʱ�ȭ!
void InitLimitedWords();

// ���� �ܾ� üũ!
BOOL FilterWordsIncluded(LPSTR szString);



#endif // __FILTER_H__