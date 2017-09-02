#pragma once

#include <Windows.h>

class NaDesktop
{
public:
	NaDesktop();
	~NaDesktop();

	static HWND GetHWND() {	return ::GetDesktopWindow(); }
	static HDC GetDC() { return ::GetWindowDC(::GetDesktopWindow()); }
};

