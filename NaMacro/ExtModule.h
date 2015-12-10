#pragma once

#include <v8.h>
#include "Common.h"
#include "NaModuleBase.h"

class NaExtModule : public NaModuleBase
{
public:
	virtual void Init(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate> &global_template);
	virtual void Release();
};

void ConvGMacroToNaMacro(V8_FUNCTION_ARGS);
void TTSSpeak(V8_FUNCTION_ARGS);