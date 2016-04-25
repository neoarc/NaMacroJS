#include "NaWindow.h"

#include "Common.h"

long NaWindow::s_lUniqueID = 0;
bool NaWindow::s_bRegisterClass = false;
std::map<long, NaWindow*> NaWindow::s_mapWindow;

NaWindow::NaWindow(long lUID, NaWindowTypes enType)
{
	m_hWnd = NULL;
	m_enType = enType;

	s_mapWindow.insert(std::pair<long, NaWindow*>(lUID, this));
	NaDebugOut(L"NaWindow(): 0x%08x\n", this);
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

long NaWindow::CreateUniqueID()
{
	return s_lUniqueID++;
}

LRESULT CALLBACK NaWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

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

void NaWindow::DestroyMap()
{
	int nSize = NaWindow::s_mapWindow.size();
	std::map<long, NaWindow*>::iterator it = NaWindow::s_mapWindow.begin();
	for (; it != NaWindow::s_mapWindow.end(); it++)
	{
		NaWindow *pWindow = it->second;
		delete pWindow;
	}

	NaWindow::s_mapWindow.clear();
}

v8::Local<v8::Object> NaWindow::CreateV8Window(v8::Isolate *isolate)
{
	v8::Local<v8::Object> obj = v8::Object::New(isolate);

	// TODO bind api to prototype (not object)

	// bind window methods
#define ADD_WINDOW_ACCESSOR(_prop, _getter, _setter)	ADD_OBJ_ACCESSOR(obj, _prop, _getter, _setter);
#define ADD_WINDOW_METHOD(_js_func, _c_func)			ADD_OBJ_METHOD(obj, _js_func, _c_func);

	// accessor
	ADD_WINDOW_ACCESSOR(x, GetX, SetX);
	ADD_WINDOW_ACCESSOR(y, GetY, SetY);
	ADD_WINDOW_ACCESSOR(width, GetWidth, SetWidth);
	ADD_WINDOW_ACCESSOR(height, GetHeight, SetHeight);
	ADD_WINDOW_ACCESSOR(text, GetText, SetText);
	ADD_WINDOW_ACCESSOR(visible, GetVisible, SetVisible);
	ADD_WINDOW_ACCESSOR(handle, GetHandle, SetHandle);

	// methods
	ADD_WINDOW_METHOD(move,		Move);
	ADD_WINDOW_METHOD(activate, Activate);
	//ADD_WINDOW_METHOD(setVisible, WindowSetVisible);

	// register to map
	long lUID = NaWindow::CreateUniqueID();
	obj->Set(
		v8::String::NewFromUtf8(isolate, "_unique_id", v8::NewStringType::kNormal).ToLocalChecked(),
		v8::Number::New(isolate, lUID)
		);

	NaWindow *pWindow = new NaWindow(lUID);

	return obj;
}

void NaWindow::FindWindows(v8::Isolate *isolate, const wchar_t * name, v8::Local<v8::Array> &results)
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
		v8::Local<v8::Value> obj = GetV8Window(isolate, hWnd);

		// Fill V8Object Array
		results->Set(nIndex++, obj);
	}	
}

v8::Local<v8::Object> NaWindow::GetV8Window(v8::Isolate * isolate, int x, int y)
{
	POINT pt;
	pt.x = x;
	pt.y = y;

	HWND hWnd = WindowFromPoint(pt);
	return GetV8Window(isolate, hWnd);
}

v8::Local<v8::Object> NaWindow::GetV8Window(v8::Isolate *isolate, HWND hWnd)
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
			v8::Local<v8::Object> obj = GetV8Window(isolate, pWindow);
			return obj;
		}
	}
	*/

	// not found
	v8::Local<v8::Object> obj = CreateV8Window(isolate);	
	NaWindow::SetHandle(isolate, obj, hWnd);

	return obj;
}

