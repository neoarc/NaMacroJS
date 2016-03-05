#pragma once

#include <v8.h>

#define MOUSECLICK_SLEEP	10

extern bool g_bExit;

v8::Local<v8::Object> GetSystemObject(v8::Isolate *isolate);

#define V8_FUNCTION_ARGS	const v8::FunctionCallbackInfo<v8::Value>& args