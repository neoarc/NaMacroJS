#pragma once

#include <v8.h>

v8::Handle<v8::String> ReadFile(const char* name);
v8::Handle<v8::Value> Print(const v8::Arguments& args);
v8::Handle<v8::Value> Exit(const v8::Arguments& args);