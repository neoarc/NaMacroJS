#include "stdafx.h"
#include <catch/catch.hpp>

#include <NaLib/NaProcess.h>

TEST_CASE( "NaProcess::FindProcesses" )
{
	system("calc");
	NaProcess::FindProcessInfo pi;
	NaProcess::FindProcesses(L"Calculator.exe", pi);
	CHECK(pi.foundlist.size() == 1);

	NaProcess* p = NaProcess::GetProcess(pi.foundlist.front());
	p->Terminate();
}
