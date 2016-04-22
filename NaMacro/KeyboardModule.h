#pragma once

#include <v8.h>
#include "Common.h"
#include "ModuleBase.h"

class NaKeyboardModule : public ModuleBase
{
public:
	virtual void Init(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate> &global_template);
	virtual void Release();

	v8::Local<v8::Object> GetKeyboardObject(v8::Isolate *isolate);

	// accessors

	// methods
	DEFINE_CLASS_METHOD(Down);
	DEFINE_CLASS_METHOD(Up);
	DEFINE_CLASS_METHOD(RegisterHotkey);
	DEFINE_CLASS_METHOD(UnregisterHotkey);
};

