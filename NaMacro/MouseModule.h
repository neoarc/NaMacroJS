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
	DEFINE_CLASS_ACCESSOR(GetX, SetX);
	DEFINE_CLASS_ACCESSOR(GetY, SetY);

	// methods
	DEFINE_CLASS_METHOD(Move);
	DEFINE_CLASS_METHOD(Click);
	DEFINE_CLASS_METHOD(LButtonDown);
	DEFINE_CLASS_METHOD(LButtonUp);
	DEFINE_CLASS_METHOD(RButtonDown);
	DEFINE_CLASS_METHOD(RButtonUp);
	DEFINE_CLASS_METHOD(WheelDown);
	DEFINE_CLASS_METHOD(WheelUp);
	DEFINE_CLASS_METHOD(PostLButtonDown);
	DEFINE_CLASS_METHOD(PostLButtonUp);
};
