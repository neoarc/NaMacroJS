#include "stdafx.h"
#include "JsWindow.h"

#include <NaLib/NaDebug.h>
#include <NaLib/NaNotifyWindow.h>

#include "BasicModule.h"
#include "NaControl.h"
#include "NaWindow.h"

#include "V8Wrap.h"
#include "resource.h"

Global<ObjectTemplate> JsWindow::s_JsWindowTemplate;

JsWindow::JsWindow()
{
	m_pNativeWindow = nullptr;

	//NaDebug::Out(L"JsWindow(): 0x%08x, %d\n", this, enType);
}

JsWindow::~JsWindow()
{
	// #TODO delete m_pNativeWindow

	//NaDebug::Out(L"~JsWindow(): 0x%08x\n", this);
}

NaWindow * JsWindow::UnwrapNativeWindow(Local<Object> obj)
{
	JsWindow* pJsWindow = static_cast<JsWindow*>(UnwrapObject(obj));
	if (pJsWindow && pJsWindow->m_pNativeWindow)
	{
		return pJsWindow->m_pNativeWindow;
	}

	return nullptr;
}

void JsWindow::FindWindows(Isolate * isolate, const wchar_t * name, Local<Array>& results)
{
	NaWindow::FindWindowsInfo info;
	info.name = const_cast<wchar_t*>(name);

	NaWindow::FindWindows(name, info);

	// Wrap HWND to V8Object
	int nIndex = 0;

	auto it = info.foundlist.begin();
	for (; it != info.foundlist.end(); ++it)
	{
		HWND hWnd = *it;

		JsWindow *pJsWindow = new JsWindow();
		pJsWindow->m_pNativeWindow = NaWindow::GetWindow(hWnd);

		Local<Value> obj = JsWindow::WrapObject(isolate, pJsWindow);

		// Fill V8Object Array
		results->Set(nIndex++, obj);
	}
}

Local<ObjectTemplate> JsWindow::MakeObjectTemplate(Isolate * isolate)
{
	EscapableHandleScope handle_scope(isolate);

	Local<ObjectTemplate> templ = ObjectTemplate::New(isolate);
	templ->SetInternalFieldCount(1);

	// bind window methods
#define ADD_WINDOW_ACCESSOR(_prop)		ADD_OBJ_ACCESSOR(templ, _prop);
#define ADD_WINDOW_ACCESSOR_RO(_prop)	ADD_OBJ_ACCESSOR_RO(templ, _prop);
#define ADD_WINDOW_METHOD(_js_func)		ADD_TEMPLATE_METHOD(templ, _js_func);

	// accessor
	ADD_WINDOW_ACCESSOR(x);
	ADD_WINDOW_ACCESSOR(y);
	ADD_WINDOW_ACCESSOR(width);
	ADD_WINDOW_ACCESSOR(height);
	ADD_WINDOW_ACCESSOR_RO(clientWidth);
	ADD_WINDOW_ACCESSOR_RO(clientHeight);
	ADD_WINDOW_ACCESSOR_RO(class);
	ADD_WINDOW_ACCESSOR(text);
	ADD_WINDOW_ACCESSOR(visible);
	ADD_WINDOW_ACCESSOR(topmost);
	ADD_WINDOW_ACCESSOR(handle);
	ADD_WINDOW_ACCESSOR(state);

	// methods
	ADD_WINDOW_METHOD(create);
	ADD_WINDOW_METHOD(move);
	ADD_WINDOW_METHOD(activate);
	ADD_WINDOW_METHOD(close);
	ADD_WINDOW_METHOD(alert);
	ADD_WINDOW_METHOD(addControl);

	return handle_scope.Escape(templ);
}

// description: x property getter/setter
void JsWindow::get_x(V8_GETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaWindow *pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		auto rc = pWindow->GetRect();
		pWindow->m_x = rc.left;
	}

	V8Wrap::SetReturnValue(info, pWindow->m_x);
}

