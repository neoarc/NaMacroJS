#include "stdafx.h"
#include "JsWindow.h"

#include <algorithm>

#include <NaLib/NaDebug.h>
#include <NaLib/NaNotifyWindow.h>

#include "JsGlobalCommon.h"
#include "NaControl.h"
#include "NaWindow.h"

#include "JsControl.h"

#include "V8Wrap.h"
#include "resource.h"
#include "NaScreen.h"

Global<ObjectTemplate> JsWindow::s_JsWindowTemplate;

JsWindow::JsWindow()
{
	m_pNativeWindow = nullptr;

	//NaDebugOut(L"JsWindow(): 0x%08x, %d\n", this, enType);
}

JsWindow::~JsWindow()
{
	// #TODO delete m_pNativeWindow

	//NaDebugOut(L"~JsWindow(): 0x%08x\n", this);
}

NaWindow * JsWindow::UnwrapNativeWindow(Local<Object> obj)
{
	auto pJsWindow = static_cast<JsWindow*>(UnwrapObject(obj));
	if (pJsWindow && pJsWindow->m_pNativeWindow)
	{
		return pJsWindow->m_pNativeWindow;
	}

	return nullptr;
}

LRESULT JsWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		{
			HWND hControlWnd = (HWND)lParam;
			int nCode = HIWORD(wParam);

			auto isolate = Isolate::GetCurrent();
			JsControl::OnCommand(isolate, hControlWnd, nCode);
		}
		break;
	}

	return NaWindow::WndProc(hWnd, message, wParam, lParam);
}

void JsWindow::FindWindows(Isolate* isolate, const wchar_t* name, Local<Array>& results)
{
	NaWindow::FindWindowsInfo info;
	info.name = const_cast<wchar_t*>(name);

	NaWindow::FindWindows(name, info);

	// Wrap HWND to V8Object
	int nIndex = 0;
	for_each(info.foundlist.begin(), info.foundlist.end(),
		[&](HWND h)
		{
			auto pJsWindow = new JsWindow();
			pJsWindow->m_pNativeWindow = NaWindow::GetWindow(h);

			auto obj = JsWindow::WrapObject(isolate, pJsWindow);

			// Fill V8Object Array
			results->Set(nIndex++, obj);
		}
	);
}

Local<ObjectTemplate> JsWindow::MakeObjectTemplate(Isolate * isolate)
{
	EscapableHandleScope handle_scope(isolate);

	Local<ObjectTemplate> templ = ObjectTemplate::New(isolate);
	templ->SetInternalFieldCount(1);

	// bind window methods
#define ADD_WINDOW_PROPERTY(_prop)		ADD_OBJ_PROPERTY(templ, _prop);
#define ADD_WINDOW_PROPERTY_RO(_prop)	ADD_OBJ_PROPERTY_RO(templ, _prop);
#define ADD_WINDOW_METHOD(_js_func)		ADD_TEMPLATE_METHOD(templ, _js_func);

	// accessor
	ADD_WINDOW_PROPERTY(x);
	ADD_WINDOW_PROPERTY(y);
	ADD_WINDOW_PROPERTY(width);
	ADD_WINDOW_PROPERTY(height);
	ADD_WINDOW_PROPERTY_RO(clientWidth);
	ADD_WINDOW_PROPERTY_RO(clientHeight);
	ADD_WINDOW_PROPERTY_RO(class);
	ADD_WINDOW_PROPERTY(text);
	ADD_WINDOW_PROPERTY(visible);
	ADD_WINDOW_PROPERTY(topmost);
	ADD_WINDOW_PROPERTY(handle);
	ADD_WINDOW_PROPERTY(state);

	// methods
	ADD_WINDOW_METHOD(create);
	ADD_WINDOW_METHOD(move);
	ADD_WINDOW_METHOD(activate);
	ADD_WINDOW_METHOD(close);
	ADD_WINDOW_METHOD(alert);
	ADD_WINDOW_METHOD(addControl);

	return handle_scope.Escape(templ);
}

