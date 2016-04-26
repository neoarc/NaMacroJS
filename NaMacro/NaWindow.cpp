#include "NaWindow.h"

#include "Common.h"

//long NaWindow::s_lUniqueID = 0;
bool NaWindow::s_bRegisterClass = false;
//std::map<long, NaWindow*> NaWindow::s_mapWindow;

Global<ObjectTemplate> NaWindow::s_NaWindowTemplate;

NaWindow::NaWindow(HWND hWnd, NaWindowTypes enType)
{
	m_hWnd = hWnd;
	m_enType = enType;

	NaDebugOut(L"NaWindow(): 0x%08x, %d\n", this, enType);
}

NaWindow::~NaWindow()
{
	NaDebugOut(L"~NaWindow(): 0x%08x\n", this);
}

HWND NaWindow::Create()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	if (s_bRegisterClass)
	{
		WNDCLASS WndClass;
		WndClass.cbClsExtra = 0;
		WndClass.cbWndExtra = 0;
		WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		WndClass.hInstance = hInstance;
		WndClass.lpfnWndProc = NaWindow::WndProc;
		WndClass.lpszClassName = NA_WINDOW_CLASS;
		WndClass.lpszMenuName = NULL;
		WndClass.style = CS_HREDRAW | CS_VREDRAW;
		RegisterClass(&WndClass);
		
		s_bRegisterClass = true;
	}

	DWORD dwStyle = 0;
	int x = 0, y = 0, nWidth = 600, nHeight = 400;

	m_hWnd = ::CreateWindow(
		NA_WINDOW_CLASS,
		NULL, // _In_opt_ LPCTSTR   lpWindowName,
		dwStyle, // _In_     DWORD     dwStyle,
		x,
		y,
		nWidth,
		nHeight,
		NULL, // _In_opt_ HWND      hWndParent,
		NULL, // _In_opt_ HMENU     hMenu,
		hInstance,
		NULL // _In_opt_ LPVOID    lpParam
		);

	return m_hWnd;
}

LRESULT CALLBACK NaWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;
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

	NaWindow::FindWindowsInfo *pInfo = (NaWindow::FindWindowsInfo*)lParam;

	std::wstring str(buf);
	int nIndex = str.find(pInfo->name);
	if (nIndex != -1)
	{
		pInfo->foundlist.push_back(hWnd);
	}

	delete buf;
	return 1;
}

Local<ObjectTemplate> NaWindow::MakeObjectTemplate(Isolate * isolate)
{
	EscapableHandleScope handle_scope(isolate);

	Local<ObjectTemplate> templ = ObjectTemplate::New(isolate);
	templ->SetInternalFieldCount(1);

	//templ->

	// bind window methods
#define ADD_WINDOW_ACCESSOR(_prop, _getter, _setter)	ADD_OBJ_ACCESSOR(templ, _prop, _getter, _setter);
#define ADD_WINDOW_METHOD(_js_func, _c_func)			ADD_TEMPLATE_METHOD(templ, _js_func, _c_func);

	// accessor
	ADD_WINDOW_ACCESSOR(x, GetX, SetX);
	ADD_WINDOW_ACCESSOR(y, GetY, SetY);
	ADD_WINDOW_ACCESSOR(width, GetWidth, SetWidth);
	ADD_WINDOW_ACCESSOR(height, GetHeight, SetHeight);
	ADD_WINDOW_ACCESSOR(text, GetText, SetText);
	ADD_WINDOW_ACCESSOR(visible, GetVisible, SetVisible);
	ADD_WINDOW_ACCESSOR(handle, GetHandle, SetHandle);

	// methods
	ADD_WINDOW_METHOD(move, Move);
	ADD_WINDOW_METHOD(activate, Activate);

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
		
		NaWindow *pNaWindow = new NaWindow;
		pNaWindow->m_hWnd = hWnd;

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
void NaWindow::GetX(V8_GETTER_ARGS)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(info.This())); 
	Isolate *isolate = info.GetIsolate();
	if (pWindow)
	{
		RECT rc;
		::GetWindowRect(pWindow->m_hWnd, &rc);

		info.GetReturnValue().Set(
			Integer::New(isolate, rc.left)
			);
	}
}

