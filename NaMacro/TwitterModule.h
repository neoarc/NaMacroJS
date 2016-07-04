#pragma once

#include <v8.h>
#include "Common.h"
#include "ModuleBase.h"

class NaTwitterModule : public ModuleBase
{
public:
	virtual void Create(Isolate *isolate, Local<ObjectTemplate>& global_template);
	virtual void Init(Isolate *isolate, Local<ObjectTemplate> &global_template);
	virtual void Release();

	// statics
	
	// methods
	DEFINE_CLASS_METHOD(Method);


};
