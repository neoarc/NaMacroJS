#include "stdafx.h"

#include "MouseModule.h"
#include "Common.h"

#include "Windows.h"

void NaMouseModule::Init(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate>& global_template)
{
	v8::HandleScope handle_scope(isolate);

	v8::Local<v8::Object> global = isolate->GetCurrentContext()->Global();
	v8::Local<v8::Object> mouse_obj = GetMouseObject(isolate);

#define ADD_MOUSE_ACCESSOR(_prop, _getter, _setter)		ADD_OBJ_ACCESSOR(mouse_obj, _prop, _getter, _setter);
#define ADD_MOUSE_METHOD(_js_func, _c_func)				ADD_OBJ_METHOD(mouse_obj, _js_func, _c_func);

	// accessors
	ADD_MOUSE_ACCESSOR(x, GetX, SetX);
	ADD_MOUSE_ACCESSOR(y, GetY, SetY);

	// methods
	ADD_MOUSE_METHOD(move,			Move);
	ADD_MOUSE_METHOD(click,			Click);
	ADD_MOUSE_METHOD(lbuttonDown,	LButtonDown);
	ADD_MOUSE_METHOD(lbuttonUp,		LButtonUp);
	ADD_MOUSE_METHOD(rbuttonDown,	RButtonDown);
	ADD_MOUSE_METHOD(rbuttonUp,		RButtonUp);
	ADD_MOUSE_METHOD(wheelDown,		WheelDown);
	ADD_MOUSE_METHOD(wheelUp,		WheelUp);
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

		// temp; using template example
		/*
		v8::Local<v8::ObjectTemplate> templ = v8::ObjectTemplate::New(isolate);

		templ->SetAccessor(
			v8::String::NewFromUtf8(isolate, "x", v8::NewStringType::kInternalized).ToLocalChecked(),
			GetX,
			SetX
			);

		mouse_value = templ->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
		*/
		system_obj->Set(mouse_name, mouse_value);
	}

	v8::Local<v8::Object> mouse_obj = mouse_value->ToObject();
	return mouse_obj;
}

// description: x getter 
void NaMouseModule::GetX(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	POINT pt;
	::GetCursorPos(&pt);

	v8::Isolate *isolate = info.GetIsolate();
	info.GetReturnValue().Set(
		v8::Integer::New(isolate, pt.x)
		);
}

// description: x setter 
void NaMouseModule::SetX(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	POINT pt;
	::GetCursorPos(&pt);

	pt.x = value->Int32Value();
	::SetCursorPos(pt.x, pt.y);
}

// description: y getter 
void NaMouseModule::GetY(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	POINT pt;
	::GetCursorPos(&pt);

	v8::Isolate *isolate = info.GetIsolate();
	info.GetReturnValue().Set(
		v8::Integer::New(isolate, pt.y)
		);
}

// description: y setter 
void NaMouseModule::SetY(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	POINT pt;
	::GetCursorPos(&pt);

	pt.y = value->Int32Value();
	::SetCursorPos(pt.x, pt.y);
}

// description: move mouse cursor
void NaMouseModule::Move(V8_FUNCTION_ARGS)
{
	int x = args[0]->Int32Value();
	int y = args[1]->Int32Value();

	::SetCursorPos(x, y);
}

// description: click mouse lbutton
void NaMouseModule::Click(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 2)
		Move(args);

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
void NaMouseModule::LButtonDown(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 2)
		Move(args);

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	::SendInput(1, &input, sizeof(INPUT));
	::Sleep(MOUSECLICK_SLEEP);
}

// description: 
void NaMouseModule::LButtonUp(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 2)
		Move(args);

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	::SendInput(1, &input, sizeof(INPUT));
	::Sleep(MOUSECLICK_SLEEP);
}


// description: 
void NaMouseModule::RButtonDown(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 2)
		Move(args);

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	::SendInput(1, &input, sizeof(INPUT));
	::Sleep(MOUSECLICK_SLEEP);
}


// description: 
void NaMouseModule::RButtonUp(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 2)
		Move(args);

	INPUT input;
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	::SendInput(1, &input, sizeof(INPUT));
	::Sleep(MOUSECLICK_SLEEP);
}


// description: 
void NaMouseModule::WheelDown(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 2)
		Move(args);

	// TODO implement
}


// description: 
void NaMouseModule::WheelUp(V8_FUNCTION_ARGS)
{
	if (args.Length() >= 2)
		Move(args);

	// TODO implement
}