#include "stdafx.h"
#include <catch/catch.hpp>

#include <NaLib/NaKnownFolder.h>


TEST_CASE("NaKnownFolder::MyDocument", "[.]")
{
	const NaString d = NaKnownFolder::MyDocument();
	CHECK(d == "D:\\Documents");
}