void JsWindow::set_x(V8_SETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaWindow *pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		auto rc = pWindow->GetRect();
		pWindow->Move(value->Int32Value(), rc.top, rc.right - rc.left, rc.bottom - rc.top);
	}
}

// description: y property getter/setter
void JsWindow::get_y(V8_GETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaWindow *pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		auto rc = pWindow->GetRect();
		pWindow->m_y = rc.top;
	}

	V8Wrap::SetReturnValue(info, pWindow->m_y);
}

void JsWindow::set_y(V8_SETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaWindow *pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		auto rc = pWindow->GetRect();
		pWindow->Move(rc.left, value->Int32Value(), rc.right - rc.left, rc.bottom - rc.top);
	}
}

// description: width property getter/setter
void JsWindow::get_width(V8_GETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaWindow *pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		auto rc = pWindow->GetRect();
		pWindow->m_width = rc.right - rc.left;
	}

	V8Wrap::SetReturnValue(info, pWindow->m_width);
}

void JsWindow::set_width(V8_SETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaWindow *pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		auto rc = pWindow->GetRect();
		pWindow->Move(rc.left, rc.top, value->Int32Value(), rc.bottom - rc.top);
	}
}

// description: height property getter/setter
void JsWindow::get_height(V8_GETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaWindow *pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		auto rc = pWindow->GetRect();
		pWindow->m_height = rc.bottom - rc.top;
	}

	V8Wrap::SetReturnValue(info, pWindow->m_height);
}

void JsWindow::set_height(V8_SETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaWindow *pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		auto rc = pWindow->GetRect();
		pWindow->Move(rc.left, rc.top, rc.right - rc.left, value->Int32Value());
	}
}

// description: client width property getter
void JsWindow::get_clientWidth(V8_GETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaWindow *pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		auto rc = pWindow->GetClientRect();
		pWindow->m_clientWidth = rc.right - rc.left;
	}

	V8Wrap::SetReturnValue(info, pWindow->m_clientWidth);
}

// description: client height property getter
void JsWindow::get_clientHeight(V8_GETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaWindow *pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		auto rc = pWindow->GetClientRect();
		pWindow->m_clientHeight = rc.bottom - rc.top;
	}

	V8Wrap::SetReturnValue(info, pWindow->m_clientHeight);
}

// description: class property getter/setter
void JsWindow::get_class(V8_GETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaWindow *pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		auto strClass = pWindow->GetClass();
		V8Wrap::SetReturnValue(info, strClass.wstr());
	}
}

// description: text property getter/setter
void JsWindow::get_text(V8_GETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaWindow *pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		auto strText = pWindow->GetText();
		V8Wrap::SetReturnValue(info, strText.wstr());
	}
}

void JsWindow::set_text(V8_SETTER_ARGS)
{
	UNUSED_PARAMETER(name);

	NaWindow *pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		String::Value str(value);
		pWindow->SetText((const wchar_t*)*str);
	}
}

// description: visible property getter/setter
void JsWindow::get_visible(Local<String> name, const PropertyCallbackInfo<Value>& info)
{
	UNUSED_PARAMETER(name);

	NaWindow *pWindow = UnwrapNativeWindow(info.This());
	bool bVisible = false;
	if (pWindow)
	{
		bVisible = pWindow->IsVisible();
	}

	V8Wrap::SetReturnValue(info, bVisible);
}

void JsWindow::set_visible(Local<String> name, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	UNUSED_PARAMETER(name);

	// get window object(this)
	NaWindow *pWindow = UnwrapNativeWindow(info.This());
	bool bVisible = value->BooleanValue();
	if (pWindow)
		pWindow->SetVisible(bVisible);
}

