#pragma once

#include <v8.h>
#include "Common.h"

void InitMouseAPI(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate> &global_template);

v8::Local<v8::Object> GetMouseObject(v8::Isolate *isolate);

void MouseMove(V8_FUNCTION_ARGS);
void MouseClick(V8_FUNCTION_ARGS);
void MouseLButtonDown(V8_FUNCTION_ARGS);
void MouseLButtonUp(V8_FUNCTION_ARGS);
void MouseRButtonDown(V8_FUNCTION_ARGS);
void MouseRButtonUp(V8_FUNCTION_ARGS);
void MouseWheelDown(V8_FUNCTION_ARGS);
void MouseWheelUp(V8_FUNCTION_ARGS);

