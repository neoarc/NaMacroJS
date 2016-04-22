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

	// methods
	DEFINE_CLASS_METHOD(Sleep);
	DEFINE_CLASS_METHOD(Alert);
	DEFINE_CLASS_METHOD(Print);
	DEFINE_CLASS_METHOD(Exit);
	DEFINE_CLASS_METHOD(FindWindows);
	DEFINE_CLASS_METHOD(FindProcesses);
	DEFINE_CLASS_METHOD(FindTrays);
};
