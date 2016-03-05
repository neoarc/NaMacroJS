#pragma once

#include "Common.h"

#include <Windows.h>
#include "v8.h"

#include <list>

#define NA_WINDOW_CLASS		L"NaWindowClass"

class NaWindow
{
public:
	NaWindow();
	~NaWindow();

	HWND Create();

	// Member
	HWND m_hWnd;
	
	// internal struct
	struct FindWindowsInfo {
		wchar_t *name;
		std::list<HWND> foundlist;
	};

	// static
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static bool s_bRegisterClass;

	static v8::Local<v8::Object> CreateV8WindowObject(v8::Isolate *isolate);
	static void FindWindows(v8::Isolate *isolate, const wchar_t *name, v8::Local<v8::Array> &results);
	static v8::Local<v8::Object> ConvertHWNDToV8WindowObject(v8::Isolate *isolate, HWND hWnd);
};

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);

void WindowMove(V8_FUNCTION_ARGS);