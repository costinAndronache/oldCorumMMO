#pragma once

#ifdef COMMONSERVER_EXPORTS
#	define COMMONSERVERDECLSPECIFIER __declspec(dllexport)//���׸� �Ѱ����� define���� ���� �Ͽ����ϴ�. COMMONSERVERDECLSPECIFIER --> COMMONSERVERDECLSPECIFIER : 050102 Hwoarang
#	define EXPIMP_TEMPLATE
#else
#	define COMMONSERVERDECLSPECIFIER __declspec(dllimport)
#	define EXPIMP_TEMPLATE extern
#endif