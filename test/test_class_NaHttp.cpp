#include "stdafx.h"
#include <catch/catch.hpp>

#include "../src/NaHttp.h"

TEST_CASE("NaHttp.SendRequest", "[.]")
{
	NaHttp http;
	NaString url(L"www.daum.net");
	http.Open(url);
	http.SendRequest();
	NaString resp = http.GetResponse();
	CHECK(resp.GetLength() > 0);
}