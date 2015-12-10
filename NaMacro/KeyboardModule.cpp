#include "stdafx.h"

#include "KeyboardModule.h"

#include "Windows.h"

void NaKeyboardModule::Init(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate>& global_template)
{
	v8::HandleScope handle_scope(isolate);

	v8::Local<v8::Object> global = isolate->GetCurrentContext()->Global();
	v8::Local<v8::Object> keyboard_obj = GetKeyboardObject(isolate);

#define ADD_KEYBOARD_API(_js_func, _c_func) \
	keyboard_obj->Set( \
		v8::String::NewFromUtf8(isolate, #_js_func, v8::NewStringType::kNormal).ToLocalChecked(), \
		v8::FunctionTemplate::New(isolate, _c_func)->GetFunction() \
	);

	ADD_KEYBOARD_API(down, KeyboardDown);
	ADD_KEYBOARD_API(up, KeyboardUp);

}

void NaKeyboardModule::Release()
{
}

// description: return 'system.keyboard'
v8::Local<v8::Object> NaKeyboardModule::GetKeyboardObject(v8::Isolate *isolate)
{
	// HandleScope 안에서 호출

	v8::Local<v8::Object> system_obj = GetSystemObject(isolate);
	v8::Local<v8::String> keyboard_name = v8::String::NewFromUtf8(isolate, "keyboard", v8::NewStringType::kNormal).ToLocalChecked();
	v8::Local<v8::Value> keyboard_value = system_obj->Get(keyboard_name);
	if (!keyboard_value.IsEmpty() && keyboard_value->IsUndefined())
	{
		// Initkeyboard
		keyboard_value = v8::Object::New(isolate);
		system_obj->Set(keyboard_name, keyboard_value);
	}

	v8::Local<v8::Object> keyboard_obj = keyboard_value->ToObject();
	return keyboard_obj;
}

// description: 
// syntax:		
void KeyboardDown(V8_FUNCTION_ARGS)
{
	
}

// description: 
// syntax:		
void KeyboardUp(V8_FUNCTION_ARGS)
{

}

// description: 
// syntax:		
void KeyboardRegisterHotkey(V8_FUNCTION_ARGS)
{
	// TODO
	// HotkeyMap
	// - Hoykey / EventHandler
	// - Create PlayformWindow
	//
	//RegisterHotKey(NULL, 1, 0, 'a');
}

// description: 
// syntax:		
void KeyboardUnregisterHotkey(V8_FUNCTION_ARGS)
{
	// TODO
	//UnregisterHotKey(NULL, 1);
}

