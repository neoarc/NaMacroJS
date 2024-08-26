#include "stdafx.h"
#include "JsScreen.h"

#include <NaLib/NaImage.h>
#include <NaLib/NaDesktop.h>

#include "JsImage.h"
#include "NaScreen.h"

void JsScreen::Init(Isolate * isolate, Local<ObjectTemplate>& /*global_template*/)
{
	HandleScope handle_scope(isolate);

	auto global = isolate->GetCurrentContext()->Global();
	auto screenObj = GetScreenObject(isolate);

#define ADD_SCREEN_PROPERTY(_prop)		ADD_OBJ_PROPERTY(screenObj, _prop);
#define ADD_SCREEN_PROPERTY_RO(_prop)	ADD_OBJ_PROPERTY_RO(screenObj, _prop);
#define ADD_SCREEN_METHOD(_js_func)		ADD_OBJ_METHOD(screenObj, _js_func);

	// accessors
	ADD_SCREEN_PROPERTY_RO(width);
	ADD_SCREEN_PROPERTY_RO(height);

	// methods
	ADD_SCREEN_METHOD(capture); 
	ADD_SCREEN_METHOD(findColor);
	ADD_SCREEN_METHOD(getPixel);
	ADD_SCREEN_METHOD(setAero);
}

void JsScreen::Release()
{
}

// must called in HandleScope
Local<Object> JsScreen::GetScreenObject(Isolate * isolate)
{
	auto systemObj = V8Wrap::GetSystemObject(isolate);
	auto screenName = String::NewFromUtf8(isolate, "screen", NewStringType::kNormal).ToLocalChecked();
	auto screenValue = systemObj->Get(screenName);
	if (!screenValue.IsEmpty() && screenValue->IsUndefined())
	{
		// InitScreen
		screenValue = Object::New(isolate);
		systemObj->Set(screenName, screenValue);
	}

	auto screenObj = screenValue->ToObject();
	return screenObj;
}

void JsScreen::get_width(V8_GETTER_ARGS)
{
	auto value = NaScreen::GetWidth();
	V8_PROP_RET(value);
}

void JsScreen::get_height(V8_GETTER_ARGS)
{
	auto value = NaScreen::GetHeight();
	V8_PROP_RET(value);
}

// Capture screen area to image object
// system.screen.capture(x, y, width, height) : image object
void JsScreen::method_capture(V8_METHOD_ARGS)
{
	if (args.Length() < 4)
	{
		// error
		V8_METHOD_RET(-1);
		return;
	}

	auto x = args[0]->Int32Value();
	auto y = args[1]->Int32Value();
	auto w = args[2]->Int32Value();
	auto h = args[3]->Int32Value();

	auto pJsImage = new JsImage();
	pJsImage->m_pNativeImage = NaImage::CaptureScreen(x, y, w, h);

	auto isolate = args.GetIsolate();
	auto imageObj = JsImage::WrapObject(isolate, pJsImage);

	V8_METHOD_RET(imageObj);
}

// Find specific color from area
// system.screen.findColor(color[, x, y, width, height])
void JsScreen::method_findColor(V8_METHOD_ARGS)
{
	auto isolate = args.GetIsolate();

	if (args.Length() < 1)
	{
		// error
		V8Wrap::NullReturnValue(args);
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

	auto pImage = NaImage::CaptureScreen(x, y, w, h);
	auto pt = pImage->FindColor(color);
	delete pImage;

	if (pt.x == -1 || pt.y == -1)
	{
		// error
		V8Wrap::NullReturnValue(args);
		return;
	}

	auto infoObj = Object::New(isolate);
	infoObj->Set(V8_STRING("x"), V8_INT(x + pt.x));
	infoObj->Set(V8_STRING("y"), V8_INT(y + pt.y));

	V8_METHOD_RET(infoObj);
}

// Get pixel color from point x,y
// system.screen.getPixel(x, y) : color
void JsScreen::method_getPixel(V8_METHOD_ARGS)
{
	if (args.Length() < 2)
	{
		// error
		V8_METHOD_RET(-1);
		return;
	}

	auto x = args[0]->Int32Value();
	auto y = args[1]->Int32Value();

	auto color = NaScreen::GetPixel(x, y);
	V8_METHOD_RET((int)color);
}

// Change aero mode to on/off
// system.screen.setAero({true,false});
void JsScreen::method_setAero(V8_METHOD_ARGS)
{
	if (args.Length() < 1)
		return;

	auto isAeroOn = args[0]->BooleanValue();
	auto hr = NaScreen::SetAero(isAeroOn);
	V8_METHOD_RET(SUCCEEDED(hr));
}
