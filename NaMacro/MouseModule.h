#pragma once

#include <v8.h>
#include "Common.h"
#include "ModuleBase.h"

class NaMouseModule : public ModuleBase
{
public:
	virtual void Init(Isolate *isolate, Local<ObjectTemplate> &global_template);
	virtual void Release();

	Local<Object> GetMouseObject(Isolate *isolate);

	// accessors
	DEFINE_CLASS_ACCESSOR(x);
	DEFINE_CLASS_ACCESSOR(y);

	// methods
	DEFINE_CLASS_METHOD(move);
	DEFINE_CLASS_METHOD(click);
	DEFINE_CLASS_METHOD(lbuttonDown);
	DEFINE_CLASS_METHOD(lbuttonUp);
	DEFINE_CLASS_METHOD(rbuttonDown);
	DEFINE_CLASS_METHOD(rbuttonUp);
	DEFINE_CLASS_METHOD(wheelDown);
	DEFINE_CLASS_METHOD(wheelUp);
	DEFINE_CLASS_METHOD(postLButtonDown);
	DEFINE_CLASS_METHOD(postLButtonUp);
};
