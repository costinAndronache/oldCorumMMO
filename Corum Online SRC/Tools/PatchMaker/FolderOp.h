#ifndef __FOLDER_OP_H__
#define __FOLDER_OP_H__

#pragma once

void	GetFolderPath(LPCTSTR szDefaultFolder, OUT LPTSTR szFolder);	// 폴더 선택하는 다이얼로그 나오게 함
BOOL	CopyFolderEx(LPCTSTR szSource, LPCTSTR szTarget);				// 폴더째 복사
BOOL	CopyFileEx(LPCTSTR szSource, LPCTSTR szTarget);					// 다수의 파일을 복사
void	DeleteFolderEx(LPCTSTR szFolder);								// 폴더째 삭제
void	SetFileAttributeEx(LPCTSTR szPath, DWORD dwAttr);				// 폴더의 모든 파일 속성을 변경
void	SetCurrentDirectoryToFolderOfExcutable();						// EXE의 위치로 현재 폴더 변경
BOOL	CheckFolderExistance(LPCTSTR szFolder);							// 폴더가 존재하는 지 확인
void	GetFileTitle(LPCTSTR szPath, OUT LPTSTR szTitle);				// PATH에서 FILE TITLE 추출
void	SearchAndDeleteFiles(LPCTSTR szFolder, LPCTSTR szFile);			// 특정 format의 파일들을 제거
BOOL	IsThereFile(LPCTSTR szFile);									// 해당 파일이 존재하는가?
FILETIME GetFileModTime(LPCTSTR szFilePath);							// 파일의 수정 시간을 구한다.

LPCSTR Convert2Multi(LPCWSTR wszSource);

#endif // __FOLDER_OP_H__