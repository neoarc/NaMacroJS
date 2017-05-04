#pragma once

#include "V8Wrap.h"
#include <v8.h>

class ModuleBase
{
public:
	ModuleBase();
	~ModuleBase();

	virtual void Create(Isolate *isolate, Local<ObjectTemplate>& global_template);
	virtual void Init(Isolate *isolate, Local<ObjectTemplate>& global_template);
	virtual void Release();
};

