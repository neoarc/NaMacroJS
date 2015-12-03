#pragma once

#include <v8.h>

void InitExtAPI(v8::Handle<v8::ObjectTemplate> &global_template);

v8::Handle<v8::Value> ConvGMacroToNaMacro(const v8::Arguments& args);