#include "stdafx.h"
#include "NaControl.h"

#include <NaLib/NaDebug.h>

#include "NaWindow.h"
#include "NaImage.h"

Global<ObjectTemplate> NaControl::s_NaControlTemplate;
std::map<HWND, Persistent<Function, CopyablePersistentTraits<Function>>> NaControl::s_mapControlCallback;

NaControl::NaControl()
{
	m_enType = NA_CONTROL_UNKNOWN;
	m_hWnd = nullptr;
	m_hParentWnd = nullptr;
	m_x = 0;
	m_y = 0;
	m_width = 0;
	m_height = 0;

	NaDebug::Out(L"NaControl(): 0x%08x\n", this);
}


NaControl::~NaControl()
{
	NaDebug::Out(L"~NaControl(): 0x%08x\n", this);
}

HWND NaControl::Create()
{
	NaString strClass;
	switch (m_enType)
	{
	case NA_CONTROL_BUTTON:
		strClass = L"Button";
		break;
	case NA_CONTROL_EDIT:
		strClass = L"Edit";
		break;
	case NA_CONTROL_STATIC:
	default:
		strClass = L"Static";
		break;
	}

	HINSTANCE hInstance = GetModuleHandle(NULL);
	DWORD dwStyle = WS_CHILD;
	m_hWnd = ::CreateWindow(
		strClass.wstr(),
		L"", // _In_opt_ LPCTSTR   lpWindowName,
		dwStyle, // _In_     DWORD     dwStyle,
		m_x,
		m_y,
		m_width,
		m_height,
		m_hParentWnd, // _In_opt_ HWND      hWndParent,
		NULL, // _In_opt_ HMENU     hMenu,
		hInstance,
		NULL // _In_opt_ LPVOID    lpParam
	);
	::SetMenu(m_hWnd, (HMENU)m_hWnd);

	return m_hWnd;
}

// description: create function called by parent
void NaControl::Create(V8_FUNCTION_ARGS, NaWindow *pParent)
{
	NaControl *pControl = this;

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
		::SetWindowText(pControl->m_hWnd, (const wchar_t*)*text);
	}

	if (args.Length() >= 7)
	{
		bool bVisible = args[6]->BooleanValue();
		::ShowWindow(pControl->m_hWnd, bVisible ? SW_SHOW : SW_HIDE);
	}

	if (args.Length() >= 8)
	{
		Local<Object> callback = args[7]->ToObject();
		if (callback->IsFunction())
		{
			Isolate *isolate = args.GetIsolate();
			Local<Function> callback_func = Local<Function>::Cast(args[7]);
			Persistent<Function, CopyablePersistentTraits<Function>> percy(isolate, callback_func);

			NaControl::s_mapControlCallback.insert(
				std::pair<HWND, Persistent<Function, CopyablePersistentTraits<Function>>>(pControl->m_hWnd, percy)
			);
		}
	}
}

void NaControl::Destroy()
{
	if (m_hWnd)
	{
		std::map <HWND, Persistent<Function, CopyablePersistentTraits<Function>>>::iterator it;
		it = NaControl::s_mapControlCallback.find(m_hWnd);
		if (it != NaControl::s_mapControlCallback.end())
		{
			NaControl::s_mapControlCallback.erase(m_hWnd);
		}

		// TODO check
		// DestroyWindow ?
	}
}

