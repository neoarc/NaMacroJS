#pragma once

#include <v8.h>
#include "Common.h"
#include "NaModuleBase.h"

class NaKeyboardModule : public NaModuleBase
{
public:
	virtual void Init(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate> &global_template);
	virtual void Release();

	v8::Local<v8::Object> GetKeyboardObject(v8::Isolate *isolate);
};

void KeyboardDown(V8_FUNCTION_ARGS);
void KeyboardUp(V8_FUNCTION_ARGS);

void KeyboardRegisterHotkey(V8_FUNCTION_ARGS);
void KeyboardUnregisterHotkey(V8_FUNCTION_ARGS);
