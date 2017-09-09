#include "stdafx.h"
#include "JsControl.h"

#include <NaLib/NaDebug.h>
#include <NaLib/NaImage.h>

#include "NaWindow.h"
#include "JsImage.h"

#include "NaControl.h"

Global<ObjectTemplate> JsControl::s_JsControlTemplate;
std::map<HWND, Persistent<Function, CopyablePersistentTraits<Function>>> JsControl::s_mapControlCallback;

JsControl::JsControl()
{
	m_pNativeControl = nullptr;

	NaDebugOut(L"JsControl(): 0x%08x\n", this);
}

JsControl::~JsControl()
{
	NaDebugOut(L"~JsControl(): 0x%08x\n", this);
}

// description: create function called by parent
void JsControl::Create(V8_FUNCTION_ARGS, NaWindow *pParent)
{
	NaControl *pControl = m_pNativeControl;

	String::Value type(args[0]);
	NaString strType((const wchar_t*)*type);
	if (strType.CompareNoCase(L"Button") == 0)
	{
		pControl->m_enType = NA_CONTROL_BUTTON;
	}
	else if (strType.CompareNoCase(L"Edit") == 0)
	{
		pControl->m_enType = NA_CONTROL_EDIT;
	}
	else if (strType.CompareNoCase(L"Static") == 0)
	{
		pControl->m_enType = NA_CONTROL_STATIC;
	}

	pControl->m_x = args[1]->Int32Value();
	pControl->m_y = args[2]->Int32Value();
	pControl->m_width = args[3]->Int32Value();
	pControl->m_height = args[4]->Int32Value();
	pControl->m_hParentWnd = pParent->m_hWnd;

	pControl->Create();

	if (args.Length() >= 6)
	{
		String::Value text(args[5]);
		pControl->SetText((const wchar_t*)*text);		
	}

	if (args.Length() >= 7)
	{
		bool bVisible = args[6]->BooleanValue();
		pControl->SetVisible(bVisible);
	}

	if (args.Length() >= 8)
	{
		if (!args[7].IsEmpty() && !args[7]->IsUndefined())
		{
			Local<Object> callback = args[7]->ToObject();
			if (callback->IsFunction())
			{
				Isolate *isolate = args.GetIsolate();
				Local<Function> callback_func = Local<Function>::Cast(args[7]);
				Persistent<Function, CopyablePersistentTraits<Function>> percy(isolate, callback_func);

				JsControl::s_mapControlCallback.insert(
					std::pair<HWND, Persistent<Function, CopyablePersistentTraits<Function>>>(pControl->m_hWnd, percy)
				);
			}
		}
	}
}

void JsControl::Destroy()
{
	NaControl *pControl = m_pNativeControl;

	if (pControl && pControl->m_hWnd)
	{
		auto it = JsControl::s_mapControlCallback.find(pControl->m_hWnd);
		if (it != JsControl::s_mapControlCallback.end())
		{
			JsControl::s_mapControlCallback.erase(pControl->m_hWnd);
		}

		// TODO check
		// DestroyWindow ?
	}
}

NaControl * JsControl::UnwrapNativeControl(Local<Object> obj)
{
	JsControl* pJsControl = reinterpret_cast<JsControl*>(UnwrapObject(obj));
	if (pJsControl && pJsControl->m_pNativeControl)
	{
		return pJsControl->m_pNativeControl;
	}

	return nullptr;
}

void JsControl::OnCommand(Isolate *isolate, HWND hWnd, int nCode)
{
	auto it = JsControl::s_mapControlCallback.find(hWnd);
	if (it != JsControl::s_mapControlCallback.end())
	{
		// Call callback-function
		Persistent<Function, CopyablePersistentTraits<Function>> percy = it->second;
		Local<Function> callback = Local<Function>::New(isolate, percy);
		if (!callback.IsEmpty())
		{
			int argc = 1;
			Local<Value> argv = { Integer::New(isolate, nCode) };			
			Local<Value> recv = isolate->GetCurrentContext()->Global();
			callback->Call(
				isolate->GetCurrentContext(),
				recv,
				argc,
				&argv
			);
		}
	}
}

