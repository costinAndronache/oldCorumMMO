#pragma once

#include "./ZipArchive/ZipArchive.h"

///////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////

enum APP_PROGRESS	{
						AP_INITIALIZED = 0,
						AP_CONNECT,
						AP_CONNECT_OK,
						AP_GET_DIRECTORY,
						AP_GET_NOTICE_INFO,
						AP_GET_NOTICE_INFO_OK,
						AP_DOWNLOAD_NOTICE,
						AP_DOWNLOAD_NOTICE_OK,
						AP_GET_VER_INFO,
						AP_GET_VER_INFO_OK,
						AP_DOWNLOAD_ZIP,
						AP_DOWNLOAD_ZIP_OK,
						AP_EXTRACT,
						AP_CHECK_PACK,
						AP_PACK,
						AP_COMPLETED
					};

char* const APS[] =	{
						"Initialized.",
						"Connect to Server",
						"Connect to Server - OK.",
						"GetDirectory",
						"Get Notice Info.",
						"Get Notice Info - OK.",
						"Download Notice",
						"Download Notice - OK.",
						"Get Ver Info",
						"Get Ver Info - OK.",
						"Download UpdateFile",
						"Download UpdateFile - OK.",
						"Extract",
						"Check Pack",
						"Pack",
						"Ready to Start."
					};

enum ERROR_INFO		{
						EI_SHUTDOWN = 0,
						EI_CHECK_PACK,
						EI_PACK,
						EI_COMPLETED
					};

char* const EIS[] =	{
						"Failed!",
						"Check Pack",
						"Pack",
						"Ready to Start."
					};







char* const g_aszPackDir[] =	{
									"\\data\\Character",
									"\\data\\DamageNumber",
									"\\data\\Effect",
									"\\data\\Item",
									"\\data\\Map_chr",
									"\\data\\Map_dds",
									"\\data\\Map_light",
									"\\data\\Map_stm",
									"\\data\\Map_tga",
									"\\data\\Map_tif",
									"\\data\\Monster",
									"\\data\\Npc",
									"\\data\\UI"
								};

char* const g_aszPackFile[] =	{
									"Character.pak",
									"DamageNumber.pak",
									"Effect.pak",
									"Item.pak",
									"Map_chr.pak",
									"Map_dds.pak",
									"Map_light.pak",
									"Map_stm.pak",
									"Map_tga.pak",
									"Map_tif.pak",
									"Monster.pak",
									"Npc.pak",
									"UI.pak"
								};
