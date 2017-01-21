#pragma once

#include <Windows.h>

#include "NaString.h"

#define NA_NOTIFYWINDOW_CLASS		L"NaNotifyWindowClass"
#define NA_NOTIFYWINDOW_MARK		0x20170121

class NaNotifyWindow
{
public:
	NaNotifyWindow();
	~NaNotifyWindow();

	void Create(HWND hParent, NaString strMessage, NaString strTitle);
	NaString m_strMessage;
	NaString m_strTitle;

	// static
	static bool s_bRegisterClass;
	static HWND s_hMasterWindow;
	static POINT s_ptLastNotifyWindow;
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static void AddNotifyWindow(NaString strMessage, NaString strTitle);
};

