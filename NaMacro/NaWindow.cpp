#include "NaWindow.h"

#include "Common.h"
#include "resource.h"

#include "BasicModule.h"

#include "NaControl.h"
#include "NaNotifyWindow.h"

bool NaWindow::s_bRegisterClass = false;
Global<ObjectTemplate> NaWindow::s_NaWindowTemplate;

NaWindow::NaWindow(HWND hWnd, NaWindowTypes enType)
{
	m_hWnd = hWnd;
	m_enType = enType;
	m_x = 0;
	m_y = 0;
	m_width = 0;
	m_height = 0;
	m_clientWidth = 0;
	m_clientHeight = 0;

	NaDebugOut(L"NaWindow(): 0x%08x, %d\n", this, enType);
}

NaWindow::~NaWindow()
{
	NaDebugOut(L"~NaWindow(): 0x%08x\n", this);
}

// description: native create method
HWND NaWindow::Create()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	if (!s_bRegisterClass)
	{
		WNDCLASS WndClass;
		WndClass.cbClsExtra = 0;
		WndClass.cbWndExtra = 0;
		WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		//WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		WndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
		WndClass.hInstance = hInstance;
		WndClass.lpfnWndProc = NaWindow::WndProc;
		WndClass.lpszClassName = NA_WINDOW_CLASS;
		WndClass.lpszMenuName = NULL;
		WndClass.style = CS_HREDRAW | CS_VREDRAW;
		RegisterClass(&WndClass);

		s_bRegisterClass = true;
	}

	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
	m_hWnd = ::CreateWindow(
		NA_WINDOW_CLASS,
		NULL, // _In_opt_ LPCTSTR   lpWindowName,
		dwStyle, // _In_     DWORD     dwStyle,
		m_x,
		m_y,
		m_width,
		m_height,
		NULL, // _In_opt_ HWND      hWndParent,
		NULL, // _In_opt_ HMENU     hMenu,
		hInstance,
		NULL // _In_opt_ LPVOID    lpParam
		);

	return m_hWnd;
}

void NaWindow::Destroy()
{
	if (m_enType == NA_WINDOW_CONSOLE)
		return;

	if (m_hWnd)
	{
		::DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}
}

LRESULT CALLBACK NaWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_COMMAND:
		{
			HWND hControlWnd = (HWND)lParam;
			int nCode = HIWORD(wParam);

			Isolate *isolate = Isolate::GetCurrent();
			NaControl::OnCommand(isolate, hControlWnd, nCode);
		}
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

// description: EnumWindows CallBack
BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	int nLen = GetWindowTextLength(hWnd);
	if (nLen == 0)
		return 1;

	nLen += sizeof(wchar_t);
	wchar_t *buf = new wchar_t[nLen];
	GetWindowText(hWnd, buf, nLen);
	wchar_t *buf2 = new wchar_t[1024];
	RealGetWindowClass(hWnd, buf2, 1024);
	NaDebugOut(L"EnumWindowsProc: %s / %s\n", buf, buf2);

	NaWindow::FindWindowsInfo *pInfo = (NaWindow::FindWindowsInfo*)lParam;

	std::wstring str(buf);
	int nIndex = str.find(pInfo->name);
	if (nIndex != -1)
	{
		pInfo->foundlist.push_back(hWnd);
	}

	delete buf;
	delete buf2;

	return 1;
}

Local<ObjectTemplate> NaWindow::MakeObjectTemplate(Isolate * isolate)
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
	ADD_WINDOW_METHOD(alert);
	ADD_WINDOW_METHOD(addControl);

	return handle_scope.Escape(templ);
}

void NaWindow::FindWindows(Isolate *isolate, const wchar_t * name, Local<Array> &results)
{
	// NOTE
	// ::FindWindow can find only exactly same named window can found.

	FindWindowsInfo info;
	info.name = const_cast<wchar_t*>(name);

	EnumWindows(EnumWindowsProc, (LPARAM)(&info));

	// Wrap HWND to V8Object
	int nIndex = 0;
	std::list<HWND>::iterator it;
	it = info.foundlist.begin();
	for (; it != info.foundlist.end(); ++it) {
		HWND hWnd = *it;
		NaWindow *pNaWindow = GetWindow(hWnd);
		Local<Value> obj = WrapObject(isolate, pNaWindow);

		// Fill V8Object Array
		results->Set(nIndex++, obj);
	}
}

