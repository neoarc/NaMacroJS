#include "stdafx.h"

#include "MouseAPI.h"
#include "Common.h"

#include "Windows.h"

void InitMouseAPI(v8::Handle<v8::ObjectTemplate>& global_template)
{
	v8::HandleScope handle_scope;

	v8::Handle<v8::Object> global = v8::Context::GetCurrent()->Global();
	v8::Local<v8::Object> mouse_obj = GetMouseObject();

#define ADD_MOUSE_API(_js_func, _c_func) mouse_obj->Set(v8::String::New(#_js_func), v8::FunctionTemplate::New(_c_func)->GetFunction());
	ADD_MOUSE_API(move,			MouseMove);
	ADD_MOUSE_API(click,		MouseClick);
	ADD_MOUSE_API(lbuttonDown,	MouseLButtonDown);
	ADD_MOUSE_API(lbuttonUp,	MouseLButtonUp);
	ADD_MOUSE_API(rbuttonDown,	MouseRButtonDown);
	ADD_MOUSE_API(rbuttonUp,	MouseRButtonUp);
	ADD_MOUSE_API(wheelDown,	MouseWheelDown);
	ADD_MOUSE_API(wheelUp,		MouseWheelUp);
}

// description: return 'system.mouse'
v8::Local<v8::Object> GetMouseObject()
{
	// HandleScope 안에서 호출

	v8::Local<v8::Object> system_obj = GetSystemObject();
	v8::Local<v8::String> mouse_name = v8::String::New("mouse");
	v8::Local<v8::Value> mouse_value = system_obj->Get(mouse_name);
	if (!mouse_value.IsEmpty() && mouse_value->IsUndefined())
	{
		// InitMouse
		mouse_value = v8::Object::New();
		system_obj->Set(mouse_name, mouse_value);
	}

	v8::Local<v8::Object> mouse_obj = mouse_value->ToObject();
	return mouse_obj;
}

// description: move mouse cursor
v8::Handle<v8::Value> MouseMove(const v8::Arguments& args)
{
	int x = args[0]->Int32Value();
	int y = args[1]->Int32Value();

	::SetCursorPos(x, y);

	return v8::Undefined();
}

// description: click mouse lbutton
v8::Handle<v8::Value> MouseClick(const v8::Arguments& args)
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

	return v8::Undefined();
}

// description: 
v8::Handle<v8::Value> MouseLButtonDown(const v8::Arguments& args)
{
	if (args.Length() >= 2)
		MouseMove(args);

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	::SendInput(1, &input, sizeof(INPUT));
	::Sleep(MOUSECLICK_SLEEP);

	return v8::Undefined();
}


// description: 
v8::Handle<v8::Value> MouseLButtonUp(const v8::Arguments& args)
{
	if (args.Length() >= 2)
		MouseMove(args);

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	::SendInput(1, &input, sizeof(INPUT));
	::Sleep(MOUSECLICK_SLEEP);

	return v8::Undefined();
}


// description: 
v8::Handle<v8::Value> MouseRButtonDown(const v8::Arguments& args)
{
	if (args.Length() >= 2)
		MouseMove(args);

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	::SendInput(1, &input, sizeof(INPUT));
	::Sleep(MOUSECLICK_SLEEP);

	return v8::Undefined();
}


// description: 
v8::Handle<v8::Value> MouseRButtonUp(const v8::Arguments& args)
{
	if (args.Length() >= 2)
		MouseMove(args);

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	::SendInput(1, &input, sizeof(INPUT));
	::Sleep(MOUSECLICK_SLEEP);

	return v8::Undefined();
}


// description: 
v8::Handle<v8::Value> MouseWheelDown(const v8::Arguments& args)
{
	if (args.Length() >= 2)
		MouseMove(args);

	// TODO implement

	return v8::Undefined();
}


// description: 
v8::Handle<v8::Value> MouseWheelUp(const v8::Arguments& args)
{
	if (args.Length() >= 2)
		MouseMove(args);

	// TODO implement

	return v8::Undefined();
}