// description: topmost property getter/setter
void JsWindow::get_topmost(Local<String> name, const PropertyCallbackInfo<Value>& info)
{
	UNUSED_PARAMETER(name);

	NaWindow *pWindow = UnwrapNativeWindow(info.This());
	bool bTopmost = false;
	if (pWindow)
		bTopmost = pWindow->IsTopmost();

	V8Wrap::SetReturnValue(info, bTopmost);
}

void JsWindow::set_topmost(Local<String> name, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	UNUSED_PARAMETER(name);

	NaWindow *pWindow = UnwrapNativeWindow(info.This());
	bool bTopmost = value->BooleanValue();
	if (pWindow)
		pWindow->SetTopmost(bTopmost);
}

// description: handle property getter/setter
void JsWindow::get_handle(Local<String> name, const PropertyCallbackInfo<Value>& info)
{
	UNUSED_PARAMETER(name);

	NaWindow *pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		V8Wrap::SetReturnValue(info, (int)pWindow->m_hWnd);
	}
	else
	{
		V8Wrap::NullReturnValue(info);
	}
}

void JsWindow::set_handle(Local<String> name, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	UNUSED_PARAMETER(name);

	NaWindow *pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		pWindow->SetHandle((HWND)value->Int32Value());
	}
}

// description: state property getter/setter
void JsWindow::get_state(Local<String> name, const PropertyCallbackInfo<Value>& info)
{
	UNUSED_PARAMETER(name);

	NaWindow *pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		auto nState = pWindow->GetState();
		switch(nState)
		{
		case SW_MAXIMIZE:
			V8Wrap::SetReturnValue(info, "maximized");
			break;
		case SW_NORMAL:
			V8Wrap::SetReturnValue(info, "normal");
			break;
		case SW_MINIMIZE:
			V8Wrap::SetReturnValue(info, "minimized");
			break;
		}
	}
	else
	{
		V8Wrap::NullReturnValue(info);
	}
}

void JsWindow::set_state(Local<String> name, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	UNUSED_PARAMETER(name);

	NaWindow *pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		String::Value type(value);
		NaString strType((const wchar_t*)*type);
		UINT nCmd = 0;

		if (strType.CompareNoCase(L"normal") == 0)
			nCmd = SW_NORMAL;
		else if (strType.CompareNoCase(L"maximized") == 0)
			nCmd = SW_MAXIMIZE;
		else if (strType.CompareNoCase(L"minimized") == 0)
			nCmd = SW_MINIMIZE;

		if (nCmd != 0)
			pWindow->SetState(nCmd);
	}
}

// description: constructor function
// syntax:		new Window([x, y[, width, height]]) : windowObj
void JsWindow::method_constructor(V8_FUNCTION_ARGS)
{
	JsWindow *pJsWindow = new JsWindow();
	auto pWindow = pJsWindow->m_pNativeWindow = new NaWindow(0, NA_WINDOW_USERCREATED);

	Local<Object> obj = WrapObject(args.GetIsolate(), pJsWindow);

	if (args.Length() >= 2)
	{
		pWindow->m_x = args[0]->Int32Value();
		pWindow->m_y = args[1]->Int32Value();
	}

	if (args.Length() >= 4)
	{
		pWindow->m_width = args[2]->Int32Value();
		pWindow->m_height = args[3]->Int32Value();
	}

	// #TODO SetReturnValueAsObject
	args.GetReturnValue().Set(obj);
}

// description: create a new window handle
// syntax:		windowObj.create([x, y[, width, height]]);
void JsWindow::method_create(V8_FUNCTION_ARGS)
{
	NaWindow *pWindow = UnwrapNativeWindow(args.This());
	if (pWindow == nullptr)
		return;

	if (args.Length() >= 2)
	{
		pWindow->m_x = args[0]->Int32Value();
		pWindow->m_y = args[1]->Int32Value();
	}

	if (args.Length() >= 4)
	{
		pWindow->m_width = args[2]->Int32Value();
		pWindow->m_height = args[3]->Int32Value();
	}

	pWindow->Create();
}