NaWindow* NaWindow::GetNaWindow(v8::Isolate * isolate, v8::Local<v8::Object> &obj)
{
	// TODO null check

	// get window handle
	NaWindow *pWindow = NULL;

	v8::Local<v8::Value> value = obj->Get(v8::String::NewFromUtf8(isolate, "_unique_id", v8::NewStringType::kNormal).ToLocalChecked());
	long lUID = value->Int32Value();
	std::map<long, NaWindow*>::iterator it = s_mapWindow.find(lUID);
	if (it != s_mapWindow.end())
	{
		pWindow = it->second;
	}
	
	// TODO validation?

	return pWindow;
}

/*
v8::Local<v8::Object> NaWindow::GetV8Window(v8::Isolate * isolate, NaWindow * pWindow)
{
	int nSize = NaWindow::s_mapWindow.size();
	std::map<long, NaWindow*>::iterator it = NaWindow::s_mapWindow.begin();
	for (; it != NaWindow::s_mapWindow.end(); it++)
	{
		NaWindow *pWindow = it->second;
		if (pWindow->m_hWnd == hWnd)
		{
			v8::Local<v8::Object> obj = GetV8Window(isolate, pWindow);
			return obj;
		}
	}

	return v8::Local<v8::Object>();
}
*/

// description: native GetHandle method
HWND NaWindow::GetHandle(v8::Isolate * isolate, v8::Local<v8::Object> obj)
{
	NaWindow *pWindow = NaWindow::GetNaWindow(isolate, obj);
	if (pWindow)
	{
		return pWindow->m_hWnd;
	}
	
	return nullptr;
}

// description: native SetHandle method
void NaWindow::SetHandle(v8::Isolate *isolate, v8::Local<v8::Object> obj, HWND hWnd)
{
	NaWindow *pWindow = NaWindow::GetNaWindow(isolate, obj);
	if (pWindow)
	{
		if (pWindow->m_hWnd == nullptr)
		{
			SetWindowLong(hWnd, 160422, (LONG)pWindow);
		}

		pWindow->m_hWnd = hWnd;
	}
}

// description: x property getter/setter
void NaWindow::GetX(V8_GETTER_ARGS)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Object> obj = info.This();
	NaWindow *pWindow = NaWindow::GetNaWindow(isolate, obj);
	if (pWindow)
	{
		RECT rc;
		::GetWindowRect(pWindow->m_hWnd, &rc);

		info.GetReturnValue().Set(
			v8::Integer::New(isolate, rc.left)
			);
	}
}

void NaWindow::SetX(V8_SETTER_ARGS)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Object> obj = info.This();
	NaWindow *pWindow = NaWindow::GetNaWindow(isolate, obj);
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
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Object> obj = info.This();
	NaWindow *pWindow = NaWindow::GetNaWindow(isolate, obj);
	if (pWindow)
	{
		RECT rc;
		::GetWindowRect(pWindow->m_hWnd, &rc);

		info.GetReturnValue().Set(
			v8::Integer::New(isolate, rc.top)
			);
	}
}

void NaWindow::SetY(V8_SETTER_ARGS)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Object> obj = info.This();
	NaWindow *pWindow = NaWindow::GetNaWindow(isolate, obj);
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
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Object> obj = info.This();
	NaWindow *pWindow = NaWindow::GetNaWindow(isolate, obj);
	if (pWindow)
	{
		RECT rc;
		::GetWindowRect(pWindow->m_hWnd, &rc);

		info.GetReturnValue().Set(
			v8::Integer::New(isolate, rc.right - rc.left)
			);
	}
}

void NaWindow::SetWidth(V8_SETTER_ARGS)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Object> obj = info.This();
	NaWindow *pWindow = NaWindow::GetNaWindow(isolate, obj);
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
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Object> obj = info.This();
	NaWindow *pWindow = NaWindow::GetNaWindow(isolate, obj);
	if (pWindow)
	{
		RECT rc;
		::GetWindowRect(pWindow->m_hWnd, &rc);

		info.GetReturnValue().Set(
			v8::Integer::New(isolate, rc.bottom - rc.top)
			);
	}
}

