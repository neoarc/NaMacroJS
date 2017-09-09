#include "stdafx.h"
#include "NaDebug.h"

#define NA_DEBUGOUT_TEMPBUFFER_SIZE	65535

void NaDebugOutA(const char* pszFormat, ...)
{
#if !defined(_DEBUG)
	return;
#endif

	char str[NA_DEBUGOUT_TEMPBUFFER_SIZE];
	memset(str, 0, sizeof(char) * NA_DEBUGOUT_TEMPBUFFER_SIZE);

	va_list arglist;
	va_start(arglist, pszFormat);
	const int len = _vscprintf(pszFormat, arglist) // _vscprintf doesn't count  
											  + 1; // terminating '\0'  
	vsprintf_s(str, len, pszFormat, arglist);
	va_end(arglist);

	OutputDebugStringA(str);
}

void NaDebugOut(const wchar_t* pszFormat, ...)
{
#if !defined(_DEBUG)
	return;
#endif

	wchar_t str[NA_DEBUGOUT_TEMPBUFFER_SIZE];
	memset(str, 0, sizeof(wchar_t) * NA_DEBUGOUT_TEMPBUFFER_SIZE);

	va_list arglist;
	va_start(arglist, pszFormat);
	const int len = _vscwprintf(pszFormat, arglist) // _vscprintf doesn't count  
											  + 1; // terminating '\0'  
	vswprintf_s(str, len, pszFormat, arglist);
	va_end(arglist);

	OutputDebugStringW(str);
}
