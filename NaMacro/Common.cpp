#include "stdafx.h"

#include "Common.h"

// Global Var
bool g_bExit = false;

// description: return 'system'
v8::Local<v8::Object> GetSystemObject()
{
	// HandleScope 안에서 호출

	v8::Handle<v8::Object> global = v8::Context::GetCurrent()->Global();
	v8::Local<v8::String> system_name = v8::String::New("system");
	v8::Local<v8::Value> system_value = global->Get(system_name);
	if (!system_value.IsEmpty() && system_value->IsUndefined())
	{
		// InitSystem
		system_value = v8::Object::New();
		global->Set(system_name, system_value);
	}

	v8::Local<v8::Object> system_obj = system_value->ToObject();
	return system_obj;
}

// Util
const char* ToCString(const v8::String::Utf8Value& value)
{
	return *value ? *value : "<string conversion failed>";
}