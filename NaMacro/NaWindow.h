#pragma once

#include <Windows.h>

#define NA_WINDOW_CLASS		L"NaWindowClass"

class NaWindow
{
public:
	NaWindow();
	~NaWindow();

	HWND Create();

	// Member
	HWND m_hWnd;

	// static
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static bool s_bRegisterClass;
};

