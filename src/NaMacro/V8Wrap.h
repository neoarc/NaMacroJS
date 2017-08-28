#pragma once

#include <NaLib/NaString.h>
#include <NaLib/NaUrl.h>

namespace V8Wrap 
{
	using namespace v8;

	extern bool g_bReportExceptions;

	Platform* Initialize();
	Isolate* CreateNewIsolate();
	void TearDown(Platform* platform);

	void CreateDefaultModules(Isolate* isolate, Local<ObjectTemplate>& global_template);
	void InitModules(Isolate* isolate, Local<ObjectTemplate>& global_template);
	void ReleaseModules();

	Local<String> ReadFile(Isolate* isolate, const char* name);
	Local<String> ReadScript(Isolate* isolate, const std::string& scriptFilePath);
	Local<Script> Compile(Isolate* isolate, Local<Context>& context,
							const std::string& scriptPath,
							Local<String>& scriptSource);
	bool RunScript(Isolate* isolate, Local<Context>& context, Local<Script>& script);
	bool RunMainFunc(Isolate* isolate);

	Local<Object> GetSystemObject(Isolate* isolate);
	NaString ReportException(Isolate* isolate, TryCatch* handler);

	Local<Value> GetObjectProperty(Local<Object>& obj, NaString strPropName);

	template<typename T> void NullReturnValue(T &t) { SetReturnValueAsNull(t.GetReturnValue()); };

	template<typename T> void SetReturnValue(T &t, bool bValue) { SetReturnValueAsBoolean(t.GetReturnValue(), bValue); };
	template<typename T> void SetReturnValue(T &t, int nValue) { SetReturnValueAsInteger(t.GetReturnValue(), nValue); };
	template<typename T> void SetReturnValue(T &t, const char* sz) { SetReturnValueAsString(t.GetReturnValue(), sz); };
	template<typename T> void SetReturnValue(T &t, const wchar_t *wsz) { SetReturnValueAsString(t.GetReturnValue(), wsz); };
	template<typename T> void SetReturnValue(T &t, const std::wstring &wstr) { SetReturnValueAsString(t.GetReturnValue(), wstr); };

	// #TODO check & replace
	template<typename T, typename T2> void SetReturnValue(T &t, Local<T2> t2) {
		auto r = t.GetReturnValue();
		r.set(t2);
	};

	// #TODO change to protected
	void SetReturnValueAsNull(ReturnValue<Value> returnValue);
	void SetReturnValueAsBoolean(ReturnValue<Value> returnValue, bool value);
	void SetReturnValueAsInteger(ReturnValue<Value> returnValue, const int value);
	void SetReturnValueAsString(ReturnValue<Value> returnValue, const char* sz);
	void SetReturnValueAsString(ReturnValue<Value> returnValue, const std::wstring& txt);
}

//------------------------------------------------------------------------------
// Macro Defines 
//------------------------------------------------------------------------------

#define V8_FUNCTION_ARGS  const FunctionCallbackInfo<Value>& args
#define V8_GETTER_ARGS	  Local<String> name, const PropertyCallbackInfo<Value>& info
#define V8_SETTER_ARGS	  Local<String> name, Local<Value> value, const PropertyCallbackInfo<void>& info

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

// #NOTE will be deprecated
#define ADD_GLOBAL_CONSTRUCTOR(_class, _global_obj) \
	Local<String> _class##_name = String::NewFromUtf8(isolate, #_class, NewStringType::kNormal).ToLocalChecked(); \
	Local<Value> _class##_value = _global_obj->Get(_class##_name); \
	if (!_class##_value.IsEmpty() && _class##_value->IsUndefined()) { \
		Local<FunctionTemplate> templ = FunctionTemplate::New(isolate, Na##_class::method_constructor); \
		_global_obj->Set(_class##_name, templ->GetFunction()); \
	}

// #NOTE new interface (based on JsClass)
#define ADD_GLOBAL_CONSTRUCTOR2(_class, _global_obj) \
	Local<String> _class##_name = String::NewFromUtf8(isolate, #_class, NewStringType::kNormal).ToLocalChecked(); \
	Local<Value> _class##_value = _global_obj->Get(_class##_name); \
	if (!_class##_value.IsEmpty() && _class##_value->IsUndefined()) { \
		Local<FunctionTemplate> templ = FunctionTemplate::New(isolate, Js##_class::method_constructor); \
		_global_obj->Set(_class##_name, templ->GetFunction()); \
	}

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

#define DEFINE_CLASS_METHOD(_method)  static void method_##_method(V8_FUNCTION_ARGS);
#define DEFINE_GLOBAL_METHOD(_method)        void method_##_method(V8_FUNCTION_ARGS);

#define ADD_GLOBAL_METHOD(_js_func)			   ADD_TEMPLATE_METHOD(global_template, _js_func);
#define ADD_GLOBAL_METHOD2(_js_func, _c_func)  ADD_TEMPLATE_METHOD2(global_template, _js_func, _c_func);

