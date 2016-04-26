#pragma once

#include <v8.h>
#include "Common.h"
#include "ModuleBase.h"

class NaKeyboardModule : public ModuleBase
{
public:
	virtual void Init(Isolate *isolate, Local<ObjectTemplate> &global_template);
	virtual void Release();

	Local<Object> GetKeyboardObject(Isolate *isolate);

	// accessors

	// methods
	DEFINE_CLASS_METHOD(Down);
	DEFINE_CLASS_METHOD(Up);
	DEFINE_CLASS_METHOD(RegisterHotkey);
	DEFINE_CLASS_METHOD(UnregisterHotkey);
};

