#ifdef COMMONSERVER_EXPORTS
#	define DECLSPECIFIER __declspec(dllexport)//���׸� �Ѱ����� define���� ���� �Ͽ����ϴ�. DECLSPECIFIER --> DECLSPECIFIER : 050102 Hwoarang
#	define EXPIMP_TEMPLATE
#else
#	define DECLSPECIFIER __declspec(dllimport)
#	define EXPIMP_TEMPLATE extern
#endif

#include "CommonDungeon.h"