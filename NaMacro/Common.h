#pragma once

#include <v8.h>
#include "../NaLib/NaString.h"
#include "../NaLib/NaUrl.h"

#define MOUSECLICK_SLEEP	10
#define NA_DEBUGOUT_TEMPBUFFER_SIZE	65535

extern bool g_bReportExceptions;
extern bool g_bExit;

using namespace v8;

// v8 Utility
NaString ReportException(Isolate *isolate, TryCatch* handler);

// Utility Functions
void NaDebugOutA(const char* pszFormat, ...); 
void NaDebugOut(const wchar_t* pszFormat, ...);
Local<String> ReadFile(Isolate *isolate, const char* name);
Local<Object> GetSystemObject(Isolate *isolate);

// Macro Defines
#define V8_FUNCTION_ARGS	const FunctionCallbackInfo<Value>& args

#define V8_GETTER_ARGS		Local<String> name, const PropertyCallbackInfo<Value>& info

#define V8_SETTER_ARGS		Local<String> name, Local<Value> value, const PropertyCallbackInfo<void>& info

#define ADD_TEMPLATE_METHOD(_obj, _js_func) \
	_obj->Set( \
		String::NewFromUtf8(isolate, #_js_func, NewStringType::kNormal).ToLocalChecked(), \
		FunctionTemplate::New(isolate, method_##_js_func) \
	);

#define ADD_TEMPLATE_METHOD2(_obj, _js_func, _c_func) \
	_obj->Set( \
		String::NewFromUtf8(isolate, #_js_func, NewStringType::kNormal).ToLocalChecked(), \
		FunctionTemplate::New(isolate, _c_func) \
	);

#define ADD_OBJ_METHOD(_obj, _js_func) \
	_obj->Set( \
		String::NewFromUtf8(isolate, #_js_func, NewStringType::kNormal).ToLocalChecked(), \
		FunctionTemplate::New(isolate, method_##_js_func)->GetFunction() \
	);

#define ADD_OBJ_ACCESSOR(_obj, _property) \
	_obj->SetAccessor( \
	String::NewFromUtf8(isolate, #_property, NewStringType::kInternalized).ToLocalChecked(), \
	get_##_property, set_##_property);

// RO = ReadOnly (No Setter)
#define ADD_OBJ_ACCESSOR_RO(_obj, _property) \
	_obj->SetAccessor( \
	String::NewFromUtf8(isolate, #_property, NewStringType::kInternalized).ToLocalChecked(), \
	get_##_property, nullptr);

#define DEFINE_CLASS_ACCESSOR_RO(_property) \
	static void get_##_property(Local<String> name, const PropertyCallbackInfo<Value>& info);

#define DEFINE_CLASS_ACCESSOR(_property) \
	static void get_##_property(Local<String> name, const PropertyCallbackInfo<Value>& info); \
	static void set_##_property(Local<String> name, Local<Value> value, const PropertyCallbackInfo<void>& info);

#define DEFINE_CLASS_METHOD(_method) \
	static void method_##_method(V8_FUNCTION_ARGS);

#define DEFINE_GLOBAL_METHOD(_method) \
	void method_##_method(V8_FUNCTION_ARGS);

#define ADD_GLOBAL_METHOD(_js_func)	\
	ADD_TEMPLATE_METHOD(global_template, _js_func);

#define ADD_GLOBAL_METHOD2(_js_func, _c_func) \
	ADD_TEMPLATE_METHOD2(global_template, _js_func, _c_func);