void NaWindow::SetHeight(V8_SETTER_ARGS)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Object> obj = info.This();
	NaWindow *pWindow = NaWindow::GetNaWindow(isolate, obj);
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
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Object> obj = info.This();
	NaWindow *pWindow = NaWindow::GetNaWindow(isolate, obj);
	if (pWindow)
	{
		wchar_t str[1024];
		::GetWindowText(pWindow->m_hWnd, str, 1024);
		
		info.GetReturnValue().Set(
			v8::String::NewFromTwoByte(isolate, (const uint16_t*)str, v8::NewStringType::kNormal).ToLocalChecked()
			);
	}
}

void NaWindow::SetText(V8_SETTER_ARGS)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Object> obj = info.This();
	NaWindow *pWindow = NaWindow::GetNaWindow(isolate, obj);
	if (pWindow)
	{
		v8::String::Value str(value);
		::SetWindowText(pWindow->m_hWnd, (const wchar_t*)*str);
	}
}

// description: visible property getter/setter
void NaWindow::GetVisible(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	// get window object(this)
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Object> obj = info.This();
	NaWindow *pWindow = NaWindow::GetNaWindow(isolate, obj);
	bool bVisible = false;
	if (pWindow)
	{
		bVisible = ::IsWindowVisible(pWindow->m_hWnd);
	}

	info.GetReturnValue().Set(
		v8::Boolean::New(isolate, bVisible)
		);
}

void NaWindow::SetVisible(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	// get window object(this)
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Object> obj = info.This();
	NaWindow *pWindow = NaWindow::GetNaWindow(isolate, obj);
	
	bool bVisible = value->BooleanValue();

	// get window type
	v8::Local<v8::Value> type_value = obj->Get(v8::String::NewFromUtf8(isolate, "_type", v8::NewStringType::kNormal).ToLocalChecked());
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
				SetHandle(isolate, obj, hConsole);
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
void NaWindow::GetHandle(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	// get window object(this)
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Object> obj = info.This();
	NaWindow *pWindow = NaWindow::GetNaWindow(isolate, obj);
	if (pWindow)
	{
		info.GetReturnValue().Set(
			v8::Integer::New(isolate, (int)pWindow->m_hWnd)
			);
	}
	else
	{
		info.GetReturnValue().SetNull();
	}
}

void NaWindow::SetHandle(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	// get window object(this)
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Object> obj = info.This();
	NaWindow *pWindow = NaWindow::GetNaWindow(isolate, obj);
	if (pWindow)
	{
		pWindow->m_hWnd = (HWND)value->Int32Value();
	}
}

// description: move window to x,y, width, height
// syntax:      windowObj.move(x, y, width, height)
void NaWindow::Move(V8_FUNCTION_ARGS)
{
	// get window object(this)
	v8::Isolate *isolate = args.GetIsolate();
	v8::Local<v8::Object> obj = args.This();
	HWND hWnd = GetHandle(isolate, obj);

	if (args.Length() < 2)
	{
		// TODO error? exception?
		return;
	}

	if (hWnd == NULL)
		return;

	// move window
	int x, y, w, h;
	x = args[0]->IntegerValue();
	y = args[1]->IntegerValue();
	if (args.Length() < 3)
	{
		RECT rc;
		::GetWindowRect(hWnd, &rc);
		w = rc.right - rc.left;
		h = rc.bottom - rc.top;
	}
	else
	{
		w = args[2]->IntegerValue();
		h = args[3]->IntegerValue();
	}
	::MoveWindow(hWnd, x, y, w, h, TRUE);
}

// description: activate window
// syntax:      windowObj.activate()
void NaWindow::Activate(V8_FUNCTION_ARGS)
{
	// get window object(this)
	v8::Isolate *isolate = args.GetIsolate();
	v8::Local<v8::Object> obj = args.This();
	HWND hWnd = GetHandle(isolate, obj);

	if (hWnd == NULL)
		return;

	::SetForegroundWindow(hWnd);
}