// description: move window to x,y, width, height
// syntax:      windowObj.move(x, y, width, height)
// syntax:		windowObj.move(("left" | "center" | "right"), ("top" | "middle" | "bottom"))
void JsWindow::method_move(V8_FUNCTION_ARGS)
{
	if (args.Length() < 2)
	{
		// TODO error? exception?
		return;
	}

	NaWindow *pWindow = UnwrapNativeWindow(args.This());
	if (pWindow == nullptr)
		return;

	// move window
	int x = 0;
	int y = 0;
	int w, h;

	auto rc = pWindow->GetRect();
	w = rc.right - rc.left;
	h = rc.bottom - rc.top;

	if (args[0]->IsString() && args[1]->IsString())
	{
		String::Value halign(args[0]);
		String::Value valign(args[1]);
		NaString strHA(*String::Utf8Value(args[0]));
		NaString strVA(*String::Utf8Value(args[1]));

		// #TODO move to correct class
		int nScreenW = GetSystemMetrics(SM_CXSCREEN);
		int nScreenH = GetSystemMetrics(SM_CYSCREEN);

		// calc h-align
		if (strHA.Compare(L"left") == 0)
			x = 0;
		else if (strHA.Compare(L"center") == 0)
			x = nScreenW / 2 - w / 2;
		else if (strHA.Compare(L"right") == 0)
			x = nScreenW - w;

		// calc v-align
		if (strVA.Compare(L"top") == 0)
			y = 0;
		else if (strVA.Compare(L"middle") == 0)
			y = nScreenH / 2 - h / 2;
		else if (strVA.Compare(L"bottom") == 0)
			y = nScreenH - h;
	}
	else
	{
		x = static_cast<int>(args[0]->IntegerValue());
		y = static_cast<int>(args[1]->IntegerValue());
	}

	if (args.Length() >= 3)
	{
		w = static_cast<int>(args[2]->IntegerValue());
		h = static_cast<int>(args[3]->IntegerValue());
	}

	pWindow->Move(x, y, w, h, true);
}

// description: activate window
// syntax:      windowObj.activate()
void JsWindow::method_activate(V8_FUNCTION_ARGS)
{
	NaWindow *pWindow = UnwrapNativeWindow(args.This());
	if (pWindow == nullptr)
		return;

	pWindow->Activate();
}

// description: post close message to window
// syntax:      windowObj.close()
void JsWindow::method_close(V8_FUNCTION_ARGS)
{
	NaWindow *pWindow = UnwrapNativeWindow(args.This());
	if (pWindow == nullptr)
		return;

	pWindow->Close();
}

// description: same as alert but window is its parent
// syntax:      windowObj.alert(message[, title[, type]])
void JsWindow::method_alert(V8_FUNCTION_ARGS)
{
	HWND hParent = nullptr;
	NaWindow *pWindow = UnwrapNativeWindow(args.This());
	if (pWindow)
		hParent = pWindow->m_hWnd;

	String::Value msg(args[0]);
	String::Value title(args[1]);
	int nType = args[2]->Int32Value();

	// #TODO move to correct class
	int nRet = ::MessageBoxW(
		hParent,
		(const wchar_t*)*msg,
		args.Length() >= 2 ? (const wchar_t*)*title : L"Alert",
		args.Length() >= 3 ? nType : MB_OK
		);

	V8Wrap::SetReturnValue(args, nRet);
}

// description: add control object on window
// syntax:		windowObj.addControl(type, x, y, width, height[, text[, visible[, command_callback]]])
void JsWindow::method_addControl(V8_FUNCTION_ARGS)
{
	NaWindow *pWindow = UnwrapNativeWindow(args.This());
	NaControl *pControl = new NaControl;
	Local<Object> obj = WrapObject(args.GetIsolate(), pControl);

	if (args.Length() < 5)
	{
		// TODO error? exception?
		return;
	}

	pControl->Create(args, pWindow);

	args.GetReturnValue().Set(obj);
}
