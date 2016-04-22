#pragma once

#include <v8.h>

#define MOUSECLICK_SLEEP	10
#define NA_DEBUGOUT_TEMPBUFFER_SIZE	65535

extern bool g_bExit;

// Utility Functions
void NaDebugOut(const char* pszFormat, ...); 
v8::Local<v8::String> ReadFile(v8::Isolate *isolate, const char* name);
v8::Local<v8::Object> GetSystemObject(v8::Isolate *isolate);

// Macro Defines
#define V8_FUNCTION_ARGS	const v8::FunctionCallbackInfo<v8::Value>& args

#define ADD_TEMPLATE_METHOD(_obj, _js_func, _c_func) \
	_obj->Set( \
		v8::String::NewFromUtf8(isolate, #_js_func, v8::NewStringType::kNormal).ToLocalChecked(), \
		v8::FunctionTemplate::New(isolate, _c_func) \
	);

#define ADD_OBJ_METHOD(_obj, _js_func, _c_func) \
	_obj->Set( \
		v8::String::NewFromUtf8(isolate, #_js_func, v8::NewStringType::kNormal).ToLocalChecked(), \
		v8::FunctionTemplate::New(isolate, _c_func)->GetFunction() \
	);

#define ADD_OBJ_ACCESSOR(_obj, _property, _getter, _setter) \
	_obj->SetAccessor( \
	v8::String::NewFromUtf8(isolate, #_property, v8::NewStringType::kInternalized).ToLocalChecked(), \
	_getter, _setter);

#define DEFINE_CLASS_ACCESSOR(_getter, _setter) \
	static void _getter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info); \
	static void _setter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

#define DEFINE_CLASS_METHOD(_method) \
	static void _method(V8_FUNCTION_ARGS);

#define DEFINE_GLOBAL_METHOD(_method) \
	void _method(V8_FUNCTION_ARGS);