void JsWindow::get_x(V8_GETTER_ARGS)
{
	auto pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		auto rc = pWindow->GetRect();
		pWindow->m_x = rc.left;
	
		V8_PROP_RET(pWindow->m_x);
	}
}

void JsWindow::set_x(V8_SETTER_ARGS)
{
	auto pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		auto rc = pWindow->GetRect();
		pWindow->Move(value->Int32Value(), rc.top, rc.right - rc.left, rc.bottom - rc.top);
	}
}

void JsWindow::get_y(V8_GETTER_ARGS)
{
	auto pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		auto rc = pWindow->GetRect();
		pWindow->m_y = rc.top;
	}

	V8_PROP_RET(pWindow->m_y);
}

void JsWindow::set_y(V8_SETTER_ARGS)
{
	auto pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		auto rc = pWindow->GetRect();
		pWindow->Move(rc.left, value->Int32Value(), rc.right - rc.left, rc.bottom - rc.top);
	}
}

void JsWindow::get_width(V8_GETTER_ARGS)
{
	auto pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		auto rc = pWindow->GetRect();
		pWindow->m_width = rc.right - rc.left;
	}

	V8_PROP_RET(pWindow->m_width);
}

void JsWindow::set_width(V8_SETTER_ARGS)
{
	auto pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		auto rc = pWindow->GetRect();
		pWindow->Move(rc.left, rc.top, value->Int32Value(), rc.bottom - rc.top);
	}
}

void JsWindow::get_height(V8_GETTER_ARGS)
{
	auto pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		auto rc = pWindow->GetRect();
		pWindow->m_height = rc.bottom - rc.top;
	}

	V8_PROP_RET(pWindow->m_height);
}

void JsWindow::set_height(V8_SETTER_ARGS)
{
	auto pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		auto rc = pWindow->GetRect();
		pWindow->Move(rc.left, rc.top, rc.right - rc.left, value->Int32Value());
	}
}

void JsWindow::get_clientWidth(V8_GETTER_ARGS)
{
	auto pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		auto rc = pWindow->GetClientRect();
		pWindow->m_clientWidth = rc.right - rc.left;
	}

	V8_PROP_RET(pWindow->m_clientWidth);
}

void JsWindow::get_clientHeight(V8_GETTER_ARGS)
{
	auto pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		auto rc = pWindow->GetClientRect();
		pWindow->m_clientHeight = rc.bottom - rc.top;
	}

	V8_PROP_RET(pWindow->m_clientHeight);
}

void JsWindow::get_class(V8_GETTER_ARGS)
{
	auto pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		auto strClass = pWindow->GetClass();
		V8_PROP_RET(strClass.wstr());
	}
}

void JsWindow::get_text(V8_GETTER_ARGS)
{
	auto pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		auto strText = pWindow->GetText();
		V8_PROP_RET(strText.wstr());
	}
}

void JsWindow::set_text(V8_SETTER_ARGS)
{
	auto pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		String::Value str(value);
		pWindow->SetText((const wchar_t*)*str);
	}
}

void JsWindow::get_visible(Local<String> name, const PropertyCallbackInfo<Value>& info)
{
	auto pWindow = UnwrapNativeWindow(info.This());
	bool bVisible = false;
	if (pWindow)
	{
		bVisible = pWindow->IsVisible();
	}

	V8_PROP_RET(bVisible);
}

void JsWindow::set_visible(Local<String> name, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	// get window object(this)
	auto pWindow = UnwrapNativeWindow(info.This());
	bool bVisible = value->BooleanValue();
	if (pWindow)
		pWindow->SetVisible(bVisible);
}

void JsWindow::get_topmost(Local<String> name, const PropertyCallbackInfo<Value>& info)
{
	auto pWindow = UnwrapNativeWindow(info.This());
	bool bTopmost = false;
	if (pWindow)
		bTopmost = pWindow->IsTopmost();

	V8_PROP_RET(bTopmost);
}

