#pragma once

#include <v8.h>

#define MOUSECLICK_SLEEP	10
#define NA_DEBUGOUT_TEMPBUFFER_SIZE	65535

extern bool g_bExit;

using namespace v8;

// Utility Functions
void NaDebugOutA(const char* pszFormat, ...); 
void NaDebugOut(const wchar_t* pszFormat, ...);
Local<String> ReadFile(Isolate *isolate, const char* name);
Local<Object> GetSystemObject(Isolate *isolate);

// Macro Defines
#define V8_FUNCTION_ARGS	const FunctionCallbackInfo<Value>& args

#define V8_GETTER_ARGS		Local<String> name, const PropertyCallbackInfo<Value>& info

#define V8_SETTER_ARGS		Local<String> name, Local<Value> value, const PropertyCallbackInfo<void>& info

#define ADD_TEMPLATE_METHOD(_obj, _js_func, _c_func) \
	_obj->Set( \
		String::NewFromUtf8(isolate, #_js_func, NewStringType::kNormal).ToLocalChecked(), \
		FunctionTemplate::New(isolate, _c_func) \
	);

#define ADD_OBJ_METHOD(_obj, _js_func, _c_func) \
	_obj->Set( \
		String::NewFromUtf8(isolate, #_js_func, NewStringType::kNormal).ToLocalChecked(), \
		FunctionTemplate::New(isolate, _c_func)->GetFunction() \
	);

#define ADD_OBJ_ACCESSOR(_obj, _property, _getter, _setter) \
	_obj->SetAccessor( \
	String::NewFromUtf8(isolate, #_property, NewStringType::kInternalized).ToLocalChecked(), \
	_getter, _setter);


#define DEFINE_CLASS_ACCESSOR_RO(_getter) \
	static void _getter(Local<String> name, const PropertyCallbackInfo<Value>& info);

#define DEFINE_CLASS_ACCESSOR(_getter, _setter) \
	static void _getter(Local<String> name, const PropertyCallbackInfo<Value>& info); \
	static void _setter(Local<String> name, Local<Value> value, const PropertyCallbackInfo<void>& info);

#define DEFINE_CLASS_METHOD(_method) \
	static void _method(V8_FUNCTION_ARGS);

#define DEFINE_GLOBAL_METHOD(_method) \
	void _method(V8_FUNCTION_ARGS);

