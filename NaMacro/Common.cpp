#include "stdafx.h"

#include "Common.h"

// Global Var
bool g_bExit = false;

// description: return 'system'
v8::Local<v8::Object> GetSystemObject(v8::Isolate *isolate)
{
	// HandleScope 안에서 호출

	v8::Local<v8::Object> global = isolate->GetCurrentContext()->Global();
	v8::Local<v8::String> system_name = v8::String::NewFromUtf8(isolate, "system", v8::NewStringType::kNormal).ToLocalChecked();
	v8::Local<v8::Value> system_value = global->Get(system_name);
	if (!system_value.IsEmpty() && system_value->IsUndefined())
	{
		// InitSystem
		system_value = v8::Object::New(isolate);
		global->Set(system_name, system_value);
	}

	v8::Local<v8::Object> system_obj = system_value->ToObject();
	return system_obj;
}