void NaWindow::SetX(V8_SETTER_ARGS)
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
void NaWindow::GetY(V8_GETTER_ARGS)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(info.This()));
	Isolate *isolate = info.GetIsolate();
	if (pWindow)
	{
		RECT rc;
		::GetWindowRect(pWindow->m_hWnd, &rc);

		info.GetReturnValue().Set(
			Integer::New(isolate, rc.top)
			);
	}
}

void NaWindow::SetY(V8_SETTER_ARGS)
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
void NaWindow::GetWidth(V8_GETTER_ARGS)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(info.This()));
	Isolate *isolate = info.GetIsolate();
	if (pWindow)
	{
		RECT rc;
		::GetWindowRect(pWindow->m_hWnd, &rc);

		info.GetReturnValue().Set(
			Integer::New(isolate, rc.right - rc.left)
			);
	}
}

void NaWindow::SetWidth(V8_SETTER_ARGS)
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
void NaWindow::GetHeight(V8_GETTER_ARGS)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(info.This())); 
	Isolate *isolate = info.GetIsolate();
	if (pWindow)
	{
		RECT rc;
		::GetWindowRect(pWindow->m_hWnd, &rc);

		info.GetReturnValue().Set(
			Integer::New(isolate, rc.bottom - rc.top)
			);
	}
}

void NaWindow::SetHeight(V8_SETTER_ARGS)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(info.This()));
	if (pWindow)
	{
		RECT rc;
		::GetWindowRect(pWindow->m_hWnd, &rc);
		::MoveWindow(pWindow->m_hWnd, rc.left, rc.top, rc.right - rc.left, value->Int32Value(), FALSE);
	}
}

// description: text property getter/setter
void NaWindow::GetText(V8_GETTER_ARGS)
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

void NaWindow::SetText(V8_SETTER_ARGS)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(info.This()));
	if (pWindow)
	{
		String::Value str(value);
		::SetWindowText(pWindow->m_hWnd, (const wchar_t*)*str);
	}
}

// description: visible property getter/setter
void NaWindow::GetVisible(Local<String> name, const PropertyCallbackInfo<Value>& info)
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

void NaWindow::SetVisible(Local<String> name, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	// get window object(this)
	Isolate *isolate = info.GetIsolate();
	Local<Object> obj = info.This();
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(obj));
	
	bool bVisible = value->BooleanValue();

	// get window type
	Local<Value> type_value = obj->Get(String::NewFromUtf8(isolate, "_type", NewStringType::kNormal).ToLocalChecked());
	int nType = type_value->IntegerValue();
	switch (nType)
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

// description: handle property getter/setter
void NaWindow::GetHandle(Local<String> name, const PropertyCallbackInfo<Value>& info)
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

void NaWindow::SetHandle(Local<String> name, Local<Value> value, const PropertyCallbackInfo<void>& info)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(info.This()));
	if (pWindow)
	{
		pWindow->m_hWnd = (HWND)value->Int32Value();
	}
}

// description: move window to x,y, width, height
// syntax:      windowObj.move(x, y, width, height)
void NaWindow::Move(V8_FUNCTION_ARGS)
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
	x = args[0]->IntegerValue();
	y = args[1]->IntegerValue();
	if (args.Length() < 3)
	{
		RECT rc;
		::GetWindowRect(pWindow->m_hWnd, &rc);
		w = rc.right - rc.left;
		h = rc.bottom - rc.top;
	}
	else
	{
		w = args[2]->IntegerValue();
		h = args[3]->IntegerValue();
	}
	::MoveWindow(pWindow->m_hWnd, x, y, w, h, TRUE);
}

// description: activate window
// syntax:      windowObj.activate()
void NaWindow::Activate(V8_FUNCTION_ARGS)
{
	NaWindow *pWindow = reinterpret_cast<NaWindow*>(UnwrapObject(args.This()));
	if (pWindow == NULL)
		return;

	::SetForegroundWindow(pWindow->m_hWnd);
}