Local<ObjectTemplate> JsControl::MakeObjectTemplate(Isolate * isolate)
{
	EscapableHandleScope handle_scope(isolate);

	Local<ObjectTemplate> templ = ObjectTemplate::New(isolate);
	templ->SetInternalFieldCount(1);

	// bind control methods
#define ADD_CONTROL_ACCESSOR(_prop)		ADD_OBJ_ACCESSOR(templ, _prop);
#define ADD_CONTROL_ACCESSOR_RO(_prop)	ADD_OBJ_ACCESSOR_RO(templ, _prop);
#define ADD_CONTROL_METHOD(_js_func)	ADD_TEMPLATE_METHOD(templ, _js_func);

	// accessor
	ADD_CONTROL_ACCESSOR(x);
	ADD_CONTROL_ACCESSOR(y);
	ADD_CONTROL_ACCESSOR(width);
	ADD_CONTROL_ACCESSOR(height);
	ADD_CONTROL_ACCESSOR(text);
	ADD_CONTROL_ACCESSOR(visible);
	ADD_CONTROL_ACCESSOR_RO(parent);
	ADD_CONTROL_ACCESSOR(image);
	ADD_CONTROL_ACCESSOR(callback);

	// methods
	ADD_CONTROL_METHOD(focus);

	return handle_scope.Escape(templ);
}

// description: x property getter/setter
void JsControl::get_x(V8_GETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaControl *pControl = UnwrapNativeControl(info.This());
	if (pControl)
	{
		auto rc = pControl->GetRect();
		pControl->m_x = rc.left;
	}

	V8Wrap::SetReturnValue(info, pControl->m_x);
}

void JsControl::set_x(V8_SETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaControl *pControl = UnwrapNativeControl(info.This());
	if (pControl)
	{
		auto rc = pControl->GetRect();
		pControl->Move(value->Int32Value(), rc.top, rc.right - rc.left, rc.bottom - rc.top);
	}
}

// description: y property getter/setter
void JsControl::get_y(V8_GETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaControl *pControl = UnwrapNativeControl(info.This());
	if (pControl)
	{
		auto rc = pControl->GetRect();
		pControl->m_y = rc.top;
	}

	V8Wrap::SetReturnValue(info, pControl->m_y);
}

void JsControl::set_y(V8_SETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaControl *pControl = UnwrapNativeControl(info.This());
	if (pControl)
	{
		auto rc = pControl->GetRect();
		pControl->Move(rc.left, value->Int32Value(), rc.right - rc.left, rc.bottom - rc.top);
	}
}

// description: width property getter/setter
void JsControl::get_width(V8_GETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaControl *pControl = UnwrapNativeControl(info.This());
	if (pControl)
	{
		auto rc = pControl->GetRect();
		pControl->m_width = rc.right - rc.left;
	}

	V8Wrap::SetReturnValue(info, pControl->m_width);
}

void JsControl::set_width(V8_SETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaControl *pControl = UnwrapNativeControl(info.This());
	if (pControl)
	{
		auto rc = pControl->GetRect();
		pControl->Move(rc.left, rc.top, value->Int32Value(), rc.bottom - rc.top);
	}
}

// description: height property getter/setter
void JsControl::get_height(V8_GETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaControl *pControl = UnwrapNativeControl(info.This());
	if (pControl)
	{
		auto rc = pControl->GetRect();
		pControl->m_height = rc.bottom - rc.top;
	}

	V8Wrap::SetReturnValue(info, pControl->m_height);
}

void JsControl::set_height(V8_SETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaControl *pControl = UnwrapNativeControl(info.This());
	if (pControl)
	{
		auto rc = pControl->GetRect();
		pControl->Move(rc.left, rc.top, rc.right - rc.left, value->Int32Value());
	}
}

// description: text property getter/setter
void JsControl::get_text(V8_GETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaControl *pControl = UnwrapNativeControl(info.This());
	if (pControl)
	{
		auto strText = pControl->GetText();
		V8Wrap::SetReturnValue(info, strText.wstr());
	}
}

void JsControl::set_text(V8_SETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaControl *pControl = UnwrapNativeControl(info.This());
	if (pControl)
	{
		String::Value str(value);
		pControl->SetText((const wchar_t*)*str);
	}
}

// description: visible property getter/setter
void JsControl::get_visible(Local<String> name, const PropertyCallbackInfo<Value>& info)
{
	UNUSED_PARAMETER(name);

	NaControl *pControl = UnwrapNativeControl(info.This());
	bool bVisible = false;
	if (pControl)
	{
		bVisible = pControl->IsVisible();
	}

	V8Wrap::SetReturnValue(info, bVisible);
}

