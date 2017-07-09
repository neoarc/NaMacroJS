#include "stdafx.h"
#include <catch/catch.hpp>

#include <NaLib/NaUrl.h>


TEST_CASE("NaUrl::GetMyDocumentDirectory", "[.]")
{
	const NaString d = NaUrl::GetMyDocumentDirectory();
	CHECK(d == "D:\\Documents");
}

