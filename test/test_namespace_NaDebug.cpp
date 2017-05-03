#include "stdafx.h"
#include <catch/catch.hpp>

#include <NaLib/NaDebug.h>


TEST_CASE("NaDebug::OutA")
{
	NaDebug::OutA("[NaDebug::OutA] called!");
	CHECK(true); // #FIXME: find out 'check' method
}

TEST_CASE("NaDebug::OutA - var args")
{
	NaDebug::OutA("[NaDebug::OutA] test case: \"%s\"", "NaDebug::OutA - var args");
	CHECK(true); // #FIXME: find out 'check' method
}

TEST_CASE("NaDebug::Out")
{
	NaDebug::Out(L"[NaDebug::Out] called!");
	CHECK(true); // #FIXME: find out 'check' method
}

TEST_CASE("NaDebug::Out - var args")
{
	NaDebug::Out(L"[NaDebug::Out] test case: \"%s\"", L"NaDebug::Out - var args");
	CHECK(true); // #FIXME: find out 'check' method
}