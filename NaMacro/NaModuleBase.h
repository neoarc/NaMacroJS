#pragma once

#include "v8.h"

class NaModuleBase
{
public:
	NaModuleBase();
	~NaModuleBase();

	virtual void Init(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate>& global_template);
	virtual void Release();
};

