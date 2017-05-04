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

	// int Find(wchar_t* ch);
	if (0)
	{
		NaString str(L"Hello,World");

		NaDebugOut(L"test8: source string: %s\n", str.wstr());
		NaDebugOut(L"test8-1: find 'l': %d\n", str.Find(L"l"));
		NaDebugOut(L"test8-2: find 'l' from 3: %d\n", str.Find(L"l", 3));
		NaDebugOut(L"test8-3: find 'l' from 4: %d\n", str.Find(L"l", 4));
		NaDebugOut(L"test8-4: find 'z': %d\n", str.Find(L"z"));

	}

	// const NaString& Left(int len);
	if (0)
	{
		NaString str(L"Hello,World");

		NaDebugOut(L"test9: source string: %s\n", str.wstr());
		NaDebugOut(L"test9-1: left(1): %s\n", str.Left(1).wstr());
		NaDebugOut(L"test9-2: left(4): %s\n", str.Left(4).wstr());
		NaDebugOut(L"test9-3: left(7): %s\n", str.Left(7).wstr());
	}

	// const NaString& Mid(int index);
	if (0)
	{
		NaString str(L"Hello,World");

		NaDebugOut(L"test10: source string: %s\n", str.wstr());
		NaDebugOut(L"test10-1: mid(1): %s\n", str.Mid(1).wstr());
		NaDebugOut(L"test10-2: mid(4): %s\n", str.Mid(4).wstr());
		NaDebugOut(L"test10-3: mid(7): %s\n", str.Mid(7).wstr());
	}

	// const NaString& Right(int len);
	if (0)
	{
		NaString str(L"Hello,World");

		NaDebugOut(L"test11: source string: %s\n", str.wstr());
		NaDebugOut(L"test11-1: right(1): %s\n", str.Right(1).wstr());
		NaDebugOut(L"test11-2: right(4): %s\n", str.Right(4).wstr());
		NaDebugOut(L"test11-3: right(7): %s\n", str.Right(7).wstr());
	}

    return 0;
}

