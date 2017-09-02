#include "stdafx.h"
#include "NaWindow.h"

#include <NaLib/NaDebug.h>
#include <NaLib/NaNotifyWindow.h>

#include "resource.h"
#include "NaControl.h"

bool NaWindow::s_bRegisterClass = false;

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

	//NaDebug::Out(L"NaWindow(): 0x%08x, %d\n", this, enType);
}

NaWindow::~NaWindow()
{
	//NaDebug::Out(L"~NaWindow(): 0x%08x\n", this);
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

RECT NaWindow::GetRect()
{
	RECT rc = { 0, 0, 0, 0 };
	if (m_hWnd)
		GetWindowRect(m_hWnd, &rc);

	return rc;
}

RECT NaWindow::GetClientRect()
{
	RECT rc = { 0, 0, 0, 0 };
	if (m_hWnd)
		::GetClientRect(m_hWnd, &rc);

	return rc;
}

void NaWindow::Move(int x, int y, int w, int h, bool bRepaint)
{
	::MoveWindow(m_hWnd, x, y, w, h, bRepaint ? 1: 0);
}

NaString NaWindow::GetClass()
{
	wchar_t wszBuf[1024];

	::RealGetWindowClass(m_hWnd, wszBuf, 1024);
	NaString str(wszBuf);
	
	return str;
}

NaString NaWindow::GetText()
{
	wchar_t wszBuf[1024];

	::GetWindowText(m_hWnd, wszBuf, 1024);
	NaString str(wszBuf);

	return str;
}

void NaWindow::SetText(const wchar_t * wszText)
{
	::SetWindowText(m_hWnd, wszText);
}

bool NaWindow::IsVisible()
{
	return ::IsWindowVisible(m_hWnd) == TRUE;
}

void NaWindow::SetVisible(bool bVisible)
{
	switch (m_enType)
	{
	case NA_WINDOW_CONSOLE:
		if (m_hWnd)
			::ShowWindow(m_hWnd, bVisible ? SW_SHOW : SW_HIDE);
		else if (bVisible)
		{
			HWND hConsole = GetConsoleWindow();
			if (hConsole == NULL)
			{
				AllocConsole();
				freopen("CONOUT$", "wt", stdout);

				hConsole = GetConsoleWindow();

				// Bind application main icon to console window
				HINSTANCE hInstance = GetModuleHandle(nullptr);
				HICON hIcon = (HICON)LoadImage(
					hInstance,
					MAKEINTRESOURCE(IDI_MAIN_ICON),
					IMAGE_ICON,
					GetSystemMetrics(SM_CXSMICON),
					GetSystemMetrics(SM_CYSMICON),
					LR_DEFAULTCOLOR);

				::SendMessage(hConsole, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
				::SendMessage(hConsole, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

				// Console output problem fix (temp)
				_wsetlocale(0, L"korean"); // LC_ALL

				MoveConsoleWindowToDefaultPosition(hConsole);

				// bind handle
				m_hWnd = hConsole;
			}
		}
		break;
	default:
		if (m_hWnd)
			::ShowWindow(m_hWnd, bVisible ? SW_SHOW : SW_HIDE);
		break;
	}
}

bool NaWindow::IsTopmost()
{
	DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	if ((dwExStyle & WS_EX_TOPMOST) != 0)
		return true;
	return false;
}

void NaWindow::SetTopmost(bool bTopmost)
{
	::SetWindowPos(
		m_hWnd,
		bTopmost ? HWND_TOPMOST : HWND_NOTOPMOST,
		0, 0, 0, 0,
		SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOREPOSITION | SWP_NOSIZE
	);
}

void NaWindow::SetHandle(HWND hWnd)
{
	m_hWnd = hWnd;
}

int NaWindow::GetState()
{
	WINDOWPLACEMENT wndpl;
	GetWindowPlacement(m_hWnd, &wndpl);

	return wndpl.showCmd;
}

void NaWindow::SetState(int nState)
{
	ShowWindow(m_hWnd, nState);
}

void NaWindow::Activate()
{
	::SetForegroundWindow(m_hWnd);
}

void NaWindow::Close()
{
	::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
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
	NaDebug::Out(L"EnumWindowsProc: %s / %s\n", buf, buf2);

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

void NaWindow::FindWindows(const wchar_t *name, FindWindowsInfo &info)
{
	// NOTE
	// ::FindWindow can find only exactly same named window can found.
	if (info.name == 0)
		info.name = const_cast<wchar_t*>(name);

	EnumWindows(EnumWindowsProc, (LPARAM)(&info));
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

CRect NaWindow::GetWorkArea()
{
	CRect rcWorkArea;
	rcWorkArea.left = 0;
	rcWorkArea.top = 0;
	rcWorkArea.right = ::GetSystemMetrics(SM_CXSCREEN);
	rcWorkArea.bottom = ::GetSystemMetrics(SM_CYSCREEN);

	if (NaTaskBarWindow::IsAutoHide())
		return rcWorkArea;

	CRect rcResult;
	CRect rcTaskbar = NaTaskBarWindow::GetRect();
	rcResult.SubtractRect(rcWorkArea, rcTaskbar);

	return rcResult;
}

void NaWindow::MoveConsoleWindowToDefaultPosition(const HWND hConsole)
{
	// #TODO verify hConsole is ConsoleWindow

	CRect rcConsole;
	GetWindowRect(hConsole, &rcConsole);

	// Default position is right-bottom
	CRect rcWorkArea = GetWorkArea();

	const int x = rcWorkArea.right - rcConsole.Width();
	const int y = rcWorkArea.bottom - rcConsole.Height();

	::MoveWindow(hConsole, x, y, rcConsole.Width(), rcConsole.Height(), TRUE);
}
bool NaTaskBarWindow::IsAutoHide()
{
	APPBARDATA abd = { 0 };
	abd.cbSize = sizeof(abd);

	const UINT state = (UINT)::SHAppBarMessage(ABM_GETSTATE, &abd);
	return (state == ABS_AUTOHIDE);
}

CRect NaTaskBarWindow::GetRect()
{
	CRect r;
	::GetWindowRect(::FindWindow(L"Shell_TrayWnd", nullptr), &r);

	return r;
}
