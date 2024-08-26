#include "stdafx.h"
#include "JsImage.h"

#include <NaLib/NaString.h>
#include <NaLib/NaDebug.h>
#include <NaLib/NaImage.h>

#include "JsScreen.h"

Global<ObjectTemplate> JsImage::s_JsImageTemplate;

JsImage::JsImage()
{
	m_pNativeImage = nullptr;
	NaDebugOut(L"JsImage(): 0x%08x\n", this);
}

JsImage::~JsImage()
{
	NaDebugOut(L"~JsImage(): 0x%08x\n", this);
	if (m_pNativeImage)
	{
		delete m_pNativeImage;
		m_pNativeImage = nullptr;
	}
}

NaImage * JsImage::UnwrapNativeImage(Local<Object> obj)
{
	JsImage* pJsImage = static_cast<JsImage*>(UnwrapObject(obj));
	if (pJsImage && pJsImage->m_pNativeImage)
	{
		return pJsImage->m_pNativeImage;
	}

	return nullptr;
}

Local<ObjectTemplate> JsImage::MakeObjectTemplate(Isolate * isolate)
{
	EscapableHandleScope handle_scope(isolate);

	Local<ObjectTemplate> templ = ObjectTemplate::New(isolate);
	templ->SetInternalFieldCount(1);

	// bind image methods
#define ADD_IMAGE_PROPERTY(_prop)	ADD_OBJ_PROPERTY(templ, _prop);
#define ADD_IMAGE_METHOD(_js_func)	ADD_TEMPLATE_METHOD(templ, _js_func);

	// accessor
	ADD_IMAGE_PROPERTY(width);
	ADD_IMAGE_PROPERTY(height);

	// methods
	ADD_IMAGE_METHOD(getPixel);
	ADD_IMAGE_METHOD(findImage);
	ADD_IMAGE_METHOD(reset);
	ADD_IMAGE_METHOD(save);

	return handle_scope.Escape(templ);
}

// description: width property getter/setter
void JsImage::get_width(V8_GETTER_ARGS)
{
	UNUSED(name);

	NaImage *pImage = UnwrapNativeImage(info.This());
	int nWidth = 0;
	if (pImage)
	{
		nWidth = pImage->m_rc.right - pImage->m_rc.left;
	}

	V8_PROP_RET(nWidth);
}

void JsImage::set_width(V8_SETTER_ARGS)
{
	UNUSED(name);
	UNUSED(value);

	NaImage *pImage = UnwrapNativeImage(info.This());
	if (pImage)
	{
		// Not Impl
	}
}

// description: height property getter/setter
void JsImage::get_height(V8_GETTER_ARGS)
{
	UNUSED(name);

	NaImage *pImage = UnwrapNativeImage(info.This());
	int nHeight = 0;
	if (pImage)
	{
		nHeight = pImage->m_rc.bottom - pImage->m_rc.top;
	}

	V8_PROP_RET(nHeight);
}

void JsImage::set_height(V8_SETTER_ARGS)
{
	UNUSED(name);
	UNUSED(value);

	NaImage *pImage = UnwrapNativeImage(info.This());
	if (pImage)
	{
		// Not Impl
	}
}

// description: constructor function
// syntax:		new Window([x, y[, width, height]]) : windowObj
void JsImage::method_constructor(V8_METHOD_ARGS)
{
	if (args.Length() >= 1)
	{
		String::Value filepath(args[0]);

		Local<StackTrace> stack_trace = StackTrace::CurrentStackTrace(
			args.GetIsolate(), 1, StackTrace::kScriptName);
		Local<StackFrame> cur_frame = stack_trace->GetFrame(0);
		NaString strBase(*String::Utf8Value(cur_frame->GetScriptName()));
		NaString strFilePath(*String::Utf8Value(args[0]));

		NaUrl url;
		url.SetBase(strBase);
		url.SetUrl(strFilePath);

		NaString strFullPath(url.GetFullUrl());

		JsImage *pJsImage = new JsImage();
		pJsImage->m_pNativeImage = NaImage::Load(strFullPath.wstr());
		Local<Object> obj = WrapObject(args.GetIsolate(), pJsImage);

		V8_METHOD_RET(obj);
		return;
	}

	V8Wrap::NullReturnValue(args);
}

