#pragma once

#include "Common.h"

#include <Windows.h>
#include "v8.h"

#include <list>
#include <map>

#define NA_WINDOW_CLASS		L"NaWindowClass"

enum NaWindowTypes
{
	NA_WINDOW_UNKNOWN,
	NA_WINDOW_NORMAL,
	NA_WINDOW_CONSOLE,
};

class NaWindow
{
public:
	NaWindow(long lUID, NaWindowTypes enType = NA_WINDOW_UNKNOWN);
	~NaWindow();

	HWND Create();

	// Member
	NaWindowTypes m_enType;
	HWND m_hWnd;

	// internal struct
	struct FindWindowsInfo {
		wchar_t *name;
		std::list<HWND> foundlist;
	};

	// management window list
	static long s_lUniqueID;
	static long CreateUniqueID();
	static std::map<long, NaWindow*> s_mapWindow;
	static void DestroyMap();

	// static
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static bool s_bRegisterClass;

	static v8::Local<v8::Object> CreateV8Window(v8::Isolate *isolate);
	static void FindWindows(v8::Isolate *isolate, const wchar_t *name, v8::Local<v8::Array> &results);
	static v8::Local<v8::Object> GetV8Window(v8::Isolate *isolate, int x, int y);

	// static - Convert between NaWindow/V8/HWND
	// HWND -> V8Window
	static v8::Local<v8::Object> GetV8Window(v8::Isolate *isolate, HWND hWnd);

	// V8Window -> NaWindow
	static NaWindow* GetNaWindow(v8::Isolate *isolate, v8::Local<v8::Object> &obj);

	// NaWindow -> V8Window
	static v8::Local<v8::Object> GetV8Window(v8::Isolate *isolate, NaWindow *pWindow);

	static HWND GetHandle(v8::Isolate *isolate, v8::Local<v8::Object> obj);
	static void SetHandle(v8::Isolate *isolate, v8::Local<v8::Object> obj, HWND hWnd);

	// accessors
	DEFINE_CLASS_ACCESSOR(GetX, SetX);
	DEFINE_CLASS_ACCESSOR(GetY, SetY);
	DEFINE_CLASS_ACCESSOR(GetWidth, SetWidth);
	DEFINE_CLASS_ACCESSOR(GetHeight, SetHeight);
	DEFINE_CLASS_ACCESSOR(GetText, SetText);
	DEFINE_CLASS_ACCESSOR(GetVisible, SetVisible);
	DEFINE_CLASS_ACCESSOR(GetHandle, SetHandle);

	// methods
	DEFINE_CLASS_METHOD(Move);
	DEFINE_CLASS_METHOD(Activate);
};

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);
