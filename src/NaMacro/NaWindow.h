#pragma once

#include "V8Wrap.h"
#include "NaObject.h"

#include <Windows.h>

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
	NA_WINDOW_INTERNAL,
};

class NaWindow : public NaObject
{
public:
	NaWindow(HWND hWnd = NULL, NaWindowTypes enType = NA_WINDOW_UNKNOWN);
	virtual ~NaWindow();

	HWND Create();
	void Destroy();

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

	// wrap object
	virtual Local<ObjectTemplate> MakeObjectTemplate(Isolate *isolate);
	virtual Global<ObjectTemplate>& GetObjectTemplate() { return s_NaWindowTemplate; };
	static Global<ObjectTemplate> s_NaWindowTemplate;

	// accessors
	DEFINE_CLASS_ACCESSOR(x);
	DEFINE_CLASS_ACCESSOR(y);
	DEFINE_CLASS_ACCESSOR(width);
	DEFINE_CLASS_ACCESSOR(height);
	DEFINE_CLASS_ACCESSOR_RO(clientWidth); 
	DEFINE_CLASS_ACCESSOR_RO(clientHeight); 
	DEFINE_CLASS_ACCESSOR_RO(class);
	DEFINE_CLASS_ACCESSOR(text);
	DEFINE_CLASS_ACCESSOR(visible);
	DEFINE_CLASS_ACCESSOR(topmost);
	DEFINE_CLASS_ACCESSOR(handle);
	DEFINE_CLASS_ACCESSOR(state);

	// methods
	DEFINE_CLASS_METHOD(constructor);
	DEFINE_CLASS_METHOD(create);
	DEFINE_CLASS_METHOD(move);
	DEFINE_CLASS_METHOD(activate);
	DEFINE_CLASS_METHOD(close);
	DEFINE_CLASS_METHOD(alert);
	DEFINE_CLASS_METHOD(addControl);
};

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);
