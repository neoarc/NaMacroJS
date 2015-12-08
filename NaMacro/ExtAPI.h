#pragma once

#include <v8.h>
#include "Common.h"

void InitExtAPI(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate> &global_template);

void ConvGMacroToNaMacro(V8_FUNCTION_ARGS);
void TTSSpeak(V8_FUNCTION_ARGS);