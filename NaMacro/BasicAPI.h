#pragma once

#include <v8.h>
#include "Common.h"

void InitBasicAPI(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate> &global_template);

v8::Local<v8::String> ReadFile(v8::Isolate *isolate, const char* name);

void Sleep(V8_FUNCTION_ARGS);
void Alert(V8_FUNCTION_ARGS);
void Print(V8_FUNCTION_ARGS);
void Exit(V8_FUNCTION_ARGS);