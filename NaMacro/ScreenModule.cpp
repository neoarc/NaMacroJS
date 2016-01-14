#include "stdafx.h"

#include "ScreenModule.h"

#include "Windows.h"

void NaScreenModule::Init(v8::Isolate * isolate, v8::Local<v8::ObjectTemplate>& global_template)
{
	v8::HandleScope handle_scope(isolate);

	v8::Local<v8::Object> global = isolate->GetCurrentContext()->Global();
	v8::Local<v8::Object> screen_obj = GetScreenObject(isolate);

#define ADD_SCREEN_API(_js_func, _c_func) \
	screen_obj->Set( \
		v8::String::NewFromUtf8(isolate, #_js_func, v8::NewStringType::kNormal).ToLocalChecked(), \
		v8::FunctionTemplate::New(isolate, _c_func)->GetFunction() \
	);

	ADD_SCREEN_API(getPixel, ScreenGetPixel);
}

void NaScreenModule::Release()
{
}

v8::Local<v8::Object> NaScreenModule::GetScreenObject(v8::Isolate * isolate)
{
	// HandleScope 안에서 호출

	v8::Local<v8::Object> system_obj = GetSystemObject(isolate);
	v8::Local<v8::String> screen_name = v8::String::NewFromUtf8(isolate, "screen", v8::NewStringType::kNormal).ToLocalChecked();
	v8::Local<v8::Value> screen_value = system_obj->Get(screen_name);
	if (!screen_value.IsEmpty() && screen_value->IsUndefined())
	{
		// InitScreen
		screen_value = v8::Object::New(isolate);
		system_obj->Set(screen_name, screen_value);
	}

	v8::Local<v8::Object> screen_obj = screen_value->ToObject();
	return screen_obj;
}

// description: get pixel color from point x,y
// syntax:		getPixel(x, y) : color
void ScreenGetPixel(V8_FUNCTION_ARGS)
{
	v8::Isolate *isolate = args.GetIsolate();

	if (args.Length() < 2)
	{
		// error
		args.GetReturnValue().Set(v8::Integer::New(isolate, -1));
		return;
	}

	int x = args[0]->Int32Value();
	int y = args[1]->Int32Value();

	HWND hDesktop = ::GetDesktopWindow();
	HDC hDC = ::GetWindowDC(hDesktop);

	// get pixel from point
	COLORREF color = ::GetPixel(hDC, x, y);
	args.GetReturnValue().Set(v8::Integer::New(isolate, color));

	// return
	::DeleteDC(hDC);
}