// description: get pixel from image buffer
// syntax:		imageObj.getPixel(x, y);
void JsImage::method_getPixel(V8_METHOD_ARGS)
{
	NaImage *pImage = UnwrapNativeImage(args.This());
	if (pImage == nullptr)
	{
		// error
		V8_METHOD_RET(-1);
		return;
	}

	if (args.Length() < 2)
	{
		// error
		V8_METHOD_RET(-1);
		return;
	}

	int x = args[0]->Int32Value();
	int y = args[1]->Int32Value();
	auto color = pImage->GetPixel(x, y);

	// return
	V8_METHOD_RET((int)color);
}

// description: find image(argument) from image(this)
// syntax:		imageObj.findImage(image_object, accuracy_factor)
void JsImage::method_findImage(V8_METHOD_ARGS)
{
	NaImage *pImageThis = UnwrapNativeImage(args.This());
	if (pImageThis == nullptr)
	{
		V8Wrap::NullReturnValue(args);
		return;
	}

	if (args.Length() < 1)
	{
		V8Wrap::NullReturnValue(args);
		return;
	}

	if (!args[0]->IsObject())
	{
		V8Wrap::NullReturnValue(args);
		return;
	}

	Local<Object> objFind = args[0]->ToObject();
	NaImage *pImageFind = UnwrapNativeImage(objFind);
	if (pImageFind == nullptr)
	{
		V8Wrap::NullReturnValue(args);
		return;
	}

	int nAccuracyFactor = 0;
	if (args.Length() >= 2)
	{
		nAccuracyFactor = args[1]->Int32Value();
		if (nAccuracyFactor < 0)
			nAccuracyFactor = 0;
	}

	POINT pt = NaImage::SearchImageInImage(pImageThis, pImageFind, nAccuracyFactor);
	if (pt.x == -1 || pt.y == -1)
	{
		// not found!
		V8Wrap::NullReturnValue(args);
		return;
	}

	auto isolate = args.GetIsolate();

	// return result object(Point)
	Local<Object> objRet = Object::New(isolate);
	objRet->Set(
		String::NewFromUtf8(isolate, "x", NewStringType::kNormal).ToLocalChecked(),
		Integer::New(isolate, pt.x)
	);
	objRet->Set(
		String::NewFromUtf8(isolate, "y", NewStringType::kNormal).ToLocalChecked(),
		Integer::New(isolate, pt.y)
	);

	// return
	V8_METHOD_RET(objRet);
}

// description: reset image buffer
// syntax:		imageObj.reset()
void JsImage::method_reset(V8_METHOD_ARGS)
{
	NaImage *pImage = UnwrapNativeImage(args.This());
	if (pImage == nullptr)
	{
		V8_METHOD_RET(false);
		return;
	}
	
	JsImage* pJsImage = static_cast<JsImage*>(UnwrapObject(args.This()));
	if (pJsImage && pJsImage->m_pNativeImage)
	{
		delete pJsImage->m_pNativeImage;
		pJsImage->m_pNativeImage = nullptr;
	}

	V8_METHOD_RET(true);
}

// description: save image buffer to file
// syntax:		imageObj.save(filename)
void JsImage::method_save(V8_METHOD_ARGS)
{
	NaImage *pImage = UnwrapNativeImage(args.This());
	if (pImage == nullptr || args.Length() < 1)
	{
		V8_METHOD_RET(false);
		return;
	}

	String::Value strV8(args[0]);
	bool bRet = pImage->Save((wchar_t*)*strV8);

	V8_METHOD_RET(bRet);
}