NaWindow* NaWindow::GetWindow(int x, int y)
{
	POINT pt;
	pt.x = x;
	pt.y = y;

	HWND hWnd = WindowFromPoint(pt);

	return GetWindow(hWnd);
}

NaWindow * NaWindow::GetActiveWindow()
{
	HWND hWnd = ::GetForegroundWindow();

	return GetWindow(hWnd);
}

NaWindow* NaWindow::GetWindow(HWND hWnd)
{
	// find cached window
	/*
	int nSize = NaWindow::s_mapWindow.size();
	std::map<long, NaWindow*>::iterator it = NaWindow::s_mapWindow.begin();
	for (; it != NaWindow::s_mapWindow.end(); it++)
	{
		NaWindow *pWindow = it->second;
		if (pWindow->m_hWnd == hWnd)
		{
			Local<Object> obj = GetV8Window(isolate, pWindow);
			return obj;
		}
	}
	*/

	NaWindow *pNaWindow = (NaWindow*)GetWindowLong(hWnd, NA_WINDOW_MARK);
	if (pNaWindow)
		return pNaWindow;

	// not found
	return (new NaWindow(hWnd));
}

// description: x property getter/setter
void NaWindow::get_x(V8_GETTER_ARGS)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(info.This()));
	Isolate *isolate = info.GetIsolate();
	if (pWindow && pWindow->m_hWnd)
	{
		RECT rc;
		::GetWindowRect(pWindow->m_hWnd, &rc);
		pWindow->m_x = rc.left;
	}

	info.GetReturnValue().Set(
		Integer::New(isolate, pWindow->m_x)
		);
}

void NaWindow::set_x(V8_SETTER_ARGS)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(info.This()));
	if (pWindow)
	{
		RECT rc;
		::GetWindowRect(pWindow->m_hWnd, &rc);
		::MoveWindow(pWindow->m_hWnd, value->Int32Value(), rc.top, rc.right - rc.left, rc.bottom - rc.top, FALSE);
	}
}

// description: y property getter/setter
void NaWindow::get_y(V8_GETTER_ARGS)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(info.This()));
	Isolate *isolate = info.GetIsolate();
	if (pWindow && pWindow->m_hWnd)
	{
		RECT rc;
		::GetWindowRect(pWindow->m_hWnd, &rc);
		pWindow->m_y = rc.top;
	}

	info.GetReturnValue().Set(
		Integer::New(isolate, pWindow->m_y)
		);
}

void NaWindow::set_y(V8_SETTER_ARGS)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(info.This()));
	if (pWindow)
	{
		RECT rc;
		::GetWindowRect(pWindow->m_hWnd, &rc);
		::MoveWindow(pWindow->m_hWnd, rc.left, value->Int32Value(), rc.right - rc.left, rc.bottom - rc.top, FALSE);
	}
}

// description: width property getter/setter
void NaWindow::get_width(V8_GETTER_ARGS)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(info.This()));
	Isolate *isolate = info.GetIsolate();
	if (pWindow && pWindow->m_hWnd)
	{
		RECT rc;
		::GetWindowRect(pWindow->m_hWnd, &rc);
		pWindow->m_width = rc.right - rc.left;
	}

	info.GetReturnValue().Set(
		Integer::New(isolate, pWindow->m_width)
		);
}

void NaWindow::set_width(V8_SETTER_ARGS)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(info.This()));
	if (pWindow)
	{
		RECT rc;
		::GetWindowRect(pWindow->m_hWnd, &rc);
		::MoveWindow(pWindow->m_hWnd, rc.left, rc.top, value->Int32Value(), rc.bottom - rc.top, FALSE);
	}
}

