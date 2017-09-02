#pragma once

#include "V8Wrap.h"
#include "ModuleBase.h"

class NaScreenModule : public ModuleBase
{
public:
	virtual void Init(Isolate *isolate, Local<ObjectTemplate> &global_template);
	virtual void Release();

	Local<Object> GetScreenObject(Isolate *isolate);

	// accessors
	DEFINE_CLASS_ACCESSOR_RO(width);
	DEFINE_CLASS_ACCESSOR_RO(height);

	// methods
	DEFINE_CLASS_METHOD(capture);
	DEFINE_CLASS_METHOD(findColor);
	DEFINE_CLASS_METHOD(getPixel);
	DEFINE_CLASS_METHOD(setAero);
};

