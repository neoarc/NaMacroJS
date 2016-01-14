#pragma once

#include <v8.h>
#include "Common.h"
#include "ModuleBase.h"

class NaScreenModule : public ModuleBase
{
public:
	virtual void Init(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate> &global_template);
	virtual void Release();

	v8::Local<v8::Object> GetScreenObject(v8::Isolate *isolate);
};

void ScreenGetPixel(V8_FUNCTION_ARGS);
