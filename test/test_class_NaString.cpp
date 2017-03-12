#include "stdafx.h"
#include <catch/catch.hpp>

#include <NaLib/NaString.h>

using namespace std;

TEST_CASE("NaString.NaString(const wchar_t *)")
{
	const wstring s = L"test1: simple constructor\n";
	NaString ns(s.c_str());
	CHECK(ns == s.c_str());
}

TEST_CASE("NaString.NaString(const NaString & nstr)")
{
	NaString ns1(L"test2: copy constructor\n");
	NaString ns2(ns1);
	CHECK(ns1 == ns2);
}

TEST_CASE("NaString.operator=(const wchar_t *lpsz)")
{
	const wstring s = L"test3: operator=\n";
	NaString ns;
	ns = s.c_str();
	CHECK(ns == s.c_str());
}

TEST_CASE("NaString.Format")
{
	NaString ns;

	const wstring s1 = L"test4-1: simple format\n";
	ns.Format(s1.c_str());
	CHECK(ns == s1.c_str());

	ns.Format(L"test4-2: format with '%s'\n", L"argument");
	CHECK(ns == L"test4-2: format with 'argument'\n");

	ns.Format(L"test4-3: copied format: %s\n", ns.wstr());
	CHECK(ns == L"test4-3: copied format: test4-2: format with 'argument'\n\n");

	ns.Format(L"test4-4: multi format: %d %d %d\n", 1, 2, 3);
	CHECK(ns == L"test4-4: multi format: 1 2 3\n");

	NaString ns2 = L"abc";

	ns.Format(L"test4-5: multi format: %s %d %d\n", ns2.wstr(), 2, 3);
	CHECK(ns == L"test4-5: multi format: abc 2 3\n");

	ns.Format(L"test4-6: multi format: %s %s %s\n", ns2.wstr(), ns2.wstr(), ns2.wstr());
	CHECK(ns == L"test4-6: multi format: abc abc abc\n");
}

TEST_CASE("NaString.GetLength")
{
	NaString ns;

	CHECK(ns.GetLength() == 0);

	ns = L"";
	CHECK(ns.GetLength() == 0);

	ns = L"hello";
	CHECK(ns.GetLength() == 5);
}

TEST_CASE("NaString.ToLower")
{
	NaString ns = L"Hello World? 가나다";
	ns.ToLower();
	CHECK(ns == L"hello world? 가나다");
}

TEST_CASE("NaString.ToUpper")
{
	NaString ns = L"Hello World? 가나다";
	ns.ToUpper();
	CHECK(ns == L"HELLO WORLD? 가나다");
}

TEST_CASE("NaString.Compare")
{
	NaString ns1(L"hello"), ns2(L"Hello");
	CHECK(ns1.Compare(ns2) > 0);
}

TEST_CASE("NaString.CompareNoCase")
{
	NaString ns1(L"hello"), ns2(L"Hello");
	CHECK(ns1.CompareNoCase(ns2) == 0);
}

TEST_CASE("NaString.Find")
{
	NaString ns(L"Hello,World");
	CHECK(ns.Find(L"l") == 2);		// find 'l'
	CHECK(ns.Find(L"l", 3) == 3);	// find 'l' from index 3
	CHECK(ns.Find(L"l", 4) == 9);	// find 'l' from index 4
	CHECK(ns.Find(L"z") == -1);
	
	CHECK(ns.Find(L"\0") == 0);		// ?!, act as MFC CString.Find
}

TEST_CASE("NaString.Left")
{
	NaString ns(L"Hello,World");
	CHECK(ns.Left(1) == L"H");
	CHECK(ns.Left(4) == L"Hell");
	CHECK(ns.Left(7) == L"Hello,W");

	// Over range
	CHECK(ns.Left(15) == L"Hello,World");

	// Invalid argument
	CHECK(ns.Left(-5) == L"");  // act as CString.Left
}

TEST_CASE("NaString.Mid")
{
	NaString ns(L"Hello,World");
	CHECK(ns.Mid(1) == L"ello,World");
	CHECK(ns.Mid(4) ==    L"o,World");
	CHECK(ns.Mid(7) ==       L"orld");

	// Invalid argument
	CHECK(ns.Mid(-5) == ns);  // act as CString.Mid
}

TEST_CASE("NaString.Right")
{
	NaString ns(L"Hello,World"); // len = 11
	CHECK(ns.Right(1) ==       L"d");
	CHECK(ns.Right(4) ==    L"orld");
	CHECK(ns.Right(7) == L"o,World");

	// Over range
	CHECK(ns.Right(15) == L"Hello,World");

	// Invalid argument
	CHECK(ns.Right(-5) == L"");  // act as CString.Right
}

TEST_CASE("NaString.ToInt")
{
	// Not initialized (null value)
	REQUIRE_THROWS(NaString().ToInt() == 0);

	// Empty string
	REQUIRE_THROWS(NaString(L"").ToInt() == 0);

	// String
	REQUIRE_THROWS(NaString(L"a").ToInt() == 0);

	// Normal case
	CHECK(NaString(L"1").ToInt() == 1);
}

TEST_CASE("NaString.ToFloat")
{
	NaString ns;

	ns = L"1";
	CHECK(ns.ToFloat() == 1.0f);
}

TEST_CASE("NaString.ToDouble")
{
	NaString ns;
	
	ns = L"1";
	CHECK(ns.ToDouble() == 1.0);
}