#include "stdafx.h"

#include "MouseModule.h"
#include "Common.h"

#include "Windows.h"

void NaMouseModule::Init(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate>& global_template)
{
	v8::HandleScope handle_scope(isolate);

	v8::Local<v8::Object> global = isolate->GetCurrentContext()->Global();
	v8::Local<v8::Object> mouse_obj = GetMouseObject(isolate);

#define ADD_MOUSE_API(_js_func, _c_func) \
	mouse_obj->Set( \
		v8::String::NewFromUtf8(isolate, #_js_func, v8::NewStringType::kNormal).ToLocalChecked(), \
		v8::FunctionTemplate::New(isolate, _c_func)->GetFunction() \
	);

	ADD_MOUSE_API(move,			MouseMove);
	ADD_MOUSE_API(click,		MouseClick);
	ADD_MOUSE_API(lbuttonDown,	MouseLButtonDown);
	ADD_MOUSE_API(lbuttonUp,	MouseLButtonUp);
	ADD_MOUSE_API(rbuttonDown,	MouseRButtonDown);
	ADD_MOUSE_API(rbuttonUp,	MouseRButtonUp);
	ADD_MOUSE_API(wheelDown,	MouseWheelDown);
	ADD_MOUSE_API(wheelUp,		MouseWheelUp);
}

void NaMouseModule::Release()
{
}

// description: return 'system.mouse'
v8::Local<v8::Object> NaMouseModule::GetMouseObject(v8::Isolate *isolate)
{
	// HandleScope 안에서 호출

	v8::Local<v8::Object> system_obj = GetSystemObject(isolate);
	v8::Local<v8::String> mouse_name = v8::String::NewFromUtf8(isolate, "mouse", v8::NewStringType::kNormal).ToLocalChecked();
	v8::Local<v8::Value> mouse_value = system_obj->Get(mouse_name);
	if (!mouse_value.IsEmpty() && mouse_value->IsUndefined())
	{
		// InitMouse
		mouse_value = v8::Object::New(isolate);
		system_obj->Set(mouse_name, mouse_value);
	}

	v8::Local<v8::Object> mouse_obj = mouse_value->ToObject();
	return mouse_obj;
}

// description: move mouse cursor
void MouseMove(V8_FUNCTION_ARGS)
{
	int x = args[0]->Int32Value();
	int y = args[1]->Int32Value();

	::SetCursorPos(x, y);
}

// description: click mouse lbutton
void MouseClick(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 2)
		MouseMove(args);

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	::SendInput(1, &input, sizeof(INPUT));
	::Sleep(MOUSECLICK_SLEEP);

	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	::SendInput(1, &input, sizeof(INPUT));
	::Sleep(MOUSECLICK_SLEEP);
}

// description: 
void MouseLButtonDown(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 2)
		MouseMove(args);

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	::SendInput(1, &input, sizeof(INPUT));
	::Sleep(MOUSECLICK_SLEEP);
}

// description: 
void MouseLButtonUp(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 2)
		MouseMove(args);

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	::SendInput(1, &input, sizeof(INPUT));
	::Sleep(MOUSECLICK_SLEEP);
}


// description: 
void MouseRButtonDown(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 2)
		MouseMove(args);

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	::SendInput(1, &input, sizeof(INPUT));
	::Sleep(MOUSECLICK_SLEEP);
}


// description: 
void MouseRButtonUp(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 2)
		MouseMove(args);

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	::SendInput(1, &input, sizeof(INPUT));
	::Sleep(MOUSECLICK_SLEEP);
}


// description: 
void MouseWheelDown(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 2)
		MouseMove(args);

	// TODO implement
}


// description: 
void MouseWheelUp(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 2)
		MouseMove(args);

	// TODO implement
}