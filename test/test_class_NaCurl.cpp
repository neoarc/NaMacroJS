#include "stdafx.h"
#include <catch/catch.hpp>

#include <NaLib/NaCurl.h>

TEST_CASE("NaCurl.Post", "[.]")
{
	NaCurl curl;
	NaString strRet = curl.Post(L"www.google.com");
	NaString strBeginningOfHtml = L"<!doctype html>";

	auto strLeft = strRet.Left(strBeginningOfHtml.GetLength());
	CHECK(strLeft == L"<!doctype html>");
}