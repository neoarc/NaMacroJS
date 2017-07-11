#include "stdafx.h"
#include <catch/catch.hpp>

#include <NaLib/NaCurl.h>
#include <NaLib/NaDebug.h>

TEST_CASE("NaCurl.Post")
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
	curl.SetCallback([&](size_t added)
	{
		NaDebug::Out(L"Downloaded: %ld (chunk: %d)\n", curl.m_lDownloaded, added);
	});

	char *outBuf = nullptr;
	long lSize = -1;
	bool bRet = curl.Get(
		L"http://www.notbadatall.com/pics/unikick300.jpg",
		//L"http://google.com/images/branding/googlelogo/2x/googlelogo_color_272x92dp.png",
		&outBuf, lSize
	);
	
	CHECK(bRet == true);

	CHECK(((void*)outBuf) != nullptr);

	CHECK(lSize == 484044);
	//CHECK(lSize == 1620);
}