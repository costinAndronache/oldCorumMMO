#ifndef __EXCEPTION_FILTER_H__
#define __EXCEPTION_FILTER_H__

#pragma once


////////////////////////////////////////////////////////////////////////////
LONG __stdcall ExceptionFilter(_EXCEPTION_POINTERS* pExceptionInfo);

#define EXCEPTION_FILTER()	ExceptionFilter(GetExceptionInformation())

extern CRITICAL_SECTION g_ExceptionCS;

#endif // __EXCEPTION_FILTER_H__