// description: height property getter/setter
void NaWindow::get_height(V8_GETTER_ARGS)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(info.This()));
	Isolate *isolate = info.GetIsolate();
	if (pWindow && pWindow->m_hWnd)
	{
		RECT rc;
		::GetWindowRect(pWindow->m_hWnd, &rc);
		pWindow->m_height = rc.bottom - rc.top;
	}

	info.GetReturnValue().Set(
		Integer::New(isolate, pWindow->m_height)
		);
}

void NaWindow::set_height(V8_SETTER_ARGS)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(info.This()));
	if (pWindow)
	{
		RECT rc;
		::GetWindowRect(pWindow->m_hWnd, &rc);
		::MoveWindow(pWindow->m_hWnd, rc.left, rc.top, rc.right - rc.left, value->Int32Value(), FALSE);
	}
}

// description: client width property getter
void NaWindow::get_clientWidth(V8_GETTER_ARGS)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(info.This()));
	Isolate *isolate = info.GetIsolate();
	if (pWindow && pWindow->m_hWnd)
	{
		RECT rc;
		::GetClientRect(pWindow->m_hWnd, &rc);
		pWindow->m_clientWidth = rc.right - rc.left;
	}

	info.GetReturnValue().Set(
		Integer::New(isolate, pWindow->m_clientWidth)
	);
}

// description: client height property getter
void NaWindow::get_clientHeight(V8_GETTER_ARGS)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(info.This()));
	Isolate *isolate = info.GetIsolate();
	if (pWindow && pWindow->m_hWnd)
	{
		RECT rc;
		::GetClientRect(pWindow->m_hWnd, &rc);
		pWindow->m_clientHeight = rc.bottom - rc.top;
	}

	info.GetReturnValue().Set(
		Integer::New(isolate, pWindow->m_clientHeight)
	);
}

// description: class property getter/setter
void NaWindow::get_class(V8_GETTER_ARGS)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(info.This()));
	Isolate *isolate = info.GetIsolate();
	if (pWindow)
	{
		wchar_t str[1024];
		::RealGetWindowClass(pWindow->m_hWnd, str, 1024);

		info.GetReturnValue().Set(
			String::NewFromTwoByte(isolate, (const uint16_t*)str, NewStringType::kNormal).ToLocalChecked()
			);
	}
}

// description: text property getter/setter
void NaWindow::get_text(V8_GETTER_ARGS)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(info.This()));
	Isolate *isolate = info.GetIsolate();
	if (pWindow)
	{
		wchar_t str[1024];
		::GetWindowText(pWindow->m_hWnd, str, 1024);

		info.GetReturnValue().Set(
			String::NewFromTwoByte(isolate, (const uint16_t*)str, NewStringType::kNormal).ToLocalChecked()
			);
	}
}

void NaWindow::set_text(V8_SETTER_ARGS)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(info.This()));
	if (pWindow)
	{
		String::Value str(value);
		::SetWindowText(pWindow->m_hWnd, (const wchar_t*)*str);
	}
}

// description: visible property getter/setter
void NaWindow::get_visible(Local<String> name, const PropertyCallbackInfo<Value>& info)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(info.This()));
	Isolate *isolate = info.GetIsolate();

	bool bVisible = false;
	if (pWindow)
	{
		bVisible = ::IsWindowVisible(pWindow->m_hWnd);
	}

	info.GetReturnValue().Set(
		Boolean::New(isolate, bVisible)
		);
}

