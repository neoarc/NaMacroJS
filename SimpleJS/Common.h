#pragma once

#include <v8.h>

extern bool g_bExit;
const char* ToCString(const v8::String::Utf8Value& value);