void JsControl::set_visible(Local<String> name, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	UNUSED_PARAMETER(name);

	// get window object(this)
	Local<Object> obj = info.This();
	NaControl *pControl = UnwrapNativeControl(obj);
	bool bVisible = value->BooleanValue();
	if (pControl)
		pControl->SetVisible(bVisible);
}

// description: parent property getter
void JsControl::get_parent(Local<String> name, const PropertyCallbackInfo<Value>& info)
{
	UNUSED_PARAMETER(name);

	Isolate *isolate = info.GetIsolate();

	// #TODO Impl
	info.GetReturnValue().Set(
		Undefined(isolate)
	);
}

// description: image property getter/setter
void JsControl::get_image(Local<String> name, const PropertyCallbackInfo<Value>& info)
{
	UNUSED_PARAMETER(name);

	Isolate *isolate = info.GetIsolate();

	// #TODO Impl
	info.GetReturnValue().Set(
		Undefined(isolate)
	);
}

void JsControl::set_image(Local<String> name, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	UNUSED_PARAMETER(name);

	Isolate *isolate = info.GetIsolate();
	Local<Object> obj = info.This();
	NaControl *pControl = UnwrapNativeControl(obj);
	NaImage *pImage = nullptr;

	if (value->IsObject())
	{
		pImage = JsImage::UnwrapNativeImage(value->ToObject());
	}
	else if (value->IsString())
	{
		// #TODO Move to NaControl ?
		Local<StackTrace> stack_trace = StackTrace::CurrentStackTrace(isolate, 1, StackTrace::kScriptName);
		Local<StackFrame> cur_frame = stack_trace->GetFrame(0);
		NaString strBase(*String::Utf8Value(cur_frame->GetScriptName()));
		NaString strFilePath(*String::Utf8Value(value));

		NaUrl url;
		url.SetBase(strBase);
		url.SetUrl(strFilePath);

		NaString strFullPath(url.GetFullUrl());

		pImage = NaImage::Load(strFullPath.wstr());
	}

	// #TODO Move to NaControl
	BITMAP bitmap;
	GetObject(pImage->m_hBitmap, sizeof(BITMAP), &bitmap);

	LONG lStyle = GetWindowLong(pControl->m_hWnd, GWL_STYLE);
	switch (pControl->m_enType)
	{
	case NA_CONTROL_BUTTON:
		SetWindowLong(pControl->m_hWnd, GWL_STYLE, lStyle | BS_BITMAP);
		SendMessage(pControl->m_hWnd, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)pImage->m_hBitmap);
		break;
	case NA_CONTROL_EDIT:
		// TODO
		break;
	case NA_CONTROL_STATIC:
		SetWindowLong(pControl->m_hWnd, GWL_STYLE, lStyle | SS_BITMAP);
		SendMessage(pControl->m_hWnd, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)pImage->m_hBitmap);
		break;
	}
}

// description: callback property getter/setter
void JsControl::get_callback(Local<String> name, const PropertyCallbackInfo<Value>& info)
{
	UNUSED_PARAMETER(name);

	// #TODO Impl
	V8Wrap::SetReturnValue(info, L"NotImplemented");
}

void JsControl::set_callback(Local<String> name, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	UNUSED_PARAMETER(name);

	// get control object(this)
	Local<Object> obj = info.This();
	NaControl *pControl = UnwrapNativeControl(obj);
	
	if (!value.IsEmpty() && !value->IsUndefined())
	{
		Local<Object> callback = value->ToObject();
		if (callback->IsFunction())
		{
			Isolate *isolate = info.GetIsolate();
			Local<Function> callback_func = Local<Function>::Cast(value);
			Persistent<Function, CopyablePersistentTraits<Function>> percy(isolate, callback_func);

			JsControl::s_mapControlCallback.insert(
				std::pair<HWND, Persistent<Function, CopyablePersistentTraits<Function>>>(pControl->m_hWnd, percy)
			);
		}
	}
}

// description: set focus to control
// syntax:		controlObj.focus();
void JsControl::method_focus(V8_FUNCTION_ARGS)
{
	NaControl *pControl = UnwrapNativeControl(args.This());
	if (pControl)
	{
		pControl->SetFocus();

		V8Wrap::SetReturnValue(args, true);
		return;
	}

	V8Wrap::SetReturnValue(args, false);
}