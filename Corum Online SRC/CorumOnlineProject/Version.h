#ifndef __VERSION_H__
#define __VERSION_H__

#pragma once

// client�� ������ ����!!
// ���ӿ��� ������ ����ϴ� ���� CURRENT_VERSION_STRING �̴�.
// �� �༮�� �̾Ƴ��� ���ؼ��� UUEXXE ���� ����Ѵ�.
// �� ���� 45 FTP ������ MAIN ������ ����!
// ���� ���� ���Ǵ� DEEPDARK����!

#define CURRENT_VERSION_STRING	"KJ-VTJa+KLtNI33BHJpBHE2+"//05033101

#if IS_TAIWAN_LOCALIZING()
	#undef CURRENT_VERSION_STRING
	#define CURRENT_VERSION_STRING	"KJ-VTJa+KLtNI33BHJpBHE2+"//05033101
#elif IS_CHINA_LOCALIZING()
	#undef CURRENT_VERSION_STRING
	#define CURRENT_VERSION_STRING	"KJ-VTJa+KLtNI33BHJpBHE2+"//05033101
#elif IS_JAPAN_LOCALIZING()
	#undef CURRENT_VERSION_STRING
	#define CURRENT_VERSION_STRING	"KJ-VTJa+KLtNI33BHJpBHE2+"//05033101
#endif

#endif // __VERSION_H__