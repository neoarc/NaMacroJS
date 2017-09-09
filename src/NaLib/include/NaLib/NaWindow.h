#pragma once

#include "NaString.h"

#include <Windows.h>

#include <list>
#include <map>

#include <atltypes.h> // CRect (will be removed)

#define NA_WINDOW_CLASS		L"NaWindowClass"
#define NA_WINDOW_MARK		0x20160426

enum NaWindowTypes
{
	NA_WINDOW_UNKNOWN = 0,

	NA_WINDOW_NORMAL,
	NA_WINDOW_CONSOLE,
	NA_WINDOW_USERCREATED,
	NA_WINDOW_INTERNAL,

	NA_WINDOW_LAST,
};

class NaWindow
{
public:
	NaWindow(HWND hWnd = NULL, NaWindowTypes enType = NA_WINDOW_UNKNOWN);
	virtual ~NaWindow();

	HWND Create(WNDPROC Proc);
	void Destroy();

	// Coordinates
	RECT GetRect();
	RECT GetClientRect();
	void Move(int x, int y, int w, int h, bool bRepaint = false);
	
	NaString GetClass();

	virtual NaString GetText();
	virtual void SetText(const wchar_t* wszText);

	virtual bool IsVisible();
	virtual void SetVisible(bool bVisible);

	virtual bool IsTopmost();
	virtual void SetTopmost(bool bTopmost);

	virtual void SetHandle(HWND hWnd);

	virtual int GetState();
	virtual void SetState(int nState);

	// Methods
	virtual void Activate();
	virtual void Close();

	// Member
	HWND m_hWnd;
	int m_x, m_y, m_width, m_height;
	int m_clientWidth, m_clientHeight;
	int m_enType;
	std::list<HWND> m_arControls;

	// internal struct
	struct FindWindowsInfo {
		wchar_t *name;
		std::list<HWND> foundlist;
	};

	// static
	static bool s_bRegisterClass;
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static void FindWindows(const wchar_t *name, FindWindowsInfo &info);
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
