#pragma once

#include "Common.h"
#include "NaObject.h"

#include <Windows.h>
#include "v8.h"

#include <list>
#include <map>

#define NA_WINDOW_CLASS		L"NaWindowClass"
#define NA_WINDOW_MARK		0x20160426

enum NaWindowTypes
{
	NA_WINDOW_UNKNOWN,
	NA_WINDOW_NORMAL,
	NA_WINDOW_CONSOLE,
	NA_WINDOW_USERCREATED,
};

class NaWindow : public NaObject
{
public:
	NaWindow(HWND hWnd = NULL, NaWindowTypes enType = NA_WINDOW_UNKNOWN);
	virtual ~NaWindow();

	HWND Create();

	// Member
	NaWindowTypes m_enType;
	HWND m_hWnd;
	int m_x, m_y, m_width, m_height;

	// internal struct
	struct FindWindowsInfo {
		wchar_t *name;
		std::list<HWND> foundlist;
	};

	// static
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static bool s_bRegisterClass;

	static void FindWindows(Isolate *isolate, const wchar_t *name, Local<Array> &results);
	static NaWindow* GetWindow(int x, int y);
	static NaWindow* GetWindow(HWND hWnd);

	// wrap object
	virtual Local<ObjectTemplate> MakeObjectTemplate(Isolate *isolate);
	virtual Global<ObjectTemplate>& GetObjectTemplate() { return s_NaWindowTemplate; };
	static Global<ObjectTemplate> s_NaWindowTemplate;

	// accessors
	DEFINE_CLASS_ACCESSOR(GetX, SetX);
	DEFINE_CLASS_ACCESSOR(GetY, SetY);
	DEFINE_CLASS_ACCESSOR(GetWidth, SetWidth);
	DEFINE_CLASS_ACCESSOR(GetHeight, SetHeight);
	DEFINE_CLASS_ACCESSOR(GetText, SetText);
	DEFINE_CLASS_ACCESSOR(GetVisible, SetVisible);
	DEFINE_CLASS_ACCESSOR(GetHandle, SetHandle);

	// methods
	DEFINE_CLASS_METHOD(Constructor);
	DEFINE_CLASS_METHOD(Create);
	DEFINE_CLASS_METHOD(Move);
	DEFINE_CLASS_METHOD(Activate);
	DEFINE_CLASS_METHOD(Alert);
};

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);
