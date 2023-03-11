#ifndef __EXCEPTION_FILTER_H__
#define __EXCEPTION_FILTER_H__

#pragma once


////////////////////////////////////////////////////////////////////////////
LONG __stdcall ExceptionFilter(_EXCEPTION_POINTERS* pExceptionInfo);

#define SET_GLOBAL_EXCEPTION_FILTER()		SetUnhandledExceptionFilter(ExceptionFilter);


#endif // __EXCEPTION_FILTER_H__
