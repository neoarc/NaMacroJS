#include "stdafx.h"
#include "ScreenModule.h"

#include "NaImage.h"

HWND NaScreenModule::m_hDesktopWnd = NULL;
HDC NaScreenModule::m_hDesktopDC = NULL;
bool NaScreenModule::m_bAeroStatus = false;

void NaScreenModule::Init(Isolate * isolate, Local<ObjectTemplate>& /*global_template*/)
{
	HandleScope handle_scope(isolate);

	Local<Object> global = isolate->GetCurrentContext()->Global();
	Local<Object> screen_obj = GetScreenObject(isolate);

#define ADD_SCREEN_ACCESSOR(_prop)		ADD_OBJ_ACCESSOR(screen_obj, _prop);
#define ADD_SCREEN_ACCESSOR_RO(_prop)	ADD_OBJ_ACCESSOR_RO(screen_obj, _prop);
#define ADD_SCREEN_METHOD(_js_func)		ADD_OBJ_METHOD(screen_obj, _js_func);

	// accessors
	ADD_SCREEN_ACCESSOR_RO(width);
	ADD_SCREEN_ACCESSOR_RO(height);

	// methods
	ADD_SCREEN_METHOD(capture); 
	ADD_SCREEN_METHOD(findColor);
	ADD_SCREEN_METHOD(getPixel);
	ADD_SCREEN_METHOD(setAero);
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

Local<Object> NaScreenModule::GetScreenObject(Isolate * isolate)
{
	// HandleScope 안에서 호출

	Local<Object> system_obj = V8Wrap::GetSystemObject(isolate);
	Local<String> screen_name = String::NewFromUtf8(isolate, "screen", NewStringType::kNormal).ToLocalChecked();
	Local<Value> screen_value = system_obj->Get(screen_name);
	if (!screen_value.IsEmpty() && screen_value->IsUndefined())
	{
		// InitScreen
		screen_value = Object::New(isolate);
		system_obj->Set(screen_name, screen_value);
	}

	Local<Object> screen_obj = screen_value->ToObject();
	return screen_obj;
}

// description: width property getter
void NaScreenModule::get_width(V8_GETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	Isolate *isolate = info.GetIsolate();
	int metrics = GetSystemMetrics(SM_CXSCREEN);

	info.GetReturnValue().Set(
		Integer::New(isolate, metrics)
		);
}

// description: height property getter
void NaScreenModule::get_height(V8_GETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	Isolate *isolate = info.GetIsolate();
	int metrics = GetSystemMetrics(SM_CYSCREEN);

	info.GetReturnValue().Set(
		Integer::New(isolate, metrics)
		);
}

// description: capture screen area to image object
// syntax:		system.screen.capture(x, y, width, height) : image object
void NaScreenModule::method_capture(V8_FUNCTION_ARGS)
{
	Isolate *isolate = args.GetIsolate();

	if (args.Length() < 4)
	{
		// error
		args.GetReturnValue().Set(Integer::New(isolate, -1));
		return;
	}

	int x = args[0]->Int32Value();
	int y = args[1]->Int32Value();
	int w = args[2]->Int32Value();
	int h = args[3]->Int32Value();

	NaImage *pImage = NaImage::CaptureScreen(x, y, w, h);
	auto image_obj = NaImage::WrapObject(isolate, pImage);

	// return
	args.GetReturnValue().Set(
		image_obj
		);
}

// description: find specific color from area
// syntax:		system.screen.findColor(color[, x, y, width, height])
void NaScreenModule::method_findColor(V8_FUNCTION_ARGS)
{
	Isolate *isolate = args.GetIsolate();

	if (args.Length() < 1)
	{
		// error
		args.GetReturnValue().SetNull();
		return;
	}

	int color = args[0]->Int32Value();
	int x = 0, y = 0, w = -1, h = -1;
	if (args.Length() >= 3)
	{
		x = args[1]->Int32Value();
		y = args[2]->Int32Value();
		if (args.Length() >= 5)
		{
			w = args[3]->Int32Value();
			h = args[4]->Int32Value();
		}
	}

	if (w < 0 || h < 0)
	{
		w = GetSystemMetrics(SM_CXSCREEN);
		h = GetSystemMetrics(SM_CYSCREEN);
	}

	NaImage *pImage = NaImage::CaptureScreen(x, y, w, h);
	POINT pt = pImage->FindColor(color);
	delete pImage;

	if (pt.x == -1 || pt.y == -1)
	{
		// error
		args.GetReturnValue().SetNull();
		return;
	}

	// make info object
	Local<Object> info_obj = Object::New(isolate);
	info_obj->Set(
		String::NewFromUtf8(isolate, "x", NewStringType::kNormal).ToLocalChecked(),
		Integer::New(isolate, x + pt.x)
	);
	info_obj->Set(
		String::NewFromUtf8(isolate, "y", NewStringType::kNormal).ToLocalChecked(),
		Integer::New(isolate, y + pt.y)
	);

	// return
	args.GetReturnValue().Set(
		info_obj
	);
}

// description: get piyel color from point x,y
// syntax:		system.screen.getPixel(x, y) : color
void NaScreenModule::method_getPixel(V8_FUNCTION_ARGS)
{
	Isolate *isolate = args.GetIsolate();

	if (args.Length() < 2)
	{
		// error
		args.GetReturnValue().Set(Integer::New(isolate, -1));
		return;
	}

	int x = args[0]->Int32Value();
	int y = args[1]->Int32Value();

	HDC hDC = NaScreenModule::GetDesktopDC();

	// get pixel from point
#define USE_FAST_GETPIXEL
#ifdef USE_FAST_GETPIXEL
	HDC hMemoryDC = ::CreateCompatibleDC(hDC);
	if (hMemoryDC == nullptr)
	{
// 		DWORD dwError = ::GetLastError();
		// TODO error handling
		return;
	}

	HBITMAP hBmp = ::CreateCompatibleBitmap(hDC, 1, 1);
	::SelectObject(hMemoryDC, hBmp);
	::BitBlt(hMemoryDC, 0, 0, 1, 1, hDC, x, y, SRCCOPY);

	COLORREF color = ::GetPixel(hMemoryDC, 0, 0);
	::DeleteDC(hMemoryDC);
#else
	COLORREF color = ::GetPixel(hDC, x, y);
#endif
	// TODO check GetPixel Failed when ReleaseDC
	//::ReleaseDC(NaScreenModule::GetDesktopHWND(), hDC);

	// return
	args.GetReturnValue().Set(Integer::New(isolate, color));
}

// description: change aero mode to on/off
// syntax:		system.screen.setAero({true,false});
void NaScreenModule::method_setAero(V8_FUNCTION_ARGS)
{
	if (args.Length() < 1)
		return;

	Isolate *isolate = args.GetIsolate();
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

	args.GetReturnValue().Set(Boolean::New(isolate, SUCCEEDED(hr)));
	return;
}
