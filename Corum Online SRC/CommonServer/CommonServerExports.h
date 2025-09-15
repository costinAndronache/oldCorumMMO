#pragma once

#ifdef COMMONSERVER_EXPORTS
#	define COMMONSERVERDECLSPECIFIER __declspec(dllexport)//제네릭 한것으로 define명을 변경 하였습니다. COMMONSERVERDECLSPECIFIER --> COMMONSERVERDECLSPECIFIER : 050102 Hwoarang
#	define EXPIMP_TEMPLATE
#else
#	define COMMONSERVERDECLSPECIFIER __declspec(dllimport)
#	define EXPIMP_TEMPLATE extern
#endif