#pragma once

#include <v8.h>

void InitMouseAPI(v8::Handle<v8::ObjectTemplate> &global_template);

v8::Local<v8::Object> GetMouseObject();
v8::Handle<v8::Value> MouseMove(const v8::Arguments& args);
v8::Handle<v8::Value> MouseClick(const v8::Arguments& args);

v8::Handle<v8::Value> MouseLButtonDown(const v8::Arguments& args);
v8::Handle<v8::Value> MouseLButtonUp(const v8::Arguments& args);
v8::Handle<v8::Value> MouseRButtonDown(const v8::Arguments& args);
v8::Handle<v8::Value> MouseRButtonUp(const v8::Arguments& args);
v8::Handle<v8::Value> MouseWheelDown(const v8::Arguments& args);
v8::Handle<v8::Value> MouseWheelUp(const v8::Arguments& args);

