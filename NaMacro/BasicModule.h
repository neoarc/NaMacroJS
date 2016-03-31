#pragma once

#include <v8.h>
#include "Common.h"
#include "ModuleBase.h"

class NaBasicModule : public ModuleBase
{
public:
	virtual void Create(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate>& global_template);
	virtual void Init(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate> &global_template);
	virtual void Release();
};

v8::Local<v8::String> ReadFile(v8::Isolate *isolate, const char* name);

void Sleep(V8_FUNCTION_ARGS);
void Alert(V8_FUNCTION_ARGS);
void Print(V8_FUNCTION_ARGS);
void Exit(V8_FUNCTION_ARGS);
void FindWindows(V8_FUNCTION_ARGS);
void FindProcesses(V8_FUNCTION_ARGS);
void FindTrays(V8_FUNCTION_ARGS);