void NaWindow::set_visible(Local<String> name, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	// get window object(this)
	Isolate *isolate = info.GetIsolate();
	Local<Object> obj = info.This();
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(obj));

	bool bVisible = value->BooleanValue();
	switch (pWindow->m_enType)
	{
	case NA_WINDOW_CONSOLE:
		if (pWindow && pWindow->m_hWnd)
			::ShowWindow(pWindow->m_hWnd, bVisible ? SW_SHOW : SW_HIDE);
		else if (bVisible)
		{
			HWND hConsole = GetConsoleWindow();
			if (hConsole == NULL)
			{
				AllocConsole();
				freopen("CONOUT$", "wt", stdout);

				hConsole = GetConsoleWindow();

				// Console output problem fix (temp)
				_wsetlocale(0, L"korean"); // LC_ALL

				// Default position is right-bottom
				// Move console window to right-bottom of screen
				{
					RECT rcConsole, rcTray;
					GetWindowRect(hConsole, &rcConsole);
					GetWindowRect(FindWindow(L"Shell_TrayWnd", nullptr), &rcTray);

					MoveWindow(
						hConsole,
						rcTray.right - (rcConsole.right - rcConsole.left),
						rcTray.top - (rcConsole.bottom - rcConsole.top),
						(rcConsole.right - rcConsole.left),
						(rcConsole.bottom - rcConsole.top),
						TRUE);
				}

				// bind handle
				pWindow->m_hWnd = hConsole;
			}
		}
		break;
	default:
		if (pWindow && pWindow->m_hWnd)
			::ShowWindow(pWindow->m_hWnd, bVisible ? SW_SHOW : SW_HIDE);
		break;
	}
}

// description: topmost property getter/setter
void NaWindow::get_topmost(Local<String> name, const PropertyCallbackInfo<Value>& info)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(info.This()));
	Isolate *isolate = info.GetIsolate();

	bool bTopmost = false;
	if (pWindow)
	{
		DWORD dwExStyle = ::GetWindowLong(pWindow->m_hWnd, GWL_EXSTYLE);
		if ((dwExStyle & WS_EX_TOPMOST) != 0)
			bTopmost = true;
	}

	info.GetReturnValue().Set(
		Boolean::New(isolate, bTopmost)
	);
}

void NaWindow::set_topmost(Local<String> name, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	Isolate *isolate = info.GetIsolate();
	Local<Object> obj = info.This();
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(obj));

	bool bTopmost = value->BooleanValue();
	if (pWindow && pWindow->m_hWnd)
		::SetWindowPos(
			pWindow->m_hWnd,
			bTopmost ? HWND_TOPMOST : HWND_NOTOPMOST,
			0, 0, 0, 0,
			SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOREPOSITION | SWP_NOSIZE
		);
}

// description: handle property getter/setter
void NaWindow::get_handle(Local<String> name, const PropertyCallbackInfo<Value>& info)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(info.This()));
	if (pWindow)
	{
		Isolate *isolate = info.GetIsolate();
		info.GetReturnValue().Set(
			Integer::New(isolate, (int)pWindow->m_hWnd)
			);
	}
	else
	{
		info.GetReturnValue().SetNull();
	}
}

void NaWindow::set_handle(Local<String> name, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(info.This()));
	if (pWindow)
	{
		pWindow->m_hWnd = (HWND)value->Int32Value();
	}
}

// description: state property getter/setter
void NaWindow::get_state(Local<String> name, const PropertyCallbackInfo<Value>& info)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(info.This()));
	if (pWindow)
	{
		WINDOWPLACEMENT wndpl;
		GetWindowPlacement(pWindow->m_hWnd, &wndpl);

		Isolate *isolate = info.GetIsolate();
		switch(wndpl.showCmd)
		{
		case SW_MAXIMIZE:
			info.GetReturnValue().Set(
				String::NewFromUtf8(isolate, "maximized", NewStringType::kInternalized).ToLocalChecked()
			);
			break;
		case SW_NORMAL:
			info.GetReturnValue().Set(
				String::NewFromUtf8(isolate, "normal", NewStringType::kInternalized).ToLocalChecked()
			);
			break;
		case SW_MINIMIZE:
			info.GetReturnValue().Set(
				String::NewFromUtf8(isolate, "minimized", NewStringType::kInternalized).ToLocalChecked()
			);
			break;
		}
	}
	else
	{
		info.GetReturnValue().SetNull();
	}
}

