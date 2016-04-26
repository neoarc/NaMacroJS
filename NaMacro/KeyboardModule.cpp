#include "stdafx.h"

#include "KeyboardModule.h"

#include "Windows.h"

void NaKeyboardModule::Init(Isolate *isolate, Local<ObjectTemplate>& global_template)
{
	HandleScope handle_scope(isolate);

	Local<Object> global = isolate->GetCurrentContext()->Global();
	Local<Object> keyboard_obj = GetKeyboardObject(isolate);

#define ADD_KEYBOARD_METHOD(_js_func, _c_func)			ADD_OBJ_METHOD(keyboard_obj, _js_func, _c_func);
#define ADD_KEYBOARD_ACCESSOR(_prop, _getter, _setter)	ADD_OBJ_ACCESSOR(keyboard_obj, _prop, _getter, _setter);
	
	// accessors

	// methods
	ADD_KEYBOARD_METHOD(down,	Down);
	ADD_KEYBOARD_METHOD(up,		Up);
}

void NaKeyboardModule::Release()
{
	VK_SPACE;
}

// description: return 'system.keyboard'
Local<Object> NaKeyboardModule::GetKeyboardObject(Isolate *isolate)
{
	// HandleScope 안에서 호출

	Local<Object> system_obj = GetSystemObject(isolate);
	Local<String> keyboard_name = String::NewFromUtf8(isolate, "keyboard", NewStringType::kNormal).ToLocalChecked();
	Local<Value> keyboard_value = system_obj->Get(keyboard_name);
	if (!keyboard_value.IsEmpty() && keyboard_value->IsUndefined())
	{
		// Initkeyboard
		keyboard_value = Object::New(isolate);
		system_obj->Set(keyboard_name, keyboard_value);
	}

	Local<Object> keyboard_obj = keyboard_value->ToObject();
	return keyboard_obj;
}

// description: 
// syntax:		
void NaKeyboardModule::Down(V8_FUNCTION_ARGS)
{
	if (args.Length() <= 0)
		return;

	int nKey = args[0]->Int32Value();

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = nKey;
	input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY;
	::SendInput(1, &input, sizeof(INPUT));
}

// description: 
// syntax:		
void NaKeyboardModule::Up(V8_FUNCTION_ARGS)
{
	if (args.Length() <= 0)
		return;

	int nKey = args[0]->Int32Value();

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = nKey;
	input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP;
	::SendInput(1, &input, sizeof(INPUT));
}

// description: 
// syntax:		
void NaKeyboardModule::RegisterHotkey(V8_FUNCTION_ARGS)
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
void NaKeyboardModule::UnregisterHotkey(V8_FUNCTION_ARGS)
{
	// TODO
	//UnregisterHotKey(NULL, 1);
}

