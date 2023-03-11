#include "ServerList.h"

#ifdef CHINA_LOCALIZING

static LPCSTR __GetFtpServer(int iRegionID)
{
	// get ftp server url from ini file.
	static char szRegionFtpUrl[0xff]={0,};

	// get region names!
	char szKey[0xff];
	sprintf(szKey, "ftp_%02d_url", iRegionID);
	GetPrivateProfileString("PATCH_FTP_INFO", szKey, "", szRegionFtpUrl, 0xff, "./Region.ini");

	return szRegionFtpUrl;
}

LPCSTR GetFtpServerNameChina()
{
	int		iRegionID = -1;
	LPCSTR* ppArgv = (LPCSTR*) __argv;
	int		nArgc = __argc;	

	for(int i = 1; i < nArgc; i++)
	{
		if(	(_strnicmp(ppArgv[i], "-region:", 8) == 0) ||
			(_strnicmp(ppArgv[i], "/region:", 8) == 0) )
		{
			iRegionID = atoi(ppArgv[i] + strlen("/region:"));
			break;
		}
	}

	// Get FTP URL of Selected Region.
	return (iRegionID == -1) ? NULL : __GetFtpServer(iRegionID);
}

#endif