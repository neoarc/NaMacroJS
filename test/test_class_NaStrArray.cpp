#include "stdafx.h"
#include <catch/catch.hpp>

#include "../../src/NaString.h"

TEST_CASE("NaStrArray.operator [](int nIndex)")
{
	NaStrArray nsa;
	nsa.Add("aA");
	nsa.Add("bB");
	nsa.Add("cC");

	CHECK(nsa[0] == "aA");
	CHECK(nsa[1] == "bB");
	CHECK(nsa[2] == "cC");
}

TEST_CASE("NaStrArray.Add")
{
	NaStrArray nsa;
	nsa.Add("aa");
	nsa.Add("22");

	CHECK(nsa.GetCount() == 2);
	CHECK(nsa[0] == "aa");
	CHECK(nsa[1] == "22");
}

TEST_CASE("NaStrArray.Remove")
{
	NaStrArray nsa;
	nsa.Add("aa");
	nsa.Add("22");

	nsa.Remove(0);

	CHECK(nsa.GetCount() == 1);
	CHECK(nsa[0] == "22");
}

TEST_CASE("NaStrArray.GetCount")
{
	NaStrArray nsa;
	nsa.Add("1");
	nsa.Add("2");

	CHECK(nsa.GetCount() == 2);
}

TEST_CASE("NaStrArray.Find")
{
	NaStrArray nsa;
	nsa.Add("aA");
	nsa.Add("bB");
	nsa.Add("cC");

	CHECK(nsa.Find("aA") == 0);
	CHECK(nsa.Find("bB") == 1);
	CHECK(nsa.Find("cC") == 2);
	CHECK(nsa.Find("xX") == -1);
}

TEST_CASE("NaStrArray.Join")
{
	NaStrArray nsa;
	nsa.Add("aA");
	nsa.Add("bB");
	nsa.Add("cC");

	CHECK(nsa.Join(L", ") == L"aA, bB, cC");
}

TEST_CASE("NaStrArray.Pop")
{
	NaStrArray nsa;
	nsa.Add("aA");
	nsa.Add("bB");
	nsa.Add("cC");

	NaString ns;
	
	ns = nsa.Pop();
	CHECK(nsa.GetCount() == 2);
	CHECK(ns == L"cC");

	ns = nsa.Pop();
	CHECK(nsa.GetCount() == 1);
	CHECK(ns == L"bB");

	ns = nsa.Pop();
	CHECK(nsa.GetCount() == 0);
	CHECK(ns == L"aA");

	ns = nsa.Pop();
	CHECK(nsa.GetCount() == 0);
	CHECK(ns == L"");
}
