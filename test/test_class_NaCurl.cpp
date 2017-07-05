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

TEST_CASE("NaCurl.Get", "[.]")
{
	NaCurl curl;

	char *outBuf = nullptr;
	long lSize = -1;
	bool bRet = curl.Get(
		L"[Your-Server-Url]/update/Resources/Sprite/Font.bmp",
		outBuf, lSize
	);
	
	CHECK(bRet == true);

	CHECK(outBuf != nullptr);

	CHECK(lSize == 49208);
}