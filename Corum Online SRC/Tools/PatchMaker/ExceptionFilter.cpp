#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "ExceptionFilter.h"

#pragma pack(push, exception)
#pragma pack(8)
#include <imagehlp.h>
#pragma pack(pop, exception)

#pragma comment(lib, "imagehlp.lib")


// ExceptionFilter() - Exception 발생시 호출되어 Stack 을 찍어준다.
// 예외 내역을 찍어주고 프로세스는 계속 진행된다..
LONG __stdcall ExceptionFilter(_EXCEPTION_POINTERS* pExceptionInfo) 
{
	// Initialize symbols..
	SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME | SYMOPT_LOAD_LINES);
	if(SymInitialize(GetCurrentProcess(), NULL, TRUE) == FALSE) 
		return EXCEPTION_EXECUTE_HANDLER;

	// create dump file..
	FILE* pFile;
	if((pFile = fopen("ExceptionStackDump.txt", "at")) == NULL) 
		return EXCEPTION_EXECUTE_HANDLER;

	char tszTemp[256] = {0,};
	fprintf(pFile, "*============================================================*\n");
	fprintf(pFile, "\tDate / Time\t\t\t: %s   ", _strdate(tszTemp));
	fprintf(pFile, "%s\n", _strtime(tszTemp));
	fprintf(pFile, "\tProcessID / ThreadID: 0x%08X / ", GetCurrentProcessId());
	fprintf(pFile, "0x%08X\n", GetCurrentThreadId());
	fprintf(pFile, "\tExceptionCode\t\t: 0x%08X\n", pExceptionInfo->ExceptionRecord->ExceptionCode);
	fprintf(pFile, "*------------------------------------------------------------*\n");

	HANDLE hProcess		= GetCurrentProcess();
	HANDLE hThread		= GetCurrentThread();
	CONTEXT& context	= *pExceptionInfo->ContextRecord;

	STACKFRAME stackFrame = {0,};
	stackFrame.AddrPC.Offset	= context.Eip;
	stackFrame.AddrPC.Mode		= AddrModeFlat;
	stackFrame.AddrStack.Offset	= context.Esp;
	stackFrame.AddrStack.Mode	= AddrModeFlat;
	stackFrame.AddrFrame.Offset	= context.Ebp;
	stackFrame.AddrFrame.Mode	= AddrModeFlat;

	// stackwalk!!
	for(int i = 0; i < 512; i++) 
	{ 
		if(stackFrame.AddrPC.Offset == 0) break;

		if(StackWalk(	IMAGE_FILE_MACHINE_I386, hProcess, hThread, 
						&stackFrame, &context, NULL, 
						SymFunctionTableAccess, SymGetModuleBase, NULL) != FALSE) 
		{
			DWORD dwDisplacement = 0;
			char chSymbol[256] = {0,};
			PIMAGEHLP_SYMBOL pSymbol = (PIMAGEHLP_SYMBOL)chSymbol;

			pSymbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL);
			pSymbol->MaxNameLength = sizeof(chSymbol) - sizeof(PIMAGEHLP_SYMBOL) + 1;

			if(SymGetSymFromAddr(hProcess, stackFrame.AddrPC.Offset, &dwDisplacement, pSymbol))
				fprintf(pFile, "0x%08x - %s() + %xh\n", 
						stackFrame.AddrPC.Offset, pSymbol->Name, stackFrame.AddrPC.Offset-pSymbol->Address);
			else
				fprintf(pFile, "0x%08x - [Unknown Symbol:Error %u]\n", 
						stackFrame.AddrPC.Offset, GetLastError());

			IMAGEHLP_MODULE module = {sizeof(IMAGEHLP_MODULE), 0,};
			if(SymGetModuleInfo(hProcess, stackFrame.AddrPC.Offset, &module) != FALSE) 
			{
				fprintf(pFile, "\tImageName: %s\n", module.ImageName);
				fprintf(pFile, "\tLoadedImageName: %s\n", module.LoadedImageName);
			}

			IMAGEHLP_LINE line = {sizeof(IMAGEHLP_LINE), 0,};
			for(int i = 0; i < 512; ++i) 
			{
				if(SymGetLineFromAddr(hProcess, stackFrame.AddrPC.Offset - i, &dwDisplacement, &line) != FALSE) 
				{
					fprintf(pFile, "\tFile: %s, %u Line\r\n", line.FileName, line.LineNumber);
					break;
				}
			}
		} 
		else break;
	} 

	fprintf(pFile, "\n");
	fclose(pFile);

	SymCleanup(hProcess);

	return EXCEPTION_EXECUTE_HANDLER;
}
