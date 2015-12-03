#pragma once

#include <v8.h>

void InitMouseAPI(v8::Handle<v8::ObjectTemplate> &global_template);

v8::Handle<v8::Value> MouseMove(const v8::Arguments& args);