#pragma once

#include "V8Wrap.h"
#include "JsObjectBase.h"

#include <Windows.h>

#include <list>
#include <map>

#include <atltypes.h> // CRect (will be removed)

#define NA_WINDOW_CLASS		L"NaWindowClass"
#define NA_WINDOW_MARK		0x20160426

enum NaWindowTypes
{
	NA_WINDOW_UNKNOWN,
	NA_WINDOW_NORMAL,
	NA_WINDOW_CONSOLE,
	NA_WINDOW_USERCREATED,
	NA_WINDOW_INTERNAL,
};

class NaWindow
{
public:
	NaWindow(HWND hWnd = NULL, NaWindowTypes enType = NA_WINDOW_UNKNOWN);
	virtual ~NaWindow();

	HWND Create();
	void Destroy();

	RECT GetRect();
	RECT GetClientRect();
	void Move(int x, int y, int w, int h, bool bRepaint = false);
	NaString GetClass();
	NaString GetText();
	void SetText(const wchar_t* wszText);
	bool IsVisible();
	void SetVisible(bool bVisible);
	bool IsTopmost();
	void SetTopmost(bool bTopmost);
	void SetHandle(HWND hWnd);
	int GetState();
	void SetState(int nState);
	void Activate();
	void Close();

	// Member
	NaWindowTypes m_enType;
	HWND m_hWnd;
	int m_x, m_y, m_width, m_height, m_clientWidth, m_clientHeight;
	std::list<HWND> m_arControls;

	// internal struct
	struct FindWindowsInfo {
		wchar_t *name;
		std::list<HWND> foundlist;
	};

	// static
	static bool s_bRegisterClass;
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static void FindWindows(Isolate *isolate, const wchar_t *name, Local<Array> &results);
	static NaWindow* GetWindow(int x, int y);
	static NaWindow* GetActiveWindow();
	static NaWindow* GetWindow(HWND hWnd);

	// utility function for console window
	
	// #TODO Remove CRect!!
	static CRect GetWorkArea();
	static void MoveConsoleWindowToDefaultPosition(const HWND hConsole);
};

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);

class NaTaskBarWindow
{
public:
	static bool IsAutoHide();
	static CRect GetRect();	
};