#pragma once
//#include "stdafx.h"
#include "global_define.h"
#include "getdxver.h"
#include "cpu_info.h"


//

#define MAXPATH		512 

enum eGRAPHIC_CARD_OPTION
{
	NORMAL				,	// nvidia / 이상없는 그래픽 카드 
	RADEON_7500_UNDER	,	// RADEON 7500 UNDER CHIP SERIES	
	RADEON_7500_ABOVE	,	// RADEON 7500 상위 CHIP SERIES 
	MATROX				,	// MATROX CHIP
	VOODOO              ,	// VOODOO CHIP
	POOR_CARD			,	// 그래픽 카드의 메모리가 32 메가 미만 이다.
	OPTION_NUM				// 옵션 개수 / 오류일때도 사용한다. 
};

#define NVIDIA_VENTDER_ID	4318
#define ATI_VENTDER_ID		4098
#define MATROX_VENTDER_ID	4139
#define VOODOO_VENDER_ID	4634

#define NVIDIA_DRIVER_ENOUGH_VER	4109
#define	ATI_DRIVER_ENOUGH_VER		6343  


int		GetGraphicDrvVerFromString( char * pszString );
int		SpecGrapicCard( char * pszGraphic, 
			char * pszGraphicDriverVersion, 
			char * pszDisplayMode );
BOOL	IsCompressedTextureFormatOk();
enum	eGRAPHIC_CARD_OPTION IdentityGraphicCard();
