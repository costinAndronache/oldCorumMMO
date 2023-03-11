#pragma once

#include "./ZipArchive/ZipArchive.h"


#if defined CHINA_LOCALIZING
#	define FTP_IP1			"corum-patch01.9you.com"
#	define FTP_IP2			"corum-patch02.9you.com"
#	define FTP_PORT			21
#	define FTP_USER			""			
#	define FTP_PASSWORD		""	
#	define FTP_FOLDER		"./corum/patch"

#elif defined JAPAN_LOCALIZING
#	define FTP_IP			"corumjp-auto.cachenet.com"	
#	define FTP_PORT			21
#	define FTP_USER			""			
#	define FTP_PASSWORD		""
#	define FTP_FOLDER		"./auto-patch"

#elif defined TAIWAN_LOCALIZING
#	define FTP_IP			"ftp-1.saga-city.com"	
#	define FTP_PORT			21
#	define FTP_USER			"corum-patch"			
#	define FTP_PASSWORD		"corum-patch"
#	define FTP_FOLDER		""

#elif defined TAIWAN_LOCALIZING_TEST
#	define FTP_IP			"ftp-1.saga-city.com"	
#	define FTP_PORT			21
#	define FTP_USER			"test-patch"			
#	define FTP_PASSWORD		"sagacity"
#	define FTP_FOLDER		""

#elif defined TAIWAN_LOCALIZING_DOWNLOAD_TEST
#	define FTP_IP			"ftp-1.saga-city.com"	
#	define FTP_PORT			21
#	define FTP_USER			"sagacity"			
#	define FTP_PASSWORD		"corum"
#	define FTP_FOLDER		""

#elif defined TEST_SERVER
#	define FTP_IP			"218.234.23.100"	
#	define FTP_PORT			21
#	define FTP_USER			"corumuser2"			
#	define FTP_PASSWORD		"#zhfnadbwj2#"
#	define FTP_FOLDER		""

#else
#	define FTP_IP			"218.234.23.100"	
#	define FTP_PORT			21
#	define FTP_USER			"corumuser1"
#	define FTP_PASSWORD		"#zhfnadbwj1#"
#	define FTP_FOLDER		""
#	define KOREAN_VERSION
#endif

#ifdef CHINA_LOCALIZING
#	define USE_SUBFOLDER
#elif defined JAPAN_LOCALIZING
#	define USE_SUBFOLDER
#endif

#ifdef CHINA_LOCALIZING
	LPCSTR GetFtpServerNameChina(); // function for getting suitable ftp url.
#endif












