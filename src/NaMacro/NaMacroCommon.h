#pragma once

#include <functional> 

namespace NaMacroCommon 
{
	using namespace v8;
	extern bool g_bExit;

	void MessageLoopTillExit(Isolate* isolate);
	std::wstring GetSystemInfoStringByAPI(std::function<int(wchar_t*, unsigned long*)> f);
}