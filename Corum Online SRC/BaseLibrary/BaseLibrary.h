#pragma once

#ifdef  _BASELIBRARYDLLBUILDING
#define BASELIBRARYDECLSPEC __declspec(dllexport)
#else
#define BASELIBRARYDECLSPEC __declspec(dllimport)
#endif

