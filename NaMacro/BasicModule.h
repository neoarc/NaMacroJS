#pragma once

#include <v8.h>
#include "Common.h"
#include "ModuleBase.h"

class NaBasicModule : public ModuleBase
{
public:
	virtual void Create(Isolate *isolate, Local<ObjectTemplate>& global_template);
	virtual void Init(Isolate *isolate, Local<ObjectTemplate> &global_template);
	virtual void Release();

	// methods
	DEFINE_CLASS_METHOD(Sleep);
	DEFINE_CLASS_METHOD(Alert);
	DEFINE_CLASS_METHOD(Print);
	DEFINE_CLASS_METHOD(Exit);
	DEFINE_CLASS_METHOD(GetWindow);
	DEFINE_CLASS_METHOD(FindWindows);
	DEFINE_CLASS_METHOD(FindProcesses);
	DEFINE_CLASS_METHOD(FindTrays);
};