void NaControl::OnCommand(Isolate *isolate, HWND hWnd, int nCode)
{
	std::map <HWND, Persistent<Function, CopyablePersistentTraits<Function>>>::iterator it;
	it = NaControl::s_mapControlCallback.find(hWnd);
	if (it != NaControl::s_mapControlCallback.end())
	{
		Persistent<Function, CopyablePersistentTraits<Function>> percy = it->second;
		Local<Function> callback = Local<Function>::New(isolate, percy);
		if (!callback.IsEmpty())
		{
			int argc = 1;
			Local<Value> argv = {
				Integer::New(isolate, nCode)
			};
			
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

Local<ObjectTemplate> NaControl::MakeObjectTemplate(Isolate * isolate)
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

	// methods
	ADD_CONTROL_METHOD(focus);

	return handle_scope.Escape(templ);
}

// description: x property getter/setter
void NaControl::get_x(V8_GETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaControl *pControl = reinterpret_cast<NaControl*>(UnwrapObject(info.This()));
	Isolate *isolate = info.GetIsolate();
	if (pControl && pControl->m_hWnd)
	{
		RECT rc;
		::GetWindowRect(pControl->m_hWnd, &rc);
		pControl->m_x = rc.left;
	}

	info.GetReturnValue().Set(
		Integer::New(isolate, pControl->m_x)
	);
}

void NaControl::set_x(V8_SETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaControl *pControl = reinterpret_cast<NaControl*>(UnwrapObject(info.This()));
	if (pControl)
	{
		RECT rc;
		::GetWindowRect(pControl->m_hWnd, &rc);
		::MoveWindow(pControl->m_hWnd, value->Int32Value(), rc.top, rc.right - rc.left, rc.bottom - rc.top, FALSE);
	}
}

// description: y property getter/setter
void NaControl::get_y(V8_GETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaControl *pControl = reinterpret_cast<NaControl*>(UnwrapObject(info.This()));
	Isolate *isolate = info.GetIsolate();
	if (pControl && pControl->m_hWnd)
	{
		RECT rc;
		::GetWindowRect(pControl->m_hWnd, &rc);
		pControl->m_y = rc.top;
	}

	info.GetReturnValue().Set(
		Integer::New(isolate, pControl->m_y)
	);
}

void NaControl::set_y(V8_SETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaControl *pControl = reinterpret_cast<NaControl*>(UnwrapObject(info.This()));
	if (pControl)
	{
		RECT rc;
		::GetWindowRect(pControl->m_hWnd, &rc);
		::MoveWindow(pControl->m_hWnd, rc.left, value->Int32Value(), rc.right - rc.left, rc.bottom - rc.top, FALSE);
	}
}

// description: width property getter/setter
void NaControl::get_width(V8_GETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaControl *pControl = reinterpret_cast<NaControl*>(UnwrapObject(info.This()));
	Isolate *isolate = info.GetIsolate();
	if (pControl && pControl->m_hWnd)
	{
		RECT rc;
		::GetWindowRect(pControl->m_hWnd, &rc);
		pControl->m_width = rc.right - rc.left;
	}

	info.GetReturnValue().Set(
		Integer::New(isolate, pControl->m_width)
	);
}

void NaControl::set_width(V8_SETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaControl *pControl = reinterpret_cast<NaControl*>(UnwrapObject(info.This()));
	if (pControl)
	{
		RECT rc;
		::GetWindowRect(pControl->m_hWnd, &rc);
		::MoveWindow(pControl->m_hWnd, rc.left, rc.top, value->Int32Value(), rc.bottom - rc.top, FALSE);
	}
}

// description: height property getter/setter
void NaControl::get_height(V8_GETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaControl *pControl = reinterpret_cast<NaControl*>(UnwrapObject(info.This()));
	Isolate *isolate = info.GetIsolate();
	if (pControl && pControl->m_hWnd)
	{
		RECT rc;
		::GetWindowRect(pControl->m_hWnd, &rc);
		pControl->m_height = rc.bottom - rc.top;
	}

	info.GetReturnValue().Set(
		Integer::New(isolate, pControl->m_height)
	);
}

void NaControl::set_height(V8_SETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaControl *pControl = reinterpret_cast<NaControl*>(UnwrapObject(info.This()));
	if (pControl)
	{
		RECT rc;
		::GetWindowRect(pControl->m_hWnd, &rc);
		::MoveWindow(pControl->m_hWnd, rc.left, rc.top, rc.right - rc.left, value->Int32Value(), FALSE);
	}
}

// description: text property getter/setter
void NaControl::get_text(V8_GETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaControl *pControl = reinterpret_cast<NaControl*>(UnwrapObject(info.This()));
	if (pControl)
	{
		wchar_t str[1024];
		::GetWindowText(pControl->m_hWnd, str, 1024);
		V8Wrap::SetTxtPropertyCallbackInfo(info, str);
	}
}

void NaControl::set_text(V8_SETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaControl *pControl = reinterpret_cast<NaControl*>(UnwrapObject(info.This()));
	if (pControl)
	{
		String::Value str(value);
		::SetWindowText(pControl->m_hWnd, (const wchar_t*)*str);
	}
}

// description: visible property getter/setter
void NaControl::get_visible(Local<String> name, const PropertyCallbackInfo<Value>& info)
{
	UNUSED_PARAMETER(name);

	NaControl *pControl = reinterpret_cast<NaControl*>(UnwrapObject(info.This()));
	Isolate *isolate = info.GetIsolate();

	bool bVisible = false;
	if (pControl)
		bVisible = (::IsWindowVisible(pControl->m_hWnd) == TRUE);

	info.GetReturnValue().Set(
		Boolean::New(isolate, bVisible)
	);
}

void NaControl::set_visible(Local<String> name, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	UNUSED_PARAMETER(name);

	// get window object(this)
	Local<Object> obj = info.This();
	NaControl *pControl = reinterpret_cast<NaControl*>(UnwrapObject(obj));

	bool bVisible = value->BooleanValue();
	
	if (pControl && pControl->m_hWnd)
		::ShowWindow(pControl->m_hWnd, bVisible ? SW_SHOW : SW_HIDE);
}

// description: parent property getter
void NaControl::get_parent(Local<String> name, const PropertyCallbackInfo<Value>& info)
{
	UNUSED_PARAMETER(name);

	Isolate *isolate = info.GetIsolate();

	// TODO Impl
	info.GetReturnValue().Set(
		Undefined(isolate)
	);
}

// description: image property getter/setter
void NaControl::get_image(Local<String> name, const PropertyCallbackInfo<Value>& info)
{
	UNUSED_PARAMETER(name);

	Isolate *isolate = info.GetIsolate();

	// TODO Impl
	info.GetReturnValue().Set(
		Undefined(isolate)
	);
}

void NaControl::set_image(Local<String> name, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	UNUSED_PARAMETER(name);

	// get window object(this)
	Isolate *isolate = info.GetIsolate();
	Local<Object> obj = info.This();
	NaControl *pControl = reinterpret_cast<NaControl*>(UnwrapObject(obj));
	NaImage *pImage = nullptr;

	if (value->IsObject())
	{
		pImage = reinterpret_cast<NaImage*>(
			NaImage::UnwrapObject(value->ToObject())
			);		
	}
	else if (value->IsString())
	{
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

// description: set focus to control
// syntax:		controlObj.focus();
void NaControl::method_focus(V8_FUNCTION_ARGS)
{
	NaControl *pControl = reinterpret_cast<NaControl*>(UnwrapObject(args.This()));

	HWND hWnd = pControl->m_hWnd;
	Isolate *isolate = args.GetIsolate();
	if (hWnd)
	{
		SetFocus(hWnd);

		args.GetReturnValue().Set(
			Boolean::New(isolate, true)
		);
		return;
	}

	args.GetReturnValue().Set(
		Boolean::New(isolate, false)
	);
}