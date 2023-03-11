#ifndef __FOLDER_OP_H__
#define __FOLDER_OP_H__

#pragma once

void	GetFolderPath(LPCTSTR szDefaultFolder, OUT LPTSTR szFolder);	// ���� �����ϴ� ���̾�α� ������ ��
BOOL	CopyFolderEx(LPCTSTR szSource, LPCTSTR szTarget);				// ����° ����
BOOL	CopyFileEx(LPCTSTR szSource, LPCTSTR szTarget);					// �ټ��� ������ ����
void	DeleteFolderEx(LPCTSTR szFolder);								// ����° ����
void	SetFileAttributeEx(LPCTSTR szPath, DWORD dwAttr);				// ������ ��� ���� �Ӽ��� ����
void	SetCurrentDirectoryToFolderOfExcutable();						// EXE�� ��ġ�� ���� ���� ����
BOOL	CheckFolderExistance(LPCTSTR szFolder);							// ������ �����ϴ� �� Ȯ��
void	GetFileTitle(LPCTSTR szPath, OUT LPTSTR szTitle);				// PATH���� FILE TITLE ����
void	SearchAndDeleteFiles(LPCTSTR szFolder, LPCTSTR szFile);			// Ư�� format�� ���ϵ��� ����
BOOL	IsThereFile(LPCTSTR szFile);									// �ش� ������ �����ϴ°�?
FILETIME GetFileModTime(LPCTSTR szFilePath);							// ������ ���� �ð��� ���Ѵ�.

LPCSTR Convert2Multi(LPCWSTR wszSource);

#endif // __FOLDER_OP_H__