void JsWindow::set_topmost(Local<String> name, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	auto pWindow = UnwrapNativeWindow(info.This());
	bool bTopmost = value->BooleanValue();
	if (pWindow)
		pWindow->SetTopmost(bTopmost);
}

void JsWindow::get_handle(Local<String> name, const PropertyCallbackInfo<Value>& info)
{
	auto pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		V8_PROP_RET((int)pWindow->m_hWnd);
	}
	else
	{
		V8Wrap::NullReturnValue(info);
	}
}

void JsWindow::set_handle(Local<String> name, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	auto pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		pWindow->SetHandle((HWND)value->Int32Value());
	}
}

void JsWindow::get_state(Local<String> name, const PropertyCallbackInfo<Value>& info)
{
	auto pWindow = UnwrapNativeWindow(info.This());
	if (pWindow)
	{
		auto nState = pWindow->GetState();
		switch(nState)
		{
		case SW_MAXIMIZE:	V8_PROP_RET("maximized"); break;
		case SW_NORMAL:		V8_PROP_RET("normal"); break;
		case SW_MINIMIZE:	V8_PROP_RET("minimized"); break;
		}
	}
	else
	{
		V8Wrap::NullReturnValue(info);
	}
}

void JsWindow::set_state(Local<String> name, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	auto pWindow = UnwrapNativeWindow(info.This());
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

// Constructor function
// new Window([x, y[, width, height]]) : windowObj
void JsWindow::method_constructor(V8_METHOD_ARGS)
{
	auto pJsWindow = new JsWindow();
	auto pWindow = pJsWindow->m_pNativeWindow = new NaWindow(0, NA_WINDOW_USERCREATED);
	auto obj = WrapObject(args.GetIsolate(), pJsWindow);

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
	V8_METHOD_RET(obj);
}

// Create a new window handle
// windowObj.create([x, y[, width, height]]);
void JsWindow::method_create(V8_METHOD_ARGS)
{
	auto pWindow = UnwrapNativeWindow(args.This());
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

	pWindow->Create(JsWindow::WndProc);
}

// Move window to x,y, width, height
// windowObj.move(x, y, width, height)
// windowObj.move(("left" | "center" | "right"), ("top" | "middle" | "bottom"))
void JsWindow::method_move(V8_METHOD_ARGS)
{
	if (args.Length() < 2)
	{
		// TODO error? exception?
		return;
	}

	auto pWindow = UnwrapNativeWindow(args.This());
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
		auto nScreenW = NaScreen::GetWidth();
		auto nScreenH = NaScreen::GetHeight(); 

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

// Activate window
// windowObj.activate()
void JsWindow::method_activate(V8_METHOD_ARGS)
{
	auto pWindow = UnwrapNativeWindow(args.This());
	if (pWindow == nullptr)
		return;

	pWindow->Activate();
}

// Post close message to window
// windowObj.close()
void JsWindow::method_close(V8_METHOD_ARGS)
{
	auto pWindow = UnwrapNativeWindow(args.This());
	if (pWindow == nullptr)
		return;

	pWindow->Close();
}

// Same as alert but window is its parent
// windowObj.alert(message[, title[, type]])
void JsWindow::method_alert(V8_METHOD_ARGS)
{
	HWND hParent = nullptr;
	auto pWindow = UnwrapNativeWindow(args.This());
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

	V8_METHOD_RET(nRet);
}

// Add control object on window
// windowObj.addControl(type, x, y, width, height[, text[, visible[, command_callback]]])
void JsWindow::method_addControl(V8_METHOD_ARGS)
{
	auto pWindow = UnwrapNativeWindow(args.This());

	JsControl *pJsControl = new JsControl;
	pJsControl->m_pNativeControl = new NaControl;

	Local<Object> obj = WrapObject(args.GetIsolate(), pJsControl);

	if (args.Length() < 5)
	{
		// TODO error? exception?
		return;
	}

	pJsControl->Create(args, pWindow);

	V8_METHOD_RET(obj);
}
