#include "stdafx.h"
#include <catch/catch.hpp>

#include <NaLib/NaDebug.h>


TEST_CASE("NaDebugOutA")
{
	NaDebugOutA("[NaDebugOutA] called!");
	CHECK(true); // #FIXME: find out 'check' method
}

TEST_CASE("NaDebugOutA - var args")
{
	NaDebugOutA("[NaDebugOutA] test case: \"%s\"", "NaDebugOutA - var args");
	CHECK(true); // #FIXME: find out 'check' method
}

TEST_CASE("NaDebugOut")
{
	NaDebugOut(L"[NaDebugOut] called!");
	CHECK(true); // #FIXME: find out 'check' method
}

TEST_CASE("NaDebugOut - var args")
{
	NaDebugOut(L"[NaDebugOut] test case: \"%s\"", L"NaDebugOut - var args");
	CHECK(true); // #FIXME: find out 'check' method
}