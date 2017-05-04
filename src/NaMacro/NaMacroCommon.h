#pragma once

#include <v8.h>

namespace NaMacroCommon {
	using namespace v8;

	extern bool g_bExit;

	void MessageLoopTillExit(Isolate* isolate);

}