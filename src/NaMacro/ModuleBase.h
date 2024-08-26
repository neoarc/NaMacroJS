#pragma once

#include "V8Wrap.h"

class ModuleBase
{
public:
	ModuleBase() {}
	~ModuleBase() {}

	virtual void Create(Isolate* isolate, Local<ObjectTemplate>& global_template) { UNUSED(isolate); UNUSED(global_template); }
	virtual void Init(Isolate* isolate, Local<ObjectTemplate>& global_template) { UNUSED(isolate); UNUSED(global_template); }
	virtual void Release() {}
};