void NaWindow::set_state(Local<String> name, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(info.This()));
	if (pWindow)
	{
		String::Value type(value);
		NaString strType((const wchar_t*)*type);
		UINT nCmd = 0;

		if (strType.CompareNoCase(L"normal") == 0)
		{
			nCmd = SW_NORMAL;
		}
		else if (strType.CompareNoCase(L"maximized") == 0)
		{
			nCmd = SW_MAXIMIZE;
		}
		else if (strType.CompareNoCase(L"minimized") == 0)
		{
			nCmd = SW_MINIMIZE;
		}

		if (nCmd != 0)
		{
			ShowWindow(pWindow->m_hWnd, nCmd);
		}
	}
}

// description: constructor function
// syntax:		new Window([x, y[, width, height]]) : windowObj
void NaWindow::method_constructor(V8_FUNCTION_ARGS)
{
	NaWindow *pWindow = new NaWindow(0, NA_WINDOW_USERCREATED);
	Local<Object> obj = WrapObject(args.GetIsolate(), pWindow);

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

	args.GetReturnValue().Set(obj);
}

// description: create a new window handle
// syntax:		windowObj.create([x, y[, width, height]]);
void NaWindow::method_create(V8_FUNCTION_ARGS)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(args.This()));
	if (pWindow == NULL)
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
void NaWindow::method_move(V8_FUNCTION_ARGS)
{
	if (args.Length() < 2)
	{
		// TODO error? exception?
		return;
	}

	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(args.This()));
	if (pWindow == NULL)
		return;

	// move window
	int x, y, w, h;
	RECT rc;
	::GetWindowRect(pWindow->m_hWnd, &rc);
	w = rc.right - rc.left;
	h = rc.bottom - rc.top;

	if (args[0]->IsString() && args[1]->IsString())
	{
		String::Value halign(args[0]);
		String::Value valign(args[1]);
		NaString strHA(halign);
		NaString strVA(valign);

		int nScreenW = GetSystemMetrics(SM_CXSCREEN);
		int nScreenH = GetSystemMetrics(SM_CYSCREEN);

		// calc h-align
		if (strHA.Compare(L"left") == 0)
		{
			x = 0;
		}
		else if (strHA.Compare(L"center") == 0)
		{
			x = nScreenW / 2 - w / 2;
		}
		else if (strHA.Compare(L"right") == 0)
		{
			x = nScreenW - w;
		}

		// calc v-align
		if (strVA.Compare(L"top") == 0)
		{
			y = 0;
		}
		else if (strVA.Compare(L"middle") == 0)
		{
			y = nScreenH / 2 - h / 2;
		}
		else if (strVA.Compare(L"bottom") == 0)
		{
			y = nScreenH - h;
		}
	}
	else
	{
		x = args[0]->IntegerValue();
		y = args[1]->IntegerValue();
	}

	if (args.Length() >= 3)
	{
		w = args[2]->IntegerValue();
		h = args[3]->IntegerValue();
	}
	::MoveWindow(pWindow->m_hWnd, x, y, w, h, TRUE);
}

// description: activate window
// syntax:      windowObj.activate()
void NaWindow::method_activate(V8_FUNCTION_ARGS)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(args.This()));
	if (pWindow == NULL)
		return;

	::SetForegroundWindow(pWindow->m_hWnd);
}

// description: same as alert but window is its parent
// syntax:      windowObj.alert(message[, title[, type]])
void NaWindow::method_alert(V8_FUNCTION_ARGS)
{
	HWND hParent = NULL;
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(args.This()));
	if (pWindow)
		hParent = pWindow->m_hWnd;

	String::Value msg(args[0]);
	String::Value title(args[1]);
	int nType = args[2]->Int32Value();

	int nRet = ::MessageBoxW(
		hParent,
		(const wchar_t*)*msg,
		args.Length() >= 2 ? (const wchar_t*)*title : L"Alert",
		args.Length() >= 3 ? nType : MB_OK
		);

	Isolate *isolate = args.GetIsolate();
	args.GetReturnValue().Set(
		Integer::New(isolate, nRet)
		);
}

// description: add control object on window
// syntax:		windowObj.addControl(type, x, y, width, height[, text[, visible[, command_callback]]])
void NaWindow::method_addControl(V8_FUNCTION_ARGS)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(args.This()));
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
