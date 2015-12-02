#include "stdafx.h"

#include "Common.h"

// Global Var
bool g_bExit = false;

// Util
const char* ToCString(const v8::String::Utf8Value& value)
{
	return *value ? *value : "<string conversion failed>";
}