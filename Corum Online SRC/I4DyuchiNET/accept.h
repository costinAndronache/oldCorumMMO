#include "stdafx.h"


class CNetwork;

BOOL StartAcceptThread( CNetwork* pNetwork );
BOOL EndAcceptThread( SOCKET socketListen, HANDLE hAcceptThread );