// NaStringTest.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include "..\..\NaLib\NaString.h"

#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>

void NaDebugOut(const wchar_t* pszFormat, ...)
{
#if !defined(_DEBUG)
	return;
#endif

#define NA_DEBUGOUT_TEMPBUFFER_SIZE 65535

	wchar_t str[NA_DEBUGOUT_TEMPBUFFER_SIZE];
	memset(str, 0, sizeof(wchar_t) * NA_DEBUGOUT_TEMPBUFFER_SIZE);

	va_list arglist;
	va_start(arglist, pszFormat);
	vswprintf_s(str, pszFormat, arglist);
	va_end(arglist);

	OutputDebugStringW(str);
}

int main()
{
	// NaString(const wchar_t *lpsz);
	if (0)
	{
		NaString str(L"test1: simple constructor\n");
		NaDebugOut(str);
	}

	// NaString(NaString &nstr);
	if (0)
	{
		NaString str(L"test2: copy constructor\n");
		NaString str2(str);
		NaDebugOut(str2);
	}

	// const NaString& operator=(const wchar_t *lpsz);
	if (0)
	{
		NaString str;
		str = L"test3: operator=\n";
		NaDebugOut(str);
	}

	// const NaString& Format(const wchar_t *fmt, ...);
	if (0)
	{
		NaString str, str2(L"abc");

		str.Format(L"test4-1: simple format\n");
		NaDebugOut(str);

		str.Format(L"test4-2: format with '%s'\n", L"argument");
		NaDebugOut(str);

		str.Format(L"test4-3: copied format: %s\n", str);
		NaDebugOut(str);

		str.Format(L"test4-4: multi format: %d %d %d\n", 1, 2, 3);
		NaDebugOut(str);

		str.Format(L"test4-5: multi format: %s %d %d\n", (const wchar_t*)str2, 2, 3);
		NaDebugOut(str);

		str.Format(L"test4-6: multi format: %s %s %s\n", str2.wstr(), str2.wstr(), str2.wstr());
		NaDebugOut(str);
	}

	// int GetLength();
	if (0)
	{
		NaString str(L"hello");
		int nLen = str.GetLength();

		str.Format(L"test5: '%s'.GetLength: %d\n", str.wstr(), nLen);
		NaDebugOut(str);
	}

	// void ToLower();
	// void ToUpper();
	if (0)
	{
		NaString str;
		str = L"Hello World? 가나다";
		NaDebugOut(L"test6: original string: %s\n", str.wstr());

		str.ToLower();
		NaDebugOut(L"test6-1: to lower: %s\n", str.wstr());

		str = L"Hello World? 가나다";
		str.ToUpper();
		NaDebugOut(L"test6-2: to upper: %s\n", str.wstr());
	}

	// int Compare(const wchar_t *lpsz);
	// int CompareNoCase(const wchar_t *lpsz);
	if (0)
	{
		NaString str(L"hello"), str2(L"Hello");

		NaDebugOut(L"test7-1: compare '%s' with '%s': %d\n", str.wstr(), str2.wstr(), str.Compare(str2));

		NaDebugOut(L"test7-2: compare (NoCase) '%s' with '%s': %d\n", str.wstr(), str2.wstr(), str.CompareNoCase(str2));
	}

    return 0;
}

