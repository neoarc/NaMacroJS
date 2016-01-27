#include "stdafx.h"

#include "ScreenModule.h"

HWND NaScreenModule::m_hDesktopWnd = NULL;
HDC NaScreenModule::m_hDesktopDC = NULL;
bool NaScreenModule::m_bAeroStatus = false;

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
	ADD_SCREEN_API(setAero, ScreenSetAero);
}

void NaScreenModule::Release()
{
	if (m_hDesktopDC)
	{
		::DeleteDC(m_hDesktopDC);
		m_hDesktopDC = NULL;
	}

	// TODO restore Aero status
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
// syntax:		system.screen.getPixel(x, y) : color
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

	HDC hDC = NaScreenModule::GetDesktopDC();

	// get pixel from point
#define USE_FAST_GETPIXEL
#ifdef USE_FAST_GETPIXEL
	HDC hMemoryDC = ::CreateCompatibleDC(hDC);
	HBITMAP hBmp = ::CreateCompatibleBitmap(hDC, 1, 1);
	::SelectObject(hMemoryDC, hBmp);
	::BitBlt(hMemoryDC, 0, 0, 1, 1, hDC, x, y, SRCCOPY);

	COLORREF color = ::GetPixel(hMemoryDC, 0, 0);
#else
	COLORREF color = ::GetPixel(hDC, x, y);
#endif

	// return
	args.GetReturnValue().Set(v8::Integer::New(isolate, color));
}

// description: change aero mode to on/off
// syntax:		system.screen.setAero({true,false});
void ScreenSetAero(V8_FUNCTION_ARGS)
{
	if (args.Length() < 1)
		return;

	v8::Isolate *isolate = args.GetIsolate();
	bool bAeroOn = args[0]->BooleanValue();

	WCHAR strFullPath[MAX_PATH * 2];
	int nLen = ::GetSystemDirectory(strFullPath, MAX_PATH * 2);
	strFullPath[nLen] = L'\\';
	wcscpy(strFullPath + nLen + 1, L"dwmapi.dll");

	HMODULE hModule = LoadLibrary(strFullPath);
	HRESULT(WINAPI *fnDwnEnableComposition)(int) = (HRESULT(__stdcall*)(int))::GetProcAddress(hModule, "DwmEnableComposition");

	HRESULT hr = S_OK;
	if (fnDwnEnableComposition != NULL)
	{
		// DWM_EC_DISABLECOMPOSITION 0
		// DWM_EC_ENABLECOMPOSITION 1
		if (bAeroOn == FALSE)
			hr = fnDwnEnableComposition(0);
		else
			hr = fnDwnEnableComposition(1);
	}

	/*
	if (bAeroOn == false)
		hr = DwmEnableComposition(DWM_EC_DISABLECOMPOSITION);
	else
		hr = DwmEnableComposition(DWM_EC_ENABLECOMPOSITION);
	*/

	args.GetReturnValue().Set(v8::Boolean::New(isolate, SUCCEEDED(hr)));
	return;